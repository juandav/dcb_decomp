#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void bzero(void *p, int n);
extern u_char D_801DDE90[];
extern u_char D_801DDED8[];
extern void (*D_80077960)();
extern void (*D_80077964)(PadPort *p);
extern void (*D_80077968)();
extern void (*D_8007796C)();
extern PadPort *(*D_80077974)(int);
extern void (*D_80077984)();
extern int (*D_80077978)();
extern int (*D_8007797C)();
extern void (*D_80077980)();
extern PadPort *D_80077994;
void func_8006B5EC();
void func_8006B584(PadPort *p);
void func_8006B6F0();
void func_8006B7AC();
PadPort *func_8006BA28(int port);
void func_8006B6E0(PadPort *port);
int func_8006BA48();
int func_8006BEA4();
void func_8006BB58();

void _padInitDirPort(void) {
    bzero(D_801DDCB0, sizeof(D_801DDCB0));
    D_80077994 = D_801DDCB0;
    D_801DDCB0[0].unk3C = D_801DDE90;
    D_801DDCB0[0].unk40 = D_801DDED8;
    D_801DDCB0[1].unk3C = D_801DDE90 + 0x23;
    D_801DDCB0[1].unk40 = D_801DDED8 + 0x23;
    D_80077960 = func_8006B5EC;
    D_80077964 = func_8006B584;
    D_80077968 = func_8006B6F0;
    D_8007796C = func_8006B7AC;
    D_80077974 = func_8006BA28;
    D_80077984 = func_8006B6E0;
    D_80077978 = func_8006BA48;
    D_8007797C = func_8006BEA4;
    D_80077980 = func_8006BB58;
}

void func_8006B584(PadPort *p) {
    int i;
    u_char *q;

    if (p->unk49 != 0) {
        p->unk49 = 0;
        p->unk46 = 0;
        p->unkE6 = 0;
        p->unk14 = NULL;
        p->unk18 = NULL;
        p->unkE3 = 0;
        p->unkE4 = 0;
        p->unkE6 = 0;
        p->unkE9 = 0;
        p->unkEA = 0;
        p->unk0 = 0;
        p->unk4 = 0;
        p->unk8 = 0;
        q = p->unk5D;
        for (i = 0; i < 6; i++) {
            *q++ = 0xFF;
        }
    }
}

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

extern void (*D_80077964)(PadPort *p);
void func_8006D2F8(PadPort *p, int mode);
void func_8006CA58(PadPort *p);
int func_8006BA48(PadPort *p) {
    if (*p->unk3C == 0xF3) {
        if (p->unkE8 == 0) {
            func_8006D2F8(p, 0);
            return 0;
        }
        if (p->unk46 == 0xFF) {
            func_8006D2F8(p, 0);
            return 0;
        }
        if (p->unk49 == 2) {
            D_80077964(p);
        }
    }
    switch (p->unk46) {
    case 0:
        break;
    case 1:
        func_8006D2F8(p, 1);
        break;
    case 0xFE:
        func_8006D2F8(p, 0);
        break;
    case 0xFF:
        break;
    default:
        if (p->unk14 != NULL) {
            p->unk14(p);
        } else {
            func_8006CA58(p);
        }
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BB58);

extern void (*D_80077964)(PadPort *p);

void _dirFailAuto(PadPort *p) {
    p->unk4C++;
    switch (p->unk46) {
    case 0:
        break;
    case 1:
        if (p->unk4A < 11) {
            p->unk4A++;
            return;
        }
        p->unk49 = 2;
        p->unk46 = 0xFF;
        return;
    default:
        if (p->unk4A < 11) {
            p->unk4A++;
            return;
        }
        if (p->unk49 != 0) {
            D_80077964(p);
        }
        break;
    }
    if (*p->unk3C != 0xF3) {
        p->unk30[0] = 0xFF;
        p->unk30[1] = 0;
        p->unkE8 = 0;
        p->unk35 = 0;
    }
}

int func_8006BEA4(u_char *p) {
    if (*(u_short *)(p + 0xE6) == 0 || p[0x46] != 0xFF) {
        return 1;
    }
    return 0;
}

OBJECT_END(1);
