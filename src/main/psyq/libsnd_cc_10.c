#include "psyq.h"

void _SsContPanpot(short seq, short sep, u_char data) {
    short seq_sep = seq | (sep << 8);
    SeqStruct *score = *(D_801D8618 + seq) + sep;
    u_char ch = score->channel;

    _SsVmSetVol(seq_sep, score->vabId, score->programs[ch], *(score->vol + ch), data);
    score->panpot[ch] = data;
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(1);
