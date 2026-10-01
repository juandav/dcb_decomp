#include "common.h"
#include "game.h"
#include "dcb/duel_launch.h"
#include "dcb/duel_session.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/sound.h"
#include "dcb/overlay_calls.h"

/* jp's duel launchers (duel_launch.c is us's and eu's): the caller has the
   opponent's deck ready, the duel gets the duel's sound effect bank, and the
   result goes back to the caller instead of to the area or the friend menu */

s32 startCpuDuel(void) {
    s32 result;

    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\kawseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    freeHeapBlocksByTag(0x1EA);
    loadSoundEffectBank(0);
    spawnTask(0, -1, 0, 0x600, runDuel, 1, getCurrentTaskId(), 0, 0);
    result = waitFrames(0x7FFFFFFF);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    return result;
}

s32 startVersusDuel(void) {
    s32 result;

    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\kawseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    freeHeapBlocksByTag(0x1EA);
    loadSoundEffectBank(0);
    spawnTask(0, -1, 0, 0x600, runDuel, 0, getCurrentTaskId(), 0, 0);
    result = waitFrames(0x7FFFFFFF);
    loadSoundEffectBank(1);
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\nisseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    return result;
}
