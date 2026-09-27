#include "common.h"
#include "gte.h"
#include "game.h"

void func_80026E90(s32 x, s32 y, s32 n) {
    Rect16 r;
    u32 *tim;
    SprtPacket *p;
    s32 i;
    TIM_IMAGE *image;

    D_801D6B12 = x;
    D_801D6B14 = y;
    D_801D6B20 = x + 0x20;
    D_801D6B22 = y + 0xF8;
    D_801D6B10 = n;
    func_800149B8(0, -1, 0, 0x800, func_8001B144, "B:\\SYSTEM.TIM", func_800148B0());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    func_8001B438(tim, D_801D6B12, D_801D6B14, -2, -2);
    image = &D_801D4850;
    r.x = D_801D6B20;
    r.y = D_801D6B22;
    r.w = 0x20;
    r.h = 8;
    LoadImage((s16 *)&r, (s32)image->caddr);
    DrawSync(0);
    func_8001AE90(tim);
    p = func_8001ACEC(D_801D6B10 * sizeof(SprtPacket) * 2);
    for (i = 0; i < 2; i++) {
        DB(i).unk40B8 = (s32)(p + D_801D6B10 * i);
    }
    func_80027044();
    D_801D6B24 = D_800793A0->unk40B8;
}

void func_80027044(void) {
    s32 i;
    s32 j;

    if (D_801D6B10 == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < D_801D6B10; j++) {
            setDrawMode(&((SprtPacket *)DB(i).unk40B8)[j].dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            setSprt(&((SprtPacket *)DB(i).unk40B8)[j].sp);
            setSemiTrans(&((SprtPacket *)DB(i).unk40B8)[j].sp, 1);
            setShadeTex(&((SprtPacket *)DB(i).unk40B8)[j].sp, 0);
            setRGB0(&((SprtPacket *)DB(i).unk40B8)[j].sp, 0x80, 0x80, 0x80);
        }
    }
}

void func_800271D0(void) {
    D_801D6B24 = D_800793A0->unk40B8;
}

void func_800271EC(s32 arg0, s32 arg1, s32 arg2, u16 arg3, s32 arg4, s32 arg5) {
    func_80027228(arg0, arg1, (Rect16 *)arg2, (u8 *)&D_8006DF98, arg3, arg4, arg5);
}

void func_80027228(s32 x, s32 y, Rect16 *r, u8 *rgb, u16 tpage, s32 n, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = r->x;
        CUR_SPRT->sp.v0 = r->y;
        CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
        CUR_SPRT->sp.w = r->w;
        CUR_SPRT->sp.h = r->h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, tpage);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_80027410(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7) {
    func_80027458(arg0, arg1, arg2, arg3, arg4, arg5, arg6, (u8 *)&D_8006DF98, arg7);
}

