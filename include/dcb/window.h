#ifndef DCB_WINDOW_H
#define DCB_WINDOW_H

#include "game.h"

#define setUV0(p, _u0, _v0) (p)->u0 = (_u0), (p)->v0 = (_v0)
#define setWH(p, _w, _h) (p)->w = _w, (p)->h = _h
#define WP ((PanelPrims *)D_800897E8)

typedef struct {
    /* 0x000 */ POLY_FT4 ft4a[4];
    /* 0x0A0 */ SPRT linea[4];
    /* 0x0F0 */ SPRT frame;
    /* 0x104 */ u8 tpage[8];
    /* 0x10C */ u8 unk10C[0xC];
    /* 0x118 */ u8 twin[0xC];
    /* 0x124 */ SPRT lineb[4];
    /* 0x174 */ POLY_FT4 ft4b[2];
    /* 0x1C4 */ SPRT linec[4];
    /* 0x214 */ POLY_FT4 ft4c[2];
    /* 0x264 */ u8 unk264[0x30];
} PanelPrims;

extern s32 D_800897E8;
extern u16 D_800897EC;
extern u16 D_800897EE;
extern u16 D_800897F0;
extern u16 D_800897F2;
extern u16 D_800897F4;
extern WindowStyle D_8006DD70[];
extern Rect16 D_8006DDE8[];
extern Rect16 D_8006DE48[];
extern Rect16 D_8006DE28[];
extern Rect16 D_8006DEA8[];
extern Rect16 D_8006DE88[];

void func_80016BEC(void);
void func_80016948(s32 n);
void func_800168C4(void);
void func_8001683C(s32 arg0);
void func_80016878(s32 arg0);
void func_80016F38(Unk80016F38 *p, Rect16 *r);
s32 func_800177E8(Unk80016F38 *);
void func_800176E4(Rect16 *a, Rect16 *b);
void func_80017B88(Rect16 *, u8, s32, s32, s32, s32);
s32 func_800170F0(Unk80016F38 *w, void (*draw)(), s32 z);
void func_80018694(Unk80016F38 *w, s32 z);
void func_80018B8C(Unk80016F38 *w, s32 z);
int func_80019084(void);
void func_80016C08(void *, void *, s32, s16 *, s32, s32, s32, s32);
void func_8001705C(s16 *, s32, s32);
s32 func_800170F0(Unk80016F38 *, void (*)(), s32);

#endif /* DCB_WINDOW_H */
