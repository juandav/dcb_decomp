#include "common.h"
#include "game.h"
#include "dcb/sai_splash.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/loader.h"
#include "dcb/sound_play.h"
#include "dcb/saiseg.h"

extern Splash *SAI_SPLASH;

void SAI_loadSplashImage(u8 index) {
    char path[24];
    u32 *tim;

    if (index < 1 || index > 4) {
        index = 0;
    }
    sprintf(path, "C:\\OBJECT\\a_%d.TIM", index);
    spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTim(tim, 0x140, 0, 0x140, 0x81);
    freeHeapBlock(tim);
    SAI_SPLASH->loading = 0;
}

void SAI_initSplashQuads(void) {
    POLY_FT4 *quad;
    s32 buf;
    s32 i;

    for (buf = 0; buf < 2; buf++) {
        quad = SAI_SPLASH->quads[buf];
        for (i = 0; i < 4; i++, quad++) {
            setlen(quad, 9);
            setcode(quad, 0x2C);
            SetSemiTrans(quad, 0);
            quad->clut = getClut(0x140, 0x81);
            quad->tpage = getTPage(0, 0, 0x140, 0);
            quad->r0 = 0x80;
            quad->g0 = 0x80;
            quad->b0 = 0x80;
            quad->u0 = quad->u2 = 0;
            quad->u1 = quad->u3 = 0xFF;
            quad->x0 = quad->x2 = SAI_SPLASH->x;
            quad->x1 = quad->x3 = SAI_SPLASH->x + 0xFF;
        }
    }
}

void SAI_resetSplash(void) {
    SAI_SPLASH->state = 0;
    SAI_SPLASH->speed = 40;
    SAI_SPLASH->x = 32;
    SAI_SPLASH->y = 44;
    SAI_SPLASH->step = 0;
    SAI_SPLASH->scroll = 0;
    SAI_SPLASH->holdTimer = 60;
}

void SAI_unfoldSplash(void) {
    POLY_FT4 *quad = SAI_SPLASH->quads[FRAME_BUFFER_INDEX];
    u8 brightness;
    s32 limit = 62;
    s32 i;

    SAI_SPLASH->scroll += SAI_SPLASH->speed;
    if (limit - SAI_SPLASH->step < SAI_SPLASH->scroll) {
        SAI_SPLASH->scroll = 0;
        SAI_SPLASH->step++;
        if (SAI_SPLASH->step > 62) {
            SAI_SPLASH->step = 62;
        }
    }
    quad->v0 = quad->v1 = 62 - SAI_SPLASH->step;
    quad->v2 = quad->v3 = 63 - SAI_SPLASH->step;
    quad->y0 = quad->y1 = 0;
    quad->y2 = quad->y3 = SAI_SPLASH->y - (s16)(SAI_SPLASH->step - 63);
    quad++;
    quad->y0 = quad->y1 = SAI_SPLASH->y - (s16)(SAI_SPLASH->step - 63);
    quad->y2 = quad->y3 = SAI_SPLASH->y + 63;
    quad->v0 = quad->v1 = 63 - SAI_SPLASH->step;
    quad->v2 = quad->v3 = 63;
    quad++;
    quad->v0 = quad->v1 = SAI_SPLASH->step + 64;
    quad->v2 = quad->v3 = SAI_SPLASH->step + 65;
    quad->y0 = quad->y1 = SAI_SPLASH->y + (s16)(SAI_SPLASH->step + 63);
    quad->y2 = quad->y3 = 0xF0;
    quad++;
    quad->y0 = quad->y1 = SAI_SPLASH->y + 63;
    quad->y2 = quad->y3 = SAI_SPLASH->y + (s16)(SAI_SPLASH->step + 63);
    quad->v0 = quad->v1 = 64;
    quad->v2 = quad->v3 = SAI_SPLASH->step + 64;
    brightness = (SAI_SPLASH->step * 128 + (limit - SAI_SPLASH->step) * 64) / limit;
    for (i = 0; i < 4; i++) {
        SAI_SPLASH->brightness[i] = brightness;
    }
    if (brightness == 0x80) {
        SAI_SPLASH->state = 1;
    }
}

void SAI_holdSplash(void) {
    SAI_SPLASH->holdTimer--;
    if (SAI_SPLASH->holdTimer < 0) {
        playSoundEffect(0x13);
        SAI_SPLASH->state = 2;
    }
}

void SAI_fadeOutSplash(void) {
    s16 value;
    s32 i;

    value = SAI_SPLASH->brightness[0];
    value -= 8;
    if (value < 0) {
        SAI_SPLASH->state = 3;
        value = 0;
    }
    for (i = 0; i < 4; i++) {
        SAI_SPLASH->brightness[i] = value;
    }
}

void SAI_drawSplash(void) {
    POLY_FT4 *quad = SAI_SPLASH->quads[FRAME_BUFFER_INDEX];
    s32 i;

    for (i = 0; i < 4; i++, quad++) {
        quad->r0 = SAI_SPLASH->brightness[i];
        quad->g0 = SAI_SPLASH->brightness[i];
        quad->b0 = SAI_SPLASH->brightness[i];
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], quad);
    }
}

void SAI_runSplash(s32 index, s32 task) {
    SAI_SPLASH = allocTaskHeapBlock(0x15C);
    SAI_SPLASH->loading = 1;
    SAI_loadSplashImage(index);
    do {
        waitFrames(1);
    } while (SAI_SPLASH->loading != 0);
    SAI_resetSplash();
    SAI_initSplashQuads();
    do {
        waitFrames(1);
        switch (SAI_SPLASH->state) {
        case 0:
            SAI_unfoldSplash();
            break;
        case 1:
            SAI_holdSplash();
            break;
        case 2:
            SAI_fadeOutSplash();
            break;
        }
        SAI_drawSplash();
    } while (SAI_SPLASH->state != 3);
    waitFrames(30);
    resumeTask(task);
}
