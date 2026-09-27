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

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDrawMode);

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

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067BE8);

void DecDCTReset(int mode) {
    if (mode == 0) {
        ResetCallback();
    }
    func_80067FC4(mode);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTGetEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTPutEnv);

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
extern u_long D_800768A8[];
extern u_long D_8007692C[];

void func_80067FC4(int mode) {
    switch (mode) {
    case 0:
        *D_800769EC = 0x80000000;
        *D_800769C0 = 0;
        *D_800769CC = 0;
        *D_800769EC = 0x60000000;
        func_800680B4(D_800768A8, 32);
        func_800680B4(D_8007692C, 32);
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

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800680B4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068144);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800681D0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068264);
