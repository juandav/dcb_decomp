#include "common.h"
#include "game.h"
#include "dcb/sub_base_deck.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/heap.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"

typedef struct {
    u16 cardIds[30];
    char name[0x32];
} DeckRecord;

extern Menu SUB_BASE_DECK_MENU;
extern UiWindow SUB_BASE_DECK_WINDOW;
extern CursorHighlight SUB_BASE_DECK_CURSOR;
extern u16 SUB_BASE_DECK_ENTRIES[0x9F];
extern const char SUB_STR_BASE_DECK_LIST[];

extern char *strcat(char *, const char *);

/* its own copy of "Deck": SUB_drawDeckNameField was in another file of the original */
const char SUB_STR_DECK[] = "Deck";

void SUB_drawBaseDeckList(UiWindow *window) {
    char buf[64];
    s32 x = window->originX;
    s32 z = window->z;
    DeckRecord *records = (DeckRecord *)(*(u8 **)SESSION_DATA + 8);
    s32 i;
    s32 y;
    s32 palette;
    u16 entry;

    for (i = 0; i < SUB_BASE_DECK_MENU.nrows; i++) {
        if (i < window->view.y / SUB_BASE_DECK_MENU.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / SUB_BASE_DECK_MENU.rowH < i) {
            break;
        }
        y = window->originY + i * SUB_BASE_DECK_MENU.rowH + 1;
        entry = SUB_BASE_DECK_ENTRIES[i];
        strcpy(buf, records[entry & 0x3FFF].name);
        strcat(buf, SUB_STR_DECK);
        palette = 8;
        if (entry & 0x4000) {
            palette = 7;
        }
        drawText(x + 2, y, (s32)buf, palette, z);
        if (palette == 7) {
            drawText(x + 0x9E, y, (s32)"Usable", 7, z);
        } else {
            drawText(x + 0x9E, y, (s32)"Not Usable", palette, z);
        }
    }
    updateMenuCursor(&SUB_BASE_DECK_MENU);
}

s32 SUB_chooseBaseDeck(PlayerDeck *deck, s32 player) {
    u8 *file;
    DeckRecord *records;
    s32 i;
    s32 count;
    s32 done;
    s32 selected;
    u16 flags;
    DeckRecord *record;

    spawnTask(0, -1, 0, 0x800, loadFile, "B:\\DECK2.DEK", getCurrentTaskId());
    file = (u8 *)waitFrames(0x7FFFFFFF);
    *(u8 **)SESSION_DATA = file;
    records = (DeckRecord *)(file + 8);
    markBuildableOpponentDecks(player);
    for (i = 0; i < 0x9F; i++) {
        SUB_BASE_DECK_ENTRIES[i] = 0;
    }
    count = 0;
    for (i = 0; i < 0x9F; i++) {
        flags = PLAYER_DATA(player).opponentDeckFlags[i];
        if (flags & 0x8000) {
            SUB_BASE_DECK_ENTRIES[count] = i | (flags & 0xC000);
            count++;
        }
    }
    SUB_BASE_DECK_MENU.pad = player;
    SUB_BASE_DECK_MENU.nrows = count;
    SUB_BASE_DECK_WINDOW.view.h = count * SUB_BASE_DECK_MENU.rowH;
    openMenu(&SUB_BASE_DECK_MENU, &SUB_BASE_DECK_WINDOW, &SUB_BASE_DECK_CURSOR, (Bytes4 *)-1);
    SUB_BASE_DECK_WINDOW.label = (s32)SUB_STR_BASE_DECK_LIST;
    playMenuSound(3);
    done = 0;
    selected = -1;
    do {
        waitFrames(FRAME_INTERVAL);
        drawWindow(&SUB_BASE_DECK_WINDOW, SUB_drawBaseDeckList, 0);
        if ((PAD_STATES[player]->pressed & 0x40) && (SUB_BASE_DECK_ENTRIES[SUB_BASE_DECK_MENU.row] & 0x4000)) {
            playMenuSound(1);
            selected = SUB_BASE_DECK_ENTRIES[SUB_BASE_DECK_MENU.row] & 0x3FFF;
            done = 1;
        } else if (PAD_STATES[player]->pressed & 0x10) {
            playMenuSound(0);
            selected = -1;
            done = 1;
        }
    } while (!done);
    playMenuSound(4);
    if (selected >= 0) {
        record = &records[selected];
        for (i = 0; i < 30; i++) {
            setCardSlotFromId((u8 *)&deck->cards[i], record->cardIds[i]);
        }
        linkDeckCardData(player, deck);
    }
    freeHeapBlock(*(u8 **)SESSION_DATA);
    return selected;
}

/* the last byte is a leftover in the original, not zero padding, and not
   the same in every version */
#if VERSION_US
const char SUB_STR_BASE_DECK_LIST[16] = "BASE DECK LIST\0\xFE";
#elif VERSION_EU
const char SUB_STR_BASE_DECK_LIST[16] = "BASE DECK LIST\0%";
#else
#error "subseg/deck/sub_base_deck: version not checked"
#endif

Menu SUB_BASE_DECK_MENU = { NULL, NULL, { 50, 40, 220, 154 }, 0, -1, 0, -1, 0xa, 0x21, 220, 12, 1, 1, 2, 1, 0, 14, 0, 0, 0 };
