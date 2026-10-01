#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

void initVramSprite(VramSprite *packet, s16 x, s16 y, s16 clut, s32 colorMode, s32 vramX, s32 vramY, s32 width, s32 height, s32 blendMode) {
    s32 u;
    s32 blend;

    setlen(&packet->sp, 4);
    packet->sp.code = 0x64; /* SPRT */
    packet->sp.clut = clut;
    packet->sp.w = width;
    packet->sp.h = height;
    packet->sp.x0 = x;
    packet->sp.y0 = y;
    /* u in texels: a texture page is 64 VRAM pixels wide, 128 texels at 8 bits and 256 at 4 */
    if (colorMode != 0) {
        u = (vramX % 64) * 2;
    } else {
        u = (vramX % 64) * 4;
    }
    packet->sp.u0 = u;
    packet->sp.v0 = vramY;
    packet->sp.r0 = 0x80;
    packet->sp.g0 = 0x80;
    packet->sp.b0 = 0x80;
    if (blendMode >= 0) {
        packet->sp.code |= 2; /* semi-transparent */
        blend = blendMode;
    } else {
        blend = 0;
    }
    /* the texture page, as getTPage(colorMode, blend, vramX, vramY) */
    SetDrawMode(packet, 0, 0, ((colorMode & 3) << 7) | ((blend & 3) << 5) | ((vramY & 0x100) >> 4) | ((vramX & 0x3C0) >> 6) | ((vramY & 0x200) * 4), &GRAPHICS);
    MargePrim(packet, &packet->sp);
}

void fillVramRect(s32 x, s32 y, s32 w, s32 h, u32 color) {
    Rect16 rect;
    u32 *buf;
    u32 *cursor;
    s32 i;

    w /= 4;
    if (w == 0 || h == 0) {
        return;
    }
    rect.x = x;
    rect.w = w;
    /* x is reused as the fill size in bytes */
    if (getLargestFreeHeapBlock() < w * (h << 2)) {
        x = getLargestFreeHeapBlock();
    } else {
        x = w * (h << 2);
    }
    rect.h = (u32)x / (w << 2);
    if (rect.h <= 0) {
        return;
    }
    buf = allocTaskHeapBlock(rect.h * (w << 2));
    if (buf == NULL) {
        return;
    }
    cursor = buf;
    for (x -= 4; x >= 0; x -= 4) {
        *cursor++ = color;
    }
    /* upload the rect in bands of as many rows as the buffer holds */
    rect.y = y;
    for (i = 0; i < h; i += rect.h, rect.y += rect.h) {
        rect.h = (h - i < rect.h) ? h - i : rect.h;
        LoadImage((s16 *)&rect, (s32)buf);
    }
    DrawSync(0);
    freeHeapBlock(buf);
}

void drawTexturedSprite(s32 x, s32 y, Rect16 *uvRect, u16 tpage, s32 clut, s32 otz, u8 brightness, s8 blendMode) {
    u16 tpageBits = tpage;

    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = uvRect->x;
        CUR_SPRT->sp.v0 = uvRect->y;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = uvRect->w;
        CUR_SPRT->sp.h = uvRect->h;
        CUR_SPRT->sp.r0 = brightness;
        CUR_SPRT->sp.g0 = brightness;
        CUR_SPRT->sp.b0 = brightness;
        if (blendMode >= 0) {
            tpageBits = tpage | (blendMode & 3) << 5;
            setSemiTrans(&CUR_SPRT->sp, 1);
        } else {
            setSemiTrans(&CUR_SPRT->sp, 0);
        }
        setDrawMode(&CUR_SPRT->dm, 0, 0, tpageBits);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}
