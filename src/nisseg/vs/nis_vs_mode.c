#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/text.h"
#include "dcb/nisseg.h"

/* The VS mode screen: the two players' names and records, and its menu;
   trading needs two different saves that both have their 100 cards. And
   the main menus, which only run their menus. */

extern NisWindow *NIS_VS_WINDOW;
extern u8 NIS_TRADE_BLOCKED;
extern u8 NIS_SAME_SAVE;
extern u8 D_8007E808[];
extern u8 D_8007E7E4[];
extern u8 D_8007E7D8[];
extern u8 D_8007E7CC[];
extern u8 D_801E46D8[];

void NIS_runVsMode(void);
void NIS_openVsWindow(NisWindowDef *def);
void NIS_closeNothing(NisWindow *window);
void NIS_drawVsRecords(NisWindow *window);
void NIS_runTradeBlocked(void);
void NIS_drawTradeBlockedReason(NisWindow *window);
void NIS_drawSameSave(NisWindow *window);
void NIS_runMenuE7CC(void);

void NIS_loadTimFile(char *path) {
    u32 *tims;

    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
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
    func_8002B65C(&menu, D_8007E808, 0);
    NIS_openVsWindow(&def);
    playMusic(0, 4, 0x7F);
    NIS_TRADE_BLOCKED = 0;
    NIS_SAME_SAVE = 0;
    NIS_STATE->otherPad = 1;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (func_8002BD58(&menu) == 0) {
            continue;
        }
        D_801E4640->unk1BE = 2;
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
                spawnTask(0, -1, 0, 0x800, NIS_runTradeBlocked, 0, 0, 0, 0);
                exitTask();
            }
            break;
        case 4:
            func_8002CACC(2);
            NIS_STATE->otherPad = 0;
            break;
        }
        func_8002B06C(&menu);
    }
}

void NIS_openVsWindow(NisWindowDef *def) {
    s32 window;

    openKanjiPage(0xF, 0x1B9);
    spawnTask(0, -1, 0, 0x800, D_8002A3E0, def, getCurrentTaskId(), 0, 0);
    window = waitFrames(0x7FFFFFFF);
    NIS_VS_WINDOW = (NisWindow *)window;
}

void NIS_closeNothing(NisWindow *window) {
}

/* the players' names, the stars to win and both records: matching it needs
   x and y kept in registers, which only a block boundary before the
   drawing gives (the permuter's do-while) */
INCLUDE_ASM("nisseg/nonmatchings/vs/nis_vs_mode", NIS_drawVsRecords);

/* what NIS_drawVsRecords draws (defined after it, so that GCC switches
   back to .rodata after the asm) */
const char NIS_FMT_PLAYER1[] = "\x82P\x82o\x81" "F%s\n"; /* "1P: %s" */
const char NIS_FMT_PLAYER2[] = "\x82Q\x82o\x81" "F%s\n"; /* "2P: %s" */
const char NIS_FMT_STARS[] = "\x8F\x9F\x82\xBF\x90\xAF\x81@\x81" "F%s\n"; /* "Stars: %s" */
const char NIS_FMT_DRAWS[] = "\x81|\x81@%s\n"; /* "- %s" */
const char NIS_FMT_PLAYER1_RECORD[] = "\x82P\x82o\x90\xED\x90\xD1\x81" "F%s\x8F\x9F%s\x94s\n"; /* "1P record: %s wins %s losses" */
const char NIS_FMT_PLAYER2_RECORD[] = "\x82Q\x82o\x90\xED\x90\xD1\x81" "F%s\x8F\x9F%s\x94s\n"; /* "2P record: %s wins %s losses" */

