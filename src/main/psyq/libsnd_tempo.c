#include "psyq.h"

extern u_long D_801D8614;

void _SsSndTempo(short seq, short sep) {
    SeqStruct *score = &D_801D8618[seq][sep];

    score->unkA8--;
    if (score->unkA8 < 0) {
        D_801D8618[seq][sep].flags &= ~0x40;
        D_801D8618[seq][sep].flags &= ~0x80;
        return;
    }
    if (score->unk4E > 0) {
        if (score->unkA8 % score->unk4E == 0) {
            if (score->unk94 > score->unkAC) {
                score->unk94--;
            } else if (score->unk94 < score->unkAC) {
                score->unk94++;
            }
            score->unk54 = (score->unk50 * score->unk94 * 10) / (D_801D8614 * 60);
            if (score->unk54 <= 0) {
                score->unk54 = 1;
            }
            if (score->unkA8 == 0 || score->unk94 == score->unkAC) {
                D_801D8618[seq][sep].flags &= ~0x40;
                D_801D8618[seq][sep].flags &= ~0x80;
            }
        }
    } else {
        if (score->unk94 > score->unkAC) {
            score->unk94 += score->unk4E;
            if (score->unk94 < score->unkAC) {
                score->unk94 = score->unkAC;
            }
        } else if (score->unk94 < score->unkAC) {
            score->unk94 -= score->unk4E;
            if (score->unk94 > score->unkAC) {
                score->unk94 = score->unkAC;
            }
        }
        score->unk54 = (score->unk50 * score->unk94 * 10) / (D_801D8614 * 60);
        if (score->unk54 <= 0) {
            score->unk54 = 1;
        }
        if (score->unkA8 == 0 || score->unk94 == score->unkAC) {
            D_801D8618[seq][sep].flags &= ~0x40;
            D_801D8618[seq][sep].flags &= ~0x80;
        }
    }
}

OBJECT_END(1);
