#include "common.h"
#include "game.h"
#include "dcb/main.h"
#include "dcb/heap.h"
#include "dcb/system.h"

s32 D_8006DD3C[2] = { 0, 0 };

int main(void) {
    Rect16 r;

    ResetCallback();
    VSync(0);
    SetDispMask(0);
    GsInitGraph(320, 240, 0, 0, 0);
    r.x = 0;
    r.y = 0;
    r.w = 640;
    r.h = 511;
    ClearImage(&r, 0, 0, 0);
    DrawSync(0);
    SsInit();
    func_8001AA80(1);
    func_800149A8(1, 0x400, func_800155F4, 0, 0, 0, 0);
    for (;;) {
        rand();
    }
}

void func_80013F04(s32 arg0) {
    s16 *p;
    s32 i;

    func_8006A804();
    if (arg0 != 0) {
        if (D_80077A08 == 0) {
            D_80077A08 = 1;
            p = &D_80077BA0;
            for (i = 0x1F; i >= 0; i--, p += 0x60) {
                if (*p > 0) {
                    D_80077AEC = p;
                    break;
                }
            }
        }
    } else if (D_80077A08 != 0) {
        D_80077A08 = 0;
        D_80077AEC = &D_80077BA0;
    }
    func_8006A814();
}

s32 func_80013FA4(s32 mode, s32 size, s32 pc, s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 *p;
    s32 *t;
    s32 *tcb;
    s32 *src;
    s32 *dst;
    s32 i;
    s32 j;
    s32 stack;
    s32 ev;

    func_8006A804();
    D_80077A08 = mode;
    for (p = (s32 *)&D_80077BA0, i = 0x1F; i >= 0; i--, p += 0x30) {
        *p = 0;
    }
    D_80077A18 = D_80077A1A = *(u16 *)&D_80077A1C = 0xFFFF;
    D_80077A0C = (Unk80077A0C *)((Thread *)&D_80077BA0 - 1);
    ((Thread *)&D_80077AE0)->flags = 0x8000FFFF;
    ((Thread *)&D_80077AE0)->next = (Thread *)D_80077A0C + 1;
    if (D_80077A08 != 0) {
        ((Thread *)&D_80077AE0)->prev = (Thread *)&D_80077AE0;
    } else {
        D_80077AEC = &D_80077BA0;
    }
    ((Thread *)&D_80077AE0)->unk14 = -1;
    tcb = *(s32 **)0x108;
    D_80077ADC = *tcb;
    src = (s32 *)(D_80077ADC + 8);
    dst = &D_80077BC0;
    for (j = 0x27; j >= 0; j--) {
        *dst++ = *src++;
    }
    t = (s32 *)&D_80077BA0;
    t[0] = 0xA0000000;
    t[0x28] = pc;
    t[0x2B] = 0x4000FF04;
    t[0xC] = a0;
    t[0xD] = a1;
    t[0xE] = a2;
    t[0xF] = a3;
    t[2] = (s32)&D_80077AE0;
    t[3] = (s32)&D_80077AE0;
    t[5] = 0;
    t[6] = 0;
    stack = (s32)func_8001ABCC(size, -3);
    if (stack == 0) {
        return -6;
    }
    t[7] = stack;
    t[0x25] = stack + (size & ~7) - 0x20;
    ev = func_8006A794(0xF2000003, 2, 0x1000, (long (*)())func_800141B8);
    func_8006A7C4(ev);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    D_80077AD8 = ev;
    func_8006A814();
    return 0;
}

long func_800141B8(void) {
    Thread *t;
    s32 *ctx;
    s32 *regs;
    s32 i;

    t = (Thread *)D_80077A0C;
    func_80014CF0();
    ctx = (s32 *)(D_80077ADC + 8);
    regs = t->regs;
    for (i = 0x27; i >= 0; i--) {
        *regs++ = *ctx++;
    }
    t->flags |= 0x20000000;
    if ((D_80077A1A = D_80077A18) == 0) {
        if (D_80077A08 == 0) {
            D_80077A14 = &D_80077BA0;
            D_80077A1C = 0;
        }
    } else {
        D_80077A10 = t;
        t = (Thread *)&D_80077BA0;
        D_80077A0C = (Unk80077A0C *)t;
        D_80077A18 = D_80077BA0;
        ctx = (s32 *)(D_80077ADC + 8);
        regs = t->regs;
        for (i = 0x27; i >= 0; i--) {
            *ctx++ = *regs++;
        }
    }
}

void *func_800142D0(void *t) {
    s16 *p = *(s16 **)((s8 *)t + 0xC);
    s16 prio = *p;

    if (*p > 0 && prio == D_80077A1A) {
        D_80077A14 = p;
        D_80077A1C = prio;
        p = D_80077A10;
        D_80077A1A = -1;
    } else {
        prio = *p;
        if ((u16)prio > (u16)D_80077A1C) {
            prio = D_80077A1C;
            p = D_80077A14;
            *(u16 *)&D_80077A1C = 0xFFFF;
        }
    }
    D_80077A0C = p;
    D_80077A18 = prio;
    return p;
}

