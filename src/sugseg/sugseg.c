#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/archive.h"
#include "dcb/loader.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/transform.h"
#include "dcb/script.h"
#include "dcb/vblank.h"
#include "dcb/frame_callback.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/anim_control.h"
#include "dcb/player_data.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim.h"
#include "dcb/stage.h"
#include "dcb/prim_util.h"
#include "dcb/model_anim.h"
#include "dcb/fade.h"
#include "dcb/sound_play.h"
#include "dcb/angle.h"
#include "gte.h"

/* main's createRingEffect, renderRingEffect and freeRingEffect call these three
   through their addresses (func_801E6C78, func_801E7020, func_801E72D4 in game.h) */
s32 SUG_startTexAnim(s32 id, s32 kind, RingEffect *obj, u8 *arg3, s32 pak);
void SUG_tickTexAnim(u8 *arg);
void SUG_freeTexAnim(u8 *obj);

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define setRECT(r, _x, _y, _w, _h) (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)

extern s16 D_800794C0;
extern s32 SUG_SCREEN_FX_X;
extern float SUG_SCREEN_FX_STEP_X;
extern float SUG_SCREEN_FX_STEP_Y;

typedef struct {
    Rect16 rect;
    void *buf0;
    void *buf1;
    s32 vertical;
    s32 depth;
    s16 speed;
    s16 period;
    s16 timer;
} ScrollTex;
s32 StoreImage(Rect16 *rect, void *p);

typedef struct {
    VECTOR pos;
    VECTOR posStep;
    VECTOR color;
    VECTOR colorStep;
    s32 light;
    s32 frame;
    s32 duration;
    s32 period;
} LightMotion;

void SUG_setGridUvsFT4(POLY_FT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink);
void SUG_setGridUvsGT4(POLY_GT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink);

typedef struct {
    u8 *c00;
    u8 *c10;
    u8 *c01;
    u8 *c11;
    s32 wx0;
    s32 wx1;
    s32 wy0;
    s32 wy1;
} Blend;

typedef struct {
    u32 tag;
    u32 code[1];
} DrTPage;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
} LineF2;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
} LineG2;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
} POLY_F3;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} POLY_F4;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, pad1;
    s16 x1, y1;
    u8 r2, g2, b2, pad2;
    s16 x2, y2;
} POLY_G3;
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 cd;
} Rgb;
/* a 3D polygon effect: a fan of `count` triangles around verts[0], an optional
   inner fan, and rings of quads, drawn with one of several primitive kinds */
typedef struct {
    /* 0x000 */ u8 unk0[0x139];
    /* 0x139 */ u8 suspended;
    /* 0x13A */ u8 unk13A[2];
    /* 0x13C */ u8 texAnim[0x20];
    /* 0x15C */ DrTPage *tpages[2];
    /* 0x164 */ POLY_F3 *tris[2];
    /* 0x16C */ POLY_F4 *quads[2];
    /* 0x174 */ POLY_G3 *gtris[2];
    /* 0x17C */ POLY_G4 *gquads[2];
    /* 0x184 */ POLY_GT3 *ttris[2];
    /* 0x18C */ POLY_GT4 *tquads[2];
    /* 0x194 */ LineF2 *lines[2];
    /* 0x19C */ SVECTOR *verts;
    /* 0x1A0 */ u8 rgb[3];
    /* 0x1A3 */ u8 unk1A3;
    /* 0x1A4 */ Rect16 uv;
    /* 0x1AC */ s32 tpage;
    /* 0x1B0 */ s32 clut;
    /* 0x1B4 */ s32 otz;
    /* 0x1B8 */ s32 vertCount;
    /* 0x1BC */ s32 ringVertCount;
    /* 0x1C0 */ s32 lineCount;
    /* 0x1C4 */ s32 ringCount;
    /* 0x1C8 */ s32 abr;
    /* 0x1CC */ s16 segments;
    /* 0x1CE */ s16 slices;
    /* 0x1D0 */ s16 pulse;
    /* 0x1D2 */ s16 pulseMode;
    /* 0x1D4 */ s16 brightness;
    /* 0x1D6 */ s16 prevBrightness;
    /* 0x1D8 */ s16 texAnimActive;
    /* 0x1DA */ u8 semiTrans;
    /* 0x1DB */ u8 kind;
    /* 0x1DC */ u8 openBottom;
    /* 0x1DD */ u8 cull;
} SphereEffect;
void SUG_drawSphereLines(SphereEffect *fx, u8 a1, s32 n, s32 a3, s32 otz);
void SUG_drawSphereF(SphereEffect *fx, s32 cull, s32 count, s32 speed, s32 otz);
void SUG_drawSphereG(SphereEffect *fx, s32 cull, s32 count, s32 speed, s32 otz);
void SUG_drawSphereGT(SphereEffect *fx, s32 cull, s32 count, s32 speed, s32 otz);

typedef struct {
    u8 unk0[0x5C];
    s32 unk5C;
    s32 unk60;
    s32 unk64;
    s32 unk68;
    s32 unk6C;
    s32 texMode;
    s32 unk74;
    s32 abr;
    u8 unk7C[0x28];
    s32 r;
    s32 g;
    s32 b;
    s32 midR;
    s32 midG;
    s32 midB;
    s32 outerR;
    s32 outerG;
    s32 outerB;
    s32 lastR;
    s32 lastG;
    s32 lastB;
    u8 unkD4[0x18];
    s32 unkEC;
    u8 unkF0[8];
    s32 unkF8;
    u8 unkFC[8];
    s32 innerRadius;
    s32 outerRadius;
    s32 innerZ;
    s32 outerZ;
    s32 midPercent;
    s32 unk118;
    u8 unk11C[4];
    s32 unk120;
    u8 unk124[4];
    s32 u1;
    s32 unk12C;
    s32 unk130;
    u8 unk134[4];
    s32 unk138;
    s32 unk13C;
    u8 unk140[0x10];
    s32 texX;
    s32 texY;
    s32 texW;
    s32 texH;
    s32 clutX;
    s32 clutY;
    s32 unk168;
    s32 unk16C;
} SpriteCommand;
u16 func_80067644(s32 x, s32 y);
SphereEffect *SUG_createSphereEffect(s16 a0, u8 *color, s16 a2, s16 a3, EffectTemplate *template, s16 a5, s16 a6, s32 a7, u8 a8, u8 a9, u8 a10, u8 a11, s16 a12, Rect16 *uv, s32 tpage, s32 clut, u8 a16, s32 a17, s32 a18);

typedef struct {
    s32 duration;
    u8 u0;
    u8 u1;
    u8 v0;
    u8 v1;
    s16 w;
    s16 h;
    s16 attr;
    u8 shade;
    u8 clutRow;
    u8 unk10[2];
    s8 originX;
    s8 originY;
} SpriteFrame;
/* the header of a sprite's data (Sprite.tex); its frames follow at 0x10 */
typedef struct {
    s32 frameCount;
    s8 loopFrame;
    u8 unk5;
    u16 mode;
    u16 clutX;
    u16 clutY;
    u16 x;
    u16 y;
} SpriteSheet;
typedef struct {
    u8 *tex;
    SpriteFrame *frames;
    POLY_FT4 polys[2];
    SVECTOR pos;
    SVECTOR v[4];
    u8 unk80;
    u8 otz;
    s8 frame;
    s8 frameTimer;
    u16 scaleX;
    u16 scaleY;
    s8 flipX;
    s8 flipY;
    u8 unk8A;
    s8 useOrigin;
} Sprite;
void SUG_initSprite(Sprite *sprite, s32 key, u16 scaleX, u16 scaleY, s16 x, s16 y, s16 z, s32 a7, s32 useOrigin, s32 subKey);
typedef struct {
    u32 turn : 1;
    u32 flag1 : 1;
    u32 counter : 1;
    u32 flag3 : 1;
    u32 unk4 : 28;
} BattleFlags;
typedef struct {
    s32 hp;
    s32 damage;
    u32 attack : 2;
    u32 element : 2;
    u32 eatUpHp : 1;
    u32 crash : 1;
    u32 unk8_6 : 1;
    u32 unk8_7 : 25;
} BattlerState;
typedef struct {
    BattlerState players[2];
    union {
        u32 word;
        BattleFlags bits;
    } flags;
} BattleState;
extern u16 SUG_HUD_TPAGE;
extern BattleState *SUG_BATTLE;
void SUG_drawNumber(s32 x, s32 y, s32 value, u8 brightness);
extern s16 SUG_TARGET_HP[2];
void SUG_showEatUpHpBanner(void);
void SUG_animateHpCounter(s32 a0);

typedef struct {
    u8 unk0[6];
    s16 id;
    u8 unk8[8];
    s32 x;
    u8 unk14[0xA64];
    s16 unkA78;
    s16 rotY;
    u8 unkA7C[0x1784];
    s32 unk2200;
    u8 unk2204[4];
    s32 unk2208;
    u8 unk220C[0xA4];
    u8 unk22B0[0x20][0x20];
    u8 unk26B0[0x24];
    s32 unk26D4;
    u8 unk26D8[8];
    void *owner;
    u8 unk26E4[8];
    Rect16 clutRect;
    u8 unk26F4[4];
    u16 clut[256];
} ModelData;
typedef struct {
    Rect16 rect;
    u16 clut[256];
    u16 faded[256];
    s16 brighten;
    s16 level;
} ClutFade;
typedef struct {
    u8 unk0[0x20];
    s32 livePos[3];
    u8 unk2C[4];
    s16 liveRot[3];
    u8 unk36[2];
    s32 liveScale[3];
    u8 unk44[0x54];
    void *parent;
    u8 unk9C[0x10];
    s32 scale[3];
    u8 unkB8[4];
    s32 scale2[3];
    u8 unkC8[4];
    s16 scaleStep[3];
    u8 unkD2[2];
    s16 pos[3];
    u8 unkDA[2];
    s16 pos2[3];
    u8 unkE2[2];
    s16 rot[3];
    u8 unkEA[2];
    s16 rot2[3];
    u8 unkF2[2];
    s16 rotAccel[3];
    u8 unkFA[0x26];
    s16 moveSpeed;
    s16 moveAccel;
    s16 period;
    s16 waveFreq;
    s16 wavePhase;
    s16 waveAmplitude;
    s16 hitRadius;
    s16 mode;
    s16 speed;
    u8 unk132[5];
    u8 fadeMode;
    u8 unk138[2];
    s8 target;
    s8 source;
} EffectInit;
typedef struct {
    u8 unk0[0x65C];
    s32 pos[3];
    s32 pos2[3];
    s32 moveSpeed;
    s32 moveAccel;
    s32 rot[3];
    s32 rot2[3];
    s32 rotAccel[3];
    s32 scale[3];
    s32 scale2[3];
    s32 scaleStep[3];
    s32 hitRadius;
    s32 period;
    s32 fadeMode;
    s32 speed;
    s32 mode;
    s32 wavePhase;
    s32 waveAmplitude;
    s32 waveFreq;
    s32 target;
    u8 unk6E8[0x10];
    s32 source;
} EffectCommand;
typedef struct {
    s32 v[4];
} Entry16;
void *SUG_createModelEffect(s16 brightness, EffectTemplate *template, s32 modelId, s32 anim, s32 a4, s32 vramSlot, u8 a6, s32 a7, s32 pak, s32 a9);
void *SUG_createSpriteEffect(s32 brightness, EffectTemplate *template, s32 key, s32 flipX, s32 flipY, s32 a5, s32 useOrigin, s32 a7, s32 subKey);
typedef struct {
    u8 unk0[0x74];
    s32 semi;
    u8 unk78[0x2C];
    s32 fromR;
    s32 fromG;
    s32 fromB;
    s32 toR;
    s32 toG;
    s32 toB;
    u8 unkBC[0x6C];
    s32 flags;
    u8 unk12C[0x10];
    s32 fixedOtz;
    u8 unk140[0x50];
    s32 pattern;
    s32 zOffset;
    s32 spin;
    s32 kind;
    u8 unk1A0[0x55C];
    s32 spreadY;
    s32 spreadX;
    s32 length;
    s32 frames;
    s32 speedRange;
    s32 reverse;
    s32 count;
    s32 endLength;
} StreakCommand;
void SUG_tickScreenCopyEffect(void);

typedef struct {
    u8 unk0[0x139];
    u8 suspended;
    u8 unk13A[2];
    ModelData *model;
    u8 texAnim[0x20];
    ClutFade fade;
    s16 lastBrightness;
    s8 modelSlot;
    u8 unk56F;
    s8 texAnimActive;
    s8 active;
    s32 unk574;
} ModelEffect;
extern MATRIX SUG_DEFAULT_LIGHT_MATRIX;
extern MATRIX SUG_DEFAULT_LIGHT_COLORS;
typedef struct {
    u8 unk0[0x130];
    u32 animId;
    u8 unk134[0x1C];
    s32 pixelX;
    s32 pixelY;
    u8 unk158[8];
    s32 clutX;
    s32 clutY;
} Unk801E8168;
typedef struct {
    u8 unk0[0x98];
    void *parent;
    u8 unk9C[0x10];
    s32 scale[3];
    u8 unkB8[4];
    s32 scale2[3];
    u8 unkC8[4];
    s16 scaleStep[3];
    u8 unkD2[2];
    s16 pos[3];
    u8 unkDA[2];
    s16 pos2[3];
    u8 unkE2[2];
    s16 rot[3];
    u8 unkEA[2];
    s16 rot2[3];
    u8 unkF2[2];
    s16 rotAccel[3];
    u8 unkFA[0x32];
    s16 hitRadius;
    s16 mode;
    s16 speed;
    u8 unk132[5];
    u8 fadeMode;
    u8 unk138[4];
} RootEffect;
extern RootEffect SUG_EFFECT_ROOT;
typedef struct {
    u8 unk0[0x65C];
    s32 unk65C[3];
    s32 unk668[3];
    u8 unk674[8];
    s32 unk67C;
    s32 unk680;
    s32 unk684;
    u8 unk688[0x18];
    s32 unk6A0[3];
    s32 unk6AC[3];
} Unk801E9FE0;
LightMotion *SUG_createLightMotion(VECTOR *pos, VECTOR *posTo, VECTOR *color, VECTOR *colorTo, s32 light, s32 period, s32 duration);

typedef struct {
    s32 v[4];
} Quad;
typedef struct {
    s16 v[4];
} Short4;
typedef struct {
    Quad *quads;
    Short4 *shorts;
    s32 head;
    s32 last;
    s32 count;
    s32 rows;
} PosHistory;
typedef struct {
    s32 *words;
    s16 *halves;
    s32 head;
    s32 last;
    s32 count;
    s32 rows;
} ValueHistory;
void SUG_uploadShadedClut(ClutFade *fade, u16 stp);
void SUG_setStageBrightness(u8 level);
typedef struct {
    u8 unk0[0x139];
    u8 suspended;
    u8 unk13A[2];
    Sprite sprite;
    s32 brightness;
    s32 active;
} SpriteEffect;
extern MATRIX D_801DBEC0;
void SUG_drawSprite(Sprite *sprite, s16 brightness);
typedef struct {
    u8 unk0[0x20];
    VECTOR pos;
    SVECTOR rot;
    s32 sx;
    u8 unk3C[0xF2];
    s16 mode;
    s16 speed;
    s16 brightness;
    u8 unk134[3];
    u8 fadeMode;
    u8 fadeState;
    u8 suspended;
    u8 unk13A[2];
    u8 texAnim[0x20];
    u8 *colors;
    DrTPage *tpages[2];
    LineG2 *lines[2];
    POLY_G4 *g4s[2];
    POLY_FT4 *ft4s[2];
    POLY_GT4 *gt4s[2];
    u8 edges[2][0x4C];
    PosHistory *histories[2];
    Short4 lastPos[2];
    VECTOR prevPos[2];
    SVECTOR prevRot[2];
    u8 xform[0x4C];
    Rect16 uv;
    s32 tpage;
    s32 clut;
    s32 otz;
    s32 count;
    s32 blend;
    s32 texAnimId;
    s16 level;
    s16 prevLevel;
    u8 semiTrans;
    u8 primKind;
    u8 followMode;
    u8 colorMode;
    u8 primeCount;
} TrailEffect;
TrailEffect *SUG_createTrailEffect(s16 a0, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, EffectTemplate *template, s16 x0, s16 x1,
                           s32 count, s16 rows, u8 a10, u8 a11, u8 a12, u8 a13, u8 a14, s32 id, Rect16 *uv, s32 tpage, s32 clut,
                           s32 a19, s32 a20);
void SUG_setTrailColors(TrailEffect *obj, u8 kind, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3);
void SUG_initTrailPrims(TrailEffect *obj, u8 semiTrans, u8 blend, u8 kind, u8 a4, Rect16 *uv, s32 tpage, s32 clut);
void SUG_freePosHistory(void **obj);
void SUG_freeTexAnim(u8 *obj);
void SUG_fillGradientColors(void *a0, s32 a1, s32 a2, s32 a3, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, Bytes4 *a8);
typedef struct {
    s32 key;
    void *data;
} TamEntry;
extern TamEntry SUG_TAM_CACHE[8];
typedef struct {
    s16 duration;
    u8 u;
    u8 v;
} AnimFrame;
typedef struct {
    u8 count;
    u8 loop;
    u8 w;
    u8 h;
} AnimHeader;
typedef struct {
    Rect16 rect;
    s16 *dst;
    AnimHeader *header;
    AnimFrame *frames;
    s16 frame;
    s16 timer;
    s32 type;
    u8 *pixels;
} TexAnim;
s32 LoadImage2(Rect16 *rect, u8 *pixels);
s32 MoveImage2(Rect16 *rect, s32 x, s32 y);
void SUG_scrollTexAnimLeft(TexAnim *image);
void SUG_scrollTexAnimRight(TexAnim *image);
void SUG_scrollTexAnimUp(TexAnim *image);
void SUG_scrollTexAnimDown(TexAnim *image);
typedef struct {
    s16 kind;
    u8 unk2[2];
    u8 *source;
} Unk801E7BEC;
typedef struct {
    u8 unk0[0x13C];
    s32 value;
} Unk801E7BEC_Dst;
typedef struct {
    s16 kind;
    u8 unk2[2];
    u8 *target;
} Unk801E7C94;
typedef union {
    s32 w;
    u8 b;
    s16 h;
} Value;
typedef struct {
    u8 unk0[0x13C];
    Value value;
} Unk801E7C94_Src;
typedef struct {
    s16 id;
    s16 active;
    s32 value;
} EffectSlot;
typedef struct {
    EffectSlot slots[150];
    s32 modelSlots[3];
    u8 xform[0x4C];
    s32 pak;
    s32 count;
} EffectSlots;
typedef struct {
    void *script;
    void *context;
    s32 *regs;
    EffectSlots *slots;
    s32 waitFrames;
    s32 unk14;
} EffectScript;
EffectScript *SUG_createEffectScript(void *script, s32 side, s32 a2, s32 *state);
s32 SUG_tickEffectScript(EffectScript *runner);
void SUG_runEffectScript(EffectScript *runner);
void SUG_freeEffectEntries(EffectSlots *slots);
typedef struct {
    u8 r;
    u8 g;
    u8 b;
    u8 cd;
} Color;
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    u32 xy[4];
} PolyF4;
typedef struct {
    u32 tpage[2][2];
    PolyF4 poly[2];
    Color color;
    Color color2;
    s16 step;
    u8 visible;
    u8 mode;
} ColorQuad;
ColorQuad *SUG_createFadeRect(Rect16 *rect, Color *color, Color *color2, u8 blend, s16 a4, u8 mode);
ScrollTex *SUG_createScrollTexture(Rect16 *rect, s32 depth, s32 mode, s16 speed);
void SUG_initEffectFromParams(EffectTemplate *template, EffectCommand *cmd, EffectSlots *ctx);
void SUG_freeEffectSlots(void *obj);
extern s32 SUG_SCREEN_FX_FRAME;
extern u8 SUG_SCREEN_FX_BASE_RGB[3];
extern u8 SUG_SCREEN_FX_RGB_STEP[3];
/* takes one argument, but SUG_stepScreenCopyFade calls it without setting it */
void SUG_moveScreenCopy();
extern s32 SUG_SCREEN_FX_Y;
extern s16 SUG_SCREEN_FX_ANGLE;
extern s32 SUG_SCREEN_FX_MOTION;
extern s32 SUG_SCREEN_FX_PHASE;
extern s32 SUG_SCREEN_FX_HOLD;
extern s32 SUG_SCREEN_FX_FADE_TIME;
void SUG_startScreenCopyFade(u8 r, u8 g, u8 b, s32 a3, s32 a4);
typedef struct {
    s32 key;
    u8 *data;
    s32 subKey;
} SpriteEntry;
extern SpriteEntry *SUG_SPRITE_CACHE;
#define setEntry(e, _key, _data, _subKey) \
    do {                                  \
        (e)->key = (_key);                \
        (e)->data = (_data);              \
        (e)->subKey = (_subKey);          \
    } while (0)
extern const char SUG_FMT_SPRITE_PATH[];
void SUG_loadSprite(s32 id, s32 x, s32 y, s32 subKey);
typedef struct {
    float pos;
    s16 target[2];
    s16 trail[6];
    s16 flags;
    s16 brightness;
    Rect16 uv;
} HudSlide;
void SUG_fillShorts(s16 *dst, s32 count, s16 value);
s32 SUG_tickHudSlides(HudSlide *obj, s32 count, s32 state, s32 speed);
void SUG_drawHudSpriteTrail(s32 x, s32 y, Rect16 *uv, u16 tpage, s32 clut, s32 otz, u8 brightness, s8 blend, s16 *trail, s32 count);
extern s16 SUG_CURRENT_SCRIPT;
extern s32 SUG_SCRIPT_STATES[2];
void SUG_initSpriteCache(void);
void SUG_initTamCache(void);
void SUG_placeBattleModels(void);
void SUG_initEffectRoot(void);
void SUG_freeSprites(void);
void SUG_freeTamCache(void);
void SUG_resetStageBrightness(void);
extern s16 D_80079584;
void SUG_orbitCamera(void);
void SUG_showAttackBanner(s32 a0);

PosHistory *SUG_createPosHistory(s32 columns, s32 rows) {
    PosHistory *table;
    s32 count;
    s32 i;

    table = allocTaskHeapBlock(sizeof(PosHistory));
    count = columns * rows;
    table->quads = allocTaskHeapBlock(count * sizeof(Quad));
    table->shorts = allocTaskHeapBlock(count * sizeof(Short4));
    table->count = count;
    table->rows = rows;
    table->head = 0;
    table->last = count - 1;
    for (i = 0; i < count; i++) {
        table->quads[i].v[0] = 0;
        table->quads[i].v[1] = 0;
        table->quads[i].v[2] = 0;
        table->shorts[i].v[0] = 0;
        table->shorts[i].v[1] = 0;
        table->shorts[i].v[2] = 0;
    }
    return table;
}

void SUG_fillPosHistory(PosHistory *table, Quad *quad, Short4 *s4) {
    s32 i;

    for (i = 0; i < table->count; i++) {
        table->quads[i] = *quad;
        table->shorts[i] = *s4;
    }
}

void SUG_pushPosHistory(PosHistory *table, Quad *quad, Short4 *s4) {
    if (quad != NULL) {
        table->quads[table->head] = *quad;
    }
    if (s4 != NULL) {
        table->shorts[table->head] = *s4;
    }
    table->last = table->head;
    table->head = (table->head + 1) % table->count;
}

void SUG_getPosHistory(PosHistory *table, s32 back, Quad *quad, Short4 *s4) {
    s32 i;

    if (back == 0) {
        if (quad != NULL) {
            *quad = table->quads[table->last];
        }
        if (s4 != NULL) {
            *s4 = table->shorts[table->last];
        }
    } else {
        i = table->head;
        i -= back * table->rows;
        if (i < 0) {
            i += table->count;
        }
        if (quad != NULL) {
            *quad = table->quads[i];
        }
        if (s4 != NULL) {
            *s4 = table->shorts[i];
        }
    }
}

void SUG_freePosHistory(void **obj) {
    obj[0] = (void *)freeHeapBlock(obj[0]);
    obj[1] = (void *)freeHeapBlock(obj[1]);
    freeHeapBlock(obj);
}

ValueHistory *SUG_createValueHistory(s32 columns, s32 rows) {
    ValueHistory *table;
    s32 count;
    s32 i;

    table = allocTaskHeapBlock(sizeof(ValueHistory));
    count = columns * rows;
    table->words = allocTaskHeapBlock(count * 4);
    table->halves = allocTaskHeapBlock(count * 2);
    table->count = count;
    table->rows = rows;
    table->head = 0;
    table->last = count - 1;
    for (i = 0; i < count; i++) {
        table->words[i] = 0;
        table->halves[i] = 0;
    }
    return table;
}

void SUG_fillValueHistory(ValueHistory *table, s32 word, s16 half) {
    s32 i;

    for (i = 0; i < table->count; i++) {
        table->words[i] = word;
        table->halves[i] = half;
    }
}

void SUG_pushValueHistory(ValueHistory *table, s32 *word, s16 *half) {
    if (word != NULL) {
        table->words[table->head] = *word;
    }
    if (half != NULL) {
        table->halves[table->head] = *half;
    }
    table->last = table->head;
    table->head = (table->head + 1) % table->count;
}

void SUG_getValueHistory(ValueHistory *table, s32 back, s32 *word, s16 *half) {
    s32 i;

    if (back == 0) {
        if (word != NULL) {
            *word = table->words[table->last];
        }
        if (half != NULL) {
            *half = table->halves[table->last];
        }
    } else {
        i = table->head;
        i -= back * table->rows;
        if (i < 0) {
            i += table->count;
        }
        if (word != NULL) {
            *word = table->words[i];
        }
        if (half != NULL) {
            *half = table->halves[i];
        }
    }
}

void SUG_freeValueHistory(void **obj) {
    obj[0] = (void *)freeHeapBlock(obj[0]);
    obj[1] = (void *)freeHeapBlock(obj[1]);
    freeHeapBlock(obj);
}

void SUG_setSphereColor(u8 *object, Bytes4 *src, s16 x, s16 y);
/* called with one argument more than their definitions take (a trailing 1) */
void initLineF2Pair();
void initPolyF3Pair();
void initPolyF4Pair();
void initPolyG3Pair();
void initPolyGT3Pair();