void func_80027458(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 n, u8 *rgb, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = u;
        CUR_SPRT->sp.v0 = v;
        CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_80027674(s32 x, s32 y, u8 c, s32 n, s32 z, s32 w, s32 h, s32 bu, s32 bv) {
    func_800276C8(x, y, c, n, (u8 *)&D_8006DF98, z, w, h, bu, bv);
}

void func_800276C8(s32 x, s32 y, u8 c, s32 n, u8 *rgb, s32 z, s32 w, s32 h, s32 bu, s32 bv) {
    if (c > 0x20 && func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = bu + (c & 0xF) * 8;
        CUR_SPRT->sp.v0 = bv + (((c - 0x20) & 0xF0) >> 1);
        CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_8002790C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8002793C(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_8002793C(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '*':
            s++;
            switch (*s) {
            case 'a':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - '0', rgb, z);
                break;
            case 'b':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - ')', rgb, z);
                break;
            case 'c':
                s++;
                n = *s - '0';
                s++;
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                break;
            case 'd':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - 0x1C, rgb, z);
                break;
            case 'e':
                s++;
                n = *s;
                if (*s < '4') {
                    n -= '#';
                } else if (*s == 'a') {
                    n = 0x11;
                } else {
                    n = *s - '"';
                }
                func_80029A0C(x, y, 3, n, rgb, z);
                s++;
                break;
            }
        case '\n':
            x = left;
            y += 6;
            s++;
            break;
        case ' ':
            x += 4;
            s++;
            break;
        default:
            c = *s++;
            if (c >= 'a') {
                c -= 0x20;
            }
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 4;
            CUR_SPRT->sp.u0 = (c & 0xF) * 4;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 5 - 0x16;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 4;
            CUR_SPRT->sp.h = 5;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_80027DB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80027DE8(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80027DE8(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;
    s32 icon;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case 1:
            s++;
            func_80029A0C(x, y, 3, *s++ - 1, rgb, z);
            x += 6;
            break;
        case '*':
            s++;
            switch (*s) {
            case 'a':
                s++;
                func_80029A0C(x, y, 3, *s++ - '0', rgb, z);
                x += 6;
                break;
            case 'b':
                s++;
                func_80029A0C(x, y, 3, *s++ - 0x29, rgb, z);
                x += 6;
                break;
            case 'c':
                s++;
                n = *s++ - '0';
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                break;
            case 'd':
                s++;
                func_80029A0C(x, y, 3, *s++ - 0x1C, rgb, z);
                x += 6;
                break;
            case 'e':
                s++;
                if (*s < 0x34) {
                    icon = *s - 0x23;
                } else if (*s == 'a') {
                    icon = 0x11;
                } else {
                    icon = *s - 0x22;
                }
                func_80029A0C(x, y, 3, icon, rgb, z);
                x += 6;
                s++;
                break;
            }
            break;
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x = left;
            y += 6;
            s++;
            break;
        case ' ':
            x += 5;
            s++;
            break;
        default:
            c = *s++;
            if (c >= 'a') {
                c -= 0x20;
            }
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 5;
            CUR_SPRT->sp.u0 = (c & 0xF) * 4 + 0x40;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 5 - 0x16;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 4;
            CUR_SPRT->sp.h = 5;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_80028228(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028258(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80028258(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s16 clut;
    s32 top;
    s32 c;

    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    top = y - 5;
    y = top;
    while (*s != 0) {
        switch (*s) {
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x += 6;
            y = top;
            s++;
            break;
        case ' ':
            y -= 5;
            s++;
            break;
        default:
            c = *s++;
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            y -= 5;
            CUR_SPRT->sp.u0 = (c & 0xF) * 6;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 4 - 0x26;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 5;
            CUR_SPRT->sp.h = 4;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_80028558(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028588(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80028588(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case '*':
            s++;
            switch (*s) {
            case 'a':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - '0', rgb, z);
                x += 6;
                break;
            case 'b':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - ')', rgb, z);
                x += 6;
                break;
            case 'c':
                s++;
                n = *s++ - '0';
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                break;
            case 'd':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - 0x1C, rgb, z);
                x += 6;
                break;
            case 'e':
                s++;
                n = *s;
                if (*s < '4') {
                    n -= '#';
                } else if (*s == 'a') {
                    n = 0x11;
                } else {
                    n = *s - '"';
                }
                func_80029A0C(x, y, 3, n, rgb, z);
                x += 6;
                s++;
                break;
            }
            break;
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x = left;
            y += 7;
            s++;
            break;
        case ' ':
            x += 6;
            s++;
            break;
        default:
            c = *s++;
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 6;
            CUR_SPRT->sp.u0 = (c & 0xF) * 6;
            CUR_SPRT->sp.v0 = (((c - 0x20) & 0xF0) >> 4) * 6 - 0x4C;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 6;
            CUR_SPRT->sp.h = 6;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_800289A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_800289D0(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_800289D0(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case 1:
            s++;
            func_80029A0C(x, y, 1, *s++ - 1, rgb, z);
            x += 8;
            break;
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x = left;
            y += 9;
            s++;
            break;
        case ' ':
            x += 8;
            s++;
            break;
        default:
            c = *s++;
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 8;
            CUR_SPRT->sp.u0 = (c & 0xF) * 8;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 7;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 7;
            CUR_SPRT->sp.h = 7;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_80028D18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028D48(arg0, arg1, (u8 *)arg2, (u8 *)&D_8006DF98, arg3, arg4);
}

s32 func_80028D48(s32 x, s32 y, u8 *s, u8 *rgb, s32 n, s32 z) {
    s32 dx;
    s32 left;
    s32 top;
    s32 dy;
    s32 prop;
    s16 clut;
    s32 c;
    s32 t;

    dx = 0;
    left = x;
    top = y;
    dy = 0;
    prop = 1;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    D_801D6B18 = 0;
    D_801D6B1C = 0;
    while (*s != 0) {
        if (*s == '*') {
            s++;
            switch (*s) {
            case 'a':
                s++;
                func_80029A0C(x, y + 1, 0, *s++ - '0', rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'b':
                s++;
                func_80029A0C(x, y + 1, 0, *s++ - ')', rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'c':
                s++;
                n = *s - '0';
                s++;
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                continue;
            case 'd':
                s++;
                func_80029A0C(x, y + 1, 0, *s++ - 0x1C, rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'e':
                s++;
                t = *s;
                if (*s < '4') {
                    t -= '#';
                } else if (*s == 'a') {
                    t = 0x11;
                } else {
                    t = *s - '"';
                }
                func_80029A0C(x, y + 1, 0, t, rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                s++;
                continue;
            case 'g':
                s++;
                func_80029A0C(x, y, 2, *s++ - '0', rgb, z);
                x += 25 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'h':
                s++;
                if (*s == '-') {
                    s++;
                    dy = '0' - *s++;
                } else {
                    dy = *s++ - '0';
                }
                continue;
            case 's':
                s++;
                prop = *s++ - '0';
                continue;
            case 'w':
                s++;
                if (*s == '-') {
                    s++;
                    dx = '0' - *s++;
                } else {
                    dx = *s++ - '0';
                }
                continue;
            }
        }
        switch (*s) {
        case '\\':
            s++;
            if (*s != 'n') {
                s++;
                break;
            }
            s++;
            x = left;
            y += 13 + dy;
            if (D_801D6B18 < x) {
                D_801D6B18 = x;
            }
            if (D_801D6B1C < y) {
                D_801D6B1C = y;
            }
            break;
        case '\n':
            s++;
            x = left;
            y += 13 + dy;
            if (D_801D6B18 < x) {
                D_801D6B18 = x;
            }
            if (D_801D6B1C < y) {
                D_801D6B1C = y;
            }
            break;
        default:
            c = *s++ - 0x20;
            if (func_80029990() != 0) {
                return; /* no value: the caller never reads it */
            }
            x += dx;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y + 1;
            CUR_SPRT->sp.u0 = (c % 16) * 6;
            if (prop != 0) {
                CUR_SPRT->sp.u0 += D_8006DF9C[c] >> 4;
            }
            CUR_SPRT->sp.v0 = (c / 16) * 12 + 0x30;
            CUR_SPRT->sp.clut = clut;
            if (prop != 0) {
                CUR_SPRT->sp.w = (u8)(D_8006DF9C[c] & 0xF);
            } else {
                CUR_SPRT->sp.w = 6;
            }
            CUR_SPRT->sp.h = 12;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            x += CUR_SPRT->sp.w;
            if (D_801D6B18 < x) {
                D_801D6B18 = x;
            }
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
    D_801D6B18 -= left;
    D_801D6B1C = D_801D6B1C - top + 12;
    return D_801D6B18;
}

INCLUDE_ASM("asm/main/nonmatchings/game_b", func_800293FC);

void func_8002961C(s32 x, s32 y, u8 *s, u8 *rgb, s32 n, s32 z) {
    s16 clut;
    s32 g;

    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case ' ':
            x += 0x10;
            s++;
            break;
        case 'c':
            s++;
            n = *s++ & 0xF;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        default:
            g = *s++;
            switch (g) {
            case '+':
                g = 10;
                break;
            case '-':
                g = 11;
                break;
            case '=':
                g = 12;
                break;
            default:
                g -= '0';
                break;
            }
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 0xC;
            CUR_SPRT->sp.u0 = (g % 8) * 16 - 0x80;
            CUR_SPRT->sp.v0 = (g / 8) * 0x15;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 0x10;
            CUR_SPRT->sp.h = 0x15;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

s32 func_80029990(void) {
    if (D_801D6B24 == D_800793A0->unk40B8 + D_801D6B10 * 0x1C) {
        return -1;
    }
    return 0;
}

void func_800299DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80029A0C(arg0, arg1, arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80029A0C(s32 x, s32 y, s32 kind, s32 n, u8 *rgb, s32 z) {
    s16 w;
    s16 h;

    w = 0xB;
    h = 0xB;
    if (func_80029990() == 0) {
        switch (kind) {
        case 0:
            CUR_SPRT->sp.u0 = (n % 14) * 12;
            CUR_SPRT->sp.v0 = (n / 14) * 11 + 0x7F;
            if (n >= 0x15 && n < 0x18) {
                CUR_SPRT->sp.clut = getClut(D_801D6B20 + 16, D_801D6B22 + 5);
            } else if (n == 0x1B) {
                CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 3);
            } else if (n >= 0x1C && n < 0x25) {
                CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 7);
            } else {
                CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            }
            w = 12;
            h = 11;
            break;
        case 1:
            CUR_SPRT->sp.u0 = n * 8;
            CUR_SPRT->sp.v0 = 0x78;
            CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            w = 7;
            h = 7;
            break;
        case 2:
            if (n < 10) {
                CUR_SPRT->sp.u0 = 0xD8;
                CUR_SPRT->sp.v0 = n * 12 + 0x18;
            } else {
                CUR_SPRT->sp.u0 = 0xC0;
                CUR_SPRT->sp.v0 = (n - 10) * 12 + 0x60;
            }
            CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            w = 0x18;
            h = 0xC;
            break;
        case 3:
            CUR_SPRT->sp.u0 = (n % 14) * 6 + 0x30;
            CUR_SPRT->sp.v0 = (n / 14) * 6 - 0x60;
            CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            w = 5;
            h = 5;
            break;
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_80029EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80029EFC(arg0, arg1, arg2, arg3, (u8 *)&D_8006DF98, arg4, (u8 *)arg5);
}

s32 func_80029EFC(s32 x, s32 y, s32 n, s32 arg3, u8 *rgb, s32 z, u8 *s) {
    s32 left;
    s32 spacing;
    s32 lineSpacing;
    s32 space;
    s32 c;

    left = x;
    spacing = 0;
    lineSpacing = 0;
    space = 6;
    if (func_80029990() != 0) {
        return;
    }
    while (*s != 0) {
        if ((u8)(*s + 0x7F) >= 0x18) {
            switch (*s) {
            case '\\':
                s++;
                if (*s == 'n') {
                    s++;
                    x = left;
                    y += 13 + lineSpacing;
                } else {
                    s++;
                }
                break;
            case 'a':
                s++;
                c = *s++;
                func_80029A0C(x, y + 1, 0, c - '0', rgb, z);
                x += 12 + spacing;
                break;
            case 'b':
                s++;
                c = *s++;
                func_80029A0C(x, y + 1, 0, c - ')', rgb, z);
                x += 12 + spacing;
                break;
            case 'c':
                s++;
                n = *s - '0';
                s++;
                break;
            case 'd':
                s++;
                c = *s++;
                func_80029A0C(x, y + 1, 0, c - 0x1C, rgb, z);
                x += 12 + spacing;
                break;
            case 'e':
                s++;
                if (*s < '4') {
                    c = *s - '#';
                } else if (*s == 'a') {
                    c = 0x11;
                } else {
                    c = *s - '"';
                }
                func_80029A0C(x, y + 1, 0, c, rgb, z);
                x += 12 + spacing;
                s++;
                break;
            case 'g':
                s++;
                c = *s++;
                func_80029A0C(x, y, 2, c - '0', rgb, z);
                x += 25 + spacing;
                break;
            case 'h':
                s++;
                if (*s == '-') {
                    s++;
                    lineSpacing = '0' - *s;
                    s++;
                } else {
                    lineSpacing = *s - '0';
                    s++;
                }
                break;
            case 'w':
                s++;
                if (*s == '-') {
                    s++;
                    spacing = '0' - *s;
                    s++;
                } else {
                    spacing = *s - '0';
                    s++;
                }
                break;
            case 'z':
                s++;
                if (space == 6) {
                    space = 12;
                } else {
                    space = 6;
                }
                break;
            case ' ':
                s++;
                x += space + spacing;
                break;
            case '\n':
                s++;
                x = left;
                y += 13;
                y += lineSpacing;
                break;
            case 's':
                s += 2;
                break;
            default:
                if ((u32)(*s - '0') < 10) {
                    if (func_80029990() != 0) {
                        return x - left;
                    }
                    CUR_SPRT->sp.x0 = x;
                    CUR_SPRT->sp.y0 = y;
                    CUR_SPRT->sp.u0 = 0x6C;
                    CUR_SPRT->sp.v0 = 0x30;
                    CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                    CUR_SPRT->sp.w = 12;
                    CUR_SPRT->sp.h = 12;
                    setSemiTrans(&CUR_SPRT->sp, 1);
                    CUR_SPRT->sp.r0 = 0x80;
                    CUR_SPRT->sp.g0 = 0x80;
                    CUR_SPRT->sp.b0 = 0x80;
                    setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
                    addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
                    addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
                    D_801D6B24 += sizeof(SprtPacket);
                    x += 6 + spacing;
                }
                s++;
                break;
            }
        } else {
            if (func_80029990() != 0) {
                return x - left;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x6C;
            CUR_SPRT->sp.v0 = 0x30;
            CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            CUR_SPRT->sp.w = 12;
            CUR_SPRT->sp.h = 12;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            x += 12 + spacing;
            s += 2;
        }
    }
    D_801D6B18 = x - left;
    D_801D6B1C = y + 12;
    return x - left;
}

s8 *func_8002A5B4(s8 *d, s8 *s) {
    if ((*d = *s) == 0) {
        return d;
    }
    return func_8002A5B4(d + 1, s + 1);
}

s8 *func_8002A5DC(s8 *buf, s8 pad, s32 n, s32 width) {
    s8 *q;
    s8 *r;
    s32 cnt;
    s8 c;

    buf += width;
    q = buf;
    *buf = 0;
    cnt = 0;
    do {
        q--;
        c = n % 10 + '0';
        *q = c;
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
        if (++cnt % 3 == 0) {
            if (n == 0) {
                break;
            }
            *--q = ',';
            if (--width <= 0) {
                buf++;
                for (r = buf; q < r; r--) {
                    *r = r[-1];
                }
                q++;
            }
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = pad;
    }
    return buf;
}

s8 *func_8002A710(s8 *buf, s8 pad, s32 n, s32 width) {
    s8 *q;
    s8 *r;

    buf += width;
    q = buf;
    *buf = 0;
    do {
        *--q = n % 10 + '0';
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = pad;
    }
    return buf;
}

void func_8002A7CC(s8 *arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 0) {
        *arg0++ = '+';
    } else {
        *arg0++ = '-';
        arg1 = -arg1;
    }
    func_8002A710(arg0, '0', arg1, arg2 - 1);
}

s8 *func_8002A820(s8 *buf, s32 n) {
    s8 *s;

    switch (n) {
    case 1:
        s = "1ab";
        break;
    case 2:
        s = "2cd";
        break;
    case 3:
        s = "3ef";
        break;
    default:
        if (n < 10) {
            return func_8002A5B4(func_8002A710(buf, ' ', n, 1), "gh");
        }
        return func_8002A5B4(func_8002A710(buf, ' ', n, 2), "i");
    }
    return func_8002A5B4(buf, s);
}

s8 *func_8002A8D4(s8 *arg0, s8 *arg1, s32 arg2) {
    s32 pad;
    s32 i;
    s8 *p;

    pad = arg2 - strlen(arg1);
    if (pad < 0) {
        p = arg0;
        for (i = 0; i < arg2; i++) {
            *p++ = '*';
        }
    } else {
        pad /= 2;
        p = arg0;
        while (pad-- > 0) {
            *p++ = ' ';
            arg2--;
        }
        while ((*p = *arg1++) != 0) {
            p++;
            arg2--;
        }
        while (arg2-- > 0) {
            *p++ = ' ';
        }
    }
    *p = 0;
    return p;
}

u16 func_8002A9D4(u8 **ps) {
    u8 *s = *ps;

    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            *ps += 2;
            return ((*ps)[-2] << 8) | (*ps)[-1];
        }
    }
    return *(*ps)++;
}

u16 func_8002AA8C(u8 *s) {
    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            return (s[0] << 8) | s[1];
        }
    }
    return s[0];
}

s32 func_8002AB20(s16 *s) {
    s16 *p;
    s32 w;

    p = s;
    w = 0;
loop:
    p++;
    if (*p != 0) {
        if (*p < 0) {
            w += 1;
        } else {
            w += 2;
        }
        goto loop;
    }
    *s = w;
    return w;
}

s16 *func_8002AB5C(s16 *d, u8 *s) {
    if ((*d = -*s) == 0) {
        return d;
    }
    s++;
    d++;
    return func_8002AB5C(d, s);
}

s16 *func_8002AB84(s16 *d, s16 *s) {
    if ((*d = *s) == 0) {
        return d;
    }
    return func_8002AB84(d + 1, s + 1);
}

s16 *func_8002ABAC(s16 *buf, u8 pad, s32 n, s32 width) {
    s16 *q;
    s16 *r;
    s32 fill;

    fill = -pad;
    buf += width;
    q = buf;
    *buf = 0;
    do {
        *--q = -'0' - n % 10;
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = fill;
    }
    return buf;
}

void func_8002AC70(s16 *arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 0) {
        *arg0++ = -0x2B;
    } else {
        *arg0++ = -0x2D;
        arg1 = -arg1;
    }
    func_8002ABAC(arg0, 0x30, arg1, arg2 - 1);
}

s16 *func_8002ACC4(s16 *buf, s32 n) {
    u8 *s;

    switch (n) {
    case 1:
        s = "1st";
        break;
    case 2:
        s = "2nd";
        break;
    case 3:
        s = "3rd";
        break;
    default:
        return func_8002AB5C(func_8002ABAC(buf, ' ', n, 1), "th");
    }
    return func_8002AB5C(buf, s);
}

s8 *func_8002AD58(s8 *buf, s32 n) {
    s8 *s;

    switch (n) {
    case 1:
        s = "1ST";
        break;
    case 2:
        s = "2ND";
        break;
    case 3:
        s = "3RD";
        break;
    default:
        return func_8002A5B4(func_8002A710(buf, ' ', n, 1), "TH");
    }
    return func_8002A5B4(buf, s);
}
