#include "dcb/window.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

WindowStyle WINDOW_STYLES[8] = {
    { { 0xE0, 0xEA, 0xEC }, { 0xE0, 0xEF, 0xF1 }, { 9, 7 }, { 0xE, 7 }, 2, 4, 0, 0xFE, 1 },
    { { 0xB8, 0xC0, 0xC2 }, { 0xD0, 0xDF, 0xE1 }, { 7, 6 }, { 0xE, 7 }, 2, 4, 0xFF, 0xFD, 1 },
    { { 0xE0, 0xE8, 0xEA }, { 0xD0, 0xD8, 0xDA }, { 7, 6 }, { 7, 6 }, 4, 4, 0xFD, 0xFD, 0 },
    { { 0xB0, 0xC0, 0xC2 }, { 0xE8, 0xF0, 0xF2 }, { 0xF, 6 }, { 7, 6 }, 4, 3, 0xFF, 0xFE, 2 },
    { { 0x88, 0x90, 0x92 }, { 0xD0, 0xDF, 0xE1 }, { 7, 6 }, { 0xE, 7 }, 3, 4, 0xFF, 0xFD, 1 },
    { { 0x80, 0x90, 0x92 }, { 0xE8, 0xF0, 0xF2 }, { 0xF, 6 }, { 7, 6 }, 5, 3, 0xFF, 0xFE, 3 },
    { { 0x98, 0xA1, 0xA3 }, { 0xCD, 0xD6, 0xD8 }, { 8, 8 }, { 8, 8 }, 2, 4, 0xFE, 0xFD, 0 },
    { { 0xB8, 0xD9, 0xDB }, { 0x99, 0xBE, 0xC0 }, { 0x20, 0x1D }, { 0x24, 0x10 }, 6, 0xD, 1, 0, 4 },
};
Rect16 WINDOW_FILL_PATTERNS[8] = {
    { 0xF8, 0xF0, 8, 8 },
    { 0xF8, 0xE8, 8, 8 },
    { 0xF8, 0xE0, 8, 8 },
    { 0xF8, 0xD8, 8, 8 },
    { 0xF8, 0xD0, 8, 8 },
    { 0xF8, 0xC8, 8, 8 },
    { 0xF8, 0xC0, 8, 8 },
    { 0xF8, 0xB8, 8, 8 },
};
Rect16 VSCROLL_BAR_UVS[4] = {
    { 0xC8, 0xE3, 8, 0 },
    { 0xD0, 0xE8, 8, 0 },
    { 0x98, 0xE3, 8, 0 },
    { 0xA0, 0xE8, 8, 0 },
};
Rect16 VSCROLL_PART_UVS[8] = {
    { 0xD0, 0xE0, 8, 8 },
    { 0xD0, 0xF0, 8, 8 },
    { 0xC8, 0xE0, 8, 2 },
    { 0xC8, 0xE6, 8, 2 },
    { 0xA0, 0xE0, 8, 8 },
    { 0xA0, 0xF0, 8, 8 },
    { 0x98, 0xE0, 8, 2 },
    { 0x98, 0xE6, 8, 2 },
};
Rect16 HSCROLL_BAR_UVS[4] = {
    { 0xCB, 0xE8, 0, 8 },
    { 0xD8, 0xE8, 0, 8 },
    { 0x9B, 0xE8, 0, 8 },
    { 0xA8, 0xE8, 0, 8 },
};
Rect16 HSCROLL_PART_UVS[8] = {
    { 0xD8, 0xE0, 8, 8 },
    { 0xD8, 0xF0, 8, 8 },
    { 0xC8, 0xE8, 2, 8 },
    { 0xCE, 0xE8, 2, 8 },
    { 0xA8, 0xE0, 8, 8 },
    { 0xA8, 0xF0, 8, 8 },
    { 0x98, 0xE8, 2, 8 },
    { 0x9E, 0xE8, 2, 8 },
};
s16 UNUSED_WINDOW_RECT[4] = { 0x100, -1, -1, 0 };

