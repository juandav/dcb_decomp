#!/usr/bin/env python3
"""Pair every function of another version (jp, eu) with its us counterpart.

    tools/match_versions.py jp [eu ...]

It reads the built ELFs and their maps (make VERSION=<v> first, for us and
for each version asked for): the us functions with their names, sizes and
modules from build/us/, the other version's functions as splat found them
from build/<v>/. Each binary pairs with the us binary of the same name; a
binary us doesn't have (INTSEG, NISSEG) is compared with all of us. The
executable's PsyQ SDK pairs only with us's SDK and its game code only with
us's game code.

The signals, from the cheapest to the most expensive:

- exact: the same bytes once the relocated fields are masked (jal targets,
  lui immediates and the %lo halves that use them), unique on both sides;
- opcodes: the same instructions with registers and immediates masked;
- adjacency: modules are linked in the same order, so between two pairs
  the functions left pair in order too (an alignment by similarity); a pair
  is confident when its neighbour on one side is already paired with the
  us function's neighbour;
- callgraph: the calls of two paired functions, aligned, pair the callees
  they don't share yet; the SDK, which pairs almost whole, anchors many;
- strings: a string (or file name) referenced by one function on each
  side.

Similarity (the score) is the Levenshtein ratio of the two opcode
sequences. A pair is confident only when it is unique and well supported;
weaker ones are candidates and are never used as names.

Writes build/<v>/version_pairs.tsv (one line per function of the version:
its address, size, binary, the us function, score, method, confidence),
build/<v>/version_pairs_data.tsv (data named from a confident pair: a
referenced string or table with the same bytes as us's named one) and
build/<v>/version_pairs.txt, a summary: per us module the share of its
functions paired and their mean similarity, and the version's functions
with no us counterpart, by region.

With --seed it also writes the names of the confident pairs into
config/<v>/symbols.txt (the executable's) and config/<v>/symbols_<overlay>.txt
(each overlay's, which its splat config reads), in place of the lines it
wrote before. A name can make splat split a function it had joined to the
one before, so after seeding rebuild the version (make VERSION=<v>
regenerate all) and seed again, until the names stop changing.
"""

import argparse
import re
import struct
import sys
from bisect import bisect_right
from collections import Counter, defaultdict
from difflib import SequenceMatcher
from pathlib import Path

import Levenshtein
from elftools.elf.elffile import ELFFile

sys.path.insert(0, str(Path(__file__).resolve().parent))
from version import ROOT  # noqa: E402

# confidence thresholds (similarity of the opcode sequences)
ADJ_SIM = 0.6           # with a neighbour, caller or callee paired
ADJ_SIM_ALONE = 0.85    # ordered, but no neighbour paired yet
CALL_SIM = 0.6          # a callee lined up by an already paired caller
CALL_SIM_MULTI = 0.4    # ...by two callers or more
STR_SIM = 0.4           # shares a string no other function uses
LOW_SIM = 0.45          # ...or with three callers, callees or neighbours paired
CAND_SIM = 0.5          # the least a candidate needs
MOVED_SIM = 0.7         # ...when it is in another binary
MIN_EXACT = 6           # instructions for an exact pair to count on its own
MIN_OPCODES = 10        # ...and for an opcode-only one
# a gap between two pairs larger than this (functions × functions) is only
# searched by its ends
MAX_GAP_CELLS = 250_000

AUTO_NAME = re.compile(r"^(?:[A-Z]+_)?(?:func|D|jtbl|jlabel)_[0-9A-F]{8}$")


def settings(version: str) -> dict:
    """The NAME := VALUE settings of mk/version/<version>.mk."""
    mk = ROOT / "mk" / "version" / f"{version}.mk"
    return dict(re.findall(r"^(\w+)\s*:=\s*(.*?)\s*$", mk.read_text(), re.M))


def binaries(version: str) -> list:
    """(binary, elf, map) for the executable and each overlay of VERSION."""
    s = settings(version)
    build = ROOT / "build" / version
    out = [("main", build / f"{s['EXE_NAME']}.elf", build / f"{s['EXE_NAME']}.map")]
    for o in s["OVERLAYS"].split():
        out.append((o, build / f"{o.upper()}.elf", build / f"{o.upper()}.map"))
    return out


# ---------------------------------------------------------------------------
# reading a version

class Func:
    __slots__ = ("version", "binary", "name", "addr", "size", "module", "region",
                 "words", "ops", "mhash", "ohash", "calls", "refs", "index",
                 "callers")

    def __repr__(self):
        return f"{self.version}:{self.name}@{self.addr:08X}"

    @property
    def n(self):
        return len(self.words)


class Memory:
    """The allocated bytes of a version's ELFs: an overlay's own first, then
    the executable's."""

    def __init__(self):
        self.ranges = {}  # binary -> [(start, end, data)]

    def add(self, binary, elf, code):
        self.code = getattr(self, "code", {})
        self.code[binary] = [(a, b) for a, b, _ in code]
        r = []
        for sec in elf.iter_sections():
            if sec["sh_flags"] & 2 and sec["sh_type"] != "SHT_NOBITS" and sec["sh_size"]:
                r.append((sec["sh_addr"], sec["sh_addr"] + sec["sh_size"], sec.data(), sec.name))
        self.ranges[binary] = sorted(r)

    def find(self, binary, addr):
        """(data, offset, section, binary) of ADDR as BINARY sees it."""
        for b in (binary, "main") if binary != "main" else ("main",):
            for start, end, data, name in self.ranges.get(b, ()):
                if start <= addr < end:
                    return data, addr - start, name, b
        return None, 0, None, None

    def read(self, binary, addr, n):
        data, off, _, _ = self.find(binary, addr)
        return None if data is None else data[off:off + n]

    def string(self, binary, addr):
        """The text at ADDR when it looks like one: at least 4 bytes before
        the NUL, no control bytes but newlines and tabs."""
        data, off, _, b = self.find(binary, addr)
        if data is None or any(lo <= addr < hi for lo, hi in self.code.get(b, ())):
            return None
        end = data.find(b"\0", off, off + 512)
        if end < 0 or end - off < 4:
            return None
        s = data[off:end]
        if any(c < 0x20 and c not in (9, 10) for c in s):
            return None
        return s


