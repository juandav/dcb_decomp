#include "common.h"
#include "game.h"
#include "dcb/text.h"
#include "dcb/window.h"
#include "dcb/card_db.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/heap.h"
#include "dcb/vram_upload.h"
#include "dcb/menu.h"
#include "dcb/memcard.h"
#include "dcb/sort.h"
#include "dcb/battle_hud.h"
#include "dcb/prim.h"
#include "dcb/partner_level.h"
#include "dcb/scroll_bg.h"
#include "dcb/frame_callback.h"
#include "dcb/dialog.h"

extern s16 SUB_EDITOR_PLAYER;
extern u8 SUB_EDITOR_BLINK;
extern s8 *SUB_DECK_EDIT_CARD;
extern s16 SUB_DECK_EDIT_CARD_ID;
extern void SUB_drawSprite(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 tp, u8 brightness, s8 abr, s32 otIndex);

typedef struct {
    u16 cardIds[30];
    char name[0x32];
} DeckRecord;
extern Menu SUB_BASE_DECK_MENU;
extern UiWindow SUB_BASE_DECK_WINDOW;
extern CursorHighlight SUB_BASE_DECK_CURSOR;
extern u16 SUB_BASE_DECK_ENTRIES[0x9F];
extern void SUB_drawBaseDeckList();

extern Menu SUB_DECK_SORT_MENU;
extern char *SUB_DECK_SORT_LABELS[];
/* a comparison function for sortArray */
typedef s32 (*SortCompare)(s8 *, s8 *);
extern SortCompare SUB_DECK_SORT_COMPARES[];
extern PlayerDeck *SUB_EDITED_DECK;
extern void SUB_linkPartnerCards(CardSlot *cards, s32 player);
extern void SUB_groupDuplicateCards(CardSlot *cards);
typedef struct {
    s16 request;
    s16 ids[8];
    s8 ages[8];
    s8 busy;
    s8 running;
} CardImageCache;
extern CardImageCache SUB_CARD_IMAGE_CACHE;
extern UiWindow SUB_WINDOWS[7];
extern void SUB_drawAutoDeckOptions();

typedef struct {
    s16 col;
    s16 prevCol;
    s16 row;
    s16 prevRow;
    u8 active;
    char name[0x13];
    u8 cursor;
    u8 onButtons;
    s8 button;
    s8 prevButton;
    s8 state;
    u8 pad;
    u8 fresh;
} NameEntry;
extern NameEntry SUB_NAME_ENTRY;
extern CursorHighlight SUB_DECK_NAME_CURSOR;
extern s32 SUB_PARTNER_TITLE_SHOWN;
extern s32 SUB_PARTNER_TITLE_Y;
extern void *SUB_CARD_LIST[301];
extern s8 *SUB_CARDS_BY_ID[301];
extern void SUB_drawSprite(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 tp, u8 brightness, s8 abr, s32 otIndex);
extern SprtPacket *SUB_SPRITE_CURSOR;
typedef struct {
    u8 inDecks[3][301];
    u8 unk387[301];
    u8 spare[301];
} DeckCardCounts;
extern DeckCardCounts *SUB_DECK_CARD_COUNTS;

extern UiWindow SUB_NAME_ENTRY_WINDOW;
extern UiWindow SUB_DECK_NAME_WINDOW;
extern UiWindow SUB_NAME_ENTRY_HELP_WINDOW;
extern void SUB_drawNameEntryGrid();
extern void SUB_drawDeckNameField();
extern void SUB_drawNameEntryHelp();
typedef struct {
    UiWindow window;
    s32 slot;
} TabWindow;
extern TabWindow SUB_PARTNER_TABS[3];
extern s32 SUB_PARTNER_SLOT;
extern UiWindow SUB_ARMOR_CHANGE_WINDOW;
extern UiWindow SUB_ARMOR_WINDOW;
extern UiWindow SUB_PARTNER_WINDOW;
extern UiWindow SUB_ABILITY_WINDOW;
extern UiWindow SUB_EQUIPMENT_WINDOW;
extern void SUB_drawPartnerTitle(void);
extern void SUB_drawPartnerTab();
extern void SUB_drawArmorChange();
extern void SUB_drawArmorDetails();
extern void SUB_drawPartnerDetails();
extern void SUB_drawAbilityList();
extern void SUB_drawEquipment();
typedef struct {
    void *primBuffers[2];
    s16 *unk8;
    void (*task)();
    s16 slide;
    s16 player;
    s8 hidden;
    s8 editing;
    s8 running;
    u8 listShown;
    u8 blink;
    u8 useDeckCounts;
} EditorState;
/* libgpu's LINE_G3 */
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 r1;
    u8 g1;
    u8 b1;
    u8 p1;
    s16 x1;
    s16 y1;
    u8 r2;
    u8 g2;
    u8 b2;
    u8 p2;
    s16 x2;
    s16 y2;
    u32 pad;
} LINE_G3;
typedef struct {
    u8 *deckCounts;
    LINE_G3 cursorLines[2][4];
    s8 *card;
    s16 cardId;
    s16 statsPage;
    s16 slot;
    s8 mode;
    u8 unk10F;
} DeckEditState;
extern EditorState SUB_EDITOR;
extern DeckEditState SUB_DECK_EDIT;
extern s16 SUB_COLLECTION_SHOW_INFO;
typedef struct {
    s16 id;
    s16 count;
} CardCount;
typedef struct {
    CardCount *lists[9];
    s32 selectedId;
    s16 uniqueCount;
    s16 totalCount;
    s16 page;
    s16 showInfo;
    s16 counts[8];
    u8 unk40[0x12D];
    u8 unk16D;
    u8 unk16E[2];
    s8 *selectedCard;
} CollectionStats;
extern CollectionStats SUB_COLLECTION_STATS;
typedef struct {
    char *name;
    u8 unk4[8];
} AbilityText;
extern AbilityText SUB_ABILITY_TEXTS[];
extern s32 SUB_PARTNER_PLAYER;
typedef struct {
    PlayerDeck *decks[3];
    s8 count;
    s8 current;
    s16 slot;
} DeckMenuState;
extern DeckMenuState SUB_DECK_MENU;
extern u8 *SUB_CARD_ARCHIVE;
typedef struct {
    s16 ids[250];
    s16 count;
    s16 pad;
} CardIdList;
extern CardIdList *SUB_AUTO_DECK_POOLS;
extern u8 SUB_CARD_LIST_MENU_PAD;
extern u8 SUB_CARD_SORT_MENU_PAD;
extern u8 SUB_DECK_SORT_MENU_PAD;
extern void SUB_loadEditorAssets(void);
extern void SUB_runCardList();
extern void SUB_drawDeckEditTitle(UiWindow *window);
extern void SUB_drawSortHint(UiWindow *window);
extern UiWindow SUB_CARD_SORT_WINDOW;
extern UiWindow SUB_DECK_SORT_WINDOW;
extern UiWindow D_801F32D8;
extern UiWindow D_801F3294[3];
extern UiWindow D_801F331C;
extern UiWindow D_801F3360;
extern UiWindow D_801F33A4;
extern UiWindow D_801F33E8;
extern void SUB_drawCardSortMenu();
extern void SUB_drawPickerList();
extern void SUB_drawPickerCardInfo();
extern void SUB_drawDeckStats();
extern void SUB_drawDeckGrid();
extern void SUB_drawSlotCardInfo();
extern void SUB_initDeckEdit(PlayerDeck *deck);
extern PlayerDeck SUB_ORIGINAL_DECK;
extern void SUB_tickDeckSlots(PlayerDeck *deck);
extern void SUB_tickCardPicker(PlayerDeck *deck);
extern void SUB_runDeckMenu();
extern CursorHighlight SUB_AUTO_DECK_CURSOR;
extern u8 SUB_AUTO_DECK_OPTIONS[6];
extern void SUB_drawCardData();
extern void SUB_drawCardCountPage();
extern void SUB_drawCardList();
extern void SUB_drawCardListHelp(UiWindow *window);
extern void SUB_drawCollectionTotals(UiWindow *window);
extern void SUB_drawSpecialtyCounts();
extern void SUB_drawDeckSummary();
extern void SUB_drawDeckMenuHelp(UiWindow *window);
extern void SUB_drawEmptyDeck(UiWindow *window);
extern u16 SUB_COLLECTION_COUNTS[8];
extern void SUB_buildCardCountLists(void);
extern SortCompare SUB_CARD_SORT_COMPARES[];
extern Menu SUB_CARD_SORT_MENU;
extern Menu SUB_CARD_LIST_MENU;
extern char *SUB_CARD_SORT_LABELS[];
extern char *SUB_CARD_SORT_LABELS_EDITING[];
extern void SUB_initCardList(void);
extern Menu SUB_EQUIPMENT_MENU;
extern s8 SUB_ABILITY_IDS[];
extern Menu SUB_ABILITY_MENU;
extern char *strcat(char *, const char *);
extern u8 SUB_CARD_LIST_MENU_ACTIVE;
extern Rect16 SUB_CARD_SORT_MENU_RECT;
extern s32 SUB_canAddCardToDeck(s32 cardId);
extern void SUB_showDeckSlots(void);
extern void SUB_drawCardIcon(s16 cardId, s16 x, s16 y, u8 brightness, s32 otIndex);
extern void SUB_openAutoDeckMenu(void);
extern void SUB_drawAutoDeckMenu(void);
extern void SUB_buildAutoDeck(PlayerDeck *deck);
extern s32 SUB_chooseBaseDeck(PlayerDeck *deck, s32 player);
extern void SUB_editDeck(PlayerDeck *deck);
extern void SUB_enterDeckName();
extern CursorHighlight SUB_NAME_ENTRY_CURSOR;
extern void SUB_drawNameEntry(void);
extern void SUB_runAutoDeckMenu(void);
extern char SUB_LABEL_BUFFER[];
extern u8 SUB_CARD_LIST_COLORS[][4];
extern u8 SUB_NAME_ENTRY_CHARS[];
extern s32 SUB_moveNameEntryCursor(void);
extern s8 *SUB_AUTO_DECK_CARD;
extern u8 SUB_OPTION_CARD_LEVELS[];
extern s32 SUB_takePoolCards(CardSlot *slots, s32 row, s32 count);
extern s8 *SUB_COLLECTION_SELECTED_CARD;
extern s32 SUB_findCachedCardImage(s16 id);
extern s8 SUB_DECK_IS_NEW;
extern s32 SUB_hasDeckChanged(PlayerDeck *a, PlayerDeck *b);
extern Partner SUB_SAVED_PARTNER;
extern Partner SUB_UNEQUIPPED_PARTNER;
extern Partner SUB_PREVIEW_PARTNER;
extern u8 SUB_ARMOR_ICONS[][3];
extern void SUB_drawArmorPortrait(s32 player, s32 slot, s32 x, s32 y, s32 otIndex);
extern s32 SUB_getStatPalette(s32 a, s32 b);
extern void SUB_drawPartnerPortrait(s32 player, s32 slot, s32 x, s32 y, s32 brightness, s32 otIndex);
extern s32 SUB_ARMOR_INDEX;
extern CursorHighlight SUB_ABILITY_CURSOR;
extern CursorHighlight SUB_EQUIPMENT_CURSOR;
extern Rect16 SUB_PARTNER_WINDOW_RECT;
extern Rect16 SUB_ARMOR_WINDOW_RECT;
extern Rect16 SUB_ARMOR_CHANGE_WINDOW_RECT;
extern s8 SUB_PARTNER_WINDOW_ANIM_DONE;
extern s8 SUB_ARMOR_WINDOW_ANIM_DONE;
extern void SUB_drawPartnerEquipment(void);
/*
 * Strings several functions share: each one is defined further down, where the
 * original put it in .rodata, instead of as a literal at its first use.
 */
