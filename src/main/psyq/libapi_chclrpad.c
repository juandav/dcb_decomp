#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("main/nonmatchings/psyq", _remove_ChgclrPAD);

extern long D_801DDCA4;

long func_8006B0B4(long v) {
    long *p = &D_801DDCA4;
    long old = *p;

    *p = v;
    return old;
}

OBJECT_END(3);

int func_8006B0D4(void (*func)()) {
    return DMACallback(3, func);
}

OBJECT_END(3);
