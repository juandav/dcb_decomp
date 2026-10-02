#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/sound_play.h"
#include "dcb/pad.h"
#include "dcb/nisseg.h"

/* The deck screens: the saved decks, each deck's menu (edit, auto deck,
   copy, rename, delete), the card grid of each kind with the copies of the
   card in view, the auto deck's questions and the deck name entry. Each
   screen is a task that runs a menu of the executable and spawns the next
   one */

extern u8 D_801E46E9;
extern s32 NIS_AUTO_DECK_FROM_MENU; /* 1 when the auto deck was asked for from a deck's menu */
extern s16 *NIS_ROOKIE_COUNTS; /* the Digimon owned of each element */
extern u8 (*NIS_DECK_BACKUP)[2]; /* the deck's cards before editing */
extern NisProfile *NIS_PROFILE_BACKUP; /* player 1's profile before a trade */
extern s32 MENU_ITEMS[]; /* player_data_jp.c's menu items, read here as words */
extern s32 DECK_KINDS_MENU;

void clearKanjiPage(s32 page);
void linkSavedDecks(s32);
NisCursor *KAW_createCursor(s32, s32, s32, s32, s32);
void KAW_freeCursor(NisCursor *cursor);
s32 NIS_countOwnedDigimonOf(s32 element, s32 level);
s32 NIS_countOwnedOptions(void);
void NIS_addCardsToDeck(s32 type, s32 index, s32 count);
void NIS_writeDeckCardCount(s32 delta);
s32 NIS_loadDeckTims(void);
void NIS_startDeckEditor(void);
void NIS_openViewerScene(void);
void NIS_closeViewerScene(void);
void NIS_runModelViewer(s32 model);
void NIS_drawDeckSummaryWindow(NisWindow *window);
void NIS_drawCardGrid(NisWindow *window);
void NIS_drawCardGridHelp(NisWindow *window);
void NIS_drawCardDetails(NisWindow *window);
void NIS_drawCardCountMenu(NisWindow *window);
void NIS_drawDeckRecord(NisWindow *window);
void NIS_drawDeckName(NisWindow *window);
void NIS_drawDeckQuestion(NisWindow *window);
void NIS_drawDeckNotFull(NisWindow *window);
void NIS_drawNameField(NisWindow *window);
void NIS_drawNameEntry(NisWindow *window);
void NIS_drawDeckCopyQuestion(NisWindow *window);
void NIS_drawAutoDeckQuestion(NisWindow *window);
void NIS_drawAutoDeckPortrait(NisWindow *window);
void NIS_keepTradeCopies();
void NIS_runTradeGrid(s32 kind);
void NIS_runVsMode(void);
extern void returnToAreaFromDeckEditor();
extern void openMemcardScreenAfterDeckEdit();

void NIS_runDeckEditor(s32 openWindows);
void NIS_askAutoDeckStyle(void);
void NIS_askAutoDeckOptions(void);
void NIS_showTooFewCards(void);
void NIS_askAutoDeckForNew(void);
void NIS_confirmDeckCopy(void);
void NIS_askNewDeckAuto(void);
void NIS_startDeckKinds(void);
void NIS_showCard(s32 kind);
void NIS_runCardGrid(s32 kind);
void NIS_runDeckKinds(s32 mode);
void NIS_backUpDeck(void);
void NIS_runDeckMenu(s32 mode);
void NIS_reopenDeckMenu(void);
void NIS_runDeckList(s32 enter);
void NIS_enterDeckList(void);
void NIS_returnToDeckList(void);
s8 NIS_isNameBlank(char *name);
s32 NIS_changeCopies(s32 decrease, s32 increase, s8 *copies, s8 max);

NisWindowDef NIS_DECK_SUMMARY_WINDOW = { { 0x122, 0x30, 0, 0xC }, { 0x86, 0x30, 0x9C, 0xAA }, 0xA, 1, NIS_drawDeckSummaryWindow, NIS_closeDeckWindow };
NisWindowDef NIS_CARD_GRID_WINDOW = { { 0x122, 0x30, 0, 0xC }, { 0x12, 0x30, 0x110, 0x94 }, 0xA, 1, NIS_drawCardGrid, NIS_closeDeckWindow };
NisWindowDef NIS_GRID_HELP_WINDOW = { { 0x1A, 0xD0, 0, 0xC }, { 0x1A, 0xD0, 0x8C, 0xC }, 0xA, 0, NIS_drawCardGridHelp, NIS_closeDeckWindow };
NisWindowDef NIS_CARD_DETAILS_WINDOW = { { 0x122, 0x30, 0, 0xC }, { 0x7A, 0x30, 0xA8, 0xAA }, 0xA, 1, NIS_drawCardDetails, NIS_closeDeckWindow };
NisWindowDef NIS_CARD_COUNT_WINDOW = { { 0x1A, 0x30, 0, 0xC }, { 0x1A, 0x30, 0x54, 0xAA }, 0xA, 0, NIS_drawCardCountMenu, NIS_closeDeckWindow };
NisWindowDef NIS_DECK_RECORD_WINDOW = { { 0x1A, 0x17, 0, 0xC }, { 0x1A, 0x17, 0x55, 0xC }, 0xA, 0, NIS_drawDeckRecord, NIS_closeDeckWindow };
NisWindowDef NIS_DECK_NAME_WINDOW = { { 0x122, 0x17, 0, 0xC }, { 0x7A, 0x17, 0xA8, 0xC }, 0xA, 1, NIS_drawDeckName, NIS_closeDeckWindow };
NisWindowDef NIS_DECK_QUESTION_WINDOW = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 0xA, 0, NIS_drawDeckQuestion, NIS_closeDeckWindow };
NisWindowDef NIS_DECK_NOT_FULL_WINDOW = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 0xA, 0, NIS_drawDeckNotFull, NIS_closeDeckWindow };
NisWindowDef NIS_NAME_FIELD_WINDOW = { { 0x1A, 0x36, 0, 0xC }, { 0x1A, 0x36, 0x54, 0xC }, 0xA, 0, NIS_drawNameField, NIS_closeDeckWindow };
NisWindowDef NIS_NAME_ENTRY_WINDOW = { { 0x1A, 0x4E, 0, 0xC }, { 0x1A, 0x4E, 0xFC, 0x8A }, 0xA, 0, NIS_drawNameEntry, NIS_closeDeckWindow };
NisWindowDef NIS_DECK_COPY_WINDOW = { { 0x17, 0x98, 0, 0xC }, { 0x17, 0x98, 0x112, 0x43 }, 0xA, 0, NIS_drawDeckCopyQuestion, NIS_closeDeckWindow };
NisWindowDef NIS_AUTO_DECK_WINDOW = { { 0x1A, 0xA0, 0, 0xC }, { 0x1A, 0xA0, 0x112, 0x3B }, 0xA, 0, NIS_drawAutoDeckQuestion, NIS_closeDeckWindow };
NisWindowDef NIS_AUTO_DECK_PORTRAIT_WINDOW = { { 0x126, 0x4F, 0, 0x10 }, { 0xE6, 0x4F, 0x40, 0x38 }, 0xA, 1, NIS_drawAutoDeckPortrait, NIS_closeDeckWindow };

