#include "dcb/prim_pair.h"
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

void initPolyFT4Pair(POLY_FT4 *poly, POLY_FT4 *otherPoly, u8 *color, s32 tpage, s32 clut, Rect16 *uvRect, Rect16 *xyRect,
                   u8 semiTrans, u8 tinted) {
    func_800677A4(poly);
    poly->tpage = tpage;
    poly->clut = clut;
    SetShadeTex(poly, tinted ^ 1);
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    if (color != 0) {
        setPrimRgb0(poly, color[0], color[1], color[2]);
    }
    if (uvRect != 0) {
        setPrimQuadUvRect((u8 *)poly, uvRect->x, uvRect->y, uvRect->w, uvRect->h);
    }
    if (xyRect != 0) {
        setPrimQuadRect(poly, xyRect->x, xyRect->y, xyRect->w, xyRect->h);
    }
    *otherPoly = *poly;
}

void initPolyFT3Pair(POLY_FT3 *poly, s32 *otherPoly, u8 *color, s32 tpage, s32 clut, Rect16 *uvRect, Rect16 *xyRect,
                   u8 semiTrans, u8 tinted) {
    func_80067724(poly);
    poly->tpage = tpage;
    poly->clut = clut;
    SetShadeTex(poly, tinted ^ 1);
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    if (color != 0) {
        setPrimRgb0(poly, color[0], color[1], color[2]);
    }
    if (uvRect != 0) {
        poly->u0 = uvRect->x + uvRect->w / 2;
        poly->v0 = uvRect->y;
        poly->u1 = uvRect->x;
        poly->v1 = uvRect->y + uvRect->h;
        poly->u2 = uvRect->x + uvRect->w;
        poly->v2 = uvRect->y + uvRect->h;
    }
    if (xyRect != 0) {
        poly->x0 = xyRect->x + xyRect->w / 2;
        poly->y0 = xyRect->y;
        poly->x1 = xyRect->x;
        poly->y1 = xyRect->y + xyRect->h;
        poly->x2 = xyRect->x + xyRect->w;
        poly->y2 = xyRect->y + xyRect->h;
    }
    otherPoly[0] = ((s32 *)poly)[0];
    otherPoly[1] = ((s32 *)poly)[1];
    otherPoly[2] = ((s32 *)poly)[2];
    otherPoly[3] = ((s32 *)poly)[3];
    otherPoly[4] = ((s32 *)poly)[4];
    otherPoly[5] = ((s32 *)poly)[5];
    otherPoly[6] = ((s32 *)poly)[6];
    otherPoly[7] = ((s32 *)poly)[7];
}

void initPolyGT3Pair(POLY_GT3 *poly, POLY_GT3 *otherPoly, u8 *color0, u8 *color1, u8 *color2, s32 tpage, s32 clut,
                   Rect16 *uvRect, Rect16 *xyRect, u8 semiTrans) {
    func_80067764(poly);
    poly->tpage = tpage;
    poly->clut = clut;
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    if (color0 != 0) {
        setPrimRgb0(poly, color0[0], color0[1], color0[2]);
    }
    if (color1 != 0) {
        setPrimRgb1(poly, color1[0], color1[1], color1[2]);
    }
    if (color2 != 0) {
        setPrimRgb2(poly, color2[0], color2[1], color2[2]);
    }
    if (uvRect != 0) {
        poly->u0 = uvRect->x + uvRect->w / 2;
        poly->v0 = uvRect->y;
        poly->u1 = uvRect->x;
        poly->v1 = uvRect->y + uvRect->h;
        poly->u2 = uvRect->x + uvRect->w;
        poly->v2 = uvRect->y + uvRect->h;
    }
    if (xyRect != 0) {
        poly->x0 = xyRect->x + xyRect->w / 2;
        poly->y0 = xyRect->y;
        poly->x1 = xyRect->x;
        poly->y1 = xyRect->y + xyRect->h;
        poly->x2 = xyRect->x + xyRect->w;
        poly->y2 = xyRect->y + xyRect->h;
    }
    *otherPoly = *poly;
}

void initPolyGT4Pair(POLY_GT4 *poly, POLY_GT4 *otherPoly, u8 *color0, u8 *color1, u8 *color2, u8 *color3, s32 tpage,
                   s32 clut, Rect16 *uvRect, Rect16 *xyRect, u8 semiTrans) {
    func_800677E4(poly);
    poly->tpage = tpage;
    poly->clut = clut;
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    } else {
        SetSemiTrans(poly, 0);
    }
    if (color0 != 0) {
        setPrimRgb0(poly, color0[0], color0[1], color0[2]);
    }
    if (color1 != 0) {
        setPrimRgb1(poly, color1[0], color1[1], color1[2]);
    }
    if (color2 != 0) {
        setPrimRgb2(poly, color2[0], color2[1], color2[2]);
    }
    if (color3 != 0) {
        setPrimRgb3(poly, color3[0], color3[1], color3[2]);
    }
    if (uvRect != 0) {
        setPrimQuadUvRect((u8 *)poly, uvRect->x, uvRect->y, uvRect->w, uvRect->h);
    }
    if (xyRect != 0) {
        setPrimQuadRect(poly, xyRect->x, xyRect->y, xyRect->w, xyRect->h);
    }
    *otherPoly = *poly;
}

