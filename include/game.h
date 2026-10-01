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
#define DUEL ((Duel *)DUEL_STATE)
#define PLAYER(p) ((Player *)DUEL_PLAYERS[p])
/* the bytes of a card's CardAnim (battle_hud.h) and where its state is */
#if VERSION_JP
#define CARD_ANIM_SIZE 40
#define SPRITE_KIND(c) (*(s8 *)(CARD_ANIMS + (c) * CARD_ANIM_SIZE + 4))
#elif VERSION_US || VERSION_EU
#define CARD_ANIM_SIZE 36
#define SPRITE_KIND(c) (*(s8 *)(CARD_ANIMS + (c) * CARD_ANIM_SIZE + 0x22))
#endif
/* the SPRITE_KIND states that send a card somewhere (tickCardMotion) */
#if VERSION_JP
#define CARD_MOVE_TO_ONLINE_DECK 1
#define CARD_MOVE_TO_HAND 3
#define CARD_MOVE_TO_OFFLINE_DECK 6
#define CARD_MOVE_TO_DIGIMON_STACK 9
#define CARD_MOVE_TO_PLAYED 14
#define CARD_MOVE_DRAWN_TO_PLAYED 19
#define CARD_MOVE_TO_DP_SLOTS 24
#elif VERSION_US || VERSION_EU
#define CARD_MOVE_TO_ONLINE_DECK 1
#define CARD_MOVE_TO_HAND 3
#define CARD_MOVE_TO_OFFLINE_DECK 8
#define CARD_MOVE_TO_DIGIMON_STACK 11
#define CARD_MOVE_TO_PLAYED 16
#define CARD_MOVE_DRAWN_TO_PLAYED 21
#define CARD_MOVE_TO_DP_SLOTS 26
#endif

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
#if VERSION_JP
    /* 0x4078 */ s32 primSlots[17]; /* jp has one more slot */
    /* 0x40BC */ s32 spritePool;
    /* 0x40C0 */ s32 windowPrimPool;
#elif VERSION_US || VERSION_EU
    /* 0x4078 */ s32 primSlots[16];
    /* 0x40B8 */ s32 spritePool;
    /* 0x40BC */ s32 windowPrimPool;
#else
#error "FrameBuffer: version not checked"
#endif
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
    u8 r, g, b, cd;
} CVECTOR;
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
    /* 0x078 */ u8 viewMatrix[0x20]; /* the camera's view MATRIX, also the parent of effects placed in view space */
    /* 0x098 */ VECTOR unk98;
    /* 0x0A8 */ SVECTOR unkA8;
    /* 0x0B0 */ VECTOR unkB0;
    /* 0x0C0 */ s32 unkC0;
    /* 0x0C4 */ GsRVIEW2 view;
    /* 0x0E4 */ u8 flatLights[0x30]; /* three GsF_LIGHTs for GsSetFlatLight */
    /* 0x114 */ s8 modelState[24];
    /* 0x12C */ s8 texAnimFrame;  /* the arena stage's texture animation */
    /* 0x12D */ s8 texAnimTimer;
    /* 0x12E */ s8 texAnimFrames;
    /* 0x12F */ s8 texAnimDelay;
#if VERSION_JP
    /* 0x130 */ u8 unk130[4]; /* jp: the fields below are 4 bytes lower */
#elif VERSION_US || VERSION_EU
    /* 0x130 */ u8 unk130[8];
#endif
    /* 0x138 */ s32 stageFlags;
    /* 0x13C */ void *models[24];
    /* 0x19C */ struct {
        s32 key;
        s32 value;
    } animCache[32];
} Scene3D;
typedef struct {
    /* 0x00 */ s16 texWindow[4]; /* the RECT prims' SetDrawMode takes as texture window */
    /* 0x08 */ void (*frameCallbacks[16])(FrameBuffer *, s32); /* = FRAME_CALLBACKS, 0-terminated */
    /* 0x48 */ s32 displayStartCounter; /* counts up to 1 before the display is turned on; 0 holds the render loop */
    /* 0x4C */ s32 scene3dEnabled;
    /* 0x50 */ s32 vblanksPerFrame;
    s16 rotX;
    s16 rotY;
    s16 rotZ;
    /* 0x5A */ s16 unk5A; /* only jp's camera task sets it */
    /* 0x5C */ s32 originX; /* the negated look-at position */
    /* 0x60 */ s32 originY;
    /* 0x64 */ s32 originZ;
    /* 0x68 */ s32 distance; /* 20.12 fixed point */
    /* 0x6C */ s32 height;
    /* 0x70 */ s32 pitch;
    s32 snapCamera;
    u8 pad78[0x4];
    s32 posX;
    s32 posY;
    s32 posZ;
    u8 pad88[0x4];
    /* 0x8C */ s16 targetModel; /* the camera's current CameraPreset, from here to targetFacedModel */
    /* 0x8E */ s16 targetPitch;
    /* 0x90 */ s16 targetDistance;
    /* 0x92 */ s16 targetHeight;
    /* 0x94 */ s16 targetYaw;
    /* 0x96 */ s16 targetFacedModel;
    FrameBuffer buffers[2];
} Graphics;
typedef struct {
    /* 0x00 */ u16 rawHeld;
    /* 0x02 */ u16 rawPressed;
    /* 0x04 */ u16 rawReleased;
    /* 0x06 */ u16 rawRepeat;
#if VERSION_JP
    /* jp has no copies for the game to read: the game reads the raw masks */
    /* 0x08 */ s8 repeatEnabled;
    /* 0x09 */ s8 repeating;
    /* 0x0A */ s16 holdTime;
    /* 0x0C */ u16 repeatButtons;
    /* 0x0E */ s16 repeatDelay;
    /* 0x10 */ s16 repeatRate;
    /* 0x12 */ u8 padStatus;
    /* 0x13 */ u8 padType;
    /* 0x14 */ s16 padExId;
#elif VERSION_US || VERSION_EU
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
#endif
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
    /* 0x00 */ s32 openMode; /* 0: the slot is free */
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
#if VERSION_JP
/* jp's windows have no styles or labels: a line frame in three colours */
typedef struct {
    /* 0x00 */ s16 originX;
    /* 0x02 */ s16 originY;
    /* 0x04 */ Rect16 view;
    /* 0x0C */ Rect16 rect;
    /* 0x14 */ Rect16 cur;
    /* 0x1C */ Rect16 from;
    /* 0x24 */ Rect16 delta;
    /* 0x2C */ s16 scrollX; /* where the view scrolls to while animating, -1 for nowhere */
    /* 0x2E */ s16 scrollY;
    /* 0x30 */ s16 scrollDX;
    /* 0x32 */ s16 scrollDY;
    /* 0x34 */ s16 offsetX; /* the view's position plus half of the size still to grow */
    /* 0x36 */ s16 offsetY;
    /* 0x38 */ CVECTOR colors[3]; /* the fill and the frame's two lines */
    /* 0x44 */ s16 animFrames;
    /* 0x46 */ s16 animFrame;
    /* 0x48 */ u16 flags;
    /* 0x4A */ s16 brightness;
    /* 0x4C */ s16 animDone;
    /* 0x4E */ s16 z;
} UiWindow;
#elif VERSION_US || VERSION_EU
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
#else
#error "UiWindow: version not checked"
#endif
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
    s16 x1, y1;
} LINE_F2;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
} LINE_G2;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    u32 pad;
} LINE_F3;
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
    u8 r1, g1, b1, p1;
    s16 x1, y1;
    u8 r2, g2, b2, p2;
    s16 x2, y2;
} POLY_G3;
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
/* a draw mode (with a texture window) and a sprite, merged into one packet */
typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 code[2];
    /* 0x0C */ SPRT sp;
} VramSprite;
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
/* The screen copy effect: the last frame drawn again as sprites, or as warped
   quads. jp's has only the sprites: there stp is at 0x60, x at 0x78, r at 0x7C */
