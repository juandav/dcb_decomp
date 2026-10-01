#include "common.h"
#include "game.h"
#include "dcb/sugseg.h"

/* the u8 arrays among these are not referenced by any code */
s16 SUG_CURRENT_SCRIPT = 0;
s16 SUG_ACTIVE_MODEL = 0;
s32 SUG_CAMERA_SPIN = 1;
s32 SUG_CAMERA_ORBIT_RANGE = 1;
s32 SUG_CAMERA_YAW_SPIN = 1;
u16 SUG_HUD_TPAGE = 10;
/* four bytes only eu has, not referenced by any code either */
#if VERSION_EU
u8 D_801F19A4[4] = { 0 };
#elif VERSION_US
#else
#error "sugseg/sug_bss: version not checked"
#endif
ClutFade SUG_STAGE_CLUT = { { 0 } };
u8 D_801EF79C[4] = { 0 };
TamEntry SUG_TAM_CACHE[8] = { { 0 } };
s32 SUG_SCREEN_FX_X = 0;
s32 SUG_SCREEN_FX_Y = 0;
float SUG_SCREEN_FX_STEP_X = 0;
float SUG_SCREEN_FX_STEP_Y = 0;
s32 SUG_SCREEN_FX_HOLD = 0;
s32 SUG_SCREEN_FX_FADE_TIME = 0;
s32 SUG_SCREEN_FX_FRAME = 0;
s32 SUG_SCREEN_FX_MOTION = 0;
u8 SUG_SCREEN_FX_BASE_RGB[3] = { 0 };
u8 SUG_SCREEN_FX_RGB_STEP[3] = { 0 };
RootEffect SUG_EFFECT_ROOT = { { 0 } };
u8 D_801EF944[12] = { 0 };
SpriteEntry *SUG_SPRITE_CACHE = NULL;
u8 D_801EF954[4] = { 0 };
BattleState *SUG_BATTLE = NULL;
u8 D_801EF95C[12] = { 0 };
void *SUG_SKILL_SCRIPTS[4] = { NULL };
s32 SUG_SCRIPT_STATES[2] = { 0 };
u8 D_801EF980[4] = { 0 };
s32 SUG_PREV_MODEL = 0;
BattleState SUG_BATTLE_STATE = { { { 0 } } };
s16 SUG_TARGET_HP[2] = { 0 };
