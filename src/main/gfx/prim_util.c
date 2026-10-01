#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim_util.h"

/* the bits of a primitive's code byte */
#define PRIM_CODE_TEXTURED 0x04
#define PRIM_CODE_GOURAUD 0x10

void setPolyGRgb1(POLY_G3 *poly, u8 r, u8 g, u8 b);
void setPolyGRgb2(POLY_G3 *poly, u8 r, u8 g, u8 b);
void setPolyF4Rect(POLY_F4 *poly, s16 x, s16 y, s16 w, s16 h);

/* the size in bytes of each primitive type */
u8 PRIM_SIZES[24] = {
    0x10, 0x14, 0x18, 0x20, 0x1C, 0x28, 0x14, 0x1C,
    0x18, 0x24, 0x20, 0x28, 0x28, 0x34, 0x14, 0x10,
    0x10, 0x10, 0xC, 0xC, 0xC, 0,
};

typedef void (*PrimInit)(void *);

/* the function that sets up each primitive type */
PrimInit PRIM_INIT_FUNCS[21] = {
    SetLineF2,
    SetLineG2,
    SetLineF3,
    SetLineG3,
    SetLineF4,
    SetLineG4,
    SetPolyF3,
    SetPolyG3,
    SetPolyF4,
    (PrimInit)SetPolyG4,
    (PrimInit)SetPolyFT3,
    (PrimInit)SetPolyGT3,
    (PrimInit)SetPolyFT4,
    (PrimInit)SetPolyGT4,
    (PrimInit)SetSprt,
    SetSprt8,
    SetSprt16,
    (PrimInit)SetTile,
    SetTile1,
    SetTile8,
    SetTile16,
};

s32 stepColorToward(s32 step, u8 *red, s32 targetRed, u8 *green, s32 targetGreen, u8 *blue, s32 targetBlue) {
    s16 vr;
    s16 vg;
    s16 vb;
    s32 doneCount;

    vr = *red;
    vg = *green;
    vb = *blue;
    doneCount = 0;
    if (vr < targetRed) {
        vr += step;
        if (vr > targetRed) {
            vr = targetRed;
            doneCount++;
        }
    } else if (vr > targetRed) {
        vr -= step;
        if (vr < targetRed) {
            vr = targetRed;
            doneCount++;
        }
    } else {
        doneCount++;
    }
    if (vg < targetGreen) {
        vg += step;
        if (vg > targetGreen) {
            vg = targetGreen;
            doneCount++;
        }
    } else if (vg > targetGreen) {
        vg -= step;
        if (vg < targetGreen) {
            vg = targetGreen;
            doneCount++;
        }
    } else {
        doneCount++;
    }
    if (vb < targetBlue) {
        vb += step;
        if (vb > targetBlue) {
            vb = targetBlue;
            doneCount++;
        }
    } else if (vb > targetBlue) {
        vb -= step;
        if (vb < targetBlue) {
            vb = targetBlue;
            doneCount++;
        }
    } else {
        doneCount++;
    }
    *red = vr;
    *green = vg;
    *blue = vb;
    return doneCount == 3;
}

void stepPrimFade(u8 fadeOut, s16 step, u8 *state, u8 *prim) {
    s16 level;

    switch (*state) {
    case 0:
        if (fadeOut == 0) {
            setPrimRgb0(prim, 0, 0, 0);
        } else {
            setPrimRgb0(prim, 0x80, 0x80, 0x80);
        }
        SetSemiTrans(prim, 1);
        *state = 1;
        break;
    case 1:
        if (fadeOut == 0) {
            setPrimRgb0(prim, 0, 0, 0);
        } else {
            setPrimRgb0(prim, 0x80, 0x80, 0x80);
        }
        SetSemiTrans(prim, 1);
        *state = 2;
        break;
    case 2:
        level = ((P_TAG *)prim)->r0;
        if (fadeOut == 0) {
            level += step;
            if (level >= 0x80) {
                SetSemiTrans(prim, 0);
                setPrimRgb0(prim, 0x80, 0x80, 0x80);
                *state = 3;
                break;
            }
        } else {
            level -= step;
            if (level <= 0) {
                SetSemiTrans(prim, 0);
                setPrimRgb0(prim, 0x80, 0x80, 0x80);
                *state = 3;
                break;
            }
        }
        setPrimRgb0(prim, level, level, level);
        return;
    case 3:
        SetSemiTrans(prim, 0);
        setPrimRgb0(prim, 0x80, 0x80, 0x80);
        *state = 4;
        break;
    case 4:
        *state = 5;
        break;
    }
}

