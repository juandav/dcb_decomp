#include "psyq.h"

/* SsFCALL.control[] from its data entry handler on: data entry, main volume,
   panpot, expression, damper, NRPN 1/2, RPN 1/2, external, reset all */
extern void (*D_801D8590[11])();

void _SsSetControlChange(short seq, short sep, u_char control) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;
    long data = *score->unk0++;

    switch (control) {
    case 0:
        score->vabId = data;
        score->delta = _SsReadDeltaValue(seq, sep);
        return;
    case 6:
        D_801D8590[0](seq, sep, data);
        return;
    case 7:
        D_801D8590[1](seq, sep, data);
        return;
    case 10:
        D_801D8590[2](seq, sep, data);
        return;
    case 11:
        D_801D8590[3](seq, sep, data);
        return;
    case 64:
        D_801D8590[4](seq, sep, data);
        return;
    case 91:
        D_801D8590[9](seq, sep, data);
        return;
    case 98:
        D_801D8590[5](seq, sep, data);
        return;
    case 99:
        D_801D8590[6](seq, sep, data);
        return;
    case 100:
        D_801D8590[7](seq, sep, data);
        return;
    case 101:
        D_801D8590[8](seq, sep, data);
        return;
    case 121:
        D_801D8590[10](seq, sep);
        return;
    }
    score->delta = _SsReadDeltaValue(seq, sep);
}

OBJECT_END(1);
