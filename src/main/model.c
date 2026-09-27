#include "common.h"
#include "gte.h"
#include "game.h"

void func_80022C4C(void) {
    Unk801D6A4C *p;

    p = D_801D6A4C = func_8001ABCC(0x29C, 0x7F);
    bzero(p, 0x29C);
    GsInitCoordinate2(NULL, &D_801D6A4C->root);
    D_80079544 = 1;
}

void func_80022CA4(s32 arg0) {
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_v1 + 0x2208)) <= 0) {
        (*(s32 *)((s8 *)temp_v1 + 0x2208)) = -1;
        return;
    }
    temp_v1_2 = D_801D6A4C->unk13C[arg0];
    (*(s32 *)((s8 *)temp_v1_2 + 0x2208)) = (s32) -(*(s32 *)((s8 *)temp_v1_2 + 0x2208));
}

void func_80022D00(s32 arg0) {
    s32 temp_v0;
    void *temp_v1;

    temp_v1 = D_801D6A4C->unk13C[arg0];
    temp_v0 = (*(s32 *)((s8 *)temp_v1 + 0x2208));
    if (temp_v0 < 0) {
        (*(s32 *)((s8 *)temp_v1 + 0x2208)) = -temp_v0;
    }
}

s32 func_80022D34(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_a0;
    void *temp_v1;
    s32 temp_v0;

    temp_a0 = D_801D6A4C->unk13C[arg0];
    temp_v1 = (s8 *)temp_a0 + 0x2200;
    temp_v0 = ((Model2220 *)temp_a0)->unk2220[arg1].unk0;
    (*(s32 *)((s8 *)temp_a0 + 0x26D8)) = arg3;
    (*(s32 *)((s8 *)temp_v1 + 4)) = temp_v0;
    (*(s32 *)((s8 *)temp_v1 + 0x14)) = temp_v0;
    (*(s32 *)((s8 *)temp_a0 + 0x2200)) = arg1;
    if (arg2 == -2) {
        arg2 = (*(s16 *)((s8 *)((Model2220 *)temp_a0)->unk2220[arg1].unk4 + 0x1A));
    }
    (*(s32 *)((s8 *)temp_v1 + 0x18)) = arg2;
    (*(s32 *)((s8 *)temp_v1 + 0xC)) = 0;
    (*(s32 *)((s8 *)temp_v1 + 0x1C)) = 0x3F800000;
    return func_80022100(temp_a0, 0, -1);
}

void func_80022DBC(s32 arg0) {
    s32 key;
    s32 i;

    key = *(s16 *)((s8 *)D_801D6A4C->unk13C[arg0] + 6);
    func_8001AFF0(arg0 + 0x5A);
    key = (key << 8) | 0x10000000;
    for (i = 0; i < 0x20; i++) {
        if ((D_801D6A4C->unk19C[i].key & ~0xFF) == key) {
            D_801D6A4C->unk19C[i].key = 0;
            D_801D6A4C->unk19C[i].value = 0;
        }
    }
}

void func_80022E58(void) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if ((D_801D6A4C->unk19C[i].key & 0x0FFFFF00) >= 0x3E80) {
            D_801D6A4C->unk19C[i].key = 0;
            D_801D6A4C->unk19C[i].value = 0;
        }
    }
    func_8001AFF0(0x82);
}

s32 func_80022ED0(s32 key, s32 n, KeyValue **pp) {
    KeyValue *e;
    KeyValue *free;
    s32 i;

    e = *pp;
    free = 0;
    for (i = 0; i < n; i++, e++) {
        if (e->key == key) {
            return e->value;
        }
        if (free == 0 && e->key == 0) {
            free = e;
        }
    }
    *pp = free;
    return 0;
}

