#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetLightMode);

void GsSetAmbient(long r, long g, long b) {
    func_8005C464(r >> 4, g >> 4, b >> 4);
}

OBJECT_END(2);
