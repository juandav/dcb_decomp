#include "common.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/prim.h"
#include "dcb/text.h"
#include "dcb/task.h"
#include "dcb/saiseg.h"

/* jp's message window and deck information screen (sai_text.c, sai_flags.c
   and sai_reward.c are us's and eu's): one object, as the jump table of
   SAI_summarizeDeck is 8-aligned only from the message window's rodata. The message window holds up to four lines,
   rendered into VRAM as they are added and typed out a glyph at a time */

void func_8006689C(char *, char *, s32);
u8 *formatSjisNumber(s32 value, s32 width, u8 *dst);
s32 uploadKanjiString(u8 *text, Rect16 *rect);

MsgLine *SAI_allocTextLine(SaiUi *ui);
s32 SAI_isLetter(s8 c);

/* the color the message window's glyphs are drawn in */
const Bytes4 SAI_TEXT_RGB = { { 0xA4, 0xA4, 0xA4 } };

void SAI_drawMessageWindow(JpWindow *win) {
    Bytes4 rgb = SAI_TEXT_RGB;
    Rect16 uv;
    char buf[0x48];
    MsgLine *line;
    s32 y;
    u32 i;
    u32 j;
    s32 n;
    u32 count;
    s32 x;

    line = SAI_UI.lines;
    SAI_UI.typing = 0;
    y = 4;
    if (SAI_STATE->flags & 0x10) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[win->z], DB(FRAME_BUFFER_INDEX).primSlots[0] + 0x60);
    }
    for (i = 0; i < 4; i++) {
        if (SAI_TYPED_TEXT.lines[i].active != 0 && SAI_UI.typing == 0) {
            SAI_TYPED_TEXT.lines[i].shown += 400;
            n = SAI_TYPED_TEXT.lines[i].shown / 100 * 2;
            if (n >= SAI_TYPED_TEXT.lines[i].length) {
                n = SAI_TYPED_TEXT.lines[i].length;
            } else {
                SAI_UI.typing = 1;
            }
            func_8006689C(buf, SAI_TYPED_TEXT.lines[i].text, n);
            buf[n] = 0;
            drawIconText(0x26, y + 0x99, 7, 1, win->z, (s32)buf);
        }
        y += 0xE;
    }
    for (i = 0; i < 4; i++, line++) {
        if (line->active == 1) {
            line->shown += 0x28;
            count = line->shown / line->width;
            if (count >= line->width) {
                count = line->width;
            } else {
                SAI_UI.typing = 1;
            }
            for (j = 0; j < count; j++) {
                x = j * 12;
                uv.x = x;
                uv.y = line->vramY;
                uv.w = 0xC;
                uv.h = 0xB;
                drawPageSpriteColored(x + 0x26, line->vramY + 0x19, &uv, rgb.b, GetTPage(0, 0, 0x3C0, 0), line->palettes[j], win->z);
            }
        }
        if (SAI_UI.typing == 1) {
            break;
        }
    }
}

/* c6(%s)c7(Ｂｉｔを手に入れた！) */
const char SAI_FMT_GOT_BITS[] = "c6(%s)c7(Ｂｉｔを手に入れた！)";

void SAI_addTextLine(s8 withBits) {
    Rect16 rect;
    char buf[0x40];
    u8 digits[0x10];
    u8 *src;
    u8 *eventName;
    u8 *playerName;
    MsgLine *line;
    u8 *dst;
    s8 *palettes;
    s8 palette;
    s32 n;
    u32 i;

    src = (u8 *)SAI_STATE->regs[16];
    eventName = (u8 *)SAI_STATE->regs[6];
    /* fake match: the empty loop ends a CSE block, so SAI_STATE is loaded
       again for the Bits instead of reusing the register, as in the original */
    do {
    } while (0);
    if (withBits) {
        sprintf(buf, SAI_FMT_GOT_BITS, formatSjisNumber(SAI_STATE->bits, 6, digits));
        src = buf;
    }
    line = SAI_allocTextLine(&SAI_UI);
    palettes = line->palettes;
    if (line != NULL) {
        palette = 7;
        playerName = (u8 *)PLAYER_DATA(0).name;
        dst = line->text;
        n = 0;
        while (*src != 0) {
            if (*src < 0x81 || *src >= 0x99) {
                switch (*src) {
                case 'c':
                    src++;
                    if (SAI_isLetter(*src)) {
                        if (*src == 'n') {
                            src++;
                            for (i = 0; i < 12; i++) {
                                if (*playerName == 0) {
                                    break;
                                }
                                *dst++ = *playerName++;
                                *palettes = palette;
                                if (n & 1) {
                                    palettes++;
                                }
                                n++;
                            }
                        } else if (*src == 'e') {
                            src++;
                            for (i = 0; i < 10; i++) {
                                if (*eventName == 0) {
                                    break;
                                }
                                *dst++ = *eventName++;
                                *palettes = palette;
                                if (n & 1) {
                                    palettes++;
                                }
                                n++;
                            }
                        }
                    } else {
                        palette = *src - '0';
                        src++;
                    }
                    break;
                case '(':
                    src++;
                    break;
                case ')':
                    src++;
                    palette = 7;
                    break;
                default:
                    src++;
                    break;
                }
            } else {
                *dst++ = *src++;
                *palettes = palette;
                if (n & 1) {
                    palettes++;
                }
                n++;
                *dst++ = *src++;
                *palettes = palette;
                if (n & 1) {
                    palettes++;
                }
                n++;
            }
        }
        *dst = 0;
        rect.x = 0x3C0;
        rect.y = line->vramY;
        rect.w = 0;
        rect.h = 0;
        line->width = uploadKanjiString(line->text, &rect);
    }
}

