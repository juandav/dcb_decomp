#include "psyq.h"
#include <stdarg.h>

extern u_short D_8006EF3C;

extern u_long *D_8006EF34;
extern long D_8006EF44;
extern long D_8006EF48;
extern long D_8006EF50;
extern long D_8006EF54;
extern void (*volatile D_8006EF5C)(void);
extern void (*volatile D_8006EF60)(void);
extern u_short D_8006EF64[];
extern volatile u_short D_801D8560[];
extern volatile u_long *D_8006EF28;
extern volatile u_long *D_8006EF2C;
extern volatile u_long *D_8006EF30;
extern long D_8006EF74;
void _spu_Fw1ts(void);
void func_8004AC20(u_char *addr, u_long size);
void func_8004B428(void);
void func_8004B450(void);

long _spu_init(long mode) {
    int i;
    u_int n;
    volatile u_short *p;

    *D_8006EF34 |= 0xB0000;
    D_8006EF40 = 0;
    D_8006EF44 = 0;
    D_8006EF3C = 0;
    D_8006EF24[0xC0] = 0;
    D_8006EF24[0xC1] = 0;
    D_8006EF24[0xD5] = 0;
    _spu_Fw1ts();
    D_8006EF24[0xC0] = 0;
    D_8006EF24[0xC1] = 0;
    n = 0;
    while (D_8006EF24[0xD7] & 0x7FF) {
        if (++n > 0xF00) {
            printf("SPU:T/O [%s]\n", "wait (reset)");
            break;
        }
    }
    D_8006EF48 = 2;
    D_8006EF4C = 3;
    D_8006EF50 = 8;
    D_8006EF54 = 7;
    D_8006EF24[0xD6] = 4;
    D_8006EF24[0xC2] = 0;
    D_8006EF24[0xC3] = 0;
    D_8006EF24[0xC6] = 0xFFFF;
    D_8006EF24[0xC7] = 0xFFFF;
    D_8006EF24[0xCC] = 0;
    D_8006EF24[0xCD] = 0;
    for (i = 0, p = D_801D8560; i < 10; i++) {
        *p++ = 0;
    }
    if (mode == 0) {
        D_8006EF3C = 0x200;
        D_8006EF24[0xC8] = 0;
        D_8006EF24[0xC9] = 0;
        D_8006EF24[0xCA] = 0;
        D_8006EF24[0xCB] = 0;
        D_8006EF24[0xD8] = 0;
        D_8006EF24[0xD9] = 0;
        D_8006EF24[0xDA] = 0;
        D_8006EF24[0xDB] = 0;
        func_8004AC20((u_char *)D_8006EF64, 0x10);
        for (i = 0; i < 24; i++) {
            D_8006EF24[i * 8 + 0] = 0;
            D_8006EF24[i * 8 + 1] = 0;
            D_8006EF24[i * 8 + 2] = 0x3FFF;
            D_8006EF24[i * 8 + 3] = 0x200;
            D_8006EF24[i * 8 + 4] = 0;
            D_8006EF24[i * 8 + 5] = 0;
        }
        D_8006EF24[0xC4] = 0xFFFF;
        D_8006EF24[0xC5] = 0xFF;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        D_8006EF24[0xC6] = 0xFFFF;
        D_8006EF24[0xC7] = 0xFF;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
    }
    D_8006EF58 = 1;
    D_8006EF24[0xD5] = 0xC000;
    D_8006EF5C = NULL;
    D_8006EF60 = NULL;
    return 0;
}

void func_8004AC20(u_char *addr, u_long size) {
    u_short *p = (u_short *)addr;
    u_short stat;
    u_short cnt;
    int n;
    int i;
    u_int wait;

    stat = D_8006EF24[0xD7] & 0x7FF;
    D_8006EF24[0xD3] = D_8006EF3C;
    _spu_Fw1ts();
    while (size != 0) {
        n = size > 0x40 ? 0x40 : size;
        for (i = 0; i < n; i += 2) {
            D_8006EF24[0xD4] = *p++;
        }
        cnt = D_8006EF24[0xD5];
        cnt &= ~0x30;
        cnt |= 0x10;
        D_8006EF24[0xD5] = cnt;
        _spu_Fw1ts();
        wait = 0;
        while (D_8006EF24[0xD7] & 0x400) {
            if (++wait > 0xF00) {
                printf("SPU:T/O [%s]\n", "wait (wrdy H -> L)");
                break;
            }
        }
        _spu_Fw1ts();
        _spu_Fw1ts();
        size -= n;
    }
    cnt = D_8006EF24[0xD5];
    cnt &= ~0x30;
    D_8006EF24[0xD5] = cnt;
    wait = 0;
    while ((D_8006EF24[0xD7] & 0x7FF) != stat) {
        if (++wait > 0xF00) {
            printf("SPU:T/O [%s]\n", "wait (dmaf clear/W)");
            break;
        }
    }
}

