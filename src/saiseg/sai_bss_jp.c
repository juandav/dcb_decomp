#include "common.h"
#include "game.h"
#include "dcb/prim_desc.h"
#include "dcb/saiseg.h"

/* jp's SAISEG data that starts zeroed (sai_bss.c is us's and eu's); the u8
   arrays are not referenced by any code */
s8 SAI_OWNED_KEY_ITEMS[14] = { 0 };
KeyItemScreen SAI_KEY_ITEMS = { 0 };
u8 D_801F6E18[8] = { 0 };
KeyItem SAI_KEY_ITEM_LIST[15] = { { 0 } };
u8 D_801F6F4C[0x64] = { 0 };
BitsReward *SAI_BITS_REWARD = NULL;
u8 D_801F6FB4[4] = { 0 };
DeckInfo SAI_DECK_INFO = { { { { 0 } } } };
SaiUi SAI_UI = { 0 };
TypedText SAI_TYPED_TEXT = { { { 0 } } };
u8 D_801F7554[4] = { 0 };
s32 SAI_DECK_INFO_WINDOW = 0;
u8 D_801F755C[0xC] = { 0 };
SlotMachine SAI_SLOT_MACHINE = { { NULL } };
u8 D_801F75B8[8] = { 0 };
PrimDesc3D SAI_MAP_PLAYER_DRAWN = { 0 };
s8 SAI_MAP_LABEL_PLACE = 0;
s8 SAI_MAP_CURSOR = 0;
s8 SAI_MAP_PLACE = 0;
s16 SAI_MAP_X = 0;
s16 SAI_MAP_Y = 0;
