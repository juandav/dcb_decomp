#include "psyq.h"

void _SsSetProgramChange(short seq, short sep, u_char prog) {
    SeqStruct *score = &D_801D8618[seq][sep];

    score->programs[score->channel] = prog;
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(1);
