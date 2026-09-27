#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetRCnt);

long GetRCnt(unsigned long spec) {
    int c = spec & 0xFFFF;

    if (c >= 3) {
        return 0;
    }
    return D_8007790C[c * 8];
}

long StartRCnt(u_long spec) {
    int timer = spec & 0xFFFF;

    D_80077908[1] |= D_80077910[timer];
    return timer < 3;
}

long StopRCnt(u_long spec) {
    D_80077908[1] &= ~D_80077910[spec & 0xFFFF];
    return 1;
}

long ResetRCnt(unsigned long spec) {
    int i = spec & 0xFFFF;

    if (i >= 3) {
        return 0;
    }
    D_8007790C[i * 8] = 0;
    return 1;
}

OBJECT_END(1);
