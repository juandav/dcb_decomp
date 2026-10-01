#include "common.h"
#include "game.h"
#include "dcb/sug_gradient.h"
#include "dcb/prim_util.h"

typedef struct {
    u8 *c00;
    u8 *c10;
    u8 *c01;
    u8 *c11;
    s32 wx0;
    s32 wx1;
    s32 wy0;
    s32 wy1;
} Blend;

/* fills a grid of a2 x a3 cells, 4 colors each, by blending the corner colors; the kinds split it
   into 1, 2 or 4 blends around the middle color a8 */
void SUG_fillGradientColors(void *a0, s32 a1, s32 a2, s32 a3, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, Bytes4 *a8) {
    Bytes4 top;
    Bytes4 left;
    Bytes4 right;
    Bytes4 bottom;
    Blend blend;
    Bytes4 *out;
    s32 x;
    s32 y;

    out = (Bytes4 *)a0;
    switch ((u8)a1) {
    case 0:
        out[0] = *c0;
        out[1] = *c1;
        out[2] = *c2;
        out[3] = *c3;
        break;
    case 1:
        blend.c00 = c0->b;
        blend.c10 = c1->b;
        blend.c01 = c2->b;
        blend.c11 = c3->b;
        for (y = 0; y < a3; y++) {
            for (x = 0; x < a2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
        }
        break;
    case 2:
        blend.c00 = c0->b;
        blend.c10 = c1->b;
        blend.c01 = a8->b;
        blend.c11 = a8->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
        }
        blend.c00 = a8->b;
        blend.c10 = a8->b;
        blend.c01 = c2->b;
        blend.c11 = c3->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
        }
        break;
    case 3:
        out = (Bytes4 *)a0;
        blend.c00 = c0->b;
        blend.c10 = a8->b;
        blend.c01 = c2->b;
        blend.c11 = a8->b;
        for (y = 0; y < a3; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 / 2 * 4;
        blend.c00 = a8->b;
        blend.c10 = c1->b;
        blend.c01 = a8->b;
        blend.c11 = c3->b;
        for (y = 0; y < a3; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        break;
    case 4:
        blend.c00 = c0->b;
        blend.c10 = c1->b;
        blend.c01 = c2->b;
        blend.c11 = c3->b;
        blend.wx0 = a2 / 2;
        blend.wx1 = a2 / 2;
        blend.wy0 = 0;
        blend.wy1 = a3;
        SUG_blendCornerColors(&blend, top.b);
        blend.wx0 = 0;
        blend.wx1 = a2;
        blend.wy0 = a3 / 2;
        blend.wy1 = a3 / 2;
        SUG_blendCornerColors(&blend, left.b);
        blend.wx0 = a2;
        blend.wx1 = 0;
        blend.wy0 = a3 / 2;
        blend.wy1 = a3 / 2;
        SUG_blendCornerColors(&blend, right.b);
        blend.wx0 = a2 / 2;
        blend.wx1 = a2 / 2;
        blend.wy0 = a3;
        blend.wy1 = 0;
        SUG_blendCornerColors(&blend, bottom.b);
        out = (Bytes4 *)a0;
        blend.c00 = c0->b;
        blend.c10 = top.b;
        blend.c01 = left.b;
        blend.c11 = a8->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 / 2 * 4;
        blend.c00 = top.b;
        blend.c10 = c1->b;
        blend.c01 = a8->b;
        blend.c11 = right.b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 * a3 / 2 * 4;
        blend.c00 = left.b;
        blend.c10 = a8->b;
        blend.c01 = c2->b;
        blend.c11 = bottom.b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 * a3 / 2 * 4 + a2 / 2 * 4;
        blend.c00 = a8->b;
        blend.c10 = right.b;
        blend.c01 = bottom.b;
        blend.c11 = c3->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        break;
    }
}

void SUG_blendCornerColors(Blend *blend, u8 *out) {
    s32 bottom[3];
    s32 top[3];

    top[0] = (blend->c10[0] * blend->wx0 + blend->c00[0] * blend->wx1) / (blend->wx0 + blend->wx1);
    top[1] = (blend->c10[1] * blend->wx0 + blend->c00[1] * blend->wx1) / (blend->wx0 + blend->wx1);
    top[2] = (blend->c10[2] * blend->wx0 + blend->c00[2] * blend->wx1) / (blend->wx0 + blend->wx1);
    bottom[0] = (blend->c11[0] * blend->wx0 + blend->c01[0] * blend->wx1) / (blend->wx0 + blend->wx1);
    bottom[1] = (blend->c11[1] * blend->wx0 + blend->c01[1] * blend->wx1) / (blend->wx0 + blend->wx1);
    bottom[2] = (blend->c11[2] * blend->wx0 + blend->c01[2] * blend->wx1) / (blend->wx0 + blend->wx1);
    out[0] = (blend->wy0 * bottom[0] + blend->wy1 * top[0]) / (blend->wy0 + blend->wy1);
    out[1] = (blend->wy0 * bottom[1] + blend->wy1 * top[1]) / (blend->wy0 + blend->wy1);
    out[2] = (blend->wy0 * bottom[2] + blend->wy1 * top[2]) / (blend->wy0 + blend->wy1);
}

/* jp has no grid UV setters */
#if VERSION_US || VERSION_EU
void SUG_setGridUvsFT4(POLY_FT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink) {
    Rect16 r;
    s32 col;
    s32 stepX;
    s32 stepY;
    s16 cellW;
    s16 cellH;
    s32 x;
    s32 row;
    s32 y;
    s32 i;

    stepX = (uv->w << 16) / cols;
    stepY = (uv->h << 16) / rows;
    cellW = uv->w / cols;
    cellH = uv->h / rows;
    r.w = cellW;
    r.h = cellH;
    col = 0;
    row = 0;
    x = 0;
    y = 0;
    for (i = 0; i < count; i++, polys++) {
        r.x = uv->x + (x >> 16);
        r.y = uv->y + (y >> 16);
        if (row == rows - 1 && shrink) {
            r.h = cellH - 1;
        } else {
            r.h = cellH;
        }
        if (col >= cols - 1) {
            col = 0;
            x = 0;
            row++;
            y += stepY;
            if (shrink) {
                r.w = cellW - 1;
            } else {
                r.w = cellW;
            }
        } else {
            col++;
            x += stepX;
            r.w = cellW;
        }
        setPrimQuadUvRect((u8 *)polys, r.x, r.y, r.w + padW, r.h + padH);
    }
}

void SUG_setGridUvsGT4(POLY_GT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink) {
    Rect16 cell;
    s32 col;
    s32 stepX;
    s32 stepY;
    s16 cellW;
    s16 cellH;
    s32 row;
    s32 accX;
    s32 accY;
    s32 i;

    stepX = (uv->w << 16) / cols;
    stepY = (uv->h << 16) / rows;
    cellW = uv->w / cols;
    cellH = uv->h / rows;
    cell.w = cellW;
    cell.h = cellH;
    col = 0;
    row = 0;
    accX = 0;
    accY = 0;
    for (i = 0; i < count; i++, polys++) {
        cell.x = uv->x + (accX >> 16);
        cell.y = uv->y + (accY >> 16);
        if (shrink) {
            if (row == rows - 1) {
                cell.h = cellH - 1;
            } else {
                cell.h = cellH;
            }
        }
        if (col >= cols - 1) {
            col = 0;
            accX = 0;
            row++;
            accY += stepY;
            if (shrink) {
                cell.w = cellW - 1;
            }
        } else {
            col++;
            accX += stepX;
            cell.w = cellW;
        }
        setPrimQuadUvRect((u8 *)polys, cell.x, cell.y, cell.w + padW, cell.h + padH);
    }
}
#endif
