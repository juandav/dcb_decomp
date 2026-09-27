#include "common.h"
#include "gte.h"
#include "game.h"

void func_80014CF0(void) {
    s32 i;

    if (D_8006E050 != 0) {
        for (i = 0; i < 2; i++) {
            ((Unk8006E050 *)D_8006E050)[i].unk24++;
        }
    }
    D_800794EC++;
}

void func_80014D64(void) {
    s32 i;
    s32 j;
    POLY_FT4 *p;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            D_800793A8.sprt[i][j].tag = 0x05000000;
            D_800793A8.sprt[i][j].code = 0x66;
            D_800793A8.sprt[i][j].u0 = 0;
            D_800793A8.sprt[i][j].v0 = 0;
            D_800793A8.sprt[i][j].w = 256 - j * 192;
            D_800793A8.sprt[i][j].h = 240;
            p = &D_800793A8.poly[i][j];
            func_800677A4(p);
            p->u0 = j * 32;
            p->v0 = 0;
            p->u1 = j * 32 - 96;
            p->v1 = 0;
            p->u2 = j * 32;
            p->v2 = 240;
            p->u3 = j * 32 - 96;
            p->v3 = 240;
            setShadeTex(p, 0);
            SetSemiTrans(p, 1);
        }
    }
    SetDrawStp(&D_800793A8.stp[0], 1);
    SetDrawStp(&D_800793A8.stp[1], 0);
    D_800793A8.r = 0xA8;
    D_800793A8.g = 0xA8;
    D_800793A8.b = 0xA8;
    D_800793A8.x = 0;
    D_800793A8.y = 0;
    D_800793A8.mode = 0;
}

void func_80014EF0(void) {
    s32 i;
    POLY_FT4 *p;
    s32 unused[4];

    if (D_800794E7 == 0) {
        return;
    }
    if (D_800794E7 != 1) {
        func_801EAD04();
    }
    for (i = 1; i >= 0; i--) {
        if (D_800793A8.mode == 1) {
            D_800793A8.sprt[D_800794F4][i].tpage = GetTPage(2, 0, i * 0x100, 0x100 - D_800794F4 * 0x100) | 0xE1000000;
            (D_800793A8.sprt[D_800794F4] + i)->x0 = D_800793A8.x + (i << 8);
            (D_800793A8.sprt[D_800794F4] + i)->y0 = D_800793A8.y;
            (D_800793A8.sprt[D_800794F4] + i)->r0 = D_800793A8.r;
            (D_800793A8.sprt[D_800794F4] + i)->g0 = D_800793A8.g;
            (D_800793A8.sprt[D_800794F4] + i)->b0 = D_800793A8.b;
            addPrim(&D_800793A0->ot[0], &D_800793A8.sprt[D_800794F4][i]);
        } else {
            p = &D_800793A8.poly[D_800794F4][i];
            p->tpage = getTPage(2, D_800793A8.abr & 3, i * 160, 0x100 - D_800794F4 * 0x100);
            p->x0 = D_800793A8.px[i][0];
            p->y0 = D_800793A8.py[i][0];
            p->x1 = D_800793A8.px[i][1];
            p->y1 = D_800793A8.py[i][1];
            p->x2 = D_800793A8.px[i][2];
            p->y2 = D_800793A8.py[i][2];
            p->x3 = D_800793A8.px[i][3];
            p->y3 = D_800793A8.py[i][3];
            p->r0 = D_800793A8.r;
            p->g0 = D_800793A8.g;
            p->b0 = D_800793A8.b;
            addPrim(&D_800793A0->ot[0], p);
        }
    }
}

void func_800152AC(void) {
    s32 *p;
    s32 i;

    SetGraphDebug(0);
    InitGeom();
    p = (s32 *)0x1F800000;
    for (i = 0; i < 0x100; i++) {
        *p++ = 0;
    }
    (*(s16 *)((s8 *)(&D_800794F8) + 0)) = 0;
    (*(s16 *)((s8 *)(&D_800794F8) + 2)) = 0;
    (*(s16 *)((s8 *)(&D_800794F8) + 4)) = 0x100;
    (*(s16 *)((s8 *)(&D_800794F8) + 6)) = 0x100;
    (*(s32 *)((s8 *)(&D_800794F8) + 0x50)) = 2;
    (*(s32 *)((s8 *)(&D_800794F8) + 0x48)) = 0;
    D_800794F0 = 1;
    func_80014D64();
}

void func_80015328(void) {
    Unk800794F8 *g;
    s32 n;
    void (**cb)(Unk800793A0 *, s32);

    g = (Unk800794F8 *)&D_800794F8;
    g->unk8[0] = 0;
    D_800794EC = 0;
    for (; g->unk48 <= 0; g->unk48++) {
        func_8001A9B0();
        func_80014C08(1);
        g->unk50 = D_800794EC;
        if (D_800794EC == 0) {
            g->unk50 = 1;
        }
        D_800794EC = 0;
    }
    SetDispMask(1);
    D_800794F4 = 0;
    D_800793A0 = &g->unk98[0];
    ClearOTagR(g->unk98[0].ot, 0x1000);
    for (;;) {
        n = D_800794F0;
        func_8001A9B0();
        while (n >= 2 || g->unk48 == 0) {
            func_80014AC8();
            n--;
        }
        D_800794F4 ^= 1;
        D_800793A0 = &g->unk98[D_800794F4];
        ClearOTagR(D_800793A0->ot, 0x1000);
        if (D_800793A8.mode != 0) {
            addPrim(&D_800793A0->ot[0], &D_800793A8.stp[1]);
            addPrim(&D_800793A0->ot[0xFFF], &D_800793A8.stp[0]);
        }
        func_8002FAE4();
        func_800271D0();
        func_80016BEC();
        if (D_8006DD4C != 0) {
            for (cb = g->unk8; *cb != 0; cb++) {
                (*cb)(D_800793A0, D_800794F4);
            }
        }
        func_80014EF0();
        func_80014AC8();
        DrawSync(0);
        if (g->unk4C != 0) {
            GsSwapDispBuff();
        }
        PutDispEnv(&D_800793A0->disp);
        PutDrawEnv(&D_800793A0->draw);
        DrawOTag(&D_800793A0->ot[0xFFF]);
        g->unk50 = D_800794EC;
        if (D_800794EC == 0) {
            g->unk50 = 1;
        }
        D_800794EC = 0;
    }
}

void func_800155F4(void) {
    s32 t;

    t = func_800148B0();
    func_8002BC58();
    func_8001A600();
    func_8006A884(0);
    func_800157B0();
    func_800152AC();
    func_80014840();
    func_80015EDC();
    func_8001AA80(0);
    func_8002ADEC(t);
    func_80026E90(0x3C0, 0x100, 0x3E8);
    func_80016948(0xD);
    func_8001B90C(0x140, 0xF0, 0);
    func_8002D404();
    func_8002F79C();
    for (;;) {
        func_80014840();
        func_80015EDC();
        func_8001AA80(0);
        func_80014C08(0xA);
        func_800168C4();
        func_8001F040();
        D_8008983C = 1;
        func_800149B8(0, -1, 0, 0x800, func_8002B3EC, 2, t);
        func_80014C08(0x7FFFFFFF);
        func_801E055C(0);
        func_8002AEA4(1);
        func_8002B688();
        func_8001B90C(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, func_80015328, 0, 0, 0, 0);
        func_80014C08(0xA);
        func_800149B8(0, -1, 0, 0x400, func_8002F4F4, 0, 0, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(0xA);
    }
}

void func_800157B0(void) {
    u8 param[8];
    Unk80081710 *p;
    s32 i;

    ResetCallback();
    while (CdInit() == 0) {
    }
    VSync(4);
    for (;;) {
        param[0] = 0x80;
        if (CdControlB(0xE, param, 0) != 0) {
            break;
        }
        VSync(0);
    }
    VSync(4);
    func_8005A344(0);
    p = D_80081710;
    for (i = 3; i >= 0; i--, p++) {
        p->unk0 = 0;
    }
    D_800857D0 = 0;
}

s32 func_80015848(s32 arg0) {
    CdFile *f;

    f = func_80015AD8((s8 *)arg0, 0);
    if (f != 0 && func_80015F34(f, 0x4000, (u8 *)&D_800857E0) != 0) {
        func_80015EAC(f);
        D_800857D0 = 1;
        return 0;
    }
    return 1;
}

FileEntry *func_800158B0(CdFile *f, char *name, s32 key) {
    u8 result[8];
    FileEntry *e;
    s32 n;
    s32 i;
    s32 r;

    if (key == 0x80 && *(s32 *)name == 0) {
        return &D_8006DD50;
    }
    for (;;) {
        if (f->remaining <= 0) {
            return 0;
        }
        CdIntToPos(f->sector, f->loc);
        e = (FileEntry *)f->buf;
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            while (CdRead(2, (u8 *)e, 0x80) == 0) {
            }
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        n = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            n = f->remaining + 0x1000;
        }
        for (; n > 0; n -= 0x20, e++) {
            if (e->key == 0) {
                return 0;
            }
            if (e->key == key) {
                for (i = 0; i < 4; i++) {
                    if (e->name[i] != *(s32 *)(name + (i << 2))) {
                        break;
                    }
                }
                if (i == 4) {
                    return e;
                }
            }
        }
    }
}

FileEntry *func_80015A3C(char *name, s32 key) {
    FileEntry *e;
    s32 n;
    s32 i;

    e = (FileEntry *)&D_800857E0;
    for (n = 0x1FF; n >= 0; n--, e++) {
        if (e->key == 0) {
            return 0;
        }
        if (e->key == key) {
            for (i = 0; i < 4; i++) {
                if (e->name[i] != *(s32 *)(name + (i << 2))) {
                    break;
                }
            }
            if (i == 4) {
                return e;
            }
        }
    }
    return 0;
}

CdFile *func_80015AD8(s8 *path, s32 mode) {
    s32 name[4];
    char search[16];
    char *q;
    CdFile *f;
    FileEntry *e;
    s32 i;
    s32 key;
    s32 sector;
    s32 size;
    u8 *p;
    s32 c;

retry:
    f = (CdFile *)D_80081710;
    for (i = 3; i >= 0; i--, f++) {
        if (f->unk0 == 0) {
            break;
        }
    }
    if (i < 0) {
        return 0;
    }
    p = (u8 *)path;
    if (p[1] != ':') {
        if (D_800857D0 == 0) {
            return 0;
        }
        sector = D_800897E0;
        size = 0;
    } else {
        q = search;
        *q++ = '\\';
        *q++ = toupper((s8)*p);
        p += 2;
        func_8002A5B4((s8 *)q, (s8 *)D_80010000);
        if (CdSearchFile(f->loc, search) == 0) {
            return 0;
        }
        size = f->fsize;
        f->remaining = size;
        sector = CdPosToInt(f->loc);
        f->sector = sector;
        if (mode == 0) {
            D_800897E0 = sector;
            D_800897E4 = size;
        }
    }
    for (;;) {
        for (i = 0; i < 4; i++) {
            name[i] = 0;
        }
        key = 0;
        for (i = 0; i < 16; i++) {
            switch (c = *p++) {
            case '.':
                goto ext;
            case 0:
                goto end;
            case '\\':
                goto dir;
            }
            ((u8 *)name)[i] = toupper((s8)c);
        }
        while ((c = *p++) != '.') {
            if (c == 0) {
                goto end;
            }
            if (c == '\\') {
                goto dir;
            }
        }
    ext:
        for (i = 0; i < 24; i += 8) {
            c = *p++;
            if (c == 0) {
                goto end;
            }
            if (c == '\\') {
                goto dir;
            }
            key += toupper((s8)c) << i;
        }
        break;
    dir:
        if (size == 0) {
            size = D_800897E4;
            e = func_80015A3C((char *)name, 0x80);
            if (e == 0) {
                return 0;
            }
        } else {
            e = func_800158B0(f, (char *)name, 0x80);
            if (e == 0) {
                return 0;
            }
        }
        f->sector = sector + e->sector;
        f->remaining = size - (e->sector << 11);
    }
end:
    if (mode == 0) {
        if (size == 0) {
            e = func_80015A3C((char *)name, 0x80);
        } else {
            e = func_800158B0(f, (char *)name, 0x80);
        }
        if (e == 0) {
            return 0;
        }
        f->sector = sector + e->sector;
        f->avail = 0;
        f->remaining = 0x4000;
    } else {
        if (size == 0) {
            size = D_800897E4;
            e = func_80015A3C((char *)name, (key << 8) + 1);
        } else {
            e = func_800158B0(f, (char *)name, (key << 8) + 1);
        }
        if (e == 0) {
            return 0;
        }
        f->sector = sector + e->sector;
        f->remaining = size - (e->sector << 11);
        f->avail = 0;
        if ((f->remaining = f->size = e->size) == 0) {
            goto retry;
        }
        f->unk0 = mode;
    }
    return f;
}

s32 func_80015EAC(CdFile *f) {
    f->unk0 = 0;
    return func_8005A364(0, 0) == 5;
}

int func_80015EDC(void) {
    Unk80081710 *p = D_80081710;
    int i;

    for (i = 3; i >= 0; i--, p++) {
        if (p->unk0 > 0) {
            p->unk0 = 0;
        }
    }
    return func_8005A364(0, 0) == 5;
}

s32 func_80015F34(CdFile *f, s32 size, u8 *dst) {
    u8 result[8];
    u8 result2[8];
    s32 total;
    s32 sectors;
    s32 n;
    s32 r;
    u8 *src;

    total = 0;
    n = f->avail;
    if (n > 0) {
        if (size < n) {
            n = size;
        }
        total = n;
        f->avail -= total;
        src = f->cur;
        size -= total;
        for (n = total - 4; n >= 0; n -= 4) {
            *(s32 *)dst = *(s32 *)src;
            src += 4;
            dst += 4;
        }
        f->cur = src;
    }
    if (f->remaining < size) {
        size = f->remaining;
    }
    if (size <= 0 || f->remaining <= 0) {
        return total;
    }
    sectors = size / 0x800;
    if (sectors > 0) {
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            while (CdRead(sectors, dst, 0x80) == 0) {
            }
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += sectors;
        dst += sectors << 11;
        n = sectors << 11;
        total += n;
        size -= n;
        f->remaining -= n;
    }
    if (size <= 0 || f->remaining <= 0) {
        return total;
    }
    CdIntToPos(f->sector, f->loc);
    do {
        while (CdControlB(2, f->loc, result2) == 0) {
        }
        do {
            f->cur = f->buf;
        } while (CdRead(2, f->buf, 0x80) == 0);
        while ((r = CdReadSync(1, 0)) > 0) {
            func_80014C08(1);
        }
    } while (r != 0);
    f->sector += 2;
    f->avail = 0x1000;
    if ((f->remaining -= 0x1000) < 0) {
        f->avail = f->remaining + 0x1000;
    }
    n = f->avail;
    if (n > 0) {
        if (size < n) {
            n = size;
        }
        total += n;
        f->avail -= n;
        src = f->cur;
        do {
            *(s32 *)dst = *(s32 *)src;
            src += 4;
            dst += 4;
            n -= 4;
        } while (n > 0);
        f->cur = src;
    }
    return total;
}

s32 func_800161D8(CdFile *f) {
    u8 result[8];
    s32 r;

    if (f->avail <= 0) {
        if (f->remaining <= 0) {
            return -1;
        }
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            do {
                f->cur = f->buf;
            } while (CdRead(2, f->buf, 0x80) == 0);
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        f->avail = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            f->avail = f->remaining + 0x1000;
        }
        if (f->avail <= 0) {
            return -1;
        }
    }
    f->avail--;
    return *f->cur++;
}

