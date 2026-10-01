#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/text.h"
#include "dcb/sound_play.h"
#include "dcb/player_rank.h"
#include "dcb/duel_launch.h"

/* jp's option screen and the end of a versus duel, where us has
   partner_level.c (us's and eu's) */

void runWindowTask();
void startChoiceMenuAction(ChoiceMenu *menu);
void addChoiceMenuItem(ChoiceMenu *menu, s32 text, void (*task)());
void openChoiceMenu(ChoiceMenu *menu, s32 x, s32 y, void (*back)(), s32 arg4);
s32 runChoiceMenu(ChoiceMenu *menu);
void openKanjiPage(s32 page, s32 capacity);
void closeKanjiPage(s32 page);
void setBackgroundScrollMode(s32 mode);
void loadScrollingBackground(s32 arg0, s32 arg1);
void showScrollingBackground(void);
void func_8005714C(void);
void func_801ECC58(void);
void D_801F0820();
void D_801F0EA8();
void SAI_runWorldMap();

extern u8 *SCROLLING_BACKGROUND;
extern s32 D_801E4988; /* the option window */
extern u8 D_801E4980; /* set when the option menu is left */
extern u8 D_801E4990;
extern void (*D_8007ED74[])();

void func_80046048(void);
void func_800460F0(void);
void func_800461DC(void);
void func_800462BC(UiWindow *window);

void func_80045DB4(void) {
}

void func_80045DBC(void) {
}

void func_80045DC4(void) {
}

/* loads the TIMs of file path into VRAM */
void func_80045DCC(char *path) {
    u32 *tims;

    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims != NULL) {
        uploadTimList(tims);
        freeHeapBlock(tims);
    }
}

/* the option screen */
void func_80045E40(void) {
    WindowSpec spec = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 10, 0, func_800462BC, func_80045DB4 };

    func_80045DCC("D:\\OPTION.TIM");
    playMusic(0, 4, 0x7F);
    openKanjiPage(0xF, 0x1B9);
    spawnTask(0, -1, 0, 0x400, runWindowTask, &spec, getCurrentTaskId(), 0, 0);
    D_801E4988 = waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, func_80046048, 0, 0, 0, 0);
    D_801E4980 = 0;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (D_801E4980 != 1);
    *(s16 *)(SCROLLING_BACKGROUND + 0x1BE) = 2;
    ((u8 *)D_801E4988)[1] = 4;
    waitFrames(30);
    closeKanjiPage(0xF);
    if (D_801E4990 != 0) {
        setBackgroundScrollMode(0);
    } else {
        setBackgroundScrollMode(2);
        playMusic(0, 4, 0x7F);
    }
    spawnTask(0, -1, 0, 0x800, D_8007ED74[D_801E4990], 1, 0, 0, 0);
    exitTask();
}

/* the option menu: sound, then polygon battles */
void func_80046048(void) {
    ChoiceMenu menu;

    openChoiceMenu(&menu, -0x3D, 0x32, func_80045DC4, 0);
    addChoiceMenuItem(&menu, 0x3F, func_800460F0);
    addChoiceMenuItem(&menu, 0x40, func_800461DC);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.result < 0) {
            D_801E4980 = 1;
            exitTask();
        }
        startChoiceMenuAction(&menu);
    }
}

/* stereo or mono */
void func_800460F0(void) {
    ChoiceMenu menu;

    openChoiceMenu(&menu, -0x3D, 0x32, func_80046048, 0);
    addChoiceMenuItem(&menu, 0x41, func_80046048);
    addChoiceMenuItem(&menu, 0x42, func_80046048);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.result >= 0) {
            switch (menu.choice) {
            case 0:
                ((PlayerProfile *)PLAYER_PROFILES)->monoSound = 0;
                SsSetStereo();
                break;
            case 1:
                ((PlayerProfile *)PLAYER_PROFILES)->monoSound = 1;
                func_8005714C();
                break;
            }
        }
        startChoiceMenuAction(&menu);
    }
}

/* whether to watch the polygon battles */
void func_800461DC(void) {
    ChoiceMenu menu;

    openChoiceMenu(&menu, -0x3D, 0x32, func_80046048, 0);
    addChoiceMenuItem(&menu, 0x43, func_80046048);
    addChoiceMenuItem(&menu, 0x44, func_80046048);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.result >= 0) {
            switch (menu.choice) {
            case 0:
                ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation = 0;
                break;
            case 1:
                ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation = 1;
                break;
            }
        }
        startChoiceMenuAction(&menu);
    }
}

/* the option window's contents: its colour and texts, then the function.
   jp: x, y and the colour end up in other callee-saved registers than the
   original's; no C form found yet */
INCLUDE_RODATA("main/nonmatchings/card/partner_level_jp", D_80010FE4);
INCLUDE_RODATA("main/nonmatchings/card/partner_level_jp", D_80010FE8);
INCLUDE_RODATA("main/nonmatchings/card/partner_level_jp", D_80011004);
INCLUDE_RODATA("main/nonmatchings/card/partner_level_jp", D_80011020);
INCLUDE_RODATA("main/nonmatchings/card/partner_level_jp", D_80011038);
INCLUDE_ASM("main/nonmatchings/card/partner_level_jp", func_800462BC);

/* loads the TIMs of file path into VRAM (as func_80045DCC) */
void func_800463F0(char *path) {
    u32 *tims;

    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims != NULL) {
        uploadTimList(tims);
        freeHeapBlock(tims);
    }
}

/* back to SAISEG */
void func_80046464(void) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    func_801ECC58();
    spawnTask(0, -1, 0, 0x800, ((PlayerProfile *)PLAYER_PROFILES)->unk28_9 ? SAI_runWorldMap : D_801F0EA8, 1, 0, 0, 0);
    exitTask();
}

/* a versus duel (with the polygon battles shown), then its record */
void func_80046550(void) {
    PlayerProfile *profiles;
    SessionData *session;
    u16 max;
    u32 count;
    u32 skip;
    s32 winner;

    profiles = (PlayerProfile *)PLAYER_PROFILES;
    session = (SessionData *)SESSION_DATA;
    max = 999;
    skip = profiles->skipBattleAnimation;
    profiles->skipBattleAnimation = 0;
    winner = startVersusDuel();
    profiles->skipBattleAnimation = skip;
    if (winner == 0) {
        count = ++session->versusWins[0];
        if (count > max) {
            session->versusWins[0] = max;
        }
        profiles[0].versusWins++;
        profiles[1].versusLosses++;
        if (profiles[0].versusWins > max) {
            profiles[0].versusWins = max;
        }
        if (profiles[1].versusLosses > max) {
            profiles[1].versusLosses = max;
        }
    } else if (winner == 1) {
        count = ++session->versusWins[1];
        if (count > max) {
            session->versusWins[1] = max;
        }
        profiles[0].versusLosses++;
        profiles[1].versusWins++;
        if (profiles[0].versusLosses > max) {
            profiles[0].versusLosses = max;
        }
        if (profiles[1].versusWins > max) {
            profiles[1].versusWins = max;
        }
    }
    updatePlayerRanks(0);
    updatePlayerRanks(1);
    loadScrollingBackground(0xE, 4);
    showScrollingBackground();
    spawnTask(0, -1, 0, 0x1000, D_801F0820, 0);
}
