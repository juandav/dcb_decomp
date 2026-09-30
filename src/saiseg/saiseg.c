#include "common.h"
#include "game.h"
#include "dcb/window.h"
#include "dcb/text.h"
#include "dcb/heap.h"
#include "dcb/script.h"
#include "dcb/frame_callback.h"
#include "dcb/task.h"
#include "dcb/hacking_shell.h"
#include "dcb/vram_upload.h"
#include "dcb/scene3d.h"
#include "dcb/game_flow.h"
#include "dcb/loader.h"
#include "dcb/duel_launch.h"
#include "dcb/card_db.h"
#include "dcb/sound_play.h"
#include "dcb/scroll_bg.h"
#include "dcb/memcard.h"
#include "dcb/archive.h"
#include "dcb/card_render.h"
#include "dcb/prim.h"
#include "dcb/menu.h"
#include "dcb/prim_util.h"
#include "dcb/battle_hud.h"
#include "dcb/game_exit.h"
#include "dcb/sound.h"
#include "dcb/dialog.h"
#include "dcb/pad.h"

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

/* the world map's states, SAI_MAP_STATE_FUNCS' index (WorldMap.state) */
enum MapState {
    MAP_OPENING,
    MAP_IDLE,
    MAP_WALKING,
    MAP_FADE_IN,
    MAP_CHANGE_REGION,
    MAP_SWITCH_REGION,
    MAP_ENTER_AREA,
    MAP_CLOSING,
    MAP_DONE,
    MAP_MENU
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
    s32 x;
    s32 y;
    s32 clutX;
    u16 clutY;
    u16 padE;
    u8 w;
    u8 h;
    u8 mode;
    u8 pad13;
} SpriteTemplate;
extern SpriteTemplate SAI_SPRITE_TEMPLATES[];

void SAI_glitchVram(s8 animate);

typedef struct {
    u8 text[0x3C];
    s16 shown;
    s8 active;
    s8 length;
} TextLine;
s32 SAI_typeTextLine(s32 x, s32 y, TextLine *line, s32 z);
s32 SAI_typeTextLines(s16 x, s16 y, s32 z);
typedef struct {
    /* 0x00 */ u8 pad[0x8];
    /* 0x08 */ s32 playTime;
    /* 0x0C */ s32 cardRate;
    /* 0x10 */ s32 abilityRate;
    /* 0x14 */ PlayerProfile *profile;
    /* 0x18 */ char *tamerRank;
    /* 0x1C */ char *collectorRank;
    /* 0x20 */ s8 state; /* 0: closed, 1: opening, 2: open */
} PlayerStats;
extern PlayerStats SAI_PLAYER_STATS;
extern char *STR_TAMER_RANKS[8];
extern char *STR_COLLECTOR_RANKS[8];

extern s8 SAI_CHOICE_MODE;
extern s8 SAI_AREA_MODE;
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
extern OpponentList SAI_OPPONENTS;
typedef struct {
    s32 unk0;
    Script *script;
    s32 *regs;
} ScriptRunner;
extern u8 SAI_CHOICE_STATES[];
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
extern WindowDef SAI_ERROR_WINDOW_DEFS[5];
void SAI_openErrorWindow(UiWindow *window, WindowDef *def);
void SAI_runSystemErrorHack(void);
void SAI_runHackingScene1(void);
void SAI_runHackingScene3(void);
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
extern OpponentInfo *SAI_OPPONENT_INFO;
extern PolyF4 *SAI_MAP_PATH_POLYS[2];
extern DrTPage SAI_MAP_PATH_TPAGES[2][20];
extern u8 SAI_REWARD_FROM_SCRIPT;
extern u8 SAI_PRIZE_PACK;
extern s16 SAI_SCRIPT_REWARD_CARDS[3];

extern TextLine SAI_TEXT_LINES[3];
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
    /* 0x116 */ u8 unk116;
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
extern AreaState SAI_AREA;
extern void (*SAI_CHOICE_MENU_FUNCS[])(void);
extern s8 SAI_CHOICE_PHASE;
extern ScriptRunner *SAI_SCRIPT[1];
extern u8 SAI_OPPONENT_COUNT;
extern s8 SAI_OPPONENT_IDS[24];
extern Sprite3D *SAI_SPRITES[];
void SAI_drawSprite(Sprite3D *sprite);
Sprite3D *SAI_createSprite(s32 id);
void SAI_setSpriteSize(Sprite3D *sprite, s16 width, s16 height);
void SAI_setSpriteBlendMode(Sprite3D *sprite, s32 abr);
void SAI_setSpriteDepth(Sprite3D *sprite, s32 otz);
extern UiWindow SAI_PLAYER_DATA_WINDOW;
extern UiWindow SAI_STATS_HINT_WINDOW;
void SAI_drawPlayerData(UiWindow *win);
void SAI_drawCompleteStatsHint(UiWindow *window);
extern u8 SAI_PANEL_COVER_ALPHA;
void SAI_freeSprite(void *ptr);
void SAI_runAreaScript(ScriptRunner *runner);
Script *SAI_createScriptContext(u8 *scriptData);
extern UiWindow SAI_MESSAGE_WINDOW;
void SAI_drawMessageWindow(UiWindow *window);
void SAI_drawChoiceMenu(void);
void SAI_drawAreaName(void);
void SAI_drawLocationLabel(void);
void SAI_initPanelCover(void);
void SAI_createPanelFrame(void);
void SAI_createPanelFrameShadow(void);
void SAI_drawPanel(void);
extern s8 SAI_PANEL_IMAGE_HIDDEN;
void SAI_drawPanelCover(void);
void SAI_drawPanelFrame(void);
void SAI_drawPanelFrameShadow(void);
extern s16 SAI_SELECT_DELAYS[6];
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
extern Splash *SAI_SPLASH;
extern const char SAI_STR_SYSTEM_ERROR[];
extern UiWindow SAI_ERROR_WINDOWS[5];
extern void (*SAI_ERROR_WINDOW_DRAW_FUNCS[5])();
extern u8 SAI_HACK_OVERLAY_FLICKER;
extern u8 SAI_HACK_OVERLAY_STATE;
extern UiWindow SAI_WORD_GRID_WINDOW;
extern UiWindow SAI_KEYWORD_WINDOW;
extern UiWindow SAI_WORD_HELP_WINDOW;
void SAI_drawWordInputGrid(UiWindow *window);
void SAI_drawKeywordField(UiWindow *window);
void SAI_drawWordInputHelp(UiWindow *window);
typedef struct {
    s16 type;
    s16 unk2;
    s32 size;
} PackEntry;
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
extern WorldMap SAI_WORLD_MAP;
extern s8 SAI_MAP_ANIM_REGION;
void SAI_initRegion0Anims(void);
void SAI_initRegion1Anims(void);
void SAI_initRegion2Anims(void);
typedef struct {
    s32 timer;
    s16 x;
    s16 y;
    u8 frames;
    u8 frameTime;
} MapAnim;
extern MapAnim *SAI_MAP_ANIMS;
void SAI_drawRegion0Anims(void);
void SAI_drawRegion1Anims(void);
void SAI_drawRegion2Anims(void);
extern u8 SAI_MAP_ACTIVE;
typedef struct {
    UiWindow window;
    s32 slot;
} RewardWindow;
extern UiWindow SAI_PARTNER_GET_WINDOW;
void SAI_drawPartnerChoices(UiWindow *win);
extern UiWindow SAI_DIGI_PARTS_WINDOW;
void SAI_drawDigiPartsList(UiWindow *window);
extern Rect16 SAI_REGION_MAP_RECTS[];
void SAI_setSpriteImage8Bit(Sprite3D *sprite, Rect16 *rect, s8 flip);
extern u8 SAI_MAP_OLD_MARKER_COUNT;
extern s8 SAI_MAP_STATE;
void SAI_initRegion(void);
extern u8 SAI_MAP_ALPHA;
extern u8 SAI_MAP_OPEN_MENU;
extern u8 SAI_MAP_MENU_TAB_STATE;
void SAI_openMapMenu(void);
extern s8 SAI_MAP_LABEL_REGION;
extern MapNode SAI_MAP_NODES[16];
s32 SAI_isSavedScriptFlagSet(u32 id);
typedef struct {
    s32 offset;
    s8 state;
} MenuTab;
extern MenuTab SAI_MAP_MENU_TAB;
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
extern RewardScreen *SAI_REWARD_SCREEN;
void SAI_drawRewardTitle(UiWindow *window);
void SAI_drawRewardResult(RewardWindow *window);
void SAI_drawRewardCard(CardWindow *window);
void SAI_showRewardCards();
typedef struct {
    u8 pad[0x1020];
    u16 armorFlags;
} SaisegSessionData;
void SAI_setSpriteImage4Bit(Sprite3D *sprite, Rect16 *rect);
extern s8 SAI_MAP_NODE;
extern u8 SAI_MAP_NAME_SLIDE_DIR;
extern s8 SAI_MAP_PORTRAIT_STATE;
typedef struct {
    /* 0x000 */ OpponentList opponents;
    /* 0x12C */ OpponentSelect select;
} SaveBlock;
extern s8 D_801F469E;
extern s8 SAI_LOCATION;
extern u8 SAI_OPEN_PANEL;
s32 SAI_spinPanel(void);
void SAI_updateOpponentPortraits(void);
extern s32 SAI_SELECT_BLINK[6];
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
    /* 0x1A7 */ s8 unk1A7;
    /* 0x1A8 */ u8 location;
    /* 0x1A9 */ s8 resumeMode; /* 1: back from a duel, 2: back from the complete stats */
    /* 0x1AA */ s8 shownOpponent;
} SaisegSession;
#define SESSION ((SaisegSession *)((SessionData *)D_8006E054)->unk100C)
#define SESSION_SUB (((SessionData *)D_8006E054)->unk100C)
extern s8 SAI_PLAYER_STATS_STATE;
extern s16 SAI_PLAYER_DATA_ROW;
extern char *SAI_PLAYER_DATA_HELP[];
extern u8 SAI_NEXT_ICON_BLINK;
extern u8 SAI_TEXT_TYPING;
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
extern WordInput SAI_WORD_INPUT;
extern CursorHighlight SAI_KEYWORD_CURSOR;
void SAI_clearOpponents(void);
void SAI_runTalkPanel(void);
void SAI_unlockArmorsFromFlags(s32 *regs);
void SAI_runPlayerData(void);
extern u8 SAI_MESSAGE_WINDOW_PALETTE;
extern Rect16 SAI_GLITCH_VRAM_RECTS[6];
int MoveImage2(Rect16 *rect, int x, int y);
s32 SAI_stepAreaScript(ScriptRunner *runner);
void SAI_tickChoiceMenu(void);
void SAI_tickOpponentSelectInput(void);
void SAI_clearTextLines(TextLine *lines);
s32 SAI_addTextLine(u8 *src);
void SAI_drawOpponentStats(void);
void SAI_stepBrightness(s8 index);
void SAI_setSpriteBrightness(Unk801EBD94 *quads, u8 value);
extern s8 SAI_SELECT_SELECTED;
extern u8 SAI_SELECT_EMPTY_BLINK[6];
s32 SAI_moveOpponentPortraits(void);
extern WindowDef SAI_STATS_HINT_WINDOW_DEF;
extern Menu SAI_PLAYER_DATA_MENU;
extern CursorHighlight SAI_PLAYER_DATA_CURSOR;
void SAI_computePlayerStats(void);
void SAI_openWindows(UiWindow *window, WindowDef *def, s32 count);
void SAI_drawPlayerDataWindows(void);
extern s8 SAI_CLOSE_PANEL;
extern s8 SAI_SELECT_STATE;
void SAI_createPanel(void);
void SAI_initOpponentSelect(void);
void SAI_toggleMessageWindow(s32 close);
void SAI_drawOpponentSelect(void);
s32 SAI_uncoverPanel(void);
s32 SAI_coverPanel(void);
s8 SAI_slideOpponentPortraits(void);
void SAI_zoomOpponentPortrait(void);
s32 SAI_slideChosenOpponentBack(void);
void SAI_randomizeOpponentDelays(void);
void SAI_freeOpponentSelect(void);
void SAI_freePanel(void);
void SAI_initMapAnims(s8 mode);
void SAI_drawMapPaths(FrameBuffer *fb);
long ratan2(long y, long x);
int csqrt(int a);
extern u8 SAI_MAP_ANIMATING;
void SAI_setRegionMapImage(void);
void SAI_loadAreaPak(void);
extern void (*SAI_ICON_MOTION_FUNCS[])(void);
extern Rect16 SAI_PLAYER_DATA_CURSOR_RECTS[15];
extern char *SAI_PLAYER_DATA_LABELS[];
TextLine *SAI_allocTextLine(TextLine *lines);
void rollRewardCards(s32 player, s32 pack);
void SAI_addScriptRewardCards(void);
void SAI_drawRewardWindows(void);
void SAI_waitForCross(void);
void SAI_drawRewardCardArt(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u16 clut, u8 *rgb);
extern s8 SAI_MAP_MENU_CURSOR;
extern s8 SAI_MAP_MENU_PHASE;
extern s32 SAI_SELECT_PROGRESS[6];
void SAI_initOpponentInfo(void);
void SAI_drawOpponentInfo(void);
void SAI_runSplash(s32 index, s32 task);
s32 SAI_slideInOpponentStats(void);
s32 SAI_slideInOpponentPortrait(void);
void SAI_tickOpponentBanner(void);
void SAI_freeOpponentInfo(void);
extern s16 SAI_MAP_FRAME_BRIGHTNESS;
extern s32 SAI_MAP_ICON_SLIDE;
extern CursorHighlight SAI_WORD_GRID_CURSOR;
extern char SAI_WORD_INPUT_CHARS[];
s32 SAI_moveWordInputCursor(void);
typedef struct {
    u8 cursor;
    s8 done;
} PartnerCursor;
typedef struct {
    s32 state;
    u8 count;
} PartnerList;
extern PartnerCursor SAI_PARTNER_CURSOR;
extern PartnerList SAI_PARTNER_LIST;
extern CursorHighlight SAI_PARTNER_GET_CURSOR;
extern u8 SAI_PARTNER_CHOICE_CARDS[4];
void SAI_drawPartnerCardArt(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u8 *rgb, s32 z, s32 palette);
extern void (*SAI_MAP_STATE_FUNCS[])(void);
extern s8 SAI_REGION_NODES[3][7];
extern s8 SAI_ICON_RUNNING;
void SAI_loadMapTextures(s32 useMap);
void SAI_runCornerIcon(s32 state);
void SAI_initCamera(void);
void SAI_initPathPolys(void);
void SAI_unlockMapNodes(void);
void SAI_createMapMenuTab(s8 keepTabState);
void SAI_createMapFrame(void);
void SAI_createMapFrameShadow(void);
void SAI_createRegionLabel(void);
void SAI_createAreaPortrait(void);
void SAI_createMapAreaName(void);
void SAI_drawMapMenuTab(void);
void SAI_drawMapFrame(void);
void SAI_drawMapFrameShadow(void);
void SAI_drawMapAnims(s8 mode);
void SAI_drawRegionFade(void);
void SAI_drawMapAreaName(void);
void SAI_drawRegionLabel(void);
void SAI_drawAreaPortrait(void);
void SAI_openDeckEditorFromMap(void);
void SAI_openEquipmentFromMap(void);
void SAI_runArea(s32 resume);
extern s8 SAI_ICON_MOTION;
extern s8 SAI_EXIT_ACTION;
extern s32 SAI_EXIT_ARG;
ScriptRunner *SAI_createAreaScript(void);
s32 *SAI_allocScriptRegisters(s32 count);
void SAI_loadScriptFlags(void);
void SAI_createAreaName(void);
void SAI_createLocationLabel(void);
void SAI_drawAreaHud(void);
void SAI_runOpponentSelectPanel(void);
void SAI_runOpponentInfoPanel(s32 index);
void SAI_reopenPlayerData(void);
void SAI_tickArea(void);
void SAI_flickerHackOverlay(void);
void SAI_saveScriptFlags(void);
void SAI_runWorldMap(s32 resume, s32 openMenu);
typedef struct {
    char *name;
    u8 unk4[8];
} DigiPart;
extern u8 SAI_OWNED_DIGI_PARTS[];
extern DigiPart SAI_DIGI_PARTS[];
extern const u8 SAI_PARTNER_ARMOR_ICONS[][3];
s32 SAI_tickPlayerDataMenu(Menu *menu);
typedef struct {
    u8 pad[0x30];
    s32 scriptOffset;
} ProfileSave;
extern u8 SAI_DIALOG[];
extern s8 SAI_PARTNER_COUNT;
extern s8 SAI_PARTNER_CHOICES[4];
void SAI_runChoiceScript(ScriptRunner *runner);
void SAI_runPartnerGet(s32 task);
void SAI_showOpponentInfo(void);
void SAI_runWordInput(char *word);
void SAI_runHackOverlay(void);
void SAI_closeHackOverlay(void);
void SAI_openChoiceMenu(s8 mode);
void SAI_addChoice(s8 id);
void SAI_addOpponent(s32 opponent);
void func_801E09F4(void);
void SAI_runRewardTask(u8 value);
void SAI_grantDigiPart(s32 ability, s32 task);
void SAI_runHackingEvent(s32 mode, s32 task);
extern const char SAI_STR_NO_KEYWORD[];
extern char *SAI_KEYWORDS[10];
int strcmp(char *a, char *b);
void SAI_drawWordInputWindows(void);
void SAI_selectMapNode(void);
void SAI_moveSpriteCorners(Sprite3D *sprite, s16 dx, s16 dy);
void SAI_initMapPaths(void);
extern const u8 SAI_PARTNER_CARD_IDS[];
void SAI_drawPartnerGetWindow(void);
void SAI_setPartnerObtainedFlag(s32 index);

void SAI_drawMessageWindow(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 found;
    char buf[0x48];

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (SAI_PLAYER_STATS_STATE == 2) {
        drawText(x + 2, y, (s32)SAI_PLAYER_DATA_HELP[SAI_PLAYER_DATA_ROW], 7, z);
        return;
    }
    if (SAI_AREA.waitingForCross != 0 && SAI_AREA.typing == 0) {
        SAI_AREA.nextBlink++;
        if (SAI_AREA.nextBlink & 0x10) {
            drawIcon(x + 0x11C, y + 0x1A, 0, 0x1B, z);
        }
    } else {
        SAI_NEXT_ICON_BLINK = 0;
    }
    found = SAI_typeTextLines(x, y, z);
    SAI_TEXT_TYPING = found;
}

void SAI_clearTextLines(TextLine *lines) {
    s32 i;

    for (i = 0; i < 3; i++, lines++) {
        lines->active = -1;
        bzero(lines, 0x3C);
    }
}

TextLine *SAI_allocTextLine(TextLine *line) {
    s32 i;

    for (i = 0; i < 3; i++, line++) {
        if (line->active == -1) {
            line->active = 0;
            line->shown = 0;
            return line;
        }
    }
    return NULL;
}

/* Copies a script text into a free line, expanding "*h0" to the player's name
   and "\0x22" to a quote; returns the line's index or -1 when all are used. */
s32 SAI_addTextLine(u8 *src) {
    u8 *name = (u8 *)((PlayerProfile *)PLAYER_PROFILES)->name;
    TextLine *slot = SAI_allocTextLine(SAI_TEXT_LINES);
    u8 *dst;
    s32 i;
    s32 found;

    if (slot == NULL) {
        return -1;
    }
    slot->text[0] = '*';
    slot->text[1] = 'w';
    slot->text[2] = '2';
    dst = &slot->text[3];
    while (*src != 0) {
        if (*src < 0x81 || *src >= 0x99) {
            if (*src == '*') {
                if (src[1] == 'h' && src[2] == '0') {
                    src += 3;
                    for (i = 0; i < 12 && *name != 0; i++) {
                        *dst++ = *name++;
                    }
                    continue;
                }
            } else if (*src == '\\') {
                found = 0;
                for (i = 1; i < 5; i++) {
                    if (!found && src[i] == 0) {
                        found = 1;
                        break;
                    }
                }
                if (found != 1 && src[1] == '0' && src[2] == 'x' && src[3] == '2' && src[4] == '2') {
                    src += 5;
                    *dst++ = '"';
                    continue;
                }
            }
        } else {
            *dst++ = *src++;
        }
        *dst++ = *src++;
    }
    *dst = 0;
    for (dst = slot->text, i = 0; i < 60 && *dst != 0; i++, dst++) {
    }
    if (i < 60) {
        slot->active = 0;
    }
    slot->length = i;
    for (i = 0; i < 3 && slot != SAI_TEXT_LINES; i++, slot--) {
    }
    return i;
}

/* not referenced by any code */
const s32 D_801DDF38 = 6;

/* where the five windows of SAI_ERROR_WINDOWS open */
WindowDef SAI_ERROR_WINDOW_DEFS[5] = {
    { { 0xA0, 0x32, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0xF, 0x82, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0x1E, 0x14, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0x5A, 0xB4, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
    { { 0xA0, 0x78, 0x40, 0x20 }, 0x80, 0x77, 0, 0, 2 },
};

/* VRAM areas SAI_glitchVram moves with MoveImage2 */
Rect16 SAI_GLITCH_VRAM_RECTS[6] = {
    { 0x220, 0xE2, 0x20, 2 },
    { 0x220, 0xE5, 0x20, 1 },
    { 0x220, 0xEC, 0x20, 1 },
    { 0x220, 0xF4, 0x20, 1 },
    { 0x230, 0xEB, 0x20, 1 },
    { 0x280, 0x1FC, 0x100, 3 },
};

WindowDef SAI_STATS_HINT_WINDOW_DEF = { { 0x88, 0x14, 0xA4, 0xD }, 0x80, 0x31, 0, 0, 8 };

/* the rows of the player's data screen */
Menu SAI_PLAYER_DATA_MENU = { NULL, NULL, { 10, 48, 300, 126 }, 0, -1, 0, -1, 0xa, 0x16, 72, 12, 0, 15, 0, 1, 0, 14, 0, 0, 0 };

/* the help line for each row of SAI_PLAYER_DATA_MENU */
char *SAI_PLAYER_DATA_HELP[12] = {
    "*w1Player's Name.",
    "*w1Tamer Rank is based on Wins. There are 8 \nRanks. \"Beginner Tamer\" with 0 wins\nto \"Invincible Tamer\" with 500+ Wins.",
    "*w1Collector Rank is set by the number of\nCards collected. 7 Titles from \"General \n\tPublic\" to the highest, plus one more.",
    "*w1This shows how far you are in the game.\nCan you achieve 100%?",
    "*w1This is the ratio of Cards collected.",
    "*w1Collected Digi-Parts ratio, 128 total.",
    "*w1Watch your wins and losses in Digi-land.\n\tRemember the Cards you were beaten by.\nIt will make you a better Card Tamer.",
    "*w1Wins & losses in \"Battle with Friends.\"\nDo battle with all your friends.\nEverybody will love it! It's guaranteed!",
    "*w1This is the first Deck you have.",
    "*w1This is the second Deck you have.",
    "*w1This is the third Deck you have.",
    "*w1These are Partners & Digi-Eggs you own.\nVeemon alone has 3 Digi-Eggs.",
};

s32 SAI_typeTextLine(s32 x, s32 y, TextLine *line, s32 z) {
    u8 buf[0x40];
    u8 *dst = buf;
    u8 *src;
    s8 i;
    u8 c;

    if (line->length == line->shown) {
        drawText(x, y, (s32)line, 7, z);
        return -1;
    }
    src = line->text;
    for (i = 0; i < line->shown; i++) {
        *dst++ = *src++;
    }
    c = *src;
    if (c == 0) {
        return 1;
    }
    if (*src < 0x81 || *src > 0x98) {
        if (c == '*') {
            switch (src[1]) {
            case 'a':
            case 'b':
            case 'c':
            case 'e':
            case 's':
            case 'w':
                if (src[2] >= '0' && src[2] <= '9') {
                    *dst++ = *src++;
                    *dst++ = *src++;
                    line->shown += 2;
                }
                break;
            }
        }
        dst[0] = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        line->shown += 1;
    } else {
        *dst++ = src[0];
        dst[0] = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        line->shown += 2;
    }
    return 1;
}

s32 SAI_typeTextLines(s16 x, s16 y, s32 z) {
    TextLine *entry = SAI_TEXT_LINES;
    s32 found = 0;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->active != -1) {
            if (SAI_typeTextLine(x + 4, y + i * 13, entry, z) == 1) {
                found = 1;
                break;
            }
        }
    }
    return found;
}

void SAI_finishTextLines(void) {
    TextLine *entry = SAI_TEXT_LINES;
    s32 i;

    for (i = 0; i < 3; i++, entry++) {
        if (entry->active != -1) {
            entry->shown = entry->length;
        }
    }
}

void SAI_releaseTextLines(TextLine *lines) {
    s32 i;

    for (i = 0; i < 3; i++, lines++) {
        lines->active = -1;
    }
}

void SAI_openChoiceMenu(s8 mode) {
    s32 i;

    SAI_AREA.choiceMode = mode;
    SAI_AREA.choiceSlide = 0;
    SAI_AREA.choiceCursor = 0;
    SAI_AREA.choiceCount = 0;
    SAI_AREA.choicePhase = 0;
    for (i = 0; i < 5; i++) {
        SAI_CHOICE_STATES[i] = 0;
    }
    if (mode == 1) {
        SAI_SPRITES[25] = SAI_createSprite(0x15);
        SAI_SPRITES[25]->pos.vx = -0x66;
        SAI_SPRITES[25]->pos.vy = -0x13;
        for (i = 0; i < 5; i++) {
            SAI_SPRITES[i + 26] = SAI_createSprite(0x14);
            SAI_SPRITES[i + 26]->pos.vx = -0x68;
            SAI_SPRITES[i + 26]->pos.vy = i * 16 - 0x37;
            SAI_SPRITES[i + 26]->corners[2].vy++;
            SAI_SPRITES[i + 26]->corners[3].vy++;
            SAI_setSpriteDepth(SAI_SPRITES[i + 26], 0x21);
            SAI_setSpriteBlendMode(SAI_SPRITES[i + 26], 0);
        }
    } else {
        SAI_SPRITES[25] = SAI_createSprite(0x16);
        SAI_SPRITES[25]->pos.vx = -0x66;
        SAI_SPRITES[25]->pos.vy = -0x2A;
        for (i = 0; i < 3; i++) {
            SAI_SPRITES[i + 26] = SAI_createSprite(0x3D);
            SAI_SPRITES[i + 26]->pos.vx = -0x68;
            SAI_SPRITES[i + 26]->pos.vy = i * 16 - 0x3F;
            SAI_SPRITES[i + 26]->corners[2].vy++;
            SAI_SPRITES[i + 26]->corners[3].vy++;
            SAI_setSpriteDepth(SAI_SPRITES[i + 26], 0x21);
            SAI_setSpriteBlendMode(SAI_SPRITES[i + 26], 0);
        }
    }
    SAI_setSpriteDepth(SAI_SPRITES[25], 0x20);
    SAI_setSpriteBlendMode(SAI_SPRITES[25], 1);
}

void SAI_addChoice(s8 id) {
    Rect16 rect;

    if (((SessionData *)D_8006E054)->unk1027 == 1 && id == 15) {
        SAI_AREA.choiceStates[SAI_AREA.choiceCount] = 2;
    }
    if (SAI_CHOICE_MODE != 0) {
        rect.x = 0x300;
        rect.y = id << 4;
        rect.w = 0x58;
    } else {
        rect.x = 0x318;
        rect.y = (id - 12) << 4;
        rect.w = 0x40;
    }
    rect.h = 0x10;
    SAI_setSpriteImage4Bit(SAI_SPRITES[SAI_AREA.choiceCount + 26], &rect);
    SAI_AREA.choiceCount++;
}

void SAI_slideInChoiceMenu(void) {
    SAI_AREA.choiceSlide++;
    if (SAI_AREA.choiceSlide > 20) {
        SAI_AREA.choiceSlide = 20;
        SAI_AREA.choicePhase = 1;
    }
}

void SAI_tickChoiceInput(void) {
    if (PAD_STATES[0]->repeat & PAD_DOWN) {
        SAI_AREA.choiceCursor++;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->repeat & PAD_UP) {
        SAI_AREA.choiceCursor--;
        playSoundEffect(2);
    } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
        playSoundEffect(1);
        SAI_CHOICE_PHASE = 2;
        SAI_SCRIPT[0]->regs[1] = -1;
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        if (SAI_AREA.choiceStates[SAI_AREA.choiceCursor] != 2) {
            playSoundEffect(0);
            SAI_AREA.choicePhase = 2;
            SAI_SCRIPT[0]->regs[1] = SAI_AREA.choiceCursor + 1;
        }
    }
    if (SAI_AREA.choiceCursor < 0) {
        SAI_AREA.choiceCursor = SAI_AREA.choiceCount - 1;
    }
    if (SAI_AREA.choiceCursor >= SAI_AREA.choiceCount) {
        SAI_AREA.choiceCursor = 0;
    }
}

void SAI_slideOutChoiceMenu(void) {
    if (--SAI_AREA.choiceSlide < 0) {
        SAI_AREA.choiceSlide = 0;
        SAI_AREA.choicePhase = 3;
        SAI_AREA.choiceCount = 0;
    }
}

void SAI_freeChoiceMenu(void) {
    s32 i;

    if (SAI_CHOICE_MODE == 0) {
        for (i = 0; i < 4; i++) {
            SAI_freeSprite(SAI_SPRITES[i + 25]);
        }
    } else {
        for (i = 0; i < 6; i++) {
            SAI_freeSprite(SAI_SPRITES[i + 25]);
        }
    }
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
}

void SAI_tickChoiceMenu(void) {
    if (SAI_CHOICE_MENU_FUNCS[SAI_CHOICE_PHASE] != NULL) {
        SAI_CHOICE_MENU_FUNCS[SAI_CHOICE_PHASE]();
    }
}

void SAI_drawChoiceMenu(void) {
    s32 count;
    s32 x;
    s32 i;

    count = 5;
    if (SAI_CHOICE_MODE == 0) {
        count = 3;
    }
    if (SAI_AREA.choiceCount != 0) {
        x = (SAI_AREA.choiceSlide * -102 + (20 - SAI_AREA.choiceSlide) * -212) / 20;
        for (i = 0; i < count; i++) {
            if (SAI_AREA.choiceMode == 0) {
                SAI_SPRITES[i + 26]->pos.vx = x + 3;
            } else {
                SAI_SPRITES[i + 26]->pos.vx = x;
            }
            if (SAI_AREA.choicePhase == 1 && SAI_AREA.choiceCursor == i) {
                if (SAI_AREA.choiceStates[i] != 0) {
                    SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xF1);
                    SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xF1);
                } else {
                    SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xF0);
                    SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xF0);
                }
            } else if (SAI_AREA.choiceStates[i] != 0) {
                SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xF2);
                SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xF2);
            } else {
                SAI_SPRITES[i + 26]->quads[0].clut = getClut(0x200, 0xEF);
                SAI_SPRITES[i + 26]->quads[1].clut = getClut(0x200, 0xEF);
            }
            SAI_drawSprite(SAI_SPRITES[i + 26]);
        }
        SAI_SPRITES[25]->pos.vx = x;
        SAI_drawSprite(SAI_SPRITES[25]);
    }
}

void SAI_setPartnerObtainedFlag(s32 index) {
    s16 flagIds[6] = { 0x126, 0x12A, 0x12D, 0x133, 0x130, 0x136 };

    SAI_SCRIPT[0]->regs[flagIds[index]] = 1;
}

void func_801E09F4(void) {
    if (SAI_SCRIPT[0]->regs[266] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 1;
    }
    if (SAI_SCRIPT[0]->regs[267] != 0) {
        ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 2;
    }
}

void SAI_saveScriptFlags(void) {
    s32 flag;
    s32 i;
    s32 bit;

    flag = 12;
    for (i = 0; i < 12; i++) {
        for (bit = 0; bit < 32; bit++) {
            if (SAI_SCRIPT[0]->regs[flag++] != 0) {
                ((PlayerProfile *)PLAYER_PROFILES)->unk23FC[i] |= 1 << bit;
            } else {
                ((PlayerProfile *)PLAYER_PROFILES)->unk23FC[i] &= ~(1 << bit);
            }
            if (flag >= 0x16B) {
                break;
            }
        }
    }
    for (flag = 0x16B, bit = 0; bit < 9 && flag < 0x175; bit++, flag++) {
        ((PlayerProfile *)PLAYER_PROFILES)->unk242C[bit] = SAI_SCRIPT[0]->regs[flag];
    }
}

