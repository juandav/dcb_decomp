#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/heap.h"
#include "dcb/vram_upload.h"

extern UiWindow D_801F22F8;
extern UiWindow D_801F2398;
extern UiWindow D_801F2438;
extern void func_801E0278();
extern void func_801E0948();
extern void func_801E0B08();
typedef struct {
    UiWindow window;
    u8 unk44[4];
} TabWindow;
extern TabWindow D_801F2870[3];
extern s32 D_801F2994;
extern UiWindow D_801F27D0;
extern UiWindow D_801F2950;
extern UiWindow D_801F2820;
extern UiWindow D_801F2690;
extern UiWindow D_801F2730;
extern void func_801E14E4(void);
extern void func_801E2098();
extern void func_801E2B8C();
extern void func_801E3BB4();
extern void func_801E2DA4();
extern void func_801E4788();
extern void func_801E4560();
extern s16 D_801F41BA;
typedef struct {
    u8 unk0[0x12];
    s16 player;
    u8 unk14[5];
    u8 useDeckCounts;
} Unk801F41A8;
typedef struct {
    u8 *deckCounts;
} Unk801F3F48;
extern Unk801F41A8 D_801F41A8;
extern Unk801F3F48 D_801F3F48;
extern s16 D_801F418A[8];
extern s8 D_801F419A[8];
extern s16 D_801F435E;
typedef struct {
    u8 unk0[0x28];
    s16 uniqueCount;
    s16 totalCount;
} CollectionStats;
extern CollectionStats D_801F4330;
typedef struct {
    u8 unk0[0xD];
    s8 unkD;
    s16 unkE;
} Unk801F4060;
extern Unk801F4060 D_801F4060;
extern u8 *D_801F342C;
typedef struct {
    s16 ids[252];
} CardIdList;
extern CardIdList *D_801F42D8;

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801DF9B4);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DDF38);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801DF9BC);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E0278);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E0948);

void func_801E0B08(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;

    drawText(x, y, (s32)"*b0 Insert", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b1 Delete", 7, z);
}

void func_801E0BA8(void) {
    drawWindow(&D_801F22F8, func_801E0278, 1);
    drawWindow(&D_801F2398, func_801E0948, 1);
    drawWindow(&D_801F2438, func_801E0B08, 1);
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E0C08);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DDFC0);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E0FF0);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E11D4);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E14E4);

s32 func_801E1680(s32 a, s32 b) {
    s32 result = 7;

    if (a < b) {
        result = 5;
    }
    if (b < a) {
        result = 2;
    }
    return result;
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E16A8);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E1B70);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E2098);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DE010);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DEFD8);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DEFE0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DEFE8);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DEFF4);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF000);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF010);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E2B8C);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E2DA4);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E3BB4);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF034);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E4560);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E4788);

void func_801E49CC(void) {
    s32 i;

    func_801E14E4();
    for (i = 0; i < 3; i++) {
        D_801F2870[i].window.brightness = 0x40;
    }
    D_801F2870[D_801F2994].window.brightness = 0x80;
    drawWindow(&D_801F2870[D_801F2994].window, func_801E2098, 30);
    for (i = 0; i < 3; i++) {
        if (i != D_801F2994) {
            drawWindow(&D_801F2870[i].window, func_801E2098, 30);
        }
    }
    drawWindow(&D_801F27D0, func_801E2B8C, 30);
    drawWindow(&D_801F2950, func_801E3BB4, 30);
    drawWindow(&D_801F2820, func_801E2DA4, 30);
    drawWindow(&D_801F2690, func_801E4788, 30);
    drawWindow(&D_801F2730, func_801E4560, 30);
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF048);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E4B34);

