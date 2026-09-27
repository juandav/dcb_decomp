#ifndef DCB_FADE_H
#define DCB_FADE_H

#include "game.h"

extern s32 D_801D69E0;
extern s32 D_801D69E4;
extern s32 D_801D69E8;
extern s32 D_801D69EC;
extern s32 D_801D69F0;
extern FadePoly D_801D69A0[2];
extern u32 D_801D69D0[2][2];

void func_8001F040(void);
void func_8001F04C(void);
s32 func_8001F058(void);
void func_8001F068(s32 arg0, s32 arg1, s32 arg2);
void func_8001F094(s32 dir, s32 abr, s32 speed);

#endif /* DCB_FADE_H */
