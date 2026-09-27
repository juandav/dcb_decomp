#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim_util.h"

/* the size in bytes of each primitive type */
u8 PRIM_SIZES[24] = {
    0x10, 0x14, 0x18, 0x20, 0x1C, 0x28, 0x14, 0x1C,
    0x18, 0x24, 0x20, 0x28, 0x28, 0x34, 0x14, 0x10,
    0x10, 0x10, 0xC, 0xC, 0xC, 0,
};

typedef void (*PrimInit)(void *);

/* the function that sets up each primitive type */
PrimInit PRIM_INIT_FUNCS[21] = {
    func_800678E4,
    func_80067904,
    func_80067924,
    SetLineG3,
    func_80067974,
    SetLineG4,
    func_80067704,
    func_80067744,
    func_80067784,
    (PrimInit)func_800677C4,
    (PrimInit)func_80067724,
    (PrimInit)func_80067764,
    (PrimInit)func_800677A4,
    (PrimInit)func_800677E4,
    (PrimInit)func_80067844,
    func_80067804,
    func_80067824,
    (PrimInit)func_800678C4,
    func_80067864,
    func_80067884,
    func_800678A4,
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
        level = prim[4];
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
    (*(u8 *)((s8 *)prim + 4)) = r;
    (*(u8 *)((s8 *)prim + 5)) = g;
    (*(u8 *)((s8 *)prim + 6)) = b;
}

void setPrimRgb1(void *prim, u8 r, u8 g, u8 b) {
    if (*((u8 *)prim + 7) & 4) {
        setPolyGTRgb1(prim, r, g, b);
    } else {
        setPolyGRgb1(prim, r, g, b);
    }
}

void setPrimRgb2(void *prim, u8 r, u8 g, u8 b) {
    if (*((u8 *)prim + 7) & 4) {
        setPolyGTRgb2(prim, r, g, b);
    } else {
        setPolyGRgb2(prim, r, g, b);
    }
}

void setPrimRgb3(void *prim, u8 r, u8 g, u8 b) {
    if (*((u8 *)prim + 7) & 4) {
        setPolyGT4Rgb3(prim, r, g, b);
    } else {
        setPolyG4Rgb3(prim, r, g, b);
    }
}

void setPrimQuadColors(u8 *prim, u8 *colors) {
    if (prim[7] & 4) {
        setPolyGT4Colors(prim, colors);
    } else {
        setPolyG4Colors(prim, colors);
    }
}

void setPolyGRgb1(void *poly, u8 r, u8 g, u8 b) {
    (*(s8 *)((s8 *)poly + 0xC)) = r;
    (*(s8 *)((s8 *)poly + 0xD)) = g;
    (*(s8 *)((s8 *)poly + 0xE)) = b;
}

void setPolyGTRgb1(void *poly, u8 r, u8 g, u8 b) {
    (*(s8 *)((s8 *)poly + 0x10)) = r;
    (*(s8 *)((s8 *)poly + 0x11)) = g;
    (*(s8 *)((s8 *)poly + 0x12)) = b;
}

void setPolyGRgb2(void *poly, u8 r, u8 g, u8 b) {
    (*(s8 *)((s8 *)poly + 0x14)) = r;
    (*(s8 *)((s8 *)poly + 0x15)) = g;
    (*(s8 *)((s8 *)poly + 0x16)) = b;
}

void setPolyGTRgb2(void *poly, u8 r, u8 g, u8 b) {
    (*(s8 *)((s8 *)poly + 0x1C)) = r;
    (*(s8 *)((s8 *)poly + 0x1D)) = g;
    (*(s8 *)((s8 *)poly + 0x1E)) = b;
}

void setPolyG4Rgb3(void *poly, u8 r, u8 g, u8 b) {
    (*(s8 *)((s8 *)poly + 0x1C)) = r;
    (*(s8 *)((s8 *)poly + 0x1D)) = g;
    (*(s8 *)((s8 *)poly + 0x1E)) = b;
}

void setPolyGT4Rgb3(void *poly, u8 r, u8 g, u8 b) {
    (*(s8 *)((s8 *)poly + 0x28)) = r;
    (*(s8 *)((s8 *)poly + 0x29)) = g;
    (*(s8 *)((s8 *)poly + 0x2A)) = b;
}

void setPolyG4Colors(u8 *poly, u8 *colors) {
    poly[0x4] = *colors++;
    poly[0x5] = *colors++;
    poly[0x6] = *colors++;
    poly[0xC] = *colors++;
    poly[0xD] = *colors++;
    poly[0xE] = *colors++;
    poly[0x14] = *colors++;
    poly[0x15] = *colors++;
    poly[0x16] = *colors++;
    poly[0x1C] = *colors++;
    poly[0x1D] = *colors++;
    poly[0x1E] = *colors;
}