void initWindowPrimPool(s32 count) {
    WindowPrims *pool;
    WindowPrims *prims;
    s16 texWindow[4];
    u16 tpage;
    s32 i;
    s32 windowIndex;
    s32 j;

    WINDOW_PRIM_POOL_SIZE = count;
    WINDOW_TEX_X = 0x3C0;
    WINDOW_TEX_Y = 0x100;
    WINDOW_CLUT_X = 0x3E0;
    WINDOW_CLUT_Y = 0x1F8;
    pool = allocPermanentHeapBlock(WINDOW_PRIM_POOL_SIZE * sizeof(WindowPrims) * 2);
    tpage = GetTPage(0, 0, WINDOW_TEX_X, WINDOW_TEX_Y);
    for (i = 0; i < 2; i++) {
        prims = (WindowPrims *)(((Graphics *)&GRAPHICS)->buffers[i].windowPrimPool = (s32)(pool + WINDOW_PRIM_POOL_SIZE * i));
        for (windowIndex = 0; windowIndex < WINDOW_PRIM_POOL_SIZE; windowIndex++, prims++) {
            for (j = 0; j < 4; j++) {
                initPrimByType(0xC, &prims->ft4a[j], 0, 0);
                prims->ft4a[j].tpage = tpage;
                initPrimByType(0xE, &prims->linea[j], 0, 0);
                initPrimByType(0xE, &prims->lineb[j], 0, 0);
                initPrimByType(0xE, &prims->linec[j], 0, 0);
            }
            for (j = 0; j < 2; j++) {
                initPrimByType(0xC, &prims->ft4b[j], 0, 0);
                prims->ft4b[j].tpage = tpage;
                initPrimByType(0xC, &prims->ft4c[j], 0, 0);
                prims->ft4c[j].tpage = tpage;
            }
            initPrimByType(0xE, &prims->frame, 0, 0);
            prims->frame.u0 = 0;
            prims->frame.v0 = 0;
            SetDrawTPage(prims->tpage, 0, 1, tpage);
            texWindow[0] = 0;
            texWindow[1] = 0;
            texWindow[2] = 0;
            texWindow[3] = 0;
            SetTexWindow(prims->twin, texWindow);
        }
    }
    WINDOW_PRIM_CURSOR = CURRENT_FRAME_BUFFER->windowPrimPool;
}

void resetWindowPrimPool(void) {
    WINDOW_PRIM_CURSOR = CURRENT_FRAME_BUFFER->windowPrimPool;
}

void openWindow(void *winPtr, void *rectPtr, s32 fromPtr, s16 *viewPtr, s32 flags, s32 style, s32 brightness, s32 frames) {
    UiWindow *w = winPtr;
    Rect16 *r = rectPtr;
    Rect16 *from = (Rect16 *)fromPtr;
    Rect16 *view = (Rect16 *)viewPtr;

    if (r == (Rect16 *)-1) {
        w->rect.x = 0;
        w->rect.y = 0;
        w->rect.w = 0;
        w->rect.h = 0;
    } else {
        w->rect = *r;
    }
    if (from == (Rect16 *)-1) {
        w->from.x = w->rect.x + w->rect.w / 2;
        w->from.y = w->rect.y + w->rect.h / 2;
        w->from.w = 0;
        w->from.h = 0;
    } else {
        w->from = *from;
    }
    if (view == (Rect16 *)-1) {
        w->view.x = 0;
        w->view.y = 0;
        w->view.w = w->rect.w;
        w->view.h = w->rect.h;
    } else {
        w->view = *view;
        if (w->view.x < 0) {
            w->view.x = 0;
        }
        if (w->view.y < 0) {
            w->view.y = 0;
        }
        if (w->view.w == 0) {
            w->view.w = w->rect.w;
        }
        if (w->view.h == 0) {
            w->view.h = w->rect.h;
        }
        if (w->view.w < w->rect.w) {
            w->view.w = w->rect.w;
        }
        if (w->view.h < w->rect.h) {
            w->view.h = w->rect.h;
        }
        if (w->view.x >= w->view.w - w->rect.w) {
            w->view.x = w->view.w - w->rect.w;
        }
        if (w->view.y >= w->view.h - w->rect.h) {
            w->view.y = w->view.h - w->rect.h;
        }
    }
    w->cur = w->rect;
    w->delta.x = w->cur.x - w->from.x;
    w->delta.y = w->cur.y - w->from.y;
    w->delta.w = w->cur.w - w->from.w;
    w->delta.h = w->cur.h - w->from.h;
    w->originX = w->rect.x - w->view.x;
    w->originY = w->rect.y - w->view.y;
    w->scroll[0] = w->view.x;
    w->scroll[1] = w->view.y;
    w->scroll[2] = w->view.x;
    w->scroll[3] = w->view.y;
    w->animFrames = frames;
    w->animFrame = 0;
    w->scrollStep = 0;
    w->flags = flags;
    w->animDone = 0;
    w->style = style;
    if ((u32)brightness > 256) {
        w->brightness = 0xFF;
    } else {
        w->brightness = brightness;
    }
    w->palette = 0;
    w->labelPalette = 0;
    if ((style >> 4) < 5) {
        w->scrollbarStyle = 0;
    } else {
        w->scrollbarStyle = 1;
    }
    w->label = 0;
}

