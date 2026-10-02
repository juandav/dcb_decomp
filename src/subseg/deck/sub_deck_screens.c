#include "common.h"
#include "game.h"
#include "dcb/sub_deck_screens.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/vram_upload.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"
#include "dcb/battle_hud.h"
#include "dcb/prim.h"
#include "dcb/frame_callback.h"
#include "dcb/dialog.h"
#include "dcb/subseg.h"
#include "dcb/sub_name_entry.h"
#include "dcb/sub_partner.h"
#include "dcb/sub_sort.h"
#include "dcb/sub_deck_editor.h"

extern u8 SUB_EDITOR_BLINK;
extern s8 *SUB_DECK_EDIT_CARD;
extern s16 SUB_DECK_EDIT_CARD_ID;
extern DeckCardCounts *SUB_DECK_CARD_COUNTS;
extern s16 SUB_COLLECTION_SHOW_INFO;
extern UiWindow SUB_CARD_SORT_WINDOW;
extern UiWindow SUB_DECK_SORT_WINDOW;
extern UiWindow SUB_PICKER_WINDOW;
extern UiWindow SUB_LIST_WINDOWS[3];
extern UiWindow SUB_STATS_WINDOW;
extern UiWindow SUB_GRID_WINDOW;
extern UiWindow SUB_CARD_INFO_WINDOW;
extern UiWindow SUB_CARD_DATA_WINDOW;
extern PlayerDeck SUB_ORIGINAL_DECK;
extern u16 SUB_COLLECTION_COUNTS[8];
extern u8 SUB_CARD_LIST_MENU_ACTIVE;
extern Rect16 SUB_CARD_SORT_MENU_RECT;
extern char SUB_LABEL_BUFFER[];
extern s8 *SUB_COLLECTION_SELECTED_CARD;
extern s8 SUB_DECK_IS_NEW;
extern const char SUB_FMT_2_DIGITS[];
extern const char SUB_STR_WINS[];
extern const char SUB_STR_LOSSES[];
extern const char SUB_STR_SUPPORT_EFFECT[];
extern s16 SUB_DECK_MENU_SLOT;
extern s8 SUB_EDITOR_HIDDEN;
extern u8 SUB_EDITOR_USE_DECK_COUNTS;
extern s8 SUB_DECK_EDIT_MODE;
extern s32 SUB_PICKER_WINDOW_LABEL;
extern CursorHighlight SUB_CARD_LIST_CURSOR;
extern CursorHighlight SUB_CARD_SORT_CURSOR;
extern CursorHighlight SUB_DECK_SORT_CURSOR;
extern u8 SUB_EDITOR_RUNNING;
extern s32 SUB_LIST_WINDOW_LABEL;

extern void SUB_drawDeckEditTitle(UiWindow *window);
extern void SUB_drawSortHint(UiWindow *window);
extern void SUB_drawPickerList();
extern void SUB_drawPickerCardInfo();
extern void SUB_drawDeckStats();
extern void SUB_drawDeckGrid();
extern void SUB_drawSlotCardInfo();
extern void SUB_runDeckMenu();
extern void SUB_drawCardData();
extern void SUB_drawCardCountPage();
extern void SUB_drawCardListHelp(UiWindow *window);
extern void SUB_drawCollectionTotals(UiWindow *window);
extern void SUB_drawSpecialtyCounts();

void SUB_buildCardCountLists(void) {
    CardCount *fire = SUB_COLLECTION_STATS.lists[0];
    CardCount *ice = SUB_COLLECTION_STATS.lists[1];
    CardCount *nature = SUB_COLLECTION_STATS.lists[2];
    CardCount *darkness = SUB_COLLECTION_STATS.lists[3];
    CardCount *rare = SUB_COLLECTION_STATS.lists[4];
    CardCount *options1 = SUB_COLLECTION_STATS.lists[5];
    CardCount *options2 = SUB_COLLECTION_STATS.lists[6];
    CardCount *options3 = SUB_COLLECTION_STATS.lists[7];
    s32 i;

    for (i = 0; i < 0xBF; i++) {
        switch (((DigimonCardData *)DIGIMON_CARDS)[i].attr >> 4) {
        case 0:
            fire->id = i;
            fire->count = getOwnedCardCount(SUB_EDITOR.player, i);
            fire++;
            break;
        case 1:
            ice->id = i;
            ice->count = getOwnedCardCount(SUB_EDITOR.player, i);
            ice++;
            break;
        case 2:
            nature->id = i;
            nature->count = getOwnedCardCount(SUB_EDITOR.player, i);
            nature++;
            break;
        case 3:
            darkness->id = i;
            darkness->count = getOwnedCardCount(SUB_EDITOR.player, i);
            darkness++;
            break;
        case 4:
            rare->id = i;
            rare->count = getOwnedCardCount(SUB_EDITOR.player, i);
            rare++;
            break;
        }
    }
    for (i = 0; i < 0x66; i++) {
        if (i + 0xBF < 0xE4) {
            options1->id = getCardId(1, i);
            options1->count = getOwnedCardCount(SUB_EDITOR.player, options1->id);
            options1++;
        } else if (i + 0xBF < 0x109) {
            options2->id = getCardId(1, i);
            options2->count = getOwnedCardCount(SUB_EDITOR.player, options2->id);
            options2++;
        } else {
            options3->id = getCardId(1, i);
            options3->count = getOwnedCardCount(SUB_EDITOR.player, options3->id);
            options3++;
        }
    }
    for (i = 0; i < 8; i++) {
        options3->id = getCardId(2, i);
        options3->count = getOwnedCardCount(SUB_EDITOR.player, options3->id);
        options3++;
    }
    fire->id = -1;
    ice->id = -1;
    nature->id = -1;
    darkness->id = -1;
    rare->id = -1;
    options1->id = -1;
    options2->id = -1;
    options3->id = -1;
}

const char SUB_STR_L1_BACK[] = "L1BACK";

const char SUB_STR_CARD_LIST[] = "CARD LIST";

const char SUB_STR_HELP[] = "HELP";

const char SUB_STR_PARTNER_TITLE[] = "PARTNER";

const char SUB_STR_CARD_DATA[] = "CARD DATA";

void SUB_countDeckCards(u8 player) {
    s32 i;
    s32 j;
    s32 id;

    for (i = 0; i < 301; i++) {
        SUB_DECK_CARD_COUNTS->spare[i] = getOwnedCardCount(player, i);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 301; j++) {
            SUB_DECK_CARD_COUNTS->inDecks[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).savedDecks[i].inUse != 0) {
            for (j = 0; j < 30; j++) {
                id = getCardId(PLAYER_DATA(player).savedDecks[i].cards[j].type, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].index);
                SUB_DECK_CARD_COUNTS->inDecks[i][id]++;
            }
        }
    }
    for (i = 0; i < 301; i++) {
        for (j = 1; j < 3; j++) {
            if (SUB_DECK_CARD_COUNTS->inDecks[0][i] < SUB_DECK_CARD_COUNTS->inDecks[j][i]) {
                SUB_DECK_CARD_COUNTS->inDecks[0][i] = SUB_DECK_CARD_COUNTS->inDecks[j][i];
            }
        }
    }
    for (i = 0; i < 301; i++) {
        SUB_DECK_CARD_COUNTS->spare[i] -= SUB_DECK_CARD_COUNTS->inDecks[0][i];
    }
}

void SUB_initCollectionStats(void) {
    s32 unused[6]; /* unused, but it is in the original stack frame */
    s32 i;
    s32 j;
    CardCount *entry;

    SUB_COLLECTION_STATS.page = 0;
    SUB_COLLECTION_STATS.unk16D = 0;
    for (i = 0; i < 8; i++) {
        SUB_COLLECTION_STATS.counts[i] = 0;
    }
    SUB_COLLECTION_STATS.totalCount = 0;
    SUB_COLLECTION_STATS.uniqueCount = 0;
    for (i = 0; i < 301; i++) {
        SUB_COLLECTION_STATS.totalCount += getOwnedCardCount(SUB_EDITOR.player, i);
        if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[i] & 0x40) {
            SUB_COLLECTION_STATS.uniqueCount++;
        }
        if (i >= 0xBF) {
            if (i >= 0x125) {
                SUB_COLLECTION_STATS.counts[6] += getOwnedCardCount(SUB_EDITOR.player, i);
            } else {
                SUB_COLLECTION_STATS.counts[5] += getOwnedCardCount(SUB_EDITOR.player, i);
            }
        }
    }
    SUB_buildCardCountLists();
    for (j = 0; j < 5; j++) {
        entry = SUB_COLLECTION_STATS.lists[j];
        for (i = 0; i < 41; i++, entry++) {
            if (entry->id >= 0) {
                SUB_COLLECTION_STATS.counts[j] += entry->count;
            } else {
                break;
            }
        }
    }
    for (i = 0; i < 7; i++) {
        SUB_COLLECTION_STATS.counts[7] += SUB_COLLECTION_STATS.counts[i];
    }
}

void SUB_drawCardListScreen(void) {
    Rect16 uv;
    s16 y;

    drawWindow(&SUB_CARD_DATA_WINDOW, SUB_drawCardData, 0);
    drawWindow(&SUB_CARD_SORT_WINDOW, SUB_drawCardSortMenu, 0);
    drawWindow(SUB_WINDOWS, SUB_drawCardCountPage, 0);
    drawWindow(SUB_LIST_WINDOWS, SUB_drawCardList, 0);
    drawWindow(&SUB_PICKER_WINDOW, SUB_drawCardListHelp, 0);
    drawWindow(&SUB_STATS_WINDOW, SUB_drawCollectionTotals, 0);
    drawWindow(&SUB_GRID_WINDOW, SUB_drawSpecialtyCounts, 0);
    if (SUB_EDITOR.hidden == 0) {
        if (++SUB_EDITOR.slide > 20) {
            SUB_EDITOR.slide = 20;
        }
    } else {
        if (--SUB_EDITOR.slide < 0) {
            SUB_EDITOR.slide = 0;
        }
    }
    y = (SUB_EDITOR.slide * 8 - (20 - SUB_EDITOR.slide) * 33) / 20;
    uv.x = 0;
    uv.y = 0x59;
    uv.w = 0x80;
    uv.h = 0x20;
    drawTexturedSprite(6, y, &uv, 0x18, 0x7E21, 30, 0x80, -1);
}

