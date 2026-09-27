#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", Gssub_make_matrix);

extern long D_801DBF98;

void func_80062C34(long v) {
    D_801DBF98 = v;
}

OBJECT_END(1);

extern long D_801DBF98;

long func_80062C44(void) {
    return D_801DBF98;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetRefView2);
