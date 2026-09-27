#include "common.h"
#include "gte.h"
#include "game.h"

void func_8002FAE4(void) {
    Fade *f;
    s32 tim;
    s16 r[4];
    u8 db;

    if (D_801D81F8.tim == 0 || *(u16 *)&D_801D81F8.mode == 0xFFFF) {
        return;
    }
    switch (D_801D81F8.unk6E) {
    case 0:
        if ((s8)D_801D81F8.unk6F < 30) {
            D_801D81F8.unk6F++;
        }
        break;
    case 1:
        if ((s8)D_801D81F8.unk6F >= -59) {
            D_801D81F8.unk6F--;
        }
        break;
    }
    f = &D_801D81F8;
    f->unk70 = (f->unk70 + (s8)f->unk6F) % 7680;
    if (f->mode != f->unk6D) {
        if (f->unk6D == -1) {
            if (f->unk72 == 0) {
                tim = func_8001BFCC(f->tim, f->mode);
                func_8001B438((u32 *)tim, f->x, f->y, f->w, f->h);
                if (f->mode != 6) {
                    f->unk7C = 0x40;
                } else {
                    D_801D81F8.unk7C = 0x80;
                }
                D_801D81F8.unk7E = 0x80;
                DrawSync(0);
                func_8001AE90((void *)tim);
            }
            D_801D81F8.unk72 += 6;
            if (D_801D81F8.unk72 > 0x80) {
                D_801D81F8.unk72 = 0x80;
                D_801D81F8.unk6D = D_801D81F8.mode;
            }
        } else {
            D_801D81F8.unk72 -= 6;
            if (D_801D81F8.unk72 < 0) {
                D_801D81F8.unk72 = 0;
                D_801D81F8.unk6D = -1;
            }
        }
    }
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].twin0);
    db = D_800794F4;
    D_801D81F8.buf[db].x0 = -((D_801D81F8.unk70 / 60) & 1);
    D_801D81F8.buf[D_800794F4].y0 = 0;
    D_801D81F8.buf[D_800794F4].u0 = (D_801D81F8.unk70 / 60) & 0xFE;
    D_801D81F8.buf[D_800794F4].v0 = D_801D81F8.unk70 / 60;
    D_801D81F8.buf[D_800794F4].r0 = D_801D81F8.unk72;
    D_801D81F8.buf[D_800794F4].g0 = D_801D81F8.unk72;
    D_801D81F8.buf[D_800794F4].b0 = D_801D81F8.unk72;
    r[0] = (D_801D81F8.x % 64) * 4;
    r[1] = D_801D81F8.y % 256;
    r[2] = D_801D81F8.unk7C;
    r[3] = D_801D81F8.unk7E;
    SetTexWindow(D_801D81F8.buf[D_800794F4].twin, r);
    addPrim(&D_800793A0->ot[0xFFF], &D_801D81F8.buf[D_800794F4]);
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].twin);
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].tpage);
}

void func_80030130(void *arg0) {
    s32 t;
    s32 sum;

    t = (*(s16 *)((s8 *)arg0 + 0x122)) * (*(s32 *)((s8 *)arg0 + 0x104)) * (*(s32 *)((s8 *)arg0 + 0x104));
    sum = (*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)) + t;
    (*(s32 *)((s8 *)arg0 + 0x20)) = (*(s16 *)((s8 *)arg0 + 0xD4)) + ((sum * (*(s32 *)((s8 *)arg0 + 0x9C))) >> 12);
    (*(s32 *)((s8 *)arg0 + 0x24)) = (*(s16 *)((s8 *)arg0 + 0xD6)) + ((sum * (*(s32 *)((s8 *)arg0 + 0xA0))) >> 12);
    (*(s32 *)((s8 *)arg0 + 0x28)) = (*(s16 *)((s8 *)arg0 + 0xD8)) + ((sum * (*(s32 *)((s8 *)arg0 + 0xA4))) >> 12);
}

void func_800301D0(void *arg0) {
    func_80030130(arg0);
    *(s32 *)((s8 *)arg0 + 0x24) +=
        -*(s16 *)((s8 *)arg0 + 0x120) * *(s32 *)((s8 *)arg0 + 0x100) +
        *(s16 *)((s8 *)arg0 + 0x120) * *(s32 *)((s8 *)arg0 + 0x100) *
            *(s32 *)((s8 *)arg0 + 0x100) / 56;
}

void func_80030264(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x20)));
}

void func_800302E0(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x24)));
}

void func_8003035C(u8 *p) {
    s32 h;
    s32 sx;
    s32 cz;

    func_80030130(p);
    h = -*(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) +
        *(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) * *(s32 *)(p + 0x100) / 56;
    sx = h * rsin(*(s16 *)(p + 0xE8)) >> 12;
    cz = h * rcos(*(s16 *)(p + 0xE8)) >> 12;
    *(s32 *)(p + 0x20) -= sx;
    *(s32 *)(p + 0x24) += cz;
}

