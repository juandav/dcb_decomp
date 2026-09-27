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

void initCursorHighlight(Unk800190F4 *highlight, Rect16 *rect, Bytes4 *color) {
    s32 i;

    for (i = 0; i < 2; i++) {
        setDrawMode(&highlight->dm[i], 0, 0, GetTPage(0, 1, 0, 0));
        initPrimByType(0x11, &highlight->prim[i], 1, 0);
    }
    setCursorHighlight(highlight, rect, color);
}

void setCursorHighlight(Unk800190F4 *highlight, Rect16 *rect, Bytes4 *color) {
    if (rect == (Rect16 *)-1) {
        highlight->unk30.x = 0;
        highlight->unk30.y = 0;
        highlight->unk30.w = 0;
        highlight->unk30.h = 0;
    } else {
        highlight->unk30 = *rect;
    }
    highlight->unk38 = highlight->unk30;
    highlight->unk40 = highlight->unk30;
    if (color == (Bytes4 *)-1) {
        highlight->unk48.b[0] = 0;
        highlight->unk48.b[1] = 0;
        highlight->unk48.b[2] = 0x80;
    } else {
        highlight->unk48 = *color;
    }
    highlight->unk4C = 0x80;
    highlight->unk4D = 0;
}

void moveCursorHighlight(Unk800190F4 *highlight, Rect16 *target) {
    if (highlight->unk4D >= 6) {
        highlight->unk4D = 0;
    }
    highlight->unk30 = highlight->unk40;
    highlight->unk38 = *target;
}

void setCursorHighlightColor(void *highlight, Bytes4 *color) {
    *(Bytes4 *)((s8 *)highlight + 0x48) = *color;
}

void drawCursorHighlight(Unk800190F4 *highlight, s32 z) {
    if (highlight->unk4D < 6) {
        highlight->unk40.x = highlight->unk30.x + (highlight->unk38.x - highlight->unk30.x) * highlight->unk4D / 6;
        highlight->unk40.y = highlight->unk30.y + (highlight->unk38.y - highlight->unk30.y) * highlight->unk4D / 6;
        highlight->unk40.w = highlight->unk30.w + (highlight->unk38.w - highlight->unk30.w) * highlight->unk4D / 6;
        highlight->unk40.h = highlight->unk30.h + (highlight->unk38.h - highlight->unk30.h) * highlight->unk4D / 6;
        highlight->unk4D++;
    } else {
        highlight->unk40 = highlight->unk38;
    }
    (highlight->prim + FRAME_BUFFER_INDEX)->x0 = highlight->unk40.x - 2;
    (highlight->prim + FRAME_BUFFER_INDEX)->y0 = highlight->unk40.y - 1;
    (highlight->prim + FRAME_BUFFER_INDEX)->w = highlight->unk40.w + 4;
    (highlight->prim + FRAME_BUFFER_INDEX)->h = highlight->unk40.h + 2;
    (highlight->prim + FRAME_BUFFER_INDEX)->r0 = highlight->unk48.b[0] * highlight->unk4C / 128;
    (highlight->prim + FRAME_BUFFER_INDEX)->g0 = highlight->unk48.b[1] * highlight->unk4C / 128;
    (highlight->prim + FRAME_BUFFER_INDEX)->b0 = highlight->unk48.b[2] * highlight->unk4C / 128;
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &highlight->prim[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &highlight->dm[FRAME_BUFFER_INDEX]);
}

void openMenu(void *menu, void *win, Unk800190F4 *highlight, Bytes4 *color) {
    s16 view[4];

    (*(void **)((s8 *)menu + 0)) = win;
    (*(Unk800190F4 **)((s8 *)menu + 4)) = highlight;
    (*(s16 *)((s8 *)menu + 0x12)) = -1;
    (*(s16 *)((s8 *)menu + 0x16)) = -1;
    (*(s8 *)((s8 *)menu + 0x26)) = 1;
    (*(s8 *)((s8 *)menu + 0x27)) = 0;
    view[0] = 0;
    view[1] = (*(s16 *)((s8 *)menu + 0x14)) * (*(u8 *)((s8 *)menu + 0x25)) - ((*(s16 *)((s8 *)menu + 0xE)) - (*(u8 *)((s8 *)menu + 0x25))) / 2;
    view[2] = (*(u8 *)((s8 *)menu + 0x24)) * (*(s16 *)((s8 *)menu + 0x1E));
    view[3] = (*(u8 *)((s8 *)menu + 0x25)) * (*(s16 *)((s8 *)menu + 0x20));
    openWindow(win, (s8 *)menu + 8, -1, view, (*(u8 *)((s8 *)menu + 0x18)), (*(u8 *)((s8 *)menu + 0x19)), 0x80, 0xC);
    view[0] = (*(u8 *)((s8 *)menu + 0x22)) + (*(u16 *)((s8 *)win + 0));
    view[1] = (*(u8 *)((s8 *)menu + 0x23)) + (*(u16 *)((s8 *)win + 2)) + (*(s16 *)((s8 *)menu + 0x14)) * (*(u8 *)((s8 *)menu + 0x25));
    view[2] = (*(u16 *)((s8 *)menu + 0x1A));
    view[3] = (*(u16 *)((s8 *)menu + 0x1C));
    initCursorHighlight(highlight, (Rect16 *)view, color);
}