#if JP_DEBUG_BUILD
void NIS_closeDeckScene(void);

NisWindowTest NIS_WINDOW_TESTS[4] = {
    { &NIS_DECK_SUMMARY_WINDOW, NIS_resumeParentTask, NULL },
    { &NIS_CARD_DETAILS_WINDOW, NIS_initCardDetailsTest, NIS_closeDeckScene },
    { &NIS_DECK_NAME_WINDOW, NIS_initDeckNameTest, NULL },
    { &NIS_DECK_QUESTION_WINDOW, NULL, NULL },
};
#endif

/* the auto deck's last question: build it? */
void NIS_confirmAutoDeck(void) {
    NisMenu menu;
    s32 cursor;

    cursor = -1;
    NIS_AUTO_DECK_QUESTION = 3;
    openChoiceMenu(&menu, 0x1B, 0x32, NULL, &cursor);
    addChoiceMenuItem(&menu, 0xB, NIS_startDeckKinds);
    addChoiceMenuItem(&menu, 0xC, NIS_startDeckEditor);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.choice == 0) {
            NIS_buildAutoDeck();
            NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = NIS_WINDOW(NIS_DECK_SCREENS.nameWindow)->state = 4;
            waitFrames(0x3C);
        }
        startChoiceMenuAction(&menu);
    }
}

/* the auto deck's third question: attack or defense? */
void NIS_askAutoDeckStyle(void) {
    NisMenu menu;
    s16 options;

    NIS_AUTO_DECK_QUESTION = 2;
    options = NIS_countOwnedOptions();
    openChoiceMenu(&menu, 0x1B, 0x32, NIS_askAutoDeckOptions, NULL);
    addChoiceMenuItem(&menu, 0x25, (options >= 10) ? NIS_confirmAutoDeck : NIS_showTooFewCards);
    addChoiceMenuItem(&menu, 0x26, NIS_confirmAutoDeck);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        NIS_DECK_EDIT.unkA = menu.choice;
        startChoiceMenuAction(&menu);
    }
}

/* the auto deck's second question: many option cards or few? */
void NIS_askAutoDeckOptions(void) {
    NisMenu menu;
    s32 cursor;

    cursor = 0;
    NIS_AUTO_DECK_QUESTION = 1;
    openChoiceMenu(&menu, 0x1B, 0x32, NIS_runDeckEditor, &cursor);
    addChoiceMenuItem(&menu, 0x23, NIS_askAutoDeckStyle);
    addChoiceMenuItem(&menu, 0x24, NIS_askAutoDeckStyle);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        NIS_DECK_EDIT.cardIndex = menu.choice;
        startChoiceMenuAction(&menu);
    }
}

/* counts the level R Digimon owned of each element */
void NIS_countOwnedRookies(void) {
    NIS_ROOKIE_COUNTS[3] = NIS_ROOKIE_COUNTS[4] = NIS_ROOKIE_COUNTS[5] = NIS_ROOKIE_COUNTS[0] = NIS_ROOKIE_COUNTS[1] = NIS_ROOKIE_COUNTS[2] = 0;
    NIS_ROOKIE_COUNTS[0] = NIS_countOwnedDigimonOf(0, 0);
    NIS_ROOKIE_COUNTS[1] = NIS_countOwnedDigimonOf(1, 0);
    NIS_ROOKIE_COUNTS[2] = NIS_countOwnedDigimonOf(2, 0);
    NIS_ROOKIE_COUNTS[3] = NIS_countOwnedDigimonOf(3, 0);
    NIS_ROOKIE_COUNTS[4] = NIS_countOwnedDigimonOf(4, 0);
}

/* "not enough cards" for the auto deck, then back to the question */
void NIS_showTooFewCards(void) {
    NisMenu menu;

    openChoiceMenu(&menu, 0xB, 0x32, (NIS_AUTO_DECK_QUESTION == 2) ? NIS_askAutoDeckStyle : NIS_runDeckEditor, NULL);
    addChoiceMenuItem(&menu, 0xB, (NIS_AUTO_DECK_QUESTION == 2) ? NIS_askAutoDeckStyle : NIS_runDeckEditor);
    if (NIS_AUTO_DECK_QUESTION == 2) {
        NIS_AUTO_DECK_QUESTION = 0xB;
    } else {
        NIS_AUTO_DECK_QUESTION = 0xA;
    }
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        startChoiceMenuAction(&menu);
    }
}

/* the auto deck's first question: which element? Only the elements with 6
   level R Digimon or more can be the main one */
void NIS_runDeckEditor(s32 openWindows) {
    NisMenu menu;
    s32 cursor;
    s32 result;

    cursor = 1;
    NIS_AUTO_DECK_QUESTION = 0;
    if (openWindows) {
        clearKanjiPage(0xF);
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_AUTO_DECK_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_AUTO_DECK_PORTRAIT_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.nameWindow = waitFrames(0x7FFFFFFF);
    }
    NIS_ROOKIE_COUNTS = NIS_ALLOC_HEAP_BLOCK(0xC, 0x19A, "BTL_GOLD", 5);
    NIS_countOwnedRookies();
    openChoiceMenu(&menu, 0x1B, 0x32, (NIS_AUTO_DECK_FROM_MENU == 0) ? NIS_askAutoDeckForNew : NIS_runDeckMenu, &cursor);
    addChoiceMenuItem(&menu, 0x1C, (NIS_ROOKIE_COUNTS[0] >= 6) ? NIS_askAutoDeckOptions : NIS_showTooFewCards);
    addChoiceMenuItem(&menu, 0x1D, (NIS_ROOKIE_COUNTS[1] >= 6) ? NIS_askAutoDeckOptions : NIS_showTooFewCards);
    addChoiceMenuItem(&menu, 0x1E, (NIS_ROOKIE_COUNTS[2] >= 6) ? NIS_askAutoDeckOptions : NIS_showTooFewCards);
    addChoiceMenuItem(&menu, 0x1F, (NIS_ROOKIE_COUNTS[3] >= 6) ? NIS_askAutoDeckOptions : NIS_showTooFewCards);
    addChoiceMenuItem(&menu, 0x20, (NIS_ROOKIE_COUNTS[4] >= 6) ? NIS_askAutoDeckOptions : NIS_showTooFewCards);
    freeHeapBlocksByTag(0x19A);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        result = runChoiceMenu(&menu);
        if (result == 0) {
            continue;
        }
        if (result < 0) {
            NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = NIS_WINDOW(NIS_DECK_SCREENS.nameWindow)->state = 4;
            waitFrames(0x1E);
        }
        NIS_DECK_EDIT.cardType = menu.choice;
        startChoiceMenuAction(&menu);
    }
}