typedef struct {
    /* 0x000 */ ScreenSprt sprt[2][2];
#if VERSION_US || VERSION_EU
    /* 0x060 */ POLY_FT4 poly[2][2];
#endif
    /* 0x100 */ DR_STP stp[2];
    /* 0x118 */ s16 x;
    /* 0x11A */ s16 y;
#if VERSION_US || VERSION_EU
    /* 0x11C */ s16 px[2][4];
    /* 0x12C */ s16 py[2][4];
#endif
    /* 0x13C */ u8 r;
    /* 0x13D */ u8 g;
    /* 0x13E */ u8 b;
    /* 0x13F */ u8 mode;
#if VERSION_US || VERSION_EU
    /* 0x140 */ u16 abr;
#endif
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
#if VERSION_JP
    /* 0x000 */ u8 unk0[0x140]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    /* 0x000 */ u8 unk0[0x13C];
#endif
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
    /* 0x19C */ s16 innerRadius;
    /* 0x19E */ s16 outerRadius;
    /* 0x1A0 */ s16 midPercent; /* where the middle ring sits, in percent from inner to outer */
    /* 0x1A2 */ s16 innerZ;
    /* 0x1A4 */ s16 outerZ;
    /* 0x1A6 */ s16 brightness;
    /* 0x1A8 */ s16 prevBrightness;
    /* 0x1AA */ u8 axisMode;
    /* 0x1AB */ u8 cullBackface;
    /* 0x1AC */ u8 abr;
    /* 0x1AD */ s8 texAnimActive;
} RingEffect;
typedef struct {
    /* 0x00 */ u16 cards[30];
    /* 0x3C */ char name[0x13];
    /* 0x4F */ char ownerName[0x15]; /* the duelist who plays it */
    /* 0x64 */ u8 cpuStyle[4]; /* the CPU's styles, copied to Player.cpuPlaceStyle .. cpuSupportStyle */
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
#if VERSION_JP
    /* 0x001 */ char name[0xF]; /* jp: the fields below are 4 bytes lower */
#elif VERSION_US || VERSION_EU
    /* 0x001 */ char name[0x13];
#endif
    /* 0x014 */ CardSlot cards[30];
#if VERSION_JP
    /* 0x100 */ u16 wins;
    /* 0x102 */ u16 losses;
    /* 0x104 */ u16 attackCounts[3]; /* the attacks used, by button */
    /* 0x10A */ u8 unk10A[2];
#elif VERSION_US || VERSION_EU
    /* 0x104 */ s32 unk104;
    /* 0x108 */ u16 saveCount; /* times the deck was saved in this slot */
    /* 0x10A */ u16 wins;
    /* 0x10C */ u16 losses;
    /* 0x10E */ u8 unk10E[2];
#endif
} PlayerDeck;
/* the slots of a player's DP pile */
#if VERSION_JP
#define DP_SLOT_COUNT 9
#elif VERSION_US || VERSION_EU
#define DP_SLOT_COUNT 8
#endif

#if VERSION_JP
/* jp's Player has the same fields where it has them, in another layout: the
   deck is a pointer to its PlayerDeck (PLAYER_CARDS), there are no stat
   popups or armor CLUTs, and the flags take two more bits */
