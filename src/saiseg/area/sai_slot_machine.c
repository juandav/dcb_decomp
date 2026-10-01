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
#include "dcb/vram_upload.h"
#include "dcb/saiseg.h"

/* jp's own: the slot machine (SAI_playSlotMachine starts it). Three reels of
   symbols scroll behind a 4-row window; the player bets 1 to 3 times 100
   Bits, each bet lighting one more pair of lines, and stops the reels one at
   a time. Its sprites are SLOT.TIM's, one VramSprite each (the reels' 12,
   the prize table's, the lines' and the buttons'), drawn by a frame
   callback from the 3rd prim slot. */

/* the symbols on a reel strip are sprites 31 to 37 */
#define REEL_ROWS 30 /* each reel strip's, ended by a row of -1 */
#define REEL_ROW_HEIGHT 40

/* no prototype: this module passes its coordinates as ints */
void initVramSprite();
void runWindowTask();
u8 *formatSjisNumber(s32 value, s32 width, u8 *dst);

void SAI_drawSlotBits(JpWindow *win);
void SAI_drawSlotBet(JpWindow *win);
void SAI_drawSlotHelp(JpWindow *win);
void SAI_tickSlotWindow(JpWindow *win);
void SAI_tickSlotBet(void);
void SAI_spinSlotReels(void);
void SAI_judgeSlotLines(void);
void SAI_paySlotPrize(void);

/* the sprites, and where they go: the background, the prize table and its
   rows, the bet lines, the line numbers, the stop buttons, the reels'
   rows of symbols and the last ones, which show while two reels show the
   start of a line */
SpriteDef SAI_SLOT_SPRITE_DEFS[50] = {
    { 0x381F, 0, 0x1F8, 0x48, 0x10, 0x10 },
    { 0x3A1F, 0, 0x200, 0, 0xD0, 0x88 },
    { 0x7D00, 1, 0x240, 0, 0x48, 0x88 },
    { 0x3B9F, 0, 0x1E0, 0x78, 0x50, 0x18 },
    { 0x3B9F, 0, 0x1E0, 0x78, 0x50, 0x18 },
    { 0x3B9F, 0, 0x1E0, 0x78, 0x50, 0x18 },
    { 0x3B9F, 0, 0x1E0, 0x78, 0x50, 0x18 },
    { 0x3B9F, 0, 0x1E0, 0x78, 0x50, 0x18 },
    { 0x3B9F, 0, 0x1E0, 0x78, 0x50, 0x18 },
    { 0x3B9F, 0, 0x1E0, 0x78, 0x50, 0x18 },
    { 0x391F, 0, 0x1F0, 0x48, 0x20, 0x20 },
    { 0x389F, 0, 0x1F8, 0x28, 0x20, 0x20 },
    { 0x389F, 0, 0x1F0, 0x28, 0x20, 0x20 },
    { 0x389F, 0, 0x1F8, 0x28, 0x20, 0x20 },
    { 0x391F, 0, 0x1F0, 0x48, 0x20, 0x20 },
    { 0x381F, 0, 0x1F8, 0x48, 0x10, 0x10 },
    { 0x381F, 0, 0x1F8, 0x48, 0x10, 0x10 },
    { 0x381F, 0, 0x1F8, 0x48, 0x10, 0x10 },
    { 0x381F, 0, 0x1F8, 0x48, 0x10, 0x10 },
    { 0x381F, 0, 0x1F8, 0x48, 0x10, 0x10 },
    { 0x399F, 0, 0x1E0, 0x90, 0x28, 0x28 },
    { 0x399F, 0, 0x1E0, 0x90, 0x28, 0x28 },
    { 0x399F, 0, 0x1E0, 0x90, 0x28, 0x28 },
    { 0x7CC0, 1, 0x140, 0, 0x100, 0xF0 },
    { 0x7CC0, 1, 0x1C0, 0, 0x40, 0xF0 },
    { 0x3DA3, 0, 0x1F0, 0, 0x38, 0x14 },
    { 0x3DE3, 0, 0x1F0, 0x14, 0x38, 0x14 },
    { 0x3DA3, 0, 0x1F0, 0, 0x38, 0x14 },
    { 0x3DE3, 0, 0x1F0, 0x14, 0x38, 0x14 },
    { 0x3DA3, 0, 0x1F0, 0, 0x38, 0x14 },
    { 0x3DE3, 0, 0x1F0, 0x14, 0x38, 0x14 },
    { 0x3823, 0, 0x210, 0x88, 0x38, 0x28 },
    { 0x38E3, 0, 0x220, 0x88, 0x38, 0x28 },
    { 0x39A3, 0, 0x220, 0xB0, 0x38, 0x28 },
    { 0x3A63, 0, 0x230, 0x88, 0x38, 0x28 },
    { 0x3B23, 0, 0x200, 0x88, 0x38, 0x28 },
    { 0x3BE3, 0, 0x200, 0xB0, 0x38, 0x28 },
    { 0x3CA3, 0, 0x210, 0xB0, 0x38, 0x28 },
    { 0x3823, 0, 0x210, 0x88, 0x38, 0x28 },
    { 0x3B23, 0, 0x200, 0x88, 0x38, 0x28 },
    { 0x38E3, 0, 0x220, 0x88, 0x38, 0x28 },
    { 0x38E3, 0, 0x220, 0x88, 0x38, 0x28 },
    { 0x38E3, 0, 0x220, 0x88, 0x38, 0x28 },
    { 0x3D63, 0, 0x1E0, 0, 0x38, 0x78 },
    { 0x3D63, 0, 0x1E0, 0, 0x38, 0x78 },
    { 0x3D63, 0, 0x1E0, 0, 0x38, 0x78 },
    { 0x3827, 0, 0x264, 0, 8, 0x92 },
    { 0x3827, 0, 0x266, 0, 8, 0x92 },
    { 0x3827, 0, 0x240, 0x92, 0xB8, 8 },
    { 0x3827, 0, 0x240, 0x9A, 0xB8, 8 },
};