void func_80030440(u8 *p) {
    s32 d;
    s32 h;
    s32 sx;
    s32 cz;

    d = *(s16 *)(p + 0x120) * *(s32 *)(p + 0x100);
    *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4) + (d * *(s32 *)(p + 0x9C) >> 12);
    *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6) + (d * *(s32 *)(p + 0xA0) >> 12);
    *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8) + (d * *(s32 *)(p + 0xA4) >> 12);
    h = -*(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) +
        *(s16 *)(p + 0x122) * *(s32 *)(p + 0x100) * *(s32 *)(p + 0x100) / 56;
    sx = h * rsin(*(s16 *)(p + 0xE8)) >> 12;
    cz = h * rcos(*(s16 *)(p + 0xE8)) >> 12;
    *(s32 *)(p + 0x20) -= sx;
    *(s32 *)(p + 0x24) += cz;
}

void func_8003058C(u8 *p) {
    s32 x;
    s32 y;
    s32 z;

    x = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    y = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    z = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4) + x;
    *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6) + y;
    *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8) + z;
}

s32 func_80030694(SVECTOR *a, SVECTOR *b) {
    VECTOR d;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    return SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz);
}

s32 func_80030718(SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    VECTOR ab;
    VECTOR cb;
    VECTOR ca;
    s32 d0;
    s32 d1;

    ab.vx = b->vx - a->vx;
    ab.vy = b->vy - a->vy;
    ab.vz = b->vz - a->vz;
    cb.vx = b->vx - c->vx;
    cb.vy = b->vy - c->vy;
    cb.vz = b->vz - c->vz;
    ca.vx = a->vx - c->vx;
    ca.vy = a->vy - c->vy;
    ca.vz = a->vz - c->vz;
    d0 = (cb.vx * ab.vx + cb.vy * ab.vy + cb.vz * ab.vz) >> 12;
    d1 = (ca.vx * ab.vx + ca.vy * ab.vy + ca.vz * ab.vz) >> 12;
    if ((d0 <= 0 && d1 >= 0) || (d0 >= 0 && d1 <= 0)) {
        return 1;
    }
    return 0;
}

void func_80030828(SVECTOR *a, SVECTOR *p, SVECTOR *b, VECTOR *out) {
    VECTOR d;
    VECTOR ap;
    VECTOR n;
    VECTOR proj;
    s32 t;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    if (SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz) < 20000) {
        VectorNormal(&d, &n);
    } else {
        d.vx >>= 4;
        d.vy >>= 4;
        d.vz >>= 4;
        VectorNormal(&d, &n);
    }
    ap.vx = p->vx - a->vx;
    ap.vy = p->vy - a->vy;
    ap.vz = p->vz - a->vz;
    t = (n.vx * ap.vx + n.vy * ap.vy + n.vz * ap.vz) >> 12;
    proj.vx = t * n.vx >> 12;
    proj.vy = t * n.vy >> 12;
    proj.vz = t * n.vz >> 12;
    out->vx = a->vx + proj.vx;
    out->vy = a->vy + proj.vy;
    out->vz = a->vz + proj.vz;
}

s32 func_800309F0(SVECTOR *arg0, SVECTOR *arg1, s32 arg2) {
    s32 v;

    v = func_80030694(arg0, arg1);
    if (-arg2 < v && v < arg2) {
        return 1;
    }
    return 0;
}

s32 func_80030A34(SVECTOR *a, SVECTOR *b, SVECTOR *c, s16 r) {
    SVECTOR sv;
    VECTOR v;

    if (func_80030718(a, b, c) != 0) {
        func_80030828(a, c, b, &v);
        sv.vx = v.vx;
        sv.vy = v.vy;
        sv.vz = v.vz;
        if (func_800309F0(c, &sv, r) != 0) {
            return 1;
        }
        return -1;
    }
    return 0;
}

Unk13C *func_80030AE4(Unk13C *src) {
    Unk13C *dst;

    dst = func_8001AD0C(0x13C);
    *dst = *src;
    func_80030E3C(dst);
    return dst;
}

void func_80030B6C(s32 arg0) {
    PushMatrix();
    func_80030F90(arg0, 0);
    PopMatrix();
}

void func_80030BA4(void *arg0) {
    func_8001AE90(arg0);
}

s32 func_80030BC4(SVECTOR *a, SVECTOR *b, VECTOR *out) {
    VECTOR d;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    if (SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz) < 20000) {
        VectorNormal(&d, out);
        return 1;
    }
    d.vx >>= 4;
    d.vy >>= 4;
    d.vz >>= 4;
    VectorNormal(&d, out);
    return -1;
}

