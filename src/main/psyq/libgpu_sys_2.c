#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

/* The first word of the GPU jump table (D_80076710, still in the data asm) points here */
const char D_8001389C[] = "$Id: sys.c,v 1.140 1998/01/12 07:52:27 noda Exp yos $";

extern u_long D_80076710[];
extern short D_800767D8[3][2];
extern short D_800767E4[3][2];
void func_8006A754(u_long);
int func_80066C0C(int mode);
void func_800674DC(u_char *p, int c, int n);

int ResetGraph(int mode) {
    switch (mode & 7) {
    case 0:
    case 3:
        printf("ResetGraph:jtb=%08x,env=%08x\n", D_80076710, &D_80076758);
    case 5:
        func_800674DC((u_char *)&D_80076758, 0, sizeof(GpuDebug));
        ResetCallback();
        func_8006A754((u_long)D_80076750 & 0xFFFFFF);
        D_80076758.type = func_80066C0C(mode);
        D_80076758.queue = 1;
        D_80076758.w = D_800767D8[D_80076758.type][0];
        D_80076758.h = D_800767E4[D_80076758.type][0];
        func_800674DC((u_char *)&D_80076758.draw, -1, sizeof(DRAWENV));
        func_800674DC((u_char *)&D_80076758.disp, -1, sizeof(DISPENV));
        return D_80076758.type;
    }
    if (D_80076758.level >= 2) {
        D_80076754("ResetGraph(%d)...\n", mode);
    }
    return D_80076750->unk34(1);
}

int SetGraphDebug(int level) {
    int old = D_80076758.level;

    D_80076758.level = level;
    if (D_80076758.level) {
        D_80076754("SetGraphDebug:level:%d,type:%d reverse:%d\n", D_80076758.level, D_80076758.type,
                   D_80076758.reverse);
    }
    return old;
}

int SetGraphQueue(int mode) {
    u_char old = D_80076758.queue;

    if (D_80076758.level >= 2) {
        D_80076754("SetGrapQue(%d)...\n", mode);
    }
    if (mode != D_80076758.queue) {
        D_80076750->unk34(1);
        D_80076758.queue = mode;
        DMACallback(2, NULL);
    }
    return old;
}

int GetGraphDebug(void) {
    return D_80076758.level;
}

u_long DrawSyncCallback(void (*func)()) {
    u_long old;

    if (D_80076758.level >= 2) {
        D_80076754("DrawSyncCallback(%08x)...\n", func);
    }
    old = (u_long)D_80076758.drawSyncCallback;
    D_80076758.drawSyncCallback = func;
    return old;
}

void func_800674DC(u_char *p, int c, int n);

void SetDispMask(int mask) {
    if (D_80076758.level >= 2) {
        D_80076754("SetDispMask(%d)...\n", mask);
    }
    if (mask == 0) {
        func_800674DC((u_char *)&D_80076758.disp, -1, sizeof(DISPENV));
    }
    D_80076750->unk10(mask ? 0x03000000 : 0x03000001);
}

int DrawSync(int mode) {
    if (D_80076758.level >= 2) {
        D_80076754("DrawSync(%d)...\n", mode);
    }
    return D_80076750->sync(mode);
}

void func_800649E8(char *name, RECT *rect) {
    switch (D_80076758.level) {
    case 1:
        if (rect->w > D_80076758.w || rect->w + rect->x > D_80076758.w || rect->y > D_80076758.h ||
            rect->y + rect->h > D_80076758.h || rect->w <= 0 || rect->x < 0 || rect->y < 0 || rect->h <= 0) {
            D_80076754("%s:bad RECT", name);
            D_80076754("(%d,%d)-(%d,%d)\n", rect->x, rect->y, rect->w, rect->h);
        }
        break;
    case 2:
        D_80076754("%s:", name);
        D_80076754("(%d,%d)-(%d,%d)\n", rect->x, rect->y, rect->w, rect->h);
        break;
    }
}