Point SAI_SLOT_SPRITE_POSITIONS[50] = {
    { 0, 0 }, { 22, 49 }, { 243, 48 }, { 240, 46 }, { 240, 65 },
    { 240, 84 }, { 240, 103 }, { 240, 122 }, { 240, 141 }, { 240, 160 },
    { 0, 31 }, { 0, 63 }, { 0, 103 }, { 0, 143 }, { 0, 175 },
    { 228, 46 }, { 228, 68 }, { 228, 108 }, { 228, 149 }, { 228, 172 },
    { 51, 176 }, { 111, 176 }, { 171, 176 }, { 0, 0 }, { 256, 0 },
    { 42, 54 }, { 42, 154 }, { 102, 54 }, { 102, 154 }, { 162, 54 },
    { 162, 154 }, { 42, 14 }, { 42, 54 }, { 42, 94 }, { 42, 134 },
    { 102, 14 }, { 102, 54 }, { 102, 94 }, { 102, 134 }, { 162, 14 },
    { 162, 54 }, { 162, 94 }, { 162, 134 }, { 42, 54 }, { 102, 54 },
    { 162, 54 }, { 30, 41 }, { 222, 41 }, { 38, 41 }, { 38, 179 },
};

s8 SAI_SLOT_REELS[REEL_ROWS + 1][3] = {
    { 31, 35, 37 }, { 32, 33, 36 }, { 33, 34, 35 }, { 34, 36, 34 }, { 35, 33, 33 },
    { 36, 34, 32 }, { 37, 35, 31 }, { 31, 32, 37 }, { 32, 31, 36 }, { 33, 36, 35 },
    { 34, 35, 34 }, { 35, 34, 33 }, { 36, 35, 32 }, { 37, 34, 31 }, { 31, 31, 37 },
    { 32, 33, 36 }, { 33, 32, 35 }, { 34, 36, 34 }, { 35, 37, 33 }, { 36, 33, 32 },
    { 37, 34, 31 }, { 31, 36, 37 }, { 32, 32, 36 }, { 33, 33, 35 }, { 34, 34, 34 },
    { 35, 36, 33 }, { 36, 37, 32 }, { 37, 33, 31 }, { 31, 31, 33 }, { 32, 34, 35 },
    { -1, -1, -1 },
};

