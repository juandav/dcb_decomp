#include "psyq.h"

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80012FBC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_init);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004AC20);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_FiDMA);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_Fr_);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_t);

void func_8004AC20(u_char *addr, u_long size);

u_long _spu_Fw(u_char *addr, u_long size) {
    if (D_8006EF40 == 0) {
        _spu_t(2, D_8006EF3C << D_8006EF4C);
        _spu_t(1);
        _spu_t(3, addr, size);
    } else {
        func_8004AC20(addr, size);
    }
    return size;
}

extern u_long *D_8006EF34;

u_long _spu_Fr(char *addr, u_long size) {
    _spu_t(2, D_8006EF3C << D_8006EF4C);
    _spu_t(0);
    _spu_t(3, addr, size);
    return size;
}

extern u_long *D_8006EF34;

void _spu_FsetRXX(int reg, u_long value, int mode) {
    if (mode == 0) {
        D_8006EF24[reg] = value;
    } else {
        D_8006EF24[reg] = value >> D_8006EF4C;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_FsetRXXa);

extern u_long *D_8006EF34;

u_long _spu_FgetRXXa(int reg, int mode) {
    u_short v = D_8006EF24[reg];

    if (mode == -1) {
        return v;
    }
    return v << D_8006EF4C;
}

extern u_long *D_8006EF34;

void _spu_FsetPCR(int flag) {
    *D_8006EF34 &= 0xFFF8FFFF;
    if (flag) {
        *D_8006EF34 |= 0x30000;
    } else {
        *D_8006EF34 |= 0x50000;
    }
}

extern u_long *D_8006EF34;

void func_8004B428(void) {
    *D_8006EF38 = (*D_8006EF38 & 0xF0FFFFFF) | 0x20000000;
}

extern u_long *D_8006EF34;

void func_8004B450(void) {
    *D_8006EF38 = (*D_8006EF38 & 0xF0FFFFFF) | 0x22000000;
}

void _spu_Fw1ts(void) {
    volatile int i;
    volatile int n = 13;

    for (i = 0; i < 60; i += 1) {
        n *= 13;
    }
}

OBJECT_END(3);