typedef struct {
    /* 0x00 */ PlayerDeck *deck;
    /* 0x04 */ s8 *battleCard;
    /* 0x08 */ s16 nameWidth;
    /* 0x0A */ s16 shufflePasses;
    /* 0x0C */ s16 stats[5];
    /* 0x16 */ s16 displayedStats[5];
    /* 0x20 */ s16 unk158;
    /* 0x22 */ s16 hpAfterBattle;
    /* 0x24 */ s16 baseAttackPowers[3];
    /* 0x2A */ s16 damageTaken;
    /* 0x2C */ s16 hpGain;
    /* 0x2E */ s16 reviveHp;
    /* 0x30 */ u8 unk168[6];
    /* 0x36 */ s16 attackHighlightTimer;
    /* 0x38 */ u32 usedAttack : 2;
    /* 0x38 */ u32 attackChoice : 2;
    /* 0x38 */ u32 shownAttack : 2;
    /* 0x38 */ u32 counter : 1;
    /* 0x38 */ u32 unk178_7 : 1;
    /* 0x38 */ u32 unk178_8 : 1;
    /* 0x38 */ u32 unk178_9 : 2;
    /* 0x38 */ u32 crash : 1;
    /* 0x38 */ u32 eatUpHp : 1;
    /* 0x38 */ u32 unk178_13 : 2;
    /* 0x38 */ u32 statPenalty : 2;
    /* 0x38 */ u32 controller : 2;
    /* 0x38 */ u32 wins : 2;
    /* 0x38 */ u32 specialty : 3;
    /* 0x38 */ u32 cpuPlaceStyle : 2;
    /* 0x38 */ u32 cpuAttackStyle : 2;
    /* 0x38 */ u32 cpuRedrawStyle : 2;
    /* 0x38 */ u32 cpuSupportStyle : 2;
    /* 0x3C */ u32 hasBattled : 1;
    /* 0x3C */ u32 unk3C_1 : 7;
    /* 0x3D */ s8 onlineDeck[30];
    /* 0x5B */ s8 offlineDeck[30];
    /* 0x79 */ s8 hand[4];
    /* 0x7D */ u8 topCards[5];
    /* 0x82 */ s8 dpSlots[DP_SLOT_COUNT];
    /* 0x8B */ s8 digimonStack[3];
    /* 0x8E */ s8 playedCard;
    /* 0x8F */ char name[0x11];
    /* 0xA0 */ s8 dpGainTimer; /* frames left of the "+Np" over the DP slots */
    /* 0xA1 */ u8 unkA1[3];
} Player;
#elif VERSION_US || VERSION_EU
typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ char deckName[0x13];
    /* 0x014 */ CardSlot cards[30];
    /* 0x104 */ u8 unk104[0xC];
    /* 0x110 */ s32 bonusFlags; /* one bit per end-of-duel bonus earned (kawseg) */
    /* 0x114 */ s8 *battleCard;
    /* 0x118 */ s16 nameWidth; /* of the name's glyphs on the VS screen (KAWSEG) */
    /* 0x11A */ s16 shufflePasses;
    /* 0x11C */ s16 stats[5];
    /* 0x126 */ s16 displayedStats[5];
    /* 0x130 */ Popup statPopups[5];
    /* 0x158 */ s16 unk158;
    /* 0x15A */ s16 hpAfterBattle;
    /* 0x15C */ s16 baseAttackPowers[3];
    /* 0x162 */ s16 damageTaken;
    /* 0x164 */ s16 hpGain;
    /* 0x166 */ s16 reviveHp; /* the HP support effect 48 brings the Digimon back with */
    /* 0x168 */ u8 unk168[6];
    /* 0x16E */ s16 attackHighlightTimer;
    /* 0x170 */ s16 armorCluts[4]; /* [1..3]: each partner's armor card CLUT; [0]: the base card's while armored */
    /* 0x178 */ u32 usedAttack : 2;
    /* 0x178 */ u32 attackChoice : 2;
    /* 0x178 */ u32 shownAttack : 2;
    /* 0x178 */ u32 counter : 1; /* cross effect "Counter": turns the foe's attack back */
    /* 0x178 */ u32 unk178_7 : 1;
    /* 0x178 */ u32 unk178_8 : 1;
    /* 0x178 */ u32 unk178_9 : 2;
    /* 0x178 */ u32 crash : 1; /* cross effect "Crash": X attack of its HP, HP to 10 */
    /* 0x178 */ u32 eatUpHp : 1; /* cross effect "Eat-up HP": gains the damage dealt */
    /* 0x178 */ u32 unk178_13 : 2;
    /* 0x178 */ u32 statPenalty : 2;
    /* 0x178 */ u32 controller : 2;
    /* 0x178 */ u32 specialty : 3;
    /* 0x178 */ u32 cpuPlaceStyle : 2; /* the CPU's style in KAW_chooseDigimonToPlace and KAW_chooseDpCard */
    /* 0x178 */ u32 cpuAttackStyle : 2; /* in KAW_chooseAttack */
    /* 0x178 */ u32 cpuRedrawStyle : 2; /* in KAW_decideRedraw */
    /* 0x178 */ u32 cpuSupportStyle : 2; /* in KAW_chooseSupportCard */
    /* 0x178 */ u32 hasBattled : 1; /* set at the Battle Phase, cleared by a new Digimon */
    /* 0x178 */ u32 unk178_31 : 1;
    /* 0x17C */ u8 wins;
    /* 0x17D */ s8 onlineDeck[30];
    /* 0x19B */ s8 offlineDeck[30];
    /* 0x1B9 */ s8 hand[4];
    /* the top card of the Online Deck, the Offline Deck, the active Digimon,
       the played card and the DP slots, as KAWSEG last took them */
    /* 0x1BD */ u8 topCards[5];
    /* 0x1C2 */ s8 dpSlots[DP_SLOT_COUNT];
    /* 0x1CA */ s8 digimonStack[3];
    /* 0x1CD */ s8 playedCard;
    /* 0x1CE */ char name[1];
} Player;
#endif
/* a player's 30 cards: jp's Player reaches them through its deck */
#if VERSION_JP
#define PLAYER_CARDS(p) ((p)->deck->cards)
#elif VERSION_US || VERSION_EU
#define PLAYER_CARDS(p) ((p)->cards)
#endif
/* the byte of a card's DigimonCardData at FIELD, read through an s8 or u8
   pointer to the data as KAWSEG does: the offset follows each version's
   layout */
#define CARD_BYTE(data, field) ((data)[(s32) & ((DigimonCardData *)0)->field])
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
    /* 0x02 */ s16 skills[2];
#if VERSION_JP
    /* 0x06 */ char name[0x12];
