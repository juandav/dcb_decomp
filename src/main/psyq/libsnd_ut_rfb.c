#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsUtSetReverbFeedback);

void func_80051C70(void) {
    SpuSetReverb(0);
}

void func_80051C90(void) {
    SpuSetReverb(1);
}
