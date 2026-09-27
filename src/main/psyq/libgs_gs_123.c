#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern MATRIX D_801DBEC0;

void Gssub_make_matrix(MATRIX *m, short s, short c, char axis) {
    *m = D_801DBEC0;
    switch (axis) {
    case 'x':
    case 'X':
        m->m[1][1] = c;
        m->m[2][2] = c;
        m->m[1][2] = -s;
        m->m[2][1] = s;
        break;
    case 'y':
    case 'Y':
        m->m[0][0] = c;
        m->m[2][2] = c;
        m->m[0][2] = s;
        m->m[2][0] = -s;
        break;
    case 'z':
    case 'Z':
        m->m[0][0] = c;
        m->m[1][1] = c;
        m->m[0][1] = -s;
        m->m[1][0] = s;
        break;
    }
}

/* ASPSX padded the jump table of the object as well */
__asm__(".section .rodata\n\t.space 4\n\t.section .text\n");

OBJECT_END(2);

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