s32 func_80014364(s32 id, s32 where, s32 prio, s32 size, s32 unused, s32 pc, s32 a0, s32 a1, s32 a2, s32 a3) {
    Thread *t;
    Thread *prev;
    Thread *next;
    s32 ret;
    s32 f;
    s32 g;
    s32 *src;
    s32 *dst;
    s32 i;
    s32 stack;

    t = (Thread *)&D_80077BA0 + id;
    if (id != 0) {
        if ((s32)t->flags < 0) {
            return -1;
        }
    } else {
        for (id++; id < 32; id++) {
            if ((s32)(++t)->flags >= 0) {
                break;
            }
        }
        if (id >= 32) {
            return -1;
        }
    }
    ret = 0;
    if (where < 0) {
        prev = (Thread *)&D_80077BA0;
        if (prio >= *(u16 *)prev) {
            do {
                prev = prev->prev;
            } while (prio >= *(u16 *)prev);
        }
        goto after;
    }
    if (where >= 32) {
        where -= 32;
        prev = (Thread *)&D_80077BA0 + where;
        if ((s32)prev->flags >= 0) {
            return -3;
        }
        f = prev->flags;
        if ((u16)f < prio) {
            ret = -0x86;
            prio = f;
        }
    after:
        next = prev->next;
        next->prev = t;
    } else {
        next = (Thread *)&D_80077BA0 + where;
        if ((s32)next->flags >= 0) {
            return -3;
        }
        g = next->flags;
        if (prio < (u16)g) {
            ret = -0x86;
            prio = g;
        }
        prev = next->prev;
        next->prev = t;
    }
    prev->next = t;
    t->next = next;
    t->prev = prev;
    if (D_80077A08 != 0 && prio > 0 && *(s16 *)next == 0) {
        D_80077AEC = (s16 *)t;
    }
    src = (s32 *)(D_80077ADC + 8);
    dst = t->regs;
    for (i = 0x27; i >= 0; i--) {
        *dst++ = *src++;
    }
    t->flags = prio | 0xA0000000;
    t->regs[32] = pc;
    t->regs[35] = 0x4000FF04;
    t->regs[4] = a0;
    t->regs[5] = a1;
    t->regs[6] = a2;
    t->regs[7] = a3;
    t->regs[31] = (s32)func_80014A90;
    t->regs[28] = D_80077C30;
    t->unk14 = id;
    t->unk18 = 0;
    stack = (s32)func_8001ABCC(size, -3);
    if (stack == 0) {
        return -6;
    }
    t->stack = stack;
    t->regs[29] = stack + (size & ~7) - 0x20;
    return ret;
}

s32 func_80014614(s32 arg0) {
    void *t;
    void *prev;
    void *next;
    void **cur;

    t = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)t + 0)) >= 0) {
        return -0x83;
    }
    if (t == D_80077A0C) {
        return -4;
    }
    if (t == &D_80077BA0) {
        return -5;
    }
    prev = (*(void **)((s8 *)t + 8));
    next = (*(void **)((s8 *)t + 0xC));
    (*(void **)((s8 *)prev + 0xC)) = next;
    (*(void **)((s8 *)next + 8)) = prev;
    if (D_80077A08 != 0) {
        cur = (void **)&D_80077AE0;
        if (cur[3] == t) {
            cur[3] = next;
        }
    }
    if ((D_80077A1A > 0) && (t == D_80077A10)) {
        D_80077A10 = next;
        D_80077A1A = (*(u16 *)((s8 *)next + 0));
    }
    if ((D_80077A1C >= 0) && (t == D_80077A14)) {
        D_80077A14 = next;
        D_80077A1C = (*(u16 *)((s8 *)next + 0));
    }
    func_8001AFF0((*(s32 *)((s8 *)t + 0x14)));
    func_8001AE90((*(void **)((s8 *)t + 0x1C)));
    (*(s32 *)((s8 *)t + 0)) = 0;
    return 0;
}

void func_80014748(void) {
    void *t;
    void *prev;
    void *next;
    void **cur;

    t = D_80077A0C;
    prev = (*(void **)((s8 *)t + 8));
    next = (*(void **)((s8 *)t + 0xC));
    (*(void **)((s8 *)prev + 0xC)) = next;
    (*(void **)((s8 *)next + 8)) = prev;
    if (D_80077A08 != 0) {
        cur = (void **)&D_80077AE0;
        if (cur[3] == t) {
            cur[3] = next;
        }
    }
    if ((D_80077A1A > 0) && (t == D_80077A10)) {
        D_80077A10 = next;
        D_80077A1A = (*(u16 *)((s8 *)next + 0));
    }
    if ((D_80077A1C >= 0) && (t == D_80077A14)) {
        D_80077A14 = next;
        D_80077A1C = (*(u16 *)((s8 *)next + 0));
    }
    func_8001AFF0((*(s32 *)((s8 *)t + 0x14)));
    func_8001AE90((*(void **)((s8 *)t + 0x1C)));
    (*(s32 *)((s8 *)t + 0)) = 0;
    func_800142D0(t);
}

int func_80014840(void) {
    int n = D_80077A0C->unk14;
    int i;
    int ret = 0;

    for (i = 1; i < 0x20; i++) {
        if (i == n) {
            continue;
        }
        if (func_80014A00(i) == 0) {
            ret++;
        }
    }
    return ret;
}

s32 func_800148B0(void) {
    return (*(s32 *)((s8 *)D_80077A0C + 0x14));
}

s32 func_800148C8(s32 arg0, s32 arg1) {
    void *temp_v1;

    temp_v1 = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)temp_v1 + 0)) >= 0) {
        return -3;
    }
    if (temp_v1 == D_80077A0C) {
        return -0x84;
    }
    (*(s32 *)((s8 *)temp_v1 + 0x18)) = arg1;
    (*(s32 *)((s8 *)temp_v1 + 4)) = 0;
    return 0;
}

s32 func_8001491C(s32 arg0) {
    void *temp_v1;

    temp_v1 = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)temp_v1 + 0)) >= 0) {
        return -3;
    }
    if (temp_v1 == D_80077A0C) {
        return -0x84;
    }
    return (*(s32 *)((s8 *)temp_v1 + 4));
}
