#include "dcb/window.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/prim_util.h"

/* jp's windows (window.c is us's and eu's): no styles, labels or textures,
   a frame of four lines and a flat fill in the window's three colours, and
   the scrollbars drawn as smaller frames */

#define copyRGB(dst, src) ((dst)->r = (src)->r, (dst)->g = (src)->g, (dst)->b = (src)->b)

s16 UNUSED_WINDOW_RECT[4] = { 0x100, -1, -1, 0 };

void initWindowPrimPool(s32 count) {
    WindowPrims *pool;
    WindowPrims *prims;
    /* us's windows set a texture window with it */
    s16 texWindow[4]; /* unused, but it is in the original stack frame */
    s32 i;
    s32 j;

    WINDOW_PRIM_POOL_SIZE = count;
    pool = allocPermanentHeapBlock(WINDOW_PRIM_POOL_SIZE * sizeof(WindowPrims) * 2);
    for (i = 0; i < 2; i++) {
        prims = (WindowPrims *)(DB(i).windowPrimPool = (s32)(pool + WINDOW_PRIM_POOL_SIZE * i));
        for (j = 0; j < WINDOW_PRIM_POOL_SIZE; j++, prims++) {
            initPrimByType(2, &prims->lines[0], 0, 0);
            initPrimByType(2, &prims->lines[1], 0, 0);
            initPrimByType(2, &prims->lines[2], 0, 0);
            initPrimByType(2, &prims->lines[3], 0, 0);
            initPrimByType(8, &prims->fill, 0, 0);
            SetDrawTPage(&prims->tpage, 0, 1, GetTPage(0, 0, 0, 0));
        }
    }
    WINDOW_PRIM_CURSOR = CURRENT_FRAME_BUFFER->windowPrimPool;
}

void resetWindowPrimPool(void) {
    WINDOW_PRIM_CURSOR = CURRENT_FRAME_BUFFER->windowPrimPool;
}

/* colors is either a brightness (0 to 256: black fill, green lines) or the
   address of three colours, one per word */
void openWindow(void *winPtr, void *rectPtr, s32 fromPtr, s16 *viewPtr, s32 flags, s32 colors, s32 frames) {
    UiWindow *w = winPtr;
    Rect16 *r = rectPtr;
    Rect16 *from = (Rect16 *)fromPtr;
    Rect16 *view = (Rect16 *)viewPtr;
    s32 i;

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
    }
    w->cur = w->rect;
    w->delta.x = w->cur.x - w->from.x;
    w->delta.y = w->cur.y - w->from.y;
    w->delta.w = w->cur.w - w->from.w;
    w->delta.h = w->cur.h - w->from.h;
    w->scrollX = -1;
    w->scrollY = -1;
    w->scrollDX = 0;
    w->scrollDY = 0;
    w->animFrames = frames;
    w->animFrame = 0;
    w->flags = flags;
    w->animDone = 0;
    if ((u32)colors > 256) {
        w->brightness = 0xFF;
        for (i = 0; i < 3; i++) {
            copyRGB(&w->colors[i], &((CVECTOR *)colors)[i]);
        }
    } else {
        w->brightness = colors;
        w->colors[0].r = 0;
        w->colors[0].g = 0;
        w->colors[0].b = 0;
        w->colors[1].r = 0;
        w->colors[1].g = 0xA0;
        w->colors[1].b = 0;
        w->colors[2].r = 0;
        w->colors[2].g = 0xA0;
        w->colors[2].b = 0;
    }
}

/* also scrolls the view to (scrollX, scrollY) unless scrollX is negative */
void animateWindowTo(UiWindow *win, Rect16 *target, s32 scrollX, s32 scrollY) {
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
    if (scrollX >= 0) {
        win->scrollDX = scrollX - win->scrollX;
        win->scrollDY = scrollY - win->scrollY;
    }
    win->scrollX = scrollX;
    win->scrollY = scrollY;
    win->animFrame = win->animFrames - win->animFrame;
    if (win->animFrame < 0) {
        win->animFrame = 0;
    }
    win->animDone = 0;
}