int ClearImage(RECT *rect, u_char r, u_char g, u_char b) {
    func_800649E8("ClearImage", rect);
    return D_80076750->addque(D_80076750->unkC, rect, 8, (b << 16) | (g << 8) | r);
}

int ClearImage2(RECT *rect, u_char r, u_char g, u_char b) {
    func_800649E8("ClearImage2", rect);
    return D_80076750->addque(D_80076750->unkC, rect, 8, 0x80000000 | (b << 16) | (g << 8) | r);
}

int LoadImage(RECT *rect, u_long *p) {
    func_800649E8("LoadImage", rect);
    return D_80076750->addque(D_80076750->unk20, rect, 8, (long)p);
}

int StoreImage(RECT *rect, u_long *p) {
    func_800649E8("StoreImage", rect);
    return D_80076750->addque(D_80076750->unk1C, rect, 8, (long)p);
}

extern u_long D_800767F0[5];

int MoveImage(RECT *rect, int x, int y) {
    func_800649E8("MoveImage", rect);
    if (rect->w == 0 || rect->h == 0) {
        return -1;
    }
    D_800767F0[2] = *(u_long *)&rect->x;
    D_800767F0[3] = (y << 16) | (x & 0xFFFF);
    D_800767F0[4] = *(u_long *)&rect->w;
    return D_80076750->addque(D_80076750->unk18, D_800767F0, sizeof(D_800767F0), 0);
}


extern u_long D_80076804[5];
extern u_long D_80076818;

u_long *ClearOTag(u_long *ot, int n) {
    u_long *term;

    if (D_80076758.level >= 2) {
        D_80076754("ClearOTag(%08x,%d)...\n", ot, n);
    }
    while (--n) {
        setlen(ot, 0);
        setaddr(ot, ot + 1);
        ot++;
    }
    term = &D_80076818;
    *term = ((u_long)D_80076804 & 0xFFFFFF) | 0x04000000;
    *ot = (u_long)term & 0xFFFFFF;
    return ot;
}

u_long *ClearOTagR(u_long *ot, int n) {
    u_long *term;

    if (D_80076758.level >= 2) {
        D_80076754("ClearOTagR(%08x,%d)...\n", ot, n);
    }
    D_80076750->unk2C(ot, n);
    term = &D_80076818;
    *term = ((u_long)D_80076804 & 0xFFFFFF) | 0x04000000;
    *ot = (u_long)term & 0xFFFFFF;
    return ot;
}

void DrawPrim(void *p) {
    int len = getlen(p);

    D_80076750->sync(0);
    D_80076750->unk14((u_long *)p + 1, len);
}

void DrawOTag(u_long *p) {
    if (D_80076758.level >= 2) {
        D_80076754("DrawOTag(%08x)...\n", p);
    }
    D_80076750->addque(D_80076750->unk18, p, 0, 0);
}

void func_800659C4(DR_ENV *dr, DRAWENV *env);

DRAWENV *PutDrawEnv(DRAWENV *env) {
    if (D_80076758.level >= 2) {
        D_80076754("PutDrawEnv(%08x)...\n", env);
    }
    func_800659C4(&env->dr_env, env);
    termPrim(&env->dr_env);
    D_80076750->addque(D_80076750->unk18, &env->dr_env, sizeof(DR_ENV), 0);
    memcpy((u_char *)&D_80076758.draw, (u_char *)env, sizeof(DRAWENV));
    return env;
}

void DrawOTagEnv(u_long *p, DRAWENV *env) {
    if (D_80076758.level >= 2) {
        D_80076754("DrawOTagEnv(%08x,&08x)...\n", p, env);
    }
    func_800659C4(&env->dr_env, env);
    setaddr(&env->dr_env, p);
    D_80076750->addque(D_80076750->unk18, &env->dr_env, sizeof(DR_ENV), 0);
    memcpy((u_char *)&D_80076758.draw, (u_char *)env, sizeof(DRAWENV));
}