#elif VERSION_US || VERSION_EU
    /* 0x06 */ char name[0x16];
#endif
} CardAttack;
typedef struct {
    /* 0x000 */ s16 id;
    /* 0x002 */ u8 type;
#if VERSION_JP
    /* 0x003 */ char name[0x10]; /* jp: the fields below are 6 bytes lower */
#elif VERSION_US || VERSION_EU
    /* 0x003 */ char name[0x16];
#endif
    /* 0x019 */ u8 rewardRank; /* 0: never a reward; else matched against REWARD_CARD_RANGES */
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
#if VERSION_JP
    /* 0x0E7 */ u8 supportText[4][0x13];
#elif VERSION_US || VERSION_EU
    /* 0x0E7 */ u8 supportText[4][0x15];
#endif
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
    /* 0x292 */ u8 armorCardId; /* the armor selected, 0 for none */
    /* 0x293 */ u8 expBonus; /* percent more EXP after a duel */
    /* 0x294 */ u8 rewardBonus; /* percent chance of a rarer reward card */
    /* 0x295 */ u8 unk295[3];
} Partner;
#if VERSION_JP
/* seven pairs of values that are drawn again when the timer reaches the
   period (jp's profile keeps seven) */
typedef struct {
    /* 0x00 */ s8 timer;
    /* 0x01 */ s8 period;
    /* 0x02 */ s8 unk2;
    /* 0x03 */ s8 unk3;
    /* 0x04 */ s16 values[7][2];
} RandomTicker;
#endif
typedef struct {
#if VERSION_JP
    /* jp's profile is laid out otherwise: only the fields its C reads so far */
    /* 0x0000 */ char name[0x10];
    /* 0x0010 */ u32 monoSound : 1; /* the option screen's sound setting */
    /* 0x0010 */ u32 skipBattleAnimation : 1; /* its polygon battle setting */
    /* 0x0010 */ u32 unk10_2 : 30;
    /* 0x0014 */ s32 playTime;
    /* 0x0018 */ u16 battleWins;
    /* 0x001A */ u16 battleLosses;
    /* 0x001C */ u16 versusWins;
    /* 0x001E */ u16 versusLosses;
    /* 0x0020 */ s16 profileId; /* random */
    /* 0x0022 */ u8 area; /* where the player is: SAISEG's map is area + 1 */
    /* 0x0023 */ u8 saveCount; /* "ＳＡＶＥ回数" */
    /* 0x0024 */ s32 bits; /* the money, "所持金" */
    /* 0x0028 */ u32 tamerRank : 3;
    /* 0x0028 */ u32 collectorRank : 3;
    /* 0x0028 */ u32 battleRank : 3;
    /* 0x0029 */ u32 unk28_9 : 1;
    /* 0x0029 */ u32 tradeUnlocked : 1;
    /* 0x0029 */ u32 hasTraded : 1;
    /* 0x0029 */ u32 unk28_12 : 1; /* the game was beaten: the ending's records can be browsed, and the player data screen scrolls */
    /* 0x0029 */ u32 unk28_13 : 1; /* the player gets 2000 Bits and two cards at the start */
    /* 0x002A */ u16 attackCounts[3]; /* the attacks used, by button */
    /* 0x0030 */ s16 maxAttackPowers[0x6E][3]; /* per Digimon card */
    /* 0x02C4 */ u16 cardWins[0x6E]; /* per Digimon card */
    /* 0x03A0 */ u16 cardLosses[0x6E];
    /* bits 0-3: copies owned of each Digimon card, of each option card, and
       of the six Digivolve cards */
    /* 0x047C */ u8 cardCollection[0x6E];
    /* 0x04EA */ u8 optionCollection[0x2B];
    /* 0x0515 */ u8 digivolveCollection[6];
    /* 0x051B */ u8 starterCardCount;
    /* 0x051C */ u8 starterCards[0x10]; /* given when the game starts, as card ids */
    /* each copy's serial number, as in us's cardCopySerials */
    /* 0x052C */ u16 cardSerials[0x6E][8];
    /* 0x0C0C */ u16 optionSerials[0x2B][8];
    /* 0x0EBC */ u16 digivolveSerials[6][8];
    /* 0x0F1C */ u16 unkF1C; /* bit n: SAISEG script register 0x13 + n */
    /* 0x0F1E */ u16 unkF1E;
    /* 0x0F20 */ u32 eventFlags[10]; /* bit n: event 0x22 + n has happened */
    /* 0x0F48 */ PlayerDeck savedDecks[3];
    /* 0x126C */ PlayerDeck hallOfFameDeck; /* the deck the player beat the game with, which the ending's records show */
    /* 0x1378 */ s32 clearTime; /* playTime when the game was beaten */
    /* 0x137C */ RandomTicker tickers[7];
#elif VERSION_US || VERSION_EU
    /* 0x0000 */ char name[0xD];
    /* 0x000D */ u8 saveCount; /* "Number of Saves", stops at 255 */
    /* 0x000E */ u8 areaId; /* the SAISEG area the player is in */
    /* 0x000F */ u8 resumeInArea; /* 0: the game goes on from the world map, else from areaId */
    /* 0x0010 */ s16 profileId; /* random; two profiles with the same id are the same save */
    /* 0x0012 */ u16 seenCardCount;
    /* 0x0014 */ s16 completionPoints; /* added by SAISEG scripts; "Game Completion" is this out of 166 */
    /* 0x0016 */ s16 profileSize;
    /* 0x0018 */ u16 battleWins;
    /* 0x001A */ u16 battleLosses;
    /* 0x001C */ u16 versusWins;
    /* 0x001E */ u16 versusLosses;
    /* 0x0020 */ u32 monoSound : 1; /* "Sound Settings": 0 stereo, 1 mono */
    /* 0x0020 */ u32 unk20_1 : 1;
    /* 0x0020 */ u32 unk20_2 : 1;
    /* 0x0020 */ u32 skipBattleAnimation : 1;
    /* 0x0020 */ u32 unk20_4 : 28;
    /* 0x0024 */ s32 playTime;
    /* 0x0028 */ u32 tamerRank : 3;
    /* 0x0028 */ u32 collectorRank : 3;
    /* 0x0028 */ u32 battleRank : 3;
    /* 0x0029 */ u32 unk28_9 : 1;
    /* 0x0029 */ u32 tradeUnlocked : 1; /* set by a SAISEG script; opens card trades */
    /* 0x0029 */ u32 hasTraded : 1; /* keeps the collector rank below its top title */
    /* 0x0029 */ u32 unk28_12 : 1;
    /* 0x0029 */ u32 unk28_13 : 1;
    /* 0x0029 */ u32 unk28_14 : 18;
    /* 0x002C */ s32 scriptFlags; /* bit n: EVOSEG script variable 20 + n */
    /* 0x0030 */ s32 scriptOffset; /* where SAISEG's area script resumes */
    /* 0x0034 */ s8 deckChoice; /* the saved deck the player duels with, -1 for none */
    /* 0x0035 */ u8 unk35;
    /* 0x0036 */ u16 attackCounts[3];
    /* 0x003C */ u8 ownedAbilities[0x10];
    /* 0x004C */ s16 cardsReceived;   /* by trade; this and the next four stop at 9999 */
    /* 0x004E */ s16 cardsGivenAway;  /* by trade */
    /* 0x0050 */ s16 fusedCards;      /* cards made by fusion */
    /* 0x0052 */ s16 fusionCardsUsed; /* cards used up in fusions */
    /* 0x0054 */ s16 fusionMutations;
    /* 0x0056 */ u16 activePartner; /* the partner chosen last (first: the starter deck's); picks the background */
    /* 0x0058 */ u8 unk58[0x28];
    /* 0x0080 */ Partner partners[3];
    /* 0x0848 */ s16 bonusCounts[0x20]; /* times each end-of-duel bonus was earned */
    /* 0x0888 */ u16 comWins[0x8E]; /* "Wins & Losses per Com" */
    /* 0x09A4 */ u16 comLosses[0x8E];
    /* 0x0AC0 */ u16 opponentDeckFlags[0x9F];
    /* 0x0BFE */ u16 opponentDeckLosses[0x9F]; /* the wins are in opponentDeckFlags */
    /* 0x0D3C */ s16 maxAttackPowers[0xBF][3]; /* per Digimon card: "Max Attack Power" */
    /* 0x11B6 */ u16 cardWins[0xBF]; /* per Digimon card */
    /* 0x1334 */ u16 cardLosses[0xBF];
    /* per card id: bits 0-2 copies owned (up to 6), 0x10 no more to win,
       0x20 first copy just obtained, 0x40 seen, 0x80 new */
    /* 0x14B2 */ u8 cardCollection[0x12D];
    /* 0x15DF */ u8 unk15DF;
    /* 0x15E0 */ u16 cardCopySerials[301][6];
    /* 0x23FC */ s32 areaScriptFlags[12]; /* bit n: SAISEG script register 12 + n */
    /* 0x242C */ u8 areaScriptValues[9]; /* SAISEG script registers 0x16B.. */
    /* 0x2435 */ u8 unk2435[3];
    /* 0x2438 */ PlayerDeck savedDecks[3];
    /* 0x2768 */ s16 rewardCards[3];
    /* 0x276E */ s8 rewardResults[3];
    /* 0x2771 */ u8 unk2771[3];
#endif
} PlayerProfile;
#if VERSION_JP
/* jp's area state, 0x54 bytes: only the fields its C reads so far */
typedef struct {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s32 *flags; /* 0x11E words */
    /* 0x10 */ void *pak; /* the area's model PAK (loadAreaPakTask) */
    /* 0x14 */ u8 unk14[0x2E];
    /* 0x42 */ u8 area;
    /* 0x43 */ s8 pakState; /* 1: loading, 2: loaded */
    /* 0x44 */ s8 unk44;
    /* 0x45 */ u8 unk45[0xA];
    /* 0x4F */ u8 unk4F;
    /* 0x50 */ u8 unk50[4];
} AreaSession;
#elif VERSION_US || VERSION_EU
/* Where SAISEG keeps its area state while other overlays run (SAISEG's
   SaisegSession sees the whole block) */
