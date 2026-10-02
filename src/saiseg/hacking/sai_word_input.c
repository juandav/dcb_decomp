#include "common.h"
#include "game.h"
#include "dcb/sai_word_input.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/sound_play.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/pad.h"
#include "dcb/saiseg.h"
#include "dcb/sai_labels.h"

extern UiWindow SAI_WORD_GRID_WINDOW;
extern UiWindow SAI_KEYWORD_WINDOW;
extern UiWindow SAI_WORD_HELP_WINDOW;
extern WordInput SAI_WORD_INPUT;
extern CursorHighlight SAI_KEYWORD_CURSOR;
extern CursorHighlight SAI_WORD_GRID_CURSOR;
extern const char SAI_STR_NO_KEYWORD[];

int strcmp(char *a, char *b);

/* the characters of the name entry grid, ten to a row */
char SAI_WORD_INPUT_CHARS[] =
    "ABCDEabcde"
    "FGHIJfghij"
    "KLMNOklmno"
    "PQRSTpqrst"
    "UVWXYuvwxy"
    "Z-   z    "
    "          "
    "          "
    "0123456789";

/* the names SAI_runWordInput recognises when one is typed in */
char *SAI_KEYWORDS[10] = {
    "OMNIMON-1",
    "WARGREYMON",
    "OMNIMON-2",
    "MTLGARURUMON",
    "A-VEEDRAMON",
    "H-KBUTERIMON",
    "VENOMMYOTIS",
    "PIEDMON",
    "MTLETEMON",
    "JIJIMON",
};

const char D_801DE788[] = "";

s32 SAI_moveWordInputCursor(void) {
    Rect16 rect;
    UiWindow *win = &SAI_WORD_GRID_WINDOW;
    s16 r;

    if (SAI_WORD_INPUT.active != 0) {
        if (SAI_WORD_INPUT.mode == 0) {
            if ((u16)PAD_STATES[0]->repeat & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) {
                playSoundEffect(2);
            }
            do {
                if (PAD_STATES[0]->repeat & PAD_UP) {
                    SAI_WORD_INPUT.row = ((s16)(SAI_WORD_INPUT.row - 1) + 1) / 9 * 9 + ((s16)(SAI_WORD_INPUT.row - 1) + 9) % 9;
                    if (SAI_WORD_INPUT.row % 9 == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & PAD_DOWN) {
                    SAI_WORD_INPUT.row = ((s16)(SAI_WORD_INPUT.row + 1) - 1) / 9 * 9 + ((s16)(SAI_WORD_INPUT.row + 1) + 9) % 9;
                    if (SAI_WORD_INPUT.row % 9 == 8) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[0]->repeat & PAD_LEFT) {
                    if (--SAI_WORD_INPUT.col < 0) {
                        SAI_WORD_INPUT.col = 9;
                        SAI_WORD_INPUT.mode = 1;
                        SAI_WORD_INPUT.sel = 7;
                    } else if (SAI_WORD_INPUT.col == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & PAD_RIGHT) {
                    if (++SAI_WORD_INPUT.col >= 10) {
                        SAI_WORD_INPUT.col = 0;
                        SAI_WORD_INPUT.mode = 1;
                        SAI_WORD_INPUT.sel = 7;
                    } else if (SAI_WORD_INPUT.col == 9) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                }
            } while (SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col] == ' ');
        } else {
            if ((u16)PAD_STATES[0]->repeat & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) {
                playSoundEffect(2);
            }
            if ((u16)PAD_STATES[0]->pressed & (PAD_LEFT | PAD_RIGHT)) {
                playSoundEffect(2);
            }
            if (PAD_STATES[0]->repeat & PAD_UP) {
                if (--SAI_WORD_INPUT.sel < 7) {
                    SAI_WORD_INPUT.sel = 8;
                } else if (SAI_WORD_INPUT.sel == 7) {
                    PAD_STATES[0]->repeatEnabled = 0;
                }
            } else if (PAD_STATES[0]->repeat & PAD_DOWN) {
                if (++SAI_WORD_INPUT.sel >= 9) {
                    SAI_WORD_INPUT.sel = 7;
                } else if (SAI_WORD_INPUT.sel == 8) {
                    PAD_STATES[0]->repeatEnabled = 0;
                }
            } else if ((u16)PAD_STATES[0]->pressed & PAD_LEFT) {
                SAI_WORD_INPUT.mode = 0;
                SAI_WORD_INPUT.prevSel = -1;
                SAI_WORD_INPUT.prevRow = SAI_WORD_INPUT.prevCol = -1;
                SAI_WORD_INPUT.col = 9;
                while (SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col] == ' ') {
                    SAI_WORD_INPUT.col--;
                }
            } else if (PAD_STATES[0]->pressed & PAD_RIGHT) {
                SAI_WORD_INPUT.mode = 0;
                SAI_WORD_INPUT.prevSel = -1;
                SAI_WORD_INPUT.prevRow = SAI_WORD_INPUT.prevCol = -1;
                SAI_WORD_INPUT.col = 0;
                while (SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col] == ' ') {
                    SAI_WORD_INPUT.col++;
                }
            }
        }
    }
    if (SAI_WORD_INPUT.mode == 0) {
        if (SAI_WORD_INPUT.row != SAI_WORD_INPUT.prevRow || SAI_WORD_INPUT.col != SAI_WORD_INPUT.prevCol) {
            SAI_WORD_INPUT.prevCol = SAI_WORD_INPUT.col;
            SAI_WORD_INPUT.prevRow = SAI_WORD_INPUT.row;
            rect.x = win->rect.x - win->scroll[2] + SAI_WORD_INPUT.col * 17 + 4;
            rect.y = win->rect.y - win->scroll[3] + SAI_WORD_INPUT.row * 14 + 1;
            rect.w = 0xC;
            rect.h = 0xC;
            if (SAI_WORD_INPUT.col >= 5) {
                rect.x += 11;
            }
            moveCursorHighlight(&SAI_WORD_GRID_CURSOR, &rect);
        }
    } else if (SAI_WORD_INPUT.sel != SAI_WORD_INPUT.prevSel) {
        SAI_WORD_INPUT.prevSel = SAI_WORD_INPUT.sel;
        switch (SAI_WORD_INPUT.sel) {
        case 0:
        case 1:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + SAI_WORD_INPUT.sel * 14 + 1;
            rect.w = 0x30;
            rect.h = 0xC;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + SAI_WORD_INPUT.sel * 14 + 1;
            rect.w = 0x24;
            rect.h = 0xC;
            break;
        case 7:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + 0x63;
            rect.w = 0x18;
            rect.h = 0xC;
            break;
        case 8:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + 0x71;
            rect.w = 0x30;
            rect.h = 0xC;
            break;
        }
        moveCursorHighlight(&SAI_WORD_GRID_CURSOR, &rect);
    }
}