void animateWindowTo(UiWindow *win, Rect16 *target) {
    if (target == (Rect16 *)-1) {
        win->delta.x = win->cur.w / 2;
        win->delta.y = win->cur.h / 2;
        win->delta.w = -win->cur.w;
        win->delta.h = -win->cur.h;
        win->cur.x += win->cur.w / 2;
        win->cur.y += win->cur.h / 2;
        win->cur.w = 0;
        win->cur.h = 0;
    } else {
        win->delta.x = target->x - win->cur.x;
        win->delta.y = target->y - win->cur.y;
        win->delta.w = target->w - win->cur.w;
        win->delta.h = target->h - win->cur.h;
        win->cur = *target;
    }
    win->animFrame = win->animFrames - win->animFrame;
    if ((s8)win->animFrame < 0) {
        win->animFrame = 0;
    }
    win->animDone = 0;
}

void scrollWindowTo(s16 *win, s32 x, s32 y) {
    if (((s8 *)win)[0x3E] >= 6) {
        ((s8 *)win)[0x3E] = 0;
    }
    if (x > win[4] - win[8]) {
        x = win[4] - win[8];
    }
    if (y > win[5] - win[9]) {
        y = win[5] - win[9];
    }
    if (x < 0) {
        x = 0;
    }
    if (y < 0) {
        y = 0;
    }
    win[0x18] = win[2];
    win[0x19] = win[3];
    win[0x1A] = x;
    win[0x1B] = y;
}

