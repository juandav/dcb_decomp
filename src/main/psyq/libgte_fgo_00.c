#include "psyq.h"

INCLUDE_ASM("main/nonmatchings/psyq", TransposeMatrix);

INCLUDE_ASM("main/nonmatchings/psyq", RotMatrix);

INCLUDE_ASM("main/nonmatchings/psyq", RotMatrixYXZ);

extern u_char D_801DBFA4;
void func_8005D104(u_long *sz0, u_long *sz1, u_long *sz2);
POLY_FT3 *RCpolyFT3A(POLY_FT3 *pk, DIVPOLYGON3 *divp, long n, CRVECTOR3 *cr);

u_long *GsTMDdivTF3NL(TMD_P_TF3 *op, SVECTOR *vp, SVECTOR *np, POLY_FT3 *pk, u_long n, u_long shift, GsOT *ot,
                      DIVPOLYGON3 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    u_long col;
    CRVECTOR3 *cr;
    long p;
    u_short otz;
    long flag;
    u_long i;

    col = 0x24808080;
    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        if (!(op->dummy & 2)) {
            if (RotAverageNclip3(&r0->v, &r1->v, &r2->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy, &p,
                                 (long *)&otz, &flag) > 0) {
                func_8005D104(&r0->sz, &r1->sz, &r2->sz);
                divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
                *(u_long *)&divp->rgbc = col;
                divp->rgbc.cd = op->cd | (D_801DBFA4 << 1);
                divp->clut = op->clut;
                divp->tpage = op->tpage;
                *(u_long *)r0->uv = *(u_long *)&op->tu0;
                *(u_long *)r1->uv = *(u_long *)&op->tu1;
                *(u_long *)r2->uv = *(u_long *)&op->tu2;
                pk = RCpolyFT3A(pk, divp, 0, cr);
            }
        } else {
            otz = RotAverage3(&r0->v, &r1->v, &r2->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy, &p,
                              &flag);
            func_8005D104(&r0->sz, &r1->sz, &r2->sz);
            divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
            *(u_long *)&divp->rgbc = col;
            divp->rgbc.cd = op->cd | (D_801DBFA4 << 1);
            divp->clut = op->clut;
            divp->tpage = op->tpage;
            *(u_long *)r0->uv = *(u_long *)&op->tu0;
            *(u_long *)r1->uv = *(u_long *)&op->tu1;
            *(u_long *)r2->uv = *(u_long *)&op->tu2;
            pk = RCpolyFT3A(pk, divp, 0, cr);
        }
    }
    return (u_long *)pk;
}

OBJECT_END(2);

INCLUDE_ASM("main/nonmatchings/psyq", func_8005D104);

INCLUDE_ASM("main/nonmatchings/psyq", RotAverage3);

u_long *GsTMDdivTNF3(TMD_P_TNF3 *op, SVECTOR *vp, POLY_FT3 *pk, u_long n, u_long shift, GsOT *ot, DIVPOLYGON3 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    CRVECTOR3 *cr;
    long p;
    u_short otz;
    long flag;
    u_long i;

    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        if (!(op->dummy & 2)) {
            if (RotAverageNclip3(&r0->v, &r1->v, &r2->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy, &p,
                                 (long *)&otz, &flag) > 0) {
                func_8005D104(&r0->sz, &r1->sz, &r2->sz);
                divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
                *(u_long *)&divp->rgbc = *(u_long *)&op->r0;
                divp->rgbc.cd = (D_801DBFA4 << 1) | (op->cd & 0xFE);
                divp->clut = op->clut;
                divp->tpage = op->tpage;
                *(u_long *)r0->uv = *(u_long *)&op->tu0;
                *(u_long *)r1->uv = *(u_long *)&op->tu1;
                *(u_long *)r2->uv = *(u_long *)&op->tu2;
                pk = RCpolyFT3A(pk, divp, 0, cr);
            }
        } else {
            otz = RotAverage3(&r0->v, &r1->v, &r2->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy, &p,
                              &flag);
            func_8005D104(&r0->sz, &r1->sz, &r2->sz);
            divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
            *(u_long *)&divp->rgbc = *(u_long *)&op->r0;
            divp->rgbc.cd = (D_801DBFA4 << 1) | (op->cd & 0xFE);
            divp->clut = op->clut;
            divp->tpage = op->tpage;
            *(u_long *)r0->uv = *(u_long *)&op->tu0;
            *(u_long *)r1->uv = *(u_long *)&op->tu1;
            *(u_long *)r2->uv = *(u_long *)&op->tu2;
            pk = RCpolyFT3A(pk, divp, 0, cr);
        }
    }
    return (u_long *)pk;
}

