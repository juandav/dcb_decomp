#ifndef DCB_NISSEG_H
#define DCB_NISSEG_H

#include "game.h"

/* NISSEG, jp's overlay of the title screen, the card trade, the VS mode and
   the deck screens. us has none; eu's is a Japanese debug build of it, with
   debug menus, which eu builds as jp's with JP_DEBUG_BUILD (include/version.h)
   against another executable than eu's (config/eu/symbols_nisseg_exe.txt) */

/* a sprite as jp's executable draws it (drawPrimDesc): its primitive's
   tag, colour and code, then where its texture is and where it goes */
typedef struct {
    /* 0x00 */ s32 tag;
    /* 0x04 */ u8 r;
    /* 0x05 */ u8 g;
    /* 0x06 */ u8 b;
    /* 0x07 */ u8 code;
    /* 0x08 */ u8 u;
    /* 0x09 */ u8 v;
    /* 0x0A */ u16 clut;
    /* 0x0C */ u8 u1;
    /* 0x0D */ u8 v1;
    /* 0x0E */ u16 tpage;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 w;
    /* 0x16 */ s16 h;
} NisSprite;

/* a window of jp's executable (runWindowTask's task opens it) */
typedef struct NisWindow {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ u8 state; /* 4 closes it */
    /* 0x02 */ u8 unk2[0x36];
    /* 0x38 */ u8 frame[0x16]; /* what drawWindowFrame draws a frame with */
    /* 0x4E */ s16 z;
} NisWindow;

/* how a window opens: it grows from one rectangle to the other */
typedef struct {
    /* 0x00 */ Rect16 from;
    /* 0x08 */ Rect16 to;
    /* 0x10 */ s32 style;
    /* 0x14 */ s32 flags;
    /* 0x18 */ void (*render)(NisWindow *window);
    /* 0x1C */ void (*close)(NisWindow *window);
} NisWindowDef;

/* a menu of jp's executable (openChoiceMenuFromList sets it up) */
typedef struct {
    /* 0x000 */ u8 unk0[0x208];
    /* 0x208 */ void (*handlers[13])(); /* what each item runs */
    /* 0x23C */ s32 choice;
    /* 0x240 */ s32 count; /* of items */
    /* 0x244 */ s32 unk244;
    /* 0x248 */ s8 unk248; /* nonzero while it opens or closes */
    /* 0x249 */ u8 unk249[3];
    /* 0x24C */ s32 unk24C;
} NisMenu;

/* jp's cards: 110 Digimon, then 43 option cards, then 6 others */
#define NIS_DIGIMON_COUNT 0x6E
#define NIS_OPTION_COUNT 0x2B
#define NIS_OTHER_COUNT 6
#define NIS_CARD_COUNT (NIS_DIGIMON_COUNT + NIS_OPTION_COUNT + NIS_OTHER_COUNT)

/* jp's Digimon cards (DIGIMON_CARDS points to them): only what NISSEG reads */
typedef struct {
    /* 0x00 */ s16 power;
    /* 0x02 */ s16 effects[2];
    /* 0x06 */ char name[0x12];
} NisAttack;

typedef struct {
    /* 0x000 */ s16 id; /* 0: no such card */
    /* 0x002 */ u8 type;
    /* 0x003 */ char name[0x11];
    /* 0x014 */ u8 elementLevel; /* the element in the high nibble, the level in the low one */
    /* 0x015 */ s8 dpCost;
    /* 0x016 */ s8 dpBonus;
    /* 0x017 */ u8 unk17;
    /* 0x018 */ s16 hp;
    /* 0x01A */ NisAttack attacks[3];
    /* 0x062 */ u8 unk62[0xD2 - 0x62];
    /* 0x0D2 */ s8 supportIcon;
    /* 0x0D3 */ u8 unkD3;
    /* 0x0D4 */ s8 supportLevel;
    /* 0x0D5 */ char supportText[4][0x13];
    /* 0x121 */ u8 unk121;
} NisCardData;

/* jp's option cards (OPTION_CARDS points to them) */
typedef struct {
    /* 0x00 */ s16 id; /* 0: no such card */
    /* 0x02 */ u8 unk2;
    /* 0x03 */ char name[0x12];
    /* 0x15 */ u8 unk15[0x71];
    /* 0x86 */ s8 level;
    /* 0x87 */ char text[4][0x13];
    /* 0xD3 */ u8 unkD3;
} NisOptionData;

