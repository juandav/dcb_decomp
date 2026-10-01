#include "common.h"
#include "game.h"
#include "dcb/sub_auto_deck.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"
#include "dcb/frame_callback.h"
#include "dcb/dialog.h"
#include "dcb/subseg.h"
#include "dcb/sub_name_entry.h"
#include "dcb/sub_base_deck.h"
#include "dcb/sub_sort.h"
#include "dcb/sub_deck_editor.h"
#include "dcb/sub_deck_screens.h"

typedef struct {
    s16 x;
    s16 w;
    u8 next[4];
} MenuItem;

extern CardIdList *SUB_AUTO_DECK_POOLS;
extern CursorHighlight SUB_AUTO_DECK_CURSOR;
extern u8 SUB_AUTO_DECK_OPTIONS[6];
extern s8 *SUB_AUTO_DECK_CARD;
extern u8 SUB_AUTO_DECK_SPECIALTY;
extern u8 SUB_AUTO_DECK_STYLE;
extern u8 SUB_AUTO_DECK_OPTION_AMOUNT;
extern u8 SUB_AUTO_DECK_RESULT;

extern void SUB_buildAutoDeck(PlayerDeck *deck);

MenuItem SUB_AUTO_DECK_ITEMS[13] = {
    { 84, 36, { 11, 3, 1, 1 } },
    { 156, 36, { 12, 5, 0, 0 } },
    { 54, 18, { 0, 7, 6, 3 } },
    { 84, 12, { 0, 7, 2, 4 } },
    { 108, 36, { 0, 7, 3, 5 } },
    { 156, 48, { 1, 8, 4, 6 } },
    { 216, 24, { 1, 8, 5, 2 } },
    { 96, 42, { 3, 9, 8, 8 } },
    { 168, 42, { 5, 10, 7, 7 } },
    { 96, 24, { 7, 11, 10, 10 } },
    { 168, 18, { 8, 12, 9, 9 } },
    { 60, 36, { 9, 0, 12, 12 } },
    { 120, 36, { 10, 1, 11, 11 } },
};

/* the level of each option card */
u8 SUB_OPTION_CARD_LEVELS[108] = {
    1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1,
    0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1,
    1, 0, 1, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1,
    /* leftovers after the last option card, not the same in every version */
#if VERSION_US
    0, 0, 0, 0, 0, 0,
#elif VERSION_EU
    0x27, 0x18, 0x00, 0xB0, 0xAF, 0x21,
#else
#error "subseg/deck/sub_auto_deck: version not checked"
#endif
};

