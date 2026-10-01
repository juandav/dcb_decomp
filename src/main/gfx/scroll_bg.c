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
#include "dcb/anim_control.h"
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
    SCROLL_BACKGROUND.tim = 0;
}

void loadScrollingBackground(void) {
    s16 texWindow[4];
    s32 i;
    ScrollBackground *bg;
    ScrollBgSprite *sprite;

    bg = &SCROLL_BACKGROUND;
    if (bg->tim != 0) {
        return;
    }
    bg->mode = -1;
    bg->shownImage = -1;
    bg->brightness = 0;
    bg->scrollPos = 0;
    bg->scrollMode = 0;
    bg->scrollSpeed = 5;
    /* one sprite per frame buffer, covering the whole screen */
    for (i = 0; i < 2; i++) {
        sprite = &SCROLL_BACKGROUND.buf[i];
        initPrimByType(0xE, sprite, 0, 0);
        sprite->w = 0x141;
        sprite->h = 0xF0;
        /* an empty texture window, drawn after the sprite to turn windowing
           back off */
        texWindow[0] = 0;
        texWindow[1] = 0;
        texWindow[2] = 0;
        texWindow[3] = 0;
        SetTexWindow((u8 *)SCROLL_BACKGROUND.buf[i].twin0, texWindow);
    }
    spawnTask(0, -1, 0, 0x800, loadFileTagged, "B:\\BG.ARC", getCurrentTaskId(), -2);
    SCROLL_BACKGROUND.tim = waitFrames(0x7FFFFFFF);
}

void freeScrollingBackground(void) {
    ScrollBackground *bg = &SCROLL_BACKGROUND;

    freeHeapBlock((void *)bg->tim);
    bg->tim = 0;
    hideScrollingBackground();
}

void changeScrollingBackground(s32 image, s32 x, s32 y, s32 w, s32 h) {
    s32 i;

    while (SCROLL_BACKGROUND.brightness != 0 && SCROLL_BACKGROUND.brightness != 0x80) {
        waitFrames(FRAME_INTERVAL);
    }
    if (SCROLL_BACKGROUND.brightness == 0) {
        SCROLL_BACKGROUND.shownImage = -1;
    }
    SCROLL_BACKGROUND.mode = image;
    if (image >= 0) {
        SCROLL_BACKGROUND.x = x;
        SCROLL_BACKGROUND.y = y;
        SCROLL_BACKGROUND.w = w;
        SCROLL_BACKGROUND.h = h;
        for (i = 0; i < 2; i++) {
            SCROLL_BACKGROUND.buf[i].clut = getClut(w, h);
            SetDrawTPage(SCROLL_BACKGROUND.buf[i].tpage, 0, 0, GetTPage(0, 0, x, y));
        }
    }
    SCROLL_BACKGROUND.scrollMode = 0;
    SCROLL_BACKGROUND.scrollSpeed = 0x1E;
}

void hideScrollingBackground(void) {
    ScrollBackground *bg;

    bg = &SCROLL_BACKGROUND;
    bg->mode = -1;
    bg->shownImage = -1;
    bg->brightness = 0;
    bg->scrollPos = 0;
}

void fadeOutScrollingBackground(void) {
    SCROLL_BACKGROUND.mode = -1;
}

void setBackgroundScrollMode(s8 scrollMode) {
    SCROLL_BACKGROUND.scrollMode = scrollMode;
}

s16 ATTACK_ICON_ORIGIN_X[3] = { 0x80, -0x40, 0x140 };
s16 ATTACK_ICON_ORIGIN_Y[2][3] = { { -0xF0, 0x99, 0x99 }, { 0xF0, 0x1C, 0x1C } };

void renderScrollingBackground(void) {
    ScrollBackground *bg;
    s32 tim;
    s16 texWindow[4];
    u8 buffer;

    /* mode and shownImage both -1: nothing to show */
    if (SCROLL_BACKGROUND.tim == 0 || *(u16 *)&SCROLL_BACKGROUND.mode == 0xFFFF) {
        return;
    }
    switch (SCROLL_BACKGROUND.scrollMode) {
    case 0:
        if ((s8)SCROLL_BACKGROUND.scrollSpeed < 30) {
            SCROLL_BACKGROUND.scrollSpeed++;
        }
        break;
    case 1:
        if ((s8)SCROLL_BACKGROUND.scrollSpeed >= -59) {
            SCROLL_BACKGROUND.scrollSpeed--;
        }
        break;
    }
    bg = &SCROLL_BACKGROUND;
    /* the scroll position is in 1/60 texel; it wraps every 128 texels */
    bg->scrollPos = (bg->scrollPos + (s8)bg->scrollSpeed) % 7680;
    /* a new image: fade the old one out, upload the new one, fade it in */
    if (bg->mode != bg->shownImage) {
        if (bg->shownImage == -1) {
            if (bg->brightness == 0) {
                tim = decompressArchiveEntry(bg->tim, bg->mode);
                uploadTim((u32 *)tim, bg->x, bg->y, bg->w, bg->h);
                if (bg->mode != 6) {
                    bg->texWindowW = 0x40;
                } else {
                    SCROLL_BACKGROUND.texWindowW = 0x80;
                }
                SCROLL_BACKGROUND.texWindowH = 0x80;
                DrawSync(0);
                freeHeapBlock((void *)tim);
            }
            SCROLL_BACKGROUND.brightness += 6;
            if (SCROLL_BACKGROUND.brightness > 0x80) {
                SCROLL_BACKGROUND.brightness = 0x80;
                SCROLL_BACKGROUND.shownImage = SCROLL_BACKGROUND.mode;
            }
        } else {
            SCROLL_BACKGROUND.brightness -= 6;
            if (SCROLL_BACKGROUND.brightness < 0) {
                SCROLL_BACKGROUND.brightness = 0;
                SCROLL_BACKGROUND.shownImage = -1;
            }
        }
    }
#if VERSION_US
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin0);
    buffer = FRAME_BUFFER_INDEX;
    SCROLL_BACKGROUND.buf[buffer].x0 = -((SCROLL_BACKGROUND.scrollPos / 60) & 1);
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].y0 = 0;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].u0 = (SCROLL_BACKGROUND.scrollPos / 60) & 0xFE;
#elif VERSION_EU
    /* eu reads the buffer index once for the first four */
    buffer = FRAME_BUFFER_INDEX;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[buffer].twin0);
    SCROLL_BACKGROUND.buf[buffer].x0 = -((SCROLL_BACKGROUND.scrollPos / 60) & 1);
    SCROLL_BACKGROUND.buf[buffer].y0 = 0;
    SCROLL_BACKGROUND.buf[buffer].u0 = (SCROLL_BACKGROUND.scrollPos / 60) & 0xFE;
#else
#error "main/gfx/scroll_bg: version not checked"
#endif
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].v0 = SCROLL_BACKGROUND.scrollPos / 60;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].r0 = SCROLL_BACKGROUND.brightness;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].g0 = SCROLL_BACKGROUND.brightness;
    SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].b0 = SCROLL_BACKGROUND.brightness;
    texWindow[0] = (SCROLL_BACKGROUND.x % 64) * 4;
    texWindow[1] = SCROLL_BACKGROUND.y % 256;
    texWindow[2] = SCROLL_BACKGROUND.texWindowW;
    texWindow[3] = SCROLL_BACKGROUND.texWindowH;
    SetTexWindow(SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin, texWindow);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], &SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].twin);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], SCROLL_BACKGROUND.buf[FRAME_BUFFER_INDEX].tpage);
}
