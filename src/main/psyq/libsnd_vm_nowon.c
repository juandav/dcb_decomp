#include "psyq.h"

/* _svm_cur */
typedef struct SvmCur {
    /* 0x00 */ char tones;
    /* 0x01 */ char vab;
    /* 0x02 */ char note;
    /* 0x03 */ char unk3;
    /* 0x04 */ char voll;
    /* 0x05 */ u_char pan2;
    /* 0x06 */ char program;
    /* 0x07 */ char prog;
    /* 0x08 */ char unk8;
    /* 0x09 */ char unk9;
    /* 0x0A */ char mvol;
    /* 0x0B */ u_char mpan;
    /* 0x0C */ char vag;
    /* 0x0D */ char vol;
    /* 0x0E */ u_char pan;
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
extern short D_801D9410[];
extern char D_801D93F0[];
extern VmVoice D_801D8EB0[];
extern u_short D_801D8EA0;
extern u_short D_801D8EA2;
extern u_short D_801D95D8;
extern u_short D_801D95DA;
extern u_short D_801D8EA4;
extern u_short D_801D8EA6;
extern u_short D_801D8EA8;
extern u_short D_801D8EAA;

void _SsVmKeyOnNow(short vagCount, short pitch) {
    SeqStruct *score;
    u_short pos;
    u_int voll_t;
    u_int volr_t;
    u_int voll;
    u_int volr;
    u_int pan;
    u_short lo;
    u_short hi;

    pos = D_801D96E0.voice * 8;
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
        volr = volr * pan / 63;
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
    *(D_801D9410 + pos + 2) = pitch;
    *(D_801D9410 + pos + 0) = voll;
    *(D_801D9410 + pos + 1) = volr;
    D_801D93F0[D_801D96E0.voice] |= 7;
    D_801D8EB0[D_801D96E0.voice].unk4 = pitch;
    if (D_801D96E0.voice < 16) {
        lo = 1 << D_801D96E0.voice;
        hi = 0;
    } else {
        lo = 0;
        hi = 1 << (D_801D96E0.voice - 16);
    }
    if (D_801D96E0.mode & 4) {
        D_801D8EA4 |= lo;
        D_801D8EA6 |= hi;
    } else {
        D_801D8EA4 &= ~lo;
        D_801D8EA6 &= ~hi;
    }
    D_801D8EA8 &= ~lo;
    D_801D8EAA &= ~hi;
    D_801D8EA0 |= lo;
    D_801D8EA2 |= hi;
    D_801D95D8 &= ~D_801D8EA0;
    D_801D95DA &= ~D_801D8EA2;
}

OBJECT_END(2);