void SUB_runCardList(void) {
    Rect16 rects[7];
    Rect16 infoRect;
    const char *labels[7] = { SUB_STR_L1_BACK, SUB_STR_CARD_LIST, SUB_STR_HELP, SUB_STR_HELP, SUB_STR_PARTNER_TITLE, SUB_STR_HELP, SUB_STR_CARD_DATA };
    s16 running;
    s32 i;
    s32 action;
    s32 flags;
    s32 style;

    running = 1;
    SUB_EDITOR.useDeckCounts = 0;
    SUB_EDITOR.listShown = 0;
    SUB_initCardList();
    rects[0].x = 0xF6;
    rects[0].y = 0xBA;
    rects[0].w = 0x80;
    rects[0].h = 0x3C;
    rects[1].x = 0x41;
    rects[1].y = 0x60;
    rects[1].w = 0x122;
    rects[1].h = 0x60;
    rects[2].x = 0x5B;
    rects[2].y = 0x99;
    rects[2].w = 0xA2;
    rects[2].h = 0xC;
    rects[3].x = 0xE6;
    rects[3].y = 0x1B;
    rects[3].w = 0xA0;
    rects[3].h = 0xE;
    rects[4].x = 0x5A;
    rects[4].y = 0xC6;
    rects[4].w = 0xA0;
    rects[4].h = 0x34;
    rects[6].x = 0x9E;
    rects[6].y = 0x5E;
    rects[6].w = 0x122;
    rects[6].h = 0x56;
    SUB_CARD_LIST_MENU.rect.h = 0x60;
    SUB_CARD_LIST_MENU.ox = 0x85;
    openMenu(&SUB_CARD_LIST_MENU, SUB_LIST_WINDOWS, &SUB_CARD_LIST_CURSOR, (Bytes4 *)-1);
    SUB_LIST_WINDOW_LABEL = (s32)SUB_STR_CARD_LIST;
    SUB_CARD_LIST_MENU.active = running;
    SUB_CARD_LIST_MENU.row = 0;
    centerMenuOnCursor(&SUB_CARD_LIST_MENU);
    openMenu(&SUB_CARD_SORT_MENU, &SUB_CARD_SORT_WINDOW, &SUB_CARD_SORT_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&SUB_CARD_SORT_WINDOW, (Rect16 *)-1);
    SUB_CARD_SORT_WINDOW.label = (s32)"SORT MENU";
    for (i = 0; i < 7; i++) {
        if (i == 1 || i == 5) {
            continue;
        }
        switch (i) {
        case 0:
            flags = 8;
            style = 0x21;
            break;
        case 2:
        case 3:
            flags = 0;
            style = 0x31;
            break;
        case 4:
            flags = 8;
            style = 0x21;
            break;
        case 5:
            flags = 0;
            style = 0x41;
            break;
        case 6:
            flags = 8;
            style = 0x21;
            break;
        default:
            flags = 0;
            style = 0x21;
            break;
        }
        SUB_openCenteredWindow(&SUB_WINDOWS[i], rects[i], (s32)labels[i], flags, style);
    }
    animateWindowTo(&SUB_CARD_DATA_WINDOW, (Rect16 *)-1);
    SUB_COLLECTION_STATS.lists[8] = allocTaskHeapBlock(0x2A);
    for (i = 0; i < 8; i++) {
        SUB_COLLECTION_STATS.lists[i] = allocTaskHeapBlock(0xA4);
    }
    SUB_initCollectionStats();
    SUB_COLLECTION_STATS.showInfo = 0;
    SUB_EDITOR.slide = 0;
    SUB_EDITOR.hidden = 0;
    playMenuSound(3);
    addFrameCallback((s32)SUB_drawCardListScreen);
    do {
        waitFrames(1);
        if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x100) {
            action = 1;
        } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x40) {
            action = 2;
        } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x20) {
            action = 3;
        } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x10) {
            action = 5;
        } else {
            action = 0;
        }
        if (action == 1) {
            if (SUB_COLLECTION_STATS.showInfo == 0) {
                SUB_EDITOR.listShown ^= 1;
                if (SUB_EDITOR.listShown != 0) {
                    SUB_CARD_LIST_MENU.active = 0;
                    animateWindowTo(&SUB_CARD_SORT_WINDOW, &SUB_CARD_SORT_MENU_RECT);
                    playMenuSound(3);
                } else {
                    SUB_CARD_LIST_MENU.active = 1;
                    animateWindowTo(&SUB_CARD_SORT_WINDOW, (Rect16 *)-1);
                    playMenuSound(4);
                }
            }
        } else if (action == 2) {
            if (SUB_EDITOR.listShown == 0 && (PLAYER_DATA(SUB_EDITOR.player).cardCollection[SUB_COLLECTION_STATS.selectedId] & 0x40)) {
                SUB_COLLECTION_STATS.showInfo ^= 1;
                if (SUB_COLLECTION_STATS.showInfo != 0) {
                    playMenuSound(4);
                    SUB_CARD_LIST_MENU.active = 0;
                    infoRect.x = rects[6].x - rects[6].w / 2;
                    infoRect.y = rects[6].y - rects[6].h / 2;
                    infoRect.w = rects[6].w & ~1;
                    infoRect.h = rects[6].h & ~1;
                    animateWindowTo(&SUB_CARD_DATA_WINDOW, &infoRect);
                    animateWindowTo(SUB_LIST_WINDOWS, (Rect16 *)-1);
                } else {
                    playMenuSound(3);
                    SUB_CARD_LIST_MENU.active = 1;
                    animateWindowTo(SUB_LIST_WINDOWS, &SUB_CARD_LIST_MENU.rect);
                    animateWindowTo(&SUB_CARD_DATA_WINDOW, (Rect16 *)-1);
                }
            }
        } else if (action == 3 || action == 4) {
            if (SUB_EDITOR.listShown == 0) {
                playMenuSound(1);
                SUB_CARD_LIST_MENU.active = 0;
                running = 0;
                for (i = 0; i < 7; i++) {
                    animateWindowTo(&SUB_WINDOWS[i], (Rect16 *)-1);
                }
            }
        } else if (action == 5) {
            if (SUB_EDITOR.listShown == 1) {
                SUB_EDITOR.listShown ^= 1;
                SUB_CARD_LIST_MENU.active = 1;
                animateWindowTo(&SUB_CARD_SORT_WINDOW, (Rect16 *)-1);
                playMenuSound(4);
            } else if (SUB_COLLECTION_STATS.showInfo == 0) {
                playMenuSound(3);
                for (i = 0; i < 7; i++) {
                    if (i != 5) {
                        animateWindowTo(&SUB_WINDOWS[i], (Rect16 *)-1);
                    }
                }
                running = 0;
            } else {
                SUB_COLLECTION_STATS.showInfo = 0;
                playMenuSound(3);
                SUB_CARD_LIST_MENU.active = 1;
                animateWindowTo(SUB_LIST_WINDOWS, &SUB_CARD_LIST_MENU.rect);
                animateWindowTo(&SUB_CARD_DATA_WINDOW, (Rect16 *)-1);
            }
        }
    } while (running);
    SUB_EDITOR.hidden = -1;
    waitFrames(20);
    removeFrameCallback((s32)SUB_drawCardListScreen);
    if (action == 3) {
        spawnTask(0, -1, 0, 0x1000, SUB_runDeckMenu, 0, 0, 0, 0);
    } else if (action == 4) {
        spawnTask(0, -1, 0, 0x1000, SUB_runPartnerEquipment, SUB_EDITOR.player, 0, 0, 0);
    } else if (action == 5) {
        SUB_EDITOR.running = 0;
    }
    exitTask();
}

void SUB_drawCardCountPage(UiWindow *window) {
    char buf[72];
    u8 palettes[8] = { 2, 1, 4, 9, 6, 8, 8, 8 };
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    CardCount *entry;
    s32 i;

    if (PAD_STATES[SUB_EDITOR.player]->pressed & 4) {
        SUB_COLLECTION_STATS.page--;
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 8) {
        SUB_COLLECTION_STATS.page++;
    }
    if (SUB_COLLECTION_STATS.page >= 8) {
        SUB_COLLECTION_STATS.page = 0;
    } else if (SUB_COLLECTION_STATS.page < 0) {
        SUB_COLLECTION_STATS.page = 7;
    }
    entry = SUB_COLLECTION_STATS.lists[SUB_COLLECTION_STATS.page];
    window->palette = palettes[SUB_COLLECTION_STATS.page];
    switch (SUB_COLLECTION_STATS.page) {
    case 0:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK   FIRE    NEXT_R1");
        break;
    case 1:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK    ICE    NEXT_R1");
        break;
    case 2:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK  NATURE   NEXT_R1");
        break;
    case 3:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK DARKNESS  NEXT_R1");
        break;
    case 4:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK   RARE    NEXT_R1");
        break;
    case 5:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK OPTION1   NEXT_R1");
        break;
    case 6:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK OPTION2   NEXT_R1");
        break;
    case 7:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK OPTION3   NEXT_R1");
        break;
    default:
        sprintf(SUB_LABEL_BUFFER, "L1_BACK DARKNESS  NEXT_R1");
        break;
    }
    window->label = (s32)SUB_LABEL_BUFFER;
    for (i = 0; i < 40; i++) {
        if (entry->id < 0) {
            SUB_drawSprite(x + (i % 8) * 16, y + 1 + (i / 8) * 12, 0x7E35, 0x344, 0x1F0, 15, 11, 0, 0x80, -1, z);
        } else {
            sprintf(buf, "%3.3d", entry->id);
            drawTinyText(x + 2 + (i % 8) * 16, y + 3 + (i / 8) * 12, (s32)buf, 8, z);
            SUB_drawSprite(x + (i % 8) * 16, y + (i / 8) * 12, getClut(0x350, entry->count + 0x1F8), 0x340, 0x1F0, 15, 11, 0, 0x80, -1, z);
            entry++;
        }
    }
}

void SUB_drawCardListHelp(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawText(x, y, (s32)"*b0:Edit Decks", 7, z);
    if (SUB_COLLECTION_STATS.showInfo == 0) {
        drawText(x + 0x66, y, (s32)"*b5:Sort", 7, z);
    } else {
        drawText(x + 0x66, y, (s32)"*b5:Sort", 8, z);
    }
}

