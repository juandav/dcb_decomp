#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetDrawBuffClip);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSwapDispBuff);

extern MATRIX D_801DBEC0;

void GsInitCoordinate2(GsCOORDINATE2 *super, GsCOORDINATE2 *base) {
    base->coord = D_801DBEC0;
    base->super = super;
    base->flg = 0;
    if ((u_long)super > 1) {
        base->super->sub = base;
    }
}

void GsSetLsMatrix(MATRIX *mp) {
    SetRotMatrix(mp);
    func_8005C444(mp);
}

OBJECT_END(1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSetLightMatrix);
