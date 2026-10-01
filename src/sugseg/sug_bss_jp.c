#include "common.h"
#include "game.h"
#include "dcb/sugseg.h"

/* jp's SUGSEG data that starts zeroed but for a few values (sug_bss.c is us's
   and eu's): jp has no stage clut fade and no screen copy effect, and its
   battle state comes first, set up with a test battle's values. The u8 arrays
   are not referenced by any code */
s16 SUG_CURRENT_SCRIPT = 0;
s16 SUG_ACTIVE_MODEL = 0;
s32 SUG_CAMERA_SPIN = 1;
s32 SUG_CAMERA_ORBIT_RANGE = 1;
s32 SUG_CAMERA_YAW_SPIN = 1;
u16 SUG_HUD_TPAGE = 10;
BattleState SUG_BATTLE_STATE = {
    { { 400, 500, 8 }, { 600, 450, 48 } },
    { { 1 } },
};
u8 D_801FCA3C[4] = { 0xC2, 0x1C, 0x00, 0x6A };
TamEntry SUG_TAM_CACHE[8] = { { 0 } };
RootEffect SUG_EFFECT_ROOT = { { 0 } };
u8 D_801FCBC0[8] = { 0, 0, 0, 0, 0x85, 0xC9, 0x74, 0x43 };
SpriteEntry *SUG_SPRITE_CACHE = NULL;
u8 D_801FCBCC[4] = { 0x8B, 0x50, 0x04, 0x83 };
BattleState *SUG_BATTLE = NULL;
u8 D_801FCBD4[12] = { 0 };
void *SUG_SKILL_SCRIPTS[4] = { NULL };
s32 SUG_SCRIPT_STATES[2] = { 0 };
u8 D_801FCBF8[4] = { 0 };
s32 SUG_PREV_MODEL = 0;
s16 SUG_TARGET_HP[2] = { 0 };