void SUB_drawCollectionTotals(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    y += 2;
    if (SUB_COLLECTION_STATS.uniqueCount >= 301) {
        drawIcon(x, y, 0, 20, z);
    }
    x += 12;
    drawTinyText(x, y, (s32)"All", 6, z);
    drawTinyText(x, y + 6, (s32)"Types", 6, z);
    sprintf(buf, "%3d", SUB_COLLECTION_STATS.uniqueCount);
    drawText(x + 26, y, (s32)buf, 7, z);
    x += 80;
    if (SUB_COLLECTION_STATS.totalCount >= 1608) {
        drawIcon(x - 24, y, 0, 20, z);
    }
    drawTinyText(x - 12, y, (s32)"Total Number", 6, z);
    drawTinyText(x - 12, y + 6, (s32)"of Cards", 6, z);
    sprintf(buf, "%4d", SUB_COLLECTION_STATS.totalCount);
    drawText(x + 38, y, (s32)buf, 7, z);
}

const char SUB_STR_CARD_INFO[] = "CARD INFO.";

const char SUB_FMT_3_DIGITS[] = "*s0%3d";

const char SUB_STR_TOTAL[] = "Total";

const char SUB_FMT_4_DIGITS[] = "*s0%4d";

/* the specialties' names in Japanese; not referenced by any code */
char *SUB_SPECIALTY_NAMES_JP[5] = {
    "\x89\xCE\x89\x8A", /* 火炎 */
    "\x95X\x90\x85", /* 氷水 */
    "\x8E\xA9\x91R", /* 自然 */
    "\x88\xC3\x8D\x95", /* 暗黒 */
    "\x92\xBF\x8E\xED", /* 珍種 */
};

void SUB_drawSpecialtyCounts(UiWindow *window) {
    char buf[144];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 i;

    window->label = (s32)SUB_STR_CARD_INFO;
    for (i = 0; i < 8; i++) {
        if (i != 7) {
            drawIcon(x + (i / 4) * 80, y + (i % 4) * 13, 0, i, z);
            sprintf(buf, SUB_FMT_3_DIGITS, SUB_COLLECTION_STATS.counts[i]);
            drawText(x + 0x10 + (i / 4) * 95, y + (i % 4) * 13, (s32)buf, 7, z);
            drawText(x + 0x25 + (i / 4) * 95, y + (i % 4) * 13, (s32)SUB_STR_CARDS, 7, z);
        } else {
            drawText(x + (i / 4) * 80 - 6, y + 0x27, (s32)SUB_STR_TOTAL, 7, z);
            sprintf(buf, SUB_FMT_4_DIGITS, SUB_COLLECTION_STATS.counts[i]);
            drawText(x + 0x19 + (i / 4) * 80, y + 0x27, (s32)buf, 7, z);
            drawText(x + 0x34 + (i / 4) * 80, y + (i % 4) * 13, (s32)SUB_STR_CARDS, 7, z);
        }
    }
}

void SUB_drawCardData(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 slot;
    s32 i;

    if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[SUB_COLLECTION_STATS.selectedId] & 0x40) {
        SUB_CARD_IMAGE_CACHE.request = SUB_COLLECTION_STATS.selectedId;
        slot = SUB_findCachedCardImage(SUB_CARD_IMAGE_CACHE.request);
        if (slot != -1) {
            SUB_drawSprite(x + 3, y + 14, getClut(0, slot + 0x1F4), (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0x40, 0x40, 1, 0x80, -1, z);
            sprintf(buf, "%3.3d", *(s16 *)SUB_COLLECTION_STATS.selectedCard);
            drawText(x + 3, y, (s32)buf, 7, z);
            drawText(x + 0x7B, y, (s32)(SUB_COLLECTION_STATS.selectedCard + 3), 7, z);
            switch (SUB_COLLECTION_STATS.selectedCard[2]) {
            case 0:
                drawIcon(x + 0x5F, y, 0, ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->attr >> 4, z);
                drawIcon(x + 0x29, y, 0, (((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->attr & 0xF) + 0x10, z);
                drawText(x + 0x1B, y, (s32)SUB_STR_LV, 7, z);
                drawText(x + 0x41, y, (s32)SUB_STR_TYPE, 7, z);
                sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_COLLECTION_STATS.selectedCard));
                drawText(x + 0xFA, y, (s32)buf, 7, z);
                drawText(x + 0x106, y, (s32)SUB_STR_CARDS, 7, z);
                drawIcon(x + 0x4B, y + 12, 0, 0x1A, z);
                sprintf(buf, SUB_FMT_4_DIGITS, ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->hp);
                drawText(x + 0x69, y + 12, (s32)buf, 7, z);
                for (i = 0; i < 3; i++) {
                    sprintf(buf, "b%d", i);
                    drawIconText(x + 0x4B, y + (i + 3) * 12, 7, 1, z, (s32)buf);
                    sprintf(buf, "*s0%4d/%4d", (u16)PLAYER_DATA(SUB_EDITOR.player).maxAttackPowers[*(s16 *)SUB_COLLECTION_STATS.selectedCard][i], ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->attack[i].power);
                    drawText(x + 0x6F, y + (i + 3) * 12, (s32)buf, 7, z);
                }
                drawSmallText(x + 0x57, y + 0x48, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->crossEffect], 7, z);
                if (CROSS_EFFECT_ICONS[((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->crossEffect] != 0) {
                    drawIcon(x + 0x91, y + 0x4E, 0, CROSS_EFFECT_ICONS[((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->crossEffect] + 0x14, z);
                }
                drawIcon(x + 0x4B, y + 0x18, 0, 0x18, z);
                sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->dpCost);
                drawText(x + 0x69, y + 0x18, (s32)buf, 7, z);
                drawIcon(x + 0x81, y + 0x18, 0, 0x19, z);
                sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->dpBonus);
                drawText(x + 0x93, y + 0x18, (s32)buf, 7, z);
                sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).cardWins[*(s16 *)SUB_COLLECTION_STATS.selectedCard]);
                drawText(x + 0xBD, y + 12, (s32)buf, 7, z);
                drawText(x + 0xD5, y + 12, (s32)SUB_STR_WINS, 6, z);
                sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).cardLosses[*(s16 *)SUB_COLLECTION_STATS.selectedCard]);
                drawText(x + 0xEE, y + 12, (s32)buf, 7, z);
                drawText(x + 0x106, y + 12, (s32)SUB_STR_LOSSES, 6, z);
                drawText(x + 0xB9, y + 0x18, (s32)SUB_STR_SUPPORT_EFFECT, 6, z);
                if (((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->supportIcon != 0) {
                    drawIcon(x + 0x10C, y + 0x18, 0, ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->supportIcon + 0x14, z);
                }
                for (i = 0; i < 4; i++) {
                    drawText(x + 0xB9, y + (i + 3) * 12, (s32)((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->supportText[i], 7, z);
                }
                break;
            case 1:
                drawIcon(x + 0x18, y, 0, 5, z);
                sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_COLLECTION_STATS.selectedCard));
                drawText(x + 0xFA, y, (s32)buf, 7, z);
                drawText(x + 0x106, y, (s32)SUB_STR_CARDS, 7, z);
                if (SUB_COLLECTION_STATS.selectedCard[0x8C] != 0) {
                    drawIcon(x + 0x8D, y + 13, 0, SUB_COLLECTION_STATS.selectedCard[0x8C] + 0x14, z);
                }
                y += 0x1A;
                for (i = 0; i < 4; i++) {
                    drawText(x + 0x8D, y, (s32)((u8 *)&((OptionCardData *)OPTION_CARDS)[*(s16 *)SUB_COLLECTION_STATS.selectedCard - 0xBF] + 0x8D + i * 21), 7, z);
                    y += 12;
                }
                break;
            case 2:
                drawIcon(x + 0x18, y, 0, 6, z);
                sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_COLLECTION_STATS.selectedCard));
                drawText(x + 0xFA, y, (s32)buf, 7, z);
                drawText(x + 0x106, y, (s32)SUB_STR_CARDS, 7, z);
                y += 0x1A;
                for (i = 0; i < 4; i++) {
                    drawText(x + 0x8D, y, (s32)((u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[*(s16 *)SUB_COLLECTION_STATS.selectedCard - 0x125] + 0x1B + i * 21), 7, z);
                    y += 12;
                }
                break;
            }
        }
    }
}

const char SUB_FMT_2_DIGITS[] = "*s0%2d";

const char SUB_STR_WINS[] = "Wins";

const char SUB_STR_LOSSES[] = "Losses";

const char SUB_STR_SUPPORT_EFFECT[] = "Support Effect";

const char SUB_STR_FIRE_ICON[] = "*a0";

const char SUB_STR_ICE_ICON[] = "*a1";

const char SUB_STR_NATURE_ICON[] = "*a2";

const char SUB_STR_DARKNESS_ICON[] = "*a3";

const char SUB_STR_RARE_ICON[] = "*a4";

const char SUB_STR_OPTION_CARD[] = "Option Card";

const char SUB_STR_PARTNER[] = "Partner";

void SUB_countSavedDecks(void) {
    s32 i;

    SUB_DECK_MENU.count = 0;
    for (i = 0; i < 3; i++) {
        SUB_DECK_MENU.decks[i] = &PLAYER_DATA(SUB_EDITOR.player).savedDecks[i];
        if (SUB_DECK_MENU.decks[i]->inUse == 1) {
            SUB_DECK_MENU.count++;
        } else {
            SUB_DECK_MENU.decks[i]->inUse = 0;
        }
    }
}

