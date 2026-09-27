#include "psyq.h"

extern u_long *D_801D98F0;

void init_ring_status(int start, u_int count) {
    u_int i = 0;
    u_long *p;

    if (count != 0) {
        do {
            p = D_801D98F0 + (i + start) * 8;
            *p = 0;
        } while (++i < count);
    }
}

OBJECT_END(3);
