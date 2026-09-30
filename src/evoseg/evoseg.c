#include "common.h"
#include "game.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/wire_grid.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/task.h"
#include "dcb/stage.h"
#include "dcb/player_data.h"
#include "dcb/card_db.h"
#include "dcb/window.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"
#include "dcb/fade.h"
#include "dcb/decompress.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/battle_hud.h"
#include "dcb/transform.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/vram_upload.h"
#include "dcb/card_zones.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/partner_level.h"
#include "dcb/sound_play.h"
#include "dcb/vblank.h"
#include "dcb/prim.h"
#include "dcb/sound.h"
#include "dcb/scroll_bg.h"
#include "dcb/menu.h"
#include "dcb/sort.h"
#include "dcb/game_flow.h"
#include "gte.h"

void EVO_renderDissolvingObject(GsDOBJ4 *obj, s32 mode);
void GsGetLs(GsCOORDINATE2 *coord, MATRIX *m);
void GsSetLsMatrix(MATRIX *m);
void GsSetLightMatrix(MATRIX *m);
void GsSortObject4(GsDOBJ4 *obj, GsOT *ot, s32 shift, u32 *scratch);

typedef struct {
    s32 unk0;
    s32 unk4;
    u32 size;
    s32 unkC;
    u8 code[1];
} EvoMsd;

typedef struct {
    EvoMsd *data;
    Script *script;
    s32 *vars;
} EvoProgram;
Script *EVO_createScriptContext(EvoMsd *data);

extern Menu EVO_CARD_LIST_MENU;
extern Menu EVO_SORT_MENU;
void EVO_playEffect(s32 index, s32 arg);
void EVO_playEffectScript(s32 index, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void EVO_findPartnerReward(void);

typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0, v0;
    u16 cba;
    u8 u1, v1;
    u16 tsb;
    u8 u2, v2;
    u16 pad;
    u8 u3, v3;
    u16 pad2;
    u16 idx[8];
} TmdPacketFT4;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
} PolyG3;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
    u8 r3, g3, b3, pad3;
    s16 x3, y3;
} PolyG4;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
} PolyF3;

typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0, v0;
    u16 cba;
    u8 u1, v1;
    u16 tsb;
    u8 u2, v2;
    u16 pad;
    u16 idx[8];
} TmdPacketFT3;
typedef struct {
    s16 r;
    s16 g;
    s16 b;
} EvoColor;
extern s8 *EVO_SHARD_PRIM;
extern SVECTOR *EVO_SHARD_VERTS;
extern EvoColor EVO_SHARD_COLOR;
extern MATRIX D_801DBEA0;
long func_80062C44(void);
void func_80062C34(long base);
typedef struct {
    s16 first;
    s16 last;
} EvoRange;
typedef struct {
    s16 id;
    s8 type;
    u8 name[0x15];
    u8 fusionPoints;
    u8 level;
    u8 attr;
    u8 pad1B[0xE5 - 0x1B];
    u8 modelId;
} EvoCardInfo;
extern EvoRange EVO_CARD_ID_RANGES[];
extern EvoCardInfo *EVO_CARDS_BY_ID[];
typedef struct {
    Rect16 rect;
    u16 src[0x100];
    u16 dst[0x100];
    s16 brighten;
    s16 level;
} EvoClut;

void EVO_initShatterScene(s32 arg);
void EVO_tickShatter();
void EVO_renderCutsceneModels();
typedef struct {
    s16 x;
    s16 y;
} EvoPose;
typedef struct {
    u8 olen;
    u8 ilen;
    u8 flag;
    u8 mode;
    u8 u0;
    u8 v0;
    u16 cba;
    u8 u1;
    u8 v1;
    u16 tsb;
} TmdPrim;
typedef struct {
    SVECTOR *vertTop;
    u32 nvert;
    SVECTOR *normTop;
    u32 nnormal;
    u8 *primTop;
    u32 nprim;
    s32 scale;
    TmdPrim prims[1];
} TmdObject;
typedef struct {
    u8 pad0[8];
    TmdObject *tmd;
    u8 padC[4];
} EvoPart;
typedef struct {
    u8 pad0[4];
    s16 partCount;
    u8 pad6[0xB80 - 0x6];
    EvoPart parts[320];
    EvoPose *pose;
    u8 pad1F84[0x22B0 - 0x1F84];
    MATRIX matrices[2];
} EvoModel;
void EVO_restartModelAnimation(s16 slot);
void EVO_remapPartTextures(EvoPart *part, s32 enable);
extern s16 D_80079584;
void EVO_runFusedDigimonTask(s32 parentTask);
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} FlatLight;
s32 GsSetFlatLight(s32 id, FlatLight *light);
extern u8 *EVO_SPARE_CARD_COUNTS;
extern u8 *EVO_DECK_CARD_COUNTS[3];
/* The state of the fusion screen (EVO_FUSION) */
typedef struct {
    /* 0x00 */ u8 pad0[0xA0];
    /* 0xA0 */ s32 *cardArchive; /* B:\M_CARD.ARC: an offset per card id to its TIM */
    /* 0xA4 */ s32 swapTimer;    /* the Card Fusion / Partner Fusion panels swapping places */
    /* 0xA8 */ s32 resumeOffset; /* where the script goes on after the cutscene */
    /* 0xAC */ s16 firstCard;    /* the first card, or the partner's card in Partner Fusion */
    /* 0xAE */ s16 secondCard;
    /* 0xB0 */ s16 result;       /* the card made, the Digi-Part won or the experience to add */
    /* 0xB2 */ s16 candidates[2];
    /* 0xB6 */ s8 partnerKind;   /* index in EVO_PARTNER_CARD_IDS, then -1/-2 while adding exp */
    /* 0xB7 */ s8 rewardStep;
    /* 0xB8 */ s8 roll;
    /* 0xB9 */ s8 swapState;
    /* 0xBA */ s8 fusionType;    /* 0: Card Fusion, 1: Partner Fusion */
    /* 0xBB */ s8 scriptState;   /* 0: the script runs; otherwise it waits */
    /* 0xBC */ s8 typeChoiceOpen;
    /* 0xBD */ s8 unkBD;
    /* 0xBE */ s8 partnerListOpen;
    /* 0xBF */ s8 partnerCount;
    /* 0xC0 */ s8 partner;       /* the partner picked in the partner list */
    /* 0xC1 */ s8 step;          /* which handler EVO_runFusion calls every frame */
    /* 0xC2 */ s8 sortMenuOpen;
    /* 0xC3 */ s8 previewOpen;
    /* 0xC4 */ s8 pickSlot;      /* 1: picking the first card, 2: the second */
    /* 0xC5 */ s8 resultKind;    /* 0: by level, 1: a recipe (with the cutscene), 2: a lucky one */
    /* 0xC6 */ s8 hideResult;
    /* 0xC7 */ s8 cutscene;      /* leave the screen to play the fusion cutscene */
    /* 0xC8 */ s8 resultStep;
    /* 0xC9 */ s8 textTyping;
    /* 0xCA */ s8 unit;          /* which fusion unit: its script, music and portrait */
    /* 0xCB */ u8 blinkTimer;
    /* 0xCC */ s8 busy[3];
} EvoFusion;
/* One of the two card trays ("TRAY1", "TRAY2") */
typedef struct {
    /* 0x000 */ POLY_FT4 polys[2][3];
    /* 0x0F0 */ u8 padF0[0x120 - 0xF0];
    /* 0x120 */ s32 merge; /* tray 2: the merge started; tray 1: its frames */
    /* 0x124 */ s16 x;
    /* 0x126 */ s16 y;
    /* 0x128 */ u8 pad128[0x12C - 0x128];
} EvoTray;
extern EvoFusion EVO_FUSION;
extern EvoTray EVO_TRAYS[2];
typedef struct {
    u32 tag;
    u32 code[1];
} DrTPage;
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} PolyF4;
/* An additive full-screen quad that flashes the screen white */
typedef struct {
    /* 0x00 */ DrTPage tpage[2];
    /* 0x10 */ PolyF4 poly[2];
    /* 0x40 */ s16 brightness;
    /* 0x42 */ s8 on;
} EvoScreenFlash;
extern EvoScreenFlash EVO_SCREEN_FLASH;
void EVO_drawScreenFlash(void);
void EVO_runFusionScript(EvoProgram *program);
extern EvoProgram *EVO_SCRIPT;
typedef struct {
    u8 text[0x3C];
    s16 pos;
    s8 active;
    s8 len;
} EvoText;
extern UiWindow EVO_CARD_LIST_WINDOW;
void EVO_startSecondCardPick(void);
extern s8 EVO_EFFECT_PLAYER;
extern s8 EVO_EFFECT_SPRITE_1;
extern s8 EVO_EFFECT_SPRITE_2;
extern u8 *EVO_EFFECT_ARCHIVE;
void EVO_runEffectScriptTask();
/* An effect object: the same layout as EffectObject */
typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ VECTOR curRot;
    /* 0x030 */ SVECTOR curScale;
    /* 0x038 */ s32 curPos[3];
    /* 0x044 */ u8 pad44[0x98 - 0x44];
    /* 0x098 */ void *parent;
    /* 0x09C */ u8 pad9C[0xAC - 0x9C];
    /* 0x0AC */ VECTOR pos;
    /* 0x0BC */ VECTOR vel;
    /* 0x0CC */ SVECTOR accel;
    /* 0x0D4 */ SVECTOR rot;
    /* 0x0DC */ SVECTOR rotVel;
    /* 0x0E4 */ SVECTOR scale;
    /* 0x0EC */ SVECTOR scaleVel;
    /* 0x0F4 */ SVECTOR scaleAccel;
    /* 0x0FC */ u8 padFC[0x118 - 0xFC];
    /* 0x118 */ s32 state;
    /* 0x11C */ s32 flag;
    /* 0x120 */ s16 moveSpeed;
    /* 0x122 */ s16 moveAccel;
    /* 0x124 */ s16 period;
    /* 0x126 */ s16 waveFreq;
    /* 0x128 */ s16 wavePhase;
    /* 0x12A */ s16 waveAmplitude;
    /* 0x12C */ s16 hitRadius;
    /* 0x12E */ s16 mode;
    /* 0x130 */ s16 speed;
    /* 0x132 */ u8 pad132[0x137 - 0x132];
    /* 0x137 */ u8 fadeMode;
    /* 0x138 */ u8 fadeState;
    /* 0x139 */ u8 suspended;
    /* 0x13A */ u8 pad13A[2];
} EvoFx;
typedef struct {
    s16 kind;
    s16 active;
    EvoFx *handle;
} EvoEntry;
typedef struct {
    EvoMsd *data;
    Script *script;
    s32 *vars;
    EvoEntry entries[16];
    s32 counter;
    void *buffer;
} EvoEffectScript;
extern EvoFx EVO_EFFECT_ROOT;
extern void (*EVO_EFFECT_TICK_FUNCS[])(EvoFx *);
extern void (*EVO_EFFECT_FREE_FUNCS[])(EvoFx *);
void EVO_runEffectScript(EvoEffectScript *loader);
typedef struct {
    u8 pad[0x260];
    s32 x;
    s32 y;
    s32 z;
} EvoObject;
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} Color;
typedef struct {
    u8 pad0[0x98];
    s32 r;
    s32 g;
    s32 b;
} EvoLight;
void func_801F893C(void *sprite, Color *color);

typedef struct {
    UiWindow win;
    s8 isPartner;
    s8 z;
    u8 pad46[2];
} EvoWindow;
extern EvoWindow EVO_WINDOWS[];

typedef struct {
    Rect16 rect;
    s32 brightness;
    s32 style;
    s32 flags;
    s32 label;
    u8 labelPalette;
} EvoWindowDef;
extern EvoWindowDef EVO_WINDOW_DEFS[];

extern u8 EVO_MAX_CARD_LEVEL;

extern SVECTOR *EVO_SHARD_VERTEX_POOL;
extern s8 EVO_SHATTER_STARTED;
extern s8 EVO_CUTSCENE_STEP;
void EVO_initGsSortTable(void);

typedef struct {
    char *name;
    s8 learnLevels[6];
    u8 unkA[2];
} EvoAbilityInfo;
extern EvoAbilityInfo EVO_DIGI_PARTS[];
typedef struct {
    u8 pad0[0xA5];
    s8 choice;
} EvoDialog;
extern EvoDialog EVO_DIALOG;
extern u8 EVO_RANK_UP_STATE;
extern u8 EVO_LEVEL_UP_PENDING;
extern s16 EVO_STAT_BONUSES[4];
extern s32 EVO_NEW_DIGI_PART;
extern UiWindow EVO_RANK_UP_WINDOW;
void EVO_addPartnerExp(void);

extern EvoFx *(*EVO_EFFECT_CREATE_FUNCS[])(s32 *, EvoEffectScript *);

/* libgs's GsSortObject4J function table (_GsFCALL) */
#define GsDivMODE_NDIV 0
#define GsDivMODE_DIV 1
#define GsLMODE_NORMAL 0
#define GsLMODE_LOFF 2
typedef u32 *(*GsSortFunc)();
typedef struct {
    GsSortFunc f3[2][3];
    GsSortFunc nf3[2];
    GsSortFunc g3[2][3];
    GsSortFunc ng3[2];
    GsSortFunc tf3[2][3];
    GsSortFunc ntf3[2];
    GsSortFunc tg3[2][3];
    GsSortFunc ntg3[2];
    GsSortFunc f4[2][3];
    GsSortFunc nf4[2];
    GsSortFunc g4[2][3];
    GsSortFunc ng4[2];
    GsSortFunc tf4[2][3];
    GsSortFunc ntf4[2];
    GsSortFunc tg4[2][3];
    GsSortFunc ntg4[2];
    GsSortFunc f3g[3];
    GsSortFunc g3g[3];
    GsSortFunc f4g[3];
    GsSortFunc g4g[3];
} _GsFCALL;
extern _GsFCALL D_801DBFB0;
u32 *GsTMDfastF3L(), *GsTMDfastTF3L(), *GsTMDfastTF3NL(), *GsTMDfastTNF3();
u32 *GsTMDfastG3L(), *GsTMDfastTG3L(), *GsTMDfastTG3NL(), *GsTMDfastTNG3();
u32 *GsTMDfastF4L(), *GsTMDfastTF4L(), *GsTMDfastTF4NL(), *GsTMDfastTNF4();
u32 *GsTMDfastG4L(), *GsTMDfastTG4L(), *GsTMDfastTG4NL(), *GsTMDfastTNG4();
u32 *GsTMDdivTF4L(), *GsTMDdivTF4NL(), *GsTMDdivTF3NL(), *GsTMDdivTG4NL();
u32 *GsTMDdivTG3NL(), *GsTMDdivTNG4(), *GsTMDdivTNG3(), *GsTMDdivTNF4();
u32 *GsTMDdivTNF3(), *GsTMDfastF4NL(), *GsTMDfastNF4();

typedef struct {
    u8 pad0[0x260];
    s32 args[34];
} EvoFxParams;

extern s16 EVO_CUTSCENE_MODELS[3];
void EVO_runShatterTask(s32 parentTask);
extern EvoText EVO_TEXT_LINES[4];
s32 EVO_addTextLine(u8 *src);
typedef struct {
    u8 card;
    u8 ability;
} EvoAbilityReward;
extern u8 EVO_PARTNER_CARD_IDS[6];
extern EvoAbilityReward EVO_PARTNER_FUSION_REWARDS[][5];

extern u8 EVO_CARD_RECEIVED;

typedef struct {
    u8 pad0[0x20];
    s16 joint;
    s16 timer;
} EvoSpark;
void EVO_drawSpark(EvoSpark *spark);
s32 EVO_randomRange(s32 min, s32 max);

typedef struct {
    s16 timer;
    u16 count;
    SVECTOR *offsets;
    SVECTOR *verts;
    s8 *prims;
    s16 primCount;
    s16 part;
} EvoShard;
extern EvoShard *EVO_SHARDS;

extern Rect16 EVO_RANK_UP_RECT;

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
    u8 pad10[0x33 - 0x10];
    s8 side;
} EvoChoice;
extern EvoChoice EVO_TYPE_CHOICE;

extern u8 EVO_SCRIPT_HALTED;
void EVO_runChoiceDialog(s32 mode);
void EVO_closeFusionTypeChoice(void);

extern u8 D_800795A8;
void EVO_initEffectFromParams(EvoFx *fx, s32 *vars, EvoEffectScript *loader);
void func_801F8928(void *sprite);
void func_801F8910(void *sprite, s32 arg);

extern SVECTOR *EVO_SHARD_VERTEX_CURSOR;

SVECTOR *ApplyMatrixSV(MATRIX *m, SVECTOR *v0, SVECTOR *v1);

extern s8 EVO_FUSION_RESULT_TYPES[][6];
void EVO_findCardOfLevel(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB);

void EVO_drawShardTG3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTF3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTG4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTF4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTNF3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardTNF4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardF4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardG3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardG4(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_drawShardF3(SVECTOR *pos, s32 unused, s16 div, s16 mul);
void EVO_clearTextLines(EvoText *slot);
void EVO_openFusionTypeChoice(void);
void EVO_openPartnerList(void);
void EVO_closePartnerList(void);
void EVO_openCardList(void);
void EVO_closeCardList(void);
void EVO_startPartnerFusion(void);
void EVO_cancelPartnerFusion(void);
void EVO_leaveForCutscene(void);
void EVO_resetFusion(void);
void EVO_cancelFirstCard(void);
void EVO_getEffectParams(EvoFx *fx, EvoFxParams *params, s32 current);
void EVO_setEffectParams(EvoFx *fx, EvoFxParams *params);
void EVO_getEffectWorldPos(void *xform, EvoObject *obj);
void EVO_createEffectEntry(s32 index, s32 kind, s32 *vars, EvoEffectScript *loader);

EvoEffectScript *EVO_createEffectScript(EvoMsd *data);

s32 StoreImage(Rect16 *rect, void *p);
extern u16 EVO_BANNER_CLUT[16];
extern s8 EVO_BANNER_FADE;
extern u8 EVO_BANNER_BRIGHTNESS;
void EVO_drawFusionBanner(void);

const char D_801DDF38[] = "\t";

void EVO_initFusionBanner(void);

void EVO_shatterFusionModels(s32 unused);
void EVO_runFusion();

typedef struct {
    s32 count;
    s16 *queue;
    u8 pad8[0x26 - 0x8];
    s8 total;
    s8 next;
    s8 model;
} EvoShatter;
extern EvoShatter EVO_SHATTER;
extern s16 EVO_SHATTER_ORDER[40];
extern s16 EVO_SHATTER_TIMERS[42];
extern s16 EVO_PART_DRAW_MODES[40];
extern u16 EVO_SHATTER_DELAY;

s32 EVO_addShard(s32 part, s32 arg);
void EVO_drawShard(s32 index);

void EVO_startShatter(s32 model);

extern u32 EVO_RAND_SEED_LO;
extern u32 EVO_RAND_SEED_HI;
extern EvoSpark EVO_SPARKS[16];
void EVO_runSparkTask(EvoSpark *spark);

extern char *EVO_SORT_LABELS[];
extern s32 (*EVO_SORT_COMPARES[])(s8 *, s8 *);
extern EvoCardInfo *EVO_CARD_LIST[];
void EVO_initCardList(void);

extern void (*EVO_WINDOW_DRAW_FUNCS[])();
extern UiWindow EVO_SORT_WINDOW;
void EVO_drawCardList();
void EVO_drawRankUpBanner(UiWindow *w);
void EVO_drawSortMenu(UiWindow *w);
void EVO_drawTray(EvoTray *tray);

s32 EVO_typeTextLine(s32 x, s32 y, EvoText *t, s32 z);

extern CursorHighlight EVO_CARD_LIST_CURSOR;
extern CursorHighlight EVO_SORT_CURSOR;

void GsMulCoord3(MATRIX *m1, MATRIX *m2);

typedef struct {
    DrTPage tpage[2];
    PolyF4 poly[2];
    Bytes4 from;
    Bytes4 to;
    s16 speed;
    u8 state;
    u8 mode;
} EvoFadeRect;
void initPolyF4Pair();

typedef struct {
    EvoFx fx;
    Model *model;
    u8 pad140[0x160 - 0x140];
    EvoClut clut;
    s16 prevLevel;
    s8 slot;
    u8 unk56F;
    s8 unk570;
    s8 active;
    u8 pad572[2];
    s32 unk574;
} EvoModelFx;
void EVO_uploadShadedClut(EvoClut *clut, u16 flags);

void EVO_tickFusionTypeChoice(void);
void EVO_tickPartnerList(void);
void EVO_tickCardList(void);
void EVO_tickPartnerReward(void);
void EVO_tickFusionResult(void);
void EVO_showBothTrays(void);
void EVO_repickSecondCard(void);
void func_801EBE08(void);
void EVO_slideTrayOut(s32 index);
void EVO_slideTrayIn(s32 index);
void EVO_showCutsceneResult(void);
void EVO_closeFusionResult(void);
void EVO_renderFusion(void);
void EVO_runFusionCutscene(void);
void EVO_loadUnitTextures(void);
void EVO_loadCardImages(void);
void EVO_initFusionScene(void);
void EVO_loadEffectArchive(void);
void EVO_countSpareCards(void);
void EVO_openWindows(void);
void EVO_initScreenFlash(void);
EvoProgram *EVO_loadUnitScript(s32 index);
s32 *EVO_allocScriptRegisters(s32 count);
s32 EVO_tickFusionScript(EvoProgram *program);
void EVO_advanceText(void);
void EVO_loadScriptFlags(void);
void EVO_slideInFirstTray(void);
void EVO_swapToFirstTray(void);
void EVO_swapToSecondTray(void);
void EVO_slideOutFirstTray(void);
void EVO_cancelSecondCard(s32 active);
void EVO_saveScriptFlags(void);

extern s16 EVO_CURSOR_CARD;
void EVO_loadCardImage(s32 id, s32 slot);
s16 EVO_findFusionResult(void);
void EVO_checkCardCapacity(s16 cardId);

extern Bytes4 EVO_TEXT_COLORS[];
extern Bytes4 EVO_TEXT_COLOR_GREY;
extern Bytes4 EVO_TEXT_COLOR_RED;
extern const char EVO_FMT_CARD_NUMBER[];
extern const char EVO_FMT_CARD_COUNT[];
extern const char EVO_STR_CARDS[];

extern const char EVO_STR_SPEC[];

void EVO_initCutsceneScene(s8 evolved) {
    if (evolved == 0) {
        EVO_initShatterScene(1);
        createWireGrid(3000, 3000, 11, 11, 1, 0);
    } else {
        initScene3D(1);
        addFrameCallback((s32)renderWireGrid);
    }
    GRID_VISIBLE = 0;
    func_80014A00(0x19);
    func_800149B8(0x19, 0x1F, 0, 0x800, &runSceneCameraTask, 1);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void EVO_freeCutsceneScene(s8 evolved) {
    func_80014A00(0x1B);
    func_80014A00(0x19);
    removeFrameCallback((s32)renderWireGrid);
    removeFrameCallback((s32)EVO_tickShatter);
    if (evolved == 0) {
        removeFrameCallback((s32)EVO_renderCutsceneModels);
    } else {
        removeFrameCallback((s32)renderSceneModels);
    }
    func_80014C08(1);
    unloadAllModels();
    GRID_VISIBLE = 0;
    freeHeapBlocksByTag(0x7F);
    if (evolved == 0) {
        freeHeapBlocksByTag(0x1F5);
        freeHeapBlocksByTag(0x41);
    } else {
        freeHeapBlocksByTag(0x84);
    }
    freeHeapBlocksByTag(0x1F4);
    freeHeapBlocksByTag(0x40);
}

void EVO_restartModelAnimation(s16 slot) {
    applyAnimationFirstFrame(slot, 0);
    startModelAnimation(slot, 0, -2, 0);
}

void EVO_placeFusionModels(void) {
    Graphics *camera;
    EvoModel *model;
    EvoPart *part;
    s32 i;

    camera = (Graphics *)&GRAPHICS;
    camera->snapCamera = 1;
    camera->unk90 = 3000;
    camera->unk92 = -((EvoModel *)SCENE_3D->models[0])->pose->y * 3;
    camera->targetModel = 0;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = -1;
    GRID_VISIBLE = 1;
    EVO_restartModelAnimation(0);
    model = SCENE_3D->models[1];
    for (i = 0, part = model->parts; i < model->partCount; i++, part++) {
        EVO_remapPartTextures(part, 1);
    }
}

void EVO_showFusedDigimon(s32 id) {
    loadDigimonModelPak(0, id, 0, 0);
    func_80014C08(2);
    func_80014C08(20);
    setModelAnimationPose(0, 0);
    D_80079584 = 0;
    func_80014C08(1);
    SCENE_3D->modelState[0] = 1;
    playModelAnimation(0, 0);
    GRID_VISIBLE = 1;
    func_800149B8(0, -1, 0, 0x400, EVO_runFusedDigimonTask, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(10);
}

void EVO_initFusionBanner(void) {
    Rect16 rect = { 320, 240, 32, 1 };
    s8 i;

    StoreImage(&rect, EVO_BANNER_CLUT);
    for (i = 0; i < 16; i++) {
        if (EVO_BANNER_CLUT[i] != 0) {
            EVO_BANNER_CLUT[i] |= 0x8000;
        } else {
            EVO_BANNER_CLUT[i] = 0;
        }
    }
    LoadImage((s16 *)&rect, (s32)EVO_BANNER_CLUT);
    for (i = 0; i < 16; i++) {
        if (EVO_BANNER_CLUT[i] != 0) {
            EVO_BANNER_CLUT[i] = 0xFFFF;
        }
    }
    rect.y = 241;
    LoadImage((s16 *)&rect, (s32)EVO_BANNER_CLUT);
    EVO_BANNER_FADE = 2;
    EVO_BANNER_BRIGHTNESS = 0;
    addFrameCallback((s32)EVO_drawFusionBanner);
}

const u8 D_801DDF44[20][3] = {
    { 0x01, 0x04, 0xEC },
    { 0x04, 0x23, 0xEC },
    { 0x02, 0x25, 0xED },
    { 0x06, 0x27, 0xED },
    { 0x8E, 0x28, 0xEE },
    { 0x2A, 0x2B, 0xEF },
    { 0x4C, 0x07, 0xF0 },
    { 0x4D, 0x4B, 0xF1 },
    { 0x4B, 0x8F, 0xF2 },
    { 0x51, 0x03, 0xF3 },
    { 0x70, 0x4A, 0xF4 },
    { 0x4E, 0x6F, 0xF5 },
    { 0x6E, 0x6F, 0xF6 },
    { 0x6D, 0x90, 0xF7 },
    { 0x93, 0x73, 0xF8 },
    { 0x96, 0x74, 0xF9 },
    { 0x91, 0x26, 0xFA },
    { 0x0C, 0x75, 0xFB },
    { 0x53, 0x0D, 0xFC },
    { 0x52, 0x97, 0xFD },
};

void EVO_drawFusionBanner(void) {
    Rect16 uv;

    if (EVO_BANNER_FADE == 1) {
        EVO_BANNER_BRIGHTNESS += 8;
        if (EVO_BANNER_BRIGHTNESS > 0x80) {
            EVO_BANNER_BRIGHTNESS = 0x80;
        }
    } else if (EVO_BANNER_FADE == 2) {
        if (EVO_BANNER_BRIGHTNESS >= 8) {
            EVO_BANNER_BRIGHTNESS -= 8;
        } else {
            EVO_BANNER_BRIGHTNESS = 0;
        }
    }
    uv.x = 0;
    uv.y = 0x80;
    uv.w = 0xFF;
    uv.h = 0x27;
    drawTexturedSprite(0x28, 0xB4, &uv, 0x25, 0x3C14, 0, EVO_BANNER_BRIGHTNESS, 0);
    drawTexturedSprite(0x28, 0xB4, &uv, 0x45, 0x3C54, 0, EVO_BANNER_BRIGHTNESS, 0);
}

void EVO_runFusedDigimonTask(s32 parentTask) {
    s32 frames;

    EVO_initFusionBanner();
    setScreenFadeParams(1, 1, 8);
    playSoundEffect(0x8D);
    func_80014C08(120);
    EVO_BANNER_FADE = 1;
    func_80014C08(180);
    frames = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
        frames++;
        if ((PAD_STATES[0]->pressed & 0x40) || frames > 180) {
            EVO_BANNER_FADE = 2;
            EVO_CUTSCENE_STEP = 6;
        }
    } while (EVO_CUTSCENE_STEP != 6);
    EVO_SCREEN_FLASH.on = 1;
    func_80014C08(10);
    func_80014A48(parentTask);
}

void EVO_runShatterTask(s32 parentTask) {
    do {
        func_80014C08(FRAME_INTERVAL);
        switch (EVO_CUTSCENE_STEP) {
        case 1:
            func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 8, 0);
            func_80014C08(60);
            EVO_CUTSCENE_STEP = 2;
            SCENE_3D->modelState[0] = -1;
            break;
        case 2:
            playSoundEffect(0x8D);
            EVO_startShatter(1);
            SCENE_3D->modelState[1] = 1;
            applyAnimationFirstFrame(1, 0);
            startModelAnimation(1, 0, -2, 0);
            D_80079584 = 1;
            func_80014C08(5);
            setScreenFadeParams(1, 1, 8);
            EVO_CUTSCENE_STEP = 3;
            break;
        case 4:
            func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 8, 0);
            func_80014C08(60);
            EVO_CUTSCENE_STEP = 5;
            break;
        }
    } while (EVO_CUTSCENE_STEP != 5);
    func_80014C08(10);
    func_80014A48(parentTask);
}

