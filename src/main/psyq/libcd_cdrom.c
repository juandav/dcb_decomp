#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern u_long *D_801D98F0;

extern long D_801D98F4;

void StSetRing(u_long *ring_addr, u_long ring_size) {
    D_801D98F0 = ring_addr;
    D_801D98F4 = ring_size;
    StClearRing();
}

OBJECT_END(1);
