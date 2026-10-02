#include "common.h"
#include "game.h"
#include "dcb/kaw_match_intro.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/duel_launch.h"
#include "dcb/card_db.h"
#include "dcb/vram_upload.h"
#include "dcb/card_render.h"
#include "dcb/menu.h"
#include "dcb/sound_play.h"
#include "dcb/prim_util.h"
#include "dcb/frame_callback.h"
#include "dcb/dialog.h"
#include "dcb/duel_setup.h"
#include "dcb/pad.h"
#include "dcb/sound.h"
#include "dcb/fade.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_tutorial.h"
#include "dcb/kaw_hand.h"
#include "dcb/kaw_hud.h"

extern s32 KAW_MATCH_LOADING;
extern POLY_G4 KAW_DECK_CHART_POLYS[2][2][3];
extern TILE KAW_DECK_LEVEL_BARS[2][2][4];
extern DR_MODE KAW_DECK_CHART_MODES[2][2];

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
    { -0xC0, 0x42, 0x78, 0x42 },
};

s32 KAW_VS_OUTER_LINE_POS[2][4] = {
    { 0x140, 0xB1, 8, 0xB1 },
    { -0xC0, 0x30, 0x78, 0x30 },
};

Menu KAW_DECK_LIST_MENUS[2] = {
    { NULL, NULL, { 24, 40, 132, 56 }, 0, -1, 0, -1, 0xA, 0x81, 132, 12, 1, 1, 2, 1, 0, 14, 0, 0, 0 },
    { NULL, NULL, { 24, 140, 132, 56 }, 0, -1, 0, -1, 0xA, 0x81, 132, 12, 1, 1, 2, 1, 0, 14, 0, 0, 1 },
};

Rect16 KAW_DECK_INFO_RECTS[2] = {
    { 0xBC, 0x1E, 0x76, 0x4C },
    { 0xBC, 0x82, 0x76, 0x4C },
};

/* the online deck of deck 0x8C, as card ids less 0x1D; the code reads 30,
   and the last two bytes are leftovers, not the same in every version */
u8 KAW_DARKNESS_WAVE_ORDER[32] = {
    0x1D, 0xA, 1, 0x16, 4, 0x1C, 0xE, 0x18, 0xB, 0xF, 5, 0x1A, 0x19, 0x10, 0x15, 3,
#if VERSION_US
    0x12, 6, 0x13, 0x17, 0xC, 0x1E, 7, 2, 0x14, 0x11, 0x1B, 0xD, 9, 8, 0x11, 0xE,
#elif VERSION_EU
    0x12, 6, 0x13, 0x17, 0xC, 0x1E, 7, 2, 0x14, 0x11, 0x1B, 0xD, 9, 8, 0xD2, 0,
#else
#error "kawseg/ui/kaw_match_intro: version not checked"
#endif
};

