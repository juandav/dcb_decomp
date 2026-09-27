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

extern long D_800779B0;
extern long D_800779B4;
extern long D_80077998;
extern long D_801DDF30[2];
extern void (*D_80077960)(long);
extern long D_800779A8;
extern long D_800779A4;
extern long D_800779A0;
extern long D_800779D0;
int func_8006C0CC(PadPort *p);
extern PadPort *D_80077994;
void func_8006C400(PadPort *p);

int func_8006BF3C(void) {
    if (D_800779D8->ctrl & 2) {
        D_800779D8->ctrl = 0;
        return 0;
    }
    D_800779D0 = 1;
    if (D_800779B0 != 0 && D_801DDF30[0] < 150) {
        D_801DDF30[0]++;
    }
    if (D_800779B4 == 0 && D_801DDF30[1] < 150) {
        D_801DDF30[1]++;
    }
    if (D_80077998 != 0 && D_800779B0 <= D_800779B4) {
        D_800779A4 = 0;
        D_800779A0 = D_800779B0;
        if (func_8006C0CC(&D_80077994[D_800779B0]) == 0) {
            D_80077960(0xFFFF);
        }
        D_800779A8 = 0;
        while (D_800779A0 <= D_800779B4) {
            func_8006C400(&D_80077994[D_800779A0]);
        }
        D_800779D8->baud = 0x88;
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C0CC);
