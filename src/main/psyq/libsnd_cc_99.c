#include "psyq.h"

void _SsContNrpn2(short seq, short sep, u_char data) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;

    switch (data) {
    case 20:
        score->unk1A[1] = data;
        score->unk1A[2] = 1;
        score->delta = _SsReadDeltaValue(seq, sep);
        score->unk8 = score->unk0;
        break;
    case 30:
        score->unk1A[1] = data;
        if (score->unk1A[3] == 0) {
            score->unk15 = 0;
            score->delta = _SsReadDeltaValue(seq, sep);
        } else if (score->unk1A[3] < 0x7F) {
            score->unk1A[3]--;
            score->delta = _SsReadDeltaValue(seq, sep);
            if (score->unk1A[3] != 0) {
                score->unk0 = score->unk8;
            } else {
                score->unk15 = 0;
            }
        } else {
            _SsReadDeltaValue(seq, sep);
            score->delta = 0;
            score->unk0 = score->unk8;
        }
        break;
    default:
        score->unk1A[1] = data;
        score->unk1F++;
        score->delta = _SsReadDeltaValue(seq, sep);
        break;
    }
}

OBJECT_END(3);