void KAW_drawDeckName(s32 x, s32 y, char *name) {
    char buf[64];

    sprintf(buf, "%s Deck", name);
    drawText(x + 0x18, y + 3, (s32)buf, 7, 1);
    KAW_drawSprite(x, y, 0x1D0, 0xCA, 0xC0, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

void KAW_drawBattleRecord(s32 x, s32 y, s32 wins, s32 losses) {
    char buf[64];

    sprintf(buf, "*s0%4d        %3d      %3d", wins + losses, wins, losses);
    drawSmallText(x + 0x24, y + 9, (s32)"BATTLES", 6, 1);
    drawSmallText(x + 0x66, y + 9, (s32)"WINS", 6, 1);
    drawSmallText(x + 0x9C, y + 9, (s32)"LOSSES", 6, 1);
    drawText(x + 8, y + 3, (s32)buf, 7, 1);
    KAW_drawSprite(x, y, 0x1D0, 0xB8, 0xC0, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
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
        count = 2;
    } else {
        count = 1;
    }
    for (i = 0; i < count; i++) {
        spawnTask(0, -1, 0, 0x800, uploadStringGlyphs, PLAYER_DATA(i).name, i, getCurrentTaskId(), 0);
        waitFrames(0x7FFFFFFF);
    }
    sprintf(path, "B:\\MATCH\\%3.3d.ARC", match);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    width0 = strlen(PLAYER(0)->name) * 16;
    if (isVersus != 0) {
        /* the width of the last image uploaded above, in VRAM halfwords */
        width1 = LOADED_TIM.prect->w * 4;
    } else {
        width1 = strlen(PLAYER(1)->name) * 16;
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
    KAW_VS_INNER_LINE_POS[1][0] = -0xC0;
    KAW_VS_INNER_LINE_POS[1][1] = 0x42;
    KAW_VS_OUTER_LINE_POS[0][0] = 0x140;
    KAW_VS_OUTER_LINE_POS[0][1] = 0xB1;
    KAW_VS_OUTER_LINE_POS[1][0] = -0xC0;
    KAW_VS_OUTER_LINE_POS[1][1] = 0x30;
    waitFrames(10);
    KAW_MATCH_LOADING = 0;
    resumeTask(task);
}

void KAW_drawDeckList(ListWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 player;
    s32 i;
    s32 deck;
    PresetDeck *decks;
    char buf[64];
    s32 top;

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    player = w->player;
    y++;
    if (KAW_MATCH_SCREEN->deckListOpen[player] != 0) {
        decks = (PresetDeck *)(((SessionData *)SESSION_DATA)->npcDeckFile + 8);
        for (i = 0; i < KAW_DECK_LIST_MENUS[player].nrows; i++) {
            if (i < w->window.view.y / KAW_DECK_LIST_MENUS[player].rowH) {
                continue;
            }
            if ((w->window.view.y + w->window.rect.h) / KAW_DECK_LIST_MENUS[player].rowH < i) {
                break;
            }
#if VERSION_EU
            top = w->window.originY + i * KAW_DECK_LIST_MENUS[player].rowH;
            y = top + 1;
#elif VERSION_US
            y = w->window.originY + i * KAW_DECK_LIST_MENUS[player].rowH;
            y++;
#else
#error "kawseg/ui/kaw_match_intro: version not checked"
#endif
            deck = KAW_MATCH_SCREEN->deckIds[player][i];
            if (deck < 3) {
                strcpy(buf, (char *)PLAYER_DATA(player).savedDecks[deck].name);
                strcat(buf, " Deck");
                drawText(x + 2, y, (s32)buf, 7, z);
            } else {
                strcpy(buf, decks[deck - 3].name);
                strcat(buf, " Deck");
                drawText(x + 2, y, (s32)buf, 5, z);
            }
        }
        updateMenuCursor(&KAW_DECK_LIST_MENUS[player]);
    } else {
        for (i = 0; i < 3; i++) {
            drawIcon(x, y + i * 14, 0, i + 7, z);
            if (PLAYER_DATA(player).savedDecks[i].inUse) {
                strcpy(buf, (char *)PLAYER_DATA(player).savedDecks[i].name);
                strcat(buf, " Deck");
            } else {
                strcpy(buf, "Unused Deck");
            }
            drawText(x + 0xE, y + i * 14, (s32)buf, 7, z);
        }
        drawIcon(x, y + 0x2A, 0, 10, z);
        drawText(x + 0xE, y + 0x2A, (s32)"Choose from List", 7, z);
    }
}

#define setXY0(p, _x0, _y0) (p)->x0 = _x0, (p)->y0 = _y0

#define setDrawTPage(p, dfe, dtd, tpage) (setlen(p, 1), ((u32 *)(p))[1] = _get_mode(dfe, dtd, tpage))

void KAW_drawDeckChart(s32 x, s32 y, s32 player, s32 z) {
    u8 counts[6];
    u8 bars[4];
    PresetDeck *decks;
    s32 i;
    s32 specialty;
    s32 level;
    s32 deck;
    s32 cx;
    s32 cy;

    decks = (PresetDeck *)(((SessionData *)SESSION_DATA)->npcDeckFile + 8);
    cx = 0x22;
    cy = 0x1F;
    for (i = 0; i < 6; i++) {
        counts[i] = 2;
    }
    for (i = 0; i < 3; i++) {
        bars[i] = 0;
    }
    deck = KAW_MATCH_SCREEN->deckIds[player][KAW_DECK_LIST_MENUS[player].row];
    for (i = 0; i < 30; i++) {
        if (deck < 3) {
            specialty = getCardSpecialty(PLAYER_DATA(player).savedDecks[deck].cards[i].id);
            level = getCardLevel(PLAYER_DATA(player).savedDecks[deck].cards[i].id);
        } else {
            specialty = getCardSpecialty(decks[deck - 3].cards[i]);
            level = getCardLevel(decks[deck - 3].cards[i]);
        }
        switch (specialty) {
        case 0:
            counts[0]++;
            break;
        case 1:
            counts[3]++;
            break;
        case 2:
            counts[4]++;
            break;
        case 3:
            counts[2]++;
            break;
        case 4:
            counts[1]++;
            break;
        case 5:
        case 6:
            counts[5]++;
            break;
        }
        switch (level) {
        case 0:
            bars[0] += 2;
            break;
        case 2:
            bars[1] += 2;
            break;
        case 3:
            bars[2] += 2;
            break;
        case 4:
        case 5:
            bars[3] += 2;
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        initPrimByType(9, &KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i], 0, 0);
        setRGB1(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i], 0xFF, 0xFF, 0xFF);
        KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i].x1 = x + cx;
        KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i].y1 = y + cy;
    }
    setRGB0(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0], 0xFF, 0, 0);
    setRGB2(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0], 0xFF, 0xFF, 0);
    setRGB3(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0], 0, 0, 0);
    setRGB0(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1], 0, 0, 0);
    setRGB2(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1], 0, 0xFF, 0xFF);
    setRGB3(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1], 0, 0xFF, 0);
    setRGB0(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2], 0, 0xFF, 0);
    setRGB2(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2], 0xFF, 0xFF, 0xFF);
    setRGB3(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2], 0xFF, 0, 0);
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].x0 = x + cx + rsin(0) * counts[0] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].y0 = y + cy + rcos(0) * counts[0] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].x2 = x + cx + rsin(0x2AA) * counts[1] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].y2 = y + cy + rcos(0x2AA) * counts[1] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].x3 = x + cx + rsin(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].y3 = y + cy + rcos(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].x0 = x + cx + rsin(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].y0 = y + cy + rcos(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].x2 = x + cx + rsin(0x7FE) * counts[3] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].y2 = y + cy + rcos(0x7FE) * counts[3] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].x3 = x + cx + rsin(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].y3 = y + cy + rcos(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].x0 = x + cx + rsin(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].y0 = y + cy + rcos(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].x2 = x + cx + rsin(0xD52) * counts[5] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].y2 = y + cy + rcos(0xD52) * counts[5] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].x3 = x + cx + rsin(0) * counts[0] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].y3 = y + cy + rcos(0) * counts[0] / 4096;
    drawIcon(x - 6 + cx + rsin(0) * 26 / 4096, y - 6 + cy + rcos(0) * 26 / 4096, 0, 0, z);
    drawIcon(x - 6 + cx + rsin(0x2AA) * 26 / 4096, y - 6 + cy + rcos(0x2AA) * 26 / 4096, 0, 4, z);
    drawIcon(x - 6 + cx + rsin(0x554) * 26 / 4096, y - 6 + cy + rcos(0x554) * 26 / 4096, 0, 3, z);
    drawIcon(x - 6 + cx + rsin(0x7FE) * 26 / 4096, y - 6 + cy + rcos(0x7FE) * 26 / 4096, 0, 1, z);
    drawIcon(x - 6 + cx + rsin(0xAA8) * 26 / 4096, y - 6 + cy + rcos(0xAA8) * 26 / 4096, 0, 2, z);
    drawIcon(x - 6 + cx + rsin(0xD52) * 26 / 4096, y - 6 + cy + rcos(0xD52) * 26 / 4096, 0, 5, z);
    for (i = 0; i < 3; i++) {
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i]);
    }
    KAW_drawSprite(x + cx - 0x16, y + cy - 0x1A, 0x3E1, 0x19A, 0x2C, 0x32, 0x3F0, 0x1FD, 0, 0, 0, 0x80, z);
    drawIcon(x + 0x44, y + 0x34, 1, 0x10, z);
    drawIcon(x + 0x50, y + 0x34, 1, 0x12, z);
    drawIcon(x + 0x5C, y + 0x34, 1, 0x13, z);
    drawIcon(x + 0x68, y + 0x34, 1, 5, z);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][0], 0xFF, 0xFF, 0);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][1], 0xFF, 0, 0);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][2], 0, 0, 0xFF);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][3], 0xFF, 0xFF, 0xFF);
    for (i = 0; i < 4; i++) {
        SetTile(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i]);
        setXY0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i], x + 0x44 + i * 12, y - (bars[i] - 0x32));
        setWH(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i], 8, bars[i]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i]);
    }
    setDrawTPage(&KAW_DECK_CHART_MODES[player][FRAME_BUFFER_INDEX], 0, 0, 0);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &KAW_DECK_CHART_MODES[player][FRAME_BUFFER_INDEX]);
}