typedef struct {
    /* 0x000 */ u8 unk0[0x1A2];
    /* 0x1A2 */ s16 unk1A2;
    /* 0x1A4 */ u8 area;
    /* 0x1A5 */ u8 unk1A5;
    /* 0x1A6 */ u8 duelResult; /* 0: the player won */
    /* 0x1A7 */ u8 unk1A7;
    /* 0x1A8 */ u8 location;
    /* 0x1A9 */ u8 resumeMode; /* 1: back from a duel, 2: back from the complete stats */
} AreaSession;
#else
#error "AreaSession: version not checked"
#endif
#if VERSION_JP
/* what jp's window task (runWindowTask) opens: a window that grows from one
   rect to the other, its contents drawn by draw, with two sprites that slide
   in to its corner */
typedef struct {
    /* 0x00 */ Rect16 from;
    /* 0x08 */ Rect16 to;
    /* 0x10 */ s32 frames; /* the window's opening time, and the sprites' speed */
    /* 0x14 */ s32 side; /* the sprites': 0 left, 1 right */
    /* 0x18 */ void (*draw)(void *window);
    /* 0x1C */ void (*update)(s8 *state); /* each frame: 3 ends the task, 4 closes the window first */
} WindowSpec;
/* a sprite of a texture page: drawTexturedSprite's arguments */
typedef struct {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ u16 tpage;
    /* 0x06 */ u16 clut;
    /* 0x08 */ Rect16 uv;
    /* 0x10 */ s32 z;
} TexSprite;
/* jp's menu of choices, a column of icons with their labels: openChoiceMenu
   opens it, addChoiceMenuItem adds a choice, runChoiceMenu runs a frame */
