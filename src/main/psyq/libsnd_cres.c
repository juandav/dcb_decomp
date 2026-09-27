#include "psyq.h"

short _SsVmGetSeqVol(short seq_sep_no, short *voll, short *volr);
void _SsVmSetSeqVol(short seq_sep_no, u_short voll, u_short volr, int mode);

void _SsSndCrescendo(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];
    u_short voll;
    u_short volr;
    int diff;
    int l;
    int r;

    score->unkA0++;
    if (score->unkA0 > score->unk9C) {
        D_801D8618[seq][sep].flags &= ~0x10;
    } else {
        diff = score->unk48 * score->unkA0 / score->unk9C;
        diff -= score->unk4A;
        if (diff != 0) {
            score->unk4A += diff;
            _SsVmGetSeqVol(seq | (sep << 8), &voll, &volr);
            l = voll + diff;
            if (l >= 0x80) {
                l = 0x7F;
            }
            if (l < 0) {
                l = 0;
            }
            r = volr + diff;
            if (r >= 0x80) {
                r = 0x7F;
            }
            if (r < 0) {
                r = 0;
            }
            _SsVmSetSeqVol(seq | (sep << 8), l, r, 1);
            if ((l == 0x7F && r == l) || (l == 0 && r == 0)) {
                D_801D8618[seq][sep].flags &= ~0x10;
            }
        }
    }
    _SsVmGetSeqVol(seq | (sep << 8), &score->unk5C, &score->unk5E);
}

OBJECT_END(2);
