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

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetTexWindow);

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

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTin);

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

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80067FC4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800680B4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068144);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800681D0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068264);