/* a new deck: make it with the auto deck? */
void NIS_askAutoDeckForNew(void) {
    NisMenu menu;
    s32 cursor = 1;
    Rect16 unused = { 0x3C0, 0xD0, 0, 0 }; /* unused, but it is in the original stack frame */

    NIS_DECK_EDIT.unk10 = 3;
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_QUESTION_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
    openChoiceMenu(&menu, 0x1B, 0x32, NULL, &cursor);
    addChoiceMenuItem(&menu, 0xB, NIS_runDeckEditor);
    addChoiceMenuItem(&menu, 0xC, NIS_startDeckKinds);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
        waitFrames(0x1E);
        if (menu.choice == 1) {
            cursor = -1;
            NIS_DEBUG_PRINT("CHECK1 !!\n");
        }
        startChoiceMenuAction(&menu);
    }
}

void NIS_showKanaPage(s32 page) {
    s32 i;
    u8 unused[8]; /* unused, but it is in the original stack frame */

    /* what it did is gone: an empty loop is left */
    for (i = 0; i < 0; i++) {
    }
}

void NIS_doNothing(void) {
    u8 unused[8]; /* unused, but it is in the original stack frame */

    return;
}

/* puts the kana at the cursor of the name entry's page (or erases the last
   one) as the name's next character */
void NIS_typeKana(char *name, s32 page, s32 column, s32 row, u8 erase) {
    char letter[3];
    s32 offset;

    if (column < 5) {
        offset = row * 11 + column * 2;
    } else {
        offset = (row + 9) * 11 + (column - 5) * 2;
    }
    /* a page of NIS_KANA is 18 rows of 11 bytes, 0xC6 */
    if (!erase) {
        letter[0] = name[NIS_DECK_EDIT.cardType * 2] = NIS_KANA[0][0][page * 0xC6 + offset];
        letter[1] = name[NIS_DECK_EDIT.cardType * 2 + 1] = NIS_KANA[0][0][page * 0xC6 + offset + 1];
        letter[2] = name[NIS_DECK_EDIT.cardType * 2 + 2] = 0;
    } else {
        letter[0] = name[NIS_DECK_EDIT.cardType * 2] = 0;
    }
    strcpy(NIS_TYPED_NAME, name);
    NIS_DEBUG_PRINT("CX   = %d\n", column);
    NIS_DEBUG_PRINT("CY   = %d\n", row);
    NIS_DEBUG_PRINT("FT   = %d\n", page);
    NIS_DEBUG_PRINT("NO   = %d\n", offset);
    NIS_DEBUG_PRINT("DNUM = %d\n", NIS_DECK_EDIT.cardType);
}

/* the row the cursor goes to when it moves left onto the commands */
s32 NIS_commandRowOf(s32 row) {
    if (row < 4) {
        return row;
    }
    if (row < 6) {
        return 4;
    }
    if (row < 7) {
        return 5;
    }
    return 6;
}

/* the row the cursor goes to when it moves right off the commands */
s32 NIS_kanaRowOf(s32 row) {
    if (row < 5) {
        return row;
    }
    if (row < 6) {
        return 6;
    }
    return 7;
}

/* 1 if the name is empty or only full-width spaces */
s8 NIS_isNameBlank(char *name) {
    char *c;
    s8 i;

    if (strlen(name) == 0) {
        return 1;
    }
    for (i = 0; i < 12; i += 2) {
        c = &name[i];
        /* a full-width space: 0x81 0x40 in Shift JIS */
        if ((u8)c[0] == 0x81 && (u8)c[1] == 0x40) {
            if (((s8 *)c)[2] == 0) {
                return 1;
            }
        } else {
            return 0;
        }
    }
    return 1;
}

/* the deck name entry: the cursor moves over a page of kana (columns 0-9)
   and the commands (column 10: the pages, back and done). Returns 0 for
   back and 1 for done */
s32 NIS_enterDeckName(char *name, s8 newDeck) {
    s32 row;
    s32 column;
    s8 blank;

    NIS_KANA_PAGE = 0;
    NIS_DECK_EDIT.copies = NIS_DECK_EDIT.unkA = NIS_DECK_EDIT.cardType = column = row = 0;
    if (newDeck == 0) {
        SCROLLING_BACKGROUND->unk1C0 = 0x1A;
        strcpy(name, NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name);
        NIS_DECK_EDIT.cardType = strlen(name) / 2;
    } else {
        SCROLLING_BACKGROUND->unk1C0 = 0x39;
    }
    strcpy(NIS_TYPED_NAME, name);
    NIS_NAME_CURSOR = KAW_createCursor(1, 6, 6, 4, 1);
    clearKanjiPage(0xF);
    waitFrames(0x1E);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_NAME_FIELD_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.lowerWindow = waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_NAME_ENTRY_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.helpWindow = waitFrames(0x7FFFFFFF);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
#if JP_DEBUG_BUILD
        sprintf(DEBUG_TEXT_LINES[4], "STR %d\n", strlen(name));
        sprintf(DEBUG_TEXT_LINES[5], "POS %d\n", newDeck);
        sprintf(DEBUG_TEXT_LINES[6], "SIZE =  %d\n", NIS_DECK_EDIT.cardType);
#endif
        if (NIS_REPEATED() & PAD_CIRCLE) {
            blank = NIS_isNameBlank(name);
            if (column < 10) {
                playSoundEffect(0);
                if (NIS_DECK_EDIT.cardType < 6) {
                    NIS_typeKana(name, NIS_KANA_PAGE, column, row, 0);
                    if (NIS_DECK_EDIT.cardType == 5) {
                        column = 10;
                        row = 6;
                    }
                    if (++NIS_DECK_EDIT.cardType >= 6) {
                        NIS_DECK_EDIT.cardType = 6;
                    }
                } else {
                    column = 10;
                    row = 6;
                }
            } else {
                clearKanjiPage(0xF);
                if (row >= 0) {
                    if (row < 5) {
                        playSoundEffect(0);
                        NIS_KANA_PAGE = row;
                        NIS_showKanaPage(row);
                    } else if (row < 7 && !(blank == 1 && row == 6)) {
                        playSoundEffect(0);
                        NIS_WINDOW(NIS_DECK_SCREENS.lowerWindow)->state = 4;
                        NIS_WINDOW(NIS_DECK_SCREENS.helpWindow)->state = 4;
                        waitFrames(0x24);
                        KAW_freeCursor(NIS_NAME_CURSOR);
                        NIS_NAME_CURSOR = NULL;
                        return row - 5;
                    }
                }
            }
        } else if (NIS_REPEATED() & PAD_CROSS) {
            playSoundEffect(1);
            if (--NIS_DECK_EDIT.cardType < 0) {
                NIS_DECK_EDIT.cardType = 0;
                column = 10;
                row = 5;
            }
            NIS_typeKana(name, 0, 1, 7, 1);
        } else if (NIS_REPEATED() & PAD_LEFT) {
            playSoundEffect(2);
            if (--column < 0) {
                column = 10;
                row = NIS_commandRowOf(row);
            } else if (column == 9) {
                row = NIS_kanaRowOf(row);
            }
        } else if (NIS_REPEATED() & PAD_RIGHT) {
            playSoundEffect(2);
            if (++column == 10) {
                row = NIS_commandRowOf(row);
            }
            if (column >= 11) {
                column = 0;
                row = NIS_kanaRowOf(row);
            }
        } else if (NIS_REPEATED() & PAD_UP) {
            playSoundEffect(2);
            if (--row < 0) {
                row = 8;
                if (column == 10) {
                    row = 6;
                }
            }
        } else if (NIS_REPEATED() & PAD_DOWN) {
            playSoundEffect(2);
            if (++row >= ((column == 10) ? 7 : 9)) {
                row = 0;
            }
        } else if (NIS_REPEATED() & PAD_START) {
            playSoundEffect(0);
            column = 10;
            row = 6;
        }
        NIS_DECK_EDIT.copies = column;
        NIS_DECK_EDIT.unkA = row;
    }
}

