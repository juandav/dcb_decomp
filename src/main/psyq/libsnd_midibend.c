#include "psyq.h"

void _SsVmPitchBend(short seq_sep, char vabId, u_char prog, u_char bend);

void _SsSetPitchBend(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];
    u_char bend = *score->unk0;
    u_char ch = score->channel;

    score->unk0++;
    _SsVmPitchBend(seq | (sep << 8), score->vabId, score->programs[ch], bend);
    score->delta = _SsReadDeltaValue(seq, sep);
}
