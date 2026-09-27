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

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006BF3C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8006C0CC);
