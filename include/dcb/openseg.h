#ifndef DCB_OPENSEG_H
#define DCB_OPENSEG_H

#include "game.h"

typedef struct {
    s32 step;
    s32 page;
    s32 shownPage;
    s32 length;
    s32 done;
    s32 waitInput;
    s32 unk18;
    s32 blink;
} TextScroll;

typedef struct {
    UiWindow window;
    s32 player;
} PlayerWindow;

typedef struct {
    /* 0x00 */ s16 col;
    /* 0x02 */ s16 prevCol;
    /* 0x04 */ s16 row;
    /* 0x06 */ s16 prevRow;
    /* 0x08 */ u8 inputEnabled;
    /* 0x09 */ char name[13];
    /* 0x16 */ u8 cursor;
    /* 0x17 */ u8 onSideMenu;
    /* 0x18 */ s8 sideRow;
    /* 0x19 */ s8 prevSideRow;
    /* 0x1A */ s8 state;
    /* 0x1B */ u8 replaceName;
} NameEntry;

typedef struct {
    u8 deck;
    s8 chosen;
} StarterSelect;

typedef struct {
     u8 *vlcbuf[2];
     s32 vlcid;
     u8 *imgbuf[2];
     s32 imgid;
     Rect16 rect[2];
     s32 rectid;
     Rect16 slice;
     s32 isdone;
} DecEnv;

/* libcd's CdlLOC */
typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdLocation;

/* DigimonCardData with a signed type */
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ s8 type;
    /* 0x03 */ char name[0x15];
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 attr;
} CardEntry;

typedef struct {
    s32 x;
    s32 y;
    Rect16 uv;
} MenuSprite;

typedef struct {
    UiWindow window;
    u8 slot;
    s8 message;
    u8 unk46[2];
} SlotWindow;

typedef struct {
    /* 0x00 */ char name[0xD];
    /* 0x0D */ u8 saveCount;
    /* 0x0E */ u8 location;
    /* 0x0F */ u8 arena;
    /* 0x10 */ u16 profileId;
    /* 0x12 */ u16 seenCardCount;
    /* 0x14 */ u16 progress;
    /* 0x16 */ u16 size;
    /* 0x18 */ u8 unk18[0xC];
    /* 0x24 */ s32 playTime;
    /* 0x28 */ u32 unk28_0 : 10;
    /* 0x29 */ u32 tradeUnlocked : 1;
    /* 0x29 */ u32 unk28_11 : 21;
    /* 0x2C */ u8 unk2C[0x2A];
    /* 0x56 */ u16 activePartner;
    /* 0x58 */ u8 unk58[0x28];
} SaveSlot;

typedef struct {
    /* 0x000 */ POLY_FT4 arrows[2];
    /* 0x050 */ POLY_FT4 arrowShadows[2];
    /* 0x0A0 */ MenuSprite sprites[6];
    /* 0x100 */ SlotWindow slotWindows[3];
    /* 0x1D8 */ SlotWindow infoWindow;
    /* 0x220 */ u8 unk220[8];
    /* 0x228 */ SaveSlot slots[2][3];
    /* 0x528 */ void *buffer;
    /* 0x52C */ s8 empty[2][3];
    /* 0x532 */ s8 animTime;
    /* 0x533 */ u8 animPhase;
    /* 0x534 */ s8 message;
    /* 0x535 */ u8 state;
    /* 0x536 */ u8 freeBlocks;
    /* 0x537 */ u8 mode;
    /* 0x538 */ u8 unk538;
    /* 0x539 */ u8 cancelled;
    /* 0x53A */ s8 ready;
    /* 0x53B */ s8 slot;
    /* 0x53C */ u8 card;
    /* 0x53D */ u8 messagePort;
    /* 0x53E */ u8 loading;
    /* 0x53F */ u8 progress;
    /* 0x540 */ u8 previewsRead;
    /* 0x541 */ u8 again;
    /* 0x542 */ u8 exitAction; /* 1: ask "Quit the game?" once the screen closes */
} MemcardScreen;

typedef struct {
    u8 slot;
    u8 file;
    u8 unk2[2];
    s32 playTime;
} SaveInfo;

typedef struct {
    /* 0x0000 */ u8 unk0[0x1010];
    /* 0x1010 */ SaveInfo saves[2];
    /* 0x1020 */ u8 unk1020[7];
    /* 0x1027 */ u8 playWithoutSaving;
    /* 0x1028 */ s8 menuRow;
} SessionView;

typedef struct {
    UiWindow window;
    u8 port;
    u8 message;
    u8 unk46[2];
} MessageWindow;


extern TextScroll OPEN_INTRO_TEXT;
/* where the registration and friend screens slide their decorations
   (OPEN_TITLE_PARTS) in: the corner art (parts 0-2), the side bar (20-21),
   the screen frame (10-19) and the panel (the step bar, 3-5, or the
   friend screen's list) */
extern s32 OPEN_CORNER_X;
extern s32 OPEN_CORNER_Y;
extern s32 OPEN_SIDEBAR_X;
extern s32 OPEN_SIDEBAR_Y;
extern s32 OPEN_FRAME_X;
extern s32 OPEN_FRAME_Y;
extern s32 OPEN_TITLE_PART_COUNT;
extern s32 OPEN_PANEL_X;
extern s32 OPEN_PANEL_Y;
extern MemcardScreen OPEN_MEMCARD;
extern u8 OPEN_MEMCARD_PROGRESS;
extern u8 OPEN_MEMCARD_MODE;
extern s8 OPEN_MEMCARD_MESSAGE;
extern Window OPEN_DIALOG;
extern u8 OPEN_MEMCARD_STATE;
extern s8 OPEN_MEMCARD_READY;
extern u8 OPEN_MEMCARD_FREE_BLOCKS;

void SsSetMono(void);

#endif /* DCB_OPENSEG_H */
