#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void SetDrawTPage(DR_TPAGE *p, int dfe, int dtd, int tpage) {
    setDrawTPage(p, dfe, dtd, tpage);
}

OBJECT_END(1);

int MargePrim(void *p0, void *p1) {
    int len = getlen(p0) + getlen(p1) + 1;

    if (len > 16) {
        return -1;
    }
    setlen(p0, len);
    *(u_long *)p1 = 0;
    return 0;
}

OBJECT_END(2);

void SetTexWindow(DR_TWIN *p, RECT *tw) {
    setlen(p, 2);
    if (tw != NULL) {
        p->code[0] = 0xE2000000 | (((tw->y & 0xFF) >> 3) << 15) | (((tw->x & 0xFF) >> 3) << 10)
                   | ((((-tw->h) & 0xFF) >> 3) << 5) | (((-tw->w) & 0xFF) >> 3);
    } else {
        p->code[0] = 0;
    }
    p->code[1] = 0;
}

void SetDrawStp(DR_STP *p, int pbw) {
    setlen(p, 2);
    p->code[0] = pbw ? 0xE6000001 : 0xE6000000;
    p->code[1] = 0;
}

OBJECT_END(2);

void SetDrawMode(DR_MODE *p, int dfe, int dtd, int tpage, RECT *tw) {
    setDrawMode(p, dfe, dtd, tpage, tw);
}

OBJECT_END(3);

extern u_long *D_801DD930;

int OpenTIM(u_long *addr) {
    D_801DD930 = addr;
    return 0;
}

TIM_IMAGE *ReadTIM(TIM_IMAGE *timimg) {
    int n = func_80067BE8(D_801DD930, timimg);

    if (n == -1) {
        return NULL;
    }
    D_801DD930 += n;
    return timimg;
}

int func_80067BE8(u_long *tim, TIM_IMAGE *timimg) {
    int size;
    int n;

    if (*tim++ != 0x10) {
        return -1;
    }
    timimg->mode = *tim++;
    if (GetGraphDebug() == 2) {
        printf("id  =%08x\n", 0x10);
    }
    if (GetGraphDebug() == 2) {
        printf("mode=%08x\n", timimg->mode);
    }
    if (GetGraphDebug() == 2) {
        printf("timaddr=%08x\n", tim);
    }
    if (timimg->mode & 8) {
        size = *tim >> 2;
        timimg->crect = (RECT *)(tim + 1);
        timimg->caddr = tim + 3;
        tim += size;
    } else {
        size = 0;
        timimg->crect = NULL;
        timimg->caddr = NULL;
    }
    n = (*tim >> 2) + 2;
    timimg->prect = (RECT *)(tim + 1);
    timimg->paddr = tim + 3;
    return size + n;
}

__asm__(".section .rodata\n\t.space 8\n\t.section .text\n");

OBJECT_END(1);

void DecDCTReset(int mode) {
    if (mode == 0) {
        ResetCallback();
    }
    func_80067FC4(mode);
}

/* libpress.h's DECDCTENV; that header clashes with the DecDCTout here */
typedef struct {
    u_char iq_y[64];
    u_char iq_c[64];
    short dct[64];
} DECDCTENV;

typedef struct DctIqTable {
    /* 0x00 */ u_long head;
    /* 0x04 */ u_char iq_y[64];
    /* 0x44 */ u_char iq_c[64];
} DctIqTable;

typedef struct DctTable {
    /* 0x00 */ u_long head;
    /* 0x04 */ short dct[64];
} DctTable;

extern DctIqTable D_800768A8;
extern DctTable D_8007692C;

DECDCTENV *DecDCTGetEnv(DECDCTENV *env) {
    u_long *src;
    u_long *dst;
    int i;

    dst = (u_long *)env->iq_y;
    src = (u_long *)D_800768A8.iq_y;
    i = 16;
    while (i--) {
        *dst++ = *src++;
    }
    dst = (u_long *)env->iq_c;
    src = (u_long *)D_800768A8.iq_c;
    i = 16;
    while (i--) {
        *dst++ = *src++;
    }
    dst = (u_long *)env->dct;
    src = (u_long *)D_8007692C.dct;
    i = 32;
    while (i--) {
        *dst++ = *src++;
    }
    return env;
}

