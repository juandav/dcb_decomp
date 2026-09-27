#include "psyq.h"

extern volatile u_short D_8006EF3C;

u_long SpuSetTransferStartAddr(u_long addr) {
    if (addr - 0x1010 > 0x7EFE8) {
        return 0;
    }
    D_8006EF3C = _spu_FsetRXXa(-1, addr);
    return D_8006EF3C << D_8006EF4C;
}

OBJECT_END(1);