/* clears a saved deck's record */
void NIS_clearDeckRecord(s8 deck) {
    s8 i;

    linkSavedDecks(0);
    NIS_PROFILE(0)->savedDecks[deck].wins = NIS_PROFILE(0)->savedDecks[deck].losses = 0;
    for (i = 0; i < 3; i++) {
        NIS_PROFILE(0)->savedDecks[deck].unk104[i] = 0;
    }
}

/* renames the deck: a new name clears its record */
void NIS_renameDeck(void) {
    char name[0x10];
    char oldName[0x10];

    strcpy(oldName, NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name);
    if (NIS_enterDeckName(name, 0) > 0 && strcmp(name, oldName) != 0) {
        strcpy(NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name, name);
        NIS_clearDeckRecord(NIS_DECK_EDIT.deck);
    }
    spawnTask(0, -1, 0, 0x1000, NIS_reopenDeckMenu);
    exitTask();
}

/* copies the deck to the first free slot, under a new name */
void NIS_copyDeck(void) {
    char name[0x10];
    s32 slot;
    s8 i;

    if (NIS_enterDeckName(name, 0) > 0) {
        slot = NIS_countSavedDecks();
        NIS_clearDeckRecord(slot);
        NIS_PROFILE(0)->savedDecks[slot] = NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck];
        NIS_PROFILE(0)->savedDecks[slot].wins = NIS_PROFILE(0)->savedDecks[slot].losses = 0;
        for (i = 0; i < 3; i++) {
            NIS_PROFILE(0)->savedDecks[slot].unk104[i] = 0;
        }
        strcpy(NIS_PROFILE(0)->savedDecks[slot].name, name);
        NIS_PROFILE(0)->savedDecks[slot].inUse = 0;
        NIS_confirmDeckCopy();
    }
    spawnTask(0, -1, 0, 0x1000, NIS_reopenDeckMenu);
    exitTask();
}

/* keep the copy? */
void NIS_confirmDeckCopy(void) {
    NisMenu menu;
    Rect16 unused = { 0x3C0, 0xD0, 0, 0 }; /* unused, but it is in the original stack frame */
    char unused2[0x30]; /* unused, but it is in the original stack frame */
    s32 cursor;

    cursor = -1;
    clearKanjiPage(0xF);
    NIS_DECK_EDIT.unk10 = 1;
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_COPY_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.nameWindow = waitFrames(0x7FFFFFFF);
    openChoiceMenu(&menu, 0x1A, 0x32, NULL, &cursor);
    addChoiceMenuItem(&menu, 0xB, NIS_reopenDeckMenu);
    addChoiceMenuItem(&menu, 0xC, NIS_reopenDeckMenu);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.choice == 0) {
            NIS_PROFILE(0)->savedDecks[NIS_countSavedDecks()].inUse = 1;
        }
        NIS_WINDOW(NIS_DECK_SCREENS.nameWindow)->state = 4;
        waitFrames(0x1E);
        startChoiceMenuAction(&menu);
    }
}

/* a new deck: its name first */
void NIS_nameNewDeck(void) {
    char name[0x10];
    NisMenu menu;

    openChoiceMenu(&menu, 0x1A, 0x32, NULL, NULL);
    NIS_DECK_EDIT.deck = (NIS_DECK_EDIT.deck < 0) ? -NIS_DECK_EDIT.deck : NIS_DECK_EDIT.deck;
    sprintf(NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name, "");
    if (NIS_enterDeckName(name, 0) > 0) {
        strcpy(NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name, name);
        spawnTask(0, -1, 0, 0x1000, NIS_askNewDeckAuto);
    } else {
        spawnTask(0, -1, 0, 0x1000, NIS_enterDeckList);
    }
    exitTask();
}

/* a new deck, empty: make it with the auto deck? */
void NIS_askNewDeckAuto(void) {
    NisMenu menu;
    Rect16 unused = { 0x3C0, 0xD0, 0, 0 }; /* unused, but it is in the original stack frame */
    char unused2[0x30]; /* unused, but it is in the original stack frame */
    s32 cursor;
    s32 i;

    cursor = -1;
    NIS_DECK_EDIT.unk10 = 2;
    for (i = 0; i < 30; i++) {
        NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].cards[i].type = 0xFF;
    }
    NIS_DECK_EDIT.cardCount = 0;
    NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].wins = NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].losses = 0;
    for (i = 0; i < 3; i++) {
        NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].unk104[i] = 0;
    }
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_QUESTION_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
    openChoiceMenu(&menu, 0x1A, 0x32, NULL, &cursor);
    addChoiceMenuItem(&menu, 0xB, NIS_askAutoDeckForNew);
    addChoiceMenuItem(&menu, 0xC, NIS_nameNewDeck);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        if (menu.choice == 0) {
            NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].inUse = 1;
        }
        NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
        waitFrames(0x1E);
        startChoiceMenuAction(&menu);
    }
}

/* deletes the deck: the ones after it move up */
#if JP_DEBUG_BUILD
/* "command taken": a named string, since both branches that print it load
   its %hi and objdiff can't pair them with one %lo */
const char NIS_FMT_COMMAND_TAKEN[] = "コマンド受理 %d\n";
#endif

