#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

#define SET_REVERB(i)                                  \
    if (all || (mask & (1 << (i)))) {                  \
        D_8006EF24[0xE0 + (i)] = attr->param[(i)];     \
    }

void _spu_setReverbAttr(SpuReverbRegs *attr) {
    long mask = attr->mask;
    int all = mask == 0;

    SET_REVERB(0);
    SET_REVERB(1);
    SET_REVERB(2);
    SET_REVERB(3);
    SET_REVERB(4);
    SET_REVERB(5);
    SET_REVERB(6);
    SET_REVERB(7);
    SET_REVERB(8);
    SET_REVERB(9);
    SET_REVERB(10);
    SET_REVERB(11);
    SET_REVERB(12);
    SET_REVERB(13);
    SET_REVERB(14);
    SET_REVERB(15);
    SET_REVERB(16);
    SET_REVERB(17);
    SET_REVERB(18);
    SET_REVERB(19);
    SET_REVERB(20);
    SET_REVERB(21);
    SET_REVERB(22);
    SET_REVERB(23);
    SET_REVERB(24);
    SET_REVERB(25);
    SET_REVERB(26);
    SET_REVERB(27);
    SET_REVERB(28);
    SET_REVERB(29);
    SET_REVERB(30);
    SET_REVERB(31);
}
