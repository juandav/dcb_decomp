#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80070B28;

long SetVideoMode(long value) {
    long old = D_80070B28;

    D_80070B28 = value;
    return old;
}

long GetVideoMode(void) {
    return D_80070B28;
}

OBJECT_END(3);
