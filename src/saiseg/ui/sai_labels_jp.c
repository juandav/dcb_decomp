#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/pad.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/fade.h"
#include "dcb/frame_callback.h"
#include "dcb/game_exit.h"
#include "dcb/saiseg.h"

/* jp's code from the deck information screen's task to the slot machine's
   (sai_labels.c is us's and eu's) */

/* where initVramSprite takes a sprite from */
typedef struct {
    /* 0x00 */ s32 clut;
    /* 0x04 */ s32 colorMode;
    /* 0x08 */ s32 vramX;
    /* 0x0C */ s32 vramY;
    /* 0x10 */ s32 width;
    /* 0x14 */ s32 height;
} SpriteDef;

/* the Bits a script gives, counted up into the player's */
typedef struct {
    /* 0x00 */ JpWindow *window;
    /* 0x04 */ s32 step;
    /* 0x08 */ s32 total;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s8 unkE;
    /* 0x0F */ s8 running;
    /* 0x10 */ s8 unk10;
} BitsReward;

extern JpGame *SCROLLING_BACKGROUND;
extern Bytes4 SAI_TEXT_RGB;
extern SpriteDef D_801F6000[];
extern JpWindowDef D_801F63C0;
extern JpWindowDef D_801F63E0;
extern BitsReward *SAI_BITS_REWARD;
extern s32 D_801F7558;
extern void runWindowTask();
extern void renderScrollingBackground();

void openKanjiPage(s32 page, s32 capacity);
void clearKanjiPage(s32 page);
void closeKanjiPage(s32 page);
u8 *formatSjisNumber(s32 value, s32 width, u8 *dst);
void openChoiceMenu(JpMenu *, s32, s32, s32, s32);
void showScrollingBackground(void);
void setBackgroundScrollMode(s32);

/* no prototypes: this module passes its coordinates as ints */
void initVramSprite();
void uploadTim();

void SAI_clearTextVram(void);
void SAI_openDeckInfo(void);
s8 func_801EDB34(void);
void SAI_addTextLine(s8 withBits);
void SAI_clearTextLines(void);
void SAI_loadMapTextures(void);
void func_801EE8F0(void);
void func_801F1130();
void SAI_runSlotMachine();
void func_801EDEF0(void);
void SAI_runBitsReward(s32 unused, s32 parent);
void func_801EDF7C(s8 fade);

void SAI_runDeckChoice(void) {
    openKanjiPage(0xF, 0x1E3);
    SAI_openDeckInfo();
    do {
        waitFrames(1);
        func_801EDB34();
    } while (SAI_STATE->regs[15] == 0);
    ((SessionData *)SESSION_DATA)->deckChoice = SAI_STATE->regs[15] - 1;
    ((JpWindow *)D_801F7558)->state = 4;
    waitFrames(30);
    closeKanjiPage(0xF);
}

void SAI_loadCardImage(s8 slot, s32 card) {
    char path[0x40];
    u32 *tim;

    sprintf(path, "B:\\L_CARD\\LC%3.3d.TIM", card);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTim(tim, slot * 32 + 0x300, 0xB0, 0, slot + 0x1FA);
    DrawSync(0);
    freeHeapBlock(tim);
}

void SAI_showLeftSprite(s32 kind) {
    SpriteDef *def;
    s8 i;

    def = &D_801F6000[kind];
    SAI_UI.unk3BC->state = 2;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SAI_UI.unk3BC->unk2 != 1);
    for (i = 0; i < 2; i++) {
        initVramSprite(DB(i).primSlots[0] + 0x40, 0x17, 0x4F, def->clut, def->colorMode, def->vramX, def->vramY, def->width, def->height, -1);
    }
    SAI_UI.unk3BC->state = 1;
    exitTask();
}

void SAI_tickRandomTickers(void) {
    ShopRecord *shop;
    s8 i;

    shop = PLAYER_DATA(0).shops;
    for (i = 0; i < 7; i++, shop++) {
        if (shop->timer >= 0 && ++shop->timer >= shop->period) {
            shop->timer = 0;
            shop->soldBits = 0;
            shop->starterStock = 0;
            /* the same counter as the loop over the shops: one restock ends it */
            for (i = 0; i < 7; i++) {
                shop->seeds[i][0] = shop->seeds[i][1];
                shop->seeds[i][1] = rand();
            }
        }
    }
}

