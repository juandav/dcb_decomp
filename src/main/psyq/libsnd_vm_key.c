#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("main/nonmatchings/psyq", _SsVmKeyOn);

extern char D_801D96D4;
extern long D_8006F564;
extern short D_801D96F8;
extern VmVoice D_801D8EB0[];
void _SsVmKeyOffNow(int);
void vmNoiseOff(u8 voice);

int _SsVmKeyOff(short seq_sep_no, short vab, short prog, u_short note) {
    u_char i;
    int count;

    count = 0;
    for (i = 0; i < D_801D96D4; i++) {
        if (!(D_8006F564 & (1 << i)) && D_801D8EB0[i].note == note && D_801D8EB0[i].prog == prog &&
            D_801D8EB0[i].unk10 == seq_sep_no && D_801D8EB0[i].vabId == vab) {
            if (D_801D8EB0[i].unk0 == 0xFF) {
                vmNoiseOff(i);
                count++;
            } else {
                D_801D96F8 = i;
                _SsVmKeyOffNow(0);
                count++;
            }
        }
    }
    return count;
}

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
