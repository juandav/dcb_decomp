#include "psyq.h"

extern long D_8006EF98;
extern long D_8006EF9C;
extern long D_8006EFA0;
extern SpuVolume D_8006EFAC;

long SpuSetReverb(long on_off) {
    u_short attr;

    switch (on_off) {
    case SPU_OFF:
        attr = D_8006EF24[0xD5];
        D_8006EF98 = 0;
        D_8006EF24[0xD5] = attr & ~0x80;
        D_8006EF24[0xC2] = 0;
        D_8006EF24[0xC3] = 0;
        D_8006EFAC.left = 0;
        D_8006EFAC.right = 0;
        break;
    case SPU_ON:
        if (D_8006EF9C != on_off && _SpuIsInAllocateArea_(D_8006EFA0)) {
            attr = D_8006EF24[0xD5];
            D_8006EF98 = 0;
            D_8006EF24[0xD5] = attr & ~0x80;
        } else {
            attr = D_8006EF24[0xD5];
            D_8006EF98 = on_off;
            D_8006EF24[0xD5] = attr | 0x80;
        }
        break;
    }
    return D_8006EF98;
}

OBJECT_END(2);
