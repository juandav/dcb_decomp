# Contributing

The conventions follow the [Digimon World decomp](https://github.com/jype0/dw_decomp),
so that the Digimon decomps read the same way.

## Matching

- A change only counts if `make compare` still matches: the executable has to
  stay byte for byte identical.
- No `NON_MATCHING` code, no inline assembly in place of C, and no tricks that
  wouldn't pass review. A function that doesn't match yet stays behind its
  `INCLUDE_ASM`.
- Code that was written in assembly, not compiled, is kept as assembly
  source instead: a comment at the top of the `.s` says what shows it is
  hand-written (things no compiler emits, such as `$at` used as a
  temporary). A splat segment becomes `hasm` in its config, and splat writes
  `src/<binary>/<name>.s` the first time (`tools/asm_source.py` then drops its
  address comments). A PsyQ object becomes `src/main/psyq/<object>.s` in
  place of `<object>.c`: `tools/asm_source.py src/main/psyq/<object>.c`
  writes it from the object's `INCLUDE_ASM`s, and the build takes the `.s`
  whenever it exists.

## Layout

- One folder per binary under `src/`: `src/main/` for the executable, and
  `src/<overlay>/` for each overlay once they are in the build.
- Inside a binary, the files are grouped by subsystem (`system/`, `gfx/`,
  `model/`, `ui/`, `duel/`, `card/`, `script/`); the SDK is in `psyq/`.
- One header per module in `include/dcb/<module>.h` with its own types, data
  and prototypes. `include/game.h` keeps only what several modules share.
- An overlay's modules are grouped by subsystem too and carry its prefix
  (`src/openseg/title/open_title.c`, `include/dcb/open_title.h`);
  `include/dcb/<overlay>.h` holds what several of them share. The data an overlay starts with zeroed is in
  `<prefix>_bss.c`, a C file with no code; data that sits apart from its
  module's, such as SAISEG's leading tables, goes in one too
  (`sai_data.c`).

## Names

A name has to come from evidence: the strings a function uses, the SDK calls
it makes, its callers, the data it reads. What isn't understood yet keeps its
address (`func_8004635C`, `D_8006E054`, `unk14`).

| What | Style | Examples |
|---|---|---|
| Functions | camelCase, a verb that says what the function does | `loadCardDatabase`, `addCardToCollection`, `renderCardSprite`, `isItemMenuBoxBusy` |
| Globals and tables | UPPER_SNAKE | `DIGIMON_CARDS`, `PARTNER_CARD_IDS`, `PLAYER_PROFILES` |
| Types | PascalCase | `CardSprite`, `PlayerDeck` |
| Struct fields | camelCase, with the offset comment kept | `/* 0x288 */ u8 cardId;` |
| Parameters and locals | camelCase, the same word for the same thing everywhere | `player`, `cardId`, `slot`, `deck` |
| Overlay symbols | the overlay's name in capitals first | `KAW_startDuel`, `KAW_D_801E0000` |
| Strings | `[OVL_]STR_`, `PATH_` or `FMT_`, then the words | `STR_THINKING`, `PATH_DECK2_DEK`, `FMT_SKILL_PATH` |

Verbs: `create*` sets up and opens something, `open*` only opens it, `tick*`
updates it once per frame, `render*`/`draw*` draws it, `init*`, `load*`,
`get*`/`set*`, `is*`/`has*`, `add*`/`remove*`, `count*`, `find*`.

Every renamed symbol also goes in `config/symbols.txt`, so that splat's
disassembly of the original uses the same name and objdiff keeps pairing them.
An overlay function or global that the executable uses by address keeps the
overlay's name there too, but in `config/symbols_overlay_calls.txt`, which only
the executable's splat config reads (another overlay may be loaded at that
address), with its declaration in `include/dcb/overlay_calls.h`.

## Pull requests

- Small and about one thing.
- Matches, renames and moving code around go in separate pull requests:
  decomp.dev shows a renamed or moved function as broken under its old name
  and new under the new one.
- Titles say what the pull request does: "Match X", "Name the card functions
  and globals", "Give the player profile a struct", "Write the window data in C".
- A pull request that renames things lists every rename in a table
  (address, old name, new name).