void func_80030CA8(u8 *p) {
    s32 i;

    p[0x139] = *(s16 *)(p + 0x12E) >= 0x5B;
    *(s32 *)(p + 0x118) = -1;
    for (i = 0; i < 3; i++) {
        ((Unk80030CA8 *)p)->unk10C[i] = 0;
    }
    *(s32 *)(p + 0x108) = 0;
    *(s32 *)(p + 0x11C) = 0;
    *(s32 *)(p + 0x100) = 0;
    *(s32 *)(p + 0x104) = 0;
    if (*(s16 *)(p + 0x12E) != 0 && *(s16 *)(p + 0x12E) != 0x5A) {
        *(s32 *)(p + 0x38) = *(s32 *)(p + 0xAC);
        *(s32 *)(p + 0x3C) = *(s32 *)(p + 0xB0);
        *(s32 *)(p + 0x40) = *(s32 *)(p + 0xB4);
        *(s16 *)(p + 0x30) = *(s16 *)(p + 0xE4);
        *(s16 *)(p + 0x32) = *(s16 *)(p + 0xE6);
        *(s16 *)(p + 0x34) = *(s16 *)(p + 0xE8);
        *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4);
        *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6);
        *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8);
        *(s32 *)(p + 0x6C) = *(s16 *)(p + 0xDC);
        *(s32 *)(p + 0x70) = *(s16 *)(p + 0xDE);
        *(s32 *)(p + 0x74) = *(s16 *)(p + 0xE0);
        func_80030BC4((SVECTOR *)(p + 0xD4), (SVECTOR *)(p + 0xDC), (VECTOR *)(p + 0x9C));
    } else {
        *(s32 *)(p + 0xAC) = *(s32 *)(p + 0x38);
        *(s32 *)(p + 0xB0) = *(s32 *)(p + 0x3C);
        *(s32 *)(p + 0xB4) = *(s32 *)(p + 0x40);
        *(s16 *)(p + 0xE4) = *(s16 *)(p + 0x30);
        *(s16 *)(p + 0xE6) = *(s16 *)(p + 0x32);
        *(s16 *)(p + 0xE8) = *(s16 *)(p + 0x34);
        *(s16 *)(p + 0xD4) = *(s32 *)(p + 0x20);
        *(s16 *)(p + 0xD6) = *(s32 *)(p + 0x24);
        *(s16 *)(p + 0xD8) = *(s32 *)(p + 0x28);
    }
}

void *func_80030E3C(void *arg0) {
    func_8001EFDC(arg0, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xD4)), (s32) (*(s16 *)((s8 *)arg0 + 0xD6)), (s32) (*(s16 *)((s8 *)arg0 + 0xD8)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE4)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE6)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE8)));
    func_8001EFDC(arg0 + 0x4C, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xDC)), (s32) (*(s16 *)((s8 *)arg0 + 0xDE)), (s32) (*(s16 *)((s8 *)arg0 + 0xE0)), 0, 0, 0);
    (*(s32 *)((s8 *)arg0 + 0x14)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x18)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x1C)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x38)) = (s32) (*(s32 *)((s8 *)arg0 + 0xAC));
    (*(s32 *)((s8 *)arg0 + 0x3C)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB0));
    (*(s32 *)((s8 *)arg0 + 0x40)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB4));
    (*(u16 *)((s8 *)arg0 + 0x30)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE4));
    (*(u16 *)((s8 *)arg0 + 0x32)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE6));
    (*(u16 *)((s8 *)arg0 + 0x34)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE8));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD4));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD6));
    (*(s32 *)((s8 *)arg0 + 0x28)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD8));
    func_80030CA8(arg0);
    (*(s32 *)((s8 *)arg0 + 0x6C)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDC));
    (*(s32 *)((s8 *)arg0 + 0x70)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDE));
    (*(s32 *)((s8 *)arg0 + 0x74)) = (s32) (*(s16 *)((s8 *)arg0 + 0xE0));
    func_80030BC4(arg0 + 0xD4, arg0 + 0xDC, arg0 + 0x9C);
    (*(s32 *)((s8 *)arg0 + 0xFC)) = 0;
    (*(s16 *)((s8 *)arg0 + 0x132)) = 0;
    (*(s8 *)((s8 *)arg0 + 0x138)) = 0;
    return arg0;
}

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", D_80010864);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", D_80010874);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", D_80010884);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", D_80010894);

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", D_800108A4);

