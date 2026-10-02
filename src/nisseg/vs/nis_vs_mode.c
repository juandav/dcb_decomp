#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/pad.h"
#include "dcb/prim_desc.h"
#include "dcb/nisseg.h"

/* The VS mode screen: the two players' names and records, and its menu;
   trading needs two different saves that both have their 100 cards. And
   the main menus, which only run their menus. */

extern NisWindow *NIS_VS_WINDOW;
extern u8 NIS_TRADE_BLOCKED;
extern u8 NIS_SAME_SAVE;
extern u8 VS_MODE_MENU[];
extern u8 D_8007E7E4[];
extern u8 D_8007E7D8[];
extern u8 MAIN_MENU[];
extern u8 D_801E46D8[];

void NIS_runVsMode(void);
void NIS_openVsWindow(NisWindowDef *def);
void NIS_closeNothing(NisWindow *window);
void NIS_drawVsRecords(NisWindow *window);
void NIS_runTradeBlocked(void);
void NIS_drawTradeBlockedReason(NisWindow *window);
void NIS_drawSameSave(NisWindow *window);
void NIS_runMainMenuTask(void);

void NIS_loadTimFile(char *path) {
    u32 *tims;

#if JP_DEBUG_BUILD
    spawnTask(0, -1, 4, 0x800, loadFile, path, getCurrentTaskId());
#else
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
#endif
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims != NULL) {
        uploadTimList(tims);
        freeHeapBlock(tims);
    }
}

void NIS_runVsMode(void) {
    NisMenu menu;
    NisWindowDef def = { { 0x17, 0x9F, 0, 0xC }, { 0x17, 0x9F, 0x112, 0x43 }, 0xA, 0, NIS_drawVsRecords, NIS_closeNothing };
    NisProfile *profiles;

    NIS_loadTimFile("D:\\VSMODE.TIM");
    openChoiceMenuFromList(&menu, VS_MODE_MENU, 0);
    NIS_openVsWindow(&def);
    playMusic(0, 4, 0x7F);
    NIS_TRADE_BLOCKED = 0;
    NIS_SAME_SAVE = 0;
    NIS_STATE->otherPad = 1;
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
#if JP_DEBUG_BUILD
        /* R1 lets both players trade or not, and the debug text says which */
        if (PAD_STATES[0]->rawPressed & PAD_R1) {
            NIS_PROFILE(0)->tradeUnlocked = !NIS_PROFILE(0)->tradeUnlocked;
            NIS_PROFILE(1)->tradeUnlocked = NIS_PROFILE(0)->tradeUnlocked;
        }
        sprintf(DEBUG_TEXT_LINES[0], "PUSH R1 TRADE = %d\n", NIS_PROFILE(1)->tradeUnlocked);
#endif
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        SCROLLING_BACKGROUND->unk1BE = 2;
        NIS_VS_WINDOW->state = 4;
        waitFrames(0x1E);
        closeKanjiPage(0xF);
        switch (menu.choice) {
        case 3:
            if (((NisProfile *)PLAYER_PROFILES)[0].profileId == ((NisProfile *)PLAYER_PROFILES)[1].profileId) {
                NIS_SAME_SAVE = 1;
            }
            if (!((NisProfile *)PLAYER_PROFILES)[0].tradeUnlocked) {
                NIS_TRADE_BLOCKED += 1;
            }
            if (!((NisProfile *)PLAYER_PROFILES)[1].tradeUnlocked) {
                NIS_TRADE_BLOCKED += 2;
            }
            if (NIS_TRADE_BLOCKED != 0 || NIS_SAME_SAVE == 1) {
                NIS_DEBUG_NAME_TASK(0, "NO TRADE");
                spawnTask(0, -1, 0, 0x800, NIS_runTradeBlocked, 0, 0, 0, 0);
                exitTask();
            }
            break;
        case 4:
            setBackgroundScrollMode(2);
            NIS_STATE->otherPad = 0;
            break;
        }
        startChoiceMenuAction(&menu);
    }
}

