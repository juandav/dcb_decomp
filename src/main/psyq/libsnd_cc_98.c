#include "psyq.h"

extern void (*D_801D8698[][16])(short, short, u_char);

void _SsContNrpn1(short seq, short sep, u_char data) {
    SeqStruct *score = &D_801D8618[seq][sep];

    if (score->unk1A[1] == 40 && D_801D8698[seq][sep] != NULL) {
        D_801D8698[seq][sep](seq, sep, data);
    }
    if (score->unk1A[1] != 30 && score->unk1A[1] != 20 && score->unk1A[1] != 40) {
        score->unk1A[0] = data;
        score->unk1A[2] = 0;
        score->unk1F++;
    }
    score->delta = _SsReadDeltaValue(seq, sep);
}
