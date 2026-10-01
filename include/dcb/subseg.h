#ifndef DCB_SUBSEG_H
#define DCB_SUBSEG_H

#include "game.h"

typedef struct {
    s16 request;
    s16 ids[8];
    s8 ages[8];
    s8 busy;
    s8 running;
} CardImageCache;

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

typedef struct {
    u8 inDecks[3][301];
    u8 unk387[301];
    u8 spare[301];
} DeckCardCounts;

typedef struct {
    UiWindow window;
    s32 slot;
} TabWindow;

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
    u16 counts[8];
    u8 unk40[0x12D];
    u8 unk16D;
    u8 unk16E[2];
    s8 *selectedCard;
} CollectionStats;

typedef struct {
    PlayerDeck *decks[3];
    s8 count;
    s8 current;
    s16 slot;
} DeckMenuState;

typedef struct {
    s16 ids[250];
    s16 count;
    s16 pad;
} CardIdList;

extern s16 SUB_EDITOR_PLAYER;
extern PlayerDeck *SUB_EDITED_DECK;
extern CardImageCache SUB_CARD_IMAGE_CACHE;
extern UiWindow SUB_WINDOWS[7];
extern void *SUB_CARD_LIST[301];
extern s8 *SUB_CARDS_BY_ID[301];
extern EditorState SUB_EDITOR;
extern DeckEditState SUB_DECK_EDIT;
extern CollectionStats SUB_COLLECTION_STATS;
extern DeckMenuState SUB_DECK_MENU;
extern u8 *SUB_CARD_ARCHIVE;
extern u8 SUB_AUTO_DECK_ENABLED;

#if VERSION_JP
/* jp's SUBSEG is a card shop (the USA SUBSEG above is the deck editor) */

/* jp: a window the executable's window task (runWindowTask) slides in from
   (fromX, fromY) to (x, y) and draws with draw every frame */
typedef struct {
    /* 0x00 */ s16 fromX;
    /* 0x02 */ s16 fromY;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ s16 w;
    /* 0x0E */ s16 h;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void (*draw)();
    /* 0x1C */ void (*tick)();
} JpWindowDesc;

/* jp: the state the window task hands back; state 4 closes the window */
typedef struct {
    /* 0x00 */ s8 unk0;
    /* 0x01 */ s8 state;
    /* 0x02 */ u8 unk2[0x36];
    /* 0x38 */ CVECTOR colors[3]; /* what drawWindowFrame draws its frame with */
    /* 0x44 */ u8 unk44[0xA];
    /* 0x4E */ s16 z;
} JpWindow;

/* jp: the executable's blinking hand cursor (KAW_createCursor) */
typedef struct {
    /* 0x0 */ u8 unk0[8];
    /* 0x8 */ s16 x;
    /* 0xA */ s16 y;
} JpCursor;

/* libgpu's DR_ENV */
typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[15];
} DR_ENV;

/* jp: where a shop item's small picture sits in VRAM; a negative palette
   (~palette) marks the cards drawn with the rare frame */
typedef struct {
    /* 0x0 */ s16 u;
    /* 0x2 */ s16 v;
    /* 0x4 */ s16 tpage;
    /* 0x6 */ s16 palette;
} ItemIcon;

/* jp: one line of a shop list: a card or a booster pack */
typedef struct {
    /* 0x0 */ ItemIcon icon;
    /* 0x8 */ u8 type;
    /* 0x9 */ u8 index;
    /* 0xA */ s8 count; /* the copies chosen */
    /* 0xB */ s8 max; /* the copies there are to choose */
    /* 0xC */ u8 brightness;
    /* 0xD */ u8 owned; /* the copies the player has */
} ShopItem;

/* jp: how many of an item the shop has */
typedef struct {
    /* 0x0 */ u8 unk0[2];
    /* 0x2 */ u8 stock; /* selling: the copies the player has */
    /* 0x3 */ s8 count; /* buying: the copies on sale */
} ShopStock;

/* jp: the items a shop screen lists, the cursor and the first row shown */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ s32 top; /* the first item shown (10 at a time) */
    /* 0x0C */ s32 cursor;
    /* 0x10 */ u8 unk10[8];
    /* 0x18 */ ShopItem items[165];
} ShopList;

/* jp: a VRAM place for a card's large picture (B:\\L_CARD\\LC%03d.TIM) */
typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
    /* 0x4 */ s16 clutX;
    /* 0x6 */ s16 clutY;
    /* 0x8 */ s16 id;
} CardImageSlot;