JpWindowDef SAI_SLOT_BITS_WINDOW = { { 0x126, 0x15, 0, 0xE }, { 0xAE, 0x15, 0x78, 0xE }, 0xA, 1, SAI_drawSlotBits, SAI_tickSlotWindow };
JpWindowDef SAI_SLOT_BET_WINDOW = { { 0x17, 0x15, 0, 0xE }, { 0x17, 0x15, 0x74, 0xE }, 0xA, 0, SAI_drawSlotBet, SAI_tickSlotWindow };
JpWindowDef SAI_SLOT_HELP_WINDOW = { { 0x17, 0xCF, 0, 0xE }, { 0x17, 0xCF, 0x110, 0xE }, 0xA, 0, SAI_drawSlotHelp, SAI_tickSlotWindow };

/* what the slot machine does each frame, by its state: bet, spin, judge
   the lines, pay; 4 quits */
void (*SAI_SLOT_STATES[])(void) = {
    SAI_tickSlotBet, SAI_spinSlotReels, SAI_judgeSlotLines, SAI_paySlotPrize, NULL,
};

#define SLOT SAI_SLOT_MACHINE
#define FB_SPRITES(fb) ((VramSprite *)(fb)->primSlots[2])
#define SLOT_SPRITES(i) ((VramSprite *)DB(i).primSlots[2])
#define REEL_SYMBOL(row, reel) SAI_SLOT_REELS[SLOT.rows[row][reel]][reel]

void SAI_drawSlotBits(JpWindow *win) {
    char line[0x88];
    u8 digits[0x18];

    formatSjisNumber(PLAYER_DATA(0).bits, 6, digits);
    sprintf(line, " 所持金 s0w-4%s d0", digits);
    drawIconText(0xAE, 0x17, 7, 1, win->z, (s32)line);
}

void SAI_drawSlotBet(JpWindow *win) {
    char line[0x88];
    u8 digits[0x10];

    formatSjisNumber(SLOT.bet * 100, 4, digits);
    sprintf(line, " 掛け金 s0w-4%s d0", digits);
    drawIconText(0x1A, 0x17, 7, 1, win->z, (s32)line);
}

void SAI_drawSlotHelp(JpWindow *win) {
    char line[0x98];

    sprintf(line, " 方向キー：　ＢＥＴ　b0：スタート／ストップ b2：戻る");
    drawIconText(0x17, 0xD1, 7, 1, win->z, (s32)line);
}

void SAI_tickSlotWindow(JpWindow *win) {
}

