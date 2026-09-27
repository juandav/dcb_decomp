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

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056B78);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056C18);

void func_80056C90(long *p, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = 0;
        } while (i-- != 0);
    }
}
