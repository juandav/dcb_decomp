#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void _SsVmSeqKeyOff(short seq_sep_no);

void _SsSndPause(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];

    _SsVmSeqKeyOff(seq | (sep << 8));
    score->unk14 = 0;
    D_801D8618[seq][sep].flags &= ~2;
}

OBJECT_END(2);

void _SsSndPlay(short a, short b) {
    _SsSeqPlay(a, b);
}

OBJECT_END(1);