s32 func_80030F90(s32 arg, s32 flag) {
    Anim *o = (Anim *)arg;
    u8 f = flag;
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    s32 r;

    if (o->mode != 10 && o->mode < 90) {
        if (o->sx != o->sxT) {
            o->sx = o->dsx * o->t + o->sx0;
            if (o->dsx >= 0) {
                if (o->sx >= o->sxT) {
                    o->sx = o->sxT;
                    o->doneX = 1;
                }
            } else if (o->sx <= o->sxT) {
                o->sx = o->sxT;
                o->doneX = 1;
            }
        } else {
            o->doneX = 1;
        }
        if (o->sy != o->syT) {
            o->sy = o->dsy * o->t + o->sy0;
            if (o->dsy >= 0) {
                if (o->sy >= o->syT) {
                    o->sy = o->syT;
                    o->doneY = 1;
                }
            } else if (o->sy <= o->syT) {
                o->sy = o->syT;
                o->doneY = 1;
            }
        } else {
            o->doneY = 1;
        }
        if (o->sz != o->szT) {
            o->sz = o->dsz * o->t + o->sz0;
            if (o->dsz >= 0) {
                if (o->sz >= o->szT) {
                    o->sz = o->szT;
                    o->doneZ = 1;
                }
            } else if (o->sz <= o->szT) {
                o->sz = o->szT;
                o->doneZ = 1;
            }
        } else {
            o->doneZ = 1;
        }
        o->rotX = o->rx0 + o->drx * o->t + o->ddrx * o->t2 * o->t2 / 64;
        o->rotY = o->ry0 + o->dry * o->t + o->ddry * o->t2 * o->t2 / 64;
        o->rotZ = o->rz0 + o->drz * o->t + o->ddrz * o->t2 * o->t2 / 64;
    }
    func_8001F01C(o, &v0);
    switch (o->mode) {
    case 6:
        o->state = -1;
    case 0:
    case 47:
    case 48:
    case 49:
    case 56:
    case 69:
    case 82:
    case 90:
        o->posX = o->px;
        o->posY = o->py;
        o->posZ = o->pz;
        break;
    case 1:
    case 11:
    case 12:
    case 13:
    case 29:
    case 30:
    case 31:
    case 50:
    case 57:
    case 63:
    case 70:
    case 76:
    case 83:
        func_80030130(o);
        break;
    case 2:
    case 14:
    case 15:
    case 16:
    case 32:
    case 33:
    case 34:
    case 51:
    case 58:
    case 64:
    case 71:
    case 77:
    case 84:
        func_800301D0(o);
        break;
    case 3:
    case 17:
    case 18:
    case 19:
    case 35:
    case 36:
    case 37:
    case 52:
    case 59:
    case 65:
    case 72:
    case 78:
    case 85:
        func_80030264(o);
        break;
    case 4:
    case 20:
    case 21:
    case 22:
    case 38:
    case 39:
    case 40:
    case 53:
    case 60:
    case 66:
    case 73:
    case 79:
    case 86:
        func_800302E0(o);
        break;
    case 5:
    case 23:
    case 24:
    case 25:
    case 41:
    case 42:
    case 43:
    case 54:
    case 61:
    case 67:
    case 74:
    case 80:
    case 87:
        func_8003035C((u8 *)o);
        break;
    case 7:
    case 26:
    case 27:
    case 28:
    case 44:
    case 45:
    case 46:
    case 55:
    case 62:
    case 68:
    case 75:
    case 81:
    case 88:
        func_80030440((u8 *)o);
        break;
    case 8:
    case 89:
        func_8003058C((u8 *)o);
        break;
    }
    if (o->state != -1 && o->mode != 0 && o->mode < 90) {
        func_8001EEA0(o, f);
        func_8001EEA0((u8 *)o + 0x4C, 0);
        func_8001F01C(o, &v1);
        func_8001F01C((u8 *)o + 0x4C, &v2);
        r = func_80030A34(&v0, &v1, &v2, o->unk12C);
        if (r == 1) {
            switch (o->mode) {
            case 11:
            case 14:
            case 17:
            case 20:
            case 23:
            case 26:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                break;
            case 12:
            case 15:
            case 18:
            case 21:
            case 24:
            case 27:
                o->mode = 0;
                o->posX = o->px2;
                o->posY = o->py2;
                o->posZ = o->pz2;
                o->px = o->px2;
                o->py = o->py2;
                o->pz = o->pz2;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
                break;
            case 13:
            case 16:
            case 19:
            case 22:
            case 25:
            case 28:
                func_80030CA8((u8 *)o);
                break;
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                o->unk139 = 1;
                break;
            case 76:
            case 77:
            case 78:
            case 79:
            case 80:
            case 81:
                o->mode = 0;
                o->posX = o->px2;
                o->posY = o->py2;
                o->posZ = o->pz2;
                o->px = o->px2;
                o->py = o->py2;
                o->pz = o->pz2;
            case 63:
            case 64:
            case 65:
            case 66:
            case 67:
            case 68:
                o->unk137 = 2;
                o->speed = -abs(o->speed);
                break;
            }
            o->state = 1;
        } else if (r == -1) {
            o->state = 2;
        }
    }
    if (o->mode < 90 && o->mode != 0 && o->state == -1) {
        o->state = 0;
    }
    if (o->flag != 0) {
        switch (o->mode) {
    case 31:
    case 34:
    case 37:
    case 40:
    case 43:
    case 46:
    case 49:
            func_80030CA8((u8 *)o);
            break;
        }
    }
    if (o->mode < 90) {
        o->t++;
        o->t2++;
        if (o->period != 0 && o->flag == 0 && o->period < o->cnt++) {
            o->cnt = o->period;
            switch (o->mode) {
            case 29:
            case 32:
            case 35:
            case 38:
            case 41:
            case 44:
            case 47:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                break;
            case 30:
            case 33:
            case 36:
            case 39:
            case 42:
            case 45:
            case 48:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
                break;
            case 31:
            case 34:
            case 37:
            case 40:
            case 43:
            case 46:
            case 49:
                func_80030CA8((u8 *)o);
                break;
            case 56:
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
                o->unk139 = 1;
                break;
            case 82:
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
            case 89:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
            case 69:
            case 70:
            case 71:
            case 72:
            case 73:
            case 74:
            case 75:
                o->unk137 = 2;
                o->speed = -abs(o->speed);
                break;
            }
            o->flag = 1;
        }
    }
    func_8001EEA0(o, f);
}

void func_80031754(void *arg0) {
    if ((*(s16 *)((s8 *)arg0 + 0x12E)) >= 0x5B) {
        if ((*(s16 *)((s8 *)arg0 + 0x128)) > (*(s32 *)((s8 *)arg0 + 0x100))) {
            (*(s32 *)((s8 *)arg0 + 0x100)) += 1;
            return;
        }
        (*(s32 *)((s8 *)arg0 + 0x100)) = 0;
        (*(s8 *)((s8 *)arg0 + 0x139)) = 0;
        (*(s16 *)((s8 *)arg0 + 0x12E)) = (s16) ((u16) (*(s16 *)((s8 *)arg0 + 0x12E)) - 0x64);
    }
}