void func_801EDED4(s8 index, u8 value) {
    SAI_UI.unk3CC[index] = value;
}

void func_801EDEF0(void) {
    SAI_clearTextVram();
    SAI_loadMapTextures();
    addFrameCallback((s32)func_801F1130);
    SAI_STATE->unk4B = 1;
    SAI_UI.unk3C8 = SAI_STATE->unk4E;
    SAI_STATE->unk48 = 0;
    SAI_STATE->unk49 = 0;
}

void func_801EDF7C(s8 fade) {
    if (!fade) {
        setBackgroundScrollMode(3);
    }
    func_801EE8F0();
    SCROLLING_BACKGROUND->unk1C0 = -1;
    removeFrameCallback((s32)func_801F1130);
    SAI_STATE->unk4E = SAI_UI.unk3C8;
    SAI_UI.unk3B4->state = 4;
    SAI_UI.unk3B0->state = 4;
    waitFrames(30);
    if (fade) {
        spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 4, 0);
    }
}

void func_801EE058(void) {
    func_801EDF7C(0);
    quitToTitleOrPlayEnding(2);
    setBackgroundScrollMode(1);
    waitFrames(30);
    SAI_UI.unk3C8 = SAI_STATE->unk4E;
    openChoiceMenu(&SAI_UI.menu, (s8)SAI_UI.unk3C8, 0x32, 0, 0);
    waitFrames(30);
    func_801EDEF0();
}

char *SAI_copyText(char *dst, char *src, s32 n) {
    char *d;

    d = dst;
    while (n > 0) {
        if ((*d++ = *src++) == 0) {
            break;
        }
        n--;
    }
    for (; n > 1; n--) {
        *d = 0;
    }
    return dst;
}

void SAI_drawMessageLines(JpWindow *win) {
    Bytes4 rgb = SAI_TEXT_RGB;
    Rect16 uv;
    MsgLine *line;
    u32 i;
    u32 j;
    u32 count;
    s32 x;

    line = SAI_UI.lines;
    SAI_UI.typing = 0;
    if (SAI_STATE->flags & 0x10) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[win->z], DB(FRAME_BUFFER_INDEX).primSlots[0] + 0x60);
    }
    for (i = 0; i < 4; i++, line++) {
        if (line->active == 1) {
            line->shown += 0x28;
            count = line->shown / line->width;
            if (count >= line->width) {
                count = line->width;
            } else {
                SAI_UI.typing = 1;
            }
            for (j = 0; j < count; j++) {
                x = j * 12;
                uv.x = x;
                uv.y = line->vramY;
                uv.w = 0xC;
                uv.h = 0xB;
                drawPageSpriteColored(x + 0x26, line->vramY + 0x19, &uv, rgb.b, GetTPage(0, 0, 0x3C0, 0), line->palettes[j], win->z);
            }
        }
        if (SAI_UI.typing == 1) {
            break;
        }
    }
}

/* it loads SESSION_DATA's address again for the Bits, where our C reuses
   the one it loaded before */
INCLUDE_ASM("saiseg/nonmatchings/ui/sai_labels_jp", SAI_runBitsReward);

void SAI_giveBits(void) {
    spawnTask(0, -1, 0, 0x1000, SAI_runBitsReward, 0, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
}

void SAI_drawBits(JpWindow *win) {
    char line[0x48];
    u8 digits[0x18];
    s32 bits = PLAYER_DATA(0).bits;

    if (SAI_UI.unk3B8->state != 4) {
        formatSjisNumber(bits, 6, digits);
        sprintf(line, " 所持金 s0w-4%s d0", digits);
        drawIconText(0xAE, 0x38, 7, 1, win->z, (s32)line);
    }
}

void SAI_playSlotMachine(void) {
    waitFrames(40);
    stopScreenFade();
    removeFrameCallback((s32)renderScrollingBackground);
    spawnTask(0, -1, 0, 0x1000, SAI_runSlotMachine, 0, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    stopScreenFade();
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 4, 0);
    waitFrames(40);
}
