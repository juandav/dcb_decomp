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
int func_8006B6F0();
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

extern long D_800779A0;
extern long D_800779A4;
extern long D_800779B4;
extern long D_800779B8[];
extern volatile SioRegs *D_80077948;
int func_8006C0CC(PadPort *p);
void _dirFailAuto(PadPort *p, long r);

void func_8006B5EC(long r) {
    PadPort *p;
    int done;

    do {
        p = &D_801DDCB0[D_800779A0];
        if (r != -9) {
            if (r == 0) {
                *(D_800779B8 + D_800779A0) = 0;
            } else {
                _dirFailAuto(p, r);
                func_8006B6E0(p);
            }
        }
        D_800779A4 = 0;
        D_80077948->ctrl = 0;
        D_800779A0++;
        done = D_800779B4 < D_800779A0 ? 1 : func_8006C0CC(&D_801DDCB0[D_800779A0]);
        r = 0xFFFF;
    } while (!done);
}

void func_8006B6E0(PadPort *port) {
    u_char cmd = port->cmd;

    port->cmd = 0;
    port->prevCmd = cmd;
}

int func_8006B6F0(PadPort *p) {
    int i = p->unk45 - 3;

    switch (p->cmd) {
    case 0:
        if (i < 6 && p->unk57[i] == 0) {
            return 0;
        }
        if (i < p->actLen) {
            return p->actTable[i];
        }
        return 0;
    case 0x4D:
        return i < p->len ? p->data[i] : 0xFF;
    }
    return i < p->len ? p->data[i] : 0;
}

extern long D_800779A8;

void func_8006B7AC(PadPort *p) {
    int i;
    int j;
    int n;
    int found;
    int power;
    u_char mask;
    u_char *align;
    u_char *act;

    bzero(p->unk57, 6);
    if (p->unkE6 != 0 && p->actTable != NULL) {
        n = p->actLen < 7 ? p->actLen : 6;
        for (i = 0; i < p->unkE9; i++) {
            found = 0;
            mask = p->unk4[i].unk2 ? 0xFF : 1;
            align = p->unk5D;
            act = p->actTable;
            for (j = 0; j < n; align++, j++, act++) {
                if (*align == i && (*act & mask)) {
                    found = 1;
                    break;
                }
            }
            if (found) {
                power = D_800779A8 + p->unk4[i].power;
                if (power < 0x3D) {
                    D_800779A8 = power;
                } else {
                    found = 0;
                }
            }
            if (found) {
                align = p->unk5D;
                act = p->unk57;
                for (j = 0; j < n; j++, act++) {
                    if (*align++ == i) {
                        *act = 1;
                    }
                }
            }
        }
    } else if ((p->unkE8 == 4 || p->unkE8 == 5 || p->unkE8 == 7) && p->unkE6 == 0 && p->actLen >= 2) {
        if ((p->actTable[0] & 0xC0) == 0x40 && (p->actTable[1] & 1) && D_800779A8 + 10 < 0x3D) {
            p->unk57[1] = 1;
            p->unk57[0] = 1;
            D_800779A8 += 10;
        }
    } else if (p->unkE8 == 3) {
        p->unk57[0] = 1;
    } else if (p->unkE6 == 0) {
        for (j = 0; j < 6; j++) {
            p->unk57[j] = 1;
        }
    }
}

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

int func_8006CADC(PadPort *p);

void func_8006BB58(PadPort *p) {
    int old;
    u_char st;
    int i;

    if ((*p->unk3C & 0xF0) == 0) {
        p->unk30[0] = 0xFF;
        p->unk30[1] = 0;
        p->unkE8 = 0;
        p->unk35 = 0;
        D_80077964(p);
        return;
    }
    old = p->unkE8;
    p->unkE8 = *(volatile u_char *)p->unk3C >> 4;
    if (p->unkE8 == 0xF) {
        p->unkE8 = old;
    } else {
        p->unk30[0] = 0;
        p->unk30[1] = p->unk3C[0];
        p->unk35 = p->unk44;
        for (i = 2; i < p->unk44; i++) {
            p->unk30[i] = p->unk3C[i];
        }
    }
    if ((p->unk3C[1] == 0 && (p->unk46 != 1 || p->unk14 != NULL) && p->unk50 == 0) || p->unkE8 != old) {
        D_80077964(p);
    }
    st = p->unk46;
    p->unk4A = 0;
    if (st == 0xFF) {
        return;
    }
    if (st != 0 && p->cmd == 0) {
        return;
    }
    if (st >= 2 && st < 0xFE && *p->unk3C != 0xF3) {
        D_80077964(p);
        return;
    }
    switch (p->unk46) {
    case 0:
        p->unk49 = 1;
        p->unk46++;
        break;
    case 1:
        p->unk47[0] = 0;
        p->unk46++;
        break;
    case 0xFE:
        p->unk46 = 0xFF;
        break;
    default:
        if (p->unk18 != NULL) {
            p->unk46 += p->unk18(p);
        } else {
            p->unk46 += func_8006CADC(p);
        }
        break;
    }
}

extern void (*D_80077964)(PadPort *p);

void _dirFailAuto(PadPort *p, long r) {
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
