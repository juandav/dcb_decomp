#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern MATRIX GsLIGHTWSMATRIX;
void func_8006295C(MATRIX *m);
void func_800629C0(MATRIX *m);

int GsSetFlatLight(int id, GsF_LIGHT *lt) {
    MATRIX lm;
    MATRIX cm;
    long unused[10]; /* unused, but it is in the original stack frame */
    long r;
    u_char cr;
    u_char cg;
    u_char cb;

    cr = lt->r;
    cg = lt->g;
    cb = lt->b;
    lm = GsLIGHTWSMATRIX;
    func_800629C0(&cm);
    r = SquareRoot0(lt->vx * lt->vx + lt->vy * lt->vy + lt->vz * lt->vz);
    if (r == 0) {
        return -1;
    }
    switch (id) {
    case 0:
        lm.m[0][0] = -lt->vx * 4096 / r;
        lm.m[0][1] = -lt->vy * 4096 / r;
        lm.m[0][2] = -lt->vz * 4096 / r;
        cm.m[0][0] = (cr << 12) / 255;
        cm.m[1][0] = (cg << 12) / 255;
        cm.m[2][0] = (cb << 12) / 255;
        break;
    case 1:
        lm.m[1][0] = -lt->vx * 4096 / r;
        lm.m[1][1] = -lt->vy * 4096 / r;
        lm.m[1][2] = -lt->vz * 4096 / r;
        cm.m[0][1] = (cr << 12) / 255;
        cm.m[1][1] = (cg << 12) / 255;
        cm.m[2][1] = (cb << 12) / 255;
        break;
    case 2:
        lm.m[2][0] = -lt->vx * 4096 / r;
        lm.m[2][1] = -lt->vy * 4096 / r;
        lm.m[2][2] = -lt->vz * 4096 / r;
        cm.m[0][2] = (cr << 12) / 255;
        cm.m[1][2] = (cg << 12) / 255;
        cm.m[2][2] = (cb << 12) / 255;
        break;
    }
    GsLIGHTWSMATRIX = lm;
    func_8006295C(&cm);
    return 0;
}

extern MATRIX D_801DBE60;

void func_8006295C(MATRIX *m) {
    D_801DBE60 = *m;
    SetColorMatrix(m);
}

extern MATRIX D_801DBE60;

void func_800629C0(MATRIX *m) {
    *m = D_801DBE60;
}

OBJECT_END(2);
