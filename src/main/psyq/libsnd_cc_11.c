#include "psyq.h"

void _SsContExpression(short seq, short sep, u_char vol) {
    SeqStruct *score = &D_801D8618[seq][sep];
    u_char ch = score->channel;

    _SsVmSetProgVol(score->vabId, score->programs[ch], vol);
    _SsVmSetVol(seq | (sep << 8), score->vabId, score->programs[ch], score->vol[ch], score->panpot[ch]);
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(3);
