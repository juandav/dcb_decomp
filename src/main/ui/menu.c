#include "common.h"
#include "gte.h"
#include "game.h"

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

INCLUDE_RODATA("asm/main/nonmatchings/ui/menu", D_80010000);

INCLUDE_RODATA("asm/main/nonmatchings/ui/menu", D_80010008);

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
