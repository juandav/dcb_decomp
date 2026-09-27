#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/menu.h"
#include "dcb/main.h"
#include "dcb/memcard.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"
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

void initDialog(u8 *dialog, u8 *text, u32 flags) {
    Rect16 r;
    s32 labelWidth;
    s32 width;
    s32 x;
    s32 centerX;

    dialog[0xA4] = flags & 0xF;
    dialog[0xB6] = flags & 0x80;
    *(u8 **)(dialog + 0x94) = text;
    measureText(text);
    dialog[0xA7] = (TEXT_WIDTH + 1) / 2;
    *(s16 *)(dialog + 0xA8) = (TEXT_WIDTH + 1) / 2 * 2 + 4;
    *(s16 *)(dialog + 0xAA) = (TEXT_HEIGHT + 1) / 2 * 2 + 4;
    if (dialog[0xA4] != 0) {
        if (text == 0) {
            *(s16 *)(dialog + 0xAA) = 0x10;
        } else {
            *(s16 *)(dialog + 0xAA) += 0x10;
        }
    }
    if (dialog[0xA4] != 2) {
        *(char **)(dialog + 0x98) = "Yes";
        *(char **)(dialog + 0x9C) = "No";
    }
    *(s16 *)(dialog + 0xAE) = strlen(*(u8 **)(dialog + 0x98)) * 6;
    *(s16 *)(dialog + 0xB2) = strlen(*(u8 **)(dialog + 0x9C)) * 6;
    labelWidth = *(s16 *)(dialog + 0xAE);
    if (labelWidth < *(s16 *)(dialog + 0xB2)) {
        labelWidth = *(s16 *)(dialog + 0xB2);
    }
    labelWidth = labelWidth * 2 + 0x10;
    if (*(s16 *)(dialog + 0xA8) < labelWidth) {
        *(s16 *)(dialog + 0xA8) = labelWidth;
    }
    width = *(s16 *)(dialog + 0xA8);
    x = (320 - width) / 2;
    centerX = x + width / 2;
    *(s16 *)(dialog + 0xAC) = centerX - (*(s16 *)(dialog + 0xAE) + 4);
    *(s16 *)(dialog + 0xB0) = centerX + 4;
    r.x = x;
    r.y = (240 - *(s16 *)(dialog + 0xAA)) / 2;
    r.w = *(s16 *)(dialog + 0xA8);
    r.h = *(s16 *)(dialog + 0xAA);
    openWindow(dialog, &r, -1, (s16 *)-1, 8, 0x77, 0x80, 8);
    dialog[0x38] = 4;
    if (dialog[0xA4] != 0) {
        initCursorHighlight((Unk800190F4 *)(dialog + 0x44), (Rect16 *)-1, (Bytes4 *)-1);
    }
    dialog[0xA5] = 2;
    dialog[0xA6] = 0;
    *(s32 *)(dialog + 0xA0) = 0;
    dialog[0xB5] = 0;
    dialog[0xB4] = 0;
}

