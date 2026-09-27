#include "psyq.h"

void _SsContBankChange(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];
    u_char *p = score->unk0;

    score->vabId = *p;
    score->unk0 = p + 1;
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(3);
