#include "common.h"
#include "game.h"
#include "dcb/evo_text.h"
#include "dcb/text.h"

void EVO_clearTextLines(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        slot->active = 0;
        slot++;
    }
}

EvoText *EVO_allocTextLine(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (slot->active == 0) {
            bzero((Scene3D *)slot, 0x3C);
            slot->active = 1;
            if (i == 0) {
                slot->pos = -1;
            } else {
                slot->pos = 0;
            }
            return slot;
        }
        slot++;
    }
    return NULL;
}

s32 EVO_addTextLine(u8 *src) {
    u8 *playerName;
    u8 *cardName;
    EvoText *t;
    u8 *dst;
    s32 i;
    s8 end;

    playerName = (u8 *)PLAYER_PROFILES;
    cardName = EVO_CARDS_BY_ID[EVO_FUSION.result]->name;
    t = EVO_allocTextLine(EVO_TEXT_LINES);
    if (t == NULL) {
        /* Shift-JIS: "the text has run out of lines" */
        printf("\x83" "e\x83L\x83X\x83g\x82\xCC\x8Ds\x90\x94\x82\xAA\x82\xA2\x82\xC1\x82\xCF\x82\xA2\x82\xC9\x82\xC8\x82\xE8\x82\xDC\x82\xB5\x82\xBD\n");
        return -1;
    }
    t->text[0] = '*';
    t->text[1] = 'w';
    t->text[2] = '1';
    dst = t->text + 3;
    while (*src != 0) {
        if (*src < 0x81 || *src > 0x98) {
            if (*src == '*') {
                if (src[1] == 'h') {
                    if (src[2] == '0') {
                        src += 3;
                        for (i = 0; i < 12; i++) {
                            if (*playerName == 0) {
                                break;
                            }
                            *dst++ = *playerName++;
                        }
                        continue;
                    } else if (src[2] == '1') {
                        src += 3;
                        for (i = 0; i < 21; i++) {
                            if (*cardName == 0) {
                                break;
                            }
                            *dst++ = *cardName++;
                        }
                        continue;
                    } else if (src[2] == '2' || src[2] == '3') {
                        src += 3;
                        for (i = 0; i < 21; i++) {
                            if (*cardName == 0) {
                                break;
                            }
                            if (i <= 0) {
                                *dst++ = *cardName++;
                            } else {
                                *dst++ = '?';
                                cardName++;
                                i++;
                            }
                        }
                        continue;
                    }
                }
            } else if (*src == '\\') {
                end = 0;
                for (i = 1; i < 5; i++) {
                    if (end == 0 && src[i] == 0) {
                        end = 1;
                        break;
                    }
                }
                if (end != 1 && src[1] == '0' && src[2] == 'x' && src[3] == '2' && src[4] == '2') {
                    src += 5;
                    *dst++ = '"';
                    continue;
                }
            }
        } else {
            *dst++ = *src++;
            *dst++ = *src++;
            continue;
        }
        *dst++ = *src++;
    }
    *dst = 0;
    dst = t->text;
    for (i = 0; i < 60 && *dst != 0; i++) {
        dst++;
    }
    if (t->pos == -1) {
        t->pos = i;
    }
    if (i >= 60) {
        t->active = 0;
    } else {
        t->active = 1;
    }
    t->len = i;
    for (i = 0; i < 4 && t != EVO_TEXT_LINES; i++) {
        t--;
    }
    return i;
}

const char D_801DF3B0[] = "";

s32 EVO_typeTextLine(s32 x, s32 y, EvoText *t, s32 z) {
    u8 buf[64];
    u8 *dst;
    u8 *src;
    s8 i;
    s32 c;

    /* the same pointers; the match depends on their order: each version's
       compiler needs its own to give them the original's registers */
#if VERSION_US
    dst = buf;
#elif VERSION_EU
    src = t->text;
#else
#error "evoseg/fusion/evo_text: version not checked"
#endif
    if (t->len == t->pos) {
        drawText(x, y, (s32)t, 7, z);
        return -1;
    }
#if VERSION_US
    src = t->text;
#elif VERSION_EU
    dst = buf;
#else
#error "evoseg/fusion/evo_text: version not checked"
#endif
    for (i = 0; i < t->pos; i++) {
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
                    t->pos += 2;
                }
                break;
            }
        }
        *dst = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 1;
    } else {
        *dst++ = src[0];
        *dst = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 2;
    }
    return 1;
}

s32 EVO_typeTextLines(s16 x, s16 y, s32 z) {
    EvoText *t;
    s32 i;

    for (t = EVO_TEXT_LINES, i = 0; i < 4; i++, t++) {
        if (t->active != 0 && EVO_typeTextLine(x, y + i * 12, t, z) == 1) {
            return 1;
        }
    }
    return 0;
}

void EVO_drawMessageWindow(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;
    u8 unused[0x48]; /* unused, but it is in the original stack frame */

    EVO_FUSION.textTyping = EVO_typeTextLines(x, y, z);
    if (EVO_FUSION.textTyping == 0 && ((u8)EVO_FUSION.scriptState == 2 || (u8)EVO_FUSION.scriptState == 3)) {
        if (++EVO_FUSION.blinkTimer & 0x10) {
            drawIcon(x + 200, y + 0x25, 0, 0x1B, z);
        }
    } else {
        EVO_FUSION.blinkTimer = 0;
    }
}
