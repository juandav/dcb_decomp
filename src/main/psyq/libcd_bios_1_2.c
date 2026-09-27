#include "psyq.h"

/* libcd_bios_1's padding: GCC outputs its deferred inline function
   func_8005A088 after all of that file's top-level asm */
OBJECT_END(1);

/* the ring buffer: one 32-byte sector header per slot, status first */
extern u_char *D_801D98F0;

#define RING_STATUS(i) (*(u_short *)(D_801D98F0 + ((i) << 5)))

void StRingStatus(short *free_sectors, short *over_sectors) {
    int i = D_801D98F4;

    *free_sectors = 0;
    *over_sectors = D_801D98D4 - D_801D98DC;
    if (*over_sectors < 0) {
        i--;
        while (i >= 0 && RING_STATUS(i) != 1) {
            i--;
        }
        i++;
        *over_sectors += i;
    }
    while (--i >= 0) {
        if (RING_STATUS(i) == 0) {
            (*free_sectors)++;
        }
    }
}

OBJECT_END(3);