void EVO_shatterFusionModels(s32 unused) {
    s32 models[2];

    models[0] = loadDigimonModelPak(0, EVO_CUTSCENE_MODELS[0], 1, 1);
    models[1] = loadDigimonModelPak(1, EVO_CUTSCENE_MODELS[1], 1, 1);
    EVO_placeFusionModels();
    D_80079584 = 0;
    func_80014C08(20);
    EVO_SCREEN_FLASH.on = 0;
    playSoundEffect(0x8D);
    func_800149B8(0, -1, 0, 0x400, EVO_runShatterTask, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(10);
}

void EVO_runFusionCutscene(void) {
    loadSoundEffectBank(0);
    playMusic(0, 0x88, 100);
    EVO_initCutsceneScene(0);
    EVO_shatterFusionModels(0x73);
    EVO_freeCutsceneScene(0);
    func_80014C08(2);
    playMusic(0, 0x89, 100);
    func_80014C08(2);
    EVO_initCutsceneScene(1);
    EVO_showFusedDigimon(EVO_CUTSCENE_MODELS[2]);
    EVO_freeCutsceneScene(1);
    func_80014C08(10);
    do {
        func_80014C08(1);
    } while (PAD_STATES[0]->pressed & 0x40);
    removeFrameCallback((s32)EVO_drawFusionBanner);
    loadSoundEffectBank(1);
    changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    func_800149B8(0, -1, 0, 0x400, EVO_runFusion, -1, getCurrentTaskId(), 0, 0);
}

void EVO_setGteMatrix(MATRIX *m) {
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
}

void EVO_renderCutsceneModels(FrameBuffer *buffer, s32 bufferIndex) {
    MATRIX localScreen;
    SVECTOR position;
    MATRIX unused;
    MATRIX lightMatrix;
    MATRIX colorMatrix;
    s32 flag;
    Model *model;
    GsDOBJ4 *obj;
    s32 i;
    s32 j;
    s32 *scratch;

    if (SCENE_3D_ENABLED != 0) {
        func_80062C34((long)buffer->scenePackets);
        colorMatrix = SCENE_LIGHT_COLORS;
        for (i = 0; i < 24; i++) {
            if (SCENE_3D->modelState[i] <= 0) {
                continue;
            }
            model = SCENE_3D->models[i];
            lightMatrix = SCENE_LIGHT_MATRIX;
            gte_SetColorMatrix(&colorMatrix);
            PushMatrix();
            position.vx = model->pos.vx;
            position.vy = model->pos.vy;
            position.vz = model->pos.vz;
            RotTrans((u16 *)&position, localScreen.t, &flag);
            RotMatrix(&SCENE_WORLD_ROTATION, &SCENE_3D->root.coord);
            SCENE_3D->root.flg = 0;
            SCENE_3D->root.coord.t[0] = localScreen.t[0];
            SCENE_3D->root.coord.t[1] = localScreen.t[1];
            SCENE_3D->root.coord.t[2] = localScreen.t[2];
            RotMatrixYXZ(&model->rot, &model->root.coord);
            model->root.flg = 0;
            ScaleMatrix(&model->root.coord, &model->scale);
            if (EVO_SHATTER_STARTED == 0) {
                EVO_startShatter(0);
                EVO_SHATTER_STARTED = 1;
            }
            scratch = (s32 *)0x1F800000;
            scratch[12] = model->tpageOffset;
            scratch[13] = model->clutOffset;
            obj = model->obj;
            for (j = 0; j < model->nobj; j++, obj++) {
                obj->coord2->flg = 0;
                if (obj->id != -1 && obj->tmd != NULL && obj->attribute == 0) {
                    GsGetLws(obj->coord2, &model->lw[j], &localScreen);
                    GsGetLs(obj->coord2, &localScreen);
                    GsSetLsMatrix(&localScreen);
                    GsSetLightMatrix(&localScreen);
                    if (EVO_PART_DRAW_MODES[j] != 16) {
                        EVO_renderDissolvingObject(obj, EVO_PART_DRAW_MODES[j]);
                    } else {
                        GsSortObject4(obj, &SCENE_3D->ot[bufferIndex], 2, (u32 *)0x1F800000);
                    }
                }
            }
            PopMatrix();
        }
    }
}

void EVO_initView(s32 projection) {
    func_8005C484(160, 120);
    func_8005C4A4(projection);
    func_80062484(projection);
    SCENE_3D->view.vpx = 0;
    SCENE_3D->view.vpy = 0;
    SCENE_3D->view.vpz = 0;
    SCENE_3D->view.vrx = 0;
    SCENE_3D->view.vry = 0;
    SCENE_3D->view.vrz = 0;
    SCENE_3D->view.rz = 0;
    SCENE_3D->view.super = NULL;
    GsSetRefView2(&SCENE_3D->view);
}

void EVO_initLights(void) {
    ((FlatLight *)SCENE_3D->unkE4)[0].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[0].vy = -100;
    ((FlatLight *)SCENE_3D->unkE4)[0].vz = 100;
    ((FlatLight *)SCENE_3D->unkE4)[0].r = 0xFF;
    ((FlatLight *)SCENE_3D->unkE4)[0].g = 0xFF;
    ((FlatLight *)SCENE_3D->unkE4)[0].b = 0xFF;
    GsSetFlatLight(0, &((FlatLight *)SCENE_3D->unkE4)[0]);
    ((FlatLight *)SCENE_3D->unkE4)[1].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[1].vy = 100;
    ((FlatLight *)SCENE_3D->unkE4)[1].vz = 100;
    ((FlatLight *)SCENE_3D->unkE4)[1].r = 0x80;
    ((FlatLight *)SCENE_3D->unkE4)[1].g = 0x80;
    ((FlatLight *)SCENE_3D->unkE4)[1].b = 0x80;
    GsSetFlatLight(1, &((FlatLight *)SCENE_3D->unkE4)[1]);
    ((FlatLight *)SCENE_3D->unkE4)[2].vx = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].vy = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].vz = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].r = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].g = 0;
    ((FlatLight *)SCENE_3D->unkE4)[2].b = 0;
    GsSetFlatLight(2, &((FlatLight *)SCENE_3D->unkE4)[2]);
    GsSetAmbient(0x40, 0x40, 0x40);
    func_8005C464(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

void EVO_initShatterScene(s32 allocBuffers) {
    s32 i;

    EVO_initGsSortTable();
    initModelScene();
    func_8006A804();
    for (i = 0; i < 2; i++) {
        if (allocBuffers) {
            DB(i).scenePackets = allocHeapBlock(0x1F400, 0x7F);
        }
        SCENE_3D->ot[i].length = 12;
        SCENE_3D->ot[i].org = DB(i).ot;
        SCENE_3D->ot[i].offset = 0;
        SCENE_3D->ot[i].point = 0;
        SCENE_3D->ot[i].tag = SCENE_3D->ot[i].org + 0xFFF;
    }
    EVO_SHARDS = allocHeapBlock(0x4B0, 0x7F);
    EVO_SHARD_VERTEX_POOL = allocHeapBlock(0x3E80, 0x7F);
    EVO_SHATTER_STARTED = 0;
    EVO_CUTSCENE_STEP = 0;
    GsInit3D();
    EVO_initView(0x1C0);
    EVO_initLights();
    func_8006A814();
    {
        MATRIX lightMatrices[2] = {
            { { { 0, 0x1800, -0x1800 }, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
            { { { 0, 0x1000, -0x5DC }, { 0, -0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
        };
        MATRIX colorMatrices[2] = {
            { { { 0x800, 0, 0 }, { 0x800, 0, 0 }, { 0x800, 0, 0 } }, { 0, 0, 0 } },
            { { { 0x1000, 0x5DC, 0 }, { 0xFF5, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
        };

        SCENE_LIGHT_MATRIX = lightMatrices[1];
        SCENE_LIGHT_COLORS = colorMatrices[1];
    }
    if (allocBuffers) {
        mountDriveTask((s32)"M:", getCurrentTaskId());
        addFrameCallback((s32)EVO_renderCutsceneModels);
    }
}

void EVO_remapPartTextures(EvoPart *part, s32 enable) {
    TmdObject *tmd;
    s32 count;
    TmdPrim *prim;
    u8 *next;
    s32 i;

    tmd = part->tmd;
    count = tmd->nprim;
    prim = tmd->prims;
    if (enable == 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        if ((prim->mode & 0x3D) == 0x2C) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x20;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x3C) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x24;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x34) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x1C;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x24) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x18;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x2D) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x20;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x28) {
            next += 0x14;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x25) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x1C;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x38) {
            next += 0x18;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x30) {
            next += 0x14;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x20) {
            next += 0x10;
            prim = (TmdPrim *)next;
        }
    }
}

/* libgs TMD primitive layouts */
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_F3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_G3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p;
} TMD_P_F4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_G4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_TF3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_TG3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 r0, g0, b0, p1;
    u16 v0, v1;
    u16 v2, p2;
} TMD_P_TNF3;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p2;
} TMD_P_TF4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_TG4;
typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u8 r0, g0, b0, p2;
    u16 v0, v1;
    u16 v2, v3;
} TMD_P_TNF4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
    u32 pad;
} LINE_F4;

/* the GPU packet being built: one of these, or a shattered wireframe */
typedef union {
    PolyF3 f3;
    PolyF4 f4;
    PolyG3 g3;
    PolyG4 g4;
    POLY_FT3 ft3;
    POLY_FT4 ft4;
    POLY_GT3 gt3;
    POLY_GT4 gt4;
    LINE_F4 line;
} EvoPacket;

extern MATRIX D_801DBE40;
extern s16 EVO_WIRE_SHADE_MIN;
extern s16 EVO_WIRE_SHADE_MAX;
extern s8 EVO_DISSOLVE_PATTERN[16];
void MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *m2);
void SetLightMatrix(MATRIX *m);
s32 RotNclip3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, s32 *sxy0, s32 *sxy1, s32 *sxy2, s32 *p, s32 *otz, s32 *flag);
s32 RotNclip4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, s32 *sxy0, s32 *sxy1, s32 *sxy2, s32 *sxy3, s32 *p,
              s32 *otz, s32 *flag);
void NormalColorCol3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, Color *in, Color *out0, Color *out1, Color *out2);
void func_8005FBE4(SVECTOR *n, Color *in, Color *out);

/* Wireframe of a quad that is shattering: a closed 4-point polyline v0 v1 v3 v2 */
#define SHATTER_QUAD(pk, shade, otz)                        \
    {                                                       \
        LINE_F2 *line;                                      \
        setlen(&(pk)->line, 6);                             \
        setcode(&(pk)->line, 0x4C);                         \
        (pk)->line.pad = 0x55555555;                        \
        (pk)->line.r0 = (pk)->line.g0 = (pk)->line.b0 = shade; \
        otz >>= 2;                                          \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &(pk)->line); \
        line = (LINE_F2 *)(&(pk)->line + 1);                \
        setlen(line, 3);                                    \
        setcode(line, 0x40);                                \
        line->r0 = line->g0 = line->b0 = shade;             \
        line->x0 = (pk)->line.x3;                           \
        line->y0 = (pk)->line.y3;                           \
        line->x1 = (pk)->line.x0;                           \
        line->y1 = (pk)->line.y0;                           \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);      \
        pk = (EvoPacket *)(line + 1);                       \
    }

/* Wireframe of a triangle that is shattering: a closed polyline v0 v1 v2 v0 */
#define SHATTER_TRI(pk, shade, otz)                           \
    {                                                         \
        (pk)->line.x3 = (pk)->line.x0;                        \
        (pk)->line.y3 = (pk)->line.y0;                        \
        setlen(&(pk)->line, 6);                               \
        setcode(&(pk)->line, 0x4C);                           \
        (pk)->line.pad = 0x55555555;                          \
        (pk)->line.r0 = (pk)->line.g0 = (pk)->line.b0 = shade; \
        otz >>= 2;                                            \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &(pk)->line); \
        pk = (EvoPacket *)(&(pk)->line + 1);                  \
    }