extern const char SUB_STR_NO_DECK_NAME[];
extern const char SUB_STR_BASE_DECK_LIST[];
extern const char SUB_STR_LV[];
extern const char SUB_STR_TYPE[];
extern const char SUB_FMT_CARD_NUMBER[];
extern const char SUB_FMT_COUNT[];
extern const char SUB_STR_CARDS[];
extern const char SUB_STR_L1_BACK[];
extern const char SUB_STR_CARD_LIST[];
extern const char SUB_STR_HELP[];
extern const char SUB_STR_PARTNER_TITLE[];
extern const char SUB_STR_CARD_DATA[];
extern const char SUB_STR_CARD_INFO[];
extern const char SUB_FMT_3_DIGITS[];
extern const char SUB_STR_TOTAL[];
extern const char SUB_FMT_4_DIGITS[];
extern const char SUB_FMT_2_DIGITS[];
extern const char SUB_STR_WINS[];
extern const char SUB_STR_LOSSES[];
extern const char SUB_STR_SUPPORT_EFFECT[];
extern const char SUB_STR_FIRE_ICON[];
extern const char SUB_STR_ICE_ICON[];
extern const char SUB_STR_NATURE_ICON[];
extern const char SUB_STR_DARKNESS_ICON[];
extern const char SUB_STR_RARE_ICON[];
extern const char SUB_STR_OPTION_CARD[];
extern const char SUB_STR_PARTNER[];
extern const char SUB_STR_DECK_1[];
extern const char SUB_STR_DECK_2[];
extern const char SUB_STR_DECK_3[];
extern const char SUB_STR_SUM[];
extern const char SUB_STR_DECK_TITLE[];
extern const char SUB_STR_DISABLE[];

extern s16 SUB_DECK_MENU_SLOT;
extern u8 SUB_AUTO_DECK_ENABLED;
extern s8 SUB_EDITOR_HIDDEN;

typedef struct {
    s16 x;
    s16 w;
    u8 next[4];
} MenuItem;

extern u8 SUB_AUTO_DECK_SPECIALTY;
extern u8 SUB_AUTO_DECK_STYLE;
extern u8 SUB_AUTO_DECK_OPTION_AMOUNT;
extern u8 SUB_AUTO_DECK_RESULT;
extern MenuItem SUB_AUTO_DECK_ITEMS[13];
extern u8 SUB_EDITOR_USE_DECK_COUNTS;
extern s8 SUB_DECK_EDIT_MODE;
extern s32 D_801F3304;
extern CursorHighlight SUB_CARD_LIST_CURSOR;
extern CursorHighlight SUB_CARD_SORT_CURSOR;
extern CursorHighlight SUB_DECK_SORT_CURSOR;
extern void SUB_openCenteredWindow(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style);
extern u8 SUB_EDITOR_RUNNING;
extern void *D_801F4350;
extern s32 D_801F32C0;
extern void SUB_initCollectionStats(void);
extern void SUB_drawCardListScreen(void);
extern void SUB_runPartnerEquipment();

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
                rect.x = window->rect.x - window->scroll[2] + SUB_NAME_ENTRY.prevCol * 17 + 15;
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
        func_80014C08(FRAME_INTERVAL);
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
    func_80014C08(20);
    removeFrameCallback((s32)SUB_drawNameEntry);
    if (SUB_NAME_ENTRY.state == 7) {
        func_800149B8(0, -1, 0, 0x1000, mode == 1 ? SUB_runDeckMenu : SUB_runAutoDeckMenu, 0, 0, 0, 0);
    } else {
        func_800149B8(0, -1, 0, 0x1000, SUB_runDeckMenu, 0, 0, 0, 0);
    }
}

/* the last two bytes are leftovers in the original, not zero padding */
const char SUB_STR_NO_DECK_NAME[36] = "A Deck Name has not been entered!\0\x0E\0";

/* its own copy of "Deck": SUB_drawDeckNameField was in another file of the original */
const char SUB_STR_DECK[] = "Deck";

void SUB_drawBaseDeckList(UiWindow *window) {
    char buf[64];
    s32 x = window->originX;
    s32 z = window->z;
    DeckRecord *records = (DeckRecord *)(*(u8 **)D_8006E054 + 8);
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

    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\DECK2.DEK", getCurrentTaskId());
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    *(u8 **)D_8006E054 = file;
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
        func_80014C08(FRAME_INTERVAL);
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
    freeHeapBlock(*(u8 **)D_8006E054);
    return selected;
}

void SUB_drawPartnerTitle(void) {
    if (isSpritePoolFull() == 0) {
        if (SUB_PARTNER_TITLE_SHOWN != 0) {
            SUB_PARTNER_TITLE_Y += 4;
            if (SUB_PARTNER_TITLE_Y > 8) {
                SUB_PARTNER_TITLE_Y = 8;
            }
        } else {
            SUB_PARTNER_TITLE_Y -= 4;
            if (SUB_PARTNER_TITLE_Y < -0x20) {
                SUB_PARTNER_TITLE_Y = -0x20;
            }
        }
        CUR_SPRT->sp.x0 = 6;
        CUR_SPRT->sp.y0 = SUB_PARTNER_TITLE_Y;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x40;
        CUR_SPRT->sp.clut = 0x7C00;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x20;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setlen(&CUR_SPRT->dm, 1);
        CUR_SPRT->dm.code[0] = 0xE1000005;
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

s32 SUB_getStatPalette(s32 a, s32 b) {
    s32 result = 7;

    if (a < b) {
        result = 5;
    }
    if (b < a) {
        result = 2;
    }
    return result;
}

void SUB_drawPartnerPortrait(s32 player, s32 slot, s32 x, s32 y, s32 brightness, s32 otIndex) {
    s32 specialty;

    if (PLAYER_DATA(player).partners[slot].cardId == 0) {
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 8;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = 0x7FA0;
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = brightness;
            CUR_SPRT->sp.g0 = brightness;
            CUR_SPRT->sp.b0 = brightness;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    } else {
        specialty = PLAYER_DATA(player).partners[slot].card[0].attr >> 4;
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x30;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = getClut(0x200, specialty + 0x1F8);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = brightness;
            CUR_SPRT->sp.g0 = brightness;
            CUR_SPRT->sp.b0 = brightness;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull() == 0) {
                CUR_SPRT->sp.x0 = x;
                CUR_SPRT->sp.y0 = y + 4;
                CUR_SPRT->sp.u0 = 0;
                CUR_SPRT->sp.v0 = getSlotPartnerIndex(player, slot) * 41;
                CUR_SPRT->sp.clut = getClut(0x240, specialty | 0x1F0);
                CUR_SPRT->sp.w = 40;
                CUR_SPRT->sp.h = 40;
                setSemiTrans(&CUR_SPRT->sp, 0);
                CUR_SPRT->sp.r0 = brightness;
                CUR_SPRT->sp.g0 = brightness;
                CUR_SPRT->sp.b0 = brightness;
                setDrawMode(&CUR_SPRT->dm, 0, 0, 0x97);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
                SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            }
        }
    }
}

void SUB_drawArmorPortrait(s32 player, s32 slot, s32 x, s32 y, s32 otIndex) {
    s32 specialty;

    if (PLAYER_DATA(player).partners[slot].armorCardId == 0) {
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 8;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = 0x7FA0;
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    } else {
        specialty = ((DigimonCardData *)DIGIMON_CARDS)[PLAYER_DATA(player).partners[slot].armorCardId].attr >> 4;
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x30;
            CUR_SPRT->sp.v0 = 0x29;
            CUR_SPRT->sp.clut = getClut(0x200, specialty + 0x1F8);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull() == 0) {
                CUR_SPRT->sp.x0 = x;
                CUR_SPRT->sp.y0 = y + 4;
                CUR_SPRT->sp.u0 = (getSelectedArmorIndex(player, getSlotPartnerIndex(player, slot)) + 1) * 44;
                CUR_SPRT->sp.v0 = getSlotPartnerIndex(player, slot) * 41;
                CUR_SPRT->sp.clut = getClut(0x240, specialty | 0x1F0);
                CUR_SPRT->sp.w = 40;
                CUR_SPRT->sp.h = 40;
                setSemiTrans(&CUR_SPRT->sp, 0);
                CUR_SPRT->sp.r0 = 0x80;
                CUR_SPRT->sp.g0 = 0x80;
                CUR_SPRT->sp.b0 = 0x80;
                setDrawMode(&CUR_SPRT->dm, 0, 0, 0x97);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
                addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
                SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            }
        }
    }
}

/* the last byte is a leftover in the original, not zero padding */
const char SUB_STR_BASE_DECK_LIST[16] = "BASE DECK LIST\0\xFE";

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

/* not referenced by any code */
u8 D_801F1933 = 16;

Menu SUB_BASE_DECK_MENU = { NULL, NULL, { 50, 40, 220, 154 }, 0, -1, 0, -1, 0xa, 0x21, 220, 12, 1, 1, 2, 1, 0, 14, 0, 0, 0 };

