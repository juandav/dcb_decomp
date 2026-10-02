#include "common.h"
#include "game.h"
#include "dcb/open_name_entry.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/menu.h"
#include "dcb/openseg.h"

extern NameEntry OPEN_NAME_ENTRY;
extern CursorHighlight OPEN_NAME_ENTRY_CURSOR;
extern CursorHighlight OPEN_NAME_CARET;
extern UiWindow OPEN_NAME_ENTRY_WINDOW;
extern UiWindow OPEN_NAME_WINDOW;
extern UiWindow OPEN_NAME_HELP_WINDOW;

/* "Is this name OK?": the string is followed by leftover bytes in the ROM, so it stays as data */
extern const char OPEN_STR_IS_THIS_NAME_OK[];

/* the letters of the name entry, ten per row */
u8 OPEN_NAME_ENTRY_LETTERS[] = "ABCDEabcde"
                  "FGHIJfghij"
                  "KLMNOklmno"
                  "PQRSTpqrst"
                  "UVWXYuvwxy"
                  "Z-   z    "
                  "          "
                  "          "
                  "0123456789";

/* not referenced by any code, and only us has it: eu has zero padding */
#if VERSION_US
u8 D_801F04BF = 0xC;
#elif VERSION_EU
#else
#error "openseg/registration/open_name_entry: version not checked"
#endif

/* moves a keyboard row by d, wrapping inside its page of 9 rows */
#define WRAP_ROW(row, d) (((s16)((row) + (d)) - (d)) / 9 * 9 + ((s16)((row) + (d)) + 9) % 9)

/* declared s32 although nothing is returned: v0 stays live at the exits */
s32 OPEN_moveNameEntryCursor(void) {
    UiWindow *window;
    Rect16 rect;

    window = &OPEN_NAME_ENTRY_WINDOW;
    if (OPEN_NAME_ENTRY.inputEnabled != 0) {
        if (OPEN_NAME_ENTRY.onSideMenu == 0) {
            if ((u16)PAD_STATES[0]->repeat & 0xF000) {
                playMenuSound(2);
            }
            do {
                if (PAD_STATES[0]->repeat & 0x1000) {
                    OPEN_NAME_ENTRY.row = WRAP_ROW(OPEN_NAME_ENTRY.row, -1);
                    if (OPEN_NAME_ENTRY.row % 9 == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x4000) {
                    OPEN_NAME_ENTRY.row = WRAP_ROW(OPEN_NAME_ENTRY.row, 1);
                    if (OPEN_NAME_ENTRY.row % 9 == 8) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[0]->repeat & 0x8000) {
                    if (--OPEN_NAME_ENTRY.col < 0) {
                        OPEN_NAME_ENTRY.col = 9;
                        OPEN_NAME_ENTRY.onSideMenu = 1;
                    } else if (OPEN_NAME_ENTRY.col == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x2000) {
                    if (++OPEN_NAME_ENTRY.col >= 10) {
                        OPEN_NAME_ENTRY.col = 0;
                        OPEN_NAME_ENTRY.onSideMenu = 1;
                    } else if (OPEN_NAME_ENTRY.col == 9) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                }
            } while (OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col] == ' ');
        } else {
            if ((u16)PAD_STATES[0]->pressed & 0xA000) {
                playMenuSound(2);
            }
            if ((u16)PAD_STATES[0]->pressed & 0x8000) {
                OPEN_NAME_ENTRY.onSideMenu = 0;
                OPEN_NAME_ENTRY.prevSideRow = -1;
                OPEN_NAME_ENTRY.prevCol = -1;
                OPEN_NAME_ENTRY.prevRow = -1;
                OPEN_NAME_ENTRY.col = 9;
                while (OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col] == ' ') {
                    OPEN_NAME_ENTRY.col--;
                }
            } else if (PAD_STATES[0]->pressed & 0x2000) {
                OPEN_NAME_ENTRY.onSideMenu = 0;
                OPEN_NAME_ENTRY.prevSideRow = -1;
                OPEN_NAME_ENTRY.prevCol = -1;
                OPEN_NAME_ENTRY.prevRow = -1;
                OPEN_NAME_ENTRY.col = 0;
                while (OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col] == ' ') {
                    OPEN_NAME_ENTRY.col++;
                }
            }
        }
    }
    if (OPEN_NAME_ENTRY.onSideMenu == 0) {
        if (OPEN_NAME_ENTRY.row != OPEN_NAME_ENTRY.prevRow || OPEN_NAME_ENTRY.col != OPEN_NAME_ENTRY.prevCol) {
            OPEN_NAME_ENTRY.prevCol = OPEN_NAME_ENTRY.col;
            OPEN_NAME_ENTRY.prevRow = OPEN_NAME_ENTRY.row;
            rect.x = window->rect.x - window->scroll[2] + OPEN_NAME_ENTRY.col * 17 + 4;
            rect.y = window->rect.y - window->scroll[3] + OPEN_NAME_ENTRY.row * 14 + 1;
            rect.w = 12;
            rect.h = 12;
            if (OPEN_NAME_ENTRY.col >= 5) {
                rect.x += 11;
            }
            moveCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, &rect);
        }
    } else if (OPEN_NAME_ENTRY.sideRow != OPEN_NAME_ENTRY.prevSideRow) {
        OPEN_NAME_ENTRY.prevSideRow = OPEN_NAME_ENTRY.sideRow;
        switch (OPEN_NAME_ENTRY.sideRow) {
        case 0:
        case 1:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + OPEN_NAME_ENTRY.sideRow * 14 + 1;
            rect.w = 0x30;
            rect.h = 12;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + OPEN_NAME_ENTRY.sideRow * 14 + 1;
            rect.w = 0x24;
            rect.h = 12;
            break;
        case 7:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x63;
            rect.w = 0x18;
            rect.h = 12;
            break;
        case 8:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x71;
            rect.w = 0x30;
            rect.h = 12;
            break;
        }
        moveCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, &rect);
    }
}

