#include "common.h"
#include "gte.h"
#include "game.h"

void func_80023DF0(Unk800793A0 *db, s32 idx) {
    MATRIX ls;
    SVECTOR sv;
    MATRIX lm;
    MATRIX lc;
    MATRIX unused;
    SVECTOR rot;
    s32 flag;
    u32 *ot;
    u32 shift;
    u32 packet;
    Model *m;
    GsDOBJ4 *obj;
    ModelLink *l;
    s32 i;
    s32 j;
    s32 *scratch;

    ot = D_801D6A4C->ot[idx].org;
    shift = 0xFFF;
    if (D_80079544 != 0) {
        packet = (u32)db->unk4070;
        lc = D_801D6A28;
        for (i = 0; i < 24; i++) {
            m = D_801D6A4C->unk13C[i];
            if (D_801D6A4C->unk114[i] <= 0 || m->unk26D4 < 0) {
                continue;
            }
            lm = D_801D6A08;
            gte_SetColorMatrix(&lc);
            PushMatrix();
            if (i != 0x17) {
                sv.vx = m->pos.vx;
                sv.vy = m->pos.vy;
                sv.vz = m->pos.vz;
                gte_ldv0(&sv);
                gte_rtv0tr();
                gte_stlvnl(ls.t);
                gte_stflg(&flag);
            } else {
                ls.t[0] = ls.t[1] = ls.t[2] = 0;
                shift >>= 2;
                ot += shift * 3;
                func_8002E7E8((u8 *)m);
            }
            if (D_801D6A4C->unk114[i] == 1) {
                RotMatrix(&D_801D6A78, &D_801D6A4C->root.coord);
                D_801D6A4C->root.flg = 0;
                D_801D6A4C->root.coord.t[0] = ls.t[0];
                D_801D6A4C->root.coord.t[1] = ls.t[1];
                D_801D6A4C->root.coord.t[2] = ls.t[2];
                RotMatrixYXZ(&m->rot, &m->root.coord);
                m->root.flg = 0;
                ScaleMatrix(&m->root.coord, &m->scale);
            } else if (D_801D6A4C->unk114[i] == 2) {
                PopMatrix();
                continue;
            } else {
                l = (ModelLink *)m->unk26E0;
                memset(&rot, 0, 8);
                if (l->unk571 != 0) {
                    func_80030F90((s32)l, l->unk56F);
                    m->root.coord = l->m;
                    l->model->rot = l->rot;
                    l->model->pos = l->pos;
                }
                RotMatrix(&rot, &D_801D6A4C->root.coord);
                D_801D6A4C->root.flg = 0;
                D_801D6A4C->root.coord.t[2] = 0;
                D_801D6A4C->root.coord.t[1] = 0;
                D_801D6A4C->root.coord.t[0] = 0;
                D_801D6A4C->unk114[i] = 2;
            }
            scratch = (s32 *)0x1F800000;
            scratch[12] = m->unk26D4;
            scratch[13] = m->unk26D0;
            obj = m->obj;
            for (j = 0; j < m->nobj; j++, obj++) {
                obj->coord2->flg = 0;
                if (obj->id != -1 && obj->tmd != NULL) {
                    GsGetLws(obj->coord2, &m->lw[j], &ls);
                    if (obj->attribute == 0) {
                        gte_SetLightMatrix(&lm);
                        gte_SetRotMatrix(&ls);
                        gte_SetTransMatrix(&ls);
                        if (obj->tmd[0] != 0) {
                            packet = func_80020370((u32 *)obj->tmd[5], ot + 1, packet, (void *)shift);
                        } else {
                            packet = func_800202D8((u32 *)obj->tmd[5], ot + 1, packet, (void *)shift);
                        }
                    }
                }
            }
            PopMatrix();
        }
    }
}

