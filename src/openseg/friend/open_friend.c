#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/scroll_bg.h"
#include "dcb/sound_play.h"
#include "dcb/fade.h"
#include "dcb/sound.h"
#include "dcb/openseg.h"
#include "dcb/open_trade.h"
#include "dcb/open_registration.h"
#include "dcb/open_title.h"
#include "dcb/open_memcard.h"

extern s32 OPEN_FRIEND_MENU_SHOWN;
extern s32 OPEN_TRADE_ENABLED;
extern PlayerWindow OPEN_PLAYER_RECORD_WINDOWS[2];
extern s32 OPEN_FRIEND_MENU_DONE;

char *strcat(char *dst, const char *src);
void runDeckEditorFromFriendMenu(s32 player);
void runPartnerEquipmentFromFriendMenu(s32 player);
void startVersusDuel(void);
void runTitleMenu(void);

void OPEN_drawFriendMenu(void) {
    s32 i;
    s32 same;
    s32 clutY;

    if (OPEN_FRIEND_MENU_SHOWN == 1) {
        OPEN_PANEL_X += 8;
        if (OPEN_PANEL_X > 8) {
            OPEN_PANEL_X = 8;
        }
    } else {
        OPEN_PANEL_X -= 8;
        if (OPEN_PANEL_X < -140) {
            OPEN_PANEL_X = -140;
        }
    }
    OPEN_drawSprite(OPEN_PANEL_X, OPEN_PANEL_Y, 0x300, 0, 0x8C, 0x96, 0x300, 0x96, 0, 1, 1, 0x80, 2);
    same = (u16)PLAYER_DATA(0).profileId == (u16)PLAYER_DATA(1).profileId;
    if ((OPEN_TRADE_ENABLED & 3) != 3) {
        same = 1;
    }
    for (i = 0; i < 7; i++) {
        if (*((s8 *)SESSION_DATA + 0x1028) == i) {
            clutY = 0x98;
            if (i == 1 && same) {
                clutY = 0x99;
            }
        } else {
            clutY = 0x97;
            if (i == 1 && same) {
                clutY = 0x9A;
            }
        }
        OPEN_drawSprite(OPEN_PANEL_X + 0x19, OPEN_PANEL_Y + 0x16 + i * 16, 0x323, i * 16, 0x58, 0x10, 0x300, clutY, 0, 1, 0, 0x80, 2);
    }
}

void OPEN_drawPlayerRecord(PlayerWindow *window) {
    /* its own copy: a "Name" literal would be merged with the one in OPEN_drawSaveSummary */
    static const char nameLabel[] = "Name";
    char text[64];
    s32 total;
    s32 i;
    s32 x;
    s32 y;
    s32 z;

    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    total = 0;
    for (i = 0; i < 301; i++) {
        total += getOwnedCardCount(window->player, i);
    }
    drawText(x + 1, y, (s32)nameLabel, 6, z);
    drawText(x + 0x43, y, (s32)PLAYER_DATA(window->player).name, 7, z);
    drawText(x + 1, y + 12, (s32)"2 Player Battle", 6, z);
    sprintf(text, "*s0%3d    %3d", PLAYER_DATA(window->player).versusWins, PLAYER_DATA(window->player).versusLosses);
    drawText(x + 0x52, y + 12, (s32)text, 7, z);
    drawSmallText(x + 0x65, y + 19, (s32)"WINS", 6, z);
    drawSmallText(x + 0x90, y + 19, (s32)"LOSSES", 6, z);
    countSeenCards(window->player);
    drawText(x + 1, y + 24, (s32)"Cards in Stock.", 6, z);
    i = PLAYER_DATA(window->player).seenCardCount * 1000 / 301;
    sprintf(text, "*s0%3d.%1d*w4*c6%%", i / 10, i % 10);
    drawText(x + 0x67, y + 24, (s32)text, 7, z);
    drawText(x + 1, y + 36, (s32)"Cards in Possession.", 6, z);
    sprintf(text, "*s0%4d", total);
    drawText(x + 0x6D, y + 36, (s32)text, 7, z);
    drawSmallText(x + 0x87, y + 0x2B, (s32)"CARDS", 6, z);
    drawText(x + 1, y + 0x30, (s32)"Deck", 6, z);
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(window->player).savedDecks[i].inUse) {
            strcpy(text, (char *)PLAYER_DATA(window->player).savedDecks[i].name);
            strcat(text, "Deck");
        } else {
            strcpy(text, "Unused Deck");
        }
        drawText(x + 0x37, y + (i + 4) * 12, (s32)text, 7, z);
    }
    if (PLAYER_DATA(window->player).tradeUnlocked) {
        drawIcon(x + 8, y + 0x44, 2, 11, z);
    }
}

