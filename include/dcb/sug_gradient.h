#ifndef DCB_SUG_GRADIENT_H
#define DCB_SUG_GRADIENT_H

#include "game.h"

void SUG_setGridUvsFT4(POLY_FT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink);
void SUG_setGridUvsGT4(POLY_GT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink);
void SUG_fillGradientColors(void *a0, s32 a1, s32 a2, s32 a3, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, Bytes4 *a8);

#endif /* DCB_SUG_GRADIENT_H */