s32 func_800162F0(CdFile *f) {
    u8 result[8];
    s16 i;
    s16 v;
    s32 r;

    if (f->avail < 2) {
        i = 0;
        v = 0;
        while (f->avail != 0 && i++ < 2) {
            v += *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        if (f->remaining <= 0) {
            return -1;
        }
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            do {
                f->cur = f->buf;
            } while (CdRead(2, f->buf, 0x80) == 0);
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        f->avail = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            f->avail = f->remaining + 0x1000;
        }
        if (f->avail + i < 2) {
            return -1;
        }
        while (f->avail != 0 && i++ < 2) {
            v += *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        return v;
    }
    f->avail -= 2;
    /* sic: undefined order; the original reads one byte twice and advances once */
    return *f->cur++ + (*f->cur++ << 8);
}

s32 func_80016500(CdFile *f) {
    u8 result[8];
    s16 i;
    s32 v;
    s32 w;
    s32 r;

    if (f->avail < 4) {
        i = 0;
        v = 0;
        while (f->avail != 0 && i++ < 4) {
            v |= *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        if (f->remaining <= 0) {
            return -1;
        }
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            do {
                f->cur = f->buf;
            } while (CdRead(2, f->buf, 0x80) == 0);
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        f->avail = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            f->avail = f->remaining + 0x1000;
        }
        if (f->avail + i < 4) {
            return -1;
        }
        while (f->avail != 0 && i++ < 4) {
            v |= *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        return v;
    }
    f->avail -= 4;
    w = (((f->cur[3] << 8) + f->cur[2] << 8) + f->cur[1] << 8) + f->cur[0];
    f->cur += 4;
    return w;
}

s8 *func_80016724(s8 *buf, s32 n, CdFile *f) {
    s8 *p;
    s32 c;

    c = 0;
    p = buf;
    while (--n > 0) {
        c = func_800161D8(f);
        if (c == -1) {
            break;
        }
        if (c == 0) {
            break;
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        if (c == 0x1A) {
            break;
        }
    }
    if (n <= 0 && !(c == -1 || c == 0 || c == '\n' || c == 0x1A)) {
        do {
            c = func_800161D8(f);
        } while (!(c == -1 || c == 0 || c == '\n' || c == 0x1A));
    }
    *p = 0;
    if (*buf == 0) {
        return 0;
    }
    return buf;
}

void func_8001683C(s32 arg0) {
    s32 *var_v0;
    s32 temp_v1;

    var_v0 = &D_80079500;
    if (arg0 != 0) {
loop_1:
        temp_v1 = (*(s32 *)((s8 *)var_v0 + 0));
        if (temp_v1 != arg0) {
            if (temp_v1 != 0) {
                var_v0 += 1;
                goto loop_1;
            }
            (*(s32 *)((s8 *)var_v0 + 0)) = arg0;
            (*(s32 *)((s8 *)var_v0 + 4)) = 0;
        }
    }
}

void func_80016878(s32 arg0) {
    s32 *p;
    s32 v;

    p = &D_80079500;
    if (arg0 == 0) {
        return;
    }
loop:
    v = *p;
    if (v == arg0) {
        goto found;
    }
    p++;
    if (v == 0) {
        return;
    }
    goto loop;
found:
    if ((*p = p[1]) == 0) {
        return;
    }
    p++;
    goto found;
}

void func_800168C4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[0] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[1] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[2] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[3] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[4] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[5] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[6] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[7] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[8] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[9] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[10] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[11] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[12] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[13] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[14] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[15] = 0;
    }
}

void func_80016948(s32 n) {
    PanelPrims *buf;
    PanelPrims *p;
    s16 r[4];
    u32 tpage;
    s32 i;
    s32 k;
    s32 j;

    D_800897EC = n;
    D_800897EE = 0x3C0;
    D_800897F0 = 0x100;
    D_800897F2 = 0x3E0;
    D_800897F4 = 0x1F8;
    buf = func_8001ACEC(D_800897EC * sizeof(PanelPrims) * 2);
    tpage = GetTPage(0, 0, D_800897EE, D_800897F0);
    for (i = 0; i < 2; i++) {
        p = (PanelPrims *)(((Unk800794F8 *)&D_800794F8)->unk98[i].unk40BC = (s32)(buf + D_800897EC * i));
        for (k = 0; k < D_800897EC; k++, p++) {
            for (j = 0; j < 4; j++) {
                func_8001E6EC(0xC, &p->ft4a[j], 0, 0);
                p->ft4a[j].tpage = tpage;
                func_8001E6EC(0xE, &p->linea[j], 0, 0);
                func_8001E6EC(0xE, &p->lineb[j], 0, 0);
                func_8001E6EC(0xE, &p->linec[j], 0, 0);
            }
            for (j = 0; j < 2; j++) {
                func_8001E6EC(0xC, &p->ft4b[j], 0, 0);
                p->ft4b[j].tpage = tpage;
                func_8001E6EC(0xC, &p->ft4c[j], 0, 0);
                p->ft4c[j].tpage = tpage;
            }
            func_8001E6EC(0xE, &p->frame, 0, 0);
            p->frame.u0 = 0;
            p->frame.v0 = 0;
            SetDrawTPage(p->tpage, 0, 1, tpage);
            r[0] = 0;
            r[1] = 0;
            r[2] = 0;
            r[3] = 0;
            SetTexWindow(p->twin, r);
        }
    }
    D_800897E8 = D_800793A0->unk40BC;
}

void func_80016BEC(void) {
    D_800897E8 = D_800793A0->unk40BC;
}

void func_80016C08(void *arg0, void *arg1, s32 arg2, s16 *arg3, s32 a4, s32 a5, s32 a6, s32 a7) {
    Unk80016F38 *w = arg0;
    Rect16 *r = arg1;
    Rect16 *from = (Rect16 *)arg2;
    Rect16 *view = (Rect16 *)arg3;

    if (r == (Rect16 *)-1) {
        w->rect.x = 0;
        w->rect.y = 0;
        w->rect.w = 0;
        w->rect.h = 0;
    } else {
        w->rect = *r;
    }
    if (from == (Rect16 *)-1) {
        w->from.x = w->rect.x + w->rect.w / 2;
        w->from.y = w->rect.y + w->rect.h / 2;
        w->from.w = 0;
        w->from.h = 0;
    } else {
        w->from = *from;
    }
    if (view == (Rect16 *)-1) {
        w->view.x = 0;
        w->view.y = 0;
        w->view.w = w->rect.w;
        w->view.h = w->rect.h;
    } else {
        w->view = *view;
        if (w->view.x < 0) {
            w->view.x = 0;
        }
        if (w->view.y < 0) {
            w->view.y = 0;
        }
        if (w->view.w == 0) {
            w->view.w = w->rect.w;
        }
        if (w->view.h == 0) {
            w->view.h = w->rect.h;
        }
        if (w->view.w < w->rect.w) {
            w->view.w = w->rect.w;
        }
        if (w->view.h < w->rect.h) {
            w->view.h = w->rect.h;
        }
        if (w->view.x >= w->view.w - w->rect.w) {
            w->view.x = w->view.w - w->rect.w;
        }
        if (w->view.y >= w->view.h - w->rect.h) {
            w->view.y = w->view.h - w->rect.h;
        }
    }
    w->cur = w->rect;
    w->delta.x = w->cur.x - w->from.x;
    w->delta.y = w->cur.y - w->from.y;
    w->delta.w = w->cur.w - w->from.w;
    w->delta.h = w->cur.h - w->from.h;
    w->unk0 = w->rect.x - w->view.x;
    w->unk2 = w->rect.y - w->view.y;
    w->unk30[0] = w->view.x;
    w->unk30[1] = w->view.y;
    w->unk30[2] = w->view.x;
    w->unk30[3] = w->view.y;
    w->unk3C = a7;
    w->unk3D = 0;
    w->unk3E = 0;
    w->unk3F = a4;
    w->unk41 = 0;
    w->unk42 = a5;
    if ((u32)a6 > 256) {
        w->unk40 = 0xFF;
    } else {
        w->unk40 = a6;
    }
    w->unk38 = 0;
    w->unk39 = 0;
    if ((a5 >> 4) < 5) {
        w->unk43 = 0;
    } else {
        w->unk43 = 1;
    }
    w->unk2C = 0;
}

void func_80016F38(Unk80016F38 *p, Rect16 *r) {
    s32 dx;
    s32 dy;

    if (r == (Rect16 *)-1) {
        dx = p->cur.w / 2;
        p->delta.x = dx;
        dy = p->cur.h / 2;
        p->delta.y = dy;
        p->delta.w = -p->cur.w;
        p->delta.h = -p->cur.h;
        p->cur.x += dx;
        p->cur.y += dy;
        p->cur.w = 0;
        p->cur.h = 0;
    } else {
        p->delta.x = r->x - p->cur.x;
        p->delta.y = r->y - p->cur.y;
        p->delta.w = r->w - p->cur.w;
        p->delta.h = r->h - p->cur.h;
        p->cur = *r;
    }
    p->unk3D = p->unk3C - p->unk3D;
    if ((s8)p->unk3D < 0) {
        p->unk3D = 0;
    }
    p->unk41 = 0;
}

void func_8001705C(s16 *arg0, s32 arg1, s32 arg2) {
    if (((s8 *)arg0)[0x3E] >= 6) {
        ((s8 *)arg0)[0x3E] = 0;
    }
    if (arg1 > arg0[4] - arg0[8]) {
        arg1 = arg0[4] - arg0[8];
    }
    if (arg2 > arg0[5] - arg0[9]) {
        arg2 = arg0[5] - arg0[9];
    }
    if (arg1 < 0) {
        arg1 = 0;
    }
    if (arg2 < 0) {
        arg2 = 0;
    }
    arg0[0x18] = arg0[2];
    arg0[0x19] = arg0[3];
    arg0[0x1A] = arg1;
    arg0[0x1B] = arg2;
}

s32 func_800170F0(Unk80016F38 *w, void (*draw)(), s32 z) {
    DISPENV env;
    Rect16 r;
    Rect16 r2;
    Rect16 r3;
    Rect16 unused;
    s32 ret;
    s32 kind;
    s32 y;

    w->z = z;
    ret = func_800177E8(w);
    if (w->from.x >= 320) {
        return ret;
    }
    if (w->from.y >= 240) {
        return ret;
    }
    if (w->from.x + w->from.w <= 0) {
        return ret;
    }
    if (w->from.y + w->from.h <= 0) {
        return ret;
    }
    if ((w->from.w | w->from.h) == 0) {
        return ret;
    }
    {
        GetDispEnv(&env);
        r.x = w->from.x + env.disp[0] - 2;
        r.y = w->from.y + env.disp[1] - 1;
        r.w = w->from.w + 4;
        r.h = w->from.h + 2;
        func_800176E4(&r, (Rect16 *)&env);
        r2.x = w->unk0 + w->view.x + env.disp[0] - 2;
        r2.y = w->unk2 + w->view.y + env.disp[1] - 1;
        r2.w = w->rect.w + 4;
        r2.h = w->rect.h + 2;
        if (w->unk3F & 2) {
            r2.w -= 8;
        }
        if (w->unk3F & 4) {
            r2.h -= 8;
        }
        func_800176E4(&r2, &r);
        SetDrawArea((DR_AREA *)&WP->unk264[0x18], (Rect16 *)&env);
        addPrim(&D_800793A0->ot[z], &WP->unk264[0x18]);
        if (w->unk2C != 0 && (w->unk3F & 8) && (kind = D_8006DD70[(w->unk42 >> 4) - 1].label) != 0) {
            switch (kind) {
            case 1:
                r3.x = r.x;
                r3.y = r.y - 7;
                r3.w = r.w;
                r3.h = 5;
                func_80027DB8(w->from.x, w->from.y - 8, w->unk2C, w->unk39, z);
                break;
            case 2:
                r3.x = r.x - 7;
                r3.y = r.y;
                r3.w = 5;
                r3.h = r.h;
                y = w->from.y;
                func_80028228(w->from.x - 9, y + strlen((u8 *)w->unk2C) * 5, w->unk2C, w->unk39, z);
                break;
            case 3:
                r3.x = r.x - 6;
                r3.y = r.y;
                r3.w = 5;
                r3.h = r.h;
                y = w->from.y;
                func_80028228(w->from.x - 8, y + strlen((u8 *)w->unk2C) * 5, w->unk2C, w->unk39, z);
                break;
            case 4:
                r3.x = r.x;
                r3.y = r.y - 10;
                r3.w = r.w;
                r3.h = 5;
                func_80027DB8(w->from.x, w->from.y - 11, w->unk2C, w->unk39, z);
                break;
            }
            func_800176E4(&r3, (Rect16 *)&env);
            SetDrawArea((DR_AREA *)&WP->unk264[0x24], &r3);
            addPrim(&D_800793A0->ot[z], &WP->unk264[0x24]);
        }
        func_80018694(w, z);
        func_80018B8C(w, z);
        SetDrawArea((DR_AREA *)&WP->unk264[0], &r);
        addPrim(&D_800793A0->ot[z], &WP->unk264[0]);
        draw(w, &D_800793A0->ot[z]);
        SetDrawArea((DR_AREA *)&WP->unk264[0xC], &r2);
        addPrim(&D_800793A0->ot[z], &WP->unk264[0xC]);
        func_80017B88(&w->from, w->unk42, w->unk3F & 1, w->unk40, w->unk38, z);
    }
    return ret;
}

void func_800176E4(Rect16 *a, Rect16 *b) {
    if (a->x < b->x) {
        a->w -= b->x - a->x;
        a->x = b->x;
    }
    if (a->y < b->y) {
        a->h -= b->y - a->y;
        a->y = b->y;
    }
    if (a->x + a->w > b->x + b->w) {
        a->w = b->x + b->w - a->x;
    }
    if (a->y + a->h > b->y + b->h) {
        a->h = b->y + b->h - a->y;
    }
}

s32 func_800177E8(Unk80016F38 *w) {
    s32 rem;
    s32 dx;
    s32 dy;
    s32 v;
    s32 ox;
    s32 oy;

    rem = (s8)w->unk3C - (s8)w->unk3D;
    w->unk41 = 0;
    v = w->delta.x;
    if (v < 0) {
        w->from.x = w->cur.x - v * rem / (s8)w->unk3C;
    } else {
        dx = v * rem - 1;
        w->from.x = w->cur.x - (dx + (s8)w->unk3C) / (s8)w->unk3C;
    }
    v = w->delta.y;
    if (v < 0) {
        w->from.y = w->cur.y - v * rem / (s8)w->unk3C;
    } else {
        dy = v * rem - 1;
        w->from.y = w->cur.y - (dy + (s8)w->unk3C) / (s8)w->unk3C;
    }
    w->from.w = w->cur.w - w->delta.w * rem / (s8)w->unk3C;
    w->from.h = w->cur.h - w->delta.h * rem / (s8)w->unk3C;
    if ((s8)w->unk3E < 6) {
        w->view.x = w->unk30[0] + (w->unk30[2] - w->unk30[0]) * (s8)w->unk3E / 6;
        w->view.y = w->unk30[1] + (w->unk30[3] - w->unk30[1]) * (s8)w->unk3E / 6;
        w->unk3E++;
    } else {
        w->view.x = w->unk30[2];
        w->view.y = w->unk30[3];
    }
    ox = w->delta.w * rem / (s8)w->unk3C / 2;
    if (ox < 0) {
        ox = abs(w->delta.w * (s8)w->unk3D / (s8)w->unk3C) / 2;
    }
    ox += w->view.x;
    oy = w->delta.h * rem / (s8)w->unk3C / 2;
    if (oy < 0) {
        oy = abs(w->delta.h * (s8)w->unk3D / (s8)w->unk3C) / 2;
    }
    oy += w->view.y;
    w->unk0 = w->from.x - ox;
    w->unk2 = w->from.y - oy;
    w->unk3D += D_800794F0;
    if ((s8)w->unk3D > (s8)w->unk3C) {
        w->unk3D = w->unk3C;
        w->unk41 = 1;
    }
    return w->unk41;
}

void func_80017B88(Rect16 *r, u8 style, s32 semi, s32 col, s32 pal, s32 z) {
    Rect16 uv[4];
    Rect16 st[4];
    u16 clut;
    u32 *ot;
    s32 k;
    s32 i;

    clut = getClut(D_800897F2 + (pal % 2) * 16, D_800897F4 + pal / 2);
    if (func_80019084() != 0) {
        return;
    }
    if (r->x < 320 && r->y < 240 && r->x + r->w > 0 && r->y + r->h > 0) {
        ot = &D_800793A0->ot[z];
        if (style & 0xF0) {
            k = (style >> 4) - 1;
            func_8001EA64(&WP->ft4a[0], (s8)D_8006DD70[k].left + r->x,
                          (s8)D_8006DD70[k].top + r->y - D_8006DD70[k].h[0],
                          r->w - ((s8)D_8006DD70[k].left - (s8)D_8006DD70[k].right), D_8006DD70[k].h[0]);
            func_8001EA64(&WP->ft4a[1], (s8)D_8006DD70[k].left + r->x,
                          (s8)D_8006DD70[k].bottom + r->y + r->h,
                          r->w - ((s8)D_8006DD70[k].left - (s8)D_8006DD70[k].right), D_8006DD70[k].h[1]);
            func_8001EA64(&WP->ft4a[2], (s8)D_8006DD70[k].left + r->x - D_8006DD70[k].w[0],
                          (s8)D_8006DD70[k].top + r->y, D_8006DD70[k].w[0],
                          r->h - ((s8)D_8006DD70[k].top - (s8)D_8006DD70[k].bottom));
            func_8001EA64(&WP->ft4a[3], (s8)D_8006DD70[k].right + r->x + r->w,
                          (s8)D_8006DD70[k].top + r->y, D_8006DD70[k].w[1],
                          r->h - ((s8)D_8006DD70[k].top - (s8)D_8006DD70[k].bottom));
            WP->linea[0].x0 = (r->x - D_8006DD70[k].w[0]) + (s8)D_8006DD70[k].left;
            WP->linea[0].y0 = (r->y - D_8006DD70[k].h[0]) + (s8)D_8006DD70[k].top;
            WP->linea[1].x0 = (r->x + r->w) + (s8)D_8006DD70[k].right;
            WP->linea[1].y0 = (r->y - D_8006DD70[k].h[0]) + (s8)D_8006DD70[k].top;
            WP->linea[2].x0 = (r->x - D_8006DD70[k].w[0]) + (s8)D_8006DD70[k].left;
            WP->linea[2].y0 = (r->y + r->h) + (s8)D_8006DD70[k].bottom;
            WP->linea[3].x0 = (r->x + r->w) + (s8)D_8006DD70[k].right;
            WP->linea[3].y0 = (r->y + r->h) + (s8)D_8006DD70[k].bottom;
            uv[0].x = D_8006DD70[k].u[1];
            uv[0].y = D_8006DD70[k].v[0];
            uv[0].w = 0;
            uv[0].h = D_8006DD70[k].h[0];
            uv[1].x = D_8006DD70[k].u[1];
            uv[1].y = D_8006DD70[k].v[2];
            uv[1].w = 0;
            uv[1].h = D_8006DD70[k].h[1];
            uv[2].x = D_8006DD70[k].u[0];
            uv[2].y = D_8006DD70[k].v[1];
            uv[2].w = D_8006DD70[k].w[0];
            uv[2].h = 0;
            uv[3].x = D_8006DD70[k].u[2];
            uv[3].y = D_8006DD70[k].v[1];
            uv[3].w = D_8006DD70[k].w[1];
            uv[3].h = 0;
            st[0].x = D_8006DD70[k].u[0];
            st[0].y = D_8006DD70[k].v[0];
            st[0].w = D_8006DD70[k].w[0];
            st[0].h = D_8006DD70[k].h[0];
            st[1].x = D_8006DD70[k].u[2];
            st[1].y = D_8006DD70[k].v[0];
            st[1].w = D_8006DD70[k].w[1];
            st[1].h = D_8006DD70[k].h[0];
            st[2].x = D_8006DD70[k].u[0];
            st[2].y = D_8006DD70[k].v[2];
            st[2].w = D_8006DD70[k].w[0];
            st[2].h = D_8006DD70[k].h[1];
            st[3].x = D_8006DD70[k].u[2];
            st[3].y = D_8006DD70[k].v[2];
            st[3].w = D_8006DD70[k].w[1];
            st[3].h = D_8006DD70[k].h[1];

            for (i = 0; i < 4; i++) {
                func_8001EC3C((u8 *)&WP->ft4a[i], uv[i].x, uv[i].y, uv[i].w, uv[i].h);
                setRGB0(&WP->ft4a[i], col, col, col);
                WP->ft4a[i].clut = clut;
                addPrim(ot, &WP->ft4a[i]);
                setUV0(&WP->linea[i], st[i].x, st[i].y);
                setWH(&WP->linea[i], st[i].w, st[i].h);
                setRGB0(&WP->linea[i], col, col, col);
                WP->linea[i].clut = clut;
                addPrim(ot, &WP->linea[i]);
            }
        }
        addPrim(ot, WP->twin);
        if (style & 0xF) {
            k = (style & 0xF) - 1;
            WP->frame.x0 = r->x - 2;
            WP->frame.y0 = r->y - 2;
            WP->frame.w = r->w + 4;
            WP->frame.h = r->h + 4;
            setSemiTrans(&WP->frame, semi);
            setRGB0(&WP->frame, col, col, col);
            WP->frame.clut = clut;
            SetTexWindow(WP->unk10C, (s16 *)&D_8006DDE8[k]);
            addPrim(ot, &WP->frame);
        }
        addPrim(ot, WP->unk10C);
        addPrim(ot, WP->tpage);
        D_800897E8 += sizeof(PanelPrims);
    }
}

void func_80018694(Unk80016F38 *w, s32 z) {
    u16 clut;
    s32 x;
    s32 y;
    s32 len;
    s32 pos;
    s32 knob;
    s32 i;
    s32 end;
    s32 off;
    s32 top;

    clut = getClut(D_800897F2 + (w->unk38 % 2) * 16, D_800897F4 + w->unk38 / 2);
    if (!(w->unk3F & 2) || w->rect.h >= w->view.h) {
        return;
    }
    x = w->unk0 + w->view.x + w->rect.w - 8;
    y = w->unk2 + w->view.y;
    len = w->rect.h - 0x10;
    if (w->unk3F & 4) {
        len -= 8;
    }
    pos = w->view.y * len / w->view.h;
    knob = w->rect.h * len - 1;
    knob = (knob + w->view.h) / w->view.h;
    if (pos + knob < 0) {
        pos = 0;
    }
    if (pos + knob > len) {
        pos = len - knob;
    }
    WP->lineb[0].x0 = x;
    WP->lineb[0].y0 = y;
    WP->lineb[1].x0 = x;
    end = len + 8;
    WP->lineb[1].y0 = y + end;
    WP->lineb[2].x0 = x;
    off = pos + 8;
    top = y + off;
    WP->lineb[2].y0 = top;
    WP->lineb[3].x0 = x;
    WP->lineb[3].y0 = top + knob - 2;
    func_8001EA64(&WP->ft4b[0], x, top + 2, 8, knob - 4);
    func_8001EA64(&WP->ft4b[1], x, y + 8, 8, len);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->lineb[i], D_8006DE48[i + w->unk43 * 4].x, D_8006DE48[i + w->unk43 * 4].y);
        setWH(&WP->lineb[i], D_8006DE48[i + w->unk43 * 4].w, D_8006DE48[i + w->unk43 * 4].h);
        setRGB0(&WP->lineb[i], w->unk40, w->unk40, w->unk40);
        WP->lineb[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->lineb[i]);
    }
    for (i = 0; i < 2; i++) {
        func_8001EC3C((u8 *)&WP->ft4b[i], D_8006DE28[i + w->unk43 * 2].x, D_8006DE28[i + w->unk43 * 2].y, D_8006DE28[i + w->unk43 * 2].w, D_8006DE28[i + w->unk43 * 2].h);
        setRGB0(&WP->ft4b[i], w->unk40, w->unk40, w->unk40);
        WP->ft4b[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->ft4b[i]);
    }
}

