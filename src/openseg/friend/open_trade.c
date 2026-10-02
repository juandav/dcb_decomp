#include "common.h"
#include "game.h"
#include "dcb/open_trade.h"
#include "dcb/heap.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/player_rank.h"
#include "dcb/menu.h"
#include "dcb/sort.h"
#include "dcb/openseg.h"

typedef s32 (*CompareFunc)(s8 *, s8 *);

extern s8 OPEN_TRADABLE_COUNTS[2][301];
extern s32 OPEN_TRADE_BANNER_SHOWN;
extern s32 OPEN_TRADE_BANNER_Y;
extern s8 OPEN_DECK_CARD_COUNTS[2][301];
extern CardEntry *OPEN_TRADE_CARD_LISTS[2][301];
extern s16 OPEN_TRADE_PICKS[2][3];
extern s32 OPEN_TRADE_STATE;
extern UiWindow OPEN_TRADE_OK_WINDOW;
extern s32 OPEN_SORT_PLAYER;
extern s16 OPEN_TRADE_PICK_X[2][3];
extern u32 *OPEN_CARD_IMAGE_ARC;
extern MessageWindow OPEN_TRADE_WARNING_WINDOWS[2];
extern s32 OPEN_TRADE_PLAYER_READY[2];
extern s32 OPEN_TRADE_QUIT;
extern UiWindow OPEN_TRADE_LIST_WINDOW;
extern PlayerWindow OPEN_SORT_MENU_WINDOWS[2];
extern PlayerWindow OPEN_CARD_LIST_WINDOWS[2];
extern PlayerWindow OPEN_CARD_INFO_WINDOWS[2];
extern CursorHighlight OPEN_SORT_MENU_CURSORS[2];
extern CursorHighlight OPEN_CARD_LIST_CURSORS[2];

/* "Do you want to Quit Trading?": the string is followed by leftover bytes, so it stays as data */
extern const char OPEN_STR_QUIT_TRADING[];

void OPEN_initTradeCardList(s32 player);

/* the card list menus of both players */
Menu OPEN_CARD_LIST_MENUS[2] = {
    { NULL, NULL, { 0xA, 0x30, 0x90, 0x62 }, 0, -1, 0, -1, 0xA, 0x26, 0x78, 0xC, 1, 0x12D, 0x12, 1, 0, 0xE, 0, 0, 0 },
    { NULL, NULL, { 0xA6, 0x30, 0x90, 0x62 }, 0, -1, 0, -1, 0xA, 0x26, 0x78, 0xC, 1, 0x12D, 0x12, 1, 0, 0xE, 0, 0, 1 },
};

/* the options of the sort menu */
char *OPEN_SORT_OPTIONS[11] = {
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
};

/* the sort menus of both players */
Menu OPEN_SORT_MENUS[2] = {
    { NULL, NULL, { 0x32, 0x3C, 0x52, 0x54 }, 0, -1, 0, -1, 0xA, 0x16, 0xD8, 0xC, 1, 0xB, 0, 1, 0, 0xE, 0, 0, 0 },
    { NULL, NULL, { 0xC8, 0x3C, 0x52, 0x54 }, 0, -1, 0, -1, 0xA, 0x16, 0x48, 0xC, 1, 0xB, 0, 1, 0, 0xE, 0, 0, 1 },
};

s32 OPEN_compareByFire(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 0) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 0) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByIce(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 1) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 1) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByNature(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 2) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 2) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByDarkness(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 3) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 3) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByRare(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 4) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 4) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByOption(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    keyA = (*a)->type == 1;
    keyB = (*b)->type == 1;
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByDigivolve(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    keyA = (*a)->type == 2;
    keyB = (*b)->type == 2;
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByLevel0(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 0) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 0) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByLevel2(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 2) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 2) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 OPEN_compareByLevel3(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 3) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 3) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[OPEN_SORT_PLAYER].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

/* how each option of the sort menu compares two cards ("Number" keeps the order) */
CompareFunc OPEN_SORT_COMPARES[11] = {
    NULL,
    (CompareFunc)OPEN_compareByFire,
    (CompareFunc)OPEN_compareByIce,
    (CompareFunc)OPEN_compareByNature,
    (CompareFunc)OPEN_compareByDarkness,
    (CompareFunc)OPEN_compareByRare,
    (CompareFunc)OPEN_compareByOption,
    (CompareFunc)OPEN_compareByDigivolve,
    (CompareFunc)OPEN_compareByLevel0,
    (CompareFunc)OPEN_compareByLevel2,
    (CompareFunc)OPEN_compareByLevel3,
};

