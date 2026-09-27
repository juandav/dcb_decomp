#include "psyq.h"

long _SsReadDeltaValue(short seq, short sep) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;
    long delta;
    long ret;
    u_char c;

    delta = *score->unk0++;
    if (delta == 0) {
        return 0;
    }
    if (delta & 0x80) {
        delta &= 0x7F;
        do {
            c = *score->unk0++;
            delta = (delta << 7) + (c & 0x7F);
        } while (c & 0x80);
    }
    ret = delta * 10;
    score->unk88 += ret;
    return ret;
}

OBJECT_END(3);
