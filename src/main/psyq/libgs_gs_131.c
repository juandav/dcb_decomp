#include "psyq.h"

void func_80063024(long *src, long *dst) {
    int n = func_800631D8(func_80063110(src));

    if (n >= 16) {
        n -= 15;
        dst[0] = src[0] >> n;
        dst[1] = src[1] >> n;
        dst[2] = src[2] >> n;
        dst[3] = src[3] >> n;
        dst[4] = src[4] >> n;
        dst[5] = src[5] >> n;
    } else {
        dst[0] = src[0];
        dst[1] = src[1];
        dst[2] = src[2];
        dst[3] = src[3];
        dst[4] = src[4];
        dst[5] = src[5];
    }
}

long func_80063110(long *v) {
    long max;
    long t;

    max = (v[0] >= 0) ? v[0] : -v[0];
    t = (v[1] >= 0) ? v[1] : -v[1];
    if (max < t) max = t;
    t = (v[2] >= 0) ? v[2] : -v[2];
    if (max < t) max = t;
    t = (v[3] >= 0) ? v[3] : -v[3];
    if (max < t) max = t;
    t = (v[4] >= 0) ? v[4] : -v[4];
    if (max < t) max = t;
    t = (v[5] >= 0) ? v[5] : -v[5];
    if (max < t) max = t;
    return max;
}

long func_800631D8(long value) {
    long bits = 0;

    while (value > 0) {
        value >>= 1;
        bits++;
    }
    return bits;
}
