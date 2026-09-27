#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80077848;
extern long D_8007784C;
extern char D_801DDC20[];
extern char D_80077879[];

void _putchar(char c) {
    switch (c) {
    case '\n':
        _putchar('\r');
        D_80077848 = 0;
        break;
    case '\t':
        do {
            _putchar(' ');
        } while (D_80077848 & 7);
        return;
    default:
        if (D_80077879[(u_char)c] & 0x97) {
            D_80077848++;
        }
        break;
    }
    if (D_8007784C >= 0x20) {
        func_8006A854(1, D_801DDC20, D_8007784C);
        D_8007784C = 0;
    }
    D_801DDC20[D_8007784C++] = c;
}

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
