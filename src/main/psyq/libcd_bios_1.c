#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013538);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013548);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80058BE4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_sync);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_ready);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_cw);

int CD_vol(CdlATV *vol) {
    *D_80070F04 = 2;
    *D_80070F14 = vol->val0;
    *D_80070F08 = vol->val1;
    *D_80070F04 = 3;
    *D_80070F10 = vol->val2;
    *D_80070F14 = vol->val3;
    *D_80070F08 = 0x20;
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_flush);

extern volatile u_short *D_80070F18;
extern volatile u_char *D_80070F04;
extern volatile u_char *D_80070F08;
extern volatile u_char *D_80070F10;
extern volatile u_char *D_80070F14;

int CD_initvol(void) {
    CdlATV vol;

    if (D_80070F18[0xDC] == 0 && D_80070F18[0xDD] == 0) {
        D_80070F18[0xC0] = 0x3FFF;
        D_80070F18[0xC1] = 0x3FFF;
    }
    D_80070F18[0xD8] = 0x3FFF;
    D_80070F18[0xD9] = 0x3FFF;
    D_80070F18[0xD5] = 0xC001;
    vol.val0 = vol.val2 = 0x80;
    vol.val1 = vol.val3 = 0;
    *D_80070F04 = 2;
    *D_80070F14 = vol.val0;
    *D_80070F08 = vol.val1;
    *D_80070F04 = 3;
    *D_80070F10 = vol.val2;
    *D_80070F14 = vol.val3;
    *D_80070F08 = 0x20;
    return 0;
}

extern long D_80070C40;

void CD_initintr(void) {
    D_80070C44 = 0;
    D_80070C40 = 0;
    D_80070C50 = 0;
    D_80070C4C = 0;
    ResetCallback();
    InterruptCallback(2, func_8005A088);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_init);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_datasync);

extern int D_80070EE8;

void CD_set_test_parmnum(int num) {
    D_80070EE8 = num;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A088);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StRingStatus);
