# TODO

Every version builds byte for byte from this source, code and data. What is
left is polish: C that reads as the original's did, and names for what still
has an address. The counts below were taken from the source and the builds
of `us`, `jp` and `eu`; `tools/hacks.py --list` gives the current list of the
workarounds.

## Fake matches

8 markers in 7 functions, each with a `/* fake match:` comment that says what
is forced and why. None of the comments records a cleaner form that came
close.

- [ ] `loadDuelCardGraphics` (us), `src/main/card/card_render.c`: a
  `CardAnim *nextAnims[3]` of which only `[0]` is used, for the original's
  0x58 frame, where reload left two spill slots. eu has a scalar and an
  unused `char path[16]` instead.
- [ ] `checkSaveIsCurrent` (jp), `src/main/system/memcard_screen.c`:
  `do { found = 1; } while (0)` keeps the store inside the loop's notes, so
  `found` gets s5.
- [ ] `saveBothProfiles` (jp), `src/main/system/memcard_screen.c`: the save's
  offset is built in `save` in two steps, and `save` is reused for the
  address, so global-alloc gives it v1.
- [ ] `OPEN_runCardTrade` (us and eu, for eu),
  `src/openseg/friend/open_trade.c`: `arc` is set to 0 at the top of the
  loop and to `OPEN_CARD_IMAGE_ARC` at its end only to lower loop.c's
  threshold (two markers).
- [ ] `SAI_grantDigiPart` (eu), `src/saiseg/player/sai_digi_parts.c`: the
  test `&& SAI_OWNED_DIGI_PARTS[i] != 0` only keeps the byte's load alive
  in the dead loop; no form shared with us matched.
- [ ] `SAI_findKeyItems` (jp), `src/saiseg/player/sai_player_data_jp.c`: an
  empty `do {} while (0)` ends a CSE block, so `i` is sign-extended again.
- [ ] `SAI_addTextLine` (jp), `src/saiseg/ui/sai_text_jp.c`: an empty
  `do {} while (0)` ends a CSE block, so `SAI_STATE` is loaded again.

## Form-dependent matches

36 comments that say the `match depends on` a form. Each is equivalent C, but
natural C would not always pick it.

- [ ] Closest to fake matches: `EVO_drawTray` (us) adds the index before the
  field offset through a cast; `NIS_takeListCopies` (jp, eu) keeps the
  copies over count in the loop's `i`; `sortEnvMappedPrimitives` (all)
  shares one variable between the texture word and the nclip result.
- [ ] A copy of a variable only for the register allocation:
  `KAW_simulateBattles`, `KAW_chooseDigivolveTarget` and
  `KAW_chooseDigimonToPlace` (two), in every version.
- [ ] An extra `do { ... } while (0)` around statements:
  `SUG_startTexAnim`, `SUG_createTrailEffect` (two) and
  `SUG_runEffectScriptTask`, in every version.
- [ ] A statement macro whose `do {} while (0)` weighs a variable's uses:
  `OPEN_runTitleScreen` (eu), `runMemcardAccess` (jp),
  `SAI_updateOpponentPortraits` (eu), `INT_makeHeroDeck` (jp, eu).
- [ ] One form of the code per version, under `#if`: `removeFrameCallback`,
  `SAI_uploadPakTextures`, `OPEN_runStarterSelect`, `SAI_runPartnerGet`,
  `OPEN_runNameEntry`, `SUG_detachEffectToWorld`, `EVO_typeTextLine`,
  `SAI_typeTextLine`, `KAW_countEarnedBonuses`. A form shared by the
  versions would remove the `#if`.
- [ ] Layout: `updatePartnerStats` (us and eu, for eu: an `if` kept on one
  line), `measureText` (jp, no braces), `runMemcardScreen` (jp, one block
  per mode), `SAI_runBitsReward` (jp, an early return).
- [ ] A type, a local or an expression: `EVO_renderDissolvingObject`
  (`code`'s type per version), `SUG_showAttackBanner` (jp, `-side - 1`),
  `SAI_setSpriteBlendMode` (eu, the mask in a variable), `SUB_drawCardIcon`
  (eu, `u` holds the page first), `NIS_drawDeckChoice1` and
  `NIS_drawVsRecords` (jp, eu, `s16 x` and `y` set at the top),
  `SAI_updateOpponentPortraits` (eu, the height in a local),
  `SUG_createSphereFromParams` (`U16_SHL`, two shifts).

## Unused locals

96 locals that nothing uses but the original's stack frame has room for,
each marked `/* unused, but it is in the original stack frame */`: 94 in the
game, 2 in PsyQ (`GsSetFlatLight`, `GsSetRefView2`). Taking any one of them
out changes at least one version's output. 26 are `char` buffers (mostly the
72-byte `buf` of window draw functions), 25 are `Rect16`, and
NISSEG and SUBSEG have 20 each.