void EVO_renderDissolvingObject(GsDOBJ4 *obj, s32 mode) {
    Color rgb;
    MATRIX m;
    s32 p;
    s32 otz;
    s32 flag;
    s32 i;
    s32 nprim;
    SVECTOR *norm;
    SVECTOR *vert;
    TmdPrim *prim;
    EvoPacket *pk;
    TmdObject *tmd;
    GsCOORDINATE2 *coord;
    s8 shade;
    union {
        TMD_P_F3 *f3;
        TMD_P_G3 *g3;
        TMD_P_F4 *f4;
        TMD_P_G4 *g4;
        TMD_P_TF3 *tf3;
        TMD_P_TG3 *tg3;
        TMD_P_TNF3 *tnf3;
        TMD_P_TF4 *tf4;
        TMD_P_TG4 *tg4;
        TMD_P_TNF4 *tnf4;
    } op;
    s32 code;

    shade = EVO_WIRE_SHADE_MIN + rand() % (EVO_WIRE_SHADE_MAX - EVO_WIRE_SHADE_MIN);
    tmd = (TmdObject *)obj->tmd;
    vert = tmd->vertTop;
    norm = tmd->normTop;
    nprim = tmd->nprim;
    prim = tmd->prims;
    pk = (EvoPacket *)func_80062C44();
    rgb.r = rgb.g = rgb.b = 0x80;
    coord = obj->coord2;
    if (coord->flg == 0) {
        coord->flg = 1;
        MulMatrix0(&coord->coord, &coord->super->workm, &coord->workm);
    }
    MulMatrix0(&D_801DBE40, &coord->workm, &m);
    SetLightMatrix(&m);
    CompMatrix(&D_801DBEA0, &coord->workm, &m);
    EVO_setGteMatrix(&m);
    for (i = 0; i < nprim; i++) {
        if ((code = prim->mode & 0x3D) == 0x2C) {
            op.tf4 = (TMD_P_TF4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tf4->v0], &vert[op.tf4->v1], &vert[op.tf4->v2], &vert[op.tf4->v3],
                              (s32 *)&pk->ft4.x0, (s32 *)&pk->ft4.x1, (s32 *)&pk->ft4.x2, (s32 *)&pk->ft4.x3, &p, &otz,
                              &flag) > 0) {
                    func_8005FBE4(&norm[op.tf4->n0], &rgb, (Color *)&pk->ft4.r0);
                    pk->ft4.u0 = op.tf4->tu0;
                    pk->ft4.v0 = op.tf4->tv0;
                    pk->ft4.u1 = op.tf4->tu1;
                    pk->ft4.v1 = op.tf4->tv1;
                    pk->ft4.u2 = op.tf4->tu2;
                    pk->ft4.v2 = op.tf4->tv2;
                    pk->ft4.u3 = op.tf4->tu3;
                    pk->ft4.v3 = op.tf4->tv3;
                    pk->ft4.clut = op.tf4->clut;
                    pk->ft4.tpage = op.tf4->tpage;
                    setlen(&pk->ft4, 9);
                    setcode(&pk->ft4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft4);
                    pk = (EvoPacket *)(&pk->ft4 + 1);
                }
                prim = (TmdPrim *)(op.tf4 + 1);
            } else {
                if (RotNclip4(&vert[op.tf4->v0], &vert[op.tf4->v1], &vert[op.tf4->v2], &vert[op.tf4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tf4 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x3C) {
            op.tg4 = (TMD_P_TG4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tg4->v0], &vert[op.tg4->v1], &vert[op.tg4->v2], &vert[op.tg4->v3],
                              (s32 *)&pk->gt4.x0, (s32 *)&pk->gt4.x1, (s32 *)&pk->gt4.x2, (s32 *)&pk->gt4.x3, &p, &otz,
                              &flag) > 0) {
                    NormalColorCol3(&norm[op.tg4->n0], &norm[op.tg4->n1], &norm[op.tg4->n2], &rgb, (Color *)&pk->gt4.r0,
                                    (Color *)&pk->gt4.r1, (Color *)&pk->gt4.r2);
                    func_8005FBE4(&norm[op.tg4->n3], &rgb, (Color *)&pk->gt4.r3);
                    pk->gt4.u0 = op.tg4->tu0;
                    pk->gt4.v0 = op.tg4->tv0;
                    pk->gt4.u1 = op.tg4->tu1;
                    pk->gt4.v1 = op.tg4->tv1;
                    pk->gt4.u2 = op.tg4->tu2;
                    pk->gt4.v2 = op.tg4->tv2;
                    pk->gt4.u3 = op.tg4->tu3;
                    pk->gt4.v3 = op.tg4->tv3;
                    pk->gt4.clut = op.tg4->clut;
                    pk->gt4.tpage = op.tg4->tpage;
                    setlen(&pk->gt4, 12);
                    setcode(&pk->gt4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->gt4);
                    pk = (EvoPacket *)(&pk->gt4 + 1);
                }
                prim = (TmdPrim *)(op.tg4 + 1);
            } else {
                if (RotNclip4(&vert[op.tg4->v0], &vert[op.tg4->v1], &vert[op.tg4->v2], &vert[op.tg4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tg4 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x34) {
            op.tg3 = (TMD_P_TG3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tg3->v0], &vert[op.tg3->v1], &vert[op.tg3->v2], (s32 *)&pk->gt3.x0,
                              (s32 *)&pk->gt3.x1, (s32 *)&pk->gt3.x2, &p, &otz, &flag) > 0) {
                    NormalColorCol3(&norm[op.tg3->n0], &norm[op.tg3->n1], &norm[op.tg3->n2], &rgb, (Color *)&pk->gt3.r0,
                                    (Color *)&pk->gt3.r1, (Color *)&pk->gt3.r2);
                    pk->gt3.u0 = op.tg3->tu0;
                    pk->gt3.v0 = op.tg3->tv0;
                    pk->gt3.u1 = op.tg3->tu1;
                    pk->gt3.v1 = op.tg3->tv1;
                    pk->gt3.u2 = op.tg3->tu2;
                    pk->gt3.v2 = op.tg3->tv2;
                    pk->gt3.clut = op.tg3->clut;
                    pk->gt3.tpage = op.tg3->tpage;
                    setlen(&pk->gt3, 9);
                    setcode(&pk->gt3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->gt3);
                    pk = (EvoPacket *)(&pk->gt3 + 1);
                }
                prim = (TmdPrim *)(op.tg3 + 1);
            } else {
                if (RotNclip3(&vert[op.tg3->v0], &vert[op.tg3->v1], &vert[op.tg3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tg3 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x24) {
            op.tf3 = (TMD_P_TF3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tf3->v0], &vert[op.tf3->v1], &vert[op.tf3->v2], (s32 *)&pk->ft3.x0,
                              (s32 *)&pk->ft3.x1, (s32 *)&pk->ft3.x2, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.tf3->n0], &rgb, (Color *)&pk->ft3.r0);
                    pk->ft3.u0 = op.tf3->tu0;
                    pk->ft3.v0 = op.tf3->tv0;
                    pk->ft3.u1 = op.tf3->tu1;
                    pk->ft3.v1 = op.tf3->tv1;
                    pk->ft3.u2 = op.tf3->tu2;
                    pk->ft3.v2 = op.tf3->tv2;
                    pk->ft3.clut = op.tf3->clut;
                    pk->ft3.tpage = op.tf3->tpage;
                    setlen(&pk->ft3, 7);
                    setcode(&pk->ft3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft3);
                    pk = (EvoPacket *)(&pk->ft3 + 1);
                }
                prim = (TmdPrim *)(op.tf3 + 1);
            } else {
                if (RotNclip3(&vert[op.tf3->v0], &vert[op.tf3->v1], &vert[op.tf3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tf3 + 1);
            }
        } else if ((prim->mode & 0x3D) == 0x2D) {
            op.tnf4 = (TMD_P_TNF4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tnf4->v0], &vert[op.tnf4->v1], &vert[op.tnf4->v2], &vert[op.tnf4->v3],
                              (s32 *)&pk->ft4.x0, (s32 *)&pk->ft4.x1, (s32 *)&pk->ft4.x2, (s32 *)&pk->ft4.x3, &p, &otz,
                              &flag) > 0) {
                    pk->ft4.u0 = op.tnf4->tu0;
                    pk->ft4.v0 = op.tnf4->tv0;
                    pk->ft4.u1 = op.tnf4->tu1;
                    pk->ft4.v1 = op.tnf4->tv1;
                    pk->ft4.u2 = op.tnf4->tu2;
                    pk->ft4.v2 = op.tnf4->tv2;
                    pk->ft4.u3 = op.tnf4->tu3;
                    pk->ft4.v3 = op.tnf4->tv3;
                    pk->ft4.clut = op.tnf4->clut;
                    pk->ft4.tpage = op.tnf4->tpage;
                    setlen(&pk->ft4, 9);
                    setcode(&pk->ft4, 0x2C);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft4);
                    pk = (EvoPacket *)(&pk->ft4 + 1);
                }
                prim = (TmdPrim *)(op.tnf4 + 1);
            } else {
                if (RotNclip4(&vert[op.tnf4->v0], &vert[op.tnf4->v1], &vert[op.tnf4->v2], &vert[op.tnf4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tnf4 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x28) {
            op.f4 = (TMD_P_F4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.f4->v0], &vert[op.f4->v1], &vert[op.f4->v2], &vert[op.f4->v3], (s32 *)&pk->f4.x0,
                              (s32 *)&pk->f4.x1, (s32 *)&pk->f4.x2, (s32 *)&pk->f4.x3, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.f4->n0], &rgb, (Color *)&pk->f4.r0);
                    setlen(&pk->f4, 5);
                    setcode(&pk->f4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->f4);
                    pk = (EvoPacket *)(&pk->f4 + 1);
                }
                prim = (TmdPrim *)(op.f4 + 1);
            } else {
                if (RotNclip4(&vert[op.f4->v0], &vert[op.f4->v1], &vert[op.f4->v2], &vert[op.f4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.f4 + 1);
            }
        } else if ((prim->mode & 0x35) == 0x25) {
            op.tnf3 = (TMD_P_TNF3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tnf3->v0], &vert[op.tnf3->v1], &vert[op.tnf3->v2], (s32 *)&pk->ft3.x0,
                              (s32 *)&pk->ft3.x1, (s32 *)&pk->ft3.x2, &p, &otz, &flag) > 0) {
                    pk->ft3.u0 = op.tnf3->tu0;
                    pk->ft3.v0 = op.tnf3->tv0;
                    pk->ft3.u1 = op.tnf3->tu1;
                    pk->ft3.v1 = op.tnf3->tv1;
                    pk->ft3.u2 = op.tnf3->tu2;
                    pk->ft3.v2 = op.tnf3->tv2;
                    pk->ft3.clut = op.tnf3->clut;
                    pk->ft3.tpage = op.tnf3->tpage;
                    setlen(&pk->ft3, 7);
                    setcode(&pk->ft3, 0x24);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft3);
                    pk = (EvoPacket *)(&pk->ft3 + 1);
                }
                prim = (TmdPrim *)(op.tnf3 + 1);
            } else {
                if (RotNclip3(&vert[op.tnf3->v0], &vert[op.tnf3->v1], &vert[op.tnf3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tnf3 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x38) {
            op.g4 = (TMD_P_G4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.g4->v0], &vert[op.g4->v1], &vert[op.g4->v2], &vert[op.g4->v3], (s32 *)&pk->g4.x0,
                              (s32 *)&pk->g4.x1, (s32 *)&pk->g4.x2, (s32 *)&pk->g4.x3, &p, &otz, &flag) > 0) {
                    NormalColorCol3(&norm[op.g4->n0], &norm[op.g4->n1], &norm[op.g4->n2], &rgb, (Color *)&pk->g4.r0,
                                    (Color *)&pk->g4.r1, (Color *)&pk->g4.r2);
                    func_8005FBE4(&norm[op.g4->n3], &rgb, (Color *)&pk->g4.r3);
                    setlen(&pk->g4, 8);
                    setcode(&pk->g4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->g4);
                    pk = (EvoPacket *)(&pk->g4 + 1);
                }
                prim = (TmdPrim *)(op.g4 + 1);
            } else {
                if (RotNclip4(&vert[op.g4->v0], &vert[op.g4->v1], &vert[op.g4->v2], &vert[op.g4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.g4 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x30) {
            op.g3 = (TMD_P_G3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.g3->v0], &vert[op.g3->v1], &vert[op.g3->v2], (s32 *)&pk->g3.x0,
                              (s32 *)&pk->g3.x1, (s32 *)&pk->g3.x2, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.g3->n0], &rgb, (Color *)&pk->g3.r0);
                    setlen(&pk->g3, 6);
                    setcode(&pk->g3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->g3);
                    pk = (EvoPacket *)(&pk->g3 + 1);
                }
                prim = (TmdPrim *)(op.g3 + 1);
            } else {
                if (RotNclip3(&vert[op.g3->v0], &vert[op.g3->v1], &vert[op.g3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.g3 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x20) {
            op.f3 = (TMD_P_F3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.f3->v0], &vert[op.f3->v1], &vert[op.f3->v2], (s32 *)&pk->f3.x0,
                              (s32 *)&pk->f3.x1, (s32 *)&pk->f3.x2, &p, &otz, &flag) > 0) {
                    func_8005FBE4(&norm[op.f3->n0], &rgb, (Color *)&pk->f3.r0);
                    setlen(&pk->f3, 4);
                    setcode(&pk->f3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->f3);
                    pk = (EvoPacket *)(&pk->f3 + 1);
                }
                prim = (TmdPrim *)(op.f3 + 1);
            } else {
                if (RotNclip3(&vert[op.f3->v0], &vert[op.f3->v1], &vert[op.f3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.f3 + 1);
            }
        }
    }
    func_80062C34((long)pk);
}

void EVO_initGsSortTable(void) {
    D_801DBFB0.f3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF3L;
    D_801DBFB0.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF3L;
    D_801DBFB0.tf3[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTF3NL;
    D_801DBFB0.ntf3[GsDivMODE_NDIV] = GsTMDfastTNF3;
    D_801DBFB0.g3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG3L;
    D_801DBFB0.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG3L;
    D_801DBFB0.tg3[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTG3NL;
    D_801DBFB0.ntg3[GsDivMODE_NDIV] = GsTMDfastTNG3;
    D_801DBFB0.f4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF4L;
    D_801DBFB0.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF4L;
    D_801DBFB0.tf4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTF4NL;
    D_801DBFB0.ntf4[GsDivMODE_NDIV] = GsTMDfastTNF4;
    D_801DBFB0.g4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG4L;
    D_801DBFB0.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG4L;
    D_801DBFB0.tg4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTG4NL;
    D_801DBFB0.ntg4[GsDivMODE_NDIV] = GsTMDfastTNG4;
    D_801DBFB0.tf4[GsDivMODE_DIV][GsLMODE_NORMAL] = GsTMDdivTF4L;
    D_801DBFB0.tf4[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTF4NL;
    D_801DBFB0.tf3[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTF3NL;
    D_801DBFB0.tg4[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTG4NL;
    D_801DBFB0.tg3[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTG3NL;
    D_801DBFB0.ntg4[GsDivMODE_DIV] = GsTMDdivTNG4;
    D_801DBFB0.ntg3[GsDivMODE_DIV] = GsTMDdivTNG3;
    D_801DBFB0.ntf4[GsDivMODE_DIV] = GsTMDdivTNF4;
    D_801DBFB0.ntf3[GsDivMODE_DIV] = GsTMDdivTNF3;
    D_801DBFB0.f4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastF4NL;
    D_801DBFB0.nf4[GsDivMODE_NDIV] = GsTMDfastNF4;
}

void EVO_getCoordWorldMatrix(GsCOORDINATE2 *coord, MATRIX *m) {
    GsCOORDINATE2 *chain[100];
    GsCOORDINATE2 **p;

    chain[0] = coord;
    p = &chain[1];
    while (coord->super != NULL) {
        coord = coord->super;
        *p++ = coord;
    }
    p--;
    *m = (*p)->coord;
    while (chain < p) {
        p--;
        GsMulCoord3(m, &(*p)->coord);
    }
}

void EVO_getPartWorldMatrix(s32 part, MATRIX *m) {
    GsCOORDINATE2 root;
    GsCOORDINATE2 node;
    VECTOR trans;

    trans = *(VECTOR *)((Model *)SCENE_3D->models[0])->bonepos[0];
    root = ((Model *)SCENE_3D->models[0])->coord[0];
    node = ((Model *)SCENE_3D->models[0])->coord[part];
    RotMatrix(&((Model *)SCENE_3D->models[0])->rots[0], &root.coord);
    ScaleMatrix(&root.coord, ((Model *)SCENE_3D->models[0])->boneScale);
    TransMatrix(&root.coord, &trans);
    EVO_getCoordWorldMatrix(&node, m);
}

void EVO_startShatter(s32 model) {
    s32 i;
    s32 j;
    s16 tmp;
    u8 unused[0x50]; /* stack space the original reserves but never touches */

    EVO_SHATTER.total = -1;
    EVO_SHATTER.next = 0;
    EVO_SHATTER.count = 0;
    EVO_SHATTER.model = model;
    EVO_SHARD_VERTEX_CURSOR = EVO_SHARD_VERTEX_POOL;
    for (i = 0; i < 60; i++) {
        EVO_SHARDS[i].timer = -1;
    }
    for (i = 0; i < 40; i++) {
        EVO_PART_DRAW_MODES[i] = 0x10;
    }
    EVO_SHATTER.total = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->partCount;
    EVO_SHATTER.queue = EVO_SHATTER_ORDER;
    for (i = 0; i < EVO_SHATTER.total; i++) {
        EVO_SHATTER_TIMERS[i] = 1;
    }
    for (i = 0; i < EVO_SHATTER.total; i++) {
        EVO_SHATTER_ORDER[i] = i;
    }
    for (i = 0; i < EVO_SHATTER.total; i++) {
        j = rand() % EVO_SHATTER.total;
        tmp = EVO_SHATTER_ORDER[i];
        EVO_SHATTER_ORDER[i] = EVO_SHATTER_ORDER[j];
        EVO_SHATTER_ORDER[j] = tmp;
    }
    if (EVO_SHATTER_STARTED == 0) {
        addFrameCallback((s32)EVO_tickShatter);
        EVO_SHATTER_STARTED = 1;
    }
    EVO_SHATTER_DELAY = 120;
}

void EVO_tickShatter(void) {
    s32 part;
    s32 i;

    if (EVO_SHATTER.next < EVO_SHATTER.total) {
        if (EVO_SHATTER_DELAY != 0) {
            EVO_SHATTER_DELAY--;
        } else {
            EVO_SHATTER_TIMERS[EVO_SHATTER.next]--;
        }
        if (EVO_SHATTER_TIMERS[EVO_SHATTER.next] < 0) {
            part = *EVO_SHATTER.queue++;
            EVO_SHATTER.next++;
            EVO_addShard(part, part);
            EVO_PART_DRAW_MODES[part] = 0;
            playSoundEffect(0x58);
        }
    }
    if (EVO_SHATTER.next > EVO_SHATTER.total - 1) {
        if (EVO_CUTSCENE_STEP == 0) {
            EVO_CUTSCENE_STEP = 1;
        } else if (EVO_CUTSCENE_STEP == 3) {
            EVO_CUTSCENE_STEP = 4;
        }
    }
    for (i = 0; i < EVO_SHATTER.count; i++) {
        EVO_drawShard(i);
    }
}

void EVO_drawShard(s32 index) {
    EvoShard *shard;
    SVECTOR *offset;
    s32 count;

    shard = &EVO_SHARDS[index];
    offset = shard->offsets;
    count = shard->primCount;
    EVO_SHARD_PRIM = shard->prims;
    EVO_SHARD_VERTS = shard->verts;
    if (shard->timer < 120) {
        if (shard->timer >= 0) {
            shard->timer++;
            EVO_SHARD_COLOR.r = (61 - shard->timer) * 74 / 60 + 54;
            EVO_SHARD_COLOR.g = EVO_SHARD_COLOR.r;
            EVO_SHARD_COLOR.b = EVO_SHARD_COLOR.r;
            while (count-- > 0) {
                if (EVO_SHARD_PRIM[3] == 0x34 || EVO_SHARD_PRIM[3] == 0x36) {
                    EVO_drawShardTG3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x1C;
                } else if (EVO_SHARD_PRIM[3] == 0x24) {
                    EVO_drawShardTF3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x18;
                } else if (EVO_SHARD_PRIM[3] == 0x3C || EVO_SHARD_PRIM[3] == 0x3E) {
                    EVO_drawShardTG4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x24;
                } else if (EVO_SHARD_PRIM[3] == 0x2C) {
                    EVO_drawShardTF4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x20;
                } else if (EVO_SHARD_PRIM[3] == 0x25) {
                    EVO_drawShardTNF3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x1C;
                } else if (EVO_SHARD_PRIM[3] == 0x2D || EVO_SHARD_PRIM[3] == 0x2F) {
                    EVO_drawShardTNF4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x20;
                } else if (EVO_SHARD_PRIM[3] == 0x28) {
                    EVO_drawShardF4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x14;
                } else if (EVO_SHARD_PRIM[3] == 0x30) {
                    EVO_drawShardG3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x14;
                } else if (EVO_SHARD_PRIM[3] == 0x38) {
                    EVO_drawShardG4(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x18;
                } else if (EVO_SHARD_PRIM[3] == 0x20) {
                    EVO_drawShardF3(offset, 0, 120, shard->timer);
                    offset++;
                    EVO_SHARD_PRIM += 0x10;
                }
            }
        }
    }
}

void EVO_drawShardTG3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)EVO_SHARD_PRIM;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardTF3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)EVO_SHARD_PRIM;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardTG4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)EVO_SHARD_PRIM;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[5]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[7]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardTF4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)EVO_SHARD_PRIM;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[1]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[4]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardTNF3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT3 *prim;
    POLY_FT3 *poly;

    prim = (TmdPacketFT3 *)EVO_SHARD_PRIM;
    poly = (POLY_FT3 *)func_80062C44();
    func_80067724(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 7);
    setcode(poly, 0x24);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardTNF4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    TmdPacketFT4 *prim;
    POLY_FT4 *poly;

    prim = (TmdPacketFT4 *)EVO_SHARD_PRIM;
    poly = (POLY_FT4 *)func_80062C44();
    func_800677A4(poly);
    SetSemiTrans(poly, 0);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    poly->tpage = prim->tsb;
    poly->clut = prim->cba;
    poly->u0 = prim->u0;
    poly->v0 = prim->v0;
    poly->u1 = prim->u1;
    poly->v1 = prim->v1;
    poly->u2 = prim->u2;
    poly->v2 = prim->v2;
    poly->u3 = prim->u3;
    poly->v3 = prim->v3;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim->idx[2]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[3]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[4]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim->idx[5]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

s32 EVO_addShard(s32 part, s32 arg) {
    SVECTOR out;
    MATRIX rot;
    MATRIX world;
    SVECTOR n0;
    SVECTOR n1;
    SVECTOR n2;
    SVECTOR n3;
    SVECTOR center;
    MATRIX unused;
    TmdObject *obj;
    SVECTOR *vert;
    SVECTOR *verts;
    SVECTOR *a;
    SVECTOR *b;
    SVECTOR *c;
    SVECTOR *d;
    u8 *prim;
    EvoShard *shard;
    s32 slot;
    u32 i;
    u32 k;

    obj = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->parts[part].tmd;
    if (obj == NULL) {
        return -1;
    }
    for (slot = 0; slot < 30 && EVO_SHARDS[slot].timer >= 0; slot++) {
    }
    if (slot == 30) {
        return -1;
    }
    PushMatrix();
    rot = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->matrices[part];
    verts = EVO_SHARD_VERTEX_CURSOR;
    vert = obj->vertTop;
    world = ((EvoModel *)SCENE_3D->models[EVO_SHATTER.model])->matrices[part];
    for (i = 0; i < obj->nvert; i++) {
        ApplyMatrixSV(&world, vert++, &out);
        EVO_SHARD_VERTEX_CURSOR->vx = out.vx + world.t[0];
        EVO_SHARD_VERTEX_CURSOR->vy = out.vy + world.t[1];
        EVO_SHARD_VERTEX_CURSOR->vz = out.vz + world.t[2];
        EVO_SHARD_VERTEX_CURSOR++;
    }
    shard = &EVO_SHARDS[slot];
    shard->timer = 0;
    shard->count = obj->nprim;
    shard->offsets = EVO_SHARD_VERTEX_CURSOR;
    shard->part = arg;
    shard->verts = verts;
    shard->prims = (s8 *)obj->primTop;
    shard->primCount = obj->nprim;
    prim = obj->primTop;
    for (k = 0; k < obj->nprim; k++) {
        switch ((s8)(prim[3] - 0x20)) {
        case 0x14:
        case 0x16:
            a = &obj->normTop[*(u16 *)(prim + 0x10)];
            b = &obj->normTop[*(u16 *)(prim + 0x14)];
            c = &obj->normTop[*(u16 *)(prim + 0x18)];
            ApplyMatrixSV(&rot, a, &n0);
            ApplyMatrixSV(&rot, b, &n1);
            ApplyMatrixSV(&rot, c, &n2);
            center.vx = (n0.vx + n1.vx + n2.vx) / 3;
            center.vy = (n0.vy + n1.vy + n2.vy) / 3;
            center.vz = (n0.vz + n1.vz + n2.vz) / 3;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x1C;
            break;
        case 0x1C:
        case 0x1E:
            a = &obj->normTop[*(u16 *)(prim + 0x14)];
            b = &obj->normTop[*(u16 *)(prim + 0x18)];
            c = &obj->normTop[*(u16 *)(prim + 0x1C)];
            d = &obj->normTop[*(u16 *)(prim + 0x20)];
            ApplyMatrixSV(&rot, a, &n0);
            ApplyMatrixSV(&rot, b, &n1);
            ApplyMatrixSV(&rot, c, &n2);
            ApplyMatrixSV(&rot, d, &n3);
            center.vx = (n0.vx + n1.vx + n2.vx) / 3;
            center.vy = (n0.vy + n1.vy + n2.vy) / 3;
            center.vz = (n0.vz + n1.vz + n2.vz) / 3;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x24;
            break;
        case 0x0C:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x14)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x20;
            break;
        case 0x04:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x10)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x18;
            break;
        case 0x0D:
        case 0x0F:
            ApplyMatrixSV(&rot, &obj->normTop[1], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x20;
            break;
        case 0x05:
            ApplyMatrixSV(&rot, &obj->normTop[1], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x1C;
            break;
        case 0x08:
        case 0x10:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x14;
            break;
        case 0x18:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x18;
            break;
        case 0x00:
            ApplyMatrixSV(&rot, &obj->normTop[*(u16 *)(prim + 0x8)], &n0);
            center = n0;
            EVO_SHARD_VERTEX_CURSOR->vx = center.vx;
            EVO_SHARD_VERTEX_CURSOR->vy = center.vy;
            EVO_SHARD_VERTEX_CURSOR->vz = center.vz;
            EVO_SHARD_VERTEX_CURSOR++;
            prim += 0x10;
            break;
        }
    }
    PopMatrix();
    EVO_SHATTER.count = slot + 1;
    return slot;
}

s32 EVO_randomRange(s32 min, s32 max) {
    s32 tmp;

    if (max == min) {
        return min;
    }
    if (max < min) {
        tmp = min;
        min = max;
        max = tmp;
    }
    EVO_RAND_SEED_LO = EVO_RAND_SEED_LO * 0x41C650AD + 0x3039;
    EVO_RAND_SEED_HI = EVO_RAND_SEED_HI * 0x41C650AD + 0x3039;
    return min + ((EVO_RAND_SEED_LO >> 16) | (EVO_RAND_SEED_HI << 16)) % (max - min + 1);
}

void EVO_clearSparks(void) {
    EvoSpark *spark;
    s32 i;

    for (i = 0; i < 16; i++) {
        spark = &EVO_SPARKS[i];
        spark->joint = -1;
    }
}

s32 EVO_addSpark(s16 joint, s16 timer) {
    EvoSpark *spark;
    EvoSpark *slot;
    s32 i;

    for (i = 0; i < 16; i++) {
        slot = &EVO_SPARKS[i];
        if (slot->joint == -1) {
            break;
        }
    }
    if (i == 16) {
        return -1;
    }
    spark = &EVO_SPARKS[i];
    spark->joint = joint;
    spark->timer = timer;
    func_800149B8(0, -1, 0, 0x1000, EVO_runSparkTask, spark, getCurrentTaskId(), 0, 0);
    return i;
}

void EVO_runSparkTask(EvoSpark *spark) {
    do {
        func_80014C08(1);
        EVO_drawSpark(spark);
    } while (--spark->timer >= 0);
    func_80014C08(10);
    spark->joint = -1;
    func_80014A90();
}

void EVO_drawSpark(EvoSpark *spark) {
    MATRIX mat;
    SVECTOR from;
    SVECTOR to;
    SVECTOR tip;
    s32 depthCue;
    s32 flag;
    s32 dist;
    s32 len;
    PolyF3 *poly;
    s32 shade;

    PushMatrix();
    mat = ((EvoModel *)SCENE_3D->models[0])->matrices[spark->joint];
    from.vx = mat.t[0];
    from.vy = mat.t[1];
    from.vz = mat.t[2];
    mat = ((EvoModel *)SCENE_3D->models[0])->matrices[1];
    to.vx = from.vx - mat.t[0];
    to.vy = from.vy - mat.t[1];
    to.vz = from.vz - mat.t[2];
    from.vx = mat.t[0];
    from.vy = mat.t[1];
    from.vz = mat.t[2];
    dist = to.vx * to.vx + to.vy * to.vy + to.vz * to.vz;
    len = EVO_randomRange(400, 500);
    len *= len;
    if (dist == 0) {
        dist = 1;
    }
    to.vx = from.vx + to.vx * len / dist;
    to.vy = from.vy + to.vy * len / dist;
    to.vz = from.vz + to.vz * len / dist;
    tip.vx = to.vx + EVO_randomRange(-80, 80);
    tip.vy = to.vy + EVO_randomRange(-80, 80);
    tip.vz = to.vz + EVO_randomRange(-80, 80);
    poly = (PolyF3 *)func_80062C44();
    func_80067704(poly);
    SetSemiTrans(poly, 1);
    shade = rand() % 128 + 10;
    poly->r0 = shade;
    poly->g0 = shade;
    poly->b0 = shade;
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    if ((u32)((RotTransPers3((s32)&tip, (s32)&from, (s32)&to, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag) >> 2) - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)poly);
        func_80062C34((long)(poly + 1));
    }
    PopMatrix();
}

void EVO_drawShardF4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyF4 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyF4 *)func_80062C44();
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[8]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 5);
    setcode(poly, 0x28);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardG3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyG3 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyG3 *)func_80062C44();
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 6);
    setcode(poly, 0x30);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardG4(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyG4 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyG4 *)func_80062C44();
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[9]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[11]];
    v3.vx = vert->vx + offset.vx;
    v3.vy = vert->vy + offset.vy;
    v3.vz = vert->vz + offset.vz;
    setlen(poly, 8);
    setcode(poly, 0x38);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers4((s32)&v0, (s32)&v1, (s32)&v2, (s32)&v3, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, (s32)&poly->x3, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

void EVO_drawShardF3(SVECTOR *pos, s32 unused, s16 div, s16 mul) {
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    SVECTOR v3;
    SVECTOR offset;
    SVECTOR *vert;
    s32 depthCue;
    s32 flag;
    s32 otz;
    u16 *prim;
    PolyF3 *poly;

    prim = (u16 *)EVO_SHARD_PRIM;
    poly = (PolyF3 *)func_80062C44();
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetSemiTrans(poly, 1);
    poly->r0 = EVO_SHARD_COLOR.r;
    poly->g0 = EVO_SHARD_COLOR.g;
    poly->b0 = EVO_SHARD_COLOR.b;
    offset.vx = pos->vx * mul / div;
    offset.vy = pos->vy * mul / div;
    offset.vz = pos->vz * mul / div;
    vert = &EVO_SHARD_VERTS[prim[5]];
    v0.vx = vert->vx + offset.vx;
    v0.vy = vert->vy + offset.vy;
    v0.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[6]];
    v1.vx = vert->vx + offset.vx;
    v1.vy = vert->vy + offset.vy;
    v1.vz = vert->vz + offset.vz;
    vert = &EVO_SHARD_VERTS[prim[7]];
    v2.vx = vert->vx + offset.vx;
    v2.vy = vert->vy + offset.vy;
    v2.vz = vert->vz + offset.vz;
    setlen(poly, 4);
    setcode(poly, 0x20);
    SetRotMatrix((s32)&D_801DBEA0);
    func_8005C444(&D_801DBEA0);
    otz = RotTransPers3((s32)&v0, (s32)&v1, (s32)&v2, (s32)&poly->x0, (s32)&poly->x1, (s32)&poly->x2, &depthCue, &flag);
    otz >>= 2;
    if ((u32)(otz - 0x21) < 0xFDF) {
        setSemiTrans(poly, 1);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
        func_80062C34((long)(poly + 1));
    }
}

