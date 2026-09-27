#ifndef DCB_TEXT_H
#define DCB_TEXT_H

#include "game.h"

#define setSprt(p) setlen(p, 4), setcode(p, 0x64)

extern u16 D_801D6B10;
extern u16 D_801D6B20;
extern s16 D_801D6B22;
extern s32 D_8006DF98;
extern u8 D_8006DF9C[];

s32 func_80029990(void);
void func_800271D0(void);
void func_80026E90(s32, s32, s32);
s8 *func_8002A5B4(s8 *d, s8 *s);
void func_80027DB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80028228(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_800293FC(u8 *);
void func_80028D18(s32, s32, s32, s32, s32);
void func_80027044(void);
void func_80027228(s32, s32, Rect16 *, u8 *, u16, s32, s32);
void func_800271EC(s32 arg0, s32 arg1, s32 arg2, u16 arg3, s32 arg4, s32 arg5);
void func_80027458(s32, s32, s32, s32, s32, s32, s32, u8 *, s32);
void func_80027410(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void func_800276C8(s32 x, s32 y, u8 c, s32 n, u8 *rgb, s32 z, s32 w, s32 h, s32 bu, s32 bv);
void func_80027674(s32 x, s32 y, u8 c, s32 n, s32 z, s32 w, s32 h, s32 bu, s32 bv);
void func_8002793C(s32, s32, u8 *, s32, u8 *, s32);
void func_8002790C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80029A0C(s32, s32, s32, s32, u8 *, s32);
void func_80027DE8(s32, s32, u8 *, s32, u8 *, s32);
void func_80028258(s32, s32, u8 *, s32, u8 *, s32);
void func_80028588(s32, s32, u8 *, s32, u8 *, s32);
void func_80028558(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_800289D0(s32, s32, u8 *, s32, u8 *, s32);
void func_800289A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_80028D48(s32, s32, u8 *, u8 *, s32, s32);
void func_800299DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
s32 func_80029EFC(s32, s32, s32, s32, u8 *, s32, u8 *);
void func_80029EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void func_8002961C(s32, s32, u8 *, u8 *, s32, s32);

#endif /* DCB_TEXT_H */
