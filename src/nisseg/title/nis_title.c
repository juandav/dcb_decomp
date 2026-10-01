#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/pad.h"
#include "dcb/fade.h"
#include "dcb/nisseg.h"
#include "dcb/prim_desc.h"

/* NISSEG's title screen: its logo, the copyright lines and a "PRESS START"
   that pulses until Start or Circle opens the main menu; left alone it goes
   back to the opening */

typedef struct {
    /* 0x00 */ NisSprite sprites[6];
    /* 0x90 */ s8 pulseDir;
    /* 0x91 */ s8 state;
    /* 0x94 */ s32 idleFrames;
    /* 0x98 */ s32 fadeFrames;
} TitleScreen;

void NIS_initTitleScreen(TitleScreen *title);
s32 NIS_tickTitleScreen(TitleScreen *title);
void NIS_pulsePressStart(TitleScreen *title, u8 step);
void NIS_startTrade(void);
void NIS_startVsDeckSelect(void);
void NIS_runMainMenu(s32 arg);
extern void runOptionScreen();
extern void enterWorldMap();
extern void runNewLoadMenu();

const s32 D_801EA3E8 = 7;

#if JP_DEBUG_BUILD
/* the names the debug build gives the tasks the debug mode starts; nothing
   reads the empty one */
const char NIS_TASK_OPTION[] = "OPTION";
const char NIS_TASK_TRADE[] = "TRADE";
const char NIS_TASK_WORLD_MAP[] = "W_MAP";
const char NIS_TASK_NEW_LOAD[] = "NEW_LOAD";
const char NIS_TASK_VS_DECK[] = "VS DECK";
const char D_801E6010[] = "";

void NIS_tickDebugModeSelect(s32 taskId);

/* the debug build's first menu: the debug mode and Sugano's menu */
NisDebugMenuItem NIS_DEBUG_MENU_ITEMS[2] = {
    { NIS_tickDebugModeSelect, NULL, 0x400, 0, 0, 0, 0, "TEST TASK" },
    { func_8002D15C, &NIS_SUGANO_MENU, 0x400, 0, 0, 0, 0, "SUGANO MENU" },
};
NisDebugMenu NIS_DEBUG_MENU = { NIS_DEBUG_MENU_ITEMS, 0, NULL, 0, 0x10, 0x20, 2, 2, { 0 }, "N_MENU" };
#endif

/* the last three bytes are leftovers, not the same in the debug build */
#if JP_DEBUG_BUILD
s8 NIS_DEBUG_MODE[4] = { 3, 0x40, 0x42, 0x30 };
#else
s8 NIS_DEBUG_MODE[4] = { 3, 0x12, 0, 0x43 };
#endif

void NIS_tickDebugModeSelect(s32 taskId) {
#if JP_DEBUG_BUILD
    D_801DEBF0 = 0;
#endif
    loadScrollingBackground(0xE, 2);
    showScrollingBackground();
    if (PAD_STATES[0]->rawHeld & PAD_R1) {
        NIS_DEBUG_MODE[0] = 2;
    }
    if (PAD_STATES[0]->rawHeld & PAD_R2) {
        NIS_DEBUG_MODE[0] = 6;
    }
    if (PAD_STATES[0]->rawHeld & PAD_L1) {
        NIS_DEBUG_MODE[0] = 4;
    }
    if (PAD_STATES[0]->rawHeld & PAD_L2) {
        NIS_DEBUG_MODE[0] = 5;
    }
    switch (NIS_DEBUG_MODE[0]) {
    case 1:
        NIS_DEBUG_NAME_TASK(0, NIS_TASK_OPTION);
        spawnTask(0, -1, 0, 0x800, runOptionScreen, 1, 0, 0, 0);
        break;
    case 2:
        NIS_DEBUG_NAME_TASK(0, NIS_TASK_TRADE);
        spawnTask(0, -1, 0, 0x800, NIS_startTrade, 1, 0, 0, 0);
        break;
    case 3:
        NIS_DEBUG_NAME_TASK(0, NIS_TASK_WORLD_MAP);
        spawnTask(0, -1, 0, 0x800, enterWorldMap, 1, 0, 0, 0);
        break;
    case 4:
        NIS_DEBUG_NAME_TASK(0, NIS_TASK_NEW_LOAD);
        spawnTask(0, -1, 0, 0x800, runNewLoadMenu, 1, 0, 0, 0);
        break;
    case 5:
        break;
    case 6:
        NIS_DEBUG_NAME_TASK(0, NIS_TASK_VS_DECK);
        spawnTask(0, -1, 0, 0x800, NIS_startVsDeckSelect, 0, 0, 0, 0);
        break;
    case 7:
        break;
    case 0:
    default:
        resumeTask(taskId);
        break;
    }
    if (DB(0).primSlots[16] == 0) {
        allocPrimDescPackets(0x40);
    }
}