void SAI_loadScriptFlags(void) {
    s32 i;
    s32 bit;
    s32 flag;

    flag = 12;
    for (i = 0; i < 12; i++) {
        for (bit = 0; bit < 32 && flag < 0x16B; bit++, flag++) {
            SAI_SCRIPT[0]->regs[flag] = ((u32)((PlayerProfile *)PLAYER_PROFILES)->unk23FC[i] >> bit) & 1;
        }
    }
    for (flag = 0x16B, bit = 0; bit < 9 && flag < 0x174; bit++, flag++) {
        SAI_SCRIPT[0]->regs[flag] = ((PlayerProfile *)PLAYER_PROFILES)->unk242C[bit];
    }
}

/* Appends an opponent to the select list; every six entries the last one
   moves to the next page behind the page arrows 0x11 (next) and 0x10 (back). */
void SAI_addOpponent(s32 opponent) {
    s8 last;

    if (SAI_OPPONENTS.count % 6 == 0 && SAI_OPPONENTS.count != 0) {
        last = SAI_OPPONENTS.ids[SAI_OPPONENTS.count - 1];
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count - 1] = 0x11;
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = 0x10;
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = last;
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = opponent;
    } else {
        SAI_OPPONENTS.ids[SAI_OPPONENTS.count++] = opponent;
    }
}

void SAI_clearOpponents(void) {
    s32 i;

    SAI_OPPONENT_COUNT = 0;
    for (i = 0; i < 24; i++) {
        SAI_OPPONENT_IDS[i] = -1;
    }
}

void SAI_unlockArmorsFromFlags(s32 *regs) {
    s16 ids[13] = { 0x127, 0x128, 0x129, 0x12B, 0x12C, 0x12E, 0x12F, 0x134, 0x135, 0x131, 0x132, 0x137, 0x138 };
    s32 i;
    s32 partner;
    s32 slot;
    s32 first;

    ((SaisegSessionData *)D_8006E054)->armorFlags = 0;
    for (slot = 0; slot < 13; slot++) {
        if (regs[ids[slot]] != 0) {
            ((SaisegSessionData *)D_8006E054)->armorFlags |= 1 << slot;
        }
    }
    for (slot = 0; slot < 3; slot++) {
        partner = getSlotPartnerIndex(0, slot);
        if (partner == -1) {
            continue;
        }
        if (partner == 0) {
            for (i = 0; i < 3; i++) {
                if (regs[ids[i]] != 0) {
                    unlockPartnerArmor(0, partner, i);
                }
            }
        } else {
            for (i = 0, first = partner * 2 + 1; i < 2; i++) {
                if (regs[ids[i + first]] != 0) {
                    unlockPartnerArmor(0, partner, i);
                }
            }
        }
    }
}

void SAI_openCenteredWindow(UiWindow *window, Rect16 pos, s32 label, s32 style, s32 brightness) {
    Rect16 rect;
    Rect16 unused; /* never used, but the original frame has room for it */
    Rect16 view;

    rect.x = pos.x - pos.w / 2;
    rect.y = pos.y - pos.h / 2;
    rect.w = pos.w & ~1;
    rect.h = pos.h & ~1;
    view.x = 0;
    view.y = 0;
    view.w = (pos.w + 10) & ~1;
    view.h = 0x2000;
    openWindow(window, &rect, -1, (s16 *)&view, style, brightness, 0x80, 0x10);
    window->label = label;
    window->labelPalette = 8;
}

void SAI_toggleMessageWindow(s32 close) {
    Rect16 rect;
    Rect16 pos = { 0xA1, 0xCF, 0x12A, 0x28 };
    char *title = "MESSAGE";

    if (close == 0) {
        SAI_openCenteredWindow(&SAI_MESSAGE_WINDOW, pos, (s32)title, 8, 0x51);
        animateWindowTo(&SAI_MESSAGE_WINDOW, (Rect16 *)-1);
    } else {
        rect.x = pos.x - pos.w / 2;
        rect.y = pos.y - pos.h / 2;
        rect.w = pos.w & ~1;
        rect.h = pos.h & ~1;
        animateWindowTo(&SAI_MESSAGE_WINDOW, &rect);
    }
}

void SAI_runChoiceScript(ScriptRunner *runner) {
    s32 result;

    do {
        result = runScriptToNextEvent(runner->script, runner->regs);
        if (result == 1) {
            if (runner->script->eventOp == 10) {
                if (runner->script->eventArg == 14) {
                    SAI_CHOICE_STATES[runner->regs[1] - 1] = result;
                }
            }
            return;
        }
        clearScriptBusy(runner->script);
    } while (result != 0);
}

void SAI_createAreaName(void) {
    Rect16 rect;

    SAI_SPRITES[21] = SAI_createSprite(0x1E);
    SAI_SPRITES[21]->pos.vx = 0x33;
    SAI_SPRITES[21]->pos.vy = -0xBC;
    SAI_setSpriteDepth(SAI_SPRITES[21], 0x21);
    SAI_SPRITES[22] = SAI_createSprite(0x1F);
    SAI_SPRITES[22]->pos.vx = 0x34;
    SAI_SPRITES[22]->pos.vy = -0xBB;
    SAI_setSpriteDepth(SAI_SPRITES[22], 0x21);
    rect.x = ((PlayerProfile *)PLAYER_PROFILES)->unkE / 6 * 24 + 0x340;
    rect.y = ((PlayerProfile *)PLAYER_PROFILES)->unkE % 6 * 20;
    rect.w = 0x60;
    rect.h = 0x14;
    SAI_setSpriteImage4Bit(SAI_SPRITES[22], &rect);
}

void SAI_createLocationLabel(void) {
    s32 unused[2];

    SAI_AREA.location = -1;
    SAI_AREA.shownLocation = -1;
    SAI_SPRITES[23] = SAI_createSprite(6);
    SAI_SPRITES[23]->pos.vx = -0x5F;
    SAI_SPRITES[23]->pos.vy = -0x98;
    SAI_setSpriteDepth(SAI_SPRITES[23], 0x21);
    SAI_SPRITES[24] = SAI_createSprite(7);
    SAI_SPRITES[24]->pos.vx = -0x5F;
    SAI_SPRITES[24]->pos.vy = -0x60;
    SAI_SPRITES[24]->pos.vy = -0x97;
    SAI_setSpriteDepth(SAI_SPRITES[24], 0x22);
    SAI_AREA.labelSlide = 0;
}

void SAI_drawAreaName(void) {
    SAI_drawSprite(SAI_SPRITES[22]);
    SAI_drawSprite(SAI_SPRITES[21]);
}

void SAI_drawLocationLabel(void) {
    Rect16 rect;

    if (SAI_LOCATION == -1) {
        rect.x = 0x2E0;
        rect.y = 0x90;
    } else {
        rect.x = 0x2E0;
        rect.y = SAI_LOCATION * 24;
    }
    rect.w = 0x74;
    rect.h = 0x18;
    if (SAI_AREA.shownLocation != SAI_AREA.location) {
        SAI_AREA.labelSlide--;
        if (SAI_AREA.labelSlide < 0) {
            SAI_AREA.labelSlide = 0;
            SAI_AREA.shownLocation = SAI_AREA.location;
            SAI_setSpriteImage4Bit(SAI_SPRITES[24], &rect);
        }
    } else if (SAI_AREA.location != -1) {
        SAI_AREA.labelSlide++;
        if (SAI_AREA.labelSlide > 30) {
            SAI_AREA.labelSlide = 30;
        }
    } else {
        SAI_AREA.labelSlide = 0;
    }
    SAI_SPRITES[23]->pos.vy = (SAI_AREA.labelSlide * -97 + (30 - SAI_AREA.labelSlide) * -152) / 30;
    SAI_SPRITES[24]->pos.vy = SAI_SPRITES[23]->pos.vy + 2;
    SAI_drawSprite(SAI_SPRITES[24]);
    SAI_drawSprite(SAI_SPRITES[23]);
}

void SAI_openWindows(UiWindow *window, WindowDef *def, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        openWindow(window, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 12);
        if (def->label != 0) {
            window->label = def->label;
        }
        window->labelPalette = def->labelPalette;
        window++;
        def++;
    }
}

void SAI_computePlayerStats(void) {
    s32 i;

    SAI_PLAYER_STATS.profile = (PlayerProfile *)PLAYER_PROFILES;
    SAI_PLAYER_STATS.tamerRank = STR_TAMER_RANKS[SAI_PLAYER_STATS.profile->tamerRank];
    SAI_PLAYER_STATS.collectorRank = STR_COLLECTOR_RANKS[SAI_PLAYER_STATS.profile->collectorRank];
    SAI_PLAYER_STATS.cardRate = 0;
    SAI_PLAYER_STATS.abilityRate = 0;
    SAI_PLAYER_STATS.state = 2;
    SAI_PLAYER_STATS.playTime = (u16)SAI_PLAYER_STATS.profile->unk14 * 1000 / 166;
    for (i = 0; i < 301; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[i] & 0x40) {
            SAI_PLAYER_STATS.cardRate += 1000;
        }
    }
    SAI_PLAYER_STATS.cardRate /= 301;
    for (i = 0; i < 128; i++) {
        if (getPartnerAbilityState(0, i)) {
            SAI_PLAYER_STATS.abilityRate += 1000;
        }
    }
    SAI_PLAYER_STATS.abilityRate /= 128;
}

/* the three icons drawn for each partner */
const u8 SAI_PARTNER_ARMOR_ICONS[6][3] = {
    { 1, 2, 9 },
    { 3, 5, 0 },
    { 4, 6, 0 },
    { 7, 6, 0 },
    { 8, 1, 0 },
    { 1, 7, 0 },
};

void (*SAI_CHOICE_MENU_FUNCS[4])(void) = {
    SAI_slideInChoiceMenu,
    SAI_tickChoiceInput,
    SAI_slideOutChoiceMenu,
    SAI_freeChoiceMenu,
};

/* not referenced by any code: "後藤豪太" in Shift-JIS */
char D_801F35A4[24] = "\x8C\xE3\x93\xA1\x8D\x8B\x91\xBE";

/* the cursor of each row of SAI_PLAYER_DATA_MENU */
Rect16 SAI_PLAYER_DATA_CURSOR_RECTS[15] = {
    { 0x64, 0, 0x18, 0xE },
    { 0x64, 0xE, 0x3C, 0xE },
    { 0x64, 0, 0x48, 0xE },
    { 0x64, 0xE, 0x30, 0xE },
    { 0x64, 0, 0x60, 0xE },
    { 0x64, 0xE, 0x60, 0xE },
    { 0x15, 0, 0x60, 0xE },
    { 0x15, 0xE, 0x48, 0xE },
    { 0x15, 0, 0x30, 0xE },
    { 0x15, 0xE, 0x30, 0xE },
    { 0x15, 0, 0x30, 0xE },
    { 0x15, 0xE, 0x90, 0xE },
    { 0x15, 0, 0x18, 0xE },
    { 0x15, 0xE, 0x3C, 0xE },
    { 0x64, 0, 0x18, 0xE },
};

/* the labels of the player's data screen */
char *SAI_PLAYER_DATA_LABELS[15] = {
    "Name",
    "Battle Title",
    "Collector Title",
    "Game Completion",
    "Card Collection",
    "Digi-Parts Stock",
    "COM Battle Stats",
    "2P Battle Stats",
    "Deck 1",
    "Deck 2",
    "Deck 3",
    "Partner Cards & Digi-Eggs",
    "Wins",
    "Losses",
    "Deck",
};

void SAI_runPlayerData(void) {
    SAI_computePlayerStats();
    if (SAI_SCRIPT[0]->regs[15] != 0) {
        SAI_openWindows(&SAI_STATS_HINT_WINDOW, &SAI_STATS_HINT_WINDOW_DEF, 1);
    }
    openMenu(&SAI_PLAYER_DATA_MENU, &SAI_PLAYER_DATA_WINDOW, &SAI_PLAYER_DATA_CURSOR, (Bytes4 *)-1);
    SAI_PLAYER_DATA_WINDOW.label = (s32)"PLAYER'S DATA";
    func_80014C08(1);
    playSoundEffect(3);
    addFrameCallback((s32)SAI_drawPlayerDataWindows);
    while (1) {
        func_80014C08(1);
        if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
            break;
        }
        if ((PAD_STATES[0]->pressed & PAD_CIRCLE) && SAI_SCRIPT[0]->regs[15] != 0) {
            SAI_SCRIPT[0]->regs[0] = 0;
            SESSION->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
            SESSION->resumeMode = 2;
            SAI_AREA.exitArg = 2;
            SAI_AREA.exitAction = AREA_EXIT_TITLE_OR_ENDING;
            break;
        }
    }
    playSoundEffect(4);
    animateWindowTo(&SAI_PLAYER_DATA_WINDOW, (Rect16 *)-1);
    if (SAI_SCRIPT[0]->regs[15] != 0) {
        animateWindowTo(&SAI_STATS_HINT_WINDOW, (Rect16 *)-1);
    }
    func_80014C08(15);
    removeFrameCallback((s32)SAI_drawPlayerDataWindows);
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    SAI_AREA.rewardBusy = 0;
    SAI_PLAYER_STATS_STATE = 0;
}

void SAI_drawPlayerDataWindows(void) {
    drawWindow(&SAI_PLAYER_DATA_WINDOW, SAI_drawPlayerData, 0x18);
    if (SAI_SCRIPT[0]->regs[15] != 0) {
        drawWindow(&SAI_STATS_HINT_WINDOW, SAI_drawCompleteStatsHint, 0x18);
    }
}

s32 SAI_tickPlayerDataMenu(Menu *menu) {
    UiWindow *win;
    CursorHighlight *highlight;
    s16 target[4];

    win = menu->win;
    highlight = menu->cursor;
    menu->moved = 0;
    if (menu->active != 0) {
        highlight->brightness = 0x80;
        if (menu->nrows | menu->rowH) {
            if (PAD_STATES[menu->pad]->repeat & (PAD_UP | PAD_DOWN | PAD_L2 | PAD_R2)) {
                playMenuSound(2);
            }
            if (PAD_STATES[menu->pad]->repeat & PAD_UP) {
                menu->moved = 1;
                if (--menu->row < 0) {
                    scrollWindowTo(&win->originX, 0, win->view.h - win->rect.h);
                    menu->row = 11;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    if (menu->row * menu->rowH < win->scroll[3]) {
                        scrollWindowTo(&win->originX, 0, menu->row * menu->rowH);
                    }
                }
            } else if (PAD_STATES[menu->pad]->repeat & PAD_DOWN) {
                menu->moved = 1;
                if (++menu->row >= 12) {
                    scrollWindowTo(&win->originX, 0, 0);
                    menu->row = 0;
                } else if (menu->row == 11) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, (menu->row + 13) * menu->rowH - win->rect.h);
                } else if (menu->row * menu->rowH >= win->scroll[3] + win->rect.h) {
                    scrollWindowTo(&win->originX, 0, (menu->row + 1) * menu->rowH - win->rect.h);
                }
            } else if (PAD_STATES[menu->pad]->repeat & PAD_L2) {
                menu->moved = 1;
                menu->row -= (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row < 0) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, 0);
                    menu->row = 0;
                } else {
                    if (menu->row == 0) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo(&win->originX, 0, win->scroll[3] - (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                }
            } else if (PAD_STATES[menu->pad]->repeat & PAD_R2) {
                menu->moved = 1;
                menu->row += (win->rect.h + menu->rowH - 1) / menu->rowH;
                if (menu->row >= menu->nrows) {
                    PAD_STATES[menu->pad]->repeatEnabled = 0;
                    scrollWindowTo(&win->originX, 0, win->view.h - win->rect.h);
                    menu->row = 11;
                } else {
                    if (menu->row == menu->nrows - 1) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                    }
                    scrollWindowTo(&win->originX, 0, win->scroll[3] + (win->rect.h + menu->rowH - 1) / menu->rowH * menu->rowH);
                    if (menu->row >= 12) {
                        PAD_STATES[menu->pad]->repeatEnabled = 0;
                        menu->row = 11;
                    }
                }
            }
        }
    } else {
        highlight->brightness = 0x40;
    }
    if (menu->row != menu->prevRow || menu->col != menu->prevCol) {
        menu->col = SAI_PLAYER_DATA_CURSOR_RECTS[menu->row].x - 10;
        menu->colW = 1;
        menu->prevCol = menu->col;
        menu->prevRow = menu->row;
        target[0] = (win->rect.x - win->scroll[2]) + menu->ox + menu->col * menu->colW - 10;
        target[1] = (win->rect.y - win->scroll[3]) + menu->oy + menu->row * menu->rowH;
        target[2] = measureText(SAI_PLAYER_DATA_LABELS[menu->row]);
        target[3] = menu->ch;
        moveCursorHighlight(menu->cursor, (Rect16 *)target);
    }
    drawCursorHighlight(highlight, win->z);
    return menu->col + menu->row * menu->ncols;
}

void SAI_drawPlayerData(UiWindow *win) {
    /* not literals: GCC would share SAI_drawRewardCard's identical string */
    static const char rateFormat[] = "*s0%3d.%1d%%";
    static const char countFormat[] = "*s0%3d";
    Rect16 uv;
    char buf[0x48];
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 j;
    s32 partner;

    x = win->originX - 0x14;
    y = win->originY + 2;
    z = win->z;
    drawText(x + 0x62, y, (s32)SAI_PLAYER_DATA_LABELS[0], 6, z);
    drawText(x + 0x132 - measureText(SAI_PLAYER_STATS.profile->name), y, (s32)SAI_PLAYER_STATS.profile->name, 7, z);
    drawText(x + 0x62, y + 0xE, (s32)SAI_PLAYER_DATA_LABELS[1], 6, z);
    drawText(x + 0x132 - measureText(SAI_PLAYER_STATS.tamerRank), y + 0xE, (s32)SAI_PLAYER_STATS.tamerRank, 7, z);
    drawText(x + 0x62, y + 0x1C, (s32)SAI_PLAYER_DATA_LABELS[2], 6, z);
    drawText(x + 0x132 - measureText(SAI_PLAYER_STATS.collectorRank), y + 0x1C, (s32)SAI_PLAYER_STATS.collectorRank, 7, z);
    drawText(x + 0x62, y + 0x2A, (s32)SAI_PLAYER_DATA_LABELS[3], 6, z);
    sprintf(buf, rateFormat, SAI_PLAYER_STATS.playTime / 10, SAI_PLAYER_STATS.playTime % 10);
    drawText(x + 0x105, y + 0x2A, (s32)buf, 7, z);
    drawText(x + 0x62, y + 0x38, (s32)SAI_PLAYER_DATA_LABELS[4], 6, z);
    sprintf(buf, rateFormat, SAI_PLAYER_STATS.cardRate / 10, SAI_PLAYER_STATS.cardRate % 10);
    drawText(x + 0x105, y + 0x38, (s32)buf, 7, z);
    drawText(x + 0x62, y + 0x46, (s32)SAI_PLAYER_DATA_LABELS[5], 6, z);
    sprintf(buf, rateFormat, SAI_PLAYER_STATS.abilityRate / 10, SAI_PLAYER_STATS.abilityRate % 10);
    drawText(x + 0x105, y + 0x46, (s32)buf, 7, z);
    drawText(x + 0x15, y + 0x54, (s32)SAI_PLAYER_DATA_LABELS[6], 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->battleWins);
    drawText(x + 0x89, y + 0x54, (s32)buf, 7, z);
    drawText(x + 0xA3, y + 0x54, (s32)"Wins", 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->battleLosses);
    drawText(x + 0xC0, y + 0x54, (s32)buf, 7, z);
    drawText(x + 0xDB, y + 0x54, (s32)"Losses", 6, z);
    drawText(x + 0x15, y + 0x62, (s32)SAI_PLAYER_DATA_LABELS[7], 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->versusWins);
    drawText(x + 0x89, y + 0x62, (s32)buf, 7, z);
    drawText(x + 0xA3, y + 0x62, (s32)"Wins", 6, z);
    sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->versusLosses);
    drawText(x + 0xC0, y + 0x62, (s32)buf, 7, z);
    drawText(x + 0xDB, y + 0x62, (s32)"Losses", 6, z);
    for (i = 0; i < 3; i++) {
        drawText(x + 0x15, y + (i + 8) * 14, (s32)SAI_PLAYER_DATA_LABELS[i + 8], 6, z);
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            sprintf(buf, "%s %s", ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].unk1, SAI_PLAYER_DATA_LABELS[14]);
            drawText(x + 0x46, y + (i + 8) * 14, (s32)buf, 7, z);
            sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].unk108[1]);
            drawText(x + 0xCA, y + (i + 8) * 14, (s32)buf, 7, z);
            drawText(x + 0xE2, y + (i + 8) * 14, (s32)SAI_PLAYER_DATA_LABELS[12], 6, z);
            sprintf(buf, countFormat, ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].unk108[2]);
            drawText(x + 0xFE, y + (i + 8) * 14, (s32)buf, 7, z);
            drawText(x + 0x116, y + (i + 8) * 14, (s32)SAI_PLAYER_DATA_LABELS[13], 6, z);
        }
    }
    drawText(x + 0x15, y + 0x9A, (s32)SAI_PLAYER_DATA_LABELS[11], 6, z);
    x += 0xC;
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
            drawText(x + 0x15 + i * 84, y + 0xA8, (s32)((PlayerProfile *)PLAYER_PROFILES)->partners[i].card[0].name, 7, z);
            drawLargeText(x + 0x15 + i * 84, y + 0xB9, (s32)"RANK", 6, z);
            sprintf(buf, "%2d", (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level);
            drawText(x + 0x3D + i * 84, y + 0xB6, (s32)buf, 7, z);
            for (j = 0; j < 3; j++) {
                if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].unlockedArmors[j] != 0) {
                    partner = getSlotPartnerIndex(0, i);
                    drawIcon(x + 0x1D + i * 84 + j * 13, y + 0xC4, 0, SAI_PARTNER_ARMOR_ICONS[partner][j] + 0x1B, z);
                }
            }
        }
    }
    uv.x = 0;
    uv.y = 0x82;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(x + 0xC, y + 0xC, &uv, 0x8B, 0x3EA0, z, 0x80, -1);
    SAI_tickPlayerDataMenu(&SAI_PLAYER_DATA_MENU);
}

void SAI_drawCompleteStatsHint(UiWindow *window) {
    char buf[0x48];

    drawText(window->originX + 6, window->originY + 1, (s32)"*b0:Player's Complete Stats", 7, window->z);
}

void SAI_initPanelCover(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_80067784(&SAI_AREA.coverPolys[i]);
        SetSemiTrans(&SAI_AREA.coverPolys[i], 1);
        SetDrawTPage(&SAI_AREA.coverTpages[i], 0, 0, 0x40);
        SAI_AREA.coverPolys[i].b0 = 0xFF;
        SAI_AREA.coverPolys[i].g0 = 0xFF;
        SAI_AREA.coverPolys[i].r0 = 0xFF;
    }
    SAI_AREA.coverAlpha = 0xFF;
    SAI_AREA.pos.vz = 0;
    SAI_AREA.pos.vy = 0;
    SAI_AREA.pos.vx = 0;
    SAI_AREA.rot.vz = 0;
    SAI_AREA.rot.vy = 0;
    SAI_AREA.rot.vx = 0;
    SAI_AREA.corners[0].vx = SAI_AREA.corners[2].vx = -0x87;
    SAI_AREA.corners[1].vx = SAI_AREA.corners[3].vx = 0x8A;
    SAI_AREA.corners[0].vy = SAI_AREA.corners[1].vy = -0x4D;
    SAI_AREA.corners[2].vy = SAI_AREA.corners[3].vy = 0x48;
    for (i = 0; i < 4; i++) {
        SAI_AREA.corners[i].vz = 0;
    }
}

void SAI_drawPanelCover(void) {
    MATRIX matrix;
    SVECTOR corners[4];
    s16 xs[2] = { -0x8E, 0xB2 };
    s16 ys[2] = { -0x4D, 0x48 };
    s32 sxy[4];
    s32 depthCue;
    s32 flag;

    buildRotTransMatrix(&SAI_AREA.pos, &SAI_AREA.rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->unk78, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    func_8005C444(&matrix);
    corners[0] = SAI_AREA.corners[0];
    corners[1] = SAI_AREA.corners[1];
    corners[2] = SAI_AREA.corners[2];
    corners[3] = SAI_AREA.corners[3];
    RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].r0 = SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].g0 = SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].b0 = SAI_AREA.coverAlpha;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x0 = sxy[0];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y0 = sxy[0] >> 16;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x1 = sxy[1];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y1 = sxy[1] >> 16;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x2 = sxy[2];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y2 = sxy[2] >> 16;
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].x3 = sxy[3];
    SAI_AREA.coverPolys[FRAME_BUFFER_INDEX].y3 = sxy[3] >> 16;
    addPrim(&CURRENT_FRAME_BUFFER->ot[36], &SAI_AREA.coverPolys[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[36], &SAI_AREA.coverTpages[FRAME_BUFFER_INDEX]);
}

void func_801E2D90(void) {
    Rect16 rect;

    rect.x = 0x180;
    rect.y = 0x100;
    rect.w = 0xFF;
    rect.h = 0x80;
    if (SAI_AREA.unk11E != 0) {
        SAI_SPRITES[0] = SAI_createSprite(4);
        SAI_AREA.unk116 = 1;
        func_80014C08(20);
        SAI_toggleMessageWindow(1);
    } else {
        if (D_801F469E == 0) {
            SAI_SPRITES[0] = SAI_createSprite(3);
        } else {
            SAI_setSpriteImage8Bit(SAI_SPRITES[0], &rect, 0);
            SAI_SPRITES[0]->quads[0].clut = SAI_SPRITES[0]->quads[1].clut = getClut(0x280, 0x1F8);
        }
        D_801F469E = 1;
    }
}

s32 SAI_uncoverPanel(void) {
    s32 value = SAI_PANEL_COVER_ALPHA;
    s32 wasZero = value == 0;
    s32 n = value - 8;

    if (n < 0) {
        n = 0;
    }
    SAI_PANEL_COVER_ALPHA = n;
    return wasZero;
}

s32 SAI_coverPanel(void) {
    s32 done;
    s32 level;

    SAI_AREA.unk115 = 1;
    level = SAI_AREA.coverAlpha;
    done = level == 0xFF;
    level += 8;
    if (level > 0xFF) {
        level = 0xFF;
    }
    SAI_PANEL_COVER_ALPHA = level;
    return done;
}

void SAI_createPanelFrame(void) {
    s16 xs[5] = { -0xA2, -0x7A, -2, 0x76, 0x9E };
    s16 ys[4] = { -0x60, -0x38, 0x38, 0x60 };
    s32 i;
    s32 j;
    s32 palette;
    s32 k;

    if ((s8)SESSION->location < 0) {
        palette = 0;
    } else {
        palette = (s8)SESSION->location;
    }
    for (j = 10, i = 1; j < 20; j++, i++) {
        SAI_SPRITES[i] = SAI_createSprite(j);
        k = palette + 0xEB;
        SAI_SPRITES[i]->quads[0].clut = getClut(0x210, k);
        SAI_SPRITES[i]->quads[1].clut = getClut(0x210, palette + 0xEB);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            k = i * 2 + j;
            SAI_SPRITES[k + 1]->corners[0].vx = SAI_SPRITES[k + 1]->corners[2].vx = xs[j + 1];
            SAI_SPRITES[k + 1]->corners[1].vx = SAI_SPRITES[k + 1]->corners[3].vx = xs[j + 2];
            SAI_SPRITES[k + 1]->corners[0].vy = SAI_SPRITES[k + 1]->corners[1].vy = ys[i * 2];
            SAI_SPRITES[k + 1]->corners[2].vy = SAI_SPRITES[k + 1]->corners[3].vy = ys[i * 2 + 1];
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            k = i * 3 + j;
            SAI_SPRITES[k + 5]->corners[0].vx = SAI_SPRITES[k + 5]->corners[2].vx = xs[i * 3];
            SAI_SPRITES[k + 5]->corners[1].vx = SAI_SPRITES[k + 5]->corners[3].vx = xs[i * 3 + 1];
            SAI_SPRITES[k + 5]->corners[0].vy = SAI_SPRITES[k + 5]->corners[1].vy = ys[j];
            SAI_SPRITES[k + 5]->corners[2].vy = SAI_SPRITES[k + 5]->corners[3].vy = ys[j + 1];
        }
    }
}

void SAI_createPanelFrameShadow(void) {
    s16 xs[5] = { -0x9D, -0x75, 3, 0x7B, 0x9F };
    s16 ys[4] = { -0x63, -0x3B, 0x3D, 0x65 };
    s32 i;
    s32 j;
    s32 k;

    for (j = 0x30, i = 11; j < 0x3A; j++, i++) {
        SAI_SPRITES[i] = SAI_createSprite(j);
        SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xE2);
        SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xE2);
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            k = i * 2 + j + 11;
            SAI_SPRITES[k]->corners[0].vx = SAI_SPRITES[k]->corners[2].vx = xs[j + 1];
            SAI_SPRITES[k]->corners[1].vx = SAI_SPRITES[k]->corners[3].vx = xs[j + 2];
            SAI_SPRITES[k]->corners[0].vy = SAI_SPRITES[k]->corners[1].vy = ys[i * 2];
            SAI_SPRITES[k]->corners[2].vy = SAI_SPRITES[k]->corners[3].vy = ys[i * 2 + 1];
        }
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            k = i * 3 + j + 15;
            SAI_SPRITES[k]->corners[0].vx = SAI_SPRITES[k]->corners[2].vx = xs[i * 3];
            SAI_SPRITES[k]->corners[1].vx = SAI_SPRITES[k]->corners[3].vx = xs[i * 3 + 1];
            SAI_SPRITES[k]->corners[0].vy = SAI_SPRITES[k]->corners[1].vy = ys[j];
            SAI_SPRITES[k]->corners[2].vy = SAI_SPRITES[k]->corners[3].vy = ys[j + 1];
        }
    }
}

const char D_801DE490[] = "";

void SAI_drawPanelFrame(void) {
    s32 i;
    s32 j;

    for (i = 10, j = 1; i < 20; i++, j++) {
        SAI_drawSprite(SAI_SPRITES[j]);
    }
}

void SAI_drawPanelFrameShadow(void) {
    s32 i;
    s32 j;

    for (i = 0x30, j = 11; i < 0x3A; i++, j++) {
        SAI_drawSprite(SAI_SPRITES[j]);
    }
}

void SAI_freePanelFrame(void) {
    s32 i;
    s32 j;

    for (i = 10, j = 1; i < 20; i++, j++) {
        SAI_freeSprite(SAI_SPRITES[j]);
    }
}

void SAI_freePanelFrameShadow(void) {
    s32 i;
    s32 j;

    for (i = 0x30, j = 11; i < 0x3A; i++, j++) {
        SAI_freeSprite(SAI_SPRITES[j]);
    }
}

s32 SAI_spinPanel(void) {
    s32 done = 0;
    s32 i;
    s32 y;

    if (SAI_AREA.opening == 1) {
        if (SAI_AREA.angle == 0x400 && SAI_AREA.openTimer == 35) {
            done = 1;
            SAI_AREA.opening = 0;
        }
        SAI_AREA.openTimer++;
        if (SAI_AREA.openTimer > 35) {
            SAI_AREA.openTimer = 35;
        }
        SAI_WORLD_MAP.iconSlide = SAI_AREA.openTimer - 20;
        if (SAI_WORLD_MAP.iconSlide < 0) {
            SAI_WORLD_MAP.iconSlide = 0;
        } else if (SAI_WORLD_MAP.iconSlide > 15) {
            SAI_WORLD_MAP.iconSlide = 15;
        }
    } else {
        SAI_AREA.openTimer--;
        if (SAI_AREA.openTimer < 0) {
            SAI_AREA.openTimer = 0;
            SAI_AREA.opening = 1;
            done = 1;
        }
        SAI_WORLD_MAP.iconSlide = SAI_AREA.openTimer - 20;
        if (SAI_WORLD_MAP.iconSlide < 0) {
            SAI_WORLD_MAP.iconSlide = 0;
        } else if (SAI_WORLD_MAP.iconSlide > 15) {
            SAI_WORLD_MAP.iconSlide = 15;
        }
    }
    SAI_AREA.angle = (SAI_AREA.openTimer << 10) / 35;
    if (SAI_AREA.angle > 0x400) {
        SAI_AREA.angle = 0x400;
    }
    for (i = 1; i < 11; i++) {
        SAI_SPRITES[i]->rot.vz = -(SAI_AREA.openTimer << 12) / 35;
    }
    for (i = 11; i < 21; i++) {
        SAI_SPRITES[i]->rot.vz = (SAI_AREA.openTimer << 12) / 35;
    }
    SAI_AREA.rot.vz = (SAI_AREA.openTimer << 12) / 35;
    SAI_AREA.pos.vz = rcos(SAI_AREA.angle) * 2 / 3;
    y = -(SAI_WORLD_MAP.iconSlide * 14) / 15;
    for (i = 1; i < 21; i++) {
        SAI_SPRITES[i]->pos.vy = y;
        SAI_SPRITES[i]->pos.vz = SAI_AREA.pos.vz;
    }
    SAI_SPRITES[0]->pos.vy = y;
    SAI_SPRITES[21]->pos.vy = (SAI_WORLD_MAP.iconSlide * -92 + (15 - SAI_WORLD_MAP.iconSlide) * -188) / 15;
    SAI_SPRITES[22]->pos.vy = SAI_SPRITES[21]->pos.vy + 1;
    return done;
}

