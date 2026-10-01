#include "common.h"
#include "game.h"
#include "dcb/sub_sort.h"
#include "dcb/text.h"
#include "dcb/card_db.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"
#include "dcb/sort.h"
#include "dcb/subseg.h"

/* a comparison function for sortArray */
typedef s32 (*SortCompare)(s8 *, s8 *);

Menu SUB_CARD_LIST_MENU = { NULL, NULL, { 15, 44, 298, 96 }, 0, -1, 0, -1, 0xa, 0x41, 115, 12, 0, 301, 147, 1, 0, 12, 0, 0, 0 };

/* the partners' names; not referenced by any code */
char *SUB_PARTNER_NAMES_JP[6] = {
    "\xCC\xDE\xB2\xD3\xDD", /* ﾌﾞｲﾓﾝ */
    "\xCE\xB0\xB8\xD3\xDD", /* ﾎｰｸﾓﾝ */
    "\xB1\xD9\xCF\xBC\xDE\xD3\xDD", /* ｱﾙﾏｼﾞﾓﾝ */
    "\xC3\xB2\xD9\xD3\xDD", /* ﾃｲﾙﾓﾝ */
    "\xCA\xDF\xC0\xD3\xDD", /* ﾊﾟﾀﾓﾝ */
    "\xDC\xB0\xD1\xD3\xDD", /* ﾜｰﾑﾓﾝ */
};

u8 SUB_CARD_LIST_COLORS[4][4] = {
    { 0x80, 0x80, 0x80, 0 },
    { 0x40, 0x40, 0x40, 0 },
    { 0x60, 0x60, 0x60, 0 },
    { 0xC0, 0x60, 0x60, 0 },
};

/* the card list's sort orders */
char *SUB_CARD_SORT_LABELS[20] = {
    "Number",
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *ea",
    "Level *e4",
    "Level *e5",
    "*d6 Strength",
    "*d4 Required DP",
    "*d5 Added DP",
    "*b0 Attack Power",
    "*b1 Attack Power",
    "*b2 Attack Power",
    "*g9 Newly Obtained",
    "Cards in Stock",
};

char *SUB_CARD_SORT_LABELS_EDITING[20] = {
    "Number",
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *ea",
    "Level *e4",
    "Level *e5",
    "*d6 Strength",
    "*d4 Required DP",
    "*d5 Added DP",
    "*b0 Attack Power",
    "*b1 Attack Power",
    "*b2 Attack Power",
    "*g9 Newly Obtained",
    "Max Usable Cards",
};

Menu SUB_CARD_SORT_MENU = { NULL, NULL, { 40, 60, 124, 112 }, 0, -1, 0, -1, 0xa, 0x16, 114, 12, 0, 20, 0, 1, 0, 14, 0, 0, 0 };

