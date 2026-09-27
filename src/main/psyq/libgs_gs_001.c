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

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80061958);

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

INCLUDE_ASM("asm/main/nonmatchings/psyq", GsSortClear);
