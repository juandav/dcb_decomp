#include "psyq.h"

typedef struct SvmCur {
    /* 0x00 */ u8 unk0[7];
    /* 0x07 */ char prog;
} SvmCur;

extern u_char D_801D9700[];
extern SvmCur D_801D96E0;
extern VagAtr *D_801D96D0;

short SsUtSetVagAtr(short vabId, short progNum, short toneNum, VagAtr *vagatr) {
    short i;

    if (D_801D9700[vabId] == 1) {
        _SsVmVSetUp(vabId, progNum);
        i = toneNum + D_801D96E0.prog * 16;
        D_801D96D0[i].prior = vagatr->prior;
        D_801D96D0[i].mode = vagatr->mode;
        D_801D96D0[i].vol = vagatr->vol;
        D_801D96D0[i].pan = vagatr->pan;
        D_801D96D0[i].center = vagatr->center;
        D_801D96D0[i].shift = vagatr->shift;
        D_801D96D0[i].max = vagatr->max;
        D_801D96D0[i].min = vagatr->min;
        D_801D96D0[i].vibW = vagatr->vibW;
        D_801D96D0[i].vibT = vagatr->vibT;
        D_801D96D0[i].porW = vagatr->porW;
        D_801D96D0[i].porT = vagatr->porT;
        D_801D96D0[i].pbmin = vagatr->pbmin;
        D_801D96D0[i].pbmax = vagatr->pbmax;
        D_801D96D0[i].adsr1 = vagatr->adsr1;
        D_801D96D0[i].adsr2 = vagatr->adsr2;
        D_801D96D0[i].prog = vagatr->prog;
        D_801D96D0[i].vag = vagatr->vag;
        return 0;
    }
    return -1;
}

OBJECT_END(3);
