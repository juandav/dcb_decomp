#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _padInitDirPort);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B584);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B5EC);

void func_8006B6E0(PadPort *port) {
    u_char cmd = port->cmd;

    port->cmd = 0;
    port->prevCmd = cmd;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B6F0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006B7AC);

PadPort *func_8006BA28(int port) {
    PadPort *p = D_801DDCB0;

    if (port & 0xF0) {
        p = &D_801DDCB0[1];
    }
    return p;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BA48);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BB58);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _dirFailAuto);

int func_8006BEA4(u_char *p) {
    if (*(u_short *)(p + 0xE6) == 0 || p[0x46] != 0xFF) {
        return 1;
    }
    return 0;
}

OBJECT_END(1);
