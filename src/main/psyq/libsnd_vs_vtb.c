#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsVabTransBody);

extern long D_8006EF5C;

u_long SpuRead(u_char *addr, u_long size) {
    if (size > 0x7EFF0) {
        size = 0x7EFF0;
    }
    _spu_Fw(addr, size);
    if (D_8006EF5C == 0) {
        D_8006EF58 = 0;
    }
    return size;
}

OBJECT_END(1);