s32 drawWindow(UiWindow *win, void (*drawContents)(), s32 z) {
    DISPENV env;
    Rect16 frameClip;
    Rect16 contentClip;
    /* jp's windows have no labels */
    Rect16 labelClip; /* unused, but it is in the original stack frame */
    s32 semiTrans;
    s32 ret;

    win->z = z;
    ret = stepWindowAnimation(win);
    if (win->from.x > 320) {
        return ret;
    }
    if (win->from.y > 240) {
        return ret;
    }
    if (win->from.x + win->from.w < 0) {
        return ret;
    }
    if (win->from.y + win->from.h < 0) {
        return ret;
    }
    if ((win->from.w | win->from.h) == 0) {
        return ret;
    }
    GetDispEnv(&env);
    frameClip.x = win->from.x + env.disp[0];
    frameClip.y = win->from.y + env.disp[1];
    frameClip.w = win->from.w;
    frameClip.h = win->from.h;
    clipRectToBounds(&frameClip, (Rect16 *)&env);
    contentClip.x = win->originX + win->view.x + env.disp[0];
    contentClip.y = win->originY + win->view.y + env.disp[1];
    contentClip.w = win->rect.w;
    contentClip.h = win->rect.h;
    if (win->flags & 1) {
        contentClip.w -= 9;
    }
    if (win->flags & 2) {
        contentClip.h -= 9;
    }
    clipRectToBounds(&contentClip, &frameClip);
    SetDrawArea(&WP->drawAreas[2], (Rect16 *)&env);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->drawAreas[2]);
    drawVerticalScrollbar(win, z);
    drawHorizontalScrollbar(win, z);
    SetDrawArea(&WP->drawAreas[0], &frameClip);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->drawAreas[0]);
    drawContents(win, &CURRENT_FRAME_BUFFER->ot[z]);
    SetDrawArea(&WP->drawAreas[1], &contentClip);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->drawAreas[1]);
    semiTrans = win->flags & 0x10;
    drawWindowFrame(&win->from, win->offsetX, win->offsetY, semiTrans != 0, win->brightness, win->colors, z);
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

s32 stepWindowAnimation(UiWindow *w) {
    s32 remaining;

    remaining = w->animFrames - w->animFrame;
    w->animDone = 0;
    w->offsetX = 0;
    w->offsetY = 0;
    if (w->delta.x < 0) {
        w->from.x = w->cur.x - w->delta.x * remaining / w->animFrames;
    } else {
        w->from.x = w->cur.x - (w->delta.x * remaining + (w->animFrames - 1)) / w->animFrames;
    }
    if (w->delta.y < 0) {
        w->from.y = w->cur.y - w->delta.y * remaining / w->animFrames;
    } else {
        w->from.y = w->cur.y - (w->delta.y * remaining + (w->animFrames - 1)) / w->animFrames;
    }
    w->from.w = w->cur.w - w->delta.w * remaining / w->animFrames;
    w->from.h = w->cur.h - w->delta.h * remaining / w->animFrames;
    if (w->scrollX < 0) {
        w->offsetX = w->delta.w * remaining / w->animFrames / 2;
        if (w->offsetX < 0) {
            w->offsetX = abs(w->delta.w * w->animFrame / w->animFrames) / 2;
        }
    } else {
        w->view.x = w->scrollX - w->scrollDX * remaining / w->animFrames;
    }
    if (w->scrollY < 0) {
        w->offsetY = w->delta.h * remaining / w->animFrames / 2;
        if (w->offsetY < 0) {
            w->offsetY = abs(w->delta.h * w->animFrame / w->animFrames) / 2;
        }
    } else {
        w->view.y = w->scrollY - w->scrollDY * remaining / w->animFrames;
    }
    w->offsetX += w->view.x;
    w->offsetY += w->view.y;
    w->originX = w->from.x - w->offsetX;
    w->originY = w->from.y - w->offsetY;
    w->animFrame += FRAME_INTERVAL;
    if (w->animFrame > w->animFrames) {
        w->animFrame = w->animFrames;
        w->animDone = 1;
    }
    return w->animDone;
}

/* the fill is offset by a pixel when offsetX is odd; colors[0] is the
   fill's, colors[1] and colors[2] the lines' */