void NIS_uploadTimFile(char *path) {
    u32 *tims;

#if JP_DEBUG_BUILD
    spawnTask(0, -1, 4, 0x800, loadFile, path, getCurrentTaskId());
#else
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
#endif
    tims = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTimList(tims);
    freeHeapBlock(tims);
}

void NIS_runTitleScreen(void) {
    TitleScreen title;

#if JP_DEBUG_BUILD
    D_801DEBF0 = 0;
#endif
    NIS_uploadTimFile("D:\\TITLE.TIM");
    if (DB(0).primSlots[16] == 0) {
        allocPrimDescPackets(0x40);
    }
    NIS_initTitleScreen(&title);
    while (1) {
        waitFrames(1);
        D_801E469C = title.sprites;
        NIS_tickTitleScreen(&title);
    }
}

void NIS_initTitleScreen(TitleScreen *title) {
    title->sprites[0].tag = 0xF;
    title->sprites[0].r = 0x80;
    title->sprites[0].g = 0x80;
    title->sprites[0].b = 0x80;
    title->sprites[0].code = 0x64;
    title->sprites[0].u = 0;
    title->sprites[0].v = 0;
    title->sprites[0].clut = 0x3C20;
    title->sprites[0].u1 = 0;
    title->sprites[0].v1 = 0;
    title->sprites[0].tpage = 0x88;
    title->sprites[0].x = 0;
    title->sprites[0].y = 0;
    title->sprites[0].w = 0x100;
    title->sprites[0].h = 0xF0;
    title->sprites[1].tag = 0xF;
    title->sprites[1].r = 0x80;
    title->sprites[1].g = 0x80;
    title->sprites[1].b = 0x80;
    title->sprites[1].code = 0x64;
    title->sprites[1].u = 0;
    title->sprites[1].v = 0;
    title->sprites[1].clut = 0x3C20;
    title->sprites[1].u1 = 0;
    title->sprites[1].v1 = 0;
    title->sprites[1].tpage = 0x8A;
    title->sprites[1].x = 0x100;
    title->sprites[1].y = 0;
    title->sprites[1].w = 0x40;
    title->sprites[1].h = 0xF0;
    title->sprites[2].tag = 0xD;
    title->sprites[2].r = 0x80;
    title->sprites[2].g = 0x80;
    title->sprites[2].b = 0x80;
    title->sprites[2].code = 0x64;
    title->sprites[2].u = 0;
    title->sprites[2].v = 0;
    title->sprites[2].clut = 0x3C60;
    title->sprites[2].u1 = 0;
    title->sprites[2].v1 = 0;
    title->sprites[2].tpage = 0x8C;
    title->sprites[2].x = 0x10;
    title->sprites[2].y = 0x2E;
    title->sprites[2].w = 0xA0;
    title->sprites[2].h = 0x40;
    title->sprites[3].tag = 0xD;
    title->sprites[3].r = 0x80;
    title->sprites[3].g = 0x80;
    title->sprites[3].b = 0x80;
    title->sprites[3].code = 0x64;
    title->sprites[3].u = 0;
    title->sprites[3].v = 0x40;
    title->sprites[3].clut = 0x3C60;
    title->sprites[3].u1 = 0;
    title->sprites[3].v1 = 0;
    title->sprites[3].tpage = 0x8C;
    title->sprites[3].x = 0xB0;
    title->sprites[3].y = 0x2E;
    title->sprites[3].w = 0x80;
    title->sprites[3].h = 0x40;
    title->sprites[4].tag = 0xD;
    title->sprites[4].r = 0x80;
    title->sprites[4].g = 0x80;
    title->sprites[4].b = 0x80;
    title->sprites[4].code = 0x64;
    title->sprites[4].u = 0;
    title->sprites[4].v = 0;
    title->sprites[4].clut = 0x3CA0;
    title->sprites[4].u1 = 0;
    title->sprites[4].v1 = 0;
    title->sprites[4].tpage = 0xB;
    title->sprites[4].x = 0x43;
    title->sprites[4].y = 0x96;
    title->sprites[4].w = 0xB0;
    title->sprites[4].h = 0x10;
    title->sprites[5].tag = 0xD;
    title->sprites[5].r = 0x80;
    title->sprites[5].g = 0x80;
    title->sprites[5].b = 0x80;
    title->sprites[5].code = 0x64;
    title->sprites[5].u = 0;
    title->sprites[5].v = 0x10;
    title->sprites[5].clut = 0x3CE0;
    title->sprites[5].u1 = 0;
    title->sprites[5].v1 = 0;
    title->sprites[5].tpage = 0xB;
    title->sprites[5].x = 0x49;
    title->sprites[5].y = 0xC6;
    title->sprites[5].w = 0xB0;
    title->sprites[5].h = 0x1C;
    title->pulseDir = 1;
    title->state = 0;
    title->idleFrames = 0;
    title->fadeFrames = 0;
    NIS_DEBUG_NAME_TASK(0, "FADE OUT");
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 0xFF, 0);
    waitFrames(2);
    setScreenFadeParams(1, 2, 8);
}

