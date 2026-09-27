#!/usr/bin/env python3
"""Reproduce some habits of the ASPSX that assembled the GCC 2.7.2 PsyQ objects.

A load from `symbol($reg)` needs a temporary for the upper half of the
address: maspsx uses $at, ASPSX used the destination register (unless that
is the index register).

GCC 2.7.2 leaves jumps (`j $31`, `j label`) and some calls in reorder mode
and maspsx follows them with a nop. The ASPSX used for those objects moved
the previous instruction into the delay slot instead, unless that would put a load of
$31 right before the jump. A `la` before the jump is split, and its low half
goes into the slot.

A load delay nop that maspsx emits after a label belongs before it.

The mfhi/mflo that ends an expanded div/rem has no load delay: ASPSX used its
result in the next instruction (_spu_FsetRXXa), maspsx puts a nop there.

usage: aspsx_reorder.py < maspsx_output.s > output.s
"""

import re
import sys

BRANCHES = re.compile(
    r"(j|jal|jr|jalr|b|bal|beq|bne|blez|bgtz|bltz|bgez|beqz|bnez|"
    r"bltzal|bgezal|beql|bnel|blezl|bgtzl|bltzl|bgezl)$"
)
LOADS = re.compile(r"(lw|lh|lhu|lb|lbu|lwl|lwr)$")
STORES = re.compile(r"(sw|sh|sb|swl|swr)$")
# conditional branches GCC can leave in reorder mode
CONDBR = ("beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez")
# Instructions the assembler expands into several machine instructions
MACROS = re.compile(r"(la|li|div|divu|rem|remu|mul|ulw|usw|ulh|ulhu)$")


def split(line):
    """Return (mnemonic, operands) for an instruction line, else None."""
    code = line.split("#", 1)[0].strip()
    if not code or code.endswith(":") or code.startswith("."):
        return None
    parts = code.split(None, 1)
    ops = [o.strip() for o in parts[1].split(",")] if len(parts) > 1 else []
    return parts[0], ops


def loads_without_at(lines):
    """lui $at / addu $at,$at,$r / lw $d,%lo(x)($at)  ->  use $d instead.

    maspsx expands a large constant offset (`lw $d,0x1F801088($r)`) with
    `addu $at,$r,$at`; ASPSX wrote it as for a symbol."""
    out = list(lines)
    for i in range(len(out) - 2):
        a, b, c = (split(x) for x in out[i : i + 3])
        if not (a and b and c) or a[0] != "lui" or a[1][:1] != ["$at"]:
            continue
        if b[0] != "addu" or b[1][0] != "$at" or "$at" not in b[1][1:]:
            continue
        if not LOADS.match(c[0]) or not c[1][1].endswith("($at)"):
            continue
        index = b[1][2] if b[1][1] == "$at" else b[1][1]
        out[i + 1] = f"addu\t$at,$at,{index}"
        dest = c[1][0]
        if dest in ("$at", index):
            continue
        out[i] = out[i].replace("$at", dest)
        out[i + 1] = out[i + 1].replace("$at", dest)
        out[i + 2] = out[i + 2].replace("($at)", f"({dest})")
    return out


def delay_slot_hazards(lines):
    """lw $x,symbol / jump / access through $x  ->  a nop after the load."""
    code = [i for i, x in enumerate(lines) if split(x)]
    insert = []
    for a, b, c in zip(code, code[1:], code[2:]):
        la, lb, lc = split(lines[a]), split(lines[b]), split(lines[c])
        if not LOADS.match(la[0]) or not BRANCHES.match(lb[0]) or len(la[1]) != 2:
            continue
        reg = la[1][0]
        if re.search(r"\(\$\w+\)$", la[1][1]):
            # a load through a register: only a store of the loaded value
            # in the slot of a call (func_80056C18), and not for a
            # symbol($reg) load, which ASPSX expanded through $at
            # (GsSwapDispBuff)
            if (lb[0] == "jal" and STORES.match(lc[0]) and lc[1][0] == reg
                    and re.match(r"-?(0x)?[0-9a-fA-F]*\(", la[1][1])):
                insert.append(a)
            continue
        # not for calls: `lw $x,symbol / jal / sw ..($x)` stays as it is
        if lb[0] in ("jal", "jalr"):
            continue
        # only when the slot uses it as the base of a memory access
        if (LOADS.match(lc[0]) or STORES.match(lc[0])) and lc[1][-1].endswith(
            f"({reg})"
        ):
            insert.append(a)
    for a in reversed(insert):
        lines.insert(a + 1, "nop  # load delay before the delay slot")
    return lines


