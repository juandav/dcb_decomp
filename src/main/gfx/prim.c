#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/prim.h"
#include "dcb/heap.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"

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
