#include "psyq.h"

/* _svm_cur */
typedef struct SvmCur {
    /* 0x00 */ char tones;
    /* 0x01 */ char vab;
    /* 0x02 */ char note;
    /* 0x03 */ char unk3;
    /* 0x04 */ char voll;
    /* 0x05 */ char pan2;
    /* 0x06 */ char program;
    /* 0x07 */ char prog;
    /* 0x08 */ char unk8;
    /* 0x09 */ char unk9;
    /* 0x0A */ char mvol;
    /* 0x0B */ char mpan;
    /* 0x0C */ char vag;
    /* 0x0D */ char vol;
    /* 0x0E */ char pan;
    /* 0x0F */ char prior;
    /* 0x10 */ char centre;
    /* 0x11 */ u_char shift;
    /* 0x12 */ char mode;
    /* 0x13 */ char unk13;
    /* 0x14 */ short seq_sep_no;
    /* 0x16 */ short vag_idx;
    /* 0x18 */ short voice;
    /* 0x1A */ short unk1A;
} SvmCur;

extern SvmCur D_801D96E0;
extern VabHdr *D_801D96CC;
extern short D_801D96C0;
/* _svm_sreg_buf: shadow of the SPU voice registers */
typedef struct SvmSreg {
    /* 0x0 */ short voll;
    /* 0x2 */ short volr;
    /* 0x4 */ short pitch;
    /* 0x6 */ short addr;
    /* 0x8 */ short adsr1;
    /* 0xA */ short adsr2;
    /* 0xC */ short unkC;
    /* 0xE */ short unkE;
} SvmSreg;
extern SvmSreg D_801D9410[];
extern char D_801D93F0[];
extern char D_801D96D4;
extern long D_8006F564;
extern VmVoice D_801D8EB0[];
extern u_short D_801D8EA0;
extern u_short D_801D8EA2;
extern u_short D_801D95D8;
extern u_short D_801D95DA;
extern u_short D_801D8EA4;
extern u_short D_801D8EA6;
extern u_short D_801D8EA8;
extern u_short D_801D8EAA;

void vmNoiseOn(u_char voice) {
    SeqStruct *score;
    u_int voll_t;
    u_int volr_t;
    u_int voll;
    u_int volr;
    u_int pan;
    u_short lo;
    u_short hi;
    short i;
    u_int v;

    score = D_801D8618[D_801D96E0.seq_sep_no & 0xFF] + ((D_801D96E0.seq_sep_no & 0xFF00) >> 8);
    voll_t = D_801D96E0.voll * 0x3FFF * D_801D96CC->mvol / 16129;
    volr_t = voll_t * D_801D96E0.mvol * D_801D96E0.vol / 16129;
    voll_t = volr_t;
    if (D_801D96E0.seq_sep_no != 0x21) {
        voll_t = volr_t * score->voll / 127;
        volr_t = volr_t * score->volr / 127;
    }
    pan = D_801D96E0.pan;
    if (pan < 64) {
        voll = voll_t;
        volr = volr_t * pan / 63;
    } else {
        voll = voll_t * (127 - pan) / 63;
        volr = volr_t;
    }
    pan = D_801D96E0.mpan;
    if (pan < 64) {
        volr = volr * pan / 63;
    } else {
        voll = voll * (127 - pan) / 63;
    }
    pan = D_801D96E0.pan2;
    if (pan < 64) {
        volr = pan * volr / 63;
    } else {
        voll = voll * (127 - pan) / 63;
    }
    if (D_801D96C0 == 1) {
        if (voll < volr) {
            voll = volr;
        } else {
            volr = voll;
        }
    }
    if (D_801D96E0.seq_sep_no != 0x21) {
        voll = voll * voll / 0x3FFF;
        volr = volr * volr / 0x3FFF;
    }
    v = voice;
    SpuSetNoiseClock((D_801D96E0.note - D_801D96E0.centre) & 0x3F);
    D_801D9410[v].volr = volr;
    D_801D9410[v].voll = voll;
    D_801D93F0[v] |= 3;
    if (v < 16) {
        lo = 1 << v;
        hi = 0;
    } else {
        lo = 0;
        hi = 1 << (v - 16);
    }
    D_801D8EB0[voice].unk4 = 10;
    for (i = 0; i < D_801D96D4; i++) {
        if (!(D_8006F564 & (1 << i))) {
            D_801D8EB0[i].unk1D &= 1;
        }
    }
    D_801D8EB0[voice].unk1D = 2;
    D_801D8EA0 |= lo;
    D_801D8EA2 |= hi;
    D_801D95D8 &= ~D_801D8EA0;
    D_801D95DA &= ~D_801D8EA2;
    if (D_801D96E0.mode & 4) {
        D_801D8EA4 |= lo;
        D_801D8EA6 |= hi;
    } else {
        D_801D8EA4 &= ~lo;
        D_801D8EA6 &= ~hi;
    }
    D_801D8EA8 = lo;
    D_801D8EAA = hi;
}

OBJECT_END(2);