void KAW_drawDeckInfo(ListWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 player;
    s32 deck;
    s32 wins;
    s32 losses;
    char buf[40];

    x = w->window.originX;
    y = w->window.originY + 1;
    z = w->window.z;
    player = w->player;
    KAW_drawDeckChart(x, y, player, z);
    deck = KAW_MATCH_SCREEN->deckIds[player][KAW_DECK_LIST_MENUS[player].row];
    if (deck < 3) {
        wins = ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[deck].wins;
        losses = ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[deck].losses;
    } else {
        wins = ((PlayerProfile *)PLAYER_PROFILES)[player].opponentDeckFlags[deck - 3] & 0x3FFF;
        losses = ((PlayerProfile *)PLAYER_PROFILES)[player].opponentDeckLosses[deck - 3];
    }
    sprintf(buf, "*s0%3d *c6Wins *c7%3d *c6Losses", wins, losses);
    drawText(x + 6, y + 0x3E, (s32)buf, 7, z);
}

void KAW_openDeckList(s32 player) {
    s32 i;
    s32 n;

    markBuildableOpponentDecks(player);
    for (i = 0; i < 0xA2; i++) {
        KAW_MATCH_SCREEN->deckIds[player][i] = 0xFFFF;
    }
    n = 0;
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).savedDecks[i].inUse) {
            KAW_MATCH_SCREEN->deckIds[player][n++] = i;
        }
    }
    for (i = 0; i < 0x9F; i++) {
        if (PLAYER_DATA(player).opponentDeckFlags[i] & 0x4000) {
            KAW_MATCH_SCREEN->deckIds[player][n++] = i + 3;
        }
    }
    KAW_DECK_LIST_MENUS[player].nrows = n;
    KAW_MATCH_SCREEN->lists[player].window.view.h = n * KAW_DECK_LIST_MENUS[player].rowH;
    KAW_MATCH_SCREEN->deckListOpen[player] = 1;
    animateWindowTo(&KAW_MATCH_SCREEN->frames[player].window, &KAW_DECK_INFO_RECTS[player]);
    playSoundEffect(0xA3);
}

void KAW_closeDeckList(s32 i) {
    KAW_DECK_LIST_MENUS[i].nrows = 3;
    KAW_DECK_LIST_MENUS[i].row = 0;
    KAW_MATCH_SCREEN->lists[i].window.scroll[3] = 0;
    KAW_MATCH_SCREEN->lists[i].window.view.y = 0;
    KAW_MATCH_SCREEN->lists[i].window.view.h = KAW_DECK_LIST_MENUS[0].rowH * 3;
    KAW_MATCH_SCREEN->deckListOpen[i] = 0;
    animateWindowTo(&KAW_MATCH_SCREEN->frames[i].window, (Rect16 *)-1);
    playSoundEffect(0xA4);
}

void KAW_renderDeckSelect(void) {
    drawWindow(&KAW_MATCH_SCREEN->lists[0].window, KAW_drawDeckList, 10);
    drawWindow(&KAW_MATCH_SCREEN->frames[0].window, KAW_drawDeckInfo, 10);
    if (KAW_MATCH_SCREEN->mode == 0) {
        drawWindow(&KAW_MATCH_SCREEN->lists[1].window, KAW_drawDeckList, 10);
        drawWindow(&KAW_MATCH_SCREEN->frames[1].window, KAW_drawDeckInfo, 10);
    }
}