# MIPS fields
def op_of(w):
    return w >> 26


def rs_of(w):
    return (w >> 21) & 31


def rt_of(w):
    return (w >> 16) & 31


def rd_of(w):
    return (w >> 11) & 31


def simm(w):
    v = w & 0xFFFF
    return v - 0x10000 if v & 0x8000 else v


LOADS_STORES = set(range(0x20, 0x2F + 1)) | {0x31, 0x32, 0x39, 0x3A}  # lb..swr, lwc/swc
LO_USERS = {0x09, 0x0D} | LOADS_STORES  # addiu, ori


def opcode_id(w):
    """The instruction without its registers and immediates."""
    op = op_of(w)
    if op == 0:
        return w & 0x3F
    if op == 1:
        return 0x40 + rt_of(w)
    if op in (0x10, 0x11, 0x12):
        rs = rs_of(w)
        return 0x100 + (op - 0x10) * 0x80 + (0x40 + (w & 0x3F) if rs >= 0x10 else rs)
    return 0x60 + op


def writes_gpr(w):
    """The register W writes, or None."""
    op = op_of(w)
    if op == 0:
        funct = w & 0x3F
        if funct in (0x08, 0x0C, 0x0D, 0x11, 0x13, 0x18, 0x19, 0x1A, 0x1B):
            return None  # jr, syscall, break, mthi, mtlo, mult, div
        return rd_of(w)
    if op == 3:
        return 31
    if 0x08 <= op <= 0x0F or 0x20 <= op <= 0x26:
        return rt_of(w)
    if op == 0x12 and rs_of(w) in (0, 2):  # mfc2, cfc2
        return rt_of(w)
    return None


def analyse(f, words):
    """Fill F's masked hash, opcode string, calls and data references."""
    hi = {}
    masked = []
    ops = []
    calls = []
    refs = []  # (instruction index, address)
    for i, w in enumerate(words):
        op = op_of(w)
        ops.append(chr(0x100 + opcode_id(w)))
        m = w
        if op in (2, 3):  # j, jal: the target is a relocation
            m = w & 0xFC000000
            if op == 3:
                calls.append((f.addr & 0xF0000000) | ((w & 0x03FFFFFF) << 2))
        elif op == 0x0F:  # lui: %hi
            m = w & 0xFFFF0000
        elif op in LO_USERS and rs_of(w) in hi:  # %lo, with the %hi above
            refs.append((i, (hi[rs_of(w)] + simm(w)) & 0xFFFFFFFF))
            m = w & 0xFFFF0000
        elif op in LO_USERS and rs_of(w) == 28:  # $gp-relative
            m = w & 0xFFFF0000
        masked.append(m)
        # what the register holds now
        r = writes_gpr(w)
        if r is not None:
            if op == 0x0F:
                hi[r] = (w & 0xFFFF) << 16
            else:
                hi.pop(r, None)
    f.mhash = hash(struct.pack(f"<{len(masked)}I", *masked))
    f.ops = "".join(ops)
    f.ohash = hash(f.ops)
    f.calls = calls
    f.refs = refs


def module_ranges(mapfile, version):
    """(start, end, module) of each object's .text in MAPFILE, the module
    named binary/path as the report does (main/system/task, kawseg/cpu/kaw_cpu;
    main/game for a version's asm segment)."""
    out = []
    for m in re.finditer(r"^ \.text\s+0x([0-9a-f]+)\s+0x([0-9a-f]+)\s+(\S+)$", mapfile.read_text(), re.M):
        start, size, obj = int(m[1], 16), int(m[2], 16), m[3]
        if not size:
            continue
        path = obj
        for prefix in (f"build/{version}/src/", f"build/{version}/asm/{version}/"):
            if path.startswith(prefix):
                path = path[len(prefix):]
        path = re.sub(r"\.(c|s)\.o$", "", path)
        out.append((start, start + size, path))
    return sorted(out)


