#include "common.h"
#include "gte.h"
#include "game.h"

INCLUDE_ASM("asm/main/nonmatchings/tmd_sort", func_8001F3C0);

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

INCLUDE_ASM("asm/main/nonmatchings/tmd_sort", func_8001F94C);

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

INCLUDE_ASM("asm/main/nonmatchings/tmd_sort", func_80020440);

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