/* the partner abilities; the code here only reads their texts */
AbilityText SUB_ABILITY_TEXTS[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7, 0, 0 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19, 0, 0 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33, 0, 0 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59, 0, 0 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78, 0, 0 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88, 0, 0 } },
    { "HP+500.", { 99, 0xFF, 75, 91, 0xFF, 95, 0, 0 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28, 0, 0 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51, 0, 0 } },
    { "All Attack Powers +200.", { 75, 0xFF, 90, 93, 0xFF, 84, 0, 0 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2, 0, 0 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15, 0, 0 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30, 0, 0 } },
    { "*b0 Attack Power +250.", { 48, 0xFF, 78, 61, 62, 61, 0, 0 } },
    { "*b0 Attack Power +300.", { 67, 0xFF, 0xFF, 85, 82, 79, 0, 0 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13, 0, 0 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29, 0, 0 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43, 0, 0 } },
    { "*b1 Attack Power +200.", { 51, 44, 0xFF, 56, 67, 66, 0, 0 } },
    { "*b1 Attack Power +250.", { 77, 0xFF, 0xFF, 76, 86, 96, 0, 0 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1, 0, 0 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9, 0, 0 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24, 0, 0 } },
    { "*b2 Attack Power +200.", { 82, 0xFF, 63, 67, 0xFF, 48, 0, 0 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, 0xFF, 34, 0, 0 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, 0xFF, 17, 0, 0 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, 0xFF, 71, 0, 0 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, 0xFF, 0xFF, 40, 62, 0, 0 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, 0xFF, 0xFF, 24, 49, 0, 0 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, 0xFF, 0xFF, 11, 21, 0, 0 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50, 0, 0 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, 0xFF, 42, 77, 73, 0, 0 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, 0xFF, 66, 0xFF, 83, 4, 0, 0 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97, 0, 0 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, 0xFF, 0xFF, 25, 0, 0 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, 0xFF, 0xFF, 0, 0 } },
    { "Jamming Support, *b2 Attack Power -100.", { 0xFF, 74, 27, 0xFF, 54, 10, 0, 0 } },
    { "Eat-up HP, *b2 Attack Power -200.", { 0xFF, 87, 50, 0xFF, 73, 0xFF, 0, 0 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, 0xFF, 0, 0 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, 0xFF, 0, 0 } },
    { "Add + 30 DP.", { 0xFF, 98, 97, 60, 78, 85, 0, 0 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11, 0, 0 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35, 0, 0 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44, 0, 0 } },
    { "Boost Attack Power +300.", { 69, 0xFF, 84, 82, 0xFF, 80, 0, 0 } },
    { "Attack Power is Doubled.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12, 0, 0 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39, 0, 0 } },
    { "Boost *b0 Attack Power +500.", { 55, 0xFF, 88, 88, 66, 74, 0, 0 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63, 0, 0 } },
    { "*b0 Attack Power is Tripled.", { 59, 0xFF, 94, 74, 0xFF, 86, 0, 0 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, 0xFF, 7, 22, 18, 0, 0 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, 0xFF, 23, 41, 36, 0, 0 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, 0xFF, 92, 59, 53, 0, 0 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, 0xFF, 67, 0, 0 } },
    { "*b1 Attack Power is Tripled.", { 83, 0xFF, 91, 94, 0xFF, 89, 0, 0 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, 0xFF, 8, 0, 0 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, 0xFF, 20, 0, 0 } },
    { "Boost *b2 Attack Power +300.", { 71, 0xFF, 60, 63, 0xFF, 90, 0, 0 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, 0xFF, 40, 0, 0 } },
    { "*b2 Attack Power is Tripled.", { 88, 0xFF, 85, 95, 0xFF, 65, 0, 0 } },
    { "Attack Power becomes same as HP.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, 0xFF, 0, 0 } },
    { "Attack becomes Eat-up HP.", { 0xFF, 93, 80, 0xFF, 61, 55, 0, 0 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, 0xFF, 17, 0xFF, 28, 75, 0, 0 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, 0xFF, 15, 0xFF, 0, 0 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { 0xFF, 66, 24, 0xFF, 13, 45, 0, 0 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, 0xFF, 33, 0xFF, 47, 14, 0, 0 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, 0xFF, 0xFF, 37, 26, 0, 0 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, 0xFF, 0xFF, 37, 0, 0 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, 0xFF, 77, 25, 38, 0, 0 } },
    { "If *a0 Opponent, X3 own Attack Power.", { 0xFF, 91, 0xFF, 65, 74, 82, 0, 0 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76, 0, 0 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, 0xFF, 89, 83, 98, 91, 0, 0 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6, 0, 0 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, 0xFF, 93, 0xFF, 93, 70, 0, 0 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93, 0, 0 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, 0xFF, 0, 0 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46, 0, 0 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, 0xFF, 0xFF, 68, 0, 0 } },
    { "Change own Specialty to *a0.", { 9, 35, 0xFF, 48, 96, 0xFF, 0, 0 } },
    { "Change own Specialty to *a1.", { 0xFF, 99, 30, 87, 58, 0xFF, 0, 0 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94, 0, 0 } },
    { "Change own Specialty to *a3.", { 30, 0xFF, 68, 98, 81, 3, 0, 0 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99, 0, 0 } },
    { "Switch Opponent's Specialty to own.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Swap Specialty with Opponent's.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, 0xFF, 97, 43, 0xFF, 0, 0 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, 0xFF, 0, 0 } },
    { "If *a2 Opponent, lower its AP to 0.", { 0xFF, 11, 52, 0xFF, 64, 22, 0, 0 } },
    { "If *a3 Opponent, lower its AP to 0.", { 0xFF, 25, 37, 13, 2, 72, 0, 0 } },
    { "If *a4 Opponent, lower its AP to 0.", { 0xFF, 75, 16, 0xFF, 34, 52, 0, 0 } },
    { "Reduce both Players' Atk Pwr to 0.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, 0xFF, 9, 48, 23, 0, 0 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, 0xFF, 20, 38, 57, 0, 0 } },
    { "If *e5, boost Attack Power +400.", { 57, 0xFF, 81, 70, 95, 77, 0, 0 } },
    { "Opponent uses *b0 Attack.", { 16, 62, 0xFF, 28, 97, 31, 0, 0 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, 0xFF, 47, 0, 0 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, 0xFF, 44, 58, 0, 0 } },
    { "Opponent uses same Attack.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81, 0, 0 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, 0xFF, 0, 0 } },
    { "Recover HP +400.", { 0xFF, 68, 92, 73, 55, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +600.", { 0xFF, 46, 86, 44, 60, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +700.", { 0xFF, 83, 69, 53, 56, 0xFF, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, 0xFF, 24, 30, 42, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 600.", { 0xFF, 63, 0xFF, 49, 49, 64, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 1000.", { 0xFF, 94, 0xFF, 64, 65, 98, 0, 0 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27, 0, 0 } },
    { "Drop 2 Cards in Opponent's Hand.", { 0xFF, 78, 25, 0xFF, 92, 56, 0, 0 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { 0xFF, 56, 18, 86, 35, 5, 0, 0 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { 0xFF, 88, 49, 0xFF, 90, 32, 0, 0 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { 0xFF, 51, 7, 29, 0xFF, 16, 0, 0 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { 0xFF, 95, 35, 89, 0xFF, 54, 0, 0 } },
    { "Move Offline Top Card to Online Deck.", { 86, 0xFF, 70, 78, 50, 0xFF, 0, 0 } },
    { "Void Opponent's Support Effect.", { 0xFF, 84, 65, 0xFF, 87, 69, 0, 0 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, 0xFF, 0, 0 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, 0xFF, 0, 0 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, 0xFF, 55, 94, 83, 0, 0 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, 0xFF, 76, 66, 79, 87, 0, 0 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41, 0, 0 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92, 0, 0 } },
    { "Boost Battle Experience by 30%.", { 80, 0xFF, 96, 90, 99, 60, 0, 0 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, 0xFF, 0, 0 } },
    { "Rare Card even more likely to appear.", { 0xFF, 90, 99, 96, 70, 0xFF, 0, 0 } },
};

void SUB_drawPartnerTab(TabWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    u8 rgb[4];
    s32 x;
    s32 y;
    s32 z;
    s32 player;
    s32 slot;
    s32 changed;
    s32 palette;
    s32 crossEffect;
    s32 next;
    s32 i;
    s32 partner;

    rgb[0] = window->window.brightness;
    rgb[1] = window->window.brightness;
    rgb[2] = window->window.brightness;
    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    player = SUB_PARTNER_PLAYER;
    slot = window->slot;
    SUB_drawPartnerPortrait(player, slot, x + 1, y + 12, window->window.brightness, z);
    if (PLAYER_DATA(player).partners[slot].cardId != 0) {
        SUB_SAVED_PARTNER = PLAYER_DATA(player).partners[slot];
        for (i = 0; i < 3; i++) {
            PLAYER_DATA(player).partners[slot].equippedAbilities[i] = -1;
        }
        updatePartnerStats(player, slot);
        SUB_UNEQUIPPED_PARTNER = PLAYER_DATA(player).partners[slot];
        PLAYER_DATA(player).partners[slot] = SUB_SAVED_PARTNER;
        changed = updatePartnerStats(player, slot);
        for (i = 0; i < 3; i++) {
            if (PLAYER_DATA(player).partners[slot].unlockedArmors[i] != 0) {
                partner = getSlotPartnerIndex(player, slot);
                drawIconColored(x + 2 + i * 13, y, 0, SUB_ARMOR_ICONS[partner][i] + 0x1B, rgb, z);
            }
        }
        drawTextColored(x + 0x32, y, (u8 *)PLAYER_DATA(player).partners[slot].card[0].name, rgb, 7, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].hp, PLAYER_DATA(player).partners[slot].card[0].hp);
        drawIconColored(x + 0x2C, y + 14, 0, 0x1A, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].hp);
        drawTextColored(x + 0x3C, y + 13, buf, rgb, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[0].power, PLAYER_DATA(player).partners[slot].card[0].attack[0].power);
        drawIconColored(x + 0x2C, y + 0x1A, 0, 7, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].attack[0].power);
        drawTextColored(x + 0x3C, y + 0x19, buf, rgb, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[1].power, PLAYER_DATA(player).partners[slot].card[0].attack[1].power);
        drawIconColored(x + 0x2C, y + 0x26, 0, 8, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].attack[1].power);
        drawTextColored(x + 0x3C, y + 0x25, buf, rgb, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[2].power, PLAYER_DATA(player).partners[slot].card[0].attack[2].power);
        drawIconColored(x + 0x2C, y + 0x32, 0, 9, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].attack[2].power);
        drawTextColored(x + 0x3C, y + 0x31, buf, rgb, palette, z);
        crossEffect = PLAYER_DATA(player).partners[slot].card[0].crossEffect;
        palette = 7;
        if (SUB_UNEQUIPPED_PARTNER.card[0].crossEffect != crossEffect) {
            palette = 5;
        }
        sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[crossEffect]);
        drawSmallTextColored(x + 0x56, y + 0x33, buf, palette, rgb, z);
        if (D_8006E4FC[crossEffect] != 0) {
            drawIconColored(x + 0x95, y + 0x31, 0, D_8006E4FC[crossEffect] + 0x14, rgb, z);
        }
        sprintf(buf, "RANK \f\a%3d", (s8)PLAYER_DATA(player).partners[slot].level);
        drawLargeTextColored(x + 0x5C, y + 14, buf, 6, rgb, z);
        next = 0;
        if ((s8)PLAYER_DATA(player).partners[slot].level < 99) {
            next = getExpForNextLevel((s8)PLAYER_DATA(player).partners[slot].level) - (u16)PLAYER_DATA(player).partners[slot].exp;
        }
        sprintf(buf, "NEXT \f\a%3d", next);
        drawLargeTextColored(x + 0x5C, y + 0x18, buf, 6, rgb, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].dpBonus, PLAYER_DATA(player).partners[slot].card[0].dpBonus);
        drawIconColored(x + 0x72, y + 0x26, 0, 0x19, rgb, z);
        sprintf(buf, "*s0%4d", PLAYER_DATA(player).partners[slot].card[0].dpBonus);
        drawTextColored(x + 0x84, y + 0x25, buf, rgb, palette, z);
        drawTextColored(x + 0xA8, y, (u8 *)"Support Effect", rgb, 6, z);
        palette = 7;
        if (changed) {
            palette = 5;
        }
        i = PLAYER_DATA(player).partners[slot].card[0].supportIcon;
        if (i != 0) {
            drawIconColored(x + 0x100, y, 0, i + 0x14, rgb, z);
        }
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, PLAYER_DATA(player).partners[slot].card[0].supportText[i], rgb, palette, z);
        }
    } else {
        drawTextColored(x + 0x78, y + 0x18, (u8 *)"No Data", rgb, 7, z);
    }
}

void SUB_drawArmorChange(UiWindow *window) {
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    s32 prevArmor;
    s32 i;

    if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].armorCardId != 0) {
        drawText(x + 6, y + 1, (s32)"Armor Change with L1 & R1", 7, z);
        prevArmor = SUB_ARMOR_INDEX;
        for (i = 0; i < 3; i++) {
            if ((u16)PAD_STATES[player]->pressed & 4) {
                SUB_ARMOR_INDEX--;
            } else if ((u16)PAD_STATES[player]->pressed & 8) {
                SUB_ARMOR_INDEX++;
            }
            SUB_ARMOR_INDEX = (SUB_ARMOR_INDEX + 3) % 3;
            if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].unlockedArmors[SUB_ARMOR_INDEX] != 0) {
                break;
            }
        }
        if (prevArmor != SUB_ARMOR_INDEX) {
            playMenuSound(1);
            selectPartnerArmor(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT), SUB_ARMOR_INDEX);
        }
    }
}