void _spu_FiDMA(void) {
    u_int i;

    if (D_8006EF74 == 0) {
        _spu_Fw1ts();
    }
    D_8006EF24[0xD5] &= ~0x30;
    i = 0;
    while (D_8006EF24[0xD5] & 0x30) {
        if (++i > 0xF00) {
            break;
        }
    }
    if (D_8006EF5C) {
        D_8006EF5C();
    } else {
        DeliverEvent(0xF0000009, 0x20);
    }
}


void _spu_Fr_(u_char *addr, u_short spuAddr, u_long size) {
    D_8006EF24[0xD3] = spuAddr;
    _spu_Fw1ts();
    D_8006EF24[0xD5] |= 0x30;
    _spu_Fw1ts();
    func_8004B450();
    *D_8006EF28 = (u_long)addr;
    *D_8006EF2C = (size << 16) | 0x10;
    D_8006EF74 = 1;
    *D_8006EF30 = 0x1000200;
}

extern long D_8006EF78;
extern long D_8006EF7C;

long _spu_t(long mode, ...) {
    va_list args;
    u_int i;
    u_short ck;
    u_long count;
    u_long dma;

    va_start(args, mode);
    switch (mode) {
    case 2:
        count = va_arg(args, u_long);
        D_8006EF3C = count >> D_8006EF4C;
        D_8006EF24[0xD3] = D_8006EF3C;
        break;
    case 1:
        D_8006EF74 = 0;
        i = 0;
        while (D_8006EF24[0xD3] != D_8006EF3C) {
            if (++i > 0xF00) {
                return -2;
            }
        }
        D_8006EF24[0xD5] = (D_8006EF24[0xD5] & ~0x30) | 0x20;
        break;
    case 0:
        D_8006EF74 = 1;
        i = 0;
        while (D_8006EF24[0xD3] != D_8006EF3C) {
            if (++i > 0xF00) {
                return -2;
            }
        }
        D_8006EF24[0xD5] = (D_8006EF24[0xD5] & ~0x30) | 0x30;
        break;
    case 3:
        if (D_8006EF74 == 1) {
            ck = 0x30;
        } else {
            ck = 0x20;
        }
        i = 0;
        while ((D_8006EF24[0xD5] & 0x30) != ck) {
            if (++i > 0xF00) {
                return -2;
            }
        }
        if (D_8006EF74 == 1) {
            func_8004B450();
        } else {
            func_8004B428();
        }
        count = va_arg(args, u_long);
        D_8006EF78 = count;
        count = va_arg(args, u_long);
        D_8006EF7C = count / 64;
        D_8006EF7C += (count % 64) ? 1 : 0;
        *D_8006EF28 = D_8006EF78;
        *D_8006EF2C = (D_8006EF7C << 16) | 0x10;
        if (D_8006EF74 == 1) {
            dma = 0x1000200;
        } else {
            dma = 0x1000201;
        }
        *D_8006EF30 = dma;
        break;
    }
    return 0;
}

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

long _spu_FsetRXXa(long reg, u_long addr) {
    long r = reg;
    u_short v;

    if (D_8006EF48 != 0 && addr % D_8006EF50 != 0) {
        addr += D_8006EF50;
        addr &= ~D_8006EF54;
    }
    v = addr >> D_8006EF4C;
    switch (r) {
    case -1:
        return v & 0xFFFF;
    case -2:
        return addr;
    default:
        D_8006EF24[r] = v;
        return addr;
    }
}

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

/* the object's string table was padded to 8 bytes */
__asm__(".section .rodata\n\t.space 8\n");

OBJECT_END(3);
