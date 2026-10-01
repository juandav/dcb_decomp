#include "common.h"
#include "game.h"
#include "dcb/sug_scroll_texture.h"
#include "dcb/heap.h"

ScrollTex *SUG_createScrollTexture(Rect16 *rect, s32 depth, s32 mode, s16 speed) {
    ScrollTex *tex;

    tex = allocTaskHeapBlock(sizeof(ScrollTex));
    tex->rect.x = rect->x;
    tex->rect.y = rect->y;
    tex->rect.w = rect->w / (4 - depth * 2);
    tex->rect.h = rect->h;
    tex->vertical = mode;
    tex->depth = depth;
    tex->speed = speed;
    tex->buf0 = allocTaskHeapBlock(rect->w / (u32)(2 - depth) * rect->h);
    tex->buf1 = allocTaskHeapBlock(rect->w / (u32)(2 - depth) * rect->h);
    tex->timer = tex->period = mode / 2 * 2;
    return tex;
}

void SUG_scrollTexture(ScrollTex *tex) {
    Rect16 rects[4];

    if (tex->speed == 0) {
        return;
    }
    if (++tex->timer < tex->period) {
        return;
    }
    tex->timer = 0;
    if (!(tex->vertical & 1)) {
        tex->speed = tex->speed % tex->rect.w;
        if (tex->speed > 0) {
            rects[0].x = tex->rect.x + tex->rect.w - tex->speed;
            rects[0].y = tex->rect.y;
            rects[0].w = tex->speed;
            rects[0].h = tex->rect.h;
            rects[1].x = tex->rect.x;
            rects[1].y = tex->rect.y;
            rects[1].w = tex->rect.w - tex->speed;
            rects[1].h = tex->rect.h;
            rects[2].x = tex->rect.x;
            rects[2].y = tex->rect.y;
            rects[2].w = tex->speed;
            rects[2].h = tex->rect.h;
            rects[3].x = tex->rect.x + tex->speed;
            rects[3].y = tex->rect.y;
            rects[3].w = tex->rect.w - tex->speed;
            rects[3].h = tex->rect.h;
        } else {
            rects[0].x = tex->rect.x;
            rects[0].y = tex->rect.y;
            rects[0].w = -tex->speed;
            rects[0].h = tex->rect.h;
            rects[1].x = tex->rect.x - tex->speed;
            rects[1].y = tex->rect.y;
            rects[1].w = tex->rect.w + tex->speed;
            rects[1].h = tex->rect.h;
            rects[2].x = tex->rect.x + tex->rect.w + tex->speed;
            rects[2].y = tex->rect.y;
            rects[2].w = -tex->speed;
            rects[2].h = tex->rect.h;
            rects[3].x = tex->rect.x;
            rects[3].y = tex->rect.y;
            rects[3].w = tex->rect.w + tex->speed;
            rects[3].h = tex->rect.h;
        }
    } else {
        tex->speed = tex->speed % tex->rect.h;
        if (tex->speed > 0) {
            rects[0].x = tex->rect.x;
            rects[0].y = tex->rect.y + tex->rect.h - tex->speed;
            rects[0].w = tex->rect.w;
            rects[0].h = tex->speed;
            rects[1].x = tex->rect.x;
            rects[1].y = tex->rect.y;
            rects[1].w = tex->rect.w;
            rects[1].h = tex->rect.h - tex->speed;
            rects[2].x = tex->rect.x;
            rects[2].y = tex->rect.y;
            rects[2].w = tex->rect.w;
            rects[2].h = tex->speed;
            rects[3].x = tex->rect.x;
            rects[3].y = tex->rect.y + tex->speed;
            rects[3].w = tex->rect.w;
            rects[3].h = tex->rect.h - tex->speed;
        } else {
            rects[0].x = tex->rect.x;
            rects[0].y = tex->rect.y;
            rects[0].w = tex->rect.w;
            rects[0].h = -tex->speed;
            rects[1].x = tex->rect.x;
            rects[1].y = tex->rect.y - tex->speed;
            rects[1].w = tex->rect.w;
            rects[1].h = tex->rect.h + tex->speed;
            rects[2].x = tex->rect.x;
            rects[2].y = tex->rect.y + tex->rect.h + tex->speed;
            rects[2].w = tex->rect.w;
            rects[2].h = -tex->speed;
            rects[3].x = tex->rect.x;
            rects[3].y = tex->rect.y;
            rects[3].w = tex->rect.w;
            rects[3].h = tex->rect.h + tex->speed;
        }
    }
    StoreImage(&rects[0], tex->buf1);
    StoreImage(&rects[1], tex->buf0);
    /* jp doesn't wait for the GPU between the copies */
#if VERSION_US || VERSION_EU
    DrawSync(0);
#endif
    LoadImage((s16 *)&rects[2], (s32)tex->buf1);
    LoadImage((s16 *)&rects[3], (s32)tex->buf0);
#if VERSION_US || VERSION_EU
    DrawSync(0);
#endif
}

void SUG_freeScrollTexture(void **obj) {
    obj[3] = (void *)freeHeapBlock(obj[3]);
    obj[2] = (void *)freeHeapBlock(obj[2]);
    freeHeapBlock(obj);
}
