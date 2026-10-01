#ifndef DCB_SAISEG_H
#define DCB_SAISEG_H

#include "game.h"
#include "dcb/script.h"
#include "dcb/menu.h"

#define SESSION ((SaisegSession *)((SessionData *)SESSION_DATA)->areaSession)

/* what the area does each frame (AreaState.mode) */
enum AreaMode {
    AREA_MODE_SCRIPT,          /* run the script */
    AREA_MODE_CHOICES,         /* the choice menu */
    AREA_MODE_SELECT_OPPONENT, /* the opponent select input */
    AREA_MODE_TEXT_FULL,       /* the text lines are full: Cross clears them and adds the rest */
    AREA_MODE_WAIT_CROSS,      /* wait for Cross */
    AREA_MODE_BUSY = 10        /* a task started by the script is running */
};

/* where runArea goes when the script ends (AreaState.exitAction) */
enum AreaExit {
    AREA_EXIT_MAP,
    AREA_EXIT_DUEL,
    AREA_EXIT_DECK_EDITOR,
    AREA_EXIT_SAVE,
    AREA_EXIT_FUSION,
    AREA_EXIT_EQUIPMENT,
    AREA_EXIT_TITLE_OR_ENDING
};

typedef struct {
    POLY_FT4 quads[2];
    VECTOR pos;
    SVECTOR rot;
    SVECTOR corners[4];
    s32 otz;
    s16 w;
    s16 h;
} Sprite3D;

typedef struct {
    u8 text[0x3C];
    s16 shown;
    s8 active;
    s8 length;
} TextLine;

typedef struct {
    /* 0x00 */ u8 pad[0x8];
    /* 0x08 */ s32 completionRate; /* "Game Completion", in tenths of a percent */
    /* 0x0C */ s32 cardRate;
    /* 0x10 */ s32 abilityRate;
    /* 0x14 */ PlayerProfile *profile;
    /* 0x18 */ char *tamerRank;
    /* 0x1C */ char *collectorRank;
    /* 0x20 */ s8 state; /* 0: closed, 1: opening, 2: open */
} PlayerStats;

typedef struct {
    /* 0x000 */ s32 unk0[0x40];
    /* 0x100 */ u8 target[2]; /* brightness the two highlighted portraits fade to */
    /* 0x102 */ u8 current[2];
    /* 0x104 */ u8 step[2];
    /* 0x106 */ u8 pad106[7];
    /* 0x10D */ s8 ids[0x1B]; /* six to a page; 16 and 17 are the page arrows */
    /* 0x128 */ s8 count;
    /* 0x129 */ u8 pad129[3];
} OpponentList;

typedef struct {
    s32 unk0;
    Script *script;
    s32 *regs;
} ScriptRunner;

typedef struct {
    Rect16 rect;
    s32 brightness;
    s32 style;
    s32 flags;
    s32 label;
    u8 labelPalette;
} WindowDef;

typedef struct {
    /* 0x0 */ s32 delay;
    /* 0x4 */ s32 progress;
} SlideEntry;

typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    s16 x1;
    s16 y1;
    s16 x2;
    s16 y2;
    s16 x3;
    s16 y3;
} PolyF4;

typedef struct {
    u32 tag;
    u32 code[1];
} DrTPage;

typedef struct {
    DrTPage tpages[2];
    PolyF4 polys[2];
} Fade;

typedef struct {
    /* 0x00 */ Fade fades[2];
    /* 0x80 */ SlideEntry entries[7];
    /* 0xB8 */ s32 wipeX;
    /* 0xBC */ s32 timer;
    /* 0xC0 */ u16 delay;
    /* 0xC2 */ u8 unkC2;
    /* 0xC3 */ u8 state;
    /* 0xC4 */ u8 showWipe;
    /* 0xC5 */ u8 unkC5;
    /* 0xC6 */ u8 statsShown;
    /* 0xC7 */ u8 alpha;
    /* 0xC8 */ u8 phase;
    /* 0xC9 */ u8 brightness;
    /* 0xCA */ u8 pulsePhase;
} OpponentInfo;

typedef struct {
    /* 0x00 */ s32 progress[6];
    /* 0x18 */ s32 blink[6];
    /* 0x30 */ s32 zoom;
    /* 0x34 */ s32 timer;
    /* 0x38 */ s16 delay[6];
    /* 0x44 */ u8 emptyBlink[6];
    /* 0x4A */ u8 pad4A[0x12];
    /* 0x5C */ s8 page;
    /* 0x5D */ s8 selected;
    /* 0x5E */ s8 unk5E;
    /* 0x5F */ u8 pad5F;
} OpponentSelect;