s32 drawWindow(UiWindow *win, void (*drawContents)(), s32 z) {
    DISPENV env;
    Rect16 frameClip;
    Rect16 contentClip;
    Rect16 labelClip;
    Rect16 unused; /* unused, but it is in the original stack frame */
    s32 ret;
    s32 labelKind;
    s32 y;

    win->z = z;
    ret = stepWindowAnimation(win);
    if (win->from.x >= 320) {
        return ret;
    }
    if (win->from.y >= 240) {
        return ret;
    }
    if (win->from.x + win->from.w <= 0) {
        return ret;
    }
    if (win->from.y + win->from.h <= 0) {
        return ret;
    }
    if ((win->from.w | win->from.h) == 0) {
        return ret;
    }
    {
        GetDispEnv(&env);
        frameClip.x = win->from.x + env.disp[0] - 2;
        frameClip.y = win->from.y + env.disp[1] - 1;
        frameClip.w = win->from.w + 4;
        frameClip.h = win->from.h + 2;
        clipRectToBounds(&frameClip, (Rect16 *)&env);
        contentClip.x = win->originX + win->view.x + env.disp[0] - 2;
        contentClip.y = win->originY + win->view.y + env.disp[1] - 1;
        contentClip.w = win->rect.w + 4;
        contentClip.h = win->rect.h + 2;
        if (win->flags & 2) {
            contentClip.w -= 8;
        }
        if (win->flags & 4) {
            contentClip.h -= 8;
        }
        clipRectToBounds(&contentClip, &frameClip);
        SetDrawArea((DR_AREA *)&WP->drawAreas[0x18], (Rect16 *)&env);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->drawAreas[0x18]);
        if (win->label != 0 && (win->flags & 8) && (labelKind = WINDOW_STYLES[(win->style >> 4) - 1].label) != 0) {
            switch (labelKind) {
            case 1:
                labelClip.x = frameClip.x;
                labelClip.y = frameClip.y - 7;
                labelClip.w = frameClip.w;
                labelClip.h = 5;
                drawSmallText(win->from.x, win->from.y - 8, win->label, win->labelPalette, z);
                break;
            case 2:
                labelClip.x = frameClip.x - 7;
                labelClip.y = frameClip.y;
                labelClip.w = 5;
                labelClip.h = frameClip.h;
                y = win->from.y;
                drawVerticalText(win->from.x - 9, y + strlen((u8 *)win->label) * 5, win->label, win->labelPalette, z);
                break;
            case 3:
                labelClip.x = frameClip.x - 6;
                labelClip.y = frameClip.y;
                labelClip.w = 5;
                labelClip.h = frameClip.h;
                y = win->from.y;
                drawVerticalText(win->from.x - 8, y + strlen((u8 *)win->label) * 5, win->label, win->labelPalette, z);
                break;
            case 4:
                labelClip.x = frameClip.x;
                labelClip.y = frameClip.y - 10;
                labelClip.w = frameClip.w;
                labelClip.h = 5;
                drawSmallText(win->from.x, win->from.y - 11, win->label, win->labelPalette, z);
                break;
            }
            clipRectToBounds(&labelClip, (Rect16 *)&env);
            SetDrawArea((DR_AREA *)&WP->drawAreas[0x24], &labelClip);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->drawAreas[0x24]);
        }
        drawVerticalScrollbar(win, z);
        drawHorizontalScrollbar(win, z);
        SetDrawArea((DR_AREA *)&WP->drawAreas[0], &frameClip);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->drawAreas[0]);
        drawContents(win, &CURRENT_FRAME_BUFFER->ot[z]);
        SetDrawArea((DR_AREA *)&WP->drawAreas[0xC], &contentClip);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->drawAreas[0xC]);
        drawWindowFrame(&win->from, win->style, win->flags & 1, win->brightness, win->palette, z);
    }
    return ret;
}

void clipRectToBounds(Rect16 *rect, Rect16 *bounds) {
    if (rect->x < bounds->x) {
        rect->w -= bounds->x - rect->x;
        rect->x = bounds->x;
    }
    if (rect->y < bounds->y) {
        rect->h -= bounds->y - rect->y;
        rect->y = bounds->y;
    }
    if (rect->x + rect->w > bounds->x + bounds->w) {
        rect->w = bounds->x + bounds->w - rect->x;
    }
    if (rect->y + rect->h > bounds->y + bounds->h) {
        rect->h = bounds->y + bounds->h - rect->y;
    }
}