u32 EVO_RAND_SEED_HI = 0x13CC25;
s16 EVO_WIRE_SHADE_MIN = 0x37;
s16 EVO_WIRE_SHADE_MAX = 0xFF;
s8 EVO_DISSOLVE_PATTERN[16] = { 6, 3, 11, 8, 10, 13, 1, 4, 14, 7, 12, 15, 5, 2, 9, 0 };
s16 EVO_PART_DRAW_MODES[40] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
};

/* not referenced by any code */
u8 D_801EF8C4[16] = { 0x40, 0x48, 0x4C, 0x50, 0x58, 0x5C, 0x20, 0x28, 0x24, 0x2C, 0x30, 0x38, 0x34, 0x3C, 0xF6, 0x57 };

/* the Digi-Parts: their text and the rank each partner learns them at */
EvoAbilityInfo EVO_DIGI_PARTS[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88 } },
    { "HP+500.", { 99, -1, 75, 91, -1, 95 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51 } },
    { "All Attack Powers +200.", { 75, -1, 90, 93, -1, 84 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30 } },
    { "*b0 Attack Power +250.", { 48, -1, 78, 61, 62, 61 } },
    { "*b0 Attack Power +300.", { 67, -1, -1, 85, 82, 79 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43 } },
    { "*b1 Attack Power +200.", { 51, 44, -1, 56, 67, 66 } },
    { "*b1 Attack Power +250.", { 77, -1, -1, 76, 86, 96 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24 } },
    { "*b2 Attack Power +200.", { 82, -1, 63, 67, -1, 48 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, -1, 34 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, -1, 17 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, -1, 71 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, -1, -1, 40, 62 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, -1, -1, 24, 49 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, -1, -1, 11, 21 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, -1, 42, 77, 73 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, -1, 66, -1, 83, 4 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, -1, -1, 25 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, -1, -1 } },
    { "Jamming Support, *b2 Attack Power -100.", { -1, 74, 27, -1, 54, 10 } },
    { "Eat-up HP, *b2 Attack Power -200.", { -1, 87, 50, -1, 73, -1 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, -1 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, -1 } },
    { "Add + 30 DP.", { -1, 98, 97, 60, 78, 85 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44 } },
    { "Boost Attack Power +300.", { 69, -1, 84, 82, -1, 80 } },
    { "Attack Power is Doubled.", { -1, -1, -1, -1, -1, -1 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39 } },
    { "Boost *b0 Attack Power +500.", { 55, -1, 88, 88, 66, 74 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63 } },
    { "*b0 Attack Power is Tripled.", { 59, -1, 94, 74, -1, 86 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, -1, 7, 22, 18 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, -1, 23, 41, 36 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, -1, 92, 59, 53 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, -1, 67 } },
    { "*b1 Attack Power is Tripled.", { 83, -1, 91, 94, -1, 89 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, -1, 8 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, -1, 20 } },
    { "Boost *b2 Attack Power +300.", { 71, -1, 60, 63, -1, 90 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, -1, 40 } },
    { "*b2 Attack Power is Tripled.", { 88, -1, 85, 95, -1, 65 } },
    { "Attack Power becomes same as HP.", { -1, -1, -1, -1, -1, -1 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, -1 } },
    { "Attack becomes Eat-up HP.", { -1, 93, 80, -1, 61, 55 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, -1, 17, -1, 28, 75 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, -1, 15, -1 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { -1, 66, 24, -1, 13, 45 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, -1, 33, -1, 47, 14 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, -1, -1, 37, 26 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, -1, -1, 37 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, -1, 77, 25, 38 } },
    { "If *a0 Opponent, X3 own Attack Power.", { -1, 91, -1, 65, 74, 82 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, -1, 89, 83, 98, 91 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, -1, 93, -1, 93, 70 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, -1 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, -1, -1, 68 } },
    { "Change own Specialty to *a0.", { 9, 35, -1, 48, 96, -1 } },
    { "Change own Specialty to *a1.", { -1, 99, 30, 87, 58, -1 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94 } },
    { "Change own Specialty to *a3.", { 30, -1, 68, 98, 81, 3 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99 } },
    { "Switch Opponent's Specialty to own.", { -1, -1, -1, -1, -1, -1 } },
    { "Swap Specialty with Opponent's.", { -1, -1, -1, -1, -1, -1 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, -1, 97, 43, -1 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, -1 } },
    { "If *a2 Opponent, lower its AP to 0.", { -1, 11, 52, -1, 64, 22 } },
    { "If *a3 Opponent, lower its AP to 0.", { -1, 25, 37, 13, 2, 72 } },
    { "If *a4 Opponent, lower its AP to 0.", { -1, 75, 16, -1, 34, 52 } },
    { "Reduce both Players' Atk Pwr to 0.", { -1, -1, -1, -1, -1, -1 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, -1, 9, 48, 23 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, -1, 20, 38, 57 } },
    { "If *e5, boost Attack Power +400.", { 57, -1, 81, 70, 95, 77 } },
    { "Opponent uses *b0 Attack.", { 16, 62, -1, 28, 97, 31 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, -1, 47 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, -1, 44, 58 } },
    { "Opponent uses same Attack.", { -1, -1, -1, -1, -1, -1 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, -1 } },
    { "Recover HP +400.", { -1, 68, 92, 73, 55, -1 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, -1 } },
    { "Halve Attack Power, recover HP +600.", { -1, 46, 86, 44, 60, -1 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, -1 } },
    { "If HP < Opponent's HP, add HP +700.", { -1, 83, 69, 53, 56, -1 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, -1, 24, 30, 42 } },
    { "If KO'd in battle, revive w/ HP 600.", { -1, 63, -1, 49, 49, 64 } },
    { "If KO'd in battle, revive w/ HP 1000.", { -1, 94, -1, 64, 65, 98 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27 } },
    { "Drop 2 Cards in Opponent's Hand.", { -1, 78, 25, -1, 92, 56 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { -1, 56, 18, 86, 35, 5 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { -1, 88, 49, -1, 90, 32 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { -1, -1, -1, -1, -1, -1 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { -1, 51, 7, 29, -1, 16 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { -1, 95, 35, 89, -1, 54 } },
    { "Move Offline Top Card to Online Deck.", { 86, -1, 70, 78, 50, -1 } },
    { "Void Opponent's Support Effect.", { -1, 84, 65, -1, 87, 69 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, -1 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, -1 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, -1, 55, 94, 83 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, -1, 76, 66, 79, 87 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92 } },
    { "Boost Battle Experience by 30%.", { 80, -1, 96, 90, 99, 60 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, -1 } },
    { "Rare Card even more likely to appear.", { -1, 90, 99, 96, 70, -1 } },
};

u8 EVO_PARTNER_CARD_IDS[6] = { 0xAF, 0xB6, 0xBE, 0xB7, 0xB8, 0xBB };
s8 EVO_FUSION_RESULT_TYPES[6][6] = {
    { 5, 2, 3, 4, 1, 0 },
    { 2, 5, 4, 0, 2, 1 },
    { 3, 4, 5, 1, 3, 2 },
    { 4, 0, 1, 5, 0, 3 },
    { 1, 2, 3, 0, 5, 4 },
    { 0, 1, 2, 3, 4, 5 },
};
EvoRange EVO_CARD_ID_RANGES[7] = {
    { 0, 33 }, { 34, 68 }, { 69, 102 }, { 103, 138 }, { 139, 171 }, { 191, 272 }, { 294, 300 },
};
EvoAbilityReward EVO_PARTNER_FUSION_REWARDS[6][5] = {
    { { 0x00, 0x09 }, { 0x04, 0x49 }, { 0x0C, 0x78 }, { 0x08, 0x30 }, { 0x02, 0x7E } },
    { { 0x45, 0x7A }, { 0x4C, 0x4D }, { 0x0D, 0x3E }, { 0x03, 0x35 }, { 0x07, 0x7C } },
    { { 0x22, 0x06 }, { 0x8E, 0x76 }, { 0x97, 0x4F }, { 0x8C, 0x3A }, { 0x28, 0x75 } },
    { { 0x46, 0x28 }, { 0x4B, 0x5F }, { 0x52, 0x79 }, { 0x48, 0x32 }, { 0x25, 0x47 } },
    { { 0x47, 0x7F }, { 0x4D, 0x66 }, { 0x49, 0x6D }, { 0x76, 0x37 }, { 0x4E, 0x68 } },
    { { 0x68, 0x7D }, { 0x69, 0x4B }, { 0x75, 0x3F }, { 0x6B, 0x3C }, { 0x4F, 0x23 } },
};
Bytes4 EVO_TEXT_COLORS[2] = { { { 0x80, 0x80, 0x80, 0 } }, { { 0x40, 0x40, 0x40, 0 } } };
Bytes4 EVO_TEXT_COLOR_GREY = { { 0x60, 0x60, 0x60, 0 } };
Bytes4 EVO_TEXT_COLOR_RED = { { 0xC0, 0x60, 0x60, 0 } };

/* not referenced by any code */
Rect16 D_801EFF68[14] = {
    { 0x10, 0x24, 0x40, 0x40 },
    { 0x5C, 0x2A, 0xD2, 0xC },
    { 0x62, 0x3E, 0xCC, 0x24 },
    { 0x10, 0x6E, 0x40, 0x40 },
    { 0x5C, 0x74, 0xD2, 0xC },
    { 0x62, 0x88, 0xCC, 0x24 },
    { 0x10, 0xAC, 0x3E, 0x38 },
    { 0x5A, 0xB4, 0xE0, 0x30 },
    { 0x10, 0x96, 0x3E, 0x38 },
    { 0x5A, 0x9C, 0xDE, 0x30 },
    { 0x76, 0x26, 0xC4, 0x7E },
    { 0xA, 0x32, 0x58, 0x72 },
    { 0x78, 0x51, 0x88, 0x34 },
    { 0x0, 0x0, 0x0, 0x0 },
};

/* not referenced by any code */
s16 D_801EFFD8[2][10] = {
    { -0x78, 0x29, 0xE, 0x29, 0x50, 0x74, -0x1, 0x0, 0xC, 0x0 },
    { -0x78, 0x29, 0xE, 0x29, 0x50, 0x74, -0x1, 0x0, 0xC, 0x1 },
};

const u8 EVO_FUSION_RECIPES[20][4] = {
    { 0x01, 0x04, 0x00, 0xEC },
    { 0x04, 0x23, 0x00, 0xEC },
    { 0x02, 0x25, 0x01, 0xED },
    { 0x06, 0x27, 0x23, 0xFE },
    { 0x8E, 0x28, 0x22, 0xEE },
    { 0x2A, 0x2B, 0x24, 0xEF },
    { 0x4C, 0x07, 0x45, 0xF0 },
    { 0x4D, 0x4B, 0x47, 0xF1 },
    { 0x4B, 0x8F, 0x46, 0xF2 },
    { 0x51, 0x03, 0x48, 0xF3 },
    { 0x70, 0x4A, 0x68, 0xF4 },
    { 0x4E, 0x6F, 0x49, 0xF5 },
    { 0x6E, 0x6F, 0x6A, 0xF6 },
    { 0x6D, 0x90, 0x69, 0xF7 },
    { 0x93, 0x73, 0x8B, 0xF8 },
    { 0x96, 0x74, 0x8D, 0xF9 },
    { 0x91, 0x26, 0x8C, 0xFA },
    { 0x0C, 0x75, 0x04, 0xFB },
    { 0x53, 0x0D, 0x4C, 0xFC },
    { 0x52, 0x97, 0x8E, 0xFD },
};

void EVO_loadUnitTextures(void) {
    char path[24];
    u32 *pack;

    sprintf(path, "C:\\OBJECT\\unit.TIS", (s8)((SessionData *)D_8006E054)->unk100C->unk1A4);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
}

char *EVO_SORT_LABELS[12] = {
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
    "Number of Cards that can be Fused.",
};

void EVO_initFusionScene(void) {
    Graphics *camera;

    initScene3D(1);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    func_80014C08(2);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->unk8E = 0;
    camera->unk90 = 20;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
    func_80014C08(2);
}

void EVO_countSpareCards(void) {
    s32 i;
    s32 j;

    for (i = 0; i < 0x12D; i++) {
        EVO_SPARE_CARD_COUNTS[i] = getOwnedCardCount(0, i);
    }
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 0x12D; j++) {
            EVO_DECK_CARD_COUNTS[i][j] = 0;
        }
    }
    for (i = 0; i < 3; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].inUse != 0) {
            for (j = 0; j < 30; j++) {
                EVO_DECK_CARD_COUNTS[i][getCardId(((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].type,
                                        ((PlayerProfile *)PLAYER_PROFILES)->savedDecks[i].cards[j].index)]++;
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        for (j = 1; j < 3; j++) {
            if (EVO_DECK_CARD_COUNTS[0][i] < EVO_DECK_CARD_COUNTS[j][i]) {
                EVO_DECK_CARD_COUNTS[0][i] = EVO_DECK_CARD_COUNTS[j][i];
            }
        }
    }
    for (i = 0; i < 0x12D; i++) {
        EVO_SPARE_CARD_COUNTS[i] -= EVO_DECK_CARD_COUNTS[0][i];
    }
}

void EVO_initCardList(void) {
    s32 i;
    s32 j;

    for (i = 0, j = 0; j < 0xBF; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(DIGIMON_CARDS + j * 0x13C);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 0x66; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(OPTION_CARDS + j * 0xE2);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 8; j++, i++) {
        EVO_CARD_LIST[i] = (EvoCardInfo *)(DIGIVOLVE_CARDS + j * 0x70);
        EVO_CARDS_BY_ID[i] = EVO_CARD_LIST[i];
    }
    for (j = 0; j < 3; j++) {
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId != 0) {
            EVO_CARD_LIST[((PlayerProfile *)PLAYER_PROFILES)->partners[j].cardId] =
                (EvoCardInfo *)&((PlayerProfile *)PLAYER_PROFILES)->partners[j].card[0];
        }
    }
}

s32 EVO_compareFireCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareIceCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 1) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 1) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareNatureCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareDarknessCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareRareCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr >> 4;
    }
    if (ka == 4) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 4) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareOptionCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 1;
    s32 kb = (*b)->type == 1;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareDigivolveCards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = (*a)->type == 2;
    s32 kb = (*b)->type == 2;

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel0Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 0) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 0) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel2Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 2) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 2) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareLevel3Cards(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka;
    s32 kb;

    if ((*a)->type != 0) {
        ka = -1;
    } else {
        ka = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        kb = -1;
    } else {
        kb = (*b)->attr & 0xF;
    }
    if (ka == 3) {
        ka = 1;
    } else {
        ka = 0;
    }
    if (kb == 3) {
        kb = 1;
    } else {
        kb = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

s32 EVO_compareSpareCounts(EvoCardInfo **a, EvoCardInfo **b) {
    s32 ka = EVO_SPARE_CARD_COUNTS[(*a)->id];
    s32 kb = EVO_SPARE_CARD_COUNTS[(*b)->id];

    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*a)->id] & 0x40)) {
        ka = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)->cardCollection[(*b)->id] & 0x40)) {
        kb = -1;
    }
    return kb - ka;
}

Menu EVO_CARD_LIST_MENU = { 0, 0, { 0x6A, 0x2C, 0xD0, 0x6C }, 0, -1, 0, -1, 0xA, 0x61, 0x74, 0xC, 0, 301, 0x36, 1, 0, 0xC };
Menu EVO_SORT_MENU = { 0, 0, { 0x28, 0x3C, 0xCA, 0x70 }, 0, -1, 0, -1, 0xA, 0x56, 0xC0, 0xC, 0, 12, 0, 1, 0, 0xE };

typedef s32 (*EvoCardCompare)(s8 *, s8 *);

s32 (*EVO_SORT_COMPARES[12])(s8 *, s8 *) = {
    0,
    (EvoCardCompare)EVO_compareFireCards,
    (EvoCardCompare)EVO_compareIceCards,
    (EvoCardCompare)EVO_compareNatureCards,
    (EvoCardCompare)EVO_compareDarknessCards,
    (EvoCardCompare)EVO_compareRareCards,
    (EvoCardCompare)EVO_compareOptionCards,
    (EvoCardCompare)EVO_compareDigivolveCards,
    (EvoCardCompare)EVO_compareLevel0Cards,
    (EvoCardCompare)EVO_compareLevel2Cards,
    (EvoCardCompare)EVO_compareLevel3Cards,
    (EvoCardCompare)EVO_compareSpareCounts,
};

Rect16 EVO_RANK_UP_RECT = { 0xD, 0x4A, 0x48, 0x9 };

void EVO_drawSortMenu(UiWindow *w) {
    s32 x = w->originX;
    s32 z = w->z;
    s32 i;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    for (i = 0; i < EVO_SORT_MENU.nrows; i++) {
        if (i < w->view.y / EVO_SORT_MENU.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / EVO_SORT_MENU.rowH < i) {
            break;
        }
        drawText(x, w->originY + i * EVO_SORT_MENU.rowH + 1, (s32)EVO_SORT_LABELS[i], 7, z);
    }
    updateMenuCursor(&EVO_SORT_MENU);
    if (EVO_SORT_MENU.active != 0 && (PAD_STATES[0]->pressed & 0x40)) {
        playMenuSound(1);
        EVO_CARD_LIST_MENU.row = 0;
        centerMenuOnCursor(&EVO_CARD_LIST_MENU);
        if (EVO_SORT_COMPARES[EVO_SORT_MENU.row] != NULL) {
            sortArray((s8 *)EVO_CARD_LIST, 0x12D, 4, EVO_SORT_COMPARES[EVO_SORT_MENU.row]);
        } else {
            EVO_initCardList();
        }
    }
}

void EVO_drawCardList(UiWindow *w) {
    char text[72];
    u8 *color;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 type;
    s32 palette;

    x = w->originX;
    z = w->z;
    x -= 10;
    if (EVO_TYPE_CHOICE.unkE == 0) {
        w->brightness = 0x80;
    } else {
        w->brightness = 0x40;
    }
    for (i = 0; i < EVO_CARD_LIST_MENU.nrows; i++) {
        if (i < w->view.y / EVO_CARD_LIST_MENU.rowH) {
            continue;
        }
        if ((w->view.y + w->rect.h) / EVO_CARD_LIST_MENU.rowH < i) {
            break;
        }
        y = w->originY + i * EVO_CARD_LIST_MENU.rowH + 1;
        type = EVO_CARD_LIST[i]->type;
        color = EVO_TEXT_COLORS[0].b;
        palette = 7;
        if (EVO_CARD_LIST[i]->fusionPoints == 0) {
            palette = 3;
        }
        if (type == 0 && (EVO_CARD_LIST[i]->attr & 0xF) > EVO_MAX_CARD_LEVEL) {
            palette = 3;
        }
        if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[EVO_CARD_LIST[i]->id] & 0x40) {
            if (EVO_SPARE_CARD_COUNTS[EVO_CARD_LIST[i]->id] == 0) {
                color = EVO_TEXT_COLOR_GREY.b;
            }
            drawTextColored(x + 0x3C, y, EVO_CARD_LIST[i]->name, color, palette, z);
            switch (type) {
            case 0:
                drawIconColored(x + 0x20, y, 0, EVO_CARD_LIST[i]->attr >> 4, color, z);
                if (palette == 3) {
                    drawIconColored(x + 0x2C, y, 0, (EVO_CARD_LIST[i]->attr & 0xF) + 0x10, EVO_TEXT_COLOR_RED.b, z);
                } else {
                    drawIconColored(x + 0x2C, y, 0, (EVO_CARD_LIST[i]->attr & 0xF) + 0x10, color, z);
                }
                break;
            case 1:
                drawIconColored(x + 0x20, y, 0, 5, color, z);
                break;
            case 2:
                drawIconColored(x + 0x20, y, 0, 6, color, z);
                break;
            }
        } else {
            palette = 9;
            drawTextColored(x + 0x20, y, "??", color, palette, z);
            drawTextColored(x + 0x3C, y, "------------------", color, palette, z);
        }
        sprintf(text, EVO_FMT_CARD_NUMBER, EVO_CARD_LIST[i]->id);
        drawTextColored(x + 0xA, y, text, color, palette, z);
        sprintf(text, EVO_FMT_CARD_COUNT, EVO_SPARE_CARD_COUNTS[EVO_CARD_LIST[i]->id]);
        drawTextColored(x + 0xB5, y, text, color, palette, z);
        drawTinyTextColored(x + 0xBD, y + 6, (u8 *)EVO_STR_CARDS, palette, color, z);
    }
    updateMenuCursor(&EVO_CARD_LIST_MENU);
}

const char EVO_FMT_CARD_NUMBER[] = "*s0%3.3d";

const char EVO_FMT_CARD_COUNT[] = "%d";

const char EVO_STR_CARDS[] = "Cards";

void EVO_drawFusionTypeTitle(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    if (w->isPartner == 0) {
        drawTextColored(x + 0x4B, y, "Card Fusion", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 6, z);
    } else {
        drawTextColored(x + 0x41, y, "Partner Fusion", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 6, z);
    }
}

