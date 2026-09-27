#include "psyq.h"

long SpuSetNoiseClock(long n_clock) {
    long clock;

    if (n_clock < 0) {
        clock = 0;
    } else if (n_clock > 0x3F) {
        clock = 0x3F;
    } else {
        clock = n_clock;
    }
    D_8006EF24[0xD5] = (D_8006EF24[0xD5] & ~0x3F00) | ((clock & 0x3F) << 8);
    return clock;
}

OBJECT_END(2);