OBJECT_END(2);

POLY_GT3 *RCpolyGT3A(POLY_GT3 *pk, DIVPOLYGON3 *divp, long n, CRVECTOR3 *cr);

u_long *GsTMDdivTG3NL(TMD_P_TG3 *op, SVECTOR *vp, SVECTOR *np, POLY_GT3 *pk, u_long n, u_long shift, GsOT *ot,
                      DIVPOLYGON3 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    CRVECTOR3 *cr;
    long p;
    u_short otz;
    long flag;
    u_long col;
    u_long i;

    col = 0x34808080;
    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    cr = &divp->cr[0];
    cr->r0 = r0;
    cr->r1 = r1;
    cr->r2 = r2;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        if (RotAverageNclip3(&r0->v, &r1->v, &r2->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy, &p,
                             (long *)&otz, &flag) <= 0) {
            continue;
        }
        func_8005D104(&r0->sz, &r1->sz, &r2->sz);
        divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
        divp->rgbc.cd = op->cd | (D_801DBFA4 << 1);
        divp->clut = op->clut;
        divp->tpage = op->tpage;
        *(u_long *)&r0->c = col;
        *(u_long *)&r1->c = col;
        *(u_long *)&r2->c = col;
        *(u_long *)r0->uv = *(u_long *)&op->tu0;
        *(u_long *)r1->uv = *(u_long *)&op->tu1;
        *(u_long *)r2->uv = *(u_long *)&op->tu2;
        pk = RCpolyGT3A(pk, divp, 0, cr);
    }
    return (u_long *)pk;
}

OBJECT_END(1);

u_long *GsTMDdivTNG3(TMD_P_TNG3 *op, SVECTOR *vp, POLY_GT3 *pk, u_long n, u_long shift, GsOT *ot, DIVPOLYGON3 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    CRVECTOR3 *cr;
    long p;
    u_short otz;
    long flag;
    u_long i;

    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        if (RotAverageNclip3(&r0->v, &r1->v, &r2->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy, &p,
                             (long *)&otz, &flag) <= 0) {
            continue;
        }
        func_8005D104(&r0->sz, &r1->sz, &r2->sz);
        divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
        divp->rgbc.cd = (D_801DBFA4 << 1) | (op->cd & 0xFE);
        divp->clut = op->clut;
        divp->tpage = op->tpage;
        *(u_long *)&r0->c = *(u_long *)&op->r0;
        *(u_long *)&r1->c = *(u_long *)&op->r1;
        *(u_long *)&r2->c = *(u_long *)&op->r2;
        *(u_long *)r0->uv = *(u_long *)&op->tu0;
        *(u_long *)r1->uv = *(u_long *)&op->tu1;
        *(u_long *)r2->uv = *(u_long *)&op->tu2;
        pk = RCpolyGT3A(pk, divp, 0, cr);
    }
    return (u_long *)pk;
}

OBJECT_END(2);

void func_8005DB44(u_long *sz0, u_long *sz1, u_long *sz2, u_long *sz3);
void NormalColorCol(SVECTOR *n, CVECTOR *in, CVECTOR *out);
POLY_FT4 *RCpolyFT4A(POLY_FT4 *pk, DIVPOLYGON4 *divp, long n, CRVECTOR4 *cr);

