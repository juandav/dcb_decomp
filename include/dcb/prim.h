#ifndef DCB_PRIM_H
#define DCB_PRIM_H

#include "game.h"

void initVramSprite(VramSprite *packet, s16 x, s16 y, s16 clut, s32 colorMode, s32 vramX, s32 vramY, s32 width, s32 height, s32 blendMode);
void fillVramRect(s32 x, s32 y, s32 w, s32 h, u32 color);
void drawTexturedSprite(s32 x, s32 y, Rect16 *uvRect, u16 tpage, s32 clut, s32 otz, u8 brightness, s8 blendMode);
void initPolyFT4Pair(POLY_FT4 *poly, POLY_FT4 *otherPoly, u8 *color, s32 tpage, s32 clut, Rect16 *uvRect, Rect16 *xyRect,
                   u8 semiTrans, u8 tinted);
void initPolyFT3Pair(POLY_FT3 *poly, s32 *otherPoly, u8 *color, s32 tpage, s32 clut, Rect16 *uvRect, Rect16 *xyRect,
                   u8 semiTrans, u8 tinted);
void initPolyGT3Pair(POLY_GT3 *poly, POLY_GT3 *otherPoly, u8 *color0, u8 *color1, u8 *color2, s32 tpage, s32 clut,
                   Rect16 *uvRect, Rect16 *xyRect, u8 semiTrans);
void initPolyGT4Pair(POLY_GT4 *poly, POLY_GT4 *otherPoly, u8 *color0, u8 *color1, u8 *color2, u8 *color3, s32 tpage,
                   s32 clut, Rect16 *uvRect, Rect16 *xyRect, u8 semiTrans, u8 unused);
void initPolyG4Pair(POLY_G4 *poly, POLY_G4 *otherPoly, u8 *color0, u8 *color1, u8 *color2, u8 *color3, s32 blendMode,
                   void *tpage0, void *tpage1, Rect16 *xyRect, u8 semiTrans, u8 unused);
void initPolyG3Pair(s32 *poly, s32 *otherPoly, u8 *color0, u8 *color1, u8 *color2, s32 blendMode, void *tpage0, void *tpage1,
                   u8 semiTrans);

#endif /* DCB_PRIM_H */
