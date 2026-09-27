#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80070C40;

extern long D_80070C48, D_80070C44, D_80070C40;

extern u_char D_80070C58[];

u_char *func_8005B194(void) {
    return D_80070C58;
}

OBJECT_END(1);

void func_8005B1A4(void) {
    CD_flush();
}

long func_8005B1C4(long v) {
    long old = D_80070C40;

    D_80070C40 = v;
    return old;
}

OBJECT_END(3);
