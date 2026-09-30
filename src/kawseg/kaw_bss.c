#include "common.h"
#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/menu.h"
#include "dcb/kawseg.h"

/* the u8 arrays among these are not referenced by any code */
u8 D_801FC400[4] = { 0 };
s32 KAW_SUPPORT_REGISTER = 0;
u8 D_801FC408[8] = { 0 };
UiWindow KAW_TUTORIAL_WINDOW = { 0 };
DeckScreen *KAW_MATCH_SCREEN = NULL;
s32 KAW_MATCH_LOADING = 0;
u8 D_801FC45C[8] = { 0 };
POLY_G4 KAW_DECK_CHART_POLYS[2][2][3] = { { { { 0 } } } };
TILE KAW_DECK_LEVEL_BARS[2][2][4] = { { { { 0 } } } };
DR_MODE KAW_DECK_CHART_MODES[2][2] = { { { 0 } } };
s32 KAW_RESULT_SCREEN_STATE = 0;
ExpScreen *KAW_EXP_SCREEN = NULL;
PrizeScreen *KAW_PRIZE_SCREEN = NULL;
EffectObject KAW_EFFECT_ROOT = { { { { 0 } } } };
s8 KAW_EFFECT_PLAYER = 0;
s8 KAW_EFFECT_CARD = 0;
s8 KAW_EFFECT_TARGET_CARD = 0;
u8 D_801FC880[4] = { 0 };
UiWindow KAW_DUEL_MENU_WINDOW = { 0 };
u8 D_801FC8C8[12] = { 0 };
CursorHighlight KAW_DUEL_MENU_CURSOR = { { { 0 } } };
DialogK KAW_MENU_DIALOG = { { 0 } };
u8 D_801FC9CC[0x18] = { 0 };
UiWindow KAW_HELP_WINDOW = { 0 };
s32 KAW_BONUS_ROW = 0;
s32 KAW_BONUS_EXP = 0;
