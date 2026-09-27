#!/usr/bin/env python3
"""Leave the return of each function in reorder mode, as GCC 2.7.2 did.

GCC 2.8 fills the delay slot of the final `j $31` itself:

    .set noreorder / .set nomacro / j $31 / addu $sp,$sp,N / .set macro / .set reorder

The compiler that built the PsyQ libraries did not; ASPSX filled that slot
later (see aspsx_reorder.py). This turns the block back into `addu` + `j $31`.

A body instruction GCC put in that slot from right after a branch target stays
there: ASPSX would not have moved it out (_SsVmAlloc: `$L: j $31; andi`).

usage: unfill_epilogue.py < cc1_output.s > output.s
"""

import sys


def at_label(out):
    """True if the last line output so far is a label."""
    for x in reversed(out):
        x = x.split("#", 1)[0].strip()
        if x:
            return x.endswith(":")
    return False


def main():
    lines = sys.stdin.read().split("\n")
    out = []
    i = 0
    while i < len(lines):
        block = [x.split("#", 1)[0].split() for x in lines[i : i + 6]]
        if (
            len(block) == 6
            and block[0] == [".set", "noreorder"]
            and block[1] == [".set", "nomacro"]
            and block[2] == ["j", "$31"]
            and block[4] == [".set", "macro"]
            and block[5] == [".set", "reorder"]
            and not (at_label(out) and not block[3][1].startswith("$sp,$sp,"))
        ):
            # drop the "#nop" GCC left for the load delay of $31
            if out and out[-1].strip() == "#nop":
                out.pop()
            out.append(lines[i + 3])
            out.append(lines[i + 2])
            i += 6
            continue
        out.append(lines[i])
        i += 1
    sys.stdout.write("\n".join(out))


if __name__ == "__main__":
    main()
