#ifndef DCB_SCRIPT_H
#define DCB_SCRIPT_H

#include "game.h"

#define OP_A(p) (*(u16 *)((p) + 2))
#define OP_B(p) (*(u16 *)((p) + 4))
#define OP_MODE(p) (*(u16 *)((p) + 6))
#define OP_VAL(p) (*(s32 *)((p) + 8))

typedef struct {
    /* 0x00 */ u8 *base;
    /* 0x04 */ u8 *start;
    /* 0x08 */ u8 *pc;
    /* 0x0C */ u32 offset;
    /* 0x10 */ u32 size;
    /* 0x14 */ u8 *event;
    /* 0x18 */ u16 eventOp;
    /* 0x1A */ u16 eventArg;
    /* 0x1C */ u16 params[4];
    /* 0x24 */ s16 busy;
} Script;

void func_80021954(void *arg0);
void *func_80020E34(void *arg0);
void func_80020E94(void *arg0, void *arg1);
s32 *func_80020ED4(s32 n);
void func_80020F24(void *arg0, void *arg1);
s32 func_80020F54(Script *s, s32 *regs);
void func_8002195C(void *arg0, s16 arg1);

#endif /* DCB_SCRIPT_H */
