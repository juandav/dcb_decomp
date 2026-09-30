#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80070C40;
extern void (*D_80070B48)();
extern long D_80070B4C;
int func_80057164(void);
void func_800571A0(void);
void func_800571C8(void);
void func_800571F0(void);

int CdInit(void) {
    int i;

    i = 4;
    while (func_80057164() != 1) {
        if (--i == -1) {
            printf("CdInit: Init failed\n");
            return 0;
        }
    }
    D_80070C40 = (long)func_800571A0;
    D_80070C44 = (long)func_800571C8;
    D_80070B48 = func_800571F0;
    D_80070B4C = 0;
    return 1;
}

__asm__(".section .rodata\n\t.space 8\n\t.section .text\n");

int func_80057164(void) {
    if (CD_init() != 0) {
        return 0;
    }
    return CD_initvol() == 0;
}

void func_800571A0(void) {
    DeliverEvent(0xF0000003, 0x20);
}

void func_800571C8(void) {
    DeliverEvent(0xF0000003, 0x40);
}

void func_800571F0(void) {
    DeliverEvent(0xF0000003, 0x40);
}

OBJECT_END(3);
