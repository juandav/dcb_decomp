#include "psyq.h"

typedef struct SvmCur {
    /* 0x00 */ u8 unk0[2];
    /* 0x02 */ char note;
    /* 0x03 */ u8 unk3[4];
    /* 0x07 */ char prog;
    /* 0x08 */ u8 unk8[4];
    /* 0x0C */ char tone;
    /* 0x0D */ u8 unkD[3];
    /* 0x10 */ u_char center;
    /* 0x11 */ u_char shift;
} SvmCur;

extern SvmCur D_801D96E0;
extern VagAtr *D_801D96D0;

u_short note2pitch(void) {
    u_char shift = D_801D96E0.shift;

    if (shift >= 0x80) {
        shift = 0x7F;
    }
    return SsPitchFromNote(D_801D96E0.note, 0, D_801D96E0.center, shift);
}

u_short note2pitch2(short note, short fine) {
    short i = D_801D96E0.prog * 16 + D_801D96E0.tone;

    return SsPitchFromNote(note, fine, D_801D96D0[i].center, D_801D96D0[i].shift);
}

extern u_short D_8006F864[];
extern u_short D_8006F87C[];

u_short SsPitchFromNote(short note, short fine, u_char center, u_char shift) {
    u_long pitch;
    int s;
    int q;
    short f;
    short n;
    short octave;
    short semi;

    s = (short)(shift + fine);
    q = s / 128;
    note += q;
    note -= center;
    n = note;
    f = s - q * 128;
    if (f < 0) {
        f += 128;
        note--;
        n = note + f / 128;
    }
    octave = n / 12 - 2;
    semi = n % 12;
    if (semi < 0) {
        semi += 12;
        octave = n / 12 - 3;
    }
    pitch = (D_8006F864[semi] * D_8006F87C[f]) >> 16;
    if (octave >= 0) {
        pitch = 0x3FFF;
    } else {
        pitch += 1 << (-octave - 1);
        pitch >>= -octave;
    }
    return pitch;
}

OBJECT_END(3);
