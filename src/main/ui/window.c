#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/window.h"
#include "dcb/heap.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"

WindowStyle D_8006DD70[8] = {
    { { 0xE0, 0xEA, 0xEC }, { 0xE0, 0xEF, 0xF1 }, { 9, 7 }, { 0xE, 7 }, 2, 4, 0, 0xFE, 1 },
    { { 0xB8, 0xC0, 0xC2 }, { 0xD0, 0xDF, 0xE1 }, { 7, 6 }, { 0xE, 7 }, 2, 4, 0xFF, 0xFD, 1 },
    { { 0xE0, 0xE8, 0xEA }, { 0xD0, 0xD8, 0xDA }, { 7, 6 }, { 7, 6 }, 4, 4, 0xFD, 0xFD, 0 },
    { { 0xB0, 0xC0, 0xC2 }, { 0xE8, 0xF0, 0xF2 }, { 0xF, 6 }, { 7, 6 }, 4, 3, 0xFF, 0xFE, 2 },
    { { 0x88, 0x90, 0x92 }, { 0xD0, 0xDF, 0xE1 }, { 7, 6 }, { 0xE, 7 }, 3, 4, 0xFF, 0xFD, 1 },
    { { 0x80, 0x90, 0x92 }, { 0xE8, 0xF0, 0xF2 }, { 0xF, 6 }, { 7, 6 }, 5, 3, 0xFF, 0xFE, 3 },
    { { 0x98, 0xA1, 0xA3 }, { 0xCD, 0xD6, 0xD8 }, { 8, 8 }, { 8, 8 }, 2, 4, 0xFE, 0xFD, 0 },
    { { 0xB8, 0xD9, 0xDB }, { 0x99, 0xBE, 0xC0 }, { 0x20, 0x1D }, { 0x24, 0x10 }, 6, 0xD, 1, 0, 4 },
};
Rect16 D_8006DDE8[8] = {
    { 0xF8, 0xF0, 8, 8 },
    { 0xF8, 0xE8, 8, 8 },
    { 0xF8, 0xE0, 8, 8 },
    { 0xF8, 0xD8, 8, 8 },
    { 0xF8, 0xD0, 8, 8 },
    { 0xF8, 0xC8, 8, 8 },
    { 0xF8, 0xC0, 8, 8 },
    { 0xF8, 0xB8, 8, 8 },
};
Rect16 D_8006DE28[4] = {
    { 0xC8, 0xE3, 8, 0 },
    { 0xD0, 0xE8, 8, 0 },
    { 0x98, 0xE3, 8, 0 },
    { 0xA0, 0xE8, 8, 0 },
};
Rect16 D_8006DE48[8] = {
    { 0xD0, 0xE0, 8, 8 },
    { 0xD0, 0xF0, 8, 8 },
    { 0xC8, 0xE0, 8, 2 },
    { 0xC8, 0xE6, 8, 2 },
    { 0xA0, 0xE0, 8, 8 },
    { 0xA0, 0xF0, 8, 8 },
    { 0x98, 0xE0, 8, 2 },
    { 0x98, 0xE6, 8, 2 },
};
Rect16 D_8006DE88[4] = {
    { 0xCB, 0xE8, 0, 8 },
    { 0xD8, 0xE8, 0, 8 },
    { 0x9B, 0xE8, 0, 8 },
    { 0xA8, 0xE8, 0, 8 },
};
Rect16 D_8006DEA8[8] = {
    { 0xD8, 0xE0, 8, 8 },
    { 0xD8, 0xF0, 8, 8 },
    { 0xC8, 0xE8, 2, 8 },
    { 0xCE, 0xE8, 2, 8 },
    { 0xA8, 0xE0, 8, 8 },
    { 0xA8, 0xF0, 8, 8 },
    { 0x98, 0xE8, 2, 8 },
    { 0x9E, 0xE8, 2, 8 },
};
s16 D_8006DEE8[4] = { 0x100, -1, -1, 0 };

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
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[0] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[1] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[2] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[3] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[4] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[5] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[6] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[7] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[8] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[9] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[10] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[11] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[12] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[13] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[14] = 0;
        ((Unk800794F8 *)&GRAPHICS)->unk98[i].unk4078[15] = 0;
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
    buf = allocPermanentHeapBlock(D_800897EC * sizeof(PanelPrims) * 2);
    tpage = GetTPage(0, 0, D_800897EE, D_800897F0);
    for (i = 0; i < 2; i++) {
        p = (PanelPrims *)(((Unk800794F8 *)&GRAPHICS)->unk98[i].unk40BC = (s32)(buf + D_800897EC * i));
        for (k = 0; k < D_800897EC; k++, p++) {
            for (j = 0; j < 4; j++) {
                initPrimByType(0xC, &p->ft4a[j], 0, 0);
                p->ft4a[j].tpage = tpage;
                initPrimByType(0xE, &p->linea[j], 0, 0);
                initPrimByType(0xE, &p->lineb[j], 0, 0);
                initPrimByType(0xE, &p->linec[j], 0, 0);
            }
            for (j = 0; j < 2; j++) {
                initPrimByType(0xC, &p->ft4b[j], 0, 0);
                p->ft4b[j].tpage = tpage;
                initPrimByType(0xC, &p->ft4c[j], 0, 0);
                p->ft4c[j].tpage = tpage;
            }
            initPrimByType(0xE, &p->frame, 0, 0);
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
    D_800897E8 = CURRENT_FRAME_BUFFER->unk40BC;
}

void func_80016BEC(void) {
    D_800897E8 = CURRENT_FRAME_BUFFER->unk40BC;
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
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->unk264[0x18]);
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
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->unk264[0x24]);
        }
        func_80018694(w, z);
        func_80018B8C(w, z);
        SetDrawArea((DR_AREA *)&WP->unk264[0], &r);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->unk264[0]);
        draw(w, &CURRENT_FRAME_BUFFER->ot[z]);
        SetDrawArea((DR_AREA *)&WP->unk264[0xC], &r2);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->unk264[0xC]);
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
    w->unk3D += FRAME_INTERVAL;
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
        ot = &CURRENT_FRAME_BUFFER->ot[z];
        if (style & 0xF0) {
            k = (style >> 4) - 1;
            setPrimQuadRect(&WP->ft4a[0], (s8)D_8006DD70[k].left + r->x,
                          (s8)D_8006DD70[k].top + r->y - D_8006DD70[k].h[0],
                          r->w - ((s8)D_8006DD70[k].left - (s8)D_8006DD70[k].right), D_8006DD70[k].h[0]);
            setPrimQuadRect(&WP->ft4a[1], (s8)D_8006DD70[k].left + r->x,
                          (s8)D_8006DD70[k].bottom + r->y + r->h,
                          r->w - ((s8)D_8006DD70[k].left - (s8)D_8006DD70[k].right), D_8006DD70[k].h[1]);
            setPrimQuadRect(&WP->ft4a[2], (s8)D_8006DD70[k].left + r->x - D_8006DD70[k].w[0],
                          (s8)D_8006DD70[k].top + r->y, D_8006DD70[k].w[0],
                          r->h - ((s8)D_8006DD70[k].top - (s8)D_8006DD70[k].bottom));
            setPrimQuadRect(&WP->ft4a[3], (s8)D_8006DD70[k].right + r->x + r->w,
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
                setPrimQuadUvRect((u8 *)&WP->ft4a[i], uv[i].x, uv[i].y, uv[i].w, uv[i].h);
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
    setPrimQuadRect(&WP->ft4b[0], x, top + 2, 8, knob - 4);
    setPrimQuadRect(&WP->ft4b[1], x, y + 8, 8, len);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->lineb[i], D_8006DE48[i + w->unk43 * 4].x, D_8006DE48[i + w->unk43 * 4].y);
        setWH(&WP->lineb[i], D_8006DE48[i + w->unk43 * 4].w, D_8006DE48[i + w->unk43 * 4].h);
        setRGB0(&WP->lineb[i], w->unk40, w->unk40, w->unk40);
        WP->lineb[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->lineb[i]);
    }
    for (i = 0; i < 2; i++) {
        setPrimQuadUvRect((u8 *)&WP->ft4b[i], D_8006DE28[i + w->unk43 * 2].x, D_8006DE28[i + w->unk43 * 2].y, D_8006DE28[i + w->unk43 * 2].w, D_8006DE28[i + w->unk43 * 2].h);
        setRGB0(&WP->ft4b[i], w->unk40, w->unk40, w->unk40);
        WP->ft4b[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->ft4b[i]);
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
    setPrimQuadRect(&WP->ft4c[0], left + 2, y, knob - 4, 8);
    setPrimQuadRect(&WP->ft4c[1], x + 8, y, len, 8);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->linec[i], D_8006DEA8[i + w->unk43 * 4].x, D_8006DEA8[i + w->unk43 * 4].y);
        setWH(&WP->linec[i], D_8006DEA8[i + w->unk43 * 4].w, D_8006DEA8[i + w->unk43 * 4].h);
        setRGB0(&WP->linec[i], w->unk40, w->unk40, w->unk40);
        WP->linec[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->linec[i]);
    }
    for (i = 0; i < 2; i++) {
        setPrimQuadUvRect((u8 *)&WP->ft4c[i], D_8006DE88[i + w->unk43 * 2].x, D_8006DE88[i + w->unk43 * 2].y, D_8006DE88[i + w->unk43 * 2].w, D_8006DE88[i + w->unk43 * 2].h);
        setRGB0(&WP->ft4c[i], w->unk40, w->unk40, w->unk40);
        WP->ft4c[i].clut = clut;
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &WP->ft4c[i]);
    }
}

int func_80019084(void) {
    if (D_800897E8 == CURRENT_FRAME_BUFFER->unk40BC + D_800897EC * 0x294) {
        printf(D_80010008);
        return -1;
    }
    return 0;
}