void drawWindowFrame(Rect16 *rect, s32 offsetX, s32 offsetY, s32 semiTrans, s32 brightness, CVECTOR *colors, s32 z) {
    s32 r[4];
    s32 g[4];
    s32 b[4];
    u32 *ot;
    WindowPrims *prims;
    LINE_F3 *line1;
    LINE_F3 *line2;
    LINE_F3 *line3;
    s32 i;
    s32 fillOffset;
    s32 odd;
    s32 left;
    POLY_F4 *fill;

    if (isWindowPrimPoolFull() != 0) {
        return;
    }
    if (rect->x >= 320 || rect->y >= 240 || rect->x + rect->w <= 0 || rect->y + rect->h <= 0) {
        return;
    }
    ot = &CURRENT_FRAME_BUFFER->ot[z];
    prims = WP;
    line1 = &prims->lines[1];
    line2 = &prims->lines[2];
    line3 = &prims->lines[3];
    fillOffset = offsetX;
    if (colors == 0) {
        for (i = 0; i < 3; i++) {
            r[i] = brightness;
            g[i] = brightness;
            b[i] = brightness;
        }
    } else {
        for (i = 0; i < 3; i++) {
            r[i] = colors[i].r * brightness / 256;
            g[i] = colors[i].g * brightness / 256;
            b[i] = colors[i].b * brightness / 256;
        }
    }
    prims->lines[0].x0 = rect->x - 4;
    prims->lines[0].y0 = rect->y + rect->h + 2;
    prims->lines[0].x1 = rect->x - 4;
    prims->lines[0].y1 = rect->y - 3;
    prims->lines[0].x2 = rect->x + rect->w + 3;
    prims->lines[0].y2 = rect->y - 3;
    line1->x0 = rect->x - 3;
    line1->y0 = rect->y + rect->h + 2;
    line1->x1 = rect->x + rect->w + 3;
    line1->y1 = rect->y + rect->h + 2;
    line1->x2 = rect->x + rect->w + 3;
    line1->y2 = rect->y - 2;
    line2->x0 = rect->x - 5;
    line2->y0 = rect->y + rect->h + 3;
    line2->x1 = rect->x - 5;
    line2->y1 = rect->y - 4;
    line2->x2 = rect->x + rect->w + 4;
    line2->y2 = rect->y - 4;
    line3->x0 = rect->x - 4;
    line3->y0 = rect->y + rect->h + 3;
    line3->x1 = rect->x + rect->w + 4;
    line3->y1 = rect->y + rect->h + 3;
    line3->x2 = rect->x + rect->w + 4;
    line3->y2 = rect->y - 3;
    prims->lines[0].r0 = r[1];
    prims->lines[0].g0 = g[1];
    prims->lines[0].b0 = b[1];
    line1->r0 = r[2];
    line1->g0 = g[2];
    line1->b0 = b[2];
    line2->r0 = r[1];
    line2->g0 = g[1];
    line2->b0 = b[1];
    line3->r0 = r[2];
    line3->g0 = g[2];
    line3->b0 = b[2];
    addPrim(ot, &prims->lines[0]);
    addPrim(ot, line1);
    addPrim(ot, line2);
    addPrim(ot, line3);
    fill = &WP->fill;
    SetSemiTrans(fill, semiTrans);
    odd = fillOffset & 1;
    left = odd + 3;
    fill->x0 = rect->x - left;
    fill->y0 = rect->y - 2;
    fill->x1 = rect->x - left + (rect->w + 6 + odd);
    fill->y1 = rect->y - 2;
    fill->x2 = rect->x - left;
    fill->y2 = rect->y + 2 + rect->h;
    fill->x3 = rect->x - left + (rect->w + 6 + odd);
    fill->y3 = rect->y + 2 + rect->h;
    fill->r0 = r[0];
    fill->g0 = g[0];
    fill->b0 = b[0];
    addPrim(ot, fill);
    addPrim(ot, &WP->tpage);
    WINDOW_PRIM_CURSOR += sizeof(WindowPrims);
}

/* the track is a frame one pixel wide in the window's colours, the thumb a
   sunken one at half brightness */
