#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SpuInit);

extern long D_8006EFF4;

extern long D_8006EF8C;

void SpuStart(void) {
    long event;

    if (D_8006EFF4 == 0) {
        D_8006EFF4 = 1;
        func_8006A804();
        _SpuDataCallback(_spu_FiDMA);
        event = func_8006A794(0xF0000009, 0x20, 0x2000, NULL);
        D_8006EF8C = event;
        func_8006A7C4(event);
        func_8006A814();
    }
}

OBJECT_END(2);