typedef struct {
    /* 0x000 */ DrTPage coverTpages[2];
    /* 0x010 */ PolyF4 coverPolys[2];
    /* 0x040 */ VECTOR pos;
    /* 0x050 */ SVECTOR rot;
    /* 0x058 */ SVECTOR corners[4];
    /* 0x078 */ s32 choiceSlide;
    /* 0x07C */ s8 choiceStates[5]; /* 2: the choice can't be picked */
    /* 0x081 */ s8 choicePhase;
    /* 0x082 */ s8 choiceMode;
    /* 0x083 */ s8 choiceCursor;
    /* 0x084 */ s8 choiceCount;
    /* 0x085 */ u8 pad85[3];
    /* 0x088 */ OpponentSelect select;
    /* 0x0E8 */ s32 openTimer;
    /* 0x0EC */ s32 angle;
    /* 0x0F0 */ u32 exitArg; /* the deck, fusion mode or ending the exit action takes */
    /* 0x0F4 */ s32 labelSlide;
    /* 0x0F8 */ u8 padF8[4];
    /* 0x0FC */ char keyword[0xD];
    /* 0x109 */ s8 imageHidden;
    /* 0x10A */ s8 openPanel; /* 1: talk, 2: opponent select, location + 1: opponent info */
    /* 0x10B */ s8 closePanel;
    /* 0x10C */ s8 opponentPicked;
    /* 0x10D */ s8 exitAction;
    /* 0x10E */ s8 mode;
    /* 0x10F */ s8 partnerCount;
    /* 0x110 */ s8 partners[4];
    /* 0x114 */ s8 selectState;
    /* 0x115 */ u8 unk115;
    /* 0x116 */ s8 unk116;
    /* 0x117 */ u8 coverAlpha;
    /* 0x118 */ s8 opening;
    /* 0x119 */ u8 pad119;
    /* 0x11A */ s8 location;
    /* 0x11B */ s8 shownLocation;
    /* 0x11C */ s8 rewardBusy;
    /* 0x11D */ u8 prizePack;
    /* 0x11E */ u8 unk11E;
    /* 0x11F */ u8 typing;
    /* 0x120 */ u8 waitingForCross;
    /* 0x121 */ u8 nextBlink;
} AreaState;

typedef struct {
    /* 0x000 */ POLY_FT4 quads[2][4];
    /* 0x140 */ s32 holdTimer;
    /* 0x144 */ s32 scroll;
    /* 0x148 */ s32 speed;
    /* 0x14C */ s32 step;
    /* 0x150 */ s16 x;
    /* 0x152 */ s16 y;
    /* 0x154 */ u8 brightness[4];
    /* 0x158 */ u8 pad158[2];
    /* 0x15A */ u8 state;
    /* 0x15B */ u8 loading;
} Splash;

typedef struct {
    u8 pad[0x4];
    u8 r;
    u8 g;
    u8 b;
    u8 pad7[0x21];
} Unk801EBD94;

typedef struct {
    s16 x;
    s16 y;
    s8 next[4];
    s8 unlocked;
    u8 pad9;
} MapNode;

typedef struct {
    VECTOR pos;
    SVECTOR rot;
    s32 length;
} MapPath;

typedef struct {
    /* 0x000 */ s8 *route;
    /* 0x004 */ MapNode *node;
    /* 0x008 */ MapNode *target;
    /* 0x00C */ MapPath paths[20];
    /* 0x23C */ DrTPage pathTpages[2][20];
    /* 0x37C */ s8 pathCount;
    /* 0x37D */ u8 pad37D[3];
    /* 0x380 */ s8 labelRegion;
    /* 0x381 */ s8 shownLabelRegion;
    /* 0x382 */ s16 labelSlide;
    /* 0x384 */ DrTPage tpages[2];
    /* 0x394 */ u8 pad394[8];
    /* 0x39C */ PolyF4 fades[2];
    /* 0x3CC */ PolyF4 *pathPolys[2];
    /* 0x3D4 */ s32 iconSlide;
    /* 0x3D8 */ s32 zoom;
    /* 0x3DC */ s32 portraitBlink;
    /* 0x3E0 */ s32 zoomAngle;
    /* 0x3E4 */ s32 moving;
    /* 0x3E8 */ s32 angle;
    /* 0x3EC */ s32 distance;
    /* 0x3F0 */ s16 menuSlide;
    /* 0x3F2 */ s16 portraitSlide;
    /* 0x3F4 */ s16 nameSlide;
    /* 0x3F6 */ s16 iconX;
    /* 0x3F8 */ s16 iconY;
    /* 0x3FA */ u8 alpha;
    /* 0x3FB */ u8 markerCount;
    /* 0x3FC */ u8 oldMarkerCount;
    /* 0x3FD */ s8 nameSlideDir;
    /* 0x3FE */ s8 portraitState;
    /* 0x3FF */ s8 menuCursor; /* 0: deck, 1: partner equipment, 2: save */
    /* 0x400 */ s8 menuPhase;
    /* 0x401 */ s8 menuChosen;
    /* 0x402 */ s8 iconMotion;
    /* 0x403 */ s8 state;
    /* 0x404 */ s8 lastRegion;
    /* 0x405 */ s8 region;
    /* 0x406 */ s8 nodeIndex;
    /* 0x407 */ s8 active;
    /* 0x408 */ s8 iconRunning;
    /* 0x409 */ s8 animating;
    /* 0x40A */ s8 animRegion;
    /* 0x40B */ s8 openMenu;
} WorldMap;

