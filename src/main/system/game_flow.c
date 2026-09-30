#include "dcb/game_flow.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/stage.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/anim_control.h"
#include "dcb/model_load.h"
#include "dcb/model_anim.h"
#include "dcb/player_data.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"

void openSaveScreenFromMap(s32 saveMode) {
    waitFrames(2);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\openseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x600, D_801EBAFC, saveMode, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    switch (saveMode) {
    case 2:
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x400, D_801F00F4, 1, 1, getCurrentTaskId(), 0);
        break;
    case 4:
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
        break;
    }
}

void continueSavedGame(void) {
    spawnTask(0, -1, 0, 0x600, D_801EBAFC, 0xFF, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    if (D_801F80C1 != 0) {
        fadeOutScrollingBackground();
        spawnTask(0, -1, 0, 0x100, runTitleMenu, 0, 0, 0, 0);
        return;
    }
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    if (PLAYER_DATA(0).resumeInArea == 0) {
        loadMusicTrack(0, 0x6F, 0x7F);
        playLoadedMusic(0);
        spawnTask(0, -1, 0, 0x400, D_801F00F4, 0, 0, getCurrentTaskId(), 0);
    } else {
        spawnTask(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
    }
}

void openPartnerFusion(s8 mode) {
    waitFrames(2);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\evoseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1600, D_801E8E88, (s32 *) mode, 0, 0, 0);
}

void returnToWorldMap(void) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
}

void openDeckEditor(s32 returnTo) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\subseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x600, D_801E8C04, 0, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    switch (returnTo) {
    case 0:
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x400, D_801F00F4, 0, 1, getCurrentTaskId(), 0);
        break;
    case 1:
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
        break;
    }
}

void openPartnerEquipment(s32 returnTo) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\subseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, D_801E4B34, 0, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    switch (returnTo) {
    case 0:
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x400, D_801F00F4, 0, 1, getCurrentTaskId(), 0);
        break;
    case 1:
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
        break;
    }
}

void runDeckEditorFromFriendMenu(s32 *param) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\subseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x600, D_801E8C04, param, getCurrentTaskId(), 0, 0);
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\openseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
}

void runPartnerEquipmentFromFriendMenu(s32 *param) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\subseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, D_801E4B34, param, getCurrentTaskId(), 1, 0);
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\openseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
}

void runTitleMenu(void) {
    s32 stack;
    s32 again;
    s32 choice;

    stack = getCurrentTaskId();
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\openseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    do {
        spawnTask(0, -1, 0, 0x800, D_801EA2F8, stack, 0, 0, 0);
        choice = waitFrames(0x7FFFFFFF);
        again = 0;
        switch (choice) {
        case 0:
            changeScrollingBackground(6, 0x380, 0, 0x380, 0x80);
            spawnTask(0, -1, 0, 0x800, D_801E6454, stack, 0, 0, 0);
            waitFrames(0x7FFFFFFF);
            waitFrames(2);
            spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
            waitFrames(0x7FFFFFFF);
            waitFrames(2);
            spawnTask(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
            break;
        case 1:
            changeScrollingBackground(6, 0x380, 0, 0x380, 0x80);
            spawnTask(0, -1, 0, 0x800, continueSavedGame, 0, 0, 0, 0);
            break;
        case 2:
            changeScrollingBackground(7, 0x380, 0, 0x380, 0x80);
            ((SessionData *)SESSION_DATA)->menuRow = 0;
            again = func_801EBD34();
            if (again == 0) {
                spawnTask(0, -1, 0, 0x800, D_801EB2E8, stack, 0, 0, 0);
            } else {
                fadeOutScrollingBackground();
            }
            break;
        }
    } while (again);
}
