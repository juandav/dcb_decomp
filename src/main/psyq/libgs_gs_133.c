#include "psyq.h"

extern u_long D_801DBE20;
extern GsCOORDINATE2 *D_801DBF00[];

void GsGetLw(GsCOORDINATE2 *coord, MATRIX *m) {
    GsCOORDINATE2 *co;
    int i;
    int j;
    int flg;

    co = coord;
    i = 0;
    j = 100;
    for (;; i++) {
        D_801DBF00[i] = co;
        if (co->super == NULL) {
            if (co->flg == D_801DBE20 || co->flg == 0) {
                co->workm = co->coord;
                flg = D_801DBE20;
                *m = co->workm;
                co->flg = flg;
            } else if (j == 100) {
                *m = D_801DBF00[0]->workm;
                i = 0;
            } else {
                i = j + 1;
                *m = D_801DBF00[i]->workm;
            }
            break;
        }
        if (co->flg == D_801DBE20) {
            *m = co->workm;
            break;
        }
        if (co->flg == 0) {
            j = i;
        }
        co = co->super;
    }
    for (; i > 0; i--) {
        GsMulCoord3(m, &D_801DBF00[i - 1]->coord);
        D_801DBF00[i - 1]->workm = *m;
        D_801DBF00[i - 1]->flg = D_801DBE20;
    }
}

OBJECT_END(2);

extern MATRIX GsWSMATRIX;

void GsGetLs(GsCOORDINATE2 *coord, MATRIX *m) {
    GsCOORDINATE2 *co;
    int i;
    int j;
    int flg;

    co = coord;
    i = 0;
    j = 100;
    for (;; i++) {
        D_801DBF00[i] = co;
        if (co->super == NULL) {
            if (co->flg == D_801DBE20 || co->flg == 0) {
                co->workm = co->coord;
                flg = D_801DBE20;
                *m = co->workm;
                co->flg = flg;
            } else if (j == 100) {
                *m = D_801DBF00[0]->workm;
                i = 0;
            } else {
                i = j + 1;
                *m = D_801DBF00[i]->workm;
            }
            break;
        }
        if (co->flg == D_801DBE20) {
            *m = co->workm;
            break;
        }
        if (co->flg == 0) {
            j = i;
        }
        co = co->super;
    }
    for (; i > 0; i--) {
        GsMulCoord3(m, &D_801DBF00[i - 1]->coord);
        D_801DBF00[i - 1]->workm = *m;
        D_801DBF00[i - 1]->flg = D_801DBE20;
    }
    GsMulCoord2(&GsWSMATRIX, m);
}

OBJECT_END(2);

void GsGetLws(GsCOORDINATE2 *coord, MATRIX *lw, MATRIX *ls) {
    GsCOORDINATE2 *co;
    int i;
    int j;
    int flg;

    co = coord;
    i = 0;
    j = 100;
    for (;; i++) {
        D_801DBF00[i] = co;
        if (co->super == NULL) {
            if (co->flg == D_801DBE20 || co->flg == 0) {
                co->workm = co->coord;
                flg = D_801DBE20;
                *lw = co->workm;
                co->flg = flg;
            } else if (j == 100) {
                *lw = D_801DBF00[0]->workm;
                i = 0;
            } else {
                i = j + 1;
                *lw = D_801DBF00[i]->workm;
            }
            break;
        }
        if (co->flg == D_801DBE20) {
            *lw = co->workm;
            break;
        }
        if (co->flg == 0) {
            j = i;
        }
        co = co->super;
    }
    for (; i > 0; i--) {
        GsMulCoord3(lw, &D_801DBF00[i - 1]->coord);
        D_801DBF00[i - 1]->workm = *lw;
        D_801DBF00[i - 1]->flg = D_801DBE20;
    }
    *ls = *lw;
    GsMulCoord2(&GsWSMATRIX, ls);
}

OBJECT_END(3);

typedef struct {
    SVECTOR *vertop;
    u_long vern;
    SVECTOR *nortop;
    u_long norn;
    u_long *primtop;
    u_long primn;
    u_long scale;
} TmdObj;

int printf(char *fmt, ...);

void GsLinkObject4(u_long tmd_base, GsDOBJ2 *objp, int n) {
    TmdObj *obj;
    u_long *p;
    u_long *top;
    u_long np;
    u_long i;
    long cnt = 0;
    u_long mode = 0;
    u_long flag = 0;
    u_long prevmode;
    u_long prevflag;

    obj = (TmdObj *)tmd_base + n;
    objp->tmd = (u_long *)obj;
    p = obj->primtop;
    np = obj->primn;
    top = p;
    for (i = 0; i < np; i++) {
        prevmode = mode;
        prevflag = flag;
        mode = *p >> 24;
        flag = *p >> 16;
        if ((u_char)prevmode != 0 && (mode != (u_char)prevmode || (u_char)flag != (u_char)prevflag)) {
            *(u_short *)top = cnt;
            cnt = 0;
            top = p;
        }
        switch (mode & 0xFD) {
        case 0x20:
            if (flag & 4) {
                p += 6;
            } else {
                p += 4;
            }
            break;
        case 0x25:
            p += 7;
            break;
        case 0x31:
            p += 6;
            break;
        case 0x29:
            p += 4;
            break;
        case 0x3C:
            p += 9;
            break;
        case 0x30:
            if (flag & 4) {
                p += 7;
            } else {
                p += 5;
            }
            break;
        case 0x28:
            if (flag & 4) {
                p += 8;
            } else {
                p += 5;
            }
            break;
        case 0x21:
            p += 4;
            break;
        case 0x24:
            p += 6;
            break;
        case 0x2C:
        case 0x2D:
            p += 8;
            break;
        case 0x3D:
            p += 11;
            break;
        case 0x35:
            p += 9;
            break;
        case 0x38:
            if (flag & 4) {
                p += 9;
            } else {
                p += 6;
            }
            break;
        case 0x39:
            p += 7;
            break;
        case 0x34:
            p += 7;
            break;
        default:
            printf("GPU CODE %02xH not assigned.\n", mode);
            break;
        }
        cnt++;
    }
    *(u_short *)top = cnt;
}