MsgLine *SAI_allocTextLine(SaiUi *ui) {
    MsgLine *line;
    s8 i;

    line = ui->lines;
    for (i = 0; i < 4; i++, line++) {
        if (line->active == 0) {
            line->active = 1;
            line->vramY = i * 14 + 0x84;
            line->shown = 0;
            return line;
        }
    }
    return NULL;
}

s32 SAI_isLetter(s8 c) {
    return c >= 'A' && c <= 'z';
}

void SAI_clearTextLines(void) {
    Rect16 rect = { 0x3C0, 0x80, 0x40, 0x80 };
    s8 i;

    for (i = 0; i < 4; i++) {
        SAI_UI.lines[i].active = 0;
    }
    ClearImage(&rect, 0, 0, 0);
}

/* the deck screen's menu and the opponent's deck */

s32 runChoiceMenu(JpMenu *menu);

void SAI_clearTextVram(void) {
    Rect16 rect = { 0x3C0, 0, 0x40, 0x100 };

    ClearImage(&rect, 0, 0, 0);
}

void SAI_runMenu(void) {
    JpMenu *menu = &SAI_UI.menu;

    SAI_STATE->regs[15] = 0;
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(menu)) {
            SAI_STATE->regs[15] = menu->selected + 1;
            exitTask();
        }
    }
}

/* the copies of a card the player has spare: those owned, less the most any
   saved deck holds when inDecks is 1 */
s32 SAI_countSpareCopies(s8 inDecks, s16 card) {
    s32 copies;
    s8 type;
    s16 id;
    s32 deck;
    s32 i;
    s32 count;
    s32 most;

    most = 0;
    if (card < 0x6E) {
        id = card;
        type = 0;
        copies = PLAYER_DATA(0).cardCollection[id] & 0xF;
    } else if (card < 0x56) {
        id = card - 0x6E;
        type = 1;
        copies = PLAYER_DATA(0).optionCollection[id] & 0xF;
    } else {
        id = card - 0x99;
        type = 2;
        copies = PLAYER_DATA(0).digivolveCollection[id] & 0xF;
    }
    if (inDecks == 1) {
        for (deck = 0; deck < 3; deck++) {
            if (PLAYER_DATA(0).savedDecks[deck].inUse != 0) {
                for (i = 0, count = 0; i < 30; i++) {
                    if (PLAYER_DATA(0).savedDecks[deck].cards[i].index == id && PLAYER_DATA(0).savedDecks[deck].cards[i].type == type) {
                        count++;
                    }
                }
                if (count >= most) {
                    most = count;
                }
            }
        }
        copies -= most;
    }
    return copies;
}

void SAI_doNothing(void) {
}

void SAI_resolveOpponentDeck(void) {
    PlayerDeck *deck;
    s32 i;
    s32 j;

    deck = &((SessionData *)SESSION_DATA)->opponentDeck;
    for (i = 0; i < 30; i++) {
        switch (deck->cards[i].type) {
        case 0:
            deck->cards[i].card = (s8 *)(DIGIMON_CARDS + deck->cards[i].index * 0x122);
            break;
        case 1:
            deck->cards[i].card = (s8 *)(OPTION_CARDS + deck->cards[i].index * 0xD4);
            break;
        case 2:
            deck->cards[i].card = (s8 *)(DIGIVOLVE_CARDS + deck->cards[i].index * 0x62);
            break;
        }
    }
    deck->wins = 0;
    deck->losses = 0;
    for (j = 0; j < 3; j++) {
        deck->attackCounts[j] = 0;
    }
}

/* the deck information screen */

extern JpGame *SCROLLING_BACKGROUND;
extern void runWindowTask();

void openChoiceMenu(JpMenu *, s32, s32, s32, s32);
void addChoiceMenuItem(JpMenu *, s32, void (*)(void));
void SAI_runMenu(void);
void SAI_summarizeDeck(DeckSummary *summary, s8 deck);
void SAI_summarizeOwnedCards(DeckSummary *owned);