void func_800680B4(u_long *adr, u_long size);

DECDCTENV *DecDCTPutEnv(DECDCTENV *env) {
    u_long *src;
    u_long *dst;
    int i;

    dst = (u_long *)D_800768A8.iq_y;
    src = (u_long *)env->iq_y;
    i = 16;
    while (i--) {
        *dst++ = *src++;
    }
    dst = (u_long *)D_800768A8.iq_c;
    src = (u_long *)env->iq_c;
    i = 16;
    while (i--) {
        *dst++ = *src++;
    }
    func_800680B4((u_long *)&D_800768A8, 32);
    func_800680B4((u_long *)&D_8007692C, 32);
    return env;
}

void DecDCTin(u_long *buf, int mode) {
    if (mode & 1) {
        *buf &= ~0x08000000;
    } else {
        *buf |= 0x08000000;
    }
    if (mode & 2) {
        *buf |= 0x02000000;
    } else {
        *buf &= ~0x02000000;
    }
    func_800680B4(buf, *(u_short *)buf);
}

void DecDCTout(void) {
    func_80068144();
}

u_long func_800682F8(void);

int DecDCTinSync(int mode) {
    if (mode != 0) {
        return (func_800682F8() >> 29) & 1;
    }
    return func_800681D0();
}

extern volatile u_long *D_800769CC;

int DecDCToutSync(int mode) {
    if (mode != 0) {
        return (*D_800769CC >> 24) & 1;
    }
    return func_80068264();
}

int DecDCTinCallback(void (*func)()) {
    return DMACallback(0, func);
}

int DecDCToutCallback(void (*func)()) {
    return DMACallback(1, func);
}

extern volatile u_long *D_800769C0;
extern volatile u_long *D_800769EC;

void func_80067FC4(int mode) {
    switch (mode) {
    case 0:
        *D_800769EC = 0x80000000;
        *D_800769C0 = 0;
        *D_800769CC = 0;
        *D_800769EC = 0x60000000;
        func_800680B4((u_long *)&D_800768A8, 32);
        func_800680B4((u_long *)&D_8007692C, 32);
        break;
    case 1:
        *D_800769EC = 0x80000000;
        *D_800769C0 = 0;
        *D_800769CC = 0;
        *D_800769CC;
        *D_800769EC = 0x60000000;
        break;
    default:
        printf("MDEC_rest:bad option(%d)\n", mode);
        break;
    }
}

extern volatile u_long *D_800769F0;
extern volatile u_long *D_800769B8;
extern volatile u_long *D_800769BC;
extern volatile u_long *D_800769C0;
extern volatile u_long *D_800769E8;
int func_800681D0(void);

void func_800680B4(u_long *addr, u_long size) {
    func_800681D0();
    *D_800769F0 |= 0x88;
    *D_800769B8 = (u_long)(addr + 1);
    *D_800769BC = ((size >> 5) << 16) | 0x20;
    *D_800769E8 = *addr;
    *D_800769C0 = 0x1000201;
}

extern volatile u_long *D_800769F0;
extern volatile u_long *D_800769C4;
extern volatile u_long *D_800769C8;
int func_80068264(void);

void func_80068144(u_long *addr, u_long size) {
    func_80068264();
    *D_800769F0 |= 0x88;
    *D_800769CC = 0;
    *D_800769C4 = (u_long)addr;
    *D_800769C8 = ((size >> 5) << 16) | 0x20;
    *D_800769CC = 0x1000200;
}

int func_800681D0(void) {
    volatile int cnt = 0x100000;

    while (*D_800769EC & 0x20000000) {
        if (--cnt == -1) {
            func_80068310("MDEC_in_sync");
            return -1;
        }
    }
    return 0;
}

int func_80068264(void) {
    volatile int cnt = 0x100000;

    while (*D_800769CC & 0x1000000) {
        if (--cnt == -1) {
            func_80068310("MDEC_out_sync");
            return -1;
        }
    }
    return 0;
}