SphereEffect *SUG_createSphereEffect(s16 a0, u8 *color, s16 a2, s16 a3, EffectTemplate *template, s16 a5, s16 a6, s32 a7, u8 a8,
                           u8 a9, u8 a10, u8 a11, s16 a12, Rect16 *uv, s32 tpage, s32 clut, u8 a16, s32 a17, s32 a18) {
    SphereEffect *fx;
    s32 rings;
    s32 total;
    s32 i;
    s32 n;
    s32 j;
    s32 k;
    s32 r;
    s32 angStep;
    s32 latStep;
    s32 ringStep;
    LineF2 *line0;
    LineF2 *line1;
    POLY_F3 *tri0;
    POLY_F3 *tri1;
    POLY_F4 *quad0;
    POLY_F4 *quad1;
    POLY_G3 *gtri0;
    POLY_G3 *gtri1;
    POLY_G4 *gquad0;
    POLY_G4 *gquad1;
    POLY_GT3 *ttri0;
    POLY_GT3 *ttri1;
    POLY_GT4 *tquad0;
    POLY_GT4 *tquad1;
    DrTPage *tp0;
    DrTPage *tp1;

    fx = allocTaskHeapBlock(0x1E0);
    fx->segments = a5;
    fx->slices = a6;
    rings = (a6 - 3) / 2;
    fx->ringCount = rings + 1;
    total = (rings + 1) * a5;
    fx->vertCount = total + 2;
    fx->ringVertCount = (rings + 2) * a5;
    fx->lineCount = total + rings * a5 + a5 * 2;
    if (a12 >= 0 && SUG_startTexAnim(a12, 3, (RingEffect *)fx, fx->texAnim, a18) != 0) {
        fx->texAnimActive = 1;
    } else {
        fx->texAnimActive = -1;
    }
    fx->verts = allocTaskHeapBlock(fx->vertCount * sizeof(SVECTOR));
    fx->kind = a10;
    fx->semiTrans = a8;
    fx->openBottom = a11;
    fx->abr = a9;
    fx->brightness = a0;
    SUG_setSphereColor((u8 *)fx, (Bytes4 *)color, a2, a3);
    fx->cull = a16;
    fx->otz = a17;
    *(EffectTemplate *)fx = *template;
    initEffectObject(fx);
    for (i = 0; i < 2; i++) {
        fx->tris[i] = NULL;
        fx->quads[i] = NULL;
        fx->gtris[i] = NULL;
        fx->gquads[i] = NULL;
        fx->ttris[i] = NULL;
        fx->tquads[i] = NULL;
        fx->lines[i] = NULL;
        fx->tpages[i] = NULL;
    }
    if (a11 != 0) {
        fx->ringVertCount /= 2;
    }
    switch (a10) {
    case 0:
        for (i = 0; i < 2; i++) {
            fx->lines[i] = allocTaskHeapBlock(fx->lineCount * sizeof(LineF2));
        }
        if (a8 != 0) {
            for (i = 0; i < 2; i++) {
                fx->tpages[i] = allocTaskHeapBlock(fx->lineCount * sizeof(DrTPage));
            }
        }
        line0 = fx->lines[0];
        line1 = fx->lines[1];
        tp0 = fx->tpages[0];
        tp1 = fx->tpages[1];
        for (i = 0; i < fx->lineCount; i++, line0++, line1++, tp0++, tp1++) {
            if (a8 == 0) {
                initLineF2Pair(line0, line1, 0, a9, 0, 0, 0, 1);
            } else {
                initLineF2Pair(line0, line1, 0, a9, tp0, tp1, 1, 1);
            }
        }
        break;
    case 8:
        if (a8 != 0) {
            for (i = 0; i < 2; i++) {
                fx->tpages[i] = allocTaskHeapBlock(fx->ringVertCount * sizeof(DrTPage));
            }
        }
        tp0 = fx->tpages[0];
        tp1 = fx->tpages[1];
        n = a5;
        if (a11 == 0) {
            n *= 2;
        }
        for (i = 0; i < 2; i++) {
            fx->tris[i] = allocTaskHeapBlock(n * sizeof(POLY_F3));
        }
        tri0 = fx->tris[0];
        tri1 = fx->tris[1];
        for (i = 0; i < n; i++, tri0++, tri1++, tp0++, tp1++) {
            if (a8 == 0) {
                initPolyF3Pair(tri0, tri1, 0, a9, 0, 0, 0, 1);
            } else {
                initPolyF3Pair(tri0, tri1, 0, a9, tp0, tp1, 1, 1);
            }
        }
        if (fx->ringVertCount - n > 0) {
            for (i = 0; i < 2; i++) {
                fx->quads[i] = allocTaskHeapBlock((fx->ringVertCount - n) * sizeof(POLY_F4));
            }
            quad0 = fx->quads[0];
            quad1 = fx->quads[1];
            for (i = 0; i < fx->ringVertCount - n; i++, quad0++, quad1++, tp0++, tp1++) {
                if (a8 == 0) {
                    initPolyF4Pair(quad0, quad1, 0, a9, 0, 0, 0, 0, 1);
                } else {
                    initPolyF4Pair(quad0, quad1, 0, a9, tp0, tp1, 0, 1, 1);
                }
            }
        }
        break;
    case 9:
        if (a8 != 0) {
            for (i = 0; i < 2; i++) {
                fx->tpages[i] = allocTaskHeapBlock(fx->ringVertCount * sizeof(DrTPage));
            }
        }
        tp0 = fx->tpages[0];
        tp1 = fx->tpages[1];
        n = a5;
        if (a11 == 0) {
            n *= 2;
        }
        for (i = 0; i < 2; i++) {
            fx->gtris[i] = allocTaskHeapBlock(n * sizeof(POLY_G3));
        }
        gtri0 = fx->gtris[0];
        gtri1 = fx->gtris[1];
        for (i = 0; i < n; i++, gtri0++, gtri1++, tp0++, tp1++) {
            if (a8 == 0) {
                initPolyG3Pair(gtri0, gtri1, 0, 0, 0, a9, 0, 0, 0, 1);
            } else {
                initPolyG3Pair(gtri0, gtri1, 0, 0, 0, a9, tp0, tp1, 1, 1);
            }
        }
        if (fx->ringVertCount - n > 0) {
            for (i = 0; i < 2; i++) {
                fx->gquads[i] = allocTaskHeapBlock((fx->ringVertCount - n) * sizeof(POLY_G4));
            }
            gquad0 = fx->gquads[0];
            gquad1 = fx->gquads[1];
            for (i = 0; i < fx->ringVertCount - n; i++, gquad0++, gquad1++, tp0++, tp1++) {
                if (a8 == 0) {
                    initPolyG4Pair(gquad0, gquad1, 0, 0, 0, 0, a9, 0, 0, 0, 0, 1);
                } else {
                    initPolyG4Pair(gquad0, gquad1, 0, 0, 0, 0, a9, tp0, tp1, 0, 1, 1);
                }
            }
        }
        break;
    case 13:
        n = a5;
        if (a11 == 0) {
            n *= 2;
        }
        for (i = 0; i < 2; i++) {
            fx->ttris[i] = allocTaskHeapBlock(n * sizeof(POLY_GT3));
        }
        ttri0 = fx->ttris[0];
        ttri1 = fx->ttris[1];
        for (i = 0; i < n; i++, ttri0++, ttri1++) {
            if (a8 == 0) {
                initPolyGT3Pair(ttri0, ttri1, 0, 0, 0, tpage, clut, 0, 0, 0, 1);
            } else {
                initPolyGT3Pair(ttri0, ttri1, 0, 0, 0, tpage, clut, 0, 0, 1, 1);
            }
            if (i / a5 == 0) {
                ttri0->u0 = uv->x + uv->w;
                ttri0->v0 = uv->y + uv->h;
                ttri0->u1 = uv->x;
                ttri0->v1 = uv->y + uv->h;
                ttri0->u2 = uv->w / 2 + uv->x;
                ttri0->v2 = uv->y;
                ttri1->u0 = uv->x + uv->w;
                ttri1->v0 = uv->y + uv->h;
                ttri1->u1 = uv->x;
                ttri1->v1 = uv->y + uv->h;
                ttri1->u2 = uv->w / 2 + uv->x;
                ttri1->v2 = uv->y;
            } else {
                ttri0->u0 = uv->w / 2 + uv->x;
                ttri0->v0 = uv->y + uv->h;
                ttri0->u1 = uv->x;
                ttri0->v1 = uv->y;
                ttri0->u2 = uv->x + uv->w;
                ttri0->v2 = uv->y;
                ttri1->u0 = uv->w / 2 + uv->x;
                ttri1->v0 = uv->y + uv->h;
                ttri1->u1 = uv->x;
                ttri1->v1 = uv->y;
                ttri1->u2 = uv->x + uv->w;
                ttri1->v2 = uv->y;
            }
        }
        if (fx->ringVertCount - n > 0) {
            for (i = 0; i < 2; i++) {
                fx->tquads[i] = allocTaskHeapBlock((fx->ringVertCount - n) * sizeof(POLY_GT4));
            }
            tquad0 = fx->tquads[0];
            tquad1 = fx->tquads[1];
            for (i = 0; i < fx->ringVertCount - n; i++, tquad0++, tquad1++) {
                if (a8 == 0) {
                    initPolyGT4Pair(tquad0, tquad1, 0, 0, 0, 0, tpage, clut, uv, 0, 0, 1);
                } else {
                    initPolyGT4Pair(tquad0, tquad1, 0, 0, 0, 0, tpage, clut, uv, 0, 1, 1);
                }
            }
        }
        fx->uv = *uv;
        fx->tpage = tpage;
        fx->clut = clut;
        break;
    }
    angStep = 0x1000 / a5;
    latStep = 0x1000 / a6;
    ringStep = 0x800 / (fx->ringCount + 1);
    fx->verts[0].vx = 0;
    fx->verts[0].vy = -a7;
    fx->verts[0].vz = 0;
    fx->verts[fx->vertCount - 1].vx = 0;
    fx->verts[fx->vertCount - 1].vy = a7;
    fx->verts[fx->vertCount - 1].vz = 0;
    k = 1;
    for (n = 0; n < fx->ringCount; n++) {
        r = a7 * rsin(ringStep * (n + 1)) >> 12;
        for (j = 0; j < a5; j++, k++) {
            fx->verts[k].vx = r * rcos(angStep * j) >> 12;
            fx->verts[k].vz = r * rsin(angStep * j) >> 12;
            fx->verts[k].vy = -(a7 * rcos(latStep * (n + 1))) >> 12;
        }
    }
    return fx;
}

void SUG_setSphereColor(u8 *object, Bytes4 *src, s16 x, s16 y) {
    *(Bytes4 *)(object + 0x1A0) = *src;
    *(s16 *)(object + 0x1D0) = x;
    *(s16 *)(object + 0x1D2) = y;
    *(s16 *)(object + 0x1D6) = -1;
}

void SUG_tickSphereEffect(SphereEffect *fx) {
    s32 speed;

    if (fx->suspended != 0) {
        tickEffectStartDelay(fx);
        return;
    }
    PushMatrix();
    tickEffectMotion((s32)fx, 0);
    if ((fx->brightness = updateEffectBrightness(fx, fx->brightness)) == 0) {
        PopMatrix();
        return;
    }
    switch (fx->kind) {
    case 0:
        if (fx->pulseMode == 0) {
            speed = 0x800 / fx->ringCount;
        } else {
            speed = 0x400 / fx->ringCount;
        }
        SUG_drawSphereLines(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    case 8:
        if (fx->pulseMode == 0) {
            speed = 0x800 / fx->ringCount;
        } else {
            speed = 0x400 / fx->ringCount;
        }
        SUG_drawSphereF(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    case 9:
        if (fx->pulseMode == 0) {
            speed = 0x800 / (fx->ringCount + 1);
        } else {
            speed = 0x400 / (fx->ringCount + 1);
        }
        SUG_drawSphereG(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    case 13:
        if (fx->pulseMode == 0) {
            speed = 0x800 / (fx->ringCount + 1);
        } else {
            speed = 0x400 / (fx->ringCount + 1);
        }
        SUG_drawSphereGT(fx, fx->cull, fx->segments, speed, fx->otz);
        break;
    }
    PopMatrix();
}

void SUG_drawSphereLines(SphereEffect *fx, u8 a1, s32 n, s32 a3, s32 otz) {
    u8 rgb[3];
    s32 i;
    s32 j;
    s32 k;
    s32 v;

    if (fx->brightness != fx->prevBrightness) {
        rgb[0] = fx->rgb[0] * fx->brightness / 256;
        rgb[1] = fx->rgb[1] * fx->brightness / 256;
        rgb[2] = fx->rgb[2] * fx->brightness / 256;
    }
    for (i = 0; i < n; i++) {
        transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][i], (s32)&fx->tpages[FRAME_BUFFER_INDEX][i],
                              (s32)&fx->verts[0], (s32)&fx->verts[i + 1], fx->semiTrans, otz);
        transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][fx->lineCount - n + i],
                              (s32)&fx->tpages[FRAME_BUFFER_INDEX][fx->lineCount - n + i], (s32)&fx->verts[fx->vertCount - 1],
                              (s32)&fx->verts[fx->vertCount - n - 1 + i], fx->semiTrans, otz);
        if (fx->brightness != fx->prevBrightness) {
            for (k = 0; k < 2; k++) {
                setRGB0(&fx->lines[k][i], rgb[0], rgb[1], rgb[2]);
                setRGB0(&fx->lines[k][fx->lineCount - n + i], rgb[0], rgb[1], rgb[2]);
            }
        }
    }
    for (j = 0; j < fx->ringCount; j++) {
        for (i = 0, v = 1; i < n; i++, v++) {
            transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][i + (j + 1) * n],
                                  (s32)&fx->tpages[FRAME_BUFFER_INDEX][i + (j + 1) * n], (s32)&fx->verts[v + j * n],
                                  (s32)&fx->verts[v % n + 1 + j * n], fx->semiTrans, otz);
            if (fx->brightness != fx->prevBrightness) {
                for (k = 0; k < 2; k++) {
                    setRGB0(&fx->lines[k][i + (j + 1) * n], rgb[0], rgb[1], rgb[2]);
                }
            }
        }
    }
    for (i = 0, v = 1; i < (fx->ringCount - 1) * n; i++, v++) {
        transformAndAddLineF2((s32)&fx->lines[FRAME_BUFFER_INDEX][i + (fx->ringCount + 1) * n],
                              (s32)&fx->tpages[FRAME_BUFFER_INDEX][i + (fx->ringCount + 1) * n], (s32)&fx->verts[v],
                              (s32)&fx->verts[v + n], fx->semiTrans, otz);
        if (fx->brightness != fx->prevBrightness) {
            for (k = 0; k < 2; k++) {
                setRGB0(&fx->lines[k][i + (fx->ringCount + 1) * n], rgb[0], rgb[1], rgb[2]);
            }
        }
    }
    fx->prevBrightness = fx->brightness;
}

/* old-style definition: the callers pass ints, cull and speed are narrowed here */
void SUG_drawSphereF(fx, cull, count, speed, otz)
    SphereEffect *fx;
    u8 cull;
    s32 count;
    s16 speed;
    s32 otz;
{
    SVECTOR a;
    SVECTOR b;
    SVECTOR c;
    SVECTOR d;
    s32 next;
    u8 dimR;
    u8 dimG;
    u8 dimB;
    u8 r;
    u8 g;
    u8 bl;
    s32 i;
    s32 scale;
    s32 sine;
    s32 cosine;
    s32 angle;
    s32 prev;
    POLY_F4 *quad;
    DrTPage *tpage;

    if (fx->pulse == 0) {
        r = fx->rgb[0];
        g = fx->rgb[1];
        bl = fx->rgb[2];
        dimR = r;
        dimG = g;
        dimB = bl;
    } else {
        r = 0;
        if (fx->pulseMode == 0) {
            g = 0;
            bl = 0;
            dimR = 0;
            dimG = 0;
            dimB = 0;
        } else {
            angle = speed * fx->ringCount * fx->pulse;
            r = fx->rgb[0];
            g = fx->rgb[1];
            bl = fx->rgb[2];
            scale = rcos(angle & 0x7FF);
            scale = scale < 0 ? -scale : scale;
            dimR = ((fx->rgb[0] * scale) >> 12) & 0xFF;
            dimG = ((fx->rgb[1] * scale) >> 12) & 0xFF;
            dimB = ((fx->rgb[2] * scale) >> 12) & 0xFF;
        }
    }
    next = 1;
    for (i = 0; i < count; i++, next++) {
        fx->tris[FRAME_BUFFER_INDEX][i].r0 = r;
        fx->tris[FRAME_BUFFER_INDEX][i].g0 = g;
        fx->tris[FRAME_BUFFER_INDEX][i].b0 = bl;
        a.vx = fx->verts[0].vx;
        a.vy = fx->verts[0].vy;
        a.vz = fx->verts[0].vz;
        b.vx = fx->verts[i + 1].vx;
        b.vy = fx->verts[i + 1].vy;
        b.vz = fx->verts[i + 1].vz;
        c.vx = fx->verts[next % count + 1].vx;
        c.vy = fx->verts[next % count + 1].vy;
        c.vz = fx->verts[next % count + 1].vz;
        transformAndAddPolyF3((s32)&fx->tris[FRAME_BUFFER_INDEX][i], (s32)&fx->tpages[FRAME_BUFFER_INDEX][i],
                              (s32)&c, (s32)&b, (s32)&a, fx->semiTrans, cull, otz);
        if (fx->openBottom == 0) {
            fx->tris[FRAME_BUFFER_INDEX][count + i].r0 = dimR;
            fx->tris[FRAME_BUFFER_INDEX][count + i].g0 = dimG;
            fx->tris[FRAME_BUFFER_INDEX][count + i].b0 = dimB;
            a.vx = fx->verts[fx->vertCount - 1].vx;
            a.vy = fx->verts[fx->vertCount - 1].vy;
            a.vz = fx->verts[fx->vertCount - 1].vz;
            b.vx = fx->verts[i + (fx->vertCount - count) - 1].vx;
            b.vy = fx->verts[i + (fx->vertCount - count) - 1].vy;
            b.vz = fx->verts[i + (fx->vertCount - count) - 1].vz;
            c.vx = fx->verts[next % count + (fx->vertCount - count) - 1].vx;
            c.vy = fx->verts[next % count + (fx->vertCount - count) - 1].vy;
            c.vz = fx->verts[next % count + (fx->vertCount - count) - 1].vz;
            transformAndAddPolyF3((s32)&fx->tris[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->tpages[FRAME_BUFFER_INDEX][count + i], (s32)&a, (s32)&b, (s32)&c,
                                  fx->semiTrans, cull, otz);
        }
    }
    quad = fx->quads[FRAME_BUFFER_INDEX];
    tpage = fx->tpages[FRAME_BUFFER_INDEX] + count * 2;
    next = 1;
    for (i = 0; i < fx->ringVertCount - count * 2; i++, next++, quad++, tpage++) {
        angle = speed * (i / count + 1) * fx->pulse;
        angle &= 0x7FF;
        if (fx->pulse == 0) {
            r = fx->rgb[0];
            g = fx->rgb[1];
            bl = fx->rgb[2];
        } else if (fx->pulseMode == 0) {
            sine = rsin(angle);
            r = ((fx->rgb[0] * sine) >> 12) & 0xFF;
            g = ((fx->rgb[1] * sine) >> 12) & 0xFF;
            bl = ((fx->rgb[2] * sine) >> 12) & 0xFF;
        } else {
            scale = rcos(angle & 0x7FF);
            scale = scale < 0 ? -scale : scale;
            r = ((fx->rgb[0] * scale) >> 12) & 0xFF;
            g = ((fx->rgb[1] * scale) >> 12) & 0xFF;
            bl = ((fx->rgb[2] * scale) >> 12) & 0xFF;
        }
        fx->quads[FRAME_BUFFER_INDEX][i].r0 = r;
        fx->quads[FRAME_BUFFER_INDEX][i].g0 = g;
        fx->quads[FRAME_BUFFER_INDEX][i].b0 = bl;
        if (next % count == 0) {
            prev = next - count;
        } else {
            prev = next;
        }
        a.vx = fx->verts[i + 1].vx;
        a.vy = fx->verts[i + 1].vy;
        a.vz = fx->verts[i + 1].vz;
        b.vx = fx->verts[prev + 1].vx;
        b.vy = fx->verts[prev + 1].vy;
        b.vz = fx->verts[prev + 1].vz;
        c.vx = fx->verts[count + i + 1].vx;
        c.vy = fx->verts[count + i + 1].vy;
        c.vz = fx->verts[count + i + 1].vz;
        d.vx = fx->verts[count + prev + 1].vx;
        d.vy = fx->verts[count + prev + 1].vy;
        d.vz = fx->verts[count + prev + 1].vz;
        transformAndAddPolyF4((s32)quad, (s32)tpage, (s32)&a, (s32)&b, (s32)&c, (s32)&d, fx->semiTrans, cull, otz);
    }
}

void SUG_drawSphereG(fx, cull, count, speed, otz)
    SphereEffect *fx;
    u8 cull;
    s32 count;
    s16 speed;
    s32 otz;
{
    Rgb c0;
    Rgb c1;
    Rgb c2;
    Rgb c3;
    Rgb base;
    s32 inner;
    s32 i;
    s32 j;
    s32 next;
    s32 prev;
    s32 s;
    s32 t;
    s32 u;
    s32 angle0;
    s32 angle1;
    s32 angle2;
    POLY_G4 *quad;
    DrTPage *tpage;

    inner = 0;
    if (fx->brightness != fx->prevBrightness) {
        base.r = fx->rgb[0] * fx->brightness / 256;
        base.g = fx->rgb[1] * fx->brightness / 256;
        base.b = fx->rgb[2] * fx->brightness / 256;
        if (fx->pulse == 0) {
            c0.r = base.r;
            c0.g = base.g;
            c0.b = base.b;
            c1 = c0;
            c2 = c1;
            c3 = c2;
        } else {
            angle0 = speed * fx->pulse;
            angle1 = speed * fx->ringCount * fx->pulse;
            angle2 = speed * (fx->ringCount + 1) * fx->pulse;
            if (fx->pulseMode == 0) {
                c0.r = 0;
                c0.g = 0;
                c0.b = 0;
                s = rsin(angle0 & 0x7FF);
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = rsin(angle1 & 0x7FF);
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = rsin(angle2 & 0x7FF);
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            } else {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                s = rcos(angle0 & 0x7FF);
                s = s < 0 ? -s : s;
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = rcos(angle1 & 0x7FF);
                s = s < 0 ? -s : s;
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = rcos(angle2 & 0x7FF);
                s = s < 0 ? -s : s;
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            }
        }
    }
    next = 1;
    for (i = 0; i < count; i++, next++) {
        if (fx->brightness != fx->prevBrightness) {
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->gtris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb1(&fx->gtris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb2(&fx->gtris[j][i], c0.r, c0.g, c0.b);
            }
        }
        transformAndAddPolyG3((s32)&fx->gtris[FRAME_BUFFER_INDEX][i], (s32)&fx->tpages[FRAME_BUFFER_INDEX][i],
                              (s32)&fx->verts[next % count + 1], (s32)&fx->verts[i + 1], (s32)fx->verts, fx->semiTrans,
                              cull, otz);
        if (fx->openBottom == 0) {
            if (fx->brightness != fx->prevBrightness) {
                for (j = 0; j < 2; j++) {
                    setPrimRgb0(&fx->gtris[j][count + i], c3.r, c3.g, c3.b);
                    setPrimRgb1(&fx->gtris[j][count + i], c2.r, c2.g, c2.b);
                    setPrimRgb2(&fx->gtris[j][count + i], c2.r, c2.g, c2.b);
                }
            }
            transformAndAddPolyG3((s32)&fx->gtris[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->tpages[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->verts[fx->vertCount - 1],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + i],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + next % count], fx->semiTrans, cull,
                                  otz);
            inner = count * 2;
        } else {
            inner = count;
        }
    }
    quad = fx->gquads[FRAME_BUFFER_INDEX];
    tpage = &fx->tpages[FRAME_BUFFER_INDEX][inner];
    next = 1;
    for (i = 0; i < fx->ringVertCount - inner; i++, next++, quad++, tpage++) {
        if (fx->brightness != fx->prevBrightness) {
            angle0 = speed * (i / count + 1) * fx->pulse;
            angle1 = speed * (i / count + 2) * fx->pulse;
            angle0 &= 0x7FF;
            angle1 &= 0x7FF;
            if (fx->pulse == 0) {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                c1 = c0;
            } else {
                if (fx->pulseMode == 0) {
                    s = rsin(angle0);
                    t = rsin(angle1);
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * t >> 12;
                    c1.g = base.g * t >> 12;
                    c1.b = base.b * t >> 12;
                } else {
                    s = rcos(angle0);
                    s = s < 0 ? -s : s;
                    u = rcos(angle1);
                    u = u < 0 ? -u : u;
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * u >> 12;
                    c1.g = base.g * u >> 12;
                    c1.b = base.b * u >> 12;
                }
            }
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->gquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb1(&fx->gquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb2(&fx->gquads[j][i], c1.r, c1.g, c1.b);
                setPrimRgb3(&fx->gquads[j][i], c1.r, c1.g, c1.b);
            }
        }
        if (next % count == 0) {
            prev = next - count;
        } else {
            prev = next;
        }
        transformAndAddPolyG4((s32)quad, (s32)tpage, (s32)&fx->verts[i + 1], (s32)&fx->verts[prev + 1],
                              (s32)&fx->verts[i + 1 + count], (s32)&fx->verts[prev + 1 + count], fx->semiTrans,
                              cull, otz);
    }
    fx->prevBrightness = fx->brightness;
}


/* old-style definition: the callers pass ints, cull and speed are narrowed here */
void SUG_drawSphereGT(fx, cull, count, speed, otz)
    SphereEffect *fx;
    u8 cull;
    s32 count;
    s16 speed;
    s32 otz;
{
    Rgb c0;
    Rgb c1;
    Rgb c2;
    Rgb c3;
    Rgb base;
    s32 inner;
    s32 i;
    s32 j;
    s32 next;
    s32 prev;
    s32 s;
    s32 t;
    s32 u;
    s32 angle0;
    s32 angle1;
    s32 angle2;
    POLY_GT4 *quad;

    inner = 0;
    if (fx->texAnimActive >= 0) {
        SUG_updateSphereUvs(fx);
    }
    if (fx->brightness != fx->prevBrightness) {
        base.r = fx->rgb[0] * fx->brightness / 256;
        base.g = fx->rgb[1] * fx->brightness / 256;
        base.b = fx->rgb[2] * fx->brightness / 256;
        if (fx->pulse == 0) {
            c0.r = base.r;
            c0.g = base.g;
            c0.b = base.b;
            c1 = c0;
            c2 = c1;
            c3 = c2;
        } else {
            angle0 = speed * fx->pulse;
            angle1 = speed * fx->ringCount * fx->pulse;
            angle2 = speed * (fx->ringCount + 1) * fx->pulse;
            if (fx->pulseMode == 0) {
                c0.r = 0;
                c0.g = 0;
                c0.b = 0;
                s = rsin(angle0 & 0x7FF);
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = rsin(angle1 & 0x7FF);
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = rsin(angle2 & 0x7FF);
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            } else {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                s = rcos(angle0 & 0x7FF);
                s = s < 0 ? -s : s;
                c1.r = base.r * s >> 12;
                c1.g = base.g * s >> 12;
                c1.b = base.b * s >> 12;
                s = rcos(angle1 & 0x7FF);
                s = s < 0 ? -s : s;
                c2.r = base.r * s >> 12;
                c2.g = base.g * s >> 12;
                c2.b = base.b * s >> 12;
                s = rcos(angle2 & 0x7FF);
                s = s < 0 ? -s : s;
                c3.r = base.r * s >> 12;
                c3.g = base.g * s >> 12;
                c3.b = base.b * s >> 12;
            }
        }
    }
    next = 1;
    for (i = 0; i < count; i++, next++) {
        if (fx->brightness != fx->prevBrightness) {
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->ttris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb1(&fx->ttris[j][i], c1.r, c1.g, c1.b);
                setPrimRgb2(&fx->ttris[j][i], c0.r, c0.g, c0.b);
            }
        }
        transformAndAddPolyGT3((s32)&fx->ttris[FRAME_BUFFER_INDEX][i], (s32)&fx->verts[next % count + 1],
                               (s32)&fx->verts[i + 1], (s32)fx->verts, cull, otz);
        if (fx->openBottom == 0) {
            if (fx->brightness != fx->prevBrightness) {
                for (j = 0; j < 2; j++) {
                    setPrimRgb0(&fx->ttris[j][count + i], c3.r, c3.g, c3.b);
                    setPrimRgb1(&fx->ttris[j][count + i], c2.r, c2.g, c2.b);
                    setPrimRgb2(&fx->ttris[j][count + i], c2.r, c2.g, c2.b);
                }
            }
            transformAndAddPolyGT3((s32)&fx->ttris[FRAME_BUFFER_INDEX][count + i],
                                  (s32)&fx->verts[fx->vertCount - 1],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + i],
                                  (s32)&fx->verts[fx->vertCount - count - 1 + next % count], cull, otz);
            inner = count * 2;
        } else {
            inner = count;
        }
    }
    quad = fx->tquads[FRAME_BUFFER_INDEX];
    next = 1;
    for (i = 0; i < fx->ringVertCount - inner; i++, next++, quad++) {
        if (fx->brightness != fx->prevBrightness) {
            angle0 = speed * (i / count + 1) * fx->pulse;
            angle1 = speed * (i / count + 2) * fx->pulse;
            angle0 &= 0x7FF;
            angle1 &= 0x7FF;
            if (fx->pulse == 0) {
                c0.r = base.r;
                c0.g = base.g;
                c0.b = base.b;
                c1 = c0;
            } else {
                if (fx->pulseMode == 0) {
                    s = rsin(angle0);
                    t = rsin(angle1);
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * t >> 12;
                    c1.g = base.g * t >> 12;
                    c1.b = base.b * t >> 12;
                } else {
                    s = rcos(angle0);
                    if (s < 0) {
                        s = -s;
                    }
                    u = rcos(angle1);
                    if (u < 0) {
                        u = -u;
                    }
                    c0.r = base.r * s >> 12;
                    c0.g = base.g * s >> 12;
                    c0.b = base.b * s >> 12;
                    c1.r = base.r * u >> 12;
                    c1.g = base.g * u >> 12;
                    c1.b = base.b * u >> 12;
                }
            }
            for (j = 0; j < 2; j++) {
                setPrimRgb0(&fx->tquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb1(&fx->tquads[j][i], c0.r, c0.g, c0.b);
                setPrimRgb2(&fx->tquads[j][i], c1.r, c1.g, c1.b);
                setPrimRgb3(&fx->tquads[j][i], c1.r, c1.g, c1.b);
            }
        }
        if (next % count == 0) {
            prev = next - count;
        } else {
            prev = next;
        }
        transformAndAddPolyGT4((s32)quad, (s32)&fx->verts[i + 1], (s32)&fx->verts[prev + 1],
                               (s32)&fx->verts[i + 1 + count], (s32)&fx->verts[prev + 1 + count], cull, otz);
    }
    fx->prevBrightness = fx->brightness;
}

