#include "psyq.h"

void _SsContMainVol(short seq, short sep, u_char vol) {
    SeqStruct *score = &D_801D8618[seq][sep];
    u_char ch = score->channel;

    _SsVmSetVol(seq | (sep << 8), score->vabId, score->programs[ch], vol, score->panpot[ch]);
    score->vol[ch] = vol;
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(1);