/* Runs the area script and carries out the events it stops on: op 10 starts
   panels and tasks, op 11 sets up choices, opponents, music and exits, op 13
   fades the portraits and gives cards. */
void SAI_runAreaScript(ScriptRunner *runner) {
    s32 result;
    s32 i;
    s32 offset;

    do {
        result = runScriptToNextEvent(runner->script, runner->regs);
        if (result == 1) {
            switch (runner->script->eventOp) {
            case 10:
                switch (runner->script->eventArg) {
                case 0:
                    if (SAI_AREA.openPanel == 0) {
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        SAI_clearOpponents();
                        func_800149B8(0, -1, 0, 0x800, SAI_runTalkPanel, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (SAI_AREA.openPanel == 1) {
                        return;
                    }
                    SAI_AREA.closePanel = 1;
                    do {
                        func_80014C08(1);
                    } while (SAI_AREA.openPanel != 0);
                    SAI_AREA_MODE = AREA_MODE_BUSY;
                    SAI_clearOpponents();
                    func_800149B8(0, -1, 0, 0x800, SAI_runTalkPanel, 0, getCurrentTaskId(), 0, 0);
                    return;
                case 1:
                    offset = runner->script->pc - runner->script->start;
                    SAI_saveScriptFlags();
                    for (i = 0; i < SAI_AREA.choiceCount;) {
                        runner->regs[1] = ++i;
                        SAI_runChoiceScript(runner);
                        runner->script->pc = runner->script->start + offset;
                        SAI_loadScriptFlags();
                    }
                    runner->regs[1] = 0;
                    SAI_AREA_MODE = AREA_MODE_CHOICES;
                    return;
                case 2:
                    if (SAI_AREA.openPanel == 0) {
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        SAI_OPPONENTS.current[0] = 0x80;
                        SAI_OPPONENTS.target[0] = 0x80;
                        SAI_OPPONENTS.current[1] = 0x80;
                        SAI_OPPONENTS.target[1] = 0x80;
                        func_800149B8(0, -1, 0, 0x800, SAI_runOpponentSelectPanel, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (SAI_AREA.openPanel == 2) {
                        return;
                    }
                    SAI_AREA.closePanel = 1;
                    do {
                        func_80014C08(1);
                    } while (SAI_AREA.openPanel != 0);
                    SAI_AREA_MODE = AREA_MODE_BUSY;
                    SAI_OPPONENTS.current[0] = 0x80;
                    SAI_OPPONENTS.target[0] = 0x80;
                    SAI_OPPONENTS.current[1] = 0x80;
                    SAI_OPPONENTS.target[1] = 0x80;
                    func_800149B8(0, -1, 0, 0x800, SAI_runOpponentSelectPanel, 0, getCurrentTaskId(), 0, 0);
                    return;
                case 3:
                    if (SAI_AREA.opponentPicked != 0) {
                        SAI_AREA.opponentPicked = 0;
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        SAI_AREA.selectState = 7;
                        return;
                    }
                    SAI_AREA_MODE = AREA_MODE_SELECT_OPPONENT;
                    return;
                case 4:
                    if (SAI_addTextLine((u8 *)runner->regs[4]) == -1) {
                        SAI_AREA_MODE = AREA_MODE_TEXT_FULL;
                        return;
                    }
                    break;
                case 5:
                    SAI_AREA.mode = AREA_MODE_WAIT_CROSS;
                    SAI_AREA.waitingForCross = 1;
                    return;
                case 6:
                    SAI_clearTextLines(SAI_TEXT_LINES);
                    break;
                case 7:
                    if (SAI_PLAYER_STATS.state == 0) {
                        SAI_unlockArmorsFromFlags(SAI_SCRIPT[0]->regs);
                        SAI_PLAYER_STATS.state = 1;
                        func_800149B8(0, -1, 0, 0x400, SAI_runPlayerData, 0, getCurrentTaskId(), 0, 0);
                        SAI_AREA_MODE = AREA_MODE_BUSY;
                    }
                    return;
                case 9:
                    runner->regs[0] = 0;
                    SAI_EXIT_ACTION = AREA_EXIT_DECK_EDITOR;
                    return;
                case 10:
                    SAI_AREA_MODE = AREA_MODE_BUSY;
                    func_800149B8(0, -1, 0, 0x400, SAI_runPartnerGet, getCurrentTaskId(), 0, 0, 0);
                    return;
                case 11:
                    SAI_PARTNER_COUNT = 0;
                    for (i = 0; i < 4; i++) {
                        SAI_PARTNER_CHOICES[i] = -1;
                    }
                    break;
                case 12:
                    if (SAI_AREA.openPanel == 0) {
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        func_800149B8(0, -1, 0, 0x800, SAI_runOpponentInfoPanel, SAI_AREA.location, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    if (SAI_AREA.openPanel == SAI_AREA.location + 1) {
                        return;
                    }
                    SAI_AREA.closePanel = 1;
                    do {
                        func_80014C08(1);
                    } while (SAI_AREA.openPanel != 0);
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    func_800149B8(0, -1, 0, 0x800, SAI_runOpponentInfoPanel, SAI_AREA.location, getCurrentTaskId(), 0, 0);
                    return;
                case 13:
                    SAI_showOpponentInfo();
                    return;
                case 14:
                    break;
                case 15:
                    animateWindowTo(&SAI_MESSAGE_WINDOW, (Rect16 *)-1);
                    playSoundEffect(4);
                    SAI_AREA.mode = AREA_MODE_BUSY;
                    bzero((Scene3D *)SAI_AREA.keyword, 0xD);
                    func_800149B8(0, -1, 0, 0x800, SAI_runWordInput, SAI_AREA.keyword, getCurrentTaskId(), 0, 0);
                    return;
                case 16:
                    runner->regs[1] = ((PlayerProfile *)PLAYER_PROFILES)->battleWins;
                    break;
                case 17:
                    runner->regs[0] = 0;
                    SAI_EXIT_ACTION = AREA_EXIT_EQUIPMENT;
                    return;
                case 18:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk28_10 = 1;
                    break;
                case 19:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk14 = 0;
                    break;
                case 20:
                    func_800149B8(0, -1, 0, 0x400, SAI_runHackOverlay, 0, 0, 0, 0);
                    break;
                case 21:
                    SAI_closeHackOverlay();
                    break;
                case 22:
                    initDialog(SAI_DIALOG, (u8 *)SAI_SCRIPT[0]->regs[9], 0);
                    runDialog(SAI_DIALOG);
                    break;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 11:
                switch (runner->script->eventArg) {
                case 0:
                    SAI_openChoiceMenu((s16)runner->script->params[0] == 0x78);
                    break;
                case 1:
                    SAI_addChoice(runner->script->params[0]);
                    break;
                case 2:
                    SESSION->resumeMode = 1;
                    SAI_AREA.exitArg = (s16)runner->script->params[0];
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_DUEL;
                    SESSION->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
                    return;
                case 3:
                    SAI_addOpponent((s16)runner->script->params[0]);
                    break;
                case 4:
                    func_801E09F4();
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_FUSION;
                    SAI_AREA.exitArg = (s16)runner->script->params[0];
                    return;
                case 6:
                    SAI_saveScriptFlags();
                    ((ProfileSave *)PLAYER_PROFILES)->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
                    ((PlayerProfile *)PLAYER_PROFILES)->unkF = runner->script->params[0];
                    runner->regs[0] = 0;
                    SAI_EXIT_ACTION = AREA_EXIT_SAVE;
                    return;
                case 7:
                    SESSION->unk1A7 = runner->script->params[0];
                    break;
                case 8:
                    SAI_LOCATION = SESSION->location = runner->script->params[0];
                    break;
                case 9:
                    SAI_AREA.prizePack = runner->script->params[0];
                    if (SAI_AREA.rewardBusy == 0) {
                        SAI_AREA.rewardBusy = 1;
                        SAI_AREA.mode = AREA_MODE_BUSY;
                        func_800149B8(0, -1, 0, 0x400, SAI_runRewardTask, 0, getCurrentTaskId(), 0, 0);
                        return;
                    }
                    break;
                case 10:
                    SAI_AREA.partners[SAI_AREA.partnerCount] = runner->script->params[0];
                    SAI_AREA.partnerCount++;
                    break;
                case 11:
                case 12:
                    break;
                case 13:
                    playSoundEffect((s16)runner->script->params[0]);
                    break;
                case 14:
                    func_80014C08((s16)runner->script->params[0]);
                    break;
                case 15:
                    do {
                        func_80014C08(1);
                    } while (isMusicIdle() != 1);
                    if (SAI_SCRIPT[0]->regs[0xB8] == 0 && (s16)runner->script->params[0] != 0x6F) {
                        SESSION->music = (s16)runner->script->params[0];
                        playMusic(0, (s16)runner->script->params[0], 100);
                    }
                    break;
                case 16:
                    SAI_AREA_MODE = AREA_MODE_BUSY;
                    func_800149B8(0, -1, 0, 0x400, SAI_grantDigiPart, (s16)runner->script->params[0], getCurrentTaskId(), 0, 0);
                    return;
                case 17:
                    ((SessionData *)D_8006E054)->npcDeckIndex[0] = runner->script->params[0];
                    break;
                case 18:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk14 += runner->script->params[0];
                    break;
                case 19:
                    SESSION->resumeMode = 1;
                    SAI_AREA.exitArg = (s16)runner->script->params[0];
                    runner->regs[0] = 0;
                    SAI_AREA.exitAction = AREA_EXIT_TITLE_OR_ENDING;
                    SESSION->scriptOffset = SAI_SCRIPT[0]->script->pc - SAI_SCRIPT[0]->script->start;
                    return;
                case 20:
                    SAI_AREA_MODE = AREA_MODE_BUSY;
                    func_800149B8(0, -1, 0, 0x400, SAI_runHackingEvent, (s16)runner->script->params[0], getCurrentTaskId(), 0, 0);
                    return;
                case 21:
                    for (i = 0; i < 3; i++) {
                        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse == 0) {
                            ((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] = 0;
                        } else if (countDeckCardsByFilter(0, &((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i], (s16)runner->script->params[0]) != 0) {
                            ((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] = 0;
                        } else {
                            ((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] = 1;
                        }
                    }
                    if (((SessionData *)D_8006E054)->unk1010[0x13] == 1 || ((SessionData *)D_8006E054)->unk1010[0x14] == 1 ||
                        ((SessionData *)D_8006E054)->unk1010[0x15] == 1) {
                        ((SessionData *)D_8006E054)->unk1010[0x12] = 1;
                        SAI_SCRIPT[0]->regs[1] = 0;
                    } else {
                        ((SessionData *)D_8006E054)->unk1010[0x12] = 0;
                        SAI_SCRIPT[0]->regs[1] = 1;
                    }
                    break;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 13:
                switch (runner->script->eventArg) {
                case 0:
                    if ((s16)runner->script->params[0] == 0) {
                        SAI_OPPONENTS.target[0] = runner->script->params[1];
                        SAI_OPPONENTS.step[0] = runner->script->params[2];
                    } else {
                        SAI_OPPONENTS.target[1] = runner->script->params[1];
                        SAI_OPPONENTS.step[1] = runner->script->params[2];
                    }
                    break;
                case 1:
                    SAI_AREA_MODE = AREA_MODE_BUSY;
                    SAI_SCRIPT_REWARD_CARDS[0] = runner->script->params[0];
                    SAI_SCRIPT_REWARD_CARDS[1] = runner->script->params[1];
                    SAI_SCRIPT_REWARD_CARDS[2] = runner->script->params[2];
                    func_800149B8(0, -1, 0, 0x400, SAI_runRewardTask, 1, getCurrentTaskId(), 0, 0);
                    return;
                default:
                    runner->regs[0] = 0;
                    return;
                }
                break;
            case 12:
            default:
                runner->regs[0] = 0;
                return;
            }
        }
        clearScriptBusy(runner->script);
    } while (result != 0);
}

s32 SAI_stepAreaScript(ScriptRunner *runner) {
    *runner->regs = 1;
    SAI_runAreaScript(runner);
    return *runner->regs;
}

Script *SAI_createScriptContext(u8 *scriptData) {
    Script *script = allocHeapBlock(sizeof(Script), 0x31);
    u8 *codeStart;

    script->base = scriptData;
    codeStart = scriptData + 0x10;
    script->start = codeStart;
    script->pc = codeStart;
    script->offset = 0;
    script->size = *(u32 *)(scriptData + 8);
    clearScriptBusy(script);
    return script;
}

s32 *SAI_allocScriptRegisters(s32 count) {
    s32 *block = allocHeapBlock(count * 4, 0x31);
    s32 *p = block;
    s32 i;

    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return block;
}

ScriptRunner *SAI_createAreaScript(void) {
    u8 unused[0x18];
    ScriptRunner *obj = allocHeapBlock(sizeof(ScriptRunner), 0x31);

    obj->unk0 = *(s32 *)&((SessionData *)D_8006E054)->unk100C->unk0[0x190];
    obj->script = SAI_createScriptContext((u8 *)obj->unk0);
    return obj;
}

void SAI_loadAreaTextures(void) {
    char path[0x18];
    u32 *pack;

    if (((PlayerProfile *)PLAYER_PROFILES)->unkE < 10) {
        sprintf(path, "C:\\DEBUG\\area0%d.TIS", ((PlayerProfile *)PLAYER_PROFILES)->unkE);
    } else {
        sprintf(path, "C:\\DEBUG\\area%d.TIS", ((PlayerProfile *)PLAYER_PROFILES)->unkE);
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    sprintf(path, "C:\\OBJECT\\world.TIS");
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
}

void SAI_tickArea(void) {
    char buf[0x20];

    switch (SAI_AREA_MODE) {
    case AREA_MODE_SCRIPT:
        SAI_stepAreaScript(SAI_SCRIPT[0]);
        break;
    case AREA_MODE_CHOICES:
        SAI_tickChoiceMenu();
        break;
    case AREA_MODE_SELECT_OPPONENT:
        SAI_tickOpponentSelectInput();
        break;
    case AREA_MODE_TEXT_FULL:
    case AREA_MODE_WAIT_CROSS:
        if (SAI_AREA.typing == 0 && (PAD_STATES[0]->pressed & PAD_CROSS)) {
            playSoundEffect(0);
            SAI_clearTextLines(SAI_TEXT_LINES);
            if (SAI_AREA.mode == AREA_MODE_TEXT_FULL) {
                SAI_addTextLine((u8 *)SAI_SCRIPT[0]->regs[4]);
            }
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.waitingForCross = 0;
        }
        break;
    case AREA_MODE_BUSY:
        break;
    default:
        sprintf(buf, "(No = %d)", SAI_AREA_MODE);
        break;
        /* unreachable, but it leaves the "" the ROM has after the format */
        printf("");
    }
}

void SAI_reopenPlayerData(void) {
    SESSION->resumeMode = 0;
    SAI_SCRIPT[0]->script->pc = SAI_SCRIPT[0]->script->start + SESSION->scriptOffset;
    playMusic(0, SESSION->music, 100);
    SAI_AREA_MODE = AREA_MODE_BUSY;
    SAI_clearOpponents();
    func_800149B8(0, -1, 0, 0x800, SAI_runTalkPanel, 0, getCurrentTaskId(), 0, 0);
    do {
        func_80014C08(1);
    } while (SAI_AREA.mode == AREA_MODE_BUSY);
    SAI_AREA.location = 0;
    SAI_unlockArmorsFromFlags(SAI_SCRIPT[0]->regs);
    SAI_PLAYER_STATS_STATE = 1;
    func_800149B8(0, -1, 0, 0x400, SAI_runPlayerData, 0, getCurrentTaskId(), 0, 0);
    SAI_AREA.mode = AREA_MODE_BUSY;
}

/* The hacking glitch: moves blocks of VRAM around, with a random pause and a
   sound between steps when animate is 1. */
void SAI_glitchVram(s8 animate) {
    Rect16 rect = { 0x220, 0xE1, 0x20, 1 };
    Rect16 unused[3];
    s32 delay;
    s8 i;

    SAI_MESSAGE_WINDOW_PALETTE = 2;
    for (i = 0; i < 6; i++) {
        delay = abs(rand() % 30);
        if (animate == 1) {
            while (delay > 0) {
                func_80014C08(1);
                delay--;
            }
            playSoundEffect(0x18);
        }
        if (i == 5) {
            MoveImage2(&SAI_GLITCH_VRAM_RECTS[i], 0x280, 0x1F8);
        } else {
            MoveImage2(&SAI_GLITCH_VRAM_RECTS[i], SAI_GLITCH_VRAM_RECTS[i].x - 0x20, SAI_GLITCH_VRAM_RECTS[i].y);
        }
    }
    delay = abs(rand() % 30);
    if (animate == 1) {
        while (delay > 0) {
            func_80014C08(1);
            delay--;
        }
        playSoundEffect(0x18);
    }
    for (i = 0; i < 6; i++) {
        MoveImage2(&SAI_GLITCH_VRAM_RECTS[4], SAI_GLITCH_VRAM_RECTS[4].x - 0x20, i + 0xEC);
    }
    delay = abs(rand() % 30);
    if (animate == 1) {
        while (delay > 0) {
            func_80014C08(1);
            delay--;
        }
        playSoundEffect(0x18);
    }
    MoveImage2(&rect, 0x380, 0x80);
}

const char D_801DE5D0[] = "";

/*
 * The area task: loads the area's pak (unless resuming), runs its script with
 * the message window and panels until the script stops, then leaves for what
 * exitAction says. resumeMode brings it back into a panel after a duel or the
 * complete stats.
 */
void SAI_runArea(s32 resume) {
    s32 timer = 0;

    if (resume == 0) {
        func_800149B8(0, -1, 0, 0x400, SAI_loadAreaPak, 0, getCurrentTaskId, 0, 0);
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
    }
    loadSoundEffectBank(1);
    SAI_initCamera();
    SAI_AREA.waitingForCross = 0;
    SAI_SCRIPT[0] = SAI_createAreaScript();
    SAI_SCRIPT[0]->regs = SAI_allocScriptRegisters(0x174);
    SAI_loadScriptFlags();
    SAI_SCRIPT[0]->regs[0] = 1;
    ((SessionData *)D_8006E054)->unk1010[0x13] = ((SessionData *)D_8006E054)->unk1010[0x14] = ((SessionData *)D_8006E054)->unk1010[0x15] = 0;
    ((SessionData *)D_8006E054)->unk1010[0x12] = 0;
    SAI_clearTextLines(SAI_TEXT_LINES);
    SAI_toggleMessageWindow(0);
    SAI_AREA.choiceCount = 0;
    SAI_AREA.openPanel = 0;
    SAI_AREA.exitAction = AREA_EXIT_MAP;
    SAI_AREA.closePanel = 0;
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    ((SessionData *)D_8006E054)->npcDeckIndex[0] = -1;
    SAI_createAreaName();
    SAI_createLocationLabel();
    addFrameCallback((s32)SAI_drawAreaHud);
    if (SAI_ICON_RUNNING != 1) {
        if (SAI_SCRIPT[0]->regs[0xB8] != 0) {
            if (SESSION->resumeMode == 1) {
                func_800149B8(0, -1, 0, 0x400, SAI_runCornerIcon, 4, getCurrentTaskId(), 0, 0);
            } else {
                func_800149B8(0, -1, 0, 0x400, SAI_runCornerIcon, 3, getCurrentTaskId(), 0, 0);
            }
        } else {
            func_800149B8(0, -1, 0, 0x400, SAI_runCornerIcon, 2, getCurrentTaskId(), 0, 0);
        }
    }
    SAI_ICON_MOTION = 2;
    if (SESSION->resumeMode == 1) {
        SAI_AREA_MODE = AREA_MODE_BUSY;
        if ((s8)SESSION->location == 1) {
            func_800149B8(0, -1, 0, 0x800, SAI_runOpponentSelectPanel, 0, getCurrentTaskId(), 0, 0);
        } else {
            func_800149B8(0, -1, 0, 0x800, SAI_runOpponentInfoPanel, 0, getCurrentTaskId(), 0, 0);
        }
    } else if (SESSION->resumeMode == 2) {
        SAI_reopenPlayerData();
    }
    if (SAI_SCRIPT[0]->regs[0xB8] != 0) {
        SAI_glitchVram(0);
        setBackgroundScrollMode(1);
    }
    do {
        func_80014C08(1);
        timer++;
        SAI_tickArea();
        if (SAI_SCRIPT[0]->regs[0xB8] != 0 && timer >= 150) {
            timer -= 150;
            SAI_flickerHackOverlay();
            playSoundEffect(0x1A);
        }
    } while (SAI_SCRIPT[0]->regs[0] != 0);
    playSoundEffect(4);
    animateWindowTo(&SAI_MESSAGE_WINDOW, (Rect16 *)-1);
    SAI_CLOSE_PANEL = 1;
    do {
        func_80014C08(1);
    } while (SAI_AREA.openPanel != 0);
    removeFrameCallback((s32)SAI_drawAreaHud);
    SAI_saveScriptFlags();
    SAI_unlockArmorsFromFlags(SAI_SCRIPT[0]->regs);
    if (SAI_SCRIPT[0]->regs[0xB8] != 0 && (SAI_EXIT_ACTION == AREA_EXIT_EQUIPMENT || SAI_EXIT_ACTION == AREA_EXIT_DECK_EDITOR)) {
        fadeOutScrollingBackground();
        do {
            func_80014C08(1);
        } while (SCROLL_BACKGROUND.shownImage != -1);
        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    }
    func_80014C08(1);
    func_80014A00(0x19);
    freeHeapBlocksByTag(0x7F);
    freeHeapBlocksByTag(0x2E);
    freeHeapBlocksByTag(0x31);
    func_80014C08(1);
    switch (SAI_EXIT_ACTION) {
    default:
        func_800149B8(0, -1, 0, 0x400, SAI_loadMapTextures, 1, getCurrentTaskId, 0, 0);
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
        playMusic(0, 0x6F, 0x64);
        func_800149B8(0, -1, 0, 0x400, SAI_runWorldMap, 1, 0, getCurrentTaskId(), 0);
        break;
    case AREA_EXIT_DUEL:
        SAI_ICON_RUNNING = 0;
        fadeOutScrollingBackground();
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x200, startCpuDuel, SAI_EXIT_ARG, 0, 0, 0);
        break;
    case AREA_EXIT_DECK_EDITOR:
        SAI_ICON_RUNNING = 0;
        func_80014C08(1);
        openDeckEditor(1);
        break;
    case AREA_EXIT_SAVE:
        SAI_ICON_RUNNING = 0;
        func_80014C08(1);
        func_800149B8(0, -1, 0, 0x400, openSaveScreenFromMap, 4, getCurrentTaskId(), 0, 0);
        break;
    case AREA_EXIT_FUSION:
        SAI_ICON_RUNNING = 0;
        func_80014C08(1);
        if (SAI_AREA.exitArg >= 3) {
            SAI_AREA.exitArg = 0;
        }
        func_800149B8(0, -1, 0, 0x400, openPartnerFusion, SAI_EXIT_ARG, 0, 0, 0);
        break;
    case AREA_EXIT_EQUIPMENT:
        SAI_ICON_RUNNING = 0;
        func_80014C08(1);
        openPartnerEquipment(1);
        break;
    case AREA_EXIT_TITLE_OR_ENDING:
        SAI_ICON_RUNNING = 0;
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x800, quitToTitleOrPlayEnding, SAI_EXIT_ARG, 0, 0, 0);
        break;
    }
}

void SAI_drawAreaHud(void) {
    drawWindow(&SAI_MESSAGE_WINDOW, SAI_drawMessageWindow, 0x17);
    SAI_drawChoiceMenu();
    SAI_drawAreaName();
    SAI_drawLocationLabel();
}

void SAI_createPanel(void) {
    SAI_initPanelCover();
    SAI_createPanelFrame();
    SAI_createPanelFrameShadow();
    addFrameCallback((s32)SAI_drawPanel);
}

void SAI_drawPanel(void) {
    if (SAI_PANEL_COVER_ALPHA != 0) {
        SAI_drawPanelCover();
    }
    SAI_drawPanelFrame();
    SAI_drawPanelFrameShadow();
    if (SAI_PANEL_IMAGE_HIDDEN != -1) {
        SAI_drawSprite(SAI_SPRITES[0]);
    }
}

void SAI_freePanel(void) {
    removeFrameCallback(SAI_drawPanel);
    func_80014C08(1);
    SAI_freeSprite(SAI_SPRITES[0]);
    SAI_freePanelFrame();
    SAI_freePanelFrameShadow();
}

void SAI_runTalkPanel(void) {
    SAI_AREA.openPanel = 1;
    SAI_AREA.imageHidden = -1;
    SAI_AREA.opening = 1;
    SAI_AREA.closePanel = 0;
    SAI_createPanel();
    SAI_SPRITES[0] = SAI_createSprite(3);
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x24);
    SAI_spinPanel();
    playSoundEffect(10);
    do {
        func_80014C08(1);
    } while (SAI_spinPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = 0;
    SAI_toggleMessageWindow(1);
    do {
        func_80014C08(1);
    } while (SAI_uncoverPanel() == 0);
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
    do {
        func_80014C08(1);
    } while (SAI_AREA.closePanel == 0);
    do {
        func_80014C08(1);
    } while (SAI_coverPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = -1;
    playSoundEffect(11);
    do {
        func_80014C08(1);
    } while (SAI_spinPanel() == 0);
    SAI_freePanel();
    SAI_OPEN_PANEL = 0;
}

void SAI_stepBrightness(s8 index) {
    s16 value = SAI_OPPONENTS.current[index];

    if (SAI_OPPONENTS.current[index] < SAI_OPPONENTS.target[index]) {
        value += SAI_OPPONENTS.step[index];
        if (SAI_OPPONENTS.target[index] < SAI_OPPONENTS.current[index]) {
            value = SAI_OPPONENTS.target[index];
        }
    } else if (SAI_OPPONENTS.target[index] < SAI_OPPONENTS.current[index]) {
        value -= SAI_OPPONENTS.step[index];
        if (SAI_OPPONENTS.current[index] < SAI_OPPONENTS.target[index]) {
            value = SAI_OPPONENTS.target[index];
        }
    }
    SAI_OPPONENTS.current[index] = value;
}

void SAI_tickOpponentSelectInput(void) {
    s8 card;
    s32 i;
    s32 *timers;

    if (PAD_STATES[0]->pressed & (PAD_UP | PAD_DOWN)) {
        SAI_AREA.select.selected += 3;
        if (SAI_AREA.select.selected >= 6) {
            SAI_AREA.select.selected -= 6;
        }
    } else if ((u16)PAD_STATES[0]->pressed & PAD_LEFT) {
        SAI_AREA.select.selected--;
        if (SAI_AREA.select.selected < 0) {
            SAI_AREA.select.selected = 5;
        }
    } else if (PAD_STATES[0]->pressed & PAD_RIGHT) {
        SAI_AREA.select.selected++;
        if (SAI_AREA.select.selected >= 6) {
            SAI_AREA.select.selected = 0;
        }
    } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
        playSoundEffect(1);
        SAI_AREA.mode = AREA_MODE_SCRIPT;
        SAI_AREA.select.unk5E = 0;
        SAI_SCRIPT[0]->regs[2] = -1;
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        card = SAI_OPPONENTS.ids[SAI_AREA.select.page * 6 + SAI_AREA.select.selected];
        if (SAI_SCRIPT[0]->regs[SAI_AREA.select.selected + 268] == 0) {
            if (card == 16 || card == 17) {
                SAI_AREA.mode = AREA_MODE_BUSY;
                SAI_AREA.selectState = 20;
                if (card == 16) {
                    SAI_AREA.select.page--;
                } else {
                    SAI_AREA.select.page++;
                }
                i = 0;
                timers = SAI_SELECT_BLINK;
            loop:
                timers[i] = rand() % 30 + 30;
                i++;
                if (i < 6) {
                    goto loop;
                }
                playSoundEffect(0);
            } else if (card >= 0 && card < 16) {
                SAI_AREA.select.zoom = 0;
                SAI_AREA.selectState = 6;
                SAI_AREA.mode = AREA_MODE_BUSY;
                SAI_SCRIPT[0]->regs[2] = card;
                SAI_AREA.opponentPicked = 1;
                playSoundEffect(0);
            }
        }
    }
    SAI_SPRITES[43]->pos.vx = SAI_SPRITES[SAI_AREA.select.selected + 31]->pos.vx;
    SAI_SPRITES[43]->pos.vy = SAI_SPRITES[SAI_AREA.select.selected + 31]->pos.vy;
    SAI_drawSprite(SAI_SPRITES[43]);
}

void SAI_randomizeOpponentDelays(void) {
    s32 i = 0;
    s16 *values = SAI_SELECT_DELAYS;

loop:
    values[i] = rand() % 30;
    i++;
    if (i < 6) {
        goto loop;
    }
}

void SAI_initOpponentSelect(void) {
    Rect16 rect;
    AreaState *ctx;
    s32 i;

    for (i = 0; i < 6; i++) {
        SAI_SPRITES[i + 31] = SAI_createSprite(8);
        SAI_setSpriteDepth(SAI_SPRITES[i + 31], 0x21);
        SAI_setSpriteSize(SAI_SPRITES[i + 31], 0x18, 0x16);
        SAI_SPRITES[i + 31]->pos.vx = 0x86;
        SAI_SPRITES[i + 31]->pos.vy = -0x57;
        SAI_setSpriteBlendMode(SAI_SPRITES[i + 31], 1);
    }
    for (i = 0, ctx = &SAI_AREA; i < 6; i++) {
        ctx->select.delay[i] = rand() % 30;
        ctx->select.progress[i] = 0;
        ctx->select.emptyBlink[i] = 0;
        SAI_SELECT_BLINK[i] = rand() % 30 + 50;
    }
    for (i = 0; i < 6; i++) {
        SAI_SPRITES[i + 37] = SAI_createSprite(0x3A);
        SAI_setSpriteDepth(SAI_SPRITES[i + 37], 0x21);
        SAI_setSpriteSize(SAI_SPRITES[i + 37], 0x12, 0x10);
        SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
        SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
    }
    SAI_SPRITES[43] = SAI_createSprite(0x3C);
    SAI_setSpriteDepth(SAI_SPRITES[43], 0x20);
    SAI_setSpriteBlendMode(SAI_SPRITES[43], 0);
    SAI_SPRITES[44] = SAI_createSprite(8);
    SAI_setSpriteDepth(SAI_SPRITES[44], 0x21);
    SAI_setSpriteBlendMode(SAI_SPRITES[44], 1);
    SAI_SPRITES[45] = SAI_createSprite(0x3B);
    SAI_setSpriteDepth(SAI_SPRITES[45], 0x21);
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx = -0xC8;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy = 0x15;
    if (SESSION->unk1A7 == 0) {
        rect.x = 0x200;
        rect.y = 0x100;
        rect.w = 0xFF;
        rect.h = 0x80;
    } else {
        rect.x = 0x180;
        rect.y = 0x180;
        rect.w = 0xFF;
        rect.h = 0x7F;
    }
    if (SESSION->resumeMode == 1) {
        SAI_SCRIPT[0]->script->pc = SAI_SCRIPT[0]->script->start + SESSION->scriptOffset;
        SAI_OPPONENTS = SESSION->opponents;
        SAI_AREA.select = SESSION->select;
        SAI_SCRIPT[0]->regs[1] = (s8)(SESSION->duelResult ^ 1);
        SAI_AREA.location = SESSION->location;
        SESSION->resumeMode = 0;
        SAI_AREA.selectState = 9;
        SAI_AREA.select.timer = 20;
        SAI_AREA.opponentPicked = 1;
    } else {
        SAI_AREA.select.page = 0;
        SAI_AREA.select.selected = 0;
        SAI_AREA.select.unk5E = 0;
        SAI_AREA.select.timer = 0;
        SAI_AREA.select.zoom = 0;
        SAI_AREA.opponentPicked = 0;
        SAI_AREA.selectState = 4;
    }
    if ((u8)SAI_AREA.select.page >= 3) {
        SAI_AREA.select.page = 0;
    }
}

void SAI_freeOpponentSelect(void) {
    s32 i;

    for (i = 0x1F; i < 0x2E; i++) {
        SAI_freeSprite(SAI_SPRITES[i]);
    }
    ((SaveBlock *)((SessionData *)D_8006E054)->unk100C)->opponents = SAI_OPPONENTS;
    ((SaveBlock *)((SessionData *)D_8006E054)->unk100C)->select = SAI_AREA.select;
    SAI_clearOpponents();
}

void SAI_updateOpponentPortraits(void) {
    Rect16 rect;
    s32 i;
    s8 *cards;
    s8 page;
    s8 card;
    s16 w;
    s16 h;

    for (i = 0; i < 6; i++) {
        page = SAI_AREA.select.page;
        cards = SAI_OPPONENT_IDS;
        card = cards[page * 6 + i];
        if (card != -1) {
            if (SAI_SELECT_BLINK[i] > 0) {
                if (SAI_SELECT_BLINK[i] & 4) {
                    rect.x = 0x318;
                } else {
                    rect.x = 0x323;
                }
                rect.y = 0xA4;
                rect.w = 0x2C;
                rect.h = 0x24;
                SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEB);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEB);
            } else if (card == 16) {
                rect.x = 0x2EB;
                rect.y = 0x90;
                rect.w = 0x2A;
                rect.h = 0x24;
                SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEE);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEE);
            } else if (card == 17) {
                rect.x = 0x2E0;
                rect.y = 0x90;
                rect.w = 0x2A;
                rect.h = 0x24;
                SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEE);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEE);
            } else {
                rect.x = card % 4 * 32 + 0x280;
                rect.y = card / 4 * 56 + 0x100;
                rect.w = 0x3F;
                rect.h = 0x38;
                SAI_setSpriteImage8Bit(SAI_SPRITES[i + 37], &rect, 0);
                SAI_SELECT_BLINK[i] = 0;
                SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x280, 0x1FB);
                SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x280, 0x1FB);
            }
        }
        w = (SAI_AREA.select.progress[i] * 44 + (20 - SAI_AREA.select.progress[i]) * 18) / 20;
        h = (SAI_AREA.select.progress[i] * 38 + (20 - SAI_AREA.select.progress[i]) * 16) / 20;
        SAI_setSpriteSize(SAI_SPRITES[i + 37], w, h);
        SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
        SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
    }
}


void SAI_zoomOpponentPortrait(void) {
    Rect16 rect;
    s32 i;
    s16 w;
    s16 h;

    if (SAI_AREA.selectState == 6) {
        if (SAI_AREA.select.zoom == 20) {
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
        SAI_AREA.select.zoom++;
        if (SAI_AREA.select.zoom > 20) {
            SAI_AREA.select.zoom = 20;
        }
    } else {
        if (SAI_AREA.select.zoom == 0) {
            SAI_AREA.mode = AREA_MODE_SELECT_OPPONENT;
            SAI_AREA.selectState = 2;
        }
        if (--SAI_AREA.select.zoom < 0) {
            SAI_AREA.select.zoom = 0;
        }
    }
    for (i = 0; i < 6; i++) {
        if (i == SAI_AREA.select.selected) {
            SAI_SPRITES[i + 31]->pos.vx = (SAI_AREA.select.zoom * 108 + ((i % 3) * 55 - 46) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_SPRITES[i + 31]->pos.vy = (SAI_AREA.select.zoom * 21 + ((i / 3) * 50 - 43) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 31], (SAI_AREA.select.zoom * 80 + (20 - SAI_AREA.select.zoom) * 56) / 20, (SAI_AREA.select.zoom * 70 + (20 - SAI_AREA.select.zoom) * 48) / 20);
            w = (SAI_AREA.select.zoom * 64 + (20 - SAI_AREA.select.zoom) * 44) / 20;
            h = (SAI_AREA.select.zoom * 56 + (20 - SAI_AREA.select.zoom) * 38) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 37], w, h);
            SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
            SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
        } else {
            SAI_SPRITES[i + 31]->pos.vx = (SAI_AREA.select.zoom * 134 + ((i % 3) * 55 - 46) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_SPRITES[i + 31]->pos.vy = (SAI_AREA.select.zoom * -87 + ((i / 3) * 50 - 43) * (20 - SAI_AREA.select.zoom)) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 31], (SAI_AREA.select.zoom * 24 + (20 - SAI_AREA.select.zoom) * 56) / 20, (SAI_AREA.select.zoom * 22 + (20 - SAI_AREA.select.zoom) * 48) / 20);
            w = (SAI_AREA.select.zoom * 18 + (20 - SAI_AREA.select.zoom) * 44) / 20;
            h = (SAI_AREA.select.zoom * 16 + (20 - SAI_AREA.select.zoom) * 38) / 20;
            SAI_setSpriteSize(SAI_SPRITES[i + 37], w, h);
            SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
            SAI_SPRITES[i + 37]->pos.vy = SAI_SPRITES[i + 31]->pos.vy - 1;
        }
    }
    SAI_SPRITES[44]->pos.vx = (SAI_AREA.select.zoom * -103 - (20 - SAI_AREA.select.zoom) * 200) / 20;
    SAI_SPRITES[44]->pos.vy = 21;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
}

