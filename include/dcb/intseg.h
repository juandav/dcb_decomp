#ifndef DCB_INTSEG_H
#define DCB_INTSEG_H

#include "game.h"

/* INTSEG: jp's new-game intro, where Babamon asks for the player's name and
   birthday and hands over one of three starter decks */

/* eu's INTSEG is a leftover of an earlier Japanese build, with debug code
   that names the tasks it spawns and prints to a debug text buffer: eu
   builds this C as jp's with JP_DEBUG_BUILD (include/version.h) */

/* a window of jp's executable */
typedef struct {
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ s16 x;
    /* 0x16 */ s16 y;
    /* 0x18 */ u8 unk18[0x36];
    /* 0x4E */ s16 z;
} IntWindow;

/* a line of text drawn into VRAM */
typedef struct {
    u8 text[0x2C];
} IntText;

/* the state of the intro: its pages and the name entry */
typedef struct {
    /* 0x000 */ IntText lines[4]; /* the page being shown */
    /* 0x0B0 */ s8 lineLengths[4]; /* in characters */
    /* 0x0B4 */ u8 lineCount;
    /* 0x0B5 */ u8 unkB5;
    /* 0x0B6 */ s16 typedChars; /* shown of the line being typed */
    /* 0x0B8 */ s16 typedLines; /* the lines shown so far, with that one */
    /* 0x0BA */ s16 unkBA;
    /* 0x0BC */ s16 keyX; /* the keyboard cursor; keyX is also the YES/NO choice */
    /* 0x0BE */ s16 keyY;
    /* 0x0C0 */ s16 unkC0;
    /* 0x0C2 */ u8 lengths[6]; /* bytes typed in each field */
    /* 0x0C8 */ u8 page; /* the next one of INT_PAGES */
    /* 0x0C9 */ u8 unkC9[3];
    /* 0x0CC */ s32 cursor; /* the cursor sprite */
    /* 0x0D0 */ u8 keyPage; /* 0 hiragana, 1 katakana, 2 letters and digits */
    /* 0x0D1 */ u8 keysPalette;
    /* 0x0D2 */ u8 buttonsPalette;
    /* 0x0D3 */ u8 maxLength;
    /* 0x0D4 */ s8 stone; /* the starter deck chosen: 0 red, 1 blue, 2 green */
    /* 0x0D5 */ u8 blinkTimer;
    /* 0x0D6 */ u8 field; /* the field being typed */
    /* 0x0D7 */ u8 fields[6][14]; /* surname, name, nickname, year, month, day */
    /* 0x12B */ u8 values[6][7]; /* what each character typed stands for (digits for the date) */
    /* 0x155 */ u8 unk155[3];
} IntState;

/* a page of the intro: its lines and what each does */
typedef struct {
    /* 0x00 */ s8 lineCount;
    /* 0x01 */ u8 lineActions[3]; /* Babamon's animation after each line */
    /* 0x04 */ u8 action; /* what happens once the page is shown */
    /* 0x05 */ u8 pad5[3];
    /* 0x08 */ u8 *lines[4];
} IntPage;

/* how a window of jp's executable opens, and what draws in it */
typedef struct {
    /* 0x00 */ Rect16 from;
    /* 0x08 */ Rect16 to;
    /* 0x10 */ s16 frames;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void (*draw)(IntWindow *win);
    /* 0x1C */ void (*close)(void);
} IntWindowDesc;

/* a blinking box of jp's executable */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 rgb[3];
    /* 0x07 */ u8 code;
    /* 0x08 */ u16 unk8;
    /* 0x0A */ u16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ s16 w;
    /* 0x16 */ s16 h;
} IntBlink;

/* a card of a saved deck in jp's player profile */
typedef struct {
    /* 0x0 */ u8 type; /* 0: Digimon, 1: option, 2: the others */
    /* 0x1 */ u8 id;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ u8 *data;
} IntDeckCard;

/* a saved deck in jp's player profile */
typedef struct {
    /* 0x000 */ u8 valid;
    /* 0x001 */ char name[15];
    /* 0x010 */ IntDeckCard cards[30];
    /* 0x100 */ u8 unk100[0xC];
} IntDeck;

/* jp's player profile, as far as INTSEG uses it */
typedef struct {
    /* 0x000 */ char name[14];
    /* 0x00E */ u8 unkE[0x46E];
    /* 0x47C */ u8 digimonCards[0x6E];
    /* 0x4EA */ u8 optionCards[0x2B];
    /* 0x515 */ u8 otherCards[6];
    /* 0x51B */ s8 deckSize;
    /* 0x51C */ u8 deck[15];
    /* 0x52B */ u8 unk52B[0xA1D];
    /* 0xF48 */ IntDeck decks[4];
    /* 0x1378 */ u8 unk1378[0xE4];
} IntProfile;

/* the windows of the intro, and the state of its menus (int_bss.c) */
extern u8 *INT_WINDOWS[8];
extern IntState INT_STATE;

#endif /* DCB_INTSEG_H */
