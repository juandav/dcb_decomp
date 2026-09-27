#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", note2pitch);

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

u_short note2pitch2(short note, short fine) {
    short i = D_801D96E0.prog * 16 + D_801D96E0.tone;

    return SsPitchFromNote(note, fine, D_801D96D0[i].center, D_801D96D0[i].shift);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsPitchFromNote);