void SUG_updateSphereUvs(SphereEffect *obj) {
    Rect16 uv;
    POLY_GT4 *gt4;
    s32 n;
    s32 i;
    s32 skip;

    n = obj->segments;
    SUG_tickTexAnim(obj->texAnim);
    uv = obj->uv;
    for (i = 0; i < n; i++) {
        obj->ttris[FRAME_BUFFER_INDEX][i].u0 = uv.x + uv.w;
        obj->ttris[FRAME_BUFFER_INDEX][i].v0 = uv.y + uv.h;
        obj->ttris[FRAME_BUFFER_INDEX][i].u1 = uv.x;
        obj->ttris[FRAME_BUFFER_INDEX][i].v1 = uv.y + uv.h;
        obj->ttris[FRAME_BUFFER_INDEX][i].u2 = uv.w / 2 + uv.x;
        obj->ttris[FRAME_BUFFER_INDEX][i].v2 = uv.y;
        obj->ttris[FRAME_BUFFER_INDEX][i].tpage = obj->tpage;
        obj->ttris[FRAME_BUFFER_INDEX][i].clut = obj->clut;
        if (obj->openBottom == 0) {
            obj->ttris[FRAME_BUFFER_INDEX][n + i].u0 = uv.w / 2 + uv.x;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].v0 = uv.y + uv.h;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].u1 = uv.x;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].v1 = uv.y;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].u2 = uv.x + uv.w;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].v2 = uv.y;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].tpage = obj->tpage;
            obj->ttris[FRAME_BUFFER_INDEX][n + i].clut = obj->clut;
        }
    }
    if (obj->openBottom != 0) {
        skip = n;
    } else {
        skip = n * 2;
    }
    gt4 = obj->tquads[FRAME_BUFFER_INDEX];
    for (i = 0; i < obj->ringVertCount - skip; i++, gt4++) {
        gt4->tpage = obj->tpage;
        gt4->clut = obj->clut;
        setPrimQuadUvRect((u8 *)gt4, uv.x, uv.y, uv.w, uv.h);
    }
}

void SUG_freeSphereEffect(SphereEffect *obj) {
    s32 i;

    freeHeapBlock(obj->verts);
    if (obj->texAnimActive >= 0) {
        SUG_freeTexAnim(obj->texAnim);
    }
    for (i = 0; i < 2; i++) {
        if (obj->tris[i] != NULL) {
            freeHeapBlock(obj->tris[i]);
        }
        if (obj->quads[i] != NULL) {
            freeHeapBlock(obj->quads[i]);
        }
        if (obj->gtris[i] != NULL) {
            freeHeapBlock(obj->gtris[i]);
        }
        if (obj->gquads[i] != NULL) {
            freeHeapBlock(obj->gquads[i]);
        }
        if (obj->ttris[i] != NULL) {
            freeHeapBlock(obj->ttris[i]);
        }
        if (obj->tquads[i] != NULL) {
            freeHeapBlock(obj->tquads[i]);
        }
        if (obj->lines[i] != NULL) {
            freeHeapBlock(obj->lines[i]);
        }
        if (obj->tpages[i] != NULL) {
            freeHeapBlock(obj->tpages[i]);
        }
    }
    freeHeapBlock(obj);
}

/* not referenced by any code */
const s32 D_801DDF38 = 4;

/* fills a grid of a2 x a3 cells, 4 colors each, by blending the corner colors; the kinds split it
   into 1, 2 or 4 blends around the middle color a8 */
void SUG_fillGradientColors(void *a0, s32 a1, s32 a2, s32 a3, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, Bytes4 *a8) {
    Bytes4 top;
    Bytes4 left;
    Bytes4 right;
    Bytes4 bottom;
    Blend blend;
    Bytes4 *out;
    s32 x;
    s32 y;

    out = (Bytes4 *)a0;
    switch ((u8)a1) {
    case 0:
        out[0] = *c0;
        out[1] = *c1;
        out[2] = *c2;
        out[3] = *c3;
        break;
    case 1:
        blend.c00 = c0->b;
        blend.c10 = c1->b;
        blend.c01 = c2->b;
        blend.c11 = c3->b;
        for (y = 0; y < a3; y++) {
            for (x = 0; x < a2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
        }
        break;
    case 2:
        blend.c00 = c0->b;
        blend.c10 = c1->b;
        blend.c01 = a8->b;
        blend.c11 = a8->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
        }
        blend.c00 = a8->b;
        blend.c10 = a8->b;
        blend.c01 = c2->b;
        blend.c11 = c3->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
        }
        break;
    case 3:
        out = (Bytes4 *)a0;
        blend.c00 = c0->b;
        blend.c10 = a8->b;
        blend.c01 = c2->b;
        blend.c11 = a8->b;
        for (y = 0; y < a3; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 / 2 * 4;
        blend.c00 = a8->b;
        blend.c10 = c1->b;
        blend.c01 = a8->b;
        blend.c11 = c3->b;
        for (y = 0; y < a3; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        break;
    case 4:
        blend.c00 = c0->b;
        blend.c10 = c1->b;
        blend.c01 = c2->b;
        blend.c11 = c3->b;
        blend.wx0 = a2 / 2;
        blend.wx1 = a2 / 2;
        blend.wy0 = 0;
        blend.wy1 = a3;
        SUG_blendCornerColors(&blend, top.b);
        blend.wx0 = 0;
        blend.wx1 = a2;
        blend.wy0 = a3 / 2;
        blend.wy1 = a3 / 2;
        SUG_blendCornerColors(&blend, left.b);
        blend.wx0 = a2;
        blend.wx1 = 0;
        blend.wy0 = a3 / 2;
        blend.wy1 = a3 / 2;
        SUG_blendCornerColors(&blend, right.b);
        blend.wx0 = a2 / 2;
        blend.wx1 = a2 / 2;
        blend.wy0 = a3;
        blend.wy1 = 0;
        SUG_blendCornerColors(&blend, bottom.b);
        out = (Bytes4 *)a0;
        blend.c00 = c0->b;
        blend.c10 = top.b;
        blend.c01 = left.b;
        blend.c11 = a8->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 / 2 * 4;
        blend.c00 = top.b;
        blend.c10 = c1->b;
        blend.c01 = a8->b;
        blend.c11 = right.b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 * a3 / 2 * 4;
        blend.c00 = left.b;
        blend.c10 = a8->b;
        blend.c01 = c2->b;
        blend.c11 = bottom.b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        out = (Bytes4 *)a0 + a2 * a3 / 2 * 4 + a2 / 2 * 4;
        blend.c00 = a8->b;
        blend.c10 = right.b;
        blend.c01 = bottom.b;
        blend.c11 = c3->b;
        for (y = 0; y < a3 / 2; y++) {
            for (x = 0; x < a2 / 2; x++) {
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[0].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y;
                blend.wy1 = a3 / 2 - y;
                SUG_blendCornerColors(&blend, out[1].b);
                blend.wx0 = x;
                blend.wx1 = a2 / 2 - x;
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[2].b);
                blend.wx0 = x + 1;
                blend.wx1 = a2 / 2 - (x + 1);
                blend.wy0 = y + 1;
                blend.wy1 = a3 / 2 - (y + 1);
                SUG_blendCornerColors(&blend, out[3].b);
                out += 4;
            }
            out += a2 / 2 * 4;
        }
        break;
    }
}

void SUG_blendCornerColors(Blend *blend, u8 *out) {
    s32 bottom[3];
    s32 top[3];

    top[0] = (blend->c10[0] * blend->wx0 + blend->c00[0] * blend->wx1) / (blend->wx0 + blend->wx1);
    top[1] = (blend->c10[1] * blend->wx0 + blend->c00[1] * blend->wx1) / (blend->wx0 + blend->wx1);
    top[2] = (blend->c10[2] * blend->wx0 + blend->c00[2] * blend->wx1) / (blend->wx0 + blend->wx1);
    bottom[0] = (blend->c11[0] * blend->wx0 + blend->c01[0] * blend->wx1) / (blend->wx0 + blend->wx1);
    bottom[1] = (blend->c11[1] * blend->wx0 + blend->c01[1] * blend->wx1) / (blend->wx0 + blend->wx1);
    bottom[2] = (blend->c11[2] * blend->wx0 + blend->c01[2] * blend->wx1) / (blend->wx0 + blend->wx1);
    out[0] = (blend->wy0 * bottom[0] + blend->wy1 * top[0]) / (blend->wy0 + blend->wy1);
    out[1] = (blend->wy0 * bottom[1] + blend->wy1 * top[1]) / (blend->wy0 + blend->wy1);
    out[2] = (blend->wy0 * bottom[2] + blend->wy1 * top[2]) / (blend->wy0 + blend->wy1);
}

void SUG_setGridUvsFT4(POLY_FT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink) {
    Rect16 r;
    s32 col;
    s32 stepX;
    s32 stepY;
    s16 cellW;
    s16 cellH;
    s32 x;
    s32 row;
    s32 y;
    s32 i;

    stepX = (uv->w << 16) / cols;
    stepY = (uv->h << 16) / rows;
    cellW = uv->w / cols;
    cellH = uv->h / rows;
    r.w = cellW;
    r.h = cellH;
    col = 0;
    row = 0;
    x = 0;
    y = 0;
    for (i = 0; i < count; i++) {
        r.x = uv->x + (x >> 16);
        r.y = uv->y + (y >> 16);
        if (row == rows - 1 && shrink) {
            r.h = cellH - 1;
        } else {
            r.h = cellH;
        }
        if (col >= cols - 1) {
            col = 0;
            x = 0;
            row++;
            y += stepY;
            if (shrink) {
                r.w = cellW - 1;
            } else {
                r.w = cellW;
            }
        } else {
            col++;
            x += stepX;
            r.w = cellW;
        }
        setPrimQuadUvRect((u8 *)polys, r.x, r.y, r.w + padW, r.h + padH);
        polys++;
    }
}

void SUG_setGridUvsGT4(POLY_GT4 *polys, Rect16 *uv, s32 count, s32 cols, s32 rows, s16 padW, s16 padH, u8 shrink) {
    Rect16 cell;
    s32 col;
    s32 stepX;
    s32 stepY;
    s16 cellW;
    s16 cellH;
    s32 row;
    s32 accX;
    s32 accY;
    s32 i;

    stepX = (uv->w << 16) / cols;
    stepY = (uv->h << 16) / rows;
    cellW = uv->w / cols;
    cellH = uv->h / rows;
    cell.w = cellW;
    cell.h = cellH;
    col = 0;
    row = 0;
    accX = 0;
    accY = 0;
    for (i = 0; i < count; i++) {
        cell.x = uv->x + (accX >> 16);
        cell.y = uv->y + (accY >> 16);
        if (shrink) {
            if (row == rows - 1) {
                cell.h = cellH - 1;
            } else {
                cell.h = cellH;
            }
        }
        if (col >= cols - 1) {
            col = 0;
            accX = 0;
            row++;
            accY += stepY;
            if (shrink) {
                cell.w = cellW - 1;
            }
        } else {
            col++;
            accX += stepX;
            cell.w = cellW;
        }
        setPrimQuadUvRect((u8 *)polys, cell.x, cell.y, cell.w + padW, cell.h + padH);
        polys++;
    }
}

void SUG_uploadShadedClut(ClutFade *fade, u16 stp) {
    u16 *src;
    u16 *dst;
    s32 brighten;
    s32 level;
    s32 color;
    s32 r;
    s32 g;
    s32 b;
    s32 i;

    brighten = fade->brighten;
    level = fade->level;
    src = fade->clut;
    dst = fade->faded;
    for (i = 0; i < fade->rect.w * fade->rect.h; i++) {
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
            color = stp | (color & 0x8000) | (b << 10) | (g << 5) | r;
        }
        *dst++ = color;
    }
    LoadImage((s16 *)&fade->rect, (s32)fade->faded);
    DrawSync(0);
}

extern const Rect16 SUG_MODEL_CLUT_RECT;

void *SUG_createModelEffect(s16 brightness, EffectTemplate *template, s32 modelId, s32 anim, s32 a4, s32 vramSlot, u8 a6, s32 a7, s32 pak, s32 a9) {
    ModelEffect *fx;
    Rect16 rect;
    s32 slot;

    fx = allocTaskHeapBlock(sizeof(ModelEffect));
    for (slot = 2; slot < 23 && SCENE_3D->modelState[slot] != 0; slot++) {
    }
    if (slot >= 23 || !loadModel(slot, modelId, vramSlot, pak, 0)) {
        freeHeapBlock(fx);
        return NULL;
    }
    fx->modelSlot = slot;
    fx->model = SCENE_3D->models[slot];
    if (anim >= 0 && loadModelAnimation(slot, anim, 0, pak)) {
        applyAnimationFirstFrame(slot, 0);
        startModelAnimation(slot, 0, -2, a7 ^ 1);
    }
    if (template != NULL) {
        *(EffectTemplate *)fx = *template;
        initEffectObject(fx);
        SCENE_3D->modelState[fx->modelSlot] = -1;
        fx->active = 1;
    } else {
        SCENE_3D->modelState[fx->modelSlot] = 1;
        fx->active = 0;
    }
    fx->unk574 = a9;
    fx->model->owner = fx;
    if (vramSlot != 0 && a4 >= 0 && SUG_startTexAnim(a4, 0, (RingEffect *)fx->model, fx->texAnim, pak)) {
        fx->texAnimActive = 1;
    } else {
        fx->texAnimActive = -1;
    }
    if (vramSlot != 0) {
        rect = SUG_MODEL_CLUT_RECT;
        if (fx->unk574 == 0) {
            rect.y = 0xF0;
        }
        rect.x += ((fx->model->unk26D4 / 0x10000 + 5) & 0xF) << 6;
        rect.y += ((fx->model->unk26D4 / 0x10000 + 5) >> 4) << 8;
        StoreImage2(&rect, (u32 *)fx->fade.clut);
        fx->fade.rect = rect;
        fx->fade.brighten = 0;
        fx->fade.level = fx->lastBrightness = brightness;
        if (brightness != 0xFF) {
            SUG_uploadShadedClut(&fx->fade, 0x8000);
        }
    } else {
        fx->model->unk26D4 = -1;
    }
    fx->unk56F = a6;
    return fx;
}

void SUG_tickModelEffect(ModelEffect *obj) {
    if (obj->active != 0) {
        if (obj->suspended != 0) {
            tickEffectStartDelay(obj);
            SCENE_3D->modelState[obj->modelSlot] = -1;
            return;
        }
        if (obj->model->unk26D4 != -1) {
            obj->fade.level = updateEffectBrightness(obj, obj->fade.level);
            if (obj->fade.level != obj->lastBrightness) {
                obj->lastBrightness = obj->fade.level;
                SUG_uploadShadedClut(&obj->fade, 0x8000);
            }
            if (obj->fade.level == 0) {
                SCENE_3D->modelState[obj->modelSlot] = -1;
                return;
            }
        }
        SCENE_3D->modelState[obj->modelSlot] = 3;
    }
    if (obj->texAnimActive >= 0) {
        SUG_tickTexAnim(obj->texAnim);
    }
}

void SUG_freeModelEffect(ModelEffect *obj) {
    unloadModel(obj->modelSlot);
    if (obj->model->unk26D4 != -1 && obj->fade.level != 0xFF) {
        obj->fade.level = 0xFF;
        SUG_uploadShadedClut(&obj->fade, 0x8000);
    }
    freeHeapBlock(obj);
}

extern ClutFade SUG_STAGE_CLUT;

void SUG_setStageBrightness(u8 level) {
    u8 r;
    u8 g;
    u8 b;

    SUG_STAGE_CLUT.level = level;
    if (level == 0xFF) {
        LoadImage((s16 *)&SUG_STAGE_CLUT.rect, (s32)SUG_STAGE_CLUT.clut);
        DrawSync(0);
    } else {
        SUG_uploadShadedClut(&SUG_STAGE_CLUT, 0);
    }
    r = STAGE_CLEAR_COLOR[0] * level / 255;
    g = STAGE_CLEAR_COLOR[1] * level / 255;
    b = STAGE_CLEAR_COLOR[2] * level / 255;
    DB(0).draw.r0 = DB(1).draw.r0 = r;
    DB(0).draw.g0 = DB(1).draw.g0 = g;
    DB(0).draw.b0 = DB(1).draw.b0 = b;
}

#define FADE_LEVEL (((u8 *)SCENE_3D)[0x130])
#define FADE_TARGET (((u8 *)SCENE_3D)[0x131])

void SUG_runStageFadeTask(void) {
    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (FADE_LEVEL > FADE_TARGET) {
            if (FADE_LEVEL < 3) {
                FADE_LEVEL = 0;
            } else {
                FADE_LEVEL -= 3;
            }
            if (FADE_LEVEL < FADE_TARGET) {
                FADE_LEVEL = FADE_TARGET;
            }
        } else if (FADE_LEVEL < FADE_TARGET) {
            if (FADE_LEVEL >= 0xFD) {
                FADE_LEVEL = 0xFF;
            } else {
                FADE_LEVEL += 3;
            }
            if (FADE_LEVEL > FADE_TARGET) {
                FADE_LEVEL = FADE_TARGET;
            }
        } else {
            continue;
        }
        SUG_setStageBrightness(FADE_LEVEL);
    }
}

void SUG_saveStageClut(ModelData *model, s16 h) {
    Rect16 rect;

    rect = SUG_MODEL_CLUT_RECT;
    rect.h = h;
    rect.x += ((model->unk26D4 / 0x10000 + 5) & 0xF) << 6;
    rect.y += ((model->unk26D4 / 0x10000 + 5) >> 4) << 8;
    StoreImage2(&rect, (u32 *)SUG_STAGE_CLUT.clut);
    SUG_STAGE_CLUT.rect = rect;
    SUG_STAGE_CLUT.brighten = 0;
    FADE_LEVEL = FADE_TARGET = 0xFF;
}

void SUG_resetStageBrightness(void) {
    SUG_setStageBrightness(0xFF);
    DB(1).draw.r0 = 0;
    DB(0).draw.r0 = 0;
    DB(1).draw.g0 = 0;
    DB(0).draw.g0 = 0;
    DB(1).draw.b0 = 0;
    DB(0).draw.b0 = 0;
}

void SUG_shadeModelClut(s32 slot, s32 target) {
    ClutFade fade;
    ModelData *model;
    s32 i;

    model = SCENE_3D->models[slot];
    if (target != 0xFF) {
        fade.rect = model->clutRect;
        fade.brighten = 0;
        fade.level = target;
        for (i = 0; i < 256; i++) {
            fade.clut[i] = model->clut[i];
        }
        SUG_uploadShadedClut(&fade, 0x8000);
    } else {
        LoadImage((s16 *)&model->clutRect, (s32)model->clut);
        DrawSync(0);
    }
}

void *SUG_createSpriteEffect(s32 brightness, EffectTemplate *template, s32 key, s32 flipX, s32 flipY, s32 a5, s32 useOrigin, s32 a7, s32 subKey) {
    SpriteEffect *fx;

    fx = allocTaskHeapBlock(sizeof(SpriteEffect));
    fx->sprite.flipX = flipX;
    fx->sprite.flipY = flipY;
    fx->sprite.unk8A = a5 == 0;
    SUG_initSprite(&fx->sprite, key, 4, 4, 0, 0, 0, a7, useOrigin, subKey);
    fx->brightness = brightness;
    if (template != NULL) {
        *(EffectTemplate *)fx = *template;
        initEffectObject(fx);
        fx->active = 1;
    } else {
        fx->active = 0;
        fx->brightness = 0xFF;
    }
    return fx;
}

void SUG_tickSpriteEffect(SpriteEffect *fx) {
    PushMatrix();
    if (fx->active != 0) {
        if (fx->suspended != 0 || (fx->brightness = updateEffectBrightness(fx, fx->brightness)) == 0) {
            PopMatrix();
            tickEffectStartDelay(fx);
            return;
        }
        tickEffectMotion((s32)fx, fx->sprite.unk8A);
    } else {
        SetRotMatrix((s32)&D_801DBEC0);
        func_8005C444(&D_801DBEC0);
    }
    SUG_drawSprite(&fx->sprite, fx->brightness);
    PopMatrix();
}

void SUG_freeSpriteEffect(void *ptr) {
    freeHeapBlock(ptr);
}

TrailEffect *SUG_createTrailEffect(s16 a0, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, EffectTemplate *template, s16 x0, s16 x1,
                           s32 count, s16 rows, u8 a10, u8 a11, u8 a12, u8 a13, u8 a14, s32 id, Rect16 *uv, s32 tpage, s32 clut,
                           s32 a19, s32 a20) {
    TrailEffect *obj;
    s32 i;

    obj = allocTaskHeapBlock(sizeof(TrailEffect));
    for (i = 0; i < 2; i++) {
        obj->histories[i] = SUG_createPosHistory(count + 1, rows);
    }
    obj->clut = clut;
    obj->tpage = tpage;
    obj->semiTrans = a12;
    obj->primKind = a14;
    obj->colorMode = a11;
    obj->count = count;
    obj->blend = a13;
    obj->level = a0;
    obj->primeCount = 0;
    obj->followMode = a10;
    initTransform(obj->xform, 0, 0, 0, 0, 0, 0, 0);
    *(EffectTemplate *)obj = *template;
    initEffectObject(obj);
    obj->otz = a19;
    obj->prevPos[1] = obj->prevPos[0] = obj->pos;
    obj->prevRot[0] = obj->rot;
    obj->prevRot[1] = obj->rot;
    initTransform(obj->edges[0], (s32)obj, x0, 0, 0, 0, 0, 0);
    initTransform(obj->edges[1], (s32)obj, x1, 0, 0, 0, 0, 0);
    obj->colors = allocTaskHeapBlock(a11 ? count * 16 : 16);
    SUG_initTrailPrims(obj, a12, a13, a14, id, uv, tpage, clut);
    SUG_setTrailColors(obj, a11, c0, c1, c2, c3);
    obj->texAnimId = id;
    if (id < 100 || SUG_startTexAnim(id, 4, (RingEffect *)obj, obj->texAnim, a20) == 0) {
        obj->texAnimId = -1;
    }
    return obj;
}