void NIS_deleteDeck(void) {
    NisMenu menu;
    Rect16 unused = { 0x3C0, 0xD0, 0, 0 }; /* unused, but it is in the original stack frame */
    char unused2[0x30]; /* unused, but it is in the original stack frame */
    s32 result;
    s32 i;

    NIS_DECK_EDIT.unk10 = 0;
    clearKanjiPage(0xF);
    strcpy(NIS_TYPED_NAME, NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].name);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_QUESTION_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
    openChoiceMenu(&menu, 0x18, 0x32, NIS_reopenDeckMenu, NULL);
    addChoiceMenuItem(&menu, 0xB, NIS_enterDeckList);
    addChoiceMenuItem(&menu, 0xC, NIS_reopenDeckMenu);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        result = runChoiceMenu(&menu);
        if (result == 0) {
            continue;
        }
        if (menu.choice == 0 && result != -1) {
            NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].inUse = 0;
            for (i = NIS_DECK_EDIT.deck; i < 2; i++) {
                if (NIS_PROFILE(0)->savedDecks[i + 1].inUse) {
                    NIS_PROFILE(0)->savedDecks[i] = NIS_PROFILE(0)->savedDecks[i + 1];
                    NIS_PROFILE(0)->savedDecks[i + 1].inUse = 0;
                }
            }
        }
        NIS_DEBUG_PRINT(NIS_FMT_COMMAND_TAKEN, result);
        NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
        waitFrames(0x1E);
        startChoiceMenuAction(&menu);
    }
}

void NIS_startDeckKinds(void) {
    spawnTask(0, -1, 0, 0x1000, NIS_runDeckKinds, 100);
}

/* "the deck isn't full" */
void NIS_showDeckNotFull(void) {
    NisMenu menu;
    Rect16 unused = { 0x3C0, 0xD0, 0, 0 }; /* unused, but it is in the original stack frame */
    s32 cursor;

    cursor = -1;
    clearKanjiPage(0xF);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_NOT_FULL_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
    openChoiceMenu(&menu, 0xB, 0x32, NULL, &cursor);
    addChoiceMenuItem(&menu, 0xB, NIS_startDeckKinds);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if (runChoiceMenu(&menu) == 0) {
            continue;
        }
        NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
        waitFrames(0x1E);
        startChoiceMenuAction(&menu);
    }
}

/* shows the card's Digimon in the model viewer, then back to the card */
void NIS_viewCardModel(void) {
    clearKanjiPage(0xF);
    closeKanjiPage(0xF);
    openKanjiPage(0x15, 0x1B9);
    clearKanjiPage(0x15);
    NIS_openViewerScene();
    NIS_runModelViewer(NIS_DIGIMON_CARDS[NIS_DECK_EDIT.cardIndex].unkD3);
    NIS_closeViewerScene();
    if (D_801E46E8 != 1) {
        NIS_DECK_SCREENS.nameWindow = 0;
    } else {
        NIS_TRADE_SCREENS.titleWindow = 0;
    }
    NIS_DECK_EDIT.fromViewer = 1;
    spawnTask(0, -1, 0, 0x1000, NIS_showCard, NIS_DECK_EDIT.kind);
    clearKanjiPage(0x15);
    closeKanjiPage(0x15);
    openKanjiPage(0xF, 0x1B9);
    clearKanjiPage(0xF);
    exitTask();
}

/* the big card picture: two picture slots in VRAM and, for each frame
   buffer, its front, back and turned polygons */
void NIS_initCardImage(void) {
    POLY_FT4 *poly;
    s32 i;
    s32 j;

    NIS_CARD_IMAGE.loaded = 0;
    for (i = 0; i < 4; i++) {
        NIS_CARD_IMAGE.slots[i].cardId = -1;
        NIS_CARD_IMAGE.slots[i].x = ((i & 1) << 5) + 0x180;
        NIS_CARD_IMAGE.slots[i].y = ((i & 2) << 5) + 0x175;
        NIS_CARD_IMAGE.slots[i].clutX = 0;
        NIS_CARD_IMAGE.slots[i].clutY = i + 0x1FE;
    }
    for (i = 0; i < 2; i++) {
        DB(i).primSlots[2] = (s32)(NIS_CARD_IMAGE.polys[i] = NIS_ALLOC_HEAP_BLOCK(0x78, 0x3A, "DETIAL", 2));
    }
    for (j = 0; j < 2; j++) {
        poly = NIS_CARD_IMAGE.polys[j];
        for (i = 0; i < 3; i++, poly++) {
            SetPolyFT4(poly);
            poly->r0 = 0x80;
            poly->g0 = 0x80;
            poly->b0 = 0x80;
        }
    }
    NIS_CARD_IMAGE.current = NIS_CARD_IMAGE.slots;
}

/* loads the picture of the card in view, unless a slot has it; get is
   NIS_showCard's -1, which only the debug build prints */
