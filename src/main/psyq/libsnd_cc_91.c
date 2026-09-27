#include "psyq.h"

void _SsContExternal(short seq, short sep, u_char depth) {
    SeqStruct *score = &D_801D8618[seq][sep];

    SsUtSetReverbDepth(depth, depth);
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(3);
