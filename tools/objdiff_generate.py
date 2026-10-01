#!/usr/bin/env python3
"""Write objdiff.json with one unit per game module.

Each C file of the game (src/main/<subsystem>/<module>.c, and each overlay's
src/<overlay>/<module>.c) is a unit, like
jype0/dw_decomp does: its target object is splat's full disassembly of that
segment (asm/<version>/main/<path>.s), which holds its .rodata and .data
too. The game's data that no module owns (asm/<version>/main/data/game.*.s,
the .bss) is reported with main.

It works on one version, VERSION as for make (tools/version.py): its
configs in config/<version>/, its objects in build/<version>/ and
expected/<version>/. The unit names don't carry the version, so every
version reports the same units.

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

Code that has no C yet is one of splat's asm segments (a version that is
still being split, such as jp and eu): each is a unit too, with no base
object, so objdiff counts all of its code and data as unmatched. Its target
is splat's assembly of the segment's code with the rodata, data and bss
segments of the same name.

The PsyQ SDK (src/main/psyq/) and the soft-float library (libmath.s and
libmath.c) are Sony's and the compiler's code, not the game's: like other
PSX decomps (jype0/dw_decomp), progress doesn't count them. Hand-written
assembly (hasm segments) isn't a C unit, so it isn't in the report either.
"""

import json
import re
import subprocess
from pathlib import Path

from elftools.elf.elffile import ELFFile

from version import BUILD_DIR, CONFIG_DIR, EXPECTED_DIR, ROOT

CATEGORIES = [
    {"id": "game", "name": "Game"},
    {"id": "executable", "name": "Executable"},
    {"id": "overlays", "name": "Overlays"},
]

# library code linked with the game, left out of the progress
LIBRARIES = {"psyq", "libmath"}

# the game's data that no module owns (its .bss), reported with main
UNOWNED_DATA = "game"

# the version's build and target objects, as objdiff.json names them
# (relative to the root)
BUILD = BUILD_DIR.relative_to(ROOT).as_posix()
EXPECTED = EXPECTED_DIR.relative_to(ROOT).as_posix()


def hand_written(binary: str) -> set:
    """The names of BINARY's hand-written assembly: its hasm segments in any
    version's config. A version that is still splat's assembly has them as
    asm segments (tools/split_version.py), which aren't units either."""
    names = set()
    for config in ROOT.glob(f"config/*/{binary}.yaml"):
        names |= set(re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, hasm, ([\w/]+)\]", config.read_text(), re.M))
    return names


def game_modules() -> list:
    """The game's modules, as (binary/path, type): its C files ("c") and the
    code still in splat's asm segments ("asm"), the executable's in ROM
    order (config/<version>/main.yaml), then each overlay's
    (config/<version>/<overlay>.yaml). A module that is only data and still
    asm (an overlay's <prefix>_bss in jp or eu) has no code segment, only
    its data segments: it is an "asm" module too."""
    configs = ["main"] + sorted(p.stem for p in CONFIG_DIR.glob("*.yaml") if p.stem != "main")
    modules = []
    for binary in configs:
        yaml = (CONFIG_DIR / f"{binary}.yaml").read_text()
        segments = re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, ([.\w]+), ([\w/]+)\]", yaml, re.M)
        skip = LIBRARIES | hand_written(binary)
        code = [m for kind, m in segments if kind in ("c", "asm")]
        modules += [(f"{binary}/{m}", kind) for kind, m in segments
                    if kind in ("c", "asm") and m not in skip]
        data_only = [m for kind, m in segments if kind in ("rodata", "data")
                     and m not in code and m not in skip | {UNOWNED_DATA}]
        modules += [(f"{binary}/{m}", "asm") for m in dict.fromkeys(data_only)]
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


def asm_rodata(path: str) -> set:
    """The names of PATH's .rodata data still included from asm (INCLUDE_RODATA)."""
    return {name[: -len(".NON_MATCHING")] for name, _ in rodata_symbols(path) if name.endswith(".NON_MATCHING")}


def mark_asm_rodata(path: str, names: set) -> None:
    """Flip the first byte of each of NAMES in PATH's .rodata.

    INCLUDE_RODATA copies the target's bytes, so objdiff would count them as
    matched although they are still asm. A changed byte makes the section
    they are in differ, and objdiff counts a data section only when all of
    it matches: the report then shows that section as missing until its
    data is written in C."""
    with open(ROOT / path, "rb") as f:
        elf = ELFFile(f)
        rodata = elf.get_section_by_name(".rodata")
        offsets = [rodata["sh_offset"] + value for name, value in rodata_symbols(path) if name in names]
    blob = bytearray((ROOT / path).read_bytes())
    for offset in offsets:
        blob[offset] ^= 0xFF
    (ROOT / path).write_bytes(blob)