typedef struct {
    s32 timer;
    s16 x;
    s16 y;
    u8 frames;
    u8 frameTime;
} MapAnim;

typedef struct {
    UiWindow window;
    s32 slot;
} RewardWindow;

typedef struct {
    s32 offset;
    s8 state;
} MenuTab;

typedef struct {
    UiWindow window;
    s16 cardId;
    u16 index;
    u16 clut;
    u16 pad4A;
} CardWindow;

typedef struct {
    UiWindow main;
    CardWindow cards[3];
    RewardWindow rewards[3];
    s32 showRewards;
} RewardScreen;

typedef struct {
    /* 0x000 */ OpponentList opponents;
    /* 0x12C */ OpponentSelect select;
    /* 0x18C */ Chunk *pak;
    /* 0x190 */ void *script;
    /* 0x194 */ s32 loading;
    /* 0x198 */ s32 scriptOffset;
    /* 0x19C */ s32 music;
    /* 0x1A0 */ u8 pad1A0[4];
    /* 0x1A4 */ s8 area;
    /* 0x1A5 */ u8 pad1A5;
    /* 0x1A6 */ u8 duelResult; /* 0: the player won */
    /* 0x1A7 */ s8 selectImage; /* script opcode 7: which VRAM image the opponent select picks (then unused) */
    /* 0x1A8 */ u8 location;
    /* 0x1A9 */ s8 resumeMode; /* 1: back from a duel, 2: back from the complete stats */
    /* 0x1AA */ s8 shownOpponent;
} SaisegSession;

typedef struct {
    /* 0x00 */ s16 col;
    /* 0x02 */ s16 prevCol;
    /* 0x04 */ s16 row;
    /* 0x06 */ s16 prevRow;
    /* 0x08 */ u8 active;
    /* 0x09 */ u8 pad9;
    /* 0x0A */ char text[0xD];
    /* 0x17 */ u8 rows;
    /* 0x18 */ u8 cursor;
    /* 0x19 */ u8 mode; /* 0: the character grid, 1: the buttons on its right */
    /* 0x1A */ s8 sel;
    /* 0x1B */ s8 prevSel;
    /* 0x1C */ s8 result;
} WordInput;

typedef struct {
    u8 cursor;
    s8 done;
} PartnerCursor;

typedef struct {
    s32 state;
    u8 count;
} PartnerList;

extern PlayerStats SAI_PLAYER_STATS;
extern s8 SAI_AREA_MODE;
extern OpponentList SAI_OPPONENTS;
extern u8 SAI_CHOICE_STATES[];
extern s16 SAI_SCRIPT_REWARD_CARDS[3];
extern TextLine SAI_TEXT_LINES[3];
extern AreaState SAI_AREA;
extern ScriptRunner *SAI_SCRIPT[1];
extern s8 SAI_OPPONENT_IDS[24];
extern Sprite3D *SAI_SPRITES[];
extern u8 SAI_PANEL_COVER_ALPHA;
extern UiWindow SAI_MESSAGE_WINDOW;
extern s8 SAI_PANEL_IMAGE_HIDDEN;
extern WorldMap SAI_WORLD_MAP;
extern s8 SAI_LOCATION;
extern u8 SAI_OPEN_PANEL;
extern s8 SAI_PLAYER_STATS_STATE;
extern s8 SAI_CLOSE_PANEL;
extern s8 SAI_ICON_RUNNING;
extern s8 SAI_EXIT_ACTION;
extern s8 SAI_PARTNER_CHOICES[4];

#if VERSION_JP
/* jp's SAISEG is its own design: its types, with what its C reads so far */

