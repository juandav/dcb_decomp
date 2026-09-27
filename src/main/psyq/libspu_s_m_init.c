#include "psyq.h"

extern char *D_8006F00C;

extern long D_8006F008;

extern long D_8006F004;

long SpuInitMalloc(long num, char *top) {
    long mode;

    if (num > 0) {
        mode = D_8006EF4C;
        ((long *)top)[0] = 0x40001010;
        D_8006F00C = top;
        D_8006F008 = 0;
        D_8006F004 = num;
        ((long *)top)[1] = (0x10000 << mode) - 0x1010;
        return num;
    }
    return 0;
}

OBJECT_END(3);
