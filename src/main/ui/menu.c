#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/menu.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

void initCursorHighlight(CursorHighlight *highlight, Rect16 *rect, Bytes4 *color) {
    s32 i;

    for (i = 0; i < 2; i++) {
        setDrawMode(&highlight->dm[i], 0, 0, GetTPage(0, 1, 0, 0));
        initPrimByType(0x11, &highlight->prim[i], 1, 0);
    }
    setCursorHighlight(highlight, rect, color);
}

void setCursorHighlight(CursorHighlight *highlight, Rect16 *rect, Bytes4 *color) {
    if (rect == (Rect16 *)-1) {
        highlight->from.x = 0;
        highlight->from.y = 0;
        highlight->from.w = 0;
        highlight->from.h = 0;
    } else {
        highlight->from = *rect;
    }
    highlight->target = highlight->from;
    highlight->cur = highlight->from;
    if (color == (Bytes4 *)-1) {
        highlight->color.b[0] = 0;
        highlight->color.b[1] = 0;
        highlight->color.b[2] = 0x80;
    } else {
        highlight->color = *color;
    }
    highlight->brightness = 0x80;
    highlight->step = 0;
}

void moveCursorHighlight(CursorHighlight *highlight, Rect16 *target) {
    if (highlight->step >= 6) {
        highlight->step = 0;
    }
    highlight->from = highlight->cur;
    highlight->target = *target;
}

void setCursorHighlightColor(CursorHighlight *highlight, Bytes4 *color) {
    highlight->color = *color;
}

void drawCursorHighlight(CursorHighlight *highlight, s32 z) {
    if (highlight->step < 6) {
        highlight->cur.x = highlight->from.x + (highlight->target.x - highlight->from.x) * highlight->step / 6;
        highlight->cur.y = highlight->from.y + (highlight->target.y - highlight->from.y) * highlight->step / 6;
        highlight->cur.w = highlight->from.w + (highlight->target.w - highlight->from.w) * highlight->step / 6;
        highlight->cur.h = highlight->from.h + (highlight->target.h - highlight->from.h) * highlight->step / 6;
        highlight->step++;
    } else {
        highlight->cur = highlight->target;
    }
    (highlight->prim + FRAME_BUFFER_INDEX)->x0 = highlight->cur.x - 2;
    (highlight->prim + FRAME_BUFFER_INDEX)->y0 = highlight->cur.y - 1;
    (highlight->prim + FRAME_BUFFER_INDEX)->w = highlight->cur.w + 4;
    (highlight->prim + FRAME_BUFFER_INDEX)->h = highlight->cur.h + 2;
    (highlight->prim + FRAME_BUFFER_INDEX)->r0 = highlight->color.b[0] * highlight->brightness / 128;
    (highlight->prim + FRAME_BUFFER_INDEX)->g0 = highlight->color.b[1] * highlight->brightness / 128;
    (highlight->prim + FRAME_BUFFER_INDEX)->b0 = highlight->color.b[2] * highlight->brightness / 128;
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &highlight->prim[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &highlight->dm[FRAME_BUFFER_INDEX]);
}

void openMenu(Menu *menu, UiWindow *win, CursorHighlight *highlight, Bytes4 *color) {
    s16 view[4];

    menu->win = win;
    menu->cursor = highlight;
    menu->prevCol = -1;
    menu->prevRow = -1;
    menu->active = 1;
    menu->moved = 0;
    /* the window shows the whole grid, scrolled to center the current row */
    view[0] = 0;
    view[1] = menu->row * menu->rowH - (menu->rect.h - menu->rowH) / 2;
    view[2] = menu->colW * menu->ncols;
    view[3] = menu->rowH * menu->nrows;
    openWindow(win, &menu->rect, -1, view, menu->windowFlags, menu->windowStyle, 0x80, 0xC);
    view[0] = menu->ox + win->originX;
    view[1] = menu->oy + win->originY + menu->row * menu->rowH;
    view[2] = menu->cw;
    view[3] = menu->ch;
    initCursorHighlight(highlight, (Rect16 *)view, color);
}

