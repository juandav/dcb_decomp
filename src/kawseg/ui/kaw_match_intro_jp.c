#include "common.h"
#include "game.h"
#include "dcb/kaw_match_intro.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/text.h"
#include "dcb/vram_upload.h"
#include "dcb/card_render.h"
#include "dcb/card_zones.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_hud.h"
#include "dcb/pad.h"
#include "dcb/player_data.h"
#include "dcb/scroll_bg.h"
#include "dcb/sound_play.h"

/* jp's match intro (kaw_match_intro.c is us's and eu's): no deck choice;
   instead the players pick Agumon's or Betamon's panel for the first turn
   while the match's graphics load, and the versus screen draws itself in
   its own loop, with no cards to pick but a shuffle count each player
   raises with the directional buttons */

extern s32 KAW_MATCH_LOADING;
extern s32 DUEL_VRAM_READY;

/* the turn order choice's step (0 pick, 1 flashing, 2 decided) and menu,
   and the hand cursor it shares with the card prize */
extern s32 KAW_TURN_ORDER_STEP;
extern ChoiceMenu KAW_TURN_ORDER_MENU;
extern void *KAW_HAND_CURSOR;

/* the versus screen's yellow bar and its draw mode, and its VS logo; the
   flash before the logo uses the result screen's polygons */
extern POLY_F4 KAW_VS_BAR_POLYS[2];
extern DR_MODE KAW_VS_BAR_MODES[2];
extern POLY_FT4 KAW_VS_LOGO_POLYS[2];
extern POLY_FT4 KAW_RESULT_WINNER_POLYS[2];

/* jp's executable's blinking hand cursor */
void *KAW_createCursor(s32, s32, s32, s32, s32);
void KAW_freeCursor(void *cursor);
void KAW_drawCursorAt(void *cursor, s32 x, s32 y);

char *strcpy(char *dst, const char *src);
char *strcat(char *dst, const char *src);
void loadDuelCardGraphics();

s32 KAW_VS_PANEL_POS[2][4] = {
    { 0x64, 0xF1, 0xB8, 0x79 },
    { 0x4C, -0x71, 8, 7 },
};

s32 KAW_VS_NAME_POS[2][4] = {
    { 0x140, 0xC3, 8, 0xC3 },
    { -0x100, 0x10, 0, 0x10 },
};

s32 KAW_VS_INNER_LINE_POS[2][4] = {
    { 0x140, 0x9F, 8, 0x9F },
    { -0x88, 0x42, 0xB0, 0x42 },
};

s32 KAW_VS_OUTER_LINE_POS[2][4] = {
    { 0x140, 0xB1, 8, 0xB1 },
    { -0x88, 0x30, 0xB0, 0x30 },
};

