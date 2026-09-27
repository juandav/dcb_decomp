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

INCLUDE_ASM("asm/main/nonmatchings/tmd_sort", func_80022100);

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

INCLUDE_ASM("asm/main/nonmatchings/tmd_sort", func_8002371C);

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

INCLUDE_ASM("asm/main/nonmatchings/tmd_sort", func_800250F4);

INCLUDE_RODATA("asm/main/nonmatchings/tmd_sort", D_80010190);