void func_80018B8C(Unk80016F38 *w, s32 z) {
    u16 clut;
    s32 x;
    s32 y;
    s32 len;
    s32 pos;
    s32 knob;
    s32 i;
    s32 end;
    s32 off;
    s32 left;

    clut = getClut(D_800897F2 + (w->unk38 % 2) * 16, D_800897F4 + w->unk38 / 2);
    if (!(w->unk3F & 4) || w->rect.w >= w->view.w) {
        return;
    }
    x = w->unk0 + w->view.x;
    y = w->unk2 + w->view.y + w->rect.h - 8;
    len = w->rect.w - 0x10;
    if (w->unk3F & 2) {
        len -= 8;
    }
    pos = w->view.x * len / w->view.w;
    knob = w->rect.w * len - 1;
    knob = (knob + w->view.w) / w->view.w;
    if (pos + knob < 0) {
        pos = 0;
    }
    if (pos + knob > len) {
        pos = len - knob;
    }
    WP->linec[0].x0 = x;
    WP->linec[0].y0 = y;
    end = len + 8;
    WP->linec[1].x0 = x + end;
    WP->linec[1].y0 = y;
    off = pos + 8;
    left = x + off;
    WP->linec[2].x0 = left;
    WP->linec[2].y0 = y;
    WP->linec[3].x0 = left + knob - 2;
    WP->linec[3].y0 = y;
    func_8001EA64(&WP->ft4c[0], left + 2, y, knob - 4, 8);
    func_8001EA64(&WP->ft4c[1], x + 8, y, len, 8);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->linec[i], D_8006DEA8[i + w->unk43 * 4].x, D_8006DEA8[i + w->unk43 * 4].y);
        setWH(&WP->linec[i], D_8006DEA8[i + w->unk43 * 4].w, D_8006DEA8[i + w->unk43 * 4].h);
        setRGB0(&WP->linec[i], w->unk40, w->unk40, w->unk40);
        WP->linec[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->linec[i]);
    }
    for (i = 0; i < 2; i++) {
        func_8001EC3C((u8 *)&WP->ft4c[i], D_8006DE88[i + w->unk43 * 2].x, D_8006DE88[i + w->unk43 * 2].y, D_8006DE88[i + w->unk43 * 2].w, D_8006DE88[i + w->unk43 * 2].h);
        setRGB0(&WP->ft4c[i], w->unk40, w->unk40, w->unk40);
        WP->ft4c[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->ft4c[i]);
    }
}

int func_80019084(void) {
    if (D_800897E8 == D_800793A0->unk40BC + D_800897EC * 0x294) {
        printf(D_80010008);
        return -1;
    }
    return 0;
}

void func_800190F4(Unk800190F4 *p, Rect16 *arg1, Bytes4 *arg2) {
    s32 i;

    for (i = 0; i < 2; i++) {
        setDrawMode(&p->dm[i], 0, 0, GetTPage(0, 1, 0, 0));
        func_8001E6EC(0x11, &p->prim[i], 1, 0);
    }
    func_800191C0(p, arg1, arg2);
}

void func_800191C0(Unk800190F4 *p, Rect16 *r, Bytes4 *c) {
    if (r == (Rect16 *)-1) {
        p->unk30.x = 0;
        p->unk30.y = 0;
        p->unk30.w = 0;
        p->unk30.h = 0;
    } else {
        p->unk30 = *r;
    }
    p->unk38 = p->unk30;
    p->unk40 = p->unk30;
    if (c == (Bytes4 *)-1) {
        p->unk48.b[0] = 0;
        p->unk48.b[1] = 0;
        p->unk48.b[2] = 0x80;
    } else {
        p->unk48 = *c;
    }
    p->unk4C = 0x80;
    p->unk4D = 0;
}

void func_80019280(Unk800190F4 *p, Rect16 *r) {
    if (p->unk4D >= 6) {
        p->unk4D = 0;
    }
    p->unk30 = p->unk40;
    p->unk38 = *r;
}

void func_800192E0(void *arg0, Bytes4 *arg1) {
    *(Bytes4 *)((s8 *)arg0 + 0x48) = *arg1;
}

void func_800192FC(Unk800190F4 *p, s32 ot) {
    if (p->unk4D < 6) {
        p->unk40.x = p->unk30.x + (p->unk38.x - p->unk30.x) * p->unk4D / 6;
        p->unk40.y = p->unk30.y + (p->unk38.y - p->unk30.y) * p->unk4D / 6;
        p->unk40.w = p->unk30.w + (p->unk38.w - p->unk30.w) * p->unk4D / 6;
        p->unk40.h = p->unk30.h + (p->unk38.h - p->unk30.h) * p->unk4D / 6;
        p->unk4D++;
    } else {
        p->unk40 = p->unk38;
    }
    (p->prim + D_800794F4)->x0 = p->unk40.x - 2;
    (p->prim + D_800794F4)->y0 = p->unk40.y - 1;
    (p->prim + D_800794F4)->w = p->unk40.w + 4;
    (p->prim + D_800794F4)->h = p->unk40.h + 2;
    (p->prim + D_800794F4)->r0 = p->unk48.b[0] * p->unk4C / 128;
    (p->prim + D_800794F4)->g0 = p->unk48.b[1] * p->unk4C / 128;
    (p->prim + D_800794F4)->b0 = p->unk48.b[2] * p->unk4C / 128;
    addPrim(&D_800793A0->ot[ot], &p->prim[D_800794F4]);
    addPrim(&D_800793A0->ot[ot], &p->dm[D_800794F4]);
}

void func_8001963C(void *arg0, void *arg1, Unk800190F4 *arg2, Bytes4 *arg3) {
    s16 r[4];

    (*(void **)((s8 *)arg0 + 0)) = arg1;
    (*(Unk800190F4 **)((s8 *)arg0 + 4)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x12)) = -1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = -1;
    (*(s8 *)((s8 *)arg0 + 0x26)) = 1;
    (*(s8 *)((s8 *)arg0 + 0x27)) = 0;
    r[0] = 0;
    r[1] = (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25)) - ((*(s16 *)((s8 *)arg0 + 0xE)) - (*(u8 *)((s8 *)arg0 + 0x25))) / 2;
    r[2] = (*(u8 *)((s8 *)arg0 + 0x24)) * (*(s16 *)((s8 *)arg0 + 0x1E));
    r[3] = (*(u8 *)((s8 *)arg0 + 0x25)) * (*(s16 *)((s8 *)arg0 + 0x20));
    func_80016C08(arg1, (s8 *)arg0 + 8, -1, r, (*(u8 *)((s8 *)arg0 + 0x18)), (*(u8 *)((s8 *)arg0 + 0x19)), 0x80, 0xC);
    r[0] = (*(u8 *)((s8 *)arg0 + 0x22)) + (*(u16 *)((s8 *)arg1 + 0));
    r[1] = (*(u8 *)((s8 *)arg0 + 0x23)) + (*(u16 *)((s8 *)arg1 + 2)) + (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25));
    r[2] = (*(u16 *)((s8 *)arg0 + 0x1A));
    r[3] = (*(u16 *)((s8 *)arg0 + 0x1C));
    func_800190F4(arg2, (Rect16 *)r, arg3);
}

void func_800197AC(void *arg0) {
    s16 r[4];
    void *img;

    img = (*(void **)((s8 *)arg0 + 0));
    func_8001705C(img, 0, (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25)) - ((*(s16 *)((s8 *)arg0 + 0xE)) - (*(u8 *)((s8 *)arg0 + 0x25))) / 2);
    r[0] = ((*(u16 *)((s8 *)img + 0xC)) - (*(u16 *)((s8 *)img + 0x34))) + (*(u8 *)((s8 *)arg0 + 0x22)) + (*(s16 *)((s8 *)arg0 + 0x10)) * (*(u8 *)((s8 *)arg0 + 0x24));
    r[1] = ((*(u16 *)((s8 *)img + 0xE)) - (*(u16 *)((s8 *)img + 0x36))) + (*(u8 *)((s8 *)arg0 + 0x23)) + (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25));
    r[2] = (*(u16 *)((s8 *)arg0 + 0x1A));
    r[3] = (*(u16 *)((s8 *)arg0 + 0x1C));
    func_80019280(*(Unk800190F4 **)((s8 *)arg0 + 4), (Rect16 *)r);
}

s32 func_800198A8(Menu *m) {
    Unk80016F38 *w;
    Unk800190F4 *c;
    s16 r[4];

    w = m->win;
    c = m->cursor;
    m->moved = 0;
    if (m->active != 0) {
        c->unk4C = 0x80;
        if (m->nrows >= 2 && m->rowH != 0) {
            if (D_80089840[m->pad]->unkE & 0x1000) {
                func_8002BB58(2);
                m->moved = 1;
                if (--m->row < 0) {
                    func_8001705C((s16 *)w, 0, w->view.h - w->rect.h);
                    m->row = m->nrows - 1;
                } else {
                if (m->row == 0) {
                    D_80089840[m->pad]->unk10 = 0;
                }
                if (m->row * m->rowH < w->unk30[3]) {
                    func_8001705C((s16 *)w, 0, m->row * m->rowH);
                }
                }
            } else if (D_80089840[m->pad]->unkE & 0x4000) {
                func_8002BB58(2);
                m->moved = 1;
                if (++m->row >= m->nrows) {
                    func_8001705C((s16 *)w, 0, 0);
                    m->row = 0;
                } else {
                if (m->row == m->nrows - 1) {
                    D_80089840[m->pad]->unk10 = 0;
                }
                if (m->row * m->rowH >= w->unk30[3] + w->rect.h) {
                    func_8001705C((s16 *)w, 0, (m->row + 1) * m->rowH - w->rect.h);
                }
                }
            } else if (D_80089840[m->pad]->unkE & 0x1) {
                if (m->row != 0) {
                    func_8002BB58(2);
                }
                m->moved = 1;
                m->row -= (w->rect.h + m->rowH - 1) / m->rowH;
                if (m->row < 0) {
                    D_80089840[m->pad]->unk10 = 0;
                    func_8001705C((s16 *)w, 0, 0);
                    m->row = 0;
                } else {
                    if (m->row == 0) {
                        D_80089840[m->pad]->unk10 = 0;
                    }
                    func_8001705C((s16 *)w, 0, w->unk30[3] - (w->rect.h + m->rowH - 1) / m->rowH * m->rowH);
                }
            } else if (D_80089840[m->pad]->unkE & 0x2) {
                if (m->row != m->nrows - 1) {
                    func_8002BB58(2);
                }
                m->moved = 1;
                m->row += (w->rect.h + m->rowH - 1) / m->rowH;
                if (m->row >= m->nrows) {
                    D_80089840[m->pad]->unk10 = 0;
                    func_8001705C((s16 *)w, 0, w->view.h - w->rect.h);
                    m->row = m->nrows - 1;
                } else {
                    if (m->row == m->nrows - 1) {
                        D_80089840[m->pad]->unk10 = 0;
                    }
                    func_8001705C((s16 *)w, 0, w->unk30[3] + (w->rect.h + m->rowH - 1) / m->rowH * m->rowH);
                }
            }
        }
    } else {
        c->unk4C = 0x40;
    }
    if (m->row != m->prevRow || m->col != m->prevCol) {
        m->prevCol = m->col;
        m->prevRow = m->row;
        r[0] = (w->rect.x - w->unk30[2]) + m->ox + m->col * m->colW;
        r[1] = (w->rect.y - w->unk30[3]) + m->oy + m->row * m->rowH;
        r[2] = m->cw;
        r[3] = m->ch;
        func_80019280(m->cursor, (Rect16 *)r);
    }
    func_800192FC(c, w->z);
    return m->col + m->row * m->ncols;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010000);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010008);

