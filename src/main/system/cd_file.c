#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/cd_file.h"
#include "dcb/text.h"

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