void OPEN_drawFriendScreen(void) {
    s32 i;

    OPEN_drawFriendMenu();
    for (i = 0; i < 2; i++) {
        drawWindow(&OPEN_PLAYER_RECORD_WINDOWS[i].window, OPEN_drawPlayerRecord, 4);
    }
    if (OPEN_FRIEND_MENU_SHOWN == 0) {
        OPEN_CORNER_Y += 16;
        if (OPEN_CORNER_Y > 240) {
            OPEN_CORNER_Y = 240;
        }
        OPEN_SIDEBAR_X -= 8;
        if (OPEN_SIDEBAR_X < -96) {
            OPEN_SIDEBAR_X -= 96;
        }
        OPEN_FRAME_Y += 16;
        if (OPEN_FRAME_Y > 240) {
            OPEN_FRAME_Y = 240;
        }
    } else {
        OPEN_CORNER_Y -= 16;
        if (OPEN_CORNER_Y < 16) {
            OPEN_CORNER_Y = 16;
        }
        OPEN_SIDEBAR_X += 8;
        if (OPEN_SIDEBAR_X > 0) {
            OPEN_SIDEBAR_X = 0;
        }
        OPEN_FRAME_Y -= 16;
        if (OPEN_FRAME_Y < 20) {
            OPEN_FRAME_Y = 20;
        }
    }
    OPEN_TITLE_PART_COUNT = 0;
    OPEN_drawTitlePart(OPEN_CORNER_X, OPEN_CORNER_Y, 0);
    OPEN_drawTitlePart(OPEN_CORNER_X, OPEN_CORNER_Y, 1);
    OPEN_drawTitlePart(OPEN_CORNER_X, OPEN_CORNER_Y, 2);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 10);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 11);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 12);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 13);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 14);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 15);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 16);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 17);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 18);
    OPEN_drawTitlePart(OPEN_FRAME_X, OPEN_FRAME_Y, 19);
    OPEN_drawTitlePart(OPEN_SIDEBAR_X, OPEN_SIDEBAR_Y, 21);
}

void OPEN_openPlayerRecordWindows(void) {
    Rect16 to;
    Rect16 from;
    s32 i;

    OPEN_CORNER_X = 12;
    OPEN_CORNER_Y = 240;
    OPEN_SIDEBAR_X = -96;
    OPEN_SIDEBAR_Y = 0;
    OPEN_FRAME_X = 14;
    OPEN_FRAME_Y = 240;
    OPEN_PANEL_X = -140;
    OPEN_PANEL_Y = 80;
    OPEN_FRIEND_MENU_SHOWN = 1;
    OPEN_FRIEND_MENU_DONE = 0;
    for (i = 0; i < 2; i++) {
        from.x = 140;
        from.y = i * 100 + 40;
        from.w = 172;
        from.h = 84;
        to.x = 320;
        to.y = i * 100 + 40;
        to.w = 172;
        to.h = 84;
        openWindow(&OPEN_PLAYER_RECORD_WINDOWS[i].window, &from, (s32)&to, (s16 *)-1, 8, 0x25, 0x80, 0xC);
        OPEN_PLAYER_RECORD_WINDOWS[i].player = i;
        if (i == 0) {
            OPEN_PLAYER_RECORD_WINDOWS[0].window.label = (s32)"PLAYER 1";
        } else {
            OPEN_PLAYER_RECORD_WINDOWS[1].window.label = (s32)"PLAYER 2";
        }
    }
}

void OPEN_closePlayerRecordWindows(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 2; i++) {
        rect.x = 320;
        rect.y = i * 100 + 40;
        rect.w = 172;
        rect.h = 84;
        animateWindowTo(&OPEN_PLAYER_RECORD_WINDOWS[i].window, &rect);
    }
}