void SUB_drawDeckSummary(UiWindow *window) {
    char buf[72];
    const char *labels[7] = { SUB_STR_FIRE_ICON, SUB_STR_ICE_ICON, SUB_STR_NATURE_ICON, SUB_STR_DARKNESS_ICON, SUB_STR_RARE_ICON, SUB_STR_OPTION_CARD, SUB_STR_PARTNER };
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 palette;
    s32 labelPalette;
    PlayerDeck *deck;
    u32 battles;
    s32 i;
    s32 dx;
    s32 dy;

    if (SUB_DECK_MENU.current == SUB_DECK_MENU.slot) {
        window->palette = 6;
        palette = 7;
        labelPalette = 6;
    } else {
        window->palette = 1;
        labelPalette = 8;
        palette = 8;
    }
    deck = &PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.current];
    sprintf(buf, "%s Deck", deck->name);
    drawText(x, y, (s32)buf, palette, z);
    battles = deck->wins + deck->losses;
    if (battles >= 1000) {
        battles = 999;
    }
    sprintf(buf, SUB_FMT_3_DIGITS, battles);
    drawText(x + 0x62, y, (s32)buf, palette, z);
    drawTinyText(x + 0x76, y + 7, (s32)"Battles", palette, z);
    sprintf(buf, SUB_FMT_3_DIGITS, deck->wins);
    drawText(x + 0x95, y, (s32)buf, palette, z);
    drawTinyText(x + 0xA9, y + 7, (s32)SUB_STR_WINS, palette, z);
    sprintf(buf, SUB_FMT_3_DIGITS, deck->losses);
    drawText(x + 0xBD, y, (s32)buf, palette, z);
    drawTinyText(x + 0xD1, y + 7, (s32)SUB_STR_LOSSES, palette, z);
    y += 13;
    for (i = 0; i < 6; i++) {
        dy = 0;
        if (i >= 4) {
            dy = 13;
        }
        dx = (i % 4) * 59;
        drawText(x + dx, y + dy, (s32)labels[i], labelPalette, z);
        if (i == 5) {
            sprintf(buf, SUB_FMT_2_DIGITS, countDeckCardsByFilter(SUB_EDITOR.player, deck, 0xC0));
        } else {
            sprintf(buf, SUB_FMT_2_DIGITS, countDeckCardsByFilter(SUB_EDITOR.player, deck, 1 << i));
        }
        if (i == 5) {
            drawText(x + 0x3A + dx, y + dy, (s32)buf, palette, z);
            drawSmallText(x + 0x4A + dx, y + dy + 7, (s32)SUB_STR_CARDS, palette, z);
        } else {
            drawText(dx + x + 0x10, y + dy, (s32)buf, palette, z);
            drawSmallText(x + 0x10 + dx + 0x10, y + dy + 7, (s32)SUB_STR_CARDS, palette, z);
        }
    }
    drawText(x + 0xA0, y + dy, (s32)labels[6], labelPalette, z);
    sprintf(buf, SUB_FMT_2_DIGITS, countDeckCardsByFilter(SUB_EDITOR.player, deck, 0x20));
    drawText(x + 0xC1, y + dy, (s32)buf, palette, z);
    drawSmallText(x + 0xD1, y + dy + 7, (s32)SUB_STR_CARDS, palette, z);
    drawText(x, y + 26, (s32)SUB_STR_LV, labelPalette, z);
    sprintf(buf, SUB_FMT_2_DIGITS, countDeckCardsByFilter(SUB_EDITOR.player, deck, 0x200));
    drawIcon(x + 0xC, y + 26, 0, 0x10, z);
    drawText(x + 0x1E, y + 26, (s32)buf, palette, z);
    drawSmallText(x + 0x2E, y + 26 + 7, (s32)SUB_STR_CARDS, palette, z);
    x += 0x51;
    drawText(x, y + 26, (s32)SUB_STR_LV, labelPalette, z);
    sprintf(buf, SUB_FMT_2_DIGITS, countDeckCardsByFilter(SUB_EDITOR.player, deck, 0x800));
    drawIcon(x + 0xC, y + 26, 0, 0x12, z);
    drawText(x + 0x1E, y + 26, (s32)buf, palette, z);
    drawSmallText(x + 0x2E, y + 26 + 7, (s32)SUB_STR_CARDS, palette, z);
    x += 0x52;
    drawText(x, y + 26, (s32)SUB_STR_LV, labelPalette, z);
    sprintf(buf, SUB_FMT_2_DIGITS, countDeckCardsByFilter(SUB_EDITOR.player, deck, 0x1000));
    drawIcon(x + 0xC, y + 26, 0, 0x13, z);
    drawText(x + 0x1E, y + 26, (s32)buf, palette, z);
    drawSmallText(x + 0x2E, y + 26 + 7, (s32)SUB_STR_CARDS, palette, z);
}

void SUB_drawEmptyDeck(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 palette;

    if (SUB_DECK_MENU.current == SUB_DECK_MENU.slot) {
        window->palette = 6;
        palette = 7;
    } else {
        window->palette = 1;
        palette = 8;
    }
    drawText(x + 0x50, y + 0x12, (s32)"NO DATA", palette, z);
}

void SUB_drawDeckMenuHelp(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawIcon(x, y, 1, 11, z);
    drawMediumText(x + 8, y, (s32)":Cursor", 7, z);
    y += 10;
    drawIcon(x, y, 1, 9, z);
    drawMediumText(x + 8, y, (s32)":Edit", 7, z);
    y += 10;
    drawIcon(x, y, 1, 7, z);
    drawMediumText(x + 8, y, (s32)":Delete", 7, z);
    y += 10;
    drawIcon(x, y, 1, 10, z);
    drawMediumText(x + 8, y, (s32)":Copy", 7, z);
    y += 10;
    drawIcon(x, y, 1, 13, z);
    drawMediumText(x + 8, y, (s32)":Name", 7, z);
    y += 10;
    drawIcon(x, y, 1, 8, z);
    drawMediumText(x + 8, y, (s32)":Back", 7, z);
}

void SUB_drawDeckMenu(void) {
    Rect16 uv;
    s16 y;
    s32 i;

    drawWindow(SUB_WINDOWS, SUB_drawDeckMenuHelp, 30);
    i = 0;
    do {
        SUB_DECK_MENU.current = i;
        if (PLAYER_DATA(SUB_EDITOR.player).savedDecks[i].inUse != 0) {
            drawWindow(&SUB_LIST_WINDOWS[i], SUB_drawDeckSummary, 30);
        } else {
            drawWindow(&SUB_LIST_WINDOWS[i], SUB_drawEmptyDeck, 30);
        }
    } while (++i < 3);
    if (SUB_EDITOR.hidden == 0) {
        if (++SUB_EDITOR.slide > 20) {
            SUB_EDITOR.slide = 20;
        }
    } else {
        if (--SUB_EDITOR.slide < 0) {
            SUB_EDITOR.slide = 0;
        }
    }
    y = (SUB_EDITOR.slide * 8 - (20 - SUB_EDITOR.slide) * 33) / 20;
    uv.x = 0;
    uv.y = 0x79;
    uv.w = 0x80;
    uv.h = 0x20;
    drawTexturedSprite(6, y, &uv, 0x18, 0x7E21, 30, 0x80, -1);
}

const char SUB_STR_DECK_1[] = "DECK 1";

const char SUB_STR_DECK_2[] = "DECK 2";

const char SUB_STR_DECK_3[] = "DECK 3";

void SUB_runDeckMenu(void) {
    u8 dialog[0xB8];
    Rect16 rects[4];
    const char *labels[4] = { SUB_STR_HELP, SUB_STR_DECK_1, SUB_STR_DECK_2, SUB_STR_DECK_3 };
    s16 running = 1;
    s8 created = 0;
    s16 action;

    s32 i;
    s32 flags;
    s32 result;
    s32 style;

    SUB_countSavedDecks();
    rects[0].x = 0x28;
    rects[0].y = 0x4E;
    rects[0].w = 0x3C;
    rects[0].h = 0x3A;
    rects[1].x = 0xC4;
    rects[1].y = 0x4A;
    rects[1].w = 0xE8;
    rects[1].h = 0x32;
    rects[2].x = 0xC4;
    rects[2].y = 0x8B;
    rects[2].w = 0xE8;
    rects[2].h = 0x32;
    rects[3].x = 0xC4;
    rects[3].y = 0xCC;
    rects[3].w = 0xE8;
    rects[3].h = 0x32;
    for (i = 0; i < 4; i++) {
        flags = 8;
        style = 0x21;
        SUB_openCenteredWindow(&SUB_WINDOWS[i], rects[i], (s32)labels[i], flags, style);
    }
    SUB_EDITOR.slide = 0;
    SUB_EDITOR.hidden = 0;
    playMenuSound(3);
    addFrameCallback((s32)SUB_drawDeckMenu);
    do {
        waitFrames(1);
        if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x10) {
            action = 1;
        } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x40) {
            action = 2;
        } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x800) {
            action = 3;
        } else if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x1000) {
            if (SUB_DECK_MENU.slot == 1) {
                PAD_STATES[SUB_EDITOR.player]->repeatEnabled = 0;
            }
            action = 4;
        } else if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x4000) {
            if (SUB_DECK_MENU.slot == 1) {
                PAD_STATES[SUB_EDITOR.player]->repeatEnabled = 0;
            }
            action = 5;
        } else if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x2000) {
            action = 6;
        } else if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x8000) {
            action = 7;
        } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x20) {
            action = 8;
        } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x80) {
            action = 9;
        } else {
            action = 0;
        }
        switch (action) {
        case 2:
            if (PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse == 0) {
                playMenuSound(1);
                initDialog(dialog, (u8 *)"Do you want to create a new Deck?", 1);
                dialog[0xA6] = SUB_EDITOR.player;
                runDialog(dialog);
                result = (s8)dialog[0xA5];

                if (result != 1) {
                    break;
                }
                PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].wins = PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].losses = 0;
                SUB_DECK_IS_NEW = result;
                SUB_AUTO_DECK_ENABLED = 0;
                created = 1;
                SUB_DECK_MENU.slot = SUB_DECK_MENU.count;
            } else {
                SUB_DECK_IS_NEW = 0;
            }
            /* fallthrough */
        case 1:
        case 3:
            if (action == 3) {
                if (PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse == 0) {
                    break;
                }
                playMenuSound(1);
            } else {
                playMenuSound(4);
            }
            for (i = 0; i < 4; i++) {
                animateWindowTo(&SUB_WINDOWS[i], (Rect16 *)-1);
            }
            running = 0;
            break;
        case 4:
            playMenuSound(2);
            if (--SUB_DECK_MENU.slot < 0) {
                SUB_DECK_MENU.slot = 2;
            }
            break;
        case 5:
            playMenuSound(2);
            if (++SUB_DECK_MENU.slot >= 3) {
                SUB_DECK_MENU.slot = 0;
            }
            break;
        case 8:
            if (SUB_DECK_MENU.decks[SUB_DECK_MENU.slot]->inUse != 0 && SUB_DECK_MENU.count >= 2) {
                playMenuSound(1);
                initDialog(dialog, (u8 *)"Do you want to delete this Deck?", 1);
                dialog[0xA6] = SUB_EDITOR.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    deleteSavedDeck(SUB_EDITOR.player, SUB_DECK_MENU.slot);
                    SUB_DECK_MENU.count--;
                    for (i = 0; i < 3; i++) {
                        if (PLAYER_DATA(SUB_EDITOR.player).savedDecks[i].inUse == 0) {
                            PLAYER_DATA(SUB_EDITOR.player).savedDecks[i].wins = PLAYER_DATA(SUB_EDITOR.player).savedDecks[i].losses = 0;
                        }
                    }
                }
            }
            break;
        case 9:
            if (SUB_DECK_MENU.decks[SUB_DECK_MENU.slot]->inUse != 0 && SUB_DECK_MENU.count < 3) {
                playMenuSound(1);
                initDialog(dialog, (u8 *)"Do you want to copy this Deck?", 1);
                dialog[0xA6] = SUB_EDITOR.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    storeSavedDeck(SUB_EDITOR.player, &PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot], SUB_DECK_MENU.count);
                    SUB_DECK_MENU.count++;
                }
            }
            break;
        }
    } while (running);
    SUB_EDITOR.hidden = -1;
    waitFrames(20);
    removeFrameCallback((s32)SUB_drawDeckMenu);
    waitFrames(1);
    switch (action) {
    case 1:
        spawnTask(0, -1, 0, 0x1000, SUB_EDITOR.task, 0, 0, 0, 0);
        break;
    case 2:
        if (created == 1) {
            sprintf(PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].name, "NEW ");
            spawnTask(0, -1, 0, 0x1000, SUB_enterDeckName, 0, PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].name, SUB_EDITOR.player, 0);
        } else {
            spawnTask(0, -1, 0, 0x1000, SUB_editDeck, &PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot], 0, 0, 0);
        }
        break;
    case 3:
        spawnTask(0, -1, 0, 0x1000, SUB_enterDeckName, 1, PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].name, SUB_EDITOR.player, 0);
        break;
    }
}