#undef WRAP_ROW

void OPEN_drawNameEntryKeyboard(UiWindow *window) {
    char text[64];
    s8 rows[7];
    s32 i;
    s32 pad;
    s32 col;
    s32 item;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX + 4;
    y = window->originY + 1;
    z = window->z;
    for (i = 0; i < 90; i++) {
        sprintf(text, "%c", OPEN_NAME_ENTRY_LETTERS[i]);
        pad = 0;
        if (i % 10 >= 5) {
            pad = 11;
        }
        col = i % 10 * 17 + 4;
        drawText(x + col + pad, y + i / 10 * 14, (s32)text, 7, z);
    }
    for (i = 0; i < 7; i++) {
        rows[i] = 4;
    }
    rows[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xD4, window->rect.y + 0x63, (s32)"OK", 6, z);
    OPEN_moveNameEntryCursor();
    if (OPEN_NAME_ENTRY.onSideMenu == 0) {
        if (PAD_STATES[0]->pressed & 0x40) {
            OPEN_NAME_ENTRY.name[OPEN_NAME_ENTRY.cursor] = OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col];
            playMenuSound(1);
            if (OPEN_NAME_ENTRY.cursor == 0 && OPEN_NAME_ENTRY.replaceName == 1) {
                for (i = 1; i < 13; i++) {
                    OPEN_NAME_ENTRY.name[i] = 0;
                }
            }
            if (OPEN_NAME_ENTRY.cursor < 11) {
                OPEN_NAME_ENTRY.cursor++;
            } else {
                OPEN_NAME_ENTRY.onSideMenu = 1;
                OPEN_NAME_ENTRY.sideRow = 7;
            }
            OPEN_NAME_ENTRY.replaceName = 0;
        } else if (PAD_STATES[0]->pressed & 0x20) {
            for (i = 11; i >= OPEN_NAME_ENTRY.cursor + 1; i--) {
                OPEN_NAME_ENTRY.name[i] = OPEN_NAME_ENTRY.name[i - 1];
            }
            OPEN_NAME_ENTRY.name[OPEN_NAME_ENTRY.cursor] = OPEN_NAME_ENTRY_LETTERS[OPEN_NAME_ENTRY.row * 10 + OPEN_NAME_ENTRY.col];
            playMenuSound(1);
            if (OPEN_NAME_ENTRY.cursor < 11) {
                OPEN_NAME_ENTRY.cursor++;
            } else {
                OPEN_NAME_ENTRY.onSideMenu = 1;
                OPEN_NAME_ENTRY.sideRow = 7;
            }
            OPEN_NAME_ENTRY.replaceName = 0;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            if (OPEN_NAME_ENTRY.name[0] != 0) {
                playMenuSound(1);
            }
            if (OPEN_NAME_ENTRY.cursor == 0) {
                OPEN_NAME_ENTRY.cursor++;
            }
            for (i = OPEN_NAME_ENTRY.cursor; i < 13; i++) {
                OPEN_NAME_ENTRY.name[i - 1] = OPEN_NAME_ENTRY.name[i];
            }
            OPEN_NAME_ENTRY.cursor--;
            OPEN_NAME_ENTRY.replaceName = 0;
        }
    } else if (PAD_STATES[0]->pressed & 0x40) {
        playMenuSound(1);
        item = OPEN_NAME_ENTRY.sideRow;
        if (item >= 0) {
            if (item < 7) {
                OPEN_NAME_ENTRY.row = item * 9;
                OPEN_NAME_ENTRY.col = 0;
                scrollWindowTo((s16 *)window, 0, OPEN_NAME_ENTRY.row * 14);
            } else if (item < 9) {
                OPEN_NAME_ENTRY.state = OPEN_NAME_ENTRY.sideRow;
            }
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        if (OPEN_NAME_ENTRY.name[0] != 0) {
            playMenuSound(1);
        }
        if (OPEN_NAME_ENTRY.cursor == 0) {
            OPEN_NAME_ENTRY.cursor++;
        }
        for (i = OPEN_NAME_ENTRY.cursor; i < 13; i++) {
            OPEN_NAME_ENTRY.name[i - 1] = OPEN_NAME_ENTRY.name[i];
        }
        OPEN_NAME_ENTRY.cursor--;
        OPEN_NAME_ENTRY.replaceName = 0;
    }
    if (PAD_STATES[0]->pressed & 0x800) {
        if (OPEN_NAME_ENTRY.onSideMenu != 1 || OPEN_NAME_ENTRY.sideRow != 7) {
            playMenuSound(1);
            OPEN_NAME_ENTRY.onSideMenu = 1;
            OPEN_NAME_ENTRY.sideRow = 7;
        }
    }
    drawCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, z);
}

