#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdInit);

int func_80057164(void) {
    if (CD_init() != 0) {
        return 0;
    }
    return CD_initvol() == 0;
}

void func_800571A0(void) {
    func_8006A784(0xF0000003, 0x20);
}

void func_800571C8(void) {
    func_8006A784(0xF0000003, 0x40);
}

void func_800571F0(void) {
    func_8006A784(0xF0000003, 0x40);
}

OBJECT_END(3);
