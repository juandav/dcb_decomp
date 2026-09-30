#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern volatile u_char *D_80070BE8;
extern volatile u_char *D_80070BF0;
extern volatile u_char *D_80070BF4;
extern volatile u_long *D_80070BF8;
extern volatile u_long *D_80070BFC;
extern volatile u_long *D_80070C08;
extern volatile u_long *D_80070C18;
extern long D_80070C30;
extern long D_801D98B0;
extern short D_801D98B4;
extern long D_801D98B8;
extern long StCdIntrFlag;
extern long D_801D98C0;
extern long D_801D98C4;
extern long D_801D98C8;
extern long D_801D98CC;
extern long D_801D98D0;
extern long D_801D98D8;
extern long D_801D98E0;
extern u_long D_801D98E4;
extern long D_801D98E8;
extern u_short *D_801D98EC;
extern StHEADER *D_801D98F0;
extern void (*D_801DBD0C)();
extern volatile u_short *D_801DBD10;
extern long D_801DBD50;
int func_8005A384(int mode, u_char *result);
/* called with a fourth argument it ignores */
void func_80058A10();
void func_80058A3C(int ch, u_long *madr, int blocks, int size, u_long chcr, u_char irq, int unused);
void data_ready_callback(void);

void StCdInterrupt(void) {
    volatile short status[4];
    CdlLOC loc;
    u_char result[8];
    long *dst;
    u_long chcr;
    long *src;
    u_int i;

    if (D_801D98CC == 1) {
        return;
    }
    if (D_801D98B8 != 0 && (*D_80070C08 & 0x01000000)) {
        StCdIntrFlag = 1;
        if (D_801D98E0 != 0) {
            D_801D98D0++;
        }
        D_80070C30 = 1;
        return;
    }
    if (func_8005A384(1, result) == CdlDiskError) {
        return;
    }
    status[1] = result[0];
    status[2] = result[1];
    if (status[1] & 4) {
        D_80070C30 = 3;
        return;
    }
    D_801DBD10 = (u_short *)&D_801D98F0[D_801D98D4];
    if (D_801DBD10[0] != 0) {
        if (D_801D98E0 != 0) {
            D_801D98D0++;
        }
        D_80070C30 = 4;
        return;
    }
    *D_80070BE8 = 0;
    *D_80070BF4 = 0;
    *D_80070BE8 = 0;
    *D_80070BF4 = 0x80;
    *D_80070BF8 = 0x20943;
    *D_80070BFC = 0x1323;
    i = 0;
    if (D_801DBD50 == 0) {
        for (; i < 4; i++) {
            ((u_char *)&loc)[i] = *D_80070BF0;
        }
        for (i = 0; i < 8; i++) {
            *D_80070BF0;
        }
    }
    chcr = 0x11000000;
    if (D_801D98E0 != 0) {
        func_80058A10((long *)D_801DBD10, (long *)(D_801D98E0 + (D_801D98D0 << 11)), 8, 0);
    } else {
        func_80058A3C(3, (u_long *)D_801DBD10, 0, 8, chcr, 0, 0);
    }
    while (*D_80070C18 & 0x01000000) {
    }
    ((StHEADER *)D_801DBD10)->loc = loc;
    *D_80070BF8 = 0x20843;
    *D_80070BFC = 0x1325;
    if (D_801D98E8 == 1 && D_801D98C4 != 0) {
        if (D_801D98C4 != D_801DBD10[4]) {
            D_801DBD10[0] = 0;
            if (D_801D98E0 != 0) {
                D_801D98D0++;
            }
            return;
        }
        D_801D98E8 = 0;
    }
    if (((StHEADER *)D_801DBD10)->id != 0x160 || ((((StHEADER *)D_801DBD10)->type >> 10) & 0x1F) != D_801D98C8) {
        if (D_801D98E0 != 0) {
            D_801D98D0 = 0;
        } else {
            D_801DBD10[0];
        }
        D_80070C30 = 5;
        D_801DBD10[0] = 0;
        return;
    }
    if (D_801D98B4 != D_801DBD10[2] || (D_801D98B0 != 0 && D_801D98B0 != D_801DBD10[4])) {
        D_801D98B0 = 0;
        D_801D98B4 = 0;
        init_ring_status(D_801D98D8, D_801D98D4 - D_801D98D8);
        D_801D98D4 = D_801D98D8;
        D_801DBD10[0] = 0;
        if (D_801D98E0 != 0) {
            D_801D98D0++;
        }
        D_80070C30 = 6;
        return;
    }
    if (D_801DBD10[2] == 0) {
        D_801D98B4 = 0;
        D_801D98B0 = D_801DBD10[4];
        if (D_801D98E4 != 0 && D_801D98B0 >= D_801D98E4) {
            D_801D98B0 = 0;
            D_801D98B4 = 0;
            init_ring_status(D_801D98D8, D_801D98D4 - D_801D98D8);
            D_801D98D4 = D_801D98D8;
            D_801DBD10[0] = 0;
            D_801D98E8 = 1;
            if (D_801DBD0C != NULL) {
                D_801DBD0C();
            }
            if (D_801D98E0 != 0) {
                D_801D98D0++;
            }
            D_80070C30 = 7;
            return;
        }
        if ((u_long)(D_801D98F4 - D_801D98D4 - 1) < D_801DBD10[3]) {
            if (D_801D98E4 == 0) {
                D_801DBD10[0] = 1;
                D_801D98E8 = 1;
                if (D_801DBD0C != NULL) {
                    D_801DBD0C();
                }
                if (D_801D98E0 != 0) {
                    D_801D98D0++;
                }
                D_80070C30 = 8;
                return;
            }
            if ((short)D_801D98F0->id != 0) {
                D_801DBD10[0] = 0;
                if (D_801D98E0 != 0) {
                    D_801D98D0++;
                }
                D_80070C30 = 9;
                return;
            }
            D_801DBD10[0] = 1;
            dst = (long *)D_801D98F0;
            src = (long *)D_801DBD10;
            D_801D98D4 = 0;
            for (i = 0; i < 8; i++) {
                *dst++ = *src++;
            }
            D_801DBD10 = (u_short *)D_801D98F0;
        }
        D_801D98D8 = D_801D98D4;
    }
    D_80070C30 = 10;
    D_801D98B4++;
    D_801D98EC = (u_short *)(&D_801D98F0[D_801D98F4] + D_801D98D4 * 0x3F);
    if (D_801D98B8 != 0) {
        chcr = 0x11000000;
        *D_80070BF8 = 0x20943;
        *D_80070BFC = 0x1323;
    } else {
        *D_80070BF8 = 0x21020843;
        chcr = 0x11400100;
    }
    if (D_801DBD10[3] - 1 == D_801DBD10[2]) {
        D_801D98CC = 1;
        if (D_801D98E0 != 0) {
            func_80058A10((long *)D_801D98EC, (long *)(D_801D98E0 + (D_801D98D0 << 11) + 0x20), 0x1F8, 1);
            D_801D98D0++;
        } else {
            func_80058A3C(3, (u_long *)D_801D98EC, 0, 0x1F8, chcr, 1, 0);
        }
        D_801D98B4 = 0;
        D_801D98B0 = 0;
        D_801D98C8 = D_801D98C0;
    } else {
        if (D_801D98E0 != 0) {
            func_80058A10((long *)D_801D98EC, (long *)(D_801D98E0 + (D_801D98D0 << 11) + 0x20), 0x1F8, 0);
            D_801D98D0++;
        } else {
            func_80058A3C(3, (u_long *)D_801D98EC, 0, 0x1F8, chcr, 0, 0);
        }
    }
    *D_80070BFC = 0x1325;
    D_801DBD10[0] = 3;
    D_801D98D4 += 1;
    if (D_801D98E0 != 0 && D_801D98CC != 0) {
        data_ready_callback();
    }
}

void func_80058A10(long *dst, long *src, u_long n) {
    u_long i = 0;

    if (n != 0) {
        do {
            *dst++ = *src++;
            i++;
        } while (i < n);
    }
}

INCLUDE_ASM("main/nonmatchings/psyq", func_80058A3C);