void SUG_shadeTrailPrims(TrailEffect *obj);

void SUG_tickTrailEffect(TrailEffect *obj) {
    Short4 pos;
    Short4 b;
    Short4 c;
    Short4 d;
    Short4 delta;
    s32 moved;
    s32 i;
    s32 j;
    s32 k;
    DrTPage *tp;
    LineG2 *line;
    POLY_G4 *g4;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;

    moved = 0;
    if (obj->suspended != 0) {
        tickEffectStartDelay(obj);
        return;
    }
    PushMatrix();
    tickEffectMotion((s32)obj, 0);
    PopMatrix();
    switch (obj->fadeMode) {
    case 1:
        obj->level = obj->sx / 16;
        if (obj->sx > 0x1000) {
            obj->level = 0x100 - (obj->sx - 0x1000) / 16;
        }
        break;
    case 2:
        obj->level += obj->speed;
        break;
    case 3:
        if (obj->fadeState == 2) {
            break;
        }
        if (obj->fadeState == 0) {
            goto grow;
        }
        obj->level -= obj->speed;
        if (obj->level < 0) {
            /* the extra block is needed for the register allocation to match */
            do {
                obj->level = 0;
            } while (0);
            obj->fadeState = 2;
        }
        break;
    case 4:
        if (obj->fadeState == 0) {
        grow:
            obj->level += obj->speed;
            if (obj->level > 0x100) {
                obj->level = 0x100;
                obj->fadeState = 1;
            }
        } else {
            obj->level -= obj->speed;
            if (obj->level < 0) {
                obj->level = 0;
                obj->fadeState = 0;
            }
        }
        break;
    }
    if (obj->level < 0) {
        obj->level = 0;
    }
    if (obj->level > 0x100) {
        obj->level = 0x100;
    }
    if (obj->mode == 10) {
        obj->level = obj->brightness;
    } else {
        obj->brightness = obj->level;
    }
    if (obj->level == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        PushMatrix();
        updateTransformMatrix(obj->edges[i], 0);
        PopMatrix();
        getTransformWorldPos(obj->edges[i], &pos);
        if (obj->followMode == 0) {
            moved = 1;
        } else if (obj->followMode == 1) {
            if (obj->lastPos[i].v[0] != pos.v[0] || obj->lastPos[i].v[1] != pos.v[1] || obj->lastPos[i].v[2] != pos.v[2]) {
                moved = 1;
            }
        } else {
            moved = 1;
            /* the extra block is needed for the delay slots to match */
            do {
                if (obj->prevPos[i].vx != obj->pos.vx || obj->prevPos[i].vy != obj->pos.vy || obj->prevPos[i].vz != obj->pos.vz ||
                    obj->prevRot[i].vx != obj->rot.vx || obj->prevRot[i].vy != obj->rot.vy || obj->prevRot[i].vz != obj->rot.vz) {
                    goto skip;
                }
            } while (0);
            moved = 2;
            delta.v[0] = obj->lastPos[i].v[0] - pos.v[0];
            delta.v[1] = obj->lastPos[i].v[1] - pos.v[1];
            delta.v[2] = obj->lastPos[i].v[2] - pos.v[2];
            for (j = 0; j < obj->histories[i]->count; j++) {
                obj->histories[i]->shorts[j].v[0] -= delta.v[0];
                obj->histories[i]->shorts[j].v[1] -= delta.v[1];
                obj->histories[i]->shorts[j].v[2] -= delta.v[2];
            }
        skip:
            obj->prevPos[i] = obj->pos;
            obj->prevRot[i] = obj->rot;
        }
        if (obj->primeCount != 2) {
            for (k = 0; k < obj->histories[i]->count; k++) {
                obj->histories[i]->shorts[k] = pos;
            }
            obj->lastPos[i] = pos;
            obj->primeCount++;
        }
        if (moved == 1) {
            SUG_pushPosHistory(obj->histories[i], NULL, &pos);
        }
        obj->lastPos[i] = pos;
    }
    PushMatrix();
    updateTransformMatrix(obj->xform, 0);
    SUG_shadeTrailPrims(obj);
    switch (obj->primKind) {
    case 1:
        tp = obj->tpages[FRAME_BUFFER_INDEX];
        line = obj->lines[FRAME_BUFFER_INDEX];
        for (i = 0; i < obj->count; i++, line++, tp++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &d);
            transformAndAddLineG2((s32)line, (s32)tp, (s32)&pos, (s32)&d, obj->semiTrans, obj->otz);
        }
        break;
    case 9:
        tp = obj->tpages[FRAME_BUFFER_INDEX];
        g4 = obj->g4s[FRAME_BUFFER_INDEX];
        for (i = 0; i < obj->count; i++, g4++, tp++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &c);
            SUG_getPosHistory(obj->histories[1], i, NULL, &b);
            SUG_getPosHistory(obj->histories[1], i + 1, NULL, &d);
            transformAndAddPolyG4((s32)g4, (s32)tp, (s32)&pos, (s32)&b, (s32)&c, (s32)&d, obj->semiTrans, 0, obj->otz);
        }
        break;
    case 12:
        ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
        if (obj->texAnimId != -1) {
            SUG_tickTexAnim(obj->texAnim);
        }
        for (i = 0; i < obj->count; i++, ft4++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &c);
            SUG_getPosHistory(obj->histories[1], i, NULL, &b);
            SUG_getPosHistory(obj->histories[1], i + 1, NULL, &d);
            setPrimQuadUvRect((u8 *)ft4, obj->uv.x, obj->uv.y, obj->uv.w, obj->uv.h);
            transformAndAddPolyFT4((s32)ft4, (s32)&pos, (s32)&b, (s32)&c, (s32)&d, 0, obj->otz);
        }
        break;
    case 13:
        gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
        if (obj->texAnimId != -1) {
            SUG_tickTexAnim(obj->texAnim);
        }
        for (i = 0; i < obj->count; i++, gt4++) {
            SUG_getPosHistory(obj->histories[0], i, NULL, &pos);
            SUG_getPosHistory(obj->histories[0], i + 1, NULL, &c);
            SUG_getPosHistory(obj->histories[1], i, NULL, &b);
            SUG_getPosHistory(obj->histories[1], i + 1, NULL, &d);
            setPrimQuadUvRect((u8 *)gt4, obj->uv.x, obj->uv.y, obj->uv.w, obj->uv.h);
            transformAndAddPolyGT4((s32)gt4, (s32)&pos, (s32)&b, (s32)&c, (s32)&d, 0, obj->otz);
        }
        break;
    }
    PopMatrix();
}

void SUG_freeTrailEffect(TrailEffect *obj) {
    s32 i;

    for (i = 0; i < 2; i++) {
        freeHeapBlock(obj->lines[i]);
        freeHeapBlock(obj->g4s[i]);
        freeHeapBlock(obj->ft4s[i]);
        freeHeapBlock(obj->gt4s[i]);
        freeHeapBlock(obj->tpages[i]);
        SUG_freePosHistory((void **)obj->histories[i]);
    }
    if (obj->texAnimId >= 0) {
        SUG_freeTexAnim(obj->texAnim);
    }
    freeHeapBlock(obj->colors);
    freeHeapBlock(obj);
}

void SUG_setTrailColors(TrailEffect *obj, u8 kind, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3) {
    obj->colorMode = kind;
    SUG_fillGradientColors(obj->colors, kind, 1, obj->count, c0, c1, c2, c3, 0);
    obj->prevLevel = -1;
}

void SUG_initTrailPrims(TrailEffect *obj, u8 semiTrans, u8 blend, u8 kind, u8 a4, Rect16 *uv, s32 tpage, s32 clut) {
    s32 i;
    LineG2 *line0;
    LineG2 *line1;
    POLY_G4 *g4a;
    POLY_G4 *g4b;
    POLY_FT4 *ft4a;
    POLY_FT4 *ft4b;
    POLY_GT4 *gt4a;
    POLY_GT4 *gt4b;
    DrTPage *tp0;
    DrTPage *tp1;

    for (i = 0; i < 2; i++) {
        obj->tpages[i] = NULL;
        obj->lines[i] = NULL;
        obj->g4s[i] = NULL;
        obj->ft4s[i] = NULL;
        obj->gt4s[i] = NULL;
    }
    switch (kind) {
    case 1:
        if (semiTrans) {
            for (i = 0; i < 2; i++) {
                obj->tpages[i] = allocTaskHeapBlock(obj->count * sizeof(DrTPage));
            }
        }
        for (i = 0; i < 2; i++) {
            obj->lines[i] = allocTaskHeapBlock(obj->count * sizeof(LineG2));
        }
        line0 = obj->lines[0];
        line1 = obj->lines[1];
        tp0 = obj->tpages[0];
        tp1 = obj->tpages[1];
        for (i = 0; i < obj->count; i++, line0++, line1++, tp0++, tp1++) {
            if (!semiTrans) {
                initLineG2Pair((s32 *)line0, (s32 *)line1, NULL, NULL, blend, NULL, NULL, 0, 1);
            } else {
                initLineG2Pair((s32 *)line0, (s32 *)line1, NULL, NULL, blend, tp0, tp1, 1, 1);
            }
        }
        break;
    case 9:
        if (semiTrans) {
            for (i = 0; i < 2; i++) {
                obj->tpages[i] = allocTaskHeapBlock(obj->count * sizeof(DrTPage));
            }
        }
        for (i = 0; i < 2; i++) {
            obj->g4s[i] = allocTaskHeapBlock(obj->count * sizeof(POLY_G4));
        }
        g4a = obj->g4s[0];
        g4b = obj->g4s[1];
        tp0 = obj->tpages[0];
        tp1 = obj->tpages[1];
        for (i = 0; i < obj->count; i++, g4a++, g4b++, tp0++, tp1++) {
            if (!semiTrans) {
                initPolyG4Pair(g4a, g4b, NULL, NULL, NULL, NULL, blend, NULL, NULL, NULL, 0, 1);
            } else {
                initPolyG4Pair(g4a, g4b, NULL, NULL, NULL, NULL, blend, tp0, tp1, NULL, 1, 1);
            }
        }
        break;
    case 12:
        for (i = 0; i < 2; i++) {
            obj->ft4s[i] = allocTaskHeapBlock(obj->count * sizeof(POLY_FT4));
        }
        ft4a = obj->ft4s[0];
        ft4b = obj->ft4s[1];
        for (i = 0; i < obj->count; i++, ft4a++, ft4b++) {
            if (!semiTrans) {
                initPolyFT4Pair(ft4a, ft4b, NULL, tpage, clut, NULL, NULL, 0, 1);
            } else {
                initPolyFT4Pair(ft4a, ft4b, NULL, tpage, clut, NULL, NULL, 1, 1);
            }
        }
        obj->uv = *uv;
        break;
    case 13:
        for (i = 0; i < 2; i++) {
            obj->gt4s[i] = allocTaskHeapBlock(obj->count * sizeof(POLY_GT4));
        }
        gt4a = obj->gt4s[0];
        gt4b = obj->gt4s[1];
        for (i = 0; i < obj->count; i++, gt4a++, gt4b++) {
            if (!semiTrans) {
                initPolyGT4Pair(gt4a, gt4b, NULL, NULL, NULL, NULL, tpage, clut, uv, NULL, 0, 1);
            } else {
                initPolyGT4Pair(gt4a, gt4b, NULL, NULL, NULL, NULL, tpage, clut, uv, NULL, 1, 1);
            }
        }
        obj->uv = *uv;
        break;
    }
}

void SUG_updateTrailUvs(TrailEffect *obj) {
    Rect16 uv;
    POLY_FT4 *ft4;
    POLY_GT4 *gt4;
    s32 i;

    SUG_tickTexAnim(obj->texAnim);
    uv = obj->uv;
    if (obj->texAnimId != 3) {
        switch (obj->primKind) {
        case 12:
            ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
            SUG_setGridUvsFT4(ft4, &uv, obj->count, 1, obj->count, 0, 0, 0);
            for (i = 0; i < obj->count; i++, ft4++) {
                ft4->tpage = obj->tpage;
                ft4->clut = obj->clut;
            }
            break;
        case 13:
            gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
            SUG_setGridUvsGT4(gt4, &uv, obj->count, 1, obj->count, 0, 0, 0);
            for (i = 0; i < obj->count; i++, gt4++) {
                gt4->tpage = obj->tpage;
                gt4->clut = obj->clut;
            }
            break;
        }
    } else {
        switch (obj->primKind) {
        case 12:
            ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
            for (i = 0; i < obj->count; i++, ft4++) {
                ft4->tpage = obj->tpage;
                ft4->clut = obj->clut;
                setPrimQuadUvRect((u8 *)ft4, uv.x, uv.y, uv.w - 1, uv.h - 1);
            }
            break;
        case 13:
            gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
            for (i = 0; i < obj->count; i++, gt4++) {
                gt4->tpage = obj->tpage;
                gt4->clut = obj->clut;
                setPrimQuadUvRect((u8 *)gt4, uv.x, uv.y, uv.w - 1, uv.h - 1);
            }
            break;
        }
    }
}


