#include "common.h"
#include "game.h"
#include "dcb/subseg.h"

/* jp's SUBSEG data that starts zeroed; the u8 arrays are not referenced by
   any code */
JpWindow *D_801F3508[4] = { 0 };
JpWindow *D_801F3518[8] = { 0 };
u8 D_801F3538 = 0;
u8 D_801F3539 = 0;
JpCursor *D_801F353C = NULL;
u8 D_801F3540[8] = { 0 };
DR_ENV D_801F3548[2] = { { 0 } };
DR_ENV D_801F35C8[2] = { { 0 } };
DRAWENV D_801F3648[2] = { { { 0 } } };
ShopState *D_801F3700 = NULL;
u8 D_801F3704[4] = { 0 };
s16 D_801F3708[18] = { 0 };
u8 D_801F372C[0x30] = { 0 };
u8 D_801F375C[4] = { 0 };
u8 D_801F3760[4] = { 0 };
u8 D_801F3764[0x1C] = { 0 };
s8 D_801F3780 = 0;
s16 D_801F3782 = 0;
u8 *D_801F3784 = NULL;
