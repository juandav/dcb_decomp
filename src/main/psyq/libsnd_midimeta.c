#include "psyq.h"

extern u_long D_801D8614;

void _SsGetMetaEvent(short seq, short sep, u_char type) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;
    long t0;
    long t1;
    long t2;

    t0 = *score->unk0++;
    t1 = *score->unk0++;
    t2 = *score->unk0++;
    score->unk94 = 60000000 / (t2 | (t0 << 16 | t1 << 8));
    if (score->unk50 * score->unk94 * 10 < D_801D8614 * 60) {
        score->unk54 = score->unk52 = (D_801D8614 * 600) / (score->unk50 * score->unk94);
    } else {
        score->unk52 = -1;
        score->unk54 = (score->unk50 * score->unk94 * 10) / (D_801D8614 * 60);
        if ((score->unk50 * score->unk94 * 10) % (D_801D8614 * 60) > D_801D8614 * 30) {
            score->unk54++;
        }
    }
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(2);
