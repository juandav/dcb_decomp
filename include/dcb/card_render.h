#ifndef DCB_CARD_RENDER_H
#define DCB_CARD_RENDER_H

#include "game.h"

/* a POLY_FT4 filled in whole words */
typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 rgbc;
    /* 0x08 */ u32 xy0;
    /* 0x0C */ u32 uv0;
    /* 0x10 */ u32 xy1;
    /* 0x14 */ u32 uv1;
    /* 0x18 */ u32 xy2;
    /* 0x1C */ u32 uv2;
    /* 0x20 */ u32 xy3;
    /* 0x24 */ u32 uv3;
} RawPolyFT4;

extern s32 D_8006E294;
extern s32 D_801D8344;
extern s32 D_801D8350;
extern s8 D_8006E2E4[];
extern DR_AREA D_801D8358[2];
extern DR_AREA D_801D8378[2];
extern u8 *D_8006E29C[];
extern DR_AREA D_801D8398[2];
extern DR_AREA D_801D83B8[2];
extern u8 *D_8006E2F8[];
extern char D_8001174C[];
extern s32 D_801D83F0;

void func_8004480C(void *, s32);
void func_80042BBC(s32, s32, s32, s32, u8 *);
void func_800446A4(s32, s32, s32);
void func_80044504(s32, s32, s32, s32, s32);
void func_80044800(void);
void func_80044AB0(CardSprite *, s32);
void func_80041E00(void);
void func_80042824(s32);
void func_80042E78(void);
void func_80043D00(s32);
void func_80044074(s32);
void func_800416D8(s32 n);
void func_80041A1C(void);
void func_80041CA8(u8 *s, s32 row, s32 arg2);
void func_8004269C(SprtInfo *info, s32 arg1, s32 z);
MATRIX *func_80045700(VECTOR *pos, SVECTOR *rot, MATRIX *m);

#endif /* DCB_CARD_RENDER_H */
