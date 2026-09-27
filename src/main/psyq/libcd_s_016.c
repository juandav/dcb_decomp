#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdControl);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdControlF);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdControlB);

int func_8005A784(CdlATV *vol) {
    CD_vol(vol);
    return 1;
}

int func_8005A7A4(void (*func)()) {
    return DMACallback(3, func);
}

OBJECT_END(3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A7D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A808);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005AA7C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005AB4C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdReadBreak);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdRead);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdReadSync);
