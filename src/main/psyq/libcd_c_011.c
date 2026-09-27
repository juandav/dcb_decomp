#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", StCdInterrupt);

void func_80058A10(long *dst, long *src, u_long n) {
    u_long i = 0;

    if (n != 0) {
        do {
            *dst++ = *src++;
            i++;
        } while (i < n);
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80058A3C);