typedef struct {
    /* 0x000 */ TexSprite cursor;
    /* 0x014 */ TexSprite bar; /* where the choice's label shows */
    /* 0x028 */ TexSprite items[20]; /* each choice's label, then its icon */
    /* 0x1B8 */ POLY_FT4 bars[2]; /* per frame buffer */
    /* 0x208 */ void (*actions[11])(); /* each choice's task, then the cancel's */
    /* 0x234 */ s32 barWidth;
    /* 0x238 */ s32 barFullWidth;
    /* 0x23C */ s32 choice;
    /* 0x240 */ s32 count;
    /* 0x244 */ s32 *arg; /* what the chosen task gets */
    /* 0x248 */ s8 result; /* negative: cancelled */
    /* 0x249 */ s8 timer;
    /* 0x24A */ s8 state; /* MENU_STATE_HANDLERS */
    /* 0x24B */ u8 blink;
    /* 0x24C */ u8 unk24C[4];
} ChoiceMenu;
/* jp's session block is 0x154 bytes, laid out differently: only the fields
   its matched code reads are placed */
typedef struct {
    /* 0x000 */ u8 *prims; /* 0x820 bytes, half for each frame buffer */
    /* 0x004 */ struct Panel *panels; /* the duel's HUD panels (battle_hud.h) */
    /* 0x008 */ void *unk8; /* 0x36C bytes, KAWSEG's (func_801FEC84) */
    /* 0x00C */ u8 unkC[4];
    /* 0x010 */ PlayerDeck opponentDeck; /* the CPU's, for the next duel */
    /* 0x11C */ s8 cpuStyle[4]; /* copied to Player.cpuPlaceStyle .. cpuSupportStyle */
    /* 0x120 */ s8 deckChoice; /* the saved deck the player takes to a duel against the CPU */
    /* 0x121 */ s8 opponentDeckIndex; /* the deck the CPU duels with */
    /* 0x122 */ s8 tutorial; /* the next duel against the CPU is the tutorial */
    /* 0x123 */ char opponentName[0x12];
    /* 0x135 */ s8 stageMusic; /* runDuelStageTask's arguments, for the duel against the CPU */
    /* 0x136 */ s8 stageId;
    /* 0x137 */ u8 unk137;
    /* 0x138 */ AreaSession *areaSession;
    /* 0x13C */ u8 unk13C[0x10];
    /* 0x14C */ u8 versusWins[2]; /* this session's, per player */
    /* 0x14E */ u8 deckChoices[2]; /* each player's saved deck in a versus duel */
    /* 0x150 */ u8 otherPad; /* 1 in VS mode: the second controller works too */
    /* 0x151 */ u8 unk151[3];
} SessionData;
#elif VERSION_US || VERSION_EU
typedef struct {
    /* 0x0000 */ u8 *npcDeckFile;
    /* 0x0004 */ u8 opponentDeckIndex;
    /* 0x0005 */ u8 unk5[3];
    /* 0x0008 */ PresetDeck opponentDeck;
    /* 0x0076 */ u8 unk76[2];
    /* 0x0078 */ Partner partnerBackup[2][3];
    /* 0x1008 */ s16 npcDeckIndex[2];
    /* 0x100C */ AreaSession *areaSession;
    /* 0x1010 */ u8 saveSlots[2][8]; /* per memory card, [0]: the save slot (OPENSEG's SaveInfo) */
    /* 0x1020 */ u8 unk1020[2];
    /* 0x1022 */ u8 deckRuleActive; /* a SAISEG script limits the decks the next duel takes */
    /* 0x1023 */ u8 deckAllowed[3]; /* per saved deck, whether that rule lets it in */
    /* 0x1026 */ u8 unk1026;
    /* 0x1027 */ u8 playWithoutSaving; /* chosen at OPENSEG's memory card prompt; greys out saving */
    /* 0x1028 */ s8 menuRow; /* the row picked in an OPENSEG menu */
    /* 0x1029 */ u8 unk1029[3];
} SessionData;
#endif
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
#if VERSION_US || VERSION_EU
    /* 0x26F4 */ void *pak;
    /* 0x26F8 */ u8 clut[0x200];
#endif
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
#if VERSION_JP
/* a mark jp's duel draws at the screen's side */
typedef struct {
    /* 0x0 */ s8 owner;
    /* 0x1 */ u8 shown;
    /* 0x2 */ s16 x;
    /* 0x4 */ s16 y;
} HudMark;
/* jp's duel state is laid out differently (cpuResult is 0x3DC bytes earlier):
   only the fields its matched code reads are placed */