def nops_before_labels(lines):
    """maspsx puts a load delay nop after a label; ASPSX kept it before."""
    out = list(lines)
    for i in range(1, len(out)):
        if out[i].startswith("nop") and "DEBUG: Reuse of" in out[i]:
            j = i - 1
            if out[j].rstrip().endswith(":") and not out[j].startswith("."):
                out[i], out[j] = out[j], out[i]
    return out


def no_nop_after_div(lines):
    """Drop the load delay nop maspsx puts after an expanded div's mfhi/mflo."""
    out = []
    for line in lines:
        if (
            line.startswith("nop")
            and "DEBUG: Reuse of" in line
            and out
            and out[-1].strip() in ("# EXPAND_DIV END", "# EXPAND_DIVU END")
        ):
            continue
        out.append(line)
    return out


# --- delay slots of conditional branches filled from the branch target ---
# Our 2.7.2 reorg (mark_target_live_regs) keeps a register that died in a
# branch "live" at its fallthrough until the next label, so it never takes
# `li $2,K` from the target of `slti $2,..; beqz $2,T` in a compare tree.
# The cc1 of the PsyQ libraries did. Empirically (the whole build is the
# test) it did so for branches predicted not taken (beq/beqz/blez/bltz) in
# functions that make calls; leaf functions and bne-type branches keep the
# nop. When the previous branch's slot already executed the same insn,
# reorg only redirects the branch past it.
ALU = re.compile(
    r"(addu|addiu|subu|and|andi|or|ori|xor|xori|nor|sll|srl|sra|sllv|srlv|srav|"
    r"slt|slti|sltu|sltiu|lui|move|li)$"
)
CALLER_SAVED = {"$1", "$2", "$3", "$4", "$5", "$6", "$7", "$8", "$9", "$10", "$11",
                "$12", "$13", "$14", "$15", "$24", "$25", "$at", "$v0", "$v1"}
REGNAMES = {"$zero": "$0", "$at": "$1", "$v0": "$2", "$v1": "$3", "$a0": "$4", "$a1": "$5",
            "$a2": "$6", "$a3": "$7", "$sp": "$29", "$fp": "$30", "$ra": "$31", "$gp": "$28"}


def reg(r):
    return REGNAMES.get(r, r)


def regs_in(op):
    return [reg(x) for x in re.findall(r"\$\w+", op)]


def one_word_li(ops):
    try:
        v = int(ops[1], 0)
    except ValueError:
        return False
    return -0x8000 <= v <= 0xFFFF or (v & 0xFFFF) == 0


def uses_defs(ins):
    """(uses, defs) register sets of an instruction, None if unknown."""
    m, ops = ins
    if m in ("nop",):
        return set(), set()
    if m in ("j",):
        return set(), set()
    if m in ("jr",):
        return set(regs_in(ops[0])), set()
    if m in ("jal", "jalr"):
        return {"$4", "$5", "$6", "$7"} | set(sum((regs_in(o) for o in ops), [])), {"$31"}
    if m in ("beq", "bne"):
        return set(regs_in(ops[0]) + regs_in(ops[1])), set()
    if m in ("beqz", "bnez", "blez", "bgtz", "bltz", "bgez"):
        return set(regs_in(ops[0])), set()
    if m in ("sw", "sh", "sb", "swl", "swr"):
        return set(sum((regs_in(o) for o in ops), [])), set()
    if m in ("lw", "lh", "lhu", "lb", "lbu", "lwl", "lwr", "la", "li", "lui"):
        d = reg(ops[0])
        u = set(regs_in(ops[1])) if len(ops) > 1 else set()
        if m in ("lwl", "lwr"):
            u.add(d)
        return u, {d}
    if m in ("mult", "multu", "div", "divu"):
        return set(sum((regs_in(o) for o in ops), [])), {"hi", "lo"}
    if m in ("mflo", "mfhi"):
        return {"lo" if m == "mflo" else "hi"}, {reg(ops[0])}
    if ALU.match(m) or m in ("neg", "negu", "not", "subu", "addu"):
        return set(sum((regs_in(o) for o in ops[1:]), [])), {reg(ops[0])}
    return None


