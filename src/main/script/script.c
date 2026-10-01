#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/script.h"
#include "dcb/heap.h"
#include "dcb/angle.h"

Script *createScriptContext(void *scriptData) {
    ScriptData *data = scriptData;
    Script *script;

    script = allocTaskHeapBlock(sizeof(Script));
    script->base = scriptData;
    script->start = data->code;
    script->pc = data->code;
    script->offset = 0;
    script->size = ((ScriptData *)script->base)->size;
    clearScriptBusy(script);
    return script;
}

void initScriptContext(void *scriptData, Script *script) {
    ScriptData *data = scriptData;

    script->base = scriptData;
    script->start = data->code;
    script->pc = data->code;
    script->offset = 0;
    script->size = ((ScriptData *)script->base)->size;
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

void freeScriptContext(Script *script, s32 *regs) {
    freeHeapBlock(regs);
    freeHeapBlock(script);
}

s32 runScriptToNextEvent(Script *script, s32 *regs) {
    u8 *pc;
    u32 op;
    s32 skip;
    s32 cond;
    s32 i;
    u32 unalignedPc;
    u16 *operand;

    if (script->busy != 0) {
        return -1;
    }
    cond = skip = 0;
    pc = script->pc;
    script->event = 0;
    /* Instructions are a u16 opcode and operands, padded to 4 bytes. Runs
       until one of them is an event for the caller to handle. */
    while (script->event == 0 && script->size > script->offset) {
        op = *(u16 *)pc;
        switch (op) {
        case 6: { /* event with an inline block of OP_A bytes */
            u8 *cur = pc;

            if (!skip) {
                script->event = pc;
                script->eventOp = op;
                script->eventArg = 0;
            }
            pc += OP_A(cur) + 4;
            break;
        }
        case 5: { /* jump to an offset from the start of the code */
            u8 *cur = pc;

            if (!skip) {
                pc = script->start;
                pc += *(s32 *)(cur + 4);
                break;
            }
            pc += 8;
            break;
        }
        case 8: { /* register OP_A = address of the inline block of OP_B bytes */
            u8 *cur = pc;

            if (!skip) {
                regs[OP_A(pc)] = (s32)(pc + 6);
            }
            pc += OP_B(cur) + 6;
            break;
        }
        case 7: /* arithmetic on register OP_A, with a constant or a register */
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
        case 9: /* comparison: when it holds, the next instruction is skipped */
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
        /* 10-14: events with 0 to 4 parameters, each a constant or a register */
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
        script->offset = (u32)pc;
        script->offset -= (u32)script->base;
    }
    script->pc = pc;
    return script->event != 0;
}

void clearScriptBusy(Script *script) {
    script->busy = 0;
}

void setScriptBusy(Script *script, s16 busy) {
    script->busy = busy;
}