- [ ] Name each one after what it was for, where a sibling function shows
  it (a `sprintf` buffer, a window's rect), instead of `unused`.
- [ ] 13 unused scalar locals make no difference to any version and can be
  deleted: `END_computeEpithet` (`j`), `INT_debugAnimationTask` (`model`),
  `KAW_getSupportOperand` (`n`), `KAW_simulateBattles` (`handCard`),
  `freeHeapBlock` (`blockAddr`), `stepMemoryCardSave` and
  `stepMemoryCardLoad` (`off`), `NIS_runVsMode` (`profiles`),
  `SAI_moveWordInputCursor` (`r`), `SAI_runAreaScript` in
  `sai_panel_jp.c` (`sprite`), `SUB_drawItemDetails` (`n`, `color`),
  `SUG_updateSphereUvs` (`cosine`).
- [ ] 18 locals are declared for every version but used only in some
  (`sug_trail.c` has 10, `anim_control.c` 3, `sug_effect_script.c` 2,
  `kaw_match_intro.c`, `open_trade.c` and `str_util.c` 1 each): declare them
  in the versions that use them.

## Names

Symbols that still have splat's name, counted in the objects linked into
each binary:

| | `us` | `jp` | `eu` |
|---|---|---|---|
| Game functions (`func_`) | 0 | 14: executable 5, SAISEG 6, INTSEG 2, SUBSEG 1 | 2: INTSEG |
| Game data (`D_`) | 156 | 166 (128 in the executable) | 169 |
| PsyQ functions (`func_`) | 173 | 215 | 257 |
| PsyQ data (`D_`) | 517 | 597 | 619 |
| Start-up and soft-float (`func_`) | 0 | 3 | 3 |
| `func_` / `D_` entries in `config/<version>/symbols*.txt` | 0 / 1,073 | 8 / 457 | 9 / 1,370 |

- [ ] Name the game functions and data above. eu's INTSEG and NISSEG also
  use 7 functions and 9 data of the debug executable they were linked
  against that are still named by address (`config/eu/symbols_*_exe.txt`).
- [ ] Most `D_` entries of the overlay symbol files name nothing in the
  build any more: us 854, jp 298, eu 1,043. In us and eu they are mostly
  strings that the C writes as literals, in jp mostly words; splat still
  splits `expected/` at them. Check whether objdiff's data needs them, then
  drop them or give them names.
- [ ] 405 `unk` struct fields in the game's headers (`include/game.h` 105,
  `sugseg.h` 59, `saiseg.h` 53, `nisseg.h` 48, `scroll_bg.h` 24,
  `kawseg.h` 21, `subseg.h` 20, `intseg.h` 18, `openseg.h` 15,
  `battle_hud.h` 11, the rest under 10), 65 more in structs defined in `.c`
  files, and 116 in `include/psyq.h`. The game's C reads or writes `unk`
  fields 532 times, most of them in jp's own files (`sai_panel_jp.c` 85,
  `duel_jp.c` 53, `tutorial_jp.c` 52, `sai_area_jp.c` 39,
  `sai_opponent_select_jp.c` 37).

## PsyQ

- [ ] 71 functions in us are still `INCLUDE_ASM`, in 21 objects
  (`libgte_fgo_00` 18, `libc2_setjmp` 14, `libgpu_sys_3` and `libapi_pad` 5
  each, `libcd_bios_1` 4, the rest 3 or fewer); 25 of them are still
  `func_`. The README calls them mostly BIOS call stubs and register-level
  code, plus a few near misses.
- [ ] In jp and eu, the libraries, the start-up code and the soft-float
  routines are still splat's disassembly (the `asm` segments of
  `config/<version>/main.yaml`); us has them as C and `.s` in
  `src/main/psyq/`, `src/main/startup.s` and `src/main/libmath.s`.
- [ ] 15 PsyQ objects (18 statements) and the game's
  `src/main/duel/card_motion.c` pad their `.rodata` with a top-level
  `__asm__` (`tools/hacks.py --list` lists them); C data would say the
  same.

## Readability

- [ ] Globals declared looser than they are used, so the C casts them each
  time: `extern s32 PLAYER_PROFILES` (291 `(PlayerProfile *)` casts, plus
  `ProfileK` 21, `NisProfile` 9, `IntProfile` 7), `extern void *SESSION_DATA`
  (179 `(SessionData *)`, `SessionView` 23, `SaisegSessionData` 2),
  `u8 *DIGIMON_CARDS` (49), `OPTION_CARDS` (20), `DIGIVOLVE_CARDS` (18),
  `HUD_PANELS` (17). The several views of one struct (the profiles, the
  session data) could become one type.
- [ ] Numbers that want names: the sound of 526 `playSoundEffect(n)` and
  170 `playMenuSound(n)` calls; the task id, place, priority and stack size
  of 492 `spawnTask(0, -1, 0, <stack size>, ...)` calls; the heap tag of 63
  `freeHeapBlocksByTag(n)` calls.
- [ ] 198 declarations with an empty parameter list outside PsyQ (165
  functions, such as `s32 spawnTask();` in `include/game.h`). A prototype
  can change a call's code, so each needs a `make compare`.
- [ ] Offsets by hand instead of fields: 8 `*(T *)((u8 *)p + n)` accesses
  (3 in `sug_effect_script.c`) and 11 `(u8 *)p + 0x...`.
- [ ] An m2c comment left in `ensureMemoryCardReady`
  (`src/main/system/memcard.c`): "Duplicate return node #9. Try simplifying
  control flow for better match".

## Tooling and docs

- [ ] The Docker workflow (`.github/workflows/docker.yaml`) builds and
  compares only us.
- [ ] The README's status table (functions per version) is written by hand;
  the CI checks the hacks badge, but not it.
- [ ] Pull requests from forks only run the `names` job: the builds need the
  private repository with the original files.
- [ ] `objdiff.json` holds one version at a time: the last one `make
  objdiff` was run for.
