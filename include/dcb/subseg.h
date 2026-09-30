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
    s16 counts[8];
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

#endif /* DCB_SUBSEG_H */
