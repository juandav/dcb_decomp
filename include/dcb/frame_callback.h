#ifndef DCB_FRAME_CALLBACK_H
#define DCB_FRAME_CALLBACK_H

#include "game.h"

#define setUV0(p, _u0, _v0) (p)->u0 = (_u0), (p)->v0 = (_v0)
#define setWH(p, _w, _h) (p)->w = _w, (p)->h = _h
#define WP ((WindowPrims *)WINDOW_PRIM_CURSOR)

#if VERSION_JP
/* jp's window: the frame's four lines, its fill, and the draw areas that
   clip the screen, the window and its contents */
typedef struct {
    /* 0x00 */ LINE_F3 lines[4];
    /* 0x60 */ POLY_F4 fill;
    /* 0x78 */ DR_MODE tpage;
    /* 0x80 */ DR_AREA drawAreas[3];
} WindowPrims;
#elif VERSION_US || VERSION_EU
typedef struct {
    /* 0x000 */ POLY_FT4 ft4a[4];
    /* 0x0A0 */ SPRT linea[4];
    /* 0x0F0 */ SPRT frame;
    /* 0x104 */ u8 tpage[8];
    /* 0x10C */ u8 fillTwin[0xC];
    /* 0x118 */ u8 twin[0xC];
    /* 0x124 */ SPRT lineb[4];
    /* 0x174 */ POLY_FT4 ft4b[2];
    /* 0x1C4 */ SPRT linec[4];
    /* 0x214 */ POLY_FT4 ft4c[2];
    /* 0x264 */ u8 drawAreas[0x30];
} WindowPrims;
#else
#error "WindowPrims: version not checked"
#endif

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

void clearFramePrimSlots(void);
void addFrameCallback(s32 callback);
void removeFrameCallback(s32 callback);

#endif /* DCB_FRAME_CALLBACK_H */
