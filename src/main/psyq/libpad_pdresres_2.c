#include "psyq.h"

extern long D_800779A4;
extern long (*D_800779E8[])(PadPort *);
extern void (*D_80077960)(long);
void func_8006D3A0(int wait);
long func_8006C990(void);

void func_8006C400(PadPort *p) {
    long r;

    r = D_800779E8[D_800779A4++](p);
    if (r >= 0) {
        if (D_800779A4 != 0) {
            if (D_800779A4 != 3 || *p->unk3C != 0x80) {
                func_8006D3A0(0x3C);
                if (func_8006C990() == 0) {
                    D_80077960(-3);
                }
            }
        }
        if (D_800779A4 >= 5) {
            D_800779A4--;
        }
    } else {
        D_80077960(r);
    }
}

extern int D_801DDF2C;
int func_8006D3C0(void);

int func_8006C4F0(PadPort *p, int data) {
    volatile SioRegs *sio;
    int rx;
    int baud;
    int id;

    if (data < 0) {
        rx = D_800779D8->data;
        p->unk44 = 0xFF;
        p->unk45 = 1;
        *p->unk40 = ~data;
        sio = D_800779D8;
        while (!(sio->stat & 1)) {
        }
        while (func_8006D3C0() == 0) {
        }
        D_800779D8->data = ~data;
    } else {
        baud = 0x88;
        id = *p->unk3C;
        if ((id >> 4) == 8 && p->unk44 >= 9) {
            baud = 0x22;
        }
        D_801DDF28 = 0x1AE;
        D_801DDF24 = *(volatile u_short *)0x1F801120;
        D_801DDF2C = *(volatile u_short *)0x1F801124;
        if (!(D_800779D8->stat & 2)) {
            volatile SioRegs *rxsio = D_800779D8;

            while (!(rxsio->stat & 2)) {
            }
        }
        rx = D_800779D8->data;
        D_800779D8->baud = baud;
        while (!(*D_800779D4 & 0x80)) {
            if (func_8006D3C0() != 0) {
                return -20;
            }
        }
        D_800779D8->data = data;
        if (baud == 0x22) {
            volatile u_long *irq = D_800779D4;
            volatile SioRegs *ctl = D_800779D8;

            *irq = ~0x80;
            ctl->ctrl |= 0x10;
        }
        p->unk45++;
        p->unk3C[p->unk44] = rx;
        p->unk44++;
    }
    return rx;
}

void func_8006D3A0(int wait);

int func_8006C714(PadPort *p, long data) {
    volatile SioRegs *sio;
    int baud;
    int id;
    int rx;
    int now;
    int pos;

    baud = 0x88;
    id = *p->unk3C;
    if ((id >> 4) == 8 && p->unk44 >= 9) {
        baud = 0x22;
    }
    sio = D_800779D8;
    do {
    } while (!(sio->stat & 2));
    func_8006D3A0(400);
    rx = D_800779D8->data;
    if (p->unk44 != 0 || (rx >> 4) != 8) {
        D_800779D8->baud = baud;
    } else {
        D_800779D8->baud = 0x22;
    }
    while (!(*D_800779D4 & 0x80)) {
        *(volatile u_short *)0x1F801124;
        now = *(volatile u_short *)0x1F801120;
        if (now < D_801DDF24) {
            if (*(volatile u_short *)0x1F801128 != 0) {
                now += *(volatile u_short *)0x1F801128;
            } else {
                now += 0x10000;
            }
        }
        if (*(volatile u_short *)0x1F801124 & 0x200) {
            if ((now - D_801DDF24) >= D_801DDF28) {
                return -2;
            }
        } else if (((now - D_801DDF24) >> 3) >= D_801DDF28) {
            return -2;
        }
    }
    if (p->unkE8 != 8 && D_800779A4 == 2) {
        func_8006D3A0(60);
        while (func_8006D3C0() == 0) {
        }
    }
    D_800779D8->data = data;
    if (D_800779A4 == 3 && rx == 0x80) {
        volatile u_long *irq = D_800779D4;
        volatile SioRegs *ctl = D_800779D8;

        *irq = ~0x80;
        ctl->ctrl |= 0x10;
    }
    pos = p->unk44;
    p->unk45++;
    if (pos != 0xFF) {
        p->unk3C[p->unk44] = rx;
    }
    p->unk44++;
    return rx;
}