void centerMenuOnCursor(Menu *menu) {
    s16 target[4];
    UiWindow *win;

    win = menu->win;
    scrollWindowTo((s16 *)win, 0, menu->row * menu->rowH - (menu->rect.h - menu->rowH) / 2);
    target[0] = (win->rect.x - win->scroll[2]) + menu->ox + menu->col * menu->colW;
    target[1] = (win->rect.y - win->scroll[3]) + menu->oy + menu->row * menu->rowH;
    target[2] = menu->cw;
    target[3] = menu->ch;
    moveCursorHighlight(menu->cursor, (Rect16 *)target);
}

s32 updateMenuCursor(Menu *menu) {
    UiWindow *win;
    CursorHighlight *highlight;
    s16 target[4];

    win = menu->win;
    highlight = menu->cursor;
    menu->moved = 0;
    if (menu->active != 0) {
        highlight->brightness = 0x80;
        if (menu->nrows >= 2 && menu->rowH != 0) {
            if (PAD_STATES[menu->pad]->repeat & 0x1000) {
                /* up: wraps around to the last row */
                playMenuSound(2);
                menu->moved = 1;
                if (--menu->row < 0) {
                    scrollWindowTo((s16 *)win, 0, win->view.h - win->rect.h);
                    menu->row = menu->nrows - 1;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    if (menu->row * menu->rowH < win->scroll[3]) {
                        scrollWindowTo((s16 *)win, 0, menu->row * menu->rowH);
                    }
                }
            } else if (PAD_STATES[menu->pad]->repeat & 0x4000) {
                /* down: wraps around to the first row */
                playMenuSound(2);
                menu->moved = 1;
                if (++menu->row >= menu->nrows) {
                    scrollWindowTo((s16 *)win, 0, 0);
                    menu->row = 0;
                } else {
                    if (menu->row == menu->nrows - 1) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    if (menu->row * menu->rowH >= win->scroll[3] + win->rect.h) {
                        scrollWindowTo((s16 *)win, 0, (menu->row + 1) * menu->rowH - win->rect.h);
                    }
                }
            } else if (PAD_STATES[menu->pad]->repeat & 0x1) {
                /* L2: one page up (a page is the rows that fit in the window) */
                if (menu->row != 0) {
                    playMenuSound(2);
                }
                menu->moved = 1;
                menu->row -= (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row < 0) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo((s16 *)win, 0, 0);
                    menu->row = 0;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo((s16 *)win, 0, win->scroll[3] - (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                }
            } else if (PAD_STATES[menu->pad]->repeat & 0x2) {
                /* R2: one page down */
                if (menu->row != menu->nrows - 1) {
                    playMenuSound(2);
                }
                menu->moved = 1;
                menu->row += (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row >= menu->nrows) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo((s16 *)win, 0, win->view.h - win->rect.h);
                    menu->row = menu->nrows - 1;
                } else {
                    if (menu->row == menu->nrows - 1) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo((s16 *)win, 0, win->scroll[3] + (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                }
            }
        }
    } else {
        /* an inactive menu dims its cursor */
        highlight->brightness = 0x40;
    }
    if (menu->row != menu->prevRow || menu->col != menu->prevCol) {
        menu->prevCol = menu->col;
        menu->prevRow = menu->row;
        target[0] = (win->rect.x - win->scroll[2]) + menu->ox + menu->col * menu->colW;
        target[1] = (win->rect.y - win->scroll[3]) + menu->oy + menu->row * menu->rowH;
        target[2] = menu->cw;
        target[3] = menu->ch;
        moveCursorHighlight(menu->cursor, (Rect16 *)target);
    }
    drawCursorHighlight(highlight, win->z);
    return menu->col + menu->row * menu->ncols;
}