s8 SAI_slideOpponentPortraits(void) {
    s32 i;
    s8 busy;

    for (i = 0; i < 6; i++) {
        if (SAI_AREA.selectState == 4 || SAI_AREA.selectState == 20) {
            if (SAI_SELECT_DELAYS[i] <= 0) {
                SAI_SELECT_PROGRESS[i]++;
            } else {
                SAI_SELECT_DELAYS[i]--;
                if (SAI_SELECT_DELAYS[i] < 0) {
                    SAI_SELECT_DELAYS[i] = 0;
                }
            }
            if (SAI_SELECT_PROGRESS[i] > 20) {
                SAI_SELECT_PROGRESS[i] = 20;
            }
        } else if (SAI_AREA.selectState == 5) {
            if (SAI_SELECT_DELAYS[i] <= 0) {
                SAI_AREA.select.blink[i] = (SAI_AREA.select.blink[i] + 1) & 0xFFF;
                if (SAI_AREA.select.blink[i] > 20 || SAI_SELECT_PROGRESS[i] != 20) {
                    SAI_SELECT_PROGRESS[i]--;
                }
            } else {
                SAI_SELECT_DELAYS[i]--;
                if (SAI_SELECT_DELAYS[i] < 0) {
                    SAI_SELECT_DELAYS[i] = 0;
                }
            }
            if (SAI_SELECT_PROGRESS[i] < 0) {
                SAI_SELECT_PROGRESS[i] = 0;
            }
        }
        SAI_SPRITES[i + 31]->pos.vx = ((i % 3 * 55 - 46) * SAI_SELECT_PROGRESS[i] + (20 - SAI_SELECT_PROGRESS[i]) * 134) / 20;
        SAI_SPRITES[i + 31]->pos.vy = ((i / 3 * 50 - 43) * SAI_SELECT_PROGRESS[i] + (20 - SAI_SELECT_PROGRESS[i]) * -87) / 20;
        SAI_setSpriteSize(SAI_SPRITES[i + 31], (SAI_SELECT_PROGRESS[i] * 56 + (20 - SAI_SELECT_PROGRESS[i]) * 24) / 20,
                      (SAI_SELECT_PROGRESS[i] * 48 + (20 - SAI_SELECT_PROGRESS[i]) * 22) / 20);
    }
    busy = 0;
    if (SAI_SELECT_STATE == 4 || SAI_SELECT_STATE == 20) {
        for (i = 0; i < 6; i++) {
            if (SAI_SELECT_BLINK[i] > 0) {
                SAI_SELECT_BLINK[i]--;
            }
            if (SAI_SELECT_BLINK[i] < 0) {
                SAI_SELECT_BLINK[i] = 0;
            }
            if (SAI_SELECT_BLINK[i] != 0) {
                busy++;
            }
        }
        if (busy == 0) {
            if (SAI_AREA.selectState == 20) {
                SAI_AREA.selectState = 2;
                SAI_AREA.mode = AREA_MODE_SELECT_OPPONENT;
            } else {
                SAI_AREA.selectState = 2;
                SAI_AREA.mode = AREA_MODE_SCRIPT;
            }
        }
    } else if (SAI_SELECT_STATE == 5) {
        for (i = 0; i < 6; i++) {
            if (SAI_SELECT_PROGRESS[i] != 0) {
                busy++;
            }
        }
        if (busy == 0) {
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
    }
    SAI_updateOpponentPortraits();
    return busy;
}

s32 SAI_slideChosenOpponentBack(void) {
    s32 result = 1;
    s32 i;

    if (SAI_AREA.selectState != 9) {
        SAI_AREA.select.timer++;
        if (SAI_AREA.select.timer > 20) {
            SAI_AREA.select.timer = 20;
            result = 0;
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
    } else {
        SAI_AREA.select.timer--;
        SAI_updateOpponentPortraits();
        for (i = 0; i < 6; i++) {
            if (SAI_AREA.select.selected == i) {
                SAI_setSpriteSize(SAI_SPRITES[i + 31], 0x50, 0x46);
                SAI_setSpriteSize(SAI_SPRITES[i + 37], 0x40, 0x38);
            } else {
                SAI_setSpriteSize(SAI_SPRITES[i + 31], 0x18, 0x16);
                SAI_setSpriteSize(SAI_SPRITES[i + 37], 0x12, 0x10);
            }
        }
        if (SAI_AREA.select.timer < 0) {
            SAI_AREA.select.timer = 0;
            result = 0;
            SAI_AREA.mode = AREA_MODE_SCRIPT;
            SAI_AREA.selectState = 2;
        }
    }
    SAI_SPRITES[44]->pos.vx = (SAI_AREA.select.timer * -200 + (20 - SAI_AREA.select.timer) * -103) / 20;
    SAI_SPRITES[44]->pos.vy = 0x15;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
    for (i = 0; i < 6; i++) {
        if (i == SAI_AREA.select.selected) {
            SAI_SPRITES[i + 31]->pos.vx = (SAI_AREA.select.timer * 210 + (20 - SAI_AREA.select.timer) * 108) / 20;
            SAI_SPRITES[i + 37]->pos.vx = SAI_SPRITES[i + 31]->pos.vx;
            SAI_SPRITES[i + 31]->pos.vy = SAI_SPRITES[i + 37]->pos.vy = 0x15;
            SAI_SPRITES[i + 37]->pos.vy--;
        } else {
            SAI_SPRITES[i + 31]->pos.vx = SAI_SPRITES[i + 37]->pos.vx = 0x86;
            SAI_SPRITES[i + 31]->pos.vy = SAI_SPRITES[i + 37]->pos.vy = -0x57;
            SAI_SPRITES[i + 37]->pos.vy--;
        }
    }
    return result;
}

void func_801E7240(void) {
    Sprite3D *card;
    s16 w;
    s16 h;

    SAI_AREA.select.timer++;
    if (SAI_AREA.select.timer > 30) {
        SAI_AREA.select.timer = 30;
        SAI_AREA.unk116 = 3;
        SAI_AREA.selectState = 3;
        SAI_AREA.mode = AREA_MODE_SCRIPT;
    }
    card = SAI_SPRITES[SAI_AREA.select.selected + 31];
    w = (SAI_AREA.select.timer * 24 + (30 - SAI_AREA.select.timer) * 80) / 30;
    h = (SAI_AREA.select.timer * 22 + (30 - SAI_AREA.select.timer) * 70) / 30;
    SAI_setSpriteSize(SAI_SPRITES[44], w, h);
    SAI_setSpriteSize(card, w, h);
    w = (SAI_AREA.select.timer * 18 + (30 - SAI_AREA.select.timer) * 64) / 30;
    h = (SAI_AREA.select.timer * 16 + (30 - SAI_AREA.select.timer) * 56) / 30;
    SAI_setSpriteSize(SAI_SPRITES[45], w, h);
    SAI_setSpriteSize(card + 6, w, h);
    SAI_SPRITES[44]->pos.vx = (SAI_AREA.select.timer * 134 + (30 - SAI_AREA.select.timer) * -103) / 30;
    SAI_SPRITES[44]->pos.vy = (SAI_AREA.select.timer * -87 + (30 - SAI_AREA.select.timer) * 21) / 30;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
    card->pos.vx = card[6].pos.vx = (SAI_AREA.select.timer * 134 + (30 - SAI_AREA.select.timer) * 108) / 30;
    card->pos.vy = card[6].pos.vy = (SAI_AREA.select.timer * -87 + (30 - SAI_AREA.select.timer) * 21) / 30;
}

void SAI_drawOpponentSelect(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 2; i++) {
        SAI_stepBrightness(i);
    }
    if (SAI_AREA_MODE == AREA_MODE_SELECT_OPPONENT) {
        SAI_OPPONENTS.target[0] = SAI_OPPONENTS.target[1] = 0x80;
    }
    SAI_setSpriteBrightness(SAI_SPRITES[45], SAI_OPPONENTS.current[0]);
    SAI_setSpriteBrightness(SAI_SPRITES[SAI_SELECT_SELECTED + 37], SAI_OPPONENTS.current[1]);
    SAI_drawSprite(SAI_SPRITES[44]);
    SAI_drawSprite(SAI_SPRITES[45]);
    for (i = 0; i < 6; i++) {
        if ((SAI_OPPONENT_IDS + SAI_AREA.select.page * 6)[i] == -1) {
            SAI_SELECT_EMPTY_BLINK[i]++;
            if (SAI_SELECT_EMPTY_BLINK[i] & 4) {
                rect.x = 0x318;
            } else {
                rect.x = 0x323;
            }
            rect.y = 0xA4;
            rect.w = 0x2C;
            rect.h = 0x24;
            SAI_setSpriteImage4Bit(SAI_SPRITES[i + 37], &rect);
            SAI_SPRITES[i + 37]->quads[0].clut = getClut(0x200, 0xEB);
            SAI_SPRITES[i + 37]->quads[1].clut = getClut(0x200, 0xEB);
        } else {
            if (SAI_SCRIPT[0]->regs[i + 268] != 0) {
                SAI_setSpriteBrightness(SAI_SPRITES[i + 37], 0x30);
            }
            SAI_SELECT_EMPTY_BLINK[i] = 0;
        }
        SAI_drawSprite(SAI_SPRITES[i + 31]);
        SAI_drawSprite(SAI_SPRITES[i + 37]);
    }
}

void SAI_runOpponentSelectPanel(void) {
    SAI_AREA.openPanel = 2;
    SAI_AREA.closePanel = 0;
    SAI_AREA.imageHidden = -1;
    SAI_AREA.opening = 1;
    SAI_createPanel();
    SAI_SPRITES[0] = SAI_createSprite(5);
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x24);
    SAI_initOpponentSelect();
    SAI_spinPanel();
    playSoundEffect(10);
    do {
        func_80014C08(1);
    } while (SAI_spinPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = 0;
    SAI_toggleMessageWindow(1);
    addFrameCallback((s32)SAI_drawOpponentSelect);
    do {
        func_80014C08(1);
    } while (SAI_uncoverPanel() == 0);
    do {
        func_80014C08(1);
        switch (SAI_SELECT_STATE) {
        case 2:
            break;
        case 4:
        case 5:
        case 20:
            SAI_slideOpponentPortraits();
            break;
        case 6:
        case 7:
            SAI_zoomOpponentPortrait();
            break;
        case 9:
            SAI_slideChosenOpponentBack();
            break;
        }
    } while (SAI_CLOSE_PANEL == 0);
    SAI_SELECT_STATE = 5;
    if (SESSION->resumeMode == 1) {
        do {
            func_80014C08(1);
        } while (SAI_slideChosenOpponentBack() != 0);
    } else {
        SAI_randomizeOpponentDelays();
        do {
            func_80014C08(1);
        } while (SAI_slideOpponentPortraits() != 0);
    }
    removeFrameCallback((s32)SAI_drawOpponentSelect);
    do {
        func_80014C08(1);
    } while (SAI_coverPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = -1;
    playSoundEffect(11);
    do {
        func_80014C08(1);
    } while (SAI_spinPanel() == 0);
    SAI_freeOpponentSelect();
    SAI_freePanel();
    SAI_OPEN_PANEL = 0;
}

void SAI_drawOpponentInfo(void) {
    s32 i;

    if (SAI_OPPONENT_INFO->showWipe == 1) {
        SAI_drawOpponentStats();
        SAI_OPPONENT_INFO->fades[0].polys[FRAME_BUFFER_INDEX].x0 = SAI_OPPONENT_INFO->fades[0].polys[FRAME_BUFFER_INDEX].x2 = SAI_OPPONENT_INFO->wipeX;
        addPrim(&CURRENT_FRAME_BUFFER->ot[35], &SAI_OPPONENT_INFO->fades[0].polys[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[35], &SAI_OPPONENT_INFO->fades[0].tpages[FRAME_BUFFER_INDEX]);
    }
    if (SAI_OPPONENT_INFO->phase < 2) {
        setRGB0(&SAI_OPPONENT_INFO->fades[1].polys[FRAME_BUFFER_INDEX], SAI_OPPONENT_INFO->alpha, SAI_OPPONENT_INFO->alpha, SAI_OPPONENT_INFO->alpha);
        addPrim(&CURRENT_FRAME_BUFFER->ot[32], &SAI_OPPONENT_INFO->fades[1].polys[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[32], &SAI_OPPONENT_INFO->fades[1].tpages[FRAME_BUFFER_INDEX]);
    }
    for (i = 0; i < 2; i++) {
        SAI_stepBrightness(i);
    }
    SAI_setSpriteBrightness(SAI_SPRITES[43], SAI_OPPONENTS.current[1]);
    SAI_setSpriteBrightness(SAI_SPRITES[45], SAI_OPPONENTS.current[0]);
    for (i = 0; i < 4; i++) {
        SAI_drawSprite(SAI_SPRITES[i + 42]);
    }
}

void SAI_drawOpponentStats(void) {
    Rect16 uv;
    char *labels[4] = { "Deck Color", "Attack", "Defense", "Digivolve Speed" };
    char buf[0x19];
    s32 x;
    s32 i;
    s32 j;

    x = (SAI_OPPONENT_INFO->entries[0].progress * 47 + (20 - SAI_OPPONENT_INFO->entries[0].progress) * 330) / 20;
    drawText(x + 0x56, 0x4B, SAI_SCRIPT[0]->regs[9], 7, 0x22);
    uv.x = 0x60;
    uv.y = SAI_SCRIPT[0]->regs[11] * 18;
    uv.w = 0x20;
    uv.h = 0x12;
    drawTexturedSprite(x, 0x45, &uv, 0x2B, 0x3A61, 0x22, SAI_OPPONENT_INFO->brightness, 0);
    uv.x = 0xC4;
    uv.y = 0;
    uv.w = 0x34;
    uv.h = 0x12;
    drawTexturedSprite(x + 0x20, 0x45, &uv, 0x2A, 0x3AA1, 0x22, SAI_OPPONENT_INFO->brightness, 0);
    x = (SAI_OPPONENT_INFO->entries[1].progress * 42 + (20 - SAI_OPPONENT_INFO->entries[1].progress) * 330) / 20;
    drawLargeText(x, 0x58, (s32)"ABILITIES", 7, 0x22);
    for (i = 0; i < 4; i++) {
        x = (SAI_OPPONENT_INFO->entries[i + 2].progress * 54 + (20 - SAI_OPPONENT_INFO->entries[i + 2].progress) * 330) / 20;
        drawText(x, i * 12 + 0x64, (s32)labels[i], 7, 0x22);
        bzero((Scene3D *)buf, 0x19);
        switch (i) {
        case 0:
            for (j = 0; j < 5; j++) {
                if (SAI_SCRIPT[0]->regs[5] & (0x10 >> j)) {
                    s32 dx;

                    uv.x = (j + 0x13) * 4;
                    uv.y = 0;
                    uv.w = 4;
                    uv.h = 10;
                    dx = j * 5 + 0x64;
                    drawTexturedSprite(x + dx, i * 12 + 0x64, &uv, 0xA, 0x3A21, 0x22, 0x80, -1);
                }
            }
            break;
        case 1:
            sprintf(buf, "%d", SAI_SCRIPT[0]->regs[6]);
            break;
        case 2:
            sprintf(buf, "%d", SAI_SCRIPT[0]->regs[7]);
            break;
        case 3:
            sprintf(buf, "%d", SAI_SCRIPT[0]->regs[8]);
            break;
        }
        if (i != 0) {
            drawText(x + 0x64, i * 12 + 0x64, (s32)buf, 7, 0x22);
        }
    }
}

void SAI_freeOpponentInfo(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        SAI_freeSprite(SAI_SPRITES[i + 42]);
    }
    *(OpponentList *)((SessionData *)D_8006E054)->unk100C = SAI_OPPONENTS;
}

void SAI_initOpponentInfo(void) {
    Fade *fade = SAI_OPPONENT_INFO->fades;
    s32 k;
    s32 i;

    for (k = 0; k < 2; k++, fade++) {
        for (i = 0; i < 2; i++) {
            func_80067784(&fade->polys[i]);
            SetSemiTrans(&fade->polys[i], 1);
            if (k == 0) {
                SetDrawTPage(&fade->tpages[i], 0, 0, 0x40);
                fade->polys[i].r0 = fade->polys[i].g0 = fade->polys[i].b0 = 0x60;
                setPrimQuadRect(&fade->polys[i], 0x140, 0x42, 1, 0x5A);
            } else {
                setPrimQuadRect(&fade->polys[i], 0, 0x42, 0x140, 0x5A);
                SetDrawTPage(&fade->tpages[i], 0, 0, 0x20);
                fade->polys[i].r0 = fade->polys[i].g0 = fade->polys[i].b0 = 0;
            }
        }
    }
    SAI_SPRITES[42] = SAI_createSprite(8);
    SAI_setSpriteDepth(SAI_SPRITES[42], 0x21);
    SAI_setSpriteBlendMode(SAI_SPRITES[42], 1);
    SAI_SPRITES[43] = SAI_createSprite(0x3B);
    SAI_setSpriteDepth(SAI_SPRITES[43], 0x21);
    SAI_SPRITES[42]->pos.vx = 0xD2;
    SAI_SPRITES[42]->pos.vy = 0x15;
    SAI_SPRITES[43]->pos.vx = SAI_SPRITES[42]->pos.vx = 0xD1;
    SAI_SPRITES[43]->pos.vy = SAI_SPRITES[42]->pos.vy = 0x14;
    SAI_SPRITES[44] = SAI_createSprite(8);
    SAI_setSpriteDepth(SAI_SPRITES[44], 0x21);
    SAI_setSpriteBlendMode(SAI_SPRITES[44], 1);
    SAI_SPRITES[45] = SAI_createSprite(0x3B);
    SAI_setSpriteDepth(SAI_SPRITES[45], 0x21);
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx = -0xC8;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy = 0x15;
    SAI_OPPONENTS.target[0] = SAI_OPPONENTS.current[0] = 0x80;
    SAI_OPPONENTS.target[1] = SAI_OPPONENTS.current[1] = 0x80;
    SAI_OPPONENT_INFO->wipeX = 0x140;
    SAI_OPPONENT_INFO->unkC2 = 0;
    SAI_OPPONENT_INFO->state = 0;
    SAI_OPPONENT_INFO->timer = 0;
    SAI_OPPONENT_INFO->statsShown = 0;
    SAI_OPPONENT_INFO->phase = 2;
}

void SAI_showOpponentInfo(void) {
    Rect16 rect;
    s32 i;

    if (SESSION->shownOpponent != (s8)SAI_SCRIPT[0]->regs[10]) {
        SAI_AREA_MODE = AREA_MODE_BUSY;
        if (SAI_OPPONENT_INFO->statsShown == 1) {
            do {
                func_80014C08(1);
                SAI_OPPONENT_INFO->state = 4;
            } while (SAI_moveOpponentPortraits() != 0);
        }
        for (i = 0; i < 7; i++) {
            SAI_OPPONENT_INFO->entries[i].delay = rand() % 30;
            SAI_OPPONENT_INFO->entries[i].progress = 0;
        }
        SAI_OPPONENT_INFO->statsShown = 0;
        SESSION->shownOpponent = SAI_SCRIPT[0]->regs[10];
        SAI_OPPONENT_INFO->timer = 0;
        SAI_OPPONENT_INFO->unkC5 = 1;
        SAI_OPPONENT_INFO->state = 1;
        SAI_OPPONENT_INFO->showWipe = 1;
        SAI_OPPONENT_INFO->alpha = 0;
        SAI_OPPONENT_INFO->phase = 0;
        SAI_OPPONENT_INFO->delay = 360;
        SAI_OPPONENT_INFO->brightness = 0x80;
        SAI_OPPONENT_INFO->pulsePhase = 0;
        rect.x = (SAI_SCRIPT[0]->regs[10] & 3) * 32 + 0x280;
        rect.y = ((u32)SAI_SCRIPT[0]->regs[10] >> 2) * 56 + 0x100;
        rect.w = 0x3F;
        rect.h = 0x38;
        SAI_setSpriteImage8Bit(SAI_SPRITES[43], &rect, 0);
        SAI_SPRITES[43]->quads[0].clut = getClut(0x280, 0x1FB);
        SAI_SPRITES[43]->quads[1].clut = getClut(0x280, 0x1FB);
    } else {
        SAI_OPPONENT_INFO->state = 0;
        SAI_AREA_MODE = AREA_MODE_SCRIPT;
    }
}

s32 SAI_slideInOpponentStats(void) {
    s32 result = 1;
    s32 finished = -1;
    s32 i;

    SAI_OPPONENT_INFO->wipeX -= 15;
    if (SAI_OPPONENT_INFO->wipeX <= 0) {
        SAI_OPPONENT_INFO->wipeX = 0;
        for (i = 0; i < 7; i++) {
            if (SAI_OPPONENT_INFO->entries[i].delay <= 0) {
                SAI_OPPONENT_INFO->entries[i].progress++;
                if (SAI_OPPONENT_INFO->entries[i].progress >= 20) {
                    SAI_OPPONENT_INFO->entries[i].progress = 20;
                    finished++;
                }
            } else {
                SAI_OPPONENT_INFO->entries[i].delay--;
            }
        }
    }
    if (finished >= 6) {
        SAI_OPPONENT_INFO->state = 2;
        result = 0;
    }
    return result;
}

s32 SAI_slideInOpponentPortrait(void) {
    s16 alpha = SAI_OPPONENT_INFO->brightness;
    s32 result = 1;

    if (SAI_OPPONENT_INFO->timer == 19) {
        playSoundEffect(0x10);
    }
    SAI_OPPONENT_INFO->timer++;
    if (SAI_OPPONENT_INFO->timer > 20) {
        SAI_OPPONENT_INFO->timer = 20;
    }
    SAI_SPRITES[43]->pos.vx = (SAI_OPPONENT_INFO->timer * 60 + (20 - SAI_OPPONENT_INFO->timer) * 210) / 20;
    SAI_SPRITES[43]->pos.vy = -1;
    if (SAI_OPPONENT_INFO->timer == 20) {
        if (SAI_OPPONENT_INFO->pulsePhase == 0) {
            alpha += 8;
            if (alpha > 0xFF) {
                alpha = 0xFF;
                SAI_OPPONENT_INFO->pulsePhase = 1;
            }
        } else {
            alpha -= 8;
            if (alpha < 0x80) {
                alpha = 0x80;
                SAI_OPPONENT_INFO->pulsePhase = 2;
                SAI_OPPONENT_INFO->state = 5;
                result = 0;
            }
        }
        SAI_OPPONENT_INFO->brightness = alpha;
    }
    if (result == 0) {
        SAI_OPPONENT_INFO->timer = 0;
    }
    return result;
}

s32 SAI_moveOpponentPortraits(void) {
    s32 result = 1;

    if (SAI_OPPONENT_INFO->state == 4) {
        SAI_OPPONENT_INFO->timer--;
        if (SAI_OPPONENT_INFO->timer < 0) {
            SAI_OPPONENT_INFO->timer = 0;
            result = 0;
        }
    } else {
        SAI_OPPONENT_INFO->timer++;
        if (SAI_OPPONENT_INFO->timer > 20) {
            SAI_OPPONENT_INFO->statsShown = 1;
            SAI_OPPONENT_INFO->timer = 20;
            SAI_OPPONENT_INFO->state = 0;
            SAI_AREA_MODE = AREA_MODE_SCRIPT;
            result = 0;
        }
    }
    SAI_SPRITES[44]->pos.vx = (SAI_OPPONENT_INFO->timer * -103 + (20 - SAI_OPPONENT_INFO->timer) * -200) / 20;
    SAI_SPRITES[44]->pos.vy = 0x15;
    SAI_SPRITES[45]->pos.vx = SAI_SPRITES[44]->pos.vx;
    SAI_SPRITES[45]->pos.vy = SAI_SPRITES[44]->pos.vy;
    if (SAI_OPPONENT_INFO->unkC5 == 1) {
        SAI_SPRITES[42]->pos.vx = (SAI_OPPONENT_INFO->timer * 108 + (20 - SAI_OPPONENT_INFO->timer) * 60) / 20;
        SAI_SPRITES[42]->pos.vy = (SAI_OPPONENT_INFO->timer * 21 + (20 - SAI_OPPONENT_INFO->timer) * -1) / 20;
        SAI_SPRITES[43]->pos.vx = SAI_SPRITES[42]->pos.vx - 1;
        SAI_SPRITES[43]->pos.vy = SAI_SPRITES[42]->pos.vy - 1;
    } else {
        SAI_SPRITES[42]->pos.vx = (SAI_OPPONENT_INFO->timer * 108 + (20 - SAI_OPPONENT_INFO->timer) * 210) / 20;
        SAI_SPRITES[42]->pos.vy = 0x15;
        SAI_SPRITES[43]->pos.vx = SAI_SPRITES[42]->pos.vx - 1;
        SAI_SPRITES[43]->pos.vy = SAI_SPRITES[42]->pos.vy - 1;
    }
    if (result == 0) {
        SAI_OPPONENT_INFO->unkC5 = 0;
    }
    return result;
}

void SAI_tickOpponentBanner(void) {
    s16 alpha = SAI_OPPONENT_INFO->alpha;

    if (SAI_OPPONENT_INFO->delay == 0) {
        if (SAI_OPPONENT_INFO->phase == 0) {
            if (alpha == 0) {
                playSoundEffect(0x11);
            }
            alpha += 8;
            if (alpha > 0xFF) {
                alpha = 0xFF;
                SAI_OPPONENT_INFO->phase = 1;
                SAI_OPPONENT_INFO->showWipe = 0;
                SAI_SPRITES[42]->pos.vx = SAI_SPRITES[43]->pos.vx;
                SAI_SPRITES[42]->pos.vy = SAI_SPRITES[43]->pos.vy;
                SAI_SPRITES[43]->pos.vx--;
                SAI_SPRITES[43]->pos.vy--;
            }
        } else {
            alpha -= 8;
            if (alpha < 0) {
                alpha = 0;
                SAI_OPPONENT_INFO->phase = 2;
                SAI_OPPONENT_INFO->state = 3;
            }
        }
        SAI_OPPONENT_INFO->alpha = alpha;
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        SAI_OPPONENT_INFO->delay = 0;
    } else if (SAI_OPPONENT_INFO->delay != 0) {
        SAI_OPPONENT_INFO->delay--;
    }
}

void SAI_runOpponentInfoPanel(s32 index) {
    Rect16 rect;

    SAI_OPPONENT_INFO = allocTaskHeapBlock(0xCC);
    SAI_initOpponentInfo();
    addFrameCallback((s32)SAI_drawOpponentInfo);
    if (SESSION->resumeMode != 0) {
        SAI_OPPONENTS = SESSION->opponents;
        SAI_LOCATION = SESSION->location;
        SAI_SCRIPT[0]->script->pc = SAI_SCRIPT[0]->script->start + SESSION->scriptOffset;
        SAI_SCRIPT[0]->regs[1] = (s8)(SESSION->duelResult ^ 1);
        rect.x = SESSION->shownOpponent % 4 * 32 + 0x280;
        rect.y = SESSION->shownOpponent / 4 * 56 + 0x100;
        rect.w = 0x3F;
        rect.h = 0x38;
        SAI_setSpriteImage8Bit(SAI_SPRITES[43], &rect, 0);
        SAI_SPRITES[43]->quads[0].clut = getClut(0x280, 0x1FB);
        SAI_SPRITES[43]->quads[1].clut = getClut(0x280, 0x1FB);
        SAI_OPPONENT_INFO->timer = 0;
        SAI_OPPONENT_INFO->unkC5 = 0;
        SAI_OPPONENT_INFO->state = 3;
        SAI_AREA_MODE = AREA_MODE_BUSY;
        SAI_OPPONENT_INFO->showWipe = 0;
    } else {
        SESSION->shownOpponent = -1;
    }
    SAI_AREA.openPanel = SAI_AREA.location + 1;
    SAI_AREA.closePanel = 0;
    SAI_AREA.imageHidden = -1;
    SAI_AREA.opening = 1;
    SAI_createPanel();
    SAI_SPRITES[0] = SAI_createSprite(4);
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x24);
    SAI_spinPanel();
    playSoundEffect(10);
    do {
        func_80014C08(1);
    } while (SAI_spinPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = 0;
    SAI_toggleMessageWindow(1);
    if (SESSION->resumeMode == 0) {
        func_800149B8(0, -1, 0, 0x800, SAI_runSplash, index - 1, getCurrentTaskId(), 0, 0);
        func_80014C08(0x7FFFFFFF);
    }
    do {
        func_80014C08(1);
    } while (SAI_uncoverPanel() == 0);
    if (SESSION->resumeMode == 0) {
        SAI_AREA_MODE = AREA_MODE_SCRIPT;
    } else {
        SESSION->resumeMode = 0;
    }
    do {
        func_80014C08(1);
        switch (SAI_OPPONENT_INFO->state) {
        case 0:
            break;
        case 1:
            SAI_slideInOpponentStats();
            break;
        case 2:
            SAI_slideInOpponentPortrait();
            break;
        case 3:
        case 4:
            SAI_moveOpponentPortraits();
            break;
        case 5:
            SAI_tickOpponentBanner();
            break;
        }
    } while (SAI_CLOSE_PANEL == 0);
    do {
        func_80014C08(1);
        SAI_OPPONENT_INFO->state = 4;
    } while (SAI_moveOpponentPortraits() != 0);
    removeFrameCallback((s32)SAI_drawOpponentInfo);
    do {
        func_80014C08(1);
    } while (SAI_coverPanel() == 0);
    SAI_PANEL_IMAGE_HIDDEN = -1;
    playSoundEffect(11);
    do {
        func_80014C08(1);
    } while (SAI_spinPanel() == 0);
    SAI_freePanel();
    SAI_freeOpponentInfo();
    SAI_OPEN_PANEL = 0;
}

void SAI_loadSplashImage(u8 index) {
    char path[24];
    u32 *tim;

    if (index < 1 || index > 4) {
        index = 0;
    }
    sprintf(path, "C:\\OBJECT\\a_%d.TIM", index);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, 0x140, 0, 0x140, 0x81);
    freeHeapBlock(tim);
    SAI_SPLASH->loading = 0;
}

void SAI_initSplashQuads(void) {
    POLY_FT4 *quad;
    s32 buf;
    s32 i;

    for (buf = 0; buf < 2; buf++) {
        quad = SAI_SPLASH->quads[buf];
        for (i = 0; i < 4; i++, quad++) {
            setlen(quad, 9);
            setcode(quad, 0x2C);
            SetSemiTrans(quad, 0);
            quad->clut = getClut(0x140, 0x81);
            quad->tpage = getTPage(0, 0, 0x140, 0);
            quad->r0 = 0x80;
            quad->g0 = 0x80;
            quad->b0 = 0x80;
            quad->u0 = quad->u2 = 0;
            quad->u1 = quad->u3 = 0xFF;
            quad->x0 = quad->x2 = SAI_SPLASH->x;
            quad->x1 = quad->x3 = SAI_SPLASH->x + 0xFF;
        }
    }
}

void SAI_resetSplash(void) {
    SAI_SPLASH->state = 0;
    SAI_SPLASH->speed = 40;
    SAI_SPLASH->x = 32;
    SAI_SPLASH->y = 44;
    SAI_SPLASH->step = 0;
    SAI_SPLASH->scroll = 0;
    SAI_SPLASH->holdTimer = 60;
}

void SAI_unfoldSplash(void) {
    POLY_FT4 *quad = SAI_SPLASH->quads[FRAME_BUFFER_INDEX];
    u8 brightness;
    s32 limit = 62;
    s32 i;

    SAI_SPLASH->scroll += SAI_SPLASH->speed;
    if (limit - SAI_SPLASH->step < SAI_SPLASH->scroll) {
        SAI_SPLASH->scroll = 0;
        SAI_SPLASH->step++;
        if (SAI_SPLASH->step > 62) {
            SAI_SPLASH->step = 62;
        }
    }
    quad->v0 = quad->v1 = 62 - SAI_SPLASH->step;
    quad->v2 = quad->v3 = 63 - SAI_SPLASH->step;
    quad->y0 = quad->y1 = 0;
    quad->y2 = quad->y3 = SAI_SPLASH->y - (s16)(SAI_SPLASH->step - 63);
    quad++;
    quad->y0 = quad->y1 = SAI_SPLASH->y - (s16)(SAI_SPLASH->step - 63);
    quad->y2 = quad->y3 = SAI_SPLASH->y + 63;
    quad->v0 = quad->v1 = 63 - SAI_SPLASH->step;
    quad->v2 = quad->v3 = 63;
    quad++;
    quad->v0 = quad->v1 = SAI_SPLASH->step + 64;
    quad->v2 = quad->v3 = SAI_SPLASH->step + 65;
    quad->y0 = quad->y1 = SAI_SPLASH->y + (s16)(SAI_SPLASH->step + 63);
    quad->y2 = quad->y3 = 0xF0;
    quad++;
    quad->y0 = quad->y1 = SAI_SPLASH->y + 63;
    quad->y2 = quad->y3 = SAI_SPLASH->y + (s16)(SAI_SPLASH->step + 63);
    quad->v0 = quad->v1 = 64;
    quad->v2 = quad->v3 = SAI_SPLASH->step + 64;
    brightness = (SAI_SPLASH->step * 128 + (limit - SAI_SPLASH->step) * 64) / limit;
    for (i = 0; i < 4; i++) {
        SAI_SPLASH->brightness[i] = brightness;
    }
    if (brightness == 0x80) {
        SAI_SPLASH->state = 1;
    }
}

void SAI_holdSplash(void) {
    SAI_SPLASH->holdTimer--;
    if (SAI_SPLASH->holdTimer < 0) {
        playSoundEffect(0x13);
        SAI_SPLASH->state = 2;
    }
}

void SAI_fadeOutSplash(void) {
    s32 value = SAI_SPLASH->brightness[0] - 8;
    s32 i;

    if (value < 0) {
        SAI_SPLASH->state = 3;
        value = 0;
    }
    for (i = 0; i < 4; i++) {
        SAI_SPLASH->brightness[i] = value;
    }
}

void SAI_drawSplash(void) {
    POLY_FT4 *quad = SAI_SPLASH->quads[FRAME_BUFFER_INDEX];
    s32 i;

    for (i = 0; i < 4; i++, quad++) {
        quad->r0 = SAI_SPLASH->brightness[i];
        quad->g0 = SAI_SPLASH->brightness[i];
        quad->b0 = SAI_SPLASH->brightness[i];
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], quad);
    }
}

