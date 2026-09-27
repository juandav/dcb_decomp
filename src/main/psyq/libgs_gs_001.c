#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void GsInitGraph(u_short x, u_short y, u_short intmode, u_short dith, u_short vrammode) {
    func_80061958(x, y, intmode, dith, vrammode);
    gte_init();
    D_801DBE24 = 0;
    func_80061ADC(x, y);
    GsSetDrawBuffClip();
    GsSetDrawBuffOffset();
}

extern DRAWENV D_801DBDA0;
extern DISPENV D_801DBE00;
extern u_short D_801DBE26;

void func_80061958(u_short x, u_short y, u_short intmode, u_short dith, u_short vrammode) {
    int mode = 0;

    if ((intmode >> 4 & 3) == 3) {
        mode = 3;
    }
    ResetGraph(mode);
    D_801DBDA0.ofs[0] = D_801DBDA0.ofs[1] = 0;
    D_801DBDA0.tw.x = D_801DBDA0.tw.y = D_801DBDA0.tw.w = D_801DBDA0.tw.h = 0;
    D_801DBDA0.tpage = 0;
    D_801DBDA0.dtd = dith;
    D_801DBDA0.dfe = 0;
    D_801DBDA0.isbg = 0;
    PutDrawEnv(&D_801DBDA0);
    setRECT(&D_801DBE00.disp, 0, 0, x, y);
    setRECT(&D_801DBE00.screen, 0, 0, 0, 0);
    if (GetVideoMode() == MODE_PAL) {
        D_801DBE00.screen.y = 24;
        D_801DBE00.pad0 = 1;
    }
    D_801DBE00.isinter = intmode & 1;
    D_801DBE26 = intmode & 4;
    D_801DBE00.isrgb24 = vrammode;
    PutDispEnv(&D_801DBE00);
}

typedef struct GsA04 {
    /* 0x0 */ u_short unk0;
    /* 0x2 */ u_char dith;
    /* 0x3 */ u_char unk3;
    /* 0x4 */ u_char unk4;
} GsA04;

typedef struct GsA54 {
    /* 0x0 */ u_short x;
    /* 0x2 */ u_short y;
    /* 0x4 */ u8 unk4[8];
    /* 0xC */ u_char intmode;
    /* 0xD */ u_char vrammode;
} GsA54;

extern GsA04 D_801DBDB4;
extern GsA54 D_801DBE04;
extern u_short D_801DBE26;

void GsInitGraph2(u_short x, u_short y, u_short intmode, u_short dith, u_short vrammode) {
    D_801DBDB4.unk0 = 0;
    D_801DBDB4.dith = dith;
    D_801DBDB4.unk3 = 0;
    D_801DBDB4.unk4 = 0;
    D_801DBE04.x = x;
    D_801DBE04.y = y;
    D_801DBE04.intmode = intmode & 1;
    D_801DBE26 = intmode & 4;
    D_801DBE04.vrammode = vrammode;
    func_80061ADC(x, y);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80061ADC);

/* BLK_FILL */
typedef struct {
    /* 0x0 */ u_long tag;
    /* 0x4 */ u_char r0, g0, b0, code;
    /* 0x8 */ u_short x0, y0;
    /* 0xC */ u_short w, h;
} BlkFill;

extern BlkFill D_801DBD68[2];
extern short D_801DBD88[2];
extern short D_801DBD8C[2];
extern DISPENV D_801DBE00;
extern long D_801DBE28;
extern u_short D_801DBE2C;

void GsSortClear(u_char r, u_char g, u_char b, GsOT *otp) {
    D_801DBD68[D_801DBE24].r0 = r;
    D_801DBD68[D_801DBE24].g0 = g;
    D_801DBD68[D_801DBE24].b0 = b;
    D_801DBD68[D_801DBE24].x0 = D_801DBD88[D_801DBE24];
    D_801DBD68[D_801DBE24].y0 = D_801DBD8C[D_801DBE24];
    D_801DBD68[D_801DBE24].h = D_801DBE2C;
    if (D_801DBE00.isrgb24) {
        D_801DBD68[D_801DBE24].w = D_801DBE28 * 3 / 2;
    } else {
        D_801DBD68[D_801DBE24].w = D_801DBE28;
    }
    AddPrim(otp->tag, &D_801DBD68[D_801DBE24]);
}