u8 *formatSjisNumber(s32 value, s32 width, u8 *dst);

void SAI_drawDeckInfo(JpWindow *win) {
    Rect16 uv = { 0, 0xE8, 0x10, 0x10 };
    char line[0x20];
    u8 count[0x10];
    u8 total[0x18];
    s32 tpage;
    s32 deck;
    DeckSummary *summary;
    s32 i;
    s32 x;

    tpage = GetTPage(1, 0, 0x140, 0);
    deck = SAI_UI.menu.selected;
    sprintf(line, "%sデック", SAI_DECK_INFO.decks[deck].name);
    drawIconText(0x8C, 0x36, 7, 1, win->z, (s32)line);
    /* 種類別カード枚数 */
    sprintf(line, "%s", "種類別カード枚数");
    drawIconText(0x8C, 0x46, 7, 1, win->z, (s32)line);
    summary = &SAI_DECK_INFO.decks[deck];
    formatSjisNumber(summary->specialties[0], 2, count);
    sprintf(line, "a0s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.specialties[0], 3, total));
    drawIconText(0x8D, 0x52, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->specialties[1], 2, count);
    sprintf(line, "a1s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.specialties[1], 3, total));
    drawIconText(0xDA, 0x52, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->specialties[2], 2, count);
    sprintf(line, "a2s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.specialties[2], 3, total));
    drawIconText(0x8D, 0x5E, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->specialties[3], 2, count);
    sprintf(line, "a3s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.specialties[3], 3, total));
    drawIconText(0xDA, 0x5E, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->specialties[4], 2, count);
    sprintf(line, "a4s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.specialties[4], 3, total));
    drawIconText(0x8D, 0x6A, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->options, 2, count);
    sprintf(line, "a5s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.options, 3, total));
    drawIconText(0xDA, 0x6A, 7, 1, win->z, (s32)line);
    /* レベル別カード枚数 */
    sprintf(line, "%s", "レベル別カード枚数");
    drawIconText(0x8C, 0x7A, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->levels[0], 2, count);
    sprintf(line, "e3s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.levels[0], 3, total));
    drawIconText(0x8D, 0x86, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->levels[1], 2, count);
    sprintf(line, "e4s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.levels[1], 4, total));
    drawIconText(0xDA, 0x86, 7, 1, win->z, (s32)line);
    formatSjisNumber(summary->levels[2], 2, count);
    sprintf(line, "e5s0w-4%sw0／s0w-4%s", count, formatSjisNumber(SAI_DECK_INFO.owned.levels[2], 4, total));
    drawIconText(0x8D, 0x92, 7, 1, win->z, (s32)line);
    formatSjisNumber(SAI_DECK_INFO.cardsOwned, 4, count);
    /* 総カード枚数　s0w-4%s枚 */
    sprintf(line, "総カード枚数　s0w-4%s枚", count);
    drawIconText(0x90, 0xA2, 7, 1, win->z, (s32)line);
    formatSjisNumber(SAI_DECK_INFO.owned.sevens, 3, count);
    /* セブンズカード　%s枚 */
    sprintf(line, "セブンズカード　%s枚", count);
    drawIconText(0x8C, 0xB2, 7, 1, win->z, (s32)line);
    x = 0x8D;
    for (i = 0; i < 7; i++) {
        if ((SAI_DECK_INFO.decks[deck].sevensHeld >> i) & 1) {
            drawTexturedSprite(x, 0xC2, &uv, tpage, 0x2B14, win->z, 0x80, -1);
        }
        uv.x += 0x10;
        x += 0x11;
    }
}

void SAI_summarizeDeck(DeckSummary *summary, s8 deck) {
    s32 i;
    s32 optionId;
    s32 digimonId;
    DigimonCardData *card;

    if ((u8)deck < 30) {
        if ((summary->inUse = PLAYER_DATA(0).savedDecks[deck].inUse) != 0) {
            summary->specialties[2] = 0;
            summary->specialties[1] = 0;
            summary->specialties[0] = 0;
            summary->options = 0;
            summary->specialties[4] = 0;
            summary->specialties[3] = 0;
            summary->sevens = 0;
            summary->sevensHeld = 0;
            for (i = 0; i < 3; i++) {
                summary->levels[i] = 0;
            }
            strcpy(summary->name, PLAYER_DATA(0).savedDecks[deck].name);
            /* finds the deck's last Digimon card, but the loop below sets
               digimonId again before anything reads it: the original keeps
               this loop emptied, with the deck's address hoisted out of it */
            for (i = 0; i < 30; i++) {
                if (PLAYER_DATA(0).savedDecks[deck].cards[i].type == 0) {
                    digimonId = PLAYER_DATA(0).savedDecks[deck].cards[i].index;
                }
            }
            for (i = 0; i < 30; i++) {
                switch (PLAYER_DATA(0).savedDecks[deck].cards[i].type) {
                case 0:
                    digimonId = PLAYER_DATA(0).savedDecks[deck].cards[i].index;
                    switch (((DigimonCardData *)(DIGIMON_CARDS + digimonId * 0x122))->attr >> 4) {
                    case 0:
                        summary->specialties[0]++;
                        break;
                    case 1:
                        summary->specialties[1]++;
                        break;
                    case 2:
                        summary->specialties[2]++;
                        break;
                    case 3:
                        summary->specialties[3]++;
                        break;
                    case 4:
                        summary->specialties[4]++;
                        break;
                    }
                    card = (DigimonCardData *)(DIGIMON_CARDS + PLAYER_DATA(0).savedDecks[deck].cards[i].index * 0x122);
                    summary->levels[card->attr & 0xF]++;
                    break;
                case 1:
                    /* the Seven cards are the options 0x23 to 0x29 */
                    optionId = PLAYER_DATA(0).savedDecks[deck].cards[i].index;
                    if ((u8)(optionId - 0x23) < 7) {
                        summary->sevensHeld |= 1 << (optionId - 0x23);
                        summary->sevens++;
                    }
                case 2:
                    summary->options++;
                    break;
                }
            }
        }
    }
}

void SAI_summarizeOwnedCards(DeckSummary *owned) {
    s32 i;

    owned->specialties[2] = 0;
    owned->specialties[1] = 0;
    owned->specialties[0] = 0;
    owned->options = 0;
    owned->specialties[4] = 0;
    owned->specialties[3] = 0;
    owned->sevens = 0;
    owned->sevensHeld = 0;
    SAI_DECK_INFO.cardsOwned = 0;
    for (i = 0; i < 3; i++) {
        owned->levels[i] = 0;
    }
    for (i = 0; i < 0x6E; i++) {
        switch (((DigimonCardData *)(DIGIMON_CARDS + i * 0x122))->attr >> 4) {
        case 0:
            owned->specialties[0] += (u8)(PLAYER_DATA(0).cardCollection[i] & 0xF);
            break;
        case 1:
            owned->specialties[1] += (u8)(PLAYER_DATA(0).cardCollection[i] & 0xF);
            break;
        case 2:
            owned->specialties[2] += (u8)(PLAYER_DATA(0).cardCollection[i] & 0xF);
            break;
        case 3:
            owned->specialties[3] += (u8)(PLAYER_DATA(0).cardCollection[i] & 0xF);
            break;
        case 4:
            owned->specialties[4] += (u8)(PLAYER_DATA(0).cardCollection[i] & 0xF);
            break;
        }
        owned->levels[((DigimonCardData *)(DIGIMON_CARDS + i * 0x122))->attr & 0xF] += (u8)(PLAYER_DATA(0).cardCollection[i] & 0xF);
        SAI_DECK_INFO.cardsOwned += PLAYER_DATA(0).cardCollection[i] & 0xF;
    }
    for (i = 0; i < 0x2B; i++) {
        owned->options += (u8)(PLAYER_DATA(0).optionCollection[i] & 0xF);
        SAI_DECK_INFO.cardsOwned += PLAYER_DATA(0).optionCollection[i] & 0xF;
        if ((u32)(i - 0x23) < 7) {
            owned->sevens += (u8)(PLAYER_DATA(0).optionCollection[i] & 0xF);
        }
    }
    for (i = 0; i < 6; i++) {
        owned->options += (u8)(PLAYER_DATA(0).digivolveCollection[i] & 0xF);
        SAI_DECK_INFO.cardsOwned += PLAYER_DATA(0).digivolveCollection[i] & 0xF;
    }
}

void SAI_openDeckInfo(void) {
    s8 i;

    for (i = 0; i < 3; i++) {
        SAI_summarizeDeck(&SAI_DECK_INFO.decks[i], i);
    }
    SAI_summarizeOwnedCards(&SAI_DECK_INFO.owned);
    i = 0;
    spawnTask(0, -1, 0, 0x600, runWindowTask, &SAI_DECK_INFO_WINDOW_DEF, getCurrentTaskId());
    SAI_DECK_INFO_WINDOW = waitFrames(0x7FFFFFFF);
    openChoiceMenu(&SAI_UI.menu, 0x3B, 0x32, 0, 0);
    while (i < 3 && SAI_DECK_INFO.decks[i].inUse != 0) {
        addChoiceMenuItem(&SAI_UI.menu, i + 0x39, SAI_runMenu);
        i++;
    }
    spawnTask(0, -1, 0, 0x1000, SAI_runMenu, 0, getCurrentTaskId(), 0, 0);
}

s8 SAI_getBackgroundState(void) {
    return SCROLLING_BACKGROUND->unk1BE;
}
