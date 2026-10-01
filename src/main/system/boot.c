#include "dcb/boot.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/cd_file.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/duel_util.h"
#include "dcb/fade.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/pad.h"
#include "dcb/player_data.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/overlay_calls.h"

#if VERSION_JP
/* jp's main() starts the hardware itself; after the movie its title screen
   is NISSEG's */
void runMainTask(void) {
    s32 mainTaskId;

    mainTaskId = getCurrentTaskId();
    killOtherTasks();
    closeAllDiscFiles();
    resetHeap(0);
    initSound(mainTaskId);
    initSystemSprites(0x3C0, 0x100, 0x3E8, 0x64);
    initWindowPrimPool(0x20);
    initPlayerData();
    for (;;) {
        killOtherTasks();
        closeAllDiscFiles();
        resetHeap(0);
        waitFrames(0xA);
        resetKanjiPages();
        clearFramePrimSlots();
        spawnTask(0, -1, 0, 0x800, playOpeningMovie, 2, mainTaskId);
        waitFrames(0x7FFFFFFF);
        OPEN_playMovie(0);
        loadSoundEffectBank(1);
        stopMusic();
        resetDisplay(0x140, 0xF0, 0);
        spawnTask(0x1F, 0, 0, 0x800, runRenderLoop, 0, 0, 0, 0);
        waitFrames(0xA);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\nisseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0xA, -1, 0, 0x800, NIS_runTitleScreen, 0, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(0xA);
    }
}
#elif VERSION_US || VERSION_EU
void runMainTask(void) {
    s32 mainTaskId;

    mainTaskId = getCurrentTaskId();
    initMemoryCard();
    initPads();
    ChangeClearPad(0);
    initDiscDrive();
    initGraphics();
    killOtherTasks();
    closeAllDiscFiles();
    resetHeap(0);
    initSound(mainTaskId);
    initSystemSprites(0x3C0, 0x100, 0x3E8);
    initWindowPrimPool(0xD);
    resetDisplay(0x140, 0xF0, 0);
    initPlayerData();
    resetScrollingBackground();
    for (;;) {
        killOtherTasks();
        closeAllDiscFiles();
        resetHeap(0);
        waitFrames(0xA);
        clearFramePrimSlots();
        initScreenFade();
        PAD_INPUT_ENABLED = 1;
        spawnTask(0, -1, 0, 0x800, playOpeningMovie, 2, mainTaskId);
        waitFrames(0x7FFFFFFF);
        OPEN_playMovie(0);
        loadSoundEffectBank(1);
        stopMusic();
        resetDisplay(0x140, 0xF0, 0);
        spawnTask(0x1F, 0, 0, 0x800, runRenderLoop, 0, 0, 0, 0);
        waitFrames(0xA);
        spawnTask(0, -1, 0, 0x400, runTitleMenu, 0, 0, 0, 0);
        waitFrames(0x7FFFFFFF);
        waitFrames(0xA);
    }
}
#else
#error "main/system/boot: version not checked"
#endif
