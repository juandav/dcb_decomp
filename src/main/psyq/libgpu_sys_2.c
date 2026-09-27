#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_8001389C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ResetGraph);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetGraphDebug);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetGraphQueue);

int GetGraphDebug(void) {
    return D_80076758.level;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", DrawSyncCallback);

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDispMask);

int DrawSync(int mode) {
    if (D_80076758.level >= 2) {
        D_80076754("DrawSync(%d)...\n", mode);
    }
    return D_80076750->sync(mode);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800649E8);

int ClearImage(RECT *rect, u_char r, u_char g, u_char b) {
    func_800649E8("ClearImage", rect);
    return D_80076750->addque(D_80076750->unkC, rect, 8, (b << 16) | (g << 8) | r);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", ClearImage2);

int LoadImage(RECT *rect, u_long *p) {
    func_800649E8("LoadImage", rect);
    return D_80076750->addque(D_80076750->unk20, rect, 8, (long)p);
}

int StoreImage(RECT *rect, u_long *p) {
    func_800649E8(D_800139D4, rect);
    return D_80076750->addque(D_80076750->unk1C, rect, 8, (long)p);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", MoveImage);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800139D4);

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_800139E0);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ClearOTag);

INCLUDE_ASM("asm/main/nonmatchings/psyq", ClearOTagR);

void DrawPrim(void *p) {
    int len = getlen(p);

    D_80076750->sync(0);
    D_80076750->unk14((u_long *)p + 1, len);
}

void DrawOTag(u_long *p) {
    if (D_80076758.level >= 2) {
        D_80076754(D_80013A1C, p);
    }
    D_80076750->addque(D_80076750->unk18, p, 0, 0);
}

INCLUDE_RODATA("asm/main/nonmatchings/psyq", D_80013A1C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", PutDrawEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DrawOTagEnv);

DRAWENV *GetDrawEnv(DRAWENV *env) {
    memcpy((u_char *)env, (u_char *)&D_80076768, sizeof(DRAWENV));
    return env;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", PutDispEnv);

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

INCLUDE_ASM("asm/main/nonmatchings/psyq", SetDrawEnv);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800659C4);

u_long func_80065C34(int dfe, int dtd, int tpage) {
    return (dtd ? 0xE1000200 : 0xE1000000) | (dfe ? 0x400 : 0) | (tpage & 0x9FF);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065C54);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065CEC);

u_long func_80065D84(short x, short y) {
    return 0xE5000000 | ((y & 0x7FF) << 11) | (x & 0x7FF);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065DA0);

extern u_long *D_80076860;

u_long func_80065E20(void) {
    return *D_80076860;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065E38);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80065F18);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066148);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066384);

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

int func_800666D8(int arg0, int arg1, int arg2) {
    return func_800666FC(arg0, arg1, 0, arg2);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800666FC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_800669AC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066C0C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066D48);

void func_80066E84(void) {
    D_80076894 = VSync(-1) + 240;
    D_80076898 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066EB8);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80066FFC);

INCLUDE_ASM("asm/main/nonmatchings/psyq", LoadImage2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StoreImage2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", MoveImage2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DrawOTag2);

void _GPU_ResetCallback(void) {
    DMACallback(2, func_800669AC);
}

void func_800674DC(u_char *p, u_char c, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = c;
        } while (i-- != 0);
    }
}

OBJECT_END(1);