void uploadClut256(s32 clutData, s16 x, s16 y) {
    s16 rect[4];

    rect[0] = x;
    rect[1] = y;
    rect[2] = 0x100;
    rect[3] = 1;
    LoadImage(rect, clutData);
    DrawSync(0);
}

void initPrimByType(s32 type, void *prim, s32 semiTrans, s32 shadeTex) {
    PRIM_INIT_FUNCS[type](prim);
    SetSemiTrans(prim, semiTrans);
    SetShadeTex(prim, shadeTex);
}

void setPrimRgb0(void *prim, u8 r, u8 g, u8 b) {
    setRGB0((P_TAG *)prim, r, g, b);
}

void setPrimRgb1(void *prim, u8 r, u8 g, u8 b) {
    if (getcode(prim) & PRIM_CODE_TEXTURED) {
        setPolyGTRgb1(prim, r, g, b);
    } else {
        setPolyGRgb1(prim, r, g, b);
    }
}

void setPrimRgb2(void *prim, u8 r, u8 g, u8 b) {
    if (getcode(prim) & PRIM_CODE_TEXTURED) {
        setPolyGTRgb2(prim, r, g, b);
    } else {
        setPolyGRgb2(prim, r, g, b);
    }
}

void setPrimRgb3(void *prim, u8 r, u8 g, u8 b) {
    if (getcode(prim) & PRIM_CODE_TEXTURED) {
        setPolyGT4Rgb3(prim, r, g, b);
    } else {
        setPolyG4Rgb3(prim, r, g, b);
    }
}

void setPrimQuadColors(u8 *prim, u8 *colors) {
    if (getcode(prim) & PRIM_CODE_TEXTURED) {
        setPolyGT4Colors((POLY_GT4 *)prim, colors);
    } else {
        setPolyG4Colors((POLY_G4 *)prim, colors);
    }
}

/* a POLY_G3 or a POLY_G4 */
void setPolyGRgb1(POLY_G3 *poly, u8 r, u8 g, u8 b) {
    poly->r1 = r;
    poly->g1 = g;
    poly->b1 = b;
}

/* a POLY_GT3 or a POLY_GT4 */
void setPolyGTRgb1(POLY_GT3 *poly, u8 r, u8 g, u8 b) {
    poly->r1 = r;
    poly->g1 = g;
    poly->b1 = b;
}

void setPolyGRgb2(POLY_G3 *poly, u8 r, u8 g, u8 b) {
    poly->r2 = r;
    poly->g2 = g;
    poly->b2 = b;
}

void setPolyGTRgb2(POLY_GT3 *poly, u8 r, u8 g, u8 b) {
    poly->r2 = r;
    poly->g2 = g;
    poly->b2 = b;
}

void setPolyG4Rgb3(POLY_G4 *poly, u8 r, u8 g, u8 b) {
    poly->r3 = r;
    poly->g3 = g;
    poly->b3 = b;
}

void setPolyGT4Rgb3(POLY_GT4 *poly, u8 r, u8 g, u8 b) {
    poly->r3 = r;
    poly->g3 = g;
    poly->b3 = b;
}

/* colors holds the four vertex colors as r, g, b triplets */
void setPolyG4Colors(POLY_G4 *poly, u8 *colors) {
    poly->r0 = *colors++;
    poly->g0 = *colors++;
    poly->b0 = *colors++;
    poly->r1 = *colors++;
    poly->g1 = *colors++;
    poly->b1 = *colors++;
    poly->r2 = *colors++;
    poly->g2 = *colors++;
    poly->b2 = *colors++;
    poly->r3 = *colors++;
    poly->g3 = *colors++;
    poly->b3 = *colors;
}

