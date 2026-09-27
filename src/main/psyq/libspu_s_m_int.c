#include "psyq.h"

typedef struct SpuMallocBlock {
    u_long addr;
    u_long size;
} SpuMallocBlock;

extern long D_8006F008;
extern SpuMallocBlock *D_8006F00C;

void _spu_gcSPU(void) {
    long i, j;

    for (i = 0; i <= D_8006F008;) {
        if (D_8006F00C[i].addr & 0x80000000) {
            for (j = i + 1; 1; j++) {
                if (D_8006F00C[j].addr != 0x2FFFFFFF) {
                    break;
                }
            }
            if ((D_8006F00C[j].addr & 0x80000000) &&
                ((D_8006F00C[j].addr & 0x0FFFFFFF) ==
                 ((D_8006F00C[i].addr & 0x0FFFFFFF) +
                  D_8006F00C[i].size))) {
                D_8006F00C[j].addr = 0x2FFFFFFF;
                D_8006F00C[i].size += D_8006F00C[j].size;
                continue;
            }
        }
        i++;
    }
    for (i = 0; i <= D_8006F008; i++) {
        if (D_8006F00C[i].size == 0) {
            D_8006F00C[i].addr = 0x2FFFFFFF;
        }
    }
    for (i = 0; i <= D_8006F008; i++) {
        if (D_8006F00C[i].addr & 0x40000000) {
            break;
        }
        for (j = i + 1; j <= D_8006F008; j++) {
            if (D_8006F00C[j].addr & 0x40000000) {
                break;
            }
            if ((D_8006F00C[j].addr & 0x0FFFFFFF) <
                (D_8006F00C[i].addr & 0x0FFFFFFF)) {
                u_long swapAddr = D_8006F00C[i].addr;
                u_long swapSize = D_8006F00C[i].size;
                D_8006F00C[i].addr = D_8006F00C[j].addr;
                D_8006F00C[i].size = D_8006F00C[j].size;
                D_8006F00C[j].addr = swapAddr;
                D_8006F00C[j].size = swapSize;
            }
        }
    }
    for (i = 0; i <= D_8006F008; i++) {
        if (D_8006F00C[i].addr & 0x40000000) {
            break;
        }
        if (D_8006F00C[i].addr == 0x2FFFFFFF) {
            D_8006F00C[i].addr = D_8006F00C[D_8006F008].addr;
            D_8006F00C[i].size = D_8006F00C[D_8006F008].size;
            D_8006F008 = i;
            break;
        }
    }
    for (i = D_8006F008 - 1; i >= 0; i--) {
        if (!(D_8006F00C[i].addr & 0x80000000)) {
            break;
        }
        D_8006F00C[i].addr &= 0x0FFFFFFF;
        D_8006F00C[i].addr |= 0x40000000;
        D_8006F00C[i].size += D_8006F00C[D_8006F008].size;
        D_8006F008 = i;
    }
}
