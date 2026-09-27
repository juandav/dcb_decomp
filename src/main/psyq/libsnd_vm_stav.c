#include "psyq.h"

typedef struct SvmCur {
    /* 0x00 */ char tones;
    /* 0x01 */ u_char vab;
    /* 0x02 */ char note;
    /* 0x03 */ u8 unk3[3];
    /* 0x06 */ u_char program;
    /* 0x07 */ char prog;
} SvmCur;

extern SvmCur D_801D96E0;
extern VagAtr *D_801D96D0;

u_char _SsVmSelectToneAndVag(u_char *tones, u_char *vags) {
    char i;
    u_char n = 0;

    for (i = 0; i < D_801D96E0.tones; i++) {
        if (D_801D96D0[i + D_801D96E0.prog * 16].min <= D_801D96E0.note &&
            D_801D96D0[i + D_801D96E0.prog * 16].max >= D_801D96E0.note) {
            vags[n] = *(u_char *)&D_801D96D0[i + D_801D96E0.prog * 16].vag;
            tones[n++] = i;
        }
    }
    return n;
}

OBJECT_END(3);
