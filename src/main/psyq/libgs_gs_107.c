#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetFlatLight);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006295C);

extern MATRIX D_801DBE60;

void func_800629C0(MATRIX *m) {
    *m = D_801DBE60;
}

OBJECT_END(2);
