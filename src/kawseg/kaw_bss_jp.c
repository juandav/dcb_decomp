#include "common.h"
#include "game.h"
#include "dcb/kawseg.h"

/* jp's KAWSEG data that starts zeroed (kaw_bss.c is us's and eu's); the u8
   arrays are not referenced by any code */
s32 KAW_SUPPORT_REGISTER = 0;
s32 KAW_GIVE_UP_DELAY = 0;
POLY_F4 KAW_VS_BAR_POLYS[2] = { { 0 } };
DR_MODE KAW_VS_BAR_MODES[2] = { { 0 } };
POLY_FT4 KAW_VS_LOGO_POLYS[2] = { { 0 } };
POLY_FT4 KAW_RESULT_WINNER_POLYS[2] = { { 0 } };
u8 D_801FFC00[0x50] = { 0 };
s32 KAW_MATCH_LOADING = 0;
u8 D_801FFC54[0xC] = { 0 };
ChoiceMenu KAW_TURN_ORDER_MENU = { { 0 } };
s32 KAW_TURN_ORDER_STEP = 0;
void *KAW_HAND_CURSOR = NULL;
s32 KAW_RESULT_SCREEN_STATE = 0;