void OPEN_drawSortMenu(PlayerWindow *window) {
    char text[72]; /* unused, but it is in the original stack frame */
    Menu *menu;
    s32 player;
    s32 i;
    s32 x;
    s32 z;

    player = window->player;
    x = window->window.originX;
    z = window->window.z;
    for (i = 0; i < OPEN_SORT_MENUS[player].nrows; i++) {
        if (i < window->window.view.y / OPEN_SORT_MENUS[player].rowH) {
            continue;
        }
        if ((window->window.view.y + window->window.rect.h) / OPEN_SORT_MENUS[player].rowH < i) {
            break;
        }
        drawText(x, window->window.originY + i * OPEN_SORT_MENUS[player].rowH + 1, (s32)OPEN_SORT_OPTIONS[i], 7, z);
    }
    menu = &OPEN_SORT_MENUS[player];
    updateMenuCursor(menu);
    if (menu->active && (PAD_STATES[player]->pressed & 0x40)) {
        playMenuSound(1);
        if (OPEN_SORT_COMPARES[menu->row] != NULL) {
            OPEN_SORT_PLAYER = player;
            sortArray((s8 *)OPEN_TRADE_CARD_LISTS[player], 301, 4, OPEN_SORT_COMPARES[menu->row]);
        } else {
            OPEN_initTradeCardList(player);
        }
        OPEN_CARD_LIST_MENUS[player].row = 0;
        centerMenuOnCursor(&OPEN_CARD_LIST_MENUS[player]);
    }
}

void OPEN_drawTradeBanner(void) {
    if (isSpritePoolFull() == 0) {
        if (OPEN_TRADE_BANNER_SHOWN != 0) {
            OPEN_TRADE_BANNER_Y += 4;
            if (OPEN_TRADE_BANNER_Y > 8) {
                OPEN_TRADE_BANNER_Y = 8;
            }
        } else {
            OPEN_TRADE_BANNER_Y -= 4;
            if (OPEN_TRADE_BANNER_Y < -32) {
                OPEN_TRADE_BANNER_Y = -32;
            }
        }
        CUR_SPRT->sp.x0 = 6;
        CUR_SPRT->sp.y0 = OPEN_TRADE_BANNER_Y;
        CUR_SPRT->sp.u0 = 0x58;
        CUR_SPRT->sp.v0 = 0x4A;
        CUR_SPRT->sp.clut = 0x7FB8;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x20;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

s32 OPEN_countSharedCardCopies(s32 player, s32 card) {
    s32 other;
    s32 otherCount;
    s32 count;
    s32 i;
    s32 j;
    s32 shared;

    other = player ^ 1;
    otherCount = getOwnedCardCount(other, card);
    count = getOwnedCardCount(player, card);
    shared = 0;
    for (i = 0; i < count; i++) {
        for (j = 0; j < otherCount; j++) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i] == ((PlayerProfile *)PLAYER_PROFILES)[other].cardCopySerials[card][j]) {
                shared++;
                break;
            }
        }
    }
    return shared;
}

s32 OPEN_findUnsharedCardSerial(s32 player, s32 card) {
    s32 other;
    s32 otherCount;
    s32 count;
    s32 i;
    s32 j;

    other = player ^ 1;
    otherCount = getOwnedCardCount(other, card);
    count = getOwnedCardCount(player, card);
    for (i = 0; i < count; i++) {
        for (j = 0; j < otherCount; j++) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i] != ((PlayerProfile *)PLAYER_PROFILES)[other].cardCopySerials[card][j]) {
                return ((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i];
            }
        }
    }
    return ((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][0];
}

void OPEN_addCardCopy(s32 player, s32 card, s32 serial) {
    PLAYER_DATA(player).cardCopySerials[card][getOwnedCardCount(player, card)] = serial;
    if (PLAYER_DATA(player).cardCollection[card] == 0) {
        PLAYER_DATA(player).cardCollection[card] |= 0x20;
    }
    PLAYER_DATA(player).cardCollection[card]++;
    if ((PLAYER_DATA(player).cardCollection[card] & 7) == 6) {
        PLAYER_DATA(player).cardCollection[card] |= 0x10;
    }
    if (((u8 *)getCardData(card))[0x19] == 0) {
        PLAYER_DATA(player).cardCollection[card] |= 0x10;
    }
    PLAYER_DATA(player).cardCollection[card] |= 0xC8;
    updatePlayerRanks(player);
    if ((u16)++PLAYER_DATA(player).cardsReceived >= 10000) {
        PLAYER_DATA(player).cardsReceived = 9999;
    }
}

