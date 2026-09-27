#ifndef DCB_LOADER_H
#define DCB_LOADER_H

#include "game.h"

extern s32 D_8006DEF0;

void func_8001B90C(s32 w, s32 h, s32 interlace);
void func_8001B10C(s32 arg0, s32 arg1);
void func_8001B5BC(u32 *addr);
void func_8001B634(u32 *addr, s32 dx, s32 dy);
void func_8001B734(u32 *p);
void func_8001B7F4(u32 *p, s32 dx, s32 dy);
void func_8001B930();
s32 func_8001B248(s32 *, s32, s32);
void func_8001B358();
void func_8001B438(u32 *, s16, s16, s16, s16);
s32 func_8001B144();
void func_8001B90C(s32, s32, s32);

#endif /* DCB_LOADER_H */