int func_8006D3C0(void);

long func_8006C990(void) {
    volatile u_long *irq = D_800779D4;
    volatile SioRegs *sio = D_800779D8;

    *irq = ~0x80;
    if (sio->stat & 0x80) {
        do {
            if (func_8006D3C0() != 0) {
                return 0;
            }
        } while (D_800779D8->stat & 0x80);
    }
    D_800779D8->ctrl |= 0x10;
    return 1;
}

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

extern int (*D_8007797C)();
void func_8006CE58(PadPort *port);
int func_8006CF00(PadPort *p);

int func_8006CD84(PadPort *p, u_char *buf) {
    u_char *size;
    u_char *mem;

    if (buf == NULL || p->unk4 != NULL || D_8007797C() != 0) {
        return 0;
    }
    mem = (u_char *)((((long)buf + 3) >> 2) << 2);
    p->unk49 = 4;
    p->unk0 = (u_short *)mem;
    p->unk46 = 1;
    p->unk14 = func_8006CE58;
    p->unk18 = func_8006CF00;
    p->unk47[0] = 0;
    size = mem + ((p->unkE3 + 1) >> 1) * 4;
    p->unk4 = (PadActInfo *)size;
    p->unk8 = (long)(size + ((p->unkE9 * 5 + 3) & 0xFFC));
    return 1;
}

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

extern int D_800779E0;
extern int (*D_80077978)();
int func_8006C4F0(PadPort *p, int arg);

void func_8006D460(PadPort *p) {
    D_800779E0 = D_80077978(p);
    *p->unk3C = 0;
    func_8006C4F0(p, -2);
}

extern long D_800779A0;
extern long D_800779B0;
extern long D_8007799C;
extern void (*D_80077990)(void);
extern void (*D_8007798C)(void);
extern int D_800779E0;
extern int (*D_80077978)();
int func_8006C714(PadPort *p, long arg);

int func_8006D4A8(PadPort *p) {
    if (D_800779A0 == D_800779B0 && D_8007799C != 0) {
        D_80077990();
        D_8007798C();
    }
    if (D_800779E0 != 0) {
        D_80077978(p->unkC);
        D_80077978(p->unkC + 1);
    }
    if (p->cmd != 0) {
        return func_8006C714(p, p->cmd);
    }
    return func_8006C714(p, 0x42);
}

extern int D_800779E0;
extern int (*D_80077978)();
extern long D_800779AC;
extern int D_800779DC;
int func_8006C714(PadPort *p, long arg);

int func_8006D580(PadPort *p) {
    int r;

    if (D_800779E0 != 0) {
        D_80077978(p->unkC + 2);
        D_80077978(p->unkC + 3);
    }
    r = func_8006C714(p, p->cmd != 0 ? 0 : D_800779AC);
    if (r >= 0) {
        D_800779DC = (r & 0xF) * 2;
        if (D_800779DC == 0) {
            D_800779DC = 0x20;
        }
        r = 0;
    }
    return r;
}

int func_8006C714(PadPort *p, long arg);
extern long D_800779E4;
extern void (*D_8007796C)();
extern u_char (*D_80077968)();

int func_8006D62C(PadPort *p) {
    int r;
    int type;
    long flag;

    flag = 0;
    if (D_800779AC != 0) {
        type = *p->unk3C;
        if ((type >> 4) == 8) {
            flag = p->cmd == 0;
        }
    }
    D_800779E4 = flag;
    if (D_800779E4 == 0 && p->cmd == 0 && p->prevCmd == 0 && (p == p->unk10 || p->unk39 == 0) && *p->unk30 == 0) {
        D_8007796C(p);
    }
    r = func_8006C714(p, D_80077968(p, D_800779E4));
    if (r == 0x5A || r == 0 || r < 0) {
        return r;
    }
    return -4;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006D748);