s16 func_800317A8(void *arg0, s16 arg1) {
    u8 *o;

    o = arg0;
    switch (o[0x137]) {
    case 1:
        arg1 = (*(s32 *)(o + 0x38)) / 16;
        if ((*(s32 *)(o + 0x38)) > 0x1000) {
            arg1 = 0x100 - ((*(s32 *)(o + 0x38)) - 0x1000) / 16;
        }
        break;
    case 2:
        arg1 += (*(u16 *)(o + 0x130));
        break;
    case 3:
        if (o[0x138] == 2) {
            break;
        }
        if (o[0x138] == 0) {
            arg1 += (*(u16 *)(o + 0x130));
            if (arg1 > 0x100) {
                arg1 = 0x100;
                o[0x138] = 1;
            }
        } else {
            arg1 -= (*(u16 *)(o + 0x130));
            if (arg1 < 0) {
                arg1 = 0;
                o[0x138] = 2;
            }
        }
        break;
    case 4:
        if (o[0x138] == 0) {
            arg1 += (*(u16 *)(o + 0x130));
            if (arg1 > 0x100) {
                arg1 = 0x100;
                o[0x138] = 1;
            }
        } else {
            arg1 -= (*(u16 *)(o + 0x130));
            if (arg1 < 0) {
                arg1 = 0;
                o[0x138] = 0;
            }
        }
        break;
    case 5:
        o[0x138] += (*(u16 *)(o + 0x130));
        if ((s8)o[0x138] >= 0) {
            arg1 = o[0x138] + 0x80;
        } else {
            arg1 = 0xFF - (o[0x138] & 0x7F);
        }
        break;
    }
    if (arg1 < 0) {
        arg1 = 0;
    }
    if (arg1 > 0x100) {
        arg1 = 0x100;
    }
    if ((*(s16 *)(o + 0x12E)) == 0xA) {
        arg1 = (*(s16 *)(o + 0x132));
    } else {
        (*(s16 *)(o + 0x132)) = arg1;
    }
    return arg1;
}

