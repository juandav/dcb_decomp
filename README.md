# Digimon Digital Card Battle decomp

| Version | Code | Data | Functions |
|---|---|---|---|
| 🇺🇸 USA (`SLUS_013.28`) | [![Code](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&version=SLUS_013.28&label=Code)](https://decomp.dev/juandav/dcb_decomp/SLUS_013.28) | [![Data](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=data&version=SLUS_013.28&label=Data)](https://decomp.dev/juandav/dcb_decomp/SLUS_013.28) | [![Functions](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=functions&version=SLUS_013.28&label=Functions)](https://decomp.dev/juandav/dcb_decomp/SLUS_013.28) |
| 🇯🇵 Japan (`SLPS_025.06`) | [![Code](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&version=SLPS_025.06&label=Code)](https://decomp.dev/juandav/dcb_decomp/SLPS_025.06) | [![Data](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=data&version=SLPS_025.06&label=Data)](https://decomp.dev/juandav/dcb_decomp/SLPS_025.06) | [![Functions](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=functions&version=SLPS_025.06&label=Functions)](https://decomp.dev/juandav/dcb_decomp/SLPS_025.06) |
| 🇪🇺 Europe (`SLES_039.00`) | [![Code](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&version=SLES_039.00&label=Code)](https://decomp.dev/juandav/dcb_decomp/SLES_039.00) | [![Data](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=data&version=SLES_039.00&label=Data)](https://decomp.dev/juandav/dcb_decomp/SLES_039.00) | [![Functions](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=functions&version=SLES_039.00&label=Functions)](https://decomp.dev/juandav/dcb_decomp/SLES_039.00) |

[![Executable](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&version=SLUS_013.28&category=executable&label=USA%20executable)](https://decomp.dev/juandav/dcb_decomp/SLUS_013.28?category=executable)
[![Overlays](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&version=SLUS_013.28&category=overlays&label=USA%20overlays)](https://decomp.dev/juandav/dcb_decomp/SLUS_013.28?category=overlays)

[![Build](https://github.com/juandav/dcb_decomp/actions/workflows/build.yaml/badge.svg)](https://github.com/juandav/dcb_decomp/actions/workflows/build.yaml)
[![Platform](https://img.shields.io/badge/platform-PlayStation-003791)](#the-games-binaries)
[![Versions](https://img.shields.io/badge/versions-USA%20%7C%20Japan%20%7C%20Europe-blue)](#how-the-versions-are-organised)
[![Compiler](https://img.shields.io/badge/compiler-GCC%202.95.2%20%7C%202.8.1-orange)](#toolchain)
[![License](https://img.shields.io/github/license/juandav/dcb_decomp)](LICENSE)

A matching decompilation of **Digimon Digital Card Battle** for the
PlayStation: C source that compiles back into byte-identical copies of the
game's executable and of its overlays. The USA, Japanese and European
releases are all fully matched, code and data, from the same source tree.

This repository does not contain any game data. You need your own copy of the
game to build it.

[![Progress map](https://decomp.dev/juandav/dcb_decomp.png)](https://decomp.dev/juandav/dcb_decomp)

<sub>Each rectangle is a module, sized by its code; green means it matches.
Click it for the details on decomp.dev.</sub>

## Status

Every version is at 100 % of its code and 100 % of its data:

| Version | Executable | Overlays | Game functions | Code | Data |
|---|---|---|---|---|---|
| `us` (`SLUS_013.28`) | 506 / 506 | 864 / 864 | **1,370 / 1,370** | 100 % | 100 % |
| `jp` (`SLPS_025.06`) | 531 / 531 | 660 / 660 | **1,191 / 1,191** | 100 % | 100 % |
| `eu` (`SLES_039.00`) | 506 / 506 | 1,092 / 1,092 | **1,598 / 1,598** | 100 % | 100 % |

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
  strings they use, the SDK calls they make, and the data they touch. The
  versions share their names.
- The PsyQ 4.7 libraries linked into the USA executable are decompiled too,
  one file per library object. They are not the game's code, so they are left
  out of the progress. The 71 PsyQ functions still in assembly are mostly BIOS
  call stubs and register-level code, plus a few near misses. In `jp` and `eu`
  the libraries, the start-up code and the soft-float routines are still
  splat's disassembly.

Progress is measured by [objdiff](https://github.com/encounter/objdiff) with one
unit per module. It is tracked on
[decomp.dev](https://decomp.dev/juandav/dcb_decomp), which comments on every
pull request with what it changes.

### Fake matches

A handful of functions only match through a forced form, such as an empty
`do {} while (0)` that ends a CSE block or a variable that exists only to
shape the code. Each spot is marked with a comment that starts with
`/* fake match:` and says what is forced and why:
```
grep -rn "fake match:" src/
```
[CONTRIBUTING.md](CONTRIBUTING.md#matching) has the rules for them. Work to
replace them with natural C is ongoing: a pull request that matches one of
them without the forced form is welcome.

## The game's binaries

The executable loads one overlay at a time from `P.DRV`, at the end of its own
`.bss` (`OVERLAY_AREA`, `0x801DDF38` in `us`). Each overlay has its own splat
config, source folder and symbol prefix. The USA release's:

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

The Japanese release has two overlays that the USA one doesn't, and no
`OPENSEG` or `EVOSEG`. The European release has all nine, its `INTSEG` and
`NISSEG` from a Japanese debug build
([How the versions are organised](#how-the-versions-are-organised)):

| Binary | Prefix | Source | What it runs |
|---|---|---|---|
| `INTSEG` | `INT_` | `src/intseg/` | jp's new-game intro, where Babamon asks for the player's name and birthday and hands over a starter deck |
| `NISSEG` | `NIS_` | `src/nisseg/` | jp's title screen, card trade, VS mode and deck screens |

Memory map of `SLUS_013.28` (psylink puts `.rodata` in front of `.text`):

| Section | Address |
|---|---|
| `.rodata` | `0x80010000`-`0x80013E4C` |
| `.text` | `0x80013E4C`-`0x8006DD3C` (game `0x80013E4C`-`0x8004A610`, PsyQ after) |
| `.data` | `0x8006DD3C`-`0x80077A08` |
| `.bss` | `0x80077A08`-`0x801DDF38` |

## How the versions are organised

One source tree builds every version, one at a time, picked with `VERSION`
(`us` by default):

| `VERSION` | Release | Executable (SHA-1) | Overlays in `P.DRV` | Game code |
|---|---|---|---|---|
| `us` | USA, SLUS-01328 | `SLUS_013.28` (`fa4e03b5e0671dce399a35f2747080ced492d2c3`) | `ENDSEG` `EVOSEG` `KAWSEG` `OPENSEG` `SAISEG` `SUBSEG` `SUGSEG` | GCC 2.95.2 |
| `jp` | Japan, SLPS-02506 | `SLPS_025.06` (`447c5256757685728bfbecaf3b3b2e7c0164b6cc`) | `ENDSEG` `INTSEG` `KAWSEG` `NISSEG` `SAISEG` `SUBSEG` `SUGSEG` | GCC 2.8.1, `--expand-div` |
| `eu` | Europe, SLES-03900 | `SLES_039.00` (`050ae623135d5c233c255c9e76956eaeabd2fa6e`) | `ENDSEG` `INTSEG` `KAWSEG` `NISSEG` `SAISEG` `SUBSEG` `SUGSEG` `VSSVER` `OPENSEG` `EVOSEG` | GCC 2.8.1 |

- `mk/version/<version>.mk` has each version's settings: the executable's
  name, the disc directory, the overlays, the compiler and the list of source
  files it builds. The Makefile builds nothing else.
- `config/<version>/` has its splat configs, symbols and checksums; the
  generated `asm/<version>/`, `build/<version>/`, `expected/<version>/` and
  `assets/<version>/` are kept apart too.
- The C sees `VERSION_US`, `VERSION_JP` and `VERSION_EU`, each 0 or 1
  (`include/version.h`), and so does the assembly. Small differences are
  `#if` blocks; a file whose contents differ throughout has one copy per
  version instead (`<module>_jp.c` next to `<module>.c`), which the report
  counts as the same unit. CONTRIBUTING.md has the rules.
- `jp` writes its text as UTF-8 in the C, like the other versions, and
  `tools/sjis_escape.py` re-encodes it as Shift JIS before cc1. Its code is
  assembled with ASPSX's expanded divisions (`--expand-div`). It draws its
  text with a kanji font of its own, 3,489 glyphs of 12x11 pixels in its
  executable: `make VERSION=jp generate` cuts it out of the disc as PNG
  sheets in `assets/jp/` (`tools/font.py`, `config/jp/fonts.txt`), and the
  build turns the sheets back into the C that `src/main/ui/str_util.c`
  includes. An edited sheet goes into the build as it is.
- `eu`'s `INTSEG` and `NISSEG` are not European: they are a Japanese debug
  build, linked against an executable that isn't `eu`'s. `eu` compiles their
  C as `jp`'s (`-UVERSION_EU -DVERSION_JP`, Shift JIS) with `JP_DEBUG_BUILD`,
  which turns on their debug code, and links them against that executable's
  names (`config/eu/symbols_<overlay>_exe.txt`). `VSSVER` isn't code but a
  Visual SourceSafe file that ended up in `P.DRV`; it stays one blob.
- The versions share their names: a function or global is called the same
  in every version, each at its own address (`tools/rename.py`,
  `tools/check_names.py`).

## Toolchain

| | |
|---|---|
| Game code, `us` | GCC 2.95.2 (`-O1 -G0`) + ASPSX 2.86, emulated with [maspsx](https://github.com/mkst/maspsx) |
| Game code, `jp` and `eu` | GCC 2.8.1 (`-O1 -G0`) + ASPSX 2.86; `jp`'s code and `eu`'s `NISSEG` with divisions expanded (maspsx `--expand-div`) |
| SDK | PsyQ 4.7 in `us` (GCC 2.7.2 `-O2`, some objects SN GCC 2.8.1) |
| Splitting | [splat](https://github.com/ethteck/splat) |
| Diffing | [objdiff](https://github.com/encounter/objdiff) 3.8.1, [decomp.dev](https://decomp.dev) |

The GCC 2.8.1 of `jp` and `eu` is PsyQ 4.4's own `CC1PSX.EXE` (GCC 2.8.1, SN32
build 4.0.0010): it compiles every object to the same bytes as the
`bin/gcc-2.8.1-psx` the build uses.

The PsyQ objects need a binary-patched GCC 2.7.2 (`tools/patch_cc1.py`) and,
for some objects, a patched SN GCC 2.8.1 (`tools/sn_cc1.py`).
`config/us/psyq_objects.txt` says which compiler each object needs.

## Building

### Prerequisites

On Debian or Ubuntu (the CI uses Ubuntu 24.04 and Python 3.12), install:
```
binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu git make python3 python3-venv unzip wget
```
Or skip this section and build in Docker ([below](#building-with-docker)).

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

### Getting the game files

Each disc goes in its own `disks/<version>/`. Extract it with mkpsxiso's
`dumpsxiso` and check the files the build reads, the executable and
`P.DRV`, which holds the overlays:
```
bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/the USA disc.bin"
sha1sum disks/us/SLUS_013.28   # fa4e03b5e0671dce399a35f2747080ced492d2c3
sha1sum disks/us/P.DRV         # c0fecd80c3b9160e72aacd5dcada21f6f7fdc343

bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/jp -s disks/jp/jp.xml "/path/to/the Japanese disc.bin"
sha1sum disks/jp/SLPS_025.06   # 447c5256757685728bfbecaf3b3b2e7c0164b6cc
sha1sum disks/jp/P.DRV         # 5bf01a68b32fe474e6c28bd6fbb821e7cc82ad10

bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/eu -s disks/eu/eu.xml "/path/to/the European disc.bin"
sha1sum disks/eu/SLES_039.00   # 050ae623135d5c233c255c9e76956eaeabd2fa6e
sha1sum disks/eu/P.DRV         # a6f3c764bddd6f5ceeb35b22dc6cc52981113157
```

Only `disks/<version>/<executable>` and `disks/<version>/P.DRV` are needed;
you only need the discs of the versions you build. `tools/extract_drv.py`
takes each overlay out of `P.DRV`, and `config/<version>/overlays.sha1`
holds their checksums.

### Build

The same three steps build each version, with `VERSION` set to `us`, `jp`
or `eu` (`us` when it is left out):
```
# Split the executable and the overlays with splat (asm/<version>/,
# build/<version>/generated/; for jp, also the font sheets in assets/jp/)
make VERSION=us generate

# Build build/<version>/<executable> and build/<version>/<OVERLAY>.BIN
make VERSION=us -j$(nproc)

# Check the executable and every overlay against the originals
make VERSION=us compare
```

`make compare` prints one `OK` per binary, and a change only counts once
every line still says `OK`. For `us`:
```
build/us/SLUS_013.28: OK
build/us/ENDSEG.BIN: OK
build/us/EVOSEG.BIN: OK
build/us/KAWSEG.BIN: OK
build/us/OPENSEG.BIN: OK
build/us/SAISEG.BIN: OK
build/us/SUBSEG.BIN: OK
build/us/SUGSEG.BIN: OK
```
`jp` checks its executable and its 7 overlays, `eu` its executable and its
10 overlays. The CI builds and compares all three versions on every push.

`make VERSION=<version> regenerate` deletes `asm/<version>/`,
`build/<version>/` (and the patched compilers in `build/tools/`),
`expected/<version>/` and `assets/<version>/`, and splits the binaries again.
Run it after changing a `config/<version>/*.yaml`, so that no stale files stay
behind in `asm/<version>/`.

To use a different binutils or objdiff, create `local.mk`:
```
TOOLCHAIN := /path/to/mipsel-linux-gnu-
OBJDIFF := /path/to/objdiff-cli
```

### Building with Docker

The `Dockerfile` gives the same environment as the CI: Ubuntu 24.04 with the
MIPS binutils and cpp, Python 3.12 with `requirements.txt`, and the tools of
`tools/dl_deps.sh` (in the image's `/opt/dcb/bin`, which it sets `BIN_DIR` to).
The image holds no game data: `tools/docker.sh` builds it (quickly, once
Docker has it cached) and runs a command in it with your clone, `disks/`
included, mounted at `/dcb`, as your own user. Clone with the submodules and
put the disc files in `disks/` as above, then:
```
tools/docker.sh make VERSION=us generate
tools/docker.sh make VERSION=us -j$(nproc)
tools/docker.sh make VERSION=us compare

# a shell in the container
tools/docker.sh
```
The prebuilt tools are x86 Linux binaries, so the image is `linux/amd64`.
`dumpsxiso` is in the image too, as
`/opt/dcb/bin/mkpsxiso-2.20-Linux/bin/dumpsxiso`, for a disc image inside
your clone.

## Progress

```
# Write objdiff.json and the target objects in expected/<version>/
make VERSION=us objdiff

# Write build/<version>/report.json
make VERSION=us report
```

After `make objdiff`, open the repository in the
[objdiff](https://github.com/encounter/objdiff) GUI to see each module's
functions and data against the original. `tools/objdiff_generate.py` makes one
unit per module (`main/<subsystem>/<module>`, `<overlay>/<subsystem>/<module>`)
and sorts them into the `executable` and `overlays` categories. It leaves
`psyq` and `libmath` out. `objdiff.json` is for the version it was last
written for.

The CI (`.github/workflows/build.yaml`) builds each version on every push,
runs `make compare` and `make report`, and uploads the reports that
decomp.dev reads. The original
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
| `config/<version>/` | the version's splat configs (`main.yaml`, `<overlay>.yaml`), symbols (`symbols.txt`, `symbols_<overlay>.txt`), checksums; `us`'s PsyQ object list, `jp`'s font tables |
| `mk/version/` | each version's settings for the Makefile: the executable's name, the disc directory, the overlays, the compiler, the source files |
| `tools/` | build helpers, `try_match.py`, `asm_source.py`, `extract_drv.py`, the report generator |
| `Dockerfile`, `tools/docker.sh` | the build environment as a Docker image, and the script that runs a command in it |
| `asm/<version>/`, `expected/<version>/`, `build/<version>/` | generated; not in git |
| `assets/<version>/` | the version's graphics as PNG (`jp`'s font), extracted from the disc by `make generate`; not in git |

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md) for the rules on matching, layout,
names and pull requests.

Some helpers, for the PsyQ functions that are still assembly or for checking a
change to matched code:

- `python3 external/m2c/m2c.py <file>.s` gives a first draft of a function,
  from the `.s` file its `INCLUDE_ASM` line names (its folder is under
  `asm/<version>/`).
- `tools/try_match.py draft.c [func ...]` compiles a draft with the project's
  compiler and compares each function with the original. It prints both side
  by side when they differ. Set `OVERLAY=<overlay>` for an overlay's
  functions, and add `--psyq`, `--gcc28` or `--nocse` for PsyQ code. It needs
  `asm/<version>/` from `make generate`; `--version` (or `VERSION`) picks the
  version, `us` by default.
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
  [decomp-permuter](https://github.com/simonlindholm/decomp-permuter),
  [old-gcc](https://github.com/decompals/old-gcc) (the PSX GCC builds),
  [mkpsxiso](https://github.com/Lameguy64/mkpsxiso),
  [psyq_headers](https://github.com/jype0/psyq_headers).
