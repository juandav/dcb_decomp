#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOn);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmKeyOff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVmSeKeyOn);

void _SsVmSeKeyOff(short seq_sep_no, short vab_no, u_short note) {
    _SsVmKeyOff(0x21, seq_sep_no, vab_no, note);
}

OBJECT_END(1);