void EVO_drawFusionTypeHelp(EvoWindow *w) {
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    if (w->isPartner == 0) {
        x += 2;
        drawTextColored(x, y, "*w1Create a New Card", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1by Fusing 2 Cards.", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Partner Cards can't be used.", EVO_TEXT_COLORS[EVO_TYPE_CHOICE.side].b, 8, z);
    } else {
        x += 2;
        drawTextColored(x, y, "*w1Increase Experience Points by", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0xC, "*w1Fusing a Card to a Partner Card.", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
        drawTextColored(x, y + 0x18, "*w1Also,2 Partner Cards can't be Fused.", EVO_TEXT_COLORS[(s8)(EVO_TYPE_CHOICE.side ^ 1)].b, 8, z);
    }
}

void EVO_drawTray(EvoTray *tray) {
    char text[40];
    Rect16 uv;
    POLY_FT4 *poly;
    s32 player;

    poly = tray->polys[FRAME_BUFFER_INDEX];
    player = 2;
    if (tray == &EVO_TRAYS[0]) {
        player = 1;
    }
    /* the index is added before the field offset in the original */
    if (((EvoFusion *)((u8 *)&EVO_FUSION + player))->busy[0] == 0) {
        bzero((Scene3D *)text, 0x21);
        sprintf(text, "TRAY%d", player);
        drawLargeText(tray->x + 16, 0x5C, (s32)text, 7, 0x1D);
        uv.x = 0;
        uv.y = 0x74;
        uv.w = 0x50;
        uv.h = 0x74;
        drawTexturedSprite(tray->x, tray->y, &uv, 0x18, 0x7BDF, 0x1E, 0x80, 0);
        return;
    }
    setlen(poly, 9);
    setcode(poly, 0x2C);
    poly->clut = 0x7BD8;
    poly->tpage = 0x18;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = tray->x;
    poly->y0 = tray->y;
    poly->x1 = tray->x + 0x50;
    poly->y1 = tray->y;
    poly->x2 = tray->x;
    poly->y2 = tray->y + 0x74;
    poly->x3 = tray->x + 0x50;
    poly->y3 = tray->y + 0x74;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 0x50;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 0x74;
    poly->u3 = 0x50;
    poly->v3 = 0x74;
    addPrim(&CURRENT_FRAME_BUFFER->ot[29], poly);
    poly++;
    setlen(poly, 9);
    setcode(poly, 0x2C);
    if (player == 1 && EVO_FUSION.hideResult == 0) {
        poly->clut = 0x7A98;
        poly->u0 = 0;
        poly->v0 = 0x40;
        poly->u1 = 0x3F;
        poly->v1 = 0x40;
        poly->u2 = 0;
        poly->v2 = 0x7F;
        poly->u3 = 0x3F;
        poly->v3 = 0x7F;
    } else {
        poly->clut = getClut(0x180, player + 0x1E7);
        poly->u0 = (player - 1) * 64;
        poly->v0 = 0;
        poly->u1 = (player - 1) * 64 + 0x3F;
        poly->v1 = 0;
        poly->u2 = (player - 1) * 64;
        poly->v2 = 0x3F;
        poly->u3 = (player - 1) * 64 + 0x3F;
        poly->v3 = 0x3F;
    }
    poly->tpage = 0x99;
    poly->r0 = 0x80;
    poly->g0 = 0x80;
    poly->b0 = 0x80;
    poly->x0 = tray->x + 8;
    poly->y0 = tray->y + 0x14;
    poly->x1 = tray->x + 0x48;
    poly->y1 = tray->y + 0x14;
    poly->x2 = tray->x + 8;
    poly->y2 = tray->y + 0x54;
    poly->x3 = tray->x + 0x48;
    poly->y3 = tray->y + 0x54;
    addPrim(&CURRENT_FRAME_BUFFER->ot[28], poly);
}

EvoWindowDef EVO_WINDOW_DEFS[14] = {
    { { 0x10, 0x24, 0x40, 0x40 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5C, 0x2A, 0xDE, 0xC }, 0x80, 0x56, 8, 0, 8 },
    { { 0x62, 0x3E, 0xD8, 0x24 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x10, 0x4C, 0x40, 0x40 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5C, 0x52, 0xDE, 0xC }, 0x80, 0x56, 8, 0, 8 },
    { { 0x62, 0x66, 0xD8, 0x24 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x10, 0xAC, 0x3E, 0x38 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5A, 0xB4, 0xE0, 0x30 }, 0x80, 0x56, 8, (s32)"MESSAGE", 8 },
    { { 0x10, 0x96, 0x3E, 0x38 }, 0x80, 0x66, 8, 0, 8 },
    { { 0x5A, 0x9C, 0xDE, 0x30 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x76, 0x26, 0xC4, 0x7E }, 0x80, 0x66, 8, 0, 8 },
    { { 0xA, 0x2C, 0x52, 0x78 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x78, 0x51, 0x88, 0x34 }, 0x80, 0x56, 8, 0, 8 },
    { { 0x42, 0x78, 0x48, 0x9 }, 0x80, 0x36, 0, 0, 8 },
};
const char D_801DF260[] = "";

void EVO_slideInFirstTray(void) {
    EVO_TRAYS[0].x += 10;
    if (EVO_TRAYS[0].x >= 15) {
        EVO_TRAYS[0].x = 14;
        EVO_FUSION.step = 3;
        EVO_CARD_LIST_MENU.active = 1;
        EVO_FUSION.scriptState = 0;
    }
}

void EVO_slideOutFirstTray(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = -1;
    }
}

void EVO_swapToSecondTray(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x78) {
        EVO_TRAYS[0].x = -0x78;
        EVO_TRAYS[1].x += 10;
        if (EVO_TRAYS[1].x >= 15) {
            EVO_TRAYS[1].x = 14;
            EVO_FUSION.step = 3;
            EVO_CARD_LIST_MENU.active = 1;
        }
    }
}

void EVO_swapToFirstTray(void) {
    EVO_TRAYS[1].x -= 10;
    if (EVO_TRAYS[1].x < -0x78) {
        EVO_TRAYS[1].x = -0x78;
        EVO_TRAYS[0].x += 10;
        if (EVO_TRAYS[0].x >= 15) {
            EVO_TRAYS[0].x = 14;
            EVO_FUSION.step = 3;
            EVO_CARD_LIST_MENU.active = 1;
        }
    }
}

void EVO_cancelSecondCard(s32 active) {
    if (active != 0) {
        EVO_TRAYS[0].x -= 10;
        EVO_TRAYS[1].x -= 10;
        if (EVO_TRAYS[0].x < -0x58) {
            EVO_TRAYS[0].x = -0x58;
        }
        if (EVO_TRAYS[1].x < 14) {
            EVO_TRAYS[1].x = 14;
        }
        if (EVO_TRAYS[0].x == -0x58 && EVO_TRAYS[1].x == 14) {
            EVO_FUSION.pickSlot = 2;
            EVO_SCRIPT->vars[8] = -1;
            EVO_FUSION.scriptState = 0;
            EVO_FUSION.step = 0;
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]++;
        }
    }
}

void EVO_toggleMessageWindows(s8 mode) {
    if (mode == 0) {
        animateWindowTo(&EVO_WINDOWS[6].win, &EVO_WINDOW_DEFS[6].rect);
        animateWindowTo(&EVO_WINDOWS[7].win, &EVO_WINDOW_DEFS[7].rect);
    } else if (mode == 1) {
        animateWindowTo(&EVO_WINDOWS[6].win, (Rect16 *)-1);
        animateWindowTo(&EVO_WINDOWS[7].win, (Rect16 *)-1);
    }
}

void EVO_runChoiceDialog(s32 mode) {
    initDialog((u8 *)&EVO_DIALOG, NULL, 1);
    if (mode != 1) {
        EVO_DIALOG.choice = 1;
    }
    runDialog(&EVO_DIALOG);
    switch (EVO_DIALOG.choice) {
    case 0:
        EVO_SCRIPT->vars[1] = 0;
        break;
    case 1:
        EVO_SCRIPT->vars[1] = 1;
        break;
    case 2:
        EVO_SCRIPT->vars[1] = 2;
        break;
    }
}

void EVO_drawScreenFlash(void) {
    if (EVO_SCREEN_FLASH.on == 0) {
        EVO_SCREEN_FLASH.brightness -= 8;
        if (EVO_SCREEN_FLASH.brightness < 0) {
            EVO_SCREEN_FLASH.brightness = 0;
        }
    } else {
        EVO_SCREEN_FLASH.brightness += 8;
        if (EVO_SCREEN_FLASH.brightness >= 0x100) {
            EVO_SCREEN_FLASH.brightness = 0xFF;
        }
    }
    if (EVO_SCREEN_FLASH.brightness != 0) {
        EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].r0 = EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].g0 =
            EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX].b0 = EVO_SCREEN_FLASH.brightness;
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &EVO_SCREEN_FLASH.poly[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[25], &EVO_SCREEN_FLASH.tpage[FRAME_BUFFER_INDEX]);
    }
}

void EVO_initScreenFlash(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_80067784(&EVO_SCREEN_FLASH.poly[i]);
        SetSemiTrans(&EVO_SCREEN_FLASH.poly[i], 1);
        setPrimQuadRect(&EVO_SCREEN_FLASH.poly[i], 0, 0, 320, 240);
        SetDrawTPage(&EVO_SCREEN_FLASH.tpage[i], 0, 0, 0x20);
        EVO_SCREEN_FLASH.poly[i].b0 = 0;
        EVO_SCREEN_FLASH.poly[i].g0 = 0;
        EVO_SCREEN_FLASH.poly[i].r0 = 0;
    }
    EVO_SCREEN_FLASH.on = 0;
    EVO_SCREEN_FLASH.brightness = 0;
    addFrameCallback((s32)EVO_drawScreenFlash);
}

void EVO_runFusionScript(EvoProgram *data) {
    s32 result;

    if (EVO_SCRIPT_HALTED == 1) {
        return;
    }
    do {
        result = runScriptToNextEvent(data->script, data->vars);
        if (result == 1) {
            switch (data->script->eventOp) {
            case 10:
                switch (data->script->eventArg) {
                case 0:
                    if (EVO_addTextLine((u8 *)data->vars[4]) == -1) {
                        EVO_FUSION.scriptState = 1;
                        return;
                    }
                    break;
                case 1:
                    return;
                case 2:
                    EVO_toggleMessageWindows(0);
                    break;
                case 4:
                    EVO_openFusionTypeChoice();
                    return;
                case 5:
                    EVO_closeFusionTypeChoice();
                    return;
                case 11:
                    EVO_FUSION.scriptState = 2;
                    return;
                case 12:
                    EVO_clearTextLines(EVO_TEXT_LINES);
                    break;
                case 8:
                    EVO_openPartnerList();
                    break;
                case 9:
                    EVO_closePartnerList();
                    break;
                case 10:
                    EVO_openCardList();
                    break;
                case 13:
                    EVO_closeCardList();
                    break;
                case 6:
                    EVO_resetFusion();
                    break;
                case 7:
                    EVO_cancelFirstCard();
                    break;
                case 14:
                    EVO_startSecondCardPick();
                    break;
                case 15:
                    EVO_FUSION.pickSlot = 1;
                    break;
                case 16:
                    EVO_leaveForCutscene();
                    break;
                case 17:
                    EVO_startPartnerFusion();
                    return;
                case 18:
                    EVO_cancelPartnerFusion();
                    return;
                case 19:
                    EVO_FUSION.resumeOffset = EVO_SCRIPT->script->pc - EVO_SCRIPT->script->start;
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.resultStep = 0;
                    EVO_FUSION.step = 10;
                    return;
                case 20:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.resultStep = 0;
                    EVO_FUSION.step = 12;
                    return;
                case 21:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 13;
                    return;
                case 22:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 5;
                    animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
                    EVO_CARD_LIST_MENU.active = 1;
                    return;
                default:
                    EVO_SCRIPT_HALTED = 0;
                    break;
                }
                break;
            case 11:
                switch (data->script->eventArg) {
                case 0:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 15;
                    return;
                case 1:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 14;
                    return;
                case 2:
                    EVO_runChoiceDialog((s16)data->script->params[0]);
                    break;
                case 3:
                    animateWindowTo(&EVO_WINDOWS[(s16)data->script->params[0]].win, (Rect16 *)-1);
                    if ((s16)data->script->params[0] == -1) {
                        animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
                        EVO_CARD_LIST_MENU.active = 1;
                    }
                    break;
                case 4:
                    EVO_FUSION.scriptState = 4;
                    EVO_FUSION.step = 16;
                    return;
                case 5:
                    playSoundEffect((s16)data->script->params[0]);
                    break;
                case 6:
                    func_80014C08((s16)data->script->params[0]);
                    break;
                case 7:
                    EVO_FUSION.rewardStep = data->script->params[0];
                    return;
                case 8:
                    if ((s16)data->script->params[0] == 0) {
                        EVO_FUSION.busy[1] = 0;
                    } else {
                        EVO_FUSION.busy[2] = 0;
                    }
                    break;
                }
                break;
            case 12:
            case 13:
                break;
            }
        }
        clearScriptBusy(data->script);
    } while (result != 0);
}

s32 EVO_tickFusionScript(EvoProgram *program) {
    *program->vars = 1;
    EVO_runFusionScript(program);
    return *program->vars;
}

Script *EVO_createScriptContext(EvoMsd *data) {
    Script *script;

    script = allocHeapBlock(sizeof(Script), 0x2C);
    script->base = (u8 *)data;
    script->start = data->code;
    script->pc = data->code;
    script->offset = 0;
    script->size = data->size;
    clearScriptBusy(script);
    return script;
}

s32 *EVO_allocScriptRegisters(s32 count) {
    s32 *flags;
    s32 *p;
    s32 i;

    flags = allocHeapBlock(count * 4, 0x2C);
    p = flags;
    for (i = 0; i < count; i++) {
        *p++ = 0;
    }
    return flags;
}

EvoProgram *EVO_loadUnitScript(s32 index) {
    char path[24];
    EvoMsd *data;
    EvoProgram *program;

    sprintf(path, "C:\\EVENT\\unit0%d.MSD", index);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    data = (EvoMsd *)func_80014C08(0x7FFFFFFF);
    program = allocHeapBlock(sizeof(EvoProgram), 0x2C);
    program->data = data;
    program->script = EVO_createScriptContext(data);
    return program;
}

void EVO_openWindows(void) {
    EvoWindowDef *def;
    s32 i;

    for (def = EVO_WINDOW_DEFS, i = 0; i < 14; i++, def++) {
        EVO_WINDOWS[i].z = 30;
        openWindow(&EVO_WINDOWS[i].win, def, -1, (s16 *)-1, def->flags, def->style, def->brightness, 12);
        if (def->label != 0) {
            EVO_WINDOWS[i].win.label = def->label;
        }
        EVO_WINDOWS[i].win.labelPalette = def->labelPalette;
        animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
    }
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].isPartner = 0;
        EVO_WINDOWS[i + 3].isPartner = 1;
    }
    openMenu(&EVO_CARD_LIST_MENU, &EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_WINDOW.labelPalette = 8;
    EVO_CARD_LIST_WINDOW.label = (s32)"CARD LIST";
    EVO_CARD_LIST_MENU.active = 0;
    openMenu(&EVO_SORT_MENU, &EVO_SORT_WINDOW, &EVO_SORT_CURSOR, (Bytes4 *)-1);
    animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    EVO_SORT_WINDOW.label = (s32)"SORT MENU";
    EVO_SORT_WINDOW.labelPalette = 8;
    EVO_WINDOWS[12].z = 0x1B;
    EVO_WINDOWS[13].z = 5;
    EVO_WINDOWS[13].win.palette = 2;
    EVO_RANK_UP_STATE = 0;
    openWindow(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
    EVO_RANK_UP_WINDOW.palette = 2;
}

void EVO_drawEmptyWindow(void) {
}

void EVO_renderFusion(void) {
    s32 i;
    s32 t;

    if (EVO_FUSION.resultStep == 1 && EVO_TRAYS[1].merge == 1 && EVO_TRAYS[0].merge < 30) {
        EVO_TRAYS[0].merge++;
        if (EVO_TRAYS[0].merge >= 30) {
            EVO_TRAYS[0].merge = 30;
            EVO_FUSION.hideResult = 0;
        }
        t = EVO_TRAYS[0].merge;
        EVO_TRAYS[0].x = (t * 58 + (30 - t) * 14) / 30;
        EVO_TRAYS[1].x = (t * 58 + (30 - t) * 102) / 30;
    }
    for (i = 0; i < 14; i++) {
        drawWindow(&EVO_WINDOWS[i].win, EVO_WINDOW_DRAW_FUNCS[i], EVO_WINDOWS[i].z);
    }
    drawWindow(&EVO_RANK_UP_WINDOW, EVO_drawRankUpBanner, 5);
    drawWindow(&EVO_CARD_LIST_WINDOW, EVO_drawCardList, 0x1D);
    drawWindow(&EVO_SORT_WINDOW, EVO_drawSortMenu, 0x1C);
    for (i = 0; i < 2; i++) {
        EVO_drawTray(&EVO_TRAYS[i]);
    }
}

void EVO_advanceText(void) {
    if (EVO_FUSION.textTyping == 0 && (PAD_STATES[0]->pressed & 0x40)) {
        EVO_clearTextLines(EVO_TEXT_LINES);
        if (EVO_FUSION.scriptState == 1) {
            EVO_addTextLine((u8 *)EVO_SCRIPT->vars[4]);
        }
        EVO_FUSION.scriptState = 0;
        playSoundEffect(0);
    }
}

void EVO_loadCardImages(void) {
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    EVO_FUSION.cardArchive = (s32 *)func_80014C08(0x7FFFFFFF);
}

void EVO_loadCardImage(s32 id, s32 slot) {
    char path[64];
    u32 *tim;

    EVO_FUSION.busy[0] = 1;
    sprintf(path, "B:\\CARD\\LC%3.3d.TIM", id);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(tim, (slot & 1) * 32 + 0x240, (slot >> 1) * 64 + 0x100, 0x180, slot + 0x1E8);
    DrawSync(0);
    freeHeapBlock(tim);
    EVO_FUSION.busy[0] = 0;
}

void EVO_loadScriptFlags(void) {
    s32 i;
    s32 bit;
    PlayerProfile *profile;

    for (i = 20, bit = 0, profile = (PlayerProfile *)PLAYER_PROFILES; i < 30; i++, bit++) {
        if ((1 << bit) & profile->scriptFlags) {
            EVO_SCRIPT->vars[i] = 1;
        }
    }
}

void EVO_saveScriptFlags(void) {
    s32 i;
    s32 bit;

    i = 20;
    bit = 0;
    while (bit < 32) {
        if (EVO_SCRIPT->vars[i] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->scriptFlags |= 1 << bit;
        }
        i++;
        bit++;
        if (i >= 30) {
            break;
        }
    }
}

void EVO_runFusion(s32 unit) {
    s32 running = 1;
    s32 i;

    EVO_loadUnitTextures();
    EVO_loadCardImages();
    EVO_initFusionScene();
    EVO_loadEffectArchive();
    for (i = 0; i < 3; i++) {
        EVO_DECK_CARD_COUNTS[i] = allocTaskHeapBlock(0x12D);
    }
    EVO_SPARE_CARD_COUNTS = allocTaskHeapBlock(0x12D);
    EVO_countSpareCards();
    EVO_openWindows();
    EVO_clearTextLines(EVO_TEXT_LINES);
    if (unit >= 0) {
        EVO_FUSION.unit = unit;
        EVO_initScreenFlash();
    }
    EVO_SCRIPT = EVO_loadUnitScript(EVO_FUSION.unit);
    EVO_SCRIPT->vars = EVO_allocScriptRegisters(30);
    if (unit < 0) {
        playMusic(0, EVO_FUSION.unit + 0x85, 100);
        EVO_FUSION.step = 17;
        EVO_FUSION.scriptState = 0;
        EVO_FUSION.hideResult = 0;
        EVO_SCRIPT->script->pc = EVO_SCRIPT->script->start + EVO_FUSION.resumeOffset;
        EVO_toggleMessageWindows(0);
    } else {
        EVO_initCardList();
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_FUSION.hideResult = 1;
    }
    EVO_FUSION.swapTimer = 0;
    EVO_FUSION.typeChoiceOpen = 0;
    EVO_FUSION.unkBD = 0;
    EVO_FUSION.partnerListOpen = 0;
    EVO_FUSION.fusionType = 0;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.resultStep = 0;
    for (i = 0; i < 2; i++) {
        EVO_TRAYS[i].x = -0x78;
        EVO_TRAYS[i].y = 0x29;
    }
    addFrameCallback((s32)EVO_renderFusion);
    EVO_loadScriptFlags();
    do {
        func_80014C08(1);
        switch (EVO_FUSION.scriptState) {
        case 0:
            running = EVO_tickFusionScript(EVO_SCRIPT);
            break;
        case 1:
        case 2:
            EVO_advanceText();
            break;
        }
        switch (EVO_FUSION.step) {
        case 0:
            break;
        case 1:
            EVO_tickFusionTypeChoice();
            break;
        case 2:
            EVO_tickPartnerList();
            break;
        case 3:
            EVO_tickCardList();
            break;
        case 4:
            EVO_tickPartnerReward();
            break;
        case 5:
            EVO_slideInFirstTray();
            break;
        case 6:
            EVO_swapToFirstTray();
            break;
        case 7:
            EVO_swapToSecondTray();
            break;
        case 8:
            EVO_slideOutFirstTray();
            break;
        case 10:
            EVO_tickFusionResult();
            break;
        case 11:
            EVO_showBothTrays();
            break;
        case 12:
            EVO_repickSecondCard();
            break;
        case 13:
            func_801EBE08();
            break;
        case 14:
            EVO_slideTrayOut((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 15:
            EVO_slideTrayIn((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 16:
            EVO_cancelSecondCard((s16)EVO_SCRIPT->script->params[0]);
            break;
        case 17:
            EVO_showCutsceneResult();
            break;
        case 18:
            EVO_closeFusionResult();
            break;
        }
    } while (running != 0 && EVO_FUSION.cutscene == 0);
    if (running == 0) {
        animateWindowTo(&EVO_WINDOWS[6].win, (Rect16 *)-1);
        animateWindowTo(&EVO_WINDOWS[7].win, (Rect16 *)-1);
        func_80014C08(20);
    }
    EVO_saveScriptFlags();
    removeFrameCallback((s32)EVO_renderFusion);
    removeFrameCallback((s32)renderSceneModels);
    func_80014C08(1);
    func_80014A00(0x1B);
    freeHeapBlock(EVO_FUSION.cardArchive);
    freeHeapBlock(EVO_SCRIPT->data);
    freeHeapBlocksByTag(0x2C);
    freeHeapBlock(EVO_EFFECT_ARCHIVE);
    freeHeapBlocksByTag(0x7F);
    if (EVO_FUSION.cutscene != 0) {
        func_80014C08(60);
        hideScrollingBackground();
        func_800149B8(0, -1, 0, 0x400, EVO_runFusionCutscene, 0, getCurrentTaskId(), 0, 0);
    } else {
        removeFrameCallback((s32)EVO_drawScreenFlash);
        func_800149B8(0, -1, 0, 0x400, returnToWorldMap, 0, 0, 0, 0);
    }
}

void EVO_clearTextLines(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        slot->active = 0;
        slot++;
    }
}

EvoText *EVO_allocTextLine(EvoText *slot) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (slot->active == 0) {
            bzero((Scene3D *)slot, 0x3C);
            slot->active = 1;
            if (i == 0) {
                slot->pos = -1;
            } else {
                slot->pos = 0;
            }
            return slot;
        }
        slot++;
    }
    return NULL;
}

s32 EVO_addTextLine(u8 *src) {
    u8 *playerName;
    u8 *cardName;
    EvoText *t;
    u8 *dst;
    s32 i;
    s32 end;

    playerName = (u8 *)PLAYER_PROFILES;
    cardName = EVO_CARDS_BY_ID[EVO_FUSION.result]->name;
    t = EVO_allocTextLine(EVO_TEXT_LINES);
    if (t == NULL) {
        /* Shift-JIS: "the text has run out of lines" */
        printf("\x83" "e\x83L\x83X\x83g\x82\xCC\x8Ds\x90\x94\x82\xAA\x82\xA2\x82\xC1\x82\xCF\x82\xA2\x82\xC9\x82\xC8\x82\xE8\x82\xDC\x82\xB5\x82\xBD\n");
        return -1;
    }
    t->text[0] = '*';
    t->text[1] = 'w';
    t->text[2] = '1';
    dst = t->text + 3;
    while (*src != 0) {
        if (*src < 0x81 || *src > 0x98) {
            if (*src == '*') {
                if (src[1] == 'h') {
                    if (src[2] == '0') {
                        src += 3;
                        for (i = 0; i < 12 && *playerName != 0; i++) {
                            *dst++ = *playerName++;
                        }
                        continue;
                    } else if (src[2] == '1') {
                        src += 3;
                        for (i = 0; i < 21 && *cardName != 0; i++) {
                            *dst++ = *cardName++;
                        }
                        continue;
                    } else if (src[2] == '2' || src[2] == '3') {
                        src += 3;
                        for (i = 0; i < 21 && *cardName != 0; i++) {
                            if (i <= 0) {
                                *dst++ = *cardName++;
                            } else {
                                *dst++ = '?';
                                cardName++;
                                i++;
                            }
                        }
                        continue;
                    }
                }
            } else if (*src == '\\') {
                end = 0;
                for (i = 1; i < 5; i++) {
                    if (end == 0 && src[i] == 0) {
                        end = 1;
                        break;
                    }
                }
                if (end != 1 && src[1] == '0' && src[2] == 'x' && src[3] == '2' && src[4] == '2') {
                    src += 5;
                    *dst++ = '"';
                    continue;
                }
            }
        } else {
            *dst++ = *src++;
            *dst++ = *src++;
            continue;
        }
        *dst++ = *src++;
    }
    *dst = 0;
    dst = t->text;
    for (i = 0; i < 60 && *dst != 0; i++) {
        dst++;
    }
    if (t->pos == -1) {
        t->pos = i;
    }
    if (i >= 60) {
        t->active = 0;
    } else {
        t->active = 1;
    }
    t->len = i;
    for (i = 0; i < 4 && t != EVO_TEXT_LINES; i++) {
        t--;
    }
    return i;
}

const char D_801DF3B0[] = "";

s32 EVO_typeTextLine(s32 x, s32 y, EvoText *t, s32 z) {
    u8 buf[64];
    u8 *dst;
    u8 *src;
    s8 i;
    u8 c;

    dst = buf;
    if (t->len == t->pos) {
        drawText(x, y, (s32)t, 7, z);
        return -1;
    }
    src = t->text;
    for (i = 0; i < t->pos; i++) {
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
                    t->pos += 2;
                }
                break;
            }
        }
        *dst = *src;
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 1;
    } else {
        *dst++ = src[0];
        *dst = src[1];
        dst[1] = 0;
        drawText(x, y, (s32)buf, 7, z);
        t->pos += 2;
    }
    return 1;
}

