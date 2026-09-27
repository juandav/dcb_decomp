#ifndef DCB_SCENE3D_H
#define DCB_SCENE3D_H

#include "game.h"

#define PULSE(n) (D_8006DF88 + (n) * 12)

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
} LINE_F2;
typedef struct {
    /* 0x000 */ MATRIX m;
    /* 0x020 */ VECTOR pos;
    /* 0x030 */ SVECTOR rot;
    /* 0x038 */ u8 unk38[0x104];
    /* 0x13C */ Model *model;
    /* 0x140 */ u8 unk140[0x42F];
    /* 0x56F */ u8 unk56F;
    /* 0x570 */ u8 unk570;
    /* 0x571 */ s8 unk571;
} ModelLink;

extern MATRIX D_801D6A08;
extern MATRIX D_801D6A28;
extern SVECTOR D_801D6A78;
extern void *D_801D6A58[];
extern SVECTOR *D_801D6A50;
extern s32 *D_801D6A48;
extern s16 D_801D69F8;
extern s16 D_801D69FA;
extern s16 D_801D69FC;
extern s16 D_801D69FE;
extern s16 D_801D6A00;
extern s32 D_8006DF84;
extern u8 D_8006DF88;
extern s32 D_8007956C;

void func_800243B0(s32 h);
void func_80024420(void);
void func_80023DF0();
void func_800246E0();
void func_80024B08(s32 w, s32 h, s32 cols, s32 rows, s32 unused, s32 vertical);
void func_80024DD4(void);
s32 func_80024E44(u8 *cam, s32 *pos, s32 cur, s16 *target);
void func_80024460(s32);

#endif /* DCB_SCENE3D_H */
