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
    """lui $at / addu $at,$at,$r / lw $d,%lo(x)($at)  ->  use $d instead."""
    out = list(lines)
    for i in range(len(out) - 2):
        a, b, c = (split(x) for x in out[i : i + 3])
        if not (a and b and c) or a[0] != "lui" or a[1][:1] != ["$at"]:
            continue
        if b[0] != "addu" or b[1][:2] != ["$at", "$at"]:
            continue
        if not LOADS.match(c[0]) or not c[1][1].endswith("($at)"):
            continue
        dest, index = c[1][0], b[1][2]
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


def main():
    lines = delay_slot_hazards(
        loads_without_at(
            nops_before_labels(no_nop_after_div(sys.stdin.read().split("\n")))
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