s32 EVO_typeTextLines(s16 x, s16 y, s32 z) {
    EvoText *t;
    s32 i;

    for (t = EVO_TEXT_LINES, i = 0; i < 4; i++, t++) {
        if (t->active != 0 && EVO_typeTextLine(x, y + i * 12, t, z) == 1) {
            return 1;
        }
    }
    return 0;
}

void EVO_drawMessageWindow(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;
    u8 unused[0x48]; /* stack space the original reserves but never touches */

    EVO_FUSION.textTyping = EVO_typeTextLines(x, y, z);
    if (EVO_FUSION.textTyping == 0 && ((u8)EVO_FUSION.scriptState == 2 || (u8)EVO_FUSION.scriptState == 3)) {
        if (++EVO_FUSION.blinkTimer & 0x10) {
            drawIcon(x + 200, y + 0x25, 0, 0x1B, z);
        }
    } else {
        EVO_FUSION.blinkTimer = 0;
    }
}

void EVO_drawUnitPortrait(UiWindow *w) {
    Rect16 uv;
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    uv.x = 0x28;
    uv.y = EVO_FUSION.unit * 56;
    uv.w = 0x40;
    uv.h = 0x38;
    drawTexturedSprite(x, y, &uv, 0x98, 0x7C18, z, w->brightness, -1);
}

void EVO_drawRankUpBanner(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (EVO_RANK_UP_STATE == 2) {
        drawLargeText(x + 1, y + 1, (s32)"RANK MAX!", 7, z);
    } else if (EVO_RANK_UP_STATE == 1) {
        drawLargeText(x + 1, y + 1, (s32)"RANK UP!", 7, z);
    }
}

void EVO_drawReceivedBanner(UiWindow *w) {
    s32 x = w->originX;
    s32 y = w->originY;
    s32 z = w->z;

    if (EVO_CARD_RECEIVED == 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void EVO_tickFusionTypeChoice(void) {
    Rect16 rects[6];
    s32 i;
    s32 dx;
    s32 dy;

    if (EVO_FUSION.swapState == 0) {
        if (PAD_STATES[0]->pressed & 0x5000) {
            playSoundEffect(2);
            if (EVO_FUSION.fusionType == 0) {
                EVO_FUSION.swapState = 1;
            } else if (EVO_FUSION.fusionType == 1) {
                EVO_FUSION.swapState = 2;
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            EVO_SCRIPT->vars[8] = 1;
            EVO_SCRIPT->vars[1] = EVO_TYPE_CHOICE.side;
            EVO_FUSION.scriptState = 0;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            EVO_SCRIPT->vars[8] = 2;
            EVO_FUSION.scriptState = 0;
        }
    } else if (EVO_FUSION.swapState == 1) {
        EVO_FUSION.swapTimer++;
        if (EVO_FUSION.swapTimer >= 21) {
            EVO_FUSION.swapTimer = 20;
            EVO_FUSION.swapState = 0;
            EVO_FUSION.fusionType = 1;
        }
    } else if (EVO_FUSION.swapState == 2) {
        EVO_FUSION.swapTimer++;
        if (EVO_FUSION.swapTimer >= 41) {
            EVO_FUSION.swapTimer = 0;
            EVO_FUSION.swapState = 0;
            EVO_FUSION.fusionType = 0;
        }
    }
    EVO_TYPE_CHOICE.side = EVO_FUSION.fusionType;
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].win.brightness = 0x80 - EVO_TYPE_CHOICE.side * 64;
        EVO_WINDOWS[i + 3].win.brightness = EVO_TYPE_CHOICE.side * 64 + 64;
    }
    dy = rsin(EVO_FUSION.swapTimer * 1024 / 20) * 40 / 4096;
    dx = rsin(EVO_FUSION.swapTimer * 2048 / 20) * 60 / 4096;
    for (i = 0; i < 3; i++) {
        rects[i] = EVO_WINDOW_DEFS[i].rect;
        rects[i + 3] = EVO_WINDOW_DEFS[i + 3].rect;
        rects[i].x += dx;
        rects[i].y += dy;
        rects[i + 3].x -= dx;
        rects[i + 3].y -= dy;
        EVO_WINDOWS[i].win.animFrame = EVO_WINDOWS[i].win.animFrames;
        EVO_WINDOWS[i + 3].win.animFrame = EVO_WINDOWS[i + 3].win.animFrames;
        animateWindowTo(&EVO_WINDOWS[i].win, &rects[i]);
        animateWindowTo(&EVO_WINDOWS[i + 3].win, &rects[i + 3]);
    }
    for (i = 0; i < 6; i++) {
        if (i < 3) {
            EVO_WINDOWS[i].z = EVO_TYPE_CHOICE.side + 30;
        } else {
            EVO_WINDOWS[i].z = (EVO_TYPE_CHOICE.side + 30) ^ 1;
        }
    }
}

void EVO_openFusionTypeChoice(void) {
    s32 i;

    EVO_MAX_CARD_LEVEL = EVO_SCRIPT->vars[12];
    for (i = 0; i < 3; i++) {
        EVO_WINDOWS[i].win.brightness = 0x80 - EVO_TYPE_CHOICE.side * 64;
        EVO_WINDOWS[i + 3].win.brightness = EVO_TYPE_CHOICE.side * 64 + 64;
    }
    if (EVO_FUSION.typeChoiceOpen == 0) {
        EVO_FUSION.typeChoiceOpen = 1;
        EVO_FUSION.swapState = 0;
        EVO_FUSION.step = 1;
        EVO_SCRIPT->vars[8] = -1;
        for (i = 0; i < 6; i++) {
            if (EVO_FUSION.fusionType == 0) {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i].rect);
            } else if (i < 3) {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i + 3].rect);
            } else {
                animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i - 3].rect);
            }
        }
        func_80014C08(30);
    }
}

void EVO_closeFusionTypeChoice(void) {
    s32 i;

    if (EVO_FUSION.typeChoiceOpen == 1) {
        EVO_FUSION.typeChoiceOpen = 0;
        EVO_FUSION.step = 0;
        for (i = 0; i < 6; i++) {
            EVO_WINDOWS[i].win.animFrame = EVO_WINDOWS[i].win.animFrames - 1;
            animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
        }
    }
}

void EVO_drawFusionTypeIcon(EvoWindow *w) {
    Rect16 uv;
    s32 x = w->win.originX;
    s32 y = w->win.originY;
    s32 z = w->win.z;

    if (w->isPartner == 0) {
        uv.x = 0;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    } else {
        uv.x = 0x40;
        uv.y = 0;
        uv.w = 0x40;
        uv.h = 0x40;
        drawTexturedSprite(x, y, &uv, 0x97, 0x7C58, z, w->win.brightness, -1);
    }
}

void EVO_openPartnerList(void) {
    s32 i;

    if (EVO_FUSION.partnerListOpen == 0) {
        EVO_FUSION.partnerListOpen = 1;
        EVO_FUSION.partnerCount = 0;
        EVO_FUSION.partner = 0;
        for (i = 10; i < 12; i++) {
            animateWindowTo(&EVO_WINDOWS[i].win, &EVO_WINDOW_DEFS[i].rect);
        }
        for (i = 0; i < 3; i++) {
            if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
                EVO_FUSION.partnerCount++;
            }
        }
    }
    EVO_SCRIPT->vars[8] = -1;
    EVO_FUSION.step = 2;
}

void EVO_closePartnerList(void) {
    s32 i;

    EVO_FUSION.partnerListOpen = 0;
    EVO_FUSION.step = 0;
    for (i = 10; i < 12; i++) {
        animateWindowTo(&EVO_WINDOWS[i].win, (Rect16 *)-1);
    }
}

void EVO_tickPartnerList(void) {
    if (PAD_STATES[0]->pressed & 0x40) {
        playSoundEffect(0);
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
        EVO_FUSION.firstCard = ((PlayerProfile *)PLAYER_PROFILES)->partners[EVO_FUSION.partner].cardId;
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(1);
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 2;
    } else if (PAD_STATES[0]->repeat & 0x4000) {
        EVO_FUSION.partner++;
        if (EVO_FUSION.partner >= EVO_FUSION.partnerCount) {
            EVO_FUSION.partner = 0;
        } else {
            playSoundEffect(2);
        }
        if (EVO_FUSION.partnerCount == 0 || EVO_FUSION.partner >= EVO_FUSION.partnerCount - 1) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    } else if (PAD_STATES[0]->repeat & 0x1000) {
        EVO_FUSION.partner--;
        if (EVO_FUSION.partner < 0) {
            EVO_FUSION.partner = EVO_FUSION.partnerCount - 1;
        } else {
            playSoundEffect(2);
        }
        if (EVO_FUSION.partner <= 0) {
            PAD_STATES[0]->repeatEnabled = 0;
        }
    }
}

void EVO_openCardList(void) {
    animateWindowTo(&EVO_WINDOWS[10].win, (Rect16 *)-1);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.step = 3;
    EVO_FUSION.previewOpen = 0;
    EVO_CARD_LIST_MENU.row = 0;
    scrollWindowTo((s16 *)EVO_CARD_LIST_MENU.win, 0, 0);
    EVO_SCRIPT->vars[8] = -1;
}

void EVO_closeCardList(void) {
    animateWindowTo(&EVO_WINDOWS[10].win, &EVO_WINDOW_DEFS[10].rect);
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_MENU.active = 0;
}

void EVO_slideTrayOut(s32 index) {
    EvoTray *trays = EVO_TRAYS;

    trays[index].x -= 10;
    if (trays[index].x < -0x58) {
        trays[index].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = -1;
        if (index == 0) {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.firstCard]--;
        } else {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.firstCard]++;
        }
    }
}

void EVO_slideTrayIn(s32 index) {
    EvoTray *trays = EVO_TRAYS;

    trays[index].x += 10;
    if (trays[index].x >= 15) {
        trays[index].x = 14;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = -1;
    }
}

void EVO_showBothTrays(void) {
    EVO_TRAYS[0].x += 10;
    EVO_TRAYS[1].x += 10;
    if (EVO_TRAYS[0].x >= 15) {
        EVO_TRAYS[0].x = 14;
    }
    if (EVO_TRAYS[1].x >= 0x67) {
        EVO_TRAYS[1].x = 0x66;
    }
    if (EVO_TRAYS[0].x == 14 && EVO_TRAYS[1].x == 0x66) {
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
    }
}

void EVO_repickSecondCard(void) {
    EVO_TRAYS[0].x -= 10;
    EVO_TRAYS[1].x -= 10;
    if (EVO_TRAYS[1].x < 14) {
        EVO_TRAYS[0].x = -0x58;
        EVO_TRAYS[1].x = 14;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        animateWindowTo(&EVO_CARD_LIST_WINDOW, &EVO_CARD_LIST_MENU.rect);
        EVO_startSecondCardPick();
    }
}

void EVO_checkCardCapacity(s16 cardId) {
    if (cardId < 0) {
        cardId = -1;
    }
    if (EVO_SCRIPT->vars[13] != 0) {
        EVO_SCRIPT->vars[11] = 0;
        return;
    }
    EVO_SCRIPT->vars[11] = getOwnedCardCount(0, cardId);
    if ((((PlayerProfile *)PLAYER_PROFILES)->cardCollection[cardId] & 7) + 1 >= 7) {
        EVO_SCRIPT->vars[11] = -1;
    } else {
        EVO_SCRIPT->vars[11] = 0;
    }
}

void EVO_tickCardList(void) {
    s16 cardId;
    s32 blocked = 0;
    s32 i;

    cardId = EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->id;
    EVO_CURSOR_CARD = cardId;
    if (PAD_STATES[0]->pressed & 0x100) {
        if (EVO_FUSION.previewOpen != 0) {
            return;
        }
        if (EVO_FUSION.sortMenuOpen == 0) {
            EVO_FUSION.sortMenuOpen = 1;
            EVO_CARD_LIST_MENU.active = 0;
            animateWindowTo(&EVO_SORT_WINDOW, &EVO_SORT_MENU.rect);
            playSoundEffect(3);
            return;
        }
        playSoundEffect(4);
        EVO_FUSION.sortMenuOpen = 0;
        EVO_CARD_LIST_MENU.active = 1;
        animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    } else if (EVO_FUSION.sortMenuOpen == 0) {
        if (EVO_FUSION.previewOpen == 0) {
            if (PAD_STATES[0]->pressed & 0x40) {
                if (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->fusionPoints == 0) {
                    blocked = 1;
                } else if (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->type == 0 &&
                           (EVO_CARD_LIST[EVO_CARD_LIST_MENU.row]->attr & 0xF) > EVO_MAX_CARD_LEVEL) {
                    blocked = 1;
                }
                if (EVO_SPARE_CARD_COUNTS[cardId] == 0) {
                    return;
                }
                if (cardId >= 0xAC && cardId < 0xBF) {
                    for (i = 0; i < 6; i++) {
                        if (EVO_PARTNER_CARD_IDS[i] == cardId) {
                            i = -1;
                            break;
                        }
                    }
                    if (i == -1) {
                        initDialog((u8 *)&EVO_DIALOG, "You can't use Partner Cards\nin Fusion.", 0);
                    } else {
                        initDialog((u8 *)&EVO_DIALOG, "You can't use that Card in Fusion.", 0);
                    }
                    runDialog(&EVO_DIALOG);
                    return;
                }
                if (blocked) {
                    return;
                }
                playSoundEffect(0);
                if (EVO_FUSION.fusionType == 0) {
                    if (EVO_FUSION.pickSlot != 2) {
                        EVO_CARD_LIST_MENU.active = 0;
                        EVO_SCRIPT->vars[8] = 1;
                        EVO_FUSION.step = 0;
                        EVO_loadCardImage(cardId, 0);
                        EVO_FUSION.firstCard = cardId;
                        EVO_FUSION.busy[1] = 1;
                        return;
                    }
                    EVO_CARD_LIST_MENU.active = 0;
                    EVO_loadCardImage(cardId, 1);
                    do {
                        func_80014C08(1);
                    } while (EVO_FUSION.busy[0] != 0);
                    EVO_FUSION.secondCard = cardId;
                    EVO_findFusionResult();
                    EVO_checkCardCapacity(EVO_FUSION.result);
                    EVO_FUSION.step = 11;
                    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
                    EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]--;
                    EVO_FUSION.busy[2] = 1;
                    EVO_loadCardImage(EVO_FUSION.result, 2);
                    do {
                        func_80014C08(1);
                    } while (EVO_FUSION.busy[0] != 0);
                } else if (EVO_FUSION.fusionType == 1) {
                    EVO_CARD_LIST_MENU.active = 0;
                    uploadTim((u32 *)((u8 *)EVO_FUSION.cardArchive + EVO_FUSION.cardArchive[cardId]), 0x1C0, 0x190, 0x180, 0x1FC);
                    EVO_FUSION.secondCard = cardId;
                    EVO_FUSION.previewOpen = 1;
                    animateWindowTo(&EVO_WINDOWS[12].win, &EVO_WINDOW_DEFS[12].rect);
                }
            } else if (PAD_STATES[0]->pressed & 0x10) {
                EVO_FUSION.previewOpen = 0;
                EVO_FUSION.step = 0;
                EVO_SCRIPT->vars[8] = 2;
                playSoundEffect(1);
            }
        } else if (PAD_STATES[0]->pressed & 0x40) {
            playSoundEffect(0);
            EVO_FUSION.step = 0;
            EVO_SCRIPT->vars[8] = 1;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playSoundEffect(1);
            EVO_CARD_LIST_MENU.active = 1;
            EVO_FUSION.previewOpen = 0;
            animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        playSoundEffect(4);
        EVO_FUSION.sortMenuOpen = 0;
        EVO_CARD_LIST_MENU.active = 1;
        animateWindowTo(&EVO_SORT_WINDOW, (Rect16 *)-1);
    }
}

void EVO_startPartnerFusion(void) {
    PlayerProfile *profile;

    EVO_SCRIPT->vars[8] = -2;
    EVO_SCRIPT->vars[18] = 0;
    EVO_SCRIPT->vars[19] = 0;
    EVO_SCRIPT->vars[7] = 1;
    EVO_playEffect(0, 0);
    EVO_findPartnerReward();
    EVO_FUSION.step = 4;
    removeCardFromCollection(0, EVO_FUSION.secondCard, 1);
    EVO_SPARE_CARD_COUNTS[EVO_FUSION.secondCard]--;
    profile = (PlayerProfile *)PLAYER_PROFILES;
    profile->fusionCardsUsed++;
    if ((u16)profile->fusionCardsUsed >= 10000) {
        profile->fusionCardsUsed = 9999;
    }
}

void EVO_cancelPartnerFusion(void) {
    EVO_SCRIPT->vars[8] = -1;
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.previewOpen = 0;
    animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
    EVO_FUSION.step = 3;
}

void EVO_leaveForCutscene(void) {
    setScreenFadeParams(0, 2, 6);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
    EVO_FUSION.cutscene = 1;
}

void EVO_findPartnerReward(void) {
    s32 i;
    s32 ability;

    EVO_FUSION.partnerKind = -1;
    ability = -1;
    for (i = 0; i < 6; i++) {
        if (EVO_PARTNER_CARD_IDS[i] == EVO_FUSION.firstCard && EVO_FUSION.partnerKind == -1) {
            EVO_FUSION.partnerKind = i;
        }
    }
    if (EVO_FUSION.partnerKind != -1) {
        for (i = 0; i < 5; i++) {
            if (EVO_PARTNER_FUSION_REWARDS[EVO_FUSION.partnerKind][i].card == EVO_FUSION.secondCard && ability == -1) {
                ability = EVO_PARTNER_FUSION_REWARDS[EVO_FUSION.partnerKind][i].ability;
                if (getPartnerAbilityState(0, ability) == 0) {
                    grantPartnerAbility(0, ability);
                    EVO_FUSION.rewardStep = 0;
                } else {
                    ability = -1;
                }
                EVO_FUSION.result = ability;
            }
        }
    }
    if (ability == -1) {
        EVO_FUSION.rewardStep = 3;
        EVO_SCRIPT->vars[19] = ability;
        EVO_FUSION.partnerKind = ability;
        EVO_FUSION.result = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->fusionPoints;
    }
}

void EVO_tickPartnerReward(void) {
    char text[168];
    s32 i;

    switch (EVO_FUSION.rewardStep) {
    case 0:
        EVO_SCRIPT->vars[18] = 2;
        break;
    case 1:
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", EVO_FUSION.result, EVO_DIGI_PARTS[EVO_FUSION.result].name);
        initDialog((u8 *)&EVO_DIALOG, text, 0x80);
        runDialog(&EVO_DIALOG);
        EVO_FUSION.rewardStep = 10;
        EVO_FUSION.step = 3;
        EVO_CARD_LIST_MENU.active = 1;
        EVO_FUSION.previewOpen = 0;
        for (i = 0; i < 4; i++) {
            EVO_STAT_BONUSES[i] = 0;
        }
        EVO_SCRIPT->vars[8] = -1;
        break;
    case 2:
        if (EVO_SCRIPT->vars[19] == -1) {
            EVO_addPartnerExp();
        } else if (EVO_FUSION.scriptState == 0) {
            if (EVO_LEVEL_UP_PENDING == 0) {
                if (EVO_NEW_DIGI_PART != -1) {
                    grantPartnerAbility(0, EVO_NEW_DIGI_PART);
                    sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c6[%s]", EVO_NEW_DIGI_PART, EVO_DIGI_PARTS[EVO_NEW_DIGI_PART].name);
                    initDialog((u8 *)&EVO_DIALOG, text, 0x80);
                    runDialog(&EVO_DIALOG);
                }
                EVO_SCRIPT->vars[19] = -1;
                if (EVO_RANK_UP_STATE != 0) {
                    animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
                }
            } else {
                EVO_LEVEL_UP_PENDING = 0;
            }
        }
        break;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
        break;
    }
}

void EVO_addPartnerExp(void) {
    char text[168];
    Partner *partner;
    s32 ability;
    s32 i;

    partner = &((PlayerProfile *)PLAYER_PROFILES)->partners[EVO_FUSION.partner];
    bzero((Scene3D *)text, 0xA1);
    if (EVO_FUSION.partnerKind >= 0) {
        EVO_SCRIPT->vars[18] = 2;
        sprintf(text, "Obtained\nDigi-Part *c5 No.%3.3d\n*c7[%s]", EVO_FUSION.result, EVO_DIGI_PARTS[EVO_FUSION.result].name);
        initDialog((u8 *)&EVO_DIALOG, text, 0x80);
        runDialog(&EVO_DIALOG);
        EVO_FUSION.partnerKind = -2;
        return;
    }
    if (EVO_FUSION.partnerKind == -1) {
        if (EVO_FUSION.result > 0) {
            EVO_FUSION.result--;
            if ((s8)partner->level < 99) {
                partner->exp++;
                if (getExpForNextLevel((s8)partner->level) - (u16)partner->exp > 0) {
                    return;
                }
                partner->level++;
                if ((s8)partner->level >= 99) {
                    EVO_FUSION.result = 0;
                }
                ability = findNewPartnerAbility((AbilityLearnEntry *)EVO_DIGI_PARTS, 0, EVO_FUSION.partner);
                EVO_SCRIPT->vars[19] = 1;
                EVO_NEW_DIGI_PART = -1;
                EVO_LEVEL_UP_PENDING = 1;
                EVO_RANK_UP_STATE = 1;
                animateWindowTo(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT);
                if (ability >= 0) {
                    EVO_SCRIPT->vars[19] = 2;
                    EVO_NEW_DIGI_PART = ability;
                }
                ability = func_8004994C(0, EVO_FUSION.partner);
                if (ability >= 0) {
                    EVO_STAT_BONUSES[ability] += 10;
                }
            } else {
                EVO_RANK_UP_STATE = 2;
                animateWindowTo(&EVO_RANK_UP_WINDOW, &EVO_RANK_UP_RECT);
                do {
                    func_80014C08(1);
                } while (!(PAD_STATES[0]->pressed & 0x40));
                playMenuSound(1);
                animateWindowTo(&EVO_RANK_UP_WINDOW, (Rect16 *)-1);
                EVO_FUSION.result = 0;
            }
        } else {
            for (i = 0; i < 4; i++) {
                if (i == 0) {
                    partner->hpBonus += EVO_STAT_BONUSES[0];
                } else {
                    partner->attackBonus[i - 1] += EVO_STAT_BONUSES[i];
                }
            }
            updatePartnerStats(0, EVO_FUSION.partner);
            EVO_FUSION.partnerKind = -2;
        }
        return;
    }
    EVO_FUSION.rewardStep = 10;
    EVO_FUSION.step = 3;
    EVO_CARD_LIST_MENU.active = 1;
    EVO_FUSION.previewOpen = 0;
    for (i = 0; i < 4; i++) {
        EVO_STAT_BONUSES[i] = 0;
    }
    EVO_SCRIPT->vars[8] = -1;
}

