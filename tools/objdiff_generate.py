#!/usr/bin/env python3
"""Write objdiff.json with the game's code and data as one unit.

The game's code lives in one C file per module, grouped by subsystem under
src/main/ (gfx/prim.c, duel/duel.c, ...), but it is reported as a single
main/game unit, as it was when it was all game.c: the module objects are linked with `ld -r`, in ROM order, into one base object,
and the same modules' target objects (splat's full disassembly of each C
segment) into one target object. The target also gets the game's data
(.data and .bss, still assembly), so data keeps being measured. That keeps
every function and section under the unit name decomp.dev has tracked all
along.

The PsyQ SDK (src/main/psyq/) and the soft-float library (libmath.c) are
Sony's and the compiler's code, not the game's: like other PSX decomps
(jype0/dw_decomp), progress doesn't count them.

Base objects are the files built from src/, where every function still
behind INCLUDE_ASM carries a .NON_MATCHING label that objdiff drops from the
progress count.
"""

import json
import re
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

CATEGORIES = [
    {"id": "game", "name": "Game"},
]

# library code linked with the game, left out of the progress
LIBRARIES = {"psyq", "libmath"}

# the game's data, disassembled by splat (not C yet)
GAME_DATA = ["expected/asm/main/data/game.data.s.o", "expected/asm/main/data/game.bss.s.o"]


def game_modules() -> list:
    """The game's C modules in ROM order, from config/main.yaml."""
    yaml = (ROOT / "config" / "main.yaml").read_text()
    return [m for m in re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, c, ([\w/]+)\]", yaml, re.M) if m not in LIBRARIES]


def link(out: str, parts: list) -> None:
    """ld -r the objects PARTS into OUT (paths relative to the root)."""
    (ROOT / out).parent.mkdir(parents=True, exist_ok=True)
    subprocess.run(["mipsel-linux-gnu-ld", "-r", "-o", out] + parts, cwd=ROOT, check=True)


def main() -> None:
    modules = game_modules()
    link("expected/report/main/game.s.o", [f"expected/asm/main/{m}.s.o" for m in modules] + GAME_DATA)
    link("build/report/main/game.c.o", [f"build/src/main/{m}.c.o" for m in modules])
    units = [
        {
            "name": "main/game",
            "target_path": "expected/report/main/game.s.o",
            "base_path": "build/report/main/game.c.o",
            "metadata": {"progress_categories": ["game"]},
        },
    ]

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
