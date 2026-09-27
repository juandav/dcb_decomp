#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *memcpy(u_char *dst, u_char *src, int n) {
    u_char *ret = NULL;
    u_char *d;

    if (dst != NULL) {
        d = dst;
        while (n > 0) {
            *dst++ = *src++;
            n--;
        }
        ret = d;
    }
    return ret;
}

OBJECT_END(3);

void *memset(u_char *p, u_char c, int n) {
    u_char *s;

    if (p == NULL) {
        return NULL;
    }
    if (n <= 0) {
        return NULL;
    }
    s = p;
    while (n > 0) {
        *p++ = c;
        n--;
    }
    return s;
}

extern u_long D_801DDC10;

int rand(void) {
    D_801DDC10 = D_801DDC10 * 0x41C64E6D + 12345;
    return (D_801DDC10 >> 16) & 0x7FFF;
}

void srand(unsigned int seed) {
    D_801DDC10 = seed;
}

OBJECT_END(1);