void SAI_initSlotMachine(void) {
    u32 *tims;
    SpriteDef *def;
    s8 i;
    s8 j;
    s32 blend;

    spawnTask(0, -1, 0, 0x800, loadFile, "C:\\OBJECT\\SLOT.TIM", getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTimList(tims);
    freeHeapBlock(tims);
    for (i = 0; i < 2; i++) {
        DB(i).primSlots[2] = (s32)(SLOT.sprites[i] = allocHeapBlock(0x640, 0x3A));
    }
    for (i = 0; i < 2; i++) {
        def = SAI_SLOT_SPRITE_DEFS;
        for (j = 0; j < 50; j++) {
            if ((u8)(j - 25) < 7) {
                blend = 2;
            } else if (j == 1) {
                blend = 0;
            } else if (j >= 46) {
                blend = 1;
            } else {
                blend = -1;
            }
            initVramSprite(&SLOT_SPRITES(i)[j], SAI_SLOT_SPRITE_POSITIONS[j].x, SAI_SLOT_SPRITE_POSITIONS[j].y,
                           def->clut, def->colorMode, def->vramX, def->vramY, def->width, def->height, blend);
            def++;
        }
    }
    SLOT.scroll[0] = SLOT.scroll[1] = SLOT.scroll[2] = 0;
    SLOT.running = 1;
    for (i = 0; i < 4; i++) {
        SLOT.rows[i][0] = SLOT.rows[i][1] = SLOT.rows[i][2] = i;
    }
}

void SAI_resetSlotMachine(void) {
    s8 i;

    SLOT.state = 0;
    SLOT.bet = 0;
    SLOT.delay = 0;
    SLOT.winLine = 0;
    SLOT.prize = 0;
    SLOT.lineBlinkTimer = 0;
    SLOT.blinkTimer = 0;
    SLOT.reach = 0;
    SLOT.brightness = 0x80;
    SLOT.dimming = 0;
    for (i = 0; i < 3; i++) {
        SLOT.stopped[i] = 0;
        SLOT.shownScroll[i] = -1;
    }
}

/* the prize table's row of the winning symbol blinks */
s8 SAI_blinkSlotPrize(void) {
    VramSprite *sprite;
    s32 clut;
    s16 tick;
    s16 frame;
    s8 row;

    if (SLOT.prize != 0) {
        SLOT.blinkTimer++;
    }
    switch (SLOT.prize) {
    case 31:
        row = 9;
        break;
    case 32:
        row = 4;
        break;
    case 33:
        row = 3;
        break;
    case 34:
        row = 5;
        break;
    case 35:
        row = 8;
        break;
    case 36:
        row = 7;
        break;
    case 37:
        row = 6;
        break;
    default:
        return -1;
    }
    /* the row cycles through 6 palettes, 5 frames each */
    tick = SLOT.blinkTimer / 5;
    frame = tick % 6;
    clut = getClut(0x1F0, 0xEE + frame);
    sprite = &SLOT.sprites[FRAME_BUFFER_INDEX][row];
    sprite->sp.clut = clut;
    return row;
}

void SAI_lightSlotWinLine(void) {
    VramSprite *sprite;
    s16 clut;

    sprite = &SLOT.sprites[FRAME_BUFFER_INDEX][1];
    if (!(SLOT.lineBlinkTimer & 4)) {
        clut = 0x3A1F;
    } else {
        switch (SLOT.winLine) {
        case 1:
            clut = 0x3B5F;
            break;
        case 2:
            clut = 0x3A5F;
            break;
        case 3:
            clut = 0x3A9F;
            break;
        case 4:
            clut = 0x3ADF;
            break;
        case 5:
            clut = 0x3B1F;
            break;
        default:
            clut = 0x3A1F;
            break;
        }
    }
    sprite->sp.clut = clut;
}

void SAI_lightSlotLineNumbers(void) {
    VramSprite *sprite;
    s8 line;
    s16 clut;

    sprite = &SLOT.sprites[FRAME_BUFFER_INDEX][15];
    if (SLOT.winLine != 0) {
        SLOT.lineBlinkTimer++;
    }
    for (line = 1; line < 6; line++) {
        clut = 0x381F;
        if (line == SLOT.winLine && (SLOT.lineBlinkTimer & 4)) {
            clut = 0x385F;
        }
        sprite->sp.clut = clut;
        sprite++;
    }
}

void SAI_lightSlotStopButtons(void) {
    VramSprite *sprite;
    s8 i;
    s16 clut;

    sprite = &SLOT.sprites[FRAME_BUFFER_INDEX][20];
    for (i = 20; i < 23; i++) {
        if (SLOT.stopped[i - 20] != 0 && SLOT.stopped[i - 20] < 11) {
            clut = 0x39DF;
            SLOT.stopped[i - 20]++;
        } else {
            clut = 0x399F;
        }
        sprite->sp.clut = clut;
        sprite++;
    }
}

void SAI_setSlotReelSymbols(s8 reel) {
    VramSprite *sprite;
    SpriteDef *def;
    s8 i;
    s8 row;

    for (i = 0; i < 2; i++) {
        sprite = &SLOT.sprites[i][31 + reel * 4];
        for (row = 0; row < 4; row++) {
            def = &SAI_SLOT_SPRITE_DEFS[REEL_SYMBOL(row, reel)];
            sprite->sp.u0 = (def->vramX % 64) * 4;
            sprite->sp.v0 = def->vramY;
            sprite->sp.clut = def->clut;
            sprite++;
        }
    }
}

void SAI_scrollSlotReels(void) {
    VramSprite *sprite;
    s8 i;
    s8 reel;
    s8 row;
    s32 y;

    for (i = 0; i < 2; i++) {
        sprite = &SLOT.sprites[i][31];
        for (reel = 0; reel < 3; reel++) {
            if (SLOT.shownScroll[reel] >= SLOT.scroll[reel]) {
                SAI_setSlotReelSymbols(reel);
            }
            SLOT.shownScroll[reel] = SLOT.scroll[reel];
            y = 14;
            for (row = 0; row < 4; row++) {
                sprite->sp.y0 = SLOT.scroll[reel] + y;
                y += REEL_ROW_HEIGHT;
                sprite++;
            }
        }
    }
}

void SAI_lightSlotBetLines(void) {
    VramSprite *sprite;
    s32 cluts[4]; /* the middle line's, the inner pair's, the outer pair's,
                     and the one being set */
    s8 i;

    sprite = &SLOT.sprites[FRAME_BUFFER_INDEX][10];
    if (SLOT.state == 0) {
        if (SLOT.bet >= 3) {
            cluts[2] = 0x395F;
        } else {
            cluts[2] = 0x391F;
        }
        if (SLOT.bet >= 2) {
            cluts[1] = 0x38DF;
        } else {
            cluts[1] = 0x389F;
        }
        if (SLOT.bet <= 0) {
            cluts[0] = 0x389F;
        } else {
            cluts[0] = 0x38DF;
        }
        for (i = 10; i < 15; i++) {
            if (i == 10 || i == 14) {
                cluts[3] = cluts[2];
            } else if (i == 11 || i == 13) {
                cluts[3] = cluts[1];
            } else {
                cluts[3] = cluts[0];
            }
            sprite->sp.clut = cluts[3];
            sprite++;
        }
    }
}

void SAI_renderSlotMachine(FrameBuffer *fb) {
    s8 i;
    s8 buf;
    s8 j;
    s8 row;

    SAI_lightSlotBetLines();
    SAI_scrollSlotReels();
    SAI_lightSlotStopButtons();
    SAI_lightSlotLineNumbers();
    SAI_lightSlotWinLine();
    for (i = 0; i < 3; i++) {
        addPrim(&fb->ot[29], &FB_SPRITES(fb)[i]);
    }
    row = SAI_blinkSlotPrize();
    if (row >= 0) {
        addPrim(&fb->ot[30], &FB_SPRITES(fb)[row]);
    }
    for (i = 10; i < 46; i++) {
        addPrim(&fb->ot[30], &FB_SPRITES(fb)[i]);
    }
    /* the last sprites show only while two reels show the start of a line */
    if (SLOT.reach == 2) {
        for (buf = 0; buf < 2; buf++) {
            for (j = i; j < 50; j++) {
                SLOT_SPRITES(buf)[j].sp.r0 = SLOT.brightness;
                SLOT_SPRITES(buf)[j].sp.g0 = SLOT.brightness;
                SLOT_SPRITES(buf)[j].sp.b0 = SLOT.brightness;
            }
        }
        for (; i < 50; i++) {
            addPrim(&fb->ot[29], &FB_SPRITES(fb)[i]);
        }
    }
}

/* the prize for the winning symbol (bet once, whatever the bet) */
void SAI_paySlotPrize(void) {
    if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
        switch (SLOT.prize) {
        case 31:
            PLAYER_DATA(0).bits += 100;
            break;
        case 32:
            PLAYER_DATA(0).bits += 2000;
            break;
        case 33:
            PLAYER_DATA(0).bits += 3000;
            break;
        case 34:
            PLAYER_DATA(0).bits += 1000;
            break;
        case 35:
            PLAYER_DATA(0).bits += 200;
            break;
        case 36:
            PLAYER_DATA(0).bits += 300;
            break;
        case 37:
            PLAYER_DATA(0).bits += 500;
            break;
        }
        if (PLAYER_DATA(0).bits > 999998) {
            PLAYER_DATA(0).bits = 999999;
        }
        SAI_resetSlotMachine();
    }
}

