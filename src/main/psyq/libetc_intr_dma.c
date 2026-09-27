#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *startIntrDMA(void) {
    func_8005704C((long *)D_80070AFC, 8);
    *D_80070AF8 = 0;
    InterruptCallback(3, func_80056E20);
    return func_80056FA0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056E20);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80056FA0);

void func_8005704C(long *p, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = 0;
        } while (i-- != 0);
    }
}

OBJECT_END(1);