s32 func_80022F34(s32 id, s32 anim, s32 slot, Chunk *pak) {
    char name[24];
    KeyValue *e;
    s32 p;
    s32 n;

    e = (KeyValue *)D_801D6A4C->unk19C;
    p = func_80022ED0((id << 8) | 0x10000000 | anim, 32, &e);
    if (p == 0) {
        if (e == 0) {
            return 0;
        }
        if (id > 1000) {
            sprintf(name, "M:\\HDF%d\\%d_%d.hdf", id / 10, id / 10, id % 10);
            n = id;
        } else {
            sprintf(name, "M:\\HDF%03d\\%c.hdf", id, anim + 'a');
            n = anim;
        }
        p = (s32)func_8001BB44(pak, 1, n);
        if (p == 0) {
            p = func_8001B248((s32 *)name, func_800148B0(), slot + 0x5A);
            if (p == 0) {
                return 0;
            }
            e->key = (id << 8) | 0x10000000 | anim;
            e->value = p;
        }
    }
    return p;
}

void func_80023094(Model2220 *m, s32 *p, s32 i) {
    m->unk2220[i].unk0 = *p++;
    m->unk2220[i].unk4 = p;
}

s32 func_800230B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    void *temp_s0;

    temp_s0 = D_801D6A4C->unk13C[arg0];
    temp_v0 = func_80022F34((*(s16 *)((s8 *)temp_s0 + 6)), arg1, arg0, (Chunk *)arg3);
    if (temp_v0 != 0) {
        func_80023094(temp_s0, (s32 *)temp_v0, arg2);
        return 1;
    }
    return 0;
}

void func_80023128(s32 arg0, s32 arg1, s32 arg2) {
    func_800230B8(arg0, arg1, arg2, 0);
}

void func_80023148(s32 slot, s32 anim) {
    u8 *m;
    BoneAnim *b;
    u8 *c;
    SVECTOR *r;
    s16 *key;
    s32 *p0;
    s32 *p3;
    s32 *p6;
    s32 i;
    s32 j;

    m = D_801D6A4C->unk13C[slot];
    b = (BoneAnim *)(m + 0xD80);
    c = m + 0x78;
    r = (SVECTOR *)(m + 0xA80);
    key = (s16 *)((u8 *)((Model2220 *)m)->unk2220[anim].unk4 + 4);
    for (i = 0; i < *(s16 *)(m + 4) + 1; i++, key += 12, b++, c += 0x50, r++) {
        for (j = 0, p0 = &b->ch[0].val, p3 = &b->ch[3].val, p6 = &b->ch[6].val; j < 3; j++) {
            *(s32 *)((u8 *)p0 + (j << 4)) = *(s32 *)((u8 *)p3 + (j << 4)) = *(s32 *)((u8 *)p6 + (j << 4)) = 0;
        }
        b->ch[0].unk0 = key[0] << 20;
        b->ch[1].unk0 = key[1] << 20;
        b->ch[2].unk0 = key[2] << 20;
        b->ch[3].unk0 = key[4] << 16;
        b->ch[4].unk0 = key[5] << 16;
        b->ch[5].unk0 = key[6] << 16;
        b->ch[6].unk0 = key[8] << 16;
        b->ch[7].unk0 = key[9] << 16;
        b->ch[8].unk0 = key[10] << 16;
        if (i < *(s16 *)(m + 4)) {
            r->vx = b->ch[0].unk0 / 0x100000;
            r->vy = b->ch[1].unk0 / 0x100000;
            r->vz = b->ch[2].unk0 / 0x100000;
            *(s32 *)(c + 0x18) = (s16)(b->ch[3].unk0 >> 16) + ((Model2220 *)m)->bonepos[i][0];
            *(s32 *)(c + 0x1C) = (s16)(b->ch[4].unk0 >> 16) + ((Model2220 *)m)->bonepos[i][1];
            *(s32 *)(c + 0x20) = (s16)(b->ch[5].unk0 >> 16) + ((Model2220 *)m)->bonepos[i][2];
            ((Model2220 *)m)->scale[i][0] = (s16)(b->ch[6].unk0 >> 16);
            ((Model2220 *)m)->scale[i][1] = (s16)(b->ch[7].unk0 >> 16);
            ((Model2220 *)m)->scale[i][2] = (s16)(b->ch[8].unk0 >> 16);
            RotMatrixYXZ(r, c + 4);
            *(s32 *)c = 0;
            ScaleMatrix(c + 4, ((Model2220 *)m)->scale[i]);
        }
    }
    func_80022CA4(slot);
}