/* the lines bet on, from the 5th down to the 3rd: two diagonals, the top and
   bottom rows, the middle row */
void SAI_judgeSlotLines(void) {
    switch (SLOT.bet) {
    case 3:
        if (REEL_SYMBOL(0, 0) == REEL_SYMBOL(1, 1) && REEL_SYMBOL(2, 2) == REEL_SYMBOL(0, 0)) {
            SLOT.winLine = 5;
            SLOT.prize = REEL_SYMBOL(2, 2);
        }
        if (REEL_SYMBOL(2, 0) == REEL_SYMBOL(1, 1) && REEL_SYMBOL(0, 2) == REEL_SYMBOL(2, 0)) {
            SLOT.winLine = 1;
            SLOT.prize = REEL_SYMBOL(1, 1);
        }
    case 2:
        if (REEL_SYMBOL(0, 0) == REEL_SYMBOL(0, 1) && REEL_SYMBOL(0, 2) == REEL_SYMBOL(0, 0)) {
            SLOT.winLine = 2;
            SLOT.prize = REEL_SYMBOL(0, 2);
        }
        if (REEL_SYMBOL(2, 0) == REEL_SYMBOL(2, 1) && REEL_SYMBOL(2, 2) == REEL_SYMBOL(2, 0)) {
            SLOT.winLine = 4;
            SLOT.prize = REEL_SYMBOL(2, 2);
        }
    case 1:
        if (REEL_SYMBOL(1, 0) == REEL_SYMBOL(1, 1) && REEL_SYMBOL(1, 2) == REEL_SYMBOL(1, 0)) {
            SLOT.winLine = 3;
            SLOT.prize = REEL_SYMBOL(1, 1);
        }
        break;
    }
    if (SLOT.prize != 0) {
        playSoundEffectOnVoice(0x16, 8);
    } else {
        playSoundEffectOnVoice(0x16, 9);
    }
    SLOT.state = 3;
}

