#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", TransposeMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotMatrix);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotMatrixYXZ);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTF3NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005D104);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RotAverage3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNF3);

extern u_char D_801DBFA4;
void func_8005D104(u_long *sz0, u_long *sz1, u_long *sz2);
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

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNG3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTF4L);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005DB44);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTF4NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNF4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTG4NL);

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsTMDdivTNG4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT3A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005E920);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT3);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT3A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005EDB0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyFT4A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005F2D4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT4);

INCLUDE_ASM("asm/main/nonmatchings/psyq", RCpolyGT4A);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005F8D0);
