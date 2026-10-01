#!/usr/bin/env python3
"""Name, in another version (jp, eu), the symbols a shared C file uses.

    tools/version_symbols.py <version> src/<binary>/<module>.c ... [--write]

A module that becomes C in a version links against the names its C uses:
every function and datum it calls or reads must have its name in that
version's symbol files, at that version's address. This reads them from the
module's object (build/<version>/src/<module>.c.o, so build it first: make
VERSION=<version> build/<version>/src/<module>.c.o) and from the original:
each relocation of the object (a jal, a %hi/%lo pair, a pointer in its data)
sits where the original has the same instruction or word, which holds the
version's address. The names the object defines itself get the address of
their section, where the version's config puts the module. A string or
table that only the module's data points to gets splat's automatic name
(D_80012345) in the binary's own symbol file, as us has them, so that
splat labels it and the report pairs the pointers. A datum the C defines
under us's automatic name gets it at the version's address too. And a field of the C's
data that a module still in asm reads by splat's name (D_80012345) gets
its address in undefined_syms, for the link.

A name goes in the version's files that have it in us (config/<v>/symbols.txt,
symbols_<overlay>.txt, symbols_overlay_calls.txt or undefined_syms*.txt),
with us's comment. A name an overlay's C defines that us has only in the
executable (code the version links into the overlay, as jp's SUGSEG holds
main's effect_object) goes in the overlay's file, marked "us-main" for
tools/check_names.py. It prints what it would add, and the names whose
reads disagree or whose address the version's files already give another
name; --write adds the rest. Check the module's functions match first: a
relocation in a function that doesn't line up with the original reads the
wrong word (the reads of one name have to agree, which catches most).
"""

import argparse
import re
import struct
import sys
from collections import defaultdict
from pathlib import Path

import yaml
from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent

LINE = re.compile(r"^\s*(?:/\*.*\*/\s*)?([A-Za-z_][\w.]*)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;\s*(?://(.*)|/\*(.*)\*/)?\s*$")
AUTO_NAME = re.compile(r"^(?:[A-Z]+_)?(?:func|D|jtbl|jlabel)_[0-9A-F]{8}$")

R_MIPS_32, R_MIPS_26, R_MIPS_HI16, R_MIPS_LO16 = 2, 4, 5, 6
HEADER = "names from the C of the modules this version builds (tools/version_symbols.py)"


def symbol_files(version: str, binary: str) -> list:
    """The symbol files the link of BINARY reads, in the order the names
    are looked for."""
    d = ROOT / "config" / version
    names = ["symbols.txt"]
    names += ["symbols_overlay_calls.txt"] if binary == "main" else [f"symbols_{binary}.txt"]
    names += ["undefined_syms.txt"] + ([] if binary == "main" else [f"undefined_syms_{binary}.txt"])
    return [d / n for n in names]


def read_names(path: Path) -> list:
    """[(name, address, comment)] of a symbol file."""
    out = []
    for line in path.read_text().splitlines():
        m = LINE.match(line)
        if m:
            out.append((m[1], int(m[2], 0), (m[3] or m[4] or "").strip()))
    return out


def module_layout(version: str, binary: str, module: str) -> tuple:
    """({section: (file offset, address)} of MODULE in the version's config
    of BINARY, the binary's bytes)."""
    config = yaml.safe_load((ROOT / "config" / version / f"{binary}.yaml").read_text())
    code = next(s for s in config["segments"] if isinstance(s, dict) and s.get("type") == "code")
    out = {}
    for s in code["subsegments"]:
        if isinstance(s, list) and len(s) >= 3 and s[2] == module:
            section = {"c": ".text", ".rodata": ".rodata", ".data": ".data"}.get(s[1])
            if section:
                out[section] = (s[0], code["vram"] + s[0] - code["start"])
    return out, (ROOT / config["options"]["target_path"]).read_bytes()


def sext16(v: int) -> int:
    return v - 0x10000 if v & 0x8000 else v