void setPolyGT4Colors(u8 *poly, u8 *colors) {
    poly[0x4] = *colors++;
    poly[0x5] = *colors++;
    poly[0x6] = *colors++;
    poly[0x10] = *colors++;
    poly[0x11] = *colors++;
    poly[0x12] = *colors++;
    poly[0x1C] = *colors++;
    poly[0x1D] = *colors++;
    poly[0x1E] = *colors++;
    poly[0x28] = *colors++;
    poly[0x29] = *colors++;
    poly[0x2A] = *colors;
}

void setPrimQuadRect(void *prim, s16 x, s16 y, s16 w, s16 h) {
    s32 kind;

    kind = (*(u8 *)((s8 *)prim + 7)) & 0x14;
    switch (kind) {                              /* irregular */
    case 0:
        setPolyF4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    case 4:
        setPolyFT4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    case 16:
        setPolyG4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    case 20:
        setPolyGT4Rect(prim, x, y, w, (s16) (s32) h);
        return;
    }
}

void setPolyF4Rect(void *poly, s16 x, s16 y, s16 w, s16 h) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)poly + 8)) = x;
    (*(s16 *)((s8 *)poly + 0xA)) = y;
    x1 = x + w;
    (*(s16 *)((s8 *)poly + 0xC)) = x1;
    (*(s16 *)((s8 *)poly + 0xE)) = y;
    (*(s16 *)((s8 *)poly + 0x10)) = x;
    y1 = y + h;
    (*(s16 *)((s8 *)poly + 0x12)) = y1;
    (*(s16 *)((s8 *)poly + 0x14)) = x1;
    (*(s16 *)((s8 *)poly + 0x16)) = y1;
}

void setPolyG4Rect(void *poly, s16 x, s16 y, s16 w, s16 h) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)poly + 8)) = x;
    (*(s16 *)((s8 *)poly + 0xA)) = y;
    x1 = x + w;
    (*(s16 *)((s8 *)poly + 0x10)) = x1;
    (*(s16 *)((s8 *)poly + 0x12)) = y;
    (*(s16 *)((s8 *)poly + 0x18)) = x;
    y1 = y + h;
    (*(s16 *)((s8 *)poly + 0x1A)) = y1;
    (*(s16 *)((s8 *)poly + 0x20)) = x1;
    (*(s16 *)((s8 *)poly + 0x22)) = y1;
}

void setPolyFT4Rect(void *poly, s16 x, s16 y, s16 w, s16 h) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)poly + 8)) = x;
    (*(s16 *)((s8 *)poly + 0xA)) = y;
    x1 = x + w;
    (*(s16 *)((s8 *)poly + 0x10)) = x1;
    (*(s16 *)((s8 *)poly + 0x12)) = y;
    (*(s16 *)((s8 *)poly + 0x18)) = x;
    y1 = y + h;
    (*(s16 *)((s8 *)poly + 0x1A)) = y1;
    (*(s16 *)((s8 *)poly + 0x20)) = x1;
    (*(s16 *)((s8 *)poly + 0x22)) = y1;
}

void setPolyGT4Rect(void *poly, s16 x, s16 y, s16 w, s16 h) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)poly + 8)) = x;
    (*(s16 *)((s8 *)poly + 0xA)) = y;
    x1 = x + w;
    (*(s16 *)((s8 *)poly + 0x14)) = x1;
    (*(s16 *)((s8 *)poly + 0x16)) = y;
    (*(s16 *)((s8 *)poly + 0x20)) = x;
    y1 = y + h;
    (*(s16 *)((s8 *)poly + 0x22)) = y1;
    (*(s16 *)((s8 *)poly + 0x2C)) = x1;
    (*(s16 *)((s8 *)poly + 0x2E)) = y1;
}

void setPrimQuadUvRect(u8 *poly, u8 u, u8 v, u8 w, u8 h) {
    if (poly[7] & 0x10) {
        setPolyGT4UvRect(poly, u, v, w, h);
    } else {
        setPolyFT4UvRect(poly, u, v, w, h);
    }
}

void setPolyFT4UvRect(u8 *poly, u8 u, u8 v, u8 w, u8 h) {
    poly[0xC] = u;
    poly[0xD] = v;
    poly[0x14] = u + w;
    poly[0x15] = v;
    poly[0x1C] = u;
    poly[0x1D] = v + h;
    poly[0x24] = u + w;
    poly[0x25] = v + h;
}

void setPolyGT4UvRect(u8 *poly, u8 u, u8 v, u8 w, u8 h) {
    poly[0xC] = u;
    poly[0xD] = v;
    poly[0x18] = u + w;
    poly[0x19] = v;
    poly[0x24] = u;
    poly[0x25] = v + h;
    poly[0x30] = u + w;
    poly[0x31] = v + h;
}