void NIS_runTradeBlocked(void) {
    NisMenu menu;
    NisWindowDef def = { { 0x17, 0x9C, 0, 0xC }, { 0x17, 0x9C, 0x112, 0x43 }, 0xA, 0, NIS_drawTradeBlockedReason, NIS_closeNothing };

    func_8002B508(&menu, 0xB, 0x32, 0, 0);
    func_8002B188(&menu, 0xB, NIS_runVsMode);
    if (NIS_SAME_SAVE) {
        def.render = NIS_drawSameSave;
    } else {
        def.render = NIS_drawTradeBlockedReason;
    }
    NIS_openVsWindow(&def);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (func_8002BD58(&menu) == 0) {
            continue;
        }
        D_801E4640->unk1BE = 2;
        NIS_VS_WINDOW->state = 4;
        waitFrames(0x1E);
        closeKanjiPage(0xF);
        func_8002B06C(&menu);
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
        sprintf(lines[0], "\x83v\x83\x8C\x83" "C\x83\x84\x81[\x82P\x82\xCC\x83J\x81[\x83h\x82\xAA");
        x = 0x26;
        sprintf(lines[1], "\x82P\x82O\x82O\x96\x87\x88\xC8\x8F\xE3\x82\xC9\x82\xC8\x82\xE9\x82\xDC\x82\xC5");
        sprintf(lines[2], "\x83g\x83\x8C\x81[\x83h\x82\xCD\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81" "B");
    } else if (NIS_TRADE_BLOCKED == 2) {
        sprintf(lines[0], "\x83v\x83\x8C\x83" "C\x83\x84\x81[\x82Q\x82\xCC\x83J\x81[\x83h\x82\xAA");
        x = 0x26;
        sprintf(lines[1], "\x82P\x82O\x82O\x96\x87\x88\xC8\x8F\xE3\x82\xC9\x82\xC8\x82\xE9\x82\xDC\x82\xC5");
        sprintf(lines[2], "\x83g\x83\x8C\x81[\x83h\x82\xCD\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81" "B");
    } else if (NIS_TRADE_BLOCKED == 3) {
        sprintf(lines[0], "\x83v\x83\x8C\x83" "C\x83\x84\x81[\x82P\x82\xC6\x83v\x83\x8C\x83" "C\x83\x84\x81[\x82Q\x82\xCC\x83J\x81[\x83h\x82\xAA");
        x = 0x26;
        sprintf(lines[1], "\x82P\x82O\x82O\x96\x87\x88\xC8\x8F\xE3\x82\xC9\x82\xC8\x82\xE9\x82\xDC\x82\xC5");
        sprintf(lines[2], "\x83g\x83\x8C\x81[\x83h\x82\xCD\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81" "B");
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

    sprintf(lines[0], "\x83v\x83\x8C\x83" "C\x83\x84\x81[\x82P\x82\xC6\x83v\x83\x8C\x83" "C\x83\x84\x81[\x82Q\x82\xCD");
    sprintf(lines[1], "\x93\xAF\x88\xEA\x83t\x83@\x83" "C\x83\x8B\x82\xC8\x82\xCC\x82\xC5");
    {
        /* and the leftover bytes after it */
        static const char cantTrade[24] = "\x83g\x83\x8C\x81[\x83h\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81" "B\0\0Wi";

        sprintf(lines[2], cantTrade);
    }
    drawIconText(0x26, 0xA1, 7, 0, window->z, (s32)lines[0]);
    drawIconText(0x26, 0xAF, 7, 0, window->z, (s32)lines[1]);
    drawIconText(0x26, 0xBD, 7, 0, window->z, (s32)lines[2]);
}

void NIS_runMenuE7E4(void) {
    NisMenu menu;

    func_8002B65C(&menu, D_8007E7E4, 0);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (func_8002BD58(&menu) != 0) {
            func_8002B06C(&menu);
        }
    }
}

void NIS_runMenuE7D8(void) {
    NisMenu menu;

    func_8002B65C(&menu, D_8007E7D8, 0);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (func_8002BD58(&menu) != 0) {
            func_8002B06C(&menu);
        }
    }
}

void NIS_runMenuE7CC(void) {
    NisMenu menu;

    func_8002B65C(&menu, D_8007E7CC, 0);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (func_8002BD58(&menu) != 0) {
            func_8002B06C(&menu);
        }
    }
}

void NIS_runMainMenu(void) {
    func_8002C820(0xE, 2);
    func_8002C9DC();
    spawnTask(0, -1, 0, 0x1000, NIS_runMenuE7CC);
}
