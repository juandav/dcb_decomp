# Digimon Digital Card Battle decomp

[![Code](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/juandav/dcb_decomp)
[![Data](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=data&label=Data)](https://decomp.dev/juandav/dcb_decomp)
[![Functions](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=functions&label=Functions)](https://decomp.dev/juandav/dcb_decomp)
[![Executable](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&category=executable&label=Executable)](https://decomp.dev/juandav/dcb_decomp?category=executable)
[![Overlays](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&category=overlays&label=Overlays)](https://decomp.dev/juandav/dcb_decomp?category=overlays)

[![Build](https://github.com/juandav/dcb_decomp/actions/workflows/build.yaml/badge.svg)](https://github.com/juandav/dcb_decomp/actions/workflows/build.yaml)
[![Platform](https://img.shields.io/badge/platform-PlayStation-003791)](#the-games-binaries)
[![Version](https://img.shields.io/badge/version-SLUS--01328%20%28USA%29-blue)](#toolchain)
[![Compiler](https://img.shields.io/badge/compiler-GCC%202.95.2-orange)](#toolchain)
[![License](https://img.shields.io/github/license/juandav/dcb_decomp)](LICENSE)

A matching decompilation of **Digimon Digital Card Battle** for the
PlayStation: C source that compiles back into a byte-identical copy of the
game's executable and of its seven overlays.

This repository does not contain any game data. You need your own copy of the
game to build it.

[![Progress map](https://decomp.dev/juandav/dcb_decomp.png)](https://decomp.dev/juandav/dcb_decomp)

<sub>Each rectangle is a module, sized by its code; green means it matches.
Click it for the details on decomp.dev.</sub>

## Status

| | Code | Data | Functions |
|---|---|---|---|
| Executable (`SLUS_013.28`) | 100 % | 100 % | 506 / 506 |
| Overlays (`P.DRV`) | 100 % | 100 % | 864 / 864 |
| **Game** | **100 %** | **100 %** | **1,370 / 1,370** |

- Every function of the game, in the executable and in the overlays, is C that
  compiles to the original bytes. No `INCLUDE_ASM` or `INCLUDE_RODATA` is left in
  the game's code.
- Jump tables, strings and constants live in the C of the function that uses
  them. Tables and globals are written in C in the module that defines them.
  Each overlay's zeroed data is laid out in its `<prefix>_bss.c`.
- Code that was written in assembly stays as assembly source, with a comment
  saying what shows it is hand-written. Examples are the task switching and
  interrupt glue (`src/main/startup.s`), the soft-float routines
  (`src/main/libmath.s`) and some PsyQ objects.
- Functions and globals carry descriptive names taken from the evidence: the
  strings they use, the SDK calls they make, and the data they touch.
- The PsyQ 4.7 libraries linked into the executable are decompiled too, one
  file per library object. They are not the game's code, so they are left out
  of the progress. The 71 PsyQ functions still in assembly are mostly BIOS
  call stubs and register-level code, plus a few near misses.

Progress is measured by [objdiff](https://github.com/encounter/objdiff) with one
unit per module. It is tracked on
[decomp.dev](https://decomp.dev/juandav/dcb_decomp), which comments on every
pull request with what it changes.

## The game's binaries

The executable loads one overlay at a time from `P.DRV`, at the end of its own
`.bss` (`OVERLAY_AREA`, `0x801DDF38`). Each overlay has its own splat config,
source folder and symbol prefix:

| Binary | Prefix | Source | What it runs |
|---|---|---|---|
| `SLUS_013.28` | | `src/main/` | the engine: tasks, rendering, models, windows and text, the card database, the duel loop, the scripts |
| `OPENSEG` | `OPEN_` | `src/openseg/` | the title screen and movies, user registration and the starter deck, memory card saves, friend battles and trades |
| `SAISEG` | `SAI_` | `src/saiseg/` | the areas and world map, opponent selection, area scripts, the hacking screen, Digi-Parts and rewards |
| `SUBSEG` | `SUB_` | `src/subseg/` | the deck editor (base and auto decks, sorting, naming) and the partner screen |
| `EVOSEG` | `EVO_` | `src/evoseg/` | card fusion and its results and rewards, the partner status, the cutscenes |
| `KAWSEG` | `KAW_` | `src/kawseg/` | the duel's rules: hands, bonuses, the CPU opponent, the tutorial, the duel menu and HUD, results, prizes and EXP |
| `SUGSEG` | `SUG_` | `src/sugseg/` | the 3D battle: its scene, camera and HUD, and the effect scripts behind every attack |
| `ENDSEG` | `END_` | `src/endseg/` | the player records screen shown by the ending and the menu |

Memory map of `SLUS_013.28` (psylink puts `.rodata` in front of `.text`):

| Section | Address |
|---|---|
| `.rodata` | `0x80010000`-`0x80013E4C` |
| `.text` | `0x80013E4C`-`0x8006DD3C` (game `0x80013E4C`-`0x8004A610`, PsyQ after) |
| `.data` | `0x8006DD3C`-`0x80077A08` |
| `.bss` | `0x80077A08`-`0x801DDF38` |

## Toolchain

| | |
|---|---|
| Version | USA (`SLUS-01328`) |
| Main executable | `SLUS_013.28`, SHA-1 `fa4e03b5e0671dce399a35f2747080ced492d2c3` |
| Game code | GCC 2.95.2 (`-O1 -G0`) + ASPSX 2.86, emulated with [maspsx](https://github.com/mkst/maspsx) |
| SDK | PsyQ 4.7 (GCC 2.7.2 `-O2`, some objects SN GCC 2.8.1) |
| Splitting | [splat](https://github.com/ethteck/splat) |
| Diffing | [objdiff](https://github.com/encounter/objdiff) 3.8.1, [decomp.dev](https://decomp.dev) |

The PsyQ objects need a binary-patched GCC 2.7.2 (`tools/patch_cc1.py`) and,
for some objects, a patched SN GCC 2.8.1 (`tools/sn_cc1.py`).
`config/us/psyq_objects.txt` says which compiler each object needs.

## Dependencies

On Debian or Ubuntu, install:
```
binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu git make python3 python3-venv unzip wget
```

Clone with the submodules (maspsx, m2c, decomp-permuter and the PsyQ headers):
```
git clone --recursive https://github.com/juandav/dcb_decomp.git
cd dcb_decomp
# or, in an existing clone:
git submodule update --init --recursive
```

Create the Python environment. Keep it active whenever you run `make` or the
scripts in `tools/`:
```
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt
```

Download the prebuilt tools into `bin/`, checked against `tools/deps.sha256`.
They are GCC 2.95.2, 2.7.2 and 2.8.1 for the PSX, objdiff-cli and mkpsxiso:
```
tools/dl_deps.sh
```

## Getting the game files

Extract the disc with mkpsxiso's `dumpsxiso` into `disks/us/`:
```
bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/Digimon - Digital Card Battle (USA).bin"
sha1sum disks/us/SLUS_013.28   # fa4e03b5e0671dce399a35f2747080ced492d2c3
```

The build reads `disks/us/SLUS_013.28` and `disks/us/P.DRV`.
`tools/extract_drv.py` takes each overlay out of `P.DRV`, and
`config/us/overlays.sha1` holds their checksums.

## Build

```
# Split the executable and the overlays with splat (writes asm/us/ and build/us/generated/)
make regenerate

# Build build/us/SLUS_013.28 and the overlays
make -j$(nproc)

# Check all eight binaries against the originals
make compare
```

`make compare` must print `OK` for the executable and for each of the seven
overlays. A change only counts once all eight still match.

`make regenerate` deletes `asm/us/`, `build/us/` (and the patched compilers in
`build/tools/`) and `expected/us/` and splits the binaries again. Run it after
changing a `config/us/*.yaml`, so that no stale files stay behind in `asm/us/`.

To use a different binutils or objdiff, create `local.mk`:
```
TOOLCHAIN := /path/to/mipsel-linux-gnu-
OBJDIFF := /path/to/objdiff-cli
```

### Versions

The build handles one version of the game at a time, picked with `VERSION`:
`make VERSION=us` (the default, and so far the only one). Each version has
its settings in `mk/version/<version>.mk` (the executable's name, the disc
directory and the overlays), its splat configs, symbols and checksums in
`config/<version>/`, its disc in `disks/<version>/`, and its own generated
`asm/<version>/`, `build/<version>/` and `expected/<version>/`. The C and the
assembly see `VERSION_US` (`VERSION_<VERSION>`). The tools take `VERSION` from
the environment too, us by default.

## Progress

```
# Write objdiff.json and the target objects in expected/us/
make objdiff

# Write build/us/report.json
make report
```

After `make objdiff`, open the repository in the
[objdiff](https://github.com/encounter/objdiff) GUI to see each module's
functions and data against the original. `tools/objdiff_generate.py` makes one
unit per module (`main/<subsystem>/<module>`, `<overlay>/<subsystem>/<module>`)
and sorts them into the `executable` and `overlays` categories. It leaves
`psyq` and `libmath` out.

The CI (`.github/workflows/build.yaml`) builds every push, runs `make compare`
and `make report`, and uploads the report that decomp.dev reads. The original
files come from a private repository, so pull requests from forks are not
built.

## Layout

| Path | Contents |
|---|---|
| `src/main/` | the executable: `main.c`, `startup.s`, the soft-float library, and one folder per subsystem |
| `src/main/system/` | start-up (`boot`), the task scheduler (`task`), the render loop and vblank, the heap, CD files, the file loader, pak archives and their decompressor, pads, memory card saves, sound and music, and the flow between the game's screens (`game_flow`, `game_exit`) |
| `src/main/gfx/` | display set-up, VRAM uploads, primitives and their helpers, 3D primitives, transforms, the TMD sorter, the screen fade and copy effects, the scrolling menu background |
| `src/main/model/` | loading models, starting and evaluating their animations, the 3D scene and its camera, the floor grid, the duel stages, effect objects and particles |
| `src/main/ui/` | windows, menus, dialogs, text drawing, string helpers, the hacking screen |
| `src/main/duel/` | starting a duel, its set-up and session, the duel loop, the CPU opponent, each player's card zones, card movement, the battle HUD and its panels |
| `src/main/card/` | the card database and collection, partners and their levels, player profiles and ranks, card rendering |
| `src/main/script/` | the script interpreter |
| `src/main/psyq/` | the PsyQ libraries, one file per library object (`.c`, or `.s` for a hand-written object) |
| `src/<overlay>/` | each overlay, split by subsystem like the executable (`src/openseg/title/`, `src/kawseg/cpu/`, ...) |
| `include/game.h` | types and declarations shared by several modules |
| `include/dcb/` | one header per module (`<module>.h`, `<prefix>_<module>.h`) and one per overlay (`<overlay>.h`) |
| `include/` | common headers, PsyQ and GTE helpers (`gte.h`), assembler macros |
| `config/us/` | the version's splat configs (`main.yaml`, `<overlay>.yaml`), symbols (`symbols.txt`, `symbols_<overlay>.txt`), the PsyQ object list, checksums |
| `mk/version/` | each version's settings for the Makefile: the executable's name, the disc directory, the overlays |
| `tools/` | build helpers, `try_match.py`, `asm_source.py`, `extract_drv.py`, the report generator |
| `asm/<version>/`, `expected/<version>/`, `build/<version>/` | generated; not in git |

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md) for the rules on matching, layout,
names and pull requests.

Some helpers, for the PsyQ functions that are still assembly or for checking a
change to matched code:

- `python3 external/m2c/m2c.py <file>.s` gives a first draft of a function,
  from the `.s` file its `INCLUDE_ASM` line names (its folder is under
  `asm/us/`).
- `tools/try_match.py draft.c [func ...]` compiles a draft with the project's
  compiler and compares each function with the original. It prints both side
  by side when they differ. Set `OVERLAY=<overlay>` for an overlay's
  functions, and add `--psyq`, `--gcc28` or `--nocse` for PsyQ code. It needs
  `asm/us/` from `make regenerate`; `--version` (or `VERSION`) picks another
  version.
- [decomp-permuter](https://github.com/simonlindholm/decomp-permuter) (in
  `external/`) searches for C that matches a near miss.
- objdiff (see [Progress](#progress)) shows the differences per function
  inside the whole module.

The PsyQ functions were named from the
[PsyQ 4.7 signatures](https://github.com/lab313ru/psx_psyq_signatures).

## Links

- [decomp.dev: Digimon Digital Card Battle](https://decomp.dev/juandav/dcb_decomp)
- Inspired by these projects:
  [Digimon World decomp](https://github.com/jype0/dw_decomp),
  [Digimon World 2 decomp](https://github.com/Wyrelade/Digimon-World-2-Decomp).
- Tools: [splat](https://github.com/ethteck/splat),
  [maspsx](https://github.com/mkst/maspsx),
  [objdiff](https://github.com/encounter/objdiff),
  [m2c](https://github.com/matt-kempster/m2c),
  [decomp-permuter](https://github.com/simonlindholm/decomp-permuter).
