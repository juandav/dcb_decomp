#!/usr/bin/env python3
"""Write objdiff.json with one unit per game module.

Each C file of the game (src/main/<subsystem>/<module>.c) is a unit, like
jype0/dw_decomp does: its target object is splat's full disassembly of that
segment (asm/main/<path>.s), which holds its .rodata and .data too. The
game's data that no module owns (asm/main/data/game.*.s, the .bss) is
reported with main.

objdiff counts a data section as matched only when all of it matches, so
units per module let each module's data count as soon as it is done.

Base objects are the files built from src/, where every function still
behind INCLUDE_ASM carries a .NON_MATCHING label that objdiff drops from the
progress count. The rodata GCC emits for string literals, constants and
jump tables has no symbol of its own, while the target names it (D_...,
jtbl_...); objdiff pairs data by name, so the report's copy of each base
object gets the target's names at the same offsets. objdiff compares a data
section's bytes and relocations up to its last symbol, so the names only
set that range; whatever the base holds there must still be identical.

The PsyQ SDK (src/main/psyq/) and the soft-float library (libmath.c) are
Sony's and the compiler's code, not the game's: like other PSX decomps
(jype0/dw_decomp), progress doesn't count them.
"""

import json
import re
import subprocess
from pathlib import Path

from elftools.elf.elffile import ELFFile

ROOT = Path(__file__).resolve().parent.parent

CATEGORIES = [
    {"id": "game", "name": "Game"},
]

# library code linked with the game, left out of the progress
LIBRARIES = {"psyq", "libmath"}

# the game's data that no module owns yet, reported with main
UNOWNED_DATA = "game"


def game_modules() -> list:
    """The game's C modules in ROM order, from config/main.yaml."""
    yaml = (ROOT / "config" / "main.yaml").read_text()
    return [m for m in re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, c, ([\w/]+)\]", yaml, re.M) if m not in LIBRARIES]


def link(out: str, parts: list) -> None:
    """ld -r the objects PARTS into OUT (paths relative to the root)."""
    (ROOT / out).parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["mipsel-linux-gnu-ld", "-r", "-o", out] + parts, cwd=ROOT, check=True)


def section_bytes(path: str, name: str) -> bytes:
    """The contents of PATH's section NAME (empty if it has none)."""
    with open(ROOT / path, "rb") as f:
        section = ELFFile(f).get_section_by_name(name)
        return section.data() if section else b""


def pad_sections(base: str, target: str) -> None:
    """Pad BASE's .rodata and .data with the zeros that end TARGET's.

    splat counts the padding after a module's last datum as part of it;
    GCC leaves it to the linker, which fills the same zeros in the ROM.
    ld -r pads them and keeps the sections' relocations."""
    pads = {}
    for name in (".rodata", ".data"):
        ours, theirs = section_bytes(base, name), section_bytes(target, name)
        extra = theirs[len(ours):]
        if ours and 0 < len(extra) < 8 and not any(extra):
            pads[name] = len(extra)
    if not pads:
        return
    script = ROOT / (base + ".ld")
    script.write_text("SECTIONS {\n" + "".join(
        f"  {name} 0 : {{ *({name}) . += {n}; }}\n" for name, n in pads.items()) + "}\n")
    padded = base + ".padded"
    subprocess.run(["mipsel-linux-gnu-ld", "-r", "-T", str(script), "-o", padded, base], cwd=ROOT, check=True)
    (ROOT / padded).replace(ROOT / base)
    script.unlink()


def rodata_symbols(path: str) -> list:
    """(name, offset) of the symbols defined in PATH's .rodata."""
    with open(ROOT / path, "rb") as f:
        elf = ELFFile(f)
        index = next((i for i, s in enumerate(elf.iter_sections()) if s.name == ".rodata"), None)
        if index is None:
            return []
        return [
            (s.name, s["st_value"])
            for s in elf.get_section_by_name(".symtab").iter_symbols()
            if s["st_shndx"] == index and s.name and s["st_info"]["type"] != "STT_SECTION"
        ]