void func_80031970(Obj32 *o) {
    SVECTOR *v;
    s32 i;
    s16 x;
    s16 y;

    v = o->unk16C;
    for (i = 0; i < o->n; i++) {
        x = rsin((i << 12) / o->n) * o->unk19C[0] / 4096;
        y = rcos((i << 12) / o->n) * o->unk19C[0] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3];
        v++;
        x = rsin(((i + 1) << 12) / o->n) * o->unk19C[0] / 4096;
        y = rcos(((i + 1) << 12) / o->n) * o->unk19C[0] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3];
        v++;
        x = rsin((i << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        y = rcos((i << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3] + (o->unk19C[4] - o->unk19C[3]) * o->unk19C[2] / 100;
        v++;
        x = rsin(((i + 1) << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        y = rcos(((i + 1) << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3] + (o->unk19C[4] - o->unk19C[3]) * o->unk19C[2] / 100;
        v++;
        x = rsin((i << 12) / o->n) * o->unk19C[1] / 4096;
        y = rcos((i << 12) / o->n) * o->unk19C[1] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[4];
        v++;
        x = rsin(((i + 1) << 12) / o->n) * o->unk19C[1] / 4096;
        y = rcos(((i + 1) << 12) / o->n) * o->unk19C[1] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[4];
        v++;
    }
}

Obj32 *func_80031F58(s16 id, Bytes4 *a, Bytes4 *b, Bytes4 *c, Unk13C *src, s32 n, u8 abr, u8 tp, s32 type,
                     s16 p0, s16 p1, s16 p2, s16 p3, s16 p4, Bytes8 *q, s32 r, s32 s, s32 t, u8 u1, u8 u2,
                     s32 w, s32 x) {
    Obj32 *o;
    u8 *prim;
    s32 i;
    s32 j;

    o = func_8001AD0C(0x1B0);
    o->type = type;
    o->n = n;
    o->unk19C[0] = p0;
    o->unk19C[1] = p1;
    o->unk19C[2] = p2;
    o->unk19C[3] = p3;
    o->unk19C[4] = p4;
    o->unk16C = func_8001AD0C(n * 48);
    func_80031970(o);
    if (type == 13) {
        o->unk170 = *q;
        o->unk178 = r;
        o->unk17C = s;
        if (t >= 0 && func_801E6C78(t, 1, o, o->unk13C, x) != 0) {
            o->unk1AD = 1;
        } else {
            o->unk1AD = -1;
        }
    } else {
        o->unk1AD = -1;
    }
    o->unk198 = tp;
    o->unk1A6 = id;
    o->unk1A8 = -1;
    o->unk185 = *a;
    o->unk189 = *b;
    o->unk18D = *c;
    *(Unk13C *)o = *src;
    func_80030E3C(o);
    o->unk1AB = u2;
    o->unk194 = w;
    o->unk1AA = u1;
    o->unk1AC = abr;
    for (i = 0; i < 2; i++) {
        if (o->type < 10) {
            o->unk15C[i] = func_8001AD0C(o->n * 16);
        } else {
            o->unk15C[i] = 0;
        }
        prim = o->unk164[i] = func_8001AD0C(D_8006DEF4[o->type] * o->n * 2);
        for (j = 0; j < o->n * 2; j++) {
            func_8001E6EC(o->type, prim, abr, 0);
            if (o->type < 10) {
                SetDrawTPage(o->unk15C[i] + j * 8, 0, 0, GetTPage(0, tp, 0, 0));
            }
            prim += D_8006DEF4[o->type];
        }
    }
    return o;
}

void func_8003230C(Obj32 *o) {
    u8 c0[8];
    u8 c1[8];
    u8 c2[8];
    SVECTOR *v;
    s32 i;

    if (o->unk0[0x139] != 0) {
        func_80031754(o);
        return;
    }
    PushMatrix();
    func_80030F90((s32)o, o->unk1AA);
    o->unk1A6 = func_800317A8(o, o->unk1A6);
    if (o->unk1A6 == 0) {
        PopMatrix();
        return;
    }
    v = o->unk16C;
    if (o->unk1A6 != o->unk1A8) {
        c0[0] = o->unk185.b[0] * o->unk1A6 / 256;
        c0[1] = o->unk185.b[1] * o->unk1A6 / 256;
        c0[2] = o->unk185.b[2] * o->unk1A6 / 256;
        c1[0] = o->unk189.b[0] * o->unk1A6 / 256;
        c1[1] = o->unk189.b[1] * o->unk1A6 / 256;
        c1[2] = o->unk189.b[2] * o->unk1A6 / 256;
        c2[0] = o->unk18D.b[0] * o->unk1A6 / 256;
        c2[1] = o->unk18D.b[1] * o->unk1A6 / 256;
        c2[2] = o->unk18D.b[2] * o->unk1A6 / 256;
    }
    switch (o->type) {
    case 9: {
        u8 *p;
        u8 *q;
        u8 *tp;

        p = o->unk164[D_800794F4];
        q = o->unk164[D_800794F4 ^ 1];
        tp = o->unk15C[D_800794F4];
        for (i = 0; i < o->n; i++) {
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c0[0], c0[1], c0[2]);
                func_8001E76C(p, c0[0], c0[1], c0[2]);
                func_8001E7B8(p, c1[0], c1[1], c1[2]);
                func_8001E804(p, c1[0], c1[1], c1[2]);
                func_8001E75C(q, c0[0], c0[1], c0[2]);
                func_8001E76C(q, c0[0], c0[1], c0[2]);
                func_8001E7B8(q, c1[0], c1[1], c1[2]);
                func_8001E804(q, c1[0], c1[1], c1[2]);
            }
            func_8001DFE0((s32)p, (s32)tp, (s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], o->unk1AC, o->unk1AB, o->unk194);
            p += 0x24;
            q += 0x24;
            tp += 8;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c1[0], c1[1], c1[2]);
                func_8001E76C(p, c1[0], c1[1], c1[2]);
                func_8001E7B8(p, c2[0], c2[1], c2[2]);
                func_8001E804(p, c2[0], c2[1], c2[2]);
                func_8001E75C(q, c1[0], c1[1], c1[2]);
                func_8001E76C(q, c1[0], c1[1], c1[2]);
                func_8001E7B8(q, c2[0], c2[1], c2[2]);
                func_8001E804(q, c2[0], c2[1], c2[2]);
            }
            func_8001DFE0((s32)p, (s32)tp, (s32)&v[2], (s32)&v[3], (s32)&v[4], (s32)&v[5], o->unk1AC, o->unk1AB, o->unk194);
            p += 0x24;
            q += 0x24;
            tp += 8;
            v += 6;
        }
        break;
    }
    case 13: {
        u8 *p;
        u8 *q;

        if (o->unk1AD >= 0) {
            func_801E7020(o->unk13C);
        }
        p = o->unk164[D_800794F4];
        q = o->unk164[D_800794F4 ^ 1];
        for (i = 0; i < o->n; i++) {
            func_8001EC3C(p, o->unk170.b[0], o->unk170.b[2], o->unk170.b[4], o->unk170.b[6]);
            *(u16 *)(p + 0x1A) = o->unk178;
            *(u16 *)(p + 0xE) = o->unk17C;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c0[0], c0[1], c0[2]);
                func_8001E76C(p, c0[0], c0[1], c0[2]);
                func_8001E7B8(p, c1[0], c1[1], c1[2]);
                func_8001E804(p, c1[0], c1[1], c1[2]);
                func_8001E75C(q, c0[0], c0[1], c0[2]);
                func_8001E76C(q, c0[0], c0[1], c0[2]);
                func_8001E7B8(q, c1[0], c1[1], c1[2]);
                func_8001E804(q, c1[0], c1[1], c1[2]);
            }
            func_8001D900((s32)p, (s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], o->unk1AB, o->unk194);
            p += 0x34;
            q += 0x34;
            func_8001EC3C(p, o->unk170.b[0], o->unk170.b[2], o->unk170.b[4], o->unk170.b[6]);
            *(u16 *)(p + 0x1A) = o->unk178;
            *(u16 *)(p + 0xE) = o->unk17C;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c1[0], c1[1], c1[2]);
                func_8001E76C(p, c1[0], c1[1], c1[2]);
                func_8001E7B8(p, c2[0], c2[1], c2[2]);
                func_8001E804(p, c2[0], c2[1], c2[2]);
                func_8001E75C(q, c1[0], c1[1], c1[2]);
                func_8001E76C(q, c1[0], c1[1], c1[2]);
                func_8001E7B8(q, c2[0], c2[1], c2[2]);
                func_8001E804(q, c2[0], c2[1], c2[2]);
            }
            func_8001D900((s32)p, (s32)&v[2], (s32)&v[3], (s32)&v[4], (s32)&v[5], o->unk1AB, o->unk194);
            p += 0x34;
            q += 0x34;
            v += 6;
        }
        break;
    }
    }
    PopMatrix();
    o->unk1A8 = o->unk1A6;
}

