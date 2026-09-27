#include "dcb/boot.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/cd_file.h"
#include "dcb/effect_object.h"
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

void runMainTask(void) {
    s32 mainTaskId;

    mainTaskId = getCurrentTaskId();
    initMemoryCard();
    initPads();
    func_8006A884(0);
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
        func_80014C08(0xA);
        clearFramePrimSlots();
        initScreenFade();
        PAD_INPUT_ENABLED = 1;
        func_800149B8(0, -1, 0, 0x800, playOpeningMovie, 2, mainTaskId);
        func_80014C08(0x7FFFFFFF);
        func_801E055C(0);
        loadSoundEffectBank(1);
        stopMusic();
        resetDisplay(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, runRenderLoop, 0, 0, 0, 0);
        func_80014C08(0xA);
        func_800149B8(0, -1, 0, 0x400, runTitleMenu, 0, 0, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(0xA);
    }
}
