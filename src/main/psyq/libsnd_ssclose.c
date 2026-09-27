#include "psyq.h"

void _SsVmSetSeqVol(short seq_sep_no, u_short voll, u_short volr, int mode);
void _SsVmSeqKeyOff(short seq_sep_no);
extern long D_801D8610;
extern short D_801D8E9A;

void func_8004C300(short seq) {
    int i;

    _SsVmSetSeqVol(seq, 0, 0, 1);
    _SsVmSeqKeyOff(seq);
    D_801D8610 &= ~(1 << seq);
    i = 0;
    if (D_801D8E9A > 0) do {
        D_801D8618[seq][i].flags = 0;
        D_801D8618[seq][i].unk22 = -1;
        D_801D8618[seq][i].unk23 = 0;
        D_801D8618[seq][i].unk48 = 0;
        D_801D8618[seq][i].unk4A = 0;
        D_801D8618[seq][i].unk9C = 0;
        D_801D8618[seq][i].unkA0 = 0;
        D_801D8618[seq][i].unk4C = 0;
        D_801D8618[seq][i].unkAC = 0;
        D_801D8618[seq][i].unkA8 = 0;
        D_801D8618[seq][i].unkA4 = 0;
        D_801D8618[seq][i].unk4E = 0;
        D_801D8618[seq][i].voll = 0x7F;
        D_801D8618[seq][i].volr = 0x7F;
    } while (++i < D_801D8E9A);
}

void SsSeqClose(short seq) {
    func_8004C300(seq);
}

void SsSepClose(short sep_access_num) {
    func_8004C300(sep_access_num);
}

OBJECT_END(3);
