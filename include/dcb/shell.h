#ifndef DCB_SHELL_H
#define DCB_SHELL_H

#include "game.h"

typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ s8 unk4[6];
    /* 0xA */ u8 unkA[2];
} Entry12;

extern u8 D_80012D68[];
extern u8 D_80012DB8[];
extern u8 D_80012DF8[];
extern s32 D_801D8538;
extern s32 D_801D853C;
extern s32 D_801D8540;
extern s32 D_801D8544;
extern s32 D_801D854C;
extern s32 D_801D8550;
extern u8 *D_801D8554;
extern u8 *D_801D8558;
extern s32 D_801D8460;
extern s32 D_801D84F4;
extern s32 D_801D84B0;
extern s32 D_801D8410;
extern s32 D_801D8548;
extern u8 *D_8006EF04[];
/* the same text as in func_800416D8, kept as its own copy */
extern char D_80012FAC[];

void func_80049EF8(s32 n, s32 arg1);
s32 func_80049840(Entry12 *tbl, s32 a, s32 b);
s32 func_80049934(s32 arg0);
s32 func_8004994C(s32 a, s32 b);
void func_80049DC0(void *arg0);
void func_80049E00(void *arg0);
void func_80049E40(void *arg0);
void func_80049A14();
void func_80049E80(void);
void func_8004A2DC(s32 mode);

#endif /* DCB_SHELL_H */
