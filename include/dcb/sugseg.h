#ifndef DCB_SUGSEG_H
#define DCB_SUGSEG_H

#include "game.h"

#define CAMERA ((Graphics *)&GRAPHICS)
#define FADE_TARGET (((u8 *)SCENE_3D)[0x131])

/* a texture animation (.tam): frames of UVs, or a scroll, played on a VRAM rect */
typedef struct {
    s16 duration;
    u8 u;
    u8 v;
} AnimFrame;

typedef struct {
    u8 count;
    u8 loop; /* bit 0: copy the frames in VRAM instead of moving the UVs; the rest: the frame to loop to */
    u8 w;
    u8 h;
} AnimHeader;

typedef struct {
    /* 0x00 */ Rect16 rect;
    /* 0x08 */ s16 *dst; /* the UVs it animates */
    /* 0x0C */ AnimHeader *header;
    /* 0x10 */ AnimFrame *frames;
    /* 0x14 */ s16 frame;
    /* 0x16 */ s16 timer;
    /* 0x18 */ s32 type; /* 0-3: scroll left, right, up, down; else play the frames */
    /* 0x1C */ u8 *pixels;
} TexAnim;

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


/* a 3D polygon effect: a fan of `count` triangles around verts[0], an optional
   inner fan, and rings of quads, drawn with one of several primitive kinds */
typedef struct {
#if VERSION_JP
    /* 0x000 */ u8 unk0[0x13D]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    /* 0x000 */ u8 unk0[0x139];
#endif
    /* 0x139 */ u8 suspended;
    /* 0x13A */ u8 unk13A[2];
    /* 0x13C */ TexAnim texAnim;
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

/* The effect script's registers (0x1CC words, allocScriptRegisters) as the
   effect commands read them: SUG_EFFECT_CREATE_FUNCS[kind] builds an effect
   from them, and SUG_setEffectParams copies the motion ones (0x65C-0x6E8) to
   it. A register below 0x1A0 means different things for different kinds; the
   comments say which. */
typedef struct {
    /* 0x000 */ u8 unk0[0x5C];
    /* 0x05C */ s32 count; /* ring segments, trail length, sphere: 1 = open at the bottom */
    /* 0x060 */ s32 slices;
    /* 0x064 */ s32 segments;
    /* 0x068 */ s32 pulse;
    /* 0x06C */ s32 pulseMode; /* the trail's colour mode */
    /* 0x070 */ s32 texDepth;
    /* 0x074 */ s32 semiTrans;
    /* 0x078 */ s32 abr;
    /* 0x07C */ u8 unk7C[4];
    /* 0x080 */ s32 rate; /* the fade rect's colour step, the scroll texture's speed */
    /* 0x084 */ s32 x;
    /* 0x088 */ s32 y;
    /* 0x08C */ u8 unk8C[0x18];
    /* 0x0A4 */ s32 r0;
    /* 0x0A8 */ s32 g0;
    /* 0x0AC */ s32 b0;
    /* 0x0B0 */ s32 r1;
    /* 0x0B4 */ s32 g1;
    /* 0x0B8 */ s32 b1;
    /* 0x0BC */ s32 r2;
    /* 0x0C0 */ s32 g2;
    /* 0x0C4 */ s32 b2;
    /* 0x0C8 */ s32 r3;
    /* 0x0CC */ s32 g3;
    /* 0x0D0 */ s32 b3;
    /* 0x0D4 */ u8 unkD4[0x10];
    /* 0x0E4 */ s32 hold; /* frames the screen copy stays still */
    /* 0x0E8 */ u8 unkE8[4];
    /* 0x0EC */ s32 brightness;
    /* 0x0F0 */ u8 unkF0[8];
    /* 0x0F8 */ s32 radius;
    /* 0x0FC */ u8 unkFC[8];
    /* 0x104 */ s32 innerRadius;
    /* 0x108 */ s32 outerRadius;
    /* 0x10C */ s32 innerZ;
    /* 0x110 */ s32 outerZ;
    /* 0x114 */ s32 midPercent;
    /* 0x118 */ s32 rows; /* trail history rows; the screen copy's fade time */
    /* 0x11C */ u8 unk11C[4];
    /* 0x120 */ s32 variant; /* fade rect and scroll texture mode, the trail's follow mode */
    /* 0x124 */ u8 unk124[4];
    /* 0x128 */ s32 flags;
    /* 0x12C */ s32 primKind;
    /* 0x130 */ u32 texAnimId;
    /* 0x134 */ u8 unk134[4];
    /* 0x138 */ s32 cull;
    /* 0x13C */ union {
        s32 w;
        u8 b;
        s16 h;
    } otz;
    /* 0x140 */ s32 rectX;
    /* 0x144 */ s32 rectY;
    /* 0x148 */ s32 rectW;
    /* 0x14C */ s32 rectH;
    /* 0x150 */ s32 texX;
    /* 0x154 */ s32 texY;
    /* 0x158 */ s32 texW;
    /* 0x15C */ s32 texH;
    /* 0x160 */ s32 clutX;
    /* 0x164 */ s32 clutY;
    /* 0x168 */ s32 edgeX0; /* where the trail's two edges sit */
    /* 0x16C */ s32 edgeX1;
    /* 0x170 */ u8 unk170[0x20];
    /* 0x190 */ s32 pattern;
    /* 0x194 */ s32 zOffset;
    /* 0x198 */ s32 spin;
    /* 0x19C */ s32 kind;
    /* 0x1A0 */ u8 unk1A0[0x4BC];
    /* a light motion reads pos and pos2 as where the light moves from and to, rot
       as its light, period and duration, and scale and scale2 as its colour */
    /* 0x65C */ s32 pos[3];
    /* 0x668 */ s32 pos2[3];
    /* 0x674 */ s32 moveSpeed;
    /* 0x678 */ s32 moveAccel;
    /* 0x67C */ s32 rot[3];
    /* 0x688 */ s32 rot2[3];
    /* 0x694 */ s32 rotAccel[3];
    /* 0x6A0 */ s32 scale[3];
    /* 0x6AC */ s32 scale2[3];
    /* 0x6B8 */ s32 scaleStep[3];
    /* 0x6C4 */ s32 hitRadius;
    /* 0x6C8 */ s32 period;
    /* 0x6CC */ s32 fadeMode;
    /* 0x6D0 */ s32 speed;
    /* 0x6D4 */ s32 mode;
    /* 0x6D8 */ s32 wavePhase;
    /* 0x6DC */ s32 waveAmplitude;
    /* 0x6E0 */ s32 waveFreq;
    /* 0x6E4 */ s32 target; /* the bone it hangs from (the effect slot if source is -1); < 0: the script's root */
    /* 0x6E8 */ s32 id; /* model id, sprite key */
    /* 0x6EC */ s32 anim;
    /* 0x6F0 */ s32 vramEntries; /* model: its VRAM slot table; sprite: nonzero if it moves */
    /* 0x6F4 */ s32 modelTexAnimId;
    /* 0x6F8 */ s32 source; /* the model: an index into modelSlots, -1 an effect slot, -2 - n model effect n */
    /* 0x6FC */ s32 spreadY;
    /* 0x700 */ s32 spreadX;
    /* 0x704 */ s32 length;
    /* 0x708 */ s32 frames;
    /* 0x70C */ s32 speedRange;
    /* 0x710 */ s32 reverse;
    /* 0x714 */ s32 streakCount;
    /* 0x718 */ s32 endLength;
    /* 0x71C */ s32 flipX;
    /* 0x720 */ s32 flipY;
    /* 0x724 */ s32 unk724;
    /* 0x728 */ s32 useOrigin;
    /* 0x72C */ s32 allBones; /* 0: animate only the model's root */
} EffectParams;

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
#if VERSION_US || VERSION_EU
    /* jp's sprites can't be flipped or placed by their frame's origin */
    s8 flipX;
    s8 flipY;
    u8 unk8A;
    s8 useOrigin;
#endif
} Sprite;