DRAWENV *GetDrawEnv(DRAWENV *env) {
    memcpy((u_char *)env, (u_char *)&D_80076768, sizeof(DRAWENV));
    return env;
}

/* Horizontal display range (in GPU clocks) of each video mode, NTSC and PAL,
   and the clocks per pixel of each horizontal resolution */
typedef struct {
    u_short start;
    u_short end;
} GpuHRange;

extern GpuHRange D_8007682C[2][5];
extern u_char D_80076854[5];

#define LIMIT(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

DISPENV *PutDispEnv(DISPENV *env) {
    long h_start, h_end, v_start, v_end;
    long width;
    volatile RECT *old;
    int mode;
    u_long cmd;

    cmd = 0x08000000;
    if (D_80076758.level >= 2) {
        D_80076754("PutDispEnv(%08x)...\n", env);
    }
    D_80076750->unk10(0x05000000 | ((env->disp.y & 0x3FF) << 10) | (env->disp.x & 0x3FF));
    old = &D_80076758.disp.disp;
    if (*(long *)&D_80076758.disp.isinter != *(long *)&env->isinter || old->x != env->disp.x ||
        old->y != env->disp.y || old->w != env->disp.w || old->h != env->disp.h) {
        if ((env->pad0 = GetVideoMode()) == 1) {
            cmd |= 0x08;
        }
        if (env->isrgb24) {
            cmd |= 0x10;
        }
        if (env->isinter) {
            cmd |= 0x20;
        }
        if (D_80076758.reverse) {
            cmd |= 0x80;
        }
        if (env->disp.w > 0x118) {
            if (env->disp.w <= 0x160) {
                cmd |= 1;
            } else if (env->disp.w <= 0x190) {
                cmd |= 0x40;
            } else if (env->disp.w <= 0x230) {
                cmd |= 2;
            } else {
                cmd |= 3;
            }
        }
        if (env->disp.h <= (env->pad0 ? 0x120 : 0x100)) {
        } else {
            cmd |= 0x24;
        }
        D_80076750->unk10(cmd);
        env->pad0 = 8;
    }
    old = &D_80076758.disp.screen;
    if (old->x != env->screen.x || old->y != env->screen.y || old->w != env->screen.w ||
        old->h != env->screen.h || env->pad0 == 8) {
        env->pad0 = GetVideoMode();
        v_start = env->screen.y + (env->pad0 ? 0x13 : 0x10);
        v_end = v_start + (env->screen.h ? env->screen.h : 0xF0);
        if (env->disp.w <= 0x118) {
            mode = 0;
        } else if (env->disp.w <= 0x160) {
            mode = 1;
        } else if (env->disp.w <= 0x190) {
            mode = 2;
        } else if (env->disp.w <= 0x230) {
            mode = 3;
        } else {
            mode = 4;
        }
        h_start = D_8007682C[env->pad0][mode].start + env->screen.x * D_80076854[mode];
        width = D_8007682C[env->pad0][mode].end - D_8007682C[env->pad0][mode].start;
        h_end = h_start + (env->screen.w ? (width * env->screen.w) >> 8 : width);
        if (env->pad0) {
            h_start = LIMIT(h_start, 0x21C, 0xC94);
            h_end = LIMIT(h_end, h_start + D_80076854[mode] * 4, 0xCBC);
            v_start = LIMIT(v_start, 0x13, 0x12F);
            v_end = LIMIT(v_end, v_start + 2, 0x131);
        } else {
            h_start = LIMIT(h_start, 0x1F4, 0xCB2);
            h_end = LIMIT(h_end, h_start + D_80076854[mode] * 4, 0xCDA);
            v_start = LIMIT(v_start, 0x10, 0x101);
            v_end = LIMIT(v_end, v_start + 2, 0x102);
        }
        D_80076750->unk10(0x06000000 | ((h_end & 0xFFF) << 12) | (h_start & 0xFFF));
        D_80076750->unk10(0x07000000 | ((v_end & 0x3FF) << 10) | (v_start & 0x3FF));
    }
    memcpy((u_char *)&D_80076758.disp, (u_char *)env, sizeof(DISPENV));
    return env;
}

