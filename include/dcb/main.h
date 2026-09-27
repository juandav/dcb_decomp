#ifndef DCB_MAIN_H
#define DCB_MAIN_H

#include "game.h"

typedef struct {
    /* 0x00 */ char unk0[0x14];
    /* 0x14 */ int unk14;
} Unk80077A0C;
typedef struct Thread {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct Thread *next;
    /* 0x0C */ struct Thread *prev;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 stack;
    /* 0x20 */ s32 regs[40];
} Thread;

extern s32 D_80077A08;
extern s16 *D_80077AEC;
extern s16 D_80077BA0;
extern s16 D_80077A18;
extern s16 D_80077A1A;
extern s16 D_80077A1C;
extern s32 D_80077AE0;
extern s32 D_80077ADC;
extern s32 D_80077AD8;
extern s32 D_80077BC0;
extern Unk80077A0C *D_80077A0C;
extern void *D_80077A10;
extern void *D_80077A14;
extern s32 D_80077C30;

int main(void);
void func_80013F04(s32 arg0);
long func_800141B8();
s32 func_80013FA4(s32 mode, s32 size, s32 pc, s32 a0, s32 a1, s32 a2, s32 a3);
s32 func_80014364(s32 id, s32 where, s32 prio, s32 size, s32 unused, s32 pc, s32 a0, s32 a1, s32 a2, s32 a3);
s32 func_80014614(s32 arg0);
void *func_800142D0(void *);
void func_80014748(void);
int func_80014840(void);
s32 func_800148C8(s32 arg0, s32 arg1);
s32 func_8001491C(s32 arg0);
s32 func_800148B0();

#endif /* DCB_MAIN_H */