const char SUB_STR_SUM[] = "SUM";

const char SUB_STR_DECK_TITLE[] = "DECK";

void SUB_uploadArchiveTim(s16 index) {
    u8 unused[0x40]; /* unused, but it is in the original stack frame */

    uploadTim((u32 *)(SUB_CARD_ARCHIVE + ((s32 *)SUB_CARD_ARCHIVE)[index + 1]), 0x220, 0x100, 0x240, 0x1F5);
}

void SUB_drawDeckEdit(void) {
    Rect16 uv;
    s16 y;

    drawWindow(&SUB_CARD_SORT_WINDOW, SUB_drawCardSortMenu, 29);
    drawWindow(&SUB_DECK_SORT_WINDOW, SUB_drawDeckSortMenu, 29);
    drawWindow(&SUB_PICKER_WINDOW, SUB_drawPickerList, 30);
    drawWindow(SUB_WINDOWS, SUB_drawPickerCardInfo, 30);
    drawWindow(SUB_LIST_WINDOWS, SUB_drawDeckEditTitle, 30);
    drawWindow(&SUB_STATS_WINDOW, SUB_drawDeckStats, 30);
    drawWindow(&SUB_GRID_WINDOW, SUB_drawDeckGrid, 30);
    drawWindow(&SUB_CARD_INFO_WINDOW, SUB_drawSlotCardInfo, 30);
    drawWindow(&SUB_CARD_DATA_WINDOW, SUB_drawSortHint, 30);
    if (SUB_EDITOR.hidden == 0) {
        if (++SUB_EDITOR.slide > 20) {
            SUB_EDITOR.slide = 20;
        }
    } else {
        if (--SUB_EDITOR.slide < 0) {
            SUB_EDITOR.slide = 0;
        }
    }
    y = (SUB_EDITOR.slide * 8 - (20 - SUB_EDITOR.slide) * 33) / 20;
    uv.x = 0;
    uv.y = 0x99;
    uv.w = 0x80;
    uv.h = 0x20;
    drawTexturedSprite(6, y, &uv, 0x18, 0x7E21, 30, 0x80, -1);
}

void SUB_initDeckEdit(PlayerDeck *deck) {
    Rect16 rects[7];
    const char *labels[7] = { SUB_STR_CARD_INFO, SUB_STR_DECK_1, SUB_STR_CARD_LIST, SUB_STR_SUM, SUB_STR_DECK_TITLE, SUB_STR_CARD_INFO, SUB_STR_HELP };
    s32 i;
    s32 j;
    LINE_G3 *line;
    s32 id;
    s32 flags;
    s32 style;

    SUB_DECK_EDIT.deckCounts = allocTaskHeapBlock(301);
    SUB_EDITED_DECK = allocTaskHeapBlock(0x110);
    *SUB_EDITED_DECK = *deck;
    SUB_DECK_EDIT.statsPage = 0;
    SUB_DECK_EDIT.slot = 0;
    SUB_DECK_EDIT.unk10F = 0;
    for (i = 0; i < 301; i++) {
        SUB_DECK_EDIT.deckCounts[i] = getOwnedCardCount(SUB_EDITOR.player, i);
    }
    if (deck->inUse == 0) {
        deck->inUse = 1;
        SUB_EDITED_DECK->inUse = 1;
        for (i = 0; i < 30; i++) {
            SUB_EDITED_DECK->cards[i].type = 0xFF;
        }
    } else {
        for (i = 0; i < 30; i++) {
            id = getCardId(SUB_EDITED_DECK->cards[i].type, SUB_EDITED_DECK->cards[i].index);
            if (SUB_DECK_EDIT.deckCounts[id] != 0) {
                SUB_DECK_EDIT.deckCounts[id]--;
            }
        }
    }
    for (j = 0; j < 2; j++) {
        line = SUB_DECK_EDIT.cursorLines[j];
        for (i = 0; i < 4; i++) {
            SetLineG3(line);
            line->r0 = 0x80;
            line->g0 = 0x80;
            line->b0 = 0x80;
            line->r1 = 0x80;
            line->g1 = 0x80;
            line->b1 = 0x80;
            line->r2 = 0x80;
            line->g2 = 0x80;
            line->b2 = 0x80;
            line++;
        }
    }
    SUB_EDITOR.useDeckCounts = 1;
    SUB_initCardList();
    SUB_DECK_EDIT.mode = 1;
    SUB_CARD_LIST_MENU.rect.h = 0x60;
    SUB_CARD_LIST_MENU.ox = 0x62;
    openMenu(&SUB_CARD_LIST_MENU, &SUB_PICKER_WINDOW, &SUB_CARD_LIST_CURSOR, (Bytes4 *)-1);
    SUB_PICKER_WINDOW_LABEL = (s32)SUB_STR_CARD_LIST;
    openMenu(&SUB_CARD_SORT_MENU, &SUB_CARD_SORT_WINDOW, &SUB_CARD_SORT_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&SUB_CARD_SORT_WINDOW, (Rect16 *)-1);
    SUB_CARD_SORT_WINDOW.label = (s32)"SORT MENU";
    SUB_CARD_LIST_MENU.row = 0;
    centerMenuOnCursor(&SUB_CARD_LIST_MENU);
    openMenu(&SUB_DECK_SORT_MENU, &SUB_DECK_SORT_WINDOW, &SUB_DECK_SORT_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&SUB_DECK_SORT_WINDOW, (Rect16 *)-1);
    SUB_DECK_SORT_WINDOW.label = (s32)"SORT MENU";
    rects[0].x = 0xA3;
    rects[0].y = 0xBB;
    rects[0].w = 0x128;
    rects[0].h = 0x4E;
    rects[1].x = 0xE6;
    rects[1].y = 0x1B;
    rects[1].w = 0xA0;
    rects[1].h = 0xE;
    rects[2].x = 0xD8;
    rects[2].y = 0x5C;
    rects[2].w = 0xD7;
    rects[2].h = 0x60;
    rects[3].x = 0x23;
    rects[3].y = 0xAD;
    rects[3].w = 0x36;
    rects[3].h = 0x66;
    rects[4].x = 0xBF;
    rects[4].y = 0xBB;
    rects[4].w = 0xF2;
    rects[4].h = 0x4A;
    rects[5].x = 0xBF;
    rects[5].y = 0x5C;
    rects[5].w = 0xF2;
    rects[5].h = 0x64;
    rects[6].x = 0x23;
    rects[6].y = 0x35;
    rects[6].w = 0x36;
    rects[6].h = 0x8;
    for (i = 0; i < 7; i++) {
        if (i == 2) {
            continue;
        }
        switch (i) {
        case 0:
        case 2:
            flags = 8;
            style = 0x41;
            break;
        case 1:
        case 4:
        case 5:
            flags = 0;
            style = 0x31;
            break;
        case 3:
        case 6:
            flags = 8;
            style = 0x21;
            break;
        default:
            style = 0;
            flags = 0;
            break;
        }
        SUB_openCenteredWindow(&SUB_WINDOWS[i], rects[i], (s32)labels[i], flags, style);
    }
    animateWindowTo(&SUB_WINDOWS[0], (Rect16 *)-1);
    animateWindowTo(&SUB_PICKER_WINDOW, (Rect16 *)-1);
}

s32 SUB_canAddCardToDeck(s32 cardId) {
    s16 unused[6] = { 0xAF, 0xB6, 0xB7, 0xB8, 0xBB, 0xBE }; /* unused, but it is in the original stack frame */
    s32 unused2[2]; /* unused, but it is in the original stack frame */
    s32 count;
    s32 i;

    if (cardId >= 0xAC && cardId <= 0xAE) {
        return 0;
    }
    if (cardId >= 0xB0 && cardId <= 0xB5) {
        return 0;
    }
    if (cardId >= 0xB9 && cardId <= 0xBA) {
        return 0;
    }
    if (cardId == 0xBC) {
        return 0;
    }
    if (cardId == 0xBD) {
        return 0;
    }
    count = 0;
    for (i = 0; i < 30; i++) {
        if (SUB_EDITED_DECK->cards[i].type != 0xFF && cardId == getCardId(SUB_EDITED_DECK->cards[i].type, SUB_EDITED_DECK->cards[i].index)) {
            count++;
        }
    }
    return count < 4;
}

s32 SUB_hasDeckChanged(PlayerDeck *a, PlayerDeck *b) {
    s8 matched[30];
    s32 i;
    s32 j;
    s32 result = 0;

    for (i = 0; i < 30; i++) {
        matched[i] = 0;
    }
    for (i = 0; i < 30; i++) {
        for (j = 0; j < 30; j++) {
            if (matched[j] == 0 && a->cards[i].type == b->cards[j].type) {
                if (a->cards[i].type == 0xFF) {
                    matched[j] = 1;
                    j = 30;
                } else if (a->cards[i].index == b->cards[j].index) {
                    matched[j] = 1;
                    j = 30;
                }
            }
        }
    }
    for (i = 0; i < 30; i++) {
        if (matched[i] == 0) {
            result = 1;
        }
    }
    return result;
}