s32 OPEN_removeCardCopy(s32 player, s32 card) {
    s32 count;
    s32 serial;
    s32 i;
    s32 j;

    count = getOwnedCardCount(player, card);
    serial = OPEN_findUnsharedCardSerial(player, card);
    for (i = 0; i < count; i++) {
        if (PLAYER_DATA(player).cardCopySerials[card][i] == serial) {
            for (j = i; j < count - 1; j++) {
                PLAYER_DATA(player).cardCopySerials[card][j] = PLAYER_DATA(player).cardCopySerials[card][j + 1];
            }
            break;
        }
    }
    PLAYER_DATA(player).cardCollection[card]--;
    updatePlayerRanks(player);
    if ((u16)++PLAYER_DATA(player).cardsGivenAway >= 10000) {
        PLAYER_DATA(player).cardsGivenAway = 9999;
    }
    return serial;
}

void OPEN_countCardsInDecks(void) {
    s8 counts[3][301];
    s32 player;
    s32 i;
    s32 j;
    s32 max;

    for (player = 0; player < 2; player++) {
        for (i = 0; i < 301; i++) {
            OPEN_DECK_CARD_COUNTS[player][i] = 0;
            for (j = 0; j < 3; j++) {
                counts[j][i] = 0;
            }
        }
        for (i = 0; i < 3; i++) {
            if (PLAYER_DATA(player).savedDecks[i].inUse != 0) {
                for (j = 0; j < 30; j++) {
                    counts[i][PLAYER_DATA(player).savedDecks[i].cards[j].id]++;
                }
            }
        }
        for (i = 0; i < 301; i++) {
            max = 0;
            for (j = 0; j < 3; j++) {
                if (max < counts[j][i]) {
                    max = counts[j][i];
                }
            }
            OPEN_DECK_CARD_COUNTS[player][i] = max;
        }
    }
}

void OPEN_initTradeCardList(s32 player) {
    s32 n;
    s32 i;
    s32 shared;

    n = 0;
    for (i = 0; i < 0xBF; i++) {
        OPEN_TRADE_CARD_LISTS[player][n++] = (CardEntry *)&((DigimonCardData *)DIGIMON_CARDS)[i];
    }
    for (i = 0; i < 0x66; i++) {
        OPEN_TRADE_CARD_LISTS[player][n++] = (CardEntry *)&((OptionCardData *)OPTION_CARDS)[i];
    }
    for (i = 0; i < 8; i++) {
        OPEN_TRADE_CARD_LISTS[player][n++] = (CardEntry *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[i];
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[i].cardId != 0) {
            OPEN_TRADE_CARD_LISTS[player][PLAYER_DATA(player).partners[i].cardId] = (CardEntry *)&PLAYER_DATA(player).partners[i];
        }
    }
    for (i = 0; i < 301; i++) {
        shared = OPEN_countSharedCardCopies(player, i);
        OPEN_TRADABLE_COUNTS[player][i] = getOwnedCardCount(player, i) - OPEN_DECK_CARD_COUNTS[player][i];
        if (OPEN_TRADABLE_COUNTS[player][i] > getOwnedCardCount(player, i) - shared) {
            OPEN_TRADABLE_COUNTS[player][i] = getOwnedCardCount(player, i) - shared;
        }
        if (OPEN_TRADABLE_COUNTS[player][i] < 0) {
            OPEN_TRADABLE_COUNTS[player][i] = 0;
        }
        if (OPEN_TRADABLE_COUNTS[player][i] + getOwnedCardCount(player ^ 1, i) >= 7) {
            OPEN_TRADABLE_COUNTS[player][i] = 6 - getOwnedCardCount(player ^ 1, i);
        }
    }
}