typedef struct {
    /* 0x000 */ u8 *cursor;
    /* 0x004 */ u8 unk4[0x400];
    /* 0x404 */ CardCache cache[6];
    /* 0x41C */ struct CardSprite *sprites; /* the sprite of each of the 60 cards */
    /* 0x420 */ s32 cpuWaitFrames;
    /* 0x424 */ s32 unk424;
    /* 0x428 */ s32 cpuResult;
    /* hand slots the cards played this turn came from (us's Duel) */
    /* 0x42C */ s16 playedFromSlot;
    /* 0x42E */ s16 discardedFromSlot;
    /* 0x430 */ s16 dpFromSlot;
    /* 0x432 */ s8 state;
    /* 0x433 */ s8 loadBusy;
    /* 0x434 */ s8 stopArtLoader;
    /* 0x435 */ s8 stopStageTask;
    /* 0x436 */ s8 stopCpuTask;
    /* 0x437 */ s8 stopTurnLoop;
    /* 0x438 */ s8 cpuRequest;
    /* 0x439 */ s8 turnPlayer;
    /* 0x43A */ s8 step;
    /* 0x43B */ s8 returnStep;
    /* 0x43C */ s8 viewPlayer;
    /* 0x43D */ s8 messageWanted; /* the message bar's message (duel_session's DUEL_MESSAGES) */
    /* 0x43E */ s8 messageShown;
    /* 0x43F */ u8 cursorPlayer;
    /* 0x440 */ s8 cursorSlot;
    /* 0x441 */ s8 cursorMode;
    /* 0x442 */ u8 winner;
    /* 0x443 */ s8 tutorial;
    /* the tutorial's message window (KAWSEG's func_801FF2C0) */
    /* 0x444 */ s8 tutorialClosing;
    /* 0x445 */ s8 tutorialMessage; /* the message to show */
    /* 0x446 */ s8 tutorialShown;
    /* 0x447 */ s8 tutorialVisible;
    /* 0x448 */ s8 tutorialOpen;
    /* 0x449 */ u8 unk449;
    /* 0x44A */ s8 unk44A;
    /* 0x44B */ u8 unk44B;
    /* 0x44C */ HudMark marks[7];
    /* 0x476 */ u8 showMarks; /* L1 or R1 toggles it */
    /* 0x477 */ s8 menuPlayer; /* who opened the Give Up prompt or the help */
    /* 0x478 */ s8 quit; /* 1: the Give Up prompt is open; 2 + the winner once given up */
    /* 0x479 */ s8 helpOpen; /* the duel waits (waitDuelFrames) while this or quit is set */
    /* 0x47A */ s8 helpPage;
    /* the cards the support panels show (drawHudPanelContents): the last
       Digimon and the last Option or Digivolve card under the cursor */
    /* 0x47B */ u8 shownDigimon;
    /* 0x47C */ u8 shownOption;
    /* 0x47D */ u8 artSlot;
    /* 0x47E */ u8 cpuPlayer;
    /* 0x47F */ s8 unk47F;
    /* 0x480 */ s8 humanPlayer; /* who plays at the controller: opens the prompts, sits at the bottom (2: nobody) */
    /* 0x481 */ s8 unk481;
    /* 0x482 */ s8 unk482;
    /* 0x483 */ u8 unk483;
} Duel;
#elif VERSION_US || VERSION_EU
typedef struct {
    /* 0x000 */ u8 unk0[0x50];
    /* 0x050 */ Player *firstAttacker;
    /* 0x054 */ Player *secondAttacker;
    /* 0x058 */ u8 *cursor;
    /* 0x05C */ u8 unk5C[0x784];
    /* 0x7E0 */ CardCache cache[6];
    /* 0x7F8 */ struct CardSprite *sprites; /* the sprite of each of the 60 cards */
    /* 0x7FC */ s32 cpuWaitFrames;
    /* 0x800 */ s32 unk800;
    /* 0x804 */ s32 cpuResult;
    /* 0x808 */ s16 fade;
    /* hand slots the cards played this turn came from, to put them back on
       undo (-1 none; playedFromSlot 4 = drawn from the Online Deck) */
    /* 0x80A */ s16 playedFromSlot;
    /* 0x80C */ s16 discardedFromSlot;
    /* 0x80E */ s16 dpFromSlot;
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
    /* 0x81D */ s8 cursorMode; /* which cards the card cursor offers, -1: none */
    /* 0x81E */ u8 winner;
    /* 0x81F */ s8 tutorial;
    /* 0x820 */ s8 tutorialBusy; /* set while the tutorial script runs */
    /* 0x821 */ s8 menuPlayer;
    /* 0x822 */ s8 awaitingInput;
    /* 0x823 */ s8 menuOpen; /* waitDuelFrames doesn't count the frames while it is set */
    /* 0x824 */ s8 quit; /* 2 + the winner when the duel ends early (tutorial, Give Up) */
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
#endif
typedef struct {
    s8 bg;
    u8 texAnimFrames;
    u8 texAnimDelay;
    s8 flags;
    /* jp's stages have no clear colour or fade level */
#if VERSION_US || VERSION_EU
    u8 rgb[3];
    u8 fadeLevel; /* the level SUGSEG's attack scenes fade the stage to */
#endif
} ArenaStage;
typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 id;
    /* 0x4 */ struct CardSprite *sprite;
} CardCursor;
typedef struct {
    /* 0x00 */ u8 unk0[0xA5];
    /* 0xA5 */ s8 choice;
} Window;
#if VERSION_JP
/* jp's card sprite has no fade-in colours, number or depth */
typedef struct CardSprite {
    /* 0x00 */ u8 rgbc[4];
    /* 0x04 */ u8 fade[4];
    /* 0x08 */ u16 clut;
    /* 0x0A */ u16 tpage;
    /* 0x0C */ u8 pal;
    /* 0x0D */ u8 flags; /* 0x80: drawn */
    /* 0x0E */ u8 u;
    /* 0x0F */ u8 v;
    /* 0x10 */ VECTOR pos;
    /* 0x20 */ SVECTOR rot;
    /* 0x28 */ s32 scale;
    /* 0x2C */ s16 sx;
    /* 0x2E */ s16 sy;
} CardSprite;
#elif VERSION_US || VERSION_EU
typedef struct CardSprite {
    /* 0x00 */ u8 rgbc[4];
    /* 0x04 */ u8 fade[4];
    /* 0x08 */ u8 from[3];
    /* 0x0B */ u8 t;
    /* 0x0C */ u8 to[3];
    /* 0x0F */ u8 num;
    /* 0x10 */ u16 clut;
    /* 0x12 */ u16 tpage;
    /* 0x14 */ u8 pal;
    /* 0x15 */ u8 flags; /* 0x80: drawn, 0x40: fading from -> to, 0x20: shows num */
    /* 0x16 */ u8 u;
    /* 0x17 */ u8 v;
    /* 0x18 */ VECTOR pos;
    /* 0x28 */ SVECTOR rot;
    /* 0x30 */ s32 scale;
    /* 0x34 */ s16 sx;
    /* 0x36 */ s16 sy;
    /* 0x38 */ s32 z;
} CardSprite;
#endif
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
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 param;
    /* 0x2 */ u8 actionStart;
    /* 0x3 */ u8 actionCount;
    /* 0x4 */ s16 value;
    /* 0x6 */ s16 supportIcon;
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
extern s32 FRAME_CALLBACKS;
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
extern void *SESSION_DATA;
extern void *DUEL_STATE;
extern u8 *DUEL_PLAYERS[];
extern ScrollBackground SCROLL_BACKGROUND;
extern s32 ATTACK_ICON_TIMER;
extern s32 DUEL_DIALOG;
extern u8 *HUD_PANELS;
extern u8 *CARD_ANIMS;
extern MsgBar DUEL_MSG_BAR;
extern u8 MSG_BAR_PLAYER_LABEL;
void runSceneCameraTask(s32 preset);