void SUB_tickDeckSlots(PlayerDeck *deck) {
    u8 dialog[0xC0];
    Rect16 from;
    Rect16 to;
    s16 r;
    s32 w;
    s32 h;
    s32 ty;
    s8 result;
    s32 i;

    SUB_EDITOR.useDeckCounts = 1;
    if (SUB_EDITOR.listShown == 0 && ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0xF000)) {
        playMenuSound(2);
    }
    if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x1000) {
        if (SUB_EDITOR.listShown == 0) {
            SUB_DECK_EDIT.slot -= 10;
            if (SUB_DECK_EDIT.slot < 10) {
                PAD_STATES[SUB_EDITOR.player]->repeatEnabled = 0;
            }
            if (SUB_DECK_EDIT.slot < 0) {
                SUB_DECK_EDIT.slot += 30;
            }
        }
    } else if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x4000) {
        if (SUB_EDITOR.listShown == 0) {
            SUB_DECK_EDIT.slot += 10;
            if (SUB_DECK_EDIT.slot >= 20) {
                PAD_STATES[SUB_EDITOR.player]->repeatEnabled = 0;
            }
            if (SUB_DECK_EDIT.slot >= 30) {
                SUB_DECK_EDIT.slot -= 30;
            }
        }
    } else if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x2000) {
        if (SUB_EDITOR.listShown == 0) {
            SUB_DECK_EDIT.slot++;
            r = SUB_DECK_EDIT.slot % 10;
            if (r == 9) {
                PAD_STATES[SUB_EDITOR.player]->repeatEnabled = 0;
            }
            r = SUB_DECK_EDIT.slot % 10;
            if (r == 0) {
                SUB_DECK_EDIT.slot -= 10;
            }
        }
    } else if ((u16)PAD_STATES[SUB_EDITOR.player]->repeat & 0x8000) {
        if (SUB_EDITOR.listShown == 0) {
            SUB_DECK_EDIT.slot--;
            r = SUB_DECK_EDIT.slot % 10;
            if (r == 0) {
                PAD_STATES[SUB_EDITOR.player]->repeatEnabled = 0;
            }
            r = SUB_DECK_EDIT.slot % 10;
            if (r == 9) {
                SUB_DECK_EDIT.slot += 10;
            } else if (SUB_DECK_EDIT.slot < 0) {
                SUB_DECK_EDIT.slot = 9;
            }
        }
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 4) {
        if (SUB_DECK_EDIT.statsPage != 0) {
            playMenuSound(1);
        }
        SUB_DECK_EDIT.statsPage = 0;
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 8) {
        if (SUB_DECK_EDIT.statsPage == 0) {
            playMenuSound(1);
        }
        SUB_DECK_EDIT.statsPage = 1;
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x40) {
        if (SUB_EDITOR.listShown == 0) {
            from.x = 0xA3;
            from.y = 0xBB;
            from.w = 0x128;
            from.h = 0x4E;
            w = from.w;
            ty = 0x94;
            to.x = 0xF;
            h = from.h;
            to.y = ty;
            to.w = w;
            to.h = h;
            SUB_DECK_EDIT.mode = 2;
            animateWindowTo(SUB_WINDOWS, &to);
            animateWindowTo(&SUB_PICKER_WINDOW, &SUB_CARD_LIST_MENU.rect);
            animateWindowTo(&SUB_STATS_WINDOW, (Rect16 *)-1);
            animateWindowTo(&SUB_GRID_WINDOW, (Rect16 *)-1);
            animateWindowTo(&SUB_CARD_INFO_WINDOW, (Rect16 *)-1);
            playMenuSound(1);
        }
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x10) {
        if (SUB_EDITOR.listShown == 0) {
            playMenuSound(4);
            result = 0;
            for (i = 0; i < 30; i++) {
                if (SUB_EDITED_DECK->cards[i].type == 0xFF) {
                    result = 1;
                    break;
                }
            }
            if (result != 1) {
                result = 2;
                for (i = 0; i < 30; i++) {
                    if (SUB_EDITED_DECK->cards[i].type == 0) {
                        result = 0;
                        break;
                    }
                }
            }
            if (result == 1) {
                initDialog(dialog, "There are not enough Cards.\nDo you want to quit editing this Deck?", 1);
                dialog[0xA6] = SUB_EDITOR.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    if (SUB_DECK_IS_NEW == 1) {
                        deck->inUse = 0;
                    }
                    SUB_EDITOR.editing = 0;
                }
            } else if (result == 2) {
                initDialog(dialog, "Please place Digimon Cards in a Deck.", 0);
                dialog[0xA6] = SUB_EDITOR.player;
                runDialog(dialog);
            } else if (SUB_hasDeckChanged(&SUB_ORIGINAL_DECK, SUB_EDITED_DECK) != 0 || SUB_DECK_IS_NEW == 1) {
                initDialog(dialog, "Do you want to update this Deck?", 1);
                dialog[0xA6] = SUB_EDITOR.player;
                runDialog(dialog);
                if ((s8)dialog[0xA5] == 1) {
                    storeSavedDeck(SUB_EDITOR.player, SUB_EDITED_DECK, SUB_DECK_MENU.slot);
                    linkSavedDecks(SUB_EDITOR.player);
                    SUB_EDITOR.editing = 0;
                } else if ((s8)dialog[0xA5] == 2) {
                    initDialog(dialog, "Do you want to quit editing this Deck?", 1);
                    dialog[0xA6] = SUB_EDITOR.player;
                    runDialog(dialog);
                    if ((s8)dialog[0xA5] == 1) {
                        if (SUB_DECK_IS_NEW == 1) {
                            deck->inUse = 0;
                        }
                        SUB_EDITOR.editing = 0;
                    }
                }
            } else {
                SUB_EDITOR.editing = 0;
                *deck = *SUB_EDITED_DECK;
            }
        } else {
            playMenuSound(4);
            SUB_CARD_LIST_MENU_ACTIVE = 1;
            animateWindowTo(&SUB_DECK_SORT_WINDOW, (Rect16 *)-1);
            SUB_EDITOR.listShown ^= 1;
        }
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x100) {
        SUB_EDITOR.listShown ^= 1;
        if (SUB_EDITOR.listShown != 0) {
            playMenuSound(3);
            SUB_CARD_LIST_MENU_ACTIVE = 0;
            animateWindowTo(&SUB_DECK_SORT_WINDOW, &SUB_DECK_SORT_MENU.rect);
        } else {
            playMenuSound(4);
            SUB_CARD_LIST_MENU_ACTIVE = 1;
            animateWindowTo(&SUB_DECK_SORT_WINDOW, (Rect16 *)-1);
        }
    }
}

void SUB_showDeckSlots(void) {
    Rect16 from;
    Rect16 to;
    s32 w1;
    s32 h1;
    s32 w2;
    s32 h2;
    s32 w3;
    s32 h3;

    SUB_DECK_EDIT.mode = 1;
    animateWindowTo(SUB_WINDOWS, (Rect16 *)-1);
    animateWindowTo(&SUB_PICKER_WINDOW, (Rect16 *)-1);
    from.x = 0x23;
    from.y = 0xAD;
    from.w = 0x36;
    from.h = 0x66;
    w1 = from.w;
    to.x = 0x8;
    h1 = from.h;
    to.y = 0x7A;
    to.w = w1;
    to.h = h1;
    animateWindowTo(&SUB_STATS_WINDOW, &to);
    from.x = 0xBF;
    from.y = 0xBB;
    from.w = 0xF2;
    from.h = 0x4A;
    w2 = from.w;
    to.x = 0x46;
    h2 = from.h;
    to.y = 0x96;
    to.w = w2;
    to.h = h2;
    animateWindowTo(&SUB_GRID_WINDOW, &to);
    from.x = 0xBF;
    from.y = 0x5C;
    from.w = 0xF2;
    from.h = 0x64;
    w3 = from.w;
    to.x = 0x46;
    h3 = from.h;
    to.y = 0x2A;
    to.w = w3;
    to.h = h3;
    animateWindowTo(&SUB_CARD_INFO_WINDOW, &to);
}

void SUB_tickCardPicker(PlayerDeck *deck) {
    Rect16 rect;
    s32 cardId;

    SUB_EDITOR.useDeckCounts = 2;
    if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x40) {
        if (SUB_EDITOR.listShown == 0 && SUB_DECK_EDIT.deckCounts[SUB_DECK_EDIT.cardId] != 0 && SUB_canAddCardToDeck(SUB_DECK_EDIT.cardId) != 0) {
            cardId = getCardId(SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot].type, SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot].index);
            SUB_DECK_EDIT.deckCounts[SUB_DECK_EDIT.cardId]--;
            if (SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot].type != 0xFF) {
                SUB_DECK_EDIT.deckCounts[cardId]++;
            }
            setCardSlotFromId((u8 *)&SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot], SUB_DECK_EDIT.cardId);
            playMenuSound(1);
            SUB_showDeckSlots();
        }
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x10) {
        if (SUB_EDITOR.listShown == 0) {
            playMenuSound(0);
            SUB_showDeckSlots();
        } else {
            playMenuSound(4);
            SUB_EDITOR.listShown ^= 1;
            SUB_CARD_LIST_MENU_ACTIVE = 1;
            animateWindowTo(&SUB_CARD_SORT_WINDOW, (Rect16 *)-1);
        }
    } else if (PAD_STATES[SUB_EDITOR.player]->pressed & 0x100) {
        SUB_EDITOR.listShown ^= 1;
        if (SUB_EDITOR.listShown != 0) {
            playMenuSound(3);
            SUB_CARD_LIST_MENU_ACTIVE = 0;
            rect.x = 0x28;
            rect.y = 0x3C;
            rect.w = 0x52;
            rect.h = 0x6E;
            animateWindowTo(&SUB_CARD_SORT_WINDOW, &SUB_CARD_SORT_MENU_RECT);
        } else {
            playMenuSound(4);
            SUB_CARD_LIST_MENU_ACTIVE = 1;
            animateWindowTo(&SUB_CARD_SORT_WINDOW, (Rect16 *)-1);
        }
    }
}

