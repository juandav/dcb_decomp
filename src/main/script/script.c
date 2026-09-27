#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/script.h"
#include "dcb/heap.h"

void *createScriptContext(void *scriptData) {
    s32 codeStart;
    void *script;

    script = allocTaskHeapBlock(0x28);
    (*(void **)((s8 *)script + 0)) = scriptData;
    codeStart = scriptData + 0x10;
    (*(s32 *)((s8 *)script + 4)) = codeStart;
    (*(s32 *)((s8 *)script + 8)) = codeStart;
    (*(s32 *)((s8 *)script + 0xC)) = 0;
    (*(s32 *)((s8 *)script + 0x10)) = (s32) (*(s32 *)((s8 *)scriptData + 8));
    clearScriptBusy(script);
    return script;
}

void initScriptContext(void *scriptData, void *script) {
    s32 codeStart;

    (*(void **)((s8 *)script + 0)) = scriptData;
    codeStart = scriptData + 0x10;
    (*(s32 *)((s8 *)script + 4)) = codeStart;
    (*(s32 *)((s8 *)script + 8)) = codeStart;
    (*(s32 *)((s8 *)script + 0xC)) = 0;
    (*(s32 *)((s8 *)script + 0x10)) = (s32) (*(s32 *)((s8 *)scriptData + 8));
    clearScriptBusy(script);
}

s32 *allocScriptRegisters(s32 count) {
    s32 *regs = allocTaskHeapBlock(count * 4);
    s32 *cursor = regs;
    s32 i;

    for (i = 0; i < count; i++) {
        *cursor++ = 0;
    }
    return regs;
}

void freeScriptContext(void *script, void *regs) {
    freeHeapBlock(regs);
    freeHeapBlock(script);
}

s32 runScriptToNextEvent(Script *script, s32 *regs) {
    u8 *pc;
    u16 op;
    s32 skip;
    s32 cond;
    s32 i;
    u32 unalignedPc;
    u16 *operand;

    if (script->busy != 0) {
        return -1;
    }
    skip = 0;
    pc = script->pc;
    script->event = 0;
    cond = 0;
    if (script->size > script->offset) {
        do {
            op = *(u16 *)pc;
            switch (op) {
            case 6: {
                u8 *cur = pc;

                if (!skip) {
                    script->event = pc;
                    script->eventOp = op;
                    script->eventArg = 0;
                }
                pc += OP_A(cur) + 4;
                break;
            }
            case 5: {
                u8 *cur = pc;

                if (!skip) {
                    pc = script->start;
                    pc += *(s32 *)(cur + 4);
                    break;
                }
                pc += 8;
                break;
            }
            case 8: {
                u8 *cur = pc;

                if (!skip) {
                    regs[OP_A(pc)] = (s32)(pc + 6);
                }
                pc += OP_B(cur) + 6;
                break;
            }
            case 7:
                if (!skip) {
                    u8 *cur = pc;

                    switch (OP_B(pc)) {
                    case 0:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] = OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] = regs[OP_VAL(cur)];
                        }
                        break;
                    case 1:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] += OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] += regs[OP_VAL(cur)];
                        }
                        break;
                    case 2:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] -= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] -= regs[OP_VAL(cur)];
                        }
                        break;
                    case 3:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] *= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] *= regs[OP_VAL(cur)];
                        }
                        break;
                    case 4:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] /= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] /= regs[OP_VAL(cur)];
                        }
                        break;
                    case 5:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] %= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] %= regs[OP_VAL(cur)];
                        }
                        break;
                    case 6:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] = rand() % (OP_VAL(cur) + 1);
                        } else {
                            regs[OP_A(cur)] = rand() % (regs[OP_VAL(cur)] + 1);
                        }
                        break;
                    }
                }
                pc += 12;
                break;
            case 9:
                if (!skip) {
                    u8 *cur = pc;

                    switch (OP_B(pc)) {
                    case 0:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] == OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] == regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 1:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] <= OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] <= regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 2:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] < OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] < regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 3:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] != OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] != regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 4:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] > OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] > regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 5:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] >= OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] >= regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    }
                } else {
                    cond = 1;
                }
                pc += 12;
                break;
            case 10: {
                u8 *cur = pc;

                if (!skip) {
                    script->event = pc;
                    script->eventOp = op;
                    script->eventArg = OP_A(cur);
                }
                pc += 4;
                break;
            }
            case 11: {
                u8 *cur = pc;

                if (!skip) {
                    operand = (u16 *)(cur + 4);
                    script->event = pc;
                    script->eventOp = op;
                    script->eventArg = OP_A(cur);
                    for (i = 0; i < 1; i++) {
                        if (operand[0] == 0) {
                            script->params[i] = operand[1];
                        } else {
                            script->params[i] = regs[operand[1]];
                        }
                        operand += 2;
                    }
                }
                pc += 8;
                break;
            }
            case 12: {
                u8 *cur = pc;

                if (!skip) {
                    operand = (u16 *)(cur + 4);
                    script->event = pc;
                    script->eventOp = op;
                    script->eventArg = OP_A(cur);
                    for (i = 0; i < 2; i++) {
                        if (operand[0] == 0) {
                            script->params[i] = operand[1];
                        } else {
                            script->params[i] = regs[operand[1]];
                        }
                        operand += 2;
                    }
                }
                pc += 12;
                break;
            }
            case 13: {
                u8 *cur = pc;

                if (!skip) {
                    operand = (u16 *)(cur + 4);
                    script->event = pc;
                    script->eventOp = op;
                    script->eventArg = OP_A(cur);
                    for (i = 0; i < 3; i++) {
                        if (operand[0] == 0) {
                            script->params[i] = operand[1];
                        } else {
                            script->params[i] = regs[operand[1]];
                        }
                        operand += 2;
                    }
                }
                pc += 16;
                break;
            }
            case 14: {
                u8 *cur = pc;

                if (!skip) {
                    operand = (u16 *)(cur + 4);
                    script->event = pc;
                    script->eventOp = op;
                    script->eventArg = OP_A(cur);
                    for (i = 0; i < 4; i++) {
                        if (operand[0] == 0) {
                            script->params[i] = operand[1];
                        } else {
                            script->params[i] = regs[operand[1]];
                        }
                        operand += 2;
                    }
                }
                pc += 20;
                break;
            }
            }
            skip = 0;
            if (cond) {
                cond = 0;
                skip = 1;
            }
            unalignedPc = (u32)pc + 3;
            pc = (u8 *)(unalignedPc & ~3);
            script->offset = pc - script->base;
        } while (script->event == 0 && script->offset < script->size);
    }
    script->pc = pc;
    return script->event != 0;
}

void clearScriptBusy(void *script) {
    (*(s16 *)((s8 *)script + 0x24)) = 0;
}

void setScriptBusy(void *script, s16 busyValue) {
    (*(s16 *)((s8 *)script + 0x24)) = busyValue;
}
