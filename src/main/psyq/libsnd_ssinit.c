#include "psyq.h"

extern u_short D_8006F570[16];
extern long D_801D8698[32][16];
extern long D_801D8614;
extern long D_801D8610;
extern long D_801D860C;

void _SsVmInit(int voices);

void _SsInit(void) {
    u_short *reg;
    int i;
    int j;

    reg = (u_short *)0x1F801D80;
    for (i = 0; i < 16; i++) {
        *reg++ = D_8006F570[i];
    }
    _SsVmInit(24);
    for (j = 0; j < 32; j++) {
        for (i = 0; i < 16; i++) {
            D_801D8698[j][i] = 0;
        }
    }
    D_801D8614 = 60;
    D_801D8610 = 0;
    D_801D860C = 0;
}