void SUB_drawAutoDeckOptions(UiWindow *window) {
    Rect16 rect;
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 dir = 0;
    MenuItem *item;
    s32 dy;

    if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0xF000) {
        playMenuSound(2);
    }
    if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x1000) {
        if (SUB_AUTO_DECK_OPTIONS[1] == 0) {
            if (SUB_AUTO_DECK_OPTIONS[0] == 11 || SUB_AUTO_DECK_OPTIONS[0] == 12) {
                PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
            }
        } else if (SUB_AUTO_DECK_OPTIONS[0] >= 2 && SUB_AUTO_DECK_OPTIONS[0] <= 6) {
            PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
        }
        dir = 1;
    } else if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x4000) {
        if (SUB_AUTO_DECK_OPTIONS[1] == 0) {
            if (SUB_AUTO_DECK_OPTIONS[0] < 2) {
                PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
            }
        } else if (SUB_AUTO_DECK_OPTIONS[0] == 9 || SUB_AUTO_DECK_OPTIONS[0] == 10) {
            PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
        }
        dir = 2;
    } else if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x8000) {
        if (SUB_AUTO_DECK_OPTIONS[0] == 1 || SUB_AUTO_DECK_OPTIONS[0] == 3 || SUB_AUTO_DECK_OPTIONS[0] == 8 || SUB_AUTO_DECK_OPTIONS[0] == 10 || SUB_AUTO_DECK_OPTIONS[0] == 12) {
            PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
        }
        dir = 3;
    } else if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x2000) {
        if (SUB_AUTO_DECK_OPTIONS[0] == 0 || SUB_AUTO_DECK_OPTIONS[0] == 5 || SUB_AUTO_DECK_OPTIONS[0] == 7 || SUB_AUTO_DECK_OPTIONS[0] == 9 || SUB_AUTO_DECK_OPTIONS[0] == 11) {
            PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
        }
        dir = 4;
    } else if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 0x40) {
        dir = 5;
        playMenuSound(1);
    }
    if (dir >= 1 && dir <= 4) {
        SUB_AUTO_DECK_OPTIONS[0] = SUB_AUTO_DECK_ITEMS[SUB_AUTO_DECK_OPTIONS[0]].next[dir - 1];
        if (SUB_AUTO_DECK_OPTIONS[1] == 0) {
            switch (SUB_AUTO_DECK_OPTIONS[0]) {
            case 3:
                SUB_AUTO_DECK_OPTIONS[0] = 11;
                break;
            case 5:
                SUB_AUTO_DECK_OPTIONS[0] = 12;
                break;
            case 9:
                SUB_AUTO_DECK_OPTIONS[0] = 0;
                break;
            case 10:
                SUB_AUTO_DECK_OPTIONS[0] = 1;
                break;
            }
        }
        item = &SUB_AUTO_DECK_ITEMS[SUB_AUTO_DECK_OPTIONS[0]];
        if (SUB_AUTO_DECK_OPTIONS[0] < 2) {
            dy = 0;
        } else if (SUB_AUTO_DECK_OPTIONS[0] < 7) {
            dy = 0x18;
        } else if (SUB_AUTO_DECK_OPTIONS[0] < 9) {
            dy = 0x30;
        } else if (SUB_AUTO_DECK_OPTIONS[0] < 11) {
            dy = 0x48;
        } else {
            dy = 0x60;
        }
        rect.x = window->rect.x + item->x;
        rect.y = window->rect.y + dy;
        rect.w = item->w;
        rect.h = 12;
        moveCursorHighlight(&SUB_AUTO_DECK_CURSOR, &rect);
    } else if (dir == 5) {
        switch (SUB_AUTO_DECK_OPTIONS[0]) {
        case 0:
            SUB_AUTO_DECK_ENABLED = 0;
            break;
        case 1:
            SUB_AUTO_DECK_ENABLED = 1;
            break;
        case 2:
            SUB_AUTO_DECK_SPECIALTY = 0;
            break;
        case 3:
            SUB_AUTO_DECK_SPECIALTY = 1;
            break;
        case 4:
            SUB_AUTO_DECK_SPECIALTY = 2;
            break;
        case 5:
            SUB_AUTO_DECK_SPECIALTY = 3;
            break;
        case 6:
            SUB_AUTO_DECK_SPECIALTY = 4;
            break;
        case 7:
            SUB_AUTO_DECK_STYLE = 0;
            break;
        case 8:
            SUB_AUTO_DECK_STYLE = 1;
            break;
        case 9:
            SUB_AUTO_DECK_OPTION_AMOUNT = 0;
            break;
        case 10:
            SUB_AUTO_DECK_OPTION_AMOUNT = 1;
            break;
        case 11:
            SUB_AUTO_DECK_RESULT = 1;
            break;
        case 12:
            SUB_AUTO_DECK_RESULT = 3;
            break;
        }
    }
    drawText(x, y, (s32)"Auto Deck", 6, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[0].x, y, (s32)SUB_STR_DISABLE, SUB_AUTO_DECK_ENABLED ? 8 : 7, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[1].x, y, (s32)"Enable", SUB_AUTO_DECK_ENABLED ? 7 : 8, z);
    drawText(x, y + 0x18, (s32)"Specialty", 6, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[2].x, y + 0x18, (s32)"Fire", (SUB_AUTO_DECK_OPTIONS[2] == 0 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[3].x, y + 0x18, (s32)"Ice", (SUB_AUTO_DECK_OPTIONS[2] == 1 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[4].x, y + 0x18, (s32)"Nature", (SUB_AUTO_DECK_OPTIONS[2] == 2 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[5].x, y + 0x18, (s32)"Darkness", (SUB_AUTO_DECK_OPTIONS[2] == 3 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[6].x, y + 0x18, (s32)"Rare", (SUB_AUTO_DECK_OPTIONS[2] == 4 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x, y + 0x30, (s32)SUB_STR_TYPE, 6, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[7].x, y + 0x30, (s32)"Offensive", (SUB_AUTO_DECK_OPTIONS[3] == 0 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[8].x, y + 0x30, (s32)"Defensive", (SUB_AUTO_DECK_OPTIONS[3] != 0 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x, y + 0x48, (s32)"Option Cards", 6, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[9].x, y + 0x48, (s32)"Many", (SUB_AUTO_DECK_OPTIONS[4] == 0 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[10].x, y + 0x48, (s32)"Few", (SUB_AUTO_DECK_OPTIONS[4] != 0 && SUB_AUTO_DECK_OPTIONS[1]) ? 7 : 8, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[11].x, y + 0x60, (s32)"Create", 6, z);
    drawText(x + SUB_AUTO_DECK_ITEMS[12].x, y + 0x60, (s32)"Cancel", 6, z);
    drawCursorHighlight(&SUB_AUTO_DECK_CURSOR, z);
}

void SUB_openAutoDeckMenu(void) {
    Rect16 rects[2];
    s32 flags[2] = { 8, 0x21 };
    s32 styles[2] = { 0x21, 0x31 };
    const char *labels[2] = { "AUTO DECK", SUB_STR_DECK_TITLE };
    s32 i;

    rects[0].x = 0xAB;
    rects[0].y = 0x8D;
    rects[0].w = 0xF2;
    rects[0].h = 0x6E;
    for (i = 0; i < 7; i++) {
        if (i != 1) {
            SUB_openCenteredWindow(&SUB_WINDOWS[i], rects[i], (s32)labels[i], flags[i], styles[i]);
        }
    }
    rects[0].x = SUB_WINDOWS[0].originX + 0x54;
    rects[0].y = SUB_WINDOWS[0].originY;
    rects[0].w = 0x24;
    rects[0].h = 0xC;
    initCursorHighlight(&SUB_AUTO_DECK_CURSOR, &rects[0], (Bytes4 *)-1);
    SUB_AUTO_DECK_OPTIONS[0] = SUB_AUTO_DECK_OPTIONS[5] = SUB_AUTO_DECK_OPTIONS[1] = SUB_AUTO_DECK_OPTIONS[2] = SUB_AUTO_DECK_OPTIONS[3] = SUB_AUTO_DECK_OPTIONS[4] = 0;
}

void SUB_drawAutoDeckMenu(void) {
    drawWindow(SUB_WINDOWS, SUB_drawAutoDeckOptions, 30);
}

void SUB_runAutoDeckMenu(void) {
    u8 dialog[0xB8];
    s32 result;

    playMenuSound(3);
    SUB_openAutoDeckMenu();
    addFrameCallback((s32)SUB_drawAutoDeckMenu);
    do {
        waitFrames(1);
    } while (SUB_AUTO_DECK_OPTIONS[5] == 0);
    animateWindowTo(SUB_WINDOWS, (Rect16 *)-1);
    waitFrames(20);
    removeFrameCallback((s32)SUB_drawAutoDeckMenu);
    switch (SUB_AUTO_DECK_OPTIONS[5]) {
    case 1:
        if (SUB_AUTO_DECK_OPTIONS[1] == 1) {
            PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse = 1;
            SUB_buildAutoDeck(&PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot]);
        } else {
            PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse = 0;
            initDialog(dialog, "Do you want to choose from a Base Deck?", 1);
            runDialogForPad((s32 *)dialog, SUB_EDITOR.player);
            result = (s8)dialog[0xA5];
            if (result == 1 && SUB_chooseBaseDeck(&PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot], SUB_EDITOR.player) >= 0) {
                PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse = result;
            }
        }
        spawnTask(0, -1, 0, 0x1000, SUB_editDeck, &PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot], 0, 0, 0);
        break;
    case 3:
        PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse = 0;
        spawnTask(0, -1, 0, 0x1000, SUB_enterDeckName, 0, PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].name, SUB_EDITOR.player, 0);
        break;
    }
}

s32 SUB_takePoolCards(CardSlot *slots, s32 row, s32 count) {
    s32 i;

    for (i = 0; i < count && SUB_AUTO_DECK_POOLS[row].ids[i] != -1; i++) {
        setCardSlotFromId((u8 *)slots, SUB_AUTO_DECK_POOLS[row].ids[i]);
        SUB_AUTO_DECK_POOLS[row].ids[i] = -1;
        slots++;
    }
    return i;
}

void SUB_buildAutoDeck(PlayerDeck *deck) {
    u8 counts[8] = { 10, 10, 6, 4, 5, 12, 8, 5 };
    s16 specialty = 0;
    s16 level = 0;
    s16 *ids = NULL;
    s32 i;
    s32 j;
    s32 n;
    s16 count;

    SUB_AUTO_DECK_POOLS = allocTaskHeapBlock(0x2760);
    for (i = 0; i < 20; i++) {
        SUB_AUTO_DECK_POOLS[i].count = 0;
    }
    for (i = 0; i < 301; i++) {
        if (i >= 0xAC && i < 0xBF) {
            continue;
        }
        n = getOwnedCardCount(SUB_EDITOR.player, i);
        if (i < 0xBF) {
            SUB_AUTO_DECK_CARD = (s8 *)&((DigimonCardData *)DIGIMON_CARDS)[i];
        } else if (i - 0xBF < 0x66) {
            SUB_AUTO_DECK_CARD = (s8 *)&((OptionCardData *)OPTION_CARDS)[i - 0xBF];
        } else {
            SUB_AUTO_DECK_CARD = (s8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[i - 0x125];
        }
        switch (SUB_AUTO_DECK_CARD[2]) {
        case 0:
            specialty = ((DigimonCardData *)SUB_AUTO_DECK_CARD)->attr >> 4;
            level = ((DigimonCardData *)SUB_AUTO_DECK_CARD)->attr & 0xF;
            if (level > 0) {
                level--;
            }
            ids = SUB_AUTO_DECK_POOLS[specialty * 3 + level].ids;
            ids += SUB_AUTO_DECK_POOLS[specialty * 3 + level].count;
            break;
        case 1:
            specialty = 5;
            level = SUB_OPTION_CARD_LEVELS[i - 0xBF];
            ids = SUB_AUTO_DECK_POOLS[specialty * 3 + level].ids;
            ids += SUB_AUTO_DECK_POOLS[specialty * 3 + level].count;
            break;
        case 2:
            specialty = 5;
            level = 2;
            ids = SUB_AUTO_DECK_POOLS[specialty * 3 + level].ids;
            ids += SUB_AUTO_DECK_POOLS[specialty * 3 + level].count;
            break;
        }
        if (n > 0) {
            if (n >= 4) {
                n = 4;
            }
            for (j = 0; j < n; j++) {
                SUB_AUTO_DECK_POOLS[specialty * 3 + level].count++;
                *ids++ = i;
            }
        }
    }
    for (i = 0; i < 20; i++) {
        for (j = SUB_AUTO_DECK_POOLS[i].count; j < 250; j++) {
            SUB_AUTO_DECK_POOLS[i].ids[j] = -1;
        }
    }
    for (i = 0; i < 20; i++) {
        for (j = 0; j < SUB_AUTO_DECK_POOLS[i].count; j++) {
            specialty = abs(rand() % SUB_AUTO_DECK_POOLS[i].count);
            count = SUB_AUTO_DECK_POOLS[i].ids[j];
            SUB_AUTO_DECK_POOLS[i].ids[j] = SUB_AUTO_DECK_POOLS[i].ids[specialty];
            SUB_AUTO_DECK_POOLS[i].ids[specialty] = count;
        }
    }
    n = 0;
    count = 0;
    for (i = 0; i < 5; i++) {
        if (i != SUB_AUTO_DECK_OPTIONS[2]) {
            if (count <= SUB_AUTO_DECK_POOLS[i * 3].count + SUB_AUTO_DECK_POOLS[i * 3 + 1].count + SUB_AUTO_DECK_POOLS[i * 3 + 2].count) {
                specialty = i;
                count = SUB_AUTO_DECK_POOLS[i * 3].count + SUB_AUTO_DECK_POOLS[i * 3 + 1].count + SUB_AUTO_DECK_POOLS[i * 3 + 2].count;
            }
        }
    }
    n += SUB_takePoolCards(&deck->cards[n], SUB_AUTO_DECK_OPTIONS[3] + 15, counts[SUB_AUTO_DECK_OPTIONS[4] * 4]);
    if (n < counts[SUB_AUTO_DECK_OPTIONS[4] * 4]) {
        n += SUB_takePoolCards(&deck->cards[n], 17, counts[SUB_AUTO_DECK_OPTIONS[4] * 4] - n);
        if (n < counts[SUB_AUTO_DECK_OPTIONS[4] * 4]) {
            n += SUB_takePoolCards(&deck->cards[n], 16 - SUB_AUTO_DECK_OPTIONS[3], counts[SUB_AUTO_DECK_OPTIONS[4] * 4] - n);
        }
    }
    for (i = 0; i < 3; i++) {
        if (i == 0) {
            count = 0;
            for (j = 0; j < 3; j++) {
                if (PLAYER_DATA(SUB_EDITOR.player).partners[j].cardId != 0) {
                    setCardSlotFromId((u8 *)&deck->cards[n + count], PLAYER_DATA(SUB_EDITOR.player).partners[j].cardId);
                    count++;
                }
            }
            count = count + SUB_takePoolCards(&deck->cards[n + count], SUB_AUTO_DECK_OPTIONS[2] * 3 + i, counts[i + 1 + SUB_AUTO_DECK_OPTIONS[4] * 4] - count);
        } else {
            count = SUB_takePoolCards(&deck->cards[n], SUB_AUTO_DECK_OPTIONS[2] * 3 + i, counts[i + 1 + SUB_AUTO_DECK_OPTIONS[4] * 4]);
        }
        if (count < counts[i + 1 + SUB_AUTO_DECK_OPTIONS[4] * 4]) {
            count = count + SUB_takePoolCards(&deck->cards[n + count], specialty * 3 + i, counts[i + 1 + SUB_AUTO_DECK_OPTIONS[4] * 4] - count);
        }
        n += count;
    }
    for (i = 0; i < 20 && n < 30; i++) {
        for (j = 0; j < SUB_AUTO_DECK_POOLS[i].count; j++) {
            if (SUB_AUTO_DECK_POOLS[i].ids[j] != -1) {
                setCardSlotFromId((u8 *)&deck->cards[n], SUB_AUTO_DECK_POOLS[i].ids[j]);
                SUB_AUTO_DECK_POOLS[i].ids[j] = -1;
                n++;
                if (n >= 30) {
                    break;
                }
            }
        }
    }
    linkDeckCardData(SUB_EDITOR.player, deck);
}