void SUB_drawPartnerDetails(UiWindow *window) {
    Rect16 rect;
    char buf[64];
    s32 x = window->originX + 3;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    s32 changed;
    s32 canEquip;
    DigimonCardData *card = &PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].card[0];
    s32 ability;
    s32 palette;
    s32 next;
    s32 diff;
    s32 partner;
    s32 i;

    SUB_SAVED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
    for (i = 0; i < 3; i++) {
        PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[i] = -1;
    }
    updatePartnerStats(player, SUB_PARTNER_SLOT);
    SUB_UNEQUIPPED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
    canEquip = 0;
    ability = SUB_ABILITY_IDS[SUB_ABILITY_MENU.row];
    if (SUB_ABILITY_MENU.active != 0) {
        PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
        if (getPartnerAbilityState(player, ability) == 1) {
            canEquip = canEquipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability);
            if (canEquip != 0) {
                PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row] = ability;
                updatePartnerStats(player, SUB_PARTNER_SLOT);
                SUB_PREVIEW_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
            }
        }
    }
    PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
    changed = updatePartnerStats(player, SUB_PARTNER_SLOT);
    SUB_drawPartnerPortrait(player, SUB_PARTNER_SLOT, x, y, 0x80, z);
    drawMediumText(x + 0x2C, y, (s32)card->name, 7, z);
    sprintf(buf, "RANK \f\a%3d", (s8)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].level);
    drawLargeText(x + 0x2C, y + 10, (s32)buf, 6, z);
    next = 0;
    if ((s8)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].level < 99) {
        next = getExpForNextLevel((s8)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].level) - (u16)PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].exp;
    }
    sprintf(buf, "NEXT \f\a%3d", next);
    drawLargeText(x + 0x2C, y + 0x14, (s32)buf, 6, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].hp, card->hp);
    drawIcon(x + 0x2C, y + 0x1E, 0, 0x1A, z);
    sprintf(buf, "*s0%4d", card->hp);
    drawText(x + 0x3A, y + 0x1E, (s32)buf, palette, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].dpBonus, card->dpBonus);
    drawIcon(x + 0x2C, y + 0x2A, 0, 0x19, z);
    sprintf(buf, "*s0%4d", card->dpBonus);
    drawText(x + 0x3A, y + 0x2A, (s32)buf, palette, z);
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].unlockedArmors[i] != 0) {
            partner = getSlotPartnerIndex(player, SUB_PARTNER_SLOT);
            drawIcon(x + 0x76 + i * 20, y, 0, SUB_ARMOR_ICONS[partner][i] + 0x1B, z);
        }
    }
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[0].power, card->attack[0].power);
    drawIcon(x + 0x76, y + 12, 0, 7, z);
    sprintf(buf, "*s0%4d", card->attack[0].power);
    drawText(x + 0x84, y + 12, (s32)buf, palette, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[1].power, card->attack[1].power);
    drawIcon(x + 0x76, y + 0x18, 0, 8, z);
    sprintf(buf, "*s0%4d", card->attack[1].power);
    drawText(x + 0x84, y + 0x18, (s32)buf, palette, z);
    palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[0].attack[2].power, card->attack[2].power);
    drawIcon(x + 0x76, y + 0x24, 0, 9, z);
    sprintf(buf, "*s0%4d", card->attack[2].power);
    drawText(x + 0x84, y + 0x24, (s32)buf, palette, z);
    palette = 7;
    if (SUB_UNEQUIPPED_PARTNER.card[0].crossEffect != card->crossEffect) {
        palette = 5;
    }
    sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
    drawSmallText(x + 0x74, y + 0x30, (s32)buf, palette, z);
    if (D_8006E4FC[card->crossEffect] != 0) {
        drawIcon(x + 0xB2, y + 0x2E, 0, D_8006E4FC[card->crossEffect] + 0x14, z);
    }
    if (canEquip) {
        diff = SUB_PREVIEW_PARTNER.card[0].hp - card->hp;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x54, y + 0x1E, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].dpBonus - card->dpBonus;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x54, y + 0x2A, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].attack[0].power - card->attack[0].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 12, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].attack[1].power - card->attack[1].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 0x18, (s32)buf, palette, z);
        }
        diff = SUB_PREVIEW_PARTNER.card[0].attack[2].power - card->attack[2].power;
        if (diff != 0) {
            if (diff > 0) {
                palette = 5;
            } else {
                palette = 2;
            }
            sprintf(buf, "*s0%+d", diff);
            drawText(x + 0x9E, y + 0x24, (s32)buf, palette, z);
        }
    }
    drawText(x, y + 0x36, (s32)"Support Effect", 6, z);
    palette = 7;
    if (changed) {
        palette = 5;
    }
    if (card->supportIcon != 0) {
        drawIcon(x + 0x58, y + 0x36, 0, card->supportIcon + 0x14, z);
    }
    for (i = 0; i < 4; i++) {
        drawText(x, y + 0x46 + i * 12, (s32)card->supportText[i], palette, z);
    }
    rect.x = x;
    rect.y = y + 0x46;
    rect.w = 0x6E;
    rect.h = 0x30;
    drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
}

void SUB_drawArmorDetails(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 canEquip;
    s32 x = window->originX + 3;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    DigimonCardData *card = &PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].card[1];
    s32 ability;
    s32 palette;
    s32 diff;
    s32 i;

    SUB_drawArmorPortrait(player, SUB_PARTNER_SLOT, x + 0x14, y + 0xA, z);
    if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].armorCardId != 0) {
        SUB_SAVED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
        for (i = 0; i < 3; i++) {
            PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[i] = -1;
        }
        updatePartnerStats(player, SUB_PARTNER_SLOT);
        SUB_UNEQUIPPED_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
        canEquip = 0;
        ability = SUB_ABILITY_IDS[SUB_ABILITY_MENU.row];
        if (SUB_ABILITY_MENU.active != 0) {
            PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
            if (getPartnerAbilityState(player, ability) == 1) {
                canEquip = canEquipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability);
                if (canEquip != 0) {
                    PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row] = ability;
                    updatePartnerStats(player, SUB_PARTNER_SLOT);
                    SUB_PREVIEW_PARTNER = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT];
                }
            }
        }
        PLAYER_DATA(player).partners[SUB_PARTNER_SLOT] = SUB_SAVED_PARTNER;
        updatePartnerStats(player, SUB_PARTNER_SLOT);
        drawMediumText(x, y, (s32)card->name, 7, z);
        drawIcon(x + 0x44, y + 12, 0, SUB_ARMOR_ICONS[getSlotPartnerIndex(player, SUB_PARTNER_SLOT)][getSelectedArmorIndex(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT))] + 0x1B, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].hp, card->hp);
        drawIcon(x, y + 0x3E, 0, 0x1A, z);
        sprintf(buf, "*s0%4d", card->hp);
        drawText(x + 0xE, y + 0x3E, (s32)buf, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].attack[0].power, card->attack[0].power);
        drawIcon(x, y + 0x4A, 0, 7, z);
        sprintf(buf, "*s0%4d", card->attack[0].power);
        drawText(x + 0xE, y + 0x4A, (s32)buf, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].attack[1].power, card->attack[1].power);
        drawIcon(x, y + 0x56, 0, 8, z);
        sprintf(buf, "*s0%4d", card->attack[1].power);
        drawText(x + 0xE, y + 0x56, (s32)buf, palette, z);
        palette = SUB_getStatPalette(SUB_UNEQUIPPED_PARTNER.card[1].attack[2].power, card->attack[2].power);
        drawIcon(x, y + 0x62, 0, 9, z);
        sprintf(buf, "*s0%4d", card->attack[2].power);
        drawText(x + 0xE, y + 0x62, (s32)buf, palette, z);
        palette = 7;
        if (SUB_UNEQUIPPED_PARTNER.card[1].crossEffect != card->crossEffect) {
            palette = 5;
        }
        sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
        drawSmallText(x - 2, y + 0x6E, (s32)buf, palette, z);
        if (D_8006E4FC[card->crossEffect] != 0) {
            drawIcon(x + 0x3C, y + 0x6C, 0, D_8006E4FC[card->crossEffect] + 0x14, z);
        }
        if (canEquip) {
            diff = SUB_PREVIEW_PARTNER.card[1].hp - card->hp;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x3E, (s32)buf, palette, z);
            }
            diff = SUB_PREVIEW_PARTNER.card[1].attack[0].power - card->attack[0].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x4A, (s32)buf, palette, z);
            }
            diff = SUB_PREVIEW_PARTNER.card[1].attack[1].power - card->attack[1].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x56, (s32)buf, palette, z);
            }
            diff = SUB_PREVIEW_PARTNER.card[1].attack[2].power - card->attack[2].power;
            if (diff != 0) {
                if (diff > 0) {
                    palette = 5;
                } else {
                    palette = 2;
                }
                sprintf(buf, "*s0%+d", diff);
                drawText(x + 0x28, y + 0x62, (s32)buf, palette, z);
            }
        }
    } else {
        drawText(x + 0x14, y + 0x4A, (s32)"No Data", 7, z);
    }
}

void SUB_drawEquipment(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 x = window->originX;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 player = SUB_PARTNER_PLAYER;
    s32 i;
    s32 ability;
    s32 palette;

    for (i = 0; i < 3; i++) {
        ability = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[i];
        if (ability == -1) {
            drawText(x, y + i * 14, (s32)"---", 7, z);
            drawText(x + 0x1C, y + i * 14, (s32)"None", 7, z);
        } else {
            palette = 7;
            if (PARTNER_ABILITIES[ability].type == 5) {
                palette = 4;
            }
            if (PARTNER_ABILITIES[ability].type == 7) {
                palette = 5;
            }
            sprintf(buf, "*s0%3.3d", ability);
            drawText(x, y + i * 14, (s32)buf, 7, z);
            drawIcon(x + 0x1A, y + i * 14, 2, PARTNER_ABILITIES[ability].type, z);
            drawText(x + 0x36, y + i * 14, (s32)SUB_ABILITY_TEXTS[ability].name, palette, z);
        }
    }
    updateMenuCursor(&SUB_EQUIPMENT_MENU);
}

void SUB_drawAbilityList(UiWindow *window) {
    char buf[72];
    u8 rgb[4];
    s32 player = SUB_PARTNER_PLAYER;
    s32 x = window->originX;
    s32 z = window->z;
    s32 i;
    s32 ability;
    s32 y;
    s32 palette;
    s32 state;

    for (i = 0; i < SUB_ABILITY_MENU.nrows; i++) {
        if (i < window->view.y / SUB_ABILITY_MENU.rowH) {
            continue;
        }
        if ((window->view.y + window->rect.h) / SUB_ABILITY_MENU.rowH < i) {
            break;
        }
        ability = SUB_ABILITY_IDS[i];
        y = window->originY + i * SUB_ABILITY_MENU.rowH + 1;
        state = getPartnerAbilityState(player, ability);
        switch (state) {
        case 1:
            rgb[0] = 0x80;
            rgb[1] = 0x80;
            rgb[2] = 0x80;
            break;
        case 2:
            rgb[0] = 0x40;
            rgb[1] = 0x40;
            rgb[2] = 0x40;
            break;
        }
        palette = 7;
        if (PARTNER_ABILITIES[ability].type == 5) {
            palette = 4;
        }
        if (PARTNER_ABILITIES[ability].type == 7) {
            palette = 5;
        }
        sprintf(buf, "*s0%3.3d", ability);
        drawTextColored(x, y, buf, rgb, palette, z);
        drawIconColored(x + 0x1A, y, 2, PARTNER_ABILITIES[ability].type, rgb, z);
        drawTextColored(x + 0x36, y, (u8 *)SUB_ABILITY_TEXTS[ability].name, rgb, palette, z);
    }
    updateMenuCursor(&SUB_ABILITY_MENU);
}