s32 func_801E56CC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E57FC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E592C(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5A60(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5B94(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 4) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 4) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5CC8(s8 **a, s8 **b) {
    s32 x = (*a)[2] == 1;
    s32 y = (*b)[2] == 1;

    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5DB0(s8 **a, s8 **b) {
    s32 x = (*a)[2] == 2;
    s32 y = (*b)[2] == 2;

    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5E98(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E5FC8(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E60FC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E6230(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E6360(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->hp;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->hp;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E6468(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if (D_801F41A8.useDeckCounts == 0) {
        x = getOwnedCardCount(D_801F41A8.player, *(s16 *)*a);
        y = getOwnedCardCount(D_801F41A8.player, *(s16 *)*b);
    } else {
        x = D_801F3F48.deckCounts[*(s16 *)*a];
        y = D_801F3F48.deckCounts[*(s16 *)*b];
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41A8.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E65DC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->dpCost;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->dpCost;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E66E4(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->dpBonus;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->dpBonus;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E67EC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[0].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[0].power;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E68F4(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[1].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[1].power;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 func_801E69FC(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[2].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[2].power;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(D_801F41BA).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E6B04);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E6BE8);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E6DA8);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF0B4);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF264);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E6FC8);

s32 func_801E75A0(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type == 0xFF) {
        x = -2;
    } else {
        x = 301 - *(s16 *)a->card;
    }
    if (b->type == 0xFF) {
        y = -2;
    } else {
        y = 301 - *(s16 *)b->card;
    }
    return y - x;
}

s32 func_801E7600(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7690(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7724(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E77B8(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E784C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 4) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 4) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E78E0(CardSlot *a, CardSlot *b) {
    s32 x = a->type == 1;
    s32 y = b->type == 1;

    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E792C(CardSlot *a, CardSlot *b) {
    s32 x = a->type == 2;
    s32 y = b->type == 2;

    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7978(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7A08(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7A9C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

void func_801E7B30(CardSlot *cards, s32 player) {
    s32 i;
    s32 j;
    s32 cardId;

    for (i = 0; i < 3; i++) {
        cardId = PLAYER_DATA(player).partners[i].cardId;
        if (cardId != 0) {
            for (j = 0; j < 30; j++) {
                if (cards[j].type != 0xFF && *(s16 *)cards[j].card == cardId) {
                    cards[j].card = (s8 *)&PLAYER_DATA(player).partners[i];
                    break;
                }
            }
        }
    }
}

s32 func_801E7C44(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->hp;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->hp;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7CAC(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->dpCost;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->dpCost;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7D14(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->dpBonus;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->dpBonus;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7D7C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[0].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[0].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7DE4(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[1].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[1].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7E4C(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[2].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[2].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 func_801E7EB4(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type == 0xFF) {
        x = -1;
    } else {
        x = getOwnedCardCount(D_801F41BA, *(s16 *)a->card) - D_801F3F48.deckCounts[*(s16 *)a->card];
    }
    if (b->type == 0xFF) {
        y = -1;
    } else {
        y = getOwnedCardCount(D_801F41BA, *(s16 *)b->card) - D_801F3F48.deckCounts[*(s16 *)b->card];
    }
    return y - x;
}

void func_801E7F8C(CardSlot *cards) {
    CardSlot sorted[30];
    s16 ids[30];
    CardSlot *out = sorted;
    s8 count = 0;
    s32 i;
    s32 j;

    for (i = 0; i < 30; i++) {
        sorted[i].type = 0xFF;
        if (cards[i].type == 0xFF) {
            ids[i] = -1;
        } else {
            ids[i] = *(s16 *)cards[i].card;
        }
    }
    for (i = 0; i < 30; i++) {
        if (ids[i] != -1) {
            for (j = i; j < 30; j++) {
                if (j == i) {
                    count = 0;
                } else if (ids[i] == ids[j]) {
                    ids[j] = -1;
                    count++;
                }
            }
            while (count >= 0) {
                count--;
                *out++ = cards[i];
            }
        }
    }
    for (i = 0; i < 30; i++) {
        cards[i] = sorted[i];
    }
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E8110);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF27C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF280);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF288);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF294);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF298);

void func_801E831C(void) {
    char path[64];
    u32 *tim;

    sprintf(path, "C:\\OBJECT\\c_map.tim");
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, 0x140, 0, 0, 0x1F4);
    freeHeapBlock(tim);
    printf("aaa\n");
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E83BC);

s32 func_801E8670(s16 id) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (id == D_801F418A[i] && D_801F419A[i] != 0) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E86D4);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E8798);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E8864);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E89FC);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E8C04);

void func_801E8D98(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style) {
    Rect16 rect;
    s32 unused[2]; /* unused, but it is in the original stack frame */
    Rect16 view;

    rect.x = area.x - area.w / 2;
    rect.y = area.y - area.h / 2;
    rect.w = area.w & ~1;
    rect.h = area.h & ~1;
    view.x = 0;
    view.y = 0;
    view.w = (area.w + 10) & ~1;
    view.h = 0x2000;
    openWindow(window, &rect, -1, (s16 *)&view, flags, style, 0x80, 12);
    window->label = label;
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E8E8C);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E913C);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E93B8);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E95D0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF318);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF320);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF32C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF334);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF33C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF348);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF364);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E9790);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF390);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801E9F68);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF478);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", jtbl_801DF484);

void func_801EA3D4(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawText(x, y, (s32)"*b0:Edit Decks", 7, z);
    if (D_801F435E == 0) {
        drawText(x + 0x66, y, (s32)"*b5:Sort", 7, z);
    } else {
        drawText(x + 0x66, y, (s32)"*b5:Sort", 8, z);
    }
}

