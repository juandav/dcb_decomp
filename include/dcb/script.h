#ifndef DCB_SCRIPT_H
#define DCB_SCRIPT_H

#include "game.h"

#define OP_A(p) (*(u16 *)((p) + 2))
#define OP_B(p) (*(u16 *)((p) + 4))
#define OP_MODE(p) (*(u16 *)((p) + 6))
#define OP_VAL(p) (*(s32 *)((p) + 8))

/* The header of a script file; the bytecode follows it. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ u32 size;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 code[1];
} ScriptData;

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

void clearScriptBusy(Script *script);
Script *createScriptContext(void *scriptData);
void initScriptContext(void *scriptData, Script *script);
s32 *allocScriptRegisters(s32 count);
void freeScriptContext(Script *script, s32 *regs);
s32 runScriptToNextEvent(Script *script, s32 *regs);
void setScriptBusy(Script *script, s16 busy);

#endif /* DCB_SCRIPT_H */
