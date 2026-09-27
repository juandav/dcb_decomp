#include "psyq.h"

typedef struct SpuMallocBlock {
    u_long addr;
    u_long size;
} SpuMallocBlock;
extern SpuMallocBlock *D_8006F00C;

long _SpuIsInAllocateArea(u_long addr) {
    u_long start;
    int i;
    SpuMallocBlock *list;

    list = D_8006F00C;
    if (list == NULL) {
        return 0;
    }
    for (i = 0;; i++) {
        if (list[i].addr & 0x80000000) {
            continue;
        }
        if (list[i].addr & 0x40000000) {
            break;
        }
        start = list[i].addr & 0x0FFFFFFF;
        if (start >= addr) {
            return 1;
        }
        if (addr < start + list[i].size) {
            return 1;
        }
    }
    return 0;
}

long _SpuIsInAllocateArea_(u_long addr) {
    u_long start;
    int i;
    SpuMallocBlock *list;

    addr <<= D_8006EF4C;
    list = D_8006F00C;
    if (list == NULL) {
        return 0;
    }
    for (i = 0;; i++) {
        if (list[i].addr & 0x80000000) {
            continue;
        }
        if (list[i].addr & 0x40000000) {
            break;
        }
        start = list[i].addr & 0x0FFFFFFF;
        if (start >= addr) {
            return 1;
        }
        if (addr < start + list[i].size) {
            return 1;
        }
    }
    return 0;
}

OBJECT_END(1);