void OPEN_drawTradeList(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 done;
    s32 i;
    s32 row;
    s32 col;
    s32 card;
    s32 specialty;

    x = window->originX;
    y = window->originY;
    z = window->z;
    done = 0;
    if (OPEN_TRADE_STATE != 0) {
        for (i = 0; i < 3; i++) {
            if (OPEN_TRADE_PICK_X[0][i] < i * 44 + 0xA6) {
                OPEN_TRADE_PICK_X[0][i] += 8;
            } else {
                OPEN_TRADE_PICK_X[0][i] = i * 44 + 0xA6;
                done++;
            }
        }
        for (i = 0; i < 3; i++) {
            if (OPEN_TRADE_PICK_X[1][i] > i * 44) {
                OPEN_TRADE_PICK_X[1][i] -= 8;
            } else {
                OPEN_TRADE_PICK_X[1][i] = i * 44;
                done++;
            }
        }
    }
    if (done == 6) {
        CUR_SPRT->sp.x0 = x + 0x54;
        CUR_SPRT->sp.y0 = y + 6;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0;
        CUR_SPRT->sp.clut = 0x7E78;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x21;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        OPEN_TRADE_STATE = 2;
    }
    for (row = 0; row < 2; row++) {
        for (col = 0; col < 3; col++) {
            if (OPEN_TRADE_PICKS[row][col] == -1) {
                continue;
            }
            card = OPEN_TRADE_PICKS[row][col];
            specialty = getCardSpecialty(card);
            if (specialty == 6) {
                specialty = 5;
            }
            uploadTim((u32 *)((u8 *)OPEN_CARD_IMAGE_ARC + OPEN_CARD_IMAGE_ARC[card]), col * 21 + 0x2C0, row * 41, 0x2C0, 0xFF - (row * 3 + col));
            if (isSpritePoolFull()) {
                return;
            }
            CUR_SPRT->sp.x0 = x + OPEN_TRADE_PICK_X[row][col];
            CUR_SPRT->sp.y0 = y + 3;
            CUR_SPRT->sp.u0 = col * 21 * 2;
            CUR_SPRT->sp.v0 = row * 41;
            CUR_SPRT->sp.clut = getClut(0x2C0, 0xFF - (row * 3 + col));
            CUR_SPRT->sp.w = 0x28;
            CUR_SPRT->sp.h = 0x28;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, 0x2C0, 0));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull()) {
                return;
            }
            CUR_SPRT->sp.x0 = x + OPEN_TRADE_PICK_X[row][col];
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x52;
            CUR_SPRT->sp.clut = getClut(0x2C0, specialty + 0xF0);
            CUR_SPRT->sp.w = 0x28;
            CUR_SPRT->sp.h = 0x30;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, 0x2C0, 0));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
    CUR_SPRT->sp.x0 = x + 0x84;
    CUR_SPRT->sp.y0 = y + 3;
    CUR_SPRT->sp.u0 = 0x80;
    CUR_SPRT->sp.v0 = 0x20;
    CUR_SPRT->sp.clut = 0x7EF8;
    CUR_SPRT->sp.w = 0x20;
    CUR_SPRT->sp.h = 0x29;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
}

void OPEN_drawTradeOk(UiWindow *window) {
    s32 held[2];
    s32 offset;
    char text[64];
    s32 count;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    count = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (OPEN_TRADE_PICKS[i][j] != -1) {
                count++;
            }
        }
    }
    if (count == 0) {
        strcpy(text, "You can't Trade between 0 Cards!");
        offset = (0x100 - measureText(text)) / 2;
        drawText(x + offset, y + 14, (s32)text, 7, z);
        return;
    }
    strcpy(text, "If these Cards are OK,\npress and hold *b2 Button.");
    offset = (0x100 - measureText(text)) / 2;
    drawText(x + offset, y + 8, (s32)text, 7, z);
    for (i = 0; i < 2; i++) {
        if ((PAD_STATES[i]->held & 0x40) || OPEN_TRADE_STATE != 0) {
            held[i] = 1;
            CUR_SPRT->sp.x0 = i * 0xD6 + x - 2;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0xA0;
            CUR_SPRT->sp.v0 = 0x20;
            CUR_SPRT->sp.clut = 0x7F78;
            CUR_SPRT->sp.w = 0x2C;
            CUR_SPRT->sp.h = 0x2A;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x3E);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        } else {
            held[i] = 0;
        }
        CUR_SPRT->sp.x0 = x + 10 + i * 0xD6;
        CUR_SPRT->sp.y0 = y + 10;
        CUR_SPRT->sp.u0 = 0xCC;
        CUR_SPRT->sp.v0 = 0x20;
        CUR_SPRT->sp.clut = 0x7F38;
        CUR_SPRT->sp.w = 0x14;
        CUR_SPRT->sp.h = 0x1D;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
    if (held[0] && held[1] && OPEN_TRADE_STATE == 0) {
        animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
        OPEN_TRADE_STATE = 1;
        playMenuSound(1);
    }
}

void OPEN_drawTradeWarning(MessageWindow *window) {
    char text[64];
    s32 port;
    s32 x;
    s32 y;
    s32 z;

    port = window->port;
    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    switch (window->message) {
    case 1:
        strcpy(text, "You can't trade\nthis Card!");
        break;
    case 2:
        strcpy(text, "You can't transfer\nany more Cards!");
        break;
    case 3:
        strcpy(text, "This Card is\nunidentified!");
        break;
    }
    drawText(x + (0x5E - measureText(text)) / 2, y, (s32)text, 7, z);
    if (PAD_STATES[port]->pressed & 0x50) {
        animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[port].window, (Rect16 *)-1);
        (&OPEN_CARD_LIST_MENUS[port])->active = 1;
        playMenuSound(4);
    }
}

