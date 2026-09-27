#ifndef DCB_PRIM_UTIL_H
#define DCB_PRIM_UTIL_H

#include "game.h"

extern void (*PRIM_INIT_FUNCS[])(void *);

void setPrimRgb1(void *prim, u8 r, u8 g, u8 b);
void initPrimByType(s32 type, void *prim, s32 semiTrans, s32 shadeTex);
void setPrimQuadUvRect(u8 *poly, u8 u, u8 v, u8 w, u8 h);
void setPrimQuadRect(void *prim, s16 x, s16 y, s16 w, s16 h);
void setPrimRgb2(void *prim, u8 r, u8 g, u8 b);
void setPrimRgb3(void *prim, u8 r, u8 g, u8 b);
void setPrimRgb0(void *prim, u8 r, u8 g, u8 b);
s32 stepColorToward(s32 step, u8 *red, s32 targetRed, u8 *green, s32 targetGreen, u8 *blue, s32 targetBlue);
void stepPrimFade(u8 fadeOut, s16 step, u8 *state, u8 *prim);
void uploadClut256(s32 clutData, s16 x, s16 y);
void setPolyGRgb1(void *poly, u8 r, u8 g, u8 b);
void setPolyGTRgb1(void *poly, u8 r, u8 g, u8 b);
void setPolyGRgb2(void *poly, u8 r, u8 g, u8 b);
void setPolyGTRgb2(void *poly, u8 r, u8 g, u8 b);
void setPolyG4Rgb3(void *poly, u8 r, u8 g, u8 b);
void setPolyGT4Rgb3(void *poly, u8 r, u8 g, u8 b);
void setPolyG4Colors(u8 *poly, u8 *colors);
void setPolyGT4Colors(u8 *poly, u8 *colors);
void setPrimQuadColors(u8 *prim, u8 *colors);
void setPolyGT4Rect(void *poly, s16 x, s16 y, s16 w, s16 h);
void setPolyG4Rect(void *poly, s16 x, s16 y, s16 w, s16 h);
void setPolyFT4Rect(void *poly, s16 x, s16 y, s16 w, s16 h);
void setPolyF4Rect(void *poly, s16 x, s16 y, s16 w, s16 h);
void setPolyGT4UvRect(u8 *poly, u8 u, u8 v, u8 w, u8 h);
void setPolyFT4UvRect(u8 *poly, u8 u, u8 v, u8 w, u8 h);

#endif /* DCB_PRIM_UTIL_H */
