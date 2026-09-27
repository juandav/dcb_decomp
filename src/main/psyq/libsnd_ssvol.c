#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void _SsVmSetSeqVol(short seq_sep_no, u_short voll, u_short volr, int mode);

void _SsSndSetVol(short seq, short sep, u_short voll, u_short volr) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;

    if (score->flags != 1) {
        score->voll = voll;
        score->volr = volr;
    } else {
        _SsVmSetSeqVol(seq | (sep << 8), voll, volr, 1);
    }
}

void SsSeqSetVol(short seq_access_num, short voll, short volr) {
    SeqStruct *score = *(D_801D8618 + seq_access_num);

    if (score->flags != 1) {
        score->voll = voll;
        score->volr = volr;
    } else {
        _SsVmSetSeqVol(seq_access_num, voll, volr, 1);
    }
}

void SsSepSetVol(short seq_access_num, short seq_num, short voll, short volr) {
    SeqStruct *score = *(D_801D8618 + seq_access_num) + seq_num;

    if (score->flags != 1) {
        score->voll = voll;
        score->volr = volr;
    } else {
        _SsVmSetSeqVol(seq_access_num | (seq_num << 8), voll, volr, 1);
    }
}

OBJECT_END(2);