#if VERSION_US || VERSION_EU
s32 stepWindowAnimation(UiWindow *w) {
    s32 remaining;
#if VERSION_US
    s32 dx;
    s32 dy;
#elif VERSION_EU
    s32 rounded; /* one temporary for both axes */
#endif
    s32 delta;
    s32 ox;
    s32 oy;

    remaining = (s8)w->animFrames - (s8)w->animFrame;
    w->animDone = 0;
    delta = w->delta.x;
    if (delta < 0) {
        w->from.x = w->cur.x - delta * remaining / (s8)w->animFrames;
    } else {
#if VERSION_US
        dx = delta * remaining - 1;
        w->from.x = w->cur.x - (dx + (s8)w->animFrames) / (s8)w->animFrames;
#elif VERSION_EU
        rounded = delta * remaining - 1;
        w->from.x = w->cur.x - (rounded + (s8)w->animFrames) / (s8)w->animFrames;
#endif
    }
    delta = w->delta.y;
    if (delta < 0) {
        w->from.y = w->cur.y - delta * remaining / (s8)w->animFrames;
    } else {
#if VERSION_US
        dy = delta * remaining - 1;
        w->from.y = w->cur.y - (dy + (s8)w->animFrames) / (s8)w->animFrames;
#elif VERSION_EU
        rounded = delta * remaining - 1;
        w->from.y = w->cur.y - (rounded + (s8)w->animFrames) / (s8)w->animFrames;
#endif
    }
    w->from.w = w->cur.w - w->delta.w * remaining / (s8)w->animFrames;
    w->from.h = w->cur.h - w->delta.h * remaining / (s8)w->animFrames;
    if ((s8)w->scrollStep < 6) {
        w->view.x = w->scroll[0] + (w->scroll[2] - w->scroll[0]) * (s8)w->scrollStep / 6;
        w->view.y = w->scroll[1] + (w->scroll[3] - w->scroll[1]) * (s8)w->scrollStep / 6;
        w->scrollStep++;
    } else {
        w->view.x = w->scroll[2];
        w->view.y = w->scroll[3];
    }
    ox = w->delta.w * remaining / (s8)w->animFrames / 2;
    if (ox < 0) {
        ox = abs(w->delta.w * (s8)w->animFrame / (s8)w->animFrames) / 2;
    }
    ox += w->view.x;
    oy = w->delta.h * remaining / (s8)w->animFrames / 2;
    if (oy < 0) {
        oy = abs(w->delta.h * (s8)w->animFrame / (s8)w->animFrames) / 2;
    }
    oy += w->view.y;
    w->originX = w->from.x - ox;
    w->originY = w->from.y - oy;
    w->animFrame += FRAME_INTERVAL;
    if ((s8)w->animFrame > (s8)w->animFrames) {
        w->animFrame = w->animFrames;
        w->animDone = 1;
    }
    return w->animDone;
}
#else
#error "main/ui/window: version not checked"
#endif

