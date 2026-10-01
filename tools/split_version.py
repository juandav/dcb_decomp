#!/usr/bin/env python3
"""Split another version's binaries (jp, eu) into us's modules.

    tools/split_version.py <version> [binary ...]

A version that is still splat's assembly has each binary as a few big asm
ranges (its rodata, its code, its data). This rewrites the subsegments of
config/<version>/<binary>.yaml (all the binaries us has, or those given) so
that they follow us's modules: one asm segment per us module, with us's
name, and the rodata and data segments each module owns. Each module is
then a unit of the report under the same name as in us, and it becomes C
by turning its segments into c, .rodata and .data and listing the file in
mk/version/<version>.mk.

Where each module starts in the version:

- code: at its first function. The functions are those of
  build/<version>/version_pairs.tsv (tools/match_versions.py <version>); a
  function the version has and us doesn't goes with the one before it. The
  modules follow the version's order, which can differ from us's;
- rodata and data: each data segment of us is placed where the references
  agree: a function paired with a us one reads, at about the same place in
  it, the same datum, so the version's address of each datum the pair reads
  votes for where the segment starts. The votes that keep the shift of the
  segment before win. A segment no paired function reads goes where its
  bytes are most alike, near where us has it.

A segment the version's config already has (by name: the psyq ranges, or
modules split before) keeps its start, so running it again changes
nothing, and a module that is C in the version's config stays C.
Hand-written assembly (us's hasm segments) is asm here, and two segments
of one name (libmath's hasm and C) become one. A data-only module (an
overlay's <prefix>_bss.c) has no code segment while it is asm: splat
writes nothing for an asm segment of no size; when it becomes C, add its
`c` segment where the code ends, as in us. The bss segments and the rest of
the config are kept.

It needs us and the version built (make VERSION=<v>: splat's asm in
asm/<v>/ and the ELFs) and the pairs, and it prints, per binary, what it
placed by bytes and the sizes that differ from us's. Then make
VERSION=<version> regenerate and compare. A boundary in the wrong place
still builds the same binary while the module is asm, so check a module's
boundaries when it becomes C.
"""

import argparse
import re
import sys
from collections import Counter, defaultdict
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent

LINE = re.compile(r"^(\s*)- \[(0x[0-9A-Fa-f]+), ([.\w]+)(?:, ([\w/]+))?\]\s*$")
INSN = re.compile(r"^\s*/\* ([0-9A-F]+) ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(.*)$")
LO = re.compile(r"%lo\((\w+)(?: \+ (0x[0-9A-Fa-f]+))?\)")
AUTO = re.compile(r"_([0-9A-F]{8})$")

# how far apart (in instructions) two reads of a pair of functions can be
# and still be the same read
NEAR = 16

CODE = {"c", "asm", "hasm"}
RODATA = {"rodata", ".rodata"}
DATA = {"data", ".data"}


def settings(version: str) -> dict:
    mk = (ROOT / "mk" / "version" / f"{version}.mk").read_text()
    return dict(re.findall(r"^(\w+)\s*:=\s*(.*?)\s*$", mk, re.M))


def elf_path(version: str, binary: str) -> Path:
    name = settings(version)["EXE_NAME"] if binary == "main" else binary.upper()
    return ROOT / "build" / version / f"{name}.elf"


def elf_symbols(path: Path) -> dict:
    with open(path, "rb") as f:
        symtab = ELFFile(f).get_section_by_name(".symtab")
        return {s.name: s["st_value"] for s in symtab.iter_symbols() if s.name}


class Binary:
    """A binary's segment layout in one version: (offset, type, name) and
    the offset of its code segment's vram."""

    def __init__(self, version: str, binary: str):
        self.path = ROOT / "config" / version / f"{binary}.yaml"
        config = yaml.safe_load(self.path.read_text())
        self.target = config["options"]["target_path"]
        code = next(s for s in config["segments"] if isinstance(s, dict) and s.get("type") == "code")
        self.start = code["start"]
        self.vram = code["vram"]
        self.subsegments = []
        for s in code["subsegments"]:
            if isinstance(s, list) and len(s) >= 3:
                self.subsegments.append((s[0], s[1], s[2]))
        # the end of the code segment: the next top-level segment
        segments = config["segments"]
        i = segments.index(code)
        nxt = segments[i + 1]
        self.end = nxt[0] if isinstance(nxt, list) else nxt["start"]

    def contents(self) -> bytes:
        """The binary's bytes, by the offsets of its config."""
        return (ROOT / self.target).read_bytes()

    def addr(self, offset: int) -> int:
        return self.vram + offset - self.start

    def offset(self, addr: int) -> int:
        return addr - self.vram + self.start

    def ranges(self, kinds: set) -> list:
        """(start, end, type, name) of the subsegments of KINDS, the end
        being the start of the next subsegment of any kind (or the end of
        the section)."""
        subs = self.subsegments
        out = []
        for i, (start, kind, name) in enumerate(subs):
            if kind not in kinds:
                continue
            end = subs[i + 1][0] if i + 1 < len(subs) else self.end
            out.append((start, end, kind, name))
        return out