void NIS_loadCardImage(s32 get) {
    char path[0x40];
    s32 id;
    s32 i;
    u32 *tim;

    id = NIS_getCardId(NIS_DECK_EDIT.cardType, NIS_DECK_EDIT.cardIndex);
    NIS_DEBUG_PRINT("get = %d\n", get);
    NIS_initCardImage();
    NIS_CARD_IMAGE.turn = 0;
    NIS_CARD_IMAGE.back = 0;
    NIS_CARD_IMAGE.delay = 0x3C;
    for (i = 0; i < 1; i++) {
        if (NIS_CARD_IMAGE.current->cardId != id) {
            if (NIS_CARD_IMAGE.current != &NIS_CARD_IMAGE.slots[1]) {
                NIS_CARD_IMAGE.current++;
            } else {
                NIS_CARD_IMAGE.current = NIS_CARD_IMAGE.slots;
            }
        } else {
            NIS_CARD_IMAGE.loaded = 1;
        }
    }
    if (NIS_CARD_IMAGE.loaded == 0) {
        NIS_CARD_IMAGE.current->cardId = id;
        NIS_CARD_IMAGE.loaded = 1;
        sprintf(path, "B:\\L_CARD\\LC%3.3d.TIM", id);
#if JP_DEBUG_BUILD
        spawnTask(0, -1, 4, 0x800, loadFile, path, getCurrentTaskId());
#else
        spawnTask(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
#endif
        tim = (u32 *)waitFrames(0x7FFFFFFF);
        uploadTim(tim, NIS_CARD_IMAGE.current->x, NIS_CARD_IMAGE.current->y, NIS_CARD_IMAGE.current->clutX, NIS_CARD_IMAGE.current->clutY);
        DrawSync(0);
        freeHeapBlock(tim);
    }
    NIS_CARD_IMAGE.loaded = 0;
}

/* which of the buttons either player repeated (each masked apart) */
#define REPEATED_OF(buttons) (((buttons) & PAD_STATES[0]->rawRepeat) | ((buttons) & PAD_STATES[NIS_STATE->otherPad]->rawRepeat))

/* takes a copy out with the decrease buttons or puts one in with the
   increase ones (at most max, 4 and a full deck); returns 1 if it did */
s32 NIS_changeCopies(s32 decrease, s32 increase, s8 *copies, s8 max) {
    if (REPEATED_OF(decrease) && *copies > 0) {
        (*copies)--;
        NIS_writeDeckCardCount(-1);
        playSoundEffect(1);
        return 1;
    }
    if (D_801E46E8 != 1) {
        if (NIS_DECK_EDIT.cardCount < 30 && *copies < 4 && REPEATED_OF(increase) && *copies < max) {
            (*copies)++;
            NIS_writeDeckCardCount(1);
            playSoundEffect(0);
            return 1;
        }
    } else if (REPEATED_OF(increase) && *copies < max) {
        (*copies)++;
        NIS_writeDeckCardCount(1);
        playSoundEffect(0);
        return 1;
    }
    return 0;
}

/* the card in view: its picture, its details and the copies in the deck;
   Circle keeps the copies, Cross puts them back, Triangle shows the
   Digimon's model */
void NIS_showCard(s32 kind) {
    s32 copies;
    s32 viewer;

    copies = NIS_DECK_EDIT.copies;
    viewer = 0;
    if (D_801E46E8 != 1) {
        NIS_DECK_EDIT.maxCopies = NIS_DECK_EDIT.unkA;
    }
    NIS_DECK_EDIT.kind = kind;
    if (D_801E46E8 != 1) {
        if (NIS_DECK_EDIT.fromViewer == 1) {
            NIS_DECK_EDIT.fromViewer = 0;
            copies = NIS_DECK_EDIT.savedCopies;
        } else {
            NIS_DECK_EDIT.savedCopies = NIS_DECK_EDIT.copies;
        }
    }
    NIS_DEBUG_PRINT("mode = %d\n", kind);
    NIS_DEBUG_PRINT("deck = %d\n", NIS_DECK_EDIT.deck);
    NIS_DEBUG_PRINT("new = %d\n", NIS_DECK_EDIT.copies);
    NIS_DEBUG_PRINT("old = %d\n", copies);
    clearKanjiPage(0xF);
    spawnTask(0, -1, 0, 0x1000, NIS_loadCardImage, -1);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_CARD_DETAILS_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_CARD_COUNT_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.lowerWindow = waitFrames(0x7FFFFFFF);
    if (NIS_DECK_EDIT.cardType == 0 && NIS_PROFILE(0)->showsRecords) {
        NIS_CARD_IN_VIEW = 1;
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_RECORD_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.motionWindow = waitFrames(0x7FFFFFFF);
    }
    if (D_801E46E8 != 1 && NIS_DECK_SCREENS.nameWindow == 0) {
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_NAME_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.nameWindow = waitFrames(0x7FFFFFFF);
    }
    waitFrames(0x5A);
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
#if JP_DEBUG_BUILD
        sprintf(DEBUG_TEXT_LINES[0], "FLAG = %d \n", viewer);
#endif
        if (NIS_CARD_IMAGE.loaded) {
            continue;
        }
        if (NIS_PRESSED() & PAD_CROSS) {
            if (D_801E46E8 != 1) {
                NIS_writeDeckCardCount(copies - NIS_DECK_EDIT.copies);
            }
            break;
        }
        if (NIS_PRESSED() & PAD_CIRCLE) {
            if (D_801E46E8 != 1) {
                NIS_addCardsToDeck(NIS_DECK_EDIT.cardType, NIS_DECK_EDIT.cardIndex, NIS_DECK_EDIT.copies - copies);
            } else {
                NIS_keepTradeCopies();
            }
            break;
        }
        if ((NIS_PRESSED() & PAD_TRIANGLE) && NIS_DECK_EDIT.cardType == 0) {
            viewer = 1;
            NIS_DEBUG_NAME_TASK(0, "MODEL_VIEW");
            spawnTask(0, -1, 0, 0x1000, NIS_viewCardModel, 0, getCurrentTaskId(), 0, 0);
            break;
        }
        NIS_changeCopies(PAD_LEFT | PAD_L1, PAD_RIGHT | PAD_R1, &NIS_DECK_EDIT.copies, NIS_DECK_EDIT.maxCopies);
    }
    NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
    NIS_WINDOW(NIS_DECK_SCREENS.lowerWindow)->state = 4;
    if (NIS_DECK_EDIT.cardType == 0 && NIS_PROFILE(0)->showsRecords) {
        NIS_WINDOW(NIS_DECK_SCREENS.motionWindow)->state = 4;
    }
    if (viewer == 1 && D_801E46E8 != viewer) {
        NIS_WINDOW(NIS_DECK_SCREENS.nameWindow)->state = 4;
    }
    waitFrames(0x3C);
    freeHeapBlocksByTag(0x3A);
    if (viewer == 0) {
        if (D_801E46E8 != 1) {
            NIS_DEBUG_PRINT("CARD SELECT\n");
            NIS_DEBUG_NAME_TASK(0, "CARD_SELECT");
            spawnTask(0, -1, 0, 0x1000, NIS_runCardGrid, kind, 0, 0, 0);
        } else {
            NIS_DEBUG_NAME_TASK(0, "SELECT");
            spawnTask(0, -1, 0, 0x800, NIS_runTradeGrid, kind, 0, 0, 0);
        }
    }
    exitTask();
}

/* the card grid of a kind (or of the deck): Circle shows a card, L1 and R1
   take a copy out or put one in */