void func_80019EA4(u8 *w, u8 *text, u32 flags) {
    Rect16 r;
    s32 m;
    s32 wd;
    s32 x;
    s32 c;

    w[0xA4] = flags & 0xF;
    w[0xB6] = flags & 0x80;
    *(u8 **)(w + 0x94) = text;
    func_800293FC(text);
    w[0xA7] = (D_801D6B18 + 1) / 2;
    *(s16 *)(w + 0xA8) = (D_801D6B18 + 1) / 2 * 2 + 4;
    *(s16 *)(w + 0xAA) = (D_801D6B1C + 1) / 2 * 2 + 4;
    if (w[0xA4] != 0) {
        if (text == 0) {
            *(s16 *)(w + 0xAA) = 0x10;
        } else {
            *(s16 *)(w + 0xAA) += 0x10;
        }
    }
    if (w[0xA4] != 2) {
        *(char **)(w + 0x98) = "Yes";
        *(char **)(w + 0x9C) = "No";
    }
    *(s16 *)(w + 0xAE) = strlen(*(u8 **)(w + 0x98)) * 6;
    *(s16 *)(w + 0xB2) = strlen(*(u8 **)(w + 0x9C)) * 6;
    m = *(s16 *)(w + 0xAE);
    if (m < *(s16 *)(w + 0xB2)) {
        m = *(s16 *)(w + 0xB2);
    }
    m = m * 2 + 0x10;
    if (*(s16 *)(w + 0xA8) < m) {
        *(s16 *)(w + 0xA8) = m;
    }
    wd = *(s16 *)(w + 0xA8);
    x = (320 - wd) / 2;
    c = x + wd / 2;
    *(s16 *)(w + 0xAC) = c - (*(s16 *)(w + 0xAE) + 4);
    *(s16 *)(w + 0xB0) = c + 4;
    r.x = x;
    r.y = (240 - *(s16 *)(w + 0xAA)) / 2;
    r.w = *(s16 *)(w + 0xA8);
    r.h = *(s16 *)(w + 0xAA);
    func_80016C08(w, &r, -1, (s16 *)-1, 8, 0x77, 0x80, 8);
    w[0x38] = 4;
    if (w[0xA4] != 0) {
        func_800190F4((Unk800190F4 *)(w + 0x44), (Rect16 *)-1, (Bytes4 *)-1);
    }
    w[0xA5] = 2;
    w[0xA6] = 0;
    *(s32 *)(w + 0xA0) = 0;
    w[0xB5] = 0;
    w[0xB4] = 0;
}

