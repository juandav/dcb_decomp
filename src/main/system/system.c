#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/system.h"
#include "dcb/cd_file.h"
#include "dcb/effect.h"
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
#include "dcb/pad.h"
#include "dcb/player_data.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/text.h"
#include "dcb/window.h"

s32 RENDER_CALLBACKS_ENABLED = 1;

void tickVblankCounters(void) {
    s32 i;

    if (PLAYER_PROFILES != 0) {
        for (i = 0; i < 2; i++) {
            ((Unk8006E050 *)PLAYER_PROFILES)[i].unk24++;
        }
    }
    VBLANK_COUNTER++;
}
