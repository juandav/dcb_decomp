# Digimon Digital Card Battle decomp

[![Code](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=code&label=Code)](https://decomp.dev/juandav/dcb_decomp)
[![Data](https://decomp.dev/juandav/dcb_decomp.svg?mode=shield&measure=data&label=Data)](https://decomp.dev/juandav/dcb_decomp)

A work in progress matching decompilation of Digimon Digital Card Battle for
the PlayStation.

This repository does not contain any game data. You need your own dump of the
game to build it.

| | |
|---|---|
| Version | USA (`SLUS-01328`) |
| Main executable | `SLUS_013.28`, SHA-1 `fa4e03b5e0671dce399a35f2747080ced492d2c3` |
| Compiler | GCC 2.95.2 (`-O1 -G0`) + ASPSX 2.86, emulated with [maspsx](https://github.com/mkst/maspsx) |
| SDK | PsyQ 4.7 |

## Dependencies

Install the following packages:
```
binutils-mipsel-linux-gnu gcc-mipsel-linux-gnu git make python3 python3-venv unzip wget
```

Install Python dependencies:
```
python3 -m venv .venv
. .venv/bin/activate
pip3 install -r requirements.txt
```

Download tools (GCC 2.95.2 for PSX, objdiff-cli and mkpsxiso):
```
tools/dl_deps.sh
```

## Build

```
# Update submodules
git submodule update --init --recursive

# Dump original PSX Digimon Digital Card Battle (USA) ISO
bin/mkpsxiso-2.20-Linux/bin/dumpsxiso -x disks/us -s disks/us/us.xml "/path/to/Digimon - Digital Card Battle (USA).bin"

# Disassemble the original executable
make regenerate

# (Optional) Create file local.mk to override defaults
TOOLCHAIN := /path/to/mipsel-linux-gnu-

# Build the executable
make -j$(nproc)

# Compare it with the original
make compare

# Generate the objdiff config and the progress report
make objdiff
make report
```

`make compare` must print `build/SLUS_013.28: OK`. A function only counts as
decompiled once the whole executable still matches.

## Layout

| Path | Contents |
|---|---|
| `config/main.yaml` | splat config for `SLUS_013.28` |
| `config/symbols.txt` | known symbols |
| `src/main/game.c` | game code, `0x80013E4C`-`0x8004A610` |
| `src/main/psyq.c` | PsyQ libraries, `0x8004A610`-`0x8006DD3C` |
| `include/` | headers and assembler macros |
| `tools/` | build helpers |

Memory map of `SLUS_013.28` (psylink puts `.rodata` in front of `.text`):

| Section | Address |
|---|---|
| `.rodata` | `0x80010000`-`0x80013E4C` |
| `.text` | `0x80013E4C`-`0x8006DD3C` |
| `.data` | `0x8006DD3C`-`0x80077A08` |
| `.bss` | `0x80077A08`-`0x801DDF38` |

## Contributing

Every function starts as an `INCLUDE_ASM` line in `src/`. To decompile one,
replace that line with C, rebuild and run `make compare`. objdiff
(`make objdiff`, then open the project in objdiff) shows the differences per
function.

Rodata is migrated into the functions that use it: a function's jump tables and
strings live in its own `.s` file, so its C version emits them itself (switch
statements work as usual). Rodata shared by several functions stays behind
`INCLUDE_RODATA`.

`tools/try_match.py draft.c [func ...]` compiles a draft with the project
toolchain and compares each function with the original executable, printing
both side by side when they differ.

### Where to start

- `src/main/game.c` is a single file for now. splat reports likely file
  boundaries from the jump tables in `.rodata` (at `0x84C`, `0x860`, `0xF94`,
  `0x10B0`, `0x18C4`, `0x1F38`, `0x2F4C` and `0x3618`), which are a good first
  hint to split it into the original source files.
- The PsyQ functions were named from the
  [PsyQ 4.7 signatures](https://github.com/lab313ru/psx_psyq_signatures).
  They can be split into one file per library object in the same way.
- The game loads overlays from `P.DRV` (`ENDSEG`, `EVOSEG`, ...) above
  `0x801DF000`; they are not part of the build yet.

## Links

Inspired by these projects:
[Digimon World decomp](https://github.com/jype0/dw_decomp),
[Digimon World 2 decomp](https://github.com/Wyrelade/Digimon-World-2-Decomp).