s8 func_8001A100(void *arg0) {
    func_800149B8(0, -1, 0, 0x400, &func_8001A1D8, arg0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (*(s8 *)((s8 *)arg0 + 0xA5));
}

s32 func_8001A164(s32 *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = func_800148B0();
    (*(s8 *)((s8 *)arg0 + 0xA6)) = (s8) arg1;
    func_800149B8(0, -1, 0, 0x400, func_8001A1D8, arg0, temp_v0, 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (s32) (*(s8 *)((s8 *)arg0 + 0xA5));
}

void func_8001A1D8(u8 *w, s32 arg1) {
    Rect16 r;
    s32 x;
    s32 width;
    u16 mask;
    PadState **pads;
    s32 on;

    D_8008983C = 0;
    if ((s8)w[0xA5] == 1) {
        x = *(s16 *)(w + 0xAC);
        width = *(s16 *)(w + 0xAE);
    } else {
        x = *(s16 *)(w + 0xB0);
        width = *(s16 *)(w + 0xB2);
    }
    r.x = x;
    r.y = (240 - *(s16 *)(w + 0xAA)) / 2 + *(s16 *)(w + 0xAA) - 14;
    r.w = width;
    r.h = 12;
    func_800191C0((Unk800190F4 *)(w + 0x44), &r, (Bytes4 *)-1);
    pads = D_80089840;
    on = 1;
    do {
        func_80014C08(D_800794F0);
        func_800170F0((Unk80016F38 *)w, func_8001A40C, 0);
        if (*(void (**)(void))(w + 0xA0) != 0) {
            (*(void (**)(void))(w + 0xA0))();
        }
        if (w[0xA4] != 0) {
            if (w[0xB5] != 0) {
                mask = 0x40;
            } else {
                mask = 0x50;
            }
        } else {
            mask = 0x40;
        }
        if (pads[w[0xA6]]->unk2 & mask) {
            break;
        }
    } while (w[0xB4] == 0);
    if (w[0xB4] != 0) {
        w[0xA5] = 3;
    } else if (pads[w[0xA6]]->unk2 & 0x10) {
        w[0xA5] = 0;
        w[0xB4] = on;
        func_8002BB58(0);
    } else {
        w[0xB4] = on;
        func_8002BB58(1);
    }
    func_80016F38((Unk80016F38 *)w, (Rect16 *)-1);
    do {
        func_80014C08(D_800794F0);
        func_800170F0((Unk80016F38 *)w, func_8001A40C, 0);
        if (*(void (**)(void))(w + 0xA0) != 0) {
            (*(void (**)(void))(w + 0xA0))();
        }
    } while (*(s8 *)(w + 0x41) == 0);
    D_8008983C = 1;
    func_80014A48(arg1, (s8)w[0xA5]);
    func_80014A90();
}

void func_8001A40C(u8 *w) {
    Rect16 r;
    s32 x;
    s32 y;

    x = *(s16 *)w + (*(s16 *)(w + 0xA8) - w[0xA7] * 2) / 2;
    y = *(s16 *)(w + 2) + 2;
    if (*(s32 *)(w + 0x94) != 0) {
        func_80028D18(x, y, *(s32 *)(w + 0x94), 7, *(s16 *)(w + 0x3A));
    }
    y = *(s16 *)(w + 2) + *(s16 *)(w + 0xAA) - 0xE;
    if (w[0xA4] != 0) {
        if (w[0xB4] == 0) {
            if ((D_80089840[w[0xA6]]->unk2 & 0x8000) && (s8)w[0xA5] != 1) {
                w[0xA5] = 1;
                r.x = *(s16 *)(w + 0xAC);
                r.y = y;
                r.w = *(s16 *)(w + 0xAE);
                r.h = 0xC;
                func_80019280((Unk800190F4 *)(w + 0x44), &r);
                func_8002BB58(2);
            }
            if ((D_80089840[w[0xA6]]->unk2 & 0x2000) && (s8)w[0xA5] != 2) {
                w[0xA5] = 2;
                r.x = *(s16 *)(w + 0xB0);
                r.y = y;
                r.w = *(s16 *)(w + 0xB2);
                r.h = 0xC;
                func_80019280((Unk800190F4 *)(w + 0x44), &r);
                func_8002BB58(2);
            }
        }
        func_80028D18(*(s16 *)(w + 0xAC), y, *(s32 *)(w + 0x98), 7, *(s16 *)(w + 0x3A));
        func_80028D18(*(s16 *)(w + 0xB0), y, *(s32 *)(w + 0x9C), 7, *(s16 *)(w + 0x3A));
        func_800192FC((Unk800190F4 *)(w + 0x44), *(s16 *)(w + 0x3A));
    }
}

void func_8001A600(void) {
    s32 i;
    u8 *buf;

    D_8008983C = 1;
    buf = func_8001ACEC(0x3C);
    for (i = 0; i < 2; i++) {
        D_80089840[i] = (PadState *)(buf + i * 0x1E);
    }
    func_8001A6B0();
    PadInitDirect(&D_800897F8, (s8 *)&D_800897F8 + 0x22);
    PadStartCom();
}

void func_8001A688(s32 arg0, s16 arg1, s16 arg2) {
    D_80089840[arg0]->repeatDelay = arg1;
    D_80089840[arg0]->repeatRate = arg2;
}

void func_8001A6B0(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        D_80089840[i]->unk10 = 1;
        D_80089840[i]->unk11 = 0;
        D_80089840[i]->unk12 = 0;
        D_80089840[i]->unk1A = 0;
        D_80089840[i]->unk1B = 0;
        D_80089840[i]->unk0 = 0;
        D_80089840[i]->unk6 = 0;
        D_80089840[i]->unk4 = 0;
        D_80089840[i]->unk2 = 0;
        D_80089840[i]->unk8 = 0;
        D_80089840[i]->unkE = 0;
        D_80089840[i]->unkC = 0;
        D_80089840[i]->unkA = 0;
        func_8001A688(i, 0x1E, 2);
    }
}

s32 func_8001A7A4(s32 port, PadState *pad, u8 *buf) {
    s32 skip;
    s32 changed;
    u16 pressed;
    s32 cur;
    s16 t;

    if (buf[1] == 0x80) {
        pad->unk0 = 0;
        pad->unk6 = 0;
        pad->unk4 = 0;
        pad->unk2 = 0;
        return 0;
    }
    pad->unk1A = PadGetState(port);
    pad->unk1B = PadInfoMode(port, 1, 0);
    pad->unk1C = PadInfoMode(port, 2, 0);
    if (pad->unk1A == 0 || buf[0] != 0) {
        pad->unk0 = 0;
        pad->unk6 = 0;
        pad->unk4 = 0;
        pad->unk2 = 0;
        return 0;
    }
    skip = 0;
    switch (pad->unk1B) {
    case 1:
    case 3:
    case 6:
        skip = 1;
        break;
    case 2:
    case 4:
    case 5:
    case 7:
        break;
    }
    if (skip) {
        return 0;
    }
    changed = pad->unk0;
    pad->unk0 = ~((buf[2] << 8) | buf[3]);
    changed ^= pad->unk0;
    pressed = changed & pad->unk0;
    pad->unk2 = pressed;
    pad->unk4 = changed & ~pad->unk0;
    pad->unk6 = pressed;
    if (pad->unk10) {
        cur = pad->unk0;
        if (cur == pad->unk14 && cur != 0) {
            t = pad->unk12;
            pad->unk12 = t + ((Unk800794F8 *)&D_800794F8)->unk50;
            if (pad->unk11 == 0) {
                if (pad->unk12 < pad->repeatDelay) {
                    return 0;
                }
                if (t != 0) {
                    pad->unk11 = 1;
                    pad->unk12 = 0;
                    pad->unk6 = pressed | cur;
                }
            } else {
                if (pad->unk12 < pad->repeatRate) {
                    return 0;
                }
                pad->unk12 = 0;
                pad->unk6 |= cur;
            }
        } else {
            pad->unk14 = cur;
            pad->unk11 = 0;
            pad->unk12 = 0;
        }
    } else if (pad->unk4) {
        pad->unk10 = 1;
    }
    return 0;
}

void func_8001A9B0(void) {
    s32 var_s1;
    void *temp_s0;

    var_s1 = 0;
    do {
        temp_s0 = D_80089840[var_s1];
        func_8001A7A4(var_s1 * 0x10, temp_s0, (u8 *)&D_800897F8 + var_s1 * 0x22);
        if (D_8008983C != 0) {
            (*(u16 *)((s8 *)temp_s0 + 8)) = (u16) (*(u16 *)((s8 *)temp_s0 + 0));
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = (u16) (*(u16 *)((s8 *)temp_s0 + 6));
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = (u16) (*(u16 *)((s8 *)temp_s0 + 4));
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = (u16) (*(u16 *)((s8 *)temp_s0 + 2));
        } else {
            (*(u16 *)((s8 *)temp_s0 + 8)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = 0U;
        }
        var_s1 += 1;
    } while (var_s1 < 2);
}

void func_8001AA80(s32 arg0) {
    s32 *p;
    s32 i;

    if (arg0 != 0) {
        p = &D_80089848;
        p[0] = (s32)&D_8008C848 & 0x3FFFFFFF;
        p[1] = 0x148000;
        p[2] = -1;
        i = 0x3FF;
        do {
            p += 3;
            p[0] = 0;
            p[1] = 0;
            i--;
            p[2] = 0;
        } while (i > 0);
        return;
    }
    p = &D_80089848;
    i = 0x3FF;
    if (p[0] != 0) {
loop:
        if (p[0] < 0 && p[2] >= 0) {
            if (func_8001AE90((void *)p[0]) == 0) {
                goto loop;
            }
        }
        i--;
        p += 3;
        if (i >= 0 && p[0] != 0) {
            goto loop;
        }
    }
}

s32 func_8001AB64(void) {
    s32 *p;
    s32 i;
    s32 max;

    max = 0;
    p = &D_80089848;
    i = 0x3FF;
    if (D_80089848 != 0) {
        do {
            if (p[0] > 0 && max < p[1]) {
                max = p[1];
            }
            i--;
            p += 3;
        } while (i >= 0 && p[0] != 0);
    }
    return max;
}

void *func_8001ABCC(s32 size, s32 tag) {
    s32 *p;
    s32 *q;
    s32 i;
    s32 addr;
    s32 avail;
    s32 res;

    size = (size + 3) & ~3;
    if (size == 0) {
        return 0;
    }
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr >= 0) {
                avail = p[1];
                if (avail >= size) {
                    res = addr | 0x80000000;
                    p[0] = res;
                    p[1] = size;
                    avail -= size;
                    p[2] = tag;
                    if (avail != 0) {
                        addr += size;
                        q = &D_80089848 + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            q[0] = q[-3];
                            q[1] = q[-2];
                            q[2] = q[-1];
                            q -= 3;
                        }
                        q[0] = addr;
                        q[1] = avail;
                        q[2] = -1;
                    }
                    func_800149A0();
                    return (void *)res;
                }
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

void *func_8001ACEC(s32 arg0) {
    return func_8001ABCC(arg0, -2);
}

void *func_8001AD0C(s32 size) {
    return func_8001ABCC(size, func_800148B0());
}

void *func_8001AD3C(void *ptr, s32 size) {
    s32 *p;
    s32 i;
    s32 addr;
    s32 rest;

    size = (size + 3) & ~3;
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr == (s32)ptr) {
                rest = p[1] - size;
                if (rest < 0) {
                    func_800149A0();
                    return 0;
                }
                if (rest != 0 && i != 0) {
                    p[1] = size;
                    p += 3;
                    addr += size;
                    if (p[0] > 0) {
                        rest += p[1];
                    } else {
                        p = &D_80089848 + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            p[0] = p[-3];
                            p[1] = p[-2];
                            p[2] = p[-1];
                            p -= 3;
                        }
                    }
                    p[0] = addr & 0x3FFFFFFF;
                    p[1] = rest;
                    p[2] = -1;
                }
                func_800149A0();
                return ptr;
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

void func_8001AE70(void *arg0) {
    func_8001AE90(arg0);
}

s32 func_8001AE90(void *ptr) {
    s32 *p;
    s32 *q;
    s32 i;
    s32 addr;
    s32 size;

    if (ptr == 0) {
        return 0;
    }
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr == (s32)ptr) {
                addr &= 0x3FFFFFFF;
                size = p[1];
                q = p;
                if (p != &D_80089848 && p[-3] > 0) {
                    p -= 3;
                    addr = p[0];
                    size += p[1];
                    i++;
                }
                if (i > 0 && q[3] > 0) {
                    if (q != p) {
                        i--;
                    }
                    q += 3;
                    size += q[1];
                }
                p[0] = addr;
                p[1] = size;
                p[2] = -1;
                if (p != q) {
                    for (i--; i > 0; i--) {
                        p += 3;
                        q += 3;
                        p[0] = q[0];
                        p[1] = q[1];
                        p[2] = q[2];
                    }
                    while (p < q) {
                        p += 3;
                        p[0] = 0;
                        p[2] = 0;
                    }
                }
                func_800149A0();
                return 0;
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

s32 func_8001AFF0(s32 arg0) {
    s32 *var_s0;
    s32 temp_a0;
    s32 var_s1;

    var_s0 = &D_80089848;
    var_s1 = 0x3FF;
    if (D_80089848 != 0) {
loop_1:
        temp_a0 = (*(s32 *)((s8 *)var_s0 + 0));
        if ((temp_a0 < 0) && ((*(s32 *)((s8 *)var_s0 + 8)) == arg0)) {
            if (func_8001AE90(temp_a0) == 0) {
                goto loop_1;
            }
        }
        var_s1 -= 1;
        var_s0 += 3;
        if ((var_s1 >= 0) && (*var_s0 != 0)) {
            goto loop_1;
        }
    }
    return 0;
}

s32 func_8001B088(s32 y, s32 x) {
    s32 a;

    if (x == 0) {
        if (y > 0) {
            return 0x400;
        }
        if (y < 0) {
            return -0x400;
        }
        return 0;
    }
    a = catan((y << 12) / x);
    if (x < 0) {
        if (y <= 0) {
            a -= 0x800;
        } else {
            a += 0x800;
        }
    }
    return a;
}

void func_8001B10C(s32 arg0, s32 arg1) {
    func_80014A48(arg1, func_80015848(arg0) == 0 ? 1 : -1);
}

s32 func_8001B144(s32 name, s32 arg1) {
    s32 size;
    CdFile *f;
    s32 buf;

    size = 0;
    while (D_8006DEF0 != 0) {
        func_80014C08(D_800794F0);
    }
    D_8006DEF0 = 1;
    f = func_80015AD8((s8 *)name, 1);
    buf = 0;
    if (f != 0) {
        size = f->size;
        buf = (s32)func_8001ABCC(size, arg1);
        if (buf == 0) {
            func_80015EAC(f);
        } else {
            func_80015F34(f, size, (u8 *)buf);
            func_80015EAC(f);
        }
    }
    D_801D4848 = size;
    func_80014A48(arg1, buf);
    D_8006DEF0 = 0;
    return buf;
}

s32 func_8001B248(s32 *name, s32 arg1, s32 arg2) {
    s32 size;
    CdFile *f;
    s32 buf;

    size = 0;
    while (D_8006DEF0 != 0) {
        func_80014C08(D_800794F0);
    }
    D_8006DEF0 = 1;
    f = func_80015AD8((s8 *)name, 1);
    buf = 0;
    if (f != 0) {
        size = f->size;
        buf = (s32)func_8001ABCC(size, arg2);
        if (buf == 0) {
            func_80015EAC(f);
        } else {
            func_80015F34(f, size, (u8 *)buf);
            func_80015EAC(f);
        }
    }
    D_801D4848 = size;
    func_80014A48(arg1, buf);
    D_8006DEF0 = 0;
    return buf;
}

void func_8001B358(s32 arg0, s32 *arg1, s32 arg2) {
    s32 temp_v0;
    s32 var_s2;

    var_s2 = 0;
    if (D_8006DEF0 != 0) {
        do {
            func_80014C08(D_800794F0);
        } while (D_8006DEF0 != 0);
    }
    D_8006DEF0 = 1;
    temp_v0 = (s32)func_80015AD8((s8 *)arg0, 1);
    if (temp_v0 != 0) {
        var_s2 = (*(s32 *)((s8 *)temp_v0 + 0x24));
        func_80015F34(temp_v0, var_s2, arg1);
        func_80015EAC((s32 *) temp_v0);
    }
    D_801D4848 = var_s2;
    func_80014A48(arg2);
    D_8006DEF0 = 0;
}

void func_8001B438(u32 *tim, s16 px, s16 py, s16 cx, s16 cy) {
    Rect16 r;

    OpenTIM(tim);
    ReadTIM(&D_801D4850);
    if (px == -1) {
        px = D_801D4850.prect->x;
        py = D_801D4850.prect->y;
    } else {
        D_801D4850.prect->x = px;
        D_801D4850.prect->y = py;
    }
    if (cx == -1) {
        cx = D_801D4850.crect->x;
        cy = D_801D4850.crect->y;
    } else if (cx != -2) {
        D_801D4850.crect->x = cx;
        D_801D4850.crect->y = cy;
    }
    r.x = px;
    r.y = py;
    r.w = D_801D4850.prect->w;
    r.h = D_801D4850.prect->h;
    LoadImage((s16 *)&r, (s32)D_801D4850.paddr);
    if ((D_801D4850.mode & 8) && cx != -2) {
        r.x = cx;
        r.y = cy;
        r.w = D_801D4850.crect->w;
        r.h = D_801D4850.crect->h;
        LoadImage((s16 *)&r, (s32)D_801D4850.caddr);
    }
}

void func_8001B5BC(u32 *addr) {
    TIM_IMAGE img;

    OpenTIM(addr);
    while (ReadTIM(&img) != 0) {
        if (img.caddr != 0) {
            LoadImage((s16 *)img.crect, (s32)img.caddr);
        }
        if (img.paddr != 0) {
            LoadImage((s16 *)img.prect, (s32)img.paddr);
        }
    }
    DrawSync(0);
}

void func_8001B634(u32 *addr, s32 dx, s32 dy) {
    TIM_IMAGE img;
    Rect16 r;

    OpenTIM(addr);
    while (ReadTIM(&img) != 0) {
        if (img.caddr != 0) {
            r.w = img.crect->w;
            r.h = img.crect->h;
            r.x = img.crect->x + dx;
            r.y = img.crect->y + dy;
            LoadImage((s16 *)&r, (s32)img.caddr);
        }
        if (img.paddr != 0) {
            r.w = img.prect->w;
            r.h = img.prect->h;
            r.x = img.prect->x + dx;
            r.y = img.prect->y + dy;
            LoadImage((s16 *)&r, (s32)img.paddr);
        }
    }
    DrawSync(0);
}

void func_8001B734(u32 *p) {
    u32 *top;
    u32 *b;
    s32 n;

    n = *p++;
    top = p;
    if ((n & 0xFFFF) == 0x7054) {
        n >>= 16;
        do {
            b = top + p[n - 1];
            if (*b++ & 8) {
                LoadImage((s16 *)(b + 1), (s32)(b + 3));
                b += *b >> 2;
            }
            LoadImage((s16 *)(b + 1), (s32)(b + 3));
            DrawSync(0);
        } while (--n > 0);
    }
}

void func_8001B7F4(u32 *p, s32 dx, s32 dy) {
    u32 *top;
    u32 *b;
    s32 n;

    n = *p++;
    top = p;
    if ((n & 0xFFFF) == 0x7054) {
        n >>= 16;
        do {
            b = top + p[n - 1];
            if (*b++ & 8) {
                ((Rect16 *)(b + 1))->x += dx;
                ((Rect16 *)(b + 1))->y += dy;
                LoadImage((s16 *)(b + 1), (s32)(b + 3));
                b += *b >> 2;
            }
            ((Rect16 *)(b + 1))->x += dx;
            ((Rect16 *)(b + 1))->y += dy;
            LoadImage((s16 *)(b + 1), (s32)(b + 3));
            DrawSync(0);
        } while (--n > 0);
    }
}

void func_8001B90C(s32 w, s32 h, s32 interlace) {
    D_80079500 = 0;
    func_8001B930(w, h, interlace);
}

void func_8001B930(s32 w, s32 h, s32 interlace) {
    s32 i;

    func_80013F04(interlace);
    for (i = 0; i < 2; i++) {
        if (h > 240) {
            SetDefDrawEnv(&DB(i).draw, 0, 0, w, h);
            SetDefDispEnv(&DB(i).disp, 0, 0, w, h);
            DB(i).disp.isinter = 1;
        } else {
            if (interlace == 0) {
                SetDefDrawEnv(&DB(i).draw, 0, i * 256, w, h);
                SetDefDispEnv(&DB(i).disp, 0, 256 - i * 256, w, h);
            } else {
                SetDefDrawEnv(&DB(i).draw, 0, i * 240, w, h);
                SetDefDispEnv(&DB(i).disp, 0, 240 - i * 240, w, h);
            }
            DB(i).disp.isinter = 0;
        }
        DB(i).draw.dtd = 0;
        DB(i).draw.dfe = 0;
        DB(i).draw.isbg = interlace ^ 1;
        DB(i).draw.tpage = GetTPage(0, 0, 0, 0);
        setRGB0(&DB(i).draw, 0, 0, 0);
        DB(i).disp.isrgb24 = interlace;
    }
    D_80079544 = 0;
}

void *func_8001BB44(Chunk *p, s32 id, s32 sub) {
    Chunk *c;

    if (p == 0) {
        return 0;
    }
    for (;;) {
        c = p++;
        if (c->id < 0) {
            return 0;
        }
        if (c->id == id && c->sub == sub) {
            return p;
        }
        p = (Chunk *)((u8 *)p + c->size);
    }
}

void func_8001BB94(Chunk *p, s32 id, s32 sub) {
    Chunk *base;
    Chunk *c;

    base = p;
    if (p == 0) {
        return;
    }
    for (;;) {
        c = p++;
        if (c->id < 0) {
            return;
        }
        if (c->id == id && (sub < 0 || c->sub == sub)) {
            c->id = -1;
            func_8001AD3C(base, (u8 *)p - (u8 *)base);
            return;
        }
        p = (Chunk *)((u8 *)p + c->size);
    }
}

void func_8001BC14(Chunk *arg0) {
    func_8001BB94(arg0, 5, -1);
}

s32 func_8001BC38(void) {
    if (--D_801D4868 >= 0) {
        return (D_801D486C >> D_801D4868) & 1;
    }
    D_801D4868 = 7;
    D_801D486C = *D_801D4870++;
    return D_801D486C >> 7;
}

u32 func_8001BCA4(s32 n) {
    u16 v;

    v = 0;
    while (D_801D4868 < n) {
        n -= D_801D4868;
        v |= (D_801D486C & ((1 << D_801D4868) - 1)) << n;
        D_801D486C = *D_801D4870++;
        D_801D4868 = 8;
    }
    D_801D4868 -= n;
    return v | ((D_801D486C >> D_801D4868) & ((1 << n) - 1));
}

s32 func_8001BD60(void) {
    s32 i;

    if (func_8001BC38() != 0) {
        i = D_801D4878++;
        if (i >= 0x21F) {
            return -1;
        }
        (&D_801D4888)[i] = func_8001BD60();
        (&D_801D5108)[i] = func_8001BD60();
    } else {
        i = func_8001BCA4(9);
    }
    return i;
}

void func_8001BDEC(u32 size) {
    s32 pos;
    s32 i;
    s32 k;
    u32 out;
    s32 root;
    s32 sym;
    s32 off;
    u8 c;

    D_801D487C = 0x1000;
    out = 0;
    root = 0;
    pos = 0xFEE;
    for (k = 0; k < pos; k++) {
        D_801D5988[k] = 0;
    }
    while (out < size) {
        if (D_801D487C == 0x1000) {
            D_801D4878 = 0x110;
            root = func_8001BD60();
            D_801D487C = 0;
        }
        sym = root;
        while (sym >= 0x110) {
            if (func_8001BC38() != 0) {
                sym = (&D_801D5108)[sym];
            } else {
                sym = (&D_801D4888)[sym];
            }
        }
        D_801D487C++;
        if (sym < 0x100) {
            *D_801D4874++ = sym;
            D_801D5988[pos] = sym;
            pos++;
            pos &= 0xFFF;
            out++;
        } else {
            sym -= 0xFD;
            off = func_8001BCA4(12);
            for (i = 0; i < sym; i++) {
                c = D_801D5988[(off + i) & 0xFFF];
                *D_801D4874++ = c;
                D_801D5988[pos] = c;
                pos++;
            pos &= 0xFFF;
            }
            out += sym;
        }
    }
}

s32 func_8001BFCC(s32 arg0, s32 arg1) {
    return func_8001C078(arg0 + ((s32 *)arg0)[arg1]);
}

s32 func_8001BFF8(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;

    D_801D4868 = 0;
    D_801D486C = 0;
    D_801D4870 = (u8 *)arg0;
    temp_s0 = func_8001BCA4(0x10);
    temp_s0_2 = (temp_s0 << 0x10) | func_8001BCA4(0x10);
    temp_v0 = func_8001ABCC(temp_s0_2, arg1);
    D_801D4874 = (u8 *)temp_v0;
    func_8001BDEC(temp_s0_2);
    return temp_v0;
}

s32 func_8001C078(s32 arg0) {
    return func_8001BFF8(arg0, func_800148B0());
}

void func_8001C0A8(s8 *base, u32 n, s32 size, s32 (*cmp)(s8 *, s8 *)) {
    u32 i;
    u32 j;
    u32 k;
    s8 *a;
    s8 *b;

    if (n < 2) {
        return;
    }
    a = base;
    if (n == 2) {
        b = a + size;
        if (cmp(a, b) > 0) {
            func_8001C1E0(a, b, size);
        }
        return;
    }
    for (i = 0; i < n; i++, a += size) {
        b = a + size;
        for (j = i; j < n - 1; j++, b += size) {
            if (cmp(a, b) > 0) {
                for (k = i; k <= j; k++) {
                    func_8001C1E0(base + size * k, b, size);
                }
            }
        }
    }
}

void func_8001C1E0(s8 *a, s8 *b, s32 size) {
    u32 i;
    s8 t;

    for (i = 0; i < size; i++) {
        t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

void func_8001C220(void *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    s32 u;
    s32 abr;

    (*(s8 *)((s8 *)arg0 + 0xF)) = 4;
    (*(u8 *)((s8 *)arg0 + 0x13)) = 0x64;
    (*(s16 *)((s8 *)arg0 + 0x1A)) = arg3;
    (*(s16 *)((s8 *)arg0 + 0x1C)) = arg7;
    (*(s16 *)((s8 *)arg0 + 0x1E)) = arg8;
    (*(s16 *)((s8 *)arg0 + 0x14)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = arg2;
    if (arg4 != 0) {
        u = (arg5 % 64) * 2;
    } else {
        u = (arg5 % 64) * 4;
    }
    (*(u8 *)((s8 *)arg0 + 0x18)) = u;
    (*(u8 *)((s8 *)arg0 + 0x19)) = arg6;
    (*(u8 *)((s8 *)arg0 + 0x10)) = 0x80;
    (*(u8 *)((s8 *)arg0 + 0x11)) = 0x80;
    (*(u8 *)((s8 *)arg0 + 0x12)) = 0x80;
    if (arg9 >= 0) {
        (*(u8 *)((s8 *)arg0 + 0x13)) |= 2;
        abr = arg9;
    } else {
        abr = 0;
    }
    SetDrawMode(arg0, 0, 0, ((arg4 & 3) << 7) | ((abr & 3) << 5) | ((arg6 & 0x100) >> 4) | ((arg5 & 0x3C0) >> 6) | ((arg6 & 0x200) * 4), &D_800794F8);
    MargePrim(arg0, (s8 *)arg0 + 0xC);
}

void func_8001C354(s32 x, s32 y, s32 w, s32 h, u32 color) {
    Rect16 r;
    u32 *buf;
    u32 *p;
    s32 i;

    w /= 4;
    if (w == 0 || h == 0) {
        return;
    }
    r.x = x;
    r.w = w;
    /* x is reused as the fill size in bytes */
    if (func_8001AB64() < w * (h << 2)) {
        x = func_8001AB64();
    } else {
        x = w * (h << 2);
    }
    r.h = (u32)x / (w << 2);
    if (r.h <= 0) {
        return;
    }
    buf = func_8001AD0C(r.h * (w << 2));
    if (buf == NULL) {
        return;
    }
    p = buf;
    for (x -= 4; x >= 0; x -= 4) {
        *p++ = color;
    }
    r.y = y;
    for (i = 0; i < h; i += r.h, r.y += r.h) {
        r.h = (h - i < r.h) ? h - i : r.h;
        LoadImage((s16 *)&r, (s32)buf);
    }
    DrawSync(0);
    func_8001AE90(buf);
}

void func_8001C4DC(s32 x, s32 y, Rect16 *r, u16 tpage, s32 clut, s32 z, u8 c, s8 abr) {
    s32 tp = tpage;

    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = r->x;
        CUR_SPRT->sp.v0 = r->y;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = r->w;
        CUR_SPRT->sp.h = r->h;
        CUR_SPRT->sp.r0 = c;
        CUR_SPRT->sp.g0 = c;
        CUR_SPRT->sp.b0 = c;
        if (abr >= 0) {
            tp |= (abr & 3) << 5;
            setSemiTrans(&CUR_SPRT->sp, 1);
        } else {
            setSemiTrans(&CUR_SPRT->sp, 0);
        }
        setDrawMode(&CUR_SPRT->dm, 0, 0, tp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_8001C6A4(POLY_FT4 *p, POLY_FT4 *dst, u8 *rgb, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy,
                   u8 semi, u8 flat) {
    func_800677A4(p);
    p->tpage = tpage;
    p->clut = clut;
    SetShadeTex(p, flat ^ 1);
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    if (uv != 0) {
        func_8001EC3C((u8 *)p, uv->x, uv->y, uv->w, uv->h);
    }
    if (xy != 0) {
        func_8001EA64(p, xy->x, xy->y, xy->w, xy->h);
    }
    *dst = *p;
}

void func_8001C810(POLY_FT3 *p, s32 *dst, u8 *rgb, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy,
                   u8 semi, u8 flat) {
    func_80067724(p);
    p->tpage = tpage;
    p->clut = clut;
    SetShadeTex(p, flat ^ 1);
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    if (uv != 0) {
        p->u0 = uv->x + uv->w / 2;
        p->v0 = uv->y;
        p->u1 = uv->x;
        p->v1 = uv->y + uv->h;
        p->u2 = uv->x + uv->w;
        p->v2 = uv->y + uv->h;
    }
    if (xy != 0) {
        p->x0 = xy->x + xy->w / 2;
        p->y0 = xy->y;
        p->x1 = xy->x;
        p->y1 = xy->y + xy->h;
        p->x2 = xy->x + xy->w;
        p->y2 = xy->y + xy->h;
    }
    dst[0] = ((s32 *)p)[0];
    dst[1] = ((s32 *)p)[1];
    dst[2] = ((s32 *)p)[2];
    dst[3] = ((s32 *)p)[3];
    dst[4] = ((s32 *)p)[4];
    dst[5] = ((s32 *)p)[5];
    dst[6] = ((s32 *)p)[6];
    dst[7] = ((s32 *)p)[7];
}

void func_8001CA54(POLY_GT3 *p, POLY_GT3 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, s32 tpage, s32 clut,
                   Rect16 *uv, Rect16 *xy, u8 semi) {
    func_80067764(p);
    p->tpage = tpage;
    p->clut = clut;
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    if (uv != 0) {
        p->u0 = uv->x + uv->w / 2;
        p->v0 = uv->y;
        p->u1 = uv->x;
        p->v1 = uv->y + uv->h;
        p->u2 = uv->x + uv->w;
        p->v2 = uv->y + uv->h;
    }
    if (xy != 0) {
        p->x0 = xy->x + xy->w / 2;
        p->y0 = xy->y;
        p->x1 = xy->x;
        p->y1 = xy->y + xy->h;
        p->x2 = xy->x + xy->w;
        p->y2 = xy->y + xy->h;
    }
    *dst = *p;
}

void func_8001CCB4(POLY_GT4 *p, POLY_GT4 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, u8 *rgb3, s32 tpage,
                   s32 clut, Rect16 *uv, Rect16 *xy, u8 semi) {
    func_800677E4(p);
    p->tpage = tpage;
    p->clut = clut;
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    if (rgb3 != 0) {
        func_8001E804(p, rgb3[0], rgb3[1], rgb3[2]);
    }
    if (uv != 0) {
        func_8001EC3C((u8 *)p, uv->x, uv->y, uv->w, uv->h);
    }
    if (xy != 0) {
        func_8001EA64(p, xy->x, xy->y, xy->w, xy->h);
    }
    *dst = *p;
}

void func_8001CE74(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, s16 *r, u8 semi) {
    func_80067784(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    if (r != 0) {
        func_8001EA64(p, r[0], r[1], r[2], r[3]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    dst[5] = p[5];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_8001CFDC(POLY_G4 *p, POLY_G4 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, u8 *rgb3, s32 abr,
                   void *tp0, void *tp1, Rect16 *xy, u8 semi) {
    func_800677C4(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    if (rgb3 != 0) {
        func_8001E804(p, rgb3[0], rgb3[1], rgb3[2]);
    }
    if (xy != 0) {
        func_8001EA64(p, xy->x, xy->y, xy->w, xy->h);
    }
    *dst = *p;
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_8001D1AC(s32 *p, s32 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, s32 abr, void *tp0, void *tp1,
                   u8 semi) {
    func_80067744(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    dst[5] = p[5];
    dst[6] = p[6];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_8001D33C(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, u8 semi) {
    func_80067704(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_8001D464(s32 *p, s32 *dst, u8 *rgb0, u8 *rgb1, s32 abr, void *tp0, void *tp1, u8 semi) {
    func_80067904(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_8001D5B4(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, u8 semi) {
    func_800678E4(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_8001D6D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg4 == 0) {
        sp30 = RotTransPers3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg5 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg5], arg0);
        }
    }
}

void func_8001D7DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg5 == 0) {
        sp38 = RotTransPers4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg6 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg6], arg0);
        }
    }
}

void func_8001D900(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg5 == 0) {
        sp38 = RotTransPers4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x14, arg0 + 0x20, arg0 + 0x2C, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x14, arg0 + 0x20, arg0 + 0x2C, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg6 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg6], arg0);
        }
    }
}

void func_8001DA24(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 arg7) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg6 == 0) {
        sp30 = RotTransPers3(arg2, arg3, arg4, arg0 + 8, arg0 + 0xC, arg0 + 0x10, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg2, arg3, arg4, arg0 + 8, arg0 + 0xC, arg0 + 0x10, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg7 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp30], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg7], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg7], arg1);
                }
            }
        }
    }
}

