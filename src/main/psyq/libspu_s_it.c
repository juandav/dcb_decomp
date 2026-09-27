#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void _spu_setInTransfer(int mode) {
    if (mode == 1) {
        D_8006EF58 = 0;
    } else {
        D_8006EF58 = 1;
    }
}

int _spu_getInTransfer(void) {
    return D_8006EF58 != 1;
}
