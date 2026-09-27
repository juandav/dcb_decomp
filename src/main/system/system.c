#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/system.h"
#include "dcb/cd_file.h"
#include "dcb/effect.h"
#include "dcb/duel_util.h"
#include "dcb/fade.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/pad.h"
#include "dcb/player_data.h"
#include "dcb/sound.h"
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

void initScreenCopyEffect(void) {
    s32 i;
    s32 j;
    POLY_FT4 *poly;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            SCREEN_COPY_EFFECT.sprt[i][j].tag = 0x05000000;
            SCREEN_COPY_EFFECT.sprt[i][j].code = 0x66;
            SCREEN_COPY_EFFECT.sprt[i][j].u0 = 0;
            SCREEN_COPY_EFFECT.sprt[i][j].v0 = 0;
            SCREEN_COPY_EFFECT.sprt[i][j].w = 256 - j * 192;
            SCREEN_COPY_EFFECT.sprt[i][j].h = 240;
            poly = &SCREEN_COPY_EFFECT.poly[i][j];
            func_800677A4(poly);
            poly->u0 = j * 32;
            poly->v0 = 0;
            poly->u1 = j * 32 - 96;
            poly->v1 = 0;
            poly->u2 = j * 32;
            poly->v2 = 240;
            poly->u3 = j * 32 - 96;
            poly->v3 = 240;
            setShadeTex(poly, 0);
            SetSemiTrans(poly, 1);
        }
    }
    SetDrawStp(&SCREEN_COPY_EFFECT.stp[0], 1);
    SetDrawStp(&SCREEN_COPY_EFFECT.stp[1], 0);
    SCREEN_COPY_EFFECT.r = 0xA8;
    SCREEN_COPY_EFFECT.g = 0xA8;
    SCREEN_COPY_EFFECT.b = 0xA8;
    SCREEN_COPY_EFFECT.x = 0;
    SCREEN_COPY_EFFECT.y = 0;
    SCREEN_COPY_EFFECT.mode = 0;
}

void renderScreenCopyEffect(void) {
    s32 i;
    POLY_FT4 *poly;
    s32 unused[4];

    if (D_800794E7 == 0) {
        return;
    }
    if (D_800794E7 != 1) {
        func_801EAD04();
    }
    for (i = 1; i >= 0; i--) {
        if (SCREEN_COPY_EFFECT.mode == 1) {
            SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX][i].tpage = GetTPage(2, 0, i * 0x100, 0x100 - FRAME_BUFFER_INDEX * 0x100) | 0xE1000000;
            (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->x0 = SCREEN_COPY_EFFECT.x + (i << 8);
            (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->y0 = SCREEN_COPY_EFFECT.y;
            (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->r0 = SCREEN_COPY_EFFECT.r;
            (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->g0 = SCREEN_COPY_EFFECT.g;
            (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->b0 = SCREEN_COPY_EFFECT.b;
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX][i]);
        } else {
            poly = &SCREEN_COPY_EFFECT.poly[FRAME_BUFFER_INDEX][i];
            poly->tpage = getTPage(2, SCREEN_COPY_EFFECT.abr & 3, i * 160, 0x100 - FRAME_BUFFER_INDEX * 0x100);
            poly->x0 = SCREEN_COPY_EFFECT.px[i][0];
            poly->y0 = SCREEN_COPY_EFFECT.py[i][0];
            poly->x1 = SCREEN_COPY_EFFECT.px[i][1];
            poly->y1 = SCREEN_COPY_EFFECT.py[i][1];
            poly->x2 = SCREEN_COPY_EFFECT.px[i][2];
            poly->y2 = SCREEN_COPY_EFFECT.py[i][2];
            poly->x3 = SCREEN_COPY_EFFECT.px[i][3];
            poly->y3 = SCREEN_COPY_EFFECT.py[i][3];
            poly->r0 = SCREEN_COPY_EFFECT.r;
            poly->g0 = SCREEN_COPY_EFFECT.g;
            poly->b0 = SCREEN_COPY_EFFECT.b;
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], poly);
        }
    }
}

void initGraphics(void) {
    s32 *scratchpad;
    s32 i;

    SetGraphDebug(0);
    InitGeom();
    scratchpad = (s32 *)0x1F800000;
    for (i = 0; i < 0x100; i++) {
        *scratchpad++ = 0;
    }
    (*(s16 *)((s8 *)(&GRAPHICS) + 0)) = 0;
    (*(s16 *)((s8 *)(&GRAPHICS) + 2)) = 0;
    (*(s16 *)((s8 *)(&GRAPHICS) + 4)) = 0x100;
    (*(s16 *)((s8 *)(&GRAPHICS) + 6)) = 0x100;
    (*(s32 *)((s8 *)(&GRAPHICS) + 0x50)) = 2;
    (*(s32 *)((s8 *)(&GRAPHICS) + 0x48)) = 0;
    FRAME_INTERVAL = 1;
    initScreenCopyEffect();
}

void runRenderLoop(void) {
    Unk800794F8 *gfx;
    s32 framesToWait;
    void (**callback)(Unk800793A0 *, s32);

    gfx = (Unk800794F8 *)&GRAPHICS;
    gfx->unk8[0] = 0;
    VBLANK_COUNTER = 0;
    for (; gfx->unk48 <= 0; gfx->unk48++) {
        pollPads();
        func_80014C08(1);
        gfx->unk50 = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->unk50 = 1;
        }
        VBLANK_COUNTER = 0;
    }
    SetDispMask(1);
    FRAME_BUFFER_INDEX = 0;
    CURRENT_FRAME_BUFFER = &gfx->unk98[0];
    ClearOTagR(gfx->unk98[0].ot, 0x1000);
    for (;;) {
        framesToWait = FRAME_INTERVAL;
        pollPads();
        while (framesToWait >= 2 || gfx->unk48 == 0) {
            func_80014AC8();
            framesToWait--;
        }
        FRAME_BUFFER_INDEX ^= 1;
        CURRENT_FRAME_BUFFER = &gfx->unk98[FRAME_BUFFER_INDEX];
        ClearOTagR(CURRENT_FRAME_BUFFER->ot, 0x1000);
        if (SCREEN_COPY_EFFECT.mode != 0) {
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &SCREEN_COPY_EFFECT.stp[1]);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], &SCREEN_COPY_EFFECT.stp[0]);
        }
        renderScrollingBackground();
        resetSpritePool();
        resetWindowPrimPool();
        if (RENDER_CALLBACKS_ENABLED != 0) {
            for (callback = gfx->unk8; *callback != 0; callback++) {
                (*callback)(CURRENT_FRAME_BUFFER, FRAME_BUFFER_INDEX);
            }
        }
        renderScreenCopyEffect();
        func_80014AC8();
        DrawSync(0);
        if (gfx->unk4C != 0) {
            GsSwapDispBuff();
        }
        PutDispEnv(&CURRENT_FRAME_BUFFER->disp);
        PutDrawEnv(&CURRENT_FRAME_BUFFER->draw);
        DrawOTag(&CURRENT_FRAME_BUFFER->ot[0xFFF]);
        gfx->unk50 = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->unk50 = 1;
        }
        VBLANK_COUNTER = 0;
    }
}
