#include "psyq.h"

/* rsin_tbl and the labels splat put inside it */
extern short D_80071058[];
extern short D_80070858[];
extern short D_8006F858[];

int rcos(int a) {
    if (a < 0) {
        a = -a;
    }
    a &= 0xFFF;
    if (a <= 0x800) {
        if (a <= 0x400) {
            return D_80071058[0x400 - a];
        }
        return -D_80070858[a];
    }
    if (a <= 0xC00) {
        return -D_80071058[0xC00 - a];
    }
    return D_8006F858[a];
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", csqrt_1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", csqrt);

extern long D_80071868[];

int catan(int a) {
    long x[14], y[14], z[14];
    int i;

    x[0] = 0x1000;
    y[0] = a;
    z[0] = 0;
    for (i = 0; i < 12; i++) {
        if (y[i] < 0) {
            x[i + 1] = x[i] - (y[i] >> i);
            y[i + 1] = y[i] + (x[i] >> i);
            z[i + 1] = z[i] - D_80071868[i];
        } else {
            x[i + 1] = x[i] + (y[i] >> i);
            y[i + 1] = y[i] - (x[i] >> i);
            z[i + 1] = z[i] + D_80071868[i];
        }
    }
    return z[12];
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005B864);

INCLUDE_ASM("asm/main/nonmatchings/psyq", InitGeom);
