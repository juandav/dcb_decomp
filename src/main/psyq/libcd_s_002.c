#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80070C40;

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdIntToPos);

extern long D_80070C48, D_80070C44, D_80070C40;

long func_8005A344(long v) {
    long old = D_80070C48;

    D_80070C48 = v;
    return old;
}

OBJECT_END(3);

void func_8005A364(void) {
    CD_sync();
}

void func_8005A384(void) {
    CD_ready();
}

long func_8005A3A4(long v) {
    long old = D_80070C44;

    D_80070C44 = v;
    return old;
}

OBJECT_END(3);
