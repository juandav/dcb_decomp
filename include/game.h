#ifndef GAME_H
#define GAME_H

#include "common.h"

#define setlen(p, _len) (((P_TAG *)(p))->len = (u8)(_len))
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u32)(_addr))
#define getaddr(p) (u32)(((P_TAG *)(p))->addr)
#define getcode(p) (u8)(((P_TAG *)(p))->code)
#define setcode(p, _code) (((P_TAG *)(p))->code = (u8)(_code))
#define setSemiTrans(p, abe) \
    ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define getClut(x, y) (((y) << 6) | (((x) >> 4) & 0x3f))
#define setShadeTex(p, tge) \
    ((tge) ? setcode(p, getcode(p) | 1) : setcode(p, getcode(p) & ~1))
#define setRGB0(p, _r0, _g0, _b0) (p)->r0 = _r0, (p)->g0 = _g0, (p)->b0 = _b0
#define getTPage(tp, abr, x, y)                                                        \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | \
     (((y) & 0x200) << 2))
#define _get_mode(dfe, dtd, tpage) \
    ((0xe1000000) | ((dtd) ? 0x0200 : 0) | ((dfe) ? 0x0400 : 0) | ((tpage) & 0x9ff))
#define setDrawMode(p, dfe, dtd, tpage) \
    (setlen(p, 1), (p)->code[0] = _get_mode(dfe, dtd, tpage))
#define CUR_SPRT ((SprtPacket *)SPRITE_POOL_CURSOR)
#define DB(i) (((Graphics *)&GRAPHICS)->buffers[i])
#define PLAYER_DATA(p) (((PlayerProfile *)PLAYER_PROFILES)[p])
#define DUEL ((Duel *)D_801D8340)
#define PLAYER(p) ((Player *)DUEL_PLAYERS[p])
#define SPRITE_KIND(c) (*(s8 *)(D_801D833C + (c) * 36 + 0x22))