void initPolyF4Pair(s32 *poly, s32 *otherPoly, u8 *color, s32 blendMode, void *tpage0, void *tpage1, s16 *xyRect, u8 semiTrans) {
    func_80067784(poly);
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    }
    if (color != 0) {
        setPrimRgb0(poly, color[0], color[1], color[2]);
    }
    if (xyRect != 0) {
        setPrimQuadRect(poly, xyRect[0], xyRect[1], xyRect[2], xyRect[3]);
    }
    otherPoly[0] = poly[0];
    otherPoly[1] = poly[1];
    otherPoly[2] = poly[2];
    otherPoly[3] = poly[3];
    otherPoly[4] = poly[4];
    otherPoly[5] = poly[5];
    if (tpage0 != 0) {
        SetDrawTPage(tpage0, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
    if (tpage1 != 0) {
        SetDrawTPage(tpage1, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
}

void initPolyG4Pair(POLY_G4 *poly, POLY_G4 *otherPoly, u8 *color0, u8 *color1, u8 *color2, u8 *color3, s32 blendMode,
                   void *tpage0, void *tpage1, Rect16 *xyRect, u8 semiTrans) {
    func_800677C4(poly);
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    }
    if (color0 != 0) {
        setPrimRgb0(poly, color0[0], color0[1], color0[2]);
    }
    if (color1 != 0) {
        setPrimRgb1(poly, color1[0], color1[1], color1[2]);
    }
    if (color2 != 0) {
        setPrimRgb2(poly, color2[0], color2[1], color2[2]);
    }
    if (color3 != 0) {
        setPrimRgb3(poly, color3[0], color3[1], color3[2]);
    }
    if (xyRect != 0) {
        setPrimQuadRect(poly, xyRect->x, xyRect->y, xyRect->w, xyRect->h);
    }
    *otherPoly = *poly;
    if (tpage0 != 0) {
        SetDrawTPage(tpage0, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
    if (tpage1 != 0) {
        SetDrawTPage(tpage1, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
}

void initPolyG3Pair(s32 *poly, s32 *otherPoly, u8 *color0, u8 *color1, u8 *color2, s32 blendMode, void *tpage0, void *tpage1,
                   u8 semiTrans) {
    func_80067744(poly);
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    }
    if (color0 != 0) {
        setPrimRgb0(poly, color0[0], color0[1], color0[2]);
    }
    if (color1 != 0) {
        setPrimRgb1(poly, color1[0], color1[1], color1[2]);
    }
    if (color2 != 0) {
        setPrimRgb2(poly, color2[0], color2[1], color2[2]);
    }
    otherPoly[0] = poly[0];
    otherPoly[1] = poly[1];
    otherPoly[2] = poly[2];
    otherPoly[3] = poly[3];
    otherPoly[4] = poly[4];
    otherPoly[5] = poly[5];
    otherPoly[6] = poly[6];
    if (tpage0 != 0) {
        SetDrawTPage(tpage0, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
    if (tpage1 != 0) {
        SetDrawTPage(tpage1, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
}

void initPolyF3Pair(s32 *poly, s32 *otherPoly, u8 *color, s32 blendMode, void *tpage0, void *tpage1, u8 semiTrans) {
    func_80067704(poly);
    if (semiTrans) {
        SetSemiTrans(poly, 1);
    }
    if (color != 0) {
        setPrimRgb0(poly, color[0], color[1], color[2]);
    }
    otherPoly[0] = poly[0];
    otherPoly[1] = poly[1];
    otherPoly[2] = poly[2];
    otherPoly[3] = poly[3];
    otherPoly[4] = poly[4];
    if (tpage0 != 0) {
        SetDrawTPage(tpage0, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
    if (tpage1 != 0) {
        SetDrawTPage(tpage1, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
}

void initLineG2Pair(s32 *line, s32 *otherLine, u8 *color0, u8 *color1, s32 blendMode, void *tpage0, void *tpage1, u8 semiTrans) {
    func_80067904(line);
    if (semiTrans) {
        SetSemiTrans(line, 1);
    }
    if (color0 != 0) {
        setPrimRgb0(line, color0[0], color0[1], color0[2]);
    }
    if (color1 != 0) {
        setPrimRgb1(line, color1[0], color1[1], color1[2]);
    }
    otherLine[0] = line[0];
    otherLine[1] = line[1];
    otherLine[2] = line[2];
    otherLine[3] = line[3];
    otherLine[4] = line[4];
    if (tpage0 != 0) {
        SetDrawTPage(tpage0, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
    if (tpage1 != 0) {
        SetDrawTPage(tpage1, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
}

void initLineF2Pair(s32 *line, s32 *otherLine, u8 *color, s32 blendMode, void *tpage0, void *tpage1, u8 semiTrans) {
    func_800678E4(line);
    if (semiTrans) {
        SetSemiTrans(line, 1);
    }
    if (color != 0) {
        setPrimRgb0(line, color[0], color[1], color[2]);
    }
    otherLine[0] = line[0];
    otherLine[1] = line[1];
    otherLine[2] = line[2];
    otherLine[3] = line[3];
    if (tpage0 != 0) {
        SetDrawTPage(tpage0, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
    if (tpage1 != 0) {
        SetDrawTPage(tpage1, 0, 0, GetTPage(0, blendMode, 0, 0));
    }
}
