#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/model.h"
#include "dcb/archive.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/model_anim.h"

void func_80022C4C(void) {
    Unk801D6A4C *p;

    p = D_801D6A4C = allocHeapBlock(0x29C, 0x7F);
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
    freeHeapBlocksByTag(arg0 + 0x5A);
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
    freeHeapBlocksByTag(0x82);
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
        p = (s32)findPakChunk(pak, 1, n);
        if (p == 0) {
            p = loadFileTagged((s32 *)name, func_800148B0(), slot + 0x5A);
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
    freeHeapBlocksByTag(arg0 + 0x40);
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
    func_80014C08(FRAME_INTERVAL);
    for (i = 0x40; i < 0x7F; i++) {
        freeHeapBlocksByTag(i);
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

INCLUDE_ASM("asm/main/nonmatchings/model/model", func_8002371C);

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
    func_80014C08(FRAME_INTERVAL);
    m = D_801D6A4C->unk13C[slot] = allocHeapBlock(0x28F8, slot + 0x40);
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
    data = findPakChunk((Chunk *)pak, 0, id);
    if (data == 0) {
        data = (u8 *)loadFileTagged((s32 *)buf, func_800148B0(), slot + 0x40);
        if (data == 0) {
            return 0;
        }
        *(s32 *)m = LOADED_FILE_SIZE;
    } else {
        *(s32 *)m = ((s32 *)data)[-1];
    }
    *(u8 **)(m + 0x26DC) = data;
    if (pos != 0) {
        i = 0;
        img = findPakChunk((Chunk *)pak, 5, id);
        if (img == 0) {
            if (func_8002371C(m) == 0) {
                goto skip;
            }
            sprintf(buf, "M:\\%s", data);
            i = 1;
            img = (u32 *)loadFile(buf, func_800148B0());
        }
        if (img != 0) {
            u = (pos & 0xF) << 6;
            v = (pos & 0x10) << 4;
            uploadTimListOffset(img, u - 0x140, v);
            OpenTIM(img);
            ReadTIM(&tim);
            *(Rect16 *)(m + 0x26E4) = *tim.prect;
            *(Rect16 *)(m + 0x26EC) = *tim.crect;
            ((Rect16 *)(m + 0x26EC))->x += u - 0x140;
            ((Rect16 *)(m + 0x26EC))->y += v;
            ((Rect16 *)(m + 0x26E4))->x += u - 0x140;
            ((Rect16 *)(m + 0x26E4))->y += v;
            if (i) {
                freeHeapBlock(img);
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