/* ASPSX padded the jump table of the object as well */
__asm__(".section .rodata\n\t.space 8\n");

OBJECT_END(3);

extern u_long D_801DBF9C;
extern u_long D_801DBF88;
extern u_long D_801DBF8C;
extern u_long D_801DBF84;
extern u_long D_801DBFA8;
extern u_long D_801DBFA4;
extern long D_801DBE28;
extern long D_801DBE2C;
extern long D_801DBE34;
extern PACKET *GsOUT_PACKET_P;
extern _GsFCALL GsFCALL4;

/* the object's string table was padded to 8 bytes before the jump table */
__asm__(".section .rodata\nD_800137FC:\n\t.asciz \"non supported code %x %x\\n\"\n\t.align 2\n\t.space 4\n\t.section .text\n");
extern char D_800137FC[];

void GsSortObject4(GsDOBJ2 *objp, GsOT *otp, int shift, u_long *scratch) {
    TmdObj *obj;
    u_long *op;
    u_long primn;
    SVECTOR *vp;
    SVECTOR *np;
    long div;
    long lmode;
    u_long code;

    if (objp->attribute & GsDOFF) {
        return;
    }
    D_801DBF9C = objp->attribute & 7;
    D_801DBF88 = (objp->attribute >> 3) & 3;
    D_801DBF8C = (objp->attribute >> 5) & 1;
    D_801DBF84 = (objp->attribute >> 6) & 1;
    D_801DBFA8 = (objp->attribute >> 9) & 7;
    D_801DBFA4 = (objp->attribute >> 30) & 1;
    if (D_801DBFA8 != 0) {
        scratch[1] = D_801DBE28;
        scratch[2] = D_801DBE2C;
        scratch[0] = D_801DBFA8;
        div = 1;
    } else {
        div = 0;
    }
    if (D_801DBF84 == 1) {
        lmode = 2;
    } else if ((D_801DBF8C == 0 && (D_801DBE34 & 1)) || (D_801DBF8C == 1 && (D_801DBF88 & 1))) {
        lmode = 1;
    } else {
        lmode = 0;
    }
    obj = (TmdObj *)objp->tmd;
    op = obj->primtop;
    primn = obj->primn;
    vp = obj->vertop;
    np = obj->nortop;
    while (primn != 0) {
        code = ((u_char *)op)[3] & 0xFD;
        switch (code) {
        case 0x20:
            if (((u_short *)op)[1] & 4) {
                GsOUT_PACKET_P = GsFCALL4.f3g[lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 6;
            } else {
                GsOUT_PACKET_P = GsFCALL4.f3[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 4;
            }
            break;
        case 0x24:
            GsOUT_PACKET_P = GsFCALL4.tf3[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 6;
            break;
        case 0x30:
            if (((u_short *)op)[1] & 4) {
                GsOUT_PACKET_P = GsFCALL4.g3g[lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 7;
            } else {
                GsOUT_PACKET_P = GsFCALL4.g3[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 5;
            }
            break;
        case 0x34:
            GsOUT_PACKET_P = GsFCALL4.tg3[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 7;
            break;
        case 0x28:
            if (((u_short *)op)[1] & 4) {
                GsOUT_PACKET_P = GsFCALL4.f4g[lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 8;
            } else {
                GsOUT_PACKET_P = GsFCALL4.f4[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 5;
            }
            break;
        case 0x2C:
            GsOUT_PACKET_P = GsFCALL4.tf4[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 8;
            break;
        case 0x38:
            if (((u_short *)op)[1] & 4) {
                GsOUT_PACKET_P = GsFCALL4.g4g[lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 9;
            } else {
                GsOUT_PACKET_P = GsFCALL4.g4[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
                primn -= *(u_short *)op;
                op += *(u_short *)op * 6;
            }
            break;
        case 0x3C:
            GsOUT_PACKET_P = GsFCALL4.tg4[div][lmode](op, vp, np, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 9;
            break;
        case 0x21:
            GsOUT_PACKET_P = GsFCALL4.nf3[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 4;
            break;
        case 0x29:
            GsOUT_PACKET_P = GsFCALL4.nf4[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 4;
            break;
        case 0x31:
            GsOUT_PACKET_P = GsFCALL4.ng3[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 6;
            break;
        case 0x25:
            GsOUT_PACKET_P = GsFCALL4.ntf3[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 7;
            break;
        case 0x35:
            GsOUT_PACKET_P = GsFCALL4.ntg3[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 9;
            break;
        case 0x39:
            GsOUT_PACKET_P = GsFCALL4.ng4[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 7;
            break;
        case 0x2D:
            GsOUT_PACKET_P = GsFCALL4.ntf4[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 8;
            break;
        case 0x3D:
            GsOUT_PACKET_P = GsFCALL4.ntg4[div](op, vp, GsOUT_PACKET_P, *(u_short *)op, shift, otp, scratch);
            primn -= *(u_short *)op;
            op += *(u_short *)op * 11;
            break;
        default:
            printf(D_800137FC, code, op);
            break;
        }
    }
}

/* ASPSX padded the jump table of the object as well */
__asm__(".section .rodata\n\t.space 8\n");