typedef struct {
    /* 0x00 */ s16 clip[4];
    /* 0x08 */ s16 ofs[2];
    /* 0x0C */ s16 tw[4];
    /* 0x14 */ u16 tpage;
    /* 0x16 */ u8 dtd;
    /* 0x17 */ u8 dfe;
    /* 0x18 */ u8 isbg;
    /* 0x19 */ u8 r0;
    /* 0x1A */ u8 g0;
    /* 0x1B */ u8 b0;
    /* 0x1C */ u32 dr_env[16];
} DRAWENV;
typedef struct {
    /* 0x00 */ s16 disp[4];
    /* 0x08 */ s16 screen[4];
    /* 0x10 */ u8 isinter;
    /* 0x11 */ u8 isrgb24;
    /* 0x12 */ u8 pad0;
    /* 0x13 */ u8 pad1;
} DISPENV;
typedef struct {
    /* 0x0000 */ DRAWENV draw;
    /* 0x005C */ DISPENV disp;
    /* 0x0070 */ u32 ot[0x1000];
    /* 0x4070 */ void *scenePackets;
    /* 0x4074 */ s32 unk4074;
    /* 0x4078 */ s32 primSlots[16];
    /* 0x40B8 */ s32 spritePool;
    /* 0x40BC */ s32 windowPrimPool;
} FrameBuffer;
typedef struct {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
} Rect16;
typedef struct {
    u8 b[4];
} Bytes4;
typedef struct {
    s32 vpx;
    s32 vpy;
    s32 vpz;
    s32 vrx;
    s32 vry;
    s32 vrz;
    s32 rz;
    void *super;
} GsRVIEW2;
typedef struct {
    u32 length;
    u32 *org;
    u32 offset;
    u32 point;
    u32 *tag;
} GsOT;
typedef struct {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad;
} SVECTOR;
typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    s32 pad;
} VECTOR;
typedef struct {
    s16 m[3][3];
    s32 t[3];
} MATRIX;
typedef struct GsCOORDINATE2 {
    /* 0x00 */ u32 flg;
    /* 0x04 */ MATRIX coord;
    /* 0x24 */ MATRIX workm;
    /* 0x44 */ void *param;
    /* 0x48 */ struct GsCOORDINATE2 *super;
    /* 0x4C */ struct GsCOORDINATE2 *sub;
} GsCOORDINATE2;
typedef struct {
    /* 0x000 */ GsOT ot[2];
    /* 0x028 */ GsCOORDINATE2 root;
    /* 0x078 */ u8 unk78[0x4C];
    /* 0x0C4 */ GsRVIEW2 view;
    /* 0x0E4 */ u8 unkE4[0x30];
    /* 0x114 */ s8 modelState[0x28];
    /* 0x13C */ void *models[24];
    /* 0x19C */ struct {
        s32 key;
        s32 value;
    } animCache[32];
} Scene3D;
typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void (*unk8[16])(FrameBuffer *, s32);
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 scene3dEnabled;
    /* 0x50 */ s32 vblanksPerFrame;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    u8 pad5A[0x1A];
    s32 snapCamera;
    u8 pad78[0x4];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 pad88[0x4];
    s16 targetModel;
    s16 unk8E;
    s16 unk90;
    s16 unk92;
    s16 unk94;
    u8 pad96[0x2];
    FrameBuffer buffers[2];
} Graphics;
typedef struct {
    /* 0x00 */ u16 rawHeld;
    /* 0x02 */ u16 rawPressed;
    /* 0x04 */ u16 rawReleased;
    /* 0x06 */ u16 rawRepeat;
    /* 0x08 */ s16 held;
    /* 0x0A */ s16 pressed;
    /* 0x0C */ s16 released;
    /* 0x0E */ s16 repeat;
    /* 0x10 */ s8 repeatEnabled;
    /* 0x11 */ s8 repeating;
    /* 0x12 */ s16 holdTime;
    /* 0x14 */ u16 repeatButtons;
    /* 0x16 */ s16 repeatDelay;
    /* 0x18 */ s16 repeatRate;
    /* 0x1A */ u8 padStatus;
    /* 0x1B */ u8 padType;
    /* 0x1C */ s16 padExId;
} PadState;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} FadePoly;
/* a sprite with its DR_TPAGE and DR_TWINs */
typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u8 r0;
    /* 0x05 */ u8 g0;
    /* 0x06 */ u8 b0;
    /* 0x07 */ u8 code;
    /* 0x08 */ s16 x0;
    /* 0x0A */ s16 y0;
    /* 0x0C */ u8 u0;
    /* 0x0D */ u8 v0;
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 w;
    /* 0x12 */ s16 h;
    /* 0x14 */ u32 tpage[2];
    /* 0x1C */ u32 twin[3];
    /* 0x28 */ u32 twin0[3];
} ScrollBgSprite;
typedef struct {
    /* 0x00 */ ScrollBgSprite buf[2];
    /* 0x68 */ s32 tim;
    /* 0x6C */ s8 mode;
    /* 0x6D */ s8 shownImage;
    /* 0x6E */ u8 scrollMode;
    /* 0x6F */ u8 scrollSpeed;
    /* 0x70 */ s16 scrollPos;
    /* 0x72 */ s16 brightness;
    /* 0x74 */ s16 w;
    /* 0x76 */ s16 h;
    /* 0x78 */ s16 x;
    /* 0x7A */ s16 y;
    /* 0x7C */ u16 texWindowW;
    /* 0x7E */ u16 texWindowH;
} ScrollBackground;
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 loc[4];
    /* 0x08 */ s32 fsize;
    /* 0x0C */ u8 fname[0x10];
    /* 0x1C */ s32 sector;
    /* 0x20 */ s32 remaining;
    /* 0x24 */ s32 size;
    /* 0x28 */ s32 avail;
    /* 0x2C */ u8 *cur;
    /* 0x30 */ u8 buf[0x1000];
} CdFile;
typedef struct {
    /* 0x00 */ s16 originX;
    /* 0x02 */ s16 originY;
    /* 0x04 */ Rect16 view;
    /* 0x0C */ Rect16 rect;
    /* 0x14 */ Rect16 cur;
    /* 0x1C */ Rect16 from;
    /* 0x24 */ Rect16 delta;
    /* 0x2C */ s32 label;
    /* 0x30 */ s16 scroll[4];
    /* 0x38 */ u8 palette;
    /* 0x39 */ u8 labelPalette;
    /* 0x3A */ s16 z;
    /* 0x3C */ u8 animFrames;
    /* 0x3D */ u8 animFrame;
    /* 0x3E */ u8 scrollStep;
    /* 0x3F */ u8 flags;
    /* 0x40 */ u8 brightness;
    /* 0x41 */ s8 animDone;
    /* 0x42 */ u8 style;
    /* 0x43 */ u8 scrollbarStyle;
} UiWindow;
typedef struct {
    u32 addr : 24;
    u32 len : 8;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
} P_TAG;
typedef struct {
    u32 tag;
    u32 code[1];
} DR_MODE;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 r3, g3, b3, p3;
    s16 x3, y3;
} POLY_G4;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad2;
} POLY_GT3;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad;
} POLY_FT3;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 u1, v1;
    u16 tpage;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
    u8 u2, v2;
    u16 pad2;
    u8 r3, g3, b3, p3;
    s16 x3, y3;
    u8 u3, v3;
    u16 pad3;
} POLY_GT4;
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 x1;
    s16 y1;
    u8 u1;
    u8 v1;
    u16 tpage;
    s16 x2;
    s16 y2;
    u8 u2;
    u8 v2;
    u16 pad1;
    s16 x3;
    s16 y3;
    u8 u3;
    u8 v3;
    u16 pad2;
} POLY_FT4;
typedef struct {
    u32 tag;
    u8 r0;
    u8 g0;
    u8 b0;
    u8 code;
    s16 x0;
    s16 y0;
    u8 u0;
    u8 v0;
    u16 clut;
    s16 w;
    s16 h;
} SPRT;
typedef struct {
    DR_MODE dm;
    SPRT sp;
} SprtPacket;
typedef struct {
    u32 tag;
    u32 code[2];
} DR_AREA;
typedef struct {
    /* 0x0 */ s8 bannerState;
    /* 0x1 */ u8 playerLabel;
    /* 0x2 */ u8 player;
    /* 0x3 */ s8 phase;
    /* 0x4 */ s8 next;
    /* 0x5 */ s8 cur;
    /* 0x6 */ s8 y;
    /* 0x7 */ s8 next2;
    /* 0x8 */ s8 cur2;
    /* 0x9 */ s8 y2;
    /* 0x0A */ s8 curLabel2;
    /* 0x0B */ s8 bannerStep;
    /* 0x0C */ s8 bannerLabel;
    /* 0x0D */ s8 bannerPhase;
    /* 0x0E */ s16 echoAge;
    /* 0x10 */ s16 timer;
    /* 0x12 */ s16 px;
    /* 0x14 */ s16 py;
    /* 0x16 */ s16 tx;
    /* 0x18 */ s16 ty;
} MsgBar;
typedef struct {
    u32 tag;
    u32 tpage;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 u0, v0;
    u16 clut;
    s16 w, h;
} ScreenSprt;
typedef struct {
    u32 tag;
    u32 code[2];
} DR_STP;
typedef struct {
    /* 0x000 */ ScreenSprt sprt[2][2];
    /* 0x060 */ POLY_FT4 poly[2][2];
    /* 0x100 */ DR_STP stp[2];
    /* 0x118 */ s16 x;
    /* 0x11A */ s16 y;
    /* 0x11C */ s16 px[2][4];
    /* 0x12C */ s16 py[2][4];
    /* 0x13C */ u8 r;
    /* 0x13D */ u8 g;
    /* 0x13E */ u8 b;
    /* 0x13F */ u8 mode;
    /* 0x140 */ u16 abr;
} Screen;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;
typedef struct {
    /* 0x0 */ s32 frameCount;
    /* 0x4 */ void *data;
} AnimClip;
typedef struct {
    /* 0x0000 */ u8 unk0[0x1F80];
    /* 0x1F80 */ s16 *bonepos[32];
    /* 0x2000 */ s32 scale[34][4];
    /* 0x2220 */ AnimClip anims[16];
    /* 0x22A0 */ u8 unk22A0[0x430];
    /* 0x26D0 */ s32 clutOffset;
    /* 0x26D4 */ s32 tpageOffset;
    /* 0x26D8 */ s32 rootOnly;
    /* 0x26DC */ u8 unk26DC[0x18];
    /* 0x26F4 */ void *pak;
} Model2220;
/* One animated channel of a bone, eased between two keys (model_anim.c) */
typedef struct {
    /* 0x0 */ s32 value;    /* fixed point: angle << 20, position/scale << 16 */
    /* 0x4 */ s32 velocity; /* added to value every frame */
    /* 0x8 */ s32 accel0;   /* added to velocity in the first half of the key */
    /* 0xC */ s32 accel1;   /* ... and this one in the second half */
} AnimChan;
/* The integer part of a position or scale channel: the high half of value */
#define ANIM_CHAN_INT(chan) (((s16 *)&(chan).value)[1])
typedef struct {
    /* 0x00 */ AnimChan ch[9];
} BoneAnim;
typedef struct {
    /* 0x00 */ u8 r;
    /* 0x01 */ u8 g;
    /* 0x02 */ u8 b;
    /* 0x03 */ u8 unk3;
    /* 0x04 */ u16 clut;
    /* 0x06 */ u16 tpage;
    /* 0x08 */ u8 u;
    /* 0x09 */ u8 v;
    /* 0x0A */ u8 w;
    /* 0x0B */ u8 h;
    /* 0x0C */ u8 unkC[4];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
} SprtInfo;
typedef struct {
    u8 b[8];
} Bytes8;
typedef struct {
    /* 0x000 */ u8 unk0[0x13C];
    /* 0x13C */ u8 texAnim[0x20];
    /* 0x15C */ u8 *tpagePrims[2];
    /* 0x164 */ u8 *prims[2];
    /* 0x16C */ void *vertices;
    /* 0x170 */ Bytes8 texCoords;
    /* 0x178 */ s32 tpage;
    /* 0x17C */ s32 clut;
    /* 0x180 */ s32 n;
    /* 0x184 */ u8 type;
    /* 0x185 */ Bytes4 innerColor;
    /* 0x189 */ Bytes4 midColor;
    /* 0x18D */ Bytes4 outerColor;
    /* 0x194 */ s32 fixedOtz;
    /* 0x198 */ s32 texDepth;
    /* 0x19C */ s16 shape[5];
    /* 0x1A6 */ s16 brightness;
    /* 0x1A8 */ s16 prevBrightness;
    /* 0x1AA */ u8 axisMode;
    /* 0x1AB */ u8 cullBackface;
    /* 0x1AC */ u8 abr;
    /* 0x1AD */ s8 texAnimActive;
} RingEffect;
typedef struct {
    /* 0x00 */ u16 cards[30];
    /* 0x3C */ char name[0x28];
    /* 0x64 */ u8 unk64[4];
    /* 0x68 */ u8 unk68[2];
    /* 0x6A */ u8 stageId;
    /* 0x6B */ u8 unk6B[2];
    /* 0x6D */ u8 partnerArmor;
} PresetDeck;
typedef struct {
    /* 0x00 */ u32 mode;
    /* 0x04 */ Rect16 *crect;
    /* 0x08 */ u32 *caddr;
    /* 0x0C */ Rect16 *prect;
    /* 0x10 */ u32 *paddr;
} TIM_IMAGE;
typedef struct {
    /* 0x0 */ s16 value;
    /* 0x2 */ u8 type;
    /* 0x3 */ u8 timer;
    /* 0x4 */ s16 x;
    /* 0x6 */ s16 y;
} Popup;
typedef struct {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 index;
    /* 0x2 */ s16 id;
    /* 0x4 */ s8 *card;
} CardSlot;
typedef struct {
    /* 0x000 */ u8 inUse;
    /* 0x001 */ u8 unk1[3];
    /* 0x004 */ s32 unk4[4];
    /* 0x014 */ CardSlot cards[30];
    /* 0x104 */ s32 unk104;
    /* 0x108 */ u16 unk108[3];
    /* 0x10E */ u8 unk10E[2];
} PlayerDeck;
typedef struct {
    /* 0x000 */ u8 unk0[0x14];
    /* 0x014 */ CardSlot cards[30];
    /* 0x104 */ u8 unk104[0xC];
    /* 0x110 */ s32 unk110;
    /* 0x114 */ s8 *battleCard;
    /* 0x118 */ u8 unk118[2];
    /* 0x11A */ s16 shufflePasses;
    /* 0x11C */ s16 stats[5];
    /* 0x126 */ s16 displayedStats[5];
    /* 0x130 */ Popup statPopups[5];
    /* 0x158 */ s16 unk158;
    /* 0x15A */ s16 hpAfterBattle;
    /* 0x15C */ s16 baseAttackPowers[3];
    /* 0x162 */ s16 damageTaken;
    /* 0x164 */ s16 hpGain;
    /* 0x166 */ u8 unk166[8];
    /* 0x16E */ s16 attackHighlightTimer;
    /* 0x170 */ s16 unk170[4];
    /* 0x178 */ u32 usedAttack : 2;
    /* 0x178 */ u32 attackChoice : 2;
    /* 0x178 */ u32 shownAttack : 2;
    /* 0x178 */ u32 unk178_6 : 1;
    /* 0x178 */ u32 unk178_7 : 1;
    /* 0x178 */ u32 unk178_8 : 1;
    /* 0x178 */ u32 unk178_9 : 2;
    /* 0x178 */ u32 unk178_11 : 1;
    /* 0x178 */ u32 unk178_12 : 1;
    /* 0x178 */ u32 unk178_13 : 2;
    /* 0x178 */ u32 statPenalty : 2;
    /* 0x178 */ u32 controller : 2;
    /* 0x178 */ u32 specialty : 3;
    /* 0x178 */ u32 unk178_22 : 2;
    /* 0x178 */ u32 unk178_24 : 2;
    /* 0x178 */ u32 unk178_26 : 2;
    /* 0x178 */ u32 unk178_28 : 2;
    /* 0x178 */ u32 unk178_30 : 1;
    /* 0x178 */ u32 unk178_31 : 1;
    /* 0x17C */ u8 wins;
    /* 0x17D */ s8 onlineDeck[30];
    /* 0x19B */ s8 offlineDeck[30];
    /* 0x1B9 */ s8 hand[4];
    /* 0x1BD */ u8 unk1BD[5];
    /* 0x1C2 */ s8 dpSlots[8];
    /* 0x1CA */ s8 digimonStack[3];
    /* 0x1CD */ s8 playedCard;
    /* 0x1CE */ char name[1];
} Player;
typedef struct {
    /* 0x00 */ u8 unk0[0xE];
    /* 0x0E */ s16 value;
    /* 0x10 */ u8 unk10[0x10];
} SupportCondition;
typedef struct {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ s16 value;
    /* 0xE */ u8 unkE[2];
} SupportAction;
typedef struct {
    /* 0x00 */ s16 power;
    /* 0x02 */ u8 unk2[4];
    /* 0x06 */ char name[0x16];
} CardAttack;
typedef struct {
    /* 0x000 */ s16 id;
    /* 0x002 */ u8 type;
    /* 0x003 */ char name[0x17];
    /* 0x01A */ u8 attr;
    /* 0x01B */ s8 dpCost;
    /* 0x01C */ s8 dpBonus;
    /* 0x01D */ u8 unk1D;
    /* 0x01E */ s16 hp;
    /* 0x020 */ CardAttack attack[3];
    /* 0x074 */ SupportCondition supportConditions[2];
    /* 0x0B4 */ SupportAction supportActions[3];
    /* 0x0E4 */ s8 crossEffect;
    /* 0x0E5 */ u8 modelId;
    /* 0x0E6 */ s8 supportIcon;
    /* 0x0E7 */ u8 supportText[4][0x15];
    /* 0x13B */ u8 unk13B;
} DigimonCardData;
typedef struct {
    /* 0x000 */ DigimonCardData card[2];
    /* 0x278 */ u8 *baseCard;
    /* 0x27C */ u8 *armorCard;
    /* 0x280 */ s16 hpBonus;
    /* 0x282 */ s16 attackBonus[3];
    /* 0x288 */ u8 cardId;
    /* 0x289 */ u8 level;
    /* 0x28A */ s16 exp;
    /* 0x28C */ s8 equippedAbilities[3];
    /* 0x28F */ u8 unlockedArmors[3];
    /* 0x292 */ u8 unk292[6];
} Partner;
typedef struct {
    /* 0x0000 */ char name[0xD];
    /* 0x000D */ u8 unkD;
    /* 0x000E */ u8 unkE;
    /* 0x000F */ u8 unkF;
    /* 0x0010 */ s16 unk10;
    /* 0x0012 */ u16 seenCardCount;
    /* 0x0014 */ s16 unk14;
    /* 0x0016 */ s16 profileSize;
    /* 0x0018 */ u16 battleWins;
    /* 0x001A */ u16 battleLosses;
    /* 0x001C */ u16 versusWins;
    /* 0x001E */ u16 versusLosses;
    /* 0x0020 */ u32 unk20_0 : 1;
    /* 0x0020 */ u32 unk20_1 : 1;
    /* 0x0020 */ u32 unk20_2 : 1;
    /* 0x0020 */ u32 skipBattleAnimation : 1;
    /* 0x0020 */ u32 unk20_4 : 28;
    /* 0x0024 */ s32 playTime;
    /* 0x0028 */ u32 tamerRank : 3;
    /* 0x0028 */ u32 collectorRank : 3;
    /* 0x0028 */ u32 battleRank : 3;
    /* 0x0029 */ u32 unk28_9 : 1;
    /* 0x0029 */ u32 unk28_10 : 1;
    /* 0x0029 */ u32 unk28_11 : 1;
    /* 0x0029 */ u32 unk28_12 : 1;
    /* 0x0029 */ u32 unk28_13 : 1;
    /* 0x0029 */ u32 unk28_14 : 18;
    /* 0x002C */ s32 unk2C;
    /* 0x0030 */ u8 unk30[6];
    /* 0x0036 */ u16 attackCounts[3];
    /* 0x003C */ u8 ownedAbilities[0x10];
    /* 0x004C */ s16 unk4C;
    /* 0x004E */ s16 unk4E;
    /* 0x0050 */ s16 unk50;
    /* 0x0052 */ s16 unk52;
    /* 0x0054 */ s16 unk54;
    /* 0x0056 */ u16 unk56;
    /* 0x0058 */ u8 unk58[0x28];
    /* 0x0080 */ Partner partners[3];
    /* 0x0848 */ s16 unk848[0x20];
    /* 0x0888 */ u16 unk888[0x8E];
    /* 0x09A4 */ u16 unk9A4[0x8E];
    /* 0x0AC0 */ u16 opponentDeckFlags[0x9F];
    /* 0x0BFE */ u16 unkBFE[0x9F];
    /* 0x0D3C */ s16 unkD3C[0xBF][3];
    /* 0x11B6 */ u16 unk11B6[0xBF];
    /* 0x1334 */ u16 unk1334[0xBF];
    /* 0x14B2 */ u8 cardCollection[0x12D];
    /* 0x15DF */ u8 unk15DF;
    /* 0x15E0 */ u16 cardCopySerials[301][6];
    /* 0x23FC */ s32 unk23FC[12];
    /* 0x242C */ u8 unk242C[9];
    /* 0x2435 */ u8 unk2435[3];
    /* 0x2438 */ PlayerDeck savedDecks[3];
    /* 0x2768 */ s16 rewardCards[3];
    /* 0x276E */ s8 rewardResults[3];
    /* 0x2771 */ u8 unk2771[3];
} PlayerProfile;
typedef struct {
    /* 0x000 */ u8 unk0[0x1A2];
    /* 0x1A2 */ s16 unk1A2;
    /* 0x1A4 */ u8 unk1A4;
    /* 0x1A5 */ u8 unk1A5[3];
    /* 0x1A8 */ u8 unk1A8;
    /* 0x1A9 */ u8 unk1A9;
} Unk8006E054Sub;
typedef struct {
    /* 0x0000 */ u8 *npcDeckFile;
    /* 0x0004 */ u8 opponentDeckIndex;
    /* 0x0005 */ u8 unk5[3];
    /* 0x0008 */ PresetDeck opponentDeck;
    /* 0x0076 */ u8 unk76[2];
    /* 0x0078 */ Partner partnerBackup[2][3];
    /* 0x1008 */ s16 npcDeckIndex[2];
    /* 0x100C */ Unk8006E054Sub *unk100C;
    /* 0x1010 */ u8 unk1010[0x17];
    /* 0x1027 */ u8 unk1027;
} SessionData;
typedef struct {
    /* 0x0 */ u32 attribute;
    /* 0x4 */ GsCOORDINATE2 *coord2;
    /* 0x8 */ u32 *tmd;
    /* 0xC */ u32 id;
} GsDOBJ4;
typedef struct {
    /* 0x00 */ AnimChan rot[3];
    /* 0x30 */ AnimChan pos[3];
    /* 0x60 */ AnimChan scale[3];
} BoneKeys;
/* The playback state of a model's animation */
typedef struct {
    /* 0x00 */ s32 clip;       /* index in Model.anims */
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 keyTimer;   /* frames left in the current key; negative when paused or stopped */
    /* 0x0C */ s32 key;        /* the key being eased to, -1 after the last one */
    /* 0x10 */ s32 halfTimer;  /* frames left in the first half of the key */
    /* 0x14 */ s32 keyCount;
    /* 0x18 */ s32 loopKey;    /* key that follows the last one (negative: stop) */
    /* 0x1C */ float timeScale; /* key durations are multiplied by it */
} ModelAnimState;
typedef struct {
    /* 0x0000 */ s32 dataSize;
    /* 0x0004 */ s16 nobj;
    /* 0x0006 */ s16 id;
    /* 0x0008 */ VECTOR pos;
    /* 0x0018 */ VECTOR scale;
    /* 0x0028 */ GsCOORDINATE2 root;
    /* 0x0078 */ GsCOORDINATE2 coord[32];
    /* 0x0A78 */ SVECTOR rot;
    /* 0x0A80 */ SVECTOR rots[32];
    /* 0x0B80 */ GsDOBJ4 obj[32];
    /* 0x0D80 */ BoneKeys keys[32];
    /* 0x1F80 */ s16 *bonepos[32];
    /* 0x2000 */ VECTOR boneScale[32];
    /* 0x2200 */ ModelAnimState anim;
    /* 0x2220 */ AnimClip anims[16];
    /* 0x22A0 */ u8 unk22A0[0x10];
    /* 0x22B0 */ MATRIX lw[32];
    /* 0x26B0 */ s8 parent[32];
    /* 0x26D0 */ s32 clutOffset;
    /* 0x26D4 */ s32 tpageOffset;
    /* 0x26D8 */ s32 rootOnly;
    /* 0x26DC */ void *data;
    /* 0x26E0 */ s32 link;
    /* 0x26E4 */ Rect16 prect;
    /* 0x26EC */ Rect16 crect;
    /* 0x26F4 */ void *pak;
    /* 0x26F8 */ u8 clut[0x200];
} Model;
typedef struct {
    s16 id;
    s16 sub;
    s32 size;
} Chunk;
typedef struct {
    s16 left;
    s16 right;
} SpuVolume;
typedef struct {
    u32 voice;
    u32 mask;
    SpuVolume volume;
    SpuVolume volmode;
    SpuVolume volumex;
    u16 pitch;
    u16 note;
    u16 sample_note;
    s16 envx;
    u32 addr;
    u32 loop_addr;
    s32 a_mode;
    s32 s_mode;
    s32 r_mode;
    u16 ar;
    u16 dr;
    u16 sr;
    u16 rr;
    u16 sl;
    u16 adsr1;
    u16 adsr2;
} SpuVoiceAttr;
typedef struct { u8 unk0[0x1F80]; s16 *unk1F80[8]; } Unk1F80;
typedef struct {
    /* 0x0 */ u8 u[3];
    /* 0x3 */ u8 v[3];
    /* 0x6 */ u8 w[2];
    /* 0x8 */ u8 h[2];
    /* 0xA */ u8 left;
    /* 0xB */ u8 top;
    /* 0xC */ u8 right;
    /* 0xD */ u8 bottom;
    /* 0xE */ u8 label;
} WindowStyle;
typedef struct {
    u32 tpage;
    u32 clut;
} ModelTextureSlot;
typedef struct {
    /* 0x00 */ char name[20];
    /* 0x14 */ s32 attr;
    /* 0x18 */ s32 size;
    /* 0x1C */ void *next;
    /* 0x20 */ s32 head;
    /* 0x24 */ char system[4];
} DirEntry;
typedef struct {
    /* 0x000 */ s32 count;
    /* 0x004 */ s32 blocks;
    /* 0x008 */ DirEntry files[15];
} CardDir;
typedef struct {
    /* 0x0 */ s16 id;
    /* 0x2 */ u8 used;
    /* 0x3 */ u8 age;
} CardCache;
typedef struct {
    /* 0x000 */ u8 unk0[0x50];
    /* 0x050 */ Player *firstAttacker;
    /* 0x054 */ Player *secondAttacker;
    /* 0x058 */ u8 *cursor;
    /* 0x05C */ u8 unk5C[0x784];
    /* 0x7E0 */ CardCache cache[6];
    /* 0x7F8 */ void *sprites;
    /* 0x7FC */ s32 cpuWaitFrames;
    /* 0x800 */ s32 unk800;
    /* 0x804 */ s32 cpuResult;
    /* 0x808 */ s16 fade;
    /* 0x80A */ s16 unk80A;
    /* 0x80C */ s16 unk80C;
    /* 0x80E */ s16 unk80E;
    /* 0x810 */ s8 state;
    /* 0x811 */ s8 loadBusy;
    /* 0x812 */ s8 stopArtLoader;
    /* 0x813 */ s8 stopStageTask;
    /* 0x814 */ s8 stopCpuTask;
    /* 0x815 */ s8 stopTurnLoop;
    /* 0x816 */ s8 cpuRequest;
    /* 0x817 */ s8 turnPlayer;
    /* 0x818 */ s8 step;
    /* 0x819 */ s8 returnStep;
    /* 0x81A */ s8 viewPlayer;
    /* 0x81B */ u8 cursorPlayer;
    /* 0x81C */ s8 cursorSlot;
    /* 0x81D */ s8 unk81D;
    /* 0x81E */ u8 winner;
    /* 0x81F */ s8 tutorial;
    /* 0x820 */ u8 unk820[2];
    /* 0x822 */ s8 awaitingInput;
    /* 0x823 */ s8 unk823;
    /* 0x824 */ s8 unk824;
    /* 0x825 */ u8 unk825;
    /* 0x826 */ u8 artSlot;
    /* 0x827 */ u8 cpuPlayer;
    /* 0x828 */ s32 ringMode; /* -1: no ring drawn */
    /* 0x82C */ s32 ringX;
    /* 0x830 */ s32 ringY;
    /* 0x834 */ s32 ringRadius;
    /* 0x838 */ s32 ringWidth;
    /* 0x83C */ s32 inPolygonBattle;
} Duel;
typedef struct {
    s8 bg;
    u8 texAnimFrames;
    u8 texAnimDelay;
    s8 flags;
    u8 rgb[3];
    u8 unk7;
} ArenaStage;
typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 id;
} CardCursor;
typedef struct {
    /* 0x00 */ u8 unk0[0xA5];
    /* 0xA5 */ s8 choice;
} Window;
typedef struct {
    /* 0x00 */ u8 rgbc[4];
    /* 0x04 */ u8 fade[4];
    /* 0x08 */ u8 from[3];
    /* 0x0B */ u8 t;
    /* 0x0C */ u8 to[3];
    /* 0x0F */ u8 num;
    /* 0x10 */ u16 clut;
    /* 0x12 */ u16 tpage;
    /* 0x14 */ u8 pal;
    /* 0x15 */ u8 flags;
    /* 0x16 */ u8 u;
    /* 0x17 */ u8 v;
    /* 0x18 */ VECTOR pos;
    /* 0x28 */ SVECTOR rot;
    /* 0x30 */ s32 scale;
    /* 0x34 */ s16 sx;
    /* 0x36 */ s16 sy;
    /* 0x38 */ s32 z;
} CardSprite;
typedef struct {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 unkD[3];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[0x10];
} HudAnchor;
typedef struct {
    /* 0x00 */ u8 unk0[0x48];
    /* 0x48 */ HudAnchor slot[3];
    /* 0xB4 */ u8 unkB4[0x24];
} Board;
typedef struct {
    /* 0x00 */ u8 unk0[0x28];
    /* 0x28 */ s16 rx;
    /* 0x2A */ s16 ry;
    /* 0x2C */ s16 rz;
    /* 0x2E */ u8 unk2E[0xE];
} Unk7F8;
typedef struct {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 param;
    /* 0x2 */ u8 actionStart;
    /* 0x3 */ u8 actionCount;
    /* 0x4 */ s16 value;
    /* 0x6 */ s16 unk6;
} PartnerAbility;