def name_rodata(base: str, target: str) -> None:
    """Give BASE's anonymous rodata the names TARGET has at the same offsets."""
    have = {name for name, _ in rodata_symbols(base)}
    # INCLUDE_RODATA data carries a .NON_MATCHING marker, which makes objdiff
    # leave it out of the section's compared range and cut the section
    # short; renamed, it is compared like the rest (and mark_asm_rodata then
    # makes sure it doesn't count as matched)
    asm = asm_rodata(base)
    args = []
    for name in sorted(asm):
        args += ["--strip-symbol", name + ".NON_MATCHING"]
    # a name the base already uses (defined elsewhere, or referenced) stays out
    with open(ROOT / base, "rb") as f:
        used = {s.name for s in ELFFile(f).get_section_by_name(".symtab").iter_symbols() if s.name}
    for name, offset in rodata_symbols(target):
        if name not in used and not name.endswith(".NON_MATCHING"):
            args += ["--add-symbol", f"{name}=.rodata:{offset:#x},object,global"]
    if args:
        subprocess.run(["mipsel-linux-gnu-objcopy"] + args + [base], cwd=ROOT, check=True)


def type_rodata_objects(path: str) -> None:
    """Give the names GCC defines in PATH's .rodata the object type.

    GCC declares no type for its data, so a const array's symbol is
    untyped, while the target's names added above are objects. objdiff
    sizes an object up to the next object, over the untyped symbols in
    between, which are then left without a size and can't be paired; as
    objects, every symbol ends where the next one starts."""
    with open(ROOT / path, "rb") as f:
        elf = ELFFile(f)
        names = [s.name for s in elf.iter_sections()]
        if ".rodata" not in names:
            return
        rodata = names.index(".rodata")
        symtab = elf.get_section_by_name(".symtab")
        entries = [symtab["sh_offset"] + i * symtab["sh_entsize"]
                   for i, s in enumerate(symtab.iter_symbols())
                   if s["st_shndx"] == rodata and s.name
                   and s["st_info"]["type"] == "STT_NOTYPE" and s["st_info"]["bind"] == "STB_GLOBAL"]
    if not entries:
        return
    blob = bytearray((ROOT / path).read_bytes())
    for entry in entries:
        # st_info: binding in the high nibble, type in the low one (1 = object)
        blob[entry + 12] = (blob[entry + 12] & 0xF0) | 1
    (ROOT / path).write_bytes(blob)


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


def unit_name(module: str) -> str:
    """The report's name for MODULE: a module one version builds from its
    own copy (src/<binary>/<module>_jp.c, CONTRIBUTING.md) is the same unit
    as the shared file's in the other versions."""
    return re.sub(r"_(us|jp|eu)$", "", module)


def unit(module: str, data: list) -> dict:
    """The objdiff unit of MODULE, with the data objects DATA in its target."""
    target = f"{EXPECTED}/asm/{module}.s.o"
    target_all = f"{EXPECTED}/report/{module}.s.o"
    link(target_all, [target] + data)
    target = target_all
    base = f"{BUILD}/report/{module}.c.o"
    (ROOT / base).parent.mkdir(parents=True, exist_ok=True)
    (ROOT / base).write_bytes((ROOT / f"{BUILD}/src/{module}.c.o").read_bytes())
    pad_sections(base, target)
    asm = asm_rodata(base)
    name_rodata(base, target)
    mark_asm_rodata(base, asm)
    type_rodata_objects(base)
    for path in (target, base):
        for section in (".data", ".rodata"):
            relocate_by_section(path, section)
    return {
        "name": unit_name(module),
        "target_path": target,
        "base_path": base,
        "metadata": {"progress_categories": ["game", "executable" if module.startswith("main/") else "overlays"],
                     "source_path": f"src/{module}.c"},
    }


def asm_unit(module: str, data: list) -> dict:
    """The objdiff unit of MODULE, an asm segment: no base object, and a
    target of its code with its data segments (binary/data/<name>.*) and the
    data objects DATA."""
    binary, name = module.split("/", 1)
    target = f"{EXPECTED}/report/{module}.s.o"
    code = f"{EXPECTED}/asm/{module}.s.o"
    link(target, [p for p in [code] if (ROOT / p).exists()]
         + data_objects(name, binary, ("rodata", "data", "bss")) + data)
    return {
        "name": unit_name(module),
        "target_path": target,
        "metadata": {"progress_categories": ["game", "executable" if binary == "main" else "overlays"]},
    }


def data_objects(name: str, binary: str = "main", sections: tuple = ("data", "bss")) -> list:
    """splat's data objects for BINARY's data segments named NAME."""
    paths = [f"{EXPECTED}/asm/{binary}/data/{name}.{s}.s.o" for s in sections]
    return [p for p in paths if (ROOT / p).exists()]


def main() -> None:
    units = []
    for module, kind in game_modules():
        # the game's data no module owns goes with main
        data = data_objects(UNOWNED_DATA) if module == "main/main" else []
        if kind == "asm":
            units.append(asm_unit(module, data))
        else:
            units.append(unit(module, data))

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