LABEL = re.compile(r"^([$\w.]+):\s*(#.*)?$")


def parse(lines, split):
    """list of (kind, index, value): ('label', i, name) / ('ins', i, (m, ops))"""
    items = []
    for i, l in enumerate(lines):
        s = l.split("#", 1)[0].strip()
        if not s:
            continue
        m = LABEL.match(s)
        if m:
            items.append(("label", i, m.group(1)))
            continue
        ins = split(l)
        if ins:
            items.append(("ins", i, ins))
    return items


def is_dead(items, pos, r, labels, depth=0):
    """True when register r is written before being read from items[pos] on
    every path (2.8 reorg's find_dead_or_set_registers, roughly)."""
    if depth > 10:
        return False
    k = pos
    while k < len(items):
        kind, _, v = items[k]
        if kind == "label":
            k += 1
            continue
        m, ops = v
        ud = uses_defs(v)
        if ud is None:
            return False
        uses, defs = ud
        cond = m in ("beq", "bne", "beqz", "bnez", "blez", "bgtz", "bltz", "bgez")
        if m in ("j", "jal", "jalr", "jr") or cond:
            if r in uses:
                return False
            # delay slot
            if k + 1 >= len(items) or items[k + 1][0] != "ins":
                return False
            sud = uses_defs(items[k + 1][2])
            if sud is None:
                return False
            if r in sud[0]:
                return False
            if r in sud[1]:
                return True
            if m == "jr":
                return r in CALLER_SAVED and r not in ("$2", "$3")
            if m in ("jal", "jalr"):
                if r in CALLER_SAVED:
                    return True
                k += 2
                continue
            tgt = ops[-1]
            if tgt not in labels:
                return False
            if m == "j":
                k = labels[tgt]
                depth += 1
                if depth > 10:
                    return False
                continue
            return is_dead(items, labels[tgt], r, labels, depth + 1) and is_dead(
                items, k + 2, r, labels, depth + 1
            )
        if r in uses:
            return False
        if r in defs:
            return True
        k += 1
    return False


def fill_from_target(lines, split):
    """Fill `bcond; nop` from the first instruction at the branch target when
    the register it sets is dead on the fallthrough path (what the PsyQ cc1's
    reorg did and our 2.7.2 reorg misses)."""
    items = parse(lines, split)
    labels = {v: n for n, (kind, _, v) in enumerate(items) if kind == "label"}
    refs = {}
    for kind, _, v in items:
        if kind == "ins":
            for o in v[1]:
                refs[o] = refs.get(o, 0) + 1
    edits = []
    # functions that make calls (leaf functions are left alone, e.g. libmcrd
    # func_8003D0EC and libspu _spu_FgetRXXa keep their nops)
    fn_of = []
    name = None
    calls = {}
    for i, l in enumerate(lines):
        t = l.strip()
        if t.startswith(".ent"):
            name = t.split()[1]
            calls[name] = False
        elif t.startswith(".end") and not t.startswith(".endr"):
            name = None
        fn_of.append(name)
        ins = split(l)
        if name and ins and ins[0] in ("jal", "jalr"):
            calls[name] = True
    for n, (kind, i, v) in enumerate(items):
        # only branches predicted not taken (reorg tried the target second)
        if kind != "ins" or v[0] not in ("beq", "beqz", "blez", "bltz"):
            continue
        if not calls.get(fn_of[i], False):
            continue
        if n + 1 >= len(items) or items[n + 1][0] != "ins" or items[n + 1][2][0] != "nop":
            continue
        if "branch/jump" not in lines[items[n + 1][1]]:
            continue
        tgt = v[1][-1]
        if tgt not in labels:
            continue
        t = labels[tgt] + 1
        while t < len(items) and items[t][0] == "label":
            t += 1
        if t >= len(items):
            continue
        tm, tops = items[t][2]
        if not ALU.match(tm) or (tm == "li" and not one_word_li(tops)):
            continue
        # only constants (`li`, `lui`, `addiu/ori $r,$zero,K`); VSync keeps the
        # nop in front of a `move $v0,$s1` target
        if not (tm in ("li", "lui") or (tm in ("addiu", "ori") and reg(tops[1]) == "$0")):
            continue
        ud = uses_defs(items[t][2])
        if ud is None or len(ud[1]) != 1:
            continue
        r = next(iter(ud[1]))
        # $at: the first half of an expanded macro, which reorg never saw
        if r in ud[0] or r in ("$0", "$1", "$at", "$29", "$31"):
            continue
        if not is_dead(items, n + 2, r, labels):
            continue
        # own the target thread: only this branch jumps there and nothing
        # falls into it (the insn before the label is a jump's delay slot)
        prev = labels[tgt] - 1
        own = (
            refs.get(tgt, 0) == 1
            and prev >= 1
            and items[prev][0] == "ins"
            and items[prev - 1][0] == "ins"
            and items[prev - 1][2][0] in ("j", "jr")
        )
        edits.append((n, t, own, tgt))
    return items, edits