/* jp's Digivolve cards (DIGIVOLVE_CARDS points to them) */
typedef struct {
    /* 0x00 */ s16 id; /* 0: no such card */
    /* 0x02 */ u8 unk2;
    /* 0x03 */ char name[0x12];
    /* 0x15 */ char text[4][0x13];
    /* 0x61 */ u8 unk61;
} NisOtherData;
#define NIS_CARD_ELEMENT(id) (NIS_DIGIMON_CARDS[id].elementLevel >> 4)
#define NIS_CARD_LEVEL(id) (NIS_DIGIMON_CARDS[id].elementLevel & 0xF)

/* a saved deck of jp's profiles */
typedef struct {
    /* 0x000 */ u8 inUse;
    /* 0x001 */ char name[0xF];
    /* 0x010 */ CardSlot cards[30];
    /* 0x100 */ u16 wins;
    /* 0x102 */ u16 losses;
    /* 0x104 */ s16 unk104[3];
    /* 0x10A */ u8 unk10A[2];
} NisDeck;

/* jp keeps a profile for each of the two players, PLAYER_PROFILES points to
   both; only the fields NISSEG reads are placed */
typedef struct {
    /* 0x0000 */ char name[0x1C];
    /* 0x001C */ u16 versusWins;
    /* 0x001E */ u16 versusLosses;
    /* 0x0020 */ u16 profileId; /* the same in both: the same save */
    /* 0x0022 */ u8 unk22[6];
    /* 0x0028 */ u32 unk28_0 : 10;
    /* 0x0029 */ u32 tradeUnlocked : 1; /* the player has 100 cards */
    /* 0x0029 */ u32 unk28_11 : 1;
    /* 0x0029 */ u32 showsRecords : 1; /* the deck screens show the records */
    /* 0x0029 */ u32 unk28_13 : 19;
    /* 0x002C */ u8 unk2C[0x2C4 - 0x2C];
    /* 0x02C4 */ u16 cardWins[NIS_DIGIMON_COUNT];
    /* 0x03A0 */ u16 cardLosses[NIS_DIGIMON_COUNT];
    /* per card: bits 0-3 copies owned, 0x80 new */
    /* 0x047C */ u8 digimonCards[NIS_DIGIMON_COUNT];
    /* 0x04EA */ u8 optionCards[NIS_OPTION_COUNT];
    /* 0x0515 */ u8 otherCards[NIS_OTHER_COUNT];
    /* 0x051B */ u8 unk51B[0xF48 - 0x51B];
    /* 0x0F48 */ NisDeck savedDecks[3];
    /* 0x126C */ u8 unk126C[0x145C - 0x126C];
} NisProfile;

/* what started the deck screens, and where they go back to */
typedef struct {
    /* 0x00 */ u8 unk0[0x4F];
    /* 0x4F */ s8 deckScreens; /* 1, 2 and 5: from a menu, 3: before a trade, 4: after it */
} NisDeckCaller;

typedef struct {
    /* 0x000 */ u8 unk0[0x138];
    /* 0x138 */ NisDeckCaller *caller;
    /* 0x13C */ u8 unk13C[0x10];
    /* 0x14C */ u8 unk14C;
    /* 0x14D */ u8 unk14D;
    /* 0x14E */ s8 deckChoice[2]; /* the saved deck each player duels with in VS mode */
    /* 0x150 */ u8 otherPad; /* 1 in VS mode: the second controller works too */
} NisGameState;

typedef struct {
    /* 0x000 */ u8 unk0[0x1BE];
    /* 0x1BE */ s16 unk1BE;
    /* 0x1C0 */ s16 unk1C0; /* the name entry help: 0x1A renaming, 0x39 a new name */
    /* 0x1C2 */ u8 unk1C2[4];
    /* 0x1C6 */ s8 scrollMode; /* of the scrolling background */
    /* 0x1C7 */ u8 unk1C7;
    /* 0x1C8 */ u8 unk1C8;
} NisUiState;

/* a drawing environment packed into a primitive (libgpu's DR_ENV) */
typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[15];
} DR_ENV;