extern s32 SPRITE_POOL_CURSOR;
extern u16 SYSTEM_TEX_X;
extern u16 SYSTEM_TEX_Y;
extern s32 PLAYER_PROFILES;
extern FrameBuffer *CURRENT_FRAME_BUFFER;
extern u8 FRAME_BUFFER_INDEX;
extern s32 FRAME_INTERVAL;
extern s32 GRAPHICS;
extern s32 PAD_INPUT_ENABLED;
extern char PATH_DRV_SUFFIX[];
extern s32 FRAME_CALLBACKS;
extern char STR_TOO_MANY_WINDOWS[];
extern PadState *PAD_STATES[];
extern s32 TEXT_WIDTH;
extern s32 TEXT_HEIGHT;
extern s32 LOADED_FILE_SIZE;
extern TIM_IMAGE LOADED_TIM;
extern s32 SCENE_3D_ENABLED;
extern Scene3D *SCENE_3D;
extern s32 OVERLAY_LOAD_ADDR;
extern s32 MUSIC_CHANGE_BUSY;
extern s32 PENDING_MUSIC_CHANGES;
extern u8 *DIGIMON_CARDS;
extern void *D_8006E054;
extern void *D_801D8340;
extern u8 *DUEL_PLAYERS[];
extern s32 PATH_OPENSEG;
extern s32 PATH_SAISEG;
extern s32 PATH_EVOSEG;
extern s32 PATH_SUBSEG;
extern s32 PATH_BG_ARC;
extern ScrollBackground SCROLL_BACKGROUND;
extern s32 ATTACK_ICON_TIMER;
extern s32 D_801D8278;
extern u8 *D_801D83EC;
extern u8 *D_801D833C;
extern MsgBar DUEL_MSG_BAR;
extern u8 D_801D83D1;
extern s32 runSceneCameraTask;
extern s32 PATH_KAWSEG_BIN;
extern s32 PATH_DECK2_DEK;

