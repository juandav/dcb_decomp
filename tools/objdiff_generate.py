#!/usr/bin/env python3
"""Write objdiff.json with the game and the PsyQ SDK as two units.

The game's code lives in one C file per module under src/main/, but it is
reported as a single main/game unit, as it was when it was all game.c: the
module objects are linked with `ld -r`, in ROM order, into one base object,
and the same modules' target objects (splat's full disassembly of each C
segment) into one target object. The target also gets the game's data
(.data and .bss, still assembly), so data keeps being measured. That keeps
every function and section under the unit name decomp.dev has tracked all
along.

src/main/psyq/ holds one file per PsyQ library object; they are linked into a
single build/src/main/psyq.c.o and form the main/psyq unit, in its own
progress category.

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
    {"id": "sdk", "name": "PsyQ SDK"},
]

# the game's data, disassembled by splat (not C yet)
GAME_DATA = ["expected/asm/main/data/game.data.s.o", "expected/asm/main/data/game.bss.s.o"]


def game_modules() -> list:
    """The game's C modules in ROM order, from config/main.yaml."""
    yaml = (ROOT / "config" / "main.yaml").read_text()
    return [m for m in re.findall(r"^\s*- \[0x[0-9A-Fa-f]+, c, (\w+)\]", yaml, re.M) if m != "psyq"]


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
        {
            "name": "main/psyq",
            "target_path": "expected/asm/main/psyq.s.o",
            "base_path": "build/src/main/psyq.c.o",
            "metadata": {"progress_categories": ["sdk"]},
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
