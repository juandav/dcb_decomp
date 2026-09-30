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

extern short D_801DBD88[2];
extern short D_801DBD8C[2];
extern DISPENV D_801DBE00;
extern long D_801DBE20;

void GsSwapDispBuff(void) {
    long cnt;

    D_801DBE00.disp.x = D_801DBD88[D_801DBE24];
    D_801DBE00.disp.y = D_801DBD8C[D_801DBE24];
    PutDispEnv(&D_801DBE00);
    SetDispMask(1);
    cnt = ++D_801DBE20;
    if (cnt == 0) {
        cnt = 1;
    }
    D_801DBE20 = cnt;
    D_801DBE24 = D_801DBE24 == 0;
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}

OBJECT_END(3);

extern MATRIX GsIDMATRIX;

void GsInitCoordinate2(GsCOORDINATE2 *super, GsCOORDINATE2 *base) {
    base->coord = GsIDMATRIX;
    base->super = super;
    base->flg = 0;
    if ((u_long)super > 1) {
        base->super->sub = base;
    }
}

void GsSetLsMatrix(MATRIX *mp) {
    SetRotMatrix(mp);
    SetTransMatrix(mp);
}

OBJECT_END(1);

extern MATRIX GsLIGHTWSMATRIX;

void GsSetLightMatrix(MATRIX *mp) {
    MATRIX m;

    m = GsLIGHTWSMATRIX;
    PushMatrix();
    MulMatrix(&m, mp);
    PopMatrix();
    SetLightMatrix(&m);
}

OBJECT_END(3);