void SUB_drawPartnerEquipment(void) {
    s32 i;

    SUB_drawPartnerTitle();
    for (i = 0; i < 3; i++) {
        SUB_PARTNER_TABS[i].window.brightness = 0x40;
    }
    SUB_PARTNER_TABS[SUB_PARTNER_SLOT].window.brightness = 0x80;
    drawWindow(&SUB_PARTNER_TABS[SUB_PARTNER_SLOT].window, SUB_drawPartnerTab, 30);
    for (i = 0; i < 3; i++) {
        if (i != SUB_PARTNER_SLOT) {
            drawWindow(&SUB_PARTNER_TABS[i].window, SUB_drawPartnerTab, 30);
        }
    }
    drawWindow(&SUB_ARMOR_CHANGE_WINDOW, SUB_drawArmorChange, 30);
    drawWindow(&SUB_ARMOR_WINDOW, SUB_drawArmorDetails, 30);
    drawWindow(&SUB_PARTNER_WINDOW, SUB_drawPartnerDetails, 30);
    drawWindow(&SUB_ABILITY_WINDOW, SUB_drawAbilityList, 30);
    drawWindow(&SUB_EQUIPMENT_WINDOW, SUB_drawEquipment, 30);
}

void SUB_runPartnerEquipment(s32 player, s32 parentTask, s32 viewOnly) {
    Rect16 rect;
    u8 dialog[0xB8];
    u32 *archive;
    s32 state;
    s32 count;
    s32 ability;
    s32 result;
    s32 i;

    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\PARTNER.ARC", getCurrentTaskId());
    archive = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(archive[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)archive + archive[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(archive);
    SUB_PARTNER_TITLE_SHOWN = 1;
    SUB_PARTNER_TITLE_Y = -0x20;
    state = 0;
    SUB_PARTNER_SLOT = 0;
    SUB_PARTNER_PLAYER = player;
    SUB_ARMOR_INDEX = 0;
    count = 0;
    for (i = 0; i < 128; i++) {
        SUB_ABILITY_IDS[i] = -1;
    }
    for (i = 0; i < 128; i++) {
        if (getPartnerAbilityState(player, i)) {
            SUB_ABILITY_IDS[count] = i;
            count++;
        }
    }
    SUB_ABILITY_MENU.nrows = count;
    SUB_ABILITY_MENU.pad = player;
    SUB_EQUIPMENT_MENU.pad = player;
    for (i = 0; i < 3; i++) {
        rect.x = 0x10;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&SUB_PARTNER_TABS[i], &rect, -1, (s16 *)-1, 8, 0x46, 0x80, 12);
        SUB_PARTNER_TABS[i].slot = i;
        switch (i) {
        case 0:
            SUB_PARTNER_TABS[i].window.label = (s32)"PARTNER 1 ";
            break;
        case 1:
            SUB_PARTNER_TABS[i].window.label = (s32)"PARTNER 2 ";
            break;
        case 2:
            SUB_PARTNER_TABS[i].window.label = (s32)"PARTNER 3 ";
            break;
        }
    }
    openMenu(&SUB_ABILITY_MENU, &SUB_ABILITY_WINDOW, &SUB_ABILITY_CURSOR, (Bytes4 *)-1);
    SUB_ABILITY_WINDOW.label = (s32)"Digi-Parts List";
    SUB_ABILITY_WINDOW.palette = 4;
    animateWindowTo(&SUB_ABILITY_WINDOW, (Rect16 *)-1);
    SUB_ABILITY_MENU.active = 0;
    openMenu(&SUB_EQUIPMENT_MENU, &SUB_EQUIPMENT_WINDOW, &SUB_EQUIPMENT_CURSOR, (Bytes4 *)-1);
    SUB_EQUIPMENT_WINDOW.label = (s32)"EQUIPMENT";
    animateWindowTo(&SUB_EQUIPMENT_WINDOW, (Rect16 *)-1);
    openWindow(&SUB_ARMOR_CHANGE_WINDOW, &SUB_ARMOR_CHANGE_WINDOW_RECT, -1, (s16 *)-1, 8, 0x32, 0x80, 12);
    animateWindowTo(&SUB_ARMOR_CHANGE_WINDOW, (Rect16 *)-1);
    openWindow(&SUB_PARTNER_WINDOW, &SUB_PARTNER_WINDOW_RECT, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SUB_PARTNER_WINDOW.label = (s32)"PARTNER";
    animateWindowTo(&SUB_PARTNER_WINDOW, (Rect16 *)-1);
    openWindow(&SUB_ARMOR_WINDOW, &SUB_ARMOR_WINDOW_RECT, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SUB_ARMOR_WINDOW.label = (s32)"ARMOR";
    animateWindowTo(&SUB_ARMOR_WINDOW, (Rect16 *)-1);
    addFrameCallback((s32)SUB_drawPartnerEquipment);
    for (;;) {
        func_80014C08(FRAME_INTERVAL);
        switch (state) {
        case -1:
            SUB_PARTNER_TITLE_SHOWN = 0;
            func_80014C08(20);
            removeFrameCallback((s32)SUB_drawPartnerEquipment);
            func_80014A48(parentTask);
            return;
        case 0:
            if (SUB_ARMOR_WINDOW_ANIM_DONE != 0 && SUB_PARTNER_WINDOW_ANIM_DONE != 0) {
                if (PAD_STATES[player]->pressed & 0x1000) {
                    playMenuSound(2);
                    SUB_PARTNER_SLOT--;
                } else if (PAD_STATES[player]->pressed & 0x4000) {
                    playMenuSound(2);
                    SUB_PARTNER_SLOT++;
                }
                SUB_PARTNER_SLOT = (SUB_PARTNER_SLOT + 3) % 3;
            }
            if (PAD_STATES[player]->pressed & 0x40) {
                if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].cardId != 0) {
                    playMenuSound(1);
                    SUB_ARMOR_INDEX = getSelectedArmorIndex(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT));
                    for (i = 0; i < 3; i++) {
                        animateWindowTo(&SUB_PARTNER_TABS[i].window, (Rect16 *)-1);
                    }
                    animateWindowTo(&SUB_PARTNER_WINDOW, &SUB_PARTNER_WINDOW_RECT);
                    if (countUnlockedPartnerArmors(player, getSlotPartnerIndex(player, SUB_PARTNER_SLOT)) >= 2) {
                        animateWindowTo(&SUB_ARMOR_CHANGE_WINDOW, &SUB_ARMOR_CHANGE_WINDOW_RECT);
                    }
                    animateWindowTo(&SUB_ARMOR_WINDOW, &SUB_ARMOR_WINDOW_RECT);
                    animateWindowTo(&SUB_EQUIPMENT_WINDOW, &SUB_EQUIPMENT_MENU.rect);
                    if (viewOnly == 0) {
                        PLAYER_DATA(player).unk56 = getSlotPartnerIndex(0, SUB_PARTNER_SLOT);
                        changeScrollingBackground(PLAYER_DATA(player).unk56, 0x380, 0, 0x380, 0x80);
                    }
                    state = 1;
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                for (i = 0; i < 3; i++) {
                    animateWindowTo(&SUB_PARTNER_TABS[i].window, (Rect16 *)-1);
                }
                playMenuSound(4);
                state = -1;
            }
            break;
        case 1:
            if (PAD_STATES[player]->pressed & 0x40) {
                playMenuSound(1);
                animateWindowTo(&SUB_ABILITY_WINDOW, &SUB_ABILITY_MENU.rect);
                SUB_EQUIPMENT_MENU.active = 0;
                SUB_ABILITY_MENU.active = 1;
                ability = PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row];
                SUB_ABILITY_MENU.row = 0;
                if (ability != -1) {
                    for (i = 0; i < SUB_ABILITY_MENU.nrows && SUB_ABILITY_IDS[i] != ability; i++) {
                        SUB_ABILITY_MENU.row++;
                    }
                }
                centerMenuOnCursor(&SUB_ABILITY_MENU);
                state = 2;
            } else if (PAD_STATES[player]->pressed & 0x80) {
                if (PLAYER_DATA(player).partners[SUB_PARTNER_SLOT].equippedAbilities[SUB_EQUIPMENT_MENU.row] != -1) {
                    playMenuSound(1);
                    unequipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row);
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                playMenuSound(0);
                for (i = 0; i < 3; i++) {
                    rect.x = 0x10;
                    rect.y = i * 65 + 0x26;
                    rect.w = 0x120;
                    rect.h = 0x3C;
                    animateWindowTo(&SUB_PARTNER_TABS[i].window, &rect);
                }
                animateWindowTo(&SUB_ARMOR_WINDOW, (Rect16 *)-1);
                animateWindowTo(&SUB_PARTNER_WINDOW, (Rect16 *)-1);
                animateWindowTo(&SUB_ARMOR_CHANGE_WINDOW, (Rect16 *)-1);
                animateWindowTo(&SUB_EQUIPMENT_WINDOW, (Rect16 *)-1);
                state = 0;
            }
            break;
        case 2:
            if (PAD_STATES[player]->pressed & 0x40) {
                ability = SUB_ABILITY_IDS[SUB_ABILITY_MENU.row];
                result = getPartnerAbilityState(player, ability);
                if (result == 1) {
                    playMenuSound(1);
                    if (canEquipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability)) {
                        equipPartnerAbility(player, SUB_PARTNER_SLOT, SUB_EQUIPMENT_MENU.row, ability);
                        animateWindowTo(&SUB_ABILITY_WINDOW, (Rect16 *)-1);
                        SUB_EQUIPMENT_MENU.active = result;
                        SUB_ABILITY_MENU.active = 0;
                        state = 1;
                    } else {
                        initDialog(dialog, "The same Digi-Part is already used. Only\n1 Digi-Part of each kind can be used.", 0);
                        runDialogForPad((s32 *)dialog, player);
                    }
                }
            } else if (PAD_STATES[player]->pressed & 0x10) {
                playMenuSound(0);
                animateWindowTo(&SUB_ABILITY_WINDOW, (Rect16 *)-1);
                SUB_EQUIPMENT_MENU.active = 1;
                SUB_ABILITY_MENU.active = 0;
                state = 1;
            }
            break;
        }
    }
}

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


Menu SUB_ABILITY_MENU = { NULL, NULL, { 12, 184, 288, 42 }, 0, -1, 0, -1, 0xa, 0x21, 264, 12, 1, 128, 24, 1, 0, 14, 0, 0, 0 };
Menu SUB_EQUIPMENT_MENU = { NULL, NULL, { 12, 184, 288, 42 }, 0, -1, 0, -1, 0x8, 0x21, 264, 12, 1, 3, 26, 1, 0, 14, 0, 0, 0 };
Rect16 SUB_PARTNER_WINDOW_RECT = { 12, 48, 192, 122 };
Rect16 SUB_ARMOR_WINDOW_RECT = { 214, 48, 88, 122 };
Rect16 SUB_ARMOR_CHANGE_WINDOW_RECT = { 146, 19, 157, 14 };

/* the icons of each partner's armors */
u8 SUB_ARMOR_ICONS[6][3] = {
    { 1, 2, 9 },
    { 3, 5, 0 },
    { 4, 6, 0 },
    { 7, 6, 0 },
    { 8, 1, 0 },
    { 1, 7, 0 },
};

/* not referenced by any code */
s16 D_801F1FE2 = 1;

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