def functions_refs(version: str, binary: str) -> dict:
    """{function address: (size, [(instruction, data address)])}: the data
    addresses the %lo's of every function in splat's asm of BINARY
    (asm/<version>/<binary>/) read, with the index of the instruction."""
    names = elf_symbols(elf_path(version, binary))
    if binary != "main":
        for k, v in elf_symbols(elf_path(version, "main")).items():
            names.setdefault(k, v)

    def resolve(name):
        m = AUTO.search(name)
        return int(m.group(1), 16) if m else names.get(name)

    refs = {}
    base = ROOT / "asm" / version / binary
    for path in base.rglob("*.s"):
        rel = path.relative_to(base).parts
        if rel[0] in ("data", "nonmatchings", "matchings"):
            continue
        current = None
        for line in path.read_text().splitlines():
            if line.startswith("glabel "):
                current = None
                continue
            m = INSN.match(line)
            if not m:
                continue
            if current is None:
                current = int(m.group(2), 16)
                refs[current] = []
                count = 0
            for name, plus in LO.findall(m.group(3)):
                a = resolve(name)
                if a is not None:
                    refs[current].append((count, a + (int(plus, 16) if plus else 0)))
            count += 1
    # (number of instructions, [(instruction, address)])
    return {f: (max([i for i, _ in r], default=0) + 1, r) for f, r in refs.items()}


def read_pairs(version: str) -> list:
    path = ROOT / "build" / version / "version_pairs.tsv"
    if not path.exists():
        sys.exit(f"{path.relative_to(ROOT)} is missing: run tools/match_versions.py {version}")
    rows = []
    lines = path.read_text().splitlines()
    head = lines[0].split("\t")
    for line in lines[1:]:
        rows.append(dict(zip(head, line.split("\t"))))
    return rows