void OPEN_drawCardList(PlayerWindow *window) {
    char text[72];
    u8 rgb[3];
    Rect16 rect;
    Rect16 rect2;
    s32 full;
    Menu *menu;
    UiWindow *win;
    s32 player;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 card;
    s32 palette;
    s32 count;
    s32 shade;
    s32 top;

    win = &window->window;
    player = window->player;
    x = win->originX;
    z = win->z;
    full = 0;
    if (OPEN_TRADE_PLAYER_READY[player] != 0) {
        CUR_SPRT->sp.x0 = player * 0x9C + 0x12;
        CUR_SPRT->sp.y0 = 0x4B;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x21;
        CUR_SPRT->sp.clut = 0x7E38;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x21;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        shade = 0x40;
    } else {
        shade = 0x80;
    }
    rgb[0] = shade;
    rgb[1] = shade;
    rgb[2] = shade;
    for (i = 0; i < OPEN_CARD_LIST_MENUS[player].nrows; i++) {
        if (i < win->view.y / OPEN_CARD_LIST_MENUS[player].rowH) {
            continue;
        }
        if ((win->view.y + win->rect.h) / OPEN_CARD_LIST_MENUS[player].rowH < i) {
            break;
        }
#if VERSION_EU
        top = win->originY + i * OPEN_CARD_LIST_MENUS[player].rowH;
        y = top + 1;
#elif VERSION_US
        y = win->originY + i * OPEN_CARD_LIST_MENUS[player].rowH + 1;
#else
#error "openseg/friend/open_trade: version not checked"
#endif
        card = OPEN_TRADE_CARD_LISTS[player][i]->id;
        palette = 8;
        if (OPEN_TRADABLE_COUNTS[player][card] > 0) {
            palette = 7;
        }
        if (OPEN_TRADE_CARD_LISTS[player][i]->unk18 == 0) {
            palette = 3;
        }
        if (PLAYER_DATA(player).cardCollection[OPEN_TRADE_CARD_LISTS[player][i]->id] & 0x40) {
            drawTextColored(x + 0x12, y, OPEN_TRADE_CARD_LISTS[player][i]->name, rgb, palette, z);
        } else {
            palette = 9;
            drawTextColored(x + 0x12, y, "-----------------", rgb, 9, z);
        }
        sprintf(text, "%3.3d", OPEN_TRADE_CARD_LISTS[player][i]->id);
        drawSmallTextColored(x, y + 6, text, palette, rgb, z);
        sprintf(text, "%d", OPEN_TRADABLE_COUNTS[player][card]);
        drawTextColored(x + 0x7E, y, text, rgb, palette, z);
    }
    menu = &OPEN_CARD_LIST_MENUS[player];
    updateMenuCursor(menu);
    if (menu->active) {
        card = OPEN_TRADE_CARD_LISTS[player][menu->row]->id;
        if (PAD_STATES[player]->pressed & 0x40) {
            if (PLAYER_DATA(player).cardCollection[card] & 0x40) {
                if (OPEN_TRADE_CARD_LISTS[player][menu->row]->unk18 == 0) {
                    rect.x = player * 0x9C + 0x21;
                    rect.y = 0x5A;
                    rect.w = 0x5E;
                    rect.h = 0x18;
                    animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[player].window, &rect);
                    OPEN_TRADE_WARNING_WINDOWS[player].message = 1;
                    menu->active = 0;
                    playMenuSound(3);
                } else if (OPEN_TRADABLE_COUNTS[player][card] == 0) {
                    rect2.x = player * 0x9C + 0x21;
                    rect2.y = 0x5A;
                    rect2.w = 0x5E;
                    rect2.h = 0x18;
                    animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[player].window, &rect2);
                    OPEN_TRADE_WARNING_WINDOWS[player].message = 2;
                    OPEN_CARD_LIST_MENUS[player].active = 0;
                    playMenuSound(3);
                } else {
                    for (i = 0; i < 3; i++) {
                        if (OPEN_TRADE_PICKS[player][i] == -1) {
                            OPEN_TRADE_PICKS[player][i] = card;
                            OPEN_TRADABLE_COUNTS[player][card]--;
                            if (i == 2) {
                                full = 1;
                            } else {
                                playMenuSound(1);
                            }
                            break;
                        }
                    }
                    if (i == 3) {
                        full = 1;
                    }
                }
            } else {
                rect.x = player * 0x9C + 0x21;
                rect.y = 0x5A;
                rect.w = 0x5E;
                rect.h = 0x18;
                animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[player].window, &rect);
                OPEN_TRADE_WARNING_WINDOWS[player].message = 3;
                OPEN_CARD_LIST_MENUS[player].active = 0;
                playMenuSound(3);
            }
        }
        if (PAD_STATES[player]->pressed & 0x10) {
            playMenuSound(0);
            for (i = 0, count = 0; i < 3; i++) {
                if (OPEN_TRADE_PICKS[player][i] != -1) {
                    count++;
                }
            }
            if (count == 0) {
                OPEN_TRADE_QUIT = player + 2;
                return;
            }
            for (i = 2; i >= 0; i--) {
                if (OPEN_TRADE_PICKS[player][i] != -1) {
                    OPEN_TRADABLE_COUNTS[player][OPEN_TRADE_PICKS[player][i]]++;
                    OPEN_TRADE_PICKS[player][i] = -1;
                    break;
                }
            }
        }
        if ((PAD_STATES[player]->pressed & 0x800) || full) {
            OPEN_CARD_LIST_MENUS[player].active = 0;
            playMenuSound(1);
            OPEN_TRADE_PLAYER_READY[player] = 1;
            if (OPEN_TRADE_PLAYER_READY[0] != 0 && OPEN_TRADE_PLAYER_READY[1] != 0) {
                rect.x = 0x20;
                rect.y = 0x78;
                rect.w = 0x100;
                rect.h = 0x28;
                animateWindowTo(&OPEN_TRADE_OK_WINDOW, &rect);
            }
        }
    } else if (OPEN_TRADE_PLAYER_READY[player] != 0 && OPEN_TRADE_STATE == 0 && (PAD_STATES[player]->pressed & 0x10)) {
        playMenuSound(0);
        OPEN_CARD_LIST_MENUS[player].active = 1;
        OPEN_TRADE_PLAYER_READY[player] = 0;
        animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
    }
}