void SUB_editDeck(PlayerDeck *deck) {
    /* a dialog buffer, declared as SUB_tickDeckSlots does */
    u8 dialog[0xC0]; /* unused, but it is in the original stack frame */
    s32 i;

    SUB_initDeckEdit(deck);
    SUB_EDITOR.useDeckCounts = 1;
    SUB_ORIGINAL_DECK = *deck;
    playMenuSound(3);
    addFrameCallback((s32)SUB_drawDeckEdit);
    SUB_EDITOR.editing = 1;
    SUB_EDITOR.slide = 0;
    SUB_EDITOR.hidden = 0;
    SUB_EDITOR.listShown = 0;
    do {
        waitFrames(1);
        if (SUB_DECK_EDIT.mode == 1) {
            SUB_tickDeckSlots(deck);
        } else {
            SUB_tickCardPicker(deck);
        }
    } while (SUB_EDITOR.editing != 0);
    SUB_EDITOR.hidden = -1;
    for (i = 0; i < 7; i++) {
        animateWindowTo(&SUB_WINDOWS[i], (Rect16 *)-1);
    }
    animateWindowTo(&SUB_CARD_SORT_WINDOW, (Rect16 *)-1);
    waitFrames(20);
    removeFrameCallback((s32)SUB_drawDeckEdit);
    waitFrames(1);
    spawnTask(0, -1, 0, 0x1000, SUB_runDeckMenu, 0, 0, 0, 0);
}

void SUB_drawSortHint(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawIcon(x, y, 1, 12, z);
    drawMediumText(x + 8, y, (s32)":Sort", 7, z);
}

const char SUB_STR_DISABLE[] = "Disable";

void SUB_drawSlotCardInfo(UiWindow *window) {
    char buf[72];
    s32 cardId = 0;
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 slot;
    s32 i;

    slot = getCardId(SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot].type, SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot].index);
    if (slot != -1) {
        if (slot >= 0) {
            SUB_CARD_IMAGE_CACHE.request = getCardId(SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot].type, SUB_EDITED_DECK->cards[SUB_DECK_EDIT.slot].index);
            cardId = SUB_CARD_IMAGE_CACHE.request;
            slot = SUB_findCachedCardImage(cardId);
        }
        if (slot == -1) {
            if (++SUB_EDITOR.blink & 4) {
                SUB_drawSprite(x + 3, y + 0x1A, 0x7E20, 0x200, 0x100, 0x40, 0x40, 0, 0x80, slot, z);
            } else {
                SUB_drawSprite(x + 3, y + 0x1A, 0x7E20, 0x210, 0x100, 0x40, 0x40, 0, 0x80, -1, z);
            }
        } else {
            SUB_EDITOR.blink = 0;
            SUB_drawSprite(x + 3, y + 0x1A, getClut(0, slot + 0x1F4), (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0x40, 0x40, 1, 0x80, -1, z);
        }
        sprintf(buf, SUB_FMT_CARD_NUMBER, *(s16 *)SUB_CARDS_BY_ID[cardId]);
        drawText(x + 3, y, (s32)buf, 7, z);
        drawText(x + 0x7B, y, (s32)(SUB_CARDS_BY_ID[cardId] + 3), 7, z);
        switch (SUB_CARDS_BY_ID[cardId][2]) {
        case 0:
            drawIcon(x + 0x57, y, 0, ((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->attr >> 4, z);
            drawIcon(x + 0x27, y, 0, (((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->attr & 0xF) + 0x10, z);
            drawText(x + 0x1B, y, (s32)SUB_STR_LV, 7, z);
            drawText(x + 0x39, y, (s32)SUB_STR_TYPE, 7, z);
            sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).cardWins[*(s16 *)SUB_CARDS_BY_ID[cardId]]);
            drawText(x + 0x1B, y + 12, (s32)buf, 7, z);
            drawText(x + 0x33, y + 12, (s32)SUB_STR_WINS, 6, z);
            sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).cardLosses[*(s16 *)SUB_CARDS_BY_ID[cardId]]);
            drawText(x + 0x53, y + 12, (s32)buf, 7, z);
            drawText(x + 0x6B, y + 12, (s32)SUB_STR_LOSSES, 6, z);
            drawIcon(x + 0x4B, y + 0x18, 0, 0x1A, z);
            sprintf(buf, SUB_FMT_4_DIGITS, ((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->hp);
            drawText(x + 0x5D, y + 0x18, (s32)buf, 7, z);
            drawIcon(x + 0x85, y + 0x18, 0, 0x18, z);
            sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->dpCost);
            drawText(x + 0x97, y + 0x18, (s32)buf, 7, z);
            drawIcon(x + 0xAF, y + 0x18, 0, 0x19, z);
            sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->dpBonus);
            drawText(x + 0xC1, y + 0x18, (s32)buf, 7, z);
            y += 12;
            for (i = 0; i < 3; i++) {
                sprintf(buf, "b%d", i);
                drawIconText(x + 0x4B, y + (i + 2) * 12, 7, 1, z, (s32)buf);
                sprintf(buf, SUB_FMT_4_DIGITS, ((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->attack[i].power);
                drawText(x + 0x5D, y + (i + 2) * 12, (s32)buf, 7, z);
            }
            drawSmallText(x + 0x45, y + 0x3F, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->crossEffect], 7, z);
            if (CROSS_EFFECT_ICONS[((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->crossEffect] != 0) {
                drawIcon(x + 0x51, y + 0x48, 0, CROSS_EFFECT_ICONS[((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->crossEffect] + 0x14, z);
            }
            drawText(x + 0x85, y + 0x18, (s32)SUB_STR_SUPPORT_EFFECT, 6, z);
            if (((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->supportIcon != 0) {
                drawIcon(x + 0xD9, y + 0x18, 0, ((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->supportIcon + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawText(x + 0x85, y + (i + 3) * 12, (s32)((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->supportText[i], 7, z);
            }
            break;
        case 1:
            drawIcon(x + 0x18, y, 0, 5, z);
            if (SUB_CARDS_BY_ID[cardId][0x8C] != 0) {
                drawIcon(x + 0x85, y + 13, 0, SUB_CARDS_BY_ID[cardId][0x8C] + 0x14, z);
            }
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x85, y, (s32)((u8 *)&((OptionCardData *)OPTION_CARDS)[*(s16 *)SUB_CARDS_BY_ID[cardId] - 0xBF] + 0x8D + i * 21), 7, z);
                y += 12;
            }
            break;
        case 2:
            drawIcon(x + 0x18, y, 0, 6, z);
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x85, y, (s32)((u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[*(s16 *)SUB_CARDS_BY_ID[cardId] - 0x125] + 0x1B + i * 21), 7, z);
                y += 12;
            }
            break;
        }
    }
}

void SUB_drawPickerList(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 x = window->originX - 11;
    s32 z = window->z;
    s32 i;
    s32 y;
    u8 palette;

    s32 type;

    u8 *rgb;
    s16 usable;


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
        if (SUB_canAddCardToDeck(*(s16 *)SUB_CARD_LIST[i]) == 0) {
            palette = 3;
            usable = 0;
        } else {
            usable = SUB_DECK_EDIT.deckCounts[*(s16 *)SUB_CARD_LIST[i]] != 0;
        }
        if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)SUB_CARD_LIST[i]] & 0x40) {
            if (SUB_DECK_EDIT.deckCounts[*(s16 *)SUB_CARD_LIST[i]] == 0) {
                rgb = SUB_CARD_LIST_COLORS[2];
            }
            if (!usable) {
                drawTextColored(x + 0xE3, y, (u8 *)SUB_STR_DISABLE, rgb, palette, z);
            } else {
                drawTextColored(x + 0xE3, y, "Able", rgb, palette, z);
            }
            drawTextColored(x + 0x69, y, (u8 *)SUB_CARD_LIST[i] + 3, rgb, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x5B, y, 0, ((DigimonCardData *)SUB_CARD_LIST[i])->attr >> 4, rgb, z);
                if (palette == 3) {
                    drawIconColored(x + 0x2D, y, 0, (((DigimonCardData *)SUB_CARD_LIST[i])->attr & 0xF) + 0x10, SUB_CARD_LIST_COLORS[3], z);
                } else {
                    drawIconColored(x + 0x2D, y, 0, (((DigimonCardData *)SUB_CARD_LIST[i])->attr & 0xF) + 0x10, rgb, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x5B, y, 0, 5, rgb, z);
                break;
            case 2:
                drawIconColored(x + 0x5B, y, 0, 6, rgb, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x59, y, (u8 *)SUB_STR_QUESTION_MARK, rgb, palette, z);
            drawTextColored(x + 0x69, y, "------------------", rgb, palette, z);
        }
        if (type == 0 || !(PLAYER_DATA(SUB_EDITOR.player).cardCollection[*(s16 *)SUB_CARD_LIST[i]] & 0x40)) {
            drawTextColored(x + 0x20, y, (u8 *)SUB_STR_LV, rgb, palette, z);
            drawTextColored(x + 0x3D, y, (u8 *)SUB_STR_TYPE, rgb, palette, z);
        }
        sprintf(buf, SUB_FMT_CARD_NUMBER, *(s16 *)SUB_CARD_LIST[i]);
        drawTextColored(x + 10, y, buf, rgb, palette, z);
        sprintf(buf, SUB_FMT_COUNT, SUB_DECK_EDIT.deckCounts[*(s16 *)SUB_CARD_LIST[i]]);
        drawTextColored(x + 0x10F, y, buf, rgb, palette, z);
        drawTinyTextColored(x + 0x118, y + 6, (u8 *)SUB_STR_CARDS, palette, rgb, z);
    }
    updateMenuCursor(&SUB_CARD_LIST_MENU);
    SUB_DECK_EDIT.cardId = *(s16 *)SUB_CARD_LIST[SUB_CARD_LIST_MENU.row];
    SUB_DECK_EDIT.card = SUB_CARD_LIST[SUB_CARD_LIST_MENU.row];
}

void SUB_drawPickerCardInfo(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 slot;
    s32 i;

    if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[SUB_DECK_EDIT.cardId] & 0x40) {
        SUB_CARD_IMAGE_CACHE.request = SUB_DECK_EDIT.cardId;
        slot = SUB_findCachedCardImage(SUB_DECK_EDIT.cardId);
        if (slot == -1) {
            if (++SUB_EDITOR.blink & 4) {
                SUB_drawSprite(x + 3, y + 14, 0x7E20, 0x200, 0x100, 0x40, 0x40, 0, 0x80, slot, z);
            } else {
                SUB_drawSprite(x + 3, y + 14, 0x7E20, 0x210, 0x100, 0x40, 0x40, 0, 0x80, -1, z);
            }
        } else {
            SUB_EDITOR.blink = 0;
            SUB_drawSprite(x + 3, y + 14, getClut(0, slot + 0x1F4), (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0x40, 0x40, 1, 0x80, -1, z);
        }
        sprintf(buf, SUB_FMT_CARD_NUMBER, *(s16 *)SUB_DECK_EDIT.card);
        drawText(x + 3, y, (s32)buf, 7, z);
        drawText(x + 0x7B, y, (s32)(SUB_DECK_EDIT.card + 3), 7, z);
        switch (*(s16 *)SUB_DECK_EDIT.card) {
        case 0xAF:
        case 0xB6:
        case 0xB7:
        case 0xB8:
        case 0xBB:
        case 0xBE:
            drawText(x + 0xCC, y, (s32)SUB_STR_PARTNER, 6, z);
            break;
        }
        switch (SUB_DECK_EDIT.card[2]) {
        case 0:
            drawIcon(x + 0x63, y, 0, ((DigimonCardData *)SUB_DECK_EDIT.card)->attr >> 4, z);
            drawIcon(x + 0x27, y, 0, (((DigimonCardData *)SUB_DECK_EDIT.card)->attr & 0xF) + 0x10, z);
            drawText(x + 0x1B, y, (s32)SUB_STR_LV, 7, z);
            drawText(x + 0x45, y, (s32)SUB_STR_TYPE, 7, z);
            sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_DECK_EDIT.card));
            drawText(x + 0xFD, y, (s32)buf, 7, z);
            drawText(x + 0x109, y, (s32)SUB_STR_CARDS, 7, z);
            drawIcon(x + 0x4B, y + 12, 0, 0x1A, z);
            sprintf(buf, SUB_FMT_4_DIGITS, ((DigimonCardData *)SUB_DECK_EDIT.card)->hp);
            drawText(x + 0x69, y + 12, (s32)buf, 7, z);
            for (i = 0; i < 3; i++) {
                sprintf(buf, "*b%d", i);
                drawText(x + 0x4B, y + (i + 2) * 12, (s32)buf, 7, z);
                sprintf(buf, SUB_FMT_4_DIGITS, ((DigimonCardData *)SUB_DECK_EDIT.card)->attack[i].power);
                drawText(x + 0x69, y + (i + 2) * 12, (s32)buf, 7, z);
            }
            drawSmallText(x + 0x57, y + 0x3E, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)SUB_DECK_EDIT.card)->crossEffect], 7, z);
            if (CROSS_EFFECT_ICONS[((DigimonCardData *)SUB_DECK_EDIT.card)->crossEffect] != 0) {
                drawIcon(x + 0x91, y + 0x45, 0, CROSS_EFFECT_ICONS[((DigimonCardData *)SUB_DECK_EDIT.card)->crossEffect] + 0x14, z);
            }
            drawIcon(x + 0x91, y + 12, 0, 0x18, z);
            sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_DECK_EDIT.card)->dpCost);
            drawText(x + 0xA3, y + 12, (s32)buf, 7, z);
            drawIcon(x + 0x91, y + 0x18, 0, 0x19, z);
            sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_DECK_EDIT.card)->dpBonus);
            drawText(x + 0xA3, y + 0x18, (s32)buf, 7, z);
            drawText(x + 0xB9, y + 12, (s32)SUB_STR_SUPPORT_EFFECT, 6, z);
            if (((DigimonCardData *)SUB_DECK_EDIT.card)->supportIcon != 0) {
                drawIcon(x + 0x10E, y + 12, 0, ((DigimonCardData *)SUB_DECK_EDIT.card)->supportIcon + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                drawText(x + 0xB9, y + (i + 2) * 12, (s32)((DigimonCardData *)SUB_DECK_EDIT.card)->supportText[i], 7, z);
            }
            break;
        case 1:
            drawIcon(x + 0x18, y, 0, 5, z);
            sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_DECK_EDIT.card));
            drawText(x + 0xFD, y, (s32)buf, 7, z);
            drawText(x + 0x109, y, (s32)SUB_STR_CARDS, 7, z);
            if (SUB_DECK_EDIT.card[0x8C] != 0) {
                drawIcon(x + 0x8D, y + 13, 0, SUB_DECK_EDIT.card[0x8C] + 0x14, z);
            }
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x8D, y, (s32)((u8 *)&((OptionCardData *)OPTION_CARDS)[*(s16 *)SUB_DECK_EDIT.card - 0xBF] + 0x8D + i * 21), 7, z);
                y += 12;
            }
            break;
        case 2:
            drawIcon(x + 0x18, y, 0, 6, z);
            sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_DECK_EDIT.card));
            drawText(x + 0xFD, y, (s32)buf, 7, z);
            drawText(x + 0x109, y, (s32)SUB_STR_CARDS, 7, z);
            y += 0x1A;
            for (i = 0; i < 4; i++) {
                drawText(x + 0x8D, y, (s32)((u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[*(s16 *)SUB_DECK_EDIT.card - 0x125] + 0x1B + i * 21), 7, z);
                y += 12;
            }
            break;
        }
    }
}

