#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/saiseg.h"

/* jp's map window and the script flags the profile saves (sai_opponent_select.c
   is us's and eu's) */

/* where initVramSprite takes a sprite from */
typedef struct {
    /* 0x00 */ s32 clut;
    /* 0x04 */ s32 colorMode;
    /* 0x08 */ s32 vramX;
    /* 0x0C */ s32 vramY;
    /* 0x10 */ s32 width;
    /* 0x14 */ s32 height;
} SpriteDef;

typedef struct {
    s32 x;
    s32 y;
} Point;

/* no prototype: this module passes its coordinates as ints */
void initVramSprite();

void SAI_runArea(void);

void func_801F0EA8(void) {
    do {
        waitFrames(1);
    } while (SAI_STATE->unk43 != 2);
    SAI_STATE->unk43 = 0;
    spawnTask(0, -1, 0, 0x1600, SAI_runArea, 0, getCurrentTaskId(), 0, 0);
    exitTask();
}

void func_801F0F48(void) {
    SpriteDef defs[4] = {
        { 0x7CC0, 1, 0x140, 0x100, 0, 0 },
        { 0x7FF8, 0, 0x300, 0x1B0, 0xB8, 0x24 },
        { 0x7F39, 0, 0x330, 0x100, 0x40, 0x100 },
        { 0x7F79, 0, 0x364, 0x1BD, 0x10, 8 },
    };
    Point positions[4] = { { 0x20, 0x23 }, { 2, 0xF }, { 0xFE, 0 }, { 0x119, 0xD1 } };
    SpriteDef *def;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        DB(i).primSlots[0] = (s32)(SAI_STATE->sprites[i] = allocHeapBlock(0xA0, 0x190));
    }
    for (i = 0; i < 2; i++) {
        for (def = defs, j = 0; j < 4; j++, def++) {
            initVramSprite(DB(i).primSlots[0] + j * 0x20, positions[j].x, positions[j].y, def->clut, def->colorMode, def->vramX,
                           def->vramY, def->width, def->height, -1);
        }
    }
}

void func_801F1130(FrameBuffer *fb) {
    s32 h;
    u16 t;
    s8 step;
    s32 x;
    s32 y;
    s32 w;
    s32 u;
    s32 v;
    s32 clut;
    s32 i;

    t = SAI_STATE->unk3C;
    step = SAI_STATE->unk4B;
    if (step != 0x7F) {
        if (step != 0) {
            x = ((4800 - (s16)t * 240) / 40 + 40) & 0xFFFE;
            y = (3280 - (s16)t * 164) / 40 + 39;
            w = (s16)t * 12;
            h = (s16)t * 164 / 20;
            u = (1200 - (s16)t * 60) / 20 + (SAI_STATE->unk46 << 7) + 0x140;
            v = (1640 - (s16)t * 82) / 20;
            clut = (SAI_STATE->unk46 + 0x1F3) << 6;
            SAI_STATE->unk3C = step + t;
            if (SAI_STATE->unk3C == 0 || SAI_STATE->unk3C >= 21) {
                switch (SAI_STATE->unk4B) {
                case -1:
                    SAI_STATE->unk4B = 1;
                    SAI_STATE->unk46 = SAI_STATE->unk47;
                    break;
                case 1:
                    SAI_STATE->unk4B = 0;
                    break;
                }
            }
            for (i = 0; i < 2; i++) {
                initVramSprite(DB(i).primSlots[0], x, y + 5, clut, 1, u, v, w, h, -1);
            }
        }
        if ((u8)(SAI_STATE->unk4B + 1) < 3) {
            addPrim(&fb->ot[30], fb->primSlots[0]);
        }
    }
}

void func_801F1440(void) {
}