void drawWindowFrame(Rect16 *rect, u8 style, s32 semiTrans, s32 brightness, s32 palette, s32 z) {
    Rect16 edgeUv[4];
    Rect16 cornerUv[4];
    u16 clut;
    u32 *ot;
    s32 styleIndex;
    s32 i;

    clut = getClut(WINDOW_CLUT_X + (palette % 2) * 16, WINDOW_CLUT_Y + palette / 2);
    if (isWindowPrimPoolFull() != 0) {
        return;
    }
    if (rect->x < 320 && rect->y < 240 && rect->x + rect->w > 0 && rect->y + rect->h > 0) {
        ot = &CURRENT_FRAME_BUFFER->ot[z];
        if (style & 0xF0) {
            styleIndex = (style >> 4) - 1;
            setPrimQuadRect(&WP->ft4a[0], (s8)WINDOW_STYLES[styleIndex].left + rect->x,
                          (s8)WINDOW_STYLES[styleIndex].top + rect->y - WINDOW_STYLES[styleIndex].h[0],
                          rect->w - ((s8)WINDOW_STYLES[styleIndex].left - (s8)WINDOW_STYLES[styleIndex].right), WINDOW_STYLES[styleIndex].h[0]);
            setPrimQuadRect(&WP->ft4a[1], (s8)WINDOW_STYLES[styleIndex].left + rect->x,
                          (s8)WINDOW_STYLES[styleIndex].bottom + rect->y + rect->h,
                          rect->w - ((s8)WINDOW_STYLES[styleIndex].left - (s8)WINDOW_STYLES[styleIndex].right), WINDOW_STYLES[styleIndex].h[1]);
            setPrimQuadRect(&WP->ft4a[2], (s8)WINDOW_STYLES[styleIndex].left + rect->x - WINDOW_STYLES[styleIndex].w[0],
                          (s8)WINDOW_STYLES[styleIndex].top + rect->y, WINDOW_STYLES[styleIndex].w[0],
                          rect->h - ((s8)WINDOW_STYLES[styleIndex].top - (s8)WINDOW_STYLES[styleIndex].bottom));
            setPrimQuadRect(&WP->ft4a[3], (s8)WINDOW_STYLES[styleIndex].right + rect->x + rect->w,
                          (s8)WINDOW_STYLES[styleIndex].top + rect->y, WINDOW_STYLES[styleIndex].w[1],
                          rect->h - ((s8)WINDOW_STYLES[styleIndex].top - (s8)WINDOW_STYLES[styleIndex].bottom));
            WP->linea[0].x0 = (rect->x - WINDOW_STYLES[styleIndex].w[0]) + (s8)WINDOW_STYLES[styleIndex].left;
            WP->linea[0].y0 = (rect->y - WINDOW_STYLES[styleIndex].h[0]) + (s8)WINDOW_STYLES[styleIndex].top;
            WP->linea[1].x0 = (rect->x + rect->w) + (s8)WINDOW_STYLES[styleIndex].right;
            WP->linea[1].y0 = (rect->y - WINDOW_STYLES[styleIndex].h[0]) + (s8)WINDOW_STYLES[styleIndex].top;
            WP->linea[2].x0 = (rect->x - WINDOW_STYLES[styleIndex].w[0]) + (s8)WINDOW_STYLES[styleIndex].left;
            WP->linea[2].y0 = (rect->y + rect->h) + (s8)WINDOW_STYLES[styleIndex].bottom;
            WP->linea[3].x0 = (rect->x + rect->w) + (s8)WINDOW_STYLES[styleIndex].right;
            WP->linea[3].y0 = (rect->y + rect->h) + (s8)WINDOW_STYLES[styleIndex].bottom;
            edgeUv[0].x = WINDOW_STYLES[styleIndex].u[1];
            edgeUv[0].y = WINDOW_STYLES[styleIndex].v[0];
            edgeUv[0].w = 0;
            edgeUv[0].h = WINDOW_STYLES[styleIndex].h[0];
            edgeUv[1].x = WINDOW_STYLES[styleIndex].u[1];
            edgeUv[1].y = WINDOW_STYLES[styleIndex].v[2];
            edgeUv[1].w = 0;
            edgeUv[1].h = WINDOW_STYLES[styleIndex].h[1];
            edgeUv[2].x = WINDOW_STYLES[styleIndex].u[0];
            edgeUv[2].y = WINDOW_STYLES[styleIndex].v[1];
            edgeUv[2].w = WINDOW_STYLES[styleIndex].w[0];
            edgeUv[2].h = 0;
            edgeUv[3].x = WINDOW_STYLES[styleIndex].u[2];
            edgeUv[3].y = WINDOW_STYLES[styleIndex].v[1];
            edgeUv[3].w = WINDOW_STYLES[styleIndex].w[1];
            edgeUv[3].h = 0;
            cornerUv[0].x = WINDOW_STYLES[styleIndex].u[0];
            cornerUv[0].y = WINDOW_STYLES[styleIndex].v[0];
            cornerUv[0].w = WINDOW_STYLES[styleIndex].w[0];
            cornerUv[0].h = WINDOW_STYLES[styleIndex].h[0];
            cornerUv[1].x = WINDOW_STYLES[styleIndex].u[2];
            cornerUv[1].y = WINDOW_STYLES[styleIndex].v[0];
            cornerUv[1].w = WINDOW_STYLES[styleIndex].w[1];
            cornerUv[1].h = WINDOW_STYLES[styleIndex].h[0];
            cornerUv[2].x = WINDOW_STYLES[styleIndex].u[0];
            cornerUv[2].y = WINDOW_STYLES[styleIndex].v[2];
            cornerUv[2].w = WINDOW_STYLES[styleIndex].w[0];
            cornerUv[2].h = WINDOW_STYLES[styleIndex].h[1];
            cornerUv[3].x = WINDOW_STYLES[styleIndex].u[2];
            cornerUv[3].y = WINDOW_STYLES[styleIndex].v[2];
            cornerUv[3].w = WINDOW_STYLES[styleIndex].w[1];
            cornerUv[3].h = WINDOW_STYLES[styleIndex].h[1];

            for (i = 0; i < 4; i++) {
                setPrimQuadUvRect((u8 *)&WP->ft4a[i], edgeUv[i].x, edgeUv[i].y, edgeUv[i].w, edgeUv[i].h);
                setRGB0(&WP->ft4a[i], brightness, brightness, brightness);
                WP->ft4a[i].clut = clut;
                addPrim(ot, &WP->ft4a[i]);
                setUV0(&WP->linea[i], cornerUv[i].x, cornerUv[i].y);
                setWH(&WP->linea[i], cornerUv[i].w, cornerUv[i].h);
                setRGB0(&WP->linea[i], brightness, brightness, brightness);
                WP->linea[i].clut = clut;
                addPrim(ot, &WP->linea[i]);
            }
        }
        addPrim(ot, WP->twin);
        if (style & 0xF) {
            styleIndex = (style & 0xF) - 1;
            WP->frame.x0 = rect->x - 2;
            WP->frame.y0 = rect->y - 2;
            WP->frame.w = rect->w + 4;
            WP->frame.h = rect->h + 4;
            setSemiTrans(&WP->frame, semiTrans);
            setRGB0(&WP->frame, brightness, brightness, brightness);
            WP->frame.clut = clut;
            SetTexWindow(WP->fillTwin, (s16 *)&WINDOW_FILL_PATTERNS[styleIndex]);
            addPrim(ot, &WP->frame);
        }
        addPrim(ot, WP->fillTwin);
        addPrim(ot, WP->tpage);
        WINDOW_PRIM_CURSOR += sizeof(WindowPrims);
    }
}