void NIS_runCardGrid(s32 kind) {
    NisMenu menu;
    NisCardList *list;
    s32 before;
    s32 rowStart;
    s32 rowEnd;
    s32 copies;
    s32 item;

    list = NIS_buildCardList(NIS_DECK_EDIT.deck, (kind < 0) ? (u32)-kind : (u32)kind, -1);
    NIS_GRID_POINTER = KAW_createCursor(1, 0x14, 0x18, 5, 1);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_CARD_GRID_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_GRID_HELP_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.helpWindow = waitFrames(0x7FFFFFFF);
    NIS_DEBUG_PRINT("mode = %d\n", kind);
    NIS_DEBUG_PRINT("deck = %d\n", NIS_DECK_EDIT.deck);
    if (list->count >= NIS_GRID_CURSOR) {
        list->cursor = NIS_GRID_CURSOR;
        list->scroll = NIS_GRID_SCROLL;
    }
    NIS_DECK_EDIT.fromViewer = 0;
    openChoiceMenu(&menu, -1, 0x13, NIS_runDeckKinds, &kind);
    /* the deck's own grid, or a kind's */
    item = (kind == 0) ? NIS_DECK_EDIT.deck + 0x12 : kind + 0x1B;
    if (list->count == 0) {
        addChoiceMenuItem(&menu, item, NULL);
    } else {
        addChoiceMenuItem(&menu, item, NIS_showCard);
    }
    if (NIS_CARD_LIST->cursor >= NIS_CARD_LIST->count) {
        NIS_CARD_LIST->cursor = NIS_CARD_LIST->count - 1;
    }
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        if ((s8)((s8)NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].inDeck + NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].count) == 0) {
            menu.handlers[0] = NULL;
        } else {
            menu.handlers[0] = NIS_showCard;
        }
        if (runChoiceMenu(&menu) != 0) {
            KAW_freeCursor(NIS_GRID_POINTER);
            NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
            NIS_WINDOW(NIS_DECK_SCREENS.helpWindow)->state = 4;
            waitFrames(0x1E);
            NIS_DECK_EDIT.cardType = NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].type;
            NIS_DECK_EDIT.cardIndex = NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].index;
            NIS_DECK_EDIT.copies = NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].inDeck;
            NIS_DECK_EDIT.unkA = NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].count;
            NIS_GRID_CURSOR = list->cursor;
            NIS_GRID_SCROLL = list->scroll;
            startChoiceMenuAction(&menu);
        }
        if (NIS_CARD_LIST->count == 0 || menu.unk248 != 0) {
            continue;
        }
        before = NIS_CARD_LIST->cursor;
        rowStart = NIS_CARD_LIST->cursor / 5 * 5;
        rowEnd = rowStart + 5;
        if (NIS_CARD_LIST->count < rowEnd) {
            rowEnd = NIS_CARD_LIST->count;
        }
        if ((NIS_REPEATED() & PAD_RIGHT) && ++NIS_CARD_LIST->cursor >= rowEnd) {
            NIS_CARD_LIST->cursor = rowStart;
        } else if ((NIS_REPEATED() & PAD_LEFT) && --NIS_CARD_LIST->cursor < rowStart) {
            NIS_CARD_LIST->cursor = rowEnd - 1;
        } else if ((NIS_REPEATED() & PAD_DOWN) && (NIS_CARD_LIST->cursor += 5) >= NIS_CARD_LIST->count) {
            if (NIS_CARD_LIST->cursor < (NIS_CARD_LIST->count - 1) / 5 * 5 + 5) {
                NIS_CARD_LIST->cursor = NIS_CARD_LIST->count - 1;
            } else {
                NIS_CARD_LIST->cursor -= 5;
            }
        } else if ((NIS_REPEATED() & PAD_UP) && NIS_CARD_LIST->cursor - 5 >= 0) {
            NIS_CARD_LIST->cursor -= 5;
        }
        if (before != NIS_CARD_LIST->cursor) {
            playSoundEffect(2);
        }
        if (NIS_CARD_LIST->scroll > NIS_CARD_LIST->cursor) {
            NIS_CARD_LIST->scroll -= 5;
        }
        if (NIS_CARD_LIST->scroll + 9 < NIS_CARD_LIST->cursor) {
            NIS_CARD_LIST->scroll += 5;
        }
        copies = (s8)NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].inDeck;
        if (NIS_changeCopies(PAD_L1, PAD_R1, (s8 *)&NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].inDeck, NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].count)) {
            NIS_addCardsToDeck(NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].type, NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].index, (s8)NIS_CARD_LIST->entries[NIS_CARD_LIST->cursor].inDeck - copies);
        }
    }
}
void NIS_freeDeckBackup(void) {
    freeHeapBlocksByTag(0x193);
}

/* a deck's card kinds (its cards, then each element's, the options...):
   the grid of the kind chosen. Cancel leaves the deck, unless it isn't
   full */
void NIS_runDeckKinds(s32 mode) {
    NisMenu menu;
    s32 result;

    clearKanjiPage(0xF);
    MENU_ITEMS[0x1A] = NIS_DECK_EDIT.deck + 0x12;
    openChoiceMenuFromList(&menu, &DECK_KINDS_MENU, &menu.choice);
    NIS_GRID_CURSOR = NIS_GRID_SCROLL = 0;
    if (mode < 0) {
        MENU_ITEMS[0x1A] = NIS_DECK_EDIT.deck + 0x12;
        waitFrames(0x5A);
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_NAME_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.nameWindow = waitFrames(0x7FFFFFFF);
    } else {
        NIS_freeDeckBackup();
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_SUMMARY_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
        if (mode == 100) {
            spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_NAME_WINDOW, getCurrentTaskId());
            NIS_DECK_SCREENS.nameWindow = waitFrames(0x7FFFFFFF);
        }
    }
    if (NIS_PROFILE(0)->showsRecords) {
        NIS_CARD_IN_VIEW = 0;
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_RECORD_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.motionWindow = waitFrames(0x7FFFFFFF);
    }
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        result = runChoiceMenu(&menu);
        if (result == 0) {
            continue;
        }
        if (NIS_PROFILE(0)->showsRecords) {
            NIS_WINDOW(NIS_DECK_SCREENS.motionWindow)->state = 4;
        }
        if (result == -1) {
            NIS_WINDOW(NIS_DECK_SCREENS.nameWindow)->state = 4;
            if (NIS_DECK_EDIT.cardCount < 30) {
                NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
                menu.handlers[10] = NIS_showDeckNotFull;
            }
            waitFrames(0x5A);
        } else {
            NIS_backUpDeck();
            if (result == 1) {
                NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
                waitFrames(0x1E);
            }
        }
        if (result != -1) {
            linkSavedDecks(0);
        }
        startChoiceMenuAction(&menu);
    }
}

/* keeps the deck's cards as they are before editing */
void NIS_backUpDeck(void) {
    s8 i;

    NIS_DECK_BACKUP = NIS_ALLOC_HEAP_BLOCK(0x3C, 0x193, "DECK_CMP", 2);
    for (i = 0; i < 30; i++) {
        NIS_DECK_BACKUP[i][0] = NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].cards[i].type;
        NIS_DECK_BACKUP[i][1] = NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].cards[i].index;
    }
}

/* a deck's menu: edit, auto deck, copy (while a slot is free), rename and
   delete (while another deck is left) */
void NIS_runDeckMenu(s32 mode) {
    NisMenu menu;
    s32 cursor;
    s32 result;

    cursor = -1;
    clearKanjiPage(0xF);
    if (mode != 0) {
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_SUMMARY_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
        if (mode == 2) {
            spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_NAME_WINDOW, getCurrentTaskId());
            NIS_DECK_SCREENS.nameWindow = waitFrames(0x7FFFFFFF);
        }
    }
    openChoiceMenu(&menu, 0x18, 0x32, NIS_returnToDeckList, &cursor);
    addChoiceMenuItem(&menu, 0x17, NIS_runDeckKinds);
    addChoiceMenuItem(&menu, 0x18, NIS_runDeckEditor);
    if (NIS_countSavedDecks() != 3) {
        addChoiceMenuItem(&menu, 0x19, NIS_copyDeck);
    }
    addChoiceMenuItem(&menu, 0x1A, NIS_renameDeck);
    if (NIS_countSavedDecks() != 1) {
        addChoiceMenuItem(&menu, 0x1B, NIS_deleteDeck);
    }
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        result = runChoiceMenu(&menu);
        if (result == 0) {
            continue;
        }
        if (result == 1 && menu.choice != 0) {
            NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
        }
        startChoiceMenuAction(&menu);
    }
}

