#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndStop);

void SsSeqStop(short seq) {
    _SsSndStop(seq, 0);
}

void SsSepStop(short a, short b) {
    _SsSndStop(a, b);
}

OBJECT_END(3);
