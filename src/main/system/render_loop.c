#include "dcb/render_loop.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/system.h"
#include "dcb/screen_copy.h"
#include "dcb/cd_file.h"
#include "dcb/effect.h"
#include "dcb/duel_util.h"
#include "dcb/fade.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/pad.h"
#include "dcb/player_data.h"
#include "dcb/sound.h"
#include "dcb/stage.h"
#include "dcb/text.h"
#include "dcb/window.h"

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