void SUG_shadeTrailPrims(TrailEffect *obj) {
    u8 c0[3];
    u8 c1[3];
    u8 c2[3];
    u8 c3[3];
    u8 *col;
    LineG2 *line;
    LineG2 *line2;
    POLY_G4 *g4;
    POLY_G4 *g42;
    POLY_FT4 *ft4;
    POLY_FT4 *ft42;
    POLY_GT4 *gt4;
    POLY_GT4 *gt42;
    s32 i;

    if (obj->level != obj->prevLevel) {
        switch (obj->primKind) {
        case 1:
            line = obj->lines[FRAME_BUFFER_INDEX];
            line2 = obj->lines[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, line++, line2++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                c1[0] = col[4] * obj->level / 256;
                c1[1] = col[5] * obj->level / 256;
                c1[2] = col[6] * obj->level / 256;
                setPrimRgb0(line, c0[0], c0[1], c0[2]);
                setPrimRgb1(line, c0[0], c0[1], c0[2]);
                setPrimRgb0(line2, c1[0], c1[1], c1[2]);
                setPrimRgb1(line2, c1[0], c1[1], c1[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
        case 9:
            g4 = obj->g4s[FRAME_BUFFER_INDEX];
            g42 = obj->g4s[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, g4++, g42++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                c1[0] = col[4] * obj->level / 256;
                c1[1] = col[5] * obj->level / 256;
                c1[2] = col[6] * obj->level / 256;
                c2[0] = col[8] * obj->level / 256;
                c2[1] = col[9] * obj->level / 256;
                c2[2] = col[10] * obj->level / 256;
                c3[0] = col[12] * obj->level / 256;
                c3[1] = col[13] * obj->level / 256;
                c3[2] = col[14] * obj->level / 256;
                setPrimRgb0(g4, c0[0], c0[1], c0[2]);
                setPrimRgb1(g4, c1[0], c1[1], c1[2]);
                setPrimRgb2(g4, c2[0], c2[1], c2[2]);
                setPrimRgb3(g4, c3[0], c3[1], c3[2]);
                setPrimRgb0(g42, c0[0], c0[1], c0[2]);
                setPrimRgb1(g42, c1[0], c1[1], c1[2]);
                setPrimRgb2(g42, c2[0], c2[1], c2[2]);
                setPrimRgb3(g42, c3[0], c3[1], c3[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
        case 12:
            ft4 = obj->ft4s[FRAME_BUFFER_INDEX];
            ft42 = obj->ft4s[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, ft4++, ft42++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                setPrimRgb0(ft4, c0[0], c0[1], c0[2]);
                setPrimRgb0(ft42, c0[0], c0[1], c0[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
        case 13:
            gt4 = obj->gt4s[FRAME_BUFFER_INDEX];
            gt42 = obj->gt4s[FRAME_BUFFER_INDEX ^ 1];
            col = obj->colors;
            for (i = 0; i < obj->count; i++, gt4++, gt42++) {
                c0[0] = col[0] * obj->level / 256;
                c0[1] = col[1] * obj->level / 256;
                c0[2] = col[2] * obj->level / 256;
                c1[0] = col[4] * obj->level / 256;
                c1[1] = col[5] * obj->level / 256;
                c1[2] = col[6] * obj->level / 256;
                c2[0] = col[8] * obj->level / 256;
                c2[1] = col[9] * obj->level / 256;
                c2[2] = col[10] * obj->level / 256;
                c3[0] = col[12] * obj->level / 256;
                c3[1] = col[13] * obj->level / 256;
                c3[2] = col[14] * obj->level / 256;
                setPrimRgb0(gt4, c0[0], c0[1], c0[2]);
                setPrimRgb1(gt4, c1[0], c1[1], c1[2]);
                setPrimRgb2(gt4, c2[0], c2[1], c2[2]);
                setPrimRgb3(gt4, c3[0], c3[1], c3[2]);
                setPrimRgb0(gt42, c0[0], c0[1], c0[2]);
                setPrimRgb1(gt42, c1[0], c1[1], c1[2]);
                setPrimRgb2(gt42, c2[0], c2[1], c2[2]);
                setPrimRgb3(gt42, c3[0], c3[1], c3[2]);
                if (obj->colorMode != 0) {
                    col += 16;
                }
            }
            break;
        }
        obj->prevLevel = obj->level;
    }
}

/* called with one argument more than the executable's prototype (dcb/prim_pair.h) */
void initPolyF4Pair();

ColorQuad *SUG_createFadeRect(Rect16 *rect, Color *color, Color *color2, u8 blend, s16 a4, u8 mode) {
    ColorQuad *quad;

    quad = allocTaskHeapBlock(sizeof(ColorQuad));
    initPolyF4Pair(&quad->poly[0], &quad->poly[1], color, blend, quad->tpage[0], quad->tpage[1], rect, 1, 1);
    quad->color = *color;
    quad->color2 = *color2;
    quad->step = a4;
    quad->mode = mode;
    if (mode < 2 || mode == 3) {
        quad->visible = 1;
    } else {
        quad->visible = 0;
    }
    return quad;
}

s32 SUG_tickFadeRect(ColorQuad *quad) {
    PolyF4 *poly;
    u32 *tpage;
    s32 fadedIn;
    s32 fadedOut;

    fadedIn = 0;
    fadedOut = 0;
    if (quad->visible == 0) {
        return -1;
    }
    poly = &quad->poly[FRAME_BUFFER_INDEX];
    tpage = quad->tpage[FRAME_BUFFER_INDEX];
    if (quad->visible == 1) {
        fadedIn = stepColorToward(quad->step, &poly->r0, quad->color2.r, &poly->g0, quad->color2.g, &poly->b0, quad->color2.b);
    } else if (quad->visible == 2) {
        fadedOut = stepColorToward(quad->step, &poly->r0, quad->color.r, &poly->g0, quad->color.g, &poly->b0, quad->color.b);
        if (fadedOut != 0 && quad->mode == 3) {
            quad->visible = 1;
        }
    }
    if (*(u32 *)&poly->r0 & 0xFFFFFF) {
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)poly);
        AddPrim((s32 *)&CURRENT_FRAME_BUFFER->ot[1], (s32)tpage);
    }
    if (fadedIn != 0 && (quad->mode == 1 || quad->mode == 3)) {
        quad->visible = 2;
    }
    if (fadedIn == 0 && fadedOut == 0) {
        return 0;
    }
    if (fadedIn != 0) {
        return 1;
    }
    if (fadedOut != 0) {
        return 2;
    }
    return -1;
}

ScrollTex *SUG_createScrollTexture(Rect16 *rect, s32 depth, s32 mode, s16 speed) {
    ScrollTex *tex;

    tex = allocTaskHeapBlock(sizeof(ScrollTex));
    tex->rect.x = rect->x;
    tex->rect.y = rect->y;
    tex->rect.w = rect->w / (4 - depth * 2);
    tex->rect.h = rect->h;
    tex->vertical = mode;
    tex->depth = depth;
    tex->speed = speed;
    tex->buf0 = allocTaskHeapBlock(rect->w / (u32)(2 - depth) * rect->h);
    tex->buf1 = allocTaskHeapBlock(rect->w / (u32)(2 - depth) * rect->h);
    tex->timer = tex->period = mode / 2 * 2;
    return tex;
}

void SUG_scrollTexture(ScrollTex *tex) {
    Rect16 rects[4];

    if (tex->speed == 0) {
        return;
    }
    if (++tex->timer < tex->period) {
        return;
    }
    tex->timer = 0;
    if (!(tex->vertical & 1)) {
        tex->speed = tex->speed % tex->rect.w;
        if (tex->speed > 0) {
            rects[0].x = tex->rect.x + tex->rect.w - tex->speed;
            rects[0].y = tex->rect.y;
            rects[0].w = tex->speed;
            rects[0].h = tex->rect.h;
            rects[1].x = tex->rect.x;
            rects[1].y = tex->rect.y;
            rects[1].w = tex->rect.w - tex->speed;
            rects[1].h = tex->rect.h;
            rects[2].x = tex->rect.x;
            rects[2].y = tex->rect.y;
            rects[2].w = tex->speed;
            rects[2].h = tex->rect.h;
            rects[3].x = tex->rect.x + tex->speed;
            rects[3].y = tex->rect.y;
            rects[3].w = tex->rect.w - tex->speed;
            rects[3].h = tex->rect.h;
        } else {
            rects[0].x = tex->rect.x;
            rects[0].y = tex->rect.y;
            rects[0].w = -tex->speed;
            rects[0].h = tex->rect.h;
            rects[1].x = tex->rect.x - tex->speed;
            rects[1].y = tex->rect.y;
            rects[1].w = tex->rect.w + tex->speed;
            rects[1].h = tex->rect.h;
            rects[2].x = tex->rect.x + tex->rect.w + tex->speed;
            rects[2].y = tex->rect.y;
            rects[2].w = -tex->speed;
            rects[2].h = tex->rect.h;
            rects[3].x = tex->rect.x;
            rects[3].y = tex->rect.y;
            rects[3].w = tex->rect.w + tex->speed;
            rects[3].h = tex->rect.h;
        }
    } else {
        tex->speed = tex->speed % tex->rect.h;
        if (tex->speed > 0) {
            rects[0].x = tex->rect.x;
            rects[0].y = tex->rect.y + tex->rect.h - tex->speed;
            rects[0].w = tex->rect.w;
            rects[0].h = tex->speed;
            rects[1].x = tex->rect.x;
            rects[1].y = tex->rect.y;
            rects[1].w = tex->rect.w;
            rects[1].h = tex->rect.h - tex->speed;
            rects[2].x = tex->rect.x;
            rects[2].y = tex->rect.y;
            rects[2].w = tex->rect.w;
            rects[2].h = tex->speed;
            rects[3].x = tex->rect.x;
            rects[3].y = tex->rect.y + tex->speed;
            rects[3].w = tex->rect.w;
            rects[3].h = tex->rect.h - tex->speed;
        } else {
            rects[0].x = tex->rect.x;
            rects[0].y = tex->rect.y;
            rects[0].w = tex->rect.w;
            rects[0].h = -tex->speed;
            rects[1].x = tex->rect.x;
            rects[1].y = tex->rect.y - tex->speed;
            rects[1].w = tex->rect.w;
            rects[1].h = tex->rect.h + tex->speed;
            rects[2].x = tex->rect.x;
            rects[2].y = tex->rect.y + tex->rect.h + tex->speed;
            rects[2].w = tex->rect.w;
            rects[2].h = -tex->speed;
            rects[3].x = tex->rect.x;
            rects[3].y = tex->rect.y;
            rects[3].w = tex->rect.w;
            rects[3].h = tex->rect.h + tex->speed;
        }
    }
    StoreImage(&rects[0], tex->buf1);
    StoreImage(&rects[1], tex->buf0);
    DrawSync(0);
    LoadImage((s16 *)&rects[2], (s32)tex->buf1);
    LoadImage((s16 *)&rects[3], (s32)tex->buf0);
    DrawSync(0);
}

void SUG_freeScrollTexture(void **obj) {
    obj[3] = (void *)freeHeapBlock(obj[3]);
    obj[2] = (void *)freeHeapBlock(obj[2]);
    freeHeapBlock(obj);
}

void SUG_initTamCache(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        SUG_TAM_CACHE[i].key = -1;
        SUG_TAM_CACHE[i].data = NULL;
    }
}

void *SUG_loadTamFile(s32 key, s32 *path, s32 sub, Chunk *pak) {
    void *data;
    s32 free;
    s32 i;

    free = -1;
    for (i = 0; i < 8; i++) {
        if (SUG_TAM_CACHE[i].key == key) {
            return SUG_TAM_CACHE[i].data;
        }
        if (free < 0 && SUG_TAM_CACHE[i].key < 0) {
            free = i;
        }
    }
    if (free >= 0) {
        data = findPakChunk(pak, 4, sub);
        if (data == NULL) {
            data = (void *)loadFileTagged(path, getCurrentTaskId(), 0x82);
            if (data != NULL) {
                SUG_TAM_CACHE[free].key = key;
                SUG_TAM_CACHE[free].data = data;
            }
        }
        return data;
    }
    return NULL;
}

const Rect16 SUG_MODEL_CLUT_RECT = { 0x30, 0x70, 0x10, 0x10 };


s32 SUG_startTexAnim(s32 id, s32 kind, RingEffect *obj, u8 *arg3, s32 pak) {
    Rect16 *uv;
    s32 slot;
    u8 y;
    s32 dx;
    s32 sub;
    char path[32];
    s16 n;
    ModelData *model;
    RingEffect *ring;
    SphereEffect *other;

    uv = NULL;
    slot = 0;
    /* the extra block is needed for the register allocation to match */
    do {
        y = 0;
        dx = 0;
    } while (0);
    sub = id;
    switch (kind) {
    case 0:
        model = (ModelData *)obj;
        if (model != NULL) {
            slot = model->unk26D4 / 0x10000 + 5;
            if (model->id > 1000) {
                n = model->id / 10;
                sprintf(path, "M:\\HDF%d\\%d_%d.tam", n, n, id);
                if (((ModelEffect *)model->owner)->unk574 == 0) {
                    y = 0x80;
                }
                sub = model->id;
            } else {
                sprintf(path, "M:\\HDF%03d\\%d.tam", model->id, id);
            }
            id |= model->id << 8;
        }
        break;
    case 1:
        ring = obj;
        if (ring->type == 0xD) {
            uv = (Rect16 *)&ring->texCoords;
            y = uv->y;
            dx = uv->x / 4;
            slot = ring->tpage;
            sprintf(path, "E:\\ANM\\%d_%d.tam", id / 10, id % 10);
        }
        break;
    case 3:
        other = (SphereEffect *)obj;
        if (other->kind == 0xD) {
            uv = &other->uv;
            y = uv->y;
            dx = uv->x / 4;
            slot = other->tpage;
            sprintf(path, "E:\\ANM\\%d_%d.tam", id / 10, id % 10);
        }
        break;
    case 4:
        if (((TrailEffect *)obj)->primKind == 12 || ((TrailEffect *)obj)->primKind == 13) {
            uv = &((TrailEffect *)obj)->uv;
            y = uv->y;
            slot = ((TrailEffect *)obj)->tpage;
            sprintf(path, "E:\\ANM\\%d_%d.tam", id / 10, id % 10);
        }
        break;
    }
    if (slot != 0) {
        ((TexAnim *)arg3)->timer = 0;
        ((TexAnim *)arg3)->frame = 0;
        ((TexAnim *)arg3)->rect.x = (slot & 0xF) << 6;
        ((TexAnim *)arg3)->rect.y = ((slot >> 4) << 8) + y;
        if (id >= 4) {
            ((TexAnim *)arg3)->header = SUG_loadTamFile(id, (s32 *)path, sub, (Chunk *)pak);
            if (((TexAnim *)arg3)->header == NULL) {
                return 0;
            }
            ((TexAnim *)arg3)->frames = (AnimFrame *)(((TexAnim *)arg3)->header + 1);
            ((TexAnim *)arg3)->rect.w = ((TexAnim *)arg3)->header->w;
            ((TexAnim *)arg3)->rect.h = ((TexAnim *)arg3)->header->h;
            ((TexAnim *)arg3)->pixels = NULL;
        } else {
            ((TexAnim *)arg3)->rect.w = (uv->w + 1) / 4;
            ((TexAnim *)arg3)->rect.h = uv->h + 1;
            ((TexAnim *)arg3)->pixels = allocTaskHeapBlock(((TexAnim *)arg3)->rect.w * 2 * ((TexAnim *)arg3)->rect.h + 4);
            StoreImage(&((TexAnim *)arg3)->rect, ((TexAnim *)arg3)->pixels);
            DrawSync(0);
        }
        ((TexAnim *)arg3)->dst = (s16 *)uv;
        ((TexAnim *)arg3)->type = id;
        if (!(((TexAnim *)arg3)->header->loop & 1)) {
            ((TexAnim *)arg3)->rect.x += dx;
        }
        return (s32)arg3;
    }
    return 0;
}

void SUG_tickTexAnim(u8 *arg) {
    TexAnim *image = (TexAnim *)arg;
    Rect16 rect;

    image->timer++;
    switch (image->type) {
    case 0:
        SUG_scrollTexAnimLeft(image);
        return;
    case 1:
        SUG_scrollTexAnimRight(image);
        return;
    case 2:
        SUG_scrollTexAnimUp(image);
        return;
    case 3:
        SUG_scrollTexAnimDown(image);
        return;
    }
    if (image->timer < image->frames[image->frame].duration) {
        return;
    }
    image->timer = 0;
    image->frame++;
    if (image->frame >= image->header->count) {
        image->frame = image->header->loop >> 1;
        if (image->frame == 0x7F) {
            image->frame--;
        }
    }
    if (!(image->header->loop & 1)) {
        image->dst[0] = image->frames[image->frame].u + ((image->rect.x * 4) & 0xFF);
        image->dst[1] = (image->rect.y & 0xFF) + image->frames[image->frame].v;
    } else {
        rect.x = image->frames[image->frame].u + image->rect.x;
        rect.y = image->frames[image->frame].v + image->rect.y;
        rect.w = image->rect.w;
        rect.h = image->rect.h;
        MoveImage2(&rect, image->rect.x + image->frames[0].u, image->rect.y + image->frames[0].v);
        DrawSync(0);
    }
}

void SUG_freeTamCache(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (SUG_TAM_CACHE[i].key != -1) {
            freeHeapBlock(SUG_TAM_CACHE[i].data);
            SUG_TAM_CACHE[i].key = -1;
            SUG_TAM_CACHE[i].data = NULL;
        }
    }
}

void SUG_freeTexAnim(u8 *obj) {
    void *ptr = *(void **)(obj + 0x1C);

    if (ptr != NULL) {
        freeHeapBlock(ptr);
    }
}

void SUG_scrollTexAnimLeft(TexAnim *image) {
    TexAnim *self;
    u8 *row;
    u8 *p;
    s8 first;
    s32 x;
    s32 y;

    self = image;
    row = image->pixels;
    for (y = 0; y < image->rect.h; y++, row += image->rect.w * 2) {
        first = row[0];
        p = row;
        for (x = 0; x < image->rect.w * 2 - 1; x++) {
            p[0] = p[1];
            p++;
        }
        *p = first;
    }
    LoadImage2(&image->rect, self->pixels);
    DrawSync(0);
}

void SUG_scrollTexAnimRight(TexAnim *image) {
    TexAnim *self;
    u8 *row;
    u8 *p;
    s8 first;
    s32 x;
    s32 y;

    self = image;
    y = 0;
    row = image->pixels;
    for (; y < image->rect.h; y++, row += image->rect.w * 2) {
        p = row + image->rect.w * 2 - 1;
        first = *p;
        for (x = 0; x < image->rect.w * 2 - 1; x++) {
            p[0] = p[-1];
            p--;
        }
        *p = first;
    }
    LoadImage2(&image->rect, self->pixels);
    DrawSync(0);
}

void SUG_scrollTexAnimUp(TexAnim *image) {
    Rect16 top;
    Rect16 bottom;
    s32 shift;

    shift = image->timer % image->rect.h;
    if (shift != 0) {
        bottom = image->rect;
        top = bottom;
        top.h -= shift;
        LoadImage2(&top, image->pixels + image->rect.w * (shift << 1));
        bottom.y += image->rect.h - shift;
        bottom.h = shift;
        LoadImage2(&bottom, image->pixels);
        DrawSync(0);
    } else {
        LoadImage2(&image->rect, image->pixels);
        DrawSync(0);
    }
}

void SUG_scrollTexAnimDown(TexAnim *image) {
    Rect16 top;
    Rect16 bottom;
    s32 shift;

    shift = image->timer % image->rect.h;
    if (shift != 0) {
        bottom = image->rect;
        top = bottom;
        top.h = shift;
        LoadImage2(&top, image->pixels + image->rect.w * ((image->rect.h - shift) << 1));
        bottom.y += shift;
        bottom.h -= shift;
        LoadImage2(&bottom, image->pixels);
        DrawSync(0);
    } else {
        LoadImage2(&image->rect, image->pixels);
        DrawSync(0);
    }
}

LightMotion *SUG_createLightMotion(VECTOR *pos, VECTOR *posTo, VECTOR *color, VECTOR *colorTo, s32 light, s32 period, s32 duration) {
    LightMotion *motion;

    motion = allocTaskHeapBlock(sizeof(LightMotion));
    if (period == 0) {
        period = 1;
    }
    motion->pos = *pos;
    motion->posStep.vx = (posTo->vx - pos->vx) / period;
    motion->posStep.vy = (posTo->vy - pos->vy) / period;
    motion->posStep.vz = (posTo->vz - pos->vz) / period;
    motion->color = *color;
    motion->colorStep.vx = (colorTo->vx - color->vx) / period;
    motion->colorStep.vy = (colorTo->vy - color->vy) / period;
    motion->colorStep.vz = (colorTo->vz - color->vz) / period;
    motion->light = light;
    motion->duration = duration;
    motion->period = period;
    motion->frame = 0;
    return motion;
}

void SUG_tickLightMotion(LightMotion *motion) {
    s32 t;

    if (motion->frame < motion->duration) {
        t = motion->frame % motion->period;
        SCENE_LIGHT_MATRIX.m[motion->light][0] = motion->pos.vx + t * motion->posStep.vx;
        SCENE_LIGHT_MATRIX.m[motion->light][1] = motion->pos.vy + t * motion->posStep.vy;
        SCENE_LIGHT_MATRIX.m[motion->light][2] = motion->pos.vz + t * motion->posStep.vz;
        SCENE_LIGHT_COLORS.m[0][motion->light] = motion->color.vx + t * motion->colorStep.vx;
        SCENE_LIGHT_COLORS.m[1][motion->light] = motion->color.vy + t * motion->colorStep.vy;
        SCENE_LIGHT_COLORS.m[2][motion->light] = motion->color.vz + t * motion->colorStep.vz;
        if (++motion->frame == motion->duration) {
            SCENE_LIGHT_MATRIX = SUG_DEFAULT_LIGHT_MATRIX;
            SCENE_LIGHT_COLORS = SUG_DEFAULT_LIGHT_COLORS;
        }
    }
}

void SUG_freeLightMotion(void *obj) {
    SCENE_LIGHT_MATRIX = SUG_DEFAULT_LIGHT_MATRIX;
    SCENE_LIGHT_COLORS = SUG_DEFAULT_LIGHT_COLORS;
    freeHeapBlock(obj);
}

void SUG_getEffectOtz(Unk801E7BEC *cmd, Unk801E7BEC_Dst *dst) {
    switch (cmd->kind) {
    case 10:
        dst->value = *(s32 *)(cmd->source + 0x1B4);
        break;
    case 12:
        dst->value = *(s32 *)(cmd->source + 0x2C4);
        break;
    case 13:
        dst->value = *(s32 *)(cmd->source + 0x194);
        break;
    case 16:
        dst->value = cmd->source[0x1BD];
        break;
    case 17:
        dst->value = *(s16 *)(cmd->source + 0x154);
        break;
    }
}

void SUG_setEffectOtz(Unk801E7C94 *cmd, Unk801E7C94_Src *src) {
    switch (cmd->kind) {
    case 10:
        *(s32 *)(cmd->target + 0x1B4) = src->value.w;
        break;
    case 12:
        *(s32 *)(cmd->target + 0x2C4) = src->value.w;
        break;
    case 13:
        *(s32 *)(cmd->target + 0x194) = src->value.w;
        break;
    case 16:
        cmd->target[0x1BD] = src->value.b;
        break;
    case 17:
        *(s16 *)(cmd->target + 0x154) = src->value.h;
        break;
    }
}

void SUG_getEffectParams(EffectInit *fx, EffectCommand *cmd, s32 live);
typedef struct Xform {
    u8 unk0[0x48];
    struct Xform *parent;
} Xform;
void GsGetLw(GsCOORDINATE2 *coord, MATRIX *out);

void SUG_detachEffectToWorld(EffectSlots *slots, s32 id, EffectCommand *cmd) {
    SVECTOR v;
    SVECTOR unused;
    VECTOR out;
    MATRIX m;
    Xform *chain[18];
    Xform saved;
    s32 flag;
    Model *model;
    GsCOORDINATE2 *coord;
    Xform *xform;
    s32 i;
    s32 n;
    Xform **p;

    memset(&unused, 0, sizeof(unused));
    model = SCENE_3D->models[slots->modelSlots[cmd->source]];
    coord = model->coord;
    SUG_getEffectParams((EffectInit *)slots->slots[id].value, cmd, 1);
    PushMatrix();
    if (cmd->source >= 0) {
        SCENE_3D->root.flg = model->root.flg = 0;
        SCENE_3D->root.coord = D_801DBEC0;
        model->root.coord = D_801DBEC0;
        for (i = 0; i < model->nobj; i++, coord++) {
            coord->flg = 0;
        }
        GsGetLw(((Model *)SCENE_3D->models[slots->modelSlots[cmd->source]])->obj[cmd->target].coord2, &m);
        SetRotMatrix((s32)&m);
        func_8005C444(&m);
    } else {
        xform = (Xform *)slots->slots[id].value;
        saved = *(Xform *)slots->xform;
        initTransform(slots->xform, 0, 0, 0, 0, 0, 0, 0);
        n = 0;
        p = chain;
        chain[0] = xform;
        if (xform != (Xform *)slots->xform) {
            do {
                xform = xform->parent;
                n++;
                if (n >= 16) {
                    break;
                }
                *(Xform **)((s32)p + (n << 2)) = xform;
            } while (xform != (Xform *)slots->xform);
        }
        for (i = n; i > 0; i--) {
            updateTransformMatrix(chain[i], 0);
        }
        *(Xform *)slots->xform = saved;
    }
    v.vx = cmd->pos[0];
    v.vy = cmd->pos[1];
    v.vz = cmd->pos[2];
    RotTrans((u16 *)&v, &out, &flag);
    cmd->pos[0] = out.vx;
    cmd->pos[1] = out.vy;
    cmd->pos[2] = out.vz;
    v.vx = cmd->pos2[0];
    v.vy = cmd->pos2[1];
    v.vz = cmd->pos2[2];
    RotTrans((u16 *)&v, &out, &flag);
    cmd->pos2[0] = out.vx;
    cmd->pos2[1] = out.vy;
    cmd->pos2[2] = out.vz;
    PopMatrix();
    cmd->target = -1;
    SUG_initEffectFromParams((EffectTemplate *)slots->slots[id].value, cmd, slots);
    initEffectObject((void *)slots->slots[id].value);
}

void SUG_uploadEffectTim(Unk801E8168 *obj, s32 dy, Chunk *pak) {
    char path[32];
    u32 *tim;
    s32 loaded = 0;

    tim = findPakChunk(pak, 5, obj->animId);
    if (tim == NULL) {
        sprintf(path, "E:\\ANM\\%d_%d.TIM", obj->animId / 10, obj->animId % 10);
        tim = (u32 *)loadFile(path, getCurrentTaskId());
        loaded = 1;
        if (tim == NULL) {
            return;
        }
    }
    uploadTim(tim, obj->pixelX, obj->pixelY + dy, obj->clutX, obj->clutY + dy);
    DrawSync(0);
    if (loaded) {
        freeHeapBlock(tim);
    }
}

void SUG_uploadTimFile(s32 id, Chunk *pak) {
    char path[32];
    u32 *tims;
    s32 loaded = 0;

    tims = findPakChunk(pak, 5, id);
    if (tims == NULL) {
        sprintf(path, "E:\\TIM\\%04d.TIM", id);
        tims = (u32 *)loadFile(path, getCurrentTaskId());
        loaded = 1;
        if (tims == NULL) {
            return;
        }
    }
    uploadTimList(tims);
    if (loaded) {
        freeHeapBlock(tims);
    }
}

s32 SUG_loadEffectPak(s32 id) {
    char path[32];
    s32 file;

    sprintf(path, "E:\\%d.PAK", id);
    file = loadFileTagged((s32 *)path, getCurrentTaskId(), 0x12C);
    if (file != 0) {
        return file;
    }
    return 0;
}

void SUG_initEffectRoot(void) {
    RootEffect fx;

    fx.pos[0] = 100;
    fx.pos[1] = -150;
    fx.pos[2] = 9000;
    fx.pos2[0] = 100;
    fx.pos2[1] = -150;
    fx.pos2[2] = 9000;
    fx.rot[0] = 160;
    fx.rot[1] = 5800;
    fx.rot[2] = 0;
    fx.rot2[0] = 0;
    fx.rot2[1] = 0;
    fx.rot2[2] = 0;
    fx.rotAccel[0] = 0;
    fx.rotAccel[1] = 0;
    fx.rotAccel[2] = 0;
    fx.scale[0] = 0x1000;
    fx.scale[1] = 0x1000;
    fx.scale[2] = 0x1000;
    fx.scale2[0] = 0x1000;
    fx.scale2[1] = 0x1000;
    fx.scale2[2] = 0x1000;
    fx.scaleStep[0] = 0;
    fx.scaleStep[1] = 0;
    fx.scaleStep[2] = 0;
    fx.fadeMode = 0;
    fx.speed = 0;
    fx.hitRadius = 0x80;
    fx.parent = SCENE_3D->unk78;
    fx.mode = 0;
    SUG_EFFECT_ROOT = fx;
    initEffectObject(&SUG_EFFECT_ROOT);
}

void SUG_initEffectSlots(EffectScript *obj) {
    s32 i;

    obj->slots = allocTaskHeapBlock(sizeof(EffectSlots));
    for (i = 0; i < 150; i++) {
        obj->slots->slots[i].id = -1;
        obj->slots->slots[i].active = 0;
        obj->slots->slots[i].value = 0;
    }
    obj->slots->count = obj->waitFrames = 0;
    obj->unk14 = 0;
    obj->slots->pak = 0;
}

extern void (*SUG_EFFECT_TICK_FUNCS[])(u8 *value);

s32 SUG_tickEffectScript(EffectScript *runner) {
    EffectSlots *slots;
    void (*fn)(u8 *);
    s32 i;

    slots = runner->slots;
    i = 0;
    PushMatrix();
    tickEffectMotion((s32)&SUG_EFFECT_ROOT, 0);
    updateTransformMatrix(slots->xform, 0);
    PopMatrix();
    runner->regs[0] = 1;
    SUG_runEffectScript(runner);
    for (; i < 150; i++) {
        if (slots->slots[i].active != 0) {
            fn = SUG_EFFECT_TICK_FUNCS[slots->slots[i].id];
            if (fn != NULL) {
                fn((u8 *)slots->slots[i].value);
                if (slots->slots[i].id >= 10) {
                    runner->regs[i + 107] = *(s32 *)(slots->slots[i].value + 0x118);
                    runner->regs[i + 257] = *(s32 *)(slots->slots[i].value + 0x11C);
                }
            }
        }
    }
    return runner->regs[0];
}

void SUG_freeEffectSlots(void *obj) {
    SUG_freeEffectEntries(obj);
    freeHeapBlock(obj);
}

extern u8 D_800795A8;
/* BoneAnim with its nine channels as three groups of x, y, z */
typedef struct {
    AnimChan ch[3][3];
} BoneChannels;
/* takes a third argument, but this caller does not set it */
void SUG_setEffectParams();
void SUG_getEffectWorldPos(void *xform, u8 *obj);
void SUG_createEffectEntry(s32 index, s32 kind, s32 arg, EffectSlots *slots);
void SUG_startScreenCopyEffect(u8 *obj, s32 clearColor);

void SUG_runEffectScript(EffectScript *runner) {
    EffectSlots *slots;
    s32 *regs;
    s32 result;
    s32 anim;
    s32 tpage;
    s32 vramY;
    u8 *prev;
    u8 *next;
    BoneChannels *src;
    BoneChannels *dst;
    s32 i;

    slots = runner->slots;
    if (runner->waitFrames != 0) {
        runner->waitFrames--;
        return;
    }
    regs = runner->regs;
    do {
        result = runScriptToNextEvent(runner->context, regs);
        if (result == 1) {
            switch (((Script *)runner->context)->eventOp) {
            case 10:
                switch (((Script *)runner->context)->eventArg) {
                case 0:
                    SUG_getEffectParams((EffectInit *)&SUG_EFFECT_ROOT, (EffectCommand *)regs, 0);
                    break;
                case 1:
                    SUG_setEffectParams(&SUG_EFFECT_ROOT, regs);
                    restartEffectMotion((u8 *)&SUG_EFFECT_ROOT);
                    break;
                case 2:
                    SCREEN_COPY_EFFECT.abr = SUG_SCREEN_FX_PHASE = 0;
                    SCREEN_COPY_EFFECT.mode = 1;
                    break;
                case 3:
                    SUG_SCREEN_FX_PHASE = SCREEN_COPY_EFFECT.abr = SCREEN_COPY_EFFECT.x = SCREEN_COPY_EFFECT.y = SCREEN_COPY_EFFECT.mode = 0;
                    SCREEN_COPY_EFFECT.r = 0xA8;
                    SCREEN_COPY_EFFECT.g = 0xA8;
                    SCREEN_COPY_EFFECT.b = 0xA8;
                    break;
                case 4:
                    D_800795A8 = 0;
                    break;
                case 5:
                    D_800795A8 = 1;
                    break;
                case 6:
                    SUG_uploadEffectTim((Unk801E8168 *)regs, slots->modelSlots[0] << 8, (Chunk *)slots->pak);
                    break;
                case 7:
                    pauseModelAnimation(slots->modelSlots[((EffectCommand *)regs)->source]);
                    break;
                case 8:
                    resumeModelAnimation(slots->modelSlots[((EffectCommand *)regs)->source]);
                    break;
                case 9:
                    ((EffectCommand *)regs)->rot[1] = -computeVectorAngle(((EffectCommand *)regs)->pos[0] - ((EffectCommand *)regs)->pos2[0],
                                                                          ((EffectCommand *)regs)->pos2[2] - ((EffectCommand *)regs)->pos[2]);
                    break;
                case 10:
                    SUG_startScreenCopyEffect((u8 *)regs, 0);
                    break;
                }
                break;
            case 11:
                switch (((Script *)runner->context)->eventArg) {
                case 0:
                    if ((s16)((Script *)runner->context)->params[0] >= 0) {
                        SUG_uploadTimFile((s16)((Script *)runner->context)->params[0], (Chunk *)slots->pak);
                    }
                    break;
                case 1:
                    setModelAnimationPose(slots->modelSlots[((EffectCommand *)regs)->source], (s16)((Script *)runner->context)->params[0]);
                    break;
                case 2:
                    anim = (s16)((Script *)runner->context)->params[0];
                    if (anim == 4 && SUG_TARGET_HP[slots->modelSlots[((EffectCommand *)regs)->source]] <= 0) {
                        anim = 5;
                    }
                    playModelAnimation(slots->modelSlots[((EffectCommand *)regs)->source], anim);
                    break;
                case 3:
                    playSoundEffect((s16)((Script *)runner->context)->params[0]);
                    break;
                case 5:
                    SUG_getEffectOtz((Unk801E7BEC *)&slots->slots[(s16)((Script *)runner->context)->params[0]], (Unk801E7BEC_Dst *)regs);
                    SUG_getEffectParams((EffectInit *)slots->slots[(s16)((Script *)runner->context)->params[0]].value, (EffectCommand *)regs, 0);
                    break;
                case 6:
                    SUG_initEffectFromParams((EffectTemplate *)slots->slots[(s16)((Script *)runner->context)->params[0]].value, (EffectCommand *)regs, slots);
                    initEffectObject((void *)slots->slots[(s16)((Script *)runner->context)->params[0]].value);
                    SUG_setEffectOtz((Unk801E7C94 *)&slots->slots[(s16)((Script *)runner->context)->params[0]], (Unk801E7C94_Src *)regs);
                    break;
                case 7:
                    stopSoundVoice(((Script *)runner->context)->params[0]);
                    break;
                case 8:
                    if (slots->slots[(s16)((Script *)runner->context)->params[0]].value != 0) {
                        slots->slots[(s16)((Script *)runner->context)->params[0]].active = 1;
                    }
                    break;
                case 9:
                    slots->slots[(s16)((Script *)runner->context)->params[0]].active = 0;
                    break;
                case 10:
                    if (runner->unk14 == 0) {
                        if (runner->regs[0] != -1) {
                            break;
                        }
                        runner->unk14 = 1;
                    }
                    runner->waitFrames = (s16)((Script *)runner->context)->params[0] - 1;
                    return;
                case 13:
                    SUG_getEffectWorldPos((void *)slots->slots[(s16)((Script *)runner->context)->params[0]].value, (u8 *)regs);
                    break;
                case 14:
                    switch ((s16)((Script *)runner->context)->params[0]) {
                    case 2:
                        D_80079584 = -1;
                        SCENE_3D->modelState[0] = SCENE_3D->modelState[1] = 1;
                        break;
                    case 0:
                        if (((Graphics *)&GRAPHICS)->targetModel > 0
                            && *(s16 *)((u8 *)SCENE_3D->models[((Graphics *)&GRAPHICS)->targetModel] + 6) > 1000) {
                            prev = SCENE_3D->models[((Graphics *)&GRAPHICS)->targetModel];
                            ((Graphics *)&GRAPHICS)->targetModel = slots->modelSlots[((EffectCommand *)regs)->source];
                            next = SCENE_3D->models[((Graphics *)&GRAPHICS)->targetModel];
                            src = (BoneChannels *)(prev + 0xD80) + *(s16 *)(prev + 4);
                            dst = (BoneChannels *)(next + 0xD80) + *(s16 *)(next + 4);
                            for (i = 0; i < 3; i++) {
                                dst->ch[0][i].value = src->ch[0][i].value;
                                dst->ch[1][i].value = src->ch[1][i].value;
                                dst->ch[2][i].value = src->ch[2][i].value;
                                dst->ch[0][i].accel0 = dst->ch[1][i].accel0 = dst->ch[2][i].accel0 =
                                    dst->ch[0][i].accel1 = dst->ch[1][i].accel1 = dst->ch[2][i].accel1 =
                                    dst->ch[0][i].velocity = dst->ch[1][i].velocity = dst->ch[2][i].velocity = 0;
                            }
                            *(s16 *)((u8 *)dst + 0x42) += *(s16 *)(prev + 0xC);
                            *(s16 *)((u8 *)dst + 0x52) += *(s16 *)(prev + 0x10);
                        } else {
                            D_80079584 = slots->modelSlots[((EffectCommand *)regs)->source];
                        }
                        SCENE_3D->modelState[((Graphics *)&GRAPHICS)->targetModel] = 1;
                        SCENE_3D->modelState[(s16)(((Graphics *)&GRAPHICS)->targetModel ^ 1)] = -1;
                        *(s16 *)((Graphics *)&GRAPHICS)->pad96 = 0;
                        break;
                    case 1:
                        ((Graphics *)&GRAPHICS)->targetModel = (s8)((u8 *)slots->slots[((EffectCommand *)regs)->source].value)[0x56E];
                        *(s16 *)((Graphics *)&GRAPHICS)->pad96 = slots->modelSlots[0];
                        break;
                    }
                    break;
                case 15:
                    SUG_getEffectOtz((Unk801E7BEC *)&slots->slots[(s16)((Script *)runner->context)->params[0]], (Unk801E7BEC_Dst *)regs);
                    SUG_getEffectParams((EffectInit *)slots->slots[(s16)((Script *)runner->context)->params[0]].value, (EffectCommand *)regs, 1);
                    break;
                case 16:
                    slots->pak = SUG_loadEffectPak((s16)((Script *)runner->context)->params[0]);
                    break;
                case 17:
                    SUG_detachEffectToWorld(slots, (s16)((Script *)runner->context)->params[0], (EffectCommand *)regs);
                    break;
                case 18:
                    SUG_startScreenCopyEffect((u8 *)regs, regs[30]);
                    if ((SCREEN_COPY_EFFECT.mode = ((Script *)runner->context)->params[0]) >= 2) {
                        SCREEN_COPY_EFFECT.abr = regs[30];
                    }
                    break;
                case 19:
                    SUG_shadeModelClut(slots->modelSlots[((EffectCommand *)regs)->source], (s16)((Script *)runner->context)->params[0]);
                    break;
                }
                break;
            case 12:
                switch (((Script *)runner->context)->eventArg) {
                case 0:
                    SUG_createEffectEntry((s16)((Script *)runner->context)->params[0], (s16)((Script *)runner->context)->params[1], (s32)regs, slots);
                    break;
                case 5:
                    ((EffectInit *)slots->slots[(s16)((Script *)runner->context)->params[0]].value)->mode = ((Script *)runner->context)->params[1];
                    break;
                case 4:
                    regs[1] = computeVectorAngle((s16)((Script *)runner->context)->params[0], (s16)((Script *)runner->context)->params[1]);
                    break;
                case 1:
                    regs[1] = rsin((s16)((Script *)runner->context)->params[1]) * (s16)((Script *)runner->context)->params[0] / 4096;
                    break;
                case 2:
                    regs[1] = rcos((s16)((Script *)runner->context)->params[1]) * (s16)((Script *)runner->context)->params[0] / 4096;
                    break;
                case 3:
                    playSoundEffectOnVoice((s16)((Script *)runner->context)->params[0], (s16)((Script *)runner->context)->params[1]);
                    break;
                }
                break;
            case 13:
                if (((Script *)runner->context)->eventArg == 0) {
                    tpage = (s16)((Script *)runner->context)->params[1];
                    vramY = ((tpage & 0x10) << 4) + ((s16)((Script *)runner->context)->params[2] << 7);
                    SUG_loadSprite((s16)((Script *)runner->context)->params[0], (tpage & 0xF) << 6, vramY + (slots->modelSlots[0] << 8),
                                  slots->pak);
                }
                break;
            }
        }
        clearScriptBusy(runner->context);
    } while (result != 0);
}

void SUG_getEffectParams(EffectInit *fx, EffectCommand *cmd, s32 live) {
    if (live == 0) {
        cmd->pos[0] = fx->pos[0];
        cmd->pos[1] = fx->pos[1];
        cmd->pos[2] = fx->pos[2];
        cmd->rot[0] = fx->rot[0];
        cmd->rot[1] = fx->rot[1];
        cmd->rot[2] = fx->rot[2];
        cmd->scale[0] = fx->scale[0];
        cmd->scale[1] = fx->scale[1];
        cmd->scale[2] = fx->scale[2];
    } else {
        cmd->pos[0] = fx->livePos[0];
        cmd->pos[1] = fx->livePos[1];
        cmd->pos[2] = fx->livePos[2];
        cmd->rot[0] = fx->liveRot[0];
        cmd->rot[1] = fx->liveRot[1];
        cmd->rot[2] = fx->liveRot[2];
        cmd->scale[0] = fx->liveScale[0];
        cmd->scale[1] = fx->liveScale[1];
        cmd->scale[2] = fx->liveScale[2];
    }
    cmd->pos2[0] = fx->pos2[0];
    cmd->pos2[1] = fx->pos2[1];
    cmd->pos2[2] = fx->pos2[2];
    cmd->moveSpeed = fx->moveSpeed;
    cmd->moveAccel = fx->moveAccel;
    cmd->rot2[0] = fx->rot2[0];
    cmd->rot2[1] = fx->rot2[1];
    cmd->rot2[2] = fx->rot2[2];
    cmd->rotAccel[0] = fx->rotAccel[0];
    cmd->rotAccel[1] = fx->rotAccel[1];
    cmd->rotAccel[2] = fx->rotAccel[2];
    cmd->scale2[0] = fx->scale2[0];
    cmd->scale2[1] = fx->scale2[1];
    cmd->scale2[2] = fx->scale2[2];
    cmd->scaleStep[0] = fx->scaleStep[0];
    cmd->scaleStep[1] = fx->scaleStep[1];
    cmd->scaleStep[2] = fx->scaleStep[2];
    cmd->hitRadius = fx->hitRadius;
    cmd->period = fx->period;
    cmd->fadeMode = fx->fadeMode;
    cmd->speed = fx->speed;
    cmd->wavePhase = fx->wavePhase;
    cmd->waveAmplitude = fx->waveAmplitude;
    cmd->waveFreq = fx->waveFreq;
    cmd->mode = fx->mode;
    cmd->source = fx->source;
    cmd->target = fx->target;
}

void SUG_setEffectParams(EffectInit *fx, EffectCommand *cmd, void *ctx) {
    fx->pos[0] = cmd->pos[0];
    fx->pos[1] = cmd->pos[1];
    fx->pos[2] = cmd->pos[2];
    fx->pos2[0] = cmd->pos2[0];
    fx->pos2[1] = cmd->pos2[1];
    fx->pos2[2] = cmd->pos2[2];
    fx->moveSpeed = cmd->moveSpeed;
    fx->moveAccel = cmd->moveAccel;
    fx->rot[0] = cmd->rot[0];
    fx->rot[1] = cmd->rot[1];
    fx->rot[2] = cmd->rot[2];
    fx->rot2[0] = cmd->rot2[0];
    fx->rot2[1] = cmd->rot2[1];
    fx->rot2[2] = cmd->rot2[2];
    fx->rotAccel[0] = cmd->rotAccel[0];
    fx->rotAccel[1] = cmd->rotAccel[1];
    fx->rotAccel[2] = cmd->rotAccel[2];
    fx->scale[0] = cmd->scale[0];
    fx->scale[1] = cmd->scale[1];
    fx->scale[2] = cmd->scale[2];
    fx->scale2[0] = cmd->scale2[0];
    fx->scale2[1] = cmd->scale2[1];
    fx->scale2[2] = cmd->scale2[2];
    fx->scaleStep[0] = cmd->scaleStep[0];
    fx->scaleStep[1] = cmd->scaleStep[1];
    fx->scaleStep[2] = cmd->scaleStep[2];
    fx->hitRadius = cmd->hitRadius;
    fx->period = cmd->period;
    fx->fadeMode = cmd->fadeMode;
    fx->speed = cmd->speed;
    fx->wavePhase = cmd->wavePhase;
    fx->waveAmplitude = cmd->waveAmplitude;
    fx->waveFreq = cmd->waveFreq;
    fx->mode = cmd->mode;
}

void SUG_getEffectWorldPos(void *xform, u8 *obj) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    *(s32 *)(obj + 0x65C) = pos.vx;
    *(s32 *)(obj + 0x660) = pos.vy;
    *(s32 *)(obj + 0x664) = pos.vz;
}

void SUG_initEffectFromParams(EffectTemplate *template, EffectCommand *cmd, EffectSlots *ctx) {
    SUG_setEffectParams((EffectInit *)template, cmd, ctx);
    ((u8 *)template)[0x13B] = cmd->source;
    ((u8 *)template)[0x13A] = cmd->target;
    if (cmd->target < 0) {
        template->data[0x26] = (s32)ctx->xform;
    } else if (cmd->source < 0) {
        if (cmd->source == -1) {
            template->data[0x26] = ctx->slots[cmd->target].value;
        } else {
            template->data[0x26] = (s32)((ModelEffect *)ctx->slots[-cmd->source - 2].value)->model->unk22B0[cmd->target];
        }
    } else {
        template->data[0x26] = (s32)((ModelData *)SCENE_3D->models[ctx->modelSlots[cmd->source]])->unk22B0[cmd->target];
    }
}

void SUG_createFadeRectFromParams(u8 *obj) {
    Rect16 rect;
    Color from;
    Color to;

    rect.x = *(s32 *)(obj + 0x140);
    rect.y = *(s32 *)(obj + 0x144);
    rect.w = *(s32 *)(obj + 0x148);
    rect.h = *(s32 *)(obj + 0x14C);
    from.r = *(s32 *)(obj + 0xA4);
    from.g = *(s32 *)(obj + 0xA8);
    from.b = *(s32 *)(obj + 0xAC);
    to.r = *(s32 *)(obj + 0xB0);
    to.g = *(s32 *)(obj + 0xB4);
    to.b = *(s32 *)(obj + 0xB8);
    SUG_createFadeRect(&rect, &from, &to, *(s32 *)(obj + 0x78), *(s32 *)(obj + 0x80), *(s32 *)(obj + 0x120));
}

void SUG_createScrollTextureFromParams(u8 *a, u8 *b) {
    Rect16 rect;

    rect.x = *(s32 *)(a + 0x150);
    rect.y = *(s32 *)(a + 0x154) + *(s32 *)(b + 0x4B0) * 256;
    rect.w = *(s32 *)(a + 0x158);
    rect.h = *(s32 *)(a + 0x15C);
    SUG_createScrollTexture(&rect, *(s32 *)(a + 0x70), *(s32 *)(a + 0x120), *(s16 *)(a + 0x80));
}

void SUG_createSphereFromParams(SpriteCommand *cmd, EffectSlots *ctx) {
    u8 color[3];
    Rect16 uv;
    EffectTemplate template;
    s32 tpage;
    s32 x;
    s32 y;
    s16 u;
    s32 v;

    SUG_initEffectFromParams(&template, (EffectCommand *)cmd, ctx);
    color[0] = cmd->r;
    color[1] = cmd->g;
    color[2] = cmd->b;
    x = cmd->texX;
    u = x & 0x3F;
    y = (ctx->modelSlots[0] << 8) + cmd->texY;
    v = y & 0xFF;
    switch (cmd->texMode) {
    case 0:
        u <<= 2;
        break;
    case 1:
        u <<= 1;
        break;
    }
    uv.x = u;
    uv.y = v;
    uv.w = cmd->texW;
    uv.h = cmd->texH;
    tpage = GetTPage(cmd->texMode, cmd->abr, cmd->texX, (ctx->modelSlots[0] << 8) + cmd->texY);
    SUG_createSphereEffect(cmd->unkEC, color, cmd->unk68, cmd->unk6C, &template, cmd->unk64, cmd->unk60, cmd->unkF8, cmd->unk74, cmd->abr,
                  cmd->unk12C, cmd->unk5C, cmd->unk130, &uv, tpage, func_80067644(cmd->clutX, (ctx->modelSlots[0] << 8) + cmd->clutY),
                  cmd->unk138, cmd->unk13C, ctx->pak);
}

void SUG_createTrailFromParams(SpriteCommand *cmd, EffectSlots *ctx) {
    Bytes4 c0;
    Bytes4 c1;
    Bytes4 c2;
    Bytes4 c3;
    Rect16 uv;
    EffectTemplate template;
    s32 tpage;
    s32 x;
    s32 y;
    s16 u;
    s32 v;

    SUG_initEffectFromParams(&template, (EffectCommand *)cmd, ctx);
    c0.b[0] = cmd->r;
    c0.b[1] = cmd->g;
    c0.b[2] = cmd->b;
    c1.b[0] = cmd->midR;
    c1.b[1] = cmd->midG;
    c1.b[2] = cmd->midB;
    c2.b[0] = cmd->outerR;
    c2.b[1] = cmd->outerG;
    c2.b[2] = cmd->outerB;
    c3.b[0] = cmd->lastR;
    c3.b[1] = cmd->lastG;
    c3.b[2] = cmd->lastB;
    x = cmd->texX;
    u = x & 0x3F;
    y = (ctx->modelSlots[0] << 8) + cmd->texY;
    v = y & 0xFF;
    switch (cmd->texMode) {
    case 0:
        u <<= 2;
        break;
    case 1:
        u <<= 1;
        break;
    }
    uv.x = u;
    uv.y = v;
    uv.w = cmd->texW;
    uv.h = cmd->texH;
    tpage = GetTPage(cmd->texMode, cmd->abr, cmd->texX, (ctx->modelSlots[0] << 8) + cmd->texY);
    SUG_createTrailEffect(cmd->unkEC, &c0, &c1, &c2, &c3, &template, cmd->unk168, cmd->unk16C, cmd->unk5C, cmd->unk118, cmd->unk120,
                  cmd->unk6C, cmd->unk74, cmd->abr, cmd->unk12C, cmd->unk130, &uv, tpage,
                  func_80067644(cmd->clutX, (ctx->modelSlots[0] << 8) + cmd->clutY), cmd->unk13C, ctx->pak);
}

void SUG_createRingFromParams(SpriteCommand *cmd, EffectSlots *ctx) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    Rect16 uv;
    EffectTemplate template;
    s32 tpage;
    s32 x;
    s32 y;
    s16 u;
    s32 v;

    SUG_initEffectFromParams(&template, (EffectCommand *)cmd, ctx);
    inner.b[0] = cmd->r;
    inner.b[1] = cmd->g;
    inner.b[2] = cmd->b;
    mid.b[0] = cmd->midR;
    mid.b[1] = cmd->midG;
    mid.b[2] = cmd->midB;
    outer.b[0] = cmd->outerR;
    outer.b[1] = cmd->outerG;
    outer.b[2] = cmd->outerB;
    x = cmd->texX;
    u = x & 0x3F;
    y = (ctx->modelSlots[0] << 8) + cmd->texY;
    v = y & 0xFF;
    switch (cmd->texMode) {
    case 0:
        u <<= 2;
        break;
    case 1:
        u <<= 1;
        break;
    }
    uv.x = u;
    uv.y = v;
    uv.w = cmd->texW;
    uv.h = cmd->texH;
    tpage = GetTPage(cmd->texMode, cmd->abr, cmd->texX, (ctx->modelSlots[0] << 8) + cmd->texY);
    createRingEffect(cmd->unkEC, &inner, &mid, &outer, &template, cmd->unk5C, cmd->unk74, cmd->abr, cmd->unk12C, cmd->innerRadius,
                     cmd->outerRadius, cmd->midPercent, cmd->innerZ, cmd->outerZ, (Bytes8 *)&uv, tpage,
                     func_80067644(cmd->clutX, (ctx->modelSlots[0] << 8) + cmd->clutY), cmd->unk130, cmd->u1, cmd->unk138, cmd->unk13C,
                     ctx->pak);
}

EffectTemplate *SUG_createEffectObjectFromParams(EffectCommand *cmd, EffectSlots *ctx) {
    EffectTemplate template;

    SUG_initEffectFromParams(&template, cmd, ctx);
    return cloneEffectObject(&template);
}

void SUG_createModelEffectFromParams(u8 *obj, u8 *b) {
    EffectTemplate buf;
    EffectTemplate *template;
    Entry16 *entry;

    template = &buf;
    SUG_initEffectFromParams(template, (EffectCommand *)obj, (EffectSlots *)b);
    entry = *(Entry16 **)(obj + 0x6F0);
    if (entry != NULL) {
        entry += *(s32 *)(b + 0x4B0);
    }
    SUG_createModelEffect(*(s32 *)(obj + 0xEC), template, *(s32 *)(obj + 0x6E8), *(s32 *)(obj + 0x6EC), *(s32 *)(obj + 0x6F4), (s32)entry,
                  *(s32 *)(obj + 0x128), *(s32 *)(obj + 0x72C), *(s32 *)(b + 0x508), *(s32 *)(b + 0x4B8));
}

void SUG_createSpriteEffectFromParams(u8 *obj, u8 *b) {
    EffectTemplate buf;
    EffectTemplate *template;
    SVECTOR unused;

    template = NULL;
    if (*(s32 *)(obj + 0x6F0) != 0) {
        template = &buf;
        SUG_initEffectFromParams(template, (EffectCommand *)obj, (EffectSlots *)b);
    }
    SUG_createSpriteEffect(*(s32 *)(obj + 0xEC), template, *(s32 *)(obj + 0x6E8), *(s32 *)(obj + 0x71C), *(s32 *)(obj + 0x720),
                  *(s32 *)(obj + 0x724), *(s32 *)(obj + 0x728), *(s32 *)(obj + 0x13C), *(s32 *)(b + 0x508));
}

void SUG_createStreaksFromParams(StreakCommand *cmd, void *ctx) {
    EffectTemplate template;
    u8 from[3];
    u8 to[3];

    from[0] = cmd->fromR;
    from[1] = cmd->fromG;
    from[2] = cmd->fromB;
    to[0] = cmd->toR;
    to[1] = cmd->toG;
    to[2] = cmd->toB;
    SUG_initEffectFromParams(&template, (EffectCommand *)cmd, ctx);
    createStreakParticles(from, to, &template, cmd->spreadX, cmd->spreadY, cmd->length, cmd->endLength, cmd->frames, cmd->speedRange,
                          cmd->reverse, cmd->count, cmd->zOffset, cmd->spin, cmd->pattern, cmd->kind, cmd->semi, cmd->flags, cmd->fixedOtz);
}

void SUG_createLightMotionFromParams(Unk801E9FE0 *obj) {
    VECTOR a;
    VECTOR b;
    VECTOR c;
    VECTOR d;

    a.vx = obj->unk65C[0];
    a.vy = obj->unk65C[1];
    a.vz = obj->unk65C[2];
    b.vx = obj->unk668[0];
    b.vy = obj->unk668[1];
    b.vz = obj->unk668[2];
    c.vx = obj->unk6A0[0];
    c.vy = obj->unk6A0[1];
    c.vz = obj->unk6A0[2];
    d.vx = obj->unk6AC[0];
    d.vy = obj->unk6AC[1];
    d.vz = obj->unk6AC[2];
    SUG_createLightMotion(&a, &b, &c, &d, obj->unk67C, obj->unk680, obj->unk684);
}

void SUG_freeFadeRect(void *ptr) {
    freeHeapBlock(ptr);
}

extern s32 (*SUG_EFFECT_CREATE_FUNCS[])(s32 arg, EffectSlots *slots);

void SUG_createEffectEntry(s32 index, s32 kind, s32 arg, EffectSlots *slots) {
    if (SUG_EFFECT_CREATE_FUNCS[kind] != NULL) {
        slots->slots[index].id = kind;
        slots->slots[index].active = 0;
        slots->slots[index].value = SUG_EFFECT_CREATE_FUNCS[kind](arg, slots);
        slots->count++;
        if ((slots->count & 0xF) == 0) {
            func_80014C08(FRAME_INTERVAL);
        }
    }
}

extern void (*SUG_EFFECT_FREE_FUNCS[])(s32 value);

void SUG_freeEffectEntries(EffectSlots *slots) {
    void (*fn)(s32);
    s32 i;

    for (i = 2; i < 23; i++) {
        if (SCENE_3D->modelState[i] > 0) {
            SCENE_3D->modelState[i] = -1;
        }
    }
    func_80014C08(FRAME_INTERVAL);
    for (i = 0; i < 150; i++) {
        if (slots->slots[i].id != -1) {
            fn = SUG_EFFECT_FREE_FUNCS[slots->slots[i].id];
            if (fn != NULL) {
                fn(slots->slots[i].value);
            }
        }
    }
}

EffectScript *SUG_createEffectScript(void *script, s32 side, s32 a2, s32 *state) {
    EffectScript *runner;

    runner = allocTaskHeapBlock(sizeof(EffectScript));
    runner->script = script;
    runner->context = createScriptContext(script);
    runner->regs = allocScriptRegisters(0x1CC);
    SUG_initEffectSlots(runner);
    runner->slots->modelSlots[0] = side;
    runner->slots->modelSlots[1] = side ^ 1;
    runner->slots->modelSlots[2] = a2;
    switch (side) {
    case 0:
    case 1:
        initTransform(runner->slots->xform, (s32)SCENE_3D->unk78, 0, 0, ((ModelData *)SCENE_3D->models[side])->x, 0,
                      ((ModelData *)SCENE_3D->models[side])->rotY, 0);
        break;
    default:
        runner->slots->modelSlots[0] = runner->slots->modelSlots[1] = 0;
        initTransform(runner->slots->xform, (s32)SCENE_3D->unk78, 0, 0, 0, 0, 0, 0);
        break;
    }
    SUG_runEffectScript(runner);
    return runner;
}

void SUG_freeEffectScript(EffectScript *runner) {
    SUG_freeEffectSlots(runner->slots);
    freeScriptContext(runner->context, runner->regs);
    if ((void *)runner->slots->pak != NULL) {
        freeHeapBlock((void *)runner->slots->pak);
    }
    freeHeapBlock(runner);
}

s32 SUG_pauseEffectScript(EffectScript *runner, s32 *state) {
    if (*state != 3) {
        *state = 1;
        truncatePakTextures((Chunk *)runner->slots->pak);
        do {
            func_80014C08(FRAME_INTERVAL);
        } while (*state == 1);
    }
}

void SUG_runEffectScriptTask(void *script, s32 slot, s32 a2, s32 *state) {
    EffectScript *obj;
    s32 prev;
    s32 running;

    /* the extra block is needed for the register allocation to match */
    do {
        prev = *state;
        obj = SUG_createEffectScript(script, slot, a2, state);
        running = *obj->regs;
        do {
            func_80014C08(FRAME_INTERVAL);
            if (*state == 3) {
                break;
            }
            if (running < 0 && *state < 0 && SUG_pauseEffectScript(obj, state) == 3) {
                break;
            }
            running = SUG_tickEffectScript(obj);
            SUG_tickScreenCopyEffect();
        } while (running != 0);
        if (prev == -2) {
            pauseModelAnimation(slot);
        }
    } while (0);
    *state = 0;
    SUG_freeEffectScript(obj);
}

/* old-style definition: one caller passes no argument */
void SUG_moveScreenCopy(frame)
    s32 frame;
{
    s32 radius;

    switch (SUG_SCREEN_FX_MOTION) {
    case 0:
        SCREEN_COPY_EFFECT.x = SUG_SCREEN_FX_X + SUG_SCREEN_FX_STEP_X * frame;
        SCREEN_COPY_EFFECT.y = SUG_SCREEN_FX_Y + SUG_SCREEN_FX_STEP_Y * frame;
        break;
    case 1:
        radius = SUG_SCREEN_FX_X + SUG_SCREEN_FX_STEP_X * frame;
        SCREEN_COPY_EFFECT.x = rcos(SUG_SCREEN_FX_Y) * radius / 4096;
        SCREEN_COPY_EFFECT.y = rsin(SUG_SCREEN_FX_Y) * radius / 4096;
        SUG_SCREEN_FX_Y = SUG_SCREEN_FX_Y + SUG_SCREEN_FX_STEP_Y;
        break;
    case 2:
        SCREEN_COPY_EFFECT.x = rand() % SUG_SCREEN_FX_X - SUG_SCREEN_FX_X / 2;
        SCREEN_COPY_EFFECT.y = rand() % SUG_SCREEN_FX_Y - SUG_SCREEN_FX_Y / 2;
        break;
    }
}

void SUG_startScreenCopyFade(u8 r, u8 g, u8 b, s32 x, s32 y) {
    SUG_SCREEN_FX_BASE_RGB[0] = SCREEN_COPY_EFFECT.r;
    SUG_SCREEN_FX_BASE_RGB[1] = SCREEN_COPY_EFFECT.g;
    SUG_SCREEN_FX_BASE_RGB[2] = SCREEN_COPY_EFFECT.b;
    SUG_SCREEN_FX_RGB_STEP[0] = (r - SCREEN_COPY_EFFECT.r) / SUG_SCREEN_FX_FADE_TIME;
    SUG_SCREEN_FX_RGB_STEP[1] = (g - SCREEN_COPY_EFFECT.g) / SUG_SCREEN_FX_FADE_TIME;
    SUG_SCREEN_FX_RGB_STEP[2] = (b - SCREEN_COPY_EFFECT.b) / SUG_SCREEN_FX_FADE_TIME;
    switch (SUG_SCREEN_FX_MOTION) {
    case 0:
        SUG_SCREEN_FX_STEP_X = (float)(x - (SUG_SCREEN_FX_X = SCREEN_COPY_EFFECT.x)) / SUG_SCREEN_FX_FADE_TIME;
        SUG_SCREEN_FX_STEP_Y = (float)(y - (SUG_SCREEN_FX_Y = SCREEN_COPY_EFFECT.y)) / SUG_SCREEN_FX_FADE_TIME;
        break;
    case 1:
        SUG_SCREEN_FX_STEP_X = (float)(x - (SUG_SCREEN_FX_X = D_800794C0)) / SUG_SCREEN_FX_FADE_TIME;
        if ((x | y) != 0) {
            SUG_SCREEN_FX_STEP_Y = y;
        }
        break;
    case 2:
        if ((x | y) != 0) {
            SUG_SCREEN_FX_X = x;
            SUG_SCREEN_FX_Y = y;
        }
        break;
    }
    SUG_SCREEN_FX_FRAME = 0;
}

void SUG_stepScreenCopyFade(void) {
    SCREEN_COPY_EFFECT.r = SUG_SCREEN_FX_BASE_RGB[0] + SUG_SCREEN_FX_RGB_STEP[0] * SUG_SCREEN_FX_FRAME;
    SCREEN_COPY_EFFECT.g = SUG_SCREEN_FX_BASE_RGB[1] + SUG_SCREEN_FX_RGB_STEP[1] * SUG_SCREEN_FX_FRAME;
    SCREEN_COPY_EFFECT.b = SUG_SCREEN_FX_BASE_RGB[2] + SUG_SCREEN_FX_RGB_STEP[2] * SUG_SCREEN_FX_FRAME;
    SUG_moveScreenCopy();
    SUG_SCREEN_FX_FRAME++;
}

void SUG_startScreenCopyEffect(u8 *obj, s32 clearColor) {
    SUG_SCREEN_FX_FRAME = 0;
    SUG_SCREEN_FX_ANGLE = SUG_SCREEN_FX_Y = 0;
    SUG_SCREEN_FX_MOTION = *(s32 *)(obj + 0x6D4);
    SUG_SCREEN_FX_PHASE = 1;
    SUG_SCREEN_FX_HOLD = *(s32 *)(obj + 0xE4);
    SUG_SCREEN_FX_FADE_TIME = *(s32 *)(obj + 0x118);
    SCREEN_COPY_EFFECT.mode = 1;
    if (clearColor) {
        SCREEN_COPY_EFFECT.r = 0;
        SCREEN_COPY_EFFECT.g = 0;
        SCREEN_COPY_EFFECT.b = 0;
    }
    SUG_startScreenCopyFade(*(s32 *)(obj + 0xA4), *(s32 *)(obj + 0xA8), *(s32 *)(obj + 0xAC), *(s32 *)(obj + 0x84), *(s32 *)(obj + 0x88));
}

extern s16 D_800794E8;

void SUG_tickScreenCopyEffect(void) {
    if (D_800794E7 == 0) {
        return;
    }
    switch (SUG_SCREEN_FX_PHASE) {
    case 1:
        if (SUG_SCREEN_FX_FRAME < SUG_SCREEN_FX_FADE_TIME) {
            SUG_stepScreenCopyFade();
            return;
        }
        if (D_800794E8 != 0) {
            SUG_startScreenCopyFade(0, 0, 0, 0, 0);
        } else {
            SUG_startScreenCopyFade(0xA8, 0xA8, 0xA8, 0, 0);
        }
        SUG_SCREEN_FX_PHASE++;
        /* fallthrough */
    case 2:
        if (++SUG_SCREEN_FX_FRAME < SUG_SCREEN_FX_HOLD) {
            if (SUG_SCREEN_FX_MOTION != 0) {
                SUG_moveScreenCopy(0);
            }
            return;
        }
        SUG_SCREEN_FX_FRAME = 0;
        SUG_SCREEN_FX_PHASE++;
        /* fallthrough */
    case 3:
        if (SUG_SCREEN_FX_FRAME < SUG_SCREEN_FX_FADE_TIME) {
            SUG_stepScreenCopyFade();
            return;
        }
        SCREEN_COPY_EFFECT.abr = SUG_SCREEN_FX_FRAME = SUG_SCREEN_FX_PHASE = SCREEN_COPY_EFFECT.x = SCREEN_COPY_EFFECT.y = SCREEN_COPY_EFFECT.mode = 0;
        SCREEN_COPY_EFFECT.r = 0xA8;
        SCREEN_COPY_EFFECT.g = 0xA8;
        SCREEN_COPY_EFFECT.b = 0xA8;
        break;
    }
}

extern s16 D_800794C2;
extern s32 D_80079548;

void SUG_updateScreenCopyQuads(void) {
    switch (D_800794E7) {
    case 2:
        SUG_SCREEN_FX_ANGLE = 0;
        SCREEN_COPY_EFFECT.px[0][1] = SCREEN_COPY_EFFECT.px[0][3] = SCREEN_COPY_EFFECT.px[1][0] = SCREEN_COPY_EFFECT.px[1][2] = SCREEN_COPY_EFFECT.x + 0xA0;
        if (SCREEN_COPY_EFFECT.x < 0) {
            SCREEN_COPY_EFFECT.px[0][0] = SCREEN_COPY_EFFECT.px[0][2] = SCREEN_COPY_EFFECT.x;
            SCREEN_COPY_EFFECT.px[1][1] = SCREEN_COPY_EFFECT.px[1][3] = 0x140;
        } else {
            SCREEN_COPY_EFFECT.px[0][0] = SCREEN_COPY_EFFECT.px[0][2] = 0;
            SCREEN_COPY_EFFECT.px[1][1] = SCREEN_COPY_EFFECT.px[1][3] = SCREEN_COPY_EFFECT.x + 0x140;
        }
        if (SCREEN_COPY_EFFECT.y < 0) {
            SCREEN_COPY_EFFECT.py[0][0] = SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = SCREEN_COPY_EFFECT.py[1][1] = SCREEN_COPY_EFFECT.y;
            SCREEN_COPY_EFFECT.py[0][2] = SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = SCREEN_COPY_EFFECT.py[1][3] = 0xF0;
        } else {
            SCREEN_COPY_EFFECT.py[0][0] = SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = SCREEN_COPY_EFFECT.py[1][1] = 0;
            SCREEN_COPY_EFFECT.py[0][2] = SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = SCREEN_COPY_EFFECT.py[1][3] = SCREEN_COPY_EFFECT.y + 0xF0;
        }
        break;
    case 3:
        SCREEN_COPY_EFFECT.px[0][1] = SCREEN_COPY_EFFECT.px[0][3] = SCREEN_COPY_EFFECT.px[1][0] = SCREEN_COPY_EFFECT.px[1][2] = 0xA0;
        SCREEN_COPY_EFFECT.px[0][0] = SCREEN_COPY_EFFECT.px[0][2] = -SCREEN_COPY_EFFECT.x;
        SCREEN_COPY_EFFECT.px[1][1] = SCREEN_COPY_EFFECT.px[1][3] = SCREEN_COPY_EFFECT.x + 0x140;
        SCREEN_COPY_EFFECT.py[0][0] = SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = SCREEN_COPY_EFFECT.py[1][1] = -SCREEN_COPY_EFFECT.y;
        SCREEN_COPY_EFFECT.py[0][2] = SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = SCREEN_COPY_EFFECT.py[1][3] = SCREEN_COPY_EFFECT.y + 0xF0;
        break;
    case 4:
        SCREEN_COPY_EFFECT.px[0][1] = SCREEN_COPY_EFFECT.px[1][0] = (0 * rcos(SUG_SCREEN_FX_ANGLE) - (-SCREEN_COPY_EFFECT.x - 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[0][3] = SCREEN_COPY_EFFECT.px[1][2] = (0 * rcos(SUG_SCREEN_FX_ANGLE) - (SCREEN_COPY_EFFECT.x + 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[0][0] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (-SCREEN_COPY_EFFECT.x - 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[0][2] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (SCREEN_COPY_EFFECT.x + 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[1][1] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (-SCREEN_COPY_EFFECT.x - 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[1][3] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (SCREEN_COPY_EFFECT.x + 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.py[0][0] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (-SCREEN_COPY_EFFECT.x - 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = (0 * rsin(SUG_SCREEN_FX_ANGLE) + (-SCREEN_COPY_EFFECT.x - 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[1][1] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (-SCREEN_COPY_EFFECT.x - 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[0][2] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (SCREEN_COPY_EFFECT.x + 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = (0 * rsin(SUG_SCREEN_FX_ANGLE) + (SCREEN_COPY_EFFECT.x + 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[1][3] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (SCREEN_COPY_EFFECT.x + 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SUG_SCREEN_FX_ANGLE += D_800794C2 * D_80079548;
        break;
    }
}

SpriteEntry *SUG_findSpriteEntry(s32 key, s32 subKey) {
    SpriteEntry *entry;
    SpriteEntry *free;
    s32 i;

    entry = SUG_SPRITE_CACHE;
    free = NULL;
    for (i = 0; i < 128; i++, entry++) {
        if (key == entry->key && subKey == entry->subKey) {
            return entry;
        }
        if (free == NULL && entry->data == NULL) {
            free = entry;
        }
    }
    return free;
}

void SUG_initSpriteCache(void) {
    s32 i;

    SUG_SPRITE_CACHE = allocHeapBlock(0x600, 0x80);
    for (i = 0; i < 128; i++) {
        SUG_SPRITE_CACHE[i].key = -1;
        SUG_SPRITE_CACHE[i].data = NULL;
        SUG_SPRITE_CACHE[i].subKey = 0;
    }
}

void SUG_initSpritePolys(Sprite *sprite) {
    SpriteSheet *sheet;
    SpriteFrame *frame;
    POLY_FT4 *poly;
    u8 u0;
    u8 u1;
    u8 v0;
    u8 v1;
    s32 i;
    s32 mode;

    frame = sprite->frames;
    sheet = (SpriteSheet *)sprite->tex;
    for (i = 0; i < 2; i++) {
        poly = &sprite->polys[i];
        u0 = frame->u0;
        u1 = frame->u1;
        v0 = frame->v0;
        v1 = frame->v1;
        func_800677A4(poly);
        poly->clut = getClut(sheet->clutX, sheet->clutY);
        mode = sheet->mode;
        /* x and y rounded down to their texture page */
        poly->tpage = getTPage(mode & 3, 0, sheet->x & (-64 << mode), sheet->y & ~0xFF);
        poly->pad2 = 1;
        poly->u0 = u0;
        poly->v0 = v0;
        poly->u1 = u1;
        poly->v1 = v0;
        poly->u2 = u0;
        poly->v2 = v1;
        poly->u3 = u1;
        poly->v3 = v1;
        poly->r0 = 0x80;
        poly->g0 = 0x80;
        poly->b0 = 0x80;
        setShadeTex(poly, 0);
    }
}

void SUG_setSpriteFrameVerts(Sprite *sprite, s32 frame) {
    SpriteFrame *f;
    s32 xs[2];
    s32 ys[2];

    f = &sprite->frames[frame];
    xs[0] = -(f->w * sprite->scaleX / 2);
    xs[1] = f->w * sprite->scaleX + xs[0] - 1;
    ys[0] = -(f->h * sprite->scaleY / 2);
    ys[1] = f->h * sprite->scaleY + ys[0] - 1;
    sprite->v[0].vx = sprite->v[2].vx = xs[sprite->flipX];
    sprite->v[1].vx = sprite->v[3].vx = xs[sprite->flipX ^ 1];
    sprite->v[0].vy = sprite->v[1].vy = ys[sprite->flipY];
    sprite->v[2].vy = sprite->v[3].vy = ys[sprite->flipY ^ 1];
    if (sprite->useOrigin != 0) {
        sprite->v[0].vx = sprite->v[2].vx = sprite->v[0].vx - f->originX * sprite->scaleX;
        sprite->v[1].vx = sprite->v[3].vx = sprite->v[1].vx - f->originX * sprite->scaleX;
        sprite->v[0].vy = sprite->v[1].vy = sprite->v[0].vy - f->originY * sprite->scaleY;
        sprite->v[2].vy = sprite->v[3].vy = sprite->v[2].vy - f->originY * sprite->scaleY;
    }
}

void SUG_initSpritePolys(Sprite *sprite);

void SUG_initSprite(Sprite *sprite, s32 key, u16 scaleX, u16 scaleY, s16 x, s16 y, s16 z, s32 a7, s32 useOrigin, s32 subKey) {
    sprite->tex = SUG_findSpriteEntry(key, subKey)->data;
    if (sprite->tex != NULL) {
        sprite->frames = (SpriteFrame *)(sprite->tex + 0x10);
        SUG_initSpritePolys(sprite);
        sprite->useOrigin = useOrigin;
        sprite->pos.vx = x;
        sprite->pos.vy = y;
        sprite->pos.vz = z;
        sprite->frame = 0;
        sprite->frameTimer = -1;
        sprite->scaleX = scaleX;
        sprite->scaleY = scaleY;
        sprite->otz = a7;
        SUG_setSpriteFrameVerts(sprite, 0);
        sprite->v[0].vz = sprite->v[1].vz = sprite->v[2].vz = sprite->v[3].vz = 0;
    }
}

void SUG_drawSprite(Sprite *sprite, s16 brightness) {
    POLY_FT4 *poly;
    MATRIX *m;
    SpriteFrame *frame;
    u32 otz;
    s32 p;
    s32 flag;
    s32 mode;
    u8 u0, u1, v0, v1;

    poly = &sprite->polys[FRAME_BUFFER_INDEX];
    m = (MATRIX *)0x1F800008;
    if (sprite->frame < 0) {
        return;
    }
    frame = &sprite->frames[sprite->frame];
    if (++sprite->frameTimer >= frame->duration) {
        if (++sprite->frame >= ((SpriteSheet *)sprite->tex)->frameCount) {
            if ((sprite->frame = ((SpriteSheet *)sprite->tex)->loopFrame) < 0) {
                sprite->frame = -8;
                return;
            }
        }
        sprite->frameTimer -= frame->duration;
        SUG_setSpriteFrameVerts(sprite, sprite->frame);
        frame = &sprite->frames[sprite->frame];
    }
    gte_ldv0(&sprite->pos);
    gte_rtv0tr();
    gte_stlvnl(m->t);
    gte_stflg(&flag);
    gte_SetTransMatrix(m);
    otz = RotAverage4(&sprite->v[0], &sprite->v[1], &sprite->v[2], &sprite->v[3], (s32 *)&poly->x0, (s32 *)&poly->x1,
                      (s32 *)&poly->x2, (s32 *)&poly->x3, &p, &flag);
    if (sprite->otz != 0) {
        otz = sprite->otz;
    }
    if (otz < 0x1000) {
        poly->r0 = poly->g0 = poly->b0 = frame->shade * brightness / 256;
        poly->clut = getClut(((SpriteSheet *)sprite->tex)->clutX, ((SpriteSheet *)sprite->tex)->clutY + frame->clutRow);
        if (frame->attr >= 0) {
            mode = ((SpriteSheet *)sprite->tex)->mode;
            poly->tpage = getTPage(mode & 3, frame->attr & 3, ((SpriteSheet *)sprite->tex)->x & (-64 << mode),
                                   ((SpriteSheet *)sprite->tex)->y & ~0xFF);
            SetSemiTrans(poly, 1);
        } else {
            SetSemiTrans(poly, 0);
        }
        u0 = frame->u0;
        u1 = frame->u1;
        v0 = frame->v0;
        v1 = frame->v1;
        poly->u0 = u0;
        poly->v0 = v0;
        poly->u1 = u1;
        poly->v1 = v0;
        poly->u2 = u0;
        poly->v2 = v1;
        poly->u3 = u1;
        poly->v3 = v1;
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
    }
}

void SUG_loadSprite(s32 id, s32 x, s32 y, s32 subKey) {
    char path[20];
    s32 task;
    SpriteEntry *entry;
    u8 *data;
    u8 *copy;
    u32 *tims;
    SpriteFrame *frame;
    s32 i;

    task = getCurrentTaskId();
    entry = SUG_findSpriteEntry(id, subKey);
    if (entry == NULL) {
        return;
    }
    data = findPakChunk((Chunk *)subKey, 3, id);
    if (data == NULL) {
        sprintf(path, "E:\\SPRITE\\%d.a2d", id);
        data = (u8 *)loadFileTagged((s32 *)path, task, 0x80);
    } else {
        copy = allocHeapBlock(((s32 *)data)[-1], 0x80);
        bcopy(data, copy, ((s32 *)data)[-1]);
        data = copy;
    }
    if (data == NULL) {
        return;
    }
    x -= 0x140;
    tims = findPakChunk((Chunk *)subKey, 5, id);
    if (tims == NULL) {
        sprintf(path, SUG_FMT_SPRITE_PATH, data);
        tims = (u32 *)loadFile(path, task);
        uploadTimListOffset(tims, x, y);
        freeHeapBlock(tims);
    } else {
        uploadTimListOffset(tims, x, y);
    }
    data += 0x14;
    setEntry(entry, id, data, subKey);
    ((SpriteSheet *)entry->data)->x += x;
    ((SpriteSheet *)entry->data)->y += y;
    ((SpriteSheet *)entry->data)->clutX += x;
    ((SpriteSheet *)entry->data)->clutY += y;
    frame = (SpriteFrame *)(entry->data + 0x10);
    for (i = 0; i < ((SpriteSheet *)entry->data)->frameCount; i++) {
        frame->v0 += y;
        frame->v1 += y;
        frame++;
    }
}

void SUG_loadSpriteFile(s32 a0, s32 a1, s32 a2) {
    SUG_loadSprite(a0, a1, a2, 0);
}

void SUG_freeSprites(void) {
    freeHeapBlocksByTag(0x80);
}

void SUG_hideSprite(s8 *obj) {
    obj[0x82] = -1;
}

void SUG_resetCameraPos(void) {
    ((Graphics *)&GRAPHICS)->posZ = 0;
    ((Graphics *)&GRAPHICS)->posX = 0;
    ((Graphics *)&GRAPHICS)->unk92 = 0;
    ((Graphics *)&GRAPHICS)->posY = -150;
}

extern s32 SUG_CAMERA_SPIN;
extern s32 D_801EF384;
extern s32 D_801EF388;
extern u16 D_80079586;
#define CAMERA ((Graphics *)&GRAPHICS)

void SUG_orbitCamera(void) {
    s32 speed;
    s32 min;
    s32 max;
    s32 outStep;
    s32 inStep;
    s32 angle;

    speed = 1;
    SUG_resetCameraPos();
    CAMERA->rotY += SUG_CAMERA_SPIN * 2;
    if (D_801EF384 == speed) {
        min = 0x100;
        max = 0x580;
        outStep = 16;
        inStep = 1;
    } else {
        min = 0x400;
        max = 0xD80;
        outStep = -1;
        inStep = -16;
    }
    angle = D_80079586 & 0xFFF;
    if (max < angle || angle < min) {
        CAMERA->unk8E += outStep * speed;
    } else {
        CAMERA->unk8E += inStep * speed;
    }
    CAMERA->unk94 += 4 / speed * D_801EF388;
}

void SUG_fillShorts(s16 *dst, s32 count, s16 value) {
    s32 i;

    for (i = 0; i < count; i++) {
        dst[i] = value;
    }
}

void SUG_drawHudSpriteTrail(s32 x, s32 y, Rect16 *uv, u16 tpage, s32 clut, s32 otz, u8 brightness, s8 blend, s16 *trail, s32 count) {
    float level;
    s32 i;

    drawTexturedSprite(x, y, uv, tpage, clut, otz, brightness, blend);
    level = brightness;
    level -= 4.0f;
    otz++;
    for (i = 0; i < count; i++, level -= 4.0f, otz++) {
        if ((s32)level >= 9) {
            drawTexturedSprite(trail[i], y, uv, tpage, clut, otz, (s32)level, 3);
        }
    }
    for (i = count - 1; i >= 0; i--) {
        if (i == 0) {
            trail[i] = x;
        } else {
            trail[i] = trail[i - 1];
        }
    }
}

void SUG_initHudSlide(HudSlide *obj, s16 a1, s16 a2, s16 a3, s8 flags) {
    obj->pos = a1;
    obj->target[0] = a2;
    obj->target[1] = a3;
    obj->flags = flags;
    if (flags & 2) {
        SUG_fillShorts(obj->trail, 6, a1);
    }
    obj->brightness = 0x80;
}

/* old-style definition: the callers pass an int, the byte is read here */
s32 SUG_tickHudSlides(obj, count, state, speed)
    HudSlide *obj;
    s32 count;
    s32 state;
    s8 speed;
{
    s32 i;
    s32 settled;

    i = 0;
    settled = 0;
    for (; i < count; i++, obj++) {
        switch (state) {
        case 0:
        case 1:
            obj->pos = (obj->target[state] - obj->pos) / (8 >> state) + obj->pos;
            if (ABS((s16)obj->pos - obj->target[state]) < 2) {
                settled++;
            }
            break;
        case 2:
            if (obj->flags & 1) {
                if (obj->brightness <= 0) {
                    obj->pos = (340.0f - obj->pos) * 0.125f + obj->pos;
                    if (ABS((s16)obj->pos - obj->target[state]) < 2) {
                        state = 3;
                    }
                } else {
                    obj->brightness--;
                }
            } else {
                obj->brightness -= speed;
                if (obj->brightness <= 0) {
                    state = 3;
                }
            }
            break;
        }
    }
    if (state < 2 && settled == count) {
        state++;
    }
    return state;
}

void SUG_drawNumber(s32 x, s32 y, s32 value, u8 brightness) {
    s32 digits[4];
    Rect16 uv;
    s32 any;
    s32 clut;
    s32 count;
    s32 i;
    s32 j;

    count = 1;
    digits[0] = value / 1000;
    digits[1] = value % 1000 / 100;
    digits[2] = value % 100 / 10;
    digits[3] = value % 10;
    uv.x = 0x78;
    uv.y = 0xA8;
    uv.w = 0x28;
    uv.h = 0x18;
    clut = 0x1469;
    if (x != 0x1F) {
        drawTexturedSprite(x, y, &uv, SUG_HUD_TPAGE, 0x1468, 1, brightness, 1);
        x += 16;
    }
    uv.w = 0x18;
    uv.y = 0xC0;
    for (i = 0; i < 4; i++) {
        j = 0;
        any = 0;
        do {
            any |= digits[j];
            j++;
        } while (j <= i);
        if (any != 0 || i == 3) {
            uv.x = digits[i] * 24;
            drawTexturedSprite(x + 4 + count * 21, y, &uv, SUG_HUD_TPAGE, clut, 1, brightness, 1);
            count++;
        }
    }
}

void SUG_runDamagePopupTask(s32 side) {
    Rect16 uv0;
    Rect16 uv1;
    u8 brightness[3];
    s32 i;

    i = 0;
    memset(brightness, 0, 3);
    side ^= 1;
    uv0.x = 0xA0;
    uv0.y = 0x90;
    uv0.w = 0x60;
    uv0.h = 0x10;
    uv1.x = 0xA0;
    uv1.y = 0xB0;
    uv1.w = 0x3C;
    uv1.h = 0x10;
    for (; i < 150; i++) {
        func_80014C08(FRAME_INTERVAL);
        drawTexturedSprite(15, 0xAE, &uv0, SUG_HUD_TPAGE, 0x1568, 1, brightness[0], 1);
        SUG_drawNumber(0x1F, 0xC1, SUG_BATTLE->players[side].damage, brightness[1]);
        drawTexturedSprite(0x3F, 0xD9, &uv1, SUG_HUD_TPAGE, 0x1569, 1, brightness[2], 1);
        if (i < 20) {
            brightness[0] = i * 8;
        } else if (i < 40) {
            brightness[1] = (i - 20) * 8;
        } else if (i < 60) {
            brightness[2] = (i - 40) * 8;
        } else if (i < 71) {
            /* hold */
        } else if (i < 90) {
            brightness[0] = (89 - i) * 8;
        } else if (i < 110) {
            brightness[1] = (109 - i) * 8;
        } else if (i < 130) {
            brightness[2] = (129 - i) * 8;
        }
    }
}

void SUG_animateHpCounter(s32 side) {
    Rect16 uv;
    s32 step;
    s32 count;
    s32 minFrame;
    s32 b;
    ModelData *model;

    count = 40;
    if (side >= 0) {
        func_800149B8(0, -1, 0, 0x400, SUG_runDamagePopupTask, side);
        minFrame = 4;
    } else {
        side = ~side;
        minFrame = 0;
    }
    uv.x = SUG_BATTLE->players[side].element * 40;
    uv.y = 0xA8;
    uv.w = 0x28;
    uv.h = 0x18;
    step = SUG_TARGET_HP[side];
    if (step < 0) {
        step = 0;
    }
    step = (SUG_BATTLE->players[side].hp - step) / 40;
    if (step == 0 && SUG_TARGET_HP[side] != SUG_BATTLE->players[side].hp) {
        if (SUG_BATTLE->players[side].hp < SUG_TARGET_HP[side]) {
            step = -1;
        } else {
            step = 1;
        }
        count = SUG_TARGET_HP[side] - SUG_BATTLE->players[side].hp < 0 ? -(SUG_TARGET_HP[side] - SUG_BATTLE->players[side].hp)
                                                                         : SUG_TARGET_HP[side] - SUG_BATTLE->players[side].hp;
    }
    do {
        func_80014C08(FRAME_INTERVAL);
        model = SCENE_3D->models[side];
        if (model->unk2208 >= 0 && model->unk2200 >= minFrame) {
            if ((SUG_BATTLE->players[side].hp -= step) < 0) {
                SUG_BATTLE->players[side].hp = 0;
                break;
            }
            count--;
        }
        SUG_drawNumber(0xA8, 0x10, SUG_BATTLE->players[side].hp, 0x80);
        drawTexturedSprite(0x78, 0x10, &uv, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].element * 16 + 0x290, 0x50), 1, 0x80, 1);
    } while (count > 0);
    if (SUG_TARGET_HP[side] > 0) {
        SUG_BATTLE->players[side].hp = SUG_TARGET_HP[side];
    } else {
        SUG_BATTLE->players[side].hp = 0;
    }
    for (b = 0x80; b >= 0; b -= 4) {
        func_80014C08(FRAME_INTERVAL);
        SUG_drawNumber(0xA8, 0x10, SUG_BATTLE->players[side].hp, b);
        drawTexturedSprite(0x78, 0x10, &uv, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].element * 16 + 0x290, 0x50), 1, b, 1);
    }
}

void SUG_showAttackLabel(s32 side) {
    HudSlide obj;
    HudSlide obj2;
    Rect16 uv2;
    s32 state;
    s32 alt;
    s32 both;

    state = 0;
    alt = SUG_BATTLE->players[side].crash;
    both = ((SUG_BATTLE->flags.word >> 1) & 1) | (alt != 0);
    SUG_initHudSlide(&obj, 0x140, both * 32 + 0x4E, both * 32 + 0x50, 2);
    obj.uv.x = 0;
    obj.uv.y = 0x88;
    obj.uv.w = 0xA0;
    obj.uv.h = 0x10;
    obj.brightness = 0x5A;
    SUG_initHudSlide(&obj2, -0x40, 0x32, 0x30, 2);
    obj2.uv.x = alt * 64;
    obj2.uv.y = 0x58;
    obj2.uv.w = 0x40;
    obj2.uv.h = 0x20;
    uv2.x = SUG_BATTLE->players[side].attack * 24 + 0xA0;
    uv2.y = 0xA0;
    uv2.w = 0x18;
    uv2.h = 0x10;
    do {
        func_80014C08(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&obj, ((SUG_BATTLE->flags.word >> 2) & 1) + 1, state, 2);
        SUG_drawHudSpriteTrail((s16)obj.pos, 0xA2, &obj.uv, SUG_HUD_TPAGE, getClut(0x280, 0x53), 1, 0x80, 1, obj.trail, 1);
        if (both) {
            SUG_drawHudSpriteTrail((s16)obj2.pos, 0x92, &obj2.uv, SUG_HUD_TPAGE, getClut(alt * 16 + 0x2A0, 0x54), 1, 0x80, 0, obj2.trail, 6);
        }
        drawTexturedSprite((s16)obj.pos + 6, 0xA2, &uv2, SUG_HUD_TPAGE, SUG_BATTLE->players[side].attack | 0x14A8, 0, 0x80, 1);
    } while (state != 2);
}

void SUG_showAttackBanner(s32 side) {
    HudSlide banners[5];
    Rect16 unused; /* unused, but it sizes the frame */
    Rect16 uv0;
    Rect16 uv1;
    s32 alt;
    s32 flipped;
    s32 c;
    s32 b;
    s32 a;
    s32 count;
    s32 state;
    s32 both;

    flipped = 0;
    state = 0;
    count = 2;
    a = -1;
    b = -1;
    c = -1;
    if (side >= 0) {
        SUG_initHudSlide(&banners[0], -0x100, 0x2B, 0x20, 2);
        banners[0].uv.x = 0;
        banners[0].uv.y = 0;
        banners[0].uv.w = 0x100;
        banners[0].uv.h = 0x28;
    } else {
        SUG_initHudSlide(&banners[0], -0x80, -0x80, 0x60, 2);
        banners[0].uv.x = 0x80;
        banners[0].uv.y = 0x58;
        banners[0].uv.w = 0x80;
        banners[0].uv.h = 0x20;
        side = ~side;
        flipped = 1;
    }
    uv0.x = SUG_BATTLE->players[side].element * 40;
    uv0.y = 0xA8;
    uv0.w = 0x28;
    uv0.h = 0x18;
    uv1.x = SUG_BATTLE->players[side].attack * 24 + 0xA0;
    uv1.y = 0xA0;
    uv1.w = 0x18;
    uv1.h = 0x10;
    alt = SUG_BATTLE->players[side].crash;
    both = ((SUG_BATTLE->flags.word >> 1) & 1) | (alt != 0);
    if (!((SUG_BATTLE->flags.word >> 2) & 1)) {
        SUG_initHudSlide(&banners[1], 0x140, both * 32 + 0x4E, both * 32 + 0x50, 2);
        if (both) {
            a = count++;
            SUG_initHudSlide(&banners[a], -0x40, 0x32, 0x30, 2);
        }
    } else {
        if (both) {
            a = count++;
            SUG_initHudSlide(&banners[a], 0x30, -0xFA, -0xFA, 2);
        }
        SUG_initHudSlide(&banners[1], both * 32 + 0x50, -0xC8, -0xC8, 2);
        b = count++;
        SUG_initHudSlide(&banners[b], 0x140, 0x4E, 0x50, 2);
    }
    if (SUG_BATTLE->players[side].unk8_6) {
        c = count++;
        SUG_initHudSlide(&banners[c], banners[1].pos + 160.0f, banners[1].target[0] + 0xA0, banners[1].target[1] + 0xA0, 2);
        setRECT(&banners[c].uv, 0xE8, 0xA0, 0x18, 0x20);
    }
    banners[1].uv.x = 0;
    banners[1].uv.y = 0x88;
    banners[1].uv.w = 0xA0;
    banners[1].uv.h = 0x10;
    if (a >= 0) {
        setRECT(&banners[a].uv, alt * 64, 0x58, 0x40, 0x20);
    }
    if (b >= 0) {
        setRECT(&banners[b].uv, 0, 0x98, 0x9F, 0x10);
    }
    banners[1].brightness = 0x5A;
    do {
        func_80014C08(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&banners[0], count, state, side < 0 ? 1 : 2);
        if (!flipped) {
            SUG_drawHudSpriteTrail((s16)banners[0].pos, 0xB4, &banners[0].uv, SUG_HUD_TPAGE, 0x1428, 1, banners[0].brightness, 0, banners[0].trail, 6);
        } else {
            SUG_drawHudSpriteTrail((s16)banners[0].pos, 0xC2, &banners[0].uv, SUG_HUD_TPAGE, 0x146A, 1, banners[0].brightness, 0, banners[0].trail, 6);
        }
        SUG_drawHudSpriteTrail((s16)banners[1].pos, 0xA2, &banners[1].uv, SUG_HUD_TPAGE, 0x14E8, 1, banners[1].brightness, 1, banners[1].trail, 1);
        if (a >= 0) {
            SUG_drawHudSpriteTrail((s16)banners[a].pos, 0x92, &banners[a].uv, SUG_HUD_TPAGE, getClut(alt * 16 + 0x2A0, 0x54), 1, banners[a].brightness, 0, banners[a].trail, 6);
        }
        if (b >= 0) {
            SUG_drawHudSpriteTrail((s16)banners[b].pos, 0xA2, &banners[b].uv, SUG_HUD_TPAGE, 0x14E9, 1, banners[b].brightness, 0, banners[b].trail, 6);
        } else {
            drawTexturedSprite((s16)banners[1].pos + 6, 0xA2, &uv1, SUG_HUD_TPAGE, SUG_BATTLE->players[side].attack | 0x14A8, 0, banners[1].brightness, 1);
        }
        if (c >= 0) {
            SUG_drawHudSpriteTrail((s16)banners[c].pos, 0x98, &banners[c].uv, SUG_HUD_TPAGE, 0x14EA, 1, banners[c].brightness, 0, banners[c].trail, 6);
        }
        drawTexturedSprite(0x10, 0x10, &uv0, SUG_HUD_TPAGE, getClut(SUG_BATTLE->players[side].element * 16 + 0x290, 0x50), 1, banners[0].brightness, 1);
    } while (state != 3);
    SUG_BATTLE->flags.word &= ~2;
    SUG_BATTLE->flags.word &= ~4;
}

void SUG_showHpBanner(s32 side) {
    HudSlide bar;
    HudSlide icon;
    HudSlide num;
    s32 unused[8];
    s32 state;

    state = 0;
    SUG_initHudSlide(&bar, -0x100, 0x3C, 0x32, 2);
    bar.uv.x = 0;
    bar.uv.y = 0xD8;
    bar.uv.w = 0x100;
    bar.uv.h = 0x28;
    SUG_initHudSlide(&icon, -0x18, 0xC6, 0xBC, 2);
    icon.uv.x = SUG_BATTLE->players[side].element * 40;
    icon.uv.y = 0xA8;
    icon.uv.w = 0x28;
    icon.uv.h = 0x18;
    SUG_initHudSlide(&num, -0x18, 0x10, 0x10, 2);
    func_80014C08(0x28);
    do {
        func_80014C08(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&bar, 3, state, 2);
        SUG_drawHudSpriteTrail((s16)bar.pos, 0xB4, &bar.uv, SUG_HUD_TPAGE, 0x1528, 1, bar.brightness, 0, bar.trail, 6);
        SUG_drawHudSpriteTrail(10, (s16)icon.pos, &icon.uv, SUG_HUD_TPAGE,
                      getClut(0x290 + SUG_BATTLE->players[side].element * 16, 0x50), 1, icon.brightness, 0,
                      icon.trail, 6);
        SUG_drawNumber(0xA8, (s16)num.pos, SUG_BATTLE->players[side].hp, num.brightness);
        if (((ModelData *)SCENE_3D->models[side])->unk2208 < 0) {
            playModelAnimation(side, 0);
        }
    } while (state != 3);
    func_80014C08(FRAME_INTERVAL);
}

void SUG_showWinnerBanner(s32 side) {
    HudSlide banner;
    HudSlide icon;
    s32 state;

    state = 0;
    SUG_initHudSlide(&banner, -0x100, 0x3C, 0x32, 3);
    banner.uv.x = 0;
    banner.uv.y = 0x28;
    banner.uv.w = 0x100;
    banner.uv.h = 0x28;
    SUG_initHudSlide(&icon, -0x18, 0xC6, 0xBC, 3);
    icon.uv.x = SUG_BATTLE->players[side].element * 40;
    icon.uv.y = 0xA8;
    icon.uv.w = 0x28;
    icon.uv.h = 0x18;
    do {
        func_80014C08(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&banner, 2, state, 2);
        SUG_drawHudSpriteTrail((s16)banner.pos, 0xB4, &banner.uv, SUG_HUD_TPAGE, 0x14EB, 1, 0x80, 0, banner.trail, 6);
        SUG_drawHudSpriteTrail(10, (s16)icon.pos, &icon.uv, SUG_HUD_TPAGE, getClut(0x290 + SUG_BATTLE->players[side].element * 16, 0x50), 1,
                      0x80, 0, icon.trail, 6);
    } while (state != 3);
}

void SUG_showEatUpHpBanner(void) {
    HudSlide obj;
    Rect16 unused;
    s32 state;

    state = 0;
    SUG_initHudSlide(&obj, -0x60, 0x7A, 0x70, 3);
    obj.uv.x = 0xA0;
    obj.uv.y = 0x78;
    obj.uv.w = 0x60;
    obj.uv.h = 0x18;
    obj.brightness = 0x20;
    do {
        func_80014C08(FRAME_INTERVAL);
        state = SUG_tickHudSlides(&obj, 1, state, 2);
        SUG_drawHudSpriteTrail((s16)obj.pos, 0xB4, &obj.uv, SUG_HUD_TPAGE, 0x146B, 1, 0x80, 0, obj.trail, 6);
    } while (state != 3);
}

void SUG_placeBattleModels(void) {
    Scene3D *scene;

    scene = SCENE_3D;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn])->x = 750;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn])->rotY = 0;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn ^ 1])->x = -750;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn ^ 1])->rotY = 0x800;
    SUG_TARGET_HP[SUG_BATTLE->flags.bits.turn] = SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn].hp - SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn ^ 1].damage;
    SUG_TARGET_HP[SUG_BATTLE->flags.bits.turn ^ 1] = SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn ^ 1].hp - SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn].damage;
    scene->modelState[0] = -1;
    SCENE_3D->modelState[1] = -1;
}

void SUG_initBattleScene(void) {
    s32 unused[8];

    SUG_CURRENT_SCRIPT = 0;
    addFrameCallback((s32)renderSceneModels);
    SUG_initSpriteCache();
    SUG_initTamCache();
    SUG_placeBattleModels();
    SUG_initEffectRoot();
    SUG_SCRIPT_STATES[1] = 0;
    SUG_SCRIPT_STATES[0] = 0;
}

void SUG_showWireGrid(void) {
    createWireGrid(2000, 3000, 9, 13, 1, 0);
    GRID_VISIBLE = 1;
    SCENE_3D->modelState[23] = 1;
}

void SUG_freeBattleScene(void) {
    SCENE_3D->modelState[0] = -1;
    SCENE_3D->modelState[1] = -1;
    SCENE_3D->modelState[23] = -1;
    func_80014A00(0x1B);
    func_80014A00(0x1A);
    removeFrameCallback((s32)renderSceneModels);
    removeFrameCallback((s32)renderWireGrid);
    freeHeapBlock(DB(0).scenePackets);
    freeHeapBlock(DB(1).scenePackets);
    SUG_freeSprites();
    SUG_freeTamCache();
    func_80022E58();
    SUG_resetStageBrightness();
}

void SUG_runCameraOrbit(s32 frames, s32 resetCamera) {
    Graphics *camera;

    if (resetCamera) {
        camera = (Graphics *)&GRAPHICS;
        *(s32 *)&camera->pad5A[0xE] = 0x9C4000;
        camera->unk90 = 4000;
        camera->snapCamera = 1;
        camera->unk8E = 400;
        *(s32 *)&camera->pad5A[0x16] = 0x190000;
    }
    D_80079584 = -1;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = 1;
    do {
        frames--;
        SUG_orbitCamera();
        func_80014C08(FRAME_INTERVAL);
    } while (frames >= 0);
}

extern s16 SUG_ACTIVE_MODEL;
extern s32 SUG_PREV_MODEL;

void SUG_playAttackTurn(s32 model, s32 a1, void *script, void (*fn)(s32), s32 switchModel) {
    s16 other;

    other = SUG_CURRENT_SCRIPT ^ 1;
    if (switchModel) {
        playModelAnimation(SUG_ACTIVE_MODEL, 0);
        SCENE_3D->modelState[SUG_ACTIVE_MODEL] = 1;
        if (SUG_ACTIVE_MODEL != SUG_PREV_MODEL) {
            SCENE_3D->modelState[SUG_PREV_MODEL] = -1;
            applyAnimationFirstFrame(SUG_PREV_MODEL, 0);
        }
        D_80079584 = SUG_ACTIVE_MODEL;
    }
    while (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] != 1) {
        func_80014C08(FRAME_INTERVAL);
    }
    if (script != NULL) {
        if (SUG_SCRIPT_STATES[other] != 0) {
            if (SUG_SCRIPT_STATES[other] < 0) {
                while (SUG_SCRIPT_STATES[other] != 1) {
                    func_80014C08(FRAME_INTERVAL);
                }
            }
            SUG_SCRIPT_STATES[other] = 3;
            while (SUG_SCRIPT_STATES[other] != 0) {
                func_80014C08(FRAME_INTERVAL);
            }
        }
        SUG_SCRIPT_STATES[other] = -1;
        func_800149B8(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, script, model, a1, &SUG_SCRIPT_STATES[other]);
    }
    SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] = 2;
    if (fn != NULL) {
        fn(SUG_ACTIVE_MODEL);
    }
    if (fn == SUG_showAttackBanner && SUG_BATTLE->players[SUG_ACTIVE_MODEL].crash) {
        func_80014C08(30);
        SUG_TARGET_HP[SUG_ACTIVE_MODEL] = 10;
        SUG_animateHpCounter(~SUG_ACTIVE_MODEL);
        if (!SUG_BATTLE->flags.bits.counter) {
            SUG_TARGET_HP[SUG_ACTIVE_MODEL] -= SUG_BATTLE->players[(s16)(SUG_ACTIVE_MODEL ^ 1)].damage;
        }
    }
    if (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] == 1) {
        SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] = 2;
    }
    while (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] != 0) {
        func_80014C08(FRAME_INTERVAL);
    }
    SUG_CURRENT_SCRIPT = other;
    SUG_PREV_MODEL = SUG_ACTIVE_MODEL;
    SUG_ACTIVE_MODEL = model;
}

