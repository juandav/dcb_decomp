#include "common.h"
#include "game.h"
#include "dcb/subseg.h"

/* jp's SUBSEG data that starts zeroed; the u8 arrays are not referenced by
   any code */
JpWindow *SUB_VIEWER_WINDOWS[4] = { 0 };
JpWindow *SUB_SHOP_WINDOWS[8] = { 0 };
u8 SUB_SAVED_CURSOR = 0;
u8 SUB_SAVED_TOP = 0;
JpCursor *SUB_GRID_CURSOR = NULL;
u8 D_801F3540[8] = { 0 };
DR_ENV SUB_PREVIEW_DR_ENVS[2] = { { 0 } };
DR_ENV SUB_SCREEN_DR_ENVS[2] = { { 0 } };
DRAWENV SUB_PREVIEW_DRAW_ENVS[2] = { { { 0 } } };
ShopState *SUB_SHOP = NULL;
u8 D_801F3704[4] = { 0 };
s16 SUB_STOCK_IDS[18] = { 0 };
u8 D_801F372C[0x30] = { 0 };
u8 D_801F375C[4] = { 0 };
u8 D_801F3760[4] = { 0 };
u8 D_801F3764[0x1C] = { 0 };
s8 SUB_SHOP_RUNNING = 0;
s16 SUB_STOCK_ID_COUNT = 0;
u8 *SUB_PACK_CARD_END = NULL;