/* a cursor of jp's executable (KAW_createCursor makes one) */
typedef struct {
    /* 0x00 */ u8 unk0[8];
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ s16 w;
    /* 0x0E */ s16 h;
} NisCursor;

/* the windows NISSEG's deck screens keep open, as waitFrames returns them */
typedef struct {
    /* 0x00 */ s32 mainWindow; /* the summary, question or card grid */
    /* 0x04 */ s32 nameWindow; /* the deck's name, or the auto deck's portrait */
    /* 0x08 */ s32 lowerWindow; /* the copies menu or the name field */
    /* 0x0C */ s32 helpWindow; /* the grid's or the viewer's help, or the name entry */
    /* 0x10 */ s32 motionWindow; /* the viewer's motions, or the record */
} NisDeckScreens;

extern NisDeckScreens NIS_DECK_SCREENS;
extern u8 NIS_GRID_CURSOR; /* where the card grid was left */
extern u8 NIS_GRID_SCROLL;
extern NisCursor *NIS_GRID_POINTER; /* the card grid's cursor */

/* the deck screens' shared state */
extern char NIS_TYPED_NAME[]; /* the name typed for a deck */
extern s8 NIS_CARD_IN_VIEW; /* 1 while a card is in view */
extern s8 NIS_AUTO_DECK_QUESTION; /* the auto deck's question */
extern char NIS_KANA[5][18][11]; /* the name entry's pages of kana */
extern s32 NIS_KANA_PAGE; /* the page of the name entry */
extern NisCursor *NIS_NAME_CURSOR; /* the name entry's cursor */


#define NIS_WINDOW(w) ((NisWindow *)(w))

/* the buttons either player pressed this frame (the second one in VS mode) */
#define NIS_PRESSED() (PAD_STATES[0]->rawPressed | PAD_STATES[NIS_STATE->otherPad]->rawPressed)
#define NIS_HELD() (PAD_STATES[0]->rawHeld | PAD_STATES[NIS_STATE->otherPad]->rawHeld)
#define NIS_REPEATED() (PAD_STATES[0]->rawRepeat | PAD_STATES[NIS_STATE->otherPad]->rawRepeat)

/* where a card's picture is in VRAM, and its frame */
typedef struct {
    /* 0x0 */ s16 u;
    /* 0x2 */ s16 v;
    /* 0x4 */ s16 tpage;
    /* 0x6 */ s16 frame;
} NisCardPicture;

/* a card of a list the deck screens show */
typedef struct {
    /* 0x0 */ NisCardPicture picture;
    /* 0x8 */ u8 type; /* 0 Digimon, 1 option, 2 other */
    /* 0x9 */ u8 index; /* within its type */
    /* 0xA */ u8 inDeck; /* copies in the deck */
    /* 0xB */ s8 count; /* copies owned, then copies to take */
    /* 0xC */ u8 brightness;
} NisCardEntry;

typedef struct {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 count;
    /* 0x008 */ s32 scroll; /* the first entry the grid shows */
    /* 0x00C */ s32 cursor;
    /* 0x010 */ u8 unk10[8];
    /* 0x018 */ NisCardEntry entries[NIS_CARD_COUNT];
} NisCardList;

/* the deck being edited */
typedef struct {
    /* 0x00 */ NisDeck *saved; /* the saved deck the screens show */
    /* 0x04 */ s32 kind; /* of the cards the grid lists */
    /* 0x08 */ s8 deck; /* the saved deck slot */
    /* 0x09 */ s8 copies; /* of the card in the details window */
    /* 0x0A */ s8 unkA; /* the auto deck's style: 0 attack, 1 defense */
    /* 0x0B */ s8 cardCount;
    /* 0x0C */ s8 cardType; /* the auto deck keeps its main element here */
    /* 0x0D */ u8 unkD;
    /* 0x0E */ s16 cardIndex; /* the auto deck: 0 many option cards, 1 few */
    /* 0x10 */ s8 unk10; /* the question NIS_drawDeckQuestion asks */
    /* 0x11 */ s8 fromViewer; /* back from the model viewer */
    /* 0x12 */ s8 savedCopies; /* the copies before the viewer */
    /* 0x13 */ s8 maxCopies;
} NisDeckEdit;

#define NIS_PROFILE(p) (&((NisProfile *)PLAYER_PROFILES)[p])

