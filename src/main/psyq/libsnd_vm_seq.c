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

extern char D_801D96D4;
extern long D_8006F564;
extern short D_801D96F8;
extern VmVoice D_801D8EB0[];

void _SsVmKeyOffNow(int);

void _SsVmSeqKeyOff(short seq_sep_no) {
    u_char i;

    for (i = 0; i < D_801D96D4; i++) {
        if (!(D_8006F564 & (1 << i)) && D_801D8EB0[i].unk10 == seq_sep_no) {
            D_801D96F8 = i;
            _SsVmKeyOffNow(0);
        }
    }
}
