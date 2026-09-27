#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", setjmp);

INCLUDE_ASM("asm/main/nonmatchings/psyq", longjmp);

extern u_char D_80077879[];

char toupper(char c) {
    if (D_80077879[(u_char)c] & 2) {
        c -= 0x20;
    }
    return c;
}

OBJECT_END(3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A734);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A744);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A754);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A76C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A784);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A794);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7B4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7C4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7E4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006A7F4);