extern void *SUG_SKILL_SCRIPTS[4];
void SUG_playSoloAnimation(s32 model, s32 anim);
void SUG_applyEatUpHp(s32 side, s32 amount);
void SUG_playNoDamageTurn(s32 model);

void SUG_playBattleExchange(void) {
    s32 first;
    s32 second;
    s32 other;
    s32 winner;
    s32 hp;
    BattleFlags flags;

    winner = -1;
    flags = SUG_BATTLE->flags.bits;
    first = flags.turn ^ (flags.flag1 | flags.flag3);
    other = first ^ flags.counter;
    second = first ^ 1;
    if (SUG_BATTLE->players[other].damage == 0) {
        other ^= 1;
    }
    SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] = -1;
    SUG_ACTIVE_MODEL = SUG_BATTLE->flags.bits.turn;
    func_800149B8(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, D_801D81AC, SUG_BATTLE->flags.bits.turn, 1, &SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT]);
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (DUEL->state != 3);
    SUG_saveStageClut(SCENE_3D->models[23], SCENE_3D->texAnimFrames);
    func_800149B8(0x1A, 0x1F, 0, 0x400, SUG_runStageFadeTask);
    while (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] != 1) {
        func_80014C08(FRAME_INTERVAL);
    }
    func_80014A00(0x19);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 1);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    DB(0).draw.r0 = DB(1).draw.r0 = STAGE_CLEAR_COLOR[0];
    DB(0).draw.g0 = DB(1).draw.g0 = STAGE_CLEAR_COLOR[1];
    DB(0).draw.b0 = DB(1).draw.b0 = STAGE_CLEAR_COLOR[2];
    SUG_showWireGrid();
    ((ModelData *)SCENE_3D->models[23])->unkA78 = 0;
    setScreenFadeParams(1, 1, 6);
    SUG_playAttackTurn(SUG_BATTLE->flags.bits.turn ^ 1, 1, D_801D81AC, SUG_showHpBanner, 0);
    SUG_playAttackTurn(other, 0, SUG_SKILL_SCRIPTS[other * 2], SUG_showHpBanner, 0);
    SUG_runCameraOrbit(0x78, 1);
    SCENE_3D->modelState[first ^ 1] = -1;
    ((Graphics *)&GRAPHICS)->targetModel = first;
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (SUG_SCRIPT_STATES[0] != 1);
    if (SUG_BATTLE->flags.bits.counter) {
        SUG_playSoloAnimation(first, SUG_BATTLE->players[first].attack + 1);
        SUG_showAttackLabel(first);
    } else if (SUG_BATTLE->players[first].damage == 0) {
        SUG_playNoDamageTurn(first);
    } else {
        FADE_TARGET = D_8006DF80;
        SUG_playAttackTurn(second, 1, SUG_SKILL_SCRIPTS[first * 2 + 1], SUG_showAttackBanner, 1);
        FADE_TARGET = 0xFF;
        hp = SUG_BATTLE->players[second].hp;
        SUG_playAttackTurn(second, 0, SUG_SKILL_SCRIPTS[second * 2], SUG_animateHpCounter, 1);
        applyAnimationFirstFrame(first, 0);
        playModelAnimation(first, 0);
        do {
            func_80014C08(FRAME_INTERVAL);
        } while (((ModelData *)SCENE_3D->models[second])->unk2208 >= 0);
        if (SUG_BATTLE->players[first].eatUpHp) {
            SUG_applyEatUpHp(first, hp);
        }
        if (SUG_TARGET_HP[second] <= 0) {
            goto firstWins;
        }
        playModelAnimation(second, 0);
        func_80014C08(20);
    }
    if (SUG_BATTLE->players[second].damage == 0) {
        SUG_playNoDamageTurn(second);
    } else {
        FADE_TARGET = D_8006DF80;
        SUG_playAttackTurn(first, 1, SUG_SKILL_SCRIPTS[second * 2 + 1], SUG_showAttackBanner, 1);
        FADE_TARGET = 0xFF;
        hp = SUG_BATTLE->players[first].hp;
        SUG_playAttackTurn(second, 0, NULL, SUG_animateHpCounter, 1);
        applyAnimationFirstFrame(second, 0);
        playModelAnimation(second, 0);
        do {
            func_80014C08(FRAME_INTERVAL);
        } while (((ModelData *)SCENE_3D->models[first])->unk2208 >= 0);
        if (SUG_BATTLE->players[second].eatUpHp) {
            SUG_applyEatUpHp(second, hp);
        } else {
            if (SUG_TARGET_HP[first] <= 0) {
                goto secondWins;
            }
            playModelAnimation(first, 0);
            func_80014C08(30);
        }
    }
    if (SUG_TARGET_HP[first] > 0) {
        playModelAnimation(first, 0);
        goto done;
    }