void SUB_drawCardList(UiWindow *window) {
    char buf[64];
    s32 x = window->originX - 4;
    s32 z = window->z;
    s32 i;
    s32 y;
    s32 palette;
    s8 type;
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
        type = ((s8 *)SUB_CARD_LIST[i])[2];
        rgb = SUB_CARD_LIST_COLORS[0];
        palette = 7;
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
            drawTextColored(x + 0x46, y, "?", rgb, palette, z);
            drawTextColored(x + 0x86, y, "-------------------", rgb, palette, z);
            drawTextColored(x + 0x74, y, "?", rgb, palette, z);
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
    SUB_COLLECTION_STATS.selectedId = *(s16 *)SUB_COLLECTION_STATS.selectedCard;
}

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
    s32 cardId;

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

void SUB_loadCardMapTim(void) {
    char path[64];
    u32 *tim;

    sprintf(path, "C:\\OBJECT\\c_map.tim");
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, 0x140, 0, 0, 0x1F4);
    freeHeapBlock(tim);
    printf("aaa\n");
}

void SUB_runCardImageCache(void) {
    char path[64];
    s32 i;
    u32 *tim;
    s32 slot;
    s8 found;

    for (i = 0; i < 8; i++) {
        SUB_CARD_IMAGE_CACHE.ids[i] = -1;
        SUB_CARD_IMAGE_CACHE.ages[i] = 0;
    }
    SUB_CARD_IMAGE_CACHE.running = 1;
    SUB_CARD_IMAGE_CACHE.request = -1;
    SUB_CARD_IMAGE_CACHE.busy = 0;
    do {
        func_80014C08(1);
        if (SUB_CARD_IMAGE_CACHE.busy == 0) {
            found = -1;
            for (i = 0; i < 8; i++) {
                if (SUB_CARD_IMAGE_CACHE.request == SUB_CARD_IMAGE_CACHE.ids[i]) {
                    found = i;
                }
            }
            if (found == -1) {
                slot = 0;
                SUB_CARD_IMAGE_CACHE.busy = 1;
                for (i = 1; i < 8; i++) {
                    if (i == 1) {
                        slot = 0;
                    }
                    if (SUB_CARD_IMAGE_CACHE.ages[slot] > SUB_CARD_IMAGE_CACHE.ages[i]) {
                        slot = i;
                    }
                }
                for (i = 0; i < 8; i++) {
                    if (i != slot) {
                        if (--SUB_CARD_IMAGE_CACHE.ages[i] < 0) {
                            SUB_CARD_IMAGE_CACHE.ages[i] = 0;
                        }
                    } else {
                        SUB_CARD_IMAGE_CACHE.ids[i] = SUB_CARD_IMAGE_CACHE.request;
                        SUB_CARD_IMAGE_CACHE.ages[i] = 0;
                    }
                }
                sprintf(path, "B:\\Card\\LC%3.3d.TIM", SUB_CARD_IMAGE_CACHE.ids[slot]);
                func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
                tim = (u32 *)func_80014C08(0x7FFFFFFF);
                uploadTim(tim, (slot / 4) * 32 + 0x140, (slot % 4) * 64 + 0x100, 0, slot + 0x1F4);
                freeHeapBlock(tim);
                SUB_CARD_IMAGE_CACHE.ages[slot] = 100;
                SUB_CARD_IMAGE_CACHE.busy = 0;
            }
        }
    } while (SUB_CARD_IMAGE_CACHE.running != 0);
    SUB_CARD_IMAGE_CACHE.busy = -1;
}

s32 SUB_findCachedCardImage(s16 id) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (id == SUB_CARD_IMAGE_CACHE.ids[i] && SUB_CARD_IMAGE_CACHE.ages[i] != 0) {
            return i;
        }
    }
    return -1;
}

void SUB_loadEditorAssets(void) {
    char path[24];
    u32 *pack;

    sprintf(path, "C:\\OBJECT\\deck.TIS");
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    SUB_CARD_ARCHIVE = (u8 *)func_80014C08(0x7FFFFFFF);
}

void SUB_initPrimBuffers(s8 keepBuffers) {
    s32 i;
    s16 *p;

    if (!keepBuffers) {
        for (i = 0; i < 2; i++) {
            DB(i).primSlots[0] = (s32)(SUB_EDITOR.primBuffers[i] = allocHeapBlock(0xAF0, 0x3C));
        }
        p = allocHeapBlock(0x320, 0x3C);
        SUB_EDITOR.unk8 = p;
    }
    p = SUB_EDITOR.unk8;
    for (i = 0; i < 100; i++) {
        p[1] = 0;
        p[0] = 0;
        p[3] = 0;
        p[2] = 0;
        p += 4;
    }
}

void SUB_drawCardIcon(s16 cardId, s16 x, s16 y, u8 brightness, s32 otIndex) {
    s32 specialty = ((DigimonCardData *)DIGIMON_CARDS)[cardId].attr >> 4;
    s32 u;
    s32 v;
    s32 clutY;
    s16 page;
    s16 column;
    s16 row;
    s32 pageU;
    s32 columnU;

    if (cardId == 300) {
        u = 0x230;
        v = 0x1E0;
    } else {
        page = cardId / 50;
        pageU = page << 6;
        column = cardId % 5;
        columnU = column * 12 + 0x240;
        u = pageU + columnU;
        row = cardId % 50;
        row = row / 5;
        v = row * 24 + 0x100;
    }
    if (cardId >= 0x11D || cardId == 200) {
        clutY = 0x1FE;
    } else if (cardId < 0xBF) {
        clutY = specialty + 0x1F8;
    } else {
        clutY = 0x1FD;
    }
    SUB_drawSprite(x, y, getClut(0x240, clutY), u, v, 24, 24, 1, brightness, -1, otIndex);
}

void SUB_drawSprite(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 tp, u8 brightness, s8 abr, s32 otIndex) {
    u16 tpage = ((tp & 3) << 7) | ((abr & 3) << 5) | ((v & 0x100) >> 4) | ((u & 0x3C0) >> 6) | ((v & 0x200) << 2);

    setlen(&SUB_SPRITE_CURSOR->sp, 4);
    setcode(&SUB_SPRITE_CURSOR->sp, 0x64);
    SUB_SPRITE_CURSOR->sp.clut = clut;
    SUB_SPRITE_CURSOR->sp.w = w;
    SUB_SPRITE_CURSOR->sp.h = h;
    SUB_SPRITE_CURSOR->sp.x0 = x;
    SUB_SPRITE_CURSOR->sp.y0 = y;
    if (tp != 0) {
        SUB_SPRITE_CURSOR->sp.u0 = (u % 64) << 1;
    } else {
        SUB_SPRITE_CURSOR->sp.u0 = (u % 64) << 2;
    }
    SUB_SPRITE_CURSOR->sp.v0 = v;
    SUB_SPRITE_CURSOR->sp.r0 = brightness;
    SUB_SPRITE_CURSOR->sp.g0 = brightness;
    SUB_SPRITE_CURSOR->sp.b0 = brightness;
    if (abr >= 0) {
        tpage |= (abr & 3) << 5;
        setSemiTrans(&SUB_SPRITE_CURSOR->sp, 1);
    } else {
        setSemiTrans(&SUB_SPRITE_CURSOR->sp, 0);
    }
    setDrawMode(&SUB_SPRITE_CURSOR->dm, 0, 0, tpage);
    addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &SUB_SPRITE_CURSOR->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &SUB_SPRITE_CURSOR->dm);
    SUB_SPRITE_CURSOR++;
}

void SUB_runDeckEditor(s32 player, s32 parentTask) {
    SUB_loadEditorAssets();
    SUB_initPrimBuffers(0);
    SUB_EDITOR.running = 1;
    SUB_EDITOR.player = player;
    SUB_CARD_LIST_MENU_PAD = SUB_EDITOR.player;
    SUB_CARD_SORT_MENU_PAD = SUB_EDITOR.player;
    SUB_DECK_SORT_MENU_PAD = SUB_EDITOR.player;
    func_800149B8(0, -1, 0, 0x1000, SUB_EDITOR.task = SUB_runCardList, 0, 0, 0, 0);
    func_800149B8(0, -1, 0, 0x1000, SUB_runCardImageCache, 0, getCurrentTaskId(), 0, 0);
    do {
        func_80014C08(1);
        SUB_SPRITE_CURSOR = SUB_EDITOR.primBuffers[FRAME_BUFFER_INDEX];
    } while (SUB_EDITOR.running == 1);
    SUB_CARD_IMAGE_CACHE.running = 0;
    do {
        func_80014C08(1);
    } while (SUB_CARD_IMAGE_CACHE.busy != -1);
    clearCollectionNewFlags(player);
    func_80014C08(2);
    freeHeapBlocksByTag(0x3C);
    freeHeapBlock(SUB_CARD_ARCHIVE);
    func_80014A48(parentTask);
}

void SUB_openCenteredWindow(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style) {
    Rect16 rect;
    s32 unused[2]; /* unused, but it is in the original stack frame */
    Rect16 view;

    rect.x = area.x - area.w / 2;
    rect.y = area.y - area.h / 2;
    rect.w = area.w & ~1;
    rect.h = area.h & ~1;
    view.x = 0;
    view.y = 0;
    view.w = (area.w + 10) & ~1;
    view.h = 0x2000;
    openWindow(window, &rect, -1, (s16 *)&view, flags, style, 0x80, 12);
    window->label = label;
}

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
        SUB_COLLECTION_COUNTS[i] = 0;
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
                SUB_COLLECTION_COUNTS[j] += entry->count;
            } else {
                break;
            }
        }
    }
    for (i = 0; i < 7; i++) {
        SUB_COLLECTION_STATS.counts[7] += SUB_COLLECTION_COUNTS[i];
    }
}

void SUB_drawCardListScreen(void) {
    Rect16 uv;
    s16 y;

    drawWindow(&D_801F33E8, SUB_drawCardData, 0);
    drawWindow(&SUB_CARD_SORT_WINDOW, SUB_drawCardSortMenu, 0);
    drawWindow(SUB_WINDOWS, SUB_drawCardCountPage, 0);
    drawWindow(D_801F3294, SUB_drawCardList, 0);
    drawWindow(&D_801F32D8, SUB_drawCardListHelp, 0);
    drawWindow(&D_801F331C, SUB_drawCollectionTotals, 0);
    drawWindow(&D_801F3360, SUB_drawSpecialtyCounts, 0);
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

const char SUB_STR_L1_BACK[] = "L1BACK";

const char SUB_STR_CARD_LIST[] = "CARD LIST";

const char SUB_STR_HELP[] = "HELP";

const char SUB_STR_PARTNER_TITLE[] = "PARTNER";

const char SUB_STR_CARD_DATA[] = "CARD DATA";

void SUB_runCardList(void) {
    Rect16 rects[7];
    Rect16 infoRect;
    const char *labels[7] = { SUB_STR_L1_BACK, SUB_STR_CARD_LIST, SUB_STR_HELP, SUB_STR_HELP, SUB_STR_PARTNER_TITLE, SUB_STR_HELP, SUB_STR_CARD_DATA };
    s32 running;
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
    openMenu(&SUB_CARD_LIST_MENU, D_801F3294, &SUB_CARD_LIST_CURSOR, (Bytes4 *)-1);
    D_801F32C0 = (s32)SUB_STR_CARD_LIST;
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
    animateWindowTo(&D_801F33E8, (Rect16 *)-1);
    D_801F4350 = allocTaskHeapBlock(0x2A);
    for (i = 0; i < 8; i++) {
        SUB_COLLECTION_STATS.lists[i] = allocTaskHeapBlock(0xA4);
    }
    SUB_initCollectionStats();
    SUB_COLLECTION_SHOW_INFO = 0;
    SUB_EDITOR.slide = 0;
    SUB_EDITOR.hidden = 0;
    playMenuSound(3);
    addFrameCallback((s32)SUB_drawCardListScreen);
    do {
        func_80014C08(1);
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
                    animateWindowTo(&D_801F33E8, &infoRect);
                    animateWindowTo(D_801F3294, (Rect16 *)-1);
                } else {
                    playMenuSound(3);
                    SUB_CARD_LIST_MENU.active = 1;
                    animateWindowTo(D_801F3294, &SUB_CARD_LIST_MENU.rect);
                    animateWindowTo(&D_801F33E8, (Rect16 *)-1);
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
                animateWindowTo(D_801F3294, &SUB_CARD_LIST_MENU.rect);
                animateWindowTo(&D_801F33E8, (Rect16 *)-1);
            }
        }
    } while (running);
    SUB_EDITOR_HIDDEN = -1;
    func_80014C08(20);
    removeFrameCallback((s32)SUB_drawCardListScreen);
    if (action == 3) {
        func_800149B8(0, -1, 0, 0x1000, SUB_runDeckMenu, 0, 0, 0, 0);
    } else if (action == 4) {
        func_800149B8(0, -1, 0, 0x1000, SUB_runPartnerEquipment, SUB_EDITOR_PLAYER, 0, 0, 0);
    } else if (action == 5) {
        SUB_EDITOR_RUNNING = 0;
    }
    func_80014A90();
}

