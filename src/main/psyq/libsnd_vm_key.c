#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOn);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOff);

void _SsVmKeyOn(int seq_sep_no, short vab, short prog, u_short note, u_short voll, u_short volr);

void _SsVmSeKeyOn(short vab, short prog, u_short note, u_short pitch, long voll, long volr) {
    u_short vol;
    u_short pan;

    if ((u_short)voll == (u_short)volr) {
        pan = 64;
        vol = voll;
    } else if ((u_short)volr < (u_short)voll) {
        pan = ((u_short)volr << 6) / (u_short)voll;
        vol = voll;
    } else {
        pan = 127 - ((u_short)voll << 6) / (u_short)volr;
        vol = volr;
    }
    _SsVmKeyOn(0x21, vab, prog, note, vol, pan);
}

void _SsVmSeKeyOff(short seq_sep_no, short vab_no, u_short note) {
    _SsVmKeyOff(0x21, seq_sep_no, vab_no, note);
}

OBJECT_END(1);
