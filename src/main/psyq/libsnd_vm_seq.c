#include "psyq.h"

extern VabHdr *D_801D96CC;
extern ProgAtr *D_801D96C4;
extern VagAtr *D_801D96D0;
extern short D_801D96C0;
extern short D_801D9410[];
extern char D_801D93F0[];
extern char D_801D96D4;
extern long D_8006F564;
extern VmVoice D_801D8EB0[];

long _SsVmVSetUp(short vab, short prog);

short _SsVmSetSeqVol(short seq_sep_no, u_short voll, u_short volr) {
    SeqStruct *score;
    short i;
    u_int voll_t;
    u_int volr_t;
    u_short l;
    u_short r;
    u_char pan;
    int v;

    score = *(D_801D8618 + (seq_sep_no & 0xFF)) + ((seq_sep_no & 0xFF00) >> 8);
    score->voll = voll;
    score->volr = volr;
    if (score->voll >= 127) {
        score->voll = 127;
    }
    if (score->volr >= 127) {
        score->volr = 127;
    }
    for (i = 0; i < D_801D96D4; i++) {
        if (!(D_8006F564 & (1 << i)) && D_801D8EB0[i].unk10 == seq_sep_no && D_801D8EB0[i].vabId == score->vabId) {
            _SsVmVSetUp(D_801D8EB0[i].vabId, D_801D8EB0[i].unk12);
            v = D_801D8EB0[i].unk8 * *(score->vol + D_801D8EB0[i].unkC) / 127 * 0x3FFF;
            voll_t = D_801D96CC->mvol * v / 16129;
            volr_t = voll_t * D_801D96C4[D_801D8EB0[i].prog].mvol * D_801D96D0[D_801D8EB0[i].unk12 * 16 + D_801D8EB0[i].tone].vol / 16129;
            voll_t = volr_t * score->voll / 127;
            volr_t = volr_t * score->volr / 127;
            pan = D_801D96D0[D_801D8EB0[i].unk12 * 16 + D_801D8EB0[i].tone].pan;
            if (pan < 64) {
                l = voll_t;
                r = volr_t * pan / 63;
            } else {
                l = voll_t * (127 - pan) / 63;
                r = volr_t;
            }
            pan = D_801D96C4[D_801D8EB0[i].prog].mpan;
            if (pan < 64) {
                r = r * pan / 63;
            } else {
                l = l * (127 - pan) / 63;
            }
            pan = D_801D8EB0[i].unkA;
            if (pan < 64) {
                r = r * pan / 63;
            } else {
                l = l * (127 - pan) / 63;
            }
            if (D_801D96C0 == 1) {
                if (l < r) {
                    l = r;
                } else {
                    r = l;
                }
            }
            l = l * l / 0x3FFF;
            r = r * r / 0x3FFF;
            *(D_801D9410 + i * 8) = l;
            *(D_801D9410 + i * 8 + 1) = r;
            D_801D93F0[i] |= 3;
        }
    }
    return seq_sep_no;
}

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
