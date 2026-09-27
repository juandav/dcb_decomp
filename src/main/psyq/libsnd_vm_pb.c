#include "psyq.h"

typedef struct SvmCur {
    /* 0x00 */ u8 unk0[7];
    /* 0x07 */ char prog;
    /* 0x08 */ u8 unk8[4];
    /* 0x0C */ char tone;
    /* 0x0D */ u8 unkD[11];
    /* 0x18 */ short voice;
} SvmCur;

extern SvmCur D_801D96E0;
extern VmVoice D_801D8EB0[];
extern VagAtr *D_801D96D0;
extern u_short D_801D9414[][8];
extern u_char D_801D93F0[];
u_short note2pitch2(u_short note, u_short fine);

short _SsVmPBVoice(short vc, short seq_sep_no, short vabId, short prog, long pitch) {
    short pb;
    u_short base;
    u_short idx;
    u_short note;
    u_short fine;

    pb = pitch - 64;
    if (D_801D8EB0[vc].unk10 == seq_sep_no && D_801D8EB0[vc].vabId == vabId && D_801D8EB0[vc].prog == prog) {
        base = D_801D8EB0[vc].note;
        idx = D_801D8EB0[vc].tone + D_801D96E0.prog * 16;
        if (pb > 0) {
            note = base + pb * D_801D96D0[idx].pbmax / 63;
            fine = (pb * D_801D96D0[idx].pbmax % 63) * 2;
        } else if (pb < 0) {
            note = base + pb * D_801D96D0[idx].pbmin / 64 - 1;
            fine = (pb * D_801D96D0[idx].pbmin % 64) * 2 + 0x7F;
        } else {
            note = base;
            fine = 0;
        }
        D_801D96E0.tone = D_801D8EB0[vc].tone;
        D_801D96E0.voice = vc;
        D_801D9414[vc][0] = note2pitch2(note, fine);
        D_801D93F0[vc] |= 4;
        return 1;
    }
    return 0;
}

extern short D_801D96F4;
extern char D_801D96D4;
short _SsVmPBVoice(short voice, short seq_sep, short vab, short prog, long pitch);

long _SsVmPitchBend(short seq_sep, short vab, short prog, u_short pitch) {
    short i;
    long ret = 0;

    _SsVmVSetUp(vab, prog);
    D_801D96F4 = seq_sep;
    for (i = 0; i < D_801D96D4; i++) {
        ret += _SsVmPBVoice(i, seq_sep, vab, prog, pitch);
    }
    return ret;
}

OBJECT_END(2);