DISPENV *GetDispEnv(DISPENV *env) {
    memcpy((u_char *)env, (u_char *)&D_800767C4, sizeof(DISPENV));
    return env;
}

int GetODE(void) {
    return D_80076750->status() >> 31;
}

void SetDrawArea(DR_AREA *p, RECT *r) {
    setlen(p, 2);
    p->code[0] = func_80065C54(r->x, r->y);
    p->code[1] = func_80065CEC(r->x + r->w - 1, r->y + r->h - 1);
}

void SetDrawOffset(DR_OFFSET *p, u_short *ofs) {
    setlen(p, 2);
    p->code[0] = func_80065D84(ofs[0], ofs[1]);
    p->code[1] = 0;
}

u_long func_80065C54(short x, short y);
u_long func_80065CEC(short x, short y);
u_long func_80065C34(int dfe, int dtd, int tpage);
u_long func_80065D84(short x, short y);
u_long func_80065DA0(RECT *tw);

void SetDrawEnv(DR_ENV *dr_env, DRAWENV *env) {
    u_long *p = (u_long *)dr_env;
    RECT r;
    int len;

    p[1] = func_80065C54(env->clip.x, env->clip.y);
    p[2] = func_80065CEC(env->clip.w + env->clip.x - 1, env->clip.y + env->clip.h - 1);
    p[3] = func_80065D84(env->ofs[0], env->ofs[1]);
    p[4] = func_80065C34(env->dfe, env->dtd, env->tpage);
    p[5] = func_80065DA0(&env->tw);
    p[6] = 0xE6000000;
    len = 7;
    if (env->isbg) {
        r.x = env->clip.x;
        r.y = env->clip.y;
        r.w = env->clip.w;
        r.h = env->clip.h;
        r.w = LIMIT(r.w, 0, D_80076758.w - 1);
        r.h = LIMIT(r.h, 0, D_80076758.h - 1);
        r.x -= env->ofs[0];
        r.y -= env->ofs[1];
        p[len++] = 0x60000000 | (env->b0 << 16) | (env->g0 << 8) | env->r0;
        p[len++] = *(u_long *)&r.x;
        p[len++] = *(u_long *)&r.w;
    }
    setlen(p, len - 1);
}

void func_800659C4(DR_ENV *dr_env, DRAWENV *env) {
    u_long *p = (u_long *)dr_env;
    RECT r;
    int len;

    p[1] = func_80065C54(env->clip.x, env->clip.y);
    p[2] = func_80065CEC(env->clip.w + env->clip.x - 1, env->clip.y + env->clip.h - 1);
    p[3] = func_80065D84(env->ofs[0], env->ofs[1]);
    p[4] = func_80065C34(env->dfe, env->dtd, env->tpage);
    p[5] = func_80065DA0(&env->tw);
    p[6] = 0xE6000000;
    len = 7;
    if (env->isbg) {
        r.x = env->clip.x;
        r.y = env->clip.y;
        r.w = env->clip.w;
        r.h = env->clip.h;
        r.w = LIMIT(r.w, 0, D_80076758.w - 1);
        r.h = LIMIT(r.h, 0, D_80076758.h - 1);
        if ((r.x & 0x3F) || (r.w & 0x3F)) {
            r.x -= env->ofs[0];
            r.y -= env->ofs[1];
            p[len++] = 0x60000000 | (env->b0 << 16) | (env->g0 << 8) | env->r0;
            p[len++] = *(u_long *)&r.x;
            p[len++] = *(u_long *)&r.w;
        } else {
            p[len++] = 0x02000000 | (env->b0 << 16) | (env->g0 << 8) | env->r0;
            p[len++] = *(u_long *)&r.x;
            p[len++] = *(u_long *)&r.w;
        }
    }
    setlen(p, len - 1);
}

