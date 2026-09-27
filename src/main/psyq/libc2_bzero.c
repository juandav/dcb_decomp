#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *bzero(unsigned char *p, int n) {
    unsigned char *s;

    if (p == NULL) {
        return NULL;
    }
    if (n <= 0) {
        return NULL;
    }
    s = p;
    while (n > 0) {
        *p++ = 0;
        n--;
    }
    return s;
}