def apply_fill(lines, split):
    items, edits = fill_from_target(lines, split)
    out = list(lines)
    after = {}
    drop = set()
    prev = None
    for n, t, own, tgt in edits:
        bi, si, ti = items[n][1], items[n + 1][1], items[t][1]
        insn = lines[ti].split("#", 1)[0].rstrip()
        if prev and prev[0] == n - 2 and prev[1] == t:
            # the previous branch's slot already did it on this path:
            # reorg just skips the now redundant insn
            out[bi] = re.sub(re.escape(tgt) + r"\s*$", prev[2], out[bi].split("#", 1)[0].rstrip())
            prev = (n, t, prev[2])
            continue
        out[si] = insn + "  # from the branch target"
        if ti in drop:
            # stolen by an earlier branch: tgt already points past it
            prev = (n, t, tgt)
        elif own and t not in after:
            drop.add(ti)
            prev = (n, t, tgt)
        else:
            name = f"{tgt}_dt{len(after) + 1}"
            after.setdefault(t, []).append(name)
            out[bi] = re.sub(re.escape(tgt) + r"\s*$", name, out[bi].split("#", 1)[0].rstrip())
            prev = (n, t, name)
    for t, names in after.items():
        ti = items[t][1]
        if ti not in drop:
            out[ti] = out[ti] + "".join(f"\n{x}:" for x in names)
    return [l for k, l in enumerate(out) if k not in drop]


