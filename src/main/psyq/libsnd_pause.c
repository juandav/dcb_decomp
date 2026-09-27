#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSndPause);

void _SsSndPlay(short a, short b) {
    _SsSeqPlay(a, b);
}

OBJECT_END(1);
