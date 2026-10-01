#include "dcb/render_loop.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
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

#if VERSION_JP
#include "dcb/prim_desc.h"
#endif

void initGraphics(void) {
#if VERSION_JP
    Rect16 vramRect = { 0, 0, 1024, 512 };
#endif
    s32 *scratchpad;
    s32 i;

#if VERSION_JP
    /* jp resets the GPU and clears VRAM here, where us's main() does */
    ResetGraph(0);
    SetGraphDebug(0);
    SetDispMask(0);
    ClearImage(&vramRect, 0, 0, 0);
    DrawSync(0);
    GsInitGraph(320, 240, 0, 0, 0);
#elif VERSION_US || VERSION_EU
    SetGraphDebug(0);
#endif
    InitGeom();
    /* clear the first 1 KB of the scratchpad */
    scratchpad = (s32 *)0x1F800000;
    for (i = 0; i < 0x100; i++) {
        *scratchpad++ = 0;
    }
    ((Graphics *)&GRAPHICS)->texWindow[0] = 0;
    ((Graphics *)&GRAPHICS)->texWindow[1] = 0;
    ((Graphics *)&GRAPHICS)->texWindow[2] = 0x100;
    ((Graphics *)&GRAPHICS)->texWindow[3] = 0x100;
    ((Graphics *)&GRAPHICS)->vblanksPerFrame = 2;
    ((Graphics *)&GRAPHICS)->displayStartCounter = 0;
    FRAME_INTERVAL = 1;
    initScreenCopyEffect();
}

void runRenderLoop(void) {
    Graphics *gfx;
    s32 framesToWait;
    void (**callback)(FrameBuffer *, s32);

    gfx = (Graphics *)&GRAPHICS;
    gfx->frameCallbacks[0] = 0;
    VBLANK_COUNTER = 0;
    for (; gfx->displayStartCounter <= 0; gfx->displayStartCounter++) {
        pollPads();
        waitFrames(1);
        gfx->vblanksPerFrame = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->vblanksPerFrame = 1;
        }
        VBLANK_COUNTER = 0;
    }
    SetDispMask(1);
    FRAME_BUFFER_INDEX = 0;
    CURRENT_FRAME_BUFFER = &gfx->buffers[0];
    ClearOTagR(gfx->buffers[0].ot, 0x1000);
    for (;;) {
        framesToWait = FRAME_INTERVAL;
        pollPads();
        while (1) {
            if (framesToWait < 2 && gfx->displayStartCounter != 0) {
                break;
            }
            yieldTask();
            framesToWait--;
        }
        FRAME_BUFFER_INDEX ^= 1;
        CURRENT_FRAME_BUFFER = &gfx->buffers[FRAME_BUFFER_INDEX];
        ClearOTagR(CURRENT_FRAME_BUFFER->ot, 0x1000);
#if VERSION_JP
        /* jp has no scrolling background here, and renders the screen copy
           effect before the callbacks */
        PRIM_DESC_OT = CURRENT_FRAME_BUFFER->ot;
        PRIM_DESC_PACKETS = (u32 *)CURRENT_FRAME_BUFFER->primSlots[16];
        resetSpritePool();
        resetWindowPrimPool();
        renderScreenCopyEffect();
#elif VERSION_US || VERSION_EU
        if (SCREEN_COPY_EFFECT.mode != 0) {
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &SCREEN_COPY_EFFECT.stp[1]);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], &SCREEN_COPY_EFFECT.stp[0]);
        }
        renderScrollingBackground();
        resetSpritePool();
        resetWindowPrimPool();
#endif
        if (RENDER_CALLBACKS_ENABLED != 0) {
            for (callback = gfx->frameCallbacks; *callback != 0; callback++) {
                (*callback)(CURRENT_FRAME_BUFFER, FRAME_BUFFER_INDEX);
            }
        }
#if VERSION_US || VERSION_EU
        renderScreenCopyEffect();
#endif
        yieldTask();
        DrawSync(0);
        if (gfx->scene3dEnabled != 0) {
            GsSwapDispBuff();
        }
        PutDispEnv(&CURRENT_FRAME_BUFFER->disp);
        PutDrawEnv(&CURRENT_FRAME_BUFFER->draw);
        DrawOTag(&CURRENT_FRAME_BUFFER->ot[0xFFF]);
        gfx->vblanksPerFrame = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->vblanksPerFrame = 1;
        }
        VBLANK_COUNTER = 0;
    }
}