def main():
    lines = delay_slot_hazards(
        loads_without_at(
            nops_before_labels(
                no_nop_after_div(apply_fill(sys.stdin.read().split("\n"), split))
            )
        )
    )
    out = []
    # index in out of the slot this pass filled after a call
    call_slot = -1
    i = 0
    while i < len(lines):
        line = lines[i]
        ins = split(line)
        nxt = lines[i + 1] if i + 1 < len(lines) else ""
        if (
            ins
            and (ins[0] == "j" or ins[0] == "jal" or ins[0] in CONDBR)
            and nxt.strip().startswith("nop")
            and "branch/jump" in nxt
        ):
            # previous instruction line
            k = len(out) - 1
            while k >= 0 and not out[k].split("#", 1)[0].strip():
                k -= 1
            prev = split(out[k]) if k >= 0 else None
            prev_la = prev
            prev2 = None
            after_call_slot = False
            at_label = False
            if prev:
                m = k - 1
                while m >= 0 and not out[m].split("#", 1)[0].strip():
                    m -= 1
                prev2 = split(out[m]) if m >= 0 else None
                # a branch target stays where it is
                at_label = m >= 0 and out[m].split("#", 1)[0].strip().endswith(":")
                # nor does the instruction after a call whose slot GCC
                # filled (CdRead); after one ASPSX filled itself it moves
                n = m - 1
                while n >= 0 and not out[n].split("#", 1)[0].strip():
                    n -= 1
                prev3 = split(out[n]) if n >= 0 else None
                after_call_slot = (
                    prev3 is not None
                    and prev3[0] in ("jal", "jalr")
                    and m != call_slot
                    and "branch/jump" not in out[m]
                )
                # ...but the stack adjustment before `j $31` does move into
                # its slot, unless the label follows a branch's delay slot
                if at_label and ins[1] == ["$31"]:
                    code = [x for x in out[:m] if split(x)]
                    at_label = len(code) < 2 or bool(BRANCHES.match(split(code[-2])[0]))
                # (a split la or store to a symbol leaves its lui there)
                if at_label and prev[0] != "la" and not (
                    STORES.match(prev[0])
                    and len(prev[1]) == 2
                    and not re.search(r"\(\$\w+\)$", prev[1][1])
                ):
                    prev = None
            sym_store = (
                prev is not None
                and STORES.match(prev[0]) is not None
                and len(prev[1]) == 2
                and not re.search(r"\(\$\w+\)$", prev[1][1])
            )
            idx_store = (
                prev is not None
                and STORES.match(prev[0]) is not None
                and len(prev[1]) == 2
                and re.match(r"^[A-Za-z_][\w.]*([+-]\d+)?\((\$\w+)\)$", prev[1][1])
            )
            reg_store = (
                prev is not None
                and STORES.match(prev[0]) is not None
                and len(prev[1]) == 2
                and re.match(r"^-?(0x)?[0-9a-fA-F]*\(\$\w+\)$", prev[1][1])
            )
            if ins[0] in CONDBR:
                # a conditional branch left in reorder mode only takes a store
                # (a register-based one; GCC's reorg doesn't move volatile
                # stores), a store to a symbol, or the low half of a `la`
                idx_store = False
                if (reg_store and not at_label and not after_call_slot
                        and not (prev2 and BRANCHES.match(prev2[0]))):
                    moved = out.pop(k)
                    out.append(line)
                    out.append(moved)
                    i += 2
                    continue
                if not sym_store and not (prev_la and prev_la[0] == "la"):
                    prev = prev_la = None
            movable = (
                prev is not None
                and ins[0] not in CONDBR
                and not at_label
                and not sym_store
                and not idx_store
                and not BRANCHES.match(prev[0])
                and not MACROS.match(prev[0])
                and prev[0] != "nop"
                and not (prev[1] and prev[1][0] == "$31")
                and not LOADS.match(prev[0])
                and not (prev2 and LOADS.match(prev2[0]) and prev2[1][:1] == ["$31"])
                and not (prev2 and BRANCHES.match(prev2[0]))
                and not after_call_slot
                # nor into the slot of a call through a register (func_800669AC)
                and not (ins[0] == "jal" and ins[1][-1].startswith("$"))
            )
            if movable:
                moved = out.pop(k)
                out.append(line)
                out.append(moved)
                if ins[0] == "jal":
                    call_slot = len(out) - 1
                i += 2
                continue
            la_addr = (
                prev_la is not None
                and prev_la[0] == "la"
                and len(prev_la[1]) == 2
                and "(" not in prev_la[1][1]
                and not (prev2 and BRANCHES.match(prev2[0]))
            )
            if la_addr:
                # ASPSX expanded `la` itself and put the low half in the slot,
                # even right after a label
                reg, sym = prev_la[1]
                out[k] = f"lui\t{reg},%hi({sym})"
                out.append(line)
                out.append(f"addiu\t{reg},{reg},%lo({sym})")
                if ins[0] == "jal":
                    call_slot = len(out) - 1
                i += 2
                continue
            if idx_store and not (prev2 and BRANCHES.match(prev2[0])):
                # a store to symbol($r): the address half stays, the store moves
                reg, addr = prev[1]
                sym, base = re.match(r"^(.*)\((\$\w+)\)$", addr).groups()
                out[k] = f".set\tnoat\nlui\t$at,%hi({sym})\naddu\t$at,$at,{base}"
                out.append(line)
                out.append(f"{prev[0]}\t{reg},%lo({sym})($at)\n.set\tat")
                if ins[0] == "jal":
                    call_slot = len(out) - 1
                i += 2
                continue
            if sym_store and not (prev2 and BRANCHES.match(prev2[0])):
                # a store to a symbol: the lui of $at stays, the store moves
                reg, sym = prev[1]
                out[k] = f".set\tnoat\nlui\t$at,%hi({sym})"
                out.append(line)
                out.append(f"{prev[0]}\t{reg},%lo({sym})($at)\n.set\tat")
                if ins[0] == "jal":
                    call_slot = len(out) - 1
                i += 2
                continue
            if (
                prev is not None
                and prev[0] == "la"
                and prev[1][0] != "$31"
                and not (prev2 and BRANCHES.match(prev2[0]))
            ):
                # ASPSX expands la and moves its second half into the slot,
                # even when the la is a branch target: its lui stays there
                reg, sym = prev[1]
                out[k] = f"lui\t{reg},%hi({sym})"
                out.append(line)
                out.append(f"addiu\t{reg},{reg},%lo({sym})")
                if ins[0] == "jal":
                    call_slot = len(out) - 1
                i += 2
                continue
        out.append(line)
        i += 1
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
