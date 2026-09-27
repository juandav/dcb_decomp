#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

int func_8006BED4(void) {
    if (!(D_800779D4[1] & 1)) {
        return 0;
    }
    if (!(D_800779D4[0] & 1)) {
        return 0;
    }
    if (D_80077988 != NULL) {
        D_80077988();
    }
    return 1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BF3C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C0CC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C400);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C4F0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C714);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C990);

void func_8006CA20(void) {
    while (!(D_800779D8->stat & 2)) {
    }
}

void func_8006CA48(PadPort *port, u_char cmd, u_char *data, u_char len) {
    port->cmd = cmd;
    port->data = data;
    port->len = len;
}

void func_8006CA58(PadPort *port) {
    switch (port->unk46) {
    case 2:
        func_8006D318(port);
        return;
    case 3:
        func_8006D32C(port, port->unkE4);
        return;
    case 4:
        func_8006D36C(port, port->unk47[0]);
        return;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006CADC);

int func_8006CD4C(u_char *p) {
    return (((p[0xE3] + 1) >> 1) << 2) + (u_short)(((p[0xE9] * 5 + 3) & ~3) + 4) + *(u_short *)(p + 0xEC);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006CD84);

void func_8006CE58(PadPort *port) {
    switch (port->unk46) {
    case 2:
        func_8006D32C(port, port->unk47[0]);
        return;
    case 3:
        func_8006D34C(port, port->unk47[0]);
        return;
    case 4:
        if (port->unk47[1] == 0) {
            func_8006D36C(port, port->unk47[0]);
            return;
        }
        func_8006D38C(port);
        return;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006CF00);

void func_8006D2F8(PadPort *port, u_char param) {
    port->cmd = 0x43;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D318(PadPort *port) {
    port->cmd = 0x45;
    port->data = NULL;
    port->len = 0;
}

void func_8006D32C(PadPort *port, u_char param) {
    port->cmd = 0x4C;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D34C(PadPort *port, u_char param) {
    port->cmd = 0x46;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D36C(PadPort *port, u_char param) {
    port->cmd = 0x47;
    port->data = &port->param;
    port->param = param;
    port->len = 1;
}

void func_8006D38C(PadPort *port) {
    port->cmd = 0x4B;
    port->data = NULL;
    port->len = 0;
}

void func_8006D3A0(int wait) {
    D_801DDF28 = wait;
    D_801DDF24 = *(volatile u_short *)0x1F801120;
}

int func_8006D3C0(void) {
    int t;

    t = *(volatile u_short *)0x1F801120;
    if (t < D_801DDF24) {
        if (*(volatile u_short *)0x1F801128 != 0) {
            t += *(volatile u_short *)0x1F801128;
        } else {
            t += 0x10000;
        }
    }
    if (*(volatile u_short *)0x1F801124 & 0x200) {
        return (t - D_801DDF24) >= D_801DDF28;
    }
    return ((t - D_801DDF24) >> 3) >= D_801DDF28;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D460);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D4A8);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D580);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D62C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D748);
