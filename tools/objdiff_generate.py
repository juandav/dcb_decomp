#!/usr/bin/env python3
"""Write objdiff.json with one unit per game module.

Each C file of the game (src/main/<subsystem>/<module>.c, and each overlay's
src/<overlay>/<module>.c) is a unit, like
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
    {"id": "executable", "name": "Executable"},
    {"id": "overlays", "name": "Overlays"},
]

# library code linked with the game, left out of the progress
LIBRARIES = {"psyq", "libmath"}

# the game's data that no module owns (its .bss), reported with main
UNOWNED_DATA = "game"


def game_modules() -> list:
    """The game's C modules, as binary/path: the executable's in ROM order
    (config/main.yaml), then each overlay's (config/<overlay>.yaml)."""
    configs = ["main"] + sorted(p.stem for p in (ROOT / "config").glob("*.yaml") if p.stem != "main")
    modules = []
    for binary in configs:
        yaml = (ROOT / "config" / f"{binary}.yaml").read_text()
        modules += [f"{binary}/{m}" for m in re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, c, ([\w/]+)\]", yaml, re.M)
                    if m not in LIBRARIES]
    return modules


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
        args += ["--strip-symbol", name + ".NON_MATCHING"]
    for name, offset in rodata_symbols(target):
        if name not in have and re.fullmatch(r"(D|jtbl)_[0-9A-F]{8}", name):
            args += ["--add-symbol", f"{name}=.rodata:{offset:#x},object,global"]
    # rodata the original keeps at the start of its .text (splat sees a
    # function there) ends the target's .rodata early; a marker where it ends
    # keeps the last table from running into it
    end = len(section_bytes(target, ".rodata"))
    if len(section_bytes(base, ".rodata")) > end:
        args += ["--add-symbol", f"rodata_in_text={'.rodata'}:{end:#x},object,local"]
    if args:
        subprocess.run(["mipsel-linux-gnu-objcopy"] + args + [base], cwd=ROOT, check=True)


def relocate_by_section(path: str, section: str) -> None:
    """Make PATH's SECTION relocations into its own .rodata section-relative.

    splat names the strings a table in .data or .rodata points to, so the
    original's relocations use those symbols; GCC relocates against .rodata
    plus the offset, stored in the word, except for strings it only knows
    by an extern name. Same data, written differently: rewrite both sides
    the way GCC writes its literals."""
    with open(ROOT / path, "rb") as f:
        elf = ELFFile(f)
        names = [s.name for s in elf.iter_sections()]
        if ".rel" + section not in names or ".rodata" not in names:
            return
        rodata = names.index(".rodata")
        data = elf.get_section_by_name(section)
        rel = elf.get_section_by_name(".rel" + section)
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


def relocate_text_by_function(path: str, section: str) -> None:
    """Make PATH's SECTION relocations into .text relative to their function.

    Jump tables point at labels inside functions: GCC relocates against .text
    plus the offset, splat against a label. When the original's .text starts
    with bytes that our C keeps in .rodata, the section offsets no longer
    agree, so express both as the function that holds the label plus the
    offset into it."""
    with open(ROOT / path, "rb") as f:
        elf = ELFFile(f)
        names = [s.name for s in elf.iter_sections()]
        if ".rel" + section not in names or ".text" not in names:
            return
        text = names.index(".text")
        data = elf.get_section_by_name(section)
        rel = elf.get_section_by_name(".rel" + section)
        symtab = elf.get_section_by_name(".symtab")
        syms = list(symtab.iter_symbols())
        funcs = sorted((s["st_value"], s["st_size"], i) for i, s in enumerate(syms)
                       if s["st_shndx"] == text and s["st_info"]["type"] == "STT_FUNC" and s["st_size"])
        blob = bytearray((ROOT / path).read_bytes())
        changed = False
        for i, r in enumerate(rel.iter_relocations()):
            sym = syms[r["r_info_sym"]]
            if r["r_info_type"] != 2 or sym["st_shndx"] != text or sym["st_info"]["type"] == "STT_FUNC":
                continue
            word = data["sh_offset"] + r["r_offset"]
            target = sym["st_value"] + int.from_bytes(blob[word:word + 4], "little")
            owner = [(v, n) for v, size, n in funcs if v <= target < v + size]
            if not owner:
                continue
            value, index = owner[0]
            entry = rel["sh_offset"] + i * rel["sh_entsize"]
            blob[entry + 4:entry + 8] = ((index << 8) | r["r_info_type"]).to_bytes(4, "little")
            blob[word:word + 4] = (target - value).to_bytes(4, "little")
            changed = True
    if changed:
        (ROOT / path).write_bytes(blob)


def unit(module: str, data: list) -> dict:
    """The objdiff unit of MODULE, with the data objects DATA in its target."""
    target = f"expected/asm/{module}.s.o"
    target_all = f"expected/report/{module}.s.o"
    link(target_all, [target] + data)
    target = target_all
    base = f"build/report/{module}.c.o"
    (ROOT / base).parent.mkdir(parents=True, exist_ok=True)
    (ROOT / base).write_bytes((ROOT / f"build/src/{module}.c.o").read_bytes())
    pad_sections(base, target)
    name_rodata(base, target)
    for path in (target, base):
        for section in (".data", ".rodata"):
            relocate_by_section(path, section)
            relocate_text_by_function(path, section)
    return {
        "name": module,
        "target_path": target,
        "base_path": base,
        "metadata": {"progress_categories": ["game", "executable" if module.startswith("main/") else "overlays"],
                     "source_path": f"src/{module}.c"},
    }


def data_objects(name: str) -> list:
    """splat's data objects for the data segment NAME."""
    paths = [f"expected/asm/main/data/{name}.{s}.s.o" for s in ("data", "bss")]
    return [p for p in paths if (ROOT / p).exists()]


def main() -> None:
    units = []
    for module in game_modules():
        units.append(unit(module, data_objects(UNOWNED_DATA) if module == "main/main" else []))

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
