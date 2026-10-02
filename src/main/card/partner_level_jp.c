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
void SAI_doNothing(void);
void NIS_runVsMode();
void SAI_returnToArea();
void SAI_runWorldMap();

extern u8 *SCROLLING_BACKGROUND;
extern s32 OPTION_WINDOW; /* the option window */
extern u8 OPTION_MENU_LEFT; /* set when the option menu is left */
extern u8 OPTION_FROM_WORLD_MAP;
void NIS_runMainMenuTask();

/* where leaving the options goes: back to the main menu, or to the world
   map when they were opened from it (OPTION_FROM_WORLD_MAP) */
void (*OPTION_EXIT_TASKS[2])() = { NIS_runMainMenuTask, SAI_runWorldMap };

void runOptionMenu(void);
void runSoundMenu(void);
void runBattleAnimationMenu(void);
void drawOptionWindow(UiWindow *window);

void ignoreOptionWindowClose(void) {
}

void func_80045DBC(void) {
}

void doNothingInOptions(void) {
}

/* loads the TIMs of file path into VRAM */
void loadTimFile(char *path) {
    u32 *tims;

    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    if (tims != NULL) {
        uploadTimList(tims);
        freeHeapBlock(tims);
    }
}

/* the option screen */
void runOptionScreen(void) {
    WindowSpec spec = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 10, 0, drawOptionWindow, ignoreOptionWindowClose };

    loadTimFile("D:\\OPTION.TIM");
    playMusic(0, 4, 0x7F);
    openKanjiPage(0xF, 0x1B9);
    spawnTask(0, -1, 0, 0x400, runWindowTask, &spec, getCurrentTaskId(), 0, 0);
    OPTION_WINDOW = waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x800, runOptionMenu, 0, 0, 0, 0);
    OPTION_MENU_LEFT = 0;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (OPTION_MENU_LEFT != 1);
    *(s16 *)(SCROLLING_BACKGROUND + 0x1BE) = 2;
    ((u8 *)OPTION_WINDOW)[1] = 4;
    waitFrames(30);
    closeKanjiPage(0xF);
    if (OPTION_FROM_WORLD_MAP != 0) {
        setBackgroundScrollMode(0);
    } else {
        setBackgroundScrollMode(2);
        playMusic(0, 4, 0x7F);
    }
    spawnTask(0, -1, 0, 0x800, OPTION_EXIT_TASKS[OPTION_FROM_WORLD_MAP], 1, 0, 0, 0);
    exitTask();
}

/* the option menu: sound, then polygon battles */
void runOptionMenu(void) {
    ChoiceMenu menu;

    openChoiceMenu(&menu, -0x3D, 0x32, doNothingInOptions, 0);
    addChoiceMenuItem(&menu, 0x3F, runSoundMenu);
    addChoiceMenuItem(&menu, 0x40, runBattleAnimationMenu);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.result < 0) {
            OPTION_MENU_LEFT = 1;
            exitTask();
        }
        startChoiceMenuAction(&menu);
    }
}

/* stereo or mono */
void runSoundMenu(void) {
    ChoiceMenu menu;

    openChoiceMenu(&menu, -0x3D, 0x32, runOptionMenu, 0);
    addChoiceMenuItem(&menu, 0x41, runOptionMenu);
    addChoiceMenuItem(&menu, 0x42, runOptionMenu);
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
void runBattleAnimationMenu(void) {
    ChoiceMenu menu;

    openChoiceMenu(&menu, -0x3D, 0x32, runOptionMenu, 0);
    addChoiceMenuItem(&menu, 0x43, runOptionMenu);
    addChoiceMenuItem(&menu, 0x44, runOptionMenu);
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

/* the option window's contents: the sound and polygon battle settings */
void drawOptionWindow(UiWindow *window) {
    u8 rgb[4] = { 0x80, 0x80, 0x80 };
    char lines[2][44];
    s16 x;
    s16 y;

    x = 0x26;
    y = 0x9D;
    if (!((PlayerProfile *)PLAYER_PROFILES)->monoSound) {
        sprintf(lines[0], "サウンド設定　　　ステレオ");
    } else {
        sprintf(lines[0], "サウンド設定　　　モノラル");
    }
    if (!((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation) {
        sprintf(lines[1], "ポリゴンバトル　　見る");
    } else {
        sprintf(lines[1], "ポリゴンバトル　　見ない");
    }
    drawIconTextColored(x, y, 7, 0, rgb, window->z, lines[0]);
    drawIconTextColored(x, y + 14, 7, 0, rgb, window->z, lines[1]);
}

/* loads the TIMs of file path into VRAM (as loadTimFile) */
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
void enterWorldMap(void) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    SAI_doNothing();
    spawnTask(0, -1, 0, 0x800, ((PlayerProfile *)PLAYER_PROFILES)->unk28_9 ? SAI_runWorldMap : SAI_returnToArea, 1, 0, 0, 0);
    exitTask();
}

/* a versus duel (with the polygon battles shown), then its record */
void runVersusDuel(void) {
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
    spawnTask(0, -1, 0, 0x1000, NIS_runVsMode, 0);
}