u_long *GsTMDdivTF4L(TMD_P_TF4 *op, SVECTOR *vp, SVECTOR *np, POLY_FT4 *pk, u_long n, u_long shift, GsOT *ot,
                      DIVPOLYGON4 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    RVECTOR *r3;
    u_long col;
    CRVECTOR4 *cr;
    long p;
    u_short otz;
    long flag;
    u_long i;

    col = 0x2C808080;
    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    r3 = &divp->r3;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    divp->cr[0].r3 = r3;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        r3->v = vp[op->v3];
        if (RotAverageNclip4(&r0->v, &r1->v, &r2->v, &r3->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy,
                             (long *)&r3->sxy, &p, (long *)&otz, &flag) <= 0) {
            continue;
        }
        func_8005DB44(&r0->sz, &r1->sz, &r2->sz, &r3->sz);
        NormalColorCol(&np[op->n0], (CVECTOR *)&col, &divp->rgbc);
        divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
        divp->rgbc.cd = op->cd | (D_801DBFA4 << 1);
        divp->clut = op->clut;
        divp->tpage = op->tpage;
        *(u_long *)r0->uv = *(u_long *)&op->tu0;
        *(u_long *)r1->uv = *(u_long *)&op->tu1;
        *(u_long *)r2->uv = *(u_long *)&op->tu2;
        *(u_long *)r3->uv = *(u_long *)&op->tu3;
        pk = RCpolyFT4A(pk, divp, 0, cr);
    }
    return (u_long *)pk;
}

OBJECT_END(2);

INCLUDE_ASM("main/nonmatchings/psyq", func_8005DB44);

u_long *GsTMDdivTF4NL(TMD_P_TF4 *op, SVECTOR *vp, SVECTOR *np, POLY_FT4 *pk, u_long n, u_long shift, GsOT *ot,
                      DIVPOLYGON4 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    RVECTOR *r3;
    u_long col;
    CRVECTOR4 *cr;
    long p;
    u_short otz;
    long flag;
    u_long i;

    col = 0x2C808080;
    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    r3 = &divp->r3;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    divp->cr[0].r3 = r3;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        r3->v = vp[op->v3];
        if (RotAverageNclip4(&r0->v, &r1->v, &r2->v, &r3->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy,
                             (long *)&r3->sxy, &p, (long *)&otz, &flag) <= 0) {
            continue;
        }
        func_8005DB44(&r0->sz, &r1->sz, &r2->sz, &r3->sz);
        divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
        *(u_long *)&divp->rgbc = col;
        divp->rgbc.cd = op->cd | (D_801DBFA4 << 1);
        divp->clut = op->clut;
        divp->tpage = op->tpage;
        *(u_long *)r0->uv = *(u_long *)&op->tu0;
        *(u_long *)r1->uv = *(u_long *)&op->tu1;
        *(u_long *)r2->uv = *(u_long *)&op->tu2;
        *(u_long *)r3->uv = *(u_long *)&op->tu3;
        pk = RCpolyFT4A(pk, divp, 0, cr);
    }
    return (u_long *)pk;
}

OBJECT_END(1);

u_long *GsTMDdivTNF4(TMD_P_TNF4 *op, SVECTOR *vp, POLY_FT4 *pk, u_long n, u_long shift, GsOT *ot, DIVPOLYGON4 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    RVECTOR *r3;
    CRVECTOR4 *cr;
    long p;
    u_short otz;
    long flag;
    u_long i;

    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    r3 = &divp->r3;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    divp->cr[0].r3 = r3;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        r3->v = vp[op->v3];
        if (RotAverageNclip4(&r0->v, &r1->v, &r2->v, &r3->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy,
                             (long *)&r3->sxy, &p, (long *)&otz, &flag) <= 0) {
            continue;
        }
        func_8005DB44(&r0->sz, &r1->sz, &r2->sz, &r3->sz);
        divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
        *(u_long *)&divp->rgbc = *(u_long *)&op->r0;
        divp->rgbc.cd = (D_801DBFA4 << 1) | (op->cd & 0xFE);
        divp->clut = op->clut;
        divp->tpage = op->tpage;
        *(u_long *)r0->uv = *(u_long *)&op->tu0;
        *(u_long *)r1->uv = *(u_long *)&op->tu1;
        *(u_long *)r2->uv = *(u_long *)&op->tu2;
        *(u_long *)r3->uv = *(u_long *)&op->tu3;
        pk = RCpolyFT4A(pk, divp, 0, cr);
    }
    return (u_long *)pk;
}

POLY_GT4 *RCpolyGT4A(POLY_GT4 *pk, DIVPOLYGON4 *divp, long n, CRVECTOR4 *cr);