u_long func_80065C34(int dfe, int dtd, int tpage) {
    return (dtd ? 0xE1000200 : 0xE1000000) | (dfe ? 0x400 : 0) | (tpage & 0x9FF);
}

#define CLAMP(x, lo, hi) ((x) < (lo) ? (lo) : (x) > (hi) ? (hi) : (x))

u_long func_80065C54(short x, short y) {
    x = CLAMP(x, 0, D_80076758.w - 1);
    y = CLAMP(y, 0, D_80076758.h - 1);
    return 0xE3000000 | ((y & 0x3FF) << 10) | (x & 0x3FF);
}

u_long func_80065CEC(short x, short y) {
    x = CLAMP(x, 0, D_80076758.w - 1);
    y = CLAMP(y, 0, D_80076758.h - 1);
    return 0xE4000000 | ((y & 0x3FF) << 10) | (x & 0x3FF);
}

u_long func_80065D84(short x, short y) {
    return 0xE5000000 | ((y & 0x7FF) << 11) | (x & 0x7FF);
}

u_long func_80065DA0(RECT *tw) {
    u_long code[4];
    u_long ret;

    if (tw == NULL) {
        ret = 0;
    } else {
        code[0] = (u_char)tw->x >> 3;
        code[2] = (-tw->w & 0xFF) >> 3;
        code[1] = (u_char)tw->y >> 3;
        code[3] = (-tw->h & 0xFF) >> 3;
        ret = 0xE2000000 | (code[1] << 15) | (code[0] << 10) | (code[3] << 5) | code[2];
    }
    return ret;
}

extern volatile u_long *D_80076860;

u_long func_80065E20(void) {
    return *D_80076860;
}

extern volatile u_long *D_8007687C;
extern volatile u_long *D_80076870;
extern volatile u_long *D_80076874;
extern volatile u_long *D_80076878;
void func_80066E84(void);
int func_80066EB8(void);

int func_80065E38(u_long *addr, int size) {
    *D_8007687C |= 0x8000000;
    *D_80076878 = 0;
    *D_80076870 = (u_long)&addr[size - 1];
    *D_80076874 = size;
    *D_80076878 = 0x11000002;
    func_80066E84();
    while (*D_80076878 & 0x1000000) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    return size;
}

extern u_long D_801DC0E0[10];
extern u_long D_801DC108[4];
void func_80066660(u_long addr);
u_long func_800666A8(u_long cmd);

int func_80065F18(RECT *rect, u_long color) {
    rect->w = LIMIT(rect->w, 0, D_80076758.w - 1);
    rect->h = LIMIT(rect->h, 0, D_80076758.h - 1);
    if ((rect->x & 0x3F) || (rect->w & 0x3F)) {
        D_801DC0E0[0] = ((u_long)D_801DC108 & 0xFFFFFF) | 0x08000000;
        D_801DC0E0[1] = 0xE3000000;
        D_801DC0E0[2] = 0xE4FFFFFF;
        D_801DC0E0[3] = 0xE5000000;
        D_801DC0E0[4] = 0xE6000000;
        D_801DC0E0[5] = (*D_80076860 & 0x7FF) | 0xE1000000 | ((color >> 31) << 10);
        D_801DC0E0[6] = (color & 0xFFFFFF) | 0x60000000;
        D_801DC0E0[7] = *(u_long *)&rect->x;
        D_801DC0E0[8] = *(u_long *)&rect->w;
        D_801DC108[0] = 0x03FFFFFF;
        D_801DC108[1] = func_800666A8(3) | 0xE3000000;
        D_801DC108[2] = func_800666A8(4) | 0xE4000000;
        D_801DC108[3] = func_800666A8(5) | 0xE5000000;
    } else {
        D_801DC0E0[0] = 0x05FFFFFF;
        D_801DC0E0[1] = 0xE6000000;
        D_801DC0E0[2] = (*D_80076860 & 0x7FF) | 0xE1000000 | ((color >> 31) << 10);
        D_801DC0E0[3] = (color & 0xFFFFFF) | 0x02000000;
        D_801DC0E0[4] = *(u_long *)&rect->x;
        D_801DC0E0[5] = *(u_long *)&rect->w;
    }
    func_80066660((u_long)D_801DC0E0);
    return 0;
}

