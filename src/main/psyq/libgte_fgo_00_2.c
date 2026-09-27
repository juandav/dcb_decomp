#include "psyq.h"

extern short D_80075ED8[];

long ratan2(long y, long x) {
    long ret;
    long neg_x;
    long neg_y;
    int i;

    neg_x = 0;
    neg_y = 0;
    if (x < 0) {
        neg_x = 1;
        x = -x;
    }
    if (y < 0) {
        neg_y = 1;
        y = -y;
    }
    if (x == 0 && y == 0) {
        return 0;
    }
    if (y < x) {
        if (y & 0x7FE00000) {
            i = y / (x >> 10);
        } else {
            i = (y << 10) / x;
        }
        ret = D_80075ED8[i];
    } else {
        if (x & 0x7FE00000) {
            i = x / (y >> 10);
        } else {
            i = (x << 10) / y;
        }
        ret = 0x400 - D_80075ED8[i];
    }
    if (neg_x) {
        ret = 0x800 - ret;
    }
    if (neg_y) {
        ret = -ret;
    }
    return ret;
}

OBJECT_END(3);