void func_801F1448(JpWindow *win) {
    s32 i;
    u8 level;

    if (SAI_UI.unk3CC[0] > SAI_UI.unk3CA) {
        SAI_UI.unk3CA += 4;
        if (SAI_UI.unk3CC[0] < SAI_UI.unk3CA) {
            SAI_UI.unk3CC[0] = SAI_UI.unk3CA;
        }
    } else if (SAI_UI.unk3CC[0] < SAI_UI.unk3CA) {
        SAI_UI.unk3CA -= 4;
        if (SAI_UI.unk3CC[0] > SAI_UI.unk3CA) {
            SAI_UI.unk3CC[0] = SAI_UI.unk3CA;
        }
    }
    level = SAI_UI.unk3CA;
    for (i = 0; i < 2; i++) {
        ((VramSprite *)DB(i).primSlots[0])[4].sp.r0 = ((VramSprite *)DB(i).primSlots[0])[4].sp.g0 =
            ((VramSprite *)DB(i).primSlots[0])[4].sp.b0 = level;
    }
    AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[win->z], (s32)&((VramSprite *)DB(FRAME_BUFFER_INDEX).primSlots[0])[4]);
}

void func_801F1590(JpWindow *win) {
    s32 i;
    u8 level;

    if (SAI_UI.unk3CC[1] > SAI_UI.unk3CB) {
        SAI_UI.unk3CB += 4;
        if (SAI_UI.unk3CC[1] < SAI_UI.unk3CB) {
            SAI_UI.unk3CC[1] = SAI_UI.unk3CB;
        }
    } else if (SAI_UI.unk3CC[1] < SAI_UI.unk3CB) {
        SAI_UI.unk3CB -= 4;
        if (SAI_UI.unk3CC[1] > SAI_UI.unk3CB) {
            SAI_UI.unk3CC[1] = SAI_UI.unk3CB;
        }
    }
    level = SAI_UI.unk3CB;
    for (i = 0; i < 2; i++) {
        ((VramSprite *)DB(i).primSlots[0])[2].sp.r0 = ((VramSprite *)DB(i).primSlots[0])[2].sp.g0 =
            ((VramSprite *)DB(i).primSlots[0])[2].sp.b0 = level;
    }
    AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[win->z], (s32)&((VramSprite *)DB(FRAME_BUFFER_INDEX).primSlots[0])[2]);
}

void func_801F16D8(void) {
}

void func_801F16E0(void) {
    /* the flags bits 3 to 15 of unkF1C set in unkF1E (one byte too many in
       its string) */
    s8 order[13] = "\x01\x0D\x02\x03\x04\x05\x07\x08\x09\x0C\x0A\x0E\x06\x00" "1";
    s32 i;
    s32 reg;
    s32 word;

    for (i = 0, word = 0x12; i < 16; i++) {
        if (SAI_STATE->regs[word++] != 0) {
            PLAYER_DATA(0).unkF1C |= 1 << i;
        }
    }
    for (i = 3, word = 0; i < 16; i++, word++) {
        if ((PLAYER_DATA(0).unkF1C >> i) & 1) {
            PLAYER_DATA(0).unkF1E |= 1 << order[word];
        }
    }
    if (PLAYER_DATA(0).unkF1C & 4) {
        PLAYER_DATA(0).unkF1E |= 0x801;
    }
    reg = 0x22;
    for (word = 0; word < 10; word++) {
        for (i = 0; i < 32; i++) {
            if (SAI_STATE->regs[reg++] != 0) {
                PLAYER_DATA(0).eventFlags[word] |= 1 << i;
            } else {
                PLAYER_DATA(0).eventFlags[word] &= ~(1 << i);
            }
            if (reg >= 0x11E) {
                i = 32;
                word = 10;
            }
        }
    }
}

void func_801F18C8(void) {
    s32 i;
    s32 j;
    s32 reg;
    s32 word;

    for (j = 0x12, i = 0; i < 16; i++, j++) {
        SAI_STATE->regs[j] = (PLAYER_DATA(0).unkF1C >> i) & 1;
    }
    reg = 0x22;
    word = 0;
    do {
        i = 0;
        do {
            SAI_STATE->regs[reg] = (PLAYER_DATA(0).eventFlags[word] >> i) & 1;
            reg++;
            if (reg >= 0x11E) {
                i = 32;
                word = 10;
            }
            i++;
        } while (i < 32);
        word++;
    } while (word < 10);
}