s32 VSync(s32);
void SetSemiTrans(void *, s32);
void SetShadeTex(void *, s32);
s32 rand(void);
s32 sprintf(char *, const char *, ...);
s32 DrawSync(s32);
s32 LoadImage(s16 *, s32);
s32 func_80014A90();
void ResetCallback(void);
void SetDispMask(s32);
void GsInitGraph(u16, u16, u16, u16, u16);
s32 ClearImage(Rect16 *, s32, s32, s32);
void MoveImage(Rect16 *rect, s32 x, s32 y);
void SsInit(void);
void func_800149A8(s32, s32, void (*)(), s32, s32, s32, s32);
s32 func_8006A804();
s32 func_8006A814();
long func_8006A794(unsigned long, long, long, long (*)());
long func_8006A7C4(long);
s32 SetRCnt(u32, u16, s32);
s32 StartRCnt(u32);
extern int func_80014A00(int);
void func_800677A4(POLY_FT4 *);
void SetDrawStp(DR_STP *, s32);
void func_801EAD04(void);
u32 GetTPage(s32, s32, s32, s32);
void SetGraphDebug(s32);
void InitGeom(void);
s32 func_80014C08(s32);
void func_80014AC8(void);
void ClearOTagR(u32 *, s32);
void GsSwapDispBuff(void);
void PutDispEnv(DISPENV *);
void PutDrawEnv(DRAWENV *);
void DrawOTag(u32 *);
void func_8006A884(s32);
s32 func_800149B8();
s32 func_801E055C(s32);
s32 CdInit(void);
s32 CdControlB(u8, u8 *, u8 *);
void func_8005A344(s32);
s32 CdIntToPos(s32, u8 *);
s32 CdRead(s32, u8 *, s32);
s32 CdReadSync(s32, u8 *);
int toupper(int);
s32 CdSearchFile(void *, char *);
s32 CdPosToInt(void *);
s32 func_8005A364(s32, s32);
extern int func_8005A364(int, int);
void SetDrawTPage(void *, s32, s32, s32);
s32 SetTexWindow(void *, s16 *);
void GetDispEnv(DISPENV *);
void SetDrawArea(DR_AREA *, Rect16 *);
extern int printf(const char *, ...);
s32 strlen(u8 *);
s32 PadInitDirect(void *, void *);
s32 PadStartCom(void);
s32 PadGetState(s32);
s32 PadInfoMode(s32, s32, s32);
void func_80014970(void);
void func_800149A0(void);
long catan(long);
s32 func_80014A48();
s32 OpenTIM(u32 *);
TIM_IMAGE *ReadTIM(TIM_IMAGE *);
void SetDefDrawEnv(DRAWENV *, s32, s32, s32, s32);
void SetDefDispEnv(DISPENV *, s32, s32, s32, s32);
s32 MargePrim(void *, void *);
s32 SetDrawMode(void *, s32, s32, s32, s32 *);
void func_80067724(POLY_FT3 *);
void func_80067764(POLY_GT3 *);
void func_800677E4(POLY_GT4 *);
void func_80067784(void *);
void func_800677C4(POLY_G4 *);
void func_80067744(void *);
void func_80067704(void *);
void func_80067904(void *);
void func_800678E4(void *);
void func_80067924(void *);
void SetLineG3(void *);
void func_80067974(void *);
void SetLineG4(void *);
void func_80067804(void *);
void func_80067824(void *);
void func_80067844(SPRT *);
void func_80067864(void *);
void func_80067884(void *);
void func_800678A4(void *);
void func_800678C4(TILE *);
s32 AddPrim(s32 *, s32);
s32 RotAverageNclip3(s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);
s32 RotTransPers3(s32, s32, s32, s32, s32, s32, s32 *, s32 *);
s32 RotAverageNclip4(s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);
s32 RotTransPers4(s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *);
s32 RotTransPers(s32, s32, s32 *, s32 *);
s32 RotMatrix(void *, void *);
s32 TransMatrix(MATRIX *, VECTOR *);
s32 ScaleMatrix(void *, void *);
void composeTransformMatrix(SVECTOR *, VECTOR *, VECTOR *, MATRIX *, s32);
MATRIX *MulMatrix2(MATRIX *, MATRIX *);
s32 RotTrans(u16 *, void *, s32 *);
s32 SetRotMatrix(s32);
s32 func_8005C444();
s32 RotMatrixYXZ(void *, void *);
void GsInitCoordinate2(GsCOORDINATE2 *, GsCOORDINATE2 *);
s32 bzero(Scene3D *, s32);
void StoreImage2(Rect16 *, u32 *);
void GsMapModelingData(u32 *);
void GsLinkObject4(u32, void *, s32);
s32 PushMatrix();
s32 PopMatrix();
void GsGetLws(GsCOORDINATE2 *, MATRIX *, MATRIX *);
void *memset(void *, s32, s32);
void func_8005C484(s32, s32);
void func_8005C4A4(s32);
void func_80062484(s32);
s32 GsSetRefView2(GsRVIEW2 *);
s32 GsSetAmbient(s32, s32, s32);
s32 GsSetLightMode(s32);
s32 func_8005C464(s32, s32, s32);
void GsInit3D(void);
s32 SsSetMVol(s32, s32);
s32 SsSetTableSize(s32 *, s32, s32);
s32 SsSetTickMode(s32);
s32 SsStart();
s32 func_80055740();
void SsVabClose(s16);
void bcopy(void *, void *, s32);
s16 SsSeqOpen(u8 *, s16);
void SsSeqClose(s16);
s32 SpuClearReverbWorkArea(s32);
s32 SsUtSetReverbDepth(s32, s32);
s32 SsUtSetReverbType(s16);
s32 func_80051C70();
s32 func_80051C90();
void SpuSetVoiceAttr(SpuVoiceAttr *);
s16 SsVabOpenHeadSticky(u8 *, s16, s32);
s32 SsVabTransBody(s32, s16);
s32 SsVabTransCompleted(s32);
s32 func_801DFBAC(s32 *);
extern short SsUtKeyOnV(short voice, short vabId, short prog, short tone,
                        short note, short fine, short voll, short volr);