void drawVerticalScrollbar(UiWindow *w, s32 z) {
    u16 clut;
    s32 x;
    s32 y;
    s32 trackLength;
    s32 thumbPos;
    s32 thumbSize;
    s32 i;
    s32 end;
    s32 off;
    s32 thumbTop;

    clut = getClut(WINDOW_CLUT_X + (w->palette % 2) * 16, WINDOW_CLUT_Y + w->palette / 2);
    if (!(w->flags & 2) || w->rect.h >= w->view.h) {
        return;
    }
    x = w->originX + w->view.x + w->rect.w - 8;
    y = w->originY + w->view.y;
    trackLength = w->rect.h - 0x10;
    if (w->flags & 4) {
        trackLength -= 8;
    }
    thumbPos = w->view.y * trackLength / w->view.h;
    thumbSize = w->rect.h * trackLength - 1;
    thumbSize = (thumbSize + w->view.h) / w->view.h;
    if (thumbPos + thumbSize < 0) {
        thumbPos = 0;
    }
    if (thumbPos + thumbSize > trackLength) {
        thumbPos = trackLength - thumbSize;
    }
    WP->lineb[0].x0 = x;
    WP->lineb[0].y0 = y;
    WP->lineb[1].x0 = x;
    end = trackLength + 8;
    WP->lineb[1].y0 = y + end;
    WP->lineb[2].x0 = x;
    off = thumbPos + 8;
    thumbTop = y + off;
    WP->lineb[2].y0 = thumbTop;
    WP->lineb[3].x0 = x;
    WP->lineb[3].y0 = thumbTop + thumbSize - 2;
    setPrimQuadRect(&WP->ft4b[0], x, thumbTop + 2, 8, thumbSize - 4);
    setPrimQuadRect(&WP->ft4b[1], x, y + 8, 8, trackLength);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->lineb[i], VSCROLL_PART_UVS[i + w->scrollbarStyle * 4].x, VSCROLL_PART_UVS[i + w->scrollbarStyle * 4].y);
        setWH(&WP->lineb[i], VSCROLL_PART_UVS[i + w->scrollbarStyle * 4].w, VSCROLL_PART_UVS[i + w->scrollbarStyle * 4].h);
        setRGB0(&WP->lineb[i], w->brightness, w->brightness, w->brightness);
        WP->lineb[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->lineb[i]);
    }
    for (i = 0; i < 2; i++) {
        setPrimQuadUvRect((u8 *)&WP->ft4b[i], VSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].x, VSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].y, VSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].w, VSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].h);
        setRGB0(&WP->ft4b[i], w->brightness, w->brightness, w->brightness);
        WP->ft4b[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->ft4b[i]);
    }
}

