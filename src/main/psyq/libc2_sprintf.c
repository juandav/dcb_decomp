#include "psyq.h"

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013D5C);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013D70);

INCLUDE_ASM("asm/main/nonmatchings/psyq", sprintf);

void *memmove(u_char *dst, u_char *src, int n) {
    u_char *d = dst;

    if (d >= src) {
        while (n-- > 0) {
            d[n] = src[n];
        }
    } else {
        while (n-- > 0) {
            *d++ = *src++;
        }
    }
    return d;
}

OBJECT_END(1);