void drawVerticalScrollbar(UiWindow *w, s32 z) {
    Rect16 rect;
    CVECTOR colors[3];
    s32 x;
    s32 y;
    s32 len;
    s32 width;
    s32 pos;
    s32 semiTrans;

    if (w->flags & 1) {
        x = w->originX + w->view.x + w->rect.w - 5;
        y = w->originY + w->view.y + 3;
        width = 0;
        len = w->rect.h - 4;
        if (w->flags & 2) {
            len -= 10;
        }
        if (w->rect.h < w->view.h) {
            if (y + 1 + w->view.y * (len - 2) / w->view.h + w->rect.h * (len - 2) / w->view.h >= y + len - 1) {
                pos = y + len - w->rect.h * (len - 2) / w->view.h - 2;
            } else {
                pos = y + 1 + w->view.y * (len - 2) / w->view.h;
            }
            semiTrans = w->flags & 0x10;
            rect.x = x;
            rect.y = pos;
            rect.w = width - 1;
            rect.h = w->rect.h * (len - 2) / w->view.h;
            drawWindowFrame(&rect, 0, 0, semiTrans != 0, w->brightness, w->colors, z);
            rect.x = x;
            rect.y = y;
            rect.w = width;
            rect.h = len - 1;
            colors[0] = w->colors[0];
            colors[1] = w->colors[2];
            colors[2] = w->colors[1];
            drawWindowFrame(&rect, 0, 0, 1, w->brightness / 2, colors, z);
        }
    }
}

void drawHorizontalScrollbar(UiWindow *w, s32 z) {
    Rect16 rect;
    CVECTOR colors[3];
    s32 x;
    s32 y;
    s32 len;
    s32 height;
    s32 pos;
    s32 semiTrans;

    if (w->flags & 2) {
        x = w->originX + w->view.x + 4;
        y = w->originY + w->view.y + w->rect.h - 4;
        len = w->rect.w - 6;
        if (w->flags & 1) {
            len -= 10;
        }
        height = 1;
        if (w->rect.w < w->view.w) {
            if (x + 1 + w->view.x * (len - 2) / w->view.w + w->rect.w * (len - 2) / w->view.w >= x + len - 1) {
                pos = x + len - w->rect.w * (len - 2) / w->view.w - 2;
            } else {
                pos = x + 1 + w->view.x * (len - 2) / w->view.w;
            }
            semiTrans = w->flags & 0x10;
            rect.x = pos;
            rect.y = y + 1;
            rect.w = w->rect.w * (len - 2) / w->view.w;
            rect.h = height - 2;
            drawWindowFrame(&rect, 0, 0, semiTrans != 0, w->brightness, w->colors, z);
            rect.x = x;
            rect.y = y;
            rect.w = len;
            rect.h = height;
            colors[0] = w->colors[0];
            colors[1] = w->colors[2];
            colors[2] = w->colors[1];
            drawWindowFrame(&rect, 0, 0, 1, w->brightness / 2, colors, z);
        }
    }
}

/* moves row up or down a window's 14-pixel rows with the pad's repeating
   up and down, wrapping round and scrolling the view to keep it shown;
   reaching either end stops the repeat */
s32 moveWindowListCursor(s32 row, UiWindow *win) {
    if (PAD_STATES[0]->rawRepeat & 0x1000) {
        row--;
        if (row < 0) {
            row = (s16)(win->view.h / 14) - 1;
            win->view.y = win->view.h - win->rect.h;
        } else {
            if (row == 0) {
                PAD_STATES[0]->repeatEnabled = 0;
            }
            if (row * 14 < win->view.y) {
                win->view.y = row * 14;
            }
        }
    }
    if (PAD_STATES[0]->rawRepeat & 0x4000) {
        row++;
        if (row > (s16)(win->view.h / 14) - 1) {
            row = 0;
            win->view.y = 0;
        } else {
            if (row == (s16)(win->view.h / 14) - 1) {
                PAD_STATES[0]->repeatEnabled = 0;
            }
            if (row * 14 >= win->view.y + win->rect.h) {
                win->view.y = (row + 1) * 14 - win->rect.h;
            }
        }
    }
    return row;
}

int isWindowPrimPoolFull(void) {
    if (WINDOW_PRIM_CURSOR == CURRENT_FRAME_BUFFER->windowPrimPool + WINDOW_PRIM_POOL_SIZE * sizeof(WindowPrims)) {
        return -1;
    }
    return 0;
}
