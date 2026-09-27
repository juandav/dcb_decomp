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
#include "dcb/scroll_bg.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

void quitToTitleOrPlayEnding(s32 mode) {
    u8 dialog[0xB8];
    Rect16 vramRect = { 0, 0, 480, 512 };
    s32 parentTask;
    s32 done;

    parentTask = getCurrentTaskId();
    if (mode == 0) {
        freeScrollingBackground();
        func_80014C08(10);
        ClearImage(&vramRect, 0, 0, 0);
        DrawSync(0);
        func_80014C08(10);
        done = 0;
        stopMusic();
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x800, playOpeningMovie, 1, parentTask);
        func_80014C08(0x7FFFFFFF);
        resetDisplay(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, runRenderLoop, 0, 0, 0, 0);
        func_80014C08(2);
        do {
            func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 8, parentTask, 0, 0);
            func_80014C08(0x7FFFFFFF);
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
        func_80014C08(20);
        func_80014A48(0);
        func_80014A90();
    } else {
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, "P:\\endseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x800, D_801DF47C, parentTask, mode, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(10);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, PATH_SAISEG_BIN, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, parentTask, 0, 0);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/system/game_exit", PATH_SAISEG_BIN);
