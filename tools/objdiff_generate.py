#!/usr/bin/env python3
"""Write objdiff.json with one unit per C file of the game under src/.

The PsyQ SDK (src/main/psyq/, one file per library object) is not part of
the game: like other PSX decomps, progress counts only the game's own code,
so it gets no unit.

Target objects are splat's full disassembly of each C segment
(expected/asm/<segment>/<file>.s.o); base objects are the files built from
src/, where every function still behind INCLUDE_ASM carries a .NON_MATCHING
label that objdiff drops from the progress count.
"""

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

CATEGORIES = [
    {"id": "game", "name": "Game"},
]


def main() -> None:
    units = []
    names = [
        src.relative_to(ROOT / "src").with_suffix("").as_posix()
        for src in (ROOT / "src").rglob("*.c")
        if not src.is_relative_to(ROOT / "src/main/psyq")
    ]
    for name in sorted(names):
        units.append(
            {
                "name": name,
                "target_path": f"expected/asm/{name}.s.o",
                "base_path": f"build/src/{name}.c.o",
                "metadata": {"progress_categories": ["game"]},
            }
        )

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