void SAI_runSplash(s32 index, s32 task) {
    SAI_SPLASH = allocTaskHeapBlock(0x15C);
    SAI_SPLASH->loading = 1;
    SAI_loadSplashImage(index);
    do {
        func_80014C08(1);
    } while (SAI_SPLASH->loading != 0);
    SAI_resetSplash();
    SAI_initSplashQuads();
    do {
        func_80014C08(1);
        switch (SAI_SPLASH->state) {
        case 0:
            SAI_unfoldSplash();
            break;
        case 1:
            SAI_holdSplash();
            break;
        case 2:
            SAI_fadeOutSplash();
            break;
        }
        SAI_drawSplash();
    } while (SAI_SPLASH->state != 3);
    func_80014C08(30);
    func_80014A48(task);
}

void SAI_openErrorWindow(UiWindow *window, WindowDef *def) {
    measureText((u8 *)SAI_STR_SYSTEM_ERROR);
    def->rect.w = (TEXT_WIDTH + 1) / 2 * 2;
    def->rect.h = (TEXT_HEIGHT + 1) / 2 * 2;
    openWindow(window, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 6);
    if (def->label != 0) {
        window->label = def->label;
    }
    window->labelPalette = def->labelPalette;
    animateWindowTo(window, (Rect16 *)-1);
}

void SAI_drawErrorText(UiWindow *window) {
    char buf[0x48];

    drawText(window->originX, window->originY, (s32)SAI_STR_SYSTEM_ERROR, 0, window->z);
}

void SAI_drawErrorWindows(void) {
    s32 i;

    for (i = 0; i < 5; i++) {
        drawWindow(&SAI_ERROR_WINDOWS[i], SAI_ERROR_WINDOW_DRAW_FUNCS[i], 9);
    }
}

void SAI_runSystemErrorHack(void) {
    stopMusic();
    addFrameCallback((s32)SAI_drawErrorWindows);
    setBackgroundScrollMode(1);
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 0, getCurrentTaskId(), 0, 0);
    func_80014C08(360);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[0], &SAI_ERROR_WINDOW_DEFS[0].rect);
    func_80014C08(7);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[1], &SAI_ERROR_WINDOW_DEFS[1].rect);
    func_80014C08(8);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[2], &SAI_ERROR_WINDOW_DEFS[2].rect);
    func_80014C08(3);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[3], &SAI_ERROR_WINDOW_DEFS[3].rect);
    func_80014C08(2);
    playMenuSound(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[4], &SAI_ERROR_WINDOW_DEFS[4].rect);
    func_80014C08(110);
    playSoundEffect(0x18);
    func_80014C08(480);
    playSoundEffect(0x19);
    func_80014C08(120);
    playSoundEffect(0x19);
    func_80014C08(240);
    animateWindowTo(&SAI_ERROR_WINDOWS[0], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(4);
    animateWindowTo(&SAI_ERROR_WINDOWS[1], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(2);
    animateWindowTo(&SAI_ERROR_WINDOWS[2], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(3);
    animateWindowTo(&SAI_ERROR_WINDOWS[3], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(1);
    animateWindowTo(&SAI_ERROR_WINDOWS[4], (Rect16 *)-1);
    playSoundEffect(0x19);
    func_80014C08(120);
    SAI_glitchVram(1);
    removeFrameCallback((s32)SAI_drawErrorWindows);
    func_80014C08(170);
    playSoundEffect(0x18);
    func_80014C08(30);
}

void SAI_runHackingScene1(void) {
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 1, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
}

void SAI_runHackingScene3(void) {
    func_800149B8(0, -1, 0, 0x800, runHackingSequence, 3, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
}

void SAI_runHackingEvent(s32 mode, s32 task) {
    s32 i;

    for (i = 0; i < 5; i++) {
        SAI_openErrorWindow(&SAI_ERROR_WINDOWS[i], &SAI_ERROR_WINDOW_DEFS[i]);
    }
    switch (mode) {
    case 0:
        SAI_runSystemErrorHack();
        break;
    case 1:
        SAI_runHackingScene1();
        break;
    case 2:
        SAI_runHackingScene3();
        break;
    }
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
    func_80014A48(task);
}

void SAI_flickerHackOverlay(void) {
    SAI_HACK_OVERLAY_FLICKER = 6;
}

void SAI_closeHackOverlay(void) {
    SAI_HACK_OVERLAY_STATE = 2;
}

const char SAI_STR_SYSTEM_ERROR[] = "               *c6SYSTEM ERROR\n*c2 Illegal Sharing: \n*c7 Unauthorized command was used.";

void SAI_runHackOverlay(void) {
    Rect16 uv = { 0, 0, 0x80, 0x80 };
    DrTPage tpages[2];
    PolyF4 polys[2];
    u8 brightness = 0;
    s32 i;

    SAI_HACK_OVERLAY_STATE = 1;
    for (i = 0; i < 2; i++) {
        SetDrawTPage(&tpages[i], 0, 0, 0x40);
        func_80067784(&polys[i]);
        SetSemiTrans(&polys[i], 1);
        setPrimQuadRect(&polys[i], 0, 0, 0x140, 0xF0);
        setPrimRgb0(&polys[i], brightness, brightness, brightness);
    }
    do {
        func_80014C08(1);
        if (SAI_HACK_OVERLAY_STATE == 1) {
            if (brightness < 0xF7) {
                brightness += 8;
            } else {
                brightness = 0xFF;
            }
        } else if (SAI_HACK_OVERLAY_STATE == 2) {
            if (brightness > 8) {
                brightness -= 8;
            } else {
                brightness = 0;
                SAI_HACK_OVERLAY_STATE = 0;
            }
        }
        if (SAI_HACK_OVERLAY_FLICKER != 0) {
            SAI_HACK_OVERLAY_FLICKER--;
        }
        if (SAI_HACK_OVERLAY_STATE != 0) {
            if ((SAI_HACK_OVERLAY_FLICKER & 1) && brightness == 0xFF) {
                drawTexturedSprite(0x60, 0x1A, &uv, 0x95, 0x7C00, 0x18, 0x80, -1);
            }
            setPrimRgb0(&polys[FRAME_BUFFER_INDEX], brightness, brightness, brightness);
            addPrim(&CURRENT_FRAME_BUFFER->ot[24], &polys[FRAME_BUFFER_INDEX]);
            addPrim(&CURRENT_FRAME_BUFFER->ot[24], &tpages[FRAME_BUFFER_INDEX]);
        }
    } while (SAI_HACK_OVERLAY_STATE != 0);
    func_80014C08(1);
}

void (*SAI_ERROR_WINDOW_DRAW_FUNCS[5])() = {
    SAI_drawErrorText,
    SAI_drawErrorText,
    SAI_drawErrorText,
    SAI_drawErrorText,
    SAI_drawErrorText,
};

/* the characters of the name entry grid, ten to a row */
char SAI_WORD_INPUT_CHARS[] =
    "ABCDEabcde"
    "FGHIJfghij"
    "KLMNOklmno"
    "PQRSTpqrst"
    "UVWXYuvwxy"
    "Z-   z    "
    "          "
    "          "
    "0123456789";

/* the names SAI_runWordInput recognises when one is typed in */
char *SAI_KEYWORDS[10] = {
    "OMNIMON-1",
    "WARGREYMON",
    "OMNIMON-2",
    "MTLGARURUMON",
    "A-VEEDRAMON",
    "H-KBUTERIMON",
    "VENOMMYOTIS",
    "PIEDMON",
    "MTLETEMON",
    "JIJIMON",
};

const char D_801DE788[] = "";

s32 SAI_moveWordInputCursor(void) {
    Rect16 rect;
    UiWindow *win = &SAI_WORD_GRID_WINDOW;
    s16 r;

    if (SAI_WORD_INPUT.active != 0) {
        if (SAI_WORD_INPUT.mode == 0) {
            if ((u16)PAD_STATES[0]->repeat & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) {
                playSoundEffect(2);
            }
            do {
                if (PAD_STATES[0]->repeat & PAD_UP) {
                    SAI_WORD_INPUT.row = ((s16)(SAI_WORD_INPUT.row - 1) + 1) / 9 * 9 + ((s16)(SAI_WORD_INPUT.row - 1) + 9) % 9;
                    if (SAI_WORD_INPUT.row % 9 == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & PAD_DOWN) {
                    SAI_WORD_INPUT.row = ((s16)(SAI_WORD_INPUT.row + 1) - 1) / 9 * 9 + ((s16)(SAI_WORD_INPUT.row + 1) + 9) % 9;
                    if (SAI_WORD_INPUT.row % 9 == 8) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[0]->repeat & PAD_LEFT) {
                    if (--SAI_WORD_INPUT.col < 0) {
                        SAI_WORD_INPUT.col = 9;
                        SAI_WORD_INPUT.mode = 1;
                        SAI_WORD_INPUT.sel = 7;
                    } else if (SAI_WORD_INPUT.col == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & PAD_RIGHT) {
                    if (++SAI_WORD_INPUT.col >= 10) {
                        SAI_WORD_INPUT.col = 0;
                        SAI_WORD_INPUT.mode = 1;
                        SAI_WORD_INPUT.sel = 7;
                    } else if (SAI_WORD_INPUT.col == 9) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                }
            } while (SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col] == ' ');
        } else {
            if ((u16)PAD_STATES[0]->repeat & (PAD_UP | PAD_DOWN | PAD_LEFT | PAD_RIGHT)) {
                playSoundEffect(2);
            }
            if ((u16)PAD_STATES[0]->pressed & (PAD_LEFT | PAD_RIGHT)) {
                playSoundEffect(2);
            }
            if (PAD_STATES[0]->repeat & PAD_UP) {
                if (--SAI_WORD_INPUT.sel < 7) {
                    SAI_WORD_INPUT.sel = 8;
                } else if (SAI_WORD_INPUT.sel == 7) {
                    PAD_STATES[0]->repeatEnabled = 0;
                }
            } else if (PAD_STATES[0]->repeat & PAD_DOWN) {
                if (++SAI_WORD_INPUT.sel >= 9) {
                    SAI_WORD_INPUT.sel = 7;
                } else if (SAI_WORD_INPUT.sel == 8) {
                    PAD_STATES[0]->repeatEnabled = 0;
                }
            } else if ((u16)PAD_STATES[0]->pressed & PAD_LEFT) {
                SAI_WORD_INPUT.mode = 0;
                SAI_WORD_INPUT.prevSel = -1;
                SAI_WORD_INPUT.prevRow = SAI_WORD_INPUT.prevCol = -1;
                SAI_WORD_INPUT.col = 9;
                while (SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col] == ' ') {
                    SAI_WORD_INPUT.col--;
                }
            } else if (PAD_STATES[0]->pressed & PAD_RIGHT) {
                SAI_WORD_INPUT.mode = 0;
                SAI_WORD_INPUT.prevSel = -1;
                SAI_WORD_INPUT.prevRow = SAI_WORD_INPUT.prevCol = -1;
                SAI_WORD_INPUT.col = 0;
                while (SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col] == ' ') {
                    SAI_WORD_INPUT.col++;
                }
            }
        }
    }
    if (SAI_WORD_INPUT.mode == 0) {
        if (SAI_WORD_INPUT.row != SAI_WORD_INPUT.prevRow || SAI_WORD_INPUT.col != SAI_WORD_INPUT.prevCol) {
            SAI_WORD_INPUT.prevCol = SAI_WORD_INPUT.col;
            SAI_WORD_INPUT.prevRow = SAI_WORD_INPUT.row;
            rect.x = win->rect.x - win->scroll[2] + SAI_WORD_INPUT.col * 17 + 4;
            rect.y = win->rect.y - win->scroll[3] + SAI_WORD_INPUT.row * 14 + 1;
            rect.w = 0xC;
            rect.h = 0xC;
            if (SAI_WORD_INPUT.col >= 5) {
                rect.x += 11;
            }
            moveCursorHighlight(&SAI_WORD_GRID_CURSOR, &rect);
        }
    } else if (SAI_WORD_INPUT.sel != SAI_WORD_INPUT.prevSel) {
        SAI_WORD_INPUT.prevSel = SAI_WORD_INPUT.sel;
        switch (SAI_WORD_INPUT.sel) {
        case 0:
        case 1:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + SAI_WORD_INPUT.sel * 14 + 1;
            rect.w = 0x30;
            rect.h = 0xC;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + SAI_WORD_INPUT.sel * 14 + 1;
            rect.w = 0x24;
            rect.h = 0xC;
            break;
        case 7:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + 0x63;
            rect.w = 0x18;
            rect.h = 0xC;
            break;
        case 8:
            rect.x = win->rect.x + 0xCC;
            rect.y = win->rect.y + 0x71;
            rect.w = 0x30;
            rect.h = 0xC;
            break;
        }
        moveCursorHighlight(&SAI_WORD_GRID_CURSOR, &rect);
    }
}

void SAI_drawWordInputGrid(UiWindow *window) {
    char text[0x40];
    u8 marks[7];
    s32 i;
    s32 x = window->originX + 4;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 gap;
    s32 dx;
    s32 sel;

    for (i = 0; i < 90; i++) {
        sprintf(text, "%c", SAI_WORD_INPUT_CHARS[i]);
        gap = 0;
        if (i % 10 >= 5) {
            gap = 11;
        }
        dx = (i % 10) * 17 + 4;
        drawText(x + dx + gap, y + (i / 10) * 14, (s32)text, 7, z);
    }
    for (i = 0; i < 7; i++) {
        marks[i] = 4;
    }
    marks[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xCC, window->rect.y + 0x63, (s32)"OK", 6, z);
    drawText(window->rect.x + 0xCC, window->rect.y + 0x71, (s32)"Cancel", 6, z);
    SAI_moveWordInputCursor();
    if (SAI_WORD_INPUT.mode == 0) {
        if (PAD_STATES[0]->repeat & PAD_CROSS) {
            SAI_WORD_INPUT.text[SAI_WORD_INPUT.cursor] = SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col];
            playSoundEffect(0);
            if (SAI_WORD_INPUT.cursor < 11) {
                SAI_WORD_INPUT.cursor++;
            } else {
                SAI_WORD_INPUT.mode = 1;
                SAI_WORD_INPUT.sel = 7;
            }
        } else if (PAD_STATES[0]->repeat & PAD_CIRCLE) {
            for (i = 11; i >= SAI_WORD_INPUT.cursor + 1; i--) {
                SAI_WORD_INPUT.text[i] = SAI_WORD_INPUT.text[i - 1];
            }
            SAI_WORD_INPUT.text[SAI_WORD_INPUT.cursor] = SAI_WORD_INPUT_CHARS[SAI_WORD_INPUT.row * 10 + SAI_WORD_INPUT.col];
            playSoundEffect(0);
            if (SAI_WORD_INPUT.cursor < 11) {
                SAI_WORD_INPUT.cursor++;
            } else {
                SAI_WORD_INPUT.mode = 1;
                SAI_WORD_INPUT.sel = 7;
            }
        } else if (PAD_STATES[0]->repeat & PAD_TRIANGLE) {
            if (SAI_WORD_INPUT.text[0] != 0) {
                playSoundEffect(0);
            }
            if (SAI_WORD_INPUT.cursor == 0) {
                SAI_WORD_INPUT.cursor++;
            }
            for (i = SAI_WORD_INPUT.cursor; i < 13; i++) {
                SAI_WORD_INPUT.text[i - 1] = SAI_WORD_INPUT.text[i];
            }
            SAI_WORD_INPUT.cursor--;
        }
    } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
        playSoundEffect(0);
        sel = SAI_WORD_INPUT.sel;
        if (sel >= 0) {
            if (sel < 7) {
                SAI_WORD_INPUT.row = sel * 9;
                SAI_WORD_INPUT.col = 0;
                scrollWindowTo(&window->originX, 0, SAI_WORD_INPUT.row * 14);
            } else if (sel < 9) {
                SAI_WORD_INPUT.result = SAI_WORD_INPUT.sel;
            }
        }
    } else if (PAD_STATES[0]->repeat & PAD_TRIANGLE) {
        if (SAI_WORD_INPUT.text[0] != 0) {
            playSoundEffect(0);
        }
        if (SAI_WORD_INPUT.cursor == 0) {
            SAI_WORD_INPUT.cursor++;
        }
        for (i = SAI_WORD_INPUT.cursor; i < 13; i++) {
            SAI_WORD_INPUT.text[i - 1] = SAI_WORD_INPUT.text[i];
        }
        SAI_WORD_INPUT.cursor--;
    }
    if (PAD_STATES[0]->pressed & PAD_START) {
        if (SAI_WORD_INPUT.mode != 1 || SAI_WORD_INPUT.sel != 7) {
            playSoundEffect(0);
            SAI_WORD_INPUT.mode = 1;
            SAI_WORD_INPUT.sel = 7;
        }
    }
    drawCursorHighlight(&SAI_WORD_GRID_CURSOR, z);
}

void SAI_drawKeywordField(UiWindow *window) {
    char text[0x40];
    Rect16 cursor;
    s32 x = window->originX + 1;
    s32 y = window->originY;
    s32 z = window->z;

    sprintf(text, "*s0%s", SAI_WORD_INPUT.text);
    drawText(x, y, (s32)text, 7, z);
    if (PAD_STATES[0]->repeat & PAD_L1) {
        if (SAI_WORD_INPUT.cursor != 0) {
            playSoundEffect(2);
            SAI_WORD_INPUT.cursor--;
        }
    } else if (PAD_STATES[0]->repeat & PAD_R1) {
        if (SAI_WORD_INPUT.cursor != 11 && SAI_WORD_INPUT.text[SAI_WORD_INPUT.cursor] != 0) {
            playSoundEffect(2);
            SAI_WORD_INPUT.cursor++;
        }
    }
    cursor.x = x + SAI_WORD_INPUT.cursor * 6;
    cursor.y = y + 13;
    cursor.w = 6;
    cursor.h = 0;
    moveCursorHighlight(&SAI_KEYWORD_CURSOR, &cursor);
    drawCursorHighlight(&SAI_KEYWORD_CURSOR, z);
}

void SAI_drawWordInputHelp(UiWindow *window) {
    s32 x = window->originX + 1;
    s32 y = window->originY + 1;
    s32 z = window->z;
    s32 unused[2];

    drawText(x, y, (s32)"*b0 Insert", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b1 Delete", 7, z);
}

void SAI_drawWordInputWindows(void) {
    drawWindow(&SAI_WORD_GRID_WINDOW, SAI_drawWordInputGrid, 0x1E);
    drawWindow(&SAI_KEYWORD_WINDOW, SAI_drawKeywordField, 0x1E);
    drawWindow(&SAI_WORD_HELP_WINDOW, SAI_drawWordInputHelp, 0x1E);
}

void SAI_runWordInput(char *word) {
    Rect16 cursorRect;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    s32 i;

    bzero((Scene3D *)SAI_WORD_INPUT.text, 13);
    strcpy(SAI_WORD_INPUT.text, word);
    SAI_WORD_INPUT.col = 0;
    SAI_WORD_INPUT.row = 0;
    SAI_WORD_INPUT.prevCol = -1;
    SAI_WORD_INPUT.prevRow = -1;
    SAI_WORD_INPUT.active = 1;
    SAI_WORD_INPUT.cursor = 0;
    SAI_WORD_INPUT.mode = 0;
    SAI_WORD_INPUT.sel = 0;
    SAI_WORD_INPUT.prevSel = -1;
    SAI_WORD_INPUT.result = 0;
    rect.x = 0x10;
    rect.y = 0x5C;
    rect.w = 0x10E;
    rect.h = 0x7E;
    SAI_WORD_INPUT.rows = 9;
    view.x = 0;
    view.y = 0;
    view.w = rect.w;
    view.h = SAI_WORD_INPUT.rows * 14;
    openWindow(&SAI_WORD_GRID_WINDOW, &rect, -1, (s16 *)&view, 10, 0x26, 0x80, 12);
    SAI_WORD_GRID_WINDOW.label = (s32)"WORD INPUT";
    cursorRect.x = SAI_WORD_GRID_WINDOW.originX + 4;
    cursorRect.y = SAI_WORD_GRID_WINDOW.originY + 1;
    cursorRect.w = 12;
    cursorRect.h = 12;
    initCursorHighlight(&SAI_WORD_GRID_CURSOR, &cursorRect, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x34;
    rect.h = 0x28;
    openWindow(&SAI_WORD_HELP_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SAI_WORD_HELP_WINDOW.label = (s32)"HELP";
    rect.x = 0x78;
    rect.y = 0x1C;
    rect.w = 0x48;
    rect.h = 0xE;
    openWindow(&SAI_KEYWORD_WINDOW, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 12);
    SAI_KEYWORD_WINDOW.label = (s32)"KEYWORD";
    cursorRect.x = SAI_KEYWORD_WINDOW.originX;
    cursorRect.y = SAI_KEYWORD_WINDOW.originY + 13;
    cursorRect.w = 12;
    cursorRect.h = 0;
    initCursorHighlight(&SAI_KEYWORD_CURSOR, &rect, (Bytes4 *)-1);
    playSoundEffect(3);
    addFrameCallback((s32)SAI_drawWordInputWindows);
    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (SAI_WORD_INPUT.result == 0) {
            continue;
        }
        if (SAI_WORD_INPUT.result == 7 && SAI_WORD_INPUT.text[0] == 0) {
            initDialog(dialog, SAI_STR_NO_KEYWORD, 0);
            runDialog(dialog);
            SAI_WORD_INPUT.result = 0;
            continue;
        }
        break;
    }
    SAI_SCRIPT[0]->regs[1] = -2;
    if (SAI_WORD_INPUT.result == 7) {
        strcpy(word, SAI_WORD_INPUT.text);
        SAI_SCRIPT[0]->regs[1] = -1;
    }
    playSoundEffect(4);
    animateWindowTo(&SAI_WORD_HELP_WINDOW, (Rect16 *)-1);
    animateWindowTo(&SAI_WORD_GRID_WINDOW, (Rect16 *)-1);
    animateWindowTo(&SAI_KEYWORD_WINDOW, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)SAI_drawWordInputWindows);
    for (i = 0; i < 10; i++) {
        if (strcmp(word, SAI_KEYWORDS[i]) == 0) {
            SAI_SCRIPT[0]->regs[1] = i;
            break;
        }
    }
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
    SAI_toggleMessageWindow(1);
}

void SAI_uploadPakTextures(u8 *pack) {
    PackEntry *entry;

    if (pack == NULL) {
        return;
    }
    do {
        entry = (PackEntry *)pack;
        pack += sizeof(PackEntry);
        if (entry->type < 0) {
            break;
        }
        if (entry->type == 5) {
            uploadTexturePack((u32 *)pack);
        }
        pack += entry->size;
    } while (1);
}

/* the last three bytes are leftovers in the original, not zero padding */
const char SAI_STR_NO_KEYWORD[36] = "A Key Word has not been entered!\0" "333";

void SAI_loadAreaPak(void) {
    char path[0x48];
    s32 area = ((PlayerProfile *)PLAYER_PROFILES)->unkE;

    SESSION->loading = 1;
    sprintf(path, "C:\\area%2.2d.pak", area);
    func_800149B8(0, -1, 0, 0x400, loadFileTagged, path, getCurrentTaskId(), 0x31);
    SESSION->pak = (Chunk *)func_80014C08(0x7FFFFFFF);
    SAI_uploadPakTextures((u8 *)SESSION->pak);
    truncatePakTextures(SESSION->pak);
    SESSION->script = findPakChunk(SESSION->pak, 2, area + 200);
    SESSION->loading = 0;
}

Sprite3D *SAI_createSprite(s32 id) {
    SpriteTemplate *tmpl = &SAI_SPRITE_TEMPLATES[id];
    s32 abr = 0;
    Sprite3D *sprite = allocHeapBlock(sizeof(Sprite3D), 0x2E);
    POLY_FT4 *quad;
    s32 i;

    sprite->corners[0].vx = sprite->corners[2].vx = -(tmpl->w >> 1);
    sprite->corners[1].vx = sprite->corners[3].vx = tmpl->w >> 1;
    sprite->corners[0].vy = sprite->corners[1].vy = -(tmpl->h >> 1);
    sprite->corners[2].vy = sprite->corners[3].vy = (tmpl->h >> 1) - 1;
    for (i = 0; i < 4; i++) {
        sprite->corners[i].vz = 0;
    }
    sprite->rot.vx = sprite->rot.vy = sprite->rot.vz = 0;
    sprite->pos.vx = sprite->pos.vy = sprite->pos.vz = 0;
    sprite->w = tmpl->w;
    sprite->h = tmpl->h - 1;
    sprite->otz = 0x23;
    for (i = 0; i < 2; i++) {
        quad = &sprite->quads[i];
        func_800677A4(quad);
        quad->r0 = 0x80;
        quad->g0 = 0x80;
        quad->b0 = 0x80;
        SetSemiTrans(quad, 0);
        quad->clut = getClut(tmpl->clutX, tmpl->clutY);
        quad->tpage = ((tmpl->mode & 3) << 7) | ((abr & 3) << 5) | ((tmpl->y & 0x100) >> 4) | ((tmpl->x & 0x3C0) >> 6) | ((tmpl->y & 0x200) << 2);
        if (tmpl->mode != 0) {
            quad->u2 = quad->u0 = (tmpl->x % 64) << 1;
            quad->u1 = quad->u3 = ((tmpl->x % 64) << 1) + tmpl->w;
        } else {
            quad->u2 = quad->u0 = (tmpl->x % 64) << 2;
            quad->u1 = quad->u3 = ((tmpl->x % 64) << 2) + tmpl->w;
        }
        quad->v0 = quad->v1 = tmpl->y;
        quad->v2 = quad->v3 = tmpl->y + tmpl->h - 1;
    }
    return sprite;
}

void SAI_freeSprite(void *ptr) {
    freeHeapBlock(ptr);
}

void SAI_drawSprite(Sprite3D *sprite) {
    MATRIX matrix;
    SVECTOR corners[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;

    buildRotTransMatrix(&sprite->pos, &sprite->rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->unk78, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    func_8005C444(&matrix);
    corners[0] = sprite->corners[0];
    corners[1] = sprite->corners[1];
    corners[2] = sprite->corners[2];
    corners[3] = sprite->corners[3];
    RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    sprite->quads[FRAME_BUFFER_INDEX].x0 = sxy[0];
    sprite->quads[FRAME_BUFFER_INDEX].y0 = sxy[0] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x1 = sxy[1];
    sprite->quads[FRAME_BUFFER_INDEX].y1 = sxy[1] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x2 = sxy[2];
    sprite->quads[FRAME_BUFFER_INDEX].y2 = sxy[2] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x3 = sxy[3];
    sprite->quads[FRAME_BUFFER_INDEX].y3 = sxy[3] >> 16;
    addPrim(&CURRENT_FRAME_BUFFER->ot[sprite->otz], &sprite->quads[FRAME_BUFFER_INDEX]);
}

void SAI_moveSpriteCorners(Sprite3D *sprite, s16 dx, s16 dy) {
    sprite->corners[2].vx += dx;
    sprite->corners[0].vx = sprite->corners[2].vx;
    sprite->corners[3].vx += dx;
    sprite->corners[1].vx = sprite->corners[3].vx;
    sprite->corners[1].vy += dy;
    sprite->corners[0].vy = sprite->corners[1].vy;
    sprite->corners[3].vy += dy;
    sprite->corners[2].vy = sprite->corners[3].vy;
}

void SAI_setSpriteDepth(Sprite3D *sprite, s32 otz) {
    sprite->otz = otz;
}

void SAI_setSpriteBrightness(Unk801EBD94 *quads, u8 value) {
    s32 i;

    for (i = 0; i < 2; i++) {
        quads[i].r = value;
        quads[i].g = value;
        quads[i].b = value;
    }
}

void SAI_setSpriteSize(Sprite3D *sprite, s16 width, s16 height) {
    s32 halfWidth;
    s32 halfHeight;

    halfWidth = width >> 1;
    sprite->corners[2].vx = -halfWidth;
    sprite->corners[0].vx = -halfWidth;
    sprite->corners[3].vx = halfWidth;
    sprite->corners[1].vx = halfWidth;
    halfHeight = height >> 1;
    sprite->corners[1].vy = -halfHeight;
    sprite->corners[0].vy = -halfHeight;
    sprite->corners[3].vy = halfHeight;
    sprite->corners[2].vy = halfHeight;
}

void SAI_setSpriteBlendMode(Sprite3D *sprite, s32 abr) {
    s16 tpage;

    if (abr >= 0) {
        SetSemiTrans(&sprite->quads[0], 1);
        SetSemiTrans(&sprite->quads[1], 1);
        tpage = sprite->quads[0].tpage & ~0x60;
        tpage |= (abr & 3) << 5;
        sprite->quads[1].tpage = tpage;
        sprite->quads[0].tpage = tpage;
    } else {
        SetSemiTrans(&sprite->quads[0], 0);
        SetSemiTrans(&sprite->quads[1], 0);
    }
}

void SAI_initPathPolys(void) {
    PolyF4 *poly;
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        DB(i).primSlots[1] = (s32)(SAI_MAP_PATH_POLYS[i] = allocHeapBlock(20 * sizeof(PolyF4), 0x28));
        poly = SAI_MAP_PATH_POLYS[i];
        for (j = 0; j < 20; j++, poly++) {
            func_80067784(poly);
            poly->r0 = 0x7F;
            poly->g0 = 0x44;
            poly->b0 = 0xC;
            SetSemiTrans(poly, 1);
            SetDrawTPage(&SAI_MAP_PATH_TPAGES[i][j], 0, 0, 0x20);
        }
    }
}

void SAI_loadMapTextures(s32 useMap) {
    /* not a literal: GCC would share SAI_loadAreaTextures's identical string */
    static const char worldPath[] = "C:\\OBJECT\\world.TIS";
    char path[0x48];
    u32 *pack;

    *(s32 *)&((SessionData *)D_8006E054)->unk100C->unk0[0x194] = 1;
    if (useMap == 0) {
        sprintf(path, worldPath);
    } else {
        sprintf(path, "C:\\OBJECT\\map.TIS");
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    *(s32 *)&((SessionData *)D_8006E054)->unk100C->unk0[0x194] = 0;
}

void SAI_initCamera(void) {
    Graphics *camera;

    initScene3D(0);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->unk8E = 0;
    camera->unk90 = 0x1C0;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
}

s32 SAI_isSavedScriptFlagSet(u32 id) {
    u32 word;
    s32 mask;

    id -= 12;
    word = id >> 5;
    id &= 31;
    return (((PlayerProfile *)PLAYER_PROFILES)->unk23FC[word] & (mask = 1 << id)) != 0;
}

void SAI_slideIconToTop(void) {
    s32 t = SAI_WORLD_MAP.iconSlide;

    SAI_WORLD_MAP.iconX = (t * 270 + (15 - t) * 136) / 15;
    SAI_WORLD_MAP.iconY = (t * 9 + (15 - t) * 96) / 15;
}

void SAI_runCornerIcon(s32 state) {
    Rect16 uv[2];
    u8 brightness;
    s32 dir;

    if (state == 3) {
        state = 2;
    } else if (state == 4) {
        /* not a Rect16: GCC would share SAI_glitchVram's identical constant */
        s16 rect[4] = { 0x220, 0xE1, 0x20, 1 };

        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
        func_80014C08(2);
        MoveImage2((Rect16 *)rect, 0x380, 0x80);
        setBackgroundScrollMode(1);
        state = 2;
    } else {
        changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    }
    uv[0].x = 0;
    uv[0].y = 0;
    uv[0].w = 0x30;
    uv[0].h = 0x30;
    uv[1].x = 0x30;
    uv[1].y = 0;
    uv[1].w = 0x30;
    uv[1].h = 0x30;
    brightness = 0x80;
    dir = 1;
    SAI_WORLD_MAP.iconX = 0x88;
    SAI_WORLD_MAP.iconY = 0x60;
    SAI_WORLD_MAP.iconRunning = 1;
    SAI_WORLD_MAP.iconMotion = state;
    do {
        func_80014C08(1);
        if (SAI_ICON_MOTION_FUNCS[SAI_WORLD_MAP.iconMotion] != NULL) {
            SAI_ICON_MOTION_FUNCS[SAI_WORLD_MAP.iconMotion]();
        }
        if (dir == 1) {
            brightness += 4;
        } else if (dir == -1) {
            brightness -= 4;
        }
        if (brightness >= 0xD8) {
            brightness = 0xD8;
            dir = -1;
        } else if (brightness < 0x70) {
            brightness = 0x70;
            dir = 1;
        }
        drawTexturedSprite(SAI_WORLD_MAP.iconX, SAI_WORLD_MAP.iconY, &uv[1], 0x28, 0x39E0, 0x19, brightness, 1);
        drawTexturedSprite(SAI_WORLD_MAP.iconX, SAI_WORLD_MAP.iconY, &uv[0], 8, 0x3960, 0x19, 0x80, -1);
    } while (SAI_WORLD_MAP.iconRunning != 0);
}

void func_801EC41C(void) {
    SAI_WORLD_MAP.iconSlide++;
    if (SAI_WORLD_MAP.iconSlide > 15) {
        SAI_WORLD_MAP.iconSlide = 15;
    }
    SAI_WORLD_MAP.zoomAngle = (SAI_WORLD_MAP.zoom << 10) / 35;
    SAI_SPRITES[0]->pos.vx = SAI_SPRITES[1]->pos.vx = SAI_WORLD_MAP.iconSlide * 134 / 15;
    SAI_SPRITES[0]->pos.vy = SAI_SPRITES[1]->pos.vy = SAI_WORLD_MAP.iconSlide * 87 / 15;
}

void SAI_slideIconToBottom(void) {
    s32 t = SAI_WORLD_MAP.iconSlide;

    SAI_WORLD_MAP.iconX = (t * 270 + (15 - t) * 136) / 15;
    SAI_WORLD_MAP.iconY = (t * 183 + (15 - t) * 96) / 15;
}

void SAI_initRegion(void) {
    s32 i;
    s8 node;

    SAI_WORLD_MAP.labelRegion = SAI_WORLD_MAP.region;
    SAI_WORLD_MAP.route = SAI_REGION_NODES[SAI_WORLD_MAP.region];
    SAI_WORLD_MAP.node = &SAI_MAP_NODES[SAI_WORLD_MAP.nodeIndex];
    SAI_SPRITES[0] = SAI_createSprite(0);
    SAI_SPRITES[0]->pos.vx = SAI_MAP_NODES[SAI_WORLD_MAP.nodeIndex].x;
    SAI_SPRITES[0]->pos.vy = SAI_MAP_NODES[SAI_WORLD_MAP.nodeIndex].y - 15;
    SAI_setSpriteDepth(SAI_SPRITES[0], 0x1E);
    SAI_SPRITES[1] = SAI_createSprite(1);
    SAI_setSpriteDepth(SAI_SPRITES[1], 0x1F);
    for (i = 0; i < 2; i++) {
        SetSemiTrans(&SAI_SPRITES[1]->quads[i], 1);
        SAI_SPRITES[1]->quads[i].tpage = 0xC7;
    }
    SAI_WORLD_MAP.alpha = 0xFF;
    SAI_WORLD_MAP.lastRegion = SAI_WORLD_MAP.region;
    SAI_WORLD_MAP.markerCount = 0;
    for (i = 0; i < 7; i++) {
        node = SAI_REGION_NODES[SAI_WORLD_MAP.region][i];
        if (node < 0) {
            continue;
        }
        if (node >= 12) {
            SAI_SPRITES[i + 21] = SAI_createSprite(0x40);
        } else {
            SAI_SPRITES[i + 21] = SAI_createSprite(9);
        }
        SAI_moveSpriteCorners(SAI_SPRITES[i + 21], SAI_MAP_NODES[node].x, SAI_MAP_NODES[node].y);
        SAI_setSpriteDepth(SAI_SPRITES[i + 21], 0x20);
        SAI_WORLD_MAP.markerCount++;
    }
    SAI_initMapPaths();
}

void SAI_tickMapMenu(void) {
    s32 x;
    s32 i;

    if (SAI_MAP_MENU_TAB.offset != 0) {
        return;
    }
    x = ((20 - SAI_WORLD_MAP.menuSlide) * -214 + SAI_WORLD_MAP.menuSlide * -117) / 20;
    if (SAI_WORLD_MAP.menuPhase == 0) {
        SAI_WORLD_MAP.menuSlide++;
        if (SAI_WORLD_MAP.menuSlide > 20) {
            SAI_WORLD_MAP.menuSlide = 20;
            SAI_WORLD_MAP.menuPhase = 1;
            SAI_WORLD_MAP.menuCursor = 0;
        }
    } else if (SAI_WORLD_MAP.menuPhase == 1) {
        if (PAD_STATES[0]->repeat & PAD_DOWN) {
            playSoundEffect(2);
            SAI_WORLD_MAP.menuCursor++;
            if (SAI_WORLD_MAP.menuCursor >= 3) {
                SAI_WORLD_MAP.menuCursor = 0;
            }
        } else if (PAD_STATES[0]->repeat & PAD_UP) {
            playSoundEffect(2);
            SAI_WORLD_MAP.menuCursor--;
            if (SAI_WORLD_MAP.menuCursor < 0) {
                SAI_WORLD_MAP.menuCursor = 2;
            }
        } else if (PAD_STATES[0]->pressed & (PAD_TRIANGLE | PAD_CIRCLE)) {
            playSoundEffect(1);
            SAI_WORLD_MAP.menuPhase = 2;
            SAI_WORLD_MAP.menuChosen = 0;
        } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
            if (((SessionData *)D_8006E054)->unk1027 != 1 || SAI_MAP_MENU_CURSOR != 2) {
                playSoundEffect(0);
                SAI_WORLD_MAP.menuPhase = 2;
                SAI_WORLD_MAP.menuChosen = 1;
                SAI_MAP_MENU_TAB_STATE = 3;
            }
        }
    } else if (SAI_WORLD_MAP.menuPhase == 2) {
        SAI_WORLD_MAP.menuSlide--;
        if (SAI_WORLD_MAP.menuSlide < 0) {
            SAI_WORLD_MAP.menuSlide = 0;
            SAI_WORLD_MAP.menuPhase = 3;
        }
    } else if (SAI_WORLD_MAP.menuPhase == 3) {
        if (SAI_WORLD_MAP.menuChosen == 0) {
            SAI_WORLD_MAP.state = MAP_IDLE;
        } else {
            SAI_MAP_STATE = MAP_ENTER_AREA;
        }
        SAI_MAP_MENU_PHASE = 4;
    }
    for (i = 30; i < 34; i++) {
        if (i == 30) {
            SAI_SPRITES[i]->pos.vx = x + 1;
        } else {
            SAI_SPRITES[i]->pos.vx = x + 3;
        }
        if (i != 30) {
            if (((SessionData *)D_8006E054)->unk1027 == 1) {
                if (SAI_WORLD_MAP.menuCursor == i - 31) {
                    if (i == 33) {
                        SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF1);
                        SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF1);
                    } else {
                        SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF0);
                        SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF0);
                    }
                } else if (i == 33) {
                    SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF2);
                    SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF2);
                } else {
                    SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xEF);
                    SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xEF);
                }
            } else if (SAI_WORLD_MAP.menuCursor == i - 31) {
                SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xF0);
                SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xF0);
            } else {
                SAI_SPRITES[i]->quads[0].clut = getClut(0x200, 0xEF);
                SAI_SPRITES[i]->quads[1].clut = getClut(0x200, 0xEF);
            }
        }
        if (SAI_WORLD_MAP.menuPhase < 3) {
            SAI_drawSprite(SAI_SPRITES[i]);
        } else if (SAI_WORLD_MAP.menuPhase == 4) {
            if (SAI_MAP_MENU_TAB.state != 3) {
                SAI_MAP_MENU_TAB.state = 1;
            }
            SAI_freeSprite(SAI_SPRITES[i]);
        }
    }
}

