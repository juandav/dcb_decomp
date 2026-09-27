#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndSetVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSeqSetVol);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSepSetVol);

void SsSeqGetVol(short access_num, short seq_num, short *voll, short *volr) {
    _SsVmGetSeqVol((short)(access_num | (seq_num << 8)), voll, volr);
}

OBJECT_END(3);