#define DECK_CHOICE(p) (PLAYER_DATA(p).deckChoice)

void KAW_runDeckSelect(s32 isVersus, s32 match) {
    s32 i;
    u8 done;
    u16 pressed;

    KAW_MATCH_SCREEN = allocTaskHeapBlock(0x778);
    spawnTask(0, -1, 0, 0x800, KAW_loadMatchGraphics, isVersus, match, getCurrentTaskId(), 0);
    KAW_MATCH_SCREEN->deckListOpen[0] = 0;
    KAW_MATCH_SCREEN->deckListOpen[1] = 0;
    KAW_MATCH_SCREEN->mode = isVersus;
    if ((DUEL->tutorial == 0 && ((SessionData *)SESSION_DATA)->npcDeckIndex[0] == -1) || isVersus == 0) {
        ((SessionData *)SESSION_DATA)->npcDeckIndex[0] = -1;
        ((SessionData *)SESSION_DATA)->npcDeckIndex[1] = -1;
        if (isVersus != 0) {
            openMenu(&KAW_DECK_LIST_MENUS[0], &KAW_MATCH_SCREEN->lists[0].window, &KAW_MATCH_SCREEN->highlights[0], (Bytes4 *)-1);
            KAW_MATCH_SCREEN->lists[0].player = 0;
            KAW_MATCH_SCREEN->lists[0].window.labelPalette = 7;
            KAW_MATCH_SCREEN->lists[0].window.label = (s32) "PLAYER DECK LIST";
            openWindow(&KAW_MATCH_SCREEN->frames[0], &KAW_DECK_INFO_RECTS[0], -1, (s16 *)-1, 8, 0x55, 0x80, 8);
            animateWindowTo(&KAW_MATCH_SCREEN->frames[0].window, (Rect16 *)-1);
            KAW_MATCH_SCREEN->frames[0].window.labelPalette = 8;
            KAW_MATCH_SCREEN->frames[0].player = 0;
            KAW_MATCH_SCREEN->frames[0].window.label = (s32) "PLAYER DECK INFO.";
            done = 2;
        } else {
            for (i = 0, done = 0; i < 2; i++) {
                openMenu(&KAW_DECK_LIST_MENUS[i], &KAW_MATCH_SCREEN->lists[i].window, &KAW_MATCH_SCREEN->highlights[i], (Bytes4 *)-1);
                KAW_MATCH_SCREEN->lists[i].player = i;
                openWindow(&KAW_MATCH_SCREEN->frames[i], &KAW_DECK_INFO_RECTS[i], -1, (s16 *)-1, 8, 0x55, 0x80, 8);
                animateWindowTo(&KAW_MATCH_SCREEN->frames[i].window, (Rect16 *)-1);
                KAW_MATCH_SCREEN->frames[i].window.labelPalette = 8;
                KAW_MATCH_SCREEN->frames[i].player = i;
                if (i == 0) {
                    KAW_MATCH_SCREEN->lists[0].window.label = (s32) "1P DECK LIST";
                    KAW_MATCH_SCREEN->frames[0].window.label = (s32) "1P DECK INFO.";
                } else {
                    KAW_MATCH_SCREEN->lists[i].window.label = (s32) "2P DECK LIST";
                    KAW_MATCH_SCREEN->frames[i].window.label = (s32) "2P DECK INFO.";
                }
                KAW_MATCH_SCREEN->lists[i].window.labelPalette = 7;
            }
        }
        playSoundEffect(0xA3);
        addFrameCallback((s32)KAW_renderDeckSelect);
        waitFrames(0x10);
        do {
            waitFrames(FRAME_INTERVAL);
            if (!(done & 1)) {
                if (KAW_MATCH_SCREEN->deckListOpen[0] != 0) {
                    if (PAD_STATES[0]->pressed & PAD_CROSS) {
                        i = KAW_MATCH_SCREEN->deckIds[0][KAW_DECK_LIST_MENUS[0].row];
                        if (((SessionData *)SESSION_DATA)->deckRuleActive != 0) {
                            if (i < 3) {
                                if (((SessionData *)SESSION_DATA)->deckAllowed[i] == 0) {
                                    playSoundEffect(0xA0);
                                    initDialog(KAW_MATCH_SCREEN->dialog, "This Deck can't be used in this Arena.", 0);
                                    runDialog(KAW_MATCH_SCREEN->dialog);
                                } else {
                                    DECK_CHOICE(0) = i;
                                    done |= 1;
                                }
                            } else {
                                playSoundEffect(0xA0);
                                initDialog(KAW_MATCH_SCREEN->dialog, "Base Deck can't be used in this Arena.", 0);
                                runDialog(KAW_MATCH_SCREEN->dialog);
                            }
                        } else {
                            if (i < 3) {
                                DECK_CHOICE(0) = i;
                            } else {
                                ((SessionData *)SESSION_DATA)->npcDeckIndex[0] = i - 3;
                                DECK_CHOICE(0) = -1;
                            }
                            done |= 1;
                        }
                    } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
                        KAW_closeDeckList(0);
                    }
                } else {
                    pressed = PAD_STATES[0]->pressed;
                    if (pressed & (PAD_TRIANGLE | PAD_CIRCLE | PAD_CROSS | PAD_SQUARE)) {
                        if (pressed & PAD_CIRCLE) {
                            i = 0;
                        } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
                            i = 1;
                        } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
                            i = 2;
                        } else {
                            i = 3;
                        }
                        if (i < 3) {
                            if (((SessionData *)SESSION_DATA)->deckRuleActive != 0 &&
                                ((SessionData *)SESSION_DATA)->deckAllowed[i] == 0) {
                                playSoundEffect(0xA0);
                                initDialog(KAW_MATCH_SCREEN->dialog, "This Deck can't be used in this Arena.", 0);
                                runDialog(KAW_MATCH_SCREEN->dialog);
                                i = -1;
                            }
                            if (i != -1 && PLAYER_DATA(0).savedDecks[i].inUse) {
                                DECK_CHOICE(0) = i;
                                done |= 1;
                            }
                        } else {
                            KAW_openDeckList(0);
                        }
                    }
                }
                if (done & 1) {
                    animateWindowTo(&KAW_MATCH_SCREEN->lists[0].window, (Rect16 *)-1);
                    animateWindowTo(&KAW_MATCH_SCREEN->frames[0].window, (Rect16 *)-1);
                    playSoundEffect(0xA0);
                }
            }
            if (isVersus == 0 && !(done & 2)) {
                if (KAW_MATCH_SCREEN->deckListOpen[1] != 0) {
                    if (PAD_STATES[1]->pressed & PAD_CROSS) {
                        playSoundEffect(0xA0);
                        i = KAW_MATCH_SCREEN->deckIds[1][KAW_DECK_LIST_MENUS[1].row];
                        if (i < 3) {
                            DECK_CHOICE(1) = i;
                        } else {
                            ((SessionData *)SESSION_DATA)->npcDeckIndex[1] = i - 3;
                            DECK_CHOICE(1) = -1;
                        }
                        done |= 2;
                    } else if (PAD_STATES[1]->pressed & PAD_TRIANGLE) {
                        KAW_closeDeckList(1);
                    }
                } else {
                    pressed = PAD_STATES[1]->pressed;
                    if (pressed & (PAD_TRIANGLE | PAD_CIRCLE | PAD_CROSS | PAD_SQUARE)) {
                        if (pressed & PAD_CIRCLE) {
                            if (PLAYER_DATA(1).savedDecks[0].inUse) {
                                DECK_CHOICE(1) = 0;
                                done |= 2;
                            }
                        } else if (PAD_STATES[1]->pressed & PAD_TRIANGLE) {
                            if (PLAYER_DATA(1).savedDecks[1].inUse) {
                                DECK_CHOICE(1) = 1;
                                done |= 2;
                            }
                        } else if (PAD_STATES[1]->pressed & PAD_CROSS) {
                            if (PLAYER_DATA(1).savedDecks[2].inUse) {
                                DECK_CHOICE(1) = 2;
                                done |= 2;
                            }
                        } else {
                            KAW_openDeckList(1);
                        }
                    }
                }
                if (done & 2) {
                    animateWindowTo(&KAW_MATCH_SCREEN->lists[1].window, (Rect16 *)-1);
                    animateWindowTo(&KAW_MATCH_SCREEN->frames[1].window, (Rect16 *)-1);
                    playSoundEffect(0xA0);
                }
            }
        } while (done != 3);
        waitFrames(0x10);
        if (((SessionData *)SESSION_DATA)->npcDeckIndex[0] == -1) {
            *(PlayerDeck *)DUEL_PLAYERS[0] = PLAYER_DATA(0).savedDecks[DECK_CHOICE(0)];
            linkDeckCardData(0, (PlayerDeck *)DUEL_PLAYERS[0]);
        } else {
            loadPresetDeckForPlayer(0);
        }
        if (isVersus == 0) {
            if (((SessionData *)SESSION_DATA)->npcDeckIndex[1] == -1) {
                *(PlayerDeck *)DUEL_PLAYERS[1] = PLAYER_DATA(1).savedDecks[DECK_CHOICE(1)];
                linkDeckCardData(1, (PlayerDeck *)DUEL_PLAYERS[1]);
            } else {
                loadPresetDeckForPlayer(1);
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            DECK_CHOICE(i) = -1;
        }
    }
    removeFrameCallback((s32)KAW_renderDeckSelect);
    markDeckCardsSeen(0);
    freeHeapBlock(((SessionData *)SESSION_DATA)->npcDeckFile);
    waitFrames(0x1E);
    while (KAW_MATCH_LOADING != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    waitFrames(2);
    freeHeapBlock(KAW_MATCH_SCREEN);
    waitFrames(2);
}