def addresses(version: str, source: str) -> tuple:
    """({name: {address: reads}}, [problems]) for the C file SOURCE."""
    rel = Path(source).resolve().relative_to(ROOT / "src")
    binary, module = rel.parts[0], "/".join(rel.parts[1:])[: -len(".c")]
    obj = ROOT / "build" / version / "src" / binary / (module + ".c.o")
    if not obj.exists():
        sys.exit(f"{obj.relative_to(ROOT)} is missing: make VERSION={version} {obj.relative_to(ROOT)}")
    layout, original = module_layout(version, binary, module)
    if ".text" not in layout:
        sys.exit(f"{module} is not a c segment in config/{version}/{binary}.yaml")
    found = defaultdict(lambda: defaultdict(int))
    problems = []
    with open(obj, "rb") as f:
        elf = ELFFile(f)
        symtab = elf.get_section_by_name(".symtab")
        sections = [s.name for s in elf.iter_sections()]
        # the names the object defines
        for sym in symtab.iter_symbols():
            if not sym.name or sym["st_info"]["bind"] != "STB_GLOBAL" or sym["st_shndx"] in ("SHN_UNDEF", "SHN_ABS"):
                continue
            if sym["st_shndx"] == "SHN_COMMON":
                continue
            section = sections[sym["st_shndx"]]
            if section in layout:
                found[sym.name][layout[section][1] + sym["st_value"]] += 1
        # the names it uses
        for section in (".text", ".rodata", ".data"):
            rel_section = elf.get_section_by_name(".rel" + section)
            if rel_section is None or section not in layout:
                continue
            ours = elf.get_section_by_name(section).data()
            off, base = layout[section]
            theirs = original[off:off + len(ours)]
            relocs = list(rel_section.iter_relocations())
            # the %hi's of each name, by offset: gas moves a %hi's
            # relocation next to the first %lo it pairs with, so they are
            # found by the register the %lo's instruction uses
            his = defaultdict(dict)
            for r in relocs:
                if r["r_info_type"] == R_MIPS_HI16:
                    his[symtab.get_symbol(r["r_info_sym"]).name][r["r_offset"]] = True
            for r in relocs:
                sym = symtab.get_symbol(r["r_info_sym"])
                o = r["r_offset"]
                if sym["st_info"]["type"] == "STT_SECTION":
                    # a pointer in the data to the module's own literals: the
                    # original's word is the address of a string splat only
                    # labels if a name says so
                    if (r["r_info_type"] == R_MIPS_32 and section != ".text"
                            and sections[sym["st_shndx"]] in (".rodata", ".data") and o + 4 <= len(theirs)):
                        found[None][struct.unpack_from("<I", theirs, o)[0]] += 1
                    continue
                if not sym.name:
                    continue
                if o + 4 > len(theirs):
                    continue
                a = struct.unpack_from("<I", ours, o)[0]
                b = struct.unpack_from("<I", theirs, o)[0]
                t = r["r_info_type"]
                if t == R_MIPS_32:
                    found[sym.name][(b - a) & 0xFFFFFFFF] += 1
                    continue
                if (a >> 26) != (b >> 26):
                    problems.append(f"{module}: the instruction at {section}+{o:#x} ({sym.name}) differs")
                    continue
                if t == R_MIPS_26:
                    pc = (base + o) & 0xF0000000
                    found[sym.name][(pc | ((b & 0x3FFFFFF) << 2)) - ((a & 0x3FFFFFF) << 2)] += 1
                elif t == R_MIPS_LO16:
                    # the closest %hi before it that loads its base register
                    # (or, failing that, the closest %hi of the name)
                    base_reg = (a >> 21) & 31
                    cands = sorted((h for h in his[sym.name] if h < o), reverse=True)
                    h = next((h for h in cands
                              if (struct.unpack_from("<I", ours, h)[0] >> 16) & 31 == base_reg), None)
                    if h is None:
                        h = cands[0] if cands else None
                    if h is None:
                        problems.append(f"{module}: %lo({sym.name}) at {section}+{o:#x} has no %hi")
                        continue
                    our_hi = struct.unpack_from("<I", ours, h)[0] & 0xFFFF
                    their_hi = struct.unpack_from("<I", theirs, h)[0] & 0xFFFF
                    addend = (our_hi << 16) + sext16(a & 0xFFFF)
                    found[sym.name][((their_hi << 16) + sext16(b & 0xFFFF) - addend) & 0xFFFFFFFF] += 1
        # what the object covers, and the names it defines
        covered = [(layout[n][1], layout[n][1] + elf.get_section_by_name(n).data_size)
                   for n in (".rodata", ".data") if n in layout and elf.get_section_by_name(n)]
        defined = {sym.name for sym in symtab.iter_symbols()
                   if sym.name and sym["st_shndx"] not in ("SHN_UNDEF",)}
    return found, problems, covered, defined


