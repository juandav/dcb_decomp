#!/usr/bin/env python3
"""Lay out GCC jump tables where the original ones are.

GCC aligns each jump table to 8 bytes relative to the start of its file's
rodata. src/main/game.c holds many of the original source files, so that
start is not where it was in the original build. This filter reads maspsx
output on stdin and, for each jump table of a function whose original table
sits at an address that is 4 modulo 8, emits `.align 2` instead of
`.align 3` and pads the table with zero words up to the size splat gave the
original table (the padding the original file needed before its next
table).

The original tables are read from splat's full disassembly of each C
segment (asm/main/<segment>.s). The file's own rodata start (its .rodata
subsegment in config/main.yaml, given the unit name as argument) decides
which tables sit 4 bytes past an 8-byte boundary relative to that start.
"""

import glob
import os
import re
import sys

ASM_DIR = os.path.join(
    os.path.dirname(os.path.dirname(os.path.abspath(__file__))), "asm", "main"
)

_tables = None

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def rodata_start(unit):
    """VRAM where the .rodata subsegment named `unit` starts (0 if none)."""
    text = open(os.path.join(ROOT, "config", "main.yaml")).read()
    m = re.search(r"\[(0x[0-9A-Fa-f]+), \.rodata, %s\]" % re.escape(unit), text)
    return 0x80010000 + int(m.group(1), 16) - 0x800 if m else 0


def load_tables():
    """Map each function to the (address, size in words) of its jump tables."""
    by_func = {}
    for path in glob.glob(os.path.join(ASM_DIR, "*.s")):
        text = open(path).read()
        sizes = {
            m.group(1): m.group(2).count(".word")
            for m in re.finditer(r"dlabel jtbl_([0-9A-F]{8})\n(.*?)enddlabel", text, re.S)
        }
        for m in re.finditer(r"glabel (\w+)\n(.*?)endlabel \1", text, re.S):
            refs = sorted(set(re.findall(r"%hi\(jtbl_([0-9A-F]{8})\)", m.group(2))))
            if refs:
                by_func[m.group(1)] = [(int(a, 16), sizes.get(a, 0)) for a in refs]
    return by_func


def jump_tables(func):
    global _tables
    if _tables is None:
        _tables = load_tables()
    return _tables.get(func, [])


def main():
    start = rodata_start(sys.argv[1]) if len(sys.argv) > 1 else 0
    lines = sys.stdin.readlines()
    out = []
    func = None
    seen = 0
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r"\s*\.ent\s+(\w+)", line)
        if m:
            func = m.group(1)
            seen = 0
        if func is not None and re.match(r"\s*\.align\s+3\s*$", line):
            tables = jump_tables(func)
            if seen < len(tables) and (tables[seen][0] - start) % 8 == 4:
                out.append(re.sub(r"\.align\s+3", ".align 2", line))
                i += 1
                # label, then the table's .word entries
                while i < len(lines) and not re.match(r"\s*\.word\s", lines[i]):
                    out.append(lines[i])
                    i += 1
                words = 0
                while i < len(lines) and re.match(r"\s*\.word\s", lines[i]):
                    out.append(lines[i])
                    words += 1
                    i += 1
                out.extend("\t.word\t0\n" for _ in range(tables[seen][1] - words))
                seen += 1
                continue
            seen += 1
        out.append(line)
        i += 1
    sys.stdout.writelines(out)


if __name__ == "__main__":
    main()
