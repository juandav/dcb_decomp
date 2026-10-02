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

extern JpGame *SCROLLING_BACKGROUND;
extern Bytes4 SAI_TEXT_RGB;
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
s8 SAI_getBackgroundState(void);
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
        SAI_getBackgroundState();
    } while (SAI_STATE->regs[15] == 0);
    ((SessionData *)SESSION_DATA)->deckChoice = SAI_STATE->regs[15] - 1;
    ((JpWindow *)SAI_DECK_INFO_WINDOW)->state = 4;
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

    def = &SAI_PORTRAIT_SPRITE_DEFS[kind];
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

void SAI_tickShopRestocks(void) {
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

void SAI_setPortraitBrightness(s8 index, u8 value) {
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

/* the Bits a script gives: their window, the message, and the count up into
   the player's */
void SAI_runBitsReward(s32 unused, s32 parent) {
    SAI_clearTextVram();
    SAI_STATE->unk4B = 1;
    SAI_UI.unk3C8 = SAI_STATE->unk4E;
    showScrollingBackground();
    openChoiceMenu(&SAI_UI.menu, (s8)SAI_UI.unk3C8, 0x32, 0, 0);
    SAI_STATE->unk48 = 0;
    SAI_STATE->unk49 = 0;
    waitFrames(3);
    addFrameCallback((s32)func_801F1130);
    /* no Bits to give: the guard returns early, and the match depends on
       it: the rest then starts at a label, so CSE loads SESSION_DATA's
       address anew as jp's does */
    if (SAI_STATE->regs[1] == 0) {
        resumeTask(parent);
        return;
    }
    openKanjiPage(0xF, 0xE7);
    clearKanjiPage(0xF);
    waitFrames(3);
    SAI_BITS_REWARD = allocHeapBlock(0x14, 0x191);
    SAI_BITS_REWARD->step = SAI_STATE->bits / 95; /* counted up in about 95 steps */
    SAI_BITS_REWARD->total = SAI_STATE->bits + PLAYER_DATA(0).bits;
    if (SAI_BITS_REWARD->total > 999998) {
        SAI_BITS_REWARD->total = 999999;
    }
    if (SAI_BITS_REWARD->step == 0) {
        SAI_BITS_REWARD->step = 1;
    }
    SAI_BITS_REWARD->running = 1;
    SAI_BITS_REWARD->unkC = 10;
    SAI_BITS_REWARD->unkE = 0;
    SAI_BITS_REWARD->unk10 = 0;
    spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_BITS_WINDOW_DEF, getCurrentTaskId());
    SAI_UI.unk3B8 = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_MESSAGE_LINES_WINDOW_DEF, getCurrentTaskId());
    SAI_BITS_REWARD->window = (JpWindow *)waitFrames(0x7FFFFFFF);
    waitFrames(60);
    playSoundEffect(0x18);
    SAI_addTextLine(1);
    SAI_UI.typing = 1;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SAI_UI.typing == 1);
    waitFrames(60);
    PLAYER_DATA(0).bits = SAI_BITS_REWARD->total;
    playSoundEffect(0x17);
    do {
        waitFrames(FRAME_INTERVAL);
        SAI_STATE->flags++;
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            SAI_STATE->flags = 0;
            playSoundEffectAtVolume(0, 0x32);
            SAI_clearTextLines();
            SAI_BITS_REWARD->running = 0;
        }
    } while (SAI_BITS_REWARD->running == 1);
    SAI_BITS_REWARD->unk10 = 0;
    SAI_BITS_REWARD->window->state = 4;
    SAI_UI.unk3B8->state = 4;
    waitFrames(30);
    freeHeapBlocksByTag(0x191);
    closeKanjiPage(0xF);
    resumeTask(parent);
}

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
