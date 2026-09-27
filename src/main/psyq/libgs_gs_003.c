#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetDrawBuffClip);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSwapDispBuff);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsInitCoordinate2);

void GsSetLsMatrix(MATRIX *mp) {
    SetRotMatrix(mp);
    func_8005C444(mp);
}

OBJECT_END(1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetLightMatrix);
