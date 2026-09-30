#include "psyq.h"

typedef struct TickEnv {
    /* 0x00 */ long mode;
    /* 0x04 */ long manual;
    /* 0x08 */ void (*tick)();
    /* 0x0C */ void (*prev)();
    /* 0x10 */ char vsync;
    /* 0x11 */ char half;
    /* 0x12 */ char irq;
} TickEnv;

extern TickEnv D_8006F594;
void func_8004EF10(void);
void func_8004EF5C(void);

void func_8004ECA0(int start) {
    u_short target;
    u_long spec;
    long wait = 1000;

    while (--wait >= 0) {
    }
    D_8006F594.vsync = 0;
    D_8006F594.irq = 6;
    D_8006F594.half = 0;
    D_8006F594.prev = NULL;
    spec = 0xF2000002;
    target = 0x44E8;
    switch (D_8006F594.mode) {
    case 0:
        D_8006F594.irq = 0x7F;
        return;
    case 5:
        D_8006F594.irq = 0;
        if (start == 0) {
            D_8006F594.vsync = 1;
        } else {
            spec = 0xF2000003;
            target = 1;
        }
        break;
    case 3:
        target = 0x89D0;
        break;
    case 2:
        break;
    default:
        if (D_8006F594.manual != 0) {
            return;
        }
        if (D_8006F594.mode < 70) {
            target = 0x204CC0 / D_8006F594.mode;
            D_8006F594.half++;
        } else {
            target = 0x409980 / D_8006F594.mode;
        }
        break;
    }
    if (D_8006F594.vsync != 0) {
        EnterCriticalSection();
        VSyncCallback(D_8006F594.tick);
    } else {
        EnterCriticalSection();
        ResetRCnt(spec);
        SetRCnt(spec, target, 0x1000);
        if (D_8006F594.irq == 0) {
            D_8006F594.prev = InterruptCallback(D_8006F594.irq, NULL);
            InterruptCallback(D_8006F594.irq, func_8004EF10);
        } else if (D_8006F594.half == 0) {
            InterruptCallback(D_8006F594.irq, D_8006F594.tick);
        } else {
            InterruptCallback(D_8006F594.irq, func_8004EF5C);
        }
    }
    ExitCriticalSection();
}

extern long D_8005B85C;

extern void (*D_8005B850[2])(void);

void SsStart(void) {
    func_8004ECA0(1);
}

extern long D_8005B85C;

extern void (*D_8005B850[2])(void);

void SsStart2(void) {
    func_8004ECA0(0);
}

extern void (*D_8006F59C[])();

void func_8004EF10(void) {
    if (D_8006F59C[1] != NULL) {
        D_8006F59C[1]();
    }
    D_8006F59C[0]();
}

extern long D_8006F5A8;

extern void (*D_8006F59C[])();

void func_8004EF5C(void) {
    if (D_8006F5A8 == 0) {
        D_8006F5A8 = 1;
    } else {
        D_8006F5A8 = 0;
        D_8006F59C[0]();
    }
}

OBJECT_END(3);
