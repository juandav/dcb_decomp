#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern long D_80077928;

void SetInitPadFlag(int num) {
    D_80077928 = num;
}

long ReadInitPadFlag(void) {
    return D_80077928;
}

void PAD_init(char *bufA, long lenA, char *bufB, long lenB) {
    _remove_ChgclrPAD();
    func_8006A804();
    _patch_pad();
    func_8006A814();
    func_8006A884(0);
    func_8006AE30();
    func_8006AF74(bufA, lenA, bufB, lenB);
    D_80077928 = 1;
}

long InitPAD(char *bufA, long lenA, char *bufB, long lenB) {
    _remove_ChgclrPAD();
    func_8006A804();
    _patch_pad();
    func_8006A814();
    func_8006A884(0);
    func_8006AE30();
    func_8006AF54(bufA, lenA, bufB, lenB);
    D_80077928 = 1;
}

long StartPAD(void) {
    func_8006AF64();
    func_8006A884(0);
    EnablePAD();
    return 1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AE30);

int func_8006AEA8(void) {
    volatile int i, j, k;

    D_8007792C[5] = 0;
    i = 10;
    while (--i != -1) {
    }
    return 0;
}

extern long *D_80077930;

int func_8006AF10(void) {
    if ((D_80077930[1] & 1) == 0 || (D_80077930[0] & 1) == 0) {
        return 0;
    }
    return 1;
}

OBJECT_END(1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF54);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF64);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF74);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF84);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006AF94);
