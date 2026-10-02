#include "common.h"
#include "game.h"
#include "dcb/sai_player_data.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/memcard.h"
#include "dcb/prim.h"
#include "dcb/menu.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"
#include "dcb/sai_labels.h"
#include "dcb/sai_data.h"

extern char *STR_TAMER_RANKS[8];
extern char *STR_COLLECTOR_RANKS[8];
extern UiWindow SAI_PLAYER_DATA_WINDOW;
extern UiWindow SAI_STATS_HINT_WINDOW;
extern CursorHighlight SAI_PLAYER_DATA_CURSOR;

void SAI_drawPlayerData(UiWindow *win);
void SAI_drawCompleteStatsHint(UiWindow *window);
void SAI_drawPlayerDataWindows(void);

/* the three icons drawn for each partner */
const u8 SAI_PARTNER_ARMOR_ICONS[6][3] = {
    { 1, 2, 9 },
    { 3, 5, 0 },
    { 4, 6, 0 },
    { 7, 6, 0 },
    { 8, 1, 0 },
    { 1, 7, 0 },
};

/* not referenced by any code: "後藤豪太" in Shift-JIS */
char SAI_UNUSED_PLAYER_NAME[24] = "\x8C\xE3\x93\xA1\x8D\x8B\x91\xBE";

/* the cursor of each row of SAI_PLAYER_DATA_MENU */
Rect16 SAI_PLAYER_DATA_CURSOR_RECTS[15] = {
    { 0x64, 0, 0x18, 0xE },
    { 0x64, 0xE, 0x3C, 0xE },
    { 0x64, 0, 0x48, 0xE },
    { 0x64, 0xE, 0x30, 0xE },
    { 0x64, 0, 0x60, 0xE },
    { 0x64, 0xE, 0x60, 0xE },
    { 0x15, 0, 0x60, 0xE },
    { 0x15, 0xE, 0x48, 0xE },
    { 0x15, 0, 0x30, 0xE },
    { 0x15, 0xE, 0x30, 0xE },
    { 0x15, 0, 0x30, 0xE },
    { 0x15, 0xE, 0x90, 0xE },
    { 0x15, 0, 0x18, 0xE },
    { 0x15, 0xE, 0x3C, 0xE },
    { 0x64, 0, 0x18, 0xE },
};

/* the labels of the player's data screen */
char *SAI_PLAYER_DATA_LABELS[15] = {
    "Name",
    "Battle Title",
    "Collector Title",
    "Game Completion",
    "Card Collection",
    "Digi-Parts Stock",
    "COM Battle Stats",
    "2P Battle Stats",
    "Deck 1",
    "Deck 2",
    "Deck 3",
    "Partner Cards & Digi-Eggs",
    "Wins",
    "Losses",
    "Deck",
};

void SAI_computePlayerStats(void) {
    s32 i;

    SAI_PLAYER_STATS.profile = (PlayerProfile *)PLAYER_PROFILES;
    SAI_PLAYER_STATS.tamerRank = STR_TAMER_RANKS[SAI_PLAYER_STATS.profile->tamerRank];
    SAI_PLAYER_STATS.collectorRank = STR_COLLECTOR_RANKS[SAI_PLAYER_STATS.profile->collectorRank];
    SAI_PLAYER_STATS.cardRate = 0;
    SAI_PLAYER_STATS.abilityRate = 0;
    SAI_PLAYER_STATS.state = 2;
    SAI_PLAYER_STATS.completionRate = (u16)SAI_PLAYER_STATS.profile->completionPoints * 1000 / 166;
    for (i = 0; i < 301; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[i] & 0x40) {
            SAI_PLAYER_STATS.cardRate += 1000;
        }
    }
    SAI_PLAYER_STATS.cardRate /= 301;
    for (i = 0; i < 128; i++) {
        if (getPartnerAbilityState(0, i)) {
            SAI_PLAYER_STATS.abilityRate += 1000;
        }
    }
    SAI_PLAYER_STATS.abilityRate /= 128;
}

void SAI_runPlayerData(void) {
    SAI_computePlayerStats();
    if (SAI_SCRIPT[0]->regs[15] != 0) {
        SAI_openWindows(&SAI_STATS_HINT_WINDOW, &SAI_STATS_HINT_WINDOW_DEF, 1);
    }
    openMenu(&SAI_PLAYER_DATA_MENU, &SAI_PLAYER_DATA_WINDOW, &SAI_PLAYER_DATA_CURSOR, (Bytes4 *)-1);
    SAI_PLAYER_DATA_WINDOW.label = (s32)"PLAYER'S DATA";
    waitFrames(1);
    playSoundEffect(3);
    addFrameCallback((s32)SAI_drawPlayerDataWindows);
    while (1) {
        waitFrames(1);
        if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
            break;
        }
        if ((PAD_STATES[0]->pressed & PAD_CIRCLE) && SAI_SCRIPT[0]->regs[15] != 0) {
            SAI_SCRIPT[0]->regs[0] = 0;
            SESSION->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
            SESSION->resumeMode = 2;
            SAI_AREA.exitArg = 2;
            SAI_AREA.exitAction = AREA_EXIT_TITLE_OR_ENDING;
            break;
        }
    }
    playSoundEffect(4);
    animateWindowTo(&SAI_PLAYER_DATA_WINDOW, (Rect16 *)-1);
    if (SAI_SCRIPT[0]->regs[15] != 0) {
        animateWindowTo(&SAI_STATS_HINT_WINDOW, (Rect16 *)-1);
    }
    waitFrames(15);
    removeFrameCallback((s32)SAI_drawPlayerDataWindows);
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    SAI_AREA.rewardBusy = 0;
    SAI_PLAYER_STATS_STATE = 0;
}

