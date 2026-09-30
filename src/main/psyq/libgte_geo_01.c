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

INCLUDE_ASM("main/nonmatchings/psyq", csqrt_1);

long csqrt_1(long a);
long func_8005FC54(long a);

int csqrt(int a) {
    long n;
    long s;
    long m;

    if (a == 0) {
        return 0;
    }
    n = 8 - func_8005FC54(a);
    if (n >= 0) {
        s = n >> 1;
        m = a >> (s * 2);
    } else {
        s = (n >> 1) + 1;
        m = a << -(s * 2);
    }
    s -= 6;
    if (s >= 0) {
        return csqrt_1(m) << s;
    }
    return csqrt_1(m) >> -s;
}

OBJECT_END(1);

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

INCLUDE_ASM("main/nonmatchings/psyq", func_8005B864);

INCLUDE_ASM("main/nonmatchings/psyq", InitGeom);