void OPEN_drawCardInfo(PlayerWindow *window) {
    char text[72];
    s32 player;
    s32 row;
    s32 x;
    s32 y;
    s32 z;

    player = window->player;
    x = window->window.originX + 1;
    y = window->window.originY + 1;
    z = window->window.z;
    row = OPEN_CARD_LIST_MENUS[player].row;
    if (PLAYER_DATA(player).cardCollection[OPEN_TRADE_CARD_LISTS[player][row]->id] & 0x40) {
        switch (OPEN_TRADE_CARD_LISTS[player][row]->type) {
        case 0:
            drawIcon(x, y, 0, OPEN_TRADE_CARD_LISTS[player][row]->attr >> 4, z);
            drawIcon(x + 14, y, 0, (OPEN_TRADE_CARD_LISTS[player][row]->attr & 0xF) + 16, z);
            break;
        case 1:
            drawIcon(x, y, 0, 5, z);
            break;
        case 2:
            drawIcon(x, y, 0, 6, z);
            break;
        }
        sprintf(text, "in Stock. \f\a%d\f\x06 Cards", getOwnedCardCount(player, OPEN_TRADE_CARD_LISTS[player][row]->id));
        drawSmallText(x + 0x22, y, (s32)text, 6, z);
        sprintf(text, "in a Deck. \f\a%d\f\x06 Cards", OPEN_DECK_CARD_COUNTS[player][OPEN_TRADE_CARD_LISTS[player][row]->id]);
        drawSmallText(x + 0x31, y + 6, (s32)text, 6, z);
    } else {
        strcpy(text, "Unidentified Card");
        drawText(x + (0x90 - measureText(text)) / 2, y, (s32)text, 7, z);
    }
}

/* the labels of the trade windows; GCC keeps one copy of each, emitted
   with OPEN_drawTradeScreen, the first function that uses them */
#define OPEN_STR_WARNING "WARNING"
#define OPEN_STR_TRADE_OK "TRADE OK?"

void OPEN_drawTradeScreen(void) {
    s32 count;
    s32 i;
    s32 j;

    OPEN_drawTradeBanner();
    count = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (OPEN_TRADE_PICKS[i][j] != -1) {
                count++;
            }
        }
    }
    if (count == 0) {
        OPEN_TRADE_OK_WINDOW.label = (s32)OPEN_STR_WARNING;
        OPEN_TRADE_OK_WINDOW.palette = 2;
    } else {
        OPEN_TRADE_OK_WINDOW.label = (s32)OPEN_STR_TRADE_OK;
        OPEN_TRADE_OK_WINDOW.palette = 1;
    }
    drawWindow(&OPEN_TRADE_OK_WINDOW, OPEN_drawTradeOk, 1);
    drawWindow(&OPEN_TRADE_LIST_WINDOW, OPEN_drawTradeList, 1);
    for (i = 0; i < 2; i++) {
        drawWindow(&OPEN_SORT_MENU_WINDOWS[i].window, OPEN_drawSortMenu, 1);
        drawWindow(&OPEN_CARD_LIST_WINDOWS[i].window, OPEN_drawCardList, 2);
        drawWindow(&OPEN_CARD_INFO_WINDOWS[i].window, OPEN_drawCardInfo, 2);
        drawWindow(&OPEN_TRADE_WARNING_WINDOWS[i].window, OPEN_drawTradeWarning, 1);
    }
}

