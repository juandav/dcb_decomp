#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSetSeqVol);

typedef struct {
    short seq_sep_no;
} SvmCur;
extern SvmCur D_801D96F4;

short _SsVmGetSeqVol(short seq_sep_no, short *voll, short *volr) {
    SeqStruct *score;

    score = &D_801D8618[seq_sep_no & 0xFF][(seq_sep_no & 0xFF00) >> 8];
    D_801D96F4.seq_sep_no = seq_sep_no;
    *voll = score->voll;
    *volr = score->volr;
    return D_801D96F4.seq_sep_no;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSeqKeyOff);
