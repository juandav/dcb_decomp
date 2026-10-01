#include "common.h"
#include "game.h"
#include "dcb/sub_name_entry.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"
#include "dcb/frame_callback.h"
#include "dcb/dialog.h"
#include "dcb/subseg.h"
#include "dcb/sub_deck_screens.h"
#include "dcb/sub_auto_deck.h"

extern NameEntry SUB_NAME_ENTRY;
extern CursorHighlight SUB_DECK_NAME_CURSOR;
extern UiWindow SUB_NAME_ENTRY_WINDOW;
extern UiWindow SUB_DECK_NAME_WINDOW;
extern UiWindow SUB_NAME_ENTRY_HELP_WINDOW;
extern CursorHighlight SUB_NAME_ENTRY_CURSOR;
extern u8 SUB_NAME_ENTRY_CHARS[];

/*
 * Strings several functions share: each one is defined further down, where the
 * original put it in .rodata, instead of as a literal at its first use.
 */
extern const char SUB_STR_NO_DECK_NAME[];

/* not referenced by any code */
const s32 D_801DDF38 = 7;

s32 SUB_moveNameEntryCursor(void) {
    Rect16 rect;
    UiWindow *window = &SUB_NAME_ENTRY_WINDOW;
    s16 r;
    if (SUB_NAME_ENTRY.active != 0) {
        if (SUB_NAME_ENTRY.onButtons == 0) {
            if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0xF000) {
                playMenuSound(2);
            }
            do {
                if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x1000) {
                    SUB_NAME_ENTRY.row--;
                    SUB_NAME_ENTRY.row = (SUB_NAME_ENTRY.row + 1) / 9 * 9 + (SUB_NAME_ENTRY.row + 9) % 9;
                    r = SUB_NAME_ENTRY.row % 9;
                    if (r == 0) {
                        PAD_STATES[SUB_NAME_ENTRY.pad]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x4000) {
                    SUB_NAME_ENTRY.row++;
                    SUB_NAME_ENTRY.row = (SUB_NAME_ENTRY.row - 1) / 9 * 9 + (SUB_NAME_ENTRY.row + 9) % 9;
                    r = SUB_NAME_ENTRY.row % 9;
                    if (r == 8) {
                        PAD_STATES[SUB_NAME_ENTRY.pad]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x8000) {
                    if (--SUB_NAME_ENTRY.col < 0) {
                        SUB_NAME_ENTRY.col = 9;
                        SUB_NAME_ENTRY.onButtons = 1;
                        SUB_NAME_ENTRY.button = 7;
                    } else if (SUB_NAME_ENTRY.col == 0) {
                        PAD_STATES[SUB_NAME_ENTRY.pad]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x2000) {
                    if (++SUB_NAME_ENTRY.col >= 10) {
                        SUB_NAME_ENTRY.col = 0;
                        SUB_NAME_ENTRY.onButtons = 1;
                        SUB_NAME_ENTRY.button = 7;
                    } else if (SUB_NAME_ENTRY.col == 9) {
                        PAD_STATES[SUB_NAME_ENTRY.pad]->repeatEnabled = 0;
                    }
                }
            } while (SUB_NAME_ENTRY_CHARS[SUB_NAME_ENTRY.row * 10 + SUB_NAME_ENTRY.col] == ' ');
        } else {
            if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0xF000) {
                playMenuSound(2);
            }
            if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->pressed & 0xA000) {
                playMenuSound(2);
            }
            if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x1000) {
                if (--SUB_NAME_ENTRY.button < 7) {
                    SUB_NAME_ENTRY.button = 8;
                } else if (SUB_NAME_ENTRY.button == 7) {
                    PAD_STATES[SUB_NAME_ENTRY.pad]->repeatEnabled = 0;
                }
            } else if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x4000) {
                if (++SUB_NAME_ENTRY.button >= 9) {
                    SUB_NAME_ENTRY.button = 7;
                } else if (SUB_NAME_ENTRY.button == 8) {
                    PAD_STATES[SUB_NAME_ENTRY.pad]->repeatEnabled = 0;
                }
            } else if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->pressed & 0x8000) {
                SUB_NAME_ENTRY.onButtons = 0;
                SUB_NAME_ENTRY.prevButton = -1;
                SUB_NAME_ENTRY.prevCol = -1;
                SUB_NAME_ENTRY.prevRow = -1;
                SUB_NAME_ENTRY.col = 9;
                while (SUB_NAME_ENTRY_CHARS[SUB_NAME_ENTRY.row * 10 + SUB_NAME_ENTRY.col] == ' ') {
                    SUB_NAME_ENTRY.col--;
                }
            } else if ((u16)PAD_STATES[SUB_NAME_ENTRY.pad]->pressed & 0x2000) {
                SUB_NAME_ENTRY.onButtons = 0;
                SUB_NAME_ENTRY.prevButton = -1;
                SUB_NAME_ENTRY.prevCol = -1;
                SUB_NAME_ENTRY.prevRow = -1;
                SUB_NAME_ENTRY.col = 0;
                while (SUB_NAME_ENTRY_CHARS[SUB_NAME_ENTRY.row * 10 + SUB_NAME_ENTRY.col] == ' ') {
                    SUB_NAME_ENTRY.col++;
                }
            }
        }
    }
    if (SUB_NAME_ENTRY.onButtons == 0) {
        if (SUB_NAME_ENTRY.row != SUB_NAME_ENTRY.prevRow || SUB_NAME_ENTRY.col != SUB_NAME_ENTRY.prevCol) {
            SUB_NAME_ENTRY.prevCol = SUB_NAME_ENTRY.col;
            SUB_NAME_ENTRY.prevRow = SUB_NAME_ENTRY.row;
            rect.x = window->rect.x - window->scroll[2] + SUB_NAME_ENTRY.prevCol * 17 + 4;
            rect.y = window->rect.y - window->scroll[3] + SUB_NAME_ENTRY.prevRow * 14 + 1;
            rect.w = 12;
            rect.h = 12;
            if (SUB_NAME_ENTRY.prevCol >= 5) {
                rect.x += 11;
            }
            moveCursorHighlight(&SUB_NAME_ENTRY_CURSOR, &rect);
        }
    } else if (SUB_NAME_ENTRY.button != SUB_NAME_ENTRY.prevButton) {
        SUB_NAME_ENTRY.prevButton = SUB_NAME_ENTRY.button;
        switch (SUB_NAME_ENTRY.button) {
        case 0:
        case 1:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + SUB_NAME_ENTRY.button * 14 + 1;
            rect.w = 0x30;
            rect.h = 12;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + SUB_NAME_ENTRY.button * 14 + 1;
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
        moveCursorHighlight(&SUB_NAME_ENTRY_CURSOR, &rect);
    }
}

