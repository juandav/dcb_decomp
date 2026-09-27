#include "psyq.h"

extern u_char D_801D9700[];
extern u_long D_801D9718[];
extern u_long D_801D9760[];
void _spu_setInTransfer(int mode);

short SsVabTransBody(u_char *addr, short vabid) {
    u_long spuAddr;

    if ((u_short)vabid <= 16 && D_801D9700[vabid] == 2) {
        spuAddr = D_801D9760[vabid];
        SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
        if (SpuSetTransferStartAddr(spuAddr)) {
            SpuRead(addr, D_801D9718[vabid]);
            D_801D9700[vabid] = 1;
            return vabid;
        }
    }
    _spu_setInTransfer(0);
    return -1;
}

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