void OPEN_drawNameEntryName(UiWindow *window) {
    Rect16 target;
    char text[64];
    s32 x;
    s32 y;
    s32 z;

    x = window->originX + 1;
    y = window->originY;
    z = window->z;
    sprintf(text, "*s0%s", OPEN_NAME_ENTRY.name);
    drawText(x, y, (s32)text, 7, z);
    if (PAD_STATES[0]->repeat & 4) {
        if (OPEN_NAME_ENTRY.cursor != 0) {
            playMenuSound(2);
            OPEN_NAME_ENTRY.cursor--;
        }
    } else if (PAD_STATES[0]->repeat & 8) {
        if (OPEN_NAME_ENTRY.cursor != 11 && OPEN_NAME_ENTRY.name[OPEN_NAME_ENTRY.cursor] != 0) {
            playMenuSound(2);
            OPEN_NAME_ENTRY.cursor++;
        }
    }
    target.x = x + OPEN_NAME_ENTRY.cursor * 6;
    target.y = y + 13;
    target.w = 6;
    target.h = 0;
    moveCursorHighlight(&OPEN_NAME_CARET, &target);
    drawCursorHighlight(&OPEN_NAME_CARET, z);
}

void OPEN_drawNameEntryHelp(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    Rect16 rect; /* unused, but it is in the original stack frame */

    x = window->originX + 1;
    y = window->originY + 1;
    z = window->z;
    drawText(x, y, (s32)"*b1 Delete", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b0 Insert", 7, z);
}

void OPEN_drawNameEntry(void) {
    drawWindow(&OPEN_NAME_ENTRY_WINDOW, OPEN_drawNameEntryKeyboard, 1);
    drawWindow(&OPEN_NAME_WINDOW, OPEN_drawNameEntryName, 1);
    drawWindow(&OPEN_NAME_HELP_WINDOW, OPEN_drawNameEntryHelp, 1);
}

void OPEN_runNameEntry(char *name, s32 parentTask) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];

    strcpy(OPEN_NAME_ENTRY.name, name);
    OPEN_NAME_ENTRY.col = 0;
    OPEN_NAME_ENTRY.row = 0;
    OPEN_NAME_ENTRY.prevCol = -1;
    OPEN_NAME_ENTRY.prevRow = -1;
    OPEN_NAME_ENTRY.inputEnabled = 1;
    OPEN_NAME_ENTRY.cursor = 0;
    OPEN_NAME_ENTRY.onSideMenu = 0;
    OPEN_NAME_ENTRY.sideRow = 7;
    OPEN_NAME_ENTRY.prevSideRow = -1;
    OPEN_NAME_ENTRY.state = 0;
    OPEN_NAME_ENTRY.replaceName = 1;
    rect.x = 0x18;
    rect.y = 0x5E;
    rect.w = 0x110;
    rect.h = 0x7E;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0x7E;
    openWindow(&OPEN_NAME_ENTRY_WINDOW, &rect, -1, (s16 *)&view, 10, 0x81, 0x80, 0xC);
    OPEN_NAME_ENTRY_WINDOW.label = (s32)"NAME ENTRY";
    OPEN_NAME_ENTRY_WINDOW.labelPalette = 7;
    cursor.x = OPEN_NAME_ENTRY_WINDOW.originX + 4;
    cursor.y = OPEN_NAME_ENTRY_WINDOW.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&OPEN_NAME_ENTRY_CURSOR, &cursor, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x36;
    rect.h = 0x28;
    openWindow(&OPEN_NAME_HELP_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 0xC);
    OPEN_NAME_HELP_WINDOW.label = (s32)"HELP";
    rect.x = 0xDE;
    rect.y = 0x1C;
    rect.w = 0x4A;
    rect.h = 0xE;
    openWindow(&OPEN_NAME_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 0xC);
    OPEN_NAME_WINDOW.label = (s32)"PLAYER NAME";
    cursor.x = OPEN_NAME_WINDOW.originX;
    cursor.y = OPEN_NAME_WINDOW.originY + 13;
    cursor.w = 12;
    cursor.h = 0;
    /* passes rect, not the cursor rect it just filled in */
    initCursorHighlight(&OPEN_NAME_CARET, &rect, (Bytes4 *)-1);
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawNameEntry);
    /* the same loop in both; the match depends on its form: each version's
       compiler only lays it out as the original from its own */
