#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80070AE8;

void *startIntrVSync(void) {
    *D_80070AEC = 0x100;
    D_80070AE8 = 0;
    func_80056DA4((long *)D_80070AC8, 8);
    InterruptCallback(0, func_80056D0C);
    return func_80056D78;
}

void func_80056D0C(void) {
    int i;

    D_80070AE8++;
    for (i = 0; i < 8; i++) {
        if (D_80070AC8[i] != NULL) {
            D_80070AC8[i]();
        }
    }
}

void *func_80056D78(int index, void (*func)()) {
    void (*old)() = D_80070AC8[index];

    if (func != old) {
        D_80070AC8[index] = func;
    }
    return old;
}

void func_80056DA4(long *p, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = 0;
        } while (i-- != 0);
    }
}

OBJECT_END(3);