void SUB_drawCardCountPage(UiWindow *window) {
    char buf[72];
    u8 palettes[8] = { 2, 1, 4, 9, 6, 8, 8, 8 };
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    CardCount *entry;
    s32 i;

    if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 4) {
        SUB_COLLECTION_STATS.page--;
    } else if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 8) {
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
    if (SUB_COLLECTION_SHOW_INFO == 0) {
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
            sprintf(buf, SUB_FMT_3_DIGITS, SUB_COLLECTION_COUNTS[i]);
            drawText(x + 0x10 + (i / 4) * 95, y + (i % 4) * 13, (s32)buf, 7, z);
            drawText(x + 0x25 + (i / 4) * 95, y + (i % 4) * 13, (s32)SUB_STR_CARDS, 7, z);
        } else {
            drawText(x + (i / 4) * 80 - 6, y + 0x27, (s32)SUB_STR_TOTAL, 7, z);
            sprintf(buf, SUB_FMT_4_DIGITS, SUB_COLLECTION_COUNTS[i]);
            drawText(x + 0x19 + (i / 4) * 80, y + 0x27, (s32)buf, 7, z);
            drawText(x + 0x34 + (i / 4) * 80, y + (i % 4) * 13, (s32)SUB_STR_CARDS, 7, z);
        }
    }
}

const char SUB_STR_CARD_INFO[] = "CARD INFO.";

const char SUB_FMT_3_DIGITS[] = "*s0%3d";

const char SUB_STR_TOTAL[] = "Total";

const char SUB_FMT_4_DIGITS[] = "*s0%4d";

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

/* the specialties' names in Japanese; not referenced by any code */
char *SUB_SPECIALTY_NAMES_JP[5] = {
    "\x89\xCE\x89\x8A", /* 火炎 */
    "\x95X\x90\x85", /* 氷水 */
    "\x8E\xA9\x91R", /* 自然 */
    "\x88\xC3\x8D\x95", /* 暗黒 */
    "\x92\xBF\x8E\xED", /* 珍種 */
};

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
    1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0,
};

/* the u8 arrays among these are not referenced by any code */
UiWindow SUB_NAME_ENTRY_WINDOW = { 0 };
u8 D_801F233C[12] = { 0 };
CursorHighlight SUB_NAME_ENTRY_CURSOR = { { { 0 } } };
UiWindow SUB_DECK_NAME_WINDOW = { 0 };
u8 D_801F23DC[12] = { 0 };
CursorHighlight SUB_DECK_NAME_CURSOR = { { { 0 } } };
UiWindow SUB_NAME_ENTRY_HELP_WINDOW = { 0 };
u8 D_801F247C[12] = { 0 };
NameEntry SUB_NAME_ENTRY = { 0 };
u8 D_801F24AC[4] = { 0 };
UiWindow SUB_BASE_DECK_WINDOW = { 0 };
u8 D_801F24F4[12] = { 0 };
CursorHighlight SUB_BASE_DECK_CURSOR = { { { 0 } } };
u16 SUB_BASE_DECK_ENTRIES[0x9F] = { 0 };
UiWindow SUB_ABILITY_WINDOW = { 0 };
u8 D_801F26D4[12] = { 0 };
CursorHighlight SUB_ABILITY_CURSOR = { { { 0 } } };
UiWindow SUB_EQUIPMENT_WINDOW = { 0 };
u8 D_801F2774[12] = { 0 };
CursorHighlight SUB_EQUIPMENT_CURSOR = { { { 0 } } };
UiWindow SUB_ARMOR_CHANGE_WINDOW = { 0 };
u8 D_801F2814[12] = { 0 };
UiWindow SUB_PARTNER_WINDOW = { 0 };
u8 D_801F2864[12] = { 0 };
TabWindow SUB_PARTNER_TABS[3] = { { { 0 } } };
u8 D_801F2948[8] = { 0 };
UiWindow SUB_ARMOR_WINDOW = { 0 };
s32 SUB_PARTNER_SLOT = 0;
s32 SUB_ARMOR_INDEX = 0;
s32 SUB_PARTNER_PLAYER = 0;
Partner SUB_SAVED_PARTNER = { { { 0 } } };
u8 D_801F2C38[8] = { 0 };
Partner SUB_UNEQUIPPED_PARTNER = { { { 0 } } };
u8 D_801F2ED8[8] = { 0 };
Partner SUB_PREVIEW_PARTNER = { { { 0 } } };
s32 SUB_PARTNER_TITLE_SHOWN = 0;
s32 SUB_PARTNER_TITLE_Y = 0;
s8 SUB_ABILITY_IDS[128] = { 0 };
CursorHighlight SUB_CARD_LIST_CURSOR = { { { 0 } } };
UiWindow SUB_WINDOWS[7] = { { 0 } };
u8 *SUB_CARD_ARCHIVE = NULL;
UiWindow SUB_CARD_SORT_WINDOW = { 0 };
u8 D_801F3474[12] = { 0 };
CursorHighlight SUB_CARD_SORT_CURSOR = { { { 0 } } };
void *SUB_CARD_LIST[301] = { 0 };
u8 D_801F3984[12] = { 0 };
s8 *SUB_CARDS_BY_ID[301] = { 0 };
u8 D_801F3E44[12] = { 0 };
UiWindow SUB_DECK_SORT_WINDOW = { 0 };
u8 D_801F3E94[12] = { 0 };
CursorHighlight SUB_DECK_SORT_CURSOR = { { { 0 } } };
CursorHighlight SUB_AUTO_DECK_CURSOR = { { { 0 } } };
s8 *SUB_AUTO_DECK_CARD = NULL;
u8 D_801F3F44[4] = { 0 };
DeckEditState SUB_DECK_EDIT = { 0 };
s8 SUB_DECK_IS_NEW = 0;
u8 D_801F405C[4] = { 0 };
DeckMenuState SUB_DECK_MENU = { { 0 } };
u8 SUB_AUTO_DECK_OPTIONS[6] = { 0 };
u8 D_801F4078[0x110] = { 0 };
CardImageCache SUB_CARD_IMAGE_CACHE = { 0 };
u8 D_801F41A4[4] = { 0 };
EditorState SUB_EDITOR = { { 0 } };
u8 D_801F41C4[4] = { 0 };
PlayerDeck SUB_ORIGINAL_DECK = { 0 };
CardIdList *SUB_AUTO_DECK_POOLS = NULL;
u8 D_801F42DC[4] = { 0 };
char SUB_LABEL_BUFFER[0x44] = { 0 };
DeckCardCounts *SUB_DECK_CARD_COUNTS = NULL;
PlayerDeck *SUB_EDITED_DECK = NULL;
u8 D_801F432C[4] = { 0 };
CollectionStats SUB_COLLECTION_STATS = { { 0 } };
SprtPacket *SUB_SPRITE_CURSOR = NULL;

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
                    func_80029EC4(x + 0x4B, y + (i + 3) * 12, 7, 1, z, (s32)buf);
                    sprintf(buf, "*s0%4d/%4d", (u16)PLAYER_DATA(SUB_EDITOR.player).unkD3C[*(s16 *)SUB_COLLECTION_STATS.selectedCard][i], ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->attack[i].power);
                    drawText(x + 0x6F, y + (i + 3) * 12, (s32)buf, 7, z);
                }
                drawSmallText(x + 0x57, y + 0x48, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->crossEffect], 7, z);
                if (D_8006E4FC[((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->crossEffect] != 0) {
                    drawIcon(x + 0x91, y + 0x4E, 0, D_8006E4FC[((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->crossEffect] + 0x14, z);
                }
                drawIcon(x + 0x4B, y + 0x18, 0, 0x18, z);
                sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->dpCost);
                drawText(x + 0x69, y + 0x18, (s32)buf, 7, z);
                drawIcon(x + 0x81, y + 0x18, 0, 0x19, z);
                sprintf(buf, SUB_FMT_2_DIGITS, ((DigimonCardData *)SUB_COLLECTION_STATS.selectedCard)->dpBonus);
                drawText(x + 0x93, y + 0x18, (s32)buf, 7, z);
                sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).unk11B6[*(s16 *)SUB_COLLECTION_STATS.selectedCard]);
                drawText(x + 0xBD, y + 12, (s32)buf, 7, z);
                drawText(x + 0xD5, y + 12, (s32)SUB_STR_WINS, 6, z);
                sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).unk1334[*(s16 *)SUB_COLLECTION_STATS.selectedCard]);
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
                sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR.player, *(s16 *)SUB_COLLECTION_SELECTED_CARD));
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
    sprintf(buf, "%s Deck", deck->unk1);
    drawText(x, y, (s32)buf, palette, z);
    battles = deck->unk108[1] + deck->unk108[2];
    if (battles >= 1000) {
        battles = 999;
    }
    sprintf(buf, SUB_FMT_3_DIGITS, battles);
    drawText(x + 0x62, y, (s32)buf, palette, z);
    drawTinyText(x + 0x76, y + 7, (s32)"Battles", palette, z);
    sprintf(buf, SUB_FMT_3_DIGITS, deck->unk108[1]);
    drawText(x + 0x95, y, (s32)buf, palette, z);
    drawTinyText(x + 0xA9, y + 7, (s32)SUB_STR_WINS, palette, z);
    sprintf(buf, SUB_FMT_3_DIGITS, deck->unk108[2]);
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
    for (i = 0; i < 3; i++) {
        SUB_DECK_MENU.current = i;
        if (PLAYER_DATA(SUB_EDITOR.player).savedDecks[i].inUse != 0) {
            drawWindow(&D_801F3294[i], SUB_drawDeckSummary, 30);
        } else {
            drawWindow(&D_801F3294[i], SUB_drawEmptyDeck, 30);
        }
    }
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
    s32 running = 1;
    s32 created = 0;
    s32 action;
    s32 i;
    s32 flags;
    s8 result;
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
        func_80014C08(1);
        if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 0x10) {
            action = 1;
        } else if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 0x40) {
            action = 2;
        } else if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 0x800) {
            action = 3;
        } else if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x1000) {
            if (SUB_DECK_MENU_SLOT == 1) {
                PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
            }
            action = 4;
        } else if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x4000) {
            if (SUB_DECK_MENU_SLOT == 1) {
                PAD_STATES[SUB_EDITOR_PLAYER]->repeatEnabled = 0;
            }
            action = 5;
        } else if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x2000) {
            action = 6;
        } else if ((u16)PAD_STATES[SUB_EDITOR_PLAYER]->repeat & 0x8000) {
            action = 7;
        } else if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 0x20) {
            action = 8;
        } else if (PAD_STATES[SUB_EDITOR_PLAYER]->pressed & 0x80) {
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
                result = dialog[0xA5];
                if (result != 1) {
                    break;
                }
                PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].unk108[1] = PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].unk108[2] = 0;
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
                if (PLAYER_DATA(SUB_EDITOR_PLAYER).savedDecks[SUB_DECK_MENU_SLOT].inUse == 0) {
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
                            PLAYER_DATA(SUB_EDITOR.player).savedDecks[i].unk108[1] = PLAYER_DATA(SUB_EDITOR.player).savedDecks[i].unk108[2] = 0;
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
    SUB_EDITOR_HIDDEN = -1;
    func_80014C08(20);
    removeFrameCallback((s32)SUB_drawDeckMenu);
    func_80014C08(1);
    switch (action) {
    case 1:
        func_800149B8(0, -1, 0, 0x1000, SUB_EDITOR.task, 0, 0, 0, 0);
        break;
    case 2:
        if (created == 1) {
            sprintf(PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].unk1, "NEW ");
            func_800149B8(0, -1, 0, 0x1000, SUB_enterDeckName, 0, PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].unk1, SUB_EDITOR.player, 0);
        } else {
            func_800149B8(0, -1, 0, 0x1000, SUB_editDeck, &PLAYER_DATA(SUB_EDITOR_PLAYER).savedDecks[SUB_DECK_MENU_SLOT], 0, 0, 0);
        }
        break;
    case 3:
        func_800149B8(0, -1, 0, 0x1000, SUB_enterDeckName, 1, PLAYER_DATA(SUB_EDITOR_PLAYER).savedDecks[SUB_DECK_MENU_SLOT].unk1, SUB_EDITOR_PLAYER, 0);
        break;
    }
}

