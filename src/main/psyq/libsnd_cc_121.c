#include "psyq.h"

void _SsContResetAll(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];
    int ch;

    SsUtReverbOff();
    func_80052050();
    ch = score->channel;
    score->programs[ch] = ch;
    score->rpn1 = 0;
    score->rpn2 = 0;
    score->vol[score->channel] = 0x7F;
    score->panpot[score->channel] = 0x40;
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(2);