s32 NIS_tickTitleScreen(TitleScreen *title) {
    switch (title->state) {
    case 0:
        if (++title->fadeFrames >= 0x20) {
            title->state = 1;
        }
        break;
    case 1:
        NIS_pulsePressStart(title, 4);
        if (++title->idleFrames > 0x4B0) {
            title->state = 2;
            title->fadeFrames = 0;
            NIS_DEBUG_NAME_TASK(0, "FADE OUT");
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
        } else if (PAD_STATES[0]->rawPressed & (PAD_START | PAD_CIRCLE)) {
            playSoundEffect(0);
            title->state = 3;
            title->fadeFrames = 0;
            NIS_DEBUG_NAME_TASK(0, "FADE OUT");
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
        }
        break;
    case 2:
        if (++title->fadeFrames >= 0x20) {
            stopScreenFade();
            resumeTask(0);
            exitTask();
        }
        break;
    case 3:
        if (++title->fadeFrames >= 0x20) {
            stopScreenFade();
            playMusic(0, 4, 0x7F);
            /* the bytes after the name are leftovers */
            NIS_DEBUG_NAME_TASK(0, "MODE SEL\0e\b");
            spawnTask(0, -1, 0, 0x800, NIS_runMainMenu, 1, 0, 0, 0);
            exitTask();
        }
        break;
    }
    drawPrimDesc((PrimDesc *)&D_801E469C[0]);
    drawPrimDesc((PrimDesc *)&D_801E469C[1]);
    drawPrimDesc((PrimDesc *)&D_801E469C[2]);
    drawPrimDesc((PrimDesc *)&D_801E469C[3]);
    drawPrimDesc((PrimDesc *)&D_801E469C[4]);
    drawPrimDesc((PrimDesc *)&D_801E469C[5]);
    return 0;
}

void NIS_pulsePressStart(TitleScreen *title, u8 step) {
    NisSprite *sprite = &title->sprites[4];

    sprite->r += title->pulseDir * step;
    sprite->g += title->pulseDir * step;
    sprite->b += title->pulseDir * step;
    if ((u8)(sprite->r - 0x40) >= 0xBB) {
        title->pulseDir *= -1;
    }
}