void SAI_initMapAnims(s8 mode) {
    SAI_MAP_ANIM_REGION = mode;
    switch (mode) {
    case 0:
        SAI_initRegion0Anims();
        break;
    case 1:
        SAI_initRegion1Anims();
        break;
    case 2:
        SAI_initRegion2Anims();
        break;
    }
}

void SAI_initRegion0Anims(void) {
    s16 xs[6] = { 0xE4, 0xE4, 0xE4, 0xE4, 0xCF, 0x8E };
    s16 ys[6] = { 0x54, 0x4B, 0x4C, 0x4C, 0x30, 0x4B };
    s32 i;

    for (i = 0; i < 6; i++) {
        SAI_MAP_ANIMS[i].x = xs[i];
        SAI_MAP_ANIMS[i].y = ys[i];
        SAI_MAP_ANIMS[i].timer = 0;
        SAI_MAP_ANIMS[i].frames = 6;
        SAI_MAP_ANIMS[i].frameTime = 4;
    }
    SAI_MAP_ANIMS[4].frameTime = 8;
    for (i = 0; i < 6; i++) {
        SAI_MAP_ANIMS[i].frameTime *= 2;
    }
}

void SAI_initRegion1Anims(void) {
    s32 x = 0xAB;
    s32 y = 0x28;

    SAI_MAP_ANIMS->x = x;
    SAI_MAP_ANIMS->y = y;
    SAI_MAP_ANIMS->timer = 0;
    SAI_MAP_ANIMS->frames = 4;
    SAI_MAP_ANIMS->frameTime = 6;
    SAI_MAP_ANIMS->frameTime *= 2;
}

void SAI_initRegion2Anims(void) {
    s16 xs[7] = { 0x6B, 0x9E, 0x90, 0xA8, 0xCA, 0xDC, 0xD7 };
    s16 ys[7] = { 0x30, 0x60, 0x75, 0x8B, 0x8F, 0x7A, 0x62 };

    SAI_MAP_ANIMS->x = xs[0];
    SAI_MAP_ANIMS->y = ys[0];
    SAI_MAP_ANIMS->timer = 0;
    SAI_MAP_ANIMS->frames = 6;
    SAI_MAP_ANIMS->frameTime = 4;
    SAI_MAP_ANIMS->frameTime *= 2;
}

void SAI_drawMapAnims(s8 mode) {
    switch (mode) {
    case 0:
        SAI_drawRegion0Anims();
        break;
    case 1:
        SAI_drawRegion1Anims();
        break;
    case 2:
        SAI_drawRegion2Anims();
        break;
    }
}

void SAI_drawRegion0Anims(void) {
    Rect16 uv[8] = {
        { 0x00, 0x00, 0x20, 0x10 },
        { 0x20, 0x00, 0x20, 0x10 },
        { 0x40, 0x00, 0x20, 0x10 },
        { 0x60, 0x00, 0x20, 0x10 },
        { 0x80, 0x00, 0x28, 0x20 },
        { 0xD0, 0x00, 0x20, 0x10 },
        { 0x50, 0x00, 0x20, 0x1A },
        { 0x50, 0x1A, 0x20, 0x20 },
    };
    s32 i;
    u8 frame;

    for (i = 0; i < 6; i++) {
        SAI_MAP_ANIMS[i].timer++;
        if (SAI_MAP_ANIMS[i].timer >= SAI_MAP_ANIMS[i].frames * SAI_MAP_ANIMS[i].frameTime) {
            SAI_MAP_ANIMS[i].timer = 0;
        }
        frame = SAI_MAP_ANIMS[i].timer / SAI_MAP_ANIMS[i].frameTime;
        if (i == 4) {
            uv[4].x += uv[4].w * (frame / 3);
            uv[4].y = (frame % 3) * uv[4].h;
            drawTexturedSprite(SAI_MAP_ANIMS[i].x, SAI_MAP_ANIMS[i].y, &uv[4], 0x3E, 0x7C3B, 0x23, 0x80, 1);
        } else {
            uv[i].y = frame * uv[i].h;
            drawTexturedSprite(SAI_MAP_ANIMS[i].x, SAI_MAP_ANIMS[i].y, &uv[i], 0x3E, 0x7C3B, 0x21, 0x80, 1);
        }
    }
    drawTexturedSprite(0xCC, 0x74, &uv[6], 0x9B, 0x6A18, 0x21, 0x80, 1);
    drawTexturedSprite(0x8F, 0x46, &uv[7], 0x9B, 0x6A18, 0x21, 0x80, 1);
}

void SAI_drawRegion1Anims(void) {
    Rect16 uv[4] = {
        { 0x40, 0x00, 0x48, 0x48 },
        { 0x20, 0x60, 0x48, 0x48 },
        { 0x20, 0xA8, 0x48, 0x48 },
        { 0x20, 0x60, 0x48, 0x48 },
    };
    u8 frame;

    SAI_MAP_ANIMS->timer++;
    if (SAI_MAP_ANIMS->timer >= SAI_MAP_ANIMS->frames * SAI_MAP_ANIMS->frameTime) {
        SAI_MAP_ANIMS->timer = 0;
    }
    frame = SAI_MAP_ANIMS->timer / SAI_MAP_ANIMS->frameTime;
    if (frame != 0) {
        drawTexturedSprite(SAI_MAP_ANIMS->x, SAI_MAP_ANIMS->y, &uv[frame], 0x9E, 0x6A58, 0x23, 0x80, -1);
    }
}

void SAI_drawRegion2Anims(void) {
    Rect16 uv[8] = {
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
        { 0x00, 0x60, 0x20, 0x18 },
        { 0x50, 0x3A, 0x18, 0x40 },
    };
    u8 frame;

    drawTexturedSprite(0xBE, 0x28, &uv[1], 0x9B, 0x6A98, 0x1D, 0x80, 1);
    SAI_MAP_ANIMS->timer++;
    if (SAI_MAP_ANIMS->timer >= SAI_MAP_ANIMS->frames * SAI_MAP_ANIMS->frameTime) {
        SAI_MAP_ANIMS->timer = 0;
    }
    frame = SAI_MAP_ANIMS->timer / SAI_MAP_ANIMS->frameTime;
    if (frame != 0) {
        uv[0].y = (frame - 1) * uv[0].h + 0x60;
        drawTexturedSprite(SAI_MAP_ANIMS->x, SAI_MAP_ANIMS->y, &uv[0], 0x9E, 0x6A98, 0x23, 0x80, -1);
    }
}

void SAI_openMap(void) {
    s32 i;

    SAI_WORLD_MAP.zoom++;
    if (SAI_WORLD_MAP.zoom >= 36) {
        SAI_WORLD_MAP.zoom = 35;
    }
    if (SAI_MAP_ICON_SLIDE == 5) {
        playSoundEffect(10);
    }
    if (SAI_WORLD_MAP.zoom > 20) {
        SAI_WORLD_MAP.iconMotion = 1;
        SAI_WORLD_MAP.iconSlide++;
        if (SAI_WORLD_MAP.iconSlide >= 16) {
            SAI_WORLD_MAP.iconSlide = 15;
            if (SAI_MAP_FRAME_BRIGHTNESS < 0x80) {
                SAI_MAP_FRAME_BRIGHTNESS += 8;
            } else {
                SAI_WORLD_MAP.state = MAP_FADE_IN;
                SAI_initRegion();
                SAI_WORLD_MAP.portraitState = 2;
                SAI_WORLD_MAP.nameSlideDir = 1;
            }
            if (SAI_MAP_FRAME_BRIGHTNESS >= 0x80) {
                SAI_MAP_FRAME_BRIGHTNESS = 0x80;
            }
        }
    }
    SAI_WORLD_MAP.zoomAngle = (SAI_WORLD_MAP.zoom << 10) / 35;
    if (SAI_WORLD_MAP.zoomAngle > 0x400) {
        SAI_WORLD_MAP.zoomAngle = 0x400;
    }
    for (i = 40; i < 48; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.iconSlide * 38 / 15;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.iconSlide / 15;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = -(SAI_WORLD_MAP.zoom << 12) / 35;
    }
    for (i = 48; i < 56; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.iconSlide * 38 / 15;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.iconSlide / 15;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
    }
    setRGB0(&SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
    SAI_SPRITES[2]->pos.vx = SAI_WORLD_MAP.iconSlide * 38 / 15;
    SAI_SPRITES[2]->pos.vy = SAI_WORLD_MAP.iconSlide / 15;
    SAI_SPRITES[2]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
    SAI_SPRITES[2]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
}

void SAI_drawRegionFade(void) {
    if (SAI_MAP_STATE == MAP_FADE_IN || SAI_MAP_STATE == MAP_OPENING) {
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].r0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].g0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].b0 = SAI_WORLD_MAP.alpha;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x0 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x0;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y0 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y0;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x1 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x1;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y1 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y1;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x2 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x2;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y2 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y2;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].x3 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].x3;
        SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].y3 = SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX].y3;
        addPrim(&CURRENT_FRAME_BUFFER->ot[28], &SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[28], &SAI_WORLD_MAP.tpages[FRAME_BUFFER_INDEX]);
    }
}

void SAI_openMapMenu(void) {
    s32 i;

    SAI_MAP_MENU_TAB_STATE = 2;
    SAI_WORLD_MAP.state = MAP_MENU;
    SAI_WORLD_MAP.menuPhase = 0;
    SAI_WORLD_MAP.menuSlide = 0;
    SAI_WORLD_MAP.menuCursor = 100;
    SAI_SPRITES[30] = SAI_createSprite(0x16);
    SAI_SPRITES[31] = SAI_createSprite(0x18);
    SAI_SPRITES[32] = SAI_createSprite(0x19);
    SAI_SPRITES[33] = SAI_createSprite(0x17);
    if (((SessionData *)D_8006E054)->unk1027 == 1) {
        SAI_SPRITES[33]->quads[0].clut = getClut(0x200, 0xF1);
        SAI_SPRITES[33]->quads[1].clut = getClut(0x200, 0xF1);
    }
    SAI_SPRITES[30]->pos.vy = -0x24;
    SAI_setSpriteDepth(SAI_SPRITES[30], 0x1A);
    SAI_SPRITES[31]->pos.vy = -0x39;
    SAI_setSpriteDepth(SAI_SPRITES[31], 0x1B);
    SAI_SPRITES[32]->pos.vy = -0x29;
    SAI_setSpriteDepth(SAI_SPRITES[32], 0x1B);
    SAI_SPRITES[33]->pos.vy = -0x19;
    SAI_setSpriteDepth(SAI_SPRITES[33], 0x1B);
    SAI_setSpriteBlendMode(SAI_SPRITES[30], 1);
    for (i = 0; i < 3; i++) {
        SAI_setSpriteBlendMode(SAI_SPRITES[i + 31], 0);
    }
}

void SAI_fadeInRegion(void) {
    s32 value;

    if (SAI_WORLD_MAP.state == MAP_FADE_IN) {
        value = SAI_WORLD_MAP.alpha - 8;
        if (value < 0) {
            value = 0;
            if (SAI_WORLD_MAP.openMenu == 0) {
                SAI_WORLD_MAP.state = MAP_IDLE;
                SAI_MAP_MENU_TAB_STATE = 1;
            } else {
                SAI_MAP_OPEN_MENU = 0;
                SAI_openMapMenu();
            }
        }
        SAI_MAP_ALPHA = value;
    }
}

void SAI_selectMapNode(void) {
    if (SAI_WORLD_MAP.nodeIndex >= 0 && SAI_WORLD_MAP.nodeIndex < 12) {
        SAI_WORLD_MAP.state = MAP_ENTER_AREA;
    } else if (SAI_WORLD_MAP.nodeIndex == 12) {
        SAI_WORLD_MAP.nodeIndex = 13;
        SAI_WORLD_MAP.region = 1;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    } else if (SAI_WORLD_MAP.nodeIndex == 13) {
        SAI_WORLD_MAP.nodeIndex = 12;
        SAI_WORLD_MAP.region = 0;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    } else if (SAI_WORLD_MAP.nodeIndex == 14) {
        SAI_WORLD_MAP.nodeIndex = 15;
        SAI_WORLD_MAP.region = 2;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    } else if (SAI_WORLD_MAP.nodeIndex == 15) {
        SAI_WORLD_MAP.nodeIndex = 14;
        SAI_WORLD_MAP.region = 1;
        SAI_WORLD_MAP.state = MAP_CHANGE_REGION;
        playSoundEffect(5);
    }
    SAI_MAP_LABEL_REGION = -1;
}

void SAI_tickMapInput(void) {
    s32 dir = -1;
    s32 dx;
    s32 dy;

    if (SAI_WORLD_MAP.moving == 0) {
        stopSoundVoice(0x17);
        if (PAD_STATES[0]->held & PAD_UP) {
            dir = 0;
        } else if (PAD_STATES[0]->held & PAD_RIGHT) {
            dir = 1;
        } else if (PAD_STATES[0]->held & PAD_DOWN) {
            dir = 2;
        } else if ((u16)PAD_STATES[0]->held & PAD_LEFT) {
            dir = 3;
        } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
            dir = 4;
        } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
            dir = 5;
        }
        if (dir == -1) {
            return;
        }
        if (dir == 4) {
            SAI_selectMapNode();
        } else if (dir == 5) {
            playSoundEffect(1);
            SAI_openMapMenu();
        } else {
            if (SAI_WORLD_MAP.node->next[dir] != -1) {
                SAI_WORLD_MAP.target = &SAI_MAP_NODES[SAI_WORLD_MAP.node->next[dir]];
            } else {
                if (PAD_STATES[0]->pressed & PAD_CROSS) {
                    SAI_selectMapNode();
                } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
                    playSoundEffect(1);
                    SAI_openMapMenu();
                }
                return;
            }
            if (SAI_WORLD_MAP.target->unlocked != 1) {
                if (PAD_STATES[0]->pressed & PAD_CROSS) {
                    SAI_selectMapNode();
                } else if (PAD_STATES[0]->pressed & PAD_TRIANGLE) {
                    playSoundEffect(1);
                    SAI_openMapMenu();
                }
                return;
            }
            playSoundEffectOnVoice(0x17, 7);
            SAI_WORLD_MAP.nodeIndex = SAI_WORLD_MAP.node->next[dir];
            SAI_WORLD_MAP.moving++;
            dx = SAI_WORLD_MAP.target->x - SAI_WORLD_MAP.node->x;
            dy = SAI_WORLD_MAP.target->y - SAI_WORLD_MAP.node->y;
            SAI_WORLD_MAP.angle = ratan2(dy, dx);
            SAI_WORLD_MAP.distance = csqrt(dx * dx + dy * dy);
            SAI_WORLD_MAP.distance /= 64;
            SAI_MAP_STATE = MAP_WALKING;
        }
    }
}

void SAI_setSpriteImage8Bit(Sprite3D *sprite, Rect16 *rect, s8 flip) {
    s32 i;

    for (i = 0; i < 2; i++) {
        sprite->quads[i].tpage = 0x80 | ((rect->y & 0x100) >> 4) | ((rect->x & 0x3C0) >> 6) | ((rect->y & 0x200) << 2);
        if (flip == -1) {
            sprite->quads[i].u0 = sprite->quads[i].u2 = ((rect->x % 64) << 1) + rect->w;
            sprite->quads[i].u1 = sprite->quads[i].u3 = (rect->x % 64) << 1;
        } else {
            sprite->quads[i].u0 = sprite->quads[i].u2 = (rect->x % 64) << 1;
            sprite->quads[i].u1 = sprite->quads[i].u3 = sprite->quads[i].u2 + rect->w;
        }
        sprite->quads[i].v0 = sprite->quads[i].v1 = rect->y;
        sprite->quads[i].v2 = sprite->quads[i].v3 = rect->y + rect->h;
    }
}

void SAI_setSpriteImage4Bit(Sprite3D *sprite, Rect16 *rect) {
    s32 i;

    for (i = 0; i < 2; i++) {
        sprite->quads[i].tpage = ((rect->y & 0x100) >> 4) | ((rect->x & 0x3C0) >> 6) | ((rect->y & 0x200) << 2);
        sprite->quads[i].u0 = sprite->quads[i].u2 = (rect->x % 64) << 2;
        sprite->quads[i].u1 = sprite->quads[i].u3 = sprite->quads[i].u2 + rect->w;
        sprite->quads[i].v0 = sprite->quads[i].v1 = rect->y;
        sprite->quads[i].v2 = sprite->quads[i].v3 = rect->y + rect->h;
    }
}

void SAI_walkMapMarker(void) {
    Rect16 rect;
    s32 angle = SAI_WORLD_MAP.angle & 0xFFF;
    s8 flip = 0;

    SAI_WORLD_MAP.moving++;
    if (SAI_WORLD_MAP.moving >= SAI_WORLD_MAP.distance) {
        SAI_WORLD_MAP.moving = 0;
        SAI_SPRITES[0]->pos.vx = SAI_WORLD_MAP.target->x;
        SAI_SPRITES[0]->pos.vy = SAI_WORLD_MAP.target->y - 15;
        SAI_WORLD_MAP.node = SAI_WORLD_MAP.target;
        SAI_WORLD_MAP.state = MAP_IDLE;
    } else {
        SAI_SPRITES[0]->pos.vx = rcos(SAI_WORLD_MAP.angle) * SAI_WORLD_MAP.moving / 4096;
        SAI_SPRITES[0]->pos.vy = rsin(SAI_WORLD_MAP.angle) * SAI_WORLD_MAP.moving / 4096;
        SAI_SPRITES[0]->pos.vx += SAI_WORLD_MAP.node->x;
        SAI_SPRITES[0]->pos.vy += SAI_WORLD_MAP.node->y - 15;
    }
    if (angle < 0x400) {
        flip = -1;
        rect.x = SAI_WORLD_MAP.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xA8;
    } else if (angle < 0x800) {
        rect.x = SAI_WORLD_MAP.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xA8;
    } else {
        if (angle >= 0xC00) {
            flip = -1;
        }
        rect.x = SAI_WORLD_MAP.moving / 6 % 6 * 13 + 0x19A;
        rect.y = 0xD0;
    }
    rect.w = 0x1A;
    rect.h = 0x28;
    if (SAI_WORLD_MAP.moving == 0) {
        rect.x = 0x180;
    }
    SAI_setSpriteImage8Bit(SAI_SPRITES[0], &rect, flip);
}

