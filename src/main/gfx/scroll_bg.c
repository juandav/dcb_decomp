#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
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
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/prim3d.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

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

s16 ATTACK_ICON_ORIGIN_X[3] = { 0x80, -0x40, 0x140 };
s16 ATTACK_ICON_ORIGIN_Y[2][3] = { { -0xF0, 0x99, 0x99 }, { 0xF0, 0x1C, 0x1C } };

void renderScrollingBackground(void) {
    Fade *bg;
    s32 tim;
    s16 texWindow[4];
    u8 buffer;

    if (SCROLL_BACKGROUND.tim == 0 || *(u16 *)&SCROLL_BACKGROUND.mode == 0xFFFF) {
        return;
    }
    switch (SCROLL_BACKGROUND.unk6E) {
    case 0:
        if ((s8)SCROLL_BACKGROUND.unk6F < 30) {
            SCROLL_BACKGROUND.unk6F++;
        }
        break;
    case 1:
        if ((s8)SCROLL_BACKGROUND.unk6F >= -59) {
            SCROLL_BACKGROUND.unk6F--;
        }
        break;
    }
    bg = &SCROLL_BACKGROUND;
    bg->unk70 = (bg->unk70 + (s8)bg->unk6F) % 7680;
    if (bg->mode != bg->unk6D) {
        if (bg->unk6D == -1) {
            if (bg->unk72 == 0) {
                tim = decompressArchiveEntry(bg->tim, bg->mode);
                uploadTim((u32 *)tim, bg->x, bg->y, bg->w, bg->h);
                if (bg->mode != 6) {
                    bg->unk7C = 0x40;
                } else {
                    SCROLL_BACKGROUND.unk7C = 0x80;
                }
                SCROLL_BACKGROUND.unk7E = 0x80;
                DrawSync(0);
                freeHeapBlock((void *)tim);
            }
            SCROLL_BACKGROUND.unk72 += 6;
            if (SCROLL_BACKGROUND.unk72 > 0x80) {
                SCROLL_BACKGROUND.unk72 = 0x80;
                SCROLL_BACKGROUND.unk6D = SCROLL_BACKGROUND.mode;
            }
        } else {
            SCROLL_BACKGROUND.unk72 -= 6;
            if (SCROLL_BACKGROUND.unk72 < 0) {
                SCROLL_BACKGROUND.unk72 = 0;
                SCROLL_BACKGROUND.unk6D = -1;
            }
        }
    }
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin0);
    buffer = FRAME_BUFFER_INDEX;
    SCROLL_BACKGROUND.buf[buffer].x0 = -((SCROLL_BACKGROUND.unk70 / 60) & 1);
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].y0 = 0;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].u0 = (SCROLL_BACKGROUND.unk70 / 60) & 0xFE;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].v0 = SCROLL_BACKGROUND.unk70 / 60;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].r0 = SCROLL_BACKGROUND.unk72;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].g0 = SCROLL_BACKGROUND.unk72;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].b0 = SCROLL_BACKGROUND.unk72;
    texWindow[0] = (SCROLL_BACKGROUND.x % 64) * 4;
    texWindow[1] = SCROLL_BACKGROUND.y % 256;
    texWindow[2] = SCROLL_BACKGROUND.unk7C;
    texWindow[3] = SCROLL_BACKGROUND.unk7E;
    SetTexWindow(SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin, texWindow);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], &SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].tpage);
}