/* whether the two reels stopped first show the start of a line bet on */
s32 SAI_isSlotReach(void) {
    switch (SLOT.bet) {
    case 3:
        if (REEL_SYMBOL(0, 0) == REEL_SYMBOL(1, 1)) {
            return 1;
        }
        if (REEL_SYMBOL(2, 0) == REEL_SYMBOL(1, 1)) {
            return 1;
        }
    case 2:
        if (REEL_SYMBOL(0, 0) == REEL_SYMBOL(0, 1)) {
            return 1;
        }
        if (REEL_SYMBOL(2, 0) == REEL_SYMBOL(2, 1)) {
            return 1;
        }
    case 1:
        if (REEL_SYMBOL(1, 0) == REEL_SYMBOL(1, 1)) {
            return 1;
        }
        break;
    }
    return 0;
}

void SAI_spinSlotReels(void) {
    s16 level;
    s32 clicked;
    s8 reel;
    s8 row;

    clicked = 0;
    for (reel = 0; reel < 3; reel++) {
        if ((SLOT.scroll[reel] += 5) >= REEL_ROW_HEIGHT) {
            if (SLOT.stopped[reel] != 0) {
                SLOT.scroll[reel] = REEL_ROW_HEIGHT;
            } else {
                clicked = 1;
                SLOT.scroll[reel] -= REEL_ROW_HEIGHT;
                for (row = 0; row < 4; row++) {
                    if (SLOT.rows[row][reel] == 0) {
                        SLOT.rows[row][reel] = REEL_ROWS - 1;
                    } else {
                        SLOT.rows[row][reel]--;
                    }
                }
            }
        }
    }
    if (SLOT.reach == 0 && SLOT.stopped[0] != 0 && SLOT.stopped[1] != 0) {
        SLOT.reach = 1;
    }
    if (SLOT.reach == 1) {
        SLOT.reach += SAI_isSlotReach();
    }
    if (SLOT.reach == 2) {
        /* the brightness bounces between 96 and 255 */
        if (SLOT.dimming == 0) {
            level = SLOT.brightness + 16;
            if (level >= 255) {
                SLOT.brightness = 510 - level;
                SLOT.dimming = 1;
            } else {
                SLOT.brightness += 16;
            }
        } else {
            level = SLOT.brightness - 16;
            if (level < 97) {
                SLOT.brightness = 192 - level;
                SLOT.dimming = 0;
            } else {
                SLOT.brightness -= 16;
            }
        }
    }
    if (clicked == 1) {
        playSoundEffectOnVoice(0x17, 5);
    }
    if (SLOT.delay++ >= 30) {
        SLOT.delay = 120;
        if (PAD_STATES[0]->rawPressed & PAD_CIRCLE) {
            for (reel = 0; reel < 3; reel++) {
                if (SLOT.stopped[reel] == 0) {
                    playSoundEffectOnVoice(0x16, 7);
                    SLOT.stopped[reel]++;
                    break;
                }
            }
        }
        if (SLOT.scroll[2] == REEL_ROW_HEIGHT && SLOT.stopped[2] != 0) {
            SLOT.reach = 0;
            SLOT.state = 2;
            SLOT.betWindow->state = 1;
            SLOT.helpWindow->state = 1;
            SLOT.bitsWindow->state = 1;
        }
    }
}