def split(version: str, binary: str, pairs: list) -> tuple:
    """The version's new subsegments of BINARY, [(offset, type, name)], and
    the lines of the report."""
    us = Binary("us", binary)
    ver = Binary(version, binary)
    report = []

    # the segments the version has already (its library ranges, or a
    # module split before) and the modules that are C already
    current = {(kind, name): start for start, kind, name in ver.subsegments}
    c_modules = {name for start, kind, name in ver.subsegments if kind == "c"}

    # --- code: each function goes with its pair's module, or with the one
    # before it
    funcs = sorted((int(r["addr"], 16), r) for r in pairs if r["binary"] == binary)
    module_of = {}
    last = None
    for addr, r in funcs:
        if r["confidence"] in ("confident", "candidate") and r["us_module"].startswith(binary + "/"):
            last = r["us_module"][len(binary) + 1:]
        module_of[addr] = last
    us_code = us.ranges(CODE)
    us_names = []
    for _, _, _, name in us_code:
        if name not in us_names:
            us_names.append(name)
    # the runs of functions of one module; a module in several runs (a
    # function that pairs with another module's) keeps its longest one, and
    # the others stay with the module before them
    runs = []
    for addr, _ in funcs:
        name = module_of[addr]
        if name is None:
            continue
        if not runs or runs[-1][1] != name:
            runs.append([addr, name, 0])
        runs[-1][2] += 1
    longest = {}
    for addr, name, n in runs:
        if name in us_names and (name not in longest or n > longest[name][1]):
            longest[name] = (addr, n)
    for addr, name, n in runs:
        if name in longest and longest[name][0] != addr:
            report.append(f"  {name}: {n} function(s) at {addr:#010x} apart from the rest, "
                          f"left in the module before")
    code_start = {name: ver.offset(addr) for name, (addr, n) in longest.items()}
    # a segment the version already has keeps its start
    for name in us_names:
        for k in CODE:
            if (k, name) in current:
                code_start[name] = current[(k, name)]
    # the code starts with the first module, whatever comes before its first
    # paired function
    code_lo = min((s for s, k, _ in ver.subsegments if k in CODE), default=None)
    if code_start and code_lo is not None:
        first = min(code_start, key=lambda n: code_start[n])
        code_start[first] = min(code_start[first], code_lo)
    # a us code segment of no size (a data-only C file such as <prefix>_bss.c)
    # goes at the end of the code, as in us
    empty = [n for s, e, k, n in us_code if e == s and n not in code_start]
    for n in us_names:
        if n not in code_start and n not in empty:
            report.append(f"  {n}: no function of it in {version}, its code is left in the module before")
    # the version's order of the modules
    order = sorted(code_start, key=lambda n: code_start[n]) + empty
    rank = {n: i for i, n in enumerate(order)}
    # whether the version links the modules in another order than us
    reordered = [n for n in order if n in us_names] != [n for n in us_names if n in order]

    # --- rodata and data: votes from the paired functions' references
    us_refs = functions_refs("us", binary)
    ver_refs = functions_refs(version, binary)
    paired = []
    for addr, r in funcs:
        if r["confidence"] == "confident" and r["us_addr"]:
            u = int(r["us_addr"], 16)
            if u in us_refs and addr in ver_refs:
                paired.append((us_refs[u], ver_refs[addr]))

    def section(kinds: set) -> tuple:
        """The range of the version's subsegments of KINDS (its section)."""
        subs = ver.subsegments
        idx = [i for i, (_, k, _) in enumerate(subs) if k in kinds]
        if not idx:
            return None, None
        end = subs[idx[-1] + 1][0] if idx[-1] + 1 < len(subs) else ver.end
        return subs[idx[0]][0], end

    us_bytes, ver_bytes = us.contents(), ver.contents()

    def similarity(us_off: int, size: int, off: int) -> int:
        """How many of the us bytes at US_OFF are the same in the version at
        OFF; words that are addresses on both sides count as the same."""
        score = 0
        for k in range(0, size, 4):
            u = us_bytes[us_off + k:us_off + k + 4]
            v = ver_bytes[off + k:off + k + 4]
            if len(u) < 4 or len(v) < 4:
                break
            if u == v or (u[3] == 0x80 and v[3] == 0x80):
                score += 4
            else:
                score += sum(x == y for x, y in zip(u, v))
        return score

    def place(kinds: set) -> dict:
        # us's segments in the version's order of the modules (a module with
        # no code stays after the one before it in us), each at the offset
        # it would have if us were in that order
        segs = us.ranges(kinds)
        if not segs:
            return {}
        ordered = segs
        if reordered:
            keys = []
            prev = -1
            for s0, e0, k0, n0 in segs:
                prev = rank[n0] if n0 in rank and n0 not in empty else prev + 0.001
                keys.append(prev)
            ordered = [seg for _, seg in sorted(zip(keys, segs), key=lambda x: x[0])]
            if ordered != segs:
                report.append(f"  the {sorted(kinds)[-1]} follows the version's order of the modules")
        segs = []
        offset = ordered[0][0]
        for s0, e0, k0, n0 in ordered:
            segs.append((offset, offset + e0 - s0, k0, n0, s0))
            offset += e0 - s0

        lo_sec, hi_sec = section(kinds)
        at = [None] * len(segs)
        # the library ranges the version already has keep their start, and
        # the first segment starts where the version's section does
        for i, (start, end, kind, name, real) in enumerate(segs):
            fixed = next((current[(k, name)] for k in kinds if (k, name) in current), None)
            if fixed is not None:
                at[i] = fixed
        if at[0] is None:
            at[0] = lo_sec

        def bounds(i):
            floor = max((a for a in at[:i] if a is not None), default=lo_sec)
            ceiling = min((a for a in at[i + 1:] if a is not None), default=hi_sec)
            return floor, ceiling

        def shift_before(i):
            j = max(k for k in range(i) if at[k] is not None)
            return at[j] - segs[j][0]

        # where the references agree
        for i, (start, end, kind, name, real) in enumerate(segs):
            if at[i] is not None:
                continue
            lo, hi = us.addr(real), us.addr(real + end - start)
            # a read votes for the reads of the paired function at about the
            # same place in it
            votes = Counter()
            for (un, ur), (vn, vr) in paired:
                for k, a in ur:
                    if lo <= a < hi:
                        for kk, b in vr:
                            if abs(kk - k * vn / un) <= NEAR:
                                votes[ver.offset(b - (a - lo))] += 1
            floor, ceiling = bounds(i)
            # the data before it moved by some amount: a start that keeps
            # about the same shift is likelier than one far off (a read of
            # some other datum nearby)
            shift = shift_before(i)
            ranked = sorted(((n / (1 + abs(o - start - shift) / 0x100), o)
                             for o, n in votes.items()
                             if floor < o <= ceiling and o % 4 == 0), reverse=True)
            if ranked and (len(ranked) == 1 or ranked[0][0] > ranked[1][0]):
                at[i] = ranked[0][1]
        # the rest: where its bytes are most alike, near where us has it
        for i, (start, end, kind, name, real) in enumerate(segs):
            if at[i] is not None:
                continue
            floor, ceiling = bounds(i)
            guess = start + shift_before(i)
            best = None
            for o in range(floor + 4, ceiling + 1, 4):
                key = (similarity(real, end - start, o), -abs(o - guess))
                if best is None or key > best[0]:
                    best = (key, o)
            at[i] = best[1] if best else min(max(guess, floor), ceiling)
            report.append(f"  {name} ({kind.lstrip('.')}): no references to place it, "
                          f"put by its bytes at {at[i]:#x}")
        return {seg[3]: a for seg, a in zip(segs, at)}

    rodata_start = place(RODATA)
    data_start = place(DATA)

    # --- the new subsegments, each section in the version's order
    def kind_for(us_kind, name):
        is_c = name in c_modules
        if us_kind in CODE:
            return "c" if is_c else "asm"
        if us_kind in RODATA:
            return ".rodata" if is_c else "rodata"
        return ".data" if is_c else "data"

    rodata = sorted((a, kind_for(".rodata", n), n) for n, a in rodata_start.items())
    code = sorted((code_start[n], kind_for("c", n), n) for n in order if n in code_start)
    data = sorted((a, kind_for(".data", n), n) for n, a in data_start.items())
    text_end = data[0][0] if data else ver.end
    # (splat writes nothing for an asm segment of no size: a data-only
    # module that is still asm only has its data segments)
    code += [(text_end, "c", n) for n in empty if n in c_modules]
    new = rodata + code + data

    # the sizes that differ from us's
    us_size = defaultdict(int)
    for group in (CODE, RODATA, DATA):
        for s0, e0, k0, n0 in us.ranges(group):
            us_size[(min(group), n0)] += e0 - s0
    for i, (a, k, n) in enumerate(new):
        end = new[i + 1][0] if i + 1 < len(new) else ver.end
        group = next(min(g) for g in (CODE, RODATA, DATA) if k in g)
        u = us_size.get((group, n))
        if u is not None and u != end - a:
            report.append(f"    size {n} ({k}): {end - a:#x} ({end - a - u:+#x} from us)")

    # check the order
    for (a, ka, na), (b, kb, nb) in zip(new, new[1:]):
        if b < a:
            report.append(f"  {nb} ({kb}) at {b:#x} is before {na} ({ka}) at {a:#x}")
    return new, report


