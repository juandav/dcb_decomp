#ifndef DCB_EVOSEG_H
#define DCB_EVOSEG_H

#include "game.h"
#include "dcb/script.h"

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
    s16 r;
    s16 g;
    s16 b;
} EvoColor;

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

typedef struct {
    Rect16 rect;
    u16 src[0x100];
    u16 dst[0x100];
    s16 brighten;
    s16 level;
} EvoClut;

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

typedef struct {
    u8 text[0x3C];
    s16 pos;
    s8 active;
    s8 len;
} EvoText;

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
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} Color;

typedef struct {
    UiWindow win;
    s8 isPartner;
    s8 z;
    u8 pad46[2];
} EvoWindow;

typedef struct {
    Rect16 rect;
    s32 brightness;
    s32 style;
    s32 flags;
    s32 label;
    u8 labelPalette;
} EvoWindowDef;

typedef struct {
    char *name;
    s8 learnLevels[6];
    u8 unkA[2];
} EvoAbilityInfo;

typedef struct {
    u8 pad0[0xA5];
    s8 choice;
} EvoDialog;

typedef struct {
    u8 card;
    u8 ability;
} EvoAbilityReward;

typedef struct {
    u8 pad0[0x20];
    s16 joint;
    s16 timer;
} EvoSpark;

typedef struct {
    s16 timer;
    u16 count;
    SVECTOR *offsets;
    SVECTOR *verts;
    s8 *prims;
    s16 primCount;
    s16 part;
} EvoShard;

typedef struct {
    u8 pad0[0xE];
    s16 unkE;
    u8 pad10[0x33 - 0x10];
    s8 side;
} EvoChoice;

typedef struct {
    s32 count;
    s16 *queue;
    u8 pad8[0x26 - 0x8];
    s8 total;
    s8 next;
    s8 model;
} EvoShatter;

extern MATRIX GsWSMATRIX;
extern EvoCardInfo *EVO_CARDS_BY_ID[];
extern u8 *EVO_SPARE_CARD_COUNTS;
extern u8 *EVO_DECK_CARD_COUNTS[3];
extern EvoFusion EVO_FUSION;
extern EvoTray EVO_TRAYS[2];
extern EvoScreenFlash EVO_SCREEN_FLASH;
extern EvoProgram *EVO_SCRIPT;
extern UiWindow EVO_CARD_LIST_WINDOW;
extern u8 *EVO_EFFECT_ARCHIVE;
extern EvoWindow EVO_WINDOWS[];
extern u8 EVO_MAX_CARD_LEVEL;
extern SVECTOR *EVO_SHARD_VERTEX_POOL;
extern s8 EVO_SHATTER_STARTED;
extern s8 EVO_CUTSCENE_STEP;
extern EvoDialog EVO_DIALOG;
extern u8 EVO_RANK_UP_STATE;
extern s16 EVO_STAT_BONUSES[4];
extern UiWindow EVO_RANK_UP_WINDOW;
extern s16 EVO_CUTSCENE_MODELS[3];
extern EvoText EVO_TEXT_LINES[4];
extern u8 EVO_CARD_RECEIVED;
extern EvoShard *EVO_SHARDS;
extern EvoChoice EVO_TYPE_CHOICE;
extern EvoCardInfo *EVO_CARD_LIST[];
extern UiWindow EVO_SORT_WINDOW;

long GsGetWorkBase(void);
void GsSetWorkBase(long base);

#endif /* DCB_EVOSEG_H */