secondWins:
    winner = second;
    goto done;
firstWins:
    winner = first;
done:
    SUG_SCRIPT_STATES[0] = SUG_SCRIPT_STATES[1] = 3;
    if (winner >= 0) {
        SCENE_3D->modelState[winner] = 1;
        playModelAnimation(winner, 6);
        D_80079584 = winner;
        func_80014C08(FRAME_INTERVAL);
        SCENE_3D->modelState[winner ^ 1] = -1;
        SUG_showWinnerBanner(winner);
        func_80014C08(8);
    } else {
        SCENE_3D->modelState[0] = 1;
        SCENE_3D->modelState[1] = 1;
    }
    func_800149B8(0, 0x1F, 0, 0x800, SUG_runCameraOrbit, 0xA0, 0);
    func_80014C08(0x82);
    DUEL->state = 4;
    func_80014C08(30);
}

void SUG_playSoloAnimation(s32 model, s32 anim) {
    SCENE_3D->modelState[model] = 1;
    SCENE_3D->modelState[model ^ 1] = -1;
    D_80079584 = model;
    playModelAnimation(model, anim);
}

void SUG_applyEatUpHp(s32 side, s32 amount) {
    s32 state;

    state = -1;
    SCENE_3D->modelState[side] = 1;
    D_80079584 = side;
    func_800149B8(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, D_801D81B0, side, 0, &state);
    func_800149B8(0, -1, 0, 0x800, SUG_showEatUpHpBanner);
    while (state != 1) {
        func_80014C08(FRAME_INTERVAL);
    }
    state = 2;
    if (SUG_BATTLE->players[side].damage < amount) {
        amount = SUG_BATTLE->players[side].damage;
    }
    SUG_TARGET_HP[side] = SUG_BATTLE->players[side].hp + amount;
    if (SUG_TARGET_HP[side] >= 0x2707) {
        SUG_TARGET_HP[side] = 0x2706;
    }
    SUG_animateHpCounter(~side);
    SUG_TARGET_HP[side] -= SUG_BATTLE->players[side ^ 1].damage;
    while (state != 0) {
        func_80014C08(FRAME_INTERVAL);
    }
}