void func_80023408(Tmd18 *t) {
    Obj18 *o;
    s32 n;
    s32 i;

    if (t->flags == 0) {
        t->flags = 1;
        n = t->nobj;
        o = t->obj;
        for (i = 0; i < n; i++) {
            o->unk14 = (s32)t + o->unk14;
            o++;
        }
    }
}

void func_80023454(s32 arg0, void *arg1, s32 arg2) {
    (*(s32 *)((s8 *)arg1 + 0xC)) = (s32) (arg2 + 1);
    (*(s32 *)((s8 *)arg1 + 0)) = 0;
    (*(s32 *)((s8 *)arg1 + 8)) = arg0;
}

s32 *func_80023468(u8 *m, s32 *p) {
    s32 i;

    for (i = 0; i < *(s16 *)(m + 4); i++) {
        ((Unk1F80 *)m)->unk1F80[i] = (s16 *)(p + 1);
        p += 3;
    }
    return p;
}

void func_800234AC(Model *m) {
    GsCOORDINATE2 *c;
    s8 *parent;
    GsDOBJ4 *o;
    s32 i;

    c = m->coord;
    GsInitCoordinate2(&D_801D6A4C->root, &m->root);
    for (i = 0, parent = m->parent, o = m->obj; i < m->nobj; i++, parent++, o++, c++) {
        o->coord2 = c;
        if (*parent < 0) {
            GsInitCoordinate2(&m->root, c);
        } else {
            GsInitCoordinate2(&m->coord[*parent], c);
        }
        c->coord.t[0] = m->bonepos[i][0];
        c->coord.t[1] = m->bonepos[i][1];
        c->coord.t[2] = m->bonepos[i][2];
    }
}

void func_800235C8(s32 arg0) {
    D_801D6A4C->unk114[arg0] = 0;
    D_801D6A4C->unk13C[arg0] = 0;
    func_8001AFF0(arg0 + 0x40);
}

void func_8002360C(void) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (D_801D6A4C->unk114[i] != 0) {
            func_80022DBC(i);
            D_801D6A4C->unk114[i] = 0;
            /* sic: the original clears the wrong slot */
            D_801D6A4C->unk13C[i - 0x40] = 0;
        }
    }
    func_80014C08(D_800794F0);
    for (i = 0x40; i < 0x7F; i++) {
        func_8001AFF0(i);
    }
}

void *func_800236B4(s32 id) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        if (D_801D6A4C->unk114[i] != 0 &&
            *(s16 *)((u8 *)D_801D6A4C->unk13C[i] + 6) == id) {
            return D_801D6A4C->unk13C[i];
        }
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/model", func_8002371C);