int func_80066148(RECT *rect, u_long *p) {
    int size;
    int blocks;
    int n;
    int stp = 0;

    func_80066E84();
    rect->w = LIMIT(rect->w, 0, D_80076758.w);
    rect->h = LIMIT(rect->h, 0, D_80076758.h);
    size = (rect->w * rect->h + 1) / 2;
    if (size <= 0) {
        return -1;
    }
    n = size % 16;
    blocks = size / 16;
    while (!(*D_80076860 & 0x4000000)) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    *D_80076860 = 0x4000000;
    *D_8007685C = 0x1000000;
    *D_8007685C = stp ? 0xB0000000 : 0xA0000000;
    *D_8007685C = *(u_long *)&rect->x;
    *D_8007685C = *(u_long *)&rect->w;
    while (n--) {
        *D_8007685C = *p++;
    }
    if (blocks) {
        *D_80076860 = 0x4000002;
        *D_80076864 = (u_long)p;
        *D_80076868 = (blocks << 16) | 0x10;
        *D_8007686C = 0x1000201;
    }
    return 0;
}

int func_80066384(RECT *rect, u_long *p) {
    int size;
    int blocks;
    int n;

    func_80066E84();
    rect->w = LIMIT(rect->w, 0, D_80076758.w);
    rect->h = LIMIT(rect->h, 0, D_80076758.h);
    size = (rect->w * rect->h + 1) / 2;
    if (size <= 0) {
        return -1;
    }
    n = size % 16;
    blocks = size / 16;
    while (!(*D_80076860 & 0x4000000)) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    *D_80076860 = 0x4000000;
    *D_8007685C = 0x1000000;
    *D_8007685C = 0xC0000000;
    *D_8007685C = *(u_long *)&rect->x;
    *D_8007685C = *(u_long *)&rect->w;
    while (!(*D_80076860 & 0x8000000)) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    while (n--) {
        *p++ = *D_8007685C;
    }
    if (blocks) {
        *D_80076860 = 0x4000003;
        *D_80076864 = (u_long)p;
        *D_80076868 = (blocks << 16) | 0x10;
        *D_8007686C = 0x1000200;
    }
    return 0;
}

void func_80066604(u_long value) {
    *D_80076860 = value;
}

void func_80066618(void) {
}

int func_80066620(u_long *p, int n) {
    int i = n - 1;

    *D_80076860 = 0x04000000;
    if (n != 0) {
        do {
            *D_8007685C = *p++;
        } while (i-- != 0);
    }
    return 0;
}

void func_80066660(u_long addr) {
    *D_80076860 = 0x04000002;
    *D_80076864 = addr;
    *D_80076868 = 0;
    *D_8007686C = 0x01000401;
}

u_long func_800666A8(u_long cmd) {
    *D_80076860 = cmd | 0x10000000;
    return *D_8007685C & 0xFFFFFF;
}

int func_800666D8(int (*func)(), u_long *param, u_long value) {
    return func_800666FC(func, param, 0, value);
}

/* GPU command queue */
typedef struct GpuQueue {
    /* 0x00 */ int (*func)();
    /* 0x04 */ u_long *param;
    /* 0x08 */ u_long value;
    /* 0x0C */ u_long data[21];
} GpuQueue;

extern volatile GpuQueue D_801DC130[64];
extern volatile long D_80076880;
extern volatile long D_80076884;
extern long D_80076888;
int func_80066EB8(void);
/* libetc.h has no prototype for it */
int SetIntrMask(int mask);

