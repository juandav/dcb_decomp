#!/usr/bin/env python3
"""Check that every version names its symbols as us does.

The versions share their names (CONTRIBUTING.md): a name in jp's or eu's
symbol files means the same function or datum as that name in us's. The
symbol files are the mapping between versions: tools/match_versions.py
--seed wrote the us names of the functions it paired, and nothing else
needs keeping. So a name of another version must still be one of us's:

- in us's file of the same binary: config/us/symbols.txt (or
  symbols_overlay_calls.txt) for the executable, symbols_<overlay>.txt for
  an overlay;
- a function there if it is a function here (type:func), data if data.

A rename made in us only (or in one version only) leaves the old name
behind somewhere, and this fails; tools/rename.py renames in every version.
Not checked: splat's automatic names (func_80012345, D_80012345), names in a
binary us doesn't have (INTSEG, NISSEG), and a name that is a version's own,
with "version-only" in its comment:

    func_name = 0x80012345; // type:func version-only

A version that links into an overlay code us has in the executable (jp's
SUGSEG holds main's effect_object and effect_prims) names it in the
overlay's file with "us-main" in the comment; that name is checked against
us's executable instead:

    tickEffectMotion = 0x801EBFA4; // type:func us-main

Likewise "us-<overlay>" for code us has in another overlay (jp's ENDSEG
holds OPENSEG's movie player): checked against that overlay's names.
"""

import re
import sys
from collections import Counter
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
CONFIG = ROOT / "config"
LINE = re.compile(r"^\s*([A-Za-z_][\w.]*)\s*=\s*(0x[0-9A-Fa-f]+|\d+)\s*;\s*(?://(.*))?$")
AUTO_NAME = re.compile(r"^(?:[A-Z]+_)?(?:func|D|jtbl|jlabel)_[0-9A-F]{8}$")


def binary_of(path):
    """main for symbols.txt, the overlay for symbols_<overlay>.txt."""
    m = re.match(r"symbols_(\w+)\.txt$", path.name)
    return m[1] if m else "main"


def read(path):
    """[(line number, name, is a function, comment)] of a symbol file."""
    out = []
    for n, line in enumerate(path.read_text().splitlines(), 1):
        m = LINE.match(line)
        if m:
            comment = m[3] or ""
            out.append((n, m[1], "type:func" in comment.split(), comment))
    return out


def main():
    us_dir = CONFIG / "us"
    us_binaries = {p.stem for p in us_dir.glob("*.yaml")}
    # us's names by binary: {binary: {name: is a function}}
    us = {}
    for path in us_dir.glob("symbols*.txt"):
        binary = binary_of(path)
        if binary == "overlay_calls":
            binary = "main"
        for _, name, func, _ in read(path):
            us.setdefault(binary, {})[name] = func
    where = {name: b for b, names in us.items() for name in names}

    errors = []
    checked = 0
    for vdir in sorted(p for p in CONFIG.iterdir() if p.is_dir() and p.name != "us"):
        seen = Counter()
        for path in sorted(vdir.glob("symbols*.txt")):
            binary = binary_of(path)
            rel = path.relative_to(ROOT)
            for n, name, func, comment in read(path):
                seen[(binary, name)] += 1
                if seen[(binary, name)] == 2:
                    errors.append(f"{rel}:{n}: {name} is named twice")
                if binary not in us_binaries or AUTO_NAME.match(name) or "version-only" in comment.split():
                    continue
                checked += 1
                home = next((w[3:] for w in comment.split() if w.startswith("us-") and w[3:] in us_binaries),
                            binary)
                if name not in us.get(home, {}):
                    if name in where:
                        errors.append(f"{rel}:{n}: {name} is {where[name]}'s in us, not {home}'s")
                    else:
                        errors.append(f"{rel}:{n}: us has no {name} (renamed in one version only? "
                                      "tools/rename.py renames in all)")
                elif us[home][name] != func:
                    kind = "a function" if us[home][name] else "data"
                    errors.append(f"{rel}:{n}: {name} is {kind} in us")
    for e in errors:
        print(e)
    if errors:
        sys.exit(f"{len(errors)} names differ from us's")
    print(f"{checked} names of the other versions are us's")


if __name__ == "__main__":
    main()
