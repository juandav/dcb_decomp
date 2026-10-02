#include "common.h"
#include "game.h"
#include "dcb/sai_text.h"
#include "dcb/text.h"
#include "dcb/sai_data.h"

extern s16 SAI_PLAYER_DATA_ROW;
extern u8 SAI_NEXT_ICON_BLINK;
extern u8 SAI_TEXT_TYPING;

s32 SAI_typeTextLines(s16 x, s16 y, s32 z);

void SAI_drawMessageWindow(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 found;
    char buf[0x48]; /* unused, but it is in the original stack frame */

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (SAI_PLAYER_STATS_STATE == 2) {
        drawText(x + 2, y, (s32)SAI_PLAYER_DATA_HELP[SAI_PLAYER_DATA_ROW], 7, z);
        return;
    }
    if (SAI_AREA.waitingForCross != 0 && SAI_AREA.typing == 0) {
        SAI_AREA.nextBlink++;
        if (SAI_AREA.nextBlink & 0x10) {
            drawIcon(x + 0x11C, y + 0x1A, 0, 0x1B, z);
        }
    } else {
        SAI_NEXT_ICON_BLINK = 0;
    }
    found = SAI_typeTextLines(x, y, z);
    SAI_TEXT_TYPING = found;
}

void SAI_clearTextLines(TextLine *lines) {
    s32 i;

    for (i = 0; i < 3; i++, lines++) {
        lines->active = -1;
        bzero(lines, 0x3C);
    }
}

TextLine *SAI_allocTextLine(TextLine *line) {
    s32 i;

    for (i = 0; i < 3; i++, line++) {
        if (line->active == -1) {
            line->active = 0;
            line->shown = 0;
            return line;
        }
    }
    return NULL;
}

/* Copies a script text into a free line, expanding "*h0" to the player's name
   and "\0x22" to a quote; returns the line's index or -1 when all are used. */
s32 SAI_addTextLine(u8 *src) {
    u8 *name = (u8 *)((PlayerProfile *)PLAYER_PROFILES)->name;
    TextLine *slot = SAI_allocTextLine(SAI_TEXT_LINES);
    u8 *dst;
    s32 i;
    s8 found;

    if (slot == NULL) {
        return -1;
    }
    slot->text[0] = '*';
    slot->text[1] = 'w';
    slot->text[2] = '2';
    dst = &slot->text[3];
    while (*src != 0) {
        if (*src < 0x81 || *src >= 0x99) {
            if (*src == '*') {
                if (src[1] == 'h' && src[2] == '0') {
                    src += 3;
                    for (i = 0; i < 12; i++) {
                        if (*name == 0) {
                            break;
                        }
                        *dst++ = *name++;
                    }
                    continue;
                }
            } else if (*src == '\\') {
                found = 0;
                for (i = 1; i < 5; i++) {
                    if (!found && src[i] == 0) {
                        found = 1;
                        break;
                    }
                }
                if (found != 1 && src[1] == '0' && src[2] == 'x' && src[3] == '2' && src[4] == '2') {
                    src += 5;
                    *dst++ = '"';
                    continue;
                }
            }
        } else {
            *dst++ = *src++;
        }
        *dst++ = *src++;
    }
    *dst = 0;
    for (dst = slot->text, i = 0; i < 60 && *dst != 0; i++, dst++) {
    }
    if (i < 60) {
        slot->active = 0;
    }
    slot->length = i;
    for (i = 0; i < 3 && slot != SAI_TEXT_LINES; i++, slot--) {
    }
    return i;
}

s32 SAI_typeTextLine(s32 x, s32 y, TextLine *line, s32 z) {
    u8 buf[0x40];
    u8 *dst;
    u8 *src;
    s8 i;
    s32 c;

    /* the same pointers; the match depends on their order: each version's
       compiler needs its own to give them the original's registers */
#if VERSION_US
    dst = buf;
#elif VERSION_EU
    src = line->text;
#else
#error "saiseg/ui/sai_text: version not checked"
#endif
    if (line->length == line->shown) {
        drawText(x, y, (s32)line, 7, z);
        return -1;
    }
#if VERSION_US
    src = line->text;
#elif VERSION_EU
    dst = buf;
#else
#error "saiseg/ui/sai_text: version not checked"
#endif
    for (i = 0; i < line->shown; i++) {
        *dst++ = *src++;
    }
    c = *src;
    if (c == 0) {
        return 1;
    }
    if (*src < 0x81 || *src > 0x98) {
        if (c == '*') {
            switch (src[1]) {
            case 'a':
            case 'b':
            case 'c':
            case 'e':
            case 's':
            case 'w':
                if (src[2] >= '0' && src[2] <= '9') {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    line->shown += 2;
                }
                break;
            }
        }
        dst[0] = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        line->shown += 1;
    } else {
        *dst++ = src[0];
        dst[0] = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        line->shown += 2;
    }
    return 1;
}

s32 SAI_typeTextLines(s16 x, s16 y, s32 z) {
    TextLine *entry = SAI_TEXT_LINES;
    s32 found = 0;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->active != -1) {
            if (SAI_typeTextLine(x + 4, y + i * 13, entry, z) == 1) {
                found = 1;
                break;
            }
        }
    }
    return found;
}

void SAI_finishTextLines(void) {
    TextLine *entry = SAI_TEXT_LINES;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->active != -1) {
            entry->shown = entry->length;
        }
    }
}

void SAI_releaseTextLines(TextLine *lines) {
    s32 i;

    for (i = 0; i < 3; i++, lines++) {
        lines->active = -1;
    }
}