#if VERSION_JP
/* jp keeps each of the battle's flags and fields in a word of its own */
typedef struct {
    s32 turn;
    s32 flag1;
    s32 counter;
    s32 flag3;
} BattleFlags;

typedef struct {
    s32 hp;
    s32 damage;
    s32 unk8;
    s32 attack;
    s32 element;
    s32 eatUpHp;
    s32 crash;
    s32 unk8_6;
} BattlerState;

typedef struct {
    BattlerState players[2];
    union {
        BattleFlags bits;
    } flags;
} BattleState;
#elif VERSION_US || VERSION_EU
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
#endif

typedef struct {
    u8 unk0[6];
    s16 id;
    u8 unk8[8];
    s32 x;
    u8 unk14[0xA64];
    s16 rotX; /* Model.rot */
    s16 rotY;
    u8 unkA7C[0x1784];
    s32 animClip; /* Model.anim.clip */
    u8 unk2204[4];
    s32 animKeyTimer; /* Model.anim.keyTimer: negative once stopped */
    u8 unk220C[0xA4];
    u8 boneMatrices[0x20][0x20]; /* Model.lw: a MATRIX per bone */
    u8 unk26B0[0x24];
    s32 tpageOffset;
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
#if VERSION_JP
    u8 unk0[0x13D]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    u8 unk0[0x139];
#endif
    u8 suspended;
    u8 unk13A[2];
    ModelData *model;
    TexAnim texAnim;
    ClutFade fade;
    s16 lastBrightness;
    s8 modelSlot;
    u8 flags;
    s8 texAnimActive;
    s8 active;
#if VERSION_US || VERSION_EU
    s32 clutBank; /* 0 takes the model's CLUT (and .tam texture) from the lower row */
#endif
} ModelEffect;

typedef struct {
#if VERSION_JP
    u8 unk0[0x9C]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    u8 unk0[0x98];
#endif
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
#if VERSION_JP
    u8 unk0[0x13D]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    u8 unk0[0x139];
#endif
    u8 suspended;
    u8 unk13A[2];
    Sprite sprite;
    s32 brightness;
    s32 active;
} SpriteEffect;

typedef struct {
    u8 unk0[0x20];
    VECTOR pos;
    SVECTOR rot;
    s32 sx;
#if VERSION_JP
    u8 unk3C[0xF6]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    u8 unk3C[0xF2];
#endif
    s16 mode;
    s16 speed;
    s16 brightness;
    u8 unk134[3];
    u8 fadeMode;
    u8 fadeState;
    u8 suspended;
    u8 unk13A[2];
    TexAnim texAnim;
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
#if VERSION_US || VERSION_EU
    s32 texAnimId;
#endif
    s16 level;
    s16 prevLevel;
    u8 semiTrans;
    u8 primKind;
    u8 followMode;
    u8 colorMode;
#if VERSION_JP
    u8 texAnimId; /* jp: always 0xFF, its trails never start a texture animation */
#endif
    u8 primeCount;
} TrailEffect;

typedef struct {
    s32 key;
    void *data;
} TamEntry;

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

typedef struct {
    s32 key;
    u8 *data;
    s32 subKey;
} SpriteEntry;

extern BattleState *SUG_BATTLE;
extern s16 SUG_TARGET_HP[2];
extern MATRIX GsIDMATRIX;
extern s16 CAMERA_TARGET_MODEL;

s32 StoreImage(Rect16 *rect, void *p);
void initPolyF4Pair();

#endif /* DCB_SUGSEG_H */