void SAI_drawWordInputGrid(UiWindow *window) {
    char text[0x40];
    u8 marks[7];
    s32 i;
    s32 x = window->originX + 4;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 gap;
    s32 dx;
    s32 sel;

    for (i = 0; i < 90; i++) {
        sprintf(text, "%c", SAI_WORD_INPUT_CHARS[i]);
        gap = 0;
        if (i % 10 >= 5) {
            gap = 11;
        }
        dx = (i % 10) * 17 + 4;
        drawText(x + dx + gap, y + (i / 10) * 14, (s32)text, 7, z);
    }
    for (i = 0; i < 7; i++) {
        marks[i] = 4;
    }
    marks[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xCC, window->rect.y + 0x63, (s32)"OK", 6, z);
    drawText(window->rect.x + 0xCC, window->rect.y + 0x71, (s32)"Cancel", 6, z);
    SAI_moveWordInputCursor();
    if (SAI_WORD_INPUT.mode == 0) {
        if (PAD_STATES[0]->repeat & PAD_CROSS) {
            SAI_WORD_INPUT.text[SAI_WORD_INPUT.cursor] = SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col];
            playSoundEffect(0);
            if (SAI_WORD_INPUT.cursor < 11) {
                SAI_WORD_INPUT.cursor++;
            } else {
                SAI_WORD_INPUT.mode = 1;
                SAI_WORD_INPUT.sel = 7;
            }
        } else if (PAD_STATES[0]->repeat & PAD_CIRCLE) {
            for (i = 11; i >= SAI_WORD_INPUT.cursor + 1; i--) {
                SAI_WORD_INPUT.text[i] = SAI_WORD_INPUT.text[i - 1];
            }
            SAI_WORD_INPUT.text[SAI_WORD_INPUT.cursor] = SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col];
            playSoundEffect(0);
            if (SAI_WORD_INPUT.cursor < 11) {
                SAI_WORD_INPUT.cursor++;
            } else {
                SAI_WORD_INPUT.mode = 1;
                SAI_WORD_INPUT.sel = 7;
            }
        } else if (PAD_STATES[0]->repeat & PAD_TRIANGLE) {
            if (SAI_WORD_INPUT.text[0] != 0) {
                playSoundEffect(0);
            }
            if (SAI_WORD_INPUT.cursor == 0) {
                SAI_WORD_INPUT.cursor++;
            }
            for (i = SAI_WORD_INPUT.cursor; i < 13; i++) {
                SAI_WORD_INPUT.text[i - 1] = SAI_WORD_INPUT.text[i];
            }
            SAI_WORD_INPUT.cursor--;
        }
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        playSoundEffect(0);
        sel = SAI_WORD_INPUT.sel;
        if (sel >= 0) {
            if (sel < 7) {
                SAI_WORD_INPUT.row = sel * 9;
                SAI_WORD_INPUT.col = 0;
                scrollWindowTo(&window->originX, 0, SAI_WORD_INPUT.row * 14);
            } else if (sel < 9) {
                SAI_WORD_INPUT.result = SAI_WORD_INPUT.sel;
            }
        }
    } else if (PAD_STATES[0]->repeat & PAD_TRIANGLE) {
        if (SAI_WORD_INPUT.text[0] != 0) {
            playSoundEffect(0);
        }
        if (SAI_WORD_INPUT.cursor == 0) {
            SAI_WORD_INPUT.cursor++;
        }
        for (i = SAI_WORD_INPUT.cursor; i < 13; i++) {
            SAI_WORD_INPUT.text[i - 1] = SAI_WORD_INPUT.text[i];
        }
        SAI_WORD_INPUT.cursor--;
    }
    if (PAD_STATES[0]->pressed & PAD_START) {
        if (SAI_WORD_INPUT.mode != 1 || SAI_WORD_INPUT.sel != 7) {
            playSoundEffect(0);
            SAI_WORD_INPUT.mode = 1;
            SAI_WORD_INPUT.sel = 7;
        }
    }
    drawCursorHighlight(&SAI_WORD_GRID_CURSOR, z);
}