def rewrite(version: str, binary: str, new: list) -> None:
    path = ROOT / "config" / version / f"{binary}.yaml"
    lines = path.read_text().splitlines()
    out = []
    i = 0
    in_subs = False
    indent = None
    written = False
    while i < len(lines):
        line = lines[i]
        if re.match(r"^\s*subsegments:\s*$", line):
            in_subs = True
            out.append(line)
            i += 1
            continue
        if in_subs:
            m = LINE.match(line)
            stripped = line.strip()
            if m and m.group(3) in CODE | RODATA | DATA:
                indent = m.group(1)
                if not written:
                    out.append(f"{indent}# us's modules, each with its rodata and data "
                               f"(tools/split_version.py)")
                    out += [f"{indent}- [{a:#X}, {k}, {n}]".replace("0X", "0x") for a, k, n in new]
                    written = True
                i += 1
                continue
            if stripped.startswith("#") and not written:
                # the comments about the old asm ranges
                i += 1
                continue
            if stripped == "" or not line.startswith(" " * 6):
                in_subs = False
        out.append(line)
        i += 1
    path.write_text("\n".join(out) + "\n")


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("version")
    parser.add_argument("binaries", nargs="*")
    parser.add_argument("-n", "--dry-run", action="store_true", help="print the subsegments, don't write them")
    args = parser.parse_args()
    if args.version == "us":
        sys.exit("us is the reference: split another version")
    pairs = read_pairs(args.version)
    binaries = args.binaries or sorted(
        p.stem for p in (ROOT / "config" / args.version).glob("*.yaml")
        if (ROOT / "config" / "us" / p.name).exists())
    for binary in binaries:
        new, report = split(args.version, binary, pairs)
        print(f"{binary}: {len(new)} subsegments")
        for line in report:
            print(line)
        if args.dry_run:
            for a, k, n in new:
                print(f"    - [{a:#x}, {k}, {n}]")
        else:
            rewrite(args.version, binary, new)


if __name__ == "__main__":
    main()
