#include "psyq.h"

/* Digit tables for %X and %x/%p. */
const char D_80013D5C[] = "0123456789ABCDEF";
const char D_80013D70[] = "0123456789abcdef";

INCLUDE_ASM("main/nonmatchings/psyq", sprintf);

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