s32 SUB_compareListFireFirst(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListIceFirst(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListNatureFirst(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListDarknessFirst(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListRareFirst(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr >> 4;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr >> 4;
    }
    if (x == 4) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 4) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListOptionFirst(s8 **a, s8 **b) {
    s32 x = (*a)[2] == 1;
    s32 y = (*b)[2] == 1;

    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListDigivolveFirst(s8 **a, s8 **b) {
    s32 x = (*a)[2] == 2;
    s32 y = (*b)[2] == 2;

    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListLevel0First(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListLevel2First(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListLevel3First(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListLevel1First(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attr & 0xF;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attr & 0xF;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListByHp(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->hp;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->hp;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListByCount(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if (SUB_EDITOR.useDeckCounts == 0) {
        x = getOwnedCardCount(SUB_EDITOR.player, *(s16 *)*a);
        y = getOwnedCardCount(SUB_EDITOR.player, *(s16 *)*b);
    } else {
        x = SUB_DECK_EDIT.deckCounts[*(s16 *)*a];
        y = SUB_DECK_EDIT.deckCounts[*(s16 *)*b];
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListByDpCost(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->dpCost;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->dpCost;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListByDpBonus(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->dpBonus;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->dpBonus;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListByCircleAttack(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[0].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[0].power;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListByTriangleAttack(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[1].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[1].power;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListByCrossAttack(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if ((*a)[2] != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)*a)->attack[2].power;
    }
    if ((*b)[2] != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)*b)->attack[2].power;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

s32 SUB_compareListNewFirst(s8 **a, s8 **b) {
    s32 x;
    s32 y;

    if (PLAYER_DATA(SUB_EDITOR_PLAYER).cardCollection[*(s16 *)*a] & 0x80) {
        x = 1;
    } else {
        x = 0;
    }
    y = (PLAYER_DATA(SUB_EDITOR_PLAYER).cardCollection[*(s16 *)*b] & 0x80) != 0;
    if (!(PLAYER_DATA(SUB_EDITOR_PLAYER).cardCollection[*(s16 *)*a] & 0x40)) {
        x = -1;
    }
    if (!(PLAYER_DATA(SUB_EDITOR_PLAYER).cardCollection[*(s16 *)*b] & 0x40)) {
        y = -1;
    }
    return y - x;
}

SortCompare SUB_CARD_SORT_COMPARES[20] = {
    NULL,
    (SortCompare)SUB_compareListFireFirst,
    (SortCompare)SUB_compareListIceFirst,
    (SortCompare)SUB_compareListNatureFirst,
    (SortCompare)SUB_compareListDarknessFirst,
    (SortCompare)SUB_compareListRareFirst,
    (SortCompare)SUB_compareListOptionFirst,
    (SortCompare)SUB_compareListDigivolveFirst,
    (SortCompare)SUB_compareListLevel0First,
    (SortCompare)SUB_compareListLevel1First,
    (SortCompare)SUB_compareListLevel2First,
    (SortCompare)SUB_compareListLevel3First,
    (SortCompare)SUB_compareListByHp,
    (SortCompare)SUB_compareListByDpCost,
    (SortCompare)SUB_compareListByDpBonus,
    (SortCompare)SUB_compareListByCircleAttack,
    (SortCompare)SUB_compareListByTriangleAttack,
    (SortCompare)SUB_compareListByCrossAttack,
    (SortCompare)SUB_compareListNewFirst,
    (SortCompare)SUB_compareListByCount,
};

/* shown, with dashes for the name, for a card whose collection entry lacks flag 0x40 */
const char SUB_STR_QUESTION_MARK[] = "?";

void SUB_initCardList(void) {
    s32 n = 0;
    s32 i;

    for (i = 0; i < 0xBF; i++) {
        SUB_CARD_LIST[n++] = &((DigimonCardData *)DIGIMON_CARDS)[i];
    }
    for (i = 0; i < 0x66; i++) {
        SUB_CARD_LIST[n++] = &((OptionCardData *)OPTION_CARDS)[i];
    }
    for (i = 0; i < 8; i++) {
        SUB_CARD_LIST[n++] = &((DigivolveCardData *)DIGIVOLVE_CARDS)[i];
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(SUB_EDITOR.player).partners[i].cardId != 0) {
            SUB_CARD_LIST[PLAYER_DATA(SUB_EDITOR.player).partners[i].cardId] = &PLAYER_DATA(SUB_EDITOR.player).partners[i].card[0];
            if (PLAYER_DATA(SUB_EDITOR.player).partners[i].armorCardId != 0) {
                SUB_CARD_LIST[PLAYER_DATA(SUB_EDITOR.player).partners[i].armorCardId] = &PLAYER_DATA(SUB_EDITOR.player).partners[i].card[1];
            }
        }
    }
    for (i = 0; i < 301; i++) {
        SUB_CARDS_BY_ID[i] = SUB_CARD_LIST[i];
    }
}

void SUB_drawCardSortMenu(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 z = window->z;
    s32 i;
    s32 y;

    for (i = 0; i < SUB_CARD_SORT_MENU.nrows; i++) {
        if (i < window->view.y / SUB_CARD_SORT_MENU.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / SUB_CARD_SORT_MENU.rowH < i) {
            break;
        }
        y = window->originY + i * SUB_CARD_SORT_MENU.rowH + 1;
        if (SUB_EDITOR.useDeckCounts == 0) {
            drawText(x, y, (s32)SUB_CARD_SORT_LABELS[i], 7, z);
        } else {
            drawText(x, y, (s32)SUB_CARD_SORT_LABELS_EDITING[i], 7, z);
        }
    }
    updateMenuCursor(&SUB_CARD_SORT_MENU);
    if (SUB_CARD_SORT_MENU.active && SUB_EDITOR.listShown && (PAD_STATES[SUB_EDITOR.player]->pressed & 0x40)) {
        playMenuSound(1);
        SUB_CARD_LIST_MENU.row = 0;
        centerMenuOnCursor(&SUB_CARD_LIST_MENU);
        if (SUB_CARD_SORT_COMPARES[SUB_CARD_SORT_MENU.row] != NULL) {
            sortArray((s8 *)SUB_CARD_LIST, 301, 4, SUB_CARD_SORT_COMPARES[SUB_CARD_SORT_MENU.row]);
        } else {
            SUB_initCardList();
        }
    }
}

void SUB_drawCardList(UiWindow *window) {
    char buf[64];
    s32 x = window->originX - 4;
    s32 z = window->z;
    s32 i;
    s32 y;
    u8 palette;
    s32 type;
    u8 *rgb;
    s32 count;

    for (i = 0; i < SUB_CARD_LIST_MENU.nrows; i++) {
        if (i < window->view.y / SUB_CARD_LIST_MENU.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / SUB_CARD_LIST_MENU.rowH < i) {
            break;
        }
        y = window->originY + i * SUB_CARD_LIST_MENU.rowH + 1;
        palette = 7;
        type = ((s8 *)SUB_CARD_LIST[i])[2];
        rgb = SUB_CARD_LIST_COLORS[0];
        count = getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_CARD_LIST[i]);
        if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)SUB_CARD_LIST[i]] & 0x40) {
            if (count == 0) {
                rgb = SUB_CARD_LIST_COLORS[2];
            }
            drawTextColored(x + 0x86, y, (u8 *)SUB_CARD_LIST[i] + 3, rgb, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x74, y, 0, ((DigimonCardData *)SUB_CARD_LIST[i])->attr >> 4, rgb, z);
                if (palette == 3) {
                    drawIconColored(x + 0x46, y, 0, (((DigimonCardData *)SUB_CARD_LIST[i])->attr & 0xF) + 0x10, SUB_CARD_LIST_COLORS[3], z);
                } else {
                    drawIconColored(x + 0x46, y, 0, (((DigimonCardData *)SUB_CARD_LIST[i])->attr & 0xF) + 0x10, rgb, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x74, y, 0, 5, rgb, z);
                break;
            case 2:
                drawIconColored(x + 0x74, y, 0, 6, rgb, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x46, y, (u8 *)SUB_STR_QUESTION_MARK, rgb, palette, z);
            drawTextColored(x + 0x86, y, "-------------------", rgb, palette, z);
            drawTextColored(x + 0x74, y, (u8 *)SUB_STR_QUESTION_MARK, rgb, palette, z);
        }
        if (type == 0 || !(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)SUB_CARD_LIST[i]] & 0x40)) {
            drawTextColored(x + 0x38, y, (u8 *)SUB_STR_LV, rgb, palette, z);
            drawTextColored(x + 0x56, y, (u8 *)SUB_STR_TYPE, rgb, palette, z);
        }
        sprintf(buf, SUB_FMT_CARD_NUMBER, *(s16 *)SUB_CARD_LIST[i]);
        drawTextColored(x + 0x20, y, buf, rgb, palette, z);
        sprintf(buf, SUB_FMT_COUNT, count);
        drawTextColored(x + 0x100, y, buf, rgb, palette, z);
        drawTextColored(x + 0x108, y, (u8 *)SUB_STR_CARDS, rgb, palette, z);
        if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)SUB_CARD_LIST[i]] & 0x80) {
            drawIconColored(x + 4, y, 2, 9, rgb, z);
        }
        if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)SUB_CARD_LIST[i]] & 0x10) {
            drawIconColored(x + 10, y, 0, 0x14, rgb, z);
        }
    }
    updateMenuCursor(&SUB_CARD_LIST_MENU);
    SUB_COLLECTION_STATS.selectedCard = SUB_CARD_LIST[SUB_CARD_LIST_MENU.row];
    SUB_COLLECTION_STATS.selectedId = *(s16 *)SUB_CARD_LIST[SUB_CARD_LIST_MENU.row];
}

const char SUB_STR_LV[] = "Lv";

const char SUB_STR_TYPE[] = "Type";

const char SUB_FMT_CARD_NUMBER[] = "*s0%3.3d";

const char SUB_FMT_COUNT[] = "%d";

const char SUB_STR_CARDS[] = "Cards";

/* the deck's sort orders */
char *SUB_DECK_SORT_LABELS[18] = {
    "Number",
    "*a0 Fire",
    "*a1 Ice",
    "*a2 Nature",
    "*a3 Darkness",
    "*a4 Rare",
    "*a5 Option",
    "*a6 Option",
    "Level *e3",
    "Level *e4",
    "Level *e5",
    "*d6 Strength",
    "*d4 Required DP",
    "*d5 Added DP",
    "*b0 Attack Power",
    "*b1 Attack Power",
    "*b2 Attack Power",
    "Cards Used",
};

Menu SUB_DECK_SORT_MENU = { NULL, NULL, { 40, 60, 100, 112 }, 0, -1, 0, -1, 0xa, 0x16, 90, 12, 0, 18, 0, 1, 0, 14, 0, 0, 0 };

s32 SUB_compareDeckByNumber(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type == 0xFF) {
        x = -2;
    } else {
        x = 301 - *(s16 *)a->card;
    }
    if (b->type == 0xFF) {
        y = -2;
    } else {
        y = 301 - *(s16 *)b->card;
    }
    return y - x;
}

s32 SUB_compareDeckFireFirst(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckIceFirst(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 1) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 1) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckNatureFirst(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckDarknessFirst(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckRareFirst(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr >> 4;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr >> 4;
    }
    if (x == 4) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 4) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckOptionFirst(CardSlot *a, CardSlot *b) {
    s32 x = a->type == 1;
    s32 y = b->type == 1;

    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckDigivolveFirst(CardSlot *a, CardSlot *b) {
    s32 x = a->type == 2;
    s32 y = b->type == 2;

    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckLevel0First(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 0) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 0) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckLevel2First(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 2) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 2) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckLevel3First(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attr & 0xF;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attr & 0xF;
    }
    if (x == 3) {
        x = 1;
    } else {
        x = 0;
    }
    if (y == 3) {
        y = 1;
    } else {
        y = 0;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

void SUB_linkPartnerCards(CardSlot *cards, s32 player) {
    s32 i;
    s32 j;
    u8 cardId;

    for (i = 0; i < 3; i++) {
        cardId = PLAYER_DATA(player).partners[i].cardId;
        if (cardId != 0) {
            for (j = 0; j < 30; j++) {
                if (cards[j].type != 0xFF && *(s16 *)cards[j].card == cardId) {
                    cards[j].card = (s8 *)&PLAYER_DATA(player).partners[i];
                    break;
                }
            }
        }
    }
}

s32 SUB_compareDeckByHp(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->hp;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->hp;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckByDpCost(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->dpCost;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->dpCost;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckByDpBonus(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->dpBonus;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->dpBonus;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckByCircleAttack(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[0].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[0].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckByTriangleAttack(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[1].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[1].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckByCrossAttack(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type != 0) {
        x = -1;
    } else {
        x = ((DigimonCardData *)a->card)->attack[2].power;
    }
    if (b->type != 0) {
        y = -1;
    } else {
        y = ((DigimonCardData *)b->card)->attack[2].power;
    }
    if (a->type == 0xFF) {
        x = -2;
    }
    if (b->type == 0xFF) {
        y = -2;
    }
    return y - x;
}

s32 SUB_compareDeckByCardsLeft(CardSlot *a, CardSlot *b) {
    s32 x;
    s32 y;

    if (a->type == 0xFF) {
        x = -1;
    } else {
        x = getOwnedCardCount(SUB_EDITOR.player, *(s16 *)a->card) - SUB_DECK_EDIT.deckCounts[*(s16 *)a->card];
    }
    if (b->type == 0xFF) {
        y = -1;
    } else {
        y = getOwnedCardCount(SUB_EDITOR.player, *(s16 *)b->card) - SUB_DECK_EDIT.deckCounts[*(s16 *)b->card];
    }
    return y - x;
}

SortCompare SUB_DECK_SORT_COMPARES[18] = {
    (SortCompare)SUB_compareDeckByNumber,
    (SortCompare)SUB_compareDeckFireFirst,
    (SortCompare)SUB_compareDeckIceFirst,
    (SortCompare)SUB_compareDeckNatureFirst,
    (SortCompare)SUB_compareDeckDarknessFirst,
    (SortCompare)SUB_compareDeckRareFirst,
    (SortCompare)SUB_compareDeckOptionFirst,
    (SortCompare)SUB_compareDeckDigivolveFirst,
    (SortCompare)SUB_compareDeckLevel0First,
    (SortCompare)SUB_compareDeckLevel2First,
    (SortCompare)SUB_compareDeckLevel3First,
    (SortCompare)SUB_compareDeckByHp,
    (SortCompare)SUB_compareDeckByDpCost,
    (SortCompare)SUB_compareDeckByDpBonus,
    (SortCompare)SUB_compareDeckByCircleAttack,
    (SortCompare)SUB_compareDeckByTriangleAttack,
    (SortCompare)SUB_compareDeckByCrossAttack,
    (SortCompare)SUB_compareDeckByCardsLeft,
};

void SUB_groupDuplicateCards(CardSlot *cards) {
    CardSlot sorted[30];
    s16 ids[30];
    CardSlot *out = sorted;
    s8 count = 0;
    s32 i;
    s32 j;

    for (i = 0; i < 30; i++) {
        sorted[i].type = 0xFF;
        if (cards[i].type == 0xFF) {
            ids[i] = -1;
        } else {
            ids[i] = *(s16 *)cards[i].card;
        }
    }
    for (i = 0; i < 30; i++) {
        if (ids[i] != -1) {
            for (j = i; j < 30; j++) {
                if (j == i) {
                    count = 0;
                } else if (ids[i] == ids[j]) {
                    ids[j] = -1;
                    count++;
                }
            }
            while (count >= 0) {
                count--;
                *out++ = cards[i];
            }
        }
    }
    for (i = 0; i < 30; i++) {
        cards[i] = sorted[i];
    }
}

void SUB_drawDeckSortMenu(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 z = window->z;
    s32 i;

    for (i = 0; i < SUB_DECK_SORT_MENU.nrows; i++) {
        if (i < window->view.y / SUB_DECK_SORT_MENU.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / SUB_DECK_SORT_MENU.rowH < i) {
            break;
        }
        drawText(x, window->originY + i * SUB_DECK_SORT_MENU.rowH + 1, (s32)SUB_DECK_SORT_LABELS[i], 7, z);
    }
    updateMenuCursor(&SUB_DECK_SORT_MENU);
    if (SUB_DECK_SORT_MENU.active && (PAD_STATES[SUB_EDITOR.player]->pressed & 0x40)) {
        playMenuSound(1);
        if (SUB_DECK_SORT_COMPARES[SUB_DECK_SORT_MENU.row] != NULL) {
            linkDeckCardData(SUB_EDITOR.player, SUB_EDITED_DECK);
            SUB_linkPartnerCards(SUB_EDITED_DECK->cards, SUB_EDITOR.player);
            sortArray((s8 *)SUB_EDITED_DECK->cards, 30, 8, SUB_DECK_SORT_COMPARES[SUB_DECK_SORT_MENU.row]);
            if (SUB_DECK_SORT_MENU.row == 17) {
                SUB_groupDuplicateCards(SUB_EDITED_DECK->cards);
                linkDeckCardData(SUB_EDITOR.player, SUB_EDITED_DECK);
            }
        }
    }
}
