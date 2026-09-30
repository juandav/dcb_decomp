#!/usr/bin/env python3
"""Lay out GCC jump tables where the original ones are.

GCC aligns each jump table to 8 bytes relative to the start of its file's
rodata. The modules in src/ don't always start where the original source
files did, so that start is not always where it was in the original build.
This filter reads maspsx
output on stdin and, for each jump table of a function whose original table
sits at an address that is 4 modulo 8, emits `.align 2` instead of
`.align 3` and pads the table with zero words up to the size splat gave the
original table (the padding the original file needed before its next
table). When the data before such a table ends 8-byte aligned, the
original file had 4 bytes of padding there that our C doesn't emit (splat
puts them in the previous symbol, as `.asciz ""` or `.word 0`); the filter
finds those tables by assembling its output and adds the 4 bytes.

The original tables are read from splat's full disassembly of each C
segment (asm/<version>/<binary>/<segment>.s). The file's own rodata start (its
.rodata subsegment in config/<version>/<binary>.yaml, given the unit's path
under src/ such as main/gfx/prim as argument) decides which tables sit 4
bytes past an 8-byte boundary relative to that start.
"""

import glob
import os
import re
import subprocess
import sys
import tempfile

from version import ASM_DIR, CONFIG_DIR

_tables = None
# the binary whose unit is being filtered: the overlays share their
# addresses, so the same function name can be in several of them
_binary = None

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))


def rodata_start(unit):
    """VRAM where the .rodata subsegment of `unit` (binary/path) starts."""
    binary, _, path = unit.partition("/")
    text = open(os.path.join(CONFIG_DIR, binary + ".yaml")).read()
    m = re.search(r"\[(0x[0-9A-Fa-f]+), \.rodata, %s\]" % re.escape(path), text)
    if not m:
        return 0
    code = re.search(r"type: code\s+start: (0x[0-9A-Fa-f]+)\s+vram: (0x[0-9A-Fa-f]+)", text)
    return int(code.group(2), 16) + int(m.group(1), 16) - int(code.group(1), 16)


def unit_asm(only=None):
    """splat's full disassembly of each C segment
    (asm/<version>/<binary>/<unit>.s), or only of those of the binary ONLY."""
    for config in glob.glob(os.path.join(CONFIG_DIR, "*.yaml")):
        binary = os.path.basename(config)[:-5]
        if only is not None and binary != only:
            continue
        for unit in re.findall(r"\[0x[0-9A-Fa-f]+, c, ([\w/]+)\]", open(config).read()):
            path = os.path.join(ASM_DIR, binary, unit + ".s")
            if os.path.exists(path):
                yield path


def load_tables():
    """Map each function to the (address, size in words) of its jump tables."""
    by_func = {}
    for path in unit_asm(_binary):
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


AS = ["mipsel-linux-gnu-as", "-EL", "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O1", "-G0",
      "-I" + os.path.join(ROOT, "include"), "-I" + os.path.join(ROOT, "external/psyq_headers/psyq_lib47/include")]


def layout(lines, start, pad_before, marks=False):
    """The filtered output: tables sitting 4 mod 8 get `.align 2`, the zero
    words the original put after them, and 4 bytes before them if their
    index is in pad_before. With marks, a label before each such table."""
    out = []
    func = None
    seen = 0
    moved = 0
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
                if moved in pad_before:
                    out.append("\t.space\t4\n")
                if marks:
                    out.append("jtbl_align_mark_%d:\n" % moved)
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
                moved += 1
                continue
            seen += 1
        out.append(line)
        i += 1
    return out, moved


def misplaced(text):
    """Indexes of the marked tables that the assembler put 0 mod 8 into .rodata."""
    with tempfile.TemporaryDirectory() as tmp:
        src, obj = os.path.join(tmp, "a.s"), os.path.join(tmp, "a.o")
        open(src, "w").write(text)
        if subprocess.run(AS + ["-o", obj, src], cwd=ROOT, capture_output=True).returncode:
            return []
        syms = subprocess.run(["mipsel-linux-gnu-nm", obj], capture_output=True, text=True).stdout
    return [int(n) for off, n in re.findall(r"^([0-9a-f]+) [a-zA-Z] jtbl_align_mark_(\d+)$", syms, re.M) if int(off, 16) % 8 == 0]


def main():
    global _binary
    start = rodata_start(sys.argv[1]) if len(sys.argv) > 1 else 0
    if len(sys.argv) > 1:
        _binary = sys.argv[1].partition("/")[0]
    lines = sys.stdin.readlines()
    pad_before = set()
    out, moved = layout(lines, start, pad_before)
    # each pad shifts what follows, so fix the first misplaced table and look again
    for _ in range(moved):
        wrong = [n for n in misplaced("".join(layout(lines, start, pad_before, marks=True)[0])) if n not in pad_before]
        if not wrong:
            break
        pad_before.add(min(wrong))
    if pad_before:
        out, _ = layout(lines, start, pad_before)
    sys.stdout.writelines(out)


if __name__ == "__main__":
    main()
