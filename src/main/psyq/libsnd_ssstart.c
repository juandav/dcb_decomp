#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004ECA0);

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