void SUB_drawNameEntryGrid(UiWindow *window) {
    /* not literals: GCC would share "Cancel" with SUB_drawAutoDeckOptions, which has its own copy */
    static const char charFormat[] = "%c";
    static const char okLabel[] = "OK";
    static const char cancelLabel[] = "Cancel";
    char buf[64];
    u8 rowPalettes[8];
    s32 x = window->originX + 4;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 i;
    s32 dx;
    s32 button;

    for (i = 0; i < 90; i++) {
        sprintf(buf, charFormat, SUB_NAME_ENTRY_CHARS[i]);
        dx = 0;
        if (i % 10 >= 5) {
            dx = 11;
        }
        drawText(x + 4 + (i % 10) * 17 + dx, y + (i / 10) * 14, (s32)buf, 7, z);
    }
    for (i = 0; i < 7; i++) {
        rowPalettes[i] = 4;
    }
    rowPalettes[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xCC, window->rect.y + 0x63, (s32)okLabel, 6, z);
    drawText(window->rect.x + 0xCC, window->rect.y + 0x71, (s32)cancelLabel, 6, z);
    SUB_moveNameEntryCursor();
    if (SUB_NAME_ENTRY.onButtons == 0) {
        if (PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x40) {
            SUB_NAME_ENTRY.name[SUB_NAME_ENTRY.cursor] = SUB_NAME_ENTRY_CHARS[SUB_NAME_ENTRY.row * 10 + SUB_NAME_ENTRY.col];
            playMenuSound(1);
            if (SUB_NAME_ENTRY.cursor == 0 && SUB_NAME_ENTRY.fresh == 1) {
                for (i = 1; i < 13; i++) {
                    SUB_NAME_ENTRY.name[i] = 0;
                }
            }
            if (SUB_NAME_ENTRY.cursor < 11) {
                SUB_NAME_ENTRY.cursor++;
            } else {
                SUB_NAME_ENTRY.onButtons = 1;
                SUB_NAME_ENTRY.button = 7;
            }
            SUB_NAME_ENTRY.fresh = 0;
        } else if (PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x20) {
            for (i = 11; i >= SUB_NAME_ENTRY.cursor + 1; i--) {
                SUB_NAME_ENTRY.name[i] = SUB_NAME_ENTRY.name[i - 1];
            }
            SUB_NAME_ENTRY.name[SUB_NAME_ENTRY.cursor] = SUB_NAME_ENTRY_CHARS[SUB_NAME_ENTRY.row * 10 + SUB_NAME_ENTRY.col];
            playMenuSound(1);
            if (SUB_NAME_ENTRY.cursor < 11) {
                SUB_NAME_ENTRY.cursor++;
            } else {
                SUB_NAME_ENTRY.onButtons = 1;
                SUB_NAME_ENTRY.button = 7;
            }
            SUB_NAME_ENTRY.fresh = 0;
        } else if (PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x10) {
            if (SUB_NAME_ENTRY.name[0] != 0) {
                playMenuSound(1);
            }
            if (SUB_NAME_ENTRY.cursor == 0) {
                SUB_NAME_ENTRY.cursor++;
            }
            for (i = SUB_NAME_ENTRY.cursor; i < 13; i++) {
                SUB_NAME_ENTRY.name[i - 1] = SUB_NAME_ENTRY.name[i];
            }
            SUB_NAME_ENTRY.cursor--;
            SUB_NAME_ENTRY.fresh = 0;
        }
    } else if (PAD_STATES[SUB_NAME_ENTRY.pad]->pressed & 0x40) {
        playMenuSound(1);
        button = SUB_NAME_ENTRY.button;
        if (button >= 0) {
            if (button < 7) {
                SUB_NAME_ENTRY.row = button * 9;
                SUB_NAME_ENTRY.col = 0;
                scrollWindowTo((s16 *)window, 0, SUB_NAME_ENTRY.row * 14);
            } else if (button < 9) {
                SUB_NAME_ENTRY.state = SUB_NAME_ENTRY.button;
            }
        }
    } else if (PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 0x10) {
        if (SUB_NAME_ENTRY.name[0] != 0) {
            playMenuSound(1);
        }
        if (SUB_NAME_ENTRY.cursor == 0) {
            SUB_NAME_ENTRY.cursor++;
        }
        for (i = SUB_NAME_ENTRY.cursor; i < 13; i++) {
            SUB_NAME_ENTRY.name[i - 1] = SUB_NAME_ENTRY.name[i];
        }
        SUB_NAME_ENTRY.cursor--;
        SUB_NAME_ENTRY.fresh = 0;
    }
    if ((PAD_STATES[SUB_NAME_ENTRY.pad]->pressed & 0x800) && !(SUB_NAME_ENTRY.onButtons == 1 && SUB_NAME_ENTRY.button == 7)) {
        playMenuSound(1);
        SUB_NAME_ENTRY.onButtons = 1;
        SUB_NAME_ENTRY.button = 7;
    }
    drawCursorHighlight(&SUB_NAME_ENTRY_CURSOR, z);
}

