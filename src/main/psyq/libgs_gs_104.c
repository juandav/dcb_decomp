#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

typedef struct {
    short offx;
    short offy;
} GsPosition;
extern GsPosition D_801DBD98;
extern int D_801DBE28;
extern int D_801DBE2C;
extern int D_801DBE30;
extern int D_801DBE34;
extern int D_801DBE38;

void GsInit3D(void) {
    D_801DBD98.offx = D_801DBE28 / 2;
    D_801DBD98.offy = D_801DBE2C / 2;
    GsSetDrawBuffOffset();
    D_801DBE38 = 10;
    D_801DBE34 = 0;
    D_801DBE30 = 0x3FFF;
}

OBJECT_END(3);

typedef struct {
    u_long vert_top;
    u_long n_vert;
    u_long normal_top;
    u_long n_normal;
    u_long primitive_top;
    u_long n_primitive;
    long scale;
} TmdObj;

void GsMapModelingData(u_long *p) {
    int i;
    int n;
    TmdObj *obj;

    if (*p & 1) {
        return;
    }
    *p++ |= 1;
    i = 0;
    n = *p++;
    if (n > 0) {
        obj = (TmdObj *)p;
        do {
            obj[i].vert_top += (u_long)p;
            obj[i].normal_top += (u_long)p;
            obj[i].primitive_top += (u_long)p;
            i++;
        } while (i < n);
    }
}

void GsSetProjection(long h) {
    SetGeomScreen(h);
}
