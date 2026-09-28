# Digimon Digital Card Battle decomp

[![Code](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/juandav/dcb_decomp)
[![Data](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=data&label=Data)](https://decomp.dev/juandav/dcb_decomp)

A work in progress matching decompilation of Digimon Digital Card Battle for
the PlayStation: C source that compiles back into a byte-identical copy of the
game's executable.

This repository does not contain any game data. You need your own copy of the
game to build it.

| | |
|---|---|
| Version | USA (`SLUS-01328`) |
| Main executable | `SLUS_013.28`, SHA-1 `fa4e03b5e0671dce399a35f2747080ced492d2c3` |
| Game code | GCC 2.95.2 (`-O1 -G0`) + ASPSX 2.86, emulated with [maspsx](https://github.com/mkst/maspsx) |
| SDK | PsyQ 4.7 (GCC 2.7.2 `-O2`, some objects SN GCC 2.8.1) |

## Status

- The game code in `SLUS_013.28` is split into one C file per module. A
  function that is not decompiled yet stays in its file as an `INCLUDE_ASM`
  line that pulls in the original assembly.
- The game's `.rodata` and `.data` are written in C, in the module that defines
  them. The `.bss` and the startup code (`startup`, hand-written assembly)
  still come from splat.
- The PsyQ 4.7 libraries linked into the executable are decompiled too, one
  file per library object. They need a binary-patched GCC 2.7.2
  (`tools/patch_cc1.py`) and, for some objects, a patched SN GCC 2.8.1
  (`tools/sn_cc1.py`); `config/psyq_objects.txt` says which. Like the
  soft-float library (`libmath.c`), they are not the game's code and are left
  out of the progress.
- Progress is measured by objdiff with one unit per game module and tracked on
  [decomp.dev](https://decomp.dev/juandav/dcb_decomp).
- The game loads overlays from `P.DRV` (`ENDSEG`, `EVOSEG`, `KAWSEG`,
  `OPENSEG`, `SAISEG`, `SUBSEG`, `SUGSEG`) at the end of the executable's
  `.bss`. `ENDSEG` (the ending) is in the build: `tools/extract_drv.py` takes
  it out of `P.DRV`, `config/endseg.yaml` splits it, `src/endseg/` holds its C,
  and `make compare` checks it too. The others will follow the same way.

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

Download the prebuilt tools into `bin/` (GCC 2.95.2, 2.7.2 and 2.8.1 for PSX,
objdiff-cli and mkpsxiso), checked against `tools/deps.sha256`:
```
tools/dl_deps.sh
```

## Getting the executable

Only `SLUS_013.28` is needed, at `disks/us/SLUS_013.28`. To extract the whole
disc with mkpsxiso's `dumpsxiso`:
```
bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/Digimon - Digital Card Battle (USA).bin"
sha1sum disks/us/SLUS_013.28   # fa4e03b5e0671dce399a35f2747080ced492d2c3
```

## Build

```
# Split the original executable with splat (writes asm/ and build/generated/)
make regenerate

# Build build/SLUS_013.28
make -j$(nproc)

# Check it against the original
make compare
```

`make compare` must print `build/SLUS_013.28: OK`. A function only counts as
decompiled once the whole executable still matches.

`make regenerate` deletes `asm/`, `build/` and `expected/` and splits the
executable again. Run it again after changing `config/main.yaml`, so that no
stale files stay behind in `asm/`.

To use a different binutils or objdiff, create `local.mk`:
```
TOOLCHAIN := /path/to/mipsel-linux-gnu-
OBJDIFF := /path/to/objdiff-cli
```

## Progress

```
# Write objdiff.json and the target objects in expected/
make objdiff

# Write build/report.json
make report
```

After `make objdiff`, open the repository in the
[objdiff](https://github.com/encounter/objdiff) GUI to see each module's
functions and data against the original. `tools/objdiff_generate.py` makes one
unit per game module (`main/<subsystem>/<module>`) and leaves `psyq` and
`libmath` out.

The CI (`.github/workflows/build.yaml`) builds every push, runs
`make compare` and `make report`, and uploads the report that decomp.dev reads.
The original executable comes from a private repository, so pull requests from
forks are not built.

## Layout

| Path | Contents |
|---|---|
| `src/main/` | the executable: `main.c`, `libmath.c` (soft-float) and one folder per subsystem |
| `src/main/system/` | start-up (`boot`), the task scheduler (`task`), the render loop and vblank, the heap, CD files, the file loader, pak archives and their decompressor, pads, memory card saves, sound and music, and the flow between the game's screens (`game_flow`, `game_exit`) |
| `src/main/gfx/` | display set-up, VRAM uploads, primitives and their helpers, 3D primitives, transforms, the TMD sorter, the screen fade and copy effects, the scrolling menu background |
| `src/main/model/` | loading models, starting and evaluating their animations, the 3D scene and its camera, the floor grid, the duel stages, effect objects and particles |
| `src/main/ui/` | windows, menus, dialogs, text drawing, string helpers, the hacking screen |
| `src/main/duel/` | starting a duel, its set-up and session, the duel loop, the CPU opponent, each player's card zones, card movement, the battle HUD and its panels |
| `src/main/card/` | the card database and collection, partners and their levels, player profiles and ranks, card rendering |
| `src/main/script/` | the script interpreter |
| `src/main/psyq/` | the PsyQ libraries, one file per library object |
| `include/game.h` | types and declarations shared by several modules |
| `include/dcb/` | one header per module: its own types, data and functions |
| `include/` | common headers, PsyQ and GTE helpers, assembler macros |
| `config/main.yaml` | splat config: where each module's code, rodata and data start |
| `config/symbols.txt` | known symbols |
| `config/psyq_objects.txt` | PsyQ objects in link order, with the compiler each needs |
| `tools/` | build helpers and `try_match.py` |
| `asm/`, `expected/`, `build/` | generated; not in git |

Memory map of `SLUS_013.28` (psylink puts `.rodata` in front of `.text`):

| Section | Address |
|---|---|
| `.rodata` | `0x80010000`-`0x80013E4C` |
| `.text` | `0x80013E4C`-`0x8006DD3C` (game `0x80013E4C`-`0x8004A610`, PsyQ after) |
| `.data` | `0x8006DD3C`-`0x80077A08` |
| `.bss` | `0x80077A08`-`0x801DDF38` |

## Contributing

Read [CONTRIBUTING.md](CONTRIBUTING.md) for the rules on matching, layout,
names and pull requests.

To decompile a function, replace its `INCLUDE_ASM` line with C, rebuild and
run `make compare`. Some helpers:

- `python3 external/m2c/m2c.py <file>.s` gives a first draft of a function,
  from the `.s` file its `INCLUDE_ASM` line names
  (`asm/main/nonmatchings/<subsystem>/<module>/<func>.s`).
- `tools/try_match.py draft.c [func ...]` compiles a draft with the project's
  compiler and compares each function with the original, printing both side
  by side when they differ. Add `--psyq`, `--gcc28` or `--nocse` for PsyQ
  code. It needs `asm/` from `make regenerate`.
- objdiff (see [Progress](#progress)) shows the differences per function
  inside the whole module.

Rodata is migrated into the functions that use it: a function's jump tables and
strings live in its own `.s` file, so its C version emits them itself. Rodata
shared by several functions stays behind `INCLUDE_RODATA`.

The PsyQ functions were named from the
[PsyQ 4.7 signatures](https://github.com/lab313ru/psx_psyq_signatures).

When the overlays join the build, each will get its own `src/<overlay>/`
folder.

## Links

Inspired by these projects:
[Digimon World decomp](https://github.com/jype0/dw_decomp),
[Digimon World 2 decomp](https://github.com/Wyrelade/Digimon-World-2-Decomp).