void NIS_openVsWindow(NisWindowDef *def) {
    s32 window;

    openKanjiPage(0xF, 0x1B9);
    NIS_DEBUG_NAME_TASK(0, "MSG_WIN");
    spawnTask(0, -1, 0, 0x800, runWindowTask, def, getCurrentTaskId(), 0, 0);
    window = waitFrames(0x7FFFFFFF);
    NIS_VS_WINDOW = (NisWindow *)window;
}

void NIS_closeNothing(NisWindow *window) {
}

extern const char NIS_FMT_PLAYER1[];
extern const char NIS_FMT_PLAYER2[];
extern const char NIS_FMT_STARS[];
extern const char NIS_FMT_DRAWS[];
extern const char NIS_FMT_PLAYER1_RECORD[];
extern const char NIS_FMT_PLAYER2_RECORD[];

/* the players' names, the stars to win and both records, a row of 0xE pixels
   apart. x and y are s16 set at the top, as in drawOptionWindow, and y moves
   down one row at a time: the match depends on both */
void NIS_drawVsRecords(NisWindow *window) {
    char lines[6][0x30];
    char numbers[8][0xC];
    NisProfile *profiles;
    u16 starCount;
    u16 drawCount;
    s16 width;
    s16 x;
    s16 y;

    x = 0x26;
    y = 0xA4;
    profiles = (NisProfile *)PLAYER_PROFILES;
    starCount = NIS_STATE->unk14C;
    drawCount = NIS_STATE->unk14D;
    width = splitDigits(drawCount, D_801E46D8);
    if (width == 0) {
        width = 1;
    }
    formatSjisNumber(profiles[0].versusWins, 3, numbers[1]);
    formatSjisNumber(profiles[0].versusLosses, 3, numbers[2]);
    formatSjisNumber(profiles[1].versusWins, 3, numbers[4]);
    formatSjisNumber(profiles[1].versusLosses, 3, numbers[5]);
    formatSjisNumber(starCount, 3, numbers[6]);
    formatSjisNumber(drawCount, width, numbers[7]);
    sprintf(lines[0], NIS_FMT_PLAYER1, profiles[0].name);
    sprintf(lines[1], NIS_FMT_PLAYER2, profiles[1].name);
    sprintf(lines[2], NIS_FMT_STARS, numbers[6]);
    sprintf(lines[3], NIS_FMT_DRAWS, numbers[7]);
    sprintf(lines[4], NIS_FMT_PLAYER1_RECORD, numbers[1], numbers[2]);
    sprintf(lines[5], NIS_FMT_PLAYER2_RECORD, numbers[4], numbers[5]);
    drawIconText(x, y, 7, 0, window->z, (s32)lines[0]);
    drawIconText(x + 0x78, y, 7, 0, window->z, (s32)lines[1]);
    y += 0xE;
    drawIconText(x, y, 7, 0, window->z, (s32)lines[2]);
    drawIconText(x + 0x6C, y, 7, 0, window->z, (s32)lines[3]);
    y += 0xE;
    drawIconText(x, y, 7, 0, window->z, (s32)lines[4]);
    y += 0xE;
    drawIconText(x, y, 7, 0, window->z, (s32)lines[5]);
}

/* what NIS_drawVsRecords draws */
const char NIS_FMT_PLAYER1[] = "１Ｐ：%s\n"; /* "1P: %s" */
const char NIS_FMT_PLAYER2[] = "２Ｐ：%s\n"; /* "2P: %s" */
const char NIS_FMT_STARS[] = "勝ち星　：%s\n"; /* "Stars: %s" */
const char NIS_FMT_DRAWS[] = "－　%s\n"; /* "- %s" */
const char NIS_FMT_PLAYER1_RECORD[] = "１Ｐ戦績：%s勝%s敗\n"; /* "1P record: %s wins %s losses" */
const char NIS_FMT_PLAYER2_RECORD[] = "２Ｐ戦績：%s勝%s敗\n"; /* "2P record: %s wins %s losses" */

