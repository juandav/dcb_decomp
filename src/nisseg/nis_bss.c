#include "common.h"
#include "game.h"
#include "dcb/nisseg.h"

/* NISSEG's data that starts zeroed: the trade's, the VS mode's, then the
   deck screens', laid out the same in jp's and in eu's debug build. The u8
   arrays are not referenced by any code */
TradeWindows NIS_TRADE_WINDOWS = { 0 };
u8 D_801FC4C0[0x10] = { 0 };
NisCursor *NIS_TRADE_CURSOR = NULL;
u8 D_801FC4D4[4] = { 0 };
TradeView NIS_TRADE_VIEW = { 0 };
TradeCardList *NIS_TRADE_LIST = NULL;
TradeMasks *NIS_TRADE_MASKS = NULL;
u8 NIS_TRADE_PLAYER = 0;
u8 D_801FC4EC[0xC] = { 0 };
TradeOffer NIS_TRADE_OFFERS[2] = { { 0 } };
u8 D_801FC56C[0xC] = { 0 };
DR_ENV NIS_TRADE_SCENE_ENVS[2] = { { 0 } };
DR_ENV NIS_TRADE_SCREEN_ENVS[2] = { { 0 } };
DRAWENV NIS_TRADE_SCENE_DRAWENVS[2] = { { { 0 } } };
NisTradeScreens NIS_TRADE_SCREENS = { 0 };
NisWindow *NIS_VS_WINDOW = NULL;
u8 D_801FC73C[4] = { 0 };
u8 NIS_TRADE_BLOCKED = 0;
u8 NIS_SAME_SAVE = 0;
u8 D_801FC744[4] = { 0 };
NisCursor *NIS_NAME_CURSOR = NULL;
u8 D_801FC74C[0xC] = { 0 };
NisDeckScreens NIS_DECK_SCREENS = { 0 };
u8 NIS_GRID_CURSOR = 0;
u8 NIS_GRID_SCROLL = 0;
NisCursor *NIS_GRID_POINTER = NULL;
u8 D_801FC774[4] = { 0 };
DR_ENV NIS_DECK_SCENE_ENVS[2] = { { 0 } };
DR_ENV NIS_DECK_SCREEN_ENVS[2] = { { 0 } };
s32 NIS_AUTO_DECK_FROM_MENU = 0;
s32 NIS_KANA_PAGE = 0;
s8 NIS_CARD_IN_VIEW = 0;
u8 D_801FC884[4] = { 0 };
NisCardImage NIS_CARD_IMAGE = { { { 0 } } };
s8 NIS_AUTO_DECK_QUESTION = 0;
NisDeckEdit NIS_DECK_EDIT = { 0 };
u8 (*NIS_DECK_BACKUP)[2] = NULL;
u8 *NIS_DIGIMON_LEFT = NULL;
s16 *NIS_ROOKIE_COUNTS = NULL;
NisCardList *NIS_CARD_LIST = NULL;
u8 *NIS_OPTIONS_LEFT = NULL;
u8 *NIS_OTHERS_LEFT = NULL;
u8 D_801FC8F4[4] = { 0 };
DRAWENV NIS_DECK_SCENE_DRAWENVS[2] = { { { 0 } } };
char NIS_TYPED_NAME[0x10] = { 0 };
NisProfile *NIS_PROFILE_BACKUP = NULL;