u_long *GsTMDdivTG4NL(TMD_P_TG4 *op, SVECTOR *vp, SVECTOR *np, POLY_GT4 *pk, u_long n, u_long shift, GsOT *ot,
                      DIVPOLYGON4 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    RVECTOR *r3;
    CRVECTOR4 *cr;
    long p;
    u_short otz;
    long flag;
    u_long col;
    u_long i;

    col = 0x3C808080;
    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    r3 = &divp->r3;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    divp->cr[0].r3 = r3;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        r3->v = vp[op->v3];
        if (RotAverageNclip4(&r0->v, &r1->v, &r2->v, &r3->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy,
                             (long *)&r3->sxy, &p, (long *)&otz, &flag) <= 0) {
            continue;
        }
        func_8005DB44(&r0->sz, &r1->sz, &r2->sz, &r3->sz);
        divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
        divp->rgbc.cd = op->cd | (D_801DBFA4 << 1);
        divp->clut = op->clut;
        divp->tpage = op->tpage;
        *(u_long *)&r0->c = col;
        *(u_long *)&r1->c = col;
        *(u_long *)&r2->c = col;
        *(u_long *)&r3->c = col;
        *(u_long *)r0->uv = *(u_long *)&op->tu0;
        *(u_long *)r1->uv = *(u_long *)&op->tu1;
        *(u_long *)r2->uv = *(u_long *)&op->tu2;
        *(u_long *)r3->uv = *(u_long *)&op->tu3;
        pk = RCpolyGT4A(pk, divp, 0, cr);
    }
    return (u_long *)pk;
}

OBJECT_END(3);

u_long *GsTMDdivTNG4(TMD_P_TNG4 *op, SVECTOR *vp, POLY_GT4 *pk, u_long n, u_long shift, GsOT *ot, DIVPOLYGON4 *divp) {
    RVECTOR *r0;
    RVECTOR *r1;
    RVECTOR *r2;
    RVECTOR *r3;
    CRVECTOR4 *cr;
    long p;
    u_short otz;
    long flag;
    u_long i;

    r0 = &divp->r0;
    r1 = &divp->r1;
    r2 = &divp->r2;
    r3 = &divp->r3;
    cr = &divp->cr[0];
    divp->cr[0].r0 = r0;
    divp->cr[0].r1 = r1;
    divp->cr[0].r2 = r2;
    divp->cr[0].r3 = r3;
    for (i = 0; i < n; op++, i++) {
        r0->v = vp[op->v0];
        r1->v = vp[op->v1];
        r2->v = vp[op->v2];
        r3->v = vp[op->v3];
        if (RotAverageNclip4(&r0->v, &r1->v, &r2->v, &r3->v, (long *)&r0->sxy, (long *)&r1->sxy, (long *)&r2->sxy,
                             (long *)&r3->sxy, &p, (long *)&otz, &flag) <= 0) {
            continue;
        }
        func_8005DB44(&r0->sz, &r1->sz, &r2->sz, &r3->sz);
        divp->ot = (u_long *)(ot->org + ((otz - ot->offset) >> shift));
        divp->rgbc.cd = (D_801DBFA4 << 1) | (op->cd & 0xFE);
        divp->clut = op->clut;
        divp->tpage = op->tpage;
        *(u_long *)&r0->c = *(u_long *)&op->r0;
        *(u_long *)&r1->c = *(u_long *)&op->r1;
        *(u_long *)&r2->c = *(u_long *)&op->r2;
        *(u_long *)&r3->c = *(u_long *)&op->r3;
        *(u_long *)r0->uv = *(u_long *)&op->tu0;
        *(u_long *)r1->uv = *(u_long *)&op->tu1;
        *(u_long *)r2->uv = *(u_long *)&op->tu2;
        *(u_long *)r3->uv = *(u_long *)&op->tu3;
        pk = RCpolyGT4A(pk, divp, 0, cr);
    }
    return (u_long *)pk;
}

OBJECT_END(3);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyFT3);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyFT3A);

INCLUDE_ASM("main/nonmatchings/psyq", func_8005E920);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyGT3);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyGT3A);

INCLUDE_ASM("main/nonmatchings/psyq", func_8005EDB0);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyFT4);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyFT4A);

INCLUDE_ASM("main/nonmatchings/psyq", func_8005F2D4);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyGT4);

INCLUDE_ASM("main/nonmatchings/psyq", RCpolyGT4A);

INCLUDE_ASM("main/nonmatchings/psyq", func_8005F8D0);