def load(version):
    """The functions of every binary of VERSION, and its memory."""
    memory = Memory()
    funcs = []
    for binary, elf_path, map_path in binaries(version):
        if not elf_path.exists():
            sys.exit(f"{elf_path.relative_to(ROOT)} is missing: run make VERSION={version} first")
        mods = module_ranges(map_path, version)
        starts = [m[0] for m in mods]
        with open(elf_path, "rb") as fh:
            elf = ELFFile(fh)
            memory.add(binary, elf, mods)
            # the linker script puts every section in one output section
            # (.main, .kawseg): the code is where the map's .text are
            seen = set()
            syms = []
            for s in elf.get_section_by_name(".symtab").iter_symbols():
                if (s["st_info"]["type"] == "STT_FUNC" and s["st_shndx"] not in ("SHN_ABS", "SHN_UNDEF")
                        and "." not in s.name and s["st_value"] not in seen):
                    k = bisect_right(starts, s["st_value"]) - 1
                    if k >= 0 and s["st_value"] < mods[k][1]:
                        seen.add(s["st_value"])
                        syms.append((s["st_value"], s["st_size"], s.name, mods[k][2]))
            # hand-written assembly gives no sizes: up to the next function
            syms.sort()
            for i, (addr, size, name, module) in enumerate(syms):
                if not size:
                    k = bisect_right(starts, addr) - 1
                    end = min(syms[i + 1][0] if i + 1 < len(syms) else mods[k][1], mods[k][1])
                    syms[i] = (addr, end - addr, name, module)
            syms = [x for x in syms if x[1] >= 4]
        for addr, size, name, module in sorted(syms):
            f = Func()
            f.version, f.binary, f.name, f.addr, f.size = version, binary, name, addr, size
            f.module = module
            # the executable's SDK is a region of its own
            f.region = binary + ("/psyq" if module.split("/")[1:2] == ["psyq"] else "")
            f.words = list(struct.unpack(f"<{size // 4}I", memory.read(binary, addr, size // 4 * 4)))
            funcs.append(f)
    by_addr = {}
    for f in funcs:
        by_addr[(f.binary, f.addr)] = f
    for f in funcs:
        analyse(f, f.words)
        f.callers = []
    # resolve calls: an overlay's own functions first, then the executable's
    for f in funcs:
        resolved = []
        for t in f.calls:
            g = by_addr.get((f.binary, t)) or by_addr.get(("main", t))
            resolved.append(g)
            if g is not None:
                g.callers.append(f)
        # function pointers taken with lui/addiu count as calls too
        for _, a in f.refs:
            g = by_addr.get((f.binary, a)) or by_addr.get(("main", a))
            if g is not None:
                resolved.append(g)
                g.callers.append(f)
        f.calls = resolved
    # the order of each region
    regions = defaultdict(list)
    for f in funcs:
        regions[f.region].append(f)
    for r in regions.values():
        r.sort(key=lambda f: f.addr)
        for i, f in enumerate(r):
            f.index = i
    return funcs, regions, memory


# ---------------------------------------------------------------------------
# pairing

def call_key(f):
    """A callee as by_calls compares them: us functions (a paired callee
    stands for its us partner) by identity, others never equal."""
    return ("us", id(f)) if f is not None and f.version == "us" else ("other", id(f))


def similarity(a, b):
    return Levenshtein.ratio(a.ops, b.ops)


class Pairing:
    def __init__(self, us_funcs, us_regions, funcs, regions):
        self.us_regions = us_regions
        self.regions = regions
        self.pair = {}       # target func -> (us func, score, method)
        self.rev = {}        # us func -> target func
        self.cand = {}       # target func -> (us func, score, method)
        self.sim_cache = {}

    def scope(self, region):
        """The us regions a region of the version pairs with."""
        if region in self.us_regions:
            return [region]
        # a binary us doesn't have: all of us's game code
        return [r for r in self.us_regions if not r.endswith("/psyq")]

    def sim(self, t, u):
        key = (id(t), id(u))
        v = self.sim_cache.get(key)
        if v is None:
            # the ratio can't beat the sizes' ratio: skip the far ones
            a, b = len(t.ops), len(u.ops)
            if 2 * min(a, b) / (a + b) < 0.4:
                v = 0.0
            else:
                v = similarity(t, u)
            self.sim_cache[key] = v
        return v

    def add(self, t, u, score, method):
        assert t not in self.pair and u not in self.rev
        self.pair[t] = (u, score, method)
        self.rev[u] = t
        self.cand.pop(t, None)

    def add_candidate(self, t, u, score, method):
        if t in self.pair:
            return
        old = self.cand.get(t)
        if old is None or old[1] < score:
            self.cand[t] = (u, score, method)

    # -- unique hashes ------------------------------------------------------
    def by_hash(self, attr, method, min_n):
        added = 0
        for region, tfuncs in self.regions.items():
            us = [u for r in self.scope(region) for u in self.us_regions[r] if u not in self.rev]
            ug, tg = defaultdict(list), defaultdict(list)
            for u in us:
                ug[getattr(u, attr)].append(u)
            for t in tfuncs:
                if t not in self.pair:
                    tg[getattr(t, attr)].append(t)
            for h, ts in tg.items():
                uu = ug.get(h, [])
                if len(ts) == 1 and len(uu) == 1:
                    t, u = ts[0], uu[0]
                    score = 1.0 if attr == "mhash" else self.sim(t, u)
                    if t.n >= min_n:
                        self.add(t, u, score, method)
                        added += 1
                    else:
                        self.add_candidate(t, u, score, method)
        return added

    # -- order --------------------------------------------------------------
    def neighbours_agree(self, t, u):
        """How many of T's neighbours (before, after) are paired with U's."""
        tr, ur = self.regions[t.region], self.us_regions[u.region]
        n = 0
        for d in (-1, 1):
            ti, ui = t.index + d, u.index + d
            if 0 <= ti < len(tr) and 0 <= ui < len(ur):
                p = self.pair.get(tr[ti])
                if p and p[0] is ur[ui]:
                    n += 1
        return n

    def anchors(self, region, us_region):
        """The confident pairs of REGION with US_REGION that keep the order
        (the longest increasing run of us addresses)."""
        ps = sorted((t.index, u.index) for t, (u, _, _) in self.pair.items()
                    if t.region == region and u.region == us_region)
        # longest increasing subsequence on the us index
        prev, tails_i = [None] * len(ps), []
        for i, (_, ui) in enumerate(ps):
            k = bisect_right([ps[j][1] for j in tails_i], ui - 0.5)
            if k == len(tails_i):
                tails_i.append(i)
            else:
                tails_i[k] = i
            prev[i] = tails_i[k - 1] if k else None
        out = []
        i = tails_i[-1] if tails_i else None
        while i is not None:
            out.append(ps[i])
            i = prev[i]
        return out[::-1]

    def align_gap(self, tg, ug):
        """Order-keeping alignment of two runs of unpaired functions, by
        similarity: [(t, u, sim)]."""
        n, m = len(tg), len(ug)
        if not n or not m:
            return []
        if n * m > MAX_GAP_CELLS:
            # only the ends: the first and last few of each side
            k = 40
            return self.align_gap(tg[:k], ug[:k]) + self.align_gap(tg[-k:], ug[-k:]) if n > k and m > k else []
        score = [[0.0] * (m + 1) for _ in range(n + 1)]
        move = [[0] * (m + 1) for _ in range(n + 1)]
        for i in range(1, n + 1):
            t = tg[i - 1]
            row, prow = score[i], score[i - 1]
            for j in range(1, m + 1):
                best, mv = prow[j], 1
                if row[j - 1] > best:
                    best, mv = row[j - 1], 2
                s = self.sim(t, ug[j - 1])
                if s >= CAND_SIM and prow[j - 1] + s > best:
                    best, mv = prow[j - 1] + s, 3
                row[j] = best
                move[i][j] = mv
        out = []
        i, j = n, m
        while i and j:
            mv = move[i][j]
            if mv == 3:
                out.append((tg[i - 1], ug[j - 1], self.sim(tg[i - 1], ug[j - 1])))
                i, j = i - 1, j - 1
            elif mv == 1:
                i -= 1
            else:
                j -= 1
        return out[::-1]

    def by_order(self):
        added = 0
        proposals = []
        for region, tfuncs in self.regions.items():
            for us_region in self.scope(region):
                if us_region != region:
                    continue  # order means nothing across binaries
                ufuncs = self.us_regions[us_region]
                anchors = [(-1, -1)] + self.anchors(region, us_region) + [(len(tfuncs), len(ufuncs))]
                for (ta, ua), (tb, ub) in zip(anchors, anchors[1:]):
                    tg = [t for t in tfuncs[ta + 1:tb] if t not in self.pair]
                    ug = [u for u in ufuncs[ua + 1:ub] if u not in self.rev]
                    proposals += self.align_gap(tg, ug)
        for t, u, s in proposals:
            if t in self.pair or u in self.rev:
                continue
            small = t.n < MIN_EXACT or u.n < MIN_EXACT
            if (s >= ADJ_SIM_ALONE and not small and not self.calls_disagree(t, u)) or self.supported(t, u, s):
                self.add(t, u, s, "adjacency")
                added += 1
            else:
                self.add_candidate(t, u, s, "adjacency")
        return added

    # -- calls --------------------------------------------------------------
    def graph_agree(self, t, u):
        """How many of T's paired callees and callers are U's."""
        uc = set(id(c) for c in u.calls if c is not None)
        ur = set(id(c) for c in u.callers)
        n = sum(1 for c in set(t.calls) if c is not None and c in self.pair and id(self.pair[c][0]) in uc)
        n += sum(1 for c in set(t.callers) if c in self.pair and id(self.pair[c][0]) in ur)
        return n

    def supported(self, t, u, s):
        """Whether a pair of similarity S has enough around it: neighbours
        in the binary paired with each other, callees and callers paired
        with each other, and no paired callee that disagrees."""
        if self.calls_disagree(t, u):
            return False
        n = self.neighbours_agree(t, u) + self.graph_agree(t, u)
        small = t.n < MIN_EXACT or u.n < MIN_EXACT
        return (s >= ADJ_SIM and n >= (2 if small else 1)) or (s >= LOW_SIM and n >= 3)

    def by_graph(self):
        """Functions left that the paired callers and callees point to: the
        unpaired callees of a paired caller's us partner, the unpaired
        callers of a paired callee's."""
        votes = defaultdict(Counter)
        for region, tfuncs in self.regions.items():
            scope = set(self.scope(region))
            for t in tfuncs:
                if t in self.pair:
                    continue
                for c in t.callers:
                    if c in self.pair:
                        for u in self.pair[c][0].calls:
                            if u is not None and u not in self.rev and u.region in scope:
                                votes[t][u] += 1
                for c in set(t.calls):
                    if c is not None and c in self.pair:
                        for u in self.pair[c][0].callers:
                            if u not in self.rev and u.region in scope:
                                votes[t][u] += 1
        best = {}
        for t, us in votes.items():
            scored = sorted(((self.sim(t, u), u) for u in us), key=lambda x: -x[0])
            s, u = scored[0]
            # a clear best: no other as similar
            if len(scored) > 1 and scored[1][0] > s - 0.1:
                continue
            best[t] = (u, s)
        claimed = Counter(u for u, _ in best.values())
        added = 0
        for t, (u, s) in best.items():
            if claimed[u] > 1 or t in self.pair or u in self.rev:
                continue
            if self.supported(t, u, s):
                self.add(t, u, s, "callgraph")
                added += 1
            elif s >= CAND_SIM:
                self.add_candidate(t, u, s, "callgraph")
        return added

    def calls_disagree(self, t, u):
        """True when T calls paired functions that U doesn't call, or is
        called by paired functions whose partners don't call U."""
        tc = [c for c in set(t.calls) if c is not None and c in self.pair]
        if tc:
            uc = set(id(c) for c in u.calls if c is not None)
            if sum(1 for c in tc if id(self.pair[c][0]) in uc) < len(tc) / 2:
                return True
        tr = [c for c in set(t.callers) if c in self.pair]
        if tr:
            ur = set(id(c) for c in u.callers)
            if sum(1 for c in tr if id(self.pair[c][0]) in ur) < len(tr) / 2:
                return True
        return False

    def by_calls(self):
        """The calls of each pair, aligned: where they differ by as many
        calls on each side, the callees not paired yet pair in order."""
        support = defaultdict(int)
        for t, (u, _, _) in list(self.pair.items()):
            ts = [self.pair[c][0] if c in self.pair else c for c in t.calls]
            us = list(u.calls)
            sm = SequenceMatcher(None, [call_key(x) for x in ts], [call_key(x) for x in us], autojunk=False)
            for tag, i1, i2, j1, j2 in sm.get_opcodes():
                if tag == "replace" and i2 - i1 == j2 - j1:
                    for a, b in zip(t.calls[i1:i2], us[j1:j2]):
                        if a is not None and b is not None and a not in self.pair and b not in self.rev:
                            support[(a, b)] += 1
        # each side proposed with one partner only
        by_t, by_u = defaultdict(set), defaultdict(set)
        for (a, b) in support:
            by_t[a].add(b)
            by_u[b].add(a)
        added = 0
        for (a, b), n in sorted(support.items(), key=lambda kv: -kv[1]):
            if a in self.pair or b in self.rev:
                continue
            if b.region not in self.scope(a.region):
                continue
            s = self.sim(a, b)
            unique = len(by_t[a]) == 1 and len(by_u[b]) == 1
            if unique and (s >= CALL_SIM or (n >= 2 and s >= CALL_SIM_MULTI)) and not self.calls_disagree(a, b):
                self.add(a, b, s, "callgraph")
                added += 1
            elif s >= CAND_SIM:
                self.add_candidate(a, b, s, "callgraph")
        return added

    # -- strings ------------------------------------------------------------
    def by_strings(self, us_memory, memory):
        def index(funcs, mem, paired):
            out = defaultdict(set)
            for f in funcs:
                if f in paired:
                    continue
                for _, a in f.refs:
                    s = mem.string(f.binary, a)
                    if s and len(s) >= 6:
                        out[s].add(f)
            return out
        added = 0
        for region, tfuncs in self.regions.items():
            us = [u for r in self.scope(region) for u in self.us_regions[r]]
            ti = index(tfuncs, memory, self.pair)
            ui = index(us, us_memory, self.rev)
            votes = defaultdict(int)
            for s, ts in ti.items():
                uu = ui.get(s)
                if uu and len(ts) == 1 and len(uu) == 1:
                    votes[(next(iter(ts)), next(iter(uu)))] += 1
            by_t, by_u = defaultdict(set), defaultdict(set)
            for (a, b) in votes:
                by_t[a].add(b)
                by_u[b].add(a)
            for (a, b), n in votes.items():
                if a in self.pair or b in self.rev:
                    continue
                s = self.sim(a, b)
                if len(by_t[a]) == 1 and len(by_u[b]) == 1 and s >= STR_SIM and not self.calls_disagree(a, b):
                    self.add(a, b, s, "strings")
                    added += 1
                elif s >= CAND_SIM:
                    self.add_candidate(a, b, s, "strings")
        return added

    # -- code in another binary ----------------------------------------------
    def moved(self):
        """A function left that is in another binary of us (code moved
        between the executable and an overlay): a unique exact match of
        some size is confident, a mutual best of high similarity a
        candidate. Its name isn't seeded: an overlay's prefix says where it
        is."""
        others = defaultdict(list)
        for r, us in self.us_regions.items():
            if r.endswith("/psyq"):
                continue
            for u in us:
                if u not in self.rev:
                    others[u.mhash].append(u)
        tg = defaultdict(list)
        for region, tfuncs in self.regions.items():
            if region.endswith("/psyq") or region not in self.us_regions:
                continue
            for t in tfuncs:
                if t not in self.pair:
                    tg[t.mhash].append(t)
        for h, ts in tg.items():
            us = [u for u in others.get(h, []) if u.region not in self.scope(ts[0].region)]
            if len(ts) == 1 and len(us) == 1 and ts[0].n >= 2 * MIN_EXACT:
                self.add(ts[0], us[0], 1.0, "moved-exact")
        best_t, best_u = {}, {}
        for region, tfuncs in self.regions.items():
            if region.endswith("/psyq") or region not in self.us_regions:
                continue
            us = [u for r in self.us_regions if r not in self.scope(region) and not r.endswith("/psyq")
                  for u in self.us_regions[r] if u not in self.rev]
            for t in tfuncs:
                if t in self.pair or t.n < MIN_EXACT:
                    continue
                for u in us:
                    a, b = len(t.ops), len(u.ops)
                    if 2 * min(a, b) / (a + b) < MOVED_SIM:
                        continue
                    s = self.sim(t, u)
                    if s > best_t.get(t, (None, 0))[1]:
                        best_t[t] = (u, s)
                    if s > best_u.get(u, (None, 0))[1]:
                        best_u[u] = (t, s)
        for t, (u, s) in best_t.items():
            if s >= MOVED_SIM and best_u[u][0] is t:
                self.add_candidate(t, u, s, "moved")

    # -- the rest -----------------------------------------------------------
    def best_candidates(self):
        """For every function left: its most similar unpaired us function in
        scope, when each is the other's best."""
        for region, tfuncs in self.regions.items():
            left = [t for t in tfuncs if t not in self.pair]
            us = [u for r in self.scope(region) for u in self.us_regions[r] if u not in self.rev]
            if not left or not us:
                continue
            best_t, best_u = {}, {}
            us_sorted = sorted(us, key=lambda u: len(u.ops))
            lens = [len(u.ops) for u in us_sorted]
            for t in left:
                a = len(t.ops)
                # only sizes the ratio allows to reach CAND_SIM
                lo = a * CAND_SIM / (2 - CAND_SIM)
                hi = a * (2 - CAND_SIM) / CAND_SIM
                k = bisect_right(lens, lo - 1)
                for u in us_sorted[k:]:
                    if len(u.ops) > hi:
                        break
                    s = self.sim(t, u)
                    if s > best_t.get(t, (None, 0))[1]:
                        best_t[t] = (u, s)
                    if s > best_u.get(u, (None, 0))[1]:
                        best_u[u] = (t, s)
            for t, (u, s) in best_t.items():
                if s >= CAND_SIM and best_u.get(u, (None,))[0] is t:
                    self.add_candidate(t, u, s, "fuzzy")


def run_pairing(us, target):
    us_funcs, us_regions, us_mem = us
    funcs, regions, mem = target
    p = Pairing(us_funcs, us_regions, funcs, regions)
    p.by_hash("mhash", "exact", MIN_EXACT)
    p.by_hash("ohash", "opcodes", MIN_OPCODES)
    while True:
        n = p.by_order() + p.by_calls() + p.by_graph() + p.by_strings(us_mem, mem)
        n += p.by_hash("mhash", "exact", MIN_EXACT) + p.by_hash("ohash", "opcodes", MIN_OPCODES)
        if not n:
            break
    p.moved()
    p.best_candidates()
    return p


# ---------------------------------------------------------------------------
# data named from the confident pairs

def data_symbols(version):
    """The data symbols of VERSION's ELFs, each sized up to the next symbol:
    {(binary, addr): (name, size)}. GCC gives its data no size."""
    out = {}
    for binary, elf_path, map_path in binaries(version):
        code = [(a, b) for a, b, _ in module_ranges(map_path, version)]
        with open(elf_path, "rb") as fh:
            elf = ELFFile(fh)
            ends = [sec["sh_addr"] + sec["sh_size"] for sec in elf.iter_sections() if sec["sh_flags"] & 2]
            syms = {}
            for s in elf.get_section_by_name(".symtab").iter_symbols():
                if (s.name and s["st_shndx"] not in ("SHN_ABS", "SHN_UNDEF")
                        and s["st_info"]["type"] in ("STT_OBJECT", "STT_NOTYPE")
                        and not any(a <= s["st_value"] < b for a, b in code)):
                    syms.setdefault(s["st_value"], s.name)
        addrs = sorted(set(syms) | set(ends))
        for a, b in zip(addrs, addrs[1:]):
            if a in syms and "." not in syms[a]:
                out[(binary, a)] = (syms[a], b - a)
    return out


def pair_data(p, us_mem, mem, us_data, us_named):
    """Data the confident pairs reference at the same place, named in us,
    with the same bytes in both: [(binary, addr, name, us_addr, kind)].
    Only from pairs with as many references on each side, which line up."""
    votes = defaultdict(set)
    support = Counter()
    for t, (u, s, method) in p.pair.items():
        if t.binary != u.binary or len(t.refs) != len(u.refs):
            continue
        for (_, ta), (_, ua) in zip(t.refs, u.refs):
            owner = u.binary if (u.binary, ua) in us_data else "main"
            sym = us_data.get((owner, ua))
            if sym is None or sym[0] not in us_named or AUTO_NAME.match(sym[0]):
                continue
            name, size = sym
            ub = us_mem.read(u.binary, ua, size)
            tb = mem.read(t.binary, ta, size)
            # the same bytes, in the same binary
            if not ub or ub != tb or not any(ub) or mem.find(t.binary, ta)[3] != owner:
                continue
            kind = "string" if us_mem.string(u.binary, ua) else "table"
            votes[(owner, name, ua, kind)].add(ta)
            support[(owner, name, ta)] += 1
    # a name for one address, an address for one name; a scalar's few bytes
    # are the same by chance too often, so it needs two references
    by_addr = defaultdict(set)
    for (owner, name, ua, kind), tas in votes.items():
        size = us_data[(owner, ua)][1]
        ta = next(iter(tas))
        if len(tas) == 1 and (kind == "string" or size >= 8 or support[(owner, name, ta)] >= 2):
            by_addr[(owner, next(iter(tas)))].add((name, ua, kind))
    out = []
    for (owner, ta), names in by_addr.items():
        if len(names) == 1:
            name, ua, kind = next(iter(names))
            out.append((owner, ta, name, ua, kind))
    return sorted(out)


# ---------------------------------------------------------------------------
# output

def us_symbol_lines():
    """The lines of us's symbol files by name: {name: (file stem, attributes)}."""
    out = {}
    for path in sorted((ROOT / "config" / "us").glob("symbols*.txt")):
        for line in path.read_text().splitlines():
            m = re.match(r"^\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;\s*(?://\s*(.*))?$", line)
            if m:
                out.setdefault(m[1], (path.stem, m[3] or ""))
    return out


def write_outputs(version, p, target, us, data_pairs):
    funcs, regions, mem = target
    us_funcs = us[0]
    build = ROOT / "build" / version
    lines = ["\t".join(["addr", "size", "binary", "name", "us_name", "us_addr", "us_module",
                        "score", "method", "confidence"])]
    for f in funcs:
        if f in p.pair:
            u, s, m = p.pair[f]
            conf = "confident"
        elif f in p.cand:
            u, s, m = p.cand[f]
            conf = "candidate"
        else:
            u, s, m, conf = None, 0.0, "", "unpaired"
        lines.append("\t".join([f"0x{f.addr:08X}", f"0x{f.size:X}", f.binary, f.name,
                                u.name if u else "", f"0x{u.addr:08X}" if u else "",
                                u.module if u else "", f"{s:.3f}", m, conf]))
    (build / "version_pairs.tsv").write_text("\n".join(lines) + "\n")
    dl = ["\t".join(["binary", "addr", "us_name", "us_addr", "kind"])]
    dl += ["\t".join([b, f"0x{a:08X}", n, f"0x{ua:08X}", k]) for b, a, n, ua, k in data_pairs]
    (build / "version_pairs_data.tsv").write_text("\n".join(dl) + "\n")

    # the summary
    out = []
    w = out.append
    cats = Counter()
    for f in funcs:
        c = "confident" if f in p.pair else "candidate" if f in p.cand else "unpaired"
        cats[(f.region.endswith("/psyq"), c, "n")] += 1
        cats[(f.region.endswith("/psyq"), c, "b")] += f.size
    w(f"# {version} against us\n")
    w("## functions\n")
    w(f"{'':10} {'game':>18} {'psyq':>18}")
    for c in ("confident", "candidate", "unpaired"):
        w(f"{c:10} {cats[(False, c, 'n')]:5} {cats[(False, c, 'b')]:8} B   "
          f"{cats[(True, c, 'n')]:5} {cats[(True, c, 'b')]:8} B")
    methods = Counter(m for _, _, m in p.pair.values())
    w("\nconfident by method: " + ", ".join(f"{k} {v}" for k, v in methods.most_common()))
    w("\n## us modules: share of functions paired (confident, +candidates) and mean similarity\n")
    w(f"{'module':44} {'funcs':>5} {'conf':>5} {'cand':>5} {'exact':>5} {'paired%':>7} {'sim':>5}")
    by_mod = defaultdict(list)
    for u in us_funcs:
        by_mod[u.module].append(u)
    cand_rev = {u: (t, s) for t, (u, s, _) in p.cand.items()}
    rows = []
    for mod, us_list in by_mod.items():
        conf = [u for u in us_list if u in p.rev]
        cand = [u for u in us_list if u not in p.rev and u in cand_rev]
        exact = [u for u in conf if p.pair[p.rev[u]][2] == "exact"]
        sims = [p.pair[p.rev[u]][1] for u in conf] + [cand_rev[u][1] for u in cand]
        rows.append((mod, len(us_list), len(conf), len(cand), len(exact),
                     100 * len(conf) / len(us_list), sum(sims) / len(us_list)))
    for r in sorted(rows, key=lambda r: (-r[6], r[0])):
        w(f"{r[0]:44} {r[1]:5} {r[2]:5} {r[3]:5} {r[4]:5} {r[5]:6.1f}% {r[6]:5.2f}")
    w("\n(sim: the mean over the module's functions of the pair's similarity, 0 when unpaired)")
    w(f"\n## {version} functions with no confident us counterpart, by region\n")
    regs = defaultdict(lambda: [0, 0, 0, 0, Counter()])
    for f in funcs:
        if f in p.pair:
            continue
        r = regs[f.region]
        r[0] += 1
        r[1] += f.size
        if f in p.cand:
            r[2] += 1
            r[4][p.cand[f][0].binary] += 1
        else:
            r[3] += f.size
    w(f"{'region':16} {'funcs':>5} {'bytes':>8} {'cand':>5} {'unpaired B':>10}  candidates' us binaries")
    for reg, (n, b, c, ub, bins) in sorted(regs.items()):
        w(f"{reg:16} {n:5} {b:8} {c:5} {ub:10}  " + ", ".join(f"{k} {v}" for k, v in bins.most_common()))
    w(f"\n## pairs across binaries (code that moved): {version} region -> us module\n")
    moved = Counter()
    for t in funcs:
        q = p.pair.get(t) or p.cand.get(t)
        if q and q[0].binary != t.binary and q[1] >= MOVED_SIM:
            moved[(t.region, q[0].module, "confident" if t in p.pair else "candidate")] += 1
    for (reg, mod, c), n in sorted(moved.items()):
        w(f"{reg:16} {mod:44} {c:10} {n:4}")
    w(f"\n## data named from confident pairs: {len(data_pairs)}")
    (build / "version_pairs.txt").write_text("\n".join(out) + "\n")
    return "\n".join(out)


# ---------------------------------------------------------------------------
# seeding the names

BEGIN = "// names from us (tools/match_versions.py --seed): the same function or datum"
END = "// end of the names from us"


def symbol_file(version, binary):
    cfg = ROOT / "config" / version
    return cfg / ("symbols.txt" if binary == "main" else f"symbols_{binary}.txt")


def seed(version, p, data_pairs):
    """Write the names into the version's symbol files, between BEGIN and
    END (replacing what an earlier run wrote there). A name given by hand
    stays: outside the block, or inside it with `manual` in its comment (a
    pair corrected by hand), which is kept in the block as it is."""
    us_lines = us_symbol_lines()
    bins = [b for b, _, _ in binaries(version)]
    # the names the files already give by hand (outside the block, or marked
    # manual inside it)
    manual = {}
    named_at = set()  # (binary, address) of the names given by hand
    kept = defaultdict(list)  # binary -> [(addr, line)] of the manual lines in the block
    for b in bins:
        path = symbol_file(version, b)
        text = path.read_text() if path.exists() else ""
        if BEGIN in text:
            block = text[text.index(BEGIN):text.index(END)]
            for line in block.splitlines():
                m = re.match(r"^\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+).*//.*\bmanual\b", line)
                if m:
                    kept[b].append((int(m[2], 16), line))
            text = text[:text.index(BEGIN)] + "\n".join(line for _, line in kept[b]) + text[text.index(END) + len(END):]
        for m in re.finditer(r"^\s*(\S+)\s*=\s*(0x[0-9A-Fa-f]+)", text, re.M):
            manual[m[1]] = (b, int(m[2], 16))
            named_at.add((b, int(m[2], 16)))
    # every name splat gives now, with its address: a name we write must not
    # be one of them somewhere else
    taken = defaultdict(dict)
    for binary, elf_path, _ in binaries(version):
        with open(elf_path, "rb") as fh:
            for s in ELFFile(fh).get_section_by_name(".symtab").iter_symbols():
                if s.name and s["st_shndx"] not in ("SHN_ABS", "SHN_UNDEF"):
                    taken[binary].setdefault(s.name, s["st_value"])

    skipped = []

    def free(name, binary, addr):
        if name in manual:
            if manual[name] != (binary, addr):
                skipped.append(name)
            return False
        # an address named by hand keeps that name (a pairing corrected by
        # hand: the version's own function where us has another)
        if (binary, addr) in named_at:
            return False
        for b in {binary, "main"}:
            if taken[b].get(name, addr) != addr:
                return False
        return True

    entries = defaultdict(list)  # binary -> [(addr, line)]
    used = set()
    for t, (u, s, method) in sorted(p.pair.items(), key=lambda kv: kv[0].addr):
        # only a function of the same binary keeps its name: an overlay's
        # prefix says where it is
        if t.binary != u.binary or u.name not in us_lines or AUTO_NAME.match(u.name):
            continue
        if not free(u.name, t.binary, t.addr):
            continue
        extra = f" size:0x{t.size:X}" if "size:" in us_lines[u.name][1] else ""
        entries[t.binary].append((t.addr, f"{u.name} = 0x{t.addr:08X}; // type:func{extra}"))
        used.add(u.name)
    for owner, addr, name, ua, kind in data_pairs:
        if name in used or name not in us_lines or not free(name, owner, addr):
            continue
        entries[owner].append((addr, f"{name} = 0x{addr:08X};"))
        used.add(name)
    written = {}
    for binary in bins:
        path = symbol_file(version, binary)
        lst = sorted(entries.get(binary, []) + kept[binary])
        old = path.read_text() if path.exists() else ""
        if BEGIN in old:
            old = old[:old.index(BEGIN)].rstrip("\n") + "\n" + old[old.index(END) + len(END):].lstrip("\n")
        if not lst:
            if old.strip() and old != path.read_text():
                path.write_text(old)
            continue
        if not old.strip():
            old = f"// {binary.upper()}'s own symbols, which only its splat config reads\n"
        block = "\n".join([BEGIN] + [line for _, line in lst] + [END])
        path.write_text(old.rstrip("\n") + "\n\n" + block + "\n")
        written[path.name] = len(lst)
        # an overlay's splat config reads its own file too
        if binary != "main":
            yaml = ROOT / "config" / version / f"{binary}.yaml"
            text = yaml.read_text()
            line = f"    - config/{version}/symbols_{binary}.txt"
            if line not in text:
                text = text.replace(f"    - config/{version}/symbols.txt\n",
                                    f"    - config/{version}/symbols.txt\n    # and its own\n{line}\n", 1)
                yaml.write_text(text)
    if skipped:
        print(f"names left out, already given to another address: {' '.join(skipped)}", file=sys.stderr)
    return written


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("versions", nargs="+", help="the versions to pair with us (jp, eu)")
    ap.add_argument("--seed", action="store_true",
                    help="write the confident pairs' names into config/<version>/symbols*.txt")
    args = ap.parse_args()
    us = load("us")
    us_data = data_symbols("us")
    us_named = set(us_symbol_lines())
    for v in args.versions:
        if v == "us":
            sys.exit("us is what the others pair with")
        target = load(v)
        p = run_pairing(us, target)
        data_pairs = pair_data(p, us[2], target[2], us_data, us_named)
        print(write_outputs(v, p, target, us, data_pairs))
        if args.seed:
            for name, n in seed(v, p, data_pairs).items():
                print(f"config/{v}/{name}: {n} names")


if __name__ == "__main__":
    main()