def name_rodata(base: str, target: str) -> None:
    """Give BASE's anonymous rodata the names TARGET has at the same offsets."""
    have = {name for name, _ in rodata_symbols(base)}
    # INCLUDE_RODATA data carries a .NON_MATCHING marker, which makes objdiff
    # leave it out of the section's compared range and cut the section
    # short; renamed, it is compared like the rest
    asm = {name[: -len(".NON_MATCHING")] for name in have if name.endswith(".NON_MATCHING")}
    args = []
    for name in sorted(asm):
        args += ["--redefine-sym", f"{name}={name}.asm", "--strip-symbol", name + ".NON_MATCHING"]
    for name, offset in rodata_symbols(target):
        if name not in have and re.fullmatch(r"(D|jtbl)_[0-9A-F]{8}", name):
            args += ["--add-symbol", f"{name}=.rodata:{offset:#x},object,global"]
    if args:
        subprocess.run(["mipsel-linux-gnu-objcopy"] + args + [base], cwd=ROOT, check=True)


def relocate_data_by_section(path: str) -> None:
    """Make PATH's .data relocations into its own .rodata section-relative.

    splat names the strings a table in .data points to, so the original's
    relocations use those symbols; GCC relocates against .rodata plus the
    offset, stored in the word. Same data, written differently: rewrite
    them the way GCC does."""
    with open(ROOT / path, "rb") as f:
        elf = ELFFile(f)
        names = [s.name for s in elf.iter_sections()]
        if ".rel.data" not in names or ".rodata" not in names:
            return
        rodata = names.index(".rodata")
        data = elf.get_section_by_name(".data")
        rel = elf.get_section_by_name(".rel.data")
        symtab = elf.get_section_by_name(".symtab")
        section_sym = next(i for i, s in enumerate(symtab.iter_symbols())
                           if s["st_info"]["type"] == "STT_SECTION" and s["st_shndx"] == rodata)
        patches = []
        for r in rel.iter_relocations():
            sym = symtab.get_symbol(r["r_info_sym"])
            if r["r_info_type"] == 2 and sym["st_shndx"] == rodata and sym["st_info"]["type"] != "STT_SECTION":
                patches.append((r, sym["st_value"]))
        if not patches:
            return
        blob = bytearray((ROOT / path).read_bytes())
        for i, r in enumerate(rel.iter_relocations()):
            match = [v for rr, v in patches if rr["r_offset"] == r["r_offset"]]
            if not match:
                continue
            entry = rel["sh_offset"] + i * rel["sh_entsize"]
            info = (section_sym << 8) | r["r_info_type"]
            blob[entry + 4:entry + 8] = info.to_bytes(4, "little")
            word = data["sh_offset"] + r["r_offset"]
            value = int.from_bytes(blob[word:word + 4], "little") + match[0]
            blob[word:word + 4] = (value & 0xFFFFFFFF).to_bytes(4, "little")
    (ROOT / path).write_bytes(blob)


def unit(module: str, data: list) -> dict:
    """The objdiff unit of MODULE, with the data objects DATA in its target."""
    target = f"expected/asm/main/{module}.s.o"
    target_all = f"expected/report/main/{module}.s.o"
    link(target_all, [target] + data)
    target = target_all
    relocate_data_by_section(target)
    base = f"build/report/main/{module}.c.o"
    (ROOT / base).parent.mkdir(parents=True, exist_ok=True)
    (ROOT / base).write_bytes((ROOT / f"build/src/main/{module}.c.o").read_bytes())
    pad_sections(base, target)
    name_rodata(base, target)
    return {
        "name": f"main/{module}",
        "target_path": target,
        "base_path": base,
        "metadata": {"progress_categories": ["game"], "source_path": f"src/main/{module}.c"},
    }


def data_objects(name: str) -> list:
    """splat's data objects for the data segment NAME."""
    paths = [f"expected/asm/main/data/{name}.{s}.s.o" for s in ("data", "bss")]
    return [p for p in paths if (ROOT / p).exists()]


def main() -> None:
    units = []
    for module in game_modules():
        units.append(unit(module, data_objects(UNOWNED_DATA) if module == "main" else []))

    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "make",
        "build_target": False,
        "build_base": True,
        "watch_patterns": ["*.c", "*.h", "*.s", "*.inc"],
        "units": units,
        "progress_categories": CATEGORIES,
    }

    with open(ROOT / "objdiff.json", "w") as f:
        json.dump(config, f, indent=2)
        f.write("\n")


if __name__ == "__main__":
    main()