void OPEN_runBattleWithFriend(void) {
    Rect16 rect;
    u8 dialog[0xB8];
    u32 *arc;
    char *message;
    s32 i;
    s32 ok;

    changeScrollingBackground(7, 0x380, 0, 0x380, 0x80);
    loadMusicTrack(0, 0x6D, 0x7F);
    playLoadedMusic(0);
    i = 0;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\FRIEND.ARC", getCurrentTaskId());
    arc = (u32 *)waitFrames(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    OPEN_openPlayerRecordWindows();
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawFriendScreen);
    do {
        waitFrames(FRAME_INTERVAL);
        OPEN_TRADE_ENABLED = PLAYER_DATA(0).tradeUnlocked | (PLAYER_DATA(1).tradeUnlocked << 1);
        if (PAD_STATES[0]->pressed & 0x40) {
            switch (((SessionView *)SESSION_DATA)->menuRow) {
            case 1:
                message = NULL;
                if ((u16)PLAYER_DATA(0).profileId == (u16)PLAYER_DATA(1).profileId) {
                    message = "You can't trade the same Data!";
                } else if (!(OPEN_TRADE_ENABLED & 3)) {
                    message = "Trade is disabled.";
                } else if (!(OPEN_TRADE_ENABLED & 1)) {
                    message = "Player 1's trade is disabled.";
                } else if (!(OPEN_TRADE_ENABLED & 2)) {
                    message = "Player 2's trade is disabled.";
                }
                if (message == NULL) {
                    playMenuSound(4);
                    OPEN_FRIEND_MENU_SHOWN = 0;
                    OPEN_closePlayerRecordWindows();
                    waitFrames(20);
                    removeFrameCallback((s32)OPEN_drawFriendScreen);
                    spawnTask(0, -1, 0, 0x800, OPEN_runCardTrade, getCurrentTaskId(), 0, 0, 0);
                    waitFrames(0x7FFFFFFF);
                    OPEN_openPlayerRecordWindows();
                    addFrameCallback((s32)OPEN_drawFriendScreen);
                } else {
                    playMenuSound(1);
                    initDialog(dialog, message, 0);
                    runDialog(dialog);
                }
                break;
            case 2:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                waitFrames(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                runDeckEditorFromFriendMenu(0);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            case 3:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                waitFrames(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                runDeckEditorFromFriendMenu(1);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            case 4:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                waitFrames(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                runPartnerEquipmentFromFriendMenu(0);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            case 5:
                playMenuSound(4);
                OPEN_FRIEND_MENU_SHOWN = 0;
                OPEN_closePlayerRecordWindows();
                waitFrames(20);
                removeFrameCallback((s32)OPEN_drawFriendScreen);
                runPartnerEquipmentFromFriendMenu(1);
                OPEN_openPlayerRecordWindows();
                addFrameCallback((s32)OPEN_drawFriendScreen);
                break;
            default:
                playMenuSound(4);
                OPEN_FRIEND_MENU_DONE = 1;
                break;
            }
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playMenuSound(4);
            ((SessionView *)SESSION_DATA)->menuRow = 6;
            OPEN_FRIEND_MENU_DONE = 1;
        } else if (PAD_STATES[0]->repeat & 0x1000) {
            playMenuSound(2);
            ((SessionView *)SESSION_DATA)->menuRow--;
        } else if (PAD_STATES[0]->repeat & 0x4000) {
            playMenuSound(2);
            ((SessionView *)SESSION_DATA)->menuRow++;
        }
        ((SessionView *)SESSION_DATA)->menuRow = (((SessionView *)SESSION_DATA)->menuRow + 7) % 7;
        if (OPEN_FRIEND_MENU_DONE != 0) {
            OPEN_FRIEND_MENU_SHOWN = 2;
            OPEN_closePlayerRecordWindows();
            if (((SessionView *)SESSION_DATA)->menuRow == 6) {
                initDialog((u8 *)&OPEN_DIALOG, "Save \"Battle with Friend\" game?", 1);
                runDialog(&OPEN_DIALOG);
                ok = 1;
                switch (OPEN_DIALOG.choice) {
                case 1:
                    ok = OPEN_saveFriendGame();
                    break;
                case 0:
                case 2:
                    initDialog((u8 *)&OPEN_DIALOG, "Return to the Title Screen?", 1);
                    runDialog(&OPEN_DIALOG);
                    if (OPEN_DIALOG.choice == 1) {
                        ok = 0;
                    }
                    break;
                }
                if (ok) {
                    playMenuSound(3);
                    OPEN_FRIEND_MENU_SHOWN = 1;
                    OPEN_FRIEND_MENU_DONE = 0;
                    for (i = 0; i < 2; i++) {
                        rect.x = 0x8C;
                        rect.y = i * 100 + 0x28;
                        rect.w = 0xAC;
                        rect.h = 0x54;
                        animateWindowTo(&OPEN_PLAYER_RECORD_WINDOWS[i].window, &rect);
                    }
                }
            }
        }
    } while (OPEN_FRIEND_MENU_DONE == 0);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    waitFrames(20);
    removeFrameCallback((s32)OPEN_drawFriendScreen);
    hideScrollingBackground();
    stopScreenFade();
    switch (((SessionView *)SESSION_DATA)->menuRow) {
    case 0:
        spawnTask(0, -1, 0, 0x200, startVersusDuel, 0, 0, 0, 0);
        break;
    case 6:
        spawnTask(0, -1, 0, 0x100, runTitleMenu, 0, 0, 0, 0);
        break;
    }
}

s32 OPEN_setPartnerObtainedFlag(s32 kind) {
    s32 id;
    s32 word;
    s32 shift;
    s32 bit;

    if (kind == 0) {
        id = 0x126;
    } else if (kind == 1) {
        id = 0x12A;
    } else if (kind == 2) {
        id = 0x12D;
    } else {
        id = 0x126;
    }
    id -= 12;
    word = id / 32;
    shift = id % 32;
    ((PlayerProfile *)PLAYER_PROFILES)->areaScriptFlags[word] |= bit = 1 << shift;
    return (((PlayerProfile *)PLAYER_PROFILES)->areaScriptFlags[word] & bit) != 0;
}