void NIS_runTradeBlocked(void) {
    NisMenu menu;
    NisWindowDef def = { { 0x17, 0x9C, 0, 0xC }, { 0x17, 0x9C, 0x112, 0x43 }, 0xA, 0, NIS_drawTradeBlockedReason, NIS_closeNothing };

    openChoiceMenu(&menu, 0xB, 0x32, 0, 0);
    addChoiceMenuItem(&menu, 0xB, NIS_runVsMode);
    if (NIS_SAME_SAVE) {
        def.render = NIS_drawSameSave;
    } else {
        def.render = NIS_drawTradeBlockedReason;
    }
    NIS_openVsWindow(&def);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        SCROLLING_BACKGROUND->unk1BE = 2;
        NIS_VS_WINDOW->state = 4;
        waitFrames(0x1E);
        closeKanjiPage(0xF);
        startChoiceMenuAction(&menu);
    }
}

/* "Player 1's cards / until they are 100 or more / can't trade.", or
   player 2's, or both's */
void NIS_drawTradeBlockedReason(NisWindow *window) {
    char lines[4][0x30];
    s32 x;
    s32 y;

    y = 0xA1;
    if (NIS_TRADE_BLOCKED == 1) {
        sprintf(lines[0], "プレイヤー１のカードが");
        x = 0x26;
        sprintf(lines[1], "１００枚以上になるまで");
        sprintf(lines[2], "トレードはできません。");
    } else if (NIS_TRADE_BLOCKED == 2) {
        sprintf(lines[0], "プレイヤー２のカードが");
        x = 0x26;
        sprintf(lines[1], "１００枚以上になるまで");
        sprintf(lines[2], "トレードはできません。");
    } else if (NIS_TRADE_BLOCKED == 3) {
        sprintf(lines[0], "プレイヤー１とプレイヤー２のカードが");
        x = 0x26;
        sprintf(lines[1], "１００枚以上になるまで");
        sprintf(lines[2], "トレードはできません。");
    } else {
        x = 0x26;
    }
    drawIconText(x, y, 7, 0, window->z, (s32)lines[0]);
    y += 0xE;
    drawIconText(x, y, 7, 0, window->z, (s32)lines[1]);
    drawIconText(x, y + 0xE, 7, 0, window->z, (s32)lines[2]);
}

/* "Player 1 and Player 2 / are the same file, so / can't trade." */
void NIS_drawSameSave(NisWindow *window) {
    char lines[4][0x30];

    sprintf(lines[0], "プレイヤー１とプレイヤー２は");
    sprintf(lines[1], "同一ファイルなので");
    {
        /* and the leftover bytes after it, not the same in the debug build */
#if JP_DEBUG_BUILD
        static const char cantTrade[24] = "トレードできません。\0\0\x96\r";
#else
        static const char cantTrade[24] = "トレードできません。\0\0Wi";
#endif

        sprintf(lines[2], cantTrade);
    }
    drawIconText(0x26, 0xA1, 7, 0, window->z, (s32)lines[0]);
    drawIconText(0x26, 0xAF, 7, 0, window->z, (s32)lines[1]);
    drawIconText(0x26, 0xBD, 7, 0, window->z, (s32)lines[2]);
}

void NIS_runMenuE7E4(void) {
    NisMenu menu;

    openChoiceMenuFromList(&menu, D_8007E7E4, 0);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) != 0) {
            startChoiceMenuAction(&menu);
        }
    }
}

void NIS_runMenuE7D8(void) {
    NisMenu menu;

    openChoiceMenuFromList(&menu, D_8007E7D8, 0);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) != 0) {
            startChoiceMenuAction(&menu);
        }
    }
}

void NIS_runMainMenuTask(void) {
    NisMenu menu;

    openChoiceMenuFromList(&menu, MAIN_MENU, 0);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) != 0) {
            startChoiceMenuAction(&menu);
        }
    }
}

void NIS_runMainMenu(void) {
#if JP_DEBUG_BUILD
    D_801DEBF0 = 0;
#endif
    loadScrollingBackground(0xE, 2);
    showScrollingBackground();
    spawnTask(0, -1, 0, 0x1000, NIS_runMainMenuTask);
}
