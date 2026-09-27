#include "psyq.h"

void _SsGetSeqData(short seq, short sep);

void _SsSeqPlay(short seq, short sep) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;
    long t;

    if (score->delta - score->unk54 > 0) {
        if (score->unk52 > 0) {
            score->unk52--;
        } else if (score->unk52 == 0) {
            score->unk52 = score->unk54;
            score->delta--;
        } else {
            score->delta -= score->unk54;
        }
    } else if (score->unk54 >= score->delta) {
        t = score->delta;
        do {
            do {
                _SsGetSeqData(seq, sep);
            } while (score->delta == 0);
            t += score->delta;
        } while (t < score->unk54);
        score->delta = t - score->unk54;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsSeqGetEof);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsGetSeqData);
