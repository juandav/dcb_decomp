#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void InitCARD(long val) {
    int ret;

    func_8006A884(0);
    VSync(0);
    ret = func_8006A804();
    if (ReadInitPadFlag() == 0) {
        val = 0;
    }
    func_80068994(val);
    _copy_memcard_patch();
    _patch_card();
    _patch_card2();
    _patch_card_info();
    if (ret == 1) {
        func_8006A814();
    }
}

long StartCARD(void) {
    int ret = func_8006A804();

    func_800689A4();
    func_8006A884(0);
    if (ret == 1) {
        func_8006A814();
    }
    return 0;
}

long StopCARD(void) {
    func_800689B4();
    _ExitCard();
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068994);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800689A4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800689B4);