void KAW_drawDeckName(s32 x, s32 y, char *name) {
    char buf[24];

    sprintf(buf, "%sデック", name);
    drawIconText(x + 0x18, y + 3, 7, 1, 1, (s32)buf);
    KAW_drawSprite(x, y, 0x1D0, 0xCA, 0x88, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

void KAW_drawBattleRecord(s32 x, s32 y, s32 wins, s32 losses) {
    char buf[16];

    sprintf(buf, "w-1%4d", wins + losses);
    drawText(x + 8, y + 3, (s32)buf, 7, 0);
    sprintf(buf, "w-1%3d", wins);
    drawText(x + 0x38, y + 3, (s32)buf, 7, 0);
    sprintf(buf, "w-1%3d", losses);
    drawText(x + 0x61, y + 3, (s32)buf, 7, 0);
    KAW_drawSprite(x, y, 0x1D0, 0xB8, 0x88, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

void KAW_loadMatchGraphics(s32 isVersus, s32 match, s32 task) {
    char path[64];
    s32 count;
    s32 i;
    u32 *arc;
    s32 width0;
    s32 width1;

    KAW_MATCH_LOADING = 1;
    if (isVersus == 0) {
        match = 999;
        count = 11;
    } else {
        count = 12;
        spawnTask(0, -1, 0, 0x800, uploadStringGlyphs, PLAYER_PROFILES, 0, getCurrentTaskId(), 0);
        waitFrames(0x7FFFFFFF);
    }
    sprintf(path, "B:\\MATCH\\%3.3d.ARC", match);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < count; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    width0 = strlen(PLAYER(0)->name);
    width0 = width0 / 2 * 32;
    if (isVersus != 0) {
        /* the width of the last image uploaded above, in VRAM halfwords */
        width1 = LOADED_TIM.prect->w * 4;
    } else {
        /* a Shift JIS glyph is two bytes, 32 pixels wide on the VS screen */
        width1 = strlen(PLAYER(1)->name);
        width1 = width1 / 2 * 32;
    }
    PLAYER(0)->nameWidth = width0;
    PLAYER(1)->nameWidth = width1;
    KAW_VS_PANEL_POS[0][0] = 100;
    KAW_VS_PANEL_POS[0][1] = 0xF1;
    KAW_VS_PANEL_POS[0][2] = 0xB8;
    KAW_VS_PANEL_POS[0][3] = 0x79;
    KAW_VS_PANEL_POS[1][0] = 0x4C;
    KAW_VS_PANEL_POS[1][1] = -0x71;
    KAW_VS_PANEL_POS[1][2] = 8;
    KAW_VS_PANEL_POS[1][3] = 7;
    KAW_VS_NAME_POS[0][0] = 0x140;
    KAW_VS_NAME_POS[0][1] = 0xC3;
    KAW_VS_NAME_POS[1][0] = -width1;
    KAW_VS_NAME_POS[1][1] = 0x10;
    KAW_VS_NAME_POS[1][2] = 0x138 - width1;
    KAW_VS_INNER_LINE_POS[0][0] = 0x140;
    KAW_VS_INNER_LINE_POS[0][1] = 0x9F;
    KAW_VS_INNER_LINE_POS[1][0] = -0x88;
    KAW_VS_INNER_LINE_POS[1][1] = 0x42;
    KAW_VS_OUTER_LINE_POS[0][0] = 0x140;
    KAW_VS_OUTER_LINE_POS[0][1] = 0xB1;
    KAW_VS_OUTER_LINE_POS[1][0] = -0x88;
    KAW_VS_OUTER_LINE_POS[1][1] = 0x30;
    waitFrames(10);
    KAW_MATCH_LOADING = 0;
    resumeTask(task);
}

/* the turn order window has nothing to update each frame */
void KAW_tickTurnOrderWindow(void) {
}

/* the turn order window's text, for each step of the choice */
void KAW_drawTurnOrderText(UiWindow *window) {
    char text[0x58];

    switch (KAW_TURN_ORDER_STEP) {
    case 0:
        strcpy(text, "c7ターンの順番を決めます。\nc6アグモンc7、c6ベタモンc7どちらかのパネルをえらんでね。");
        break;
    case 1:
        strcpy(text, "c7ターンの順番を決定w2中……");
        break;
    case 2:
        strcpy(text, PLAYER(DUEL->turnPlayer)->name);
        strcat(text, "c7のターンに決定！！");
        break;
    }
    drawIconText(window->originX + 0x10, window->originY + 8, 5 - DUEL->turnPlayer, 1, window->z, (s32)text);
}

/* Loads the match's graphics while the player picks Agumon's or Betamon's
   panel; the panels flash in turn, slowing down, and the one they stop on
   says who plays first */
void KAW_runTurnOrderChoice(s32 isVersus, s32 match) {
    WindowSpec spec = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 10, 0, KAW_drawTurnOrderText, KAW_tickTurnOrderWindow };
    WindowTask *window;
    s32 frame;
    s32 slide;
    s32 chosen;
    s32 lit;
    s32 ticks;
    s32 period;

    KAW_TURN_ORDER_STEP = 0;
    spawnTask(0, -1, 0, 0x800, KAW_loadMatchGraphics, isVersus, match, getCurrentTaskId(), 0);
    openChoiceMenu(&KAW_TURN_ORDER_MENU, 0x3E, 0x32, NULL, NULL);
    spawnTask(0, -1, 0, 0x800, runWindowTask, &spec, getCurrentTaskId());
    window = waitFrames(0x7FFFFFFF);
    waitFrames(0x1E);
    frame = 0;
    slide = -0xA0;
    chosen = 0;
    playSoundEffect(0xA5);
    KAW_HAND_CURSOR = KAW_createCursor(1, 0xE, 0xE, 4, 1);
    do {
        frame++;
        waitFrames(FRAME_INTERVAL);
        slide += 6;
        if (slide > 0) {
            slide = 0;
        }
        KAW_drawSprite(slide + 0x78, 0x64, 0x3F0, 0x1D0, 0x20, 0x20, 0x3F0, 0x1F0, 0, 0, 0, 0x80, 0);
        KAW_drawSprite(0xA8 - slide, 0x64, 0x3F8, 0x1D0, 0x20, 0x20, 0x3F0, 0x1F0, 0, 0, 0, 0x80, 0);
        if (frame > 60) {
            if ((PAD_STATES[0]->rawPressed & PAD_RIGHT) && chosen == 0) {
                playSoundEffect(0xA2);
                chosen = 1;
            }
            if ((PAD_STATES[0]->rawPressed & PAD_LEFT) && chosen == 1) {
                playSoundEffect(0xA2);
                chosen = 0;
            }
        }
        if (frame > 60) {
            KAW_drawCursorAt(KAW_HAND_CURSOR, chosen * 0x30 + 0x88, 0x74);
        }
    } while (slide != 0 || !(PAD_STATES[0]->rawPressed & PAD_CIRCLE));
    playSoundEffect(0xA0);
    frame = 0;
    ticks = 0;
    lit = rand() % 2;
    KAW_TURN_ORDER_STEP = 1;
    do {
        frame++;
        waitFrames(FRAME_INTERVAL);
        period = abs(frame * 2 - 200) / 20;
        ticks++;
        if (period < 4) {
            period = 4;
        }
        if (ticks % period == 0) {
            playSoundEffect(0xA2);
            ticks = 0;
            lit ^= 1;
        }
        KAW_drawSprite(slide + 0x78, 0x64, 0x3F0, 0x1D0, 0x20, 0x20, 0x3F0, 0x1F0, 0, 0, 0, lit * 0x60 + 0x20, 0);
        KAW_drawSprite(0xA8 - slide, 0x64, 0x3F8, 0x1D0, 0x20, 0x20, 0x3F0, 0x1F0, 0, 0, 0, (lit ^ 1) * 0x60 + 0x20, 0);
        KAW_drawSprite(chosen * 0x30 + 0x78, 0x5C, 0x3F8, 0x1B8, 0x20, 8, 0x3F0, 0x1F1, 0, 0, 0, 0x80, 0);
        /* the other panel's mark: the CPU's, or the other player's */
        if (isVersus == 0) {
            KAW_drawSprite((chosen ^ 1) * 0x30 + 0x78, 0x5C, 0x3F8, 0x1C0, 0x20, 8, 0x3F0, 0x1F1, 0, 0, 0, 0x80, 0);
        } else {
            KAW_drawSprite((chosen ^ 1) * 0x30 + 0x78, 0x5C, 0x3F8, 0x1C8, 0x20, 8, 0x3F0, 0x1F1, 0, 0, 0, 0x80, 0);
        }
    } while (frame < 0x227);
    playSoundEffect(0xA0);
    frame = 0;
    if (DUEL->tutorial) {
        lit = chosen;
    }
    if (chosen == lit) {
        DUEL->turnPlayer = 1;
    } else {
        DUEL->turnPlayer = 0;
    }
    KAW_TURN_ORDER_STEP = 2;
    do {
        frame++;
        waitFrames(FRAME_INTERVAL);
        KAW_drawSprite(slide + 0x78, 0x64, 0x3F0, 0x1D0, 0x20, 0x20, 0x3F0, 0x1F0, 0, 0, 0, ((lit * (frame % 8)) << 7) + 0x20, 0);
        KAW_drawSprite(0xA8 - slide, 0x64, 0x3F8, 0x1D0, 0x20, 0x20, 0x3F0, 0x1F0, 0, 0, 0, (((lit ^ 1) * (frame % 8)) << 7) + 0x20, 0);
        KAW_drawSprite(chosen * 0x30 + 0x78, 0x5C, 0x3F8, 0x1B8, 0x20, 8, 0x3F0, 0x1F1, 0, 0, 0, 0x80, 0);
        /* the other panel's mark: the CPU's, or the other player's */
        if (isVersus == 0) {
            KAW_drawSprite((chosen ^ 1) * 0x30 + 0x78, 0x5C, 0x3F8, 0x1C0, 0x20, 8, 0x3F0, 0x1F1, 0, 0, 0, 0x80, 0);
        } else {
            KAW_drawSprite((chosen ^ 1) * 0x30 + 0x78, 0x5C, 0x3F8, 0x1C8, 0x20, 8, 0x3F0, 0x1F1, 0, 0, 0, 0x80, 0);
        }
    } while (frame < 0x79 && !(PAD_STATES[0]->rawPressed & PAD_CIRCLE));
    playSoundEffect(0xA5);
    window->state = 4;
    waitFrames(0x1E);
    while (KAW_MATCH_LOADING != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    freeScrollingBackground();
    KAW_freeCursor(KAW_HAND_CURSOR);
    waitFrames(10);
}

#define setDrawTPage(p, dfe, dtd, tpage) (setlen(p, 1), ((u32 *)(p))[1] = _get_mode(dfe, dtd, tpage))

#define setUVWH(p, _u0, _v0, _w, _h)                                                            \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u0) + (_w), (p)->v1 = (_v0), (p)->u2 = (_u0), \
    (p)->v2 = (_v0) + (_h), (p)->u3 = (_u0) + (_w), (p)->v3 = (_v0) + (_h)

/* The versus screen before a duel: the two panels slide in with the
   players' names, decks and records, a flash and the VS logo zoom in, and
   each player shuffles their deck by pressing the directional buttons until
   the cards are loaded and Circle is pressed (or 300 frames pass) */
void KAW_runVersusIntro(s32 mode, s32 deckId) {
    char buf[64];
    s32 wins[2];
    s32 losses[2];
    s32 nameWidth0;
    s32 nameWidth1;
    s32 frame;
    s32 pulse;
    s32 barW;
    s32 logoScale;
    s32 introZoom;
    s32 i;

    nameWidth0 = PLAYER(0)->nameWidth;
    nameWidth1 = PLAYER(1)->nameWidth;
    frame = 0;
    barW = 0;
    logoScale = 0;
    introZoom = 0;
    for (i = 0; i < 2; i++) {
        if (mode != 0) {
            wins[i] = ((ProfileK *)PLAYER_PROFILES)[i].battleWins;
            losses[i] = ((ProfileK *)PLAYER_PROFILES)[i].battleLosses;
        } else {
            wins[i] = ((ProfileK *)PLAYER_PROFILES)[i].versusWins;
            losses[i] = ((ProfileK *)PLAYER_PROFILES)[i].versusLosses;
        }
    }
    spawnTask(0, -1, 0, 0x1000, loadDuelCardGraphics, mode, getCurrentTaskId(), 0, 0);
    for (i = 0; i < 2; i++) {
        PLAYER(i)->shufflePasses = 0;
    }
    do {
        waitFrames(FRAME_INTERVAL);
        for (i = 0; i < 2; i++) {
            if (PLAYER(i)->controller != 2 && (PAD_STATES[i]->rawPressed & (PAD_UP | PAD_RIGHT | PAD_DOWN | PAD_LEFT))) {
                PLAYER(i)->shufflePasses += 10;
            }
        }
        i = (frame * 8) % 256;
        if (i >= 0x80) {
            pulse = 0x17F - i;
        } else {
            pulse = i + 0x80;
        }
        frame++;
        if (frame > 20) {
            if ((barW += 16) > 160) {
                barW = 160;
            }
            initPrimByType(8, &KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX], 0, 0);
            setRGB0(&KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX], 0xFF, 0xFF, 0);
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].x0 = 160 - barW;
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].y0 = 0x77;
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].x1 = barW + 160;
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].y1 = 0x77;
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].x2 = 160 - barW;
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].y2 = 0x79;
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].x3 = barW + 160;
            KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX].y3 = 0x79;
            setDrawTPage(&KAW_VS_BAR_MODES[FRAME_BUFFER_INDEX], 0, 0, 0);
            addPrim(&CURRENT_FRAME_BUFFER->ot[5], &KAW_VS_BAR_POLYS[FRAME_BUFFER_INDEX]);
            addPrim(&CURRENT_FRAME_BUFFER->ot[5], &KAW_VS_BAR_MODES[FRAME_BUFFER_INDEX]);
        }
        for (i = 0; i < 2; i++) {
            if (frame > 0) {
                STEP_TOWARD(KAW_VS_PANEL_POS[i][1], KAW_VS_PANEL_POS[i][3], 12);
            }
            if (frame > 20) {
                STEP_TOWARD(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][2], 8);
            }
            if (frame > 30) {
                STEP_TOWARD(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][2], 24);
            }
            if (frame > 40) {
                STEP_TOWARD(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][2], 24);
            }
            if (frame > 50) {
                STEP_TOWARD(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][2], 24);
            }
        }
        if (frame == 8) {
            playSoundEffect(0x83);
        }
        if (frame == 26) {
            playSoundEffect(0x8D);
            playSoundEffect(0x8D);
        }
        if (frame == 30) {
            playSoundEffect(0xA7);
        }
        if (frame == 40) {
            playSoundEffect(0xA7);
        }
        if (frame == 50) {
            playSoundEffect(0xA7);
        }
        if (frame > 8 && frame < 25) {
            if (frame < 22) {
                introZoom += 10;
            } else if ((introZoom -= 5) < 100) {
                introZoom = 100;
            }
            initPrimByType(0xC, &KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], 0, 0);
            setRGB0(&KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], 0x80, 0x80, 0x80);
            setXYWH(&KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], 160 - (introZoom * 64) / 100, 120 - (introZoom * 64) / 100,
                    (introZoom * 128) / 100, (introZoom * 128) / 100);
            setUVWH(&KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX], 0x60, 0x28, 0x80, 0x80);
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].tpage = 6;
            KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX].clut = 0x3E19;
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &KAW_RESULT_WINNER_POLYS[FRAME_BUFFER_INDEX]);
        }
        if (frame > 70) {
            if (frame < 82) {
                logoScale += 10;
            } else if ((logoScale -= 5) < 100) {
                logoScale = 100;
            }
            initPrimByType(0xC, &KAW_VS_LOGO_POLYS[FRAME_BUFFER_INDEX], 0, 0);
            setRGB0(&KAW_VS_LOGO_POLYS[FRAME_BUFFER_INDEX], 0x80, 0x80, 0x80);
            setXYWH(&KAW_VS_LOGO_POLYS[FRAME_BUFFER_INDEX], 160 - (logoScale * 32) / 100, 120 - logoScale / 5,
                    (logoScale * 64) / 100, (logoScale * 40) / 100);
            setUVWH(&KAW_VS_LOGO_POLYS[FRAME_BUFFER_INDEX], 0x60, 0, 0x40, 0x28);
            KAW_VS_LOGO_POLYS[FRAME_BUFFER_INDEX].tpage = 6;
            KAW_VS_LOGO_POLYS[FRAME_BUFFER_INDEX].clut = 0x3E18;
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &KAW_VS_LOGO_POLYS[FRAME_BUFFER_INDEX]);
        }
        i = 0;
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1C0, nameWidth0, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 0);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 8, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, pulse, 2);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x78, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, pulse, 2);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 0x20, KAW_VS_PANEL_POS[i][1], 0x180, 0, 0x20, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x80, KAW_VS_PANEL_POS[i][1], 0x188, 0, 8, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
        KAW_drawDeckName(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], PLAYER(i)->deck->name);
        KAW_drawBattleRecord(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], wins[i], losses[i]);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
        if (PLAYER(i)->shufflePasses != 0) {
            sprintf(buf, "w-1%5d", PLAYER(i)->shufflePasses);
            drawText(KAW_VS_PANEL_POS[i][0] + 0x58, KAW_VS_PANEL_POS[i][1] + 0x64, (s32)buf, 7, 0);
        }
        i = 1;
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1E0, nameWidth1, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 0);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 6, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, pulse, 2);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x7A, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, pulse, 2);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 8, KAW_VS_PANEL_POS[i][1], 0x18A, 0, 8, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x80, KAW_VS_PANEL_POS[i][1], 0x18C, 0, 0x20, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
        KAW_drawDeckName(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], PLAYER(i)->deck->name);
        if (mode == 0) {
            KAW_drawBattleRecord(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], wins[i], losses[i]);
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
        if (PLAYER(i)->shufflePasses != 0) {
            sprintf(buf, "w-1%d", PLAYER(i)->shufflePasses);
            drawText(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 0x64, (s32)buf, 7, 0);
        }
    } while (!DUEL_VRAM_READY || (!(PAD_STATES[0]->rawPressed & PAD_CIRCLE) && frame < 301));
    for (i = 0; i < 2; i++) {
        PLAYER(i)->shufflePasses += 600;
        if (!DUEL->tutorial) {
            shuffleOnlineDeck(i);
        }
    }
}