void SAI_drawKeywordField(UiWindow *window) {
    char text[0x40];
    Rect16 cursor;
    s32 x = window->originX + 1;
    s32 y = window->originY;
    s32 z = window->z;

    sprintf(text, "*s0%s", SAI_WORD_INPUT.text);
    drawText(x, y, (s32)text, 7, z);
    if (PAD_STATES[0]->repeat & PAD_L1) {
        if (SAI_WORD_INPUT.cursor != 0) {
            playSoundEffect(2);
            SAI_WORD_INPUT.cursor--;
        }
    } else if (PAD_STATES[0]->repeat & PAD_R1) {
        if (SAI_WORD_INPUT.cursor != 11 && SAI_WORD_INPUT.text[SAI_WORD_INPUT.cursor] != 0) {
            playSoundEffect(2);
            SAI_WORD_INPUT.cursor++;
        }
    }
    cursor.x = x + SAI_WORD_INPUT.cursor * 6;
    cursor.y = y + 13;
    cursor.w = 6;
    cursor.h = 0;
    moveCursorHighlight(&SAI_KEYWORD_CURSOR, &cursor);
    drawCursorHighlight(&SAI_KEYWORD_CURSOR, z);
}

void SAI_drawWordInputHelp(UiWindow *window) {
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 unused[2]; /* unused, but it is in the original stack frame */

    drawText(x, y, (s32)"*b0 Insert", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b1 Delete", 7, z);
}

void SAI_drawWordInputWindows(void) {
    drawWindow(&SAI_WORD_GRID_WINDOW, SAI_drawWordInputGrid, 0x1E);
    drawWindow(&SAI_KEYWORD_WINDOW, SAI_drawKeywordField, 0x1E);
    drawWindow(&SAI_WORD_HELP_WINDOW, SAI_drawWordInputHelp, 0x1E);
}

void SAI_runWordInput(char *word) {
    Rect16 cursorRect;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    s32 i;

    bzero((Scene3D *)SAI_WORD_INPUT.text, 13);
    strcpy(SAI_WORD_INPUT.text, word);
    SAI_WORD_INPUT.col = 0;
    SAI_WORD_INPUT.row = 0;
    SAI_WORD_INPUT.prevCol = -1;
    SAI_WORD_INPUT.prevRow = -1;
    SAI_WORD_INPUT.active = 1;
    SAI_WORD_INPUT.cursor = 0;
    SAI_WORD_INPUT.mode = 0;
    SAI_WORD_INPUT.sel = 0;
    SAI_WORD_INPUT.prevSel = -1;
    SAI_WORD_INPUT.result = 0;
    rect.x = 0x10;
    rect.y = 0x5C;
    rect.w = 0x10E;
    rect.h = 0x7E;
    SAI_WORD_INPUT.rows = 9;
    view.x = 0;
    view.y = 0;
    view.w = rect.w;
    view.h = SAI_WORD_INPUT.rows * 14;
    openWindow(&SAI_WORD_GRID_WINDOW, &rect, -1, (s16 *)&view, 10, 0x26, 0x80, 12);
    SAI_WORD_GRID_WINDOW.label = (s32)"WORD INPUT";
    cursorRect.x = SAI_WORD_GRID_WINDOW.originX + 4;
    cursorRect.y = SAI_WORD_GRID_WINDOW.originY + 1;
    cursorRect.w = 12;
    cursorRect.h = 12;
    initCursorHighlight(&SAI_WORD_GRID_CURSOR, &cursorRect, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x34;
    rect.h = 0x28;
    openWindow(&SAI_WORD_HELP_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SAI_WORD_HELP_WINDOW.label = (s32)"HELP";
    rect.x = 0x78;
    rect.y = 0x1C;
    rect.w = 0x48;
    rect.h = 0xE;
    openWindow(&SAI_KEYWORD_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SAI_KEYWORD_WINDOW.label = (s32)"KEYWORD";
    cursorRect.x = SAI_KEYWORD_WINDOW.originX;
    cursorRect.y = SAI_KEYWORD_WINDOW.originY + 13;
    cursorRect.w = 12;
    cursorRect.h = 0;
    initCursorHighlight(&SAI_KEYWORD_CURSOR, &rect, (Bytes4 *)-1);
    playSoundEffect(3);
    addFrameCallback((s32)SAI_drawWordInputWindows);
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (SAI_WORD_INPUT.result == 0) {
            continue;
        }
        if (SAI_WORD_INPUT.result == 7 && SAI_WORD_INPUT.text[0] == 0) {
            initDialog(dialog, SAI_STR_NO_KEYWORD, 0);
            runDialog(dialog);
            SAI_WORD_INPUT.result = 0;
            continue;
        }
        break;
    }
    SAI_SCRIPT[0]->regs[1] = -2;
    if (SAI_WORD_INPUT.result == 7) {
        strcpy(word, SAI_WORD_INPUT.text);
        SAI_SCRIPT[0]->regs[1] = -1;
    }
    playSoundEffect(4);
    animateWindowTo(&SAI_WORD_HELP_WINDOW, (Rect16 *)-1);
    animateWindowTo(&SAI_WORD_GRID_WINDOW, (Rect16 *)-1);
    animateWindowTo(&SAI_KEYWORD_WINDOW, (Rect16 *)-1);
    waitFrames(20);
    removeFrameCallback((s32)SAI_drawWordInputWindows);
    for (i = 0; i < 10; i++) {
        if (strcmp(word, SAI_KEYWORDS[i]) == 0) {
            SAI_SCRIPT[0]->regs[1] = i;
            break;
        }
    }
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
    SAI_toggleMessageWindow(1);
}

/* the last three bytes are leftovers in the original, not zero padding, and
   not the same in every version */
#if VERSION_US
const char SAI_STR_NO_KEYWORD[36] = "A Key Word has not been entered!\0" "333";
#elif VERSION_EU
const char SAI_STR_NO_KEYWORD[36] = "A Key Word has not been entered!\0" "e1\x06";
#else
#error "saiseg/hacking/sai_word_input: version not checked"
#endif