void drawHorizontalScrollbar(UiWindow *w, s32 z) {
    u16 clut;
    s32 x;
    s32 y;
    s32 trackLength;
    s32 thumbPos;
    s32 thumbSize;
    s32 i;
    s32 end;
    s32 off;
    s32 thumbLeft;

    clut = getClut(WINDOW_CLUT_X + (w->palette % 2) * 16, WINDOW_CLUT_Y + w->palette / 2);
    if (!(w->flags & 4) || w->rect.w >= w->view.w) {
        return;
    }
    x = w->originX + w->view.x;
    y = w->originY + w->view.y + w->rect.h - 8;
    trackLength = w->rect.w - 0x10;
    if (w->flags & 2) {
        trackLength -= 8;
    }
    thumbPos = w->view.x * trackLength / w->view.w;
    thumbSize = w->rect.w * trackLength - 1;
    thumbSize = (thumbSize + w->view.w) / w->view.w;
    if (thumbPos + thumbSize < 0) {
        thumbPos = 0;
    }
    if (thumbPos + thumbSize > trackLength) {
        thumbPos = trackLength - thumbSize;
    }
    WP->linec[0].x0 = x;
    WP->linec[0].y0 = y;
    end = trackLength + 8;
    WP->linec[1].x0 = x + end;
    WP->linec[1].y0 = y;
    off = thumbPos + 8;
    thumbLeft = x + off;
    WP->linec[2].x0 = thumbLeft;
    WP->linec[2].y0 = y;
    WP->linec[3].x0 = thumbLeft + thumbSize - 2;
    WP->linec[3].y0 = y;
    setPrimQuadRect(&WP->ft4c[0], thumbLeft + 2, y, thumbSize - 4, 8);
    setPrimQuadRect(&WP->ft4c[1], x + 8, y, trackLength, 8);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->linec[i], HSCROLL_PART_UVS[i + w->scrollbarStyle * 4].x, HSCROLL_PART_UVS[i + w->scrollbarStyle * 4].y);
        setWH(&WP->linec[i], HSCROLL_PART_UVS[i + w->scrollbarStyle * 4].w, HSCROLL_PART_UVS[i + w->scrollbarStyle * 4].h);
        setRGB0(&WP->linec[i], w->brightness, w->brightness, w->brightness);
        WP->linec[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->linec[i]);
    }
    for (i = 0; i < 2; i++) {
        setPrimQuadUvRect((u8 *)&WP->ft4c[i], HSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].x, HSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].y, HSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].w, HSCROLL_BAR_UVS[i + w->scrollbarStyle * 2].h);
        setRGB0(&WP->ft4c[i], w->brightness, w->brightness, w->brightness);
        WP->ft4c[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->ft4c[i]);
    }
}

int isWindowPrimPoolFull(void) {
    if (WINDOW_PRIM_CURSOR == CURRENT_FRAME_BUFFER->windowPrimPool + WINDOW_PRIM_POOL_SIZE * 0x294) {
        /* "ウインドウ表示数が多すぎます" (too many windows on screen) */
        printf("\x83" "E\x83" "C\x83\x93\x83h\x83" "E\x95\\\x8E\xA6\x90\x94\x82\xAA\x91\xBD\x82\xB7\x82\xAC\x82\xDC\x82\xB7\n");
        return -1;
    }
    return 0;
}
