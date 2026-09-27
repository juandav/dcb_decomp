#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

int func_8005B304(void) {
    return CD_getsector2() == 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CD_getsector2);

void func_8005B414(void) {
    CD_datasync();
}