void centerMenuOnCursor(void *menu) {
    s16 target[4];
    void *win;

    win = (*(void **)((s8 *)menu + 0));
    scrollWindowTo(win, 0, (*(s16 *)((s8 *)menu + 0x14)) * (*(u8 *)((s8 *)menu + 0x25)) - ((*(s16 *)((s8 *)menu + 0xE)) - (*(u8 *)((s8 *)menu + 0x25))) / 2);
    target[0] = ((*(u16 *)((s8 *)win + 0xC)) - (*(u16 *)((s8 *)win + 0x34))) + (*(u8 *)((s8 *)menu + 0x22)) + (*(s16 *)((s8 *)menu + 0x10)) * (*(u8 *)((s8 *)menu + 0x24));
    target[1] = ((*(u16 *)((s8 *)win + 0xE)) - (*(u16 *)((s8 *)win + 0x36))) + (*(u8 *)((s8 *)menu + 0x23)) + (*(s16 *)((s8 *)menu + 0x14)) * (*(u8 *)((s8 *)menu + 0x25));
    target[2] = (*(u16 *)((s8 *)menu + 0x1A));
    target[3] = (*(u16 *)((s8 *)menu + 0x1C));
    moveCursorHighlight(*(Unk800190F4 **)((s8 *)menu + 4), (Rect16 *)target);
}

s32 updateMenuCursor(Menu *menu) {
    Unk80016F38 *win;
    Unk800190F4 *highlight;
    s16 target[4];

    win = menu->win;
    highlight = menu->cursor;
    menu->moved = 0;
    if (menu->active != 0) {
        highlight->unk4C = 0x80;
        if (menu->nrows >= 2 && menu->rowH != 0) {
            if (PAD_STATES[menu->pad]->unkE & 0x1000) {
                playMenuSound(2);
                menu->moved = 1;
                if (--menu->row < 0) {
                    scrollWindowTo((s16 *)win, 0, win->view.h - win->rect.h);
                    menu->row = menu->nrows - 1;
                } else {
                if (menu->row == 0) {
                    PAD_STATES[menu->pad]->unk10 = 0;
                }
                if (menu->row * menu->rowH < win->unk30[3]) {
                    scrollWindowTo((s16 *)win, 0, menu->row * menu->rowH);
                }
                }
            } else if (PAD_STATES[menu->pad]->unkE & 0x4000) {
                playMenuSound(2);
                menu->moved = 1;
                if (++menu->row >= menu->nrows) {
                    scrollWindowTo((s16 *)win, 0, 0);
                    menu->row = 0;
                } else {
                if (menu->row == menu->nrows - 1) {
                    PAD_STATES[menu->pad]->unk10 = 0;
                }
                if (menu->row * menu->rowH >= win->unk30[3] + win->rect.h) {
                    scrollWindowTo((s16 *)win, 0, (menu->row + 1) * menu->rowH - win->rect.h);
                }
                }
            } else if (PAD_STATES[menu->pad]->unkE & 0x1) {
                if (menu->row != 0) {
                    playMenuSound(2);
                }
                menu->moved = 1;
                menu->row -= (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row < 0) {
                    PAD_STATES[menu->pad]->unk10 = 0;
                    scrollWindowTo((s16 *)win, 0, 0);
                    menu->row = 0;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->unk10 = 0;
                    }
                    scrollWindowTo((s16 *)win, 0, win->unk30[3] - (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                }
            } else if (PAD_STATES[menu->pad]->unkE & 0x2) {
                if (menu->row != menu->nrows - 1) {
                    playMenuSound(2);
                }
                menu->moved = 1;
                menu->row += (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row >= menu->nrows) {
                    PAD_STATES[menu->pad]->unk10 = 0;
                    scrollWindowTo((s16 *)win, 0, win->view.h - win->rect.h);
                    menu->row = menu->nrows - 1;
                } else {
                    if (menu->row == menu->nrows - 1) {
                        PAD_STATES[menu->pad]->unk10 = 0;
                    }
                    scrollWindowTo((s16 *)win, 0, win->unk30[3] + (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                }
            }
        }
    } else {
        highlight->unk4C = 0x40;
    }
    if (menu->row != menu->prevRow || menu->col != menu->prevCol) {
        menu->prevCol = menu->col;
        menu->prevRow = menu->row;
        target[0] = (win->rect.x - win->unk30[2]) + menu->ox + menu->col * menu->colW;
        target[1] = (win->rect.y - win->unk30[3]) + menu->oy + menu->row * menu->rowH;
        target[2] = menu->cw;
        target[3] = menu->ch;
        moveCursorHighlight(menu->cursor, (Rect16 *)target);
    }
    drawCursorHighlight(highlight, win->z);
    return menu->col + menu->row * menu->ncols;
}

INCLUDE_RODATA("asm/main/nonmatchings/ui/menu", PATH_DRV_SUFFIX);

INCLUDE_RODATA("asm/main/nonmatchings/ui/menu", STR_TOO_MANY_WINDOWS);