void SAI_tickSlotBet(void) {
    if (PAD_STATES[0]->rawPressed & (PAD_UP | PAD_RIGHT)) {
        if (PLAYER_DATA(0).bits >= 100 && SLOT.bet < 3) {
            PLAYER_DATA(0).bits -= 100;
            SLOT.bet++;
            playSoundEffectOnVoice(0x16, 6);
        }
    } else if (PAD_STATES[0]->rawPressed & (PAD_DOWN | PAD_LEFT)) {
        if (SLOT.bet > 0) {
            PLAYER_DATA(0).bits += 100;
            SLOT.bet--;
            playSoundEffectOnVoice(0x16, 6);
        }
    } else if (SLOT.bet != 0 && (PAD_STATES[0]->rawPressed & PAD_CIRCLE)) {
        SLOT.state = 1;
        SLOT.betWindow->state = 2;
        SLOT.helpWindow->state = 2;
        SLOT.bitsWindow->state = 2;
    } else if (PAD_STATES[0]->rawPressed & PAD_CROSS) {
        SLOT.state = 4;
        PLAYER_DATA(0).bits += SLOT.bet * 100;
        SLOT.bet = 0;
        SLOT.running = 0;
    }
}

void SAI_runSlotMachine(s32 unused, s32 parent) {
    void (*tick)(void);

    SAI_initSlotMachine();
    SAI_resetSlotMachine();
    openKanjiPage(0xF, 0xE7);
    clearKanjiPage(0xF);
    addFrameCallback((s32)SAI_renderSlotMachine);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 4, 0);
    waitFrames(30);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_SLOT_BET_WINDOW, getCurrentTaskId());
    SLOT.betWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_SLOT_HELP_WINDOW, getCurrentTaskId());
    SLOT.helpWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &SAI_SLOT_BITS_WINDOW, getCurrentTaskId());
    SLOT.bitsWindow = (JpWindow *)waitFrames(0x7FFFFFFF);
    do {
        waitFrames(1);
        tick = SAI_SLOT_STATES[SLOT.state];
        if (tick != NULL) {
            tick();
        }
    } while (SLOT.running != 0);
    SLOT.betWindow->state = 4;
    SLOT.helpWindow->state = 4;
    SLOT.bitsWindow->state = 4;
    waitFrames(30);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 4, 0);
    waitFrames(30);
    waitFrames(30);
    removeFrameCallback((s32)SAI_renderSlotMachine);
    waitFrames(2);
    clearKanjiPage(0xF);
    closeKanjiPage(0xF);
    freeHeapBlocksByTag(0x3A);
    resumeTask(parent);
}

/* the map menu's Move choice: the map's own screen does it */
void SAI_moveOnMap(void) {
}

void SAI_runWorldMap();

/* not called */
void SAI_startWorldMap(void) {
    spawnTask(0, -1, 0, 0x800, SAI_runWorldMap, 1, 0, 0, 0);
}