void SUB_uploadArchiveTim(s16 index) {
    u8 unused[0x40];

    uploadTim((u32 *)(SUB_CARD_ARCHIVE + ((s32 *)SUB_CARD_ARCHIVE)[index + 1]), 0x220, 0x100, 0x240, 0x1F5);
}

void SUB_drawDeckEdit(void) {
    Rect16 uv;
    s16 y;

    drawWindow(&SUB_CARD_SORT_WINDOW, SUB_drawCardSortMenu, 29);
    drawWindow(&SUB_DECK_SORT_WINDOW, SUB_drawDeckSortMenu, 29);
    drawWindow(&D_801F32D8, SUB_drawPickerList, 30);
    drawWindow(SUB_WINDOWS, SUB_drawPickerCardInfo, 30);
    drawWindow(D_801F3294, SUB_drawDeckEditTitle, 30);
    drawWindow(&D_801F331C, SUB_drawDeckStats, 30);
    drawWindow(&D_801F3360, SUB_drawDeckGrid, 30);
    drawWindow(&D_801F33A4, SUB_drawSlotCardInfo, 30);
    drawWindow(&D_801F33E8, SUB_drawSortHint, 30);
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

const char SUB_STR_SUM[] = "SUM";

const char SUB_STR_DECK_TITLE[] = "DECK";

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
    SUB_EDITOR_USE_DECK_COUNTS = 1;
    SUB_initCardList();
    SUB_DECK_EDIT_MODE = 1;
    SUB_CARD_LIST_MENU.rect.h = 0x60;
    SUB_CARD_LIST_MENU.ox = 0x62;
    openMenu(&SUB_CARD_LIST_MENU, &D_801F32D8, &SUB_CARD_LIST_CURSOR, (Bytes4 *)-1);
    D_801F3304 = (s32)SUB_STR_CARD_LIST;
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
    animateWindowTo(&D_801F32D8, (Rect16 *)-1);
}

s32 SUB_canAddCardToDeck(s32 cardId) {
    s16 unused[6] = { 0xAF, 0xB6, 0xB7, 0xB8, 0xBB, 0xBE }; /* unused, but it is in the original stack frame */
    s32 unused2[2];
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
    s16 w;
    s16 h;
    s16 ty;
    s32 result;
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
            animateWindowTo(&D_801F32D8, &SUB_CARD_LIST_MENU.rect);
            animateWindowTo(&D_801F331C, (Rect16 *)-1);
            animateWindowTo(&D_801F3360, (Rect16 *)-1);
            animateWindowTo(&D_801F33A4, (Rect16 *)-1);
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
    s16 w;
    s16 h;

    SUB_DECK_EDIT.mode = 1;
    animateWindowTo(SUB_WINDOWS, (Rect16 *)-1);
    animateWindowTo(&D_801F32D8, (Rect16 *)-1);
    from.x = 0x23;
    from.y = 0xAD;
    from.w = 0x36;
    from.h = 0x66;
    w = from.w;
    to.x = 0x8;
    h = from.h;
    to.y = 0x7A;
    to.w = w;
    to.h = h;
    animateWindowTo(&D_801F331C, &to);
    from.x = 0xBF;
    from.y = 0xBB;
    from.w = 0xF2;
    from.h = 0x4A;
    w = from.w;
    to.x = 0x46;
    h = from.h;
    to.y = 0x96;
    to.w = w;
    to.h = h;
    animateWindowTo(&D_801F3360, &to);
    from.x = 0xBF;
    from.y = 0x5C;
    from.w = 0xF2;
    from.h = 0x64;
    w = from.w;
    to.x = 0x46;
    h = from.h;
    to.y = 0x2A;
    to.w = w;
    to.h = h;
    animateWindowTo(&D_801F33A4, &to);
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
    u8 unused[0xC0]; /* unused, but it is in the original stack frame */
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
        func_80014C08(1);
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
    func_80014C08(20);
    removeFrameCallback((s32)SUB_drawDeckEdit);
    func_80014C08(1);
    func_800149B8(0, -1, 0, 0x1000, SUB_runDeckMenu, 0, 0, 0, 0);
}

void SUB_drawSortHint(UiWindow *window) {
    char buf[72]; /* unused, but it is in the original stack frame */
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;

    drawIcon(x, y, 1, 12, z);
    drawMediumText(x + 8, y, (s32)":Sort", 7, z);
}

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
            sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).unk11B6[*(s16 *)SUB_CARDS_BY_ID[cardId]]);
            drawText(x + 0x1B, y + 12, (s32)buf, 7, z);
            drawText(x + 0x33, y + 12, (s32)SUB_STR_WINS, 6, z);
            sprintf(buf, SUB_FMT_3_DIGITS, PLAYER_DATA(SUB_EDITOR.player).unk1334[*(s16 *)SUB_CARDS_BY_ID[cardId]]);
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
                func_80029EC4(x + 0x4B, y + (i + 2) * 12, 7, 1, z, (s32)buf);
                sprintf(buf, SUB_FMT_4_DIGITS, ((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->attack[i].power);
                drawText(x + 0x5D, y + (i + 2) * 12, (s32)buf, 7, z);
            }
            drawSmallText(x + 0x45, y + 0x3F, (s32)CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->crossEffect], 7, z);
            if (D_8006E4FC[((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->crossEffect] != 0) {
                drawIcon(x + 0x51, y + 0x48, 0, D_8006E4FC[((DigimonCardData *)SUB_CARDS_BY_ID[cardId])->crossEffect] + 0x14, z);
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

const char SUB_STR_DISABLE[] = "Disable";

void SUB_drawPickerList(UiWindow *window) {
    s32 unused[2]; /* unused, but it is in the original stack frame */
    char buf[64];
    s32 x = window->originX - 11;
    s32 z = window->z;
    s32 i;
    s32 y;
    s32 palette;
    s8 type;
    u8 *rgb;
    s32 usable;

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
            drawTextColored(x + 0x59, y, "?", rgb, palette, z);
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

    if (PLAYER_DATA(SUB_EDITOR.player).cardCollection[SUB_DECK_EDIT_CARD_ID] & 0x40) {
        SUB_CARD_IMAGE_CACHE.request = SUB_DECK_EDIT_CARD_ID;
        slot = SUB_findCachedCardImage(SUB_DECK_EDIT_CARD_ID);
        if (slot == -1) {
            if (++SUB_EDITOR.blink & 4) {
                SUB_drawSprite(x + 3, y + 14, 0x7E20, 0x200, 0x100, 0x40, 0x40, 0, 0x80, slot, z);
            } else {
                SUB_drawSprite(x + 3, y + 14, 0x7E20, 0x210, 0x100, 0x40, 0x40, 0, 0x80, -1, z);
            }
        } else {
            SUB_EDITOR_BLINK = 0;
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
        switch (SUB_DECK_EDIT_CARD[2]) {
        case 0:
            drawIcon(x + 0x63, y, 0, ((DigimonCardData *)SUB_DECK_EDIT.card)->attr >> 4, z);
            drawIcon(x + 0x27, y, 0, (((DigimonCardData *)SUB_DECK_EDIT.card)->attr & 0xF) + 0x10, z);
            drawText(x + 0x1B, y, (s32)SUB_STR_LV, 7, z);
            drawText(x + 0x45, y, (s32)SUB_STR_TYPE, 7, z);
            sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR_PLAYER, *(s16 *)SUB_DECK_EDIT.card));
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
            if (D_8006E4FC[((DigimonCardData *)SUB_DECK_EDIT.card)->crossEffect] != 0) {
                drawIcon(x + 0x91, y + 0x45, 0, D_8006E4FC[((DigimonCardData *)SUB_DECK_EDIT.card)->crossEffect] + 0x14, z);
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
            sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR_PLAYER, *(s16 *)SUB_DECK_EDIT.card));
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
            sprintf(buf, SUB_FMT_COUNT, getOwnedCardCount(SUB_EDITOR_PLAYER, *(s16 *)SUB_DECK_EDIT_CARD));
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
        sprintf(buf, "%s Deck", SUB_EDITED_DECK->unk1);
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
    s8 result;

    playMenuSound(3);
    SUB_openAutoDeckMenu();
    addFrameCallback((s32)SUB_drawAutoDeckMenu);
    do {
        func_80014C08(1);
    } while (SUB_AUTO_DECK_OPTIONS[5] == 0);
    animateWindowTo(SUB_WINDOWS, (Rect16 *)-1);
    func_80014C08(20);
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
            result = dialog[0xA5];
            if (result == 1 && SUB_chooseBaseDeck(&PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot], SUB_EDITOR.player) >= 0) {
                PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse = result;
            }
        }
        func_800149B8(0, -1, 0, 0x1000, SUB_editDeck, &PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot], 0, 0, 0);
        break;
    case 3:
        PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].inUse = 0;
        func_800149B8(0, -1, 0, 0x1000, SUB_enterDeckName, 0, PLAYER_DATA(SUB_EDITOR.player).savedDecks[SUB_DECK_MENU.slot].unk1, SUB_EDITOR.player, 0);
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
    s32 level = 0;
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
            if (SUB_AUTO_DECK_POOLS[i * 3].count + SUB_AUTO_DECK_POOLS[i * 3 + 1].count + SUB_AUTO_DECK_POOLS[i * 3 + 2].count >= count) {
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
    if (n < 30) {
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
    }
    linkDeckCardData(SUB_EDITOR.player, deck);
}