s32 func_8002386C(s32 slot, s32 id, s32 pos, s32 pak, s8 kind) {
    char buf[16];
    TIM_IMAGE tim;
    u8 *m;
    u8 *data;
    u32 *img;
    s32 u;
    s32 v;
    s32 i;
    s32 j;

    if (slot >= 0x18) {
        return 0;
    }
    if (D_801D6A4C->unk13C[slot] != 0) {
        func_80022DBC(slot);
        func_800235C8(slot);
    }
    func_80014C08(D_800794F0);
    m = D_801D6A4C->unk13C[slot] = func_8001ABCC(0x28F8, slot + 0x40);
    bzero(m, 0x28F8);
    *(s32 *)(m + 0x26F4) = pak;
    func_80022CA4(slot);
    RotMatrixYXZ(m + 0xA78, m + 0x2C);
    *(s32 *)(m + 0x28) = 0;
    *(s32 *)(m + 0x20) = 0x1000;
    *(s32 *)(m + 0x1C) = 0x1000;
    *(s32 *)(m + 0x18) = 0x1000;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 3; j++) {
            ((Model *)m)->keys[i].rot[j].unk0 = 0;
            ((Model *)m)->keys[i].pos[j].unk0 = 0;
            ((Model *)m)->keys[i].scale[j].unk0 = 0x10000000;
        }
    }
    if (pos < 0) {
        pos = (slot >> 1) + (slot & 1) * 16 + 5;
    }
    *(s32 *)(m + 0x26D4) = (pos - 5) << 16;
    *(s32 *)(m + 0x26D0) = ((((pos & 0x10) << 10) | ((pos & 0xF) * 4)) - 0x14) << 16;
    *(s16 *)(m + 6) = id;
    if (id > 1000) {
        sprintf(buf, "M:\\%d_%d.omd", id / 10, id % 10);
    } else {
        sprintf(buf, "M:\\%03d.omd", id);
    }
    data = func_8001BB44((Chunk *)pak, 0, id);
    if (data == 0) {
        data = (u8 *)func_8001B248((s32 *)buf, func_800148B0(), slot + 0x40);
        if (data == 0) {
            return 0;
        }
        *(s32 *)m = D_801D4848;
    } else {
        *(s32 *)m = ((s32 *)data)[-1];
    }
    *(u8 **)(m + 0x26DC) = data;
    if (pos != 0) {
        i = 0;
        img = func_8001BB44((Chunk *)pak, 5, id);
        if (img == 0) {
            if (func_8002371C(m) == 0) {
                goto skip;
            }
            sprintf(buf, "M:\\%s", data);
            i = 1;
            img = (u32 *)func_8001B144(buf, func_800148B0());
        }
        if (img != 0) {
            u = (pos & 0xF) << 6;
            v = (pos & 0x10) << 4;
            func_8001B634(img, u - 0x140, v);
            OpenTIM(img);
            ReadTIM(&tim);
            *(Rect16 *)(m + 0x26E4) = *tim.prect;
            *(Rect16 *)(m + 0x26EC) = *tim.crect;
            ((Rect16 *)(m + 0x26EC))->x += u - 0x140;
            ((Rect16 *)(m + 0x26EC))->y += v;
            ((Rect16 *)(m + 0x26E4))->x += u - 0x140;
            ((Rect16 *)(m + 0x26E4))->y += v;
            if (i) {
                func_8001AE90(img);
            }
        }
    }
skip:
    StoreImage2((Rect16 *)(m + 0x26EC), (u32 *)(m + 0x26F8));
    data += 0x10;
    *(s16 *)(m + 4) = *(u16 *)data;
    data += 4;
    for (i = 0; i < 32; i++) {
        ((Model *)m)->parent[i] = *data++;
    }
    data = (u8 *)func_80023468(m, (s32 *)data);
    if (kind == 0) {
        for (i = 0; i < *(s16 *)(m + 4); i++) {
            if (i != 0) {
                for (data += 4; *(s32 *)data != 0x30444D4F; data += 4) {
                }
            }
            ((Model *)m)->obj[i].tmd = 0;
            func_80023408((Tmd18 *)data);
            func_80023454((s32)(data + 12), &((Model *)m)->obj[i], i);
        }
    } else {
        for (i = 0; i < *(s16 *)(m + 4); i++) {
            if (i != 0) {
                for (data += 4; *(s32 *)data != 0x41 || ((s32 *)data)[1] != 0; data += 4) {
                }
            }
            ((Model *)m)->obj[i].tmd = 0;
            GsMapModelingData((u32 *)(data + 4));
            GsLinkObject4((u32)(data + 12), &((Model *)m)->obj[i], 0);
        }
    }
    func_800234AC((Model *)m);
    return 1;
}

void func_80023DA4(s32 arg0, s32 arg1, s32 arg2) {
    func_8002386C(arg0, arg1, arg2, 0, 0);
}

void func_80023DC8(s32 arg0, s32 arg1, s32 arg2) {
    func_8002386C(arg0, arg1, arg2, 0, 1);
}

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

INCLUDE_ASM("asm/main/nonmatchings/model", func_800250F4);

INCLUDE_RODATA("asm/main/nonmatchings/model", D_80010190);
