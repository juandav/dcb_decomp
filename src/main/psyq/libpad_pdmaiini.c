#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", PadStartCom);

extern u_char D_800779C0[];

void PadStopCom(void) {
    func_8006A804();
    func_8006A894(3, 1);
    func_8006AF94(2, D_800779C0);
    func_8006A814();
}