def asm_reads(version: str, binary: str, covered: list, defined: set) -> dict:
    """{name: address} of splat's automatic names, in the binary's asm, of
    addresses inside COVERED that the C doesn't define: a field of the C's
    data that a module still in asm reads by its own name."""
    config = (ROOT / "config" / version / f"{binary}.yaml").read_text()
    c_modules = set(re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, c, ([\w/]+)\]", config, re.M))
    base = ROOT / "asm" / version / binary
    out = {}
    for path in base.rglob("*.s"):
        rel = path.relative_to(base)
        if rel.parts[0] in ("nonmatchings", "matchings"):
            continue
        # the C modules' own disassembly (objdiff's targets) isn't linked
        module = re.sub(r"\.(rodata|data|bss)$", "", rel.with_suffix("").as_posix())
        if (module[len("data/"):] if module.startswith("data/") else module) in c_modules:
            continue
        for name in set(re.findall(r"\b((?:[A-Z]+_)?D_([0-9A-F]{8}))\b", path.read_text())):
            addr = int(name[1], 16)
            if name[0] not in defined and any(lo <= addr < hi for lo, hi in covered):
                out[name[0]] = addr
    return out


def main() -> None:
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("version")
    parser.add_argument("sources", nargs="+")
    parser.add_argument("--write", action="store_true", help="add the names to the version's symbol files")
    args = parser.parse_args()

    problems = []
    add = defaultdict(list)
    by_binary = defaultdict(list)
    for source in args.sources:
        rel = Path(source).resolve().relative_to(ROOT / "src")
        by_binary[rel.parts[0]].append(source)
    for binary, sources in by_binary.items():
        found = defaultdict(lambda: defaultdict(int))
        covered, defined = [], set()
        for source in sources:
            f, p, c, d = addresses(args.version, source)
            problems += p
            covered += c
            defined |= d
            for name, addrs in f.items():
                for a, n in addrs.items():
                    found[name][a] += n

        # where us has each name (for this binary's link), and the version's
        # names
        us_files = defaultdict(list)
        us_comment = {}
        for path in symbol_files("us", binary):
            for name, _, comment in read_names(path) if path.exists() else []:
                us_files[name].append(path.name)
                us_comment.setdefault(name, comment)
        # the version's names, by file
        have = {}
        by_address = {}
        for path in symbol_files(args.version, binary):
            have[path.name] = {}
            by_address[path.name] = defaultdict(set)
            for name, addr, _ in read_names(path) if path.exists() else []:
                have[path.name][name] = addr
                by_address[path.name][addr].add(name)

        # the literals only the data points to get splat's automatic name,
        # in the binary's own file (as in us)
        own = "symbols.txt" if binary == "main" else f"symbols_{binary}.txt"
        for addr in sorted(found.pop(None, {})):
            name = f"D_{addr:08X}"
            if any(name in names for names in have.values()) or (addr, name, "") in add[own]:
                continue
            others = {n for f in by_address for n in by_address[f].get(addr, ())}
            if others:
                continue
            add[own].append((addr, name, ""))

        # the fields of the C's data that modules still in asm read by
        # splat's names: the link needs their addresses
        undefined = "undefined_syms.txt" if binary == "main" else f"undefined_syms_{binary}.txt"
        for name, addr in sorted(asm_reads(args.version, binary, covered, defined).items(),
                                 key=lambda x: x[1]):
            if name not in have.get(undefined, {}):
                add[undefined].append((addr, name, ""))

        for name in sorted(found):
            addrs = found[name]
            if len(addrs) > 1:
                listed = ", ".join(f"{a:#010x} ({n}x)" for a, n in sorted(addrs.items(), key=lambda x: -x[1]))
                problems.append(f"{name}: the reads disagree: {listed}")
                continue
            addr = next(iter(addrs))
            if name not in us_files:
                if any(name in names for names in have.values()):
                    continue
                if AUTO_NAME.match(name) and name in defined:
                    # a datum the C defines under us's automatic name (one
                    # no code uses): the version's address gets the same
                    # name, so that the report pairs it
                    if (addr, name, "") not in add[own]:
                        add[own].append((addr, name, ""))
                    continue
                problems.append(f"{name} = {addr:#010x}: no us symbol file of {binary} has it")
                continue
            # code us has in the executable that this version links into
            # the overlay: its own names go in the overlay's file, and its
            # other C there uses them from that file
            main_files = {"symbols.txt", "symbols_overlay_calls.txt", "undefined_syms.txt"}
            if binary != "main" and name in have[own] and set(us_files[name]) <= main_files:
                if have[own][name] != addr:
                    problems.append(f"{name}: {own} has it at {have[own][name]:#010x}, the C reads {addr:#010x}")
                continue
            if binary != "main" and name in defined and set(us_files[name]) <= main_files:
                comment = " ".join(["us-main"] + ([us_comment[name]] if us_comment[name] else []))
                if name in have[own]:
                    if have[own][name] != addr:
                        problems.append(f"{name}: {own} has it at {have[own][name]:#010x}, the C reads {addr:#010x}")
                elif (addr, name, comment) not in add[own]:
                    add[own].append((addr, name, comment))
                continue
            # in each of the files us has it in (a label in an overlay's
            # symbols and its address in undefined_syms for the link)
            for file in us_files[name]:
                if name in have[file]:
                    if have[file][name] != addr:
                        problems.append(f"{name}: {file} has it at {have[file][name]:#010x}, the C reads {addr:#010x}")
                    continue
                others = {n for n in by_address[file].get(addr, ()) if not AUTO_NAME.match(n)}
                if others:
                    problems.append(f"{name}: {addr:#010x} is {', '.join(sorted(others))} in {file}")
                    continue
                if (addr, name, us_comment[name]) not in add[file]:
                    add[file].append((addr, name, us_comment[name]))

    for p in problems:
        print("!", p)
    for file, entries in sorted(add.items()):
        print(f"config/{args.version}/{file}:")
        lines = []
        for addr, name, comment in sorted(entries):
            if file.startswith("undefined_syms"):
                line = f"{name} = {addr:#010X};".replace("0X", "0x")
            else:
                line = f"{name} = {addr:#010X};".replace("0X", "0x") + (f" // {comment}" if comment else "")
            lines.append(line)
            print("  " + line)
        if args.write:
            path = ROOT / "config" / args.version / file
            text = path.read_text() if path.exists() else ""
            if file.startswith("undefined_syms"):
                header = ("/* from the C of the modules this version builds (tools/version_symbols.py):\n"
                          "   what it calls by address, and the fields of its data that the modules\n"
                          "   still in asm read by splat's names */")
                if not text:
                    text = f"/* addresses {path.stem.split('_')[-1].upper() if '_syms_' in path.stem else 'the executable'} uses that none of its objects defines */\n"
            else:
                header = f"// {HEADER}"
            if header not in text:
                text = text.rstrip("\n") + ("\n\n" if text else "") + header + "\n"
            path.write_text(text.rstrip("\n") + "\n" + "\n".join(lines) + "\n")


if __name__ == "__main__":
    main()
