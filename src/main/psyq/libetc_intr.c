#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

int ResetCallback(void) {
    return D_80070AA8->resetCallback();
}

void *InterruptCallback(int irq, void (*func)()) {
    return D_80070AA8->interruptCallback(irq, func);
}

void *DMACallback(int dma, void (*func)()) {
    return D_80070AA8->dmaCallback(dma, func);
}

int VSyncCallback(void (*func)()) {
    return (int)D_80070AA8->vsyncCallbacks(4, func);
}

void *VSyncCallbacks(int ch, void (*func)()) {
    return D_80070AA8->vsyncCallbacks(ch, func);
}

int StopCallback(void) {
    return D_80070AA8->stopCallback();
}

int RestartCallback(void) {
    return D_80070AA8->restartCallback();
}

int CheckCallback(void) {
    return D_8006FA22;
}

u_short GetIntrMask(void) {
    return *D_80070AB0;
}

u_short SetIntrMask(u_short mask) {
    u_short old = *D_80070AB0;

    *D_80070AB0 = mask;
    return old;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056788);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056860);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056A30);

typedef struct {
    /* 0x00 */ u_short inited;
    /* 0x02 */ u_short unk2;
    /* 0x04 */ u8 unk4[0x2E];
    /* 0x32 */ u_short mask;
    /* 0x34 */ u_long dpcr;
    /* 0x38 */ u_long buf[1];
} IntrEnv;
extern IntrEnv D_8006FA20;
extern volatile u_long *D_80070AB4;
extern volatile u_short *D_80070AAC;
void func_8006A804(void);
void func_8006A7E4(void);

void *func_80056B78(void) {
    if (D_8006FA20.inited == 0) {
        return NULL;
    }
    func_8006A804();
    D_8006FA20.mask = *D_80070AB0;
    D_8006FA20.dpcr = *D_80070AB4;
    *D_80070AAC = *D_80070AB0 = 0;
    *D_80070AB4 &= 0x77777777;
    func_8006A7E4();
    D_8006FA20.inited = 0;
    return &D_8006FA20;
}

int func_8006A7F4(u_long *);
void func_8006A814(void);

void *func_80056C18(void) {
    if (D_8006FA20.inited != 0) {
        return NULL;
    }
    func_8006A7F4(D_8006FA20.buf);
    D_8006FA20.inited = 1;
    *D_80070AB0 = D_8006FA20.mask;
    *D_80070AB4 = D_8006FA20.dpcr;
    func_8006A814();
    return &D_8006FA20;
}

void func_80056C90(long *p, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = 0;
        } while (i-- != 0);
    }
}