/* deck/nis_auto_deck */
s32 NIS_getCardId(s32 type, s32 index);
s32 NIS_countCards(s32 group, s32 kind, s32 element, s32 level);
NisCardPicture NIS_getCardPicture(s32 type, s32 index);
s32 NIS_countSavedDecks(void);
NisCardList *NIS_buildCardList(s32 deck, u32 kind, s32 arg);
void NIS_buildAutoDeck(void);
void NIS_closeDeckWindow(NisWindow *window);

/* jp's executable */
void loadScrollingBackground(s32, s32);
void showScrollingBackground(void);
void stopScreenFade(void);
void openChoiceMenuFromList(NisMenu *menu, void *items, s32);
s32 runChoiceMenu(NisMenu *menu);
void startChoiceMenuAction(NisMenu *menu);
void openChoiceMenu(NisMenu *menu, s32 title, s32 x, void (*cancel)(), s32 *cursor);
void addChoiceMenuItem(NisMenu *menu, s32, void (*)());
void openKanjiPage(s32, s32);
void closeKanjiPage(s32);
void setBackgroundScrollMode(s32);
char *formatSjisNumber(s32 value, s32 width, char *dst);
void drawScrollArrow(s32 x, s32 y, s32 dir, s32 palette, s32 z);
void uploadKanjiString(char *text, Rect16 *rect);
/* jp keeps KAW_drawCursor in the executable */
void KAW_drawCursor(NisCursor *cursor);
void runWindowTask();
#define NIS_STATE ((NisGameState *)SESSION_DATA)

/* the debug build (eu's NISSEG) prints where it is, names the tasks it
   starts and calls func_80018F5C before each frame it waits for, all of it
   functions of its executable; jp's NISSEG does none of it */
#if JP_DEBUG_BUILD
void setTaskName(s32 taskId, char *name);
void func_80018F5C(void);
#define NIS_DEBUG_PRINT(...) printf(__VA_ARGS__)
#define NIS_DEBUG_NAME_TASK(taskId, name) setTaskName(taskId, name)
#define NIS_DEBUG_FRAME() func_80018F5C()
void func_800184F0(s32, s32, s32);
extern u8 D_801E46E9;
extern s32 D_801DEBF0;
extern char DEBUG_TEXT_LINES[8][0x40]; /* the lines of debug text it shows */
extern const char NIS_STR_ERROR_TASK[]; /* nis_auto_deck.c's "ERROR TASK" */
extern const char NIS_STR_DECK_EDIT[]; /* nis_auto_deck.c's "DECK EDIT" */

/* its debug menus: each item starts a task or opens a submenu (with the
   executable's func_8002D15C) */
typedef struct NisDebugMenu NisDebugMenu;
typedef struct {
    /* 0x00 */ void (*run)();
    /* 0x04 */ NisDebugMenu *submenu;
    /* 0x08 */ s16 stackSize;
    /* 0x0A */ s16 value; /* what the item's action reads, from min to max */
    /* 0x0C */ s16 min;
    /* 0x0E */ s16 max;
    /* 0x10 */ u8 hasValue;
    /* 0x11 */ char name[15];
} NisDebugMenuItem;
struct NisDebugMenu {
    /* 0x00 */ NisDebugMenuItem *items;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 (*open)(void); /* NIS_loadDeckTims */
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 count;
    /* 0x15 */ u8 unk15;
    /* 0x16 */ u8 unk16[2];
    /* 0x18 */ char name[16];
};
void func_8002D15C();
void freeScrollingBackground(void);
void *allocNamedTaskHeapBlock(s32 size, char *name, s32 tag);
void *allocNamedHeapBlock(s32 size, s32 ownerTag, char *name, s32 tag);
void func_80019CE8(); /* a task that shows an error */
void NIS_openViewerScene(void);
void NIS_closeViewerScene(void);
void NIS_runModelViewer(s32 digimonId);
extern NisDebugMenu NIS_SUGANO_MENU;

/* the windows the debug menu's WINDOW TEST opens one at a time
   (NIS_WINDOW_TESTS): what it calls before opening the window and after
   closing it */
