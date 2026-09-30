#include "psyq.h"

__asm__(".section .rodata\n\t.asciz \"$Id: intr.c,v 1.75 1997/02/07 09:00:36 makoto Exp $\"\n\t.align 2\n\t.section .text\n");

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

typedef struct {
    /* 0x00 */ u_short inited;
    /* 0x02 */ u_short unk2;
    /* 0x04 */ void (*handlers[11])();
    /* 0x30 */ u_short enabled;
    /* 0x32 */ u_short mask;
    /* 0x34 */ u_long dpcr;
    /* 0x38 */ u_long buf[12];
    /* 0x68 */ u_long stack[1024];
} IntrEnv;
extern IntrEnv D_8006FA20;

void func_80056860(void);
void func_80056C90(long *p, int n);
int setjmp(u_long *buf);
int HookEntryInt(u_long *);
void *startIntrVSync(void);
void *startIntrDMA(void);
void func_8006A76C();
void ExitCriticalSection(void);
extern volatile u_long *D_80070AB4;
extern volatile u_short *D_80070AAC;

void *func_80056788(void) {
    if (D_8006FA20.inited != 0) {
        return NULL;
    }
    *D_80070AAC = *D_80070AB0 = 0;
    *D_80070AB4 = 0x33333333;
    func_80056C90((long *)&D_8006FA20, sizeof(IntrEnv) / 4);
    if (setjmp(D_8006FA20.buf)) {
        func_80056860();
    }
    D_8006FA20.buf[1] = (u_long)&D_8006FA20.stack[1004];
    HookEntryInt(D_8006FA20.buf);
    D_8006FA20.inited = 1;
    D_80070AA8->vsyncCallbacks = startIntrVSync();
    D_80070AA8->dmaCallback = startIntrDMA();
    func_8006A76C(D_80070AA8);
    ExitCriticalSection();
    return &D_8006FA20;
}

extern long D_80070AB8;

void func_80056860(void) {
    int i;
    u_short mask;
    short pending;

    if (D_8006FA20.inited == 0) {
        printf("unexpected interrupt(%04x)\n", *D_80070AAC);
        ReturnFromException();
    }
    D_8006FA20.unk2 = 1;
    while ((mask = D_8006FA20.enabled & *D_80070AAC & *D_80070AB0) != 0) {
        for (i = 0; mask != 0 && i < 11; i++, mask >>= 1) {
            if (mask & 1) {
                *D_80070AAC = ~(1 << i);
                if (D_8006FA20.handlers[i] != NULL) {
                    D_8006FA20.handlers[i]();
                }
            }
        }
    }
    pending = *D_80070AAC & *D_80070AB0;
    if (pending) {
        if (D_80070AB8++ > 0x800) {
            printf("intr timeout(%04x:%04x)\n", *D_80070AAC, *D_80070AB0);
            D_80070AB8 = 0;
            *D_80070AAC = 0;
        }
    } else {
        D_80070AB8 = 0;
    }
    D_8006FA20.unk2 = 0;
    ReturnFromException();
}

__asm__(".section .rodata\n\t.space 4\n\t.section .text\n");

void ChangeClearPad(int);
void ChangeClearRCnt(int, int);

void *func_80056A30(int irq, void (*func)()) {
    void (*old)() = D_8006FA20.handlers[irq];
    int mask;

    if (func != old && D_8006FA20.inited) {
        mask = *D_80070AB0;
        *D_80070AB0 = 0;
        if (func != NULL) {
            D_8006FA20.handlers[irq] = func;
            mask |= 1 << irq;
            D_8006FA20.enabled |= 1 << irq;
        } else {
            D_8006FA20.handlers[irq] = NULL;
            mask &= ~(1 << irq);
            D_8006FA20.enabled &= ~(1 << irq);
        }
        if (irq == 0) {
            ChangeClearPad(func == NULL);
            ChangeClearRCnt(3, func == NULL);
        }
        if (irq == 4) {
            ChangeClearRCnt(0, func == NULL);
        }
        if (irq == 5) {
            ChangeClearRCnt(1, func == NULL);
        }
        if (irq == 6) {
            ChangeClearRCnt(2, func == NULL);
        }
        *D_80070AB0 = mask;
    }
    return old;
}

void ResetEntryInt(void);

void *func_80056B78(void) {
    if (D_8006FA20.inited == 0) {
        return NULL;
    }
    EnterCriticalSection();
    D_8006FA20.mask = *D_80070AB0;
    D_8006FA20.dpcr = *D_80070AB4;
    *D_80070AAC = *D_80070AB0 = 0;
    *D_80070AB4 &= 0x77777777;
    ResetEntryInt();
    D_8006FA20.inited = 0;
    return &D_8006FA20;
}

void *func_80056C18(void) {
    if (D_8006FA20.inited != 0) {
        return NULL;
    }
    HookEntryInt(D_8006FA20.buf);
    D_8006FA20.inited = 1;
    *D_80070AB0 = D_8006FA20.mask;
    *D_80070AB4 = D_8006FA20.dpcr;
    ExitCriticalSection();
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