int func_800666FC(int (*func)(), u_long *param, int size, u_long value) {
    int i;
    u_long v;
    GpuDebug *dbg;

    func_80066E84();
    while (((D_80076880 + 1) & 0x3F) == D_80076884) {
        if (func_80066EB8() != 0) {
            return -1;
        }
        func_800669AC();
    }
    D_80076888 = SetIntrMask(0);
    dbg = &D_80076758;
    dbg->unk8 = 1;
    if (dbg->queue == 0 || (D_80076880 == D_80076884 && !(*D_8007686C & 0x01000000) &&
                            dbg->drawSyncCallback == NULL)) {
        do {
        } while (!(*D_80076860 & 0x04000000));
        func(param, value);
        SetIntrMask(D_80076888);
        return 0;
    }
    DMACallback(2, func_800669AC);
    if (size != 0) {
        for (i = 0; i < size / 4; i++) {
            v = param[i];
            D_801DC130[D_80076880].data[i] = v;
        }
        D_801DC130[D_80076880].param = (u_long *)D_801DC130[D_80076880].data;
    } else {
        D_801DC130[D_80076880].param = param;
    }
    D_801DC130[D_80076880].value = value;
    D_801DC130[D_80076880].func = func;
    D_80076880 = (D_80076880 + 1) & 0x3F;
    SetIntrMask(D_80076888);
    func_800669AC();
    return (D_80076880 - D_80076884) & 0x3F;
}

extern long D_8007688C;

int func_800669AC(void) {
    if (*D_8007686C & 0x1000000) {
        return 1;
    }
    D_8007688C = SetIntrMask(0);
    while (D_80076880 != D_80076884 && !(*D_8007686C & 0x1000000)) {
        if (((D_80076884 + 1) & 0x3F) == D_80076880 && D_80076758.drawSyncCallback == NULL) {
            DMACallback(2, NULL);
        }
        while (!(*D_80076860 & 0x4000000)) {
        }
        D_801DC130[D_80076884].func(D_801DC130[D_80076884].param, D_801DC130[D_80076884].value);
        D_80076884 = (D_80076884 + 1) & 0x3F;
    }
    SetIntrMask(D_8007688C);
    if (D_80076880 == D_80076884 && !(*D_8007686C & 0x1000000) && D_80076758.unk8 &&
        D_80076758.drawSyncCallback != NULL) {
        D_80076758.unk8 = 0;
        D_80076758.drawSyncCallback();
    }
    return (D_80076880 - D_80076884) & 0x3F;
}

extern long D_80076890;
extern volatile u_long *D_8007687C;
int func_80066FFC(int mode);

int func_80066C0C(int mode) {
    D_80076890 = SetIntrMask(0);
    D_80076880 = D_80076884 = 0;
    switch (mode & 7) {
    case 0:
    case 5:
        *D_8007686C = 0x401;
        *D_8007687C |= 0x800;
        *D_80076860 = 0;
        func_800674DC((u_char *)D_801DC130, 0, sizeof(D_801DC130));
        break;
    case 1:
    case 3:
        *D_8007686C = 0x401;
        *D_8007687C |= 0x800;
        *D_80076860 = 0x2000000;
        *D_80076860 = 0x1000000;
        break;
    }
    SetIntrMask(D_80076890);
    if (mode & 7) {
        return 0;
    }
    return func_80066FFC(mode);
}

void func_80066E84(void);
int func_80066EB8(void);
extern volatile long D_80076880;
extern volatile long D_80076884;

