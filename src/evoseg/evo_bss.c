#include "common.h"
#include "game.h"
#include "dcb/menu.h"
#include "dcb/evoseg.h"

/* not referenced by any code; the values are leftovers, not the same in
   every version */
#if VERSION_US
u32 D_801F02BC = 0xF05B2E46;
#elif VERSION_EU
u32 D_801F02BC = 0x0C00A1F2;
#else
#error "evoseg/evo_bss: version not checked"
#endif

s8 EVO_BANNER_FADE = 0;
/* not referenced by any code */
u8 D_801F02C4[4] = { 0 };
u16 EVO_BANNER_CLUT[16] = { 0 };
u8 EVO_BANNER_BRIGHTNESS = 0;
/* not referenced by any code */
#if VERSION_US
u8 D_801F02E9 = 0x23;
u8 D_801F02EA = 0x8F;
u8 D_801F02EB = 0x2B;
u8 D_801F02EC[4] = { 0x74, 0x68, 0x7F, 0xC3 };
#elif VERSION_EU
u8 D_801F02E9 = 0;
u8 D_801F02EA = 0xB2;
u8 D_801F02EB = 0x8F;
u8 D_801F02EC[4] = { 0x5C, 0x00, 0xB1, 0x8F };
#else
#error "evoseg/evo_bss: version not checked"
#endif
EvoSpark EVO_SPARKS[16] = { { { 0 } } };
EvoColor EVO_SHARD_COLOR = { 0 };
SVECTOR *EVO_SHARD_VERTS = 0;
/* not referenced by any code */
u8 D_801F053C[4] = { 0 };
s8 *EVO_SHARD_PRIM = 0;
/* not referenced by any code */
u8 D_801F0544[12] = { 0 };
s16 EVO_SHATTER_ORDER[40] = { 0 };
s16 EVO_SHATTER_TIMERS[42] = { 0 };
s8 EVO_CUTSCENE_STEP = 0;
u16 EVO_SHATTER_DELAY = 0;
s8 EVO_SHATTER_STARTED = 0;
EvoShard *EVO_SHARDS = 0;
EvoShatter EVO_SHATTER = { 0 };
/* not referenced by any code */
u8 D_801F062C[0x2584] = { 0 };
u32 EVO_RAND_SEED_LO = 0;
/* not referenced by any code */
u8 D_801F2BB4[4] = { 0 };
SVECTOR *EVO_SHARD_VERTEX_CURSOR = 0;
SVECTOR *EVO_SHARD_VERTEX_POOL = 0;
/* not referenced by any code */
u8 D_801F2BC0[0x3C] = { 0 };
s16 EVO_CUTSCENE_MODELS[3] = { 0 };
/* not referenced by any code */
u8 D_801F2C04[0x177C] = { 0 };
UiWindow EVO_SORT_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F43C4[12] = { 0 };
UiWindow EVO_CARD_LIST_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F4414[12] = { 0 };
CursorHighlight EVO_SORT_CURSOR = { { { 0 } } };
CursorHighlight EVO_CARD_LIST_CURSOR = { { { 0 } } };
EvoCardInfo *EVO_CARD_LIST[301] = { 0 };
/* not referenced by any code */
u8 D_801F4974[12] = { 0 };
EvoCardInfo *EVO_CARDS_BY_ID[301] = { 0 };
u8 *EVO_SPARE_CARD_COUNTS = 0;
/* not referenced by any code */
u8 D_801F4E38[8] = { 0 };
u8 *EVO_DECK_CARD_COUNTS[3] = { 0 };
u8 EVO_SCRIPT_HALTED = 0;
s32 EVO_NEW_DIGI_PART = 0;
/* not referenced by any code */
u8 D_801F4E54[4] = { 0 };
EvoWindow EVO_WINDOWS[14] = { { { 0 } } };
/* not referenced by any code */
u8 D_801F5248[8] = { 0 };
EvoProgram *EVO_SCRIPT = 0;
/* not referenced by any code */
u8 D_801F5254[4] = { 0 };
EvoText EVO_TEXT_LINES[4] = { { { 0 } } };
u8 EVO_MAX_CARD_LEVEL = 0;
/* not referenced by any code */
u8 D_801F535C[4] = { 0 };
u8 EVO_CARD_RECEIVED = 0;
/* not referenced by any code */
u8 D_801F5364[28] = { 0 };
EvoChoice EVO_TYPE_CHOICE = { { 0 } };
/* not referenced by any code */
u8 D_801F53B4[20] = { 0 };
EvoScreenFlash EVO_SCREEN_FLASH = { { { 0 } } };
/* not referenced by any code */
u8 D_801F540C[4] = { 0 };
UiWindow EVO_RANK_UP_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F5454[4] = { 0 };
s16 EVO_CURSOR_CARD = 0;
/* not referenced by any code */
u8 D_801F545C[28] = { 0 };
EvoFusion EVO_FUSION = { { 0 } };
EvoTray EVO_TRAYS[2] = { { { { { 0 } } } } };
u8 EVO_RANK_UP_STATE = 0;
u8 EVO_LEVEL_UP_PENDING = 0;
/* not referenced by any code */
u8 D_801F57A4[4] = { 0 };
EvoDialog EVO_DIALOG = { { 0 } };
/* not referenced by any code */
u8 D_801F5850[16] = { 0 };
s16 EVO_STAT_BONUSES[4] = { 0 };
EvoFx EVO_EFFECT_ROOT = { { 0 } };
s8 EVO_EFFECT_PLAYER = 0;
s8 EVO_EFFECT_SPRITE_1 = 0;
s8 EVO_EFFECT_SPRITE_2 = 0;
u8 *EVO_EFFECT_ARCHIVE = 0;
