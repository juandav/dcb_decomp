#ifndef DCB_PRIM_UTIL_H
#define DCB_PRIM_UTIL_H

#include "game.h"

extern void (*D_8006DF0C[])(void *);

void func_8001E76C(void *, u8, u8, u8);
void func_8001E6EC(s32, void *, s32, s32);
void func_8001EC3C(u8 *, u8, u8, u8, u8);
void func_8001EA64(void *, s16, s16, s16, s16);
void func_8001E7B8(void *, u8, u8, u8);
void func_8001E804(void *, u8, u8, u8);
void func_8001E75C(void *, u8, u8, u8);
s32 func_8001E3C8(s32 step, u8 *r, s32 tr, u8 *g, s32 tg, u8 *b, s32 tb);
void func_8001E4E8(u8 dir, s16 step, u8 *state, u8 *prim);
void func_8001E6A4(s32 arg0, s16 arg1, s16 arg2);
void func_8001E894(void *, u8, u8, u8);
void func_8001E8A4(void *, u8, u8, u8);
void func_8001E8B4(void *, u8, u8, u8);
void func_8001E8C4(void *, u8, u8, u8);
void func_8001E8D4(void *, u8, u8, u8);
void func_8001E8E4(void *, u8, u8, u8);
void func_8001E8F4(u8 *p, u8 *c);
void func_8001E9AC(u8 *p, u8 *c);
void func_8001E850(u8 *p, u8 *c);
void func_8001EBF4(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001EB64(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001EBAC(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001EB1C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001ECC8(u8 *, u8, u8, u8, u8);
void func_8001EC8C(u8 *, u8, u8, u8, u8);
void func_8001ED04(void *arg0);

#endif /* DCB_PRIM_UTIL_H */