void SUB_drawDeckEditTitle(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 width;

    if (SUB_DECK_EDIT.mode == 1) {
        sprintf(buf, "%s Deck", SUB_EDITED_DECK->name);
        width = measureText(buf) - 0xA0;
        drawText(x - width, y + 1, (s32)buf, 7, z);
    } else {
        drawText(x + 0x30, y + 1, (s32)"Card Selection", 7, z);
    }
}

void SUB_drawDeckStats(UiWindow *window) {
    char buf[72];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 i;

    switch (SUB_DECK_EDIT.statsPage) {
    case 0:
        sprintf(SUB_LABEL_BUFFER, "         R1");
        break;
    case 1:
        sprintf(SUB_LABEL_BUFFER, "L1      ");
        break;
    }
    window->label = (s32)SUB_LABEL_BUFFER;
    if (SUB_DECK_EDIT.statsPage == 0) {
        s32 masks[7] = { 0x1, 0x2, 0x4, 0x8, 0x10, 0x40, 0x80 };

        for (i = 0; i < 7; i++) {
            drawIcon(x, y, 0, i, z);
            sprintf(buf, "%2d", countDeckCardsByFilter(SUB_EDITOR.player, SUB_EDITED_DECK, masks[i]));
            drawText(x + 0x15, y, (s32)buf, 7, z);
            drawTinyText(x + 0x23, y + 6, (s32)SUB_STR_CARDS, 7, z);
            y += 13;
        }
        drawTinyText(x, y + 6, (s32)SUB_STR_TOTAL, 7, z);
        sprintf(buf, "%2d", countDeckCardsByFilter(SUB_EDITOR.player, SUB_EDITED_DECK, 0xDF));
        drawText(x + 0x15, y, (s32)buf, 7, z);
        drawTinyText(x + 0x23, y + 6, (s32)SUB_STR_CARDS, 7, z);
    } else if (SUB_DECK_EDIT.statsPage == 1) {
        s32 masks[5] = { 0x200, 0x800, 0x1000, 0x40, 0x80 };

        for (i = 0; i < 5; i++) {
            if (i == 0) {
                drawIcon(x, y, 0, 0x10, z);
            } else if (i >= 3) {
                drawIcon(x, y, 0, i + 2, z);
            } else {
                drawIcon(x, y, 0, i + 0x11, z);
            }
            sprintf(buf, "%2d", countDeckCardsByFilter(SUB_EDITOR.player, SUB_EDITED_DECK, masks[i]));
            drawText(x + 0x15, y, (s32)buf, 7, z);
            drawTinyText(x + 0x23, y + 6, (s32)SUB_STR_CARDS, 7, z);
            y += 13;
        }
        y += 13;
        drawTinyText(x, y + 6, (s32)SUB_STR_TOTAL, 7, z);
        sprintf(buf, "%2d", countDeckCardsByFilter(SUB_EDITOR.player, SUB_EDITED_DECK, 0xDF));
        drawText(x + 0x15, y, (s32)buf, 7, z);
        drawTinyText(x + 0x23, y + 6, (s32)SUB_STR_CARDS, 7, z);
        y += 13;
        drawText(x, y, (s32)"Pa", 6, z);
        sprintf(buf, "%2d", countDeckCardsByFilter(SUB_EDITOR.player, SUB_EDITED_DECK, 0x20));
        drawText(x + 0x15, y, (s32)buf, 6, z);
        drawTinyText(x + 0x23, y + 6, (s32)SUB_STR_CARDS, 6, z);
    }
}

void SUB_drawDeckGrid(UiWindow *window) {
    s16 xs[4];
    s16 ys[4];
    char buf[72]; /* unused, but it is in the original stack frame */
    LINE_G3 *line = SUB_DECK_EDIT.cursorLines[FRAME_BUFFER_INDEX];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 i;
    u8 brightness;
    s16 col;
    s16 row;

    if (SUB_DECK_EDIT.mode == 1 || SUB_DECK_EDIT.mode == 2) {
        col = SUB_DECK_EDIT.slot % 10;
        xs[0] = x - 1 + col * 24;
        xs[1] = xs[0] + 25;
        row = SUB_DECK_EDIT.slot / 10;
        ys[0] = y - 1 + row * 24;
        ys[1] = ys[0] + 25;
        line->x0 = line->x1 = line->x2 = xs[0];
        line->y0 = line->y1 = ys[0];
        line->y2 = ys[1];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
        line++;
        line->x0 = line->x1 = xs[0];
        line->x2 = xs[1];
        line->y0 = line->y1 = line->y2 = ys[0];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
        line++;
        line->x0 = line->x1 = line->x2 = xs[1];
        line->y0 = line->y1 = ys[0];
        line->y2 = ys[1];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
        line++;
        line->x0 = line->x1 = xs[0];
        line->x2 = xs[1];
        line->y0 = line->y1 = line->y2 = ys[1];
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], line);
    }
    for (i = 0; i < 30; i++) {
        if (SUB_EDITED_DECK->cards[i].type != 0xFF) {
            if (SUB_DECK_EDIT.mode == 0) {
                brightness = 0x80;
            } else if (SUB_DECK_EDIT.slot == i) {
                brightness = 0x90;
            } else {
                brightness = 0x40;
            }
            SUB_drawCardIcon(getCardId(SUB_EDITED_DECK->cards[i].type, SUB_EDITED_DECK->cards[i].index), x + (i % 10) * 24, y + (i / 10) * 24, brightness, z);
        }
    }
}
