#include "psyq.h"

void _SsContRpn2(short seq, short sep, u_char value) {
    SeqStruct *score = &D_801D8618[seq][sep];

    score->rpn2 = value;
    score->unk1E++;
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(1);
