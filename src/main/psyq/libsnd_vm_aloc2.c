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

extern SvmCur D_801D96E0;
extern VmVoice D_801D8EB0[];
extern u_long D_801D9598[];
extern ProgAtr *D_801D96C4;
extern VagAtr *D_801D96D0;
extern u_short D_801D9410[];
extern u_char D_801D93F0[];

/* Sets up the voice just allocated in D_801D96E0.voice: its address, ADSR
   and dirty flags in the shadow SPU registers (8 halfwords per voice) */
void _SsVmDoAllocate(void) {
    int i;
    short sreg;
    int tone;
    u_short adsr2;
    short rr;

    sreg = D_801D96E0.voice << 3;
    D_801D8EB0[D_801D96E0.voice].envx = 0x7FFF;
    for (i = 0; i < 16; i++) {
        D_801D9598[i] &= ~(1 << D_801D96E0.voice);
    }
    if ((D_801D96E0.vag & 1) > 0) {
        (D_801D9410 + sreg)[3] = D_801D96C4[(D_801D96E0.vag - 1) / 2].reserved2;
        D_801D93F0[D_801D96E0.voice] |= 8;
    } else {
        (D_801D9410 + sreg)[3] = D_801D96C4[(D_801D96E0.vag - 1) / 2].reserved2 >> 16;
        D_801D93F0[D_801D96E0.voice] |= 8;
    }
    tone = D_801D96E0.prog * 16 + D_801D96E0.tone;
    (D_801D9410 + sreg)[4] = D_801D96D0[tone].adsr1;
    adsr2 = D_801D96D0[tone].adsr2;
    rr = D_801D9678 + (adsr2 & 0x1F);
    adsr2 &= 0xFFE0;
    if (rr >= 0x20) {
        rr = 0x1F;
    }
    rr |= adsr2;
    (D_801D9410 + sreg)[5] = rr;
    D_801D93F0[D_801D96E0.voice] |= 0x30;
}


OBJECT_END(2);
