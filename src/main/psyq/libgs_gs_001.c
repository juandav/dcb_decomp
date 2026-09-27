#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void GsInitGraph(u_short x, u_short y, u_short intmode, u_short dith, u_short vrammode) {
    func_80061958(x, y, intmode, dith, vrammode);
    gte_init();
    D_801DBE24 = 0;
    func_80061ADC(x, y);
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80061958);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsInitGraph2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80061ADC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSortClear);
