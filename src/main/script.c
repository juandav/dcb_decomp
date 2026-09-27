#include "common.h"
#include "gte.h"
#include "game.h"

void *func_80020E34(void *arg0) {
    s32 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_8001AD0C(0x28);
    (*(void **)((s8 *)temp_v0 + 0)) = arg0;
    temp_v0_2 = arg0 + 0x10;
    (*(s32 *)((s8 *)temp_v0 + 4)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 8)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 0xC)) = 0;
    (*(s32 *)((s8 *)temp_v0 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(temp_v0);
    return temp_v0;
}

void func_80020E94(void *arg0, void *arg1) {
    s32 temp_v0;

    (*(void **)((s8 *)arg1 + 0)) = arg0;
    temp_v0 = arg0 + 0x10;
    (*(s32 *)((s8 *)arg1 + 4)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 8)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 0xC)) = 0;
    (*(s32 *)((s8 *)arg1 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(arg1);
}

s32 *func_80020ED4(s32 n) {
    s32 *p = func_8001AD0C(n * 4);
    s32 *q = p;
    s32 i;

    for (i = 0; i < n; i++) {
        *q++ = 0;
    }
    return p;
}

void func_80020F24(void *arg0, void *arg1) {
    func_8001AE90(arg1);
    func_8001AE90(arg0);
}

s32 func_80020F54(Script *s, s32 *regs) {
    u8 *pc;
    u16 op;
    s32 skip;
    s32 cond;
    s32 i;
    u32 next;
    u16 *arg;

    if (s->busy != 0) {
        return -1;
    }
    skip = 0;
    pc = s->pc;
    s->event = 0;
    cond = 0;
    if (s->size > s->offset) {
        do {
            op = *(u16 *)pc;
            switch (op) {
            case 6: {
                u8 *cur = pc;

                if (!skip) {
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = 0;
                }
                pc += OP_A(cur) + 4;
                break;
            }
            case 5: {
                u8 *cur = pc;

                if (!skip) {
                    pc = s->start;
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
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                }
                pc += 4;
                break;
            }
            case 11: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 1; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 8;
                break;
            }
            case 12: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 2; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 12;
                break;
            }
            case 13: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 3; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 16;
                break;
            }
            case 14: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 4; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
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
            next = (u32)pc + 3;
            pc = (u8 *)(next & ~3);
            s->offset = pc - s->base;
        } while (s->event == 0 && s->offset < s->size);
    }
    s->pc = pc;
    return s->event != 0;
}

void func_80021954(void *arg0) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = 0;
}

void func_8002195C(void *arg0, s16 arg1) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = arg1;
}
