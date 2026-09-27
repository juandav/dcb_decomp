#include "psyq.h"

/* _svm_cur */
typedef struct SvmCur {
    /* 0x00 */ char tones;
    /* 0x01 */ u_char vab;
    /* 0x02 */ char note;
    /* 0x03 */ u8 unk3[3];
    /* 0x06 */ u_char program;
    /* 0x07 */ char prog;
} SvmCur;

/* ProgAtr as the library sees it: only the low byte of reserved1 is used */
typedef struct ProgAtrB {
    /* 0x0 */ u8 unk0[8];
    /* 0x8 */ u_char reserved1;
    /* 0x9 */ u8 unk9[7];
} ProgAtrB;

extern SvmCur D_801D96E0;
extern VagAtr *D_801D96D0;
extern ProgAtrB *D_801D96C4;
extern VabHdr *D_801D96CC;
extern short D_801D96C2;
extern u_char D_801D9700[16];
extern ProgAtrB *D_801D95F8[16];
extern VabHdr *D_801D9638[16];
extern VagAtr *D_801D9680[16];

long _SsVmVSetUp(short vab, short prog) {
    if ((u_short)vab >= 16) {
        return -1;
    }
    if (D_801D9700[vab] != 1) {
        return -1;
    }
    if (prog < D_801D96C2) {
        D_801D96CC = D_801D9638[vab];
        D_801D96C4 = D_801D95F8[vab];
        D_801D96D0 = D_801D9680[vab];
        D_801D96E0.vab = vab;
        D_801D96E0.program = prog;
        D_801D96E0.prog = D_801D96C4[prog].reserved1;
        return 0;
    }
    return -1;
}