void SAI_drawPlayerDataWindows(void) {
    drawWindow(&SAI_PLAYER_DATA_WINDOW, SAI_drawPlayerData, 0x18);
    if (SAI_SCRIPT[0]->regs[15] != 0) {
        drawWindow(&SAI_STATS_HINT_WINDOW, SAI_drawCompleteStatsHint, 0x18);
    }
}

s32 SAI_tickPlayerDataMenu(Menu *menu) {
    UiWindow *win;
    CursorHighlight *highlight;
    s16 target[4];

    win = menu->win;
    highlight = menu->cursor;
    menu->moved = 0;
    if (menu->active != 0) {
        highlight->brightness = 0x80;
        if (menu->nrows | menu->rowH) {
            if (PAD_STATES[menu->pad]->repeat & (PAD_UP | PAD_DOWN | PAD_L2 | PAD_R2)) {
                playMenuSound(2);
            }
            if (PAD_STATES[menu->pad]->repeat & PAD_UP) {
                menu->moved = 1;
                if (--menu->row < 0) {
                    scrollWindowTo(&win->originX, 0, win->view.h - win->rect.h);
                    menu->row = 11;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    if (menu->row * menu->rowH < win->scroll[3]) {
                        scrollWindowTo(&win->originX, 0, menu->row * menu->rowH);
                    }
                }
            } else if (PAD_STATES[menu->pad]->repeat & PAD_DOWN) {
                menu->moved = 1;
                if (++menu->row >= 12) {
                    scrollWindowTo(&win->originX, 0, 0);
                    menu->row = 0;
                } else if (menu->row == 11) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, (menu->row + 13) * menu->rowH - win->rect.h);
                } else if (menu->row * menu->rowH >= win->scroll[3] + win->rect.h) {
                    scrollWindowTo(&win->originX, 0, (menu->row + 1) * menu->rowH - win->rect.h);
                }
            } else if (PAD_STATES[menu->pad]->repeat & PAD_L2) {
                menu->moved = 1;
                menu->row -= (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row < 0) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, 0);
                    menu->row = 0;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo(&win->originX, 0, win->scroll[3] - (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                }
            } else if (PAD_STATES[menu->pad]->repeat & PAD_R2) {
                menu->moved = 1;
                menu->row += (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row >= menu->nrows) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, win->view.h - win->rect.h);
                    menu->row = 11;
                } else {
                    if (menu->row == menu->nrows - 1) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo(&win->originX, 0, win->scroll[3] + (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                    if (menu->row >= 12) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                        menu->row = 11;
                    }
                }
            }
        }
    } else {
        highlight->brightness = 0x40;
    }
    if (menu->row != menu->prevRow || menu->col != menu->prevCol) {
        menu->col = SAI_PLAYER_DATA_CURSOR_RECTS[menu->row].x - 10;
        menu->colW = 1;
        menu->prevCol = menu->col;
        menu->prevRow = menu->row;
        target[0] = (win->rect.x - win->scroll[2]) + menu->ox + menu->col * menu->colW - 10;
        target[1] = (win->rect.y - win->scroll[3]) + menu->oy + menu->row * menu->rowH;
        target[2] = measureText(SAI_PLAYER_DATA_LABELS[menu->row]);
        target[3] = menu->ch;
        moveCursorHighlight(menu->cursor, (Rect16 *)target);
    }
    drawCursorHighlight(highlight, win->z);
    return menu->col + menu->row * menu->ncols;
}