void EVO_tickFusionResult(void) {
    s32 i;

    if (EVO_FUSION.resultStep == 0) {
        EVO_FUSION.resultStep = 1;
        for (i = 0; i < 2; i++) {
            EVO_TRAYS[i].merge = 0;
        }
        EVO_SCRIPT->vars[8] = -1;
        return;
    }
    if (EVO_FUSION.resultStep == 1) {
        if (EVO_FUSION.resultKind == 1) {
            EVO_playEffect(7, 0);
        } else {
            EVO_playEffect(6, 0);
        }
        removeCardFromCollection(0, EVO_FUSION.firstCard, 1);
        removeCardFromCollection(0, EVO_FUSION.secondCard, 1);
        if (addCardToCollection(0, EVO_FUSION.result, 1) >= 0) {
            EVO_SPARE_CARD_COUNTS[EVO_FUSION.result]++;
            EVO_CARD_RECEIVED = 1;
        } else {
            EVO_CARD_RECEIVED = 0;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->fusedCards++;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusedCards >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusedCards = 9999;
        }
        ((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed += 2;
        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed >= 10000) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusionCardsUsed = 9999;
        }
        if (EVO_SCRIPT->vars[13] != 0) {
            ((PlayerProfile *)PLAYER_PROFILES)->fusionMutations++;
            if ((u16)((PlayerProfile *)PLAYER_PROFILES)->fusionMutations >= 10000) {
                ((PlayerProfile *)PLAYER_PROFILES)->fusionMutations = 9999;
            }
        }
        if (EVO_FUSION.resultKind == 1) {
            EVO_FUSION.cutscene = 1;
            EVO_CUTSCENE_MODELS[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->modelId;
            EVO_CUTSCENE_MODELS[1] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->modelId;
            return;
        }
        animateWindowTo(&EVO_WINDOWS[13].win, &EVO_WINDOW_DEFS[13].rect);
        EVO_FUSION.resultStep = 2;
    }
    EVO_TRAYS[0].x = 0x3A;
    EVO_TRAYS[1].x = -0x58;
    EVO_FUSION.scriptState = 0;
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&EVO_WINDOWS[13].win, (Rect16 *)-1);
        playSoundEffect(0);
        EVO_FUSION.step = 0x12;
        EVO_FUSION.resultStep = 3;
    }
}

void EVO_closeFusionResult(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_SCRIPT->vars[8] = 1;
        EVO_FUSION.resultStep = 0;
    }
}

void EVO_showCutsceneResult(void) {
    if (EVO_FUSION.cutscene != 0) {
        EVO_playEffect(8, 0);
        EVO_FUSION.cutscene = 0;
        animateWindowTo(&EVO_WINDOWS[13].win, &EVO_WINDOW_DEFS[13].rect);
    }
    if (PAD_STATES[0]->pressed & 0x40) {
        animateWindowTo(&EVO_WINDOWS[13].win, (Rect16 *)-1);
        playSoundEffect(0);
        EVO_FUSION.step = 0x12;
        EVO_FUSION.resultStep = 3;
    }
}

void func_801EBE08(void) {
    EVO_TRAYS[0].x -= 10;
    if (EVO_TRAYS[0].x < -0x58) {
        EVO_TRAYS[0].x = -0x58;
        EVO_FUSION.step = 0;
        EVO_FUSION.scriptState = 0;
        EVO_SCRIPT->vars[8] = 1;
    }
}

#define PARTNER(i) (((PlayerProfile *)PLAYER_PROFILES)->partners[i])

void EVO_drawPartnerList(UiWindow *w) {
    Rect16 rect;
    char text[72];
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 dim;
    s32 level;
    s32 index;
    s32 next;

    x = w->originX;
    y = w->originY;
    z = w->z;
    for (i = 0; i < 3; i++) {
        if (PARTNER(i).cardId == 0) {
            continue;
        }
        if (EVO_FUSION.partner == i) {
            dim = 0;
            level = 0; /* dead store, but it keeps the branch the original has */
        } else {
            dim = 1;
        }
        level = PARTNER(i).card[0].attr >> 4;
        if (level != 0) {
            level--;
        }
        index = getSlotPartnerIndex(0, i);
        rect.x = (index % 3) * 40;
        rect.y = (index / 3) * 40 + 0x140;
        rect.w = 40;
        rect.h = 40;
        drawTexturedSprite(x, y, &rect, 0x97, ((level + 0x1F8) << 6) | 0x18, z, dim ? 0x40 : 0x80, -1);
        drawTextColored(x + 0x2C, y + 2, PARTNER(i).card[0].name, EVO_TEXT_COLORS[dim].b, 8, z);
        drawLargeTextColored(x + 0x82, y + 4, "NEXT", 6, EVO_TEXT_COLORS[dim].b, z);
        next = 0;
        if ((s8)PARTNER(i).level < 99) {
            next = getExpForNextLevel((s8)PARTNER(i).level) - (u16)PARTNER(i).exp;
        }
        sprintf(text, "*s0%3d", next);
        drawTextColored(x + 0xB0, y + 2, text, EVO_TEXT_COLORS[dim].b, 8, z);
        y += 2;
        drawLargeTextColored(x + 0x2C, y + 0x10, "RANK", 6, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%2d", (s8)PARTNER(i).level);
        drawTextColored(x + 0x56, y + 0xE, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x6C, y + 0xE, 0, 0x1A, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].hp);
        drawTextColored(x + 0x7A, y + 0xE, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x9C, y + 0xE, 0, 7, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[0].power);
        drawTextColored(x + 0xAA, y + 0xE, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x6C, y + 0x1A, 0, 8, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[1].power);
        drawTextColored(x + 0x7A, y + 0x1A, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawIconColored(x + 0x9C, y + 0x1A, 0, 9, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", PARTNER(i).card[0].attack[2].power);
        drawTextColored(x + 0xAA, y + 0x1A, text, EVO_TEXT_COLORS[dim].b, 8, z);
        drawLargeTextColored(x + 0x2C, y + 0x1C, "EXP", 6, EVO_TEXT_COLORS[dim].b, z);
        sprintf(text, "*s0%4d", (u16)PARTNER(i).exp);
        drawTextColored(x + 0x4A, y + 0x1A, text, EVO_TEXT_COLORS[dim].b, 8, z);
        y += 0x28;
    }
}

void EVO_drawPartnerStatus(UiWindow *w) {
    u8 palettes[8] = { 2, 1, 4, 9, 6, 8, 8, 8 };
    char text[72];
    Rect16 rect;
    Partner *partner;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 index;
    s32 next;

    x = w->originX;
    y = w->originY;
    z = w->z;
    partner = &PARTNER(EVO_FUSION.partner);
    w->palette = palettes[partner->card[0].attr >> 4];
    drawTextColored(x, y, partner->card[0].name, EVO_TEXT_COLORS[0].b, 7, z);
    drawLargeTextColored(x, y + 0x10, "RANK", 6, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%2d", (s8)partner->level);
    drawTextColored(x + 0x34, y + 0xE, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawLargeTextColored(x, y + 0x1E, "EXP", 6, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", (u16)partner->exp);
    drawTextColored(x + 0x28, y + 0x1C, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawLargeTextColored(x, y + 0x2C, "NEXT", 6, EVO_TEXT_COLORS[0].b, z);
    next = 0;
    if ((s8)partner->level < 99) {
        next = getExpForNextLevel((s8)partner->level) - (u16)partner->exp;
    }
    sprintf(text, "*s0%3d", next);
    drawTextColored(x + 0x2E, y + 0x2A, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x38, 0, 0x1A, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].hp);
    drawTextColored(x + 0x28, y + 0x38, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x46, 0, 7, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[0].power);
    drawTextColored(x + 0x28, y + 0x46, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x54, 0, 8, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[1].power);
    drawTextColored(x + 0x28, y + 0x54, text, EVO_TEXT_COLORS[0].b, 7, z);
    drawIconColored(x, y + 0x62, 0, 9, EVO_TEXT_COLORS[0].b, z);
    sprintf(text, "*s0%4d", partner->card[0].attack[2].power);
    drawTextColored(x + 0x28, y + 0x62, text, EVO_TEXT_COLORS[0].b, 7, z);
    for (i = 0; i < 4; i++) {
        if (EVO_STAT_BONUSES[i] > 0) {
            sprintf(text, "+%d", EVO_STAT_BONUSES[i]);
            drawTextColored(x + 0x42, y + (i + 4) * 14, text, EVO_TEXT_COLORS[0].b, 5, z);
        }
    }
    index = getSlotPartnerIndex(0, EVO_FUSION.partner);
    rect.x = (index % 3) * 84;
    rect.y = (index / 3) * 123;
    rect.w = 0x54;
    rect.h = 0x7B;
    drawTexturedSprite(x, y, &rect, 0x1A, getClut(index * 16 + 0x190, 0x1EF), z, 0x80, -1);
}

void EVO_drawCardInfo(UiWindow *w) {
    Rect16 rect;
    char text[72];
    s32 x;
    s32 y;
    s32 z;
    s16 id;

    x = w->originX;
    y = w->originY;
    z = w->z;
    id = EVO_FUSION.secondCard;
    if (id < 0xBF) {
        sprintf(text, EVO_FMT_CARD_NUMBER, ((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, ((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->attr >> 4, z);
        drawIcon(x + 0x46, y + 0x27, 0, (((DigimonCardData *)(DIGIMON_CARDS + id * 0x13C))->attr & 0xF) + 0x10, z);
        drawLargeText(x, y + 0x2A, (s32)"Level", 6, z);
    } else if ((id -= 0xBF) < 0x66) {
        sprintf(text, EVO_FMT_CARD_NUMBER, ((DigimonCardData *)(OPTION_CARDS + id * 0xE2))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(OPTION_CARDS + id * 0xE2))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, 5, z);
    } else {
        id -= 0x66;
        sprintf(text, EVO_FMT_CARD_NUMBER, ((DigimonCardData *)(DIGIVOLVE_CARDS + id * 0x70))->id);
        drawText(x + 0x40, y + 0xD, (s32)text, 7, z);
        drawText(x, y, (s32)((DigimonCardData *)(DIGIVOLVE_CARDS + id * 0x70))->name, 7, z);
        drawIcon(x + 0x46, y + 0x1A, 0, 6, z);
    }
    drawLargeText(x, y + 0x12, (s32)"Number", 6, z);
    drawLargeText(x, y + 0x1E, (s32)EVO_STR_SPEC, 6, z);
    rect.x = 0;
    rect.y = 0x90;
    rect.w = 0x28;
    rect.h = 0x28;
    drawTexturedSprite(x + 0x60, y + 0xD, &rect, 0x97, 0x7F18, z, 0x80, -1);
}

/* the last two bytes are leftovers in the original, not zero padding */
const char EVO_STR_SPEC[8] = "Spec.\0\x85\xA4";

void EVO_resetFusion(void) {
    EVO_FUSION.secondCard = -1;
    EVO_FUSION.firstCard = -1;
    EVO_FUSION.hideResult = 1;
    EVO_FUSION.scriptState = 4;
    EVO_FUSION.sortMenuOpen = 0;
    EVO_FUSION.previewOpen = 0;
    EVO_FUSION.pickSlot = 1;
    EVO_FUSION.busy[1] = 0;
    EVO_FUSION.busy[2] = 0;
}

void EVO_cancelFirstCard(void) {
    EVO_FUSION.pickSlot = 1;
    EVO_FUSION.step = 8;
    animateWindowTo(&EVO_CARD_LIST_WINDOW, (Rect16 *)-1);
    EVO_CARD_LIST_MENU.active = 0;
}

void EVO_startSecondCardPick(void) {
    EVO_FUSION.pickSlot = 2;
    EVO_SCRIPT->vars[8] = -1;
    EVO_FUSION.step = 7;
}

s16 EVO_findFusionResult(void) {
    s8 types[2];
    s8 i;
    s8 j;
    s8 level;
    s8 kind;

    EVO_FUSION.result = -1;
    level = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->fusionPoints + EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->fusionPoints;
    for (i = 0; i < 2; i++) {
        if (i == 0) {
            types[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->type;
        } else {
            types[i] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->type;
        }
        switch (types[i]) {
        case 0:
            if (i == 0) {
                types[0] = EVO_CARDS_BY_ID[EVO_FUSION.firstCard]->attr >> 4;
            } else {
                types[i] = EVO_CARDS_BY_ID[EVO_FUSION.secondCard]->attr >> 4;
            }
            break;
        case 1:
        case 2:
            types[i] = 5;
            break;
        }
    }
    kind = EVO_FUSION_RESULT_TYPES[types[0]][types[1]];
    for (j = 0; j < 2; j++) {
        for (i = 0; i < 20; i++) {
            if (EVO_FUSION_RECIPES[i][j] == EVO_FUSION.firstCard && EVO_FUSION_RECIPES[i][1 - j] == EVO_FUSION.secondCard) {
                EVO_FUSION.result = EVO_FUSION_RECIPES[i][2];
                EVO_CUTSCENE_MODELS[2] = EVO_FUSION_RECIPES[i][3];
            }
        }
    }
    EVO_SCRIPT->vars[14] = 0;
    EVO_SCRIPT->vars[13] = 0;
    if (EVO_FUSION.result == -1) {
        EVO_FUSION.roll = rand() % 100;
        if (EVO_FUSION.roll < 0) {
            EVO_FUSION.roll = EVO_FUSION.roll * -1;
        }
        if (EVO_FUSION.roll <= (s8)(level / 10)) {
            EVO_SCRIPT->vars[13] = 1;
            EVO_FUSION.resultKind = 2;
            EVO_FUSION.roll = rand() % 100;
            if (EVO_FUSION.roll < 0) {
                EVO_FUSION.roll = EVO_FUSION.roll * -1;
            }
            if (EVO_FUSION.roll < 21) {
                EVO_FUSION.result = rand() % 12 + 0x111;
                if (getOwnedCardCount(0, EVO_FUSION.result) >= 6) {
                    EVO_FUSION.result = 200;
                }
            } else if (EVO_FUSION.roll < 61) {
                EVO_FUSION.result = 200;
            } else {
                EVO_FUSION.roll = rand() % 3 + 1;
                EVO_findCardOfLevel(kind, level + EVO_FUSION.roll, &EVO_FUSION.result, EVO_FUSION.firstCard, EVO_FUSION.secondCard);
                if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                    EVO_FUSION.result = -1;
                }
                for (i = 0; EVO_FUSION.result == -1;) {
                    i++;
                    EVO_findCardOfLevel(kind, level + EVO_FUSION.roll + i, &EVO_FUSION.candidates[0], 500, 500);
                    EVO_findCardOfLevel(kind, level + EVO_FUSION.roll - i, &EVO_FUSION.candidates[1], 500, 500);
                    if (EVO_FUSION.candidates[0] != -1 && EVO_FUSION.candidates[1] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[rand() % 2];
                    } else if (EVO_FUSION.candidates[0] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[0];
                    } else if (EVO_FUSION.candidates[1] != -1) {
                        EVO_FUSION.result = EVO_FUSION.candidates[1];
                    }
                    if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                        EVO_FUSION.result = -1;
                    }
                }
            }
        } else {
            EVO_findCardOfLevel(kind, level, &EVO_FUSION.result, EVO_FUSION.firstCard, EVO_FUSION.secondCard);
            if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                EVO_FUSION.result = -1;
            }
            for (i = 0; EVO_FUSION.result == -1;) {
                i++;
                EVO_findCardOfLevel(kind, level + i, &EVO_FUSION.candidates[0], 500, 500);
                EVO_findCardOfLevel(kind, level - i, &EVO_FUSION.candidates[1], 500, 500);
                if (EVO_FUSION.candidates[0] != -1 && EVO_FUSION.candidates[1] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[rand() % 2];
                } else if (EVO_FUSION.candidates[0] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[0];
                } else if (EVO_FUSION.candidates[1] != -1) {
                    EVO_FUSION.result = EVO_FUSION.candidates[1];
                }
                if (EVO_FUSION.result == EVO_FUSION.firstCard || EVO_FUSION.result == EVO_FUSION.secondCard) {
                    EVO_FUSION.result = -1;
                }
            }
            EVO_FUSION.resultKind = 0;
        }
    } else {
        EVO_FUSION.resultKind = 1;
        EVO_SCRIPT->vars[14] = 1;
    }
    switch (EVO_CARDS_BY_ID[EVO_FUSION.result]->type) {
    case 0:
        EVO_SCRIPT->vars[15] = EVO_CARDS_BY_ID[EVO_FUSION.result]->attr & 0xF;
        EVO_SCRIPT->vars[16] = EVO_CARDS_BY_ID[EVO_FUSION.result]->attr >> 4;
        break;
    case 1:
        EVO_SCRIPT->vars[15] = 5;
        EVO_SCRIPT->vars[16] = 5;
        break;
    case 2:
        EVO_SCRIPT->vars[15] = 6;
        EVO_SCRIPT->vars[16] = 6;
        break;
    }
    EVO_SCRIPT->vars[17] = EVO_FUSION.resultKind;
    EVO_SCRIPT->vars[11] = EVO_FUSION.result;
    return EVO_FUSION.result;
}

void EVO_findCardOfLevel(u8 type, u8 level, s16 *out, s16 excludeA, s16 excludeB) {
    s16 candidates[10];
    s16 i;
    s16 j;
    s16 tmp;
    s8 count;

    *out = -1;
    if (level < 2) {
        level = 2;
    }
    if (level >= 36) {
        level = 35;
    }
    count = 0;
    for (i = 0; i < 10; i++) {
        candidates[i] = -1;
    }
    if (type == 5) {
        for (i = EVO_CARD_ID_RANGES[5].first; i <= EVO_CARD_ID_RANGES[5].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = EVO_CARD_ID_RANGES[6].first; i <= EVO_CARD_ID_RANGES[6].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level && i != excludeA && i != excludeB) {
                candidates[count] = i;
                count++;
            }
        }
        for (i = 0; i < count; i++) {
            j = rand() % count;
            tmp = candidates[i];
            candidates[i] = candidates[j];
            candidates[j] = tmp;
        }
        *out = candidates[0];
    } else {
        for (i = EVO_CARD_ID_RANGES[type].first; i <= EVO_CARD_ID_RANGES[type].last; i++) {
            if (EVO_CARDS_BY_ID[i]->level == level) {
                *out = i;
                return;
            }
        }
    }
}

void EVO_uploadShadedClut(EvoClut *clut, u16 flags) {
    u16 *src;
    u16 *dst;
    s32 brighten;
    s32 level;
    s32 i;
    s32 color;
    s32 r;
    s32 g;
    s32 b;

    brighten = clut->brighten;
    level = clut->level;
    src = clut->src;
    dst = clut->dst;
    for (i = 0; i < clut->rect.w * clut->rect.h; i++) {
        color = *src++;
        if (color != 0) {
            r = color & 0x1F;
            g = (color >> 5) & 0x1F;
            b = (color >> 10) & 0x1F;
            if (brighten == 0) {
                r = r * level / 255;
                g = g * level / 255;
                b = b * level / 255;
            } else {
                r += (31 - r) * level / 255;
                g += (31 - g) * level / 255;
                b += (31 - b) * level / 255;
            }
            if (r < 0) {
                r = 0;
            } else if (r >= 32) {
                r = 31;
            }
            if (g < 0) {
                g = 0;
            } else if (g >= 32) {
                g = 31;
            }
            if (b < 0) {
                b = 0;
            } else if (b >= 32) {
                b = 31;
            }
            color = flags | (color & 0x8000) | (b << 10) | (g << 5) | r;
        }
        *dst++ = color;
    }
    LoadImage((s16 *)&clut->rect, (s32)clut->dst);
    DrawSync(0);
}

EvoModelFx *EVO_createModelEffect(s16 level, EvoFx *fx, s32 modelId, s32 anim, s32 unused, s32 vramSlot, u8 arg6,
                          s32 loop, s32 pak, s32 arg9) {
    EvoModelFx *obj;
    s32 slot;

    obj = allocTaskHeapBlock(sizeof(EvoModelFx));
    for (slot = 2; slot < 23; slot++) {
        if (SCENE_3D->modelState[slot] == 0) {
            break;
        }
    }
    if (slot >= 23 || loadModel(slot, modelId, vramSlot, pak, 0) == 0) {
        freeHeapBlock(obj);
        return NULL;
    }
    obj->slot = slot;
    obj->model = SCENE_3D->models[slot];
    if (anim >= 0 && loadModelAnimation(slot, anim, 0, pak) != 0) {
        applyAnimationFirstFrame(slot, 0);
        startModelAnimation(slot, 0, -2, loop ^ 1);
    }
    if (fx != NULL) {
        obj->fx = *fx;
        initEffectObject(obj);
        SCENE_3D->modelState[obj->slot] = -1;
        obj->active = 1;
    } else {
        SCENE_3D->modelState[obj->slot] = 1;
        obj->active = 0;
    }
    obj->unk574 = arg9;
    obj->model->link = (s32)obj;
    obj->unk570 = -1;
    {
        Rect16 rect = { 0x30, 0x70, 0x10, 0x10 };

        if (obj->unk574 == 0) {
            rect.y = 0xF0;
        }
        rect.x += ((obj->model->tpageOffset / 0x10000 + 5) & 0xF) << 6;
        rect.y += ((obj->model->tpageOffset / 0x10000 + 5) >> 4) << 8;
        StoreImage2(&rect, (u32 *)obj->clut.src);
        obj->clut.rect = rect;
        obj->clut.brighten = 0;
        obj->prevLevel = level;
        obj->clut.level = level;
        if (level != 0xFF) {
            EVO_uploadShadedClut(&obj->clut, 0x8000);
        }
    }
    obj->unk56F = arg6;
    return obj;
}

void EVO_tickModelEffect(EvoModelFx *obj) {
    if (obj->active != 0) {
        if (obj->fx.suspended != 0) {
            tickEffectStartDelay(obj);
            SCENE_3D->modelState[obj->slot] = -1;
        } else {
            obj->clut.level = updateEffectBrightness(obj, obj->clut.level);
            if (obj->clut.level != obj->prevLevel) {
                obj->prevLevel = obj->clut.level;
                EVO_uploadShadedClut(&obj->clut, 0x8000);
            }
            if (obj->clut.level == 0) {
                SCENE_3D->modelState[obj->slot] = -1;
            } else {
                SCENE_3D->modelState[obj->slot] = 3;
            }
        }
    }
}

void EVO_freeModelEffect(EvoModelFx *obj) {
    unloadModel(obj->slot);
    if (obj->clut.level != 0xFF) {
        obj->clut.level = 0xFF;
        EVO_uploadShadedClut(&obj->clut, 0x8000);
    }
    freeHeapBlock(obj);
}

EvoFadeRect *EVO_createFadeRect(s16 *rect, Bytes4 *from, Bytes4 *to, u8 blendMode, s16 speed, u8 mode) {
    EvoFadeRect *f;

    f = allocTaskHeapBlock(sizeof(EvoFadeRect));
    initPolyF4Pair(&f->poly[0], &f->poly[1], from, blendMode, &f->tpage[0], &f->tpage[1], rect, 1, 1);
    f->from = *from;
    f->to = *to;
    f->speed = speed;
    f->mode = mode;
    if (mode < 2 || mode == 3) {
        f->state = 1;
    } else {
        f->state = 0;
    }
    return f;
}

s32 EVO_tickFadeRect(EvoFadeRect *f) {
    PolyF4 *poly;
    DrTPage *tpage;
    s32 doneIn = 0;
    s32 doneOut = 0;

    if (f->state == 0) {
        return -1;
    }
    poly = &f->poly[FRAME_BUFFER_INDEX];
    tpage = &f->tpage[FRAME_BUFFER_INDEX];
    if (f->state == 1) {
        doneIn = stepColorToward(f->speed, &poly->r0, f->to.b[0], &poly->g0, f->to.b[1], &poly->b0, f->to.b[2]);
    } else if (f->state == 2) {
        doneOut = stepColorToward(f->speed, &poly->r0, f->from.b[0], &poly->g0, f->from.b[1], &poly->b0, f->from.b[2]);
        if (doneOut != 0 && f->mode == 3) {
            f->state = 1;
        }
    }
    if (*(u32 *)&poly->r0 & 0xFFFFFF) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)poly);
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)tpage);
    }
    if (doneIn != 0 && (f->mode == 1 || f->mode == 3)) {
        f->state = 2;
    }
    if (doneIn == 0 && doneOut == 0) {
        return 0;
    }
    if (doneIn != 0) {
        return 1;
    }
    if (doneOut != 0) {
        return 2;
    }
    return -1;
}

s32 EVO_loadEffectPak(s32 index) {
    char path[32];
    s32 file;

    sprintf(path, "C:\\EVO_PAK\\%d.PAK", index);
    file = loadFileTagged((s32 *)path, getCurrentTaskId(), 0x12C);
    if (file != 0) {
        return file;
    }
    return 0;
}

void EVO_loadEffectArchive(void) {
    func_800149B8(0, -1, 0, 0x800, loadFileTagged, "C:\\Unit_eff.arc", getCurrentTaskId(), -2);
    EVO_EFFECT_ARCHIVE = (u8 *)func_80014C08(0x7FFFFFFF);
}

void EVO_freeEffectArchive(void) {
    freeHeapBlock(EVO_EFFECT_ARCHIVE);
}

void EVO_playEffect(s32 index, s32 arg) {
    EVO_playEffectScript(index, arg, arg, 0, 0);
}

void func_801EE304(s32 index, s32 arg, s32 arg2) {
    EVO_playEffectScript(index, arg, arg, arg2, arg2);
}