#if VERSION_US || VERSION_EU
void OPEN_runCardTrade(s32 parentTask) {
    s32 open[2];
    Rect16 rect;
    u8 dialog[0xB8];
    s32 i;
    s32 j;
    s16 card;
    u32 *arc;

    OPEN_TRADE_BANNER_SHOWN = 1;
    OPEN_TRADE_BANNER_Y = -32;
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\TRADE.ARC", getCurrentTaskId());
    OPEN_CARD_IMAGE_ARC = (u32 *)waitFrames(0x7FFFFFFF);
    for (i = 0; i < (s32)(OPEN_CARD_IMAGE_ARC[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)OPEN_CARD_IMAGE_ARC + OPEN_CARD_IMAGE_ARC[i]), -1, -1, -1, -1);
        waitFrames(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(OPEN_CARD_IMAGE_ARC);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\CARD_F.TIM", getCurrentTaskId());
    OPEN_CARD_IMAGE_ARC = (u32 *)waitFrames(0x7FFFFFFF);
    uploadTim(OPEN_CARD_IMAGE_ARC, 0x2C0, 0x52, 0x2C0, 0xF0);
    DrawSync(0);
    waitFrames(FRAME_INTERVAL);
    freeHeapBlock(OPEN_CARD_IMAGE_ARC);
    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    OPEN_CARD_IMAGE_ARC = (u32 *)waitFrames(0x7FFFFFFF);
    rect.x = 0x20;
    rect.y = 0x78;
    rect.w = 0x100;
    rect.h = 0x28;
    openWindow(&OPEN_TRADE_OK_WINDOW, &rect, -1, (s16 *)-1, 8, 0x25, 0x80, 0xC);
    animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
    OPEN_TRADE_OK_WINDOW.label = (s32)OPEN_STR_TRADE_OK;
    rect.x = 0xC;
    rect.y = 0xB6;
    rect.w = 0x128;
    rect.h = 0x2E;
    openWindow(&OPEN_TRADE_LIST_WINDOW, &rect, -1, (s16 *)-1, 8, 0x25, 0x80, 0xC);
    OPEN_TRADE_LIST_WINDOW.label = (s32)"TRADE LIST";
    for (i = 0; i < 2; i++) {
        openMenu(&OPEN_SORT_MENUS[i], &OPEN_SORT_MENU_WINDOWS[i].window, &OPEN_SORT_MENU_CURSORS[i], (Bytes4 *)-1);
        animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, (Rect16 *)-1);
        OPEN_SORT_MENU_WINDOWS[i].window.label = (s32)"SORT MENU";
        OPEN_SORT_MENU_WINDOWS[i].player = i;
        rect.x = i * 0x9C + 0x21;
        rect.y = 0x5A;
        rect.w = 0x5E;
        rect.h = 0x18;
        openWindow(&OPEN_TRADE_WARNING_WINDOWS[i].window, &rect, -1, (s16 *)-1, 8, 0x11, 0x80, 0xC);
        animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[i].window, (Rect16 *)-1);
        OPEN_TRADE_WARNING_WINDOWS[i].window.label = (s32)OPEN_STR_WARNING;
        OPEN_TRADE_WARNING_WINDOWS[i].window.palette = 2;
        OPEN_TRADE_WARNING_WINDOWS[i].port = i;
        rect.x = i * 0x9C + 0xA;
        rect.y = 0x9A;
        rect.w = 0x90;
        rect.h = 0xE;
        openWindow(&OPEN_CARD_INFO_WINDOWS[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 0xC);
        OPEN_CARD_INFO_WINDOWS[i].player = i;
        openMenu(&OPEN_CARD_LIST_MENUS[i], &OPEN_CARD_LIST_WINDOWS[i].window, &OPEN_CARD_LIST_CURSORS[i], (Bytes4 *)-1);
        if (i == 0) {
            OPEN_CARD_LIST_WINDOWS[i].window.label = (s32)"1P CARD LIST";
        } else {
            OPEN_CARD_LIST_WINDOWS[i].window.label = (s32)"2P CARD LIST";
        }
        OPEN_CARD_LIST_WINDOWS[i].player = i;
        OPEN_TRADE_PLAYER_READY[i] = 0;
        for (j = 0; j < 3; j++) {
            OPEN_TRADE_PICKS[i][j] = -1;
            OPEN_TRADE_PICK_X[i][j] = j * 44 + i * 166;
        }
        open[i] = 0;
    }
    OPEN_countCardsInDecks();
    for (i = 0; i < 2; i++) {
        OPEN_initTradeCardList(i);
        OPEN_CARD_LIST_MENUS[i].row = 0;
        centerMenuOnCursor(&OPEN_CARD_LIST_MENUS[i]);
    }
    OPEN_TRADE_STATE = 0;
    OPEN_TRADE_QUIT = 0;
    playMenuSound(3);
    addFrameCallback((s32)OPEN_drawTradeScreen);
    do {
        /* fake match: arc exists only for loop.c. This dead store is moved
           out of the loop (flow deletes it later), and the extra move lowers
           loop.c's threshold, so eu keeps &open inside the loop as the
           original does. The use after the loop keeps cse from deleting it. */
        arc = 0;
        waitFrames(FRAME_INTERVAL);
        for (i = 0; i < 2; i++) {
            if (OPEN_TRADE_PLAYER_READY[i] == 0) {
                if (open[i]) {
                    if (PAD_STATES[i]->pressed & 0x110) {
                        open[i] = 0;
                        playMenuSound(4);
                        OPEN_CARD_LIST_MENUS[i].active = 1;
                        animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, (Rect16 *)-1);
                    }
                } else if (PAD_STATES[i]->pressed & 0x100) {
                    open[i] = 1;
                    playMenuSound(3);
                    OPEN_CARD_LIST_MENUS[i].active = 0;
                    animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, &OPEN_SORT_MENUS[i].rect);
                }
            }
        }
        if (OPEN_TRADE_STATE == 2 && ((PAD_STATES[0]->pressed & 0x40) || (PAD_STATES[1]->pressed & 0x40))) {
            playMenuSound(1);
            PLAYER_DATA(0).hasTraded = 1;
            PLAYER_DATA(1).hasTraded = 1;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    card = OPEN_TRADE_PICKS[i][j];
                    if (card >= 0) {
                        OPEN_addCardCopy(i ^ 1, card, OPEN_removeCardCopy(i, card));
                    }
                }
            }
            OPEN_countCardsInDecks();
            for (i = 0; i < 2; i++) {
                OPEN_initTradeCardList(i);
                OPEN_CARD_LIST_MENUS[i].row = 0;
                centerMenuOnCursor(&OPEN_CARD_LIST_MENUS[i]);
            }
            OPEN_TRADE_STATE = 0;
            for (i = 0; i < 2; i++) {
                OPEN_CARD_LIST_MENUS[i].active = 1;
                OPEN_TRADE_PLAYER_READY[i] = 0;
                for (j = 0; j < 3; j++) {
                    OPEN_TRADE_PICKS[i][j] = -1;
                    OPEN_TRADE_PICK_X[i][j] = j * 44 + i * 166;
                }
            }
        }
        if (OPEN_TRADE_QUIT >= 2) {
            initDialog(dialog, OPEN_STR_QUIT_TRADING, 1);
            runDialogForPad((s32 *)dialog, OPEN_TRADE_QUIT - 2);
            switch ((s8)dialog[0xA5]) {
            case 1:
                OPEN_TRADE_QUIT = 1;
                break;
            case 0:
            case 2:
                OPEN_TRADE_QUIT = 0;
                break;
            }
        }
    } while (OPEN_TRADE_QUIT != 1);
    animateWindowTo(&OPEN_TRADE_OK_WINDOW, (Rect16 *)-1);
    animateWindowTo(&OPEN_TRADE_LIST_WINDOW, (Rect16 *)-1);
    for (i = 0; i < 2; i++) {
        animateWindowTo(&OPEN_SORT_MENU_WINDOWS[i].window, (Rect16 *)-1);
        animateWindowTo(&OPEN_CARD_LIST_WINDOWS[i].window, (Rect16 *)-1);
        animateWindowTo(&OPEN_CARD_INFO_WINDOWS[i].window, (Rect16 *)-1);
        animateWindowTo(&OPEN_TRADE_WARNING_WINDOWS[i].window, (Rect16 *)-1);
    }
    playMenuSound(4);
    OPEN_TRADE_BANNER_SHOWN = 0;
    waitFrames(20);
    removeFrameCallback((s32)OPEN_drawTradeScreen);
    waitFrames(2);
    arc = OPEN_CARD_IMAGE_ARC; /* fake match: see arc = 0 above */
    freeHeapBlock(arc);
    resumeTask(parentTask);
}
#else
#error "openseg/friend/open_trade: version not checked"
#endif

/* the last three bytes are leftovers in the original, not zero padding, and
   not the same in every version */
#if VERSION_US
const char OPEN_STR_QUIT_TRADING[32] = "Do you want to Quit Trading?\0\x10\x02\x02";
#elif VERSION_EU
const char OPEN_STR_QUIT_TRADING[32] = "Do you want to Quit Trading?\0\0\0\x94";
#else
#error "openseg/friend/open_trade: version not checked"
#endif