void func_801EA478(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    y += 2;
    if (D_801F4330.uniqueCount >= 301) {
        drawIcon(x, y, 0, 20, z);
    }
    x += 12;
    drawTinyText(x, y, (s32)"All", 6, z);
    drawTinyText(x, y + 6, (s32)"Types", 6, z);
    sprintf(buf, "%3d", D_801F4330.uniqueCount);
    drawText(x + 26, y, (s32)buf, 7, z);
    x += 80;
    if (D_801F4330.totalCount >= 1608) {
        drawIcon(x - 24, y, 0, 20, z);
    }
    drawTinyText(x - 12, y, (s32)"Total Number", 6, z);
    drawTinyText(x - 12, y + 6, (s32)"of Cards", 6, z);
    sprintf(buf, "%4d", D_801F4330.totalCount);
    drawText(x + 38, y, (s32)buf, 7, z);
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EA5F0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF4F0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF4FC);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF504);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF50C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF53C);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EA8AC);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EB1FC);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF54C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF554);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF55C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF564);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF574);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF578);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF57C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF580);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF584);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF588);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF594);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF59C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF5B8);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EB2BC);

void func_801EB8BC(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 palette;

    if (D_801F4060.unkD == D_801F4060.unkE) {
        window->palette = 6;
        palette = 7;
    } else {
        window->palette = 1;
        palette = 8;
    }
    drawText(x + 0x50, y + 0x12, (s32)"NO DATA", palette, z);
}

void func_801EB92C(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawIcon(x, y, 1, 11, z);
    drawMediumText(x + 8, y, (s32)":Cursor", 7, z);
    y += 10;
    drawIcon(x, y, 1, 9, z);
    drawMediumText(x + 8, y, (s32)":Edit", 7, z);
    y += 10;
    drawIcon(x, y, 1, 7, z);
    drawMediumText(x + 8, y, (s32)":Delete", 7, z);
    y += 10;
    drawIcon(x, y, 1, 10, z);
    drawMediumText(x + 8, y, (s32)":Copy", 7, z);
    y += 10;
    drawIcon(x, y, 1, 13, z);
    drawMediumText(x + 8, y, (s32)":Name", 7, z);
    y += 10;
    drawIcon(x, y, 1, 8, z);
    drawMediumText(x + 8, y, (s32)":Back", 7, z);
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EBAC0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF600);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF608);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF610);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF618);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EBCC0);

void func_801EC75C(s16 index) {
    u8 unused[0x40];

    uploadTim((u32 *)(D_801F342C + ((s32 *)D_801F342C)[index + 1]), 0x220, 0x100, 0x240, 0x1F5);
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EC7A8);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF6C0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF6C4);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF6CC);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EC998);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801ECE70);

s32 func_801ECF80(PlayerDeck *a, PlayerDeck *b) {
    s8 matched[30];
    s32 i;
    s32 j;
    s32 result = 0;

    for (i = 0; i < 30; i++) {
        matched[i] = 0;
    }
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 30; j++) {
            if (matched[j] == 0 && a->cards[i].type == b->cards[j].type) {
                if (a->cards[i].type == 0xFF) {
                    matched[j] = 1;
                    j = 30;
                } else if (a->cards[i].index == b->cards[j].index) {
                    matched[j] = 1;
                    j = 30;
                }
            }
        }
    }
    for (i = 0; i < 30; i++) {
        if (matched[i] == 0) {
            result = 1;
        }
    }
    return result;
}

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF708);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801ED070);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801ED944);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EDA88);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EDD60);

void func_801EDEFC(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawIcon(x, y, 1, 12, z);
    drawMediumText(x + 8, y, (s32)":Sort", 7, z);
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EDF6C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF7D4);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EE8CC);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EEE40);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EF740);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EF7EC);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801EFC78);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF86C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF888);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF88C);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF8A0);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801F0024);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801F0A20);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801F0BB0);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF968);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF970);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF978);

INCLUDE_RODATA("asm/subseg/nonmatchings/subseg", D_801DF984);

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801F0BE0);

s32 func_801F0FD8(CardSlot *slots, s32 row, s32 count) {
    s32 i;

    for (i = 0; i < count && D_801F42D8[row].ids[i] != -1; i++) {
        setCardSlotFromId((u8 *)slots, D_801F42D8[row].ids[i]);
        D_801F42D8[row].ids[i] = -1;
        slots++;
    }
    return i;
}

INCLUDE_ASM("asm/subseg/nonmatchings/subseg", func_801F10D8);
