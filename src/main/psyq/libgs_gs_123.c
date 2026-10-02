#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

extern MATRIX GsIDMATRIX;

void Gssub_make_matrix(MATRIX *m, short s, short c, char axis) {
    *m = GsIDMATRIX;
    switch (axis) {
    case 'x':
    case 'X':
        m->m[1][1] = c;
        m->m[2][2] = c;
        m->m[1][2] = -s;
        m->m[2][1] = s;
        break;
    case 'y':
    case 'Y':
        m->m[0][0] = c;
        m->m[2][2] = c;
        m->m[0][2] = s;
        m->m[2][0] = -s;
        break;
    case 'z':
    case 'Z':
        m->m[0][0] = c;
        m->m[1][1] = c;
        m->m[0][1] = -s;
        m->m[1][0] = s;
        break;
    }
}

/* ASPSX padded the jump table of the object as well */
__asm__(".section .rodata\n\t.space 4\n\t.section .text\n");

OBJECT_END(2);

void GsSetWorkBase(PACKET *outpacketp) {
    GsOUT_PACKET_P = outpacketp;
}

OBJECT_END(1);

PACKET *GsGetWorkBase(void) {
    return GsOUT_PACKET_P;
}

extern MATRIX GsWSMATRIX;
extern MATRIX D_801DBEE0;
extern MATRIX D_801DBE80;
void gte_rotate_z_matrix(MATRIX *m, long r);
void func_80063024(long *src, long *dst);

int GsSetRefView2(GsRVIEW2 *pv) {
    GsRVIEW2 rv;
    MATRIX m;
    MATRIX tm;
    MATRIX lw; /* unused, but it is in the original stack frame */
    VECTOR vec;
    long r;
    long t;
    long s;

    GsWSMATRIX = D_801DBEE0;
    gte_rotate_z_matrix(&GsWSMATRIX, -pv->rz);
    func_80063024((long *)pv, (long *)&rv);
    r = SquareRoot0((rv.vrx - rv.vpx) * (rv.vrx - rv.vpx) + (rv.vry - rv.vpy) * (rv.vry - rv.vpy) + (rv.vrz - rv.vpz) * (rv.vrz - rv.vpz));
    if (r == 0) {
        return 1;
    }
    t = rv.vpy - rv.vry;
    s = -((t << 12) / r);
    t = SquareRoot0((rv.vrx - rv.vpx) * (rv.vrx - rv.vpx) + (rv.vrz - rv.vpz) * (rv.vrz - rv.vpz));
    Gssub_make_matrix(&m, s, (t << 12) / r, 'x');
    MulMatrix(&GsWSMATRIX, &m);
    if (t != 0) {
        r = t;
        t = rv.vrx - rv.vpx;
        s = (t << 12) / r;
        t = rv.vrz - rv.vpz;
        Gssub_make_matrix(&m, -s, (t << 12) / r, 'y');
        MulMatrix(&GsWSMATRIX, &m);
    }
    vec.vx = -pv->vpx;
    vec.vy = -pv->vpy;
    vec.vz = -pv->vpz;
    ApplyMatrixLV(&GsWSMATRIX, &vec, (VECTOR *)GsWSMATRIX.t);
    if (pv->super != NULL) {
        GsGetLw(pv->super, &m);
        TransposeMatrix(&m, &tm);
        ApplyMatrixLV(&tm, (VECTOR *)m.t, &vec);
        tm.t[0] = -vec.vx;
        tm.t[1] = -vec.vy;
        tm.t[2] = -vec.vz;
        GsMulCoord2(&GsWSMATRIX, &tm);
        GsWSMATRIX = tm;
    }
    D_801DBE80 = GsWSMATRIX;
    return 0;
}