#define setUVWH(p, _u0, _v0, _w, _h)                                                            \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u0) + (_w), (p)->v1 = (_v0), (p)->u2 = (_u0), \
    (p)->v2 = (_v0) + (_h), (p)->u3 = (_u0) + (_w), (p)->v3 = (_v0) + (_h)

void KAW_renderVersusScreen(void) {
    char buf[64]; /* unused, but it is in the original stack frame */
    VersusPrims *prims;
    s32 i;

    prims = (VersusPrims *)CURRENT_FRAME_BUFFER->primSlots[15];
    if (KAW_MATCH_SCREEN->introState != 0) {
        switch (KAW_MATCH_SCREEN->introState) {
        case 1:
            KAW_MATCH_SCREEN->unk510 = 0;
            KAW_MATCH_SCREEN->introZoom = 0;
            KAW_MATCH_SCREEN->introBrightness = 0x80;
            KAW_MATCH_SCREEN->introState++;
        case 2:
            KAW_MATCH_SCREEN->introZoom += 10;
            if (KAW_MATCH_SCREEN->introZoom > 150) {
                KAW_MATCH_SCREEN->introState++;
            }
            break;
        case 3:
            KAW_MATCH_SCREEN->introZoom -= 5;
            if (KAW_MATCH_SCREEN->introZoom < 100) {
                KAW_MATCH_SCREEN->introZoom = 100;
                KAW_MATCH_SCREEN->introState++;
            }
            break;
        case 4:
            KAW_MATCH_SCREEN->introBrightness -= 12;
            if (KAW_MATCH_SCREEN->introBrightness <= 0) {
                KAW_MATCH_SCREEN->introBrightness = 0;
                KAW_MATCH_SCREEN->introState = 0;
            }
            break;
        }
        setRGB0(&prims->intro, KAW_MATCH_SCREEN->introBrightness, KAW_MATCH_SCREEN->introBrightness, KAW_MATCH_SCREEN->introBrightness);
        setXYWH(&prims->intro, 160 - (KAW_MATCH_SCREEN->introZoom * 64) / 100, 120 - (KAW_MATCH_SCREEN->introZoom * 64) / 100,
                (KAW_MATCH_SCREEN->introZoom * 128) / 100, (KAW_MATCH_SCREEN->introZoom * 128) / 100);
        setUVWH(&prims->intro, 0x60, 0x28, 0x80, 0x80);
        prims->intro.tpage = 0x26;
        prims->intro.clut = 0x3E19;
        addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->intro);
    }
    i = (KAW_MATCH_SCREEN->barH * 192) / 40;
    setRGB0(&prims->fade, i, i, i);
    setXYWH(&prims->fade, 0, 120 - KAW_MATCH_SCREEN->barH, 320, KAW_MATCH_SCREEN->barH * 2);
    setlen(&prims->fadeMode, 1);
    prims->fadeMode.code[0] = 0xE1000040;
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->fade);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->fadeMode);
    setRGB0(&prims->bars[0], 0xFF, 0xFF, 0);
    setXYWH(&prims->bars[0], 160 - KAW_MATCH_SCREEN->barW, 119 - KAW_MATCH_SCREEN->barH, KAW_MATCH_SCREEN->barW * 2, 1);
    setRGB0(&prims->bars[1], 0xFF, 0xFF, 0);
    setXYWH(&prims->bars[1], 160 - KAW_MATCH_SCREEN->barW, KAW_MATCH_SCREEN->barH + 120, KAW_MATCH_SCREEN->barW * 2, 1);
    setlen(&prims->barMode, 1);
    prims->barMode.code[0] = 0xE1000000;
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->bars[0]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->bars[1]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->barMode);
    if (KAW_MATCH_SCREEN->logoShown != 0) {
        setRGB0(&prims->logo, 0x80, 0x80, 0x80);
        setXYWH(&prims->logo, 160 - (KAW_MATCH_SCREEN->logoScale * 32) / 100, 120 - KAW_MATCH_SCREEN->logoScale / 5,
                (KAW_MATCH_SCREEN->logoScale * 64) / 100, (KAW_MATCH_SCREEN->logoScale * 40) / 100);
        setUVWH(&prims->logo, 0x60, 0, 0x40, 0x28);
        prims->logo.tpage = 6;
        prims->logo.clut = 0x3E18;
        addPrim(&CURRENT_FRAME_BUFFER->ot[1], &prims->logo);
    }
    for (i = 0; i < 2; i++) {
        KAW_drawCard3D(&KAW_MATCH_SCREEN->cards[i], 1, &prims->cards[i]);
    }
    i = (((PlayerProfile *)PLAYER_PROFILES)->playTime * 8) % 256;
    if (i >= 0x80) {
        KAW_MATCH_SCREEN->pulse = 0x17F - i;
    } else {
        KAW_MATCH_SCREEN->pulse = i + 0x80;
    }
    i = 0;
    KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1C0, PLAYER(i)->nameWidth, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 1);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 8, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x78, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 0x20, KAW_VS_PANEL_POS[i][1], 0x180, 0, 0x20, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x80, KAW_VS_PANEL_POS[i][1], 0x188, 0, 8, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
    KAW_drawDeckName(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], (char *)DUEL_PLAYERS[i] + 1);
    KAW_drawBattleRecord(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
    i = 1;
    KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1E0, PLAYER(i)->nameWidth, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 1);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 6, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x7A, KAW_VS_PANEL_POS[i][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] - 8, KAW_VS_PANEL_POS[i][1], 0x18A, 0, 8, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x80, KAW_VS_PANEL_POS[i][1], 0x18C, 0, 0x20, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
    KAW_drawDeckName(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], (char *)DUEL_PLAYERS[i] + 1);
    KAW_drawBattleRecord(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
    KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
    if (KAW_MATCH_SCREEN->timer > 0x20) {
        if (KAW_MATCH_SCREEN->cards[KAW_MATCH_SCREEN->chosen - 2].u != 0) {
            KAW_drawSprite(KAW_VS_PANEL_POS[0][0] + 0x4C, KAW_VS_PANEL_POS[0][1] + 0x4C, 0x1B4, 0, 0x30, 0x20, 0x190, 0xFD, 0, 1, 1, 0x80, 3);
            KAW_drawSprite(KAW_VS_PANEL_POS[1][0] + 4, KAW_VS_PANEL_POS[1][1] + 4, 0x1A8, 0, 0x30, 0x20, 0x190, 0xFC, 0, 1, 1, 0x80, 3);
        } else {
            KAW_drawSprite(KAW_VS_PANEL_POS[0][0] + 0x4C, KAW_VS_PANEL_POS[0][1] + 0x4C, 0x1A8, 0, 0x30, 0x20, 0x190, 0xFC, 0, 1, 1, 0x80, 3);
            KAW_drawSprite(KAW_VS_PANEL_POS[1][0] + 4, KAW_VS_PANEL_POS[1][1] + 4, 0x1B4, 0, 0x30, 0x20, 0x190, 0xFD, 0, 1, 1, 0x80, 3);
        }
    }
}

