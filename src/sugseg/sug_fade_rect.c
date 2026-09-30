#include "common.h"
#include "game.h"
#include "dcb/sug_fade_rect.h"
#include "dcb/heap.h"
#include "dcb/prim_util.h"

ColorQuad *SUG_createFadeRect(Rect16 *rect, Color *color, Color *color2, u8 blend, s16 step, u8 mode) {
    ColorQuad *quad;

    quad = allocTaskHeapBlock(sizeof(ColorQuad));
    initPolyF4Pair(&quad->poly[0], &quad->poly[1], color, blend, quad->tpage[0], quad->tpage[1], rect, 1, 1);
    quad->color = *color;
    quad->color2 = *color2;
    quad->step = step;
    quad->mode = mode;
    if (mode < 2 || mode == 3) {
        quad->visible = 1;
    } else {
        quad->visible = 0;
    }
    return quad;
}

s32 SUG_tickFadeRect(ColorQuad *quad) {
    PolyF4 *poly;
    u32 *tpage;
    s32 fadedIn;
    s32 fadedOut;

    fadedIn = 0;
    fadedOut = 0;
    if (quad->visible == 0) {
        return -1;
    }
    poly = &quad->poly[FRAME_BUFFER_INDEX];
    tpage = quad->tpage[FRAME_BUFFER_INDEX];
    if (quad->visible == 1) {
        fadedIn = stepColorToward(quad->step, &poly->r0, quad->color2.r, &poly->g0, quad->color2.g, &poly->b0, quad->color2.b);
    } else if (quad->visible == 2) {
        fadedOut = stepColorToward(quad->step, &poly->r0, quad->color.r, &poly->g0, quad->color.g, &poly->b0, quad->color.b);
        if (fadedOut != 0 && quad->mode == 3) {
            quad->visible = 1;
        }
    }
    if (*(u32 *)&poly->r0 & 0xFFFFFF) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)poly);
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)tpage);
    }
    if (fadedIn != 0 && (quad->mode == 1 || quad->mode == 3)) {
        quad->visible = 2;
    }
    if (fadedIn == 0 && fadedOut == 0) {
        return 0;
    }
    if (fadedIn != 0) {
        return 1;
    }
    if (fadedOut != 0) {
        return 2;
    }
    return -1;
}
