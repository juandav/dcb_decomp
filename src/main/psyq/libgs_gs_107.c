#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetFlatLight);

extern MATRIX D_801DBE60;

void func_8006295C(MATRIX *m) {
    D_801DBE60 = *m;
    SetColorMatrix(m);
}

extern MATRIX D_801DBE60;

void func_800629C0(MATRIX *m) {
    *m = D_801DBE60;
}

OBJECT_END(2);
