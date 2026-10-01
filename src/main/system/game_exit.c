#include "dcb/game_exit.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/partner_level.h"
#include "dcb/hacking_shell.h"
#include "dcb/card_db.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/overlay_calls.h"
#include "dcb/battle_hud.h"

#if VERSION_JP
/* jp asks nothing before going back to the title and plays the movie from
   ENDSEG; it loads SAISEG in every mode, but after the ending (mode 1) starts
   a task of main's own instead of SAI_runArea */
void quitToTitleOrPlayEnding(s32 mode) {
    Rect16 vramRect = { 0, 0, 480, 512 };

    if (mode == 0) {
        waitFrames(10);
        ClearImage(&vramRect, 0, 0, 0);
        DrawSync(0);
        waitFrames(10);
        stopMusic();
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\endseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        waitFrames(10);
        spawnTask(0, -1, 0, 0x800, playOpeningMovie, 1, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        resetDisplay(0x140, 0xF0, 0);
        spawnTask(0x1F, 0, 0, 0x800, runRenderLoop, 0, 0, 0, 0);
        waitFrames(2);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
    } else if (mode == 1) {
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\endseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x600, END_runPlayerRecords, getCurrentTaskId(), 0, 0, 0);
        waitFrames(0x7FFFFFFF);
        waitFrames(10);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x800, openMemcardScreenBeforeTitle);
    } else {
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\endseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x600, END_runPlayerRecords, getCurrentTaskId(), 0, 0, 0);
        waitFrames(0x7FFFFFFF);
        waitFrames(10);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
    }
}
#elif VERSION_US || VERSION_EU
void quitToTitleOrPlayEnding(s32 mode) {
    u8 dialog[0xB8];
    Rect16 vramRect = { 0, 0, 480, 512 };
    s32 parentTask;
    s32 done;

    parentTask = getCurrentTaskId();
    if (mode == 0) {
        freeScrollingBackground();
        waitFrames(10);
        ClearImage(&vramRect, 0, 0, 0);
        DrawSync(0);
        waitFrames(10);
        stopMusic();
        waitFrames(10);
        spawnTask(0, -1, 0, 0x800, playOpeningMovie, 1, parentTask);
        waitFrames(0x7FFFFFFF);
        resetDisplay(0x140, 0xF0, 0);
        spawnTask(0x1F, 0, 0, 0x800, runRenderLoop, 0, 0, 0, 0);
        waitFrames(2);
        done = 0;
        do {
            spawnTask(0, -1, 0, 0x600, OPEN_runMemcardScreen, 8, parentTask, 0, 0);
            waitFrames(0x7FFFFFFF);
            playMenuSound(3);
            initDialog(dialog,
                          "*c6 Is it OK to return to Title Screen?\n*c3(Unless you save the game now,\nyou won't be able "
                          "to continue.)",
                          1);
            runDialog(dialog);
            switch ((s8)dialog[0xA5]) {
            case 1:
                done = 1;
                break;
            case 0:
            case 2:
                done = 0;
                break;
            }
        } while (!done);
        waitFrames(20);
        resumeTask(0);
        exitTask();
    } else {
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\endseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x800, END_runPlayerRecords, parentTask, mode, 0, 0);
        waitFrames(0x7FFFFFFF);
        waitFrames(10);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\saiseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        waitFrames(0x7FFFFFFF);
        waitFrames(2);
        spawnTask(0, -1, 0, 0x1600, SAI_runArea, 0, parentTask, 0, 0);
    }
}
#else
#error "main/system/game_exit: version not checked"
#endif