/* libgte's setVector */
#define setVector(v, _x, _y, _z) (v)->vx = (_x), (v)->vy = (_y), (v)->vz = (_z)

void KAW_runVersusIntro(s32 mode, s32 deckId) {
    s32 i;
    s32 j;
    s32 frame;
    s32 step;
    s32 k;
    char buf[64]; /* unused, but it is in the original stack frame */

    KAW_MATCH_SCREEN = allocTaskHeapBlock(sizeof(DeckScreen));
    waitForMusicChange();
    if (mode != 0) {
        loadMusicTrack(0, ((u8 *)SESSION_DATA)[0x70], 0x7F);
        loadMusicTrack(1, ((u8 *)SESSION_DATA)[0x71], 0x64);
    } else {
        loadMusicTrack(0, rand() % 2 + 0x8F, 0x7F);
        loadMusicTrack(1, rand() % 2 + 0x93, 0x64);
    }
    playLoadedMusic(0);
    spawnTask(0, -1, 0, 0x1000, loadDuelCardGraphics, mode, getCurrentTaskId(), 0, 0);
    frame = 0;
    if (mode != 0) {
        k = getBaseDeckId(deckId);
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleWins = ((PlayerProfile *)PLAYER_PROFILES)->comLosses[k];
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleLosses = ((PlayerProfile *)PLAYER_PROFILES)->comWins[k];
    }
    for (i = 0; i < 2; i++) {
        if (mode != 0) {
            KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
            KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
        } else {
            KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
            KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
        }
    }
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[15] = (s32)&KAW_MATCH_SCREEN->prims[i];
        initPrimByType(0xC, &KAW_MATCH_SCREEN->prims[i].intro, 1, 0);
        initPrimByType(0xC, &KAW_MATCH_SCREEN->prims[i].logo, 1, 0);
        initPrimByType(8, &KAW_MATCH_SCREEN->prims[i].fade, 1, 0);
        for (j = 0; j < 2; j++) {
            initPrimByType(8, &KAW_MATCH_SCREEN->prims[i].bars[j], 0, 0);
        }
    }
    j = rand() % 2;
    for (i = 0; i < 2; i++) {
        KAW_MATCH_SCREEN->cards[i].code = 0x2C;
        setRGB0(&KAW_MATCH_SCREEN->cards[i], 0x80, 0x80, 0x80);
        if (KAW_DUEL->tutorial) {
            k = 1;
        } else {
            k = i ^ j;
        }
        KAW_MATCH_SCREEN->cards[i].tpage = ((k * 10 + 0x180) & 0x3FF) >> 6;
        KAW_MATCH_SCREEN->cards[i].clut = ((k + 0xFA) << 6) | 0x19;
        KAW_MATCH_SCREEN->cards[i].u = (k * 10 + 0x180) % 64 * 4;
        KAW_MATCH_SCREEN->cards[i].v = 0x70;
        setVector(&KAW_MATCH_SCREEN->cards[i].pos, i * 400 - 200, 0, 0);
        setVector(&KAW_MATCH_SCREEN->cards[i].rot, 0x2000, 0x2800 - (i << 12), 0x2000);
        PLAYER(i)->shufflePasses = 0;
    }
    KAW_MATCH_SCREEN->mode = mode;
    KAW_MATCH_SCREEN->deckId = deckId;
    KAW_MATCH_SCREEN->introState = 0;
    KAW_MATCH_SCREEN->logoShown = 0;
    KAW_MATCH_SCREEN->logoScale = 0;
    KAW_MATCH_SCREEN->choice = 0;
    KAW_MATCH_SCREEN->pulse = 0;
    KAW_MATCH_SCREEN->chosen = 0;
    KAW_MATCH_SCREEN->timer = 0;
    KAW_MATCH_SCREEN->barW = 0;
    KAW_MATCH_SCREEN->barH = 0;
    KAW_MATCH_SCREEN->cursor = (s16 *)KAW_createCursor(1, 0x12, 0x16, 6, 1);
    addFrameCallback((s32)KAW_renderVersusScreen);
    step = KAW_DUEL->tutorial;
    do {
        waitFrames(FRAME_INTERVAL);
        frame++;
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
            KAW_MATCH_SCREEN->introState = 1;
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
        if (frame > 70) {
            if (KAW_MATCH_SCREEN->timer < 60) {
                if ((KAW_MATCH_SCREEN->barH += 2) > 35) {
                    KAW_MATCH_SCREEN->barH = 35;
                }
            } else {
                if ((KAW_MATCH_SCREEN->barH -= 2) < 0) {
                    KAW_MATCH_SCREEN->barH = 0;
                }
            }
        }
        if (frame > 20) {
            if ((KAW_MATCH_SCREEN->barW += 16) > 160) {
                KAW_MATCH_SCREEN->barW = 160;
            }
        }
        if (KAW_MATCH_SCREEN->timer == 80) {
            KAW_MATCH_SCREEN->introState = 1;
            playSoundEffect(0x83);
        }
        if (KAW_MATCH_SCREEN->timer > 80) {
            KAW_MATCH_SCREEN->logoShown = 1;
            if (KAW_MATCH_SCREEN->timer < 92) {
                KAW_MATCH_SCREEN->logoScale += 10;
            } else {
                if ((KAW_MATCH_SCREEN->logoScale -= 5) < 100) {
                    KAW_MATCH_SCREEN->logoScale = 100;
                }
            }
        }
        if (frame == 80) {
            playSoundEffect(0xA5);
        }
        if (KAW_MATCH_SCREEN->timer == 51) {
            playSoundEffect(0xA5);
        }
        if (frame > 80) {
            if (KAW_MATCH_SCREEN->chosen >= 2) {
                i = KAW_MATCH_SCREEN->chosen - 2;
                if (KAW_MATCH_SCREEN->cards[i].rot.vy != 0x2000) {
                    if (KAW_MATCH_SCREEN->cards[i].rot.vy < 0x2000) {
                        KAW_MATCH_SCREEN->cards[i].rot.vy += 0x40;
                    } else {
                        KAW_MATCH_SCREEN->cards[i].rot.vy -= 0x40;
                    }
                }
                if (KAW_MATCH_SCREEN->timer >= 50) {
                    if (step == 2) {
                        KAW_showTutorialMessage(0x34, "It looks like I go first!");
                        PAD_INPUT_ENABLED = 1;
                        step = 3;
                    }
                    for (i = 0; i < 2; i++) {
                        if (abs(KAW_MATCH_SCREEN->cards[i].pos.vx) >= 200) {
                            if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                                KAW_MATCH_SCREEN->cards[i].pos.vx = -200;
                            } else {
                                KAW_MATCH_SCREEN->cards[i].pos.vx = 200;
                            }
                        } else if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                            KAW_MATCH_SCREEN->cards[i].pos.vx -= 10;
                        } else {
                            KAW_MATCH_SCREEN->cards[i].pos.vx += 10;
                        }
                    }
                }
            } else {
                for (i = 0; i < 2; i++) {
                    if (abs(KAW_MATCH_SCREEN->cards[i].pos.vx) <= 80) {
                        if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                            KAW_MATCH_SCREEN->cards[i].pos.vx = -80;
                        } else {
                            KAW_MATCH_SCREEN->cards[i].pos.vx = 80;
                        }
                        KAW_MATCH_SCREEN->chosen = 1;
                    } else if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                        KAW_MATCH_SCREEN->cards[i].pos.vx += 8;
                    } else {
                        KAW_MATCH_SCREEN->cards[i].pos.vx -= 8;
                    }
                }
            }
        }
        if (KAW_MATCH_SCREEN->chosen == 1) {
            if (step == 1) {
                KAW_showTutorialMessage(0x34, "Let's decide who gets 1st Turn.\nChoose a Card with the directional\nbuttons and press the *b2 button.");
                PAD_INPUT_ENABLED = 1;
                step = 2;
            }
            if ((u16)PAD_STATES[0]->pressed & PAD_RIGHT) {
                if (KAW_MATCH_SCREEN->choice == 0) {
                    playSoundEffect(0xA2);
                    KAW_MATCH_SCREEN->choice = 1;
                }
            }
            if ((u16)(PAD_STATES[0]->pressed & PAD_LEFT)) {
                if (KAW_MATCH_SCREEN->choice == 1) {
                    playSoundEffect(0xA2);
                    KAW_MATCH_SCREEN->choice = 0;
                }
            }
            KAW_drawCursorAt(KAW_MATCH_SCREEN->cursor, KAW_MATCH_SCREEN->choice * 160 + 0x4F, 0x78);
            if (PAD_STATES[0]->pressed & PAD_CROSS) {
                playSoundEffect(0xA6);
                KAW_MATCH_SCREEN->chosen = KAW_MATCH_SCREEN->choice + 2;
                KAW_MATCH_SCREEN->timer = 1;
            }
        }
        if (KAW_MATCH_SCREEN->timer != 0) {
            KAW_MATCH_SCREEN->timer++;
        }
    } while (!DUEL_VRAM_READY || KAW_MATCH_SCREEN->timer < 181);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    waitFrames(40);
    if (!KAW_DUEL->tutorial) {
        for (i = 0; i < 2; i++) {
            PLAYER(i)->shufflePasses += 600;
            shuffleOnlineDeck(i);
        }
        if (deckId == 0x8C) {
            for (i = 0; i < 30; i++) {
                PLAYER(1)->onlineDeck[i] = KAW_DARKNESS_WAVE_ORDER[i] + 0x1D;
            }
        }
    }
    if (KAW_MATCH_SCREEN->cards[KAW_MATCH_SCREEN->chosen - 2].u != 0) {
        DUEL->turnPlayer = 1;
    } else {
        DUEL->turnPlayer = 0;
    }
    removeFrameCallback((s32)KAW_renderVersusScreen);
    KAW_freeCursor(KAW_MATCH_SCREEN->cursor);
    waitFrames(2);
    freeHeapBlock(KAW_MATCH_SCREEN);
    waitFrames(2);
}
