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

void _SsSndNextSep(short seq, short sep);
void _SsVmSeqKeyOff(short seq_sep_no);

void _SsSeqGetEof(short seq, short sep, char type) {
    SeqStruct *score = *(D_801D8618 + seq) + sep;

    score->unk21++;
    if (score->unk20 == 0) {
        score->unk88 = 0;
        score->unk1A[2] = 0;
        score->delta = 0;
        if ((*(D_801D8618 + seq) + sep)->flags & 0x400) {
            score->unk0 = score->unkC;
        } else {
            score->unk0 = score->unk4;
        }
    } else if (score->unk21 < score->unk20) {
        score->unk88 = 0;
        score->unk1A[2] = 0;
        score->delta = 0;
        if ((*(D_801D8618 + seq) + sep)->flags & 0x400) {
            score->unk0 = score->unkC;
            score->unk8 = score->unkC;
        } else {
            score->unk0 = score->unk4;
            score->unk8 = score->unk4;
        }
    } else {
        (*(D_801D8618 + seq) + sep)->flags &= ~1;
        (*(D_801D8618 + seq) + sep)->flags &= ~8;
        (*(D_801D8618 + seq) + sep)->flags &= ~2;
        (*(D_801D8618 + seq) + sep)->flags |= 0x200;
        (*(D_801D8618 + seq) + sep)->flags |= 4;
        score->unk14 = 0;
        if ((*(D_801D8618 + seq) + sep)->flags & 0x400) {
            score->unk8 = score->unkC;
        } else {
            score->unk8 = score->unk4;
        }
        if (score->unk22 != -1) {
            score->unk14 = 0;
            _SsSndNextSep(score->unk22, score->unk23);
            _SsVmSeqKeyOff(seq | (sep << 8));
        }
        _SsVmSeqKeyOff(seq | (sep << 8));
        score->delta = score->unk54;
    }
}

INCLUDE_ASM("main/nonmatchings/psyq", _SsGetSeqData);
