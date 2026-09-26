#!/usr/bin/env python3
"""Write objdiff.json with one unit per C file under src/.

Target objects are splat's full disassembly of each C segment
(expected/asm/<segment>/<file>.s.o); base objects are the files built from
src/, where every function still behind INCLUDE_ASM carries a .NON_MATCHING
label that objdiff drops from the progress count.
"""

import json
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

# Units whose name starts with one of these prefixes go to that category;
# everything else is game code.
CATEGORY_PREFIXES = {
    "main/psyq": "sdk",
}

CATEGORIES = [
    {"id": "game", "name": "Game"},
    {"id": "sdk", "name": "PsyQ SDK"},
]


def category_for(name: str) -> str:
    for prefix, category in CATEGORY_PREFIXES.items():
        if name == prefix or name.startswith(prefix + "/"):
            return category
    return "game"


def main() -> None:
    units = []
    for src in sorted((ROOT / "src").rglob("*.c")):
        rel = src.relative_to(ROOT / "src").with_suffix("")
        name = rel.as_posix()
        units.append(
            {
                "name": name,
                "target_path": f"expected/asm/{name}.s.o",
                "base_path": f"build/src/{name}.c.o",
                "metadata": {"progress_categories": [category_for(name)]},
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
