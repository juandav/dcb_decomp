#ifndef DCB_PRIM_PAIR_H
#define DCB_PRIM_PAIR_H

#include "game.h"
#include "dcb/prim.h"

void initPolyF4Pair(s32 *poly, s32 *otherPoly, u8 *color, s32 blendMode, void *tpage0, void *tpage1, s16 *xyRect, u8 semiTrans);
void initPolyF3Pair(s32 *poly, s32 *otherPoly, u8 *color, s32 blendMode, void *tpage0, void *tpage1, u8 semiTrans);
void initLineG2Pair(s32 *line, s32 *otherLine, u8 *color0, u8 *color1, s32 blendMode, void *tpage0, void *tpage1, u8 semiTrans,
                    u8 unused);
void initLineF2Pair(s32 *line, s32 *otherLine, u8 *color, s32 blendMode, void *tpage0, void *tpage1, u8 semiTrans);

#endif /* DCB_PRIM_PAIR_H */
