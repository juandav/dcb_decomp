#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_801D98F4;

void StClearRing(void) {
    D_801D98DC = 0;
    D_801D98D8 = 0;
    D_801D98D4 = 0;
    D_801D98CC = 0;
    init_ring_status(0, D_801D98F4);
    StCdIntrFlag = 0;
    D_801D98B4 = 0;
    D_801D98B0 = 0;
}