s8 runDialog(void *dialog) {
    func_800149B8(0, -1, 0, 0x400, &dialogTask, dialog, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (*(s8 *)((s8 *)dialog + 0xA5));
}

s32 runDialogForPad(s32 *dialog, s32 pad) {
    s32 task;

    task = getCurrentTaskId();
    (*(s8 *)((s8 *)dialog + 0xA6)) = (s8) pad;
    func_800149B8(0, -1, 0, 0x400, dialogTask, dialog, task, 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (s32) (*(s8 *)((s8 *)dialog + 0xA5));
}

void dialogTask(u8 *dialog, s32 parentTask) {
    Rect16 r;
    s32 x;
    s32 width;
    u16 closeButtons;
    PadState **pads;
    s32 on;

    PAD_INPUT_ENABLED = 0;
    if ((s8)dialog[0xA5] == 1) {
        x = *(s16 *)(dialog + 0xAC);
        width = *(s16 *)(dialog + 0xAE);
    } else {
        x = *(s16 *)(dialog + 0xB0);
        width = *(s16 *)(dialog + 0xB2);
    }
    r.x = x;
    r.y = (240 - *(s16 *)(dialog + 0xAA)) / 2 + *(s16 *)(dialog + 0xAA) - 14;
    r.w = width;
    r.h = 12;
    setCursorHighlight((Unk800190F4 *)(dialog + 0x44), &r, (Bytes4 *)-1);
    pads = PAD_STATES;
    on = 1;
    do {
        func_80014C08(FRAME_INTERVAL);
        drawWindow((Unk80016F38 *)dialog, drawDialogBody, 0);
        if (*(void (**)(void))(dialog + 0xA0) != 0) {
            (*(void (**)(void))(dialog + 0xA0))();
        }
        if (dialog[0xA4] != 0) {
            if (dialog[0xB5] != 0) {
                closeButtons = 0x40;
            } else {
                closeButtons = 0x50;
            }
        } else {
            closeButtons = 0x40;
        }
        if (pads[dialog[0xA6]]->unk2 & closeButtons) {
            break;
        }
    } while (dialog[0xB4] == 0);
    if (dialog[0xB4] != 0) {
        dialog[0xA5] = 3;
    } else if (pads[dialog[0xA6]]->unk2 & 0x10) {
        dialog[0xA5] = 0;
        dialog[0xB4] = on;
        playMenuSound(0);
    } else {
        dialog[0xB4] = on;
        playMenuSound(1);
    }
    animateWindowTo((Unk80016F38 *)dialog, (Rect16 *)-1);
    do {
        func_80014C08(FRAME_INTERVAL);
        drawWindow((Unk80016F38 *)dialog, drawDialogBody, 0);
        if (*(void (**)(void))(dialog + 0xA0) != 0) {
            (*(void (**)(void))(dialog + 0xA0))();
        }
    } while (*(s8 *)(dialog + 0x41) == 0);
    PAD_INPUT_ENABLED = 1;
    func_80014A48(parentTask, (s8)dialog[0xA5]);
    func_80014A90();
}

void drawDialogBody(u8 *dialog) {
    Rect16 r;
    s32 x;
    s32 y;

    x = *(s16 *)dialog + (*(s16 *)(dialog + 0xA8) - dialog[0xA7] * 2) / 2;
    y = *(s16 *)(dialog + 2) + 2;
    if (*(s32 *)(dialog + 0x94) != 0) {
        drawText(x, y, *(s32 *)(dialog + 0x94), 7, *(s16 *)(dialog + 0x3A));
    }
    y = *(s16 *)(dialog + 2) + *(s16 *)(dialog + 0xAA) - 0xE;
    if (dialog[0xA4] != 0) {
        if (dialog[0xB4] == 0) {
            if ((PAD_STATES[dialog[0xA6]]->unk2 & 0x8000) && (s8)dialog[0xA5] != 1) {
                dialog[0xA5] = 1;
                r.x = *(s16 *)(dialog + 0xAC);
                r.y = y;
                r.w = *(s16 *)(dialog + 0xAE);
                r.h = 0xC;
                moveCursorHighlight((Unk800190F4 *)(dialog + 0x44), &r);
                playMenuSound(2);
            }
            if ((PAD_STATES[dialog[0xA6]]->unk2 & 0x2000) && (s8)dialog[0xA5] != 2) {
                dialog[0xA5] = 2;
                r.x = *(s16 *)(dialog + 0xB0);
                r.y = y;
                r.w = *(s16 *)(dialog + 0xB2);
                r.h = 0xC;
                moveCursorHighlight((Unk800190F4 *)(dialog + 0x44), &r);
                playMenuSound(2);
            }
        }
        drawText(*(s16 *)(dialog + 0xAC), y, *(s32 *)(dialog + 0x98), 7, *(s16 *)(dialog + 0x3A));
        drawText(*(s16 *)(dialog + 0xB0), y, *(s32 *)(dialog + 0x9C), 7, *(s16 *)(dialog + 0x3A));
        drawCursorHighlight((Unk800190F4 *)(dialog + 0x44), *(s16 *)(dialog + 0x3A));
    }
}
