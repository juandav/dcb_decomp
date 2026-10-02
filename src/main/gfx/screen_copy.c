#include "dcb/screen_copy.h"
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
#include "dcb/overlay_calls.h"

#if VERSION_JP
/* jp's effect only has the sprites, and adds the mask-bit prims itself */
void initScreenCopyEffect(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            SCREEN_COPY_EFFECT.sprt[i][j].tag = 0x05000000;
            SCREEN_COPY_EFFECT.sprt[i][j].code = 0x66;
            SCREEN_COPY_EFFECT.sprt[i][j].u0 = 0;
            SCREEN_COPY_EFFECT.sprt[i][j].v0 = 0;
            SCREEN_COPY_EFFECT.sprt[i][j].w = 256 - j * 192;
            SCREEN_COPY_EFFECT.sprt[i][j].h = 240;
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
    s32 unused[2]; /* unused, but it is in the original stack frame */

    if (SCREEN_COPY_EFFECT.mode == 0) {
        return;
    }
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], &SCREEN_COPY_EFFECT.stp[1]);
    for (i = 0; i < 2; i++) {
        SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX][i].tpage = GetTPage(2, 0, i * 0x100, 0x100 - FRAME_BUFFER_INDEX * 0x100) | 0xE1000000;
        (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->x0 = SCREEN_COPY_EFFECT.x + (i << 8);
        (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->y0 = SCREEN_COPY_EFFECT.y;
        (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->r0 = SCREEN_COPY_EFFECT.r;
        (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->g0 = SCREEN_COPY_EFFECT.g;
        (SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX] + i)->b0 = SCREEN_COPY_EFFECT.b;
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &SCREEN_COPY_EFFECT.sprt[FRAME_BUFFER_INDEX][i]);
    }
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFFF], &SCREEN_COPY_EFFECT.stp[0]);
}
#elif VERSION_US || VERSION_EU
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
            SetPolyFT4(poly);
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
    s32 unused[4]; /* unused, but it is in the original stack frame */

    if (SCREEN_COPY_MODE == 0) {
        return;
    }
    if (SCREEN_COPY_MODE != 1) {
        SUG_updateScreenCopyQuads();
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
#endif