void NIS_openDeckMenu(void) {
    NIS_runDeckMenu(0);
}

void NIS_reopenDeckMenu(void) {
    NIS_runDeckMenu(1);
}

void NIS_ignoreCancel(void) {
}

/* the saved decks: the one chosen gets its menu, the free slot makes a new
   deck; cancel goes back to where the deck screens were opened from */
void NIS_runDeckList(s32 enter) {
    NisMenu menu;
    Rect16 rect = { 0x180, 0x100, 0x40, 0x100 };
    s32 result;
    s32 i;
    s32 count;
    s32 choice;
    s32 items;
    s32 last;
    s32 shown;
    s32 newSlot;

    D_801E46E8 = 0;
    if (enter) {
        playMusic(1, 0x21, 0x73);
        if (NIS_STATE->caller->deckScreens == 3) {
            NIS_STATE->caller->deckScreens = 4;
            NIS_PROFILE_BACKUP = NIS_ALLOC_HEAP_BLOCK(sizeof(NisProfile), 0x10, "2P_DATA", 5);
            *NIS_PROFILE_BACKUP = *NIS_PROFILE(0);
            *NIS_PROFILE(0) = *NIS_PROFILE(1);
            D_801E46E9 = 1;
            /* "swapped" */
            NIS_DEBUG_PRINT("入れ替え完了\n");
        } else if (NIS_STATE->caller->deckScreens != 4) {
            D_801E46E9 = 0;
        }
        openKanjiPage(0xF, 0x1B9);
        clearKanjiPage(0xF);
        /* "initialized" */
        NIS_DEBUG_PRINT("初期化完了\n");
        NIS_DECK_EDIT.deck = 0;
        NIS_DECK_EDIT.saved = NIS_PROFILE(0)->savedDecks;
        NIS_DECK_EDIT.cardCount = 30;
        ClearImage(&rect, 0, 0, 0);
        do {
            result = NIS_loadDeckTims();
            waitFrames(0x5A);
        } while (result != 0);
        spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_DECK_SUMMARY_WINDOW, getCurrentTaskId());
        NIS_DECK_SCREENS.mainWindow = waitFrames(0x7FFFFFFF);
    }
    i = 0;
    count = NIS_countSavedDecks();
    openChoiceMenu(&menu, 0x19, 0x32, NIS_ignoreCancel, NULL);
    for (; i < count; i++) {
        addChoiceMenuItem(&menu, i + 0x12, NIS_openDeckMenu);
    }
    if (count != 3) {
        addChoiceMenuItem(&menu, 0x15, NIS_nameNewDeck);
    }
    while (1) {
        NIS_DEBUG_FRAME();
        waitFrames(FRAME_INTERVAL);
        result = runChoiceMenu(&menu);
        if (result != 0) {
            /* "deck in use" */
            NIS_DEBUG_PRINT("使用デッキ %d\n", NIS_DECK_EDIT.deck);
            /* cancel, or the item after the decks: the window closes */
            if (result < 0) {
                goto close;
            }
            items = menu.count;
            choice = menu.choice;
            last = items - 1;
            if ((count == 3) ? items == choice : last == choice) {
            close:
                NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 4;
            }
            if (NIS_STATE->caller->deckScreens == 0 || result >= 0) {
                startChoiceMenuAction(&menu);
                continue;
            }
            waitFrames(0x5A);
            clearKanjiPage(0xF);
            closeKanjiPage(0xF);
            switch (NIS_STATE->caller->deckScreens) {
            case 1:
                NIS_STATE->caller->deckScreens = 0;
                NIS_DEBUG_NAME_TASK(0, NIS_STR_DECK_EDIT);
                spawnTask(0, -1, 0, 0x800, returnToAreaFromDeckEditor, 1, getCurrentTaskId(), 0, 0);
                break;
            case 2:
                NIS_STATE->caller->deckScreens = 0;
                NIS_DEBUG_NAME_TASK(0, NIS_STR_DECK_EDIT);
                spawnTask(0, -1, 0, 0x800, NIS_runVsMode, 1, getCurrentTaskId(), 0, 0);
                break;
            case 4:
                NIS_STATE->caller->deckScreens = 0;
                *NIS_PROFILE(1) = *NIS_PROFILE(0);
                *NIS_PROFILE(0) = *NIS_PROFILE_BACKUP;
                freeHeapBlocksByTag(0x10);
                NIS_DEBUG_NAME_TASK(0, NIS_STR_DECK_EDIT);
                spawnTask(0, -1, 0, 0x800, NIS_runVsMode, 1, getCurrentTaskId(), 0, 0);
                break;
            case 5:
                NIS_STATE->caller->deckScreens = 0;
                NIS_DEBUG_NAME_TASK(0, NIS_STR_DECK_EDIT);
                spawnTask(0, -1, 0, 0x800, openMemcardScreenAfterDeckEdit, 1, getCurrentTaskId(), 0, 0);
                break;
            }
            exitTask();
            continue;
        }
        if (NIS_DECK_EDIT.deck == (choice = menu.choice)) {
            continue;
        }
        /* the cursor moved: show the deck under it, or the free slot */
        shown = menu.count;
        newSlot = shown - 1;
        if ((count == 3) ? choice < shown : choice < newSlot) {
            NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 1;
            NIS_DECK_EDIT.deck = menu.choice;
            NIS_DECK_EDIT.saved = &NIS_PROFILE(0)->savedDecks[menu.choice];
        } else {
            NIS_DECK_EDIT.deck = -menu.choice;
            NIS_WINDOW(NIS_DECK_SCREENS.mainWindow)->state = 2;
        }
    }
}

void NIS_enterDeckList(void) {
    NIS_runDeckList(1);
}

void NIS_returnToDeckList(void) {
    NIS_runDeckList(0);
}

void NIS_enterDeckListFromVs(void) {
    NIS_STATE->caller->deckScreens = 2;
    NIS_runDeckList(1);
}

void NIS_enterDeckListForTrade(void) {
    NIS_STATE->caller->deckScreens = 3;
    NIS_runDeckList(1);
}

void NIS_enterDeckListFromMenu(void) {
    NIS_STATE->caller->deckScreens = 5;
    NIS_runDeckList(1);
}