/* jp: the card shop (B:\\OBJECT\\shop.TIM), allocated by SUB_openBuyShop and
   SUB_openSellShop */
typedef struct {
    /* 0x000 */ ShopStock packs[6];
    /* 0x018 */ ShopStock digimonCards[110];
    /* 0x1D0 */ ShopStock optionCards[43];
    /* 0x27C */ ShopStock digivolveCards[6];
    /* 0x294 */ ShopList list;
    /* 0xBB4 */ CardImageSlot imageSlots[4];
    /* 0xBDC */ CardImageSlot *imageSlot;
    /* 0xBE0 */ POLY_FT4 *polys[2];
    /* 0xBE8 */ s16 flip;
    /* 0xBEA */ s8 flipBack;
    /* 0xBEB */ s8 loading; /* a card picture is being loaded */
    /* 0xBEC */ s8 flipTimer;
    /* 0xBED */ s8 itemChosen; /* the item list has picked its first item */
    /* 0xBEE */ u8 unkBEE[2];
    /* 0xBF0 */ s32 total; /* what the chosen cards cost (or fetch) */
    /* 0xBF4 */ s32 savedTotal;
    /* 0xBF8 */ s16 mode; /* 11 buying, 12 the packs bought, 13 selling */
    /* 0xBFA */ s8 shopId;
    /* 0xBFB */ s8 paid;
    /* 0xBFC */ s8 fromOpened; /* the card detail came from the packs bought */
} ShopState;

/* jp: the item a shop screen works on (past SUBSEG's end, where NISSEG has its bss) */
typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ s8 deck;
    /* 0x5 */ s8 count;
    /* 0x6 */ s8 max;
    /* 0x7 */ u8 owned;
    /* 0x8 */ s8 deckCount;
    /* 0x9 */ s8 type; /* 0 Digimon card, 1 option, 2 Digivolve option, 3 booster pack */
    /* 0xA */ s16 index;
} ShopCursor;

/* jp: a booster pack's cards: count card numbers from base + start */
typedef struct {
    /* 0x0 */ s32 base;
    /* 0x4 */ s32 start;
    /* 0x8 */ s32 count;
    /* 0xC */ s32 unkC;
} PackRange;

extern PackRange SUB_PACK_RANGES[7];
extern JpWindowDesc SUB_STOCK_GRID_WINDOW;
extern JpWindowDesc SUB_DETAILS_WINDOW;
extern JpWindowDesc SUB_TOTAL_WINDOW;
extern JpWindowDesc SUB_BITS_WINDOW;
extern JpWindowDesc SUB_STOCK_HELP_WINDOW;
extern JpWindowDesc SUB_OPENED_HELP_WINDOW;
extern JpWindowDesc SUB_OPENED_GRID_WINDOW;
extern JpWindowDesc SUB_OPENED_TITLE_WINDOW;
extern JpWindowDesc SUB_PICTURE_WINDOW;
extern JpWindowDesc SUB_VIEWER_HELP_WINDOW;
extern JpWindowDesc SUB_VIEWER_MOTION_WINDOW;
extern s8 SUB_OPTION_GROUPS[46];
extern char *SUB_SPECIALTY_NAMES[6];
extern char *SUB_DIGIVOLVE_EFFECT_NAMES[5];
extern char *SUB_OPTION_KINDS[2];
extern s16 SUB_RANDOM_SEED;
extern JpWindow *SUB_VIEWER_WINDOWS[4];
extern JpWindow *SUB_SHOP_WINDOWS[8];
extern u8 SUB_SAVED_CURSOR;
extern u8 SUB_SAVED_TOP;
extern JpCursor *SUB_GRID_CURSOR;
extern DR_ENV SUB_PREVIEW_DR_ENVS[2];
extern DR_ENV SUB_SCREEN_DR_ENVS[2];
extern DRAWENV SUB_PREVIEW_DRAW_ENVS[2];
extern ShopState *SUB_SHOP;
extern s16 SUB_STOCK_IDS[18];
extern s8 SUB_SHOP_RUNNING;
extern s16 SUB_STOCK_ID_COUNT;
extern u8 *SUB_PACK_CARD_END;
extern ShopCursor SUB_SHOP_ITEM;
extern ShopList *SUB_SHOP_LIST;

#endif

#endif /* DCB_SUBSEG_H */