int func_80066D48(int mode) {
    int n;

    if (mode == 0) {
        func_80066E84();
        while (D_80076880 != D_80076884) {
            func_800669AC();
            if (func_80066EB8()) {
                return -1;
            }
        }
        while ((*D_8007686C & 0x1000000) || !(*D_80076860 & 0x4000000)) {
            if (func_80066EB8()) {
                return -1;
            }
        }
        return 0;
    }
    n = (D_80076880 - D_80076884) & 0x3F;
    if (n) {
        func_800669AC();
    }
    if ((*D_8007686C & 0x1000000) || !(*D_80076860 & 0x4000000)) {
        if (n != 0) {
            return n;
        }
        return 1;
    }
    return n;
}

void func_80066E84(void) {
    D_80076894 = VSync(-1) + 240;
    D_80076898 = 0;
}

int func_80066EB8(void) {
    if (VSync(-1) > D_80076894 || D_80076898++ > 0xF0000) {
        (void)*D_80076860;
        printf("GPU timeout:que=%d,stat=%08x,chcr=%08x,madr=%08x\n", (D_80076880 - D_80076884) & 0x3F,
               *D_80076860, *D_8007686C, *D_80076864);
        D_80076890 = SetIntrMask(0);
        D_80076880 = D_80076884 = 0;
        *D_8007686C = 0x401;
        *D_8007687C |= 0x800;
        *D_80076860 = 0x2000000;
        *D_80076860 = 0x1000000;
        SetIntrMask(D_80076890);
        return -1;
    }
    return 0;
}

int func_80066FFC(int mode) {
    *D_80076860 = 0x10000007;
    if ((*D_8007685C & 0xFFFFFF) != 2) {
        *D_8007685C = (*D_80076860 & 0x3FFF) | 0xE1001000;
        *D_8007685C;
        return 0;
    }
    if (!(mode & 8)) {
        return 1;
    }
    *D_80076860 = 0x09000001;
    return 2;
}

void func_80066E84(void);
int func_80066EB8(void);
void _GPU_ResetCallback(void);

int LoadImage2(RECT *rect, u_long *p) {
    func_800649E8("LoadImage2", rect);
    D_80076894 = VSync(-1) + 240;
    D_80076898 = 0;
    while ((*D_8007686C & 0x1000000) || !(*D_80076860 & 0x4000000)) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    DMACallback(2, _GPU_ResetCallback);
    D_80076750->unk20(rect, p);
    return 0;
}

int StoreImage2(RECT *rect, u_long *p) {
    func_800649E8("StoreImage", rect);
    D_80076894 = VSync(-1) + 240;
    D_80076898 = 0;
    while ((*D_8007686C & 0x1000000) || !(*D_80076860 & 0x4000000)) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    DMACallback(2, _GPU_ResetCallback);
    D_80076750->unk1C(rect, p);
    return 0;
}

int MoveImage2(RECT *rect, int x, int y) {
    func_800649E8("MoveImage", rect);
    D_80076894 = VSync(-1) + 240;
    D_80076898 = 0;
    while ((*D_8007686C & 0x1000000) || !(*D_80076860 & 0x4000000)) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    DMACallback(2, _GPU_ResetCallback);
    if (rect->w == 0 || rect->h == 0) {
        return -1;
    }
    D_800767F0[2] = *(u_long *)&rect->x;
    D_800767F0[3] = (y << 16) | (x & 0xFFFF);
    D_800767F0[4] = *(u_long *)&rect->w;
    D_80076750->unk18(D_800767F0);
    return 0;
}

int DrawOTag2(u_long *p) {
    if (D_80076758.level >= 2) {
        D_80076754("DrawOTag(%08x)...\n", p);
    }
    D_80076894 = VSync(-1) + 240;
    D_80076898 = 0;
    while ((*D_8007686C & 0x1000000) || !(*D_80076860 & 0x4000000)) {
        if (func_80066EB8()) {
            return -1;
        }
    }
    DMACallback(2, _GPU_ResetCallback);
    D_80076750->unk18(p);
    return 0;
}

void _GPU_ResetCallback(void) {
    DMACallback(2, func_800669AC);
}

void func_800674DC(u_char *p, int c, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = c;
        } while (i-- != 0);
    }
}

OBJECT_END(1);
