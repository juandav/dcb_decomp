#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/vblank.h"
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

s32 RENDER_CALLBACKS_ENABLED = 1;

void tickVblankCounters(void) {
    s32 i;

    if (PLAYER_PROFILES != 0) {
        for (i = 0; i < 2; i++) {
            ((PlayerProfile *)PLAYER_PROFILES)[i].playTime++;
        }
    }
    VBLANK_COUNTER++;
}
