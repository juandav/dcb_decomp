"""The version of the game the tools work on, and where its files are.

VERSION comes from the environment, as the Makefile exports it (make
VERSION=us), and defaults to us like the Makefile. Each version has its
settings in mk/version/<version>.mk (the executable's name and the disc),
its splat configs, symbols and checksums in config/<version>/; splat writes
its disassembly to asm/<version>/, the build goes to build/<version>/ and
objdiff's target objects to expected/<version>/.
"""

import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent

VERSION = os.environ.get("VERSION") or "us"

_MK = ROOT / "mk" / "version" / f"{VERSION}.mk"
if not _MK.is_file():
    sys.exit(f"unsupported VERSION {VERSION}: there is no {_MK.relative_to(ROOT)}")
# the NAME := VALUE settings of the version's .mk
_SETTINGS = dict(re.findall(r"^(\w+)\s*:=\s*(.*?)\s*$", _MK.read_text(), re.M))

# the executable's name (SLUS_013.28) and the dumped disc it is on
EXE_NAME = _SETTINGS["EXE_NAME"]
DISK_DIR = ROOT / _SETTINGS["DISK_DIR"]

# the GCC that built the game's code (bin/gcc-<GCC_VERSION>-psx/cc1), as
# the Makefile defaults it
GCC_VERSION = _SETTINGS.get("GCC_VERSION", "2.95.2")
# the extra maspsx flags of the version's game code
MASPSX_EXTRA = _SETTINGS.get("MASPSX_EXTRA", "")

# splat configs (<binary>.yaml), symbols and checksums
CONFIG_DIR = ROOT / "config" / VERSION
# splat's output (<binary>/...)
ASM_DIR = ROOT / "asm" / VERSION
# objects, ELFs, binaries and the report
BUILD_DIR = ROOT / "build" / VERSION
# objdiff's target objects
EXPECTED_DIR = ROOT / "expected" / VERSION
