#include "psyq.h"

/* _spu_env: bit 0 set while the SPU registers are written through the queue */
extern volatile long D_8006EFF0;
/* _spu_RQmask: the register groups the queue has to write */
extern volatile long D_8006EFBC;
/* _spu_RQ: the queued copy of the voice control registers */
extern volatile u_short D_801D83D8[];

/* Turns the voices in bits on or off (SPU_ON/SPU_OFF) or sets them all
   (SPU_BIT) in the register pair at addr1 (voices 0-15) and addr2 (16-23),
   and returns the resulting 24-bit mask */
u_long _SpuSetAnyVoice(long on_off, u_long bits, int addr1, int addr2) {
    u_long ret;

    if (D_8006EFF0 & 1) {
        ret = ((D_801D83D8[addr2] & 0xFF) << 16) | D_801D83D8[addr1];
    } else {
        ret = ((D_8006EF24[addr2] & 0xFF) << 16) | D_8006EF24[addr1];
    }
    switch (on_off) {
    case SPU_ON:
        if (D_8006EFF0 & 1) {
            D_801D83D8[addr1] |= bits;
            D_801D83D8[addr2] |= (bits >> 16) & 0xFF;
            D_8006EFBC |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            D_8006EF24[addr1] |= bits;
            D_8006EF24[addr2] |= (bits >> 16) & 0xFF;
        }
        ret |= bits & 0xFFFFFF;
        break;
    case SPU_OFF:
        if (D_8006EFF0 & 1) {
            D_801D83D8[addr1] &= ~bits;
            D_801D83D8[addr2] &= ~((bits >> 16) & 0xFF);
            D_8006EFBC |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            D_8006EF24[addr1] &= ~bits;
            D_8006EF24[addr2] &= ~((bits >> 16) & 0xFF);
        }
        ret &= ~(bits & 0xFFFFFF);
        break;
    case SPU_BIT:
        if (D_8006EFF0 & 1) {
            D_801D83D8[addr1] = bits;
            D_801D83D8[addr2] = (bits >> 16) & 0xFF;
            D_8006EFBC |= 1 << ((addr1 - 0xC6) >> 1);
        } else {
            D_8006EF24[addr1] = bits;
            D_8006EF24[addr2] = (bits >> 16) & 0xFF;
        }
        ret = bits & 0xFFFFFF;
        break;
    }
    return ret & 0xFFFFFF;
}


OBJECT_END(1);