void setPolyGT4Colors(POLY_GT4 *poly, u8 *colors) {
    poly->r0 = *colors++;
    poly->g0 = *colors++;
    poly->b0 = *colors++;
    poly->r1 = *colors++;
    poly->g1 = *colors++;
    poly->b1 = *colors++;
    poly->r2 = *colors++;
    poly->g2 = *colors++;
    poly->b2 = *colors++;
    poly->r3 = *colors++;
    poly->g3 = *colors++;
    poly->b3 = *colors;
}

void setPrimQuadRect(void *prim, s16 x, s16 y, s16 w, s16 h) {
    s32 kind;

    kind = getcode(prim) & (PRIM_CODE_GOURAUD | PRIM_CODE_TEXTURED);
    switch (kind) {                              /* irregular */
    case 0:
        setPolyF4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    case PRIM_CODE_TEXTURED:
        setPolyFT4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    case PRIM_CODE_GOURAUD:
        setPolyG4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    case PRIM_CODE_GOURAUD | PRIM_CODE_TEXTURED:
        setPolyGT4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    }
}

/* the right and bottom edges that setPoly*Rect work out: jp and eu keep
   them in 16 bits */
#if VERSION_JP || VERSION_EU
typedef s16 RectEdge;
#elif VERSION_US
typedef s32 RectEdge;
#endif

void setPolyF4Rect(POLY_F4 *poly, s16 x, s16 y, s16 w, s16 h) {
    RectEdge right;
    RectEdge bottom;

    poly->x0 = x;
    poly->y0 = y;
    right = x + w;
    poly->x1 = right;
    poly->y1 = y;
    poly->x2 = x;
    bottom = y + h;
    poly->y2 = bottom;
    poly->x3 = right;
    poly->y3 = bottom;
}

void setPolyG4Rect(POLY_G4 *poly, s16 x, s16 y, s16 w, s16 h) {
    RectEdge right;
    RectEdge bottom;

    poly->x0 = x;
    poly->y0 = y;
    right = x + w;
    poly->x1 = right;
    poly->y1 = y;
    poly->x2 = x;
    bottom = y + h;
    poly->y2 = bottom;
    poly->x3 = right;
    poly->y3 = bottom;
}

void setPolyFT4Rect(POLY_FT4 *poly, s16 x, s16 y, s16 w, s16 h) {
    RectEdge right;
    RectEdge bottom;

    poly->x0 = x;
    poly->y0 = y;
    right = x + w;
    poly->x1 = right;
    poly->y1 = y;
    poly->x2 = x;
    bottom = y + h;
    poly->y2 = bottom;
    poly->x3 = right;
    poly->y3 = bottom;
}

void setPolyGT4Rect(POLY_GT4 *poly, s16 x, s16 y, s16 w, s16 h) {
    RectEdge right;
    RectEdge bottom;

    poly->x0 = x;
    poly->y0 = y;
    right = x + w;
    poly->x1 = right;
    poly->y1 = y;
    poly->x2 = x;
    bottom = y + h;
    poly->y2 = bottom;
    poly->x3 = right;
    poly->y3 = bottom;
}

void setPrimQuadUvRect(u8 *poly, u8 u, u8 v, u8 w, u8 h) {
    if (getcode(poly) & PRIM_CODE_GOURAUD) {
        setPolyGT4UvRect((POLY_GT4 *)poly, u, v, w, h);
    } else {
        setPolyFT4UvRect((POLY_FT4 *)poly, u, v, w, h);
    }
}

void setPolyFT4UvRect(POLY_FT4 *poly, u8 u, u8 v, u8 w, u8 h) {
    poly->u0 = u;
    poly->v0 = v;
    poly->u1 = u + w;
    poly->v1 = v;
    poly->u2 = u;
    poly->v2 = v + h;
    poly->u3 = u + w;
    poly->v3 = v + h;
}

void setPolyGT4UvRect(POLY_GT4 *poly, u8 u, u8 v, u8 w, u8 h) {
    poly->u0 = u;
    poly->v0 = v;
    poly->u1 = u + w;
    poly->v1 = v;
    poly->u2 = u;
    poly->v2 = v + h;
    poly->u3 = u + w;
    poly->v3 = v + h;
}
