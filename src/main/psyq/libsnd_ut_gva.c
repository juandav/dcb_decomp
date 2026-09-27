#include "psyq.h"

typedef struct SvmCur {
    /* 0x00 */ u8 unk0[7];
    /* 0x07 */ char prog;
} SvmCur;

extern u_char D_801D9700[];
extern SvmCur D_801D96E0;
extern VagAtr *D_801D96D0;


short SsUtGetVagAtr(short vabId, short progNum, short toneNum, VagAtr *p) {
    short i;

    if (D_801D9700[vabId] == 1) {
        _SsVmVSetUp(vabId, progNum);
        i = D_801D96E0.prog * 16 + toneNum;
        p->prior = D_801D96D0[i].prior;
        p->mode = D_801D96D0[i].mode;
        p->vol = D_801D96D0[i].vol;
        p->pan = D_801D96D0[i].pan;
        p->center = D_801D96D0[i].center;
        p->shift = D_801D96D0[i].shift;
        p->max = D_801D96D0[i].max;
        p->min = D_801D96D0[i].min;
        p->vibW = D_801D96D0[i].vibW;
        p->vibT = D_801D96D0[i].vibT;
        p->porW = D_801D96D0[i].porW;
        p->porT = D_801D96D0[i].porT;
        p->pbmin = D_801D96D0[i].pbmin;
        p->pbmax = D_801D96D0[i].pbmax;
        p->adsr1 = D_801D96D0[i].adsr1;
        p->adsr2 = D_801D96D0[i].adsr2;
        p->prog = D_801D96D0[i].prog;
        p->vag = D_801D96D0[i].vag;
        return 0;
    }
    return -1;
}

OBJECT_END(2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtKeyOnV);

extern int D_801D860C;
extern short D_801D96F8;
void _SsVmKeyOffNow(int);

short SsUtKeyOffV(short voice) {
    if (D_801D860C == 1) {
        return -1;
    }
    D_801D860C = 1;
    if ((u_short)voice < 24) {
        D_801D96F8 = voice;
        _SsVmKeyOffNow(0);
        D_801D860C = 0;
        return 0;
    }
    D_801D860C = 0;
    return -1;
}
