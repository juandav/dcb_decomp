#include "psyq.h"

extern long D_8006F534[];
extern volatile long D_8006EF5C;
extern long D_8006EF8C;
extern u_char D_8006F014[];
long _SpuIsInAllocateArea_(u_long addr);

long SpuClearReverbWorkArea(long rev_mode) {
    volatile long callback = 0;
    long restore = 0;
    u_long size;
    u_long addr;
    long mode;
    long more;
    u_long chunk;

    if ((u_long)rev_mode >= 10 || _SpuIsInAllocateArea_(D_8006F534[rev_mode])) {
        return -1;
    }
    if (rev_mode == 0) {
        size = 0x10 << D_8006EF4C;
        addr = 0xFFF0 << D_8006EF4C;
    } else {
        size = (0x10000 - D_8006F534[rev_mode]) << D_8006EF4C;
        addr = D_8006F534[rev_mode] << D_8006EF4C;
    }
    mode = D_8006EF40;
    if (mode == 1) {
        D_8006EF40 = 0;
        restore = 1;
    }
    more = 1;
    if (D_8006EF5C != 0) {
        callback = D_8006EF5C;
        D_8006EF5C = 0;
    }
    do {
        chunk = 0x400;
        if (size <= 0x400) {
            chunk = size;
            more = 0;
        }
        _spu_t(2, addr);
        _spu_t(1);
        _spu_t(3, D_8006F014, chunk);
        WaitEvent(D_8006EF8C);
        size -= 0x400;
        addr += 0x400;
    } while (more);
    if (restore) {
        D_8006EF40 = mode;
    }
    if (callback) {
        D_8006EF5C = callback;
    }
    return 0;
}

OBJECT_END(3);