void func_8001DBAC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 arg8) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg7 == 0) {
        sp38 = RotTransPers4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0xC, arg0 + 0x10, arg0 + 0x14, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0xC, arg0 + 0x10, arg0 + 0x14, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg8 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp38], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg8], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg8], arg1);
                }
            }
        }
    }
}

void func_8001DD4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg4 == 0) {
        sp30 = RotTransPers3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x14, arg0 + 0x20, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x14, arg0 + 0x20, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg5 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg5], arg0);
        }
    }
}

void func_8001DE58(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 arg7) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg6 == 0) {
        sp30 = RotTransPers3(arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg7 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp30], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg7], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg7], arg1);
                }
            }
        }
    }
}

void func_8001DFE0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 arg8) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg7 == 0) {
        sp38 = RotTransPers4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg8 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp38], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg8], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg8], arg1);
                }
            }
        }
    }
}

void func_8001E180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp10;
    s32 sp14;
    s32 temp_v0;

    RotTransPers(arg2, arg0 + 8, &sp10, &sp14);
    temp_v0 = RotTransPers(arg3, arg0 + 0xC, &sp10, &sp14);
    if ((u32) (temp_v0 - 2) < 0xFFFU) {
        if (arg5 == 0) {
            AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg1);
            }
        } else {
            AddPrim((s32 *) &D_800793A0->ot[arg5], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[arg5], arg1);
            }
        }
    }
}

void func_8001E2A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp10;
    s32 sp14;
    s32 temp_v0;

    RotTransPers(arg2, arg0 + 8, &sp10, &sp14);
    temp_v0 = RotTransPers(arg3, arg0 + 0x10, &sp10, &sp14);
    if ((u32) (temp_v0 - 2) < 0xFFFU) {
        if (arg5 == 0) {
            AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg1);
            }
        } else {
            AddPrim((s32 *) &D_800793A0->ot[arg5], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[arg5], arg1);
            }
        }
    }
}

s32 func_8001E3C8(s32 step, u8 *r, s32 tr, u8 *g, s32 tg, u8 *b, s32 tb) {
    s16 vr;
    s16 vg;
    s16 vb;
    s32 done;

    vr = *r;
    vg = *g;
    vb = *b;
    done = 0;
    if (vr < tr) {
        vr += step;
        if (vr > tr) {
            vr = tr;
            done++;
        }
    } else if (vr > tr) {
        vr -= step;
        if (vr < tr) {
            vr = tr;
            done++;
        }
    } else {
        done++;
    }
    if (vg < tg) {
        vg += step;
        if (vg > tg) {
            vg = tg;
            done++;
        }
    } else if (vg > tg) {
        vg -= step;
        if (vg < tg) {
            vg = tg;
            done++;
        }
    } else {
        done++;
    }
    if (vb < tb) {
        vb += step;
        if (vb > tb) {
            vb = tb;
            done++;
        }
    } else if (vb > tb) {
        vb -= step;
        if (vb < tb) {
            vb = tb;
            done++;
        }
    } else {
        done++;
    }
    *r = vr;
    *g = vg;
    *b = vb;
    return done == 3;
}

void func_8001E4E8(u8 dir, s16 step, u8 *state, u8 *prim) {
    s16 c;

    switch (*state) {
    case 0:
        if (dir == 0) {
            func_8001E75C(prim, 0, 0, 0);
        } else {
            func_8001E75C(prim, 0x80, 0x80, 0x80);
        }
        SetSemiTrans(prim, 1);
        *state = 1;
        break;
    case 1:
        if (dir == 0) {
            func_8001E75C(prim, 0, 0, 0);
        } else {
            func_8001E75C(prim, 0x80, 0x80, 0x80);
        }
        SetSemiTrans(prim, 1);
        *state = 2;
        break;
    case 2:
        c = prim[4];
        if (dir == 0) {
            c += step;
            if (c >= 0x80) {
                SetSemiTrans(prim, 0);
                func_8001E75C(prim, 0x80, 0x80, 0x80);
                *state = 3;
                break;
            }
        } else {
            c -= step;
            if (c <= 0) {
                SetSemiTrans(prim, 0);
                func_8001E75C(prim, 0x80, 0x80, 0x80);
                *state = 3;
                break;
            }
        }
        func_8001E75C(prim, c, c, c);
        return;
    case 3:
        SetSemiTrans(prim, 0);
        func_8001E75C(prim, 0x80, 0x80, 0x80);
        *state = 4;
        break;
    case 4:
        *state = 5;
        break;
    }
}

void func_8001E6A4(s32 arg0, s16 arg1, s16 arg2) {
    s16 r[4];

    r[0] = arg1;
    r[1] = arg2;
    r[2] = 0x100;
    r[3] = 1;
    LoadImage(r, arg0);
    DrawSync(0);
}

void func_8001E6EC(s32 type, void *p, s32 abe, s32 tge) {
    D_8006DF0C[type](p);
    SetSemiTrans(p, abe);
    SetShadeTex(p, tge);
}

void func_8001E75C(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(u8 *)((s8 *)arg0 + 4)) = arg1;
    (*(u8 *)((s8 *)arg0 + 5)) = arg2;
    (*(u8 *)((s8 *)arg0 + 6)) = arg3;
}

void func_8001E76C(void *arg0, u8 r, u8 g, u8 b) {
    if (*((u8 *)arg0 + 7) & 4) {
        func_8001E8A4(arg0, r, g, b);
    } else {
        func_8001E894(arg0, r, g, b);
    }
}

void func_8001E7B8(void *arg0, u8 r, u8 g, u8 b) {
    if (*((u8 *)arg0 + 7) & 4) {
        func_8001E8C4(arg0, r, g, b);
    } else {
        func_8001E8B4(arg0, r, g, b);
    }
}

void func_8001E804(void *arg0, u8 r, u8 g, u8 b) {
    if (*((u8 *)arg0 + 7) & 4) {
        func_8001E8E4(arg0, r, g, b);
    } else {
        func_8001E8D4(arg0, r, g, b);
    }
}

void func_8001E850(u8 *p, u8 *c) {
    if (p[7] & 4) {
        func_8001E9AC(p, c);
    } else {
        func_8001E8F4(p, c);
    }
}

void func_8001E894(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0xC)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0xD)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0xE)) = arg3;
}

void func_8001E8A4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x10)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x11)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x12)) = arg3;
}

void func_8001E8B4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x14)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x15)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x16)) = arg3;
}

void func_8001E8C4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x1C)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x1D)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x1E)) = arg3;
}

void func_8001E8D4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x1C)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x1D)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x1E)) = arg3;
}

void func_8001E8E4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x28)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x29)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x2A)) = arg3;
}

void func_8001E8F4(u8 *p, u8 *c) {
    p[0x4] = *c++;
    p[0x5] = *c++;
    p[0x6] = *c++;
    p[0xC] = *c++;
    p[0xD] = *c++;
    p[0xE] = *c++;
    p[0x14] = *c++;
    p[0x15] = *c++;
    p[0x16] = *c++;
    p[0x1C] = *c++;
    p[0x1D] = *c++;
    p[0x1E] = *c;
}

void func_8001E9AC(u8 *p, u8 *c) {
    p[0x4] = *c++;
    p[0x5] = *c++;
    p[0x6] = *c++;
    p[0x10] = *c++;
    p[0x11] = *c++;
    p[0x12] = *c++;
    p[0x1C] = *c++;
    p[0x1D] = *c++;
    p[0x1E] = *c++;
    p[0x28] = *c++;
    p[0x29] = *c++;
    p[0x2A] = *c;
}

void func_8001EA64(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 temp_v1;

    temp_v1 = (*(u8 *)((s8 *)arg0 + 7)) & 0x14;
    switch (temp_v1) {                              /* irregular */
    case 0:
        func_8001EB1C(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    case 4:
        func_8001EBAC(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    case 16:
        func_8001EB64(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    case 20:
        func_8001EBF4(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    }
}

void func_8001EB1C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0xC)) = x1;
    (*(s16 *)((s8 *)arg0 + 0xE)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x10)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x12)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x14)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = y1;
}

void func_8001EB64(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0x10)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x12)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x18)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x1A)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x20)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x22)) = y1;
}

void func_8001EBAC(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0x10)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x12)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x18)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x1A)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x20)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x22)) = y1;
}

void func_8001EBF4(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0x14)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x20)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x22)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x2C)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x2E)) = y1;
}

void func_8001EC3C(u8 *p, u8 x, u8 y, u8 w, u8 h) {
    if (p[7] & 0x10) {
        func_8001ECC8(p, x, y, w, h);
    } else {
        func_8001EC8C(p, x, y, w, h);
    }
}

void func_8001EC8C(u8 *p, u8 x, u8 y, u8 w, u8 h) {
    p[0xC] = x;
    p[0xD] = y;
    p[0x14] = x + w;
    p[0x15] = y;
    p[0x1C] = x;
    p[0x1D] = y + h;
    p[0x24] = x + w;
    p[0x25] = y + h;
}

void func_8001ECC8(u8 *p, u8 x, u8 y, u8 w, u8 h) {
    p[0xC] = x;
    p[0xD] = y;
    p[0x18] = x + w;
    p[0x19] = y;
    p[0x24] = x;
    p[0x25] = y + h;
    p[0x30] = x + w;
    p[0x31] = y + h;
}

void func_8001ED04(void *arg0) {
    (*(s16 *)((s8 *)arg0 + 0)) = 0x1000;
    (*(s16 *)((s8 *)arg0 + 6)) = 0;
    (*(s16 *)((s8 *)arg0 + 0xC)) = 0;
    (*(s16 *)((s8 *)arg0 + 2)) = 0;
    (*(s16 *)((s8 *)arg0 + 8)) = 0x1000;
    (*(s16 *)((s8 *)arg0 + 0xE)) = 0;
    (*(s16 *)((s8 *)arg0 + 4)) = 0;
    (*(s16 *)((s8 *)arg0 + 0xA)) = 0;
    (*(s16 *)((s8 *)arg0 + 0x10)) = 0x1000;
}

void func_8001ED30(s32 arg0, s16 *arg1, void *arg2) {
    s32 axis;

    axis = arg0 & 0xFF;
    if (axis == 0) {
        return;
    }
    func_8001ED04(arg2);
    switch (axis) {
    case 4:
        return;
    case 3:
        arg1[1] = 0;
        arg1[2] = 0;
        break;
    case 2:
        arg1[0] = 0;
        arg1[2] = 0;
        break;
    case 1:
        arg1[0] = 0;
        arg1[1] = 0;
        break;
    }
    RotMatrix(arg1, arg2);
}

/* old-style definition: the callers pass an int, the byte is read here */
void func_8001EDE0(rot, trans, scale, m, axis)
    SVECTOR *rot;
    VECTOR *trans;
    VECTOR *scale;
    MATRIX *m;
    u8 axis;
{
    RotMatrix(rot, m);
    func_8001ED30(axis, (s16 *)rot, m);
    TransMatrix(m, trans);
    if (scale != 0 && (scale->vx != 0x1000 || scale->vy != scale->vx || scale->vz != scale->vy)) {
        ScaleMatrix(m, scale);
    }
    func_8001EFB0((s32)m);
}

