# Contributing

The conventions follow the [Digimon World decomp](https://github.com/jype0/dw_decomp),
so that the Digimon decomps read the same way.

## Matching

- A change only counts if `make compare` still matches: the executable has to
  stay byte for byte identical.
- No `NON_MATCHING` code, no inline assembly in place of C, and no tricks that
  wouldn't pass review. A function that doesn't match yet stays behind its
  `INCLUDE_ASM`.
- A fake match is the last resort, not a shortcut: only for a function that
  natural C has failed to match after a real search (other source shapes,
  types, statement order, the permuter's legitimate finds), a forced form is
  allowed: an empty `do {} while (0)` used as a CSE or loop barrier, a
  variable reused for an unrelated job or a pointer kept in an integer, an
  array of which only one element is used or a local that only shapes the
  stack frame, a condition or statement that exists only for codegen, or what
  the permuter found, cleaned up as far as it still matches. Mark the exact
  spot with a comment that starts with `/* fake match:` and gives, in one or
  two lines, what is forced and why: the compiler decision it reproduces.
  Example: `/* fake match: the empty loop ends a CSE block, so SESSION_DATA's
  address is loaded again, as in the original */`. A forced form without
  its reason doesn't pass review, and the rest of the function stays
  readable C.
- Two lesser workarounds have a standard marker of their own, so that
  `tools/hacks.py` can count them (the README's badge, which the CI keeps
  up to date):
  - a local that nothing reads or writes, kept because the original's
    stack frame has room for it, ends its declaration with exactly
    `/* unused, but it is in the original stack frame */`. Anything more
    to say about it goes in a comment of its own above it. A local that
    makes no difference to the output is deleted instead.
  - C that only matches in one of several equivalent forms (a statement
    macro's `do {} while (0)`, an extra block, an `if` kept on one line or
    without braces, a copy of a variable, a type, each version's own form
    of a loop) has a comment that says the `match depends on` that form,
    and why: `/* kept on one line: the match depends on it, since GCC
    2.8.1's line notes decide where slot*83 is computed */`.

  After adding or removing a fake match or one of these, run
  `tools/hacks.py` and update the README's badge and table to its counts;
  `tools/hacks.py --list` lists them all.
- Code that was written in assembly, not compiled, is kept as assembly
  source instead: a comment at the top of the `.s` says what shows it is
  hand-written (things no compiler emits, such as `$at` used as a
  temporary). A splat segment becomes `hasm` in its config, and splat writes
  `src/<binary>/<name>.s` the first time (`tools/asm_source.py` then drops its
  address comments). A PsyQ object becomes `src/main/psyq/<object>.s` in
  place of `<object>.c`: `tools/asm_source.py src/main/psyq/<object>.c`
  writes it from the object's `INCLUDE_ASM`s, and the build takes the `.s`
  whenever it exists.

## Versions

The same source builds every version of the game (`make VERSION=us`, `jp`,
`eu`). The Makefile passes one `-DVERSION_<VERSION>`, and `include/version.h`
(through `common.h`) makes `VERSION_US`, `VERSION_JP` and `VERSION_EU` all
defined, each 0 or 1; the assembly gets the same names from `--defsym`.

- Test a version with `#if`, never `#ifdef` or `defined()`: `#if VERSION_JP`.
  A misspelt name is then a `-Wundef` warning instead of silently false.
- A condition names the versions it is for: `#if VERSION_US || VERSION_EU`,
  not `#if !VERSION_JP`, so that a version added later doesn't fall into a
  branch nobody checked for it.
- The versions have no order: no `VERSION >= ...` or "newer than" tests.
- Only name a version you have checked. While a module is still asm in some
  version, its blocks name only the versions that build the module from C,
  and end with `#else` + `#error "<module>: version not checked"` when a
  version could fall through. Whoever moves the module to C in that version
  adds it to the branch that matches: the Japanese and European versions are
  both GCC 2.8.1 and often share a form that the USA version doesn't.
- Each version lists the files it builds in `mk/version/<version>.mk`
  (`MAIN_C_SRC`, `<OVERLAY>_C_SRC`, `<BINARY>_HASM_SRC`), and the Makefile
  builds nothing else: a new file goes in the list of every version that
  has it. A file only one version has is listed only there, and a file
  whose contents differ throughout gets one copy per version instead of an
  `#if` around all of it: `<module>_jp.c` next to `<module>.c`, which that
  version's config names as its segment (`effect/sug_model_effect_jp`); the
  report counts it as the same unit (`sugseg/effect/sug_model_effect`).
- Text is written as UTF-8 in the C, Japanese included: the Japanese version
  stores it as Shift JIS, so its `.mk` sets `TEXT_ENCODING := cp932` and
  `tools/sjis_escape.py` re-encodes the non-ASCII characters of the string
  and character literals as octal escapes between cpp and cc1 (GCC would take
  a trail byte of 0x5C for an escape).
- A version that is still splat's assembly is split into us's modules by
  `tools/split_version.py <version>` (after `tools/match_versions.py
  <version>`): each module is an asm segment with its rodata and data,
  under us's name, so the report has the same units. A module becomes C by
  turning those segments into `c`, `.rodata` and `.data` in the version's
  config and listing its file in the version's `.mk`.
- eu's INTSEG and NISSEG aren't European: they are a Japanese debug build,
  linked against an executable that isn't eu's. eu compiles their C as jp's
  (`-UVERSION_EU -DVERSION_JP`, Shift JIS) with `-DJP_DEBUG_BUILD=1`, which
  turns on the debug code (`#if JP_DEBUG_BUILD`, never `VERSION_EU`), and
  links them against that executable's names,
  `config/eu/symbols_<overlay>_exe.txt`, instead of the executable's.

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

Every renamed symbol also goes in `config/us/symbols.txt` (an overlay's in
`config/us/symbols_<overlay>.txt`), so that splat's disassembly of the
original uses the same name and objdiff keeps pairing them.
An overlay function or global that the executable uses by address keeps the
overlay's name there too, but in `config/us/symbols_overlay_calls.txt`, which
only the executable's splat config reads (another overlay may be loaded at
that address), with its declaration in `include/dcb/overlay_calls.h`.

The versions share their names: a function or datum is called the same in
every version, each at its own address. `tools/match_versions.py` pairs the
functions of jp and eu with us's and, with `--seed`, writes the us names of
the confident pairs into `config/<version>/symbols.txt` and
`config/<version>/symbols_<overlay>.txt`; what it can't pair surely keeps
splat's name for now. So renames go through `tools/rename.py OLD NEW`, which
renames in every version's symbol files, in `src/` and in `include/` at once,
keeps an overlay's prefix and refuses a name that already exists. A rename
made by hand has to touch every version the same way. Then
`make VERSION=<version> regenerate` each version. The CI runs
`tools/check_names.py`, which fails when a name in jp's or eu's symbol files
isn't us's name in the same binary any more; a name only one version has
(its own code) says so with `version-only` in its comment. A version that
links into an overlay code us has in the executable (jp's SUGSEG holds
main's `effect_object` and `effect_prims`) gives it us's name in the
overlay's symbol file, with `us-main` in the comment: check_names checks it
against us's executable, and `tools/version_symbols.py` writes it so. Code
us has in another overlay is marked `us-<overlay>` the same way (jp's
ENDSEG holds OPENSEG's movie player: `us-openseg`).

## Pull requests

- Small and about one thing.
- Matches, renames and moving code around go in separate pull requests:
  decomp.dev shows a renamed or moved function as broken under its old name
  and new under the new one.
- Titles say what the pull request does: "Match X", "Name the card functions
  and globals", "Give the player profile a struct", "Write the window data in C".
- A pull request that renames things lists every rename in a table
  (address, old name, new name).
