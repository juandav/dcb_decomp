#include "psyq.h"

extern long D_801D98E4;
extern StHEADER *D_801D98F0;

u_long StGetNext(u_long **addr, u_long **header) {
    volatile u_short *ptr = (u_short *)&D_801D98F0[D_801D98DC];

    if (*ptr == 1) {
        D_801D98DC = 0;
        if (D_801D98E4 != 0) {
            *ptr = 0;
        }
        ptr = (u_short *)&D_801D98F0[D_801D98DC];
    }
    if (*ptr != 2) {
        return 1;
    }
    *ptr = 4;
    *addr = (u_long *)(&D_801D98F0[D_801D98F4] + D_801D98DC * 0x3F);
    *header = (u_long *)ptr;
    return 0;
}

OBJECT_END(2);
