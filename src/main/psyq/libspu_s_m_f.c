#include "psyq.h"

typedef struct SpuMallocRec {
    /* 0x0 */ u_long addr;
    /* 0x4 */ u_long size;
} SpuMallocRec;

extern SpuMallocRec *D_8006F00C;
extern long D_8006F004;

void SpuFree(unsigned long addr) {
    int i;

    for (i = 0; i < D_8006F004; i++) {
        if (D_8006F00C[i].addr & 0x40000000) {
            break;
        }
        if (D_8006F00C[i].addr == addr) {
            D_8006F00C[i].addr = addr | 0x80000000;
            break;
        }
    }
    _spu_gcSPU();
}

OBJECT_END(1);
