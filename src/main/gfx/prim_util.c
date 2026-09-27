#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim_util.h"

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