void SAI_fadeOutRegion(void) {
    s32 alpha = SAI_WORLD_MAP.alpha + 8;

    if (alpha > 0xFF) {
        alpha = 0xFF;
        SAI_WORLD_MAP.oldMarkerCount = SAI_WORLD_MAP.markerCount;
        SAI_WORLD_MAP.markerCount = 0;
        if (SAI_WORLD_MAP.state == MAP_CHANGE_REGION) {
            SAI_WORLD_MAP.state = MAP_SWITCH_REGION;
            SAI_setRegionMapImage();
        } else {
            if (SAI_WORLD_MAP.menuChosen != 1) {
                ((PlayerProfile *)PLAYER_PROFILES)->unkE = SESSION->area = SAI_WORLD_MAP.nodeIndex;
                func_800149B8(0, -1, 0, 0x400, SAI_loadAreaPak, 0, getCurrentTaskId, 0, 0);
            }
            SAI_MAP_MENU_TAB_STATE = 3;
            SAI_WORLD_MAP.labelRegion = -1;
            SAI_WORLD_MAP.nameSlideDir = 2;
            SAI_WORLD_MAP.portraitState = 4;
            SAI_WORLD_MAP.state = MAP_CLOSING;
            playSoundEffect(11);
        }
        removeFrameCallback((s32)SAI_drawMapPaths);
        SAI_MAP_ANIMATING = 0;
    }
    SAI_WORLD_MAP.alpha = alpha;
    SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].r0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].g0 = SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX].b0 = alpha;
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], &SAI_WORLD_MAP.fades[FRAME_BUFFER_INDEX]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], &SAI_WORLD_MAP.tpages[FRAME_BUFFER_INDEX]);
}

void SAI_switchRegion(void) {
    s32 i = 0;

    if (SAI_MAP_OLD_MARKER_COUNT != 0) {
        do {
            SAI_freeSprite(SAI_SPRITES[i + 21]);
            i++;
        } while (i < SAI_WORLD_MAP.oldMarkerCount);
    }
    SAI_freeSprite(SAI_SPRITES[0]);
    SAI_freeSprite(SAI_SPRITES[1]);
    SAI_MAP_STATE = MAP_FADE_IN;
    SAI_initRegion();
}

void SAI_closeMap(void) {
    s32 i;

    if (SAI_WORLD_MAP.iconSlide == 0) {
        SAI_WORLD_MAP.zoom--;
        if (SAI_WORLD_MAP.zoom < 0) {
            SAI_WORLD_MAP.zoom = 0;
        }
        SAI_WORLD_MAP.zoomAngle = (SAI_WORLD_MAP.zoom << 10) / 35;
        if (SAI_WORLD_MAP.zoomAngle <= 0) {
            SAI_WORLD_MAP.zoomAngle = 0;
        }
    } else {
        SAI_WORLD_MAP.iconSlide--;
        if (SAI_WORLD_MAP.iconSlide < 0) {
            SAI_WORLD_MAP.iconSlide = 0;
        }
    }
    for (i = 40; i < 48; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.zoom * 38 / 35;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.zoom / 35;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = -(SAI_WORLD_MAP.zoom << 12) / 35;
    }
    for (i = 48; i < 56; i++) {
        setRGB0(&SAI_SPRITES[i]->quads[FRAME_BUFFER_INDEX], SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS, SAI_MAP_FRAME_BRIGHTNESS);
        SAI_SPRITES[i]->pos.vx = SAI_WORLD_MAP.zoom * 38 / 35;
        SAI_SPRITES[i]->pos.vy = SAI_WORLD_MAP.zoom / 35;
        SAI_SPRITES[i]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
        SAI_SPRITES[i]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
    }
    setRGB0(&SAI_SPRITES[2]->quads[FRAME_BUFFER_INDEX], 0, 0, 0);
    SAI_SPRITES[2]->pos.vx = SAI_WORLD_MAP.zoom * 38 / 35;
    SAI_SPRITES[2]->pos.vy = SAI_WORLD_MAP.zoom / 35;
    SAI_SPRITES[2]->pos.vz = rcos(SAI_WORLD_MAP.zoomAngle) * 2 / 3;
    SAI_SPRITES[2]->rot.vz = (SAI_WORLD_MAP.zoom << 12) / 35;
    SAI_MAP_FRAME_BRIGHTNESS -= 8;
    if (SAI_MAP_FRAME_BRIGHTNESS < 0x40) {
        SAI_MAP_FRAME_BRIGHTNESS = 0x40;
    }
    if (SAI_WORLD_MAP.zoomAngle == 0 && SAI_WORLD_MAP.zoom == 0) {
        SAI_WORLD_MAP.state = MAP_DONE;
    }
}

void func_801EF1A8(void) {
    SAI_MAP_ACTIVE = 0;
}

void SAI_createRegionLabel(void) {
    SAI_WORLD_MAP.shownLabelRegion = -1;
    SAI_WORLD_MAP.labelRegion = -1;
    SAI_WORLD_MAP.labelSlide = 0;
    SAI_SPRITES[4] = SAI_createSprite(6);
    SAI_SPRITES[4]->pos.vx = -0x5F;
    SAI_SPRITES[4]->pos.vy = -0x98;
    SAI_setSpriteDepth(SAI_SPRITES[4], 0x18);
    SAI_SPRITES[3] = SAI_createSprite(7);
    SAI_SPRITES[3]->pos.vx = -0x5F;
    SAI_SPRITES[3]->pos.vy = -0x97;
    SAI_setSpriteDepth(SAI_SPRITES[3], 0x19);
    SAI_SPRITES[3]->quads[0].clut = SAI_SPRITES[3]->quads[1].clut = getClut(0x200, 0xEA);
}

void SAI_drawRegionLabel(void) {
    Rect16 rect;

    rect.x = 0x200;
    rect.y = SAI_WORLD_MAP.labelRegion * 24 + 0x30;
    rect.w = 0x74;
    rect.h = 0x18;
    if (SAI_WORLD_MAP.labelRegion != SAI_WORLD_MAP.shownLabelRegion || SAI_WORLD_MAP.labelRegion == -1) {
        SAI_WORLD_MAP.labelSlide--;
        if (SAI_WORLD_MAP.labelSlide < 0) {
            SAI_WORLD_MAP.labelSlide = 0;
            SAI_WORLD_MAP.shownLabelRegion = SAI_WORLD_MAP.labelRegion;
            if (SAI_WORLD_MAP.shownLabelRegion != -1) {
                SAI_setSpriteImage4Bit(SAI_SPRITES[3], &rect);
            }
        }
    } else {
        SAI_WORLD_MAP.labelSlide++;
        if (SAI_WORLD_MAP.labelSlide > 20) {
            SAI_WORLD_MAP.labelSlide = 20;
        }
    }
    SAI_SPRITES[4]->pos.vy = (SAI_WORLD_MAP.labelSlide * -97 + (20 - SAI_WORLD_MAP.labelSlide) * -152) / 20;
    SAI_SPRITES[3]->pos.vy = SAI_SPRITES[4]->pos.vy + 2;
    SAI_drawSprite(SAI_SPRITES[3]);
    SAI_drawSprite(SAI_SPRITES[4]);
}

void SAI_createAreaPortrait(void) {
    s32 i;

    if (SAI_MAP_PORTRAIT_STATE == 0) {
        SAI_SPRITES[35] = SAI_createSprite(0x1A);
        SAI_SPRITES[36] = SAI_createSprite(0x1B);
        SAI_SPRITES[37] = SAI_createSprite(0x1C);
        for (i = 35; i < 38; i++) {
            SAI_setSpriteDepth(SAI_SPRITES[i], 0x1B);
        }
        SAI_SPRITES[35]->pos.vy = 0x3B;
        SAI_SPRITES[36]->pos.vy = SAI_SPRITES[37]->pos.vy = 0x32;
        SAI_MAP_PORTRAIT_STATE = 1;
    }
    SAI_WORLD_MAP.portraitSlide = 0;
    SAI_WORLD_MAP.portraitBlink = 0;
}

void SAI_drawAreaPortrait(void) {
    Rect16 rect;

    SAI_SPRITES[35]->pos.vx = (SAI_WORLD_MAP.portraitSlide * -108 + (20 - SAI_WORLD_MAP.portraitSlide) * -200) / 20;
    SAI_SPRITES[36]->pos.vx = SAI_SPRITES[37]->pos.vx = SAI_SPRITES[35]->pos.vx - 7;
    SAI_WORLD_MAP.portraitBlink = (SAI_WORLD_MAP.portraitBlink + 1) & 0xFFF;
    if (SAI_WORLD_MAP.portraitState == 2) {
        SAI_WORLD_MAP.portraitSlide += 2;
        if (SAI_WORLD_MAP.portraitSlide > 20) {
            SAI_WORLD_MAP.portraitSlide = 20;
            SAI_WORLD_MAP.portraitState = 3;
        }
    } else if (SAI_WORLD_MAP.portraitState == 4) {
        SAI_WORLD_MAP.portraitSlide -= 2;
        if (SAI_WORLD_MAP.portraitSlide < 0) {
            SAI_WORLD_MAP.portraitSlide = 0;
            SAI_WORLD_MAP.portraitState = 1;
        }
    }
    SAI_drawSprite(SAI_SPRITES[35]);
    if (SAI_WORLD_MAP.nodeIndex >= 12 || SAI_WORLD_MAP.moving != 0 || SAI_WORLD_MAP.portraitSlide != 20) {
        if (SAI_WORLD_MAP.portraitBlink & 4) {
            rect.x = 0x318;
        } else {
            rect.x = 0x328;
        }
        rect.y = 0xC8;
        rect.w = 0x40;
        rect.h = 0x37;
        SAI_setSpriteImage4Bit(SAI_SPRITES[36], &rect);
        SAI_drawSprite(SAI_SPRITES[36]);
    } else {
        rect.x = SAI_MAP_NODE % 4 * 32 + 0x180;
        rect.y = SAI_MAP_NODE / 4 * 56;
        rect.w = 0x40;
        rect.h = 0x37;
        SAI_setSpriteImage8Bit(SAI_SPRITES[37], &rect, 0);
        SAI_drawSprite(SAI_SPRITES[37]);
    }
}

void SAI_createMapAreaName(void) {
    SAI_MAP_NAME_SLIDE_DIR = 0;
    SAI_SPRITES[38] = SAI_createSprite(0x1F);
    SAI_SPRITES[39] = SAI_createSprite(0x1E);
    SAI_SPRITES[38]->pos.vx = 0x26;
    SAI_SPRITES[39]->pos.vx = 0x25;
    SAI_setSpriteDepth(SAI_SPRITES[38], 0x19);
    SAI_setSpriteDepth(SAI_SPRITES[39], 0x19);
}

void SAI_drawMapAreaName(void) {
    Rect16 rect;
    s32 i;

    SAI_SPRITES[39]->pos.vy = (SAI_WORLD_MAP.nameSlide * 89 + (20 - SAI_WORLD_MAP.nameSlide) * 134) / 20;
    SAI_SPRITES[38]->pos.vy = SAI_SPRITES[39]->pos.vy + 1;
    if (SAI_WORLD_MAP.nameSlideDir == 1) {
        SAI_WORLD_MAP.nameSlide += 2;
        if (SAI_WORLD_MAP.nameSlide > 20) {
            SAI_WORLD_MAP.nameSlide = 20;
            SAI_WORLD_MAP.nameSlideDir = 0;
        }
    } else if (SAI_WORLD_MAP.nameSlideDir == 2) {
        SAI_WORLD_MAP.nameSlide -= 2;
        if (SAI_WORLD_MAP.nameSlide < 0) {
            SAI_WORLD_MAP.nameSlide = 0;
            SAI_WORLD_MAP.nameSlideDir = 0;
        }
    }
    if (SAI_MAP_NODE >= 12) {
        switch (SAI_MAP_NODE) {
        case 12:
            rect.x = 0x2C0;
            rect.y = 0x5A;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 13:
            rect.x = 0x2C0;
            rect.y = 0x46;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 14:
            rect.x = 0x2C0;
            rect.y = 0x6E;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        case 15:
            rect.x = 0x2C0;
            rect.y = 0x5A;
            rect.w = 0x60;
            rect.h = 0x14;
            break;
        }
    } else {
        rect.x = SAI_MAP_NODE / 6 * 24 + 0x340;
        rect.y = SAI_MAP_NODE % 6 * 20;
        rect.w = 0x60;
        rect.h = 0x14;
    }
    SAI_setSpriteImage4Bit(SAI_SPRITES[38], &rect);
    for (i = 38; i < 40; i++) {
        SAI_drawSprite(SAI_SPRITES[i]);
    }
}

void SAI_createMapFrame(void) {
    s16 xs[4] = { -0x80, -0x58, 0x58, 0x7C };
    s16 ys[4] = { -0x78, -0x48, 0x48, 0x78 };
    s32 i;
    s32 col;
    s32 row;

    for (i = 40; i < 48; i++) {
        SAI_SPRITES[i] = SAI_createSprite(i - 8);
        SAI_setSpriteDepth(SAI_SPRITES[i], 0x1A);
    }
    SAI_SPRITES[40]->corners[0].vx = SAI_SPRITES[40]->corners[2].vx = xs[1];
    SAI_SPRITES[40]->corners[1].vx = SAI_SPRITES[40]->corners[3].vx = xs[2];
    SAI_SPRITES[40]->corners[0].vy = SAI_SPRITES[40]->corners[1].vy = ys[0];
    SAI_SPRITES[40]->corners[2].vy = SAI_SPRITES[40]->corners[3].vy = ys[1];
    SAI_SPRITES[41]->corners[0].vx = SAI_SPRITES[41]->corners[2].vx = xs[1];
    SAI_SPRITES[41]->corners[1].vx = SAI_SPRITES[41]->corners[3].vx = xs[2];
    SAI_SPRITES[41]->corners[0].vy = SAI_SPRITES[41]->corners[1].vy = ys[2];
    SAI_SPRITES[41]->corners[2].vy = SAI_SPRITES[41]->corners[3].vy = ys[3];
    for (i = 0; i < 6; i++) {
        col = i / 3;
        row = i % 3;
        SAI_SPRITES[i + 42]->corners[0].vx = SAI_SPRITES[i + 42]->corners[2].vx = xs[col * 2];
        SAI_SPRITES[i + 42]->corners[1].vx = SAI_SPRITES[i + 42]->corners[3].vx = xs[col * 2 + 1];
        SAI_SPRITES[i + 42]->corners[0].vy = SAI_SPRITES[i + 42]->corners[1].vy = ys[row];
        SAI_SPRITES[i + 42]->corners[2].vy = SAI_SPRITES[i + 42]->corners[3].vy = ys[row + 1];
    }
}

void SAI_createMapFrameShadow(void) {
    s16 xs[4] = { -0x8B, -0x53, 0x5D, 0x89 };
    s16 ys[4] = { -0x75, -0x45, 0x43, 0x77 };
    s32 i;
    s32 col;
    s32 row;
    s32 id;

    for (i = 48, id = 40; i < 56; i++, id++) {
        SAI_SPRITES[i] = SAI_createSprite(id);
        SAI_setSpriteDepth(SAI_SPRITES[i], 0x1C);
    }
    SAI_SPRITES[48]->corners[0].vx = SAI_SPRITES[48]->corners[2].vx = xs[1];
    SAI_SPRITES[48]->corners[1].vx = SAI_SPRITES[48]->corners[3].vx = xs[2];
    SAI_SPRITES[48]->corners[0].vy = SAI_SPRITES[48]->corners[1].vy = ys[0];
    SAI_SPRITES[48]->corners[2].vy = SAI_SPRITES[48]->corners[3].vy = ys[1];
    SAI_SPRITES[49]->corners[0].vx = SAI_SPRITES[49]->corners[2].vx = xs[1];
    SAI_SPRITES[49]->corners[1].vx = SAI_SPRITES[49]->corners[3].vx = xs[2];
    SAI_SPRITES[49]->corners[0].vy = SAI_SPRITES[49]->corners[1].vy = ys[2];
    SAI_SPRITES[49]->corners[2].vy = SAI_SPRITES[49]->corners[3].vy = ys[3];
    for (i = 0; i < 6; i++) {
        col = i / 3;
        row = i % 3;
        SAI_SPRITES[i + 50]->corners[0].vx = SAI_SPRITES[i + 50]->corners[2].vx = xs[col * 2];
        SAI_SPRITES[i + 50]->corners[1].vx = SAI_SPRITES[i + 50]->corners[3].vx = xs[col * 2 + 1];
        SAI_SPRITES[i + 50]->corners[0].vy = SAI_SPRITES[i + 50]->corners[1].vy = ys[row];
        SAI_SPRITES[i + 50]->corners[2].vy = SAI_SPRITES[i + 50]->corners[3].vy = ys[row + 1];
    }
}

void SAI_drawMapFrame(void) {
    s32 i;

    for (i = 0x28; i < 0x30; i++) {
        SAI_drawSprite(SAI_SPRITES[i]);
    }
}

void SAI_drawMapFrameShadow(void) {
    s32 i;

    for (i = 0x30; i < 0x38; i++) {
        SAI_drawSprite(SAI_SPRITES[i]);
    }
}

void SAI_unlockMapNodes(void) {
    u16 ids[12] = { 0x11A, 0x11B, 0x11C, 0x11D, 0x11E, 0x11F, 0x120, 0x121, 0x122, 0x123, 0x124, 0x125 };
    s32 i;

    for (i = 0; i < 12; i++) {
        SAI_MAP_NODES[i].unlocked = SAI_isSavedScriptFlagSet(ids[i]);
    }
    for (i = 12; i < 16; i++) {
        SAI_MAP_NODES[i].unlocked = 0;
    }
    if (SAI_isSavedScriptFlagSet(ids[5])) {
        SAI_MAP_NODES[12].unlocked = 1;
        SAI_MAP_NODES[13].unlocked = 1;
    }
    if (SAI_isSavedScriptFlagSet(ids[9])) {
        SAI_MAP_NODES[14].unlocked = 1;
        SAI_MAP_NODES[15].unlocked = 1;
    }
}

void SAI_setRegionMapImage(void) {
    SAI_setSpriteImage8Bit(SAI_SPRITES[2], &SAI_REGION_MAP_RECTS[SAI_WORLD_MAP.region], 0);
    SAI_SPRITES[2]->quads[0].clut = SAI_SPRITES[2]->quads[1].clut = getClut(0x180, SAI_WORLD_MAP.region + 0x1A8);
}

/*
 * The world map task: the player's marker walks between the unlocked nodes of
 * the three regions; entering a node loads its area, and the menu leads to the
 * deck editor, the partner equipment or the save screen.
 */
void SAI_runWorldMap(s32 resume, s32 openMenu) {
    s32 i;
    s32 j;
    s8 node;

    if (resume == 0) {
        func_800149B8(0, -1, 0, 0x400, SAI_loadMapTextures, 0, getCurrentTaskId, 0, 0);
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
    }
    loadSoundEffectBank(1);
    SAI_initCamera();
    SAI_initPathPolys();
    SAI_unlockMapNodes();
    SAI_createMapMenuTab(0);
    if (SAI_ICON_RUNNING != 1) {
        func_800149B8(0, -1, 0, 0x400, SAI_runCornerIcon, 0, getCurrentTaskId(), 0, 0);
    }
    SAI_MAP_ANIMS = allocTaskHeapBlock(sizeof(MapAnim) * 6);
    SAI_initMapAnims(SAI_WORLD_MAP.region);
    SAI_createMapFrame();
    SAI_createMapFrameShadow();
    SESSION_SUB->unk1A4 = SAI_WORLD_MAP.nodeIndex = ((PlayerProfile *)PLAYER_PROFILES)->unkE;
    for (j = 0; j < 3; j++) {
        for (i = 0; i < 7; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->unkE == SAI_REGION_NODES[j][i]) {
                SAI_WORLD_MAP.region = j;
            }
        }
    }
    SAI_MAP_FRAME_BRIGHTNESS = 0x40;
    SAI_WORLD_MAP.active = 1;
    SAI_WORLD_MAP.iconSlide = 0;
    SAI_WORLD_MAP.zoom = 0;
    SAI_WORLD_MAP.state = MAP_OPENING;
    SAI_WORLD_MAP.portraitState = 0;
    SAI_WORLD_MAP.alpha = 0xFF;
    SAI_WORLD_MAP.menuChosen = 0;
    SAI_WORLD_MAP.markerCount = 0;
    SAI_WORLD_MAP.animating = 0;
    SAI_WORLD_MAP.lastRegion = SAI_WORLD_MAP.region;
    SAI_WORLD_MAP.openMenu = openMenu;
    SAI_createRegionLabel();
    SAI_createAreaPortrait();
    SAI_createMapAreaName();
    SAI_SPRITES[2] = SAI_createSprite(2);
    SAI_setRegionMapImage();
    for (i = 0; i < 2; i++) {
        setRGB0(&SAI_SPRITES[2]->quads[i], 0, 0, 0);
    }
    SAI_setSpriteDepth(SAI_SPRITES[2], 0x23);
    for (i = 0; i < 2; i++) {
        func_80067784(&SAI_WORLD_MAP.fades[i]);
        SetSemiTrans(&SAI_WORLD_MAP.fades[i], 1);
        SetDrawTPage(&SAI_WORLD_MAP.tpages[i], 0, 0, 0x40);
        SAI_WORLD_MAP.fades[i].r0 = SAI_WORLD_MAP.fades[i].g0 = SAI_WORLD_MAP.fades[i].b0 = 0xFF;
        SAI_WORLD_MAP.fades[i].x0 = SAI_WORLD_MAP.fades[i].x2 = 100;
        SAI_WORLD_MAP.fades[i].x1 = SAI_WORLD_MAP.fades[i].x3 = 0x128;
        SAI_WORLD_MAP.fades[i].y0 = SAI_WORLD_MAP.fades[i].y1 = 0x28;
        SAI_WORLD_MAP.fades[i].y2 = SAI_WORLD_MAP.fades[i].y3 = 0xC8;
    }
    do {
        func_80014C08(1);
        if (SAI_MAP_STATE_FUNCS[SAI_WORLD_MAP.state] != NULL) {
            SAI_MAP_STATE_FUNCS[SAI_WORLD_MAP.state]();
        }
        for (i = 0; i < SAI_WORLD_MAP.markerCount; i++) {
            node = SAI_REGION_NODES[SAI_WORLD_MAP.lastRegion][i];
            if (SAI_MAP_NODES[node].unlocked == 1) {
                SAI_drawSprite(SAI_SPRITES[i + 21]);
            }
        }
        if (SAI_WORLD_MAP.markerCount != 0) {
            SAI_drawSprite(SAI_SPRITES[0]);
            SAI_SPRITES[1]->pos.vx = SAI_SPRITES[0]->pos.vx;
            SAI_SPRITES[1]->pos.vy = SAI_SPRITES[0]->pos.vy + 15;
            SAI_drawSprite(SAI_SPRITES[1]);
        }
        SAI_drawMapMenuTab();
        SAI_drawMapFrame();
        SAI_drawMapFrameShadow();
        if (SAI_WORLD_MAP.animating != 0) {
            SAI_drawMapAnims(SAI_WORLD_MAP.animRegion);
        }
        SAI_drawSprite(SAI_SPRITES[2]);
        SAI_drawRegionFade();
        SAI_drawMapAreaName();
        SAI_drawRegionLabel();
        if (SAI_WORLD_MAP.portraitState >= 2) {
            SAI_drawAreaPortrait();
        }
    } while (SAI_WORLD_MAP.state != MAP_DONE);
    func_80014C08(1);
    freeHeapBlocksByTag(0x2E);
    SAI_WORLD_MAP.active = -1;
    func_80014A00(0x19);
    freeHeapBlocksByTag(0x28);
    freeHeapBlocksByTag(0x7F);
    freeHeapBlocksByTag(0x29);
    func_80014C08(0x1E);
    SESSION_SUB->unk1A2 = 0;
    SESSION_SUB->unk1A5[0] = 0;
    ((PlayerProfile *)PLAYER_PROFILES)->unkE = SESSION_SUB->unk1A4 = (u8)SAI_WORLD_MAP.nodeIndex;
    if (SAI_WORLD_MAP.menuChosen == 1) {
        SAI_WORLD_MAP.iconRunning = 0;
        switch (SAI_WORLD_MAP.menuCursor) {
        case 0:
            func_800149B8(0, -1, 0, 0x1600, SAI_openDeckEditorFromMap, 0, getCurrentTaskId(), 0, 0);
            break;
        case 1:
            func_800149B8(0, -1, 0, 0x1600, SAI_openEquipmentFromMap, 0, getCurrentTaskId(), 0, 0);
            break;
        case 2:
            ((PlayerProfile *)PLAYER_PROFILES)->unkF = 0;
            func_800149B8(0, -1, 0, 0x400, openSaveScreenFromMap, 2, getCurrentTaskId(), 0, 0);
            break;
        }
    } else {
        do {
            func_80014C08(1);
        } while (SESSION->loading != 0);
        func_80014C08(1);
        func_800149B8(0, -1, 0, 0x1600, SAI_runArea, 1, getCurrentTaskId(), 0, 0);
    }
    func_80014A90();
}

void SAI_initMapPaths(void) {
    MapPath *path;
    MapNode *node;
    s32 dx;
    s32 dy;
    s32 i;
    s32 j;
    s8 next;
    s16 angle;

    SAI_initMapAnims(SAI_WORLD_MAP.region);
    path = SAI_WORLD_MAP.paths;
    SAI_WORLD_MAP.animating = 1;
    SAI_WORLD_MAP.pathCount = 0;
    for (i = 0; i < 7; i++) {
        if (SAI_WORLD_MAP.route[i] == -1) {
            continue;
        }
        node = &SAI_MAP_NODES[SAI_WORLD_MAP.route[i]];
        if (node->unlocked != 1) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            next = node->next[j];
            if (i < next && next >= 0 && SAI_MAP_NODES[next].unlocked != 0) {
                dx = SAI_MAP_NODES[next].x - node->x;
                dy = SAI_MAP_NODES[next].y - node->y;
                angle = ratan2(dy, dx);
                path->rot.vz = -angle;
                path->pos.vx = node->x;
                path->pos.vy = node->y;
                path->length = csqrt(dx * dx + dy * dy);
                path->length /= 64;
                SAI_WORLD_MAP.pathCount++;
                path++;
            }
        }
    }
    addFrameCallback((s32)SAI_drawMapPaths);
}

void SAI_drawMapPaths(FrameBuffer *fb) {
    MATRIX matrix;
    SVECTOR corners[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;
    s8 i;

    for (i = 0; i < SAI_WORLD_MAP.pathCount; i++) {
        buildRotTransMatrix(&SAI_WORLD_MAP.paths[i].pos, &SAI_WORLD_MAP.paths[i].rot, &matrix);
        CompMatrix((MATRIX *)SCENE_3D->unk78, &matrix, &matrix);
        SetRotMatrix((s32)&matrix);
        func_8005C444(&matrix);
        corners[0].vx = 0;
        corners[0].vy = 2;
        corners[0].vz = 0;
        corners[1].vx = SAI_WORLD_MAP.paths[i].length;
        corners[1].vy = 2;
        corners[1].vz = 0;
        corners[2].vx = 0;
        corners[2].vy = -2;
        corners[2].vz = 0;
        corners[3].vx = SAI_WORLD_MAP.paths[i].length;
        corners[3].vy = -2;
        corners[3].vz = 0;
        RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
        ((PolyF4 *)fb->primSlots[1])[i].x0 = sxy[0];
        ((PolyF4 *)fb->primSlots[1])[i].y0 = sxy[0] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x1 = sxy[1];
        ((PolyF4 *)fb->primSlots[1])[i].y1 = sxy[1] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x2 = sxy[2];
        ((PolyF4 *)fb->primSlots[1])[i].y2 = sxy[2] >> 16;
        ((PolyF4 *)fb->primSlots[1])[i].x3 = sxy[3];
        ((PolyF4 *)fb->primSlots[1])[i].y3 = sxy[3] >> 16;
        addPrim(&fb->ot[34], &((PolyF4 *)fb->primSlots[1])[i]);
        addPrim(&fb->ot[34], &SAI_WORLD_MAP.pathTpages[FRAME_BUFFER_INDEX][i]);
    }
}

void SAI_createMapMenuTab(s8 keepTabState) {
    if (keepTabState == 0) {
        SAI_MAP_MENU_TAB_STATE = 0;
    }
    SAI_MAP_MENU_TAB.offset = 0;
    SAI_SPRITES[5] = SAI_createSprite(0x41);
    SAI_SPRITES[5]->pos.vx = -0xD5;
    SAI_SPRITES[5]->pos.vy = -0x24;
    SAI_setSpriteDepth(SAI_SPRITES[5], 0x1A);
}

void SAI_drawMapMenuTab(void) {
    char buf[0x48];
    s32 x;
    s32 state;

    state = SAI_MAP_MENU_TAB.state;
    if (state == 1) {
        SAI_MAP_MENU_TAB.offset++;
        if (SAI_MAP_MENU_TAB.offset > 20) {
            SAI_MAP_MENU_TAB.offset = 20;
        }
    } else if (state > 0) {
        if (state < 4) {
            SAI_MAP_MENU_TAB.offset--;
            if (SAI_MAP_MENU_TAB.offset < 0) {
                SAI_MAP_MENU_TAB.offset = 0;
            }
        }
    }
    x = ((20 - SAI_MAP_MENU_TAB.offset) * -213 - SAI_MAP_MENU_TAB.offset * 116) / 20;
    SAI_SPRITES[5]->pos.vx = x;
    SAI_drawSprite(SAI_SPRITES[5]);
}

void func_801F0E84(void) {
}

void SAI_openDeckEditorFromMap(void) {
    openDeckEditor(0);
}

void SAI_openEquipmentFromMap(void) {
    openPartnerEquipment(0);
}

void SAI_runRewardTask(u8 value) {
    SAI_REWARD_FROM_SCRIPT = value;
    SAI_showRewardCards();
    func_80014C08(5);
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    SAI_AREA.rewardBusy = 0;
}

void SAI_waitForCross(void) {
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->pressed & PAD_CROSS));
}