void func_8001EEA0(void *arg0, s32 arg1) {
    s16 v[4];
    s32 flag;
    s32 axis;
    s32 sx;
    s32 sy;
    void *rot;

    axis = arg1 & 0xFF;
    if ((*(s32 *)((s8 *)arg0 + 0x48)) == 0) {
        func_8001EDE0((s8 *)arg0 + 0x30, (s8 *)arg0 + 0x20, (s8 *)arg0 + 0x38, arg0, axis);
        return;
    }
    rot = (s8 *)arg0 + 0x30;
    func_8001EFB0((*(s32 *)((s8 *)arg0 + 0x48)));
    RotMatrix(rot, arg0);
    MulMatrix2(*(MATRIX **)((s8 *)arg0 + 0x48), arg0);
    func_8001ED30(axis, rot, arg0);
    v[0] = (*(u16 *)((s8 *)arg0 + 0x20));
    v[1] = (*(u16 *)((s8 *)arg0 + 0x24));
    v[2] = (*(u16 *)((s8 *)arg0 + 0x28));
    RotTrans(v, (s8 *)arg0 + 0x14, &flag);
    sx = (*(s32 *)((s8 *)arg0 + 0x38));
    if ((sx != 0x1000 || (sy = (*(s32 *)((s8 *)arg0 + 0x3C))) != sx || (*(s32 *)((s8 *)arg0 + 0x40)) != sy) && axis != 4) {
        ScaleMatrix(arg0, (s8 *)arg0 + 0x38);
    }
    func_8001EFB0((s32) arg0);
}

void func_8001EFB0(s32 arg0) {
    func_8005C444();
    SetRotMatrix(arg0);
}

void func_8001EFDC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7) {
    (*(s32 *)((s8 *)arg0 + 0x20)) = arg2;
    (*(s32 *)((s8 *)arg0 + 0x24)) = arg3;
    (*(s32 *)((s8 *)arg0 + 0x28)) = arg4;
    (*(s16 *)((s8 *)arg0 + 0x30)) = arg5;
    (*(s16 *)((s8 *)arg0 + 0x32)) = arg6;
    (*(s16 *)((s8 *)arg0 + 0x34)) = arg7;
    (*(s32 *)((s8 *)arg0 + 0x38)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x3C)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x40)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x48)) = arg1;
}

void func_8001F01C(void *arg0, void *arg1) {
    (*(u16 *)((s8 *)arg1 + 0)) = (u16) (*(u16 *)((s8 *)arg0 + 0x14));
    (*(u16 *)((s8 *)arg1 + 2)) = (u16) (*(u16 *)((s8 *)arg0 + 0x18));
    (*(u16 *)((s8 *)arg1 + 4)) = (u16) (*(u16 *)((s8 *)arg0 + 0x1C));
}

void func_8001F040(void) {
    D_801D69E0 = 0;
}

void func_8001F04C(void) {
    D_801D69E0 = 0;
}

s32 func_8001F058(void) {
    return D_801D69E0;
}

void func_8001F068(s32 arg0, s32 arg1, s32 arg2) {
    D_801D69E8 = arg0;
    D_801D69EC = arg1;
    D_801D69F0 = arg2;
    D_801D69E4 = arg0 * 0xFF;
}

void func_8001F094(s32 dir, s32 abr, s32 speed) {
    while (D_801D69E0 != 0) {
        func_80014C08(D_800794F0);
    }
    D_801D69E8 = dir;
    D_801D69EC = abr;
    D_801D69F0 = speed;
    D_801D69E0 = 1;
    D_801D69E4 = dir * 0xFF;
    while (D_801D69E0 != 0) {
        func_80014C08(D_800794F0);
        if (D_801D69E8 != 0) {
            if ((D_801D69E4 -= D_801D69F0) < 0) {
                D_801D69E4 = 0;
                break;
            }
        } else if ((D_801D69E4 += D_801D69F0) >= 0x100) {
            D_801D69E4 = 0xFF;
        }
        SetDrawTPage(D_801D69D0[D_800794F4], 0, 0, GetTPage(0, D_801D69EC, 0, 0));
        func_8001E6EC(8, &D_801D69A0[D_800794F4], 1, 0);
        func_8001E75C(&D_801D69A0[D_800794F4], (u8)D_801D69E4, (u8)D_801D69E4, (u8)D_801D69E4);
        SetSemiTrans(&D_801D69A0[D_800794F4], 1);
        D_801D69A0[D_800794F4].x0 = 0;
        D_801D69A0[D_800794F4].y0 = 0;
        D_801D69A0[D_800794F4].x1 = 320;
        D_801D69A0[D_800794F4].y1 = 0;
        D_801D69A0[D_800794F4].x2 = 0;
        D_801D69A0[D_800794F4].y2 = 240;
        D_801D69A0[D_800794F4].x3 = 320;
        D_801D69A0[D_800794F4].y3 = 240;
        AddPrim((s32 *)D_800793A0->ot, (s32)&D_801D69A0[D_800794F4]);
        AddPrim((s32 *)D_800793A0->ot, (s32)D_801D69D0[D_800794F4]);
    }
    D_801D69E0 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F3C0);

void func_8001F518(u32 i0, u32 *idx, u8 *base) {
    u8 *v0;
    u8 *v1;
    u8 *v2;
    u8 *c0;
    u8 *c1;
    u32 i1;
    u32 i2;

    v0 = base + (i0 >> 16);
    gte_lwc2(12, 0, v0);
    gte_lwc2(17, 4, v0);
    i1 = idx[1];
    i2 = idx[2];
    v1 = base + (i1 >> 16);
    v2 = base + (i2 >> 16);
    gte_lwc2(13, 0, v1);
    gte_lwc2(18, 4, v1);
    gte_lwc2(14, 0, v2);
    gte_lwc2(19, 4, v2);
    c0 = base + (i0 & 0xFFFF);
    gte_nclip();
    c1 = base + (i1 & 0xFFFF);
    base += i2 & 0xFFFF;
    gte_lwc2(20, 0, c0);
    gte_lwc2(21, 0, c1);
    gte_lwc2(22, 0, base);
}

void func_8001F580(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    base += i & 0xFFFF;
    gte_lwc2(12, 0, p);
    gte_lwc2(17, 4, p);
    gte_lwc2(20, 0, base);
}

void func_8001F5A4(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(12, 0, p);
    gte_lwc2(17, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(20, 0, base);
}

void func_8001F5CC(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    base += i & 0xFFFF;
    gte_lwc2(13, 0, p);
    gte_lwc2(18, 4, p);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
    gte_lwc2(21, 0, base);
}

void func_8001F5FC(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(13, 0, p);
    gte_lwc2(18, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(21, 0, base);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
}

void func_8001F630(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    base += i & 0xFFFF;
    gte_lwc2(14, 0, p);
    gte_lwc2(19, 4, p);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
    gte_lwc2(22, 0, base);
}

void func_8001F660(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(14, 0, p);
    gte_lwc2(19, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(22, 0, base);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
}

void func_8001F694(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    base += i & 0xFFFF;
    gte_lwc2(0, 0, p);
    gte_lwc2(16, 4, p);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
    gte_lwc2(6, 0, base);
}

u32 *func_8001F6C4(u32 *p, u32 *ot, s32 gouraud, u32 code) {
    u32 *next;
    register u32 rgb asm("$8");
    u32 len;
    u32 z;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, p);
    p[1] = rgb | code;
    next = p;
    if (gouraud) {
        gte_swc2(2, 12, p);
        gte_swc2(21, 16, p);
        gte_swc2(13, 20, p);
        gte_swc2(4, 24, p);
        gte_swc2(22, 28, p);
        gte_swc2(14, 32, p);
        gte_swc2(3, 36, p);
        gte_mfc2(7, z);
        len = 0x09000000;
        next = p + 10;
    } else {
        gte_swc2(2, 12, p);
        gte_swc2(13, 16, p);
        gte_swc2(4, 20, p);
        gte_swc2(14, 24, p);
        gte_swc2(3, 28, p);
        gte_mfc2(7, z);
        len = 0x07000000;
        next += 8;
    }
    if (z >= (u32)SORT_WORK->unk20) {
        return p;
    }
    tag = len | ot[z];
    ot[z] = (u32)p;
    *p = tag;
    return next;
}

u32 *func_8001F768(u32 *p, u32 *ot, s32 gouraud, u32 code) {
    register u32 rgb asm("$8");
    u32 *next;
    u32 len;
    u32 z;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, p);
    p[1] = rgb | code;
    next = p;
    if (gouraud) {
        gte_swc2(2, 12, p);
        gte_swc2(21, 16, p);
        gte_swc2(13, 20, p);
        gte_swc2(4, 24, p);
        gte_swc2(22, 28, p);
        gte_swc2(14, 32, p);
        gte_swc2(3, 36, p);
        gte_mfc2(7, z);
        len = 0x0C000000;
        next = p + 13;
        gte_swc2(6, 40, p);
        gte_swc2(0, 44, p);
        gte_swc2(5, 48, p);
    } else {
        gte_swc2(2, 12, p);
        gte_swc2(13, 16, p);
        gte_swc2(4, 20, p);
        gte_swc2(14, 24, p);
        gte_swc2(3, 28, p);
        gte_swc2(0, 32, p);
        gte_swc2(5, 36, p);
        gte_mfc2(7, z);
        len = 0x09000000;
        next += 10;
    }
    if (z >= (u32)SORT_WORK->unk20) {
        return p;
    }
    tag = len | ot[z];
    ot[z] = (u32)p;
    *p = tag;
    return next;
}

u32 *func_8001F824(u32 *p, u32 *ot, s32 gouraud, u32 code) {
    register u32 rgb asm("$8");
    u32 *next;
    u32 len;
    u32 z;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, p);
    p[1] = rgb | code;
    next = p;
    if (gouraud) {
        gte_swc2(21, 12, p);
        gte_swc2(13, 16, p);
        gte_swc2(22, 20, p);
        gte_swc2(14, 24, p);
        gte_mfc2(7, z);
        len = 0x06000000;
        next = p + 7;
    } else {
        gte_swc2(13, 12, p);
        gte_swc2(14, 16, p);
        gte_mfc2(7, z);
        len = 0x04000000;
        next += 5;
    }
    if (z >= (u32)SORT_WORK->unk20) {
        return p;
    }
    tag = len | ot[z];
    ot[z] = (u32)p;
    *p = tag;
    return next;
}

u32 *func_8001F8B0(u32 *p, u32 *ot, s32 gouraud, u32 code) {
    register u32 rgb asm("$8");
    u32 *next;
    u32 len;
    u32 z;
    u32 tag;

    gte_mfc2(20, rgb);
    gte_swc2(12, 8, p);
    p[1] = rgb | code;
    next = p;
    if (gouraud) {
        gte_swc2(21, 12, p);
        gte_swc2(13, 16, p);
        gte_swc2(22, 20, p);
        gte_swc2(14, 24, p);
        gte_mfc2(7, z);
        len = 0x08000000;
        next = p + 9;
        gte_swc2(6, 28, p);
        gte_swc2(0, 32, p);
    } else {
        gte_swc2(13, 12, p);
        gte_swc2(14, 16, p);
        gte_swc2(0, 20, p);
        gte_mfc2(7, z);
        len = 0x05000000;
        next += 6;
    }
    if (z >= (u32)SORT_WORK->unk20) {
        return p;
    }
    tag = len | ot[z];
    ot[z] = (u32)p;
    *p = tag;
    return next;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F94C);

u32 func_800202D8(u32 *data, u32 *ot, u32 packet, void *arg3) {
    SortWork *w;
    s32 n;

    w = SORT_WORK;
    w->data = data;
    w->ot = ot;
    w->packet = packet & 0xFFFFFF;
    w->work = (u32 *)0x1F80007C;
    w->unk20 = arg3;
    n = *data++;
    w->data = data;
    for (; n > 0; n--) {
        SORT_WORK->data = func_8001F3C0(SORT_WORK->data, SORT_WORK->work);
        func_8001F94C(w);
    }
    return SORT_WORK->packet;
}

u32 func_80020370(u32 *data, u32 *ot, u32 packet, void *arg3) {
    SortWork *w;
    s32 n;

    w = SORT_WORK;
    w->data = data;
    w->ot = ot;
    w->packet = packet & 0xFFFFFF;
    w->work = (u32 *)0x1F80007C;
    w->unk20 = arg3;
    n = *data++;
    w->data = data;
    for (; n > 0; n--) {
        if (*SORT_WORK->data++ != 0) {
            SORT_WORK->data = func_80020440(SORT_WORK->data, SORT_WORK->work);
            func_80020778(w);
        } else {
            SORT_WORK->data = func_8001F3C0(SORT_WORK->data, SORT_WORK->work);
            func_8001F94C(w);
        }
    }
    return SORT_WORK->packet;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020440);