void SUB_drawDeckNameField(UiWindow *window) {
    Rect16 rect;
    char buf[248];
    s32 x = window->originX + 1;
    s32 y = window->originY;
    s32 z = window->z;

    sprintf(buf, "*s0%s", SUB_NAME_ENTRY.name);
    drawText(x, y, (s32)buf, 7, z);
    drawText(x + 0x4C, y, (s32)"Deck", 6, z);
    if (PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 4) {
        if (SUB_NAME_ENTRY.cursor != 0) {
            playMenuSound(2);
            SUB_NAME_ENTRY.cursor--;
        }
    } else if (PAD_STATES[SUB_NAME_ENTRY.pad]->repeat & 8) {
        if (SUB_NAME_ENTRY.cursor != 11 && SUB_NAME_ENTRY.name[SUB_NAME_ENTRY.cursor] != 0) {
            SUB_NAME_ENTRY.cursor++;
            playMenuSound(2);
        }
    }
    rect.x = x + SUB_NAME_ENTRY.cursor * 6;
    rect.y = y + 13;
    rect.w = 6;
    rect.h = 0;
    moveCursorHighlight(&SUB_DECK_NAME_CURSOR, &rect);
    drawCursorHighlight(&SUB_DECK_NAME_CURSOR, z);
}

void SUB_drawNameEntryHelp(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;

    drawText(x, y, (s32)"*b0 Insert", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b1 Delete", 7, z);
}

void SUB_drawNameEntry(void) {
    drawWindow(&SUB_NAME_ENTRY_WINDOW, SUB_drawNameEntryGrid, 1);
    drawWindow(&SUB_DECK_NAME_WINDOW, SUB_drawDeckNameField, 1);
    drawWindow(&SUB_NAME_ENTRY_HELP_WINDOW, SUB_drawNameEntryHelp, 1);
}

void SUB_enterDeckName(s32 mode, char *name, s32 pad) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];

    bzero((Scene3D *)SUB_NAME_ENTRY.name, 13);
    strcpy(SUB_NAME_ENTRY.name, name);
    SUB_NAME_ENTRY.col = 0;
    SUB_NAME_ENTRY.row = 0;
    SUB_NAME_ENTRY.prevCol = -1;
    SUB_NAME_ENTRY.prevRow = -1;
    SUB_NAME_ENTRY.active = 1;
    SUB_NAME_ENTRY.cursor = 0;
    SUB_NAME_ENTRY.onButtons = 0;
    SUB_NAME_ENTRY.button = 0;
    SUB_NAME_ENTRY.prevButton = -1;
    SUB_NAME_ENTRY.state = 0;
    SUB_NAME_ENTRY.pad = pad;
    SUB_NAME_ENTRY.fresh = 1;
    rect.x = 0x18;
    rect.y = 0x5E;
    rect.w = 0x110;
    rect.h = 0x7E;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0x7E;
    openWindow(&SUB_NAME_ENTRY_WINDOW, &rect, -1, (s16 *)&view, 10, 0x26, 0x80, 12);
    SUB_NAME_ENTRY_WINDOW.label = (s32)"NAME ENTRY";
    cursor.x = SUB_NAME_ENTRY_WINDOW.originX + 4;
    cursor.y = SUB_NAME_ENTRY_WINDOW.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&SUB_NAME_ENTRY_CURSOR, &cursor, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x34;
    rect.h = 0x28;
    openWindow(&SUB_NAME_ENTRY_HELP_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SUB_NAME_ENTRY_HELP_WINDOW.label = (s32)"HELP";
    rect.x = 0xBC;
    rect.y = 0x1C;
    rect.w = 0x6C;
    rect.h = 0xE;
    openWindow(&SUB_DECK_NAME_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SUB_DECK_NAME_WINDOW.label = (s32)"DECK NAME";
    cursor.x = SUB_DECK_NAME_WINDOW.originX;
    cursor.y = SUB_DECK_NAME_WINDOW.originY + 13;
    cursor.w = 12;
    cursor.h = 0;
    initCursorHighlight(&SUB_DECK_NAME_CURSOR, &rect, (Bytes4 *)-1);
    playMenuSound(3);
    addFrameCallback((s32)SUB_drawNameEntry);
    for (;;) {
        waitFrames(FRAME_INTERVAL);
        if (SUB_NAME_ENTRY.state == 0) {
            continue;
        }
        if (SUB_NAME_ENTRY.state == 7 && SUB_NAME_ENTRY.name[0] == 0) {
            initDialog(dialog, (u8 *)SUB_STR_NO_DECK_NAME, 0);
            dialog[0xA6] = pad;
            runDialog(dialog);
            SUB_NAME_ENTRY.state = 0;
            continue;
        }
        break;
    }
    if (SUB_NAME_ENTRY.state == 7) {
        strcpy(name, SUB_NAME_ENTRY.name);
    }
    playMenuSound(4);
    animateWindowTo(&SUB_NAME_ENTRY_HELP_WINDOW, (Rect16 *)-1);
    animateWindowTo(&SUB_NAME_ENTRY_WINDOW, (Rect16 *)-1);
    animateWindowTo(&SUB_DECK_NAME_WINDOW, (Rect16 *)-1);
    waitFrames(20);
    removeFrameCallback((s32)SUB_drawNameEntry);
    if (SUB_NAME_ENTRY.state == 7) {
        spawnTask(0, -1, 0, 0x1000, mode == 1 ? SUB_runDeckMenu : SUB_runAutoDeckMenu, 0, 0, 0, 0);
    } else {
        spawnTask(0, -1, 0, 0x1000, SUB_runDeckMenu, 0, 0, 0, 0);
    }
}

/* the last two bytes are leftovers in the original, not zero padding, and
   not the same in every version */
#if VERSION_US
const char SUB_STR_NO_DECK_NAME[36] = "A Deck Name has not been entered!\0\x0E\0";
#elif VERSION_EU
const char SUB_STR_NO_DECK_NAME[36] = "A Deck Name has not been entered!\0\0\x03";
#else
#error "subseg/deck/sub_name_entry: version not checked"
#endif

/* the characters of the name entry grid, ten to a row */
u8 SUB_NAME_ENTRY_CHARS[] =
    "ABCDEabcde"
    "FGHIJfghij"
    "KLMNOklmno"
    "PQRSTpqrst"
    "UVWXYuvwxy"
    "Z-   z    "
    "          "
    "          "
    "0123456789"
;

/* not referenced by any code, and only us has it (eu's is zero padding) */
#if VERSION_US
u8 D_801F1933 = 16;
#elif VERSION_EU
#else
#error "subseg/deck/sub_name_entry: version not checked"
#endif
