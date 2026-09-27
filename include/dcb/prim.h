#ifndef DCB_PRIM_H
#define DCB_PRIM_H

#include "game.h"

void func_8001C220(void *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);
void func_8001C354(s32 x, s32 y, s32 w, s32 h, u32 color);
void func_8001C4DC(s32 x, s32 y, Rect16 *r, u16 tpage, s32 clut, s32 z, u8 c, s8 abr);
void func_8001C6A4(POLY_FT4 *p, POLY_FT4 *dst, u8 *rgb, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy, u8 semi, u8 flat);
void func_8001C810(POLY_FT3 *p, s32 *dst, u8 *rgb, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy, u8 semi, u8 flat);
void func_8001CA54(POLY_GT3 *p, POLY_GT3 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy, u8 semi);
void func_8001CCB4(POLY_GT4 *p, POLY_GT4 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, u8 *rgb3, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy, u8 semi);
void func_8001CE74(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, s16 *r, u8 semi);
void func_8001CFDC(POLY_G4 *p, POLY_G4 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, u8 *rgb3, s32 abr, void *tp0, void *tp1, Rect16 *xy, u8 semi);
void func_8001D1AC(s32 *p, s32 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, s32 abr, void *tp0, void *tp1, u8 semi);
void func_8001D33C(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, u8 semi);
void func_8001D464(s32 *p, s32 *dst, u8 *rgb0, u8 *rgb1, s32 abr, void *tp0, void *tp1, u8 semi);
void func_8001D5B4(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, u8 semi);

#endif /* DCB_PRIM_H */