void SUG_playNoDamageTurn(s32 model) {
    SCENE_3D->modelState[model] = 1;
    playModelAnimation(model, 0);
    D_80079584 = model;
    SCENE_3D->modelState[model ^ 1] = -1;
    SUG_showAttackBanner(~model);
}

extern BattleState SUG_BATTLE_STATE;

void SUG_runPolygonBattle(s32 arg, s32 parentTask) {
    s32 i;

    SUG_BATTLE = &SUG_BATTLE_STATE;
    SUG_BATTLE_STATE.flags.bits.flag1 = PLAYER((s8)(DUEL->turnPlayer ^ 1))->unk178_8;
    SUG_BATTLE_STATE.flags.bits.counter = PLAYER(0)->counter | PLAYER(1)->counter;
    SUG_BATTLE_STATE.flags.bits.flag3 = PLAYER(DUEL->turnPlayer)->unk178_7;
    SUG_BATTLE_STATE.flags.bits.turn = DUEL->turnPlayer;
    for (i = 0; i < 2; i++) {
        SUG_BATTLE->players[i].hp = PLAYER(i)->unk158;
        SUG_BATTLE->players[i].damage = PLAYER(i ^ 1)->damageTaken;
        switch (PLAYER(i)->controller) {
        case 0:
        case 2:
            SUG_BATTLE->players[i].element = 0;
            break;
        case 1:
            SUG_BATTLE->players[i].element = 2;
            break;
        case 3:
            SUG_BATTLE->players[i].element = 1;
            break;
        }
        SUG_BATTLE->players[i].attack = PLAYER(i)->usedAttack;
        SUG_BATTLE->players[i].eatUpHp = PLAYER(i)->eatUpHp;
        SUG_BATTLE->players[i].crash = PLAYER(i)->crash;
        SUG_BATTLE->players[i].unk8_6 = PLAYER(i)->unk178_13;
        SUG_SKILL_SCRIPTS[i * 2] = (void *)DUEL_DIGIMON_MODELS[i].attackModels[SUG_BATTLE->players[i].attack];
        SUG_SKILL_SCRIPTS[i * 2 + 1] = (void *)DUEL_DIGIMON_MODELS[i].unk14[SUG_BATTLE->players[i].attack];
    }
    SUG_initBattleScene();
    SUG_playBattleExchange();
    SUG_freeBattleScene();
    DB(0).draw.b0 = DB(1).draw.b0 = 0;
    DB(0).draw.g0 = DB(1).draw.g0 = 0;
    DB(0).draw.r0 = DB(1).draw.r0 = 0;
    func_80014A48(parentTask);
}

/* the last three bytes are leftovers in the original, not zero padding */
const char SUG_FMT_SPRITE_PATH[16] = "E:\\SPRITE\\%s\0\0\xA2\xAF";

/* the light matrix and colours SUG_tickLightMotion sets */
MATRIX SUG_DEFAULT_LIGHT_MATRIX = { { { 0, -0x1000, -0x5DC }, { 0, 0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } };
MATRIX SUG_DEFAULT_LIGHT_COLORS = { { { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } };
s32 SUG_SCREEN_FX_PHASE = 0;
s16 SUG_SCREEN_FX_ANGLE = 0;

/* per effect slot kind: the functions that update, create and free it */
typedef void (*SlotUpdate)(u8 *value);
typedef s32 (*SlotCreate)(s32 arg, EffectSlots *slots);
typedef void (*SlotFree)(s32 value);

SlotUpdate SUG_EFFECT_TICK_FUNCS[18] = {
    (SlotUpdate)SUG_tickFadeRect,
    (SlotUpdate)SUG_scrollTexture,
    NULL,
    (SlotUpdate)SUG_tickLightMotion,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (SlotUpdate)SUG_tickSphereEffect,
    NULL,
    (SlotUpdate)SUG_tickTrailEffect,
    (SlotUpdate)renderRingEffect,
    (SlotUpdate)updateEffectObject,
    (SlotUpdate)SUG_tickModelEffect,
    (SlotUpdate)SUG_tickSpriteEffect,
    (SlotUpdate)renderStreakParticles,
};

SlotCreate SUG_EFFECT_CREATE_FUNCS[18] = {
    (SlotCreate)SUG_createFadeRectFromParams,
    (SlotCreate)SUG_createScrollTextureFromParams,
    NULL,
    (SlotCreate)SUG_createLightMotionFromParams,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (SlotCreate)SUG_createSphereFromParams,
    NULL,
    (SlotCreate)SUG_createTrailFromParams,
    (SlotCreate)SUG_createRingFromParams,
    (SlotCreate)SUG_createEffectObjectFromParams,
    (SlotCreate)SUG_createModelEffectFromParams,
    (SlotCreate)SUG_createSpriteEffectFromParams,
    (SlotCreate)SUG_createStreaksFromParams,
};

SlotFree SUG_EFFECT_FREE_FUNCS[18] = {
    (SlotFree)SUG_freeFadeRect,
    (SlotFree)SUG_freeScrollTexture,
    NULL,
    (SlotFree)SUG_freeLightMotion,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    (SlotFree)SUG_freeSphereEffect,
    NULL,
    (SlotFree)SUG_freeTrailEffect,
    (SlotFree)freeRingEffect,
    (SlotFree)freeEffectObject,
    (SlotFree)SUG_freeModelEffect,
    (SlotFree)SUG_freeSpriteEffect,
    (SlotFree)freeStreakParticles,
};

/* the u8 arrays among these are not referenced by any code */
s16 SUG_CURRENT_SCRIPT = 0;
s16 SUG_ACTIVE_MODEL = 0;
s32 SUG_CAMERA_SPIN = 1;
s32 D_801EF384 = 1;
s32 D_801EF388 = 1;
u16 SUG_HUD_TPAGE = 10;
ClutFade SUG_STAGE_CLUT = { { 0 } };
u8 D_801EF79C[4] = { 0 };
TamEntry SUG_TAM_CACHE[8] = { { 0 } };
s32 SUG_SCREEN_FX_X = 0;
s32 SUG_SCREEN_FX_Y = 0;
float SUG_SCREEN_FX_STEP_X = 0;
float SUG_SCREEN_FX_STEP_Y = 0;
s32 SUG_SCREEN_FX_HOLD = 0;
s32 SUG_SCREEN_FX_FADE_TIME = 0;
s32 SUG_SCREEN_FX_FRAME = 0;
s32 SUG_SCREEN_FX_MOTION = 0;
u8 SUG_SCREEN_FX_BASE_RGB[3] = { 0 };
u8 SUG_SCREEN_FX_RGB_STEP[3] = { 0 };
RootEffect SUG_EFFECT_ROOT = { { 0 } };
u8 D_801EF944[12] = { 0 };
SpriteEntry *SUG_SPRITE_CACHE = NULL;
u8 D_801EF954[4] = { 0 };
BattleState *SUG_BATTLE = NULL;
u8 D_801EF95C[12] = { 0 };
void *SUG_SKILL_SCRIPTS[4] = { NULL };
s32 SUG_SCRIPT_STATES[2] = { 0 };
u8 D_801EF980[4] = { 0 };
s32 SUG_PREV_MODEL = 0;
BattleState SUG_BATTLE_STATE = { { { 0 } } };
s16 SUG_TARGET_HP[2] = { 0 };