void SAI_drawRewardCardArt(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u16 clut, u8 *rgb) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = vramX % 64 * 2 + 2;
        CUR_SPRT->sp.v0 = vramY % 256 + 2;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, vramX, vramY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (frame > 5) {
            frame = 5;
        }
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x80;
            CUR_SPRT->sp.v0 = 0x30;
            CUR_SPRT->sp.clut = getClut(0x210, frame + 0xE2);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, 8);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void SAI_drawRewardCard(CardWindow *win) {
    char buf[0x48];
    u8 rgb[4];
    s32 x;
    s32 y;
    s32 z;
    s32 specialty;
    s32 i;
    DigimonCardData *card;
    u8 *data;

    if (SAI_REWARD_SCREEN->showRewards != 0) {
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[win->index] < 0) {
            rgb[0] = 0x40;
            rgb[1] = 0x40;
            rgb[2] = 0x40;
            win->window.brightness = 0x40;
        } else {
            rgb[0] = 0x80;
            rgb[1] = 0x80;
            rgb[2] = 0x80;
            win->window.brightness = 0x80;
        }
    } else {
        rgb[0] = 0x80;
        rgb[1] = 0x80;
        rgb[2] = 0x80;
        win->window.brightness = 0x80;
    }
    x = win->window.originX;
    y = win->window.originY;
    z = win->window.z;
    if (win->cardId >= 0) {
        specialty = getCardSpecialty(win->cardId);
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[win->cardId] & 0x20) {
            drawIcon(x + 0xF, y + 0x2A, 2, 9, 0);
        }
        SAI_drawRewardCardArt(x + 1, y + 0xC, win->index * 20 + 0x200, 0x78, specialty, win->clut, rgb);
        drawTextColored(x + 3, y, "No.", rgb, 7, z);
        sprintf(buf, "*s0%3d", win->cardId);
        drawTextColored(x + 0x17, y, buf, rgb, 7, z);
        if (specialty < 5) {
            card = &((DigimonCardData *)DIGIMON_CARDS)[win->cardId];
            drawTextColored(x + 0x32, y, card->name, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0xE, 0, 0x1A, rgb, z);
            sprintf(buf, "*s0%4d", card->hp);
            drawTextColored(x + 0x3C, y + 0xD, buf, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0x1A, 0, 7, rgb, z);
            sprintf(buf, "*s0%4d", card->attack[0].power);
            drawTextColored(x + 0x3C, y + 0x19, buf, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0x26, 0, 8, rgb, z);
            sprintf(buf, "*s0%4d", card->attack[1].power);
            drawTextColored(x + 0x3C, y + 0x25, buf, rgb, 7, z);
            drawIconColored(x + 0x2C, y + 0x32, 0, 9, rgb, z);
            sprintf(buf, "*s0%4d", card->attack[2].power);
            drawTextColored(x + 0x3C, y + 0x31, buf, rgb, 7, z);
            sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
            drawSmallTextColored(x + 0x5A, y + 0x33, buf, 7, rgb, z);
            drawIconColored(x + 0x74, y + 0x1A, 0, 0x18, rgb, z);
            sprintf(buf, "*s0%2d", card->dpCost);
            drawTextColored(x + 0x86, y + 0x19, buf, rgb, 7, z);
            drawIconColored(x + 0x74, y + 0x26, 0, 0x19, rgb, z);
            sprintf(buf, "*s0%2d", card->dpBonus);
            drawTextColored(x + 0x86, y + 0x25, buf, rgb, 7, z);
            drawTextColored(x + 0xB4, y, "Support Effect", rgb, 6, z);
            for (i = 0; i < 4; i++) {

            drawTextColored(x + 0xB4, y + 0xD + i * 12, card->supportText[i], rgb, 7, z);
            }
        } else if (specialty == 5) {
            data = (u8 *)&((OptionCardData *)OPTION_CARDS)[win->cardId - 0xBF];
            drawTextColored(x + 0x32, y, data + 3, rgb, 7, z);
            drawTextColored(x + 0xB4, y, "Option Description", rgb, 6, z);
            for (i = 0; i < 4; i++) {

            drawTextColored(x + 0xB4, y + 0xD + i * 12, data + (i * 0x15 + 0x8D), rgb, 7, z);
            }
        } else {
            data = (u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[win->cardId - 0x125];
            drawTextColored(x + 0x32, y, data + 3, rgb, 7, z);
            drawTextColored(x + 0xB4, y, "Option Description", rgb, 6, z);
            for (i = 0; i < 4; i++) {

            drawTextColored(x + 0xB4, y + 0xD + i * 12, data + (i * 0x15 + 0x1B), rgb, 7, z);
            }
        }
    } else {
        drawTextColored(x + 0x74, y + 0x1A, "NO DATA", rgb, 6, z);
    }
}

void SAI_drawRewardResult(RewardWindow *win) {
    s32 x = win->window.originX;
    s32 y = win->window.originY;
    s32 z = win->window.z;

    if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[win->slot] < 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void SAI_drawRewardTitle(UiWindow *window) {
    if (SAI_REWARD_FROM_SCRIPT == 0) {
        drawText(window->originX + 2, window->originY + 1, (s32)"Earned a Prize Pack", 7, 0);
        drawText(window->originX + 0x92, window->originY + 1, (s32)D_8006E31C[SAI_PRIZE_PACK], 6, 0);
    } else {
        drawText(window->originX + 2, window->originY + 1, (s32)"Received", 7, 0);
    }
}

void SAI_drawRewardWindows(void) {
    s32 i;

    drawWindow(&SAI_REWARD_SCREEN->main, SAI_drawRewardTitle, 0);
    for (i = 0; i < 3; i++) {
        if (SAI_REWARD_SCREEN->showRewards != 0 && ((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] != 100) {
            drawWindow(&SAI_REWARD_SCREEN->rewards[i].window, SAI_drawRewardResult, 0);
        }
        drawWindow(&SAI_REWARD_SCREEN->cards[i].window, SAI_drawRewardCard, 0);
    }
}

void SAI_addScriptRewardCards(void) {
    s32 i;
    s32 clearNew;
    u8 flags;

    for (i = 0; i < 3; i++) {
        ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i] = SAI_SCRIPT_REWARD_CARDS[i];
        if (SAI_SCRIPT_REWARD_CARDS[i] >= 0) {
            flags = ((PlayerProfile *)PLAYER_PROFILES)->cardCollection[((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i]];
            clearNew = 0;
            if (flags & 0x40) {
                flags &= 0x20;
                clearNew = flags != 0;
            }
            ((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] = addCardToCollection(0, ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i], 1);
            if (clearNew) {
                ((PlayerProfile *)PLAYER_PROFILES)->cardCollection[((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i]] &= ~0x20;
            }
        } else {
            ((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] = 100;
        }
    }
}

void SAI_showRewardCards(s32 fromScript) {
    /* the ROM keeps two stray bytes after the terminator */
    static const char path[16] = "B:\\M_CARD.ARC\0\xBB\xBB";
    Rect16 rect;
    u8 *tims;
    s32 i;
    s16 card;

    SAI_REWARD_SCREEN = allocPermanentHeapBlock(sizeof(RewardScreen));
    SAI_REWARD_SCREEN->showRewards = 0;
    if (fromScript == 0) {
        rollRewardCards(0, SAI_PRIZE_PACK);
        addRewardCardsToCollection(0);
    } else {
        SAI_addScriptRewardCards();
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tims = (u8 *)func_80014C08(0x7FFFFFFF);
    playSoundEffect(3);
    rect.x = 0x10;
    rect.y = 0x10;
    rect.w = 0x120;
    rect.h = 0xE;
    openWindow(&SAI_REWARD_SCREEN->main, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        card = ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i];
        if (card >= 0) {
            uploadTim((u32 *)(tims + ((s32 *)tims)[card]), i * 20 + 0x200, 0x78, 0x200, i + 0xFB);
        }
        SAI_REWARD_SCREEN->cards[i].index = i;
        SAI_REWARD_SCREEN->cards[i].cardId = card;
        SAI_REWARD_SCREEN->cards[i].clut = getClut(0x200, i + 0xFB);
        rect.x = 0x10;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&SAI_REWARD_SCREEN->cards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    }
    DrawSync(0);
    func_80014C08(FRAME_INTERVAL);
    freeHeapBlock(tims);
    addFrameCallback((s32)SAI_drawRewardWindows);
    func_80014C08(20);
    SAI_waitForCross();
    playSoundEffect(0);
    SAI_REWARD_SCREEN->showRewards = 1;
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i] >= 0) {
            rect.x = 0xC8;
            rect.y = i * 65 + 0x40;
            rect.w = 0x48;
            rect.h = 9;
            if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] < 0) {
                rect.x = 0xC4;
                rect.w = 0x50;
            }
            openWindow(&SAI_REWARD_SCREEN->rewards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
            SAI_REWARD_SCREEN->rewards[i].slot = i;
            SAI_REWARD_SCREEN->rewards[i].window.palette = 2;
        }
    }
    SAI_waitForCross();
    playSoundEffect(4);
    animateWindowTo(&SAI_REWARD_SCREEN->main, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&SAI_REWARD_SCREEN->cards[i].window, (Rect16 *)-1);
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] != 100) {
            animateWindowTo(&SAI_REWARD_SCREEN->rewards[i].window, (Rect16 *)-1);
        }
    }
    func_80014C08(20);
    freeHeapBlock(SAI_REWARD_SCREEN);
    func_80014C08(10);
    removeFrameCallback((s32)SAI_drawRewardWindows);
}

void SAI_drawPartnerCardArt(s32 x, s32 y, s32 vramX, s32 vramY, s32 frame, u8 *rgb, s32 z, s32 palette) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = vramX % 64 * 2 + 2;
        CUR_SPRT->sp.v0 = vramY % 256 + 2;
        CUR_SPRT->sp.clut = getClut(0x280, palette + 0x1EE);
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, vramX, vramY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0xA8;
            CUR_SPRT->sp.clut = getClut(0x290, frame + 0x1F2);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x18);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

const u8 SAI_PARTNER_CARD_IDS[6] = { 0xAF, 0xB6, 0xBE, 0xB8, 0xB7, 0xBB };

void SAI_drawPartnerChoices(UiWindow *win) {
    /* not literals: GCC would share SAI_drawRewardCard's identical strings */
    static const char valueFormat[] = "%4d";
    static const char effectFormat[] = "(%s)";
    static const char supportEffect[] = "Support Effect";
    Rect16 rect;
    char buf[0x10];
    u8 rgb[4];
    s32 i;
    s32 cardId;
    s32 y;
    s32 z;
    s32 x;
    s32 j;
    s32 tx;

    z = win->z;
    y = win->originY + 1;
    for (i = 0; i < SAI_PARTNER_LIST.count; i++) {
        cardId = SAI_PARTNER_CHOICE_CARDS[i];
        if (SAI_PARTNER_CURSOR.cursor == i) {
            rgb[0] = 0x80;
            rgb[1] = 0x80;
            rgb[2] = 0x80;
        } else {
            rgb[0] = 0x40;
            rgb[1] = 0x40;
            rgb[2] = 0x40;
        }
        x = win->originX;
        tx = x + 4;
        SAI_drawPartnerCardArt(tx, y + 4, i * 20 + 0x200, 0x180, getCardSpecialty(cardId), rgb, z, i);
        tx = x + 0x30;
        drawTextColored(tx, y + 1, ((DigimonCardData *)DIGIMON_CARDS)[cardId].name, rgb, 7, z);
        drawIconColored(tx, y + 0xD, 0, 7, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].attack[0].power);
        drawTextColored(x + 0x3E, y + 0xD, buf, rgb, 7, z);
        drawIconColored(tx, y + 0x19, 0, 8, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].attack[1].power);
        drawTextColored(x + 0x3E, y + 0x19, buf, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 9, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].attack[2].power);
        drawTextColored(x + 0x3E, y + 0x25, buf, rgb, 7, z);
        sprintf(buf, effectFormat, CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)DIGIMON_CARDS)[cardId].crossEffect]);
        drawLargeTextColored(tx, y + 0x31, buf, 7, rgb, z);
        tx = x + 0x5C;
        drawIconColored(tx, y + 0x19, 0, 0x19, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].dpBonus);
        drawTextColored(x + 0x68, y + 0x19, buf, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 0x1A, rgb, z);
        sprintf(buf, valueFormat, ((DigimonCardData *)DIGIMON_CARDS)[cardId].hp);
        drawTextColored(x + 0x6A, y + 0x25, buf, rgb, 7, z);
        tx = x + 0x88;
        drawTextColored(x + 0x98, y, supportEffect, rgb, 6, z);
        for (j = 0; j < 4; j++) {
            drawTextColored(tx, y + 0xF + j * 12, ((DigimonCardData *)DIGIMON_CARDS)[cardId].supportText[j], rgb, 7, z);
        }
        rect.x = tx;
        rect.y = y + 0xF;
        rect.w = 0x6E;
        rect.h = 0x30;
        drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
        y += 0x48;
    }
    if (SAI_PARTNER_LIST.state != 5) {
        if (PAD_STATES[0]->repeat & PAD_UP) {
            if (SAI_PARTNER_CURSOR.cursor != 0) {
                playSoundEffect(2);
                SAI_PARTNER_CURSOR.cursor--;
                scrollWindowTo(&win->originX, 0, SAI_PARTNER_CURSOR.cursor * 72);
            }
        } else if (PAD_STATES[0]->repeat & PAD_DOWN) {
            if (SAI_PARTNER_CURSOR.cursor < SAI_PARTNER_LIST.count - 1) {
                playSoundEffect(2);
                SAI_PARTNER_CURSOR.cursor++;
                scrollWindowTo(&win->originX, 0, SAI_PARTNER_CURSOR.cursor * 72);
            }
        } else if (PAD_STATES[0]->pressed & PAD_CROSS) {
            playSoundEffect(0);
            SAI_PARTNER_CURSOR.done = 1;
        }
        SAI_PARTNER_LIST.state = SAI_PARTNER_CURSOR.cursor + 6;
        rect.x = win->rect.x - win->scroll[2] + 1;
        rect.y = win->rect.y - win->scroll[3] + SAI_PARTNER_CURSOR.cursor * 72;
        rect.w = 0x104;
        rect.h = 0x48;
        moveCursorHighlight(&SAI_PARTNER_GET_CURSOR, &rect);
        drawCursorHighlight(&SAI_PARTNER_GET_CURSOR, z);
    }
}

void SAI_drawPartnerGetWindow(void) {
    drawWindow(&SAI_PARTNER_GET_WINDOW, SAI_drawPartnerChoices, 1);
}

void SAI_runPartnerGet(s32 task) {
    Rect16 cursorRect;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    u8 *file;
    s32 i;

    SAI_PARTNER_LIST.count = 0;
    for (i = 0; i < 4; i++) {
        if (SAI_PARTNER_CHOICES[i] != -1) {
            SAI_PARTNER_CHOICE_CARDS[i] = SAI_PARTNER_CARD_IDS[SAI_PARTNER_CHOICES[i]];
            SAI_PARTNER_LIST.count++;
        }
    }
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\CARD_F.TIM", getCurrentTaskId());
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    uploadTim((u32 *)file, 0x200, 0x1A8, 0x290, 0x1F2);
    DrawSync(0);
    func_80014C08(FRAME_INTERVAL);
    freeHeapBlock(file);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\P_CARD.ARC", getCurrentTaskId());
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < SAI_PARTNER_LIST.count; i++) {
        uploadTim((u32 *)(file + ((s32 *)file)[getPartnerIndex(SAI_PARTNER_CHOICE_CARDS[i])]), i * 20 + 0x200, 0x180, 0x280, i + 0x1EE);
    }
    SAI_PARTNER_CURSOR.done = 0;
    SAI_PARTNER_CURSOR.cursor = 0;
    rect.x = 0x1E;
    rect.y = 0x44;
    rect.w = 0x104;
    rect.h = 0x5A;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = SAI_PARTNER_LIST.count * 72;
    openWindow(&SAI_PARTNER_GET_WINDOW, &rect, -1, (s16 *)&view, 10, 0x86, 0x80, 12);
    SAI_PARTNER_GET_WINDOW.label = (s32)"PARTNER GET";
    SAI_PARTNER_GET_WINDOW.labelPalette = 7;
    cursorRect.x = SAI_PARTNER_GET_WINDOW.originX + 4;
    cursorRect.y = SAI_PARTNER_GET_WINDOW.originY + 1;
    cursorRect.w = 12;
    cursorRect.h = 12;
    initCursorHighlight(&SAI_PARTNER_GET_CURSOR, &cursorRect, (Bytes4 *)-1);
    func_80014C08(FRAME_INTERVAL);
    playSoundEffect(3);
    addFrameCallback((s32)SAI_drawPartnerGetWindow);
    do {
    wait:
        func_80014C08(FRAME_INTERVAL);
        if (SAI_PARTNER_LIST.state == 5 && (PAD_STATES[0]->pressed & PAD_CROSS)) {
            SAI_PARTNER_LIST.state = 6;
        }
        if (SAI_PARTNER_CURSOR.done == 0) {
            goto wait;
        }
        initDialog(dialog, "Is this Partner OK?", 1);
        runDialog(dialog);
        switch ((s8)dialog[0xA5]) {
        case 1:
            break;
        case 0:
        case 2:
            SAI_PARTNER_CURSOR.done = 0;
            break;
        }
    } while (SAI_PARTNER_CURSOR.done == 0);
    animateWindowTo(&SAI_PARTNER_GET_WINDOW, (Rect16 *)-1);
    playSoundEffect(4);
    freeHeapBlock(file);
    func_80014C08(20);
    removeFrameCallback((s32)SAI_drawPartnerGetWindow);
    obtainPartner(0, SAI_AREA.partners[SAI_PARTNER_CURSOR.cursor]);
    SAI_AREA.mode = AREA_MODE_SCRIPT;
    SAI_setPartnerObtainedFlag(SAI_AREA.partners[SAI_PARTNER_CURSOR.cursor]);
    func_80014A48(task);
}

SpriteTemplate SAI_SPRITE_TEMPLATES[66] = {
    { 0x180, 0xA8, 0x200, 0xF8, 0, 0x1A, 0x28, 1, 0 },
    { 0x1E8, 0xA8, 0x200, 0xF8, 0, 0x18, 0x10, 1, 0 },
    { 0x180, 0x100, 0x180, 0x1A8, 0, 0xC6, 0xA2, 1, 0 },
    { 0x180, 0x100, 0x280, 0x1F8, 0, 0xFF, 0x80, 1, 0 },
    { 0x180, 0x180, 0x280, 0x1F9, 0, 0xFF, 0x7F, 1, 0 },
    { 0x200, 0x100, 0x280, 0x1FA, 0, 0xFF, 0x80, 1, 0 },
    { 0x2C0, 0xC0, 0x200, 0xF3, 0, 0x80, 0x20, 0, 0 },
    { 0x2E0, 0, 0x200, 0xED, 0, 0x74, 0x18, 0, 0 },
    { 0x2C0, 0, 0x200, 0xF4, 0, 0x50, 0x46, 0, 0 },
    { 0x229, 0, 0x200, 0xE8, 0, 0x10, 0xD, 0, 0 },
    { 0x320, 0x100, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x128, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x150, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x178, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x340, 0x100, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x340, 0x128, 0x280, 0x1F7, 0, 0x27, 0x70, 0, 0 },
    { 0x340, 0x198, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x34A, 0x100, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x34A, 0x128, 0x280, 0x1F7, 0, 0x27, 0x70, 0, 0 },
    { 0x34A, 0x198, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x300, 0xA0, 0x200, 0xEF, 0, 0x58, 0x10, 0, 0 },
    { 0x340, 0x78, 0x200, 0xE3, 0, 0x70, 0x6C, 0, 0 },
    { 0x300, 0xB0, 0x200, 0xE3, 0, 0x54, 0x4C, 0, 0 },
    { 0x318, 0x30, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x318, 0x60, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x318, 0x70, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x360, 0x78, 0x200, 0xE6, 0, 0x6C, 0x7A, 0, 0 },
    { 0x318, 0xC8, 0x200, 0xEB, 0, 0x40, 0x37, 0, 0 },
    { 0x180, 0, 0x200, 0xF9, 0, 0x40, 0x37, 1, 0 },
    { 0x200, 0x30, 0x200, 0xEA, 0, 0x84, 0x30, 0, 0 },
    { 0x2E0, 0xB4, 0x200, 0xE5, 0, 0x68, 0x1C, 0, 0 },
    { 0x340, 0, 0x200, 0xE4, 0, 0x60, 0x14, 0, 0 },
    { 0x240, 0, 0x200, 0xE5, 0, 0xAF, 0x30, 0, 0 },
    { 0x240, 0x30, 0x200, 0xE5, 0, 0xAF, 0x30, 0, 0 },
    { 0x280, 0, 0x200, 0xE5, 0, 0x27, 0x30, 0, 0 },
    { 0x280, 0x30, 0x200, 0xE5, 0, 0x27, 0x90, 0, 0 },
    { 0x280, 0xC0, 0x200, 0xE5, 0, 0x27, 0x30, 0, 0 },
    { 0x28A, 0, 0x200, 0xE5, 0, 0x23, 0x30, 0, 0 },
    { 0x28A, 0x30, 0x200, 0xE5, 0, 0x23, 0x90, 0, 0 },
    { 0x28A, 0xC0, 0x200, 0xE5, 0, 0x23, 0x30, 0, 0 },
    { 0x240, 0x60, 0x200, 0xE2, 0, 0xAF, 0x30, 0, 0 },
    { 0x240, 0x90, 0x200, 0xE2, 0, 0xAF, 0x34, 0, 0 },
    { 0x298, 0, 0x200, 0xE2, 0, 0x37, 0x30, 0, 0 },
    { 0x298, 0x30, 0x200, 0xE2, 0, 0x37, 0x88, 0, 0 },
    { 0x298, 0xB8, 0x200, 0xE2, 0, 0x37, 0x34, 0, 0 },
    { 0x2A6, 0, 0x200, 0xE2, 0, 0x2B, 0x30, 0, 0 },
    { 0x2A6, 0x30, 0x200, 0xE2, 0, 0x2B, 0x88, 0, 0 },
    { 0x2A6, 0xB8, 0x200, 0xE2, 0, 0x2B, 0x34, 0, 0 },
    { 0x300, 0x100, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x128, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x150, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x178, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x358, 0x100, 0x280, 0x1F6, 0, 0x27, 0x28, 0, 0 },
    { 0x358, 0x128, 0x280, 0x1F6, 0, 0x27, 0x78, 0, 0 },
    { 0x358, 0x1A0, 0x280, 0x1F6, 0, 0x27, 0x28, 0, 0 },
    { 0x362, 0x100, 0x280, 0x1F6, 0, 0x23, 0x28, 0, 0 },
    { 0x362, 0x128, 0x280, 0x1F6, 0, 0x23, 0x78, 0, 0 },
    { 0x362, 0x1A0, 0x280, 0x1F6, 0, 0x23, 0x28, 0, 0 },
    { 0x318, 0xA4, 0x200, 0xEB, 0, 0x2A, 0x24, 0, 0 },
    { 0x2C0, 0x82, 0x200, 0xFA, 0, 0x40, 0x38, 1, 0 },
    { 0x218, 0, 0x200, 0xEC, 0, 0x34, 0x30, 0, 0 },
    { 0x318, 0x80, 0x200, 0xF1, 0, 0x40, 0x10, 0, 0 },
    { 0x2E0, 0x80, 0x200, 0xEE, 0, 0x2A, 0x24, 0, 0 },
    { 0x2EB, 0x80, 0x200, 0xEE, 0, 0x2A, 0x24, 0, 0 },
    { 0x22D, 0, 0x200, 0xE8, 0, 0x10, 0xD, 0, 0 },
    { 0x328, 0, 0x210, 0xF2, 0, 0x58, 0x50, 0, 0 },
};

/* the map's nodes: position, the node in each direction, unlocked */
MapNode SAI_MAP_NODES[16] = {
    { 73, 26, { -1, 12, 4, 1 }, 1, 0 },
    { 16, -17, { 3, 0, 2, 3 }, 1, 0 },
    { 14, 34, { 1, 1, -1, -1 }, 1, 0 },
    { -27, -50, { -1, 1, 1, -1 }, 1, 0 },
    { 81, 53, { 0, -1, -1, 0 }, 1, 0 },
    { -22, 25, { -1, 7, 13, 13 }, 1, 0 },
    { 61, 67, { 7, 7, -1, -1 }, 1, 0 },
    { 67, 18, { 8, -1, 6, 5 }, 1, 0 },
    { 47, -37, { 14, 7, 7, 14 }, 1, 0 },
    { 70, -36, { -1, 15, -1, 10 }, 1, 0 },
    { -31, -16, { 9, 9, 11, -1 }, 1, 0 },
    { 44, 21, { 10, -1, -1, 10 }, 1, 0 },
    { 107, -11, { -1, -1, 0, 0 }, 1, 0 },
    { -46, 33, { 5, 5, -1, -1 }, 1, 0 },
    { -16, -44, { -1, 8, 8, -1 }, 1, 0 },
    { 120, -18, { 9, -1, -1, 9 }, 1, 0 },
};

/* the nodes of each of the three regions, ended by -1 */
s8 SAI_REGION_NODES[3][7] = {
    { 0, 1, 2, 3, 4, 12, -1 },
    { 5, 6, 7, 8, 13, 14, -1 },
    { 9, 10, 11, 15, -1, -1, -1 },
};

Rect16 SAI_REGION_MAP_RECTS[3] = {
    { 0x180, 0x100, 0xC6, 0xA2 },
    { 0x200, 0x100, 0xC6, 0xA2 },
    { 0x280, 0x100, 0xC6, 0xA2 },
};

void (*SAI_ICON_MOTION_FUNCS[3])(void) = {
    NULL,
    SAI_slideIconToBottom,
    SAI_slideIconToTop,
};

void (*SAI_MAP_STATE_FUNCS[10])(void) = {
    SAI_openMap,
    SAI_tickMapInput,
    SAI_walkMapMarker,
    SAI_fadeInRegion,
    SAI_fadeOutRegion,
    SAI_switchRegion,
    SAI_fadeOutRegion,
    SAI_closeMap,
    func_801EF1A8,
    SAI_tickMapMenu,
};

u8 SAI_PARTNER_CHOICE_CARDS[4] = { 0xB7, 0xB8, 0xBB, 0x0 };

/* the partner abilities; the code here only reads their texts */
DigiPart SAI_DIGI_PARTS[128] = {
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

/* the u8 arrays among these are not referenced by any code */
UiWindow SAI_STATS_HINT_WINDOW = { 0 };
u8 D_801F437C[12] = { 0 };
CursorHighlight SAI_PLAYER_DATA_CURSOR = { { { 0 } } };
UiWindow SAI_WORD_GRID_WINDOW = { 0 };
u8 D_801F441C[12] = { 0 };
CursorHighlight SAI_WORD_GRID_CURSOR = { { { 0 } } };
UiWindow SAI_KEYWORD_WINDOW = { 0 };
u8 D_801F44BC[12] = { 0 };
CursorHighlight SAI_KEYWORD_CURSOR = { { { 0 } } };
UiWindow SAI_WORD_HELP_WINDOW = { 0 };
u8 D_801F455C[12] = { 0 };
WordInput SAI_WORD_INPUT = { 0 };
AreaState SAI_AREA = { { { 0 } } };
u8 D_801F46AC[4] = { 0 };
UiWindow SAI_ERROR_WINDOWS[5] = { { 0 } };
Splash *SAI_SPLASH = NULL;
u8 SAI_HACK_OVERLAY_STATE = 0;
u8 D_801F480C[4] = { 0 };
PlayerStats SAI_PLAYER_STATS = { { 0 } };
u8 SAI_HACK_OVERLAY_FLICKER = 0;
ScriptRunner *SAI_SCRIPT[1] = { NULL };
u8 D_801F483C[4] = { 0 };
TextLine SAI_TEXT_LINES[3] = { { { 0 } } };
u8 D_801F4900[8] = { 0 };
OpponentList SAI_OPPONENTS = { { 0 } };
u8 D_801F4A34[4] = { 0 };
u8 SAI_DIALOG[0xB8] = { 0 };
OpponentInfo *SAI_OPPONENT_INFO = NULL;
u8 D_801F4AF4[4] = { 0 };
s16 SAI_SCRIPT_REWARD_CARDS[3] = { 0 };
u8 D_801F4AFE = 0;
u8 SAI_REWARD_FROM_SCRIPT = 0;
UiWindow SAI_PLAYER_DATA_WINDOW = { 0 };
UiWindow SAI_MESSAGE_WINDOW = { 0 };
u8 D_801F4B88[0xD0] = { 0 };
s16 SAI_MAP_FRAME_BRIGHTNESS = 0;
u8 D_801F4C5C[0x1EC] = { 0 };
WorldMap SAI_WORLD_MAP = { 0 };
u8 D_801F5254[4] = { 0 };
MenuTab SAI_MAP_MENU_TAB = { 0 };
Sprite3D *SAI_SPRITES[60] = { NULL };
MapAnim *SAI_MAP_ANIMS = NULL;
u8 D_801F5354[4] = { 0 };
RewardScreen *SAI_REWARD_SCREEN = NULL;
u8 D_801F535C[4] = { 0 };
UiWindow SAI_PARTNER_GET_WINDOW = { 0 };
u8 D_801F53A4[12] = { 0 };
CursorHighlight SAI_PARTNER_GET_CURSOR = { { { 0 } } };
PartnerCursor SAI_PARTNER_CURSOR = { 0 };
u8 D_801F5404[4] = { 0 };
PartnerList SAI_PARTNER_LIST = { 0 };
UiWindow SAI_DIGI_PARTS_WINDOW = { 0 };
u8 D_801F5454[4] = { 0 };
u8 SAI_OWNED_DIGI_PARTS[16] = { 0 };

void SAI_drawDigiPartsList(UiWindow *window) {
    char buf[0x18];
    s32 x = window->originX;
    s32 y = window->originY;
    s32 z = window->z;
    s32 count;
    s32 i;
    s32 icon;
    s32 palette;

    drawText(x + 0x40, y + 1, (s32)"Earned Digi-Parts List", 6, 0);
    count = 0;
    for (i = 0; i < 128; i++) {
        if ((SAI_OWNED_DIGI_PARTS[i / 8] >> (i % 8)) & 1) {
            count++;
            if (count >= (window->view.y - 15) / 13 && (window->view.y + window->rect.h - 15) / 13 >= count) {
                sprintf(buf, "%3.3d", i);
                drawText(x + 2, y + 15 + (count - 1) * 13, (s32)buf, 5, z);
                if (i < 7) {
                    icon = 0;
                } else if (i < 10) {
                    icon = 1;
                } else if (i < 15) {
                    icon = 2;
                } else if (i < 20) {
                    icon = 3;
                } else if (i < 24) {
                    icon = 4;
                } else if (i < 38) {
                    icon = 5;
                } else if (i < 41) {
                    icon = 6;
                } else if (i < 121) {
                    icon = 7;
                } else {
                    icon = 8;
                }
                drawIcon(x + 0x16, y + 15 + (count - 1) * 13, 2, icon, z);
                palette = 7;
                if (i >= 24 && i < 38) {
                    palette = 4;
                }
                if (i >= 41 && i < 121) {
                    palette = 5;
                }
                drawText(x + 0x30, y + 15 + (count - 1) * 13, (s32)SAI_DIGI_PARTS[i].name, palette, z);
            }
        }
    }
    window->view.h = count * 13 + 15;
    if (window->view.h - window->rect.h >= 0) {
        if (PAD_STATES[0]->repeat & PAD_L2) {
            scrollWindowTo(&window->originX, 0, window->view.y - window->rect.h);
        }
        if (PAD_STATES[0]->repeat & PAD_R2) {
            scrollWindowTo(&window->originX, 0, window->view.y + window->rect.h);
        }
        if (PAD_STATES[0]->repeat & PAD_UP) {
            scrollWindowTo(&window->originX, 0, window->view.y - 13);
        }
        if (PAD_STATES[0]->repeat & PAD_DOWN) {
            scrollWindowTo(&window->originX, 0, window->view.y + 13);
        }
    }
    if (count == 0) {
        drawText(x + 0x1A, y + 0xE, (s32)"None", 7, 0);
    }
}

void SAI_drawDigiPartsWindow(void) {
    drawWindow(&SAI_DIGI_PARTS_WINDOW, SAI_drawDigiPartsList, 0);
}

extern const char SAI_STR_GET_DIGIPARTS_LIST[];

/*
 * Task that grants a partner ability (a Digi-Part) and shows the parts list
 * until Cross is pressed.
 */
void SAI_grantDigiPart(s32 ability, s32 task) {
    Rect16 unused; /* never used, but the original frame has room for it */
    Rect16 rect;
    s32 i;
    UiWindow *win;

    rect.x = 0x28;
    rect.y = 0x2C;
    rect.w = 0xF0;
    rect.h = 0xA8;
    openWindow(&SAI_DIGI_PARTS_WINDOW, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    SAI_DIGI_PARTS_WINDOW.label = (s32)SAI_STR_GET_DIGIPARTS_LIST;
    /*
     * Dead code in the original: this loop's result is never used. GCC deletes
     * the store only after register allocation, which is why the ROM's loop
     * counter sits in a0; without it the code differs.
     */
    for (i = 0; i < 16; i++) {
        if ((SAI_OWNED_DIGI_PARTS[i] >> (ability % 8)) & 1) {
            win = &SAI_DIGI_PARTS_WINDOW;
        }
    }
    /* mark the ability as owned */
    SAI_OWNED_DIGI_PARTS[ability / 8] |= 1 << (ability % 8);
    grantPartnerAbility(0, ability);
    playSoundEffect(3);
    addFrameCallback((s32)SAI_drawDigiPartsWindow);
    do {
        func_80014C08(1);
    } while (!(PAD_STATES[0]->pressed & PAD_CROSS));
    playSoundEffect(4);
    win = &SAI_DIGI_PARTS_WINDOW;
    animateWindowTo(win, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)SAI_drawDigiPartsWindow);
    func_80014C08(1);
    SAI_AREA_MODE = AREA_MODE_SCRIPT;
    func_80014A48(task);
}

/* the last byte is a leftover in the original, not zero padding */
const char SAI_STR_GET_DIGIPARTS_LIST[20] = "GET DIGIPARTS LIST\0\x99";
