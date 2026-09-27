#include "psyq.h"

void _SsVmSetSeqVol(short seq_sep_no, u_short voll, u_short volr, int mode);

void Snd_SetPlayMode(short seq, short sep, char mode, short loop) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;

    score->unk0 = score->unk4;
    score->unk8 = score->unk4;
    score->unkC = score->unk4;
    (*(D_801D8618 + seq) + sep)->flags &= ~0x200;
    (*(D_801D8618 + seq) + sep)->flags &= ~4;
    score->unk20 = loop;
    if (mode == 1) {
        (*(D_801D8618 + seq) + sep)->flags |= 1;
        score->unk14 = 1;
        score->unk21 = 0;
        _SsVmSetSeqVol(seq | (sep << 8), score->voll, score->volr, 1);
    } else if (mode == 0) {
        (*(D_801D8618 + seq) + sep)->flags |= 2;
    }
}

OBJECT_END(2);
