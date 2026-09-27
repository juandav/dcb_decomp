#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _putchar);

extern long D_8007784C;

extern char D_801DDC20[];

void _putchar_flash(void) {
    if (D_8007784C > 0) {
        func_8006A854(1, D_801DDC20, D_8007784C);
        D_8007784C = 0;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", putchar);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013D5C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013D70);

INCLUDE_ASM("asm/main/nonmatchings/psyq", sprintf);

INCLUDE_ASM("asm/main/nonmatchings/psyq", memmove);