void func_800243B0(s32 h) {
    func_8005C484(0xA0, 0x78);
    func_8005C4A4(h);
    func_80062484(h);
    D_801D6A4C->unkC4.vpx = 0;
    D_801D6A4C->unkC4.vpy = 0;
    D_801D6A4C->unkC4.vpz = 0;
    D_801D6A4C->unkC4.vrx = 0;
    D_801D6A4C->unkC4.vry = 0;
    D_801D6A4C->unkC4.vrz = 0;
    D_801D6A4C->unkC4.rz = 0;
    D_801D6A4C->unkC4.super = 0;
    GsSetRefView2(&D_801D6A4C->unkC4);
}

void func_80024420(void) {
    GsSetAmbient(0x40, 0x40, 0x40);
    func_8005C464(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

void func_80024460(s32 alloc) {
    s32 i;

    func_80022C4C();
    func_8006A804();
    for (i = 0; i < 2; i++) {
        if (alloc) {
            DB(i).unk4070 = func_8001ABCC(0xBB80, 0x7F);
        }
        D_801D6A4C->ot[i].length = 12;
        D_801D6A4C->ot[i].org = DB(i).ot;
        D_801D6A4C->ot[i].offset = 0;
        D_801D6A4C->ot[i].point = 0;
        D_801D6A4C->ot[i].tag = D_801D6A4C->ot[i].org + 0xFFF;
    }
    GsInit3D();
    func_800243B0(0x1C0);
    func_80024420();
    func_8006A814();
    {
        MATRIX lm[2] = {
            { { { 0, 0x1800, -0x1800 }, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
            { { { 0, -0x1000, -0x5DC }, { 0, 0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
        };
        MATRIX lc[2] = {
            { { { 0x800, 0, 0 }, { 0x800, 0, 0 }, { 0x800, 0, 0 } }, { 0, 0, 0 } },
            { { { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
        };

        D_801D6A08 = lm[1];
        D_801D6A28 = lc[1];
    }
    if (alloc) {
        func_8001B10C((s32) "M:", func_800148B0());
        func_8001683C((s32)func_80023DF0);
    }
}

void func_800246E0(Unk800793A0 *db, s32 n) {
    LINE_F2 *l;
    SVECTOR *v;
    SVECTOR *vv;
    s32 *xy;
    s32 *xx;
    s32 i;
    s32 j;
    u8 c;
    s32 p;
    s32 flag;

    l = D_801D6A58[n];
    if (D_8006DF84 == 0) {
        return;
    }
    v = D_801D6A50;
    xy = D_801D6A48;
    for (i = 0; i < D_801D69FC * D_801D69FE; i++, v++, xy++) {
        if ((v->pad = RotTransPers((s32)v, (s32)xy, &p, &flag)) < 0x3C) {
            v->pad = -1;
        }
    }
    v = D_801D6A50;
    xy = D_801D6A48;
    for (j = 0; j < D_801D69FE; j++, v++, xy++) {
        for (i = 0; i < D_801D69FC - 1; i++, v++, xy++) {
            c = PULSE(j + i);
            if ((s8)PULSE(j + i) < 0) {
                c = -c;
            }
            if ((u16)v[0].pad < 0x1000 && (u16)v[1].pad < 0x1000) {
                *(s32 *)&l->x0 = xy[0];
                *(s32 *)&l->x1 = xy[1];
                l->r0 = c >> 1;
                l->g0 = c + 0x40;
                l->b0 = (0x80 - c) / 2;
                AddPrim((s32 *)&db->ot[v[1].pad < v[0].pad ? v[0].pad : v[1].pad], (s32)l++);
            }
        }
    }
    v = D_801D6A50;
    xy = D_801D6A48;
    for (i = 0; i < D_801D69FC; i++, v++, xy++) {
        vv = v;
        xx = xy;
        for (j = 0; j < D_801D69FE - 1; j++, vv += D_801D69FC, xx += D_801D69FC) {
            c = PULSE(i + j);
            if ((s8)PULSE(i + j) < 0) {
                c = -c;
            }
            if ((u16)vv[0].pad < 0x1000 && (u16)vv[D_801D69FC].pad < 0x1000) {
                *(s32 *)&l->x0 = xx[0];
                *(s32 *)&l->x1 = xx[D_801D69FC];
                l->r0 = c >> 1;
                l->g0 = c + 0x40;
                l->b0 = (0x80 - c) / 2;
                AddPrim((s32 *)&db->ot[vv[1].pad < vv[0].pad ? vv[0].pad : vv[1].pad], (s32)l++);
            }
        }
    }
    D_8006DF88 += 2;
}

void func_80024B08(s32 w, s32 h, s32 cols, s32 rows, s32 unused, s32 vertical) {
    u8 *l;
    SVECTOR *v;
    s32 i;
    s32 j;
    s32 r;

    D_801D69FC = cols;
    D_801D69FE = rows;
    D_801D6A00 = ((s16)cols - 1) * (s16)rows + ((s16)rows - 1) * (s16)cols;
    D_801D69F8 = w;
    D_801D69FA = h;
    D_8006DF84 = 1;
    for (i = 0; i < 2; i++) {
        l = D_801D6A58[i] = func_8001AD0C(D_801D6A00 * 16);
        for (j = 0; j < D_801D6A00; j++) {
            func_800678E4(l);
            l[4] = 8;
            l[5] = 0x40;
            l[6] = 8;
            l += 16;
        }
    }
    D_801D6A50 = v = (SVECTOR *)func_8001AD0C(D_801D69FC * (D_801D69FE << 3));
    D_801D6A48 = (s32 *)func_8001AD0C(D_801D69FC * (D_801D69FE << 2));
    for (r = 0; r < D_801D69FE; r++) {
        for (j = 0; j < D_801D69FC; j++) {
            v->vx = D_801D69F8 / 2 - D_801D69F8 / (D_801D69FC - 1) * j;
            if (vertical) {
                v->vy = D_801D69FA / 2 - D_801D69FA / (D_801D69FE - 1) * r;
                v->vz = 0;
            } else {
                v->vz = D_801D69FA / 2 - D_801D69FA / (D_801D69FE - 1) * r;
                v->vy = 0;
            }
            v++;
        }
    }
    func_8001683C((s32)func_800246E0);
}

void func_80024DD4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_8001AE90(D_801D6A58[i]);
    }
    func_8001AE90(D_801D6A50);
    func_8001AE90(D_801D6A48);
}

s32 func_80024E44(u8 *cam, s32 *pos, s32 cur, s16 *target) {
    s32 k;
    s32 n;
    s32 d;
    s32 step;

    if (D_8007956C != 0) {
        *(s32 *)(cam + 0x5C) = -pos[0];
        *(s32 *)(cam + 0x60) = -pos[1];
        *(s32 *)(cam + 0x64) = -pos[2];
        *(s32 *)(cam + 0x68) = target[2] << 12;
        *(s32 *)(cam + 0x6C) = target[3] << 12;
        *(s32 *)(cam + 0x70) = target[1] << 12;
        return target[4];
    }
    k = *(s32 *)(cam + 0x50);
    if (k == 0) {
        k = 1;
    }
    n = 30 / k;
    if (n == 0) {
        n = 1;
    }
    d = (target[3] << 12) - *(s32 *)(cam + 0x6C);
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(cam + 0x6C) += step;
    }
    d = (target[2] << 12) - *(s32 *)(cam + 0x68);
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(cam + 0x68) += step;
    }
    d = (target[1] << 12) - *(s32 *)(cam + 0x70);
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d > 0 ? 1 : -1;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(cam + 0x70) += step;
    }
    d = target[4] - cur;
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d > 0 ? 1 : -1;
        }
        cur += step;
    }
    *(s32 *)(cam + 0x5C) = -pos[0];
    *(s32 *)(cam + 0x60) = -pos[1];
    *(s32 *)(cam + 0x64) = -pos[2];
    return cur;
}

INCLUDE_ASM("asm/main/nonmatchings/model/scene3d", func_800250F4);

INCLUDE_RODATA("asm/main/nonmatchings/model/scene3d", D_80010190);
