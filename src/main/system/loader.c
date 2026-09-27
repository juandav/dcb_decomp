#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/loader.h"
#include "dcb/cd_file.h"
#include "dcb/heap.h"
#include "dcb/main.h"

s32 D_8006DEF0 = 0;

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