void SAI_drawPlayerData(UiWindow *win) {
    /* not literals: GCC would share SAI_drawRewardCard's identical string */
    static const char rateFormat[] = "*s0%3d.%1d%%";
    static const char countFormat[] = "*s0%3d";
    Rect16 uv;
    char buf[0x48];
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 j;
    s32 partner;

    x = win->originX - 0x14;
    y = win->originY + 2;
    z = win->z;
    drawText(x + 0x62, y, (s32)SAI_PLAYER_DATA_LABELS[0], 6, z);
    drawText(x + 0x132 - measureText(SAI_PLAYER_STATS.profile->name), y, (s32)SAI_PLAYER_STATS.profile->name, 7, z);
    drawText(x + 0x62, y + 0xE, (s32)SAI_PLAYER_DATA_LABELS[1], 6, z);
    drawText(x + 0x132 - measureText(SAI_PLAYER_STATS.tamerRank), y + 0xE, (s32)SAI_PLAYER_STATS.tamerRank, 7, z);
    drawText(x + 0x62, y + 0x1C, (s32)SAI_PLAYER_DATA_LABELS[2], 6, z);
    drawText(x + 0x132 - measureText(SAI_PLAYER_STATS.collectorRank), y + 0x1C, (s32)SAI_PLAYER_STATS.collectorRank, 7, z);
    drawText(x + 0x62, y + 0x2A, (s32)SAI_PLAYER_DATA_LABELS[3], 6, z);
    sprintf(buf, rateFormat, SAI_PLAYER_STATS.completionRate / 10, SAI_PLAYER_STATS.completionRate % 10);
    drawText(x + 0x105, y + 0x2A, (s32)buf, 7, z);
    drawText(x + 0x62, y + 0x38, (s32)SAI_PLAYER_DATA_LABELS[4], 6, z);
    sprintf(buf, rateFormat, SAI_PLAYER_STATS.cardRate / 10, SAI_PLAYER_STATS.cardRate % 10);
    drawText(x + 0x105, y + 0x38, (s32)buf, 7, z);
    drawText(x + 0x62, y + 0x46, (s32)SAI_PLAYER_DATA_LABELS[5], 6, z);
    sprintf(buf, rateFormat, SAI_PLAYER_STATS.abilityRate / 10, SAI_PLAYER_STATS.abilityRate % 10);
    drawText(x + 0x105, y + 0x46, (s32)buf, 7, z);
    drawText(x + 0x15, y + 0x54, (s32)SAI_PLAYER_DATA_LABELS[6], 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->battleWins);
    drawText(x + 0x89, y + 0x54, (s32)buf, 7, z);
    drawText(x + 0xA3, y + 0x54, (s32)"Wins", 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->battleLosses);
    drawText(x + 0xC0, y + 0x54, (s32)buf, 7, z);
    drawText(x + 0xDB, y + 0x54, (s32)"Losses", 6, z);
    drawText(x + 0x15, y + 0x62, (s32)SAI_PLAYER_DATA_LABELS[7], 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->versusWins);
    drawText(x + 0x89, y + 0x62, (s32)buf, 7, z);
    drawText(x + 0xA3, y + 0x62, (s32)"Wins", 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->versusLosses);
    drawText(x + 0xC0, y + 0x62, (s32)buf, 7, z);
    drawText(x + 0xDB, y + 0x62, (s32)"Losses", 6, z);
    for (i = 0; i < 3; i++) {
        drawText(x + 0x15, y + (i + 8) * 14, (s32)SAI_PLAYER_DATA_LABELS[i + 8], 6, z);
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            sprintf(buf, "%s %s", ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].name, SAI_PLAYER_DATA_LABELS[14]);
            drawText(x + 0x46, y + (i + 8) * 14, (s32)buf, 7, z);
            sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].wins);
            drawText(x + 0xCA, y + (i + 8) * 14, (s32)buf, 7, z);
            drawText(x + 0xE2, y + (i + 8) * 14, (s32)SAI_PLAYER_DATA_LABELS[12], 6, z);
            sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].losses);
            drawText(x + 0xFE, y + (i + 8) * 14, (s32)buf, 7, z);
            drawText(x + 0x116, y + (i + 8) * 14, (s32)SAI_PLAYER_DATA_LABELS[13], 6, z);
        }
    }
    drawText(x + 0x15, y + 0x9A, (s32)SAI_PLAYER_DATA_LABELS[11], 6, z);
    x += 0xC;
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
            drawText(x + 0x15 + i * 84, y + 0xA8, (s32)((PlayerProfile *)PLAYER_PROFILES)->partners[i].card[0].name, 7, z);
            drawLargeText(x + 0x15 + i * 84, y + 0xB9, (s32)"RANK", 6, z);
            sprintf(buf, "%2d", (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level);
            drawText(x + 0x3D + i * 84, y + 0xB6, (s32)buf, 7, z);
            for (j = 0; j < 3; j++) {
                if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].unlockedArmors[j] != 0) {
                    partner = getSlotPartnerIndex(0, i);
                    drawIcon(x + 0x1D + i * 84 + j * 13, y + 0xC4, 0, SAI_PARTNER_ARMOR_ICONS[partner][j] + 0x1B, z);
                }
            }
        }
    }
    uv.x = 0;
    uv.y = 0x82;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(x + 0xC, y + 0xC, &uv, 0x8B, 0x3EA0, z, 0x80, -1);
    SAI_tickPlayerDataMenu(&SAI_PLAYER_DATA_MENU);
}

void SAI_drawCompleteStatsHint(UiWindow *window) {
    char buf[0x48]; /* unused, but it is in the original stack frame */

    drawText(window->originX + 6, window->originY + 1, (s32)"*b0:Player's Complete Stats", 7, window->z);
}