s32 VSync(s32);
void SetSemiTrans(void *, s32);
void SetShadeTex(void *, s32);
s32 rand(void);
s32 sprintf(char *, const char *, ...);
s32 DrawSync(s32);
s32 LoadImage(s16 *, s32);
s32 exitTask();
void ResetCallback(void);
void SetDispMask(s32);
void GsInitGraph(u16, u16, u16, u16, u16);
s32 ClearImage(Rect16 *, s32, s32, s32);
void MoveImage(Rect16 *rect, s32 x, s32 y);
void SsInit(void);
void launchTaskScheduler(s32, s32, void (*)(), s32, s32, s32, s32);
s32 EnterCriticalSection();
s32 ExitCriticalSection();
long OpenEvent(unsigned long, long, long, long (*)());
long EnableEvent(long);
s32 SetRCnt(u32, u16, s32);
s32 StartRCnt(u32);
extern int endTask(int);
void SetPolyFT4(POLY_FT4 *);
void SetDrawStp(DR_STP *, s32);
u16 GetTPage(s32, s32, s32, s32);
void SetGraphDebug(s32);
void InitGeom(void);
s32 waitFrames(s32);
void yieldTask(void);
void ClearOTagR(u32 *, s32);
void GsSwapDispBuff(void);
void PutDispEnv(DISPENV *);
void PutDrawEnv(DRAWENV *);
void DrawOTag(u32 *);
void ChangeClearPad(s32);
s32 spawnTask();
s32 CdInit(void);
s32 CdControlB(u8, u8 *, u8 *);
void CdSetDebug(s32);
s32 CdIntToPos(s32, u8 *);
s32 CdRead(s32, u8 *, s32);
s32 CdReadSync(s32, u8 *);
int toupper(int);
s32 CdSearchFile(void *, char *);
s32 CdPosToInt(void *);
s32 CdSync(s32, s32);
extern int CdSync(int, int);
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
void disableInterrupts(void);
void restoreInterrupts(void);
long catan(long);
s32 resumeTask();
s32 OpenTIM(u32 *);
TIM_IMAGE *ReadTIM(TIM_IMAGE *);
void SetDefDrawEnv(DRAWENV *, s32, s32, s32, s32);
void SetDefDispEnv(DISPENV *, s32, s32, s32, s32);
s32 MargePrim(void *, void *);
s32 SetDrawMode(void *, s32, s32, s32, s32 *);
void SetPolyFT3(POLY_FT3 *);
void SetPolyGT3(POLY_GT3 *);
void SetPolyGT4(POLY_GT4 *);
void SetPolyF4(void *);
void SetPolyG4(POLY_G4 *);
void SetPolyG3(void *);
void SetPolyF3(void *);
void SetLineG2(void *);
void SetLineF2(void *);
void SetLineF3(void *);
void SetLineG3(void *);
void SetLineF4(void *);
void SetLineG4(void *);
void SetSprt8(void *);
void SetSprt16(void *);
void SetSprt(SPRT *);
void SetTile1(void *);
void SetTile8(void *);
void SetTile16(void *);
void SetTile(TILE *);
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
s32 SetTransMatrix();
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
void SetGeomOffset(s32, s32);
void SetGeomScreen(s32);
void GsSetProjection(s32);
s32 GsSetRefView2(GsRVIEW2 *);
s32 GsSetAmbient(s32, s32, s32);
s32 GsSetLightMode(s32);
s32 SetBackColor(s32, s32, s32);
void GsInit3D(void);
s32 SsSetMVol(s32, s32);
s32 SsSetTableSize(s32 *, s32, s32);
s32 SsSetTickMode(s32);
s32 SsStart();
s32 SsSetStereo();
void SsVabClose(s16);
void bcopy(void *, void *, s32);
s16 SsSeqOpen(u8 *, s16);
void SsSeqClose(s16);
s32 SpuClearReverbWorkArea(s32);
s32 SsUtSetReverbDepth(s32, s32);
s32 SsUtSetReverbType(s16);
s32 SsUtReverbOff();
s32 SsUtReverbOn();
void SpuSetVoiceAttr(SpuVoiceAttr *);
s16 SsVabOpenHeadSticky(u8 *, s16, s32);
s16 SsVabTransBody(s32, s16);
s32 SsVabTransCompleted(s32);
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
void _bu_init(void);
s32 TestEvent(s32);
s32 _card_clear(s32);
s32 _card_info(s32);
s32 _card_load(s32);
s32 _card_format(s32);
s32 format(char *name);
s32 open(char *, s32);
s32 close(s32);
s32 lseek(s32, s32, s32);
s32 write(s32, void *, s32);
s32 read(s32, void *, s32);
DirEntry *firstfile(char *, DirEntry *);
DirEntry *nextfile(DirEntry *);
char *strcpy(char *, const char *);
s32 rsin(s32);
s32 rcos(s32);
long SquareRoot0(long);
void VectorNormal(VECTOR *, VECTOR *);
int abs(int);
s32 endTask(s32);
MATRIX *CompMatrix(MATRIX *, MATRIX *, MATRIX *);
s32 RotAverage4(SVECTOR *, SVECTOR *, SVECTOR *, SVECTOR *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
MATRIX *MulMatrix(MATRIX *, MATRIX *);
MATRIX *MatrixNormal(MATRIX *, MATRIX *);
MATRIX *TransposeMatrix(MATRIX *, MATRIX *);

#endif /* GAME_H */
