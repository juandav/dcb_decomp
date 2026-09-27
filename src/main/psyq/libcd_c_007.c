#include "psyq.h"

extern u_char *D_801D98F0;

u_long StFreeRing(u_long *base) {
    int i;
    int index;
    short *hdr;
    short *p;
    short n;

    index = (base - (u_long *)(D_801D98F0 + D_801D98F4 * 32)) / 504;
    hdr = (short *)(D_801D98F0 + index * 32);
    n = hdr[3];
    if (hdr[0] != 4) {
        return 1;
    }
    i = 0;
    if (n > 0) {
        do {
            p = (short *)(D_801D98F0 + ((i++ + index) << 5));
            *p = 0;
        } while (i < n);
    }
    D_801D98DC = i + index;
    return 0;
}

OBJECT_END(1);
