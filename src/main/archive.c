#include "common.h"
#include "gte.h"
#include "game.h"

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