s32 SsUtKeyOffV(s16);
s32 SsUtAllKeyOff(s32);
s32 SsSeqStop(s16);
void SsSeqGetVol(s16, s16, s16 *, s16 *);
void SsSeqSetVol(s16, s16, s16);
void SsSeqPlay(s16, char, s16);
s32 InitCARD(s32);
void StartCARD(void);
void func_80068804(void);
s32 func_8006A7B4(s32);
s32 _card_clear(s32);
s32 func_80068814(s32);
s32 func_80068824(s32);
s32 _card_format(s32);
s32 func_8006A824(char *, s32);
s32 func_8006A864(s32);
s32 func_8006A834(s32, s32, s32);
s32 func_8006A854(s32, void *, s32);
s32 func_8006A844(s32, void *, s32);
DirEntry *firstfile(char *, DirEntry *);
DirEntry *func_8006A874(DirEntry *);
char *strcpy(char *, const char *);
void D_801EBAFC();
void D_801F00F4();
void D_801E4D80();
void D_801E8E88();
void D_801E8C04();
void D_801E4B34();
void D_801EA2F8();
void D_801E6454();
void D_801EB2E8();
s32 func_801EBD34(void);
s32 rsin(s32);
s32 rcos(s32);
long SquareRoot0(long);
void VectorNormal(VECTOR *, VECTOR *);
s32 func_801E6C78(s32, s32, RingEffect *, u8 *, s32);
void func_801E7020(u8 *);
void func_801E72D4(u8 *);
s32 func_801E6AA4();
s32 func_801E9F5C();
s32 func_801EA374();
s32 func_801EA558();
s32 func_801EA8B4();
s32 func_801EAB4C();
s32 func_801EBACC();
s32 func_801EC4CC();
s32 func_801EC528();
s32 func_801EC570();
s32 func_801EC608();
s32 func_801EC7C0();
s32 func_801EC84C();
void func_801EC8E0(s32 player, s32 slot);
s32 func_801ECA30();
s32 func_801ECAC4();
s32 func_801ECB40();
s32 func_801ECBCC();
s32 func_801ECC58();
s32 func_801ECD68();
s32 func_801ECE24();
s32 func_801ECF0C();
s32 func_801F6214();
s32 func_801F6268();
s32 func_801FA4E4();
s32 func_801FA780();
void func_801FB444();
int abs(int);
s32 func_801F8200();
s32 func_801F8854();
s32 func_801F8998(s32, s32, s32, s32, s32);
s32 func_80014A00(s32);
s32 func_801F848C();
s32 func_801F88E8();
s32 func_801EB53C(s32);
s32 func_801F97F4();
MATRIX *CompMatrix(MATRIX *, MATRIX *, MATRIX *);
s32 RotAverage4(SVECTOR *, SVECTOR *, SVECTOR *, SVECTOR *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
void func_801F8E34(void *, s32);
MATRIX *MulMatrix(MATRIX *, MATRIX *);
MATRIX *MatrixNormal(MATRIX *, MATRIX *);
MATRIX *TransposeMatrix(MATRIX *, MATRIX *);
void D_801DF47C();

#endif /* GAME_H */