void func_80032AA0(Obj32 *p) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_8001AE90(p->unk15C[i]);
        func_8001AE90(p->unk164[i]);
    }
    if (p->unk1AD >= 0) {
        func_801E72D4(p->unk13C);
    }
    func_8001AE90(p->unk16C);
    func_8001AE90(p);
}

Particles *func_80032B44(u8 *c0, u8 *c1, Unk13C *src, s16 sx, s16 sy, s16 a5, s16 a6, s16 frames, s16 a8, s16 a9,
                         s16 count, s16 a11, s16 a12, s16 a13, s16 kind, s16 semi, s32 flags, s32 a17) {
    Particles *o;
    Particle *p;
    LINE_G2 *l;
    s32 i;
    s32 k;
    s32 angle;

    o = func_8001AD0C(0x15C);
    o->p = p = func_8001AD0C(count * 0x88);
    k = 0;
    if (src == 0) {
        o->parent = (u8 *)D_801D6A4C + 0x78;
        o->own = 0;
    } else {
        o->base = *src;
        func_80030E3C(o);
        o->parent = o;
        o->own = 1;
    }
    o->unk14E = a11;
    o->unk154 = a17;
    o->unk15A = flags & 1;
    o->count = count;
    o->unk150 = 0;
    o->frames = frames;
    o->rgb[0] = c0[0];
    o->rgb[1] = c0[1];
    o->rgb[2] = c0[2];
    o->drgb[0] = (c1[0] - o->rgb[0]) / o->frames;
    o->drgb[1] = (c1[1] - o->rgb[1]) / o->frames;
    o->drgb[2] = (c1[2] - o->rgb[2]) / o->frames;
    o->unk158 = a9 == 0 ? 1 : -1;
    o->kind = kind;
    for (i = 0; i < o->count; i++, p++) {
        if (o->kind == 0) {
            l = &p->line[0];
            func_800678E4(l);
            setSemiTrans(l, semi);
            l = &p->line[1];
            func_800678E4(l);
            setSemiTrans(l, semi);
        } else {
            l = &p->line[0];
            func_80067904(l);
            setSemiTrans(l, semi);
            l->r0 = c0[0];
            l->g0 = c0[1];
            l->b0 = c0[2];
            l->r1 = c1[0];
            l->g1 = c1[1];
            l->b1 = c1[2];
            l++;
            func_80067904(l);
            setSemiTrans(l, semi);
            l->r0 = c0[0];
            l->g0 = c0[1];
            l->b0 = c0[2];
            l->r1 = c1[0];
            l->g1 = c1[1];
            l->b1 = c1[2];
        }
        func_8001EFDC(p, (s32)o->parent, 0, 0, 0, 0, 0, 0);
        p->unk7C = a5 * 8;
        p->unk7E = rand() % a8 + 1;
        if (sy == 0) {
            sy = 1;
        }
        if (sx == 0) {
            sx = 1;
        }
        if (a13 < 3) {
            p->unk32 = rand() % sx - sx / 2;
            p->unk30 = rand() % sy - sy / 2;
            p->unk34 = 0;
            p->unk7A = 0;
        } else {
            p->unk32 = 0;
            p->unk30 = 0;
            p->unk34 = 0;
            p->unk7A = sx - 0xB4;
        }
        p->unk80 = p->unk7E * frames;
        p->unk78 = 0;
        p->unk76 = 0;
        p->unk74 = 0;
        if (a12 != 0) {
            switch ((s16)(a13 % 3)) {
            case 0:
                angle = i << 12;
                k = angle / o->count;
                p->unk76 = a12;
                break;
            case 1:
                k = rand() % 4096;
                p->unk76 = a12;
                break;
            case 2:
                k = rand() % 4096;
                p->unk76 = rand() % a12;
                break;
            }
            p->unk34 = k;
        }
        p->unk82 = rand() & 0xFFF;
        p->unk84 = rand() & 0x1FF;
        if (i & 1) {
            p->unk84 = -p->unk84;
        }
    }
    o->unk147 = flags & 2;
    if (a6 == 0) {
        o->unk156 = 0;
    } else {
        o->unk156 = (a6 - a5) * 8 / o->frames;
    }
    return o;
}

