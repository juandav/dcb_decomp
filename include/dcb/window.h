#ifndef DCB_WINDOW_H
#define DCB_WINDOW_H

#include "game.h"

#define setUV0(p, _u0, _v0) (p)->u0 = (_u0), (p)->v0 = (_v0)
#define setWH(p, _w, _h) (p)->w = _w, (p)->h = _h
#define WP ((PanelPrims *)WINDOW_PRIM_CURSOR)

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

extern s32 WINDOW_PRIM_CURSOR;
extern u16 WINDOW_PRIM_POOL_SIZE;
extern u16 WINDOW_TEX_X;
extern u16 WINDOW_TEX_Y;
extern u16 WINDOW_CLUT_X;
extern u16 WINDOW_CLUT_Y;
extern WindowStyle WINDOW_STYLES[];
extern Rect16 WINDOW_FILL_PATTERNS[];
extern Rect16 VSCROLL_PART_UVS[];
extern Rect16 VSCROLL_BAR_UVS[];
extern Rect16 HSCROLL_PART_UVS[];
extern Rect16 HSCROLL_BAR_UVS[];

void resetWindowPrimPool(void);
void initWindowPrimPool(s32 count);
void clearFramePrimSlots(void);
void addFrameCallback(s32 callback);
void removeFrameCallback(s32 callback);
void animateWindowTo(Unk80016F38 *win, Rect16 *target);
s32 stepWindowAnimation(Unk80016F38 *w);
void clipRectToBounds(Rect16 *rect, Rect16 *bounds);
void drawWindowFrame(Rect16 *rect, u8 style, s32 semiTrans, s32 brightness, s32 palette, s32 z);
s32 drawWindow(Unk80016F38 *w, void (*draw)(), s32 z);
void drawVerticalScrollbar(Unk80016F38 *w, s32 z);
void drawHorizontalScrollbar(Unk80016F38 *w, s32 z);
int isWindowPrimPoolFull(void);
void openWindow(void *winPtr, void *rectPtr, s32 fromPtr, s16 *viewPtr, s32 flags, s32 style, s32 brightness, s32 frames);
void scrollWindowTo(s16 *win, s32 x, s32 y);
s32 drawWindow(Unk80016F38 *, void (*)(), s32);

#endif /* DCB_WINDOW_H */
