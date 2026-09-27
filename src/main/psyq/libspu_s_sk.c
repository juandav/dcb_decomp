#include "psyq.h"

extern long D_8006EFF0;
extern volatile u_short D_801D8560[4];
extern volatile long D_8006EFBC;
extern volatile long D_8006EFB8;
extern long D_8006EF90;

void SpuSetKey(long on_off, u_long voice_bit) {
    u_long hi;
    volatile u_short *rq;

    voice_bit &= 0xFFFFFF;
    hi = voice_bit >> 16;
    switch (on_off) {
    case 1:
        if (D_8006EFF0 & 1) {
            rq = D_801D8560;
            rq[0] = voice_bit;
            rq[1] = hi;
            D_8006EFBC |= 1;
            D_8006EFB8 |= voice_bit;
            if (rq[2] & voice_bit) {
                rq[2] &= ~voice_bit;
            }
            if (rq[3] & hi) {
                rq[3] &= ~hi;
            }
        } else {
            D_8006EF24[0xC4] = voice_bit;
            D_8006EF24[0xC5] = hi;
            D_8006EF90 |= voice_bit;
        }
        break;
    case 0:
        if (D_8006EFF0 & 1) {
            rq = D_801D8560;
            rq[2] = voice_bit;
            rq[3] = hi;
            D_8006EFBC |= 1;
            D_8006EFB8 &= ~voice_bit;
            if (rq[0] & voice_bit) {
                rq[0] &= ~voice_bit;
            }
            if (rq[1] & hi) {
                rq[1] &= ~hi;
            }
        } else {
            D_8006EF24[0xC6] = voice_bit;
            D_8006EF24[0xC7] = hi;
            D_8006EF90 &= ~voice_bit;
        }
        break;
    }
}

OBJECT_END(1);