/* what jp's window task (runWindowTask) hands back */
typedef struct {
    /* 0x00 */ u8 unk0;
    /* 0x01 */ s8 state; /* 4 closes it */
    /* 0x02 */ s8 unk2;
    /* 0x03 */ u8 unk3[0x4B];
    /* 0x4E */ s16 z;
} JpWindow;

/* what jp's window task opens */
typedef struct {
    /* 0x00 */ s16 unk0[4];
    /* 0x08 */ Rect16 rect;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ void (*draw)(JpWindow *);
    /* 0x1C */ void (*unk1C)(JpWindow *);
} JpWindowDef;

/* SAISEG's state (SessionData.areaSession) */
typedef struct SaiState {
    /* 0x00 */ VramSprite *sprites[2]; /* the map window's, one block for each frame buffer */
    /* 0x08 */ ScriptRunner *runner;
    /* 0x0C */ s32 *regs; /* the script's registers: [6] who speaks, [16] what */
    /* 0x10 */ Chunk *pak; /* the map's PAK */
    /* 0x14 */ s32 bits; /* the Bits the player gets */
    /* 0x18 */ s32 unk18; /* how the area ends */
    /* 0x1C */ s32 map; /* the map the player is on */
    /* 0x20 */ s32 menuItems[6];
    /* 0x38 */ s32 scriptOffset; /* where the script goes on after a duel */
    /* 0x3C */ s16 unk3C;
    /* 0x3E */ s16 musicTrack;
    /* 0x40 */ s16 musicVolume;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ s8 unk43;
    /* 0x44 */ s8 unk44;
    /* 0x45 */ s8 unk45;
    /* 0x46 */ s8 unk46;
    /* 0x47 */ u8 unk47;
    /* 0x48 */ s8 unk48;
    /* 0x49 */ s8 unk49;
    /* 0x4A */ u8 unk4A;
    /* 0x4B */ s8 unk4B;
    /* 0x4C */ u8 flags;
    /* 0x4D */ u8 menuCount;
    /* 0x4E */ u8 unk4E;
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ s8 unk50;
} SaiState;

/* a state of jp's executable that SAISEG reads */
typedef struct {
    /* 0x000 */ u8 unk0[0x1BE];
    /* 0x1BE */ s16 unk1BE;
    /* 0x1C0 */ s16 unk1C0;
} JpGame;

#define SAI_STATE ((struct SaiState *)((SessionData *)SESSION_DATA)->areaSession)

/* a menu of jp's executable (openChoiceMenu sets it up, addChoiceMenuItem adds
   an item, runChoiceMenu runs it a frame) */
typedef struct {
    /* 0x000 */ u8 unk0[0x23C];
    /* 0x23C */ s32 selected;
} JpMenu;

/* a line of the message window, rendered into VRAM as it is added */
typedef struct {
    /* 0x00 */ s32 active;
    /* 0x04 */ s32 vramY; /* where the line's glyphs are rendered */
    /* 0x08 */ u32 width;
    /* 0x0C */ u32 shown;
    /* 0x10 */ u8 text[0x30];
    /* 0x40 */ s8 palettes[0x18]; /* one for each two bytes of text */
} MsgLine;

/* a line typed as text instead */
typedef struct {
    /* 0x00 */ s32 shown;
    /* 0x04 */ s32 length;
    /* 0x08 */ s8 active;
    /* 0x09 */ char text[0x43];
} TypedLine;

/* what SAISEG's screens share: the message window, a menu, its windows */
typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 typing;
    /* 0x002 */ u8 unk2[2];
    /* 0x004 */ MsgLine lines[4];
    /* 0x164 */ JpMenu menu;
    /* 0x3A4 */ u8 unk3A4[0xC];
    /* 0x3B0 */ JpWindow *unk3B0;
    /* 0x3B4 */ JpWindow *unk3B4;
    /* 0x3B8 */ JpWindow *unk3B8;
    /* 0x3BC */ JpWindow *unk3BC;
    /* 0x3C0 */ s32 unk3C0;
    /* 0x3C4 */ s32 unk3C4;
    /* 0x3C8 */ u8 unk3C8;
    /* 0x3C9 */ u8 unk3C9;
    /* 0x3CA */ u8 unk3CA;
    /* 0x3CB */ u8 unk3CB;
    /* 0x3CC */ u8 unk3CC[4];
} SaiUi;

/* the typed lines, and where the next one goes */
typedef struct {
    /* 0x000 */ TypedLine lines[4];
    /* 0x130 */ TypedLine *next;
} TypedText;

extern SaiUi SAI_UI;
extern TypedText SAI_TYPED_TEXT;
#endif

int MoveImage2(Rect16 *rect, int x, int y);

#endif /* DCB_SAISEG_H */
