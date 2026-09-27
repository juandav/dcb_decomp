#include "dcb/scroll_bg.h"
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
#include "dcb/model.h"
#include "dcb/model_load.h"
#include "dcb/model_anim.h"
#include "dcb/player_data.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"

void resetScrollingBackground(void) {
    D_801D8260 = 0;
}

void loadScrollingBackground(void) {
    s16 texWindow[4];
    s32 i;
    s8 *bg;
    s8 *buf;

    bg = (s8 *)&SCROLL_BACKGROUND;
    if ((*(s32 *)(bg + 0x68)) != 0) {
        return;
    }
    (*(s8 *)(bg + 0x6C)) = -1;
    (*(s8 *)(bg + 0x6D)) = -1;
    (*(s16 *)(bg + 0x72)) = 0;
    (*(s16 *)(bg + 0x70)) = 0;
    (*(s8 *)(bg + 0x6E)) = 0;
    (*(s8 *)(bg + 0x6F)) = 5;
    for (i = 0; i < 2; i++) {
        buf = (s8 *)&SCROLL_BACKGROUND + i * 0x34;
        initPrimByType(0xE, buf, 0, 0);
        (*(s16 *)(buf + 0x10)) = 0x141;
        (*(s16 *)(buf + 0x12)) = 0xF0;
        texWindow[0] = 0;
        texWindow[1] = 0;
        texWindow[2] = 0;
        texWindow[3] = 0;
        SetTexWindow((s8 *)&D_801D8220 + i * 0x34, texWindow);
    }
    func_800149B8(0, -1, 0, 0x800, loadFileTagged, &PATH_BG_ARC, getCurrentTaskId(), -2);
    D_801D8260 = func_80014C08(0x7FFFFFFF);
}

void freeScrollingBackground(void) {
    s8 *bg = (s8 *)&SCROLL_BACKGROUND;

    freeHeapBlock(*(void **)(bg + 0x68));
    *(void **)(bg + 0x68) = 0;
    hideScrollingBackground();
}

void changeScrollingBackground(s32 image, s32 x, s32 y, s32 w, s32 h) {
    s32 i;

    if (SCROLL_BACKGROUND.unk72 != 0 && SCROLL_BACKGROUND.unk72 != 0x80) {
        do {
            func_80014C08(FRAME_INTERVAL);
        } while (SCROLL_BACKGROUND.unk72 != 0 && SCROLL_BACKGROUND.unk72 != 0x80);
    }
    if (SCROLL_BACKGROUND.unk72 == 0) {
        SCROLL_BACKGROUND.unk6D = -1;
    }
    SCROLL_BACKGROUND.mode = image;
    if (image >= 0) {
        SCROLL_BACKGROUND.x = x;
        SCROLL_BACKGROUND.y = y;
        SCROLL_BACKGROUND.w = w;
        SCROLL_BACKGROUND.h = h;
        for (i = 0; i < 2; i++) {
            (SCROLL_BACKGROUND.buf + i)->clut = getClut(w, h);
            SetDrawTPage((SCROLL_BACKGROUND.buf + i)->tpage, 0, 0, GetTPage(0, 0, x, y));
        }
    }
    SCROLL_BACKGROUND.unk6E = 0;
    SCROLL_BACKGROUND.unk6F = 0x1E;
}

void hideScrollingBackground(void) {
    s8 *bg;

    bg = (s8 *)&SCROLL_BACKGROUND;
    bg[0x6C] = -1;
    bg[0x6D] = -1;
    (*(s16 *)(bg + 0x72)) = 0;
    (*(s16 *)(bg + 0x70)) = 0;
}

void fadeOutScrollingBackground(void) {
    D_801D8264 = -1;
}

void setBackgroundScrollMode(s8 scrollMode) {
    D_801D8266 = scrollMode;
}