void func_80033258(Particles *o) {
    Particle *p;
    LINE_G2 *l;
    SVECTOR *v;
    s32 limit;
    s32 i;
    s32 t;
    s32 d;
    u32 z;
    s16 dx;
    s16 dz;
    s32 a;
    u8 r;
    u8 g;
    u8 bl;
    s32 b;

    p = o->p;
    limit = 10000;
    if (o->own != 0) {
        if (((u8 *)o)[0x139] != 0) {
            func_80031754(o);
            return;
        }
        PushMatrix();
        func_80030F90((s32)o, o->unk15A);
        PopMatrix();
        limit = (*(s16 *)((u8 *)o + 0x124) + o->frames - 1) / o->frames * o->frames;
    }
    PushMatrix();
    if (o->kind == 0) {
        if (o->unk147 == 0) {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 8;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vz += d;
                    z = RotTransPers((s32)v, (s32)&l->r1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        r = o->rgb[0] + o->drgb[0] * t;
                        g = o->rgb[1] + o->drgb[1] * t;
                        bl = o->rgb[2] + o->drgb[2] * t;
                        l->r0 = r;
                        l->g0 = g;
                        l->b0 = bl;
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p++;
        }
        } else {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 16;
                v->vx += dx = d * rsin(p->unk82) / 4096;
                v->vz += dz = d * rcos(p->unk82) / 4096;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vx -= dx;
                    v->vz -= dz;
                    z = RotTransPers((s32)v, (s32)&l->r1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        r = o->rgb[0] + o->drgb[0] * t;
                        g = o->rgb[1] + o->drgb[1] * t;
                        bl = o->rgb[2] + o->drgb[2] * t;
                        l->r0 = r;
                        l->g0 = g;
                        l->b0 = bl;
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p->unk82 += p->unk84;
            p++;
        }
        }
    } else {
        if (o->unk147 == 0) {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 8;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vz += d;
                    z = RotTransPers((s32)v, (s32)&l->x1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p++;
        }
        } else {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 16;
                v->vx += dx = d * rsin(p->unk82) / 4096;
                v->vz += dz = d * rcos(p->unk82) / 4096;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vx -= dx;
                    v->vz -= dz;
                    z = RotTransPers((s32)v, (s32)&l->x1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p->unk82 += p->unk84;
            p++;
        }
        }
    }
    o->unk150++;
    PopMatrix();
}

void func_80033CD4(void *arg0) {
    func_8001AE90((*(void **)((s8 *)arg0 + 0x140)));
    func_8001AE90(arg0);
}

void func_80033D08(s32 n) {
    while (n > 0) {
        func_80014C08(D_800794F0);
        if (((s8 *)D_801D8340)[0x823] == 0) {
            n--;
        }
        if (((s8 *)D_801D8340)[0x815] != 0) {
            ((s8 *)D_801D8340)[0x815] = 0;
            func_80014A90();
            return;
        }
    }
}

s32 func_80033D9C(void) {
    void *var_v0_2;

    if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
        (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
        func_80014A90();
        return -1;
    }
    if ((((u32) (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178)) >> 0x11) & 3) == 1) {
        var_v0_2 = *D_80089840;
    } else {
        var_v0_2 = D_80089840[(*(s8 *)((s8 *)D_801D8340 + 0x817))];
    }
    if (!((*(u16 *)((s8 *)var_v0_2 + 0xA)) & 0x40)) {
        return 0;
    }
    func_8002B498(0xA0);
    return 1;
}

void func_80033E7C(void) {
    s32 n;

    (*(s32 *)((s8 *)D_801D8340 + 0x7FC)) = 0;
    while (1) {
        if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
            func_80014A90();
            return;
        }
        func_80014C08(D_800794F0);
        if ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x816)) = 0;
            return;
        }
        if ((*(s8 *)((s8 *)D_801D8340 + 0x816)) == 0) {
            return;
        }
        n = (*(s32 *)((s8 *)D_801D8340 + 0x7FC))++;
        if (n >= 0xF1) {
            return;
        }
    }
}

void func_80033F34(void) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;

    if (D_801D8330 != 0) {
        D_801D8330--;
    }
    for (i = 0; i < 2; i++) {
        x = 0x80;
        y = i * -125 + 0x99;
        for (j = 0; j < 3; j++) {
            if (func_80029990() != 0) {
                return;
            }
            if (D_801D8330 == 0 && ((*(u32 *)(D_801D8348[i] + 0x178) >> 2) & 3) != j) {
                continue;
            }
            CUR_SPRT->sp.x0 = x + (x - D_8006E280[j]) * D_801D8330 / 32;
            CUR_SPRT->sp.y0 = y + (y - D_8006E288[i][j]) * D_801D8330 / 32;
            CUR_SPRT->sp.u0 = j * 64 + 64;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7FF0;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = x;
            CUR_SPRT->sp.g0 = x;
            CUR_SPRT->sp.b0 = x;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1D);
            addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        }
    }
}

void func_800341EC(void) {
    s32 var_a1;
    u32 temp_v0;

    temp_v0 = (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178));
    var_a1 = (temp_v0 >> 0x11) & 1;
    if (((temp_v0 >> 0x11) & 3) == 1) {
        var_a1 = 0;
    }
    func_8001A164(&D_801D8278, var_a1);
}

INCLUDE_RODATA("asm/main/nonmatchings/model/effect", D_80010C9C);
