#ifndef DCB_MENU_H
#define DCB_MENU_H

#include "game.h"

typedef struct {
    /* 0x00 */ DR_MODE dm[2];
    /* 0x10 */ TILE prim[2];
    /* 0x30 */ Rect16 from;
    /* 0x38 */ Rect16 target;
    /* 0x40 */ Rect16 cur;
    /* 0x48 */ Bytes4 color;
    /* 0x4C */ u8 brightness;
    /* 0x4D */ s8 step;
} CursorHighlight;
typedef struct {
    /* 0x00 */ UiWindow *win;
    /* 0x04 */ CursorHighlight *cursor;
    /* 0x08 */ Rect16 rect;
    /* 0x10 */ s16 col;
    /* 0x12 */ s16 prevCol;
    /* 0x14 */ s16 row;
    /* 0x16 */ s16 prevRow;
    /* 0x18 */ u8 windowFlags;
    /* 0x19 */ u8 windowStyle;
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

void setCursorHighlight(CursorHighlight *highlight, Rect16 *rect, Bytes4 *color);
void initCursorHighlight(CursorHighlight *highlight, Rect16 *rect, Bytes4 *color);
void moveCursorHighlight(CursorHighlight *highlight, Rect16 *target);
void setCursorHighlightColor(CursorHighlight *highlight, Bytes4 *color);
void openMenu(Menu *menu, UiWindow *win, CursorHighlight *highlight, Bytes4 *color);
void centerMenuOnCursor(Menu *menu);
void drawCursorHighlight(CursorHighlight *highlight, s32 z);
s32 updateMenuCursor(Menu *menu);

#endif /* DCB_MENU_H */