#if VERSION_US
    while (1) {
#elif VERSION_EU
    do {
#else
#error "openseg/registration/open_name_entry: version not checked"
#endif
        waitFrames(FRAME_INTERVAL);
        if (OPEN_NAME_ENTRY.state == 0) {
            continue;
        }
        if (OPEN_NAME_ENTRY.name[0] == 0) {
            initDialog(dialog, "A name has not been entered!", 0);
            runDialog(dialog);
            OPEN_NAME_ENTRY.state = 0;
        } else {
            initDialog(dialog, OPEN_STR_IS_THIS_NAME_OK, 1);
            runDialog(dialog);
            switch ((s8)dialog[0xA5]) {
            case 1:
                break;
            case 0:
            case 2:
                OPEN_NAME_ENTRY.state = 0;
                break;
            }
        }
#if VERSION_US
        if (OPEN_NAME_ENTRY.state != 0) {
            break;
        }
    }
#elif VERSION_EU
    } while (OPEN_NAME_ENTRY.state == 0);
#endif
    if (OPEN_NAME_ENTRY.state == 7) {
        strcpy(name, OPEN_NAME_ENTRY.name);
    }
    playMenuSound(4);
    animateWindowTo(&OPEN_NAME_HELP_WINDOW, (Rect16 *)-1);
    animateWindowTo(&OPEN_NAME_ENTRY_WINDOW, (Rect16 *)-1);
    animateWindowTo(&OPEN_NAME_WINDOW, (Rect16 *)-1);
    waitFrames(20);
    removeFrameCallback((s32)OPEN_drawNameEntry);
    resumeTask(parentTask);
}

/* the last three bytes are leftovers in the original, not zero padding, and
   not the same in every version */
#if VERSION_US
const char OPEN_STR_IS_THIS_NAME_OK[20] = "Is this name OK?\0\x18\x62\0";
#elif VERSION_EU
const char OPEN_STR_IS_THIS_NAME_OK[20] = "Is this name OK?\0\x67\x32\x16";
#else
#error "openseg/registration/open_name_entry: version not checked"
#endif
