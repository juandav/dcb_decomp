#include "psyq.h"

typedef struct SvmCur {
    /* 0x00 */ u_char tones;
    /* 0x01 */ u8 unk1;
    /* 0x02 */ u_char note;
    /* 0x03 */ u_char fine;
    /* 0x04 */ char vol;
    /* 0x05 */ char pan;
    /* 0x06 */ u8 unk6;
    /* 0x07 */ char prog;
    /* 0x08 */ u8 unk8[2];
    /* 0x0A */ u_char mvol;
    /* 0x0B */ u_char mpan;
    /* 0x0C */ char tone;
    /* 0x0D */ u_char vvol;
    /* 0x0E */ u_char vpan;
    /* 0x0F */ u_char prior;
    /* 0x10 */ u_char center;
    /* 0x11 */ u_char shift;
    /* 0x12 */ u_char mode;
    /* 0x13 */ u8 unk13;
    /* 0x14 */ short seqSep;
    /* 0x16 */ short vag;
    /* 0x18 */ short voice;
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

extern int D_801D860C;
extern ProgAtr *D_801D96C4;
extern VmVoice D_801D8EB0[];
void _SsVmDoAllocate(void);
void vmNoiseOn(u_char voice);
u_short note2pitch2(u_short note, u_short fine);
void _SsVmKeyOnNow(int count, u_short pitch);

short SsUtKeyOnV(short voice, short vabId, short prog, short tone, short note, short fine, short voll, short volr) {
    short i;
    int vag;
    int n = (u_short)note;
    int f = (u_short)fine;

    if (D_801D860C == 1) {
        return -1;
    }
    D_801D860C = 1;
    if ((u_short)voice >= 24 || _SsVmVSetUp(vabId, prog) != 0) {
        D_801D860C = 0;
        return -1;
    }
    D_801D96E0.seqSep = 0x21;
    D_801D96E0.note = n;
    D_801D96E0.fine = f;
    D_801D96E0.tone = tone;
    if (voll == volr) {
        D_801D96E0.pan = 0x40;
        D_801D96E0.vol = voll;
    } else if (volr < voll) {
        D_801D96E0.pan = (volr << 6) / voll;
        D_801D96E0.vol = voll;
    } else {
        D_801D96E0.pan = 0x7F - (voll << 6) / volr;
        D_801D96E0.vol = volr;
    }
    D_801D96E0.mvol = D_801D96C4[prog].mvol;
    D_801D96E0.mpan = D_801D96C4[prog].mpan;
    D_801D96E0.tones = D_801D96C4[prog].tones;
    i = D_801D96E0.tone + D_801D96E0.prog * 16;
    D_801D96E0.prior = D_801D96D0[i].prior;
    vag = (u_short)D_801D96D0[i].vag;
    D_801D96E0.vag = vag;
    D_801D96E0.vvol = D_801D96D0[i].vol;
    D_801D96E0.vpan = D_801D96D0[i].pan;
    D_801D96E0.center = D_801D96D0[i].center;
    D_801D96E0.shift = D_801D96D0[i].shift;
    D_801D96E0.mode = D_801D96D0[i].mode;
    if (vag == 0) {
        D_801D860C = 0;
        return -1;
    }
    D_801D96E0.voice = voice;
    D_801D8EB0[voice].unk10 = 0x21;
    D_801D8EB0[voice].vabId = vabId;
    D_801D8EB0[voice].unk12 = D_801D96E0.prog;
    D_801D8EB0[voice].prog = prog;
    D_801D8EB0[voice].unk0 = D_801D96E0.vag;
    D_801D8EB0[voice].tone = D_801D96E0.tone;
    D_801D8EB0[voice].note = n;
    D_801D8EB0[voice].unk1D = 1;
    D_801D8EB0[voice].unk2 = 0;
    D_801D8EB0[voice].unk36 = D_801D96E0.vol;
    D_801D8EB0[voice].unkA = D_801D96E0.pan;
    _SsVmDoAllocate();
    if (D_801D96E0.vag == 0xFF) {
        vmNoiseOn(voice);
    } else {
        _SsVmKeyOnNow(1, note2pitch2(n, f));
    }
    D_801D860C = 0;
    return voice;
}

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
