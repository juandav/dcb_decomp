#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern DRAWENV D_801DBDA0;
extern RECT D_801DBE18;
extern short D_801DBD88[];
extern short D_801DBD8C[];

void GsSetDrawBuffClip(void) {
    int x = D_801DBE18.x + D_801DBD88[D_801DBE24]; int y = D_801DBE18.y + D_801DBD8C[D_801DBE24]; D_801DBDA0.clip.x = x; D_801DBDA0.clip.y = y; D_801DBDA0.clip.w = D_801DBE18.w; D_801DBDA0.clip.h = D_801DBE18.h;
    PutDrawEnv(&D_801DBDA0);
}

OBJECT_END(2);

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

extern MATRIX D_801DBE40;

void GsSetLightMatrix(MATRIX *mp) {
    MATRIX m;

    m = D_801DBE40;
    PushMatrix();
    MulMatrix(&m, mp);
    PopMatrix();
    SetLightMatrix(&m);
}

OBJECT_END(3);
