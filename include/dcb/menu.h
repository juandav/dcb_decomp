#ifndef DCB_MENU_H
#define DCB_MENU_H

#include "game.h"

typedef struct {
    /* 0x00 */ DR_MODE dm[2];
    /* 0x10 */ TILE prim[2];
    /* 0x30 */ Rect16 unk30;
    /* 0x38 */ Rect16 unk38;
    /* 0x40 */ Rect16 unk40;
    /* 0x48 */ Bytes4 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ s8 unk4D;
} Unk800190F4;
typedef struct {
    /* 0x00 */ Unk80016F38 *win;
    /* 0x04 */ Unk800190F4 *cursor;
    /* 0x08 */ Rect16 rect;
    /* 0x10 */ s16 col;
    /* 0x12 */ s16 prevCol;
    /* 0x14 */ s16 row;
    /* 0x16 */ s16 prevRow;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ s16 cw;
    /* 0x1C */ s16 ch;
    /* 0x1E */ s16 ncols;
    /* 0x20 */ s16 nrows;
    /* 0x22 */ u8 ox;
    /* 0x23 */ u8 oy;
    /* 0x24 */ u8 colW;
    /* 0x25 */ u8 rowH;
    /* 0x26 */ u8 active;
    /* 0x27 */ u8 moved;
    /* 0x28 */ u8 pad;
} Menu;

void setCursorHighlight(Unk800190F4 *highlight, Rect16 *rect, Bytes4 *color);
void initCursorHighlight(Unk800190F4 *highlight, Rect16 *rect, Bytes4 *color);
void moveCursorHighlight(Unk800190F4 *highlight, Rect16 *target);
void setCursorHighlightColor(void *highlight, Bytes4 *color);
void openMenu(void *menu, void *win, Unk800190F4 *highlight, Bytes4 *color);
void centerMenuOnCursor(void *menu);
void drawCursorHighlight(Unk800190F4 *highlight, s32 z);
s32 updateMenuCursor(Menu *menu);

#endif /* DCB_MENU_H */