void func_8002060C(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(12, 0, p);
    gte_lwc2(17, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(20, 0, base);
    gte_lwc2(25, 4, base);
}

void func_80020638(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(13, 0, p);
    gte_lwc2(18, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(21, 0, base);
    gte_lwc2(26, 4, base);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
        gte_lwc2(25, 4, base);
    }
}

void func_80020674(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(14, 0, p);
    gte_lwc2(19, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(22, 0, base);
    gte_lwc2(27, 4, base);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
        gte_lwc2(25, 4, base);
    }
}

u32 *func_800206B0(u32 *p, u32 *ot, s32 gouraud, u32 code) {
    SortWork *w;
    s32 a;
    s32 b;
    s32 c;
    u32 z;
    u32 tag;

    gte_mfc2(25, a);
    gte_mfc2(26, b);
    gte_mfc2(27, c);
    if (a > 0 && b > 0 && c > 0) {
        return p;
    }
    w = SORT_WORK;
    p[1] = w->unk2C;
    p[3] = (a & 0xFFFF) | w->clut;
    p[5] = (b & 0xFFFF) | w->tpage;
    p[7] = (c | w->tpage) & 0xFFFF;
    gte_swc2(12, 8, p);
    gte_swc2(13, 16, p);
    gte_swc2(14, 24, p);
    gte_mfc2(7, z);
    if (z >= (u32)w->unk20) {
        return p;
    }
    tag = ot[z] | 0x07000000;
    ot[z] = (u32)p;
    *p = tag;
    return p + 8;
}

void func_80020778(SortWork *w) {
    u32 *pkt;
    u32 *d;
    u32 hdr;
    s32 tex;
    u32 code;
    s32 gouraud;
    u32 idx;
    u32 n;
    u32 len;
    u32 k;
    register s32 opz asm("$4");
    u8 *base;
    u8 *p0;
    u8 *p1;
    u8 *p2;
    u8 *q0;
    u8 *q1;
    u8 *q2;
    u32 i1;
    u32 i2;

    pkt = (u32 *)SORT_WORK->packet;
    d = SORT_WORK->data;
    while ((hdr = *d++) != 0) {
        SORT_WORK->count[0] = hdr >> 20;
        SORT_WORK->count[1] = (hdr >> 8) & 0xFFF;
        code = hdr << 24;
        SORT_WORK->code = code;
        SORT_WORK->quad = code & 0x08000000;
        SORT_WORK->textured = code & 0x04000000;
        gouraud = code & 0x10000000;
        tex = *d++;
        if (tex >= 0) {
            SORT_WORK->unk38 = 0;
            SORT_WORK->unk2C = (tex & 0xFFFFFF) | 0x26000000;
            SORT_WORK->clut = D_8006DF60[tex >> 24].clut;
            SORT_WORK->tpage = D_8006DF60[tex >> 24].tpage;
        } else {
            SORT_WORK->unk38 = 1;
            SORT_WORK->unk2C = (tex & 0xFFFFFF) | 0x26000000;
            SORT_WORK->clut = *d++;
            SORT_WORK->tpage = *d++;
        }
        idx = *d;
        for (SORT_WORK->pass = 0; SORT_WORK->pass != 2; SORT_WORK->pass++) {
            while (SORT_WORK->count[SORT_WORK->pass] != 0) {
                n = idx;
                len = idx >> 16;
                idx = *++d;
                for (n &= 0xFFFF; n != 0; n--) {
                    base = (u8 *)SORT_WORK->work;
                    p0 = base + (idx >> 16);
                    gte_lwc2(12, 0, p0);
                    gte_lwc2(17, 4, p0);
                    i1 = d[1];
                    i2 = d[2];
                    p1 = base + (i1 >> 16);
                    p2 = base + (i2 >> 16);
                    gte_lwc2(13, 0, p1);
                    gte_lwc2(18, 4, p1);
                    gte_lwc2(14, 0, p2);
                    gte_lwc2(19, 4, p2);
                    q0 = base + (idx & 0xFFFF);
                    gte_nclip();
                    q1 = base + (i1 & 0xFFFF);
                    q2 = base + (i2 & 0xFFFF);
                    gte_lwc2(20, 0, q0);
                    gte_lwc2(21, 0, q1);
                    gte_lwc2(22, 0, q2);
                    gte_lwc2(25, 4, q0);
                    gte_lwc2(26, 4, q1);
                    gte_lwc2(27, 4, q2);
                    gte_mfc2(24, opz);
                    if (SORT_WORK->textured) {
                        gte_lwc2(2, 12, d);
                        gte_lwc2(4, 16, d);
                        idx = d[6];
                        if (opz > 0) {
                            gte_avsz3();
                            gte_lwc2(3, 20, d);
                            pkt = func_8001F6C4(func_800206B0(pkt, SORT_WORK->ot, gouraud, SORT_WORK->code),
                                                SORT_WORK->ot, gouraud, SORT_WORK->code);
                        }
                        d += 6;
                        for (k = 1; k != len;) {
                            func_80020674(gouraud, idx, (u8 *)SORT_WORK->work);
                            gte_lwc2(3, 4, d);
                            idx = d[2];
                            gte_mfc2(24, opz);
                            if ((k & 1) ? opz < 0 : opz > 0) {
                                STRIP_DRAW(func_8001F6C4);
                            }
                            d += 2;
                            if (++k == len) {
                                break;
                            }
                            if (SORT_WORK->pass != 0) {
                                func_8002060C(gouraud, idx, (u8 *)SORT_WORK->work);
                                gte_lwc2(2, 4, d);
                                gte_mfc2(24, opz);
                                idx = d[2];
                                if ((k & 1) ? opz < 0 : opz > 0) {
                                    STRIP_DRAW(func_8001F6C4);
                                }
                                d += 2;
                                if (++k == len) {
                                    break;
                                }
                            }
                            func_80020638(gouraud, idx, (u8 *)SORT_WORK->work);
                            gte_lwc2(4, 4, d);
                            gte_mfc2(24, opz);
                            idx = d[2];
                            if ((k & 1) ? opz < 0 : opz > 0) {
                                STRIP_DRAW(func_8001F6C4);
                            }
                            d += 2;
                            k++;
                        }
                    } else {
                        idx = d[3];
                        if (opz > 0) {
                            STRIP_DRAW(func_8001F824);
                        }
                        d += 3;
                        for (k = 1; k != len;) {
                            func_80020674(gouraud, idx, (u8 *)SORT_WORK->work);
                            idx = *++d;
                            gte_mfc2(24, opz);
                            if ((k & 1) ? opz < 0 : opz > 0) {
                                STRIP_DRAW(func_8001F824);
                            }
                            if (++k == len) {
                                break;
                            }
                            if (SORT_WORK->pass != 0) {
                                func_8002060C(gouraud, idx, (u8 *)SORT_WORK->work);
                                idx = *++d;
                                gte_mfc2(24, opz);
                                if ((k & 1) ? opz < 0 : opz > 0) {
                                    STRIP_DRAW(func_8001F824);
                                }
                                if (++k == len) {
                                    break;
                                }
                            }
                            func_80020638(gouraud, idx, (u8 *)SORT_WORK->work);
                            idx = *++d;
                            gte_mfc2(24, opz);
                            if ((k & 1) ? opz < 0 : opz > 0) {
                                STRIP_DRAW(func_8001F824);
                            }
                            k++;
                        }
                    }
                }
                SORT_WORK->count[SORT_WORK->pass]--;
            }
        }
    }
    SORT_WORK->packet = (u32)pkt;
    SORT_WORK->data = d;
}

void *func_80020E34(void *arg0) {
    s32 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_8001AD0C(0x28);
    (*(void **)((s8 *)temp_v0 + 0)) = arg0;
    temp_v0_2 = arg0 + 0x10;
    (*(s32 *)((s8 *)temp_v0 + 4)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 8)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 0xC)) = 0;
    (*(s32 *)((s8 *)temp_v0 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(temp_v0);
    return temp_v0;
}

void func_80020E94(void *arg0, void *arg1) {
    s32 temp_v0;

    (*(void **)((s8 *)arg1 + 0)) = arg0;
    temp_v0 = arg0 + 0x10;
    (*(s32 *)((s8 *)arg1 + 4)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 8)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 0xC)) = 0;
    (*(s32 *)((s8 *)arg1 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(arg1);
}

s32 *func_80020ED4(s32 n) {
    s32 *p = func_8001AD0C(n * 4);
    s32 *q = p;
    s32 i;

    for (i = 0; i < n; i++) {
        *q++ = 0;
    }
    return p;
}

void func_80020F24(void *arg0, void *arg1) {
    func_8001AE90(arg1);
    func_8001AE90(arg0);
}

s32 func_80020F54(Script *s, s32 *regs) {
    u8 *pc;
    u16 op;
    s32 skip;
    s32 cond;
    s32 i;
    u32 next;
    u16 *arg;

    if (s->busy != 0) {
        return -1;
    }
    skip = 0;
    pc = s->pc;
    s->event = 0;
    cond = 0;
    if (s->size > s->offset) {
        do {
            op = *(u16 *)pc;
            switch (op) {
            case 6: {
                u8 *cur = pc;

                if (!skip) {
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = 0;
                }
                pc += OP_A(cur) + 4;
                break;
            }
            case 5: {
                u8 *cur = pc;

                if (!skip) {
                    pc = s->start;
                    pc += *(s32 *)(cur + 4);
                    break;
                }
                pc += 8;
                break;
            }
            case 8: {
                u8 *cur = pc;

                if (!skip) {
                    regs[OP_A(pc)] = (s32)(pc + 6);
                }
                pc += OP_B(cur) + 6;
                break;
            }
            case 7:
                if (!skip) {
                    u8 *cur = pc;

                    switch (OP_B(pc)) {
                    case 0:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] = OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] = regs[OP_VAL(cur)];
                        }
                        break;
                    case 1:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] += OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] += regs[OP_VAL(cur)];
                        }
                        break;
                    case 2:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] -= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] -= regs[OP_VAL(cur)];
                        }
                        break;
                    case 3:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] *= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] *= regs[OP_VAL(cur)];
                        }
                        break;
                    case 4:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] /= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] /= regs[OP_VAL(cur)];
                        }
                        break;
                    case 5:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] %= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] %= regs[OP_VAL(cur)];
                        }
                        break;
                    case 6:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] = rand() % (OP_VAL(cur) + 1);
                        } else {
                            regs[OP_A(cur)] = rand() % (regs[OP_VAL(cur)] + 1);
                        }
                        break;
                    }
                }
                pc += 12;
                break;
            case 9:
                if (!skip) {
                    u8 *cur = pc;

                    switch (OP_B(pc)) {
                    case 0:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] == OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] == regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 1:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] <= OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] <= regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 2:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] < OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] < regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 3:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] != OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] != regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 4:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] > OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] > regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 5:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] >= OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] >= regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    }
                } else {
                    cond = 1;
                }
                pc += 12;
                break;
            case 10: {
                u8 *cur = pc;

                if (!skip) {
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                }
                pc += 4;
                break;
            }
            case 11: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 1; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 8;
                break;
            }
            case 12: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 2; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 12;
                break;
            }
            case 13: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 3; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 16;
                break;
            }
            case 14: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 4; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 20;
                break;
            }
            }
            skip = 0;
            if (cond) {
                cond = 0;
                skip = 1;
            }
            next = (u32)pc + 3;
            pc = (u8 *)(next & ~3);
            s->offset = pc - s->base;
        } while (s->event == 0 && s->offset < s->size);
    }
    s->pc = pc;
    return s->event != 0;
}

void func_80021954(void *arg0) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = 0;
}

void func_8002195C(void *arg0, s16 arg1) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = arg1;
}

void func_80021964(u8 *m) {
    u8 *k;
    SVECTOR v;
    MATRIX mat;
    VECTOR lv;
    s32 flag;

    k = m + 0xD80;
    if (*(s16 *)(k + 0x52) == 0) {
        return;
    }
    v.vy = 0;
    v.vx = 0;
    v.vz = *(s16 *)(k + 0x52);
    PushMatrix();
    mat.t[2] = 0;
    mat.t[1] = 0;
    mat.t[0] = 0;
    gte_SetTransMatrix(&mat);
    RotMatrix(m + 0xA78, &mat);
    gte_SetRotMatrix(&mat);
    gte_ldv0(&v);
    gte_rtv0tr();
    gte_stlvnl(&lv);
    gte_stflg(&flag);
    *(s16 *)(k + 0x52) = 0;
    *(s32 *)(m + 8) += lv.vx;
    *(s32 *)(m + 0xC) += lv.vy;
    *(s32 *)(m + 0x10) += lv.vz;
    PopMatrix();
}

void func_80021AA8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 start;
    s32 end;
    s32 d0;
    s32 d1;
    s32 mid;

    start = arg4 << 0x14;
    arg0[0] = start;
    end = arg5 << 0x14;
    mid = end - start;
    d0 = mid / arg1;
    d1 = (((arg6 - arg5) << 0x14) / arg2 + d0) / 2;
    mid = d0 * 2 - (d1 + arg0[1]) / 2;
    arg0[2] = (mid - arg0[1]) / arg3;
    arg0[3] = (d1 - mid) / arg3;
}

void func_80021B60(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 d1;
    s32 t;

    t = arg4 << 0x10;
    arg0[0] = t;
    t = ((arg5 << 0x10) - t) / arg1;
    d1 = (((arg6 - arg5) << 0x10) / arg2 + t) / 2;
    t = t * 2 - (d1 + arg0[1]) / 2;
    arg0[2] = (t - arg0[1]) / arg3;
    arg0[3] = (d1 - t) / arg3;
}

void func_80021C18(u8 *m) {
    BoneAnim *b;
    s32 i;
    s32 *t;

    b = (BoneAnim *)(m + 0xD80);
    t = (s32 *)(m + 0x2200);
    t[4]--;
    i = 0;
    if (*(s32 *)(m + 0x26D8) != 0) {
        i = *(s16 *)(m + 4);
        b += i;
    }
    for (; i < *(s16 *)(m + 4) + 1; b++, i++) {
        if (t[4] >= 0) {
                b->ch[0].val += b->ch[0].d0;
                b->ch[1].val += b->ch[1].d0;
                b->ch[2].val += b->ch[2].d0;
                b->ch[3].val += b->ch[3].d0;
                b->ch[4].val += b->ch[4].d0;
                b->ch[5].val += b->ch[5].d0;
                b->ch[6].val += b->ch[6].d0;
                b->ch[7].val += b->ch[7].d0;
                b->ch[8].val += b->ch[8].d0;
        } else {
                b->ch[0].val += b->ch[0].d1;
                b->ch[1].val += b->ch[1].d1;
                b->ch[2].val += b->ch[2].d1;
                b->ch[3].val += b->ch[3].d1;
                b->ch[4].val += b->ch[4].d1;
                b->ch[5].val += b->ch[5].d1;
                b->ch[6].val += b->ch[6].d1;
                b->ch[7].val += b->ch[7].d1;
                b->ch[8].val += b->ch[8].d1;
        }
    }
}

s32 func_80021DF8(void *arg0) {
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s6;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    void *temp_s1;
    void *temp_v1;
    void *var_s2;
    void *var_s3;
    void *var_s4;

    var_s2 = arg0 + 0xD80;
    var_s3 = arg0 + 0x78;
    var_s4 = arg0 + 0xA80;
    var_s6 = 0;
    if ((*(s16 *)((s8 *)arg0 + 4)) > 0) {
        do {
            var_v0 = (*(s32 *)((s8 *)var_s2 + 0));
            if (var_v0 < 0) {
                var_v0 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 0)) = (s16) (var_v0 >> 0x14);
            var_v0_2 = (*(s32 *)((s8 *)var_s2 + 0x10));
            if (var_v0_2 < 0) {
                var_v0_2 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 2)) = (s16) (var_v0_2 >> 0x14);
            var_v0_3 = (*(s32 *)((s8 *)var_s2 + 0x20));
            if (var_v0_3 < 0) {
                var_v0_3 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 4)) = (s16) (var_v0_3 >> 0x14);
            (*(s32 *)((s8 *)var_s3 + 0x18)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x32)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 0)));
            (*(s32 *)((s8 *)var_s3 + 0x1C)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x42)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 2)));
            (*(s32 *)((s8 *)var_s3 + 0x20)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x52)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 4)));
            temp_s0 = var_s6 * 0x10;
            temp_v1 = arg0 + temp_s0;
            (*(s32 *)((s8 *)temp_v1 + 0x2000)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x62));
            (*(s32 *)((s8 *)temp_v1 + 0x2004)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x72));
            (*(s32 *)((s8 *)temp_v1 + 0x2008)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x82));
            temp_s1 = var_s3 + 4;
            RotMatrixYXZ(var_s4, temp_s1);
            (*(s32 *)((s8 *)var_s3 + 0)) = 0;
            ScaleMatrix(temp_s1, arg0 + (temp_s0 + 0x2000));
            (*(s32 *)((s8 *)var_s2 + 0)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0)) + (*(s32 *)((s8 *)var_s2 + 4)));
            (*(s32 *)((s8 *)var_s2 + 0x10)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x10)) + (*(s32 *)((s8 *)var_s2 + 0x14)));
            (*(s32 *)((s8 *)var_s2 + 0x20)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x20)) + (*(s32 *)((s8 *)var_s2 + 0x24)));
            (*(s32 *)((s8 *)var_s2 + 0x30)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x30)) + (*(s32 *)((s8 *)var_s2 + 0x34)));
            (*(s32 *)((s8 *)var_s2 + 0x40)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x40)) + (*(s32 *)((s8 *)var_s2 + 0x44)));
            (*(s32 *)((s8 *)var_s2 + 0x50)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x50)) + (*(s32 *)((s8 *)var_s2 + 0x54)));
            (*(s32 *)((s8 *)var_s2 + 0x60)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x60)) + (*(s32 *)((s8 *)var_s2 + 0x64)));
            (*(s32 *)((s8 *)var_s2 + 0x70)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x70)) + (*(s32 *)((s8 *)var_s2 + 0x74)));
            (*(s32 *)((s8 *)var_s2 + 0x80)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x80)) + (*(s32 *)((s8 *)var_s2 + 0x84)));
            var_s6 += 1;
            var_s2 += 0x90;
            var_s3 += 0x50;
            var_s4 += 8;
        } while (var_s6 < (*(s16 *)((s8 *)arg0 + 4)));
    }
    (*(s32 *)((s8 *)var_s2 + 0)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0)) + (*(s32 *)((s8 *)var_s2 + 4)));
    (*(s32 *)((s8 *)var_s2 + 0x10)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x10)) + (*(s32 *)((s8 *)var_s2 + 0x14)));
    (*(s32 *)((s8 *)var_s2 + 0x20)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x20)) + (*(s32 *)((s8 *)var_s2 + 0x24)));
    (*(s32 *)((s8 *)var_s2 + 0x30)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x30)) + (*(s32 *)((s8 *)var_s2 + 0x34)));
    (*(s32 *)((s8 *)var_s2 + 0x40)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x40)) + (*(s32 *)((s8 *)var_s2 + 0x44)));
    (*(s32 *)((s8 *)var_s2 + 0x50)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x50)) + (*(s32 *)((s8 *)var_s2 + 0x54)));
    (*(s32 *)((s8 *)var_s2 + 0x60)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x60)) + (*(s32 *)((s8 *)var_s2 + 0x64)));
    (*(s32 *)((s8 *)var_s2 + 0x70)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x70)) + (*(s32 *)((s8 *)var_s2 + 0x74)));
    temp_v0 = (*(s32 *)((s8 *)var_s2 + 0x80)) + (*(s32 *)((s8 *)var_s2 + 0x84));
    (*(s32 *)((s8 *)var_s2 + 0x80)) = temp_v0;
    return temp_v0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022100);

void func_80022B98(void) {
    s32 temp_v0;
    s32 var_s1;
    void *temp_s0;
    void *temp_v1;

    D_80079544 = 1;
    var_s1 = 0;
loop_1:
    temp_s0 = D_801D6A4C->unk13C[var_s1];
    if (D_801D6A4C->unk114[var_s1] > 0) {
        temp_v1 = temp_s0 + 0x2200;
        if ((*(s32 *)((s8 *)temp_s0 + 0x2208)) >= 0) {
            temp_v0 = (*(s32 *)((s8 *)temp_v1 + 8)) - 1;
            (*(s32 *)((s8 *)temp_v1 + 8)) = temp_v0;
            if (temp_v0 <= 0) {
                func_80022100(temp_s0, (*(s32 *)((s8 *)temp_v1 + 0x18)), 0);
            }
            func_80021DF8(temp_s0);
            func_80021C18(temp_s0);
        }
    }
    var_s1 += 1;
    if (var_s1 < 0x18) {
        goto loop_1;
    }
    var_s1 = 0;
    func_80014C08(D_800794F0);
    goto loop_1;
}

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002371C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800250F4);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010190);