typedef struct {
    /* 0x0 */ NisWindowDef *window;
    /* 0x4 */ void (*open)();
    /* 0x8 */ void (*close)();
} NisWindowTest;
void NIS_resumeParentTask();
void NIS_initCardDetailsTest();
void NIS_initDeckNameTest();
/* its allocHeapBlock names the block too */
#define NIS_ALLOC_HEAP_BLOCK(size, ownerTag, name, tag) allocNamedHeapBlock(size, ownerTag, name, tag)
#else
#define NIS_DEBUG_PRINT(...)
#define NIS_DEBUG_NAME_TASK(taskId, name)
#define NIS_DEBUG_FRAME()
#define NIS_ALLOC_HEAP_BLOCK(size, ownerTag, name, tag) allocHeapBlock(size, ownerTag)
#endif
extern NisUiState *SCROLLING_BACKGROUND;
extern u8 D_801E46E8; /* 1 in the trade */

/* the trade's window that the card screen leaves open */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 titleWindow; /* the player's name and offer */
} NisTradeScreens;
extern NisTradeScreens NIS_TRADE_SCREENS;
extern u8 *OPTION_CARDS; /* as dcb/card_db.h */
extern u8 *DIGIVOLVE_CARDS;
#define NIS_DIGIMON_CARDS ((NisCardData *)DIGIMON_CARDS)
#define NIS_OPTION_CARDS ((NisOptionData *)OPTION_CARDS)
#define NIS_DIGIVOLVE_CARDS ((NisOtherData *)DIGIVOLVE_CARDS)
extern NisDeckEdit NIS_DECK_EDIT;
extern NisCardList *NIS_CARD_LIST; /* the last list built */

/* the big card picture of the deck screens, which turns over */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 clutX;
    /* 0x6 */ s16 clutY;
    /* 0x8 */ s16 cardId; /* -1: none loaded */
} NisCardImageSlot;

typedef struct {
    /* 0x00 */ NisCardImageSlot slots[4];
    /* 0x28 */ NisCardImageSlot *current;
    /* 0x2C */ POLY_FT4 *polys[2]; /* front, back and turned for each frame buffer */
    /* 0x34 */ s16 turn;
    /* 0x36 */ s8 back;
    /* 0x37 */ s8 loaded;
    /* 0x38 */ s8 delay;
} NisCardImage;

extern NisCardImage NIS_CARD_IMAGE;
extern NisSprite *D_801E469C;

/* trade/nis_trade: the card trade between the two players of VS mode */

/* a card the player can trade */
typedef struct {
    /* 0x0 */ NisCardPicture picture;
    /* 0x8 */ u8 brightness;
    /* 0x9 */ u8 type; /* 0 Digimon, 1 option, 2 other */
    /* 0xA */ u8 index; /* within its type */
    /* 0xB */ s8 chosen; /* copies to trade */
    /* 0xC */ s8 max; /* copies that can be traded */
    /* 0xD */ s8 owned;
} TradeCard;

typedef struct {
    /* 0x000 */ s32 scroll; /* the first card the grid shows */
    /* 0x004 */ s32 cursor;
    /* 0x008 */ s32 count;
    /* 0x00C */ TradeCard cards[NIS_CARD_COUNT];
} TradeCardList;

/* what a player gives: up to three kinds of cards */
typedef struct {
    /* 0x00 */ s8 kinds;
    /* 0x01 */ s8 copies;
    /* 0x02 */ TradeCard cards[4];
} TradeOffer;

/* bit n: the player's copy n of a card has a serial number the other
   player's copies don't have, so it can be traded */
typedef struct {
    /* 0x00 */ u8 digimon[NIS_DIGIMON_COUNT];
    /* 0x6E */ u8 options[NIS_OPTION_COUNT];
    /* 0x99 */ u8 others[NIS_OTHER_COUNT];
} TradeMasks;

/* the trade's windows, as waitFrames returns them */
typedef struct {
    /* 0x00 */ s32 offers; /* both offers, under the trade menu */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 grid;
    /* 0x0C */ s32 help;
    /* 0x10 */ s32 card; /* the trade's own card screen */
    /* 0x14 */ s32 question;
} TradeWindows;

/* the card in view of the trade's own card screen */
typedef struct {
    /* 0x0 */ s8 copies;
    /* 0x1 */ s8 max;
    /* 0x2 */ s8 owned;
    /* 0x3 */ s8 shown; /* the copies written into VRAM */
    /* 0x4 */ s8 type;
    /* 0x6 */ s16 index;
} TradeView;

#endif /* DCB_NISSEG_H */