void EVO_playEffectScript(s32 index, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 data;

    EVO_EFFECT_PLAYER = 0;
    EVO_EFFECT_SPRITE_1 = 0;
    EVO_EFFECT_SPRITE_2 = 0;
    data = decompressArchiveEntry((s32)EVO_EFFECT_ARCHIVE, index);
    func_800149B8(0, 0x1F, 0, 0x800, EVO_runEffectScriptTask, data, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    freeHeapBlock((void *)data);
}

s32 EVO_tickEffectScript(EvoEffectScript *loader) {
    s32 i;

    PushMatrix();
    tickEffectMotion((s32)&EVO_EFFECT_ROOT, 0);
    PopMatrix();
    loader->vars[0] = 1;
    EVO_runEffectScript(loader);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].active != 0 && EVO_EFFECT_TICK_FUNCS[loader->entries[i].kind] != NULL) {
            EVO_EFFECT_TICK_FUNCS[loader->entries[i].kind](loader->entries[i].handle);
            if (loader->entries[i].kind > 0) {
                loader->vars[i + 88] = loader->entries[i].handle->state;
                loader->vars[i + 120] = loader->entries[i].handle->flag;
            }
        }
    }
    return loader->vars[0];
}

void EVO_freeEffectEntries(EvoEffectScript *loader) {
    s32 i;

    func_80014C08(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (loader->entries[i].kind != -1) {
            EVO_EFFECT_FREE_FUNCS[loader->entries[i].kind](loader->entries[i].handle);
        }
    }
    func_80014C08(FRAME_INTERVAL);
    if (loader->buffer != NULL) {
        freeHeapBlock(loader->buffer);
    }
}

void EVO_setZonePosition(s32 kind, EvoObject *obj) {
    switch (kind) {
    case 0:
        obj->x = 28;
        obj->y = -13;
        obj->z = 0;
        break;
    case 1:
        obj->x = -106;
        obj->y = -13;
        obj->z = 0;
        break;
    case 2:
        obj->x = -12;
        obj->y = -21;
        obj->z = 0;
        break;
    case 3:
        obj->x = -90;
        obj->y = -21;
        obj->z = 0;
        break;
    case 4:
        obj->x = -59;
        obj->y = -21;
        obj->z = 0;
        break;
    default:
        obj->x = 0;
        obj->y = 0;
        obj->z = 0;
        break;
    }
}

void EVO_setCardSpriteColor(s32 index, EvoLight *light) {
    Color color;

    if (index >= 0) {
        color.r = light->r;
        color.g = light->g;
        color.b = light->b;
        func_801F893C(SPRITE(index), &color);
    }
}

void EVO_runEffectScript(EvoEffectScript *loader) {
    s32 *vars;
    s32 result;
    s32 index;

    if (loader->counter != 0) {
        loader->counter--;
        return;
    }
    vars = loader->vars;
    do {
        result = runScriptToNextEvent(loader->script, vars);
        if (result == 1) {
            switch (loader->script->eventOp) {
            case 10:
                switch (loader->script->eventArg) {
                case 0:
                    EVO_getEffectParams(&EVO_EFFECT_ROOT, (EvoFxParams *)vars, 0);
                    break;
                case 1:
                    EVO_setEffectParams(&EVO_EFFECT_ROOT, (EvoFxParams *)vars);
                    restartEffectMotion((u8 *)&EVO_EFFECT_ROOT);
                    break;
                case 2:
                    D_800794E7 = 1;
                    break;
                case 3:
                    D_800794E7 = 0;
                    break;
                case 4:
                    D_800795A8 = 0;
                    break;
                case 5:
                    D_800795A8 = 1;
                    break;
                case 6:
                    EVO_setZonePosition(EVO_EFFECT_SPRITE_1, (EvoObject *)vars);
                    break;
                case 7:
                    EVO_setZonePosition(EVO_EFFECT_SPRITE_2, (EvoObject *)vars);
                    break;
                case 8:
                    EVO_setCardSpriteColor(EVO_EFFECT_SPRITE_1, (EvoLight *)vars);
                    break;
                case 9:
                    EVO_setCardSpriteColor(EVO_EFFECT_SPRITE_2, (EvoLight *)vars);
                    break;
                case 10:
                    index = EVO_EFFECT_SPRITE_1;
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                case 11:
                    index = EVO_EFFECT_SPRITE_2;
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                case 12:
                    EVO_TRAYS[0].merge = 0;
                    EVO_TRAYS[1].merge = 1;
                    break;
                case 13:
                    EVO_SCREEN_FLASH.on = 1;
                    break;
                case 14:
                    EVO_SCREEN_FLASH.on = 0;
                    break;
                case 15:
                    EVO_TRAYS[0].x = 0x3A;
                    EVO_TRAYS[1].x = -0x58;
                    break;
                }
                break;
            case 11:
                switch (loader->script->eventArg) {
                case 0:
                    playSoundEffect((s16)loader->script->params[0]);
                    break;
                case 1:
                    EVO_getEffectParams(loader->entries[(s16)loader->script->params[0]].handle, (EvoFxParams *)vars, 0);
                    break;
                case 2:
                    EVO_initEffectFromParams(loader->entries[(s16)loader->script->params[0]].handle, vars, loader);
                    initEffectObject(loader->entries[(s16)loader->script->params[0]].handle);
                    break;
                case 3:
                    stopSoundVoice(loader->script->params[0]);
                    break;
                case 4:
                    if (loader->entries[(s16)loader->script->params[0]].handle != NULL) {
                        loader->entries[(s16)loader->script->params[0]].active = 1;
                    }
                    break;
                case 5:
                    loader->entries[(s16)loader->script->params[0]].active = 0;
                    break;
                case 6:
                    loader->counter = (s16)loader->script->params[0] - 1;
                    return;
                case 7:
                    EVO_getEffectWorldPos(loader->entries[(s16)loader->script->params[0]].handle, (EvoObject *)vars);
                    break;
                case 8:
                    EVO_getEffectParams(loader->entries[(s16)loader->script->params[0]].handle, (EvoFxParams *)vars, 1);
                    break;
                case 9:
                    EVO_setZonePosition(getActiveDigimonCard(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 10:
                    EVO_setZonePosition(getPlayedCard(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 11:
                    EVO_setZonePosition(peekOnlineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 12:
                    EVO_setZonePosition(peekOfflineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoObject *)vars);
                    break;
                case 15:
                    EVO_setCardSpriteColor(peekOnlineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoLight *)vars);
                    break;
                case 16:
                    EVO_setCardSpriteColor(peekOfflineDeckTop(EVO_EFFECT_PLAYER ^ (s16)loader->script->params[0]), (EvoLight *)vars);
                    break;
                case 18:
                    printf("NO USE\n");
                    break;
                case 19:
                    index = EVO_EFFECT_SPRITE_1;
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)loader->script->params[0]);
                    }
                    break;
                case 20:
                    index = EVO_EFFECT_SPRITE_2;
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)loader->script->params[0]);
                    }
                    break;
                case 21:
                    EVO_setZonePosition((s16)loader->script->params[0], (EvoObject *)vars);
                    break;
                case 22:
                    loader->buffer = (void *)EVO_loadEffectPak((s16)loader->script->params[0]);
                    break;
                case 23:
                    animateWindowTo(&EVO_WINDOWS[12].win, (Rect16 *)-1);
                    break;
                }
                break;
            case 12:
                switch (loader->script->eventArg) {
                case 6:
                    EVO_createEffectEntry((s16)loader->script->params[0], (s16)loader->script->params[1], vars, loader);
                    break;
                case 1:
                    vars[1] = rsin((s16)loader->script->params[1]) * (s16)loader->script->params[0] / 4096;
                    break;
                case 2:
                    vars[1] = rcos((s16)loader->script->params[1]) * (s16)loader->script->params[0] / 4096;
                    break;
                case 3:
                    playSoundEffectOnVoice((s16)loader->script->params[0], (s16)loader->script->params[1]);
                    break;
                case 5:
                    printf("NO USE\n");
                    break;
                }
                break;
            }
        }
        clearScriptBusy(loader->script);
    } while (result != 0);
}

void EVO_getEffectParams(EvoFx *fx, EvoFxParams *params, s32 current) {
    if (current == 0) {
        params->args[0] = fx->rot.vx;
        params->args[1] = fx->rot.vy;
        params->args[2] = fx->rot.vz;
        params->args[8] = fx->scale.vx;
        params->args[9] = fx->scale.vy;
        params->args[10] = fx->scale.vz;
        params->args[17] = fx->pos.vx;
        params->args[18] = fx->pos.vy;
        params->args[19] = fx->pos.vz;
    } else {
        params->args[0] = fx->curRot.vx;
        params->args[1] = fx->curRot.vy;
        params->args[2] = fx->curRot.vz;
        params->args[8] = fx->curScale.vx;
        params->args[9] = fx->curScale.vy;
        params->args[10] = fx->curScale.vz;
        params->args[17] = fx->curPos[0];
        params->args[18] = fx->curPos[1];
        params->args[19] = fx->curPos[2];
    }
    params->args[3] = fx->rotVel.vx;
    params->args[4] = fx->rotVel.vy;
    params->args[5] = fx->rotVel.vz;
    params->args[6] = fx->moveSpeed;
    params->args[7] = fx->moveAccel;
    params->args[11] = fx->scaleVel.vx;
    params->args[12] = fx->scaleVel.vy;
    params->args[13] = fx->scaleVel.vz;
    params->args[14] = fx->scaleAccel.vx;
    params->args[15] = fx->scaleAccel.vy;
    params->args[16] = fx->scaleAccel.vz;
    params->args[20] = fx->vel.vx;
    params->args[21] = fx->vel.vy;
    params->args[22] = fx->vel.vz;
    params->args[23] = fx->accel.vx;
    params->args[24] = fx->accel.vy;
    params->args[25] = fx->accel.vz;
    params->args[26] = fx->hitRadius;
    params->args[27] = fx->period;
    params->args[28] = fx->fadeMode;
    params->args[29] = fx->speed;
    params->args[31] = fx->wavePhase;
    params->args[32] = fx->waveAmplitude;
    params->args[33] = fx->waveFreq;
    params->args[30] = fx->mode;
}

void EVO_setEffectParams(EvoFx *fx, EvoFxParams *params) {
    fx->rot.vx = params->args[0];
    fx->rot.vy = params->args[1];
    fx->rot.vz = params->args[2];
    fx->rotVel.vx = params->args[3];
    fx->rotVel.vy = params->args[4];
    fx->rotVel.vz = params->args[5];
    fx->moveSpeed = params->args[6];
    fx->moveAccel = params->args[7];
    fx->scale.vx = params->args[8];
    fx->scale.vy = params->args[9];
    fx->scale.vz = params->args[10];
    fx->scaleVel.vx = params->args[11];
    fx->scaleVel.vy = params->args[12];
    fx->scaleVel.vz = params->args[13];
    fx->scaleAccel.vx = params->args[14];
    fx->scaleAccel.vy = params->args[15];
    fx->scaleAccel.vz = params->args[16];
    fx->pos.vx = params->args[17];
    fx->pos.vy = params->args[18];
    fx->pos.vz = params->args[19];
    fx->vel.vx = params->args[20];
    fx->vel.vy = params->args[21];
    fx->vel.vz = params->args[22];
    fx->accel.vx = params->args[23];
    fx->accel.vy = params->args[24];
    fx->accel.vz = params->args[25];
    fx->hitRadius = params->args[26];
    fx->period = params->args[27];
    fx->fadeMode = params->args[28];
    fx->speed = params->args[29];
    fx->wavePhase = params->args[31];
    fx->waveAmplitude = params->args[32];
    fx->waveFreq = params->args[33];
    fx->mode = params->args[30];
}

void EVO_getEffectWorldPos(void *xform, EvoObject *obj) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    obj->x = pos.vx;
    obj->y = pos.vy;
    obj->z = pos.vz;
}

void EVO_initEffectFromParams(EvoFx *fx, s32 *vars, EvoEffectScript *loader) {
    EVO_setEffectParams(fx, (EvoFxParams *)vars);
    if (vars[186] == -2) {
        fx->parent = NULL;
    } else if (vars[186] == -1) {
        fx->parent = &EVO_EFFECT_ROOT;
    } else {
        fx->parent = loader->entries[vars[186]].handle;
    }
}

EvoFx *EVO_createFadeRectFromParams(s32 *vars) {
    Rect16 rect;
    Bytes4 from;
    Bytes4 to;

    rect.x = vars[75];
    rect.y = vars[76];
    rect.w = vars[77];
    rect.h = vars[78];
    from.b[0] = vars[38];
    from.b[1] = vars[39];
    from.b[2] = vars[40];
    to.b[0] = vars[41];
    to.b[1] = vars[42];
    to.b[2] = vars[43];
    return (EvoFx *)EVO_createFadeRect((s16 *)&rect, &from, &to, vars[28], vars[29], vars[69]);
}

EvoFx *EVO_createRingFromParams(s32 *vars, EvoEffectScript *loader) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    EvoFx fx;

    EVO_initEffectFromParams(&fx, vars, loader);
    inner.b[0] = vars[38];
    inner.b[1] = vars[39];
    inner.b[2] = vars[40];
    mid.b[0] = vars[41];
    mid.b[1] = vars[42];
    mid.b[2] = vars[43];
    outer.b[0] = vars[44];
    outer.b[1] = vars[45];
    outer.b[2] = vars[46];
    return (EvoFx *)createRingEffect(vars[56], &inner, &mid, &outer, (EffectTemplate *)&fx, vars[22], vars[27], vars[28],
                                     vars[71], vars[62], vars[63], vars[66], vars[64], vars[65], NULL, 0, 0, 0,
                                     vars[70], vars[73], vars[74], 0);
}

EvoFx *EVO_createEffectObjectFromParams(s32 *vars, EvoEffectScript *loader) {
    EvoFx fx;

    EVO_initEffectFromParams(&fx, vars, loader);
    return (EvoFx *)cloneEffectObject((EffectTemplate *)&fx);
}

EvoFx *EVO_createModelEffectFromParams(s32 *vars, EvoEffectScript *loader) {
    EvoFx fx;
    EvoFx *template = &fx;

    EVO_initEffectFromParams(template, vars, loader);
    return (EvoFx *)EVO_createModelEffect(vars[56], template, vars[84], vars[85], -1, vars[86], vars[70], vars[87], (s32)loader->buffer, 0);
}

EvoFx *EVO_createStreaksFromParams(s32 *vars, EvoEffectScript *loader) {
    EvoFx fx;
    Bytes4 start;
    Bytes4 end;

    start.b[0] = vars[38];
    start.b[1] = vars[39];
    start.b[2] = vars[40];
    end.b[0] = vars[41];
    end.b[1] = vars[42];
    end.b[2] = vars[43];
    EVO_initEffectFromParams(&fx, vars, loader);
    return (EvoFx *)createStreakParticles(start.b, end.b, (EffectTemplate *)&fx, vars[189], vars[188], vars[190], vars[195],
                                          vars[191], vars[192], vars[193], vars[194], vars[80], vars[81], vars[79],
                                          vars[82], vars[27], vars[70], vars[74]);
}

void EVO_freeFadeRect(void *obj) {
    freeHeapBlock(obj);
}

void EVO_createEffectEntry(s32 index, s32 kind, s32 *vars, EvoEffectScript *loader) {
    if (EVO_EFFECT_CREATE_FUNCS[kind] != NULL) {
        loader->entries[index].kind = kind;
        loader->entries[index].active = 0;
        loader->entries[index].handle = EVO_EFFECT_CREATE_FUNCS[kind](vars, loader);
        loader->counter++;
        if ((loader->counter & 0xF) == 0) {
            func_80014C08(FRAME_INTERVAL);
        }
    }
}

EvoEffectScript *EVO_createEffectScript(EvoMsd *data) {
    EvoFx fx;
    EvoEffectScript *loader;
    s32 i;

    loader = allocTaskHeapBlock(sizeof(EvoEffectScript));
    loader->data = data;
    loader->script = createScriptContext(data);
    loader->vars = allocScriptRegisters(0xC4);
    for (i = 0; i < 16; i++) {
        loader->entries[i].kind = -1;
        loader->entries[i].active = 0;
        loader->entries[i].handle = NULL;
    }
    loader->counter = 0;
    loader->buffer = NULL;
    fx.rot.vx = 0;
    fx.rot.vy = 0;
    fx.rot.vz = 0;
    fx.rotVel.vx = 0;
    fx.rotVel.vy = 0;
    fx.rotVel.vz = 0;
    fx.scale.vx = 0;
    fx.scale.vy = 0;
    fx.scale.vz = 0;
    fx.scaleVel.vx = 0;
    fx.scaleVel.vy = 0;
    fx.scaleVel.vz = 0;
    fx.scaleAccel.vx = 0;
    fx.scaleAccel.vy = 0;
    fx.scaleAccel.vz = 0;
    fx.pos.vx = 0x1000;
    fx.pos.vy = 0x1000;
    fx.pos.vz = 0x1000;
    fx.vel.vx = 0x1000;
    fx.vel.vy = 0x1000;
    fx.vel.vz = 0x1000;
    fx.accel.vx = 0;
    fx.accel.vy = 0;
    fx.accel.vz = 0;
    fx.fadeMode = 0;
    fx.speed = 0;
    fx.hitRadius = 0x80;
    fx.parent = SCENE_3D->unk78;
    fx.mode = 0;
    EVO_EFFECT_ROOT = fx;
    initEffectObject(&EVO_EFFECT_ROOT);
    EVO_runEffectScript(loader);
    return loader;
}

void EVO_runEffectScriptTask(EvoMsd *data, s32 parentTask) {
    EvoEffectScript *loader;

    loader = EVO_createEffectScript(data);
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (EVO_tickEffectScript(loader) != 0);
    EVO_freeEffectEntries(loader);
    freeScriptContext(loader->script, loader->vars);
    freeHeapBlock(loader);
    func_80014A48(parentTask);
}

void (*EVO_WINDOW_DRAW_FUNCS[14])() = {
    EVO_drawFusionTypeIcon, EVO_drawFusionTypeTitle, EVO_drawFusionTypeHelp, EVO_drawFusionTypeIcon, EVO_drawFusionTypeTitle, EVO_drawFusionTypeHelp, EVO_drawUnitPortrait,
    EVO_drawMessageWindow, EVO_drawEmptyWindow, EVO_drawEmptyWindow, EVO_drawPartnerList, EVO_drawPartnerStatus, EVO_drawCardInfo, EVO_drawReceivedBanner,
};

typedef void (*EvoFxFunc)(EvoFx *);

void (*EVO_EFFECT_TICK_FUNCS[5])(EvoFx *) = {
    (EvoFxFunc)EVO_tickFadeRect,
    (EvoFxFunc)renderRingEffect,
    (EvoFxFunc)updateEffectObject,
    (EvoFxFunc)renderStreakParticles,
    (EvoFxFunc)EVO_tickModelEffect,
};

EvoFx *(*EVO_EFFECT_CREATE_FUNCS[5])(s32 *, EvoEffectScript *) = {
    (EvoFx * (*)(s32 *, EvoEffectScript *)) EVO_createFadeRectFromParams,
    EVO_createRingFromParams,
    EVO_createEffectObjectFromParams,
    EVO_createStreaksFromParams,
    EVO_createModelEffectFromParams,
};

void (*EVO_EFFECT_FREE_FUNCS[5])(EvoFx *) = {
    (EvoFxFunc)EVO_freeFadeRect,
    (EvoFxFunc)freeRingEffect,
    (EvoFxFunc)freeEffectObject,
    (EvoFxFunc)freeStreakParticles,
    (EvoFxFunc)EVO_freeModelEffect,
};

/* not referenced by any code */
u32 D_801F02BC = 0xF05B2E46;

s8 EVO_BANNER_FADE = 0;
/* not referenced by any code */
u8 D_801F02C4[4] = { 0 };
u16 EVO_BANNER_CLUT[16] = { 0 };
u8 EVO_BANNER_BRIGHTNESS = 0;
/* not referenced by any code */
u8 D_801F02E9 = 0x23;
u8 D_801F02EA = 0x8F;
u8 D_801F02EB = 0x2B;
u8 D_801F02EC[4] = { 0x74, 0x68, 0x7F, 0xC3 };
EvoSpark EVO_SPARKS[16] = { { { 0 } } };
EvoColor EVO_SHARD_COLOR = { 0 };
SVECTOR *EVO_SHARD_VERTS = 0;
/* not referenced by any code */
u8 D_801F053C[4] = { 0 };
s8 *EVO_SHARD_PRIM = 0;
/* not referenced by any code */
u8 D_801F0544[12] = { 0 };
s16 EVO_SHATTER_ORDER[40] = { 0 };
s16 EVO_SHATTER_TIMERS[42] = { 0 };
s8 EVO_CUTSCENE_STEP = 0;
u16 EVO_SHATTER_DELAY = 0;
s8 EVO_SHATTER_STARTED = 0;
EvoShard *EVO_SHARDS = 0;
EvoShatter EVO_SHATTER = { 0 };
/* not referenced by any code */
u8 D_801F062C[0x2584] = { 0 };
u32 EVO_RAND_SEED_LO = 0;
/* not referenced by any code */
u8 D_801F2BB4[4] = { 0 };
SVECTOR *EVO_SHARD_VERTEX_CURSOR = 0;
SVECTOR *EVO_SHARD_VERTEX_POOL = 0;
/* not referenced by any code */
u8 D_801F2BC0[0x3C] = { 0 };
s16 EVO_CUTSCENE_MODELS[3] = { 0 };
/* not referenced by any code */
u8 D_801F2C04[0x177C] = { 0 };
UiWindow EVO_SORT_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F43C4[12] = { 0 };
UiWindow EVO_CARD_LIST_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F4414[12] = { 0 };
CursorHighlight EVO_SORT_CURSOR = { { { 0 } } };
CursorHighlight EVO_CARD_LIST_CURSOR = { { { 0 } } };
EvoCardInfo *EVO_CARD_LIST[301] = { 0 };
/* not referenced by any code */
u8 D_801F4974[12] = { 0 };
EvoCardInfo *EVO_CARDS_BY_ID[301] = { 0 };
u8 *EVO_SPARE_CARD_COUNTS = 0;
/* not referenced by any code */
u8 D_801F4E38[8] = { 0 };
u8 *EVO_DECK_CARD_COUNTS[3] = { 0 };
u8 EVO_SCRIPT_HALTED = 0;
s32 EVO_NEW_DIGI_PART = 0;
/* not referenced by any code */
u8 D_801F4E54[4] = { 0 };
EvoWindow EVO_WINDOWS[14] = { { { 0 } } };
/* not referenced by any code */
u8 D_801F5248[8] = { 0 };
EvoProgram *EVO_SCRIPT = 0;
/* not referenced by any code */
u8 D_801F5254[4] = { 0 };
EvoText EVO_TEXT_LINES[4] = { { { 0 } } };
u8 EVO_MAX_CARD_LEVEL = 0;
/* not referenced by any code */
u8 D_801F535C[4] = { 0 };
u8 EVO_CARD_RECEIVED = 0;
/* not referenced by any code */
u8 D_801F5364[28] = { 0 };
EvoChoice EVO_TYPE_CHOICE = { { 0 } };
/* not referenced by any code */
u8 D_801F53B4[20] = { 0 };
EvoScreenFlash EVO_SCREEN_FLASH = { { { 0 } } };
/* not referenced by any code */
u8 D_801F540C[4] = { 0 };
UiWindow EVO_RANK_UP_WINDOW = { 0 };
/* not referenced by any code */
u8 D_801F5454[4] = { 0 };
s16 EVO_CURSOR_CARD = 0;
/* not referenced by any code */
u8 D_801F545C[28] = { 0 };
EvoFusion EVO_FUSION = { { 0 } };
EvoTray EVO_TRAYS[2] = { { { { { 0 } } } } };
u8 EVO_RANK_UP_STATE = 0;
u8 EVO_LEVEL_UP_PENDING = 0;
/* not referenced by any code */
u8 D_801F57A4[4] = { 0 };
EvoDialog EVO_DIALOG = { { 0 } };
/* not referenced by any code */
u8 D_801F5850[16] = { 0 };
s16 EVO_STAT_BONUSES[4] = { 0 };
EvoFx EVO_EFFECT_ROOT = { { 0 } };
s8 EVO_EFFECT_PLAYER = 0;
s8 EVO_EFFECT_SPRITE_1 = 0;
s8 EVO_EFFECT_SPRITE_2 = 0;
u8 *EVO_EFFECT_ARCHIVE = 0;
