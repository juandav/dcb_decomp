#ifndef DCB_SCROLL_BG_H
#define DCB_SCROLL_BG_H

#include "game.h"
#include "dcb/stage.h"

void resetScrollingBackground(void);
void fadeOutScrollingBackground(void);
void changeScrollingBackground(s32 image, s32 x, s32 y, s32 w, s32 h);
void loadScrollingBackground(void);
void hideScrollingBackground(void);
void setBackgroundScrollMode(s8 scrollMode);
void freeScrollingBackground(void);

typedef struct {
    /* 0x000 */ u8 unk0[0x10C];
    /* 0x10C */ s32 unk10C[3];
} Unk80030CA8;
typedef struct {
    s32 data[0x4F];
} Unk13C;
typedef struct {
    /* 0x000 */ u8 unk0[0x20];
    /* 0x020 */ s32 posX;
    /* 0x024 */ s32 posY;
    /* 0x028 */ s32 posZ;
    /* 0x02C */ u8 unk2C[4];
    /* 0x030 */ s16 rotX;
    /* 0x032 */ s16 rotY;
    /* 0x034 */ s16 rotZ;
    /* 0x036 */ u8 unk36[2];
    /* 0x038 */ s32 sx;
    /* 0x03C */ s32 sy;
    /* 0x040 */ s32 sz;
    /* 0x044 */ s32 sw;
    /* 0x048 */ u8 unk48[0x64];
    /* 0x0AC */ s32 sx0;
    /* 0x0B0 */ s32 sy0;
    /* 0x0B4 */ s32 sz0;
    /* 0x0B8 */ s32 unkB8;
    /* 0x0BC */ s32 sxT;
    /* 0x0C0 */ s32 syT;
    /* 0x0C4 */ s32 szT;
    /* 0x0C8 */ s32 swT;
    /* 0x0CC */ s16 dsx;
    /* 0x0CE */ s16 dsy;
    /* 0x0D0 */ s16 dsz;
    /* 0x0D2 */ s16 unkD2;
    /* 0x0D4 */ s16 px;
    /* 0x0D6 */ s16 py;
    /* 0x0D8 */ s16 pz;
    /* 0x0DA */ s16 unkDA;
    /* 0x0DC */ s16 px2;
    /* 0x0DE */ s16 py2;
    /* 0x0E0 */ s16 pz2;
    /* 0x0E2 */ s16 unkE2;
    /* 0x0E4 */ u16 rx0;
    /* 0x0E6 */ u16 ry0;
    /* 0x0E8 */ u16 rz0;
    /* 0x0EA */ s16 unkEA;
    /* 0x0EC */ s16 drx;
    /* 0x0EE */ s16 dry;
    /* 0x0F0 */ s16 drz;
    /* 0x0F2 */ s16 unkF2;
    /* 0x0F4 */ s16 ddrx;
    /* 0x0F6 */ s16 ddry;
    /* 0x0F8 */ s16 ddrz;
    /* 0x0FA */ u8 unkFA[6];
    /* 0x100 */ s32 t;
    /* 0x104 */ s32 t2;
    /* 0x108 */ s32 cnt;
    /* 0x10C */ s32 doneX;
    /* 0x110 */ s32 doneY;
    /* 0x114 */ s32 doneZ;
    /* 0x118 */ s32 state;
    /* 0x11C */ s32 flag;
    /* 0x120 */ u8 unk120[4];
    /* 0x124 */ s16 period;
    /* 0x126 */ u8 unk126[6];
    /* 0x12C */ s16 unk12C;
    /* 0x12E */ s16 mode;
    /* 0x130 */ s16 speed;
    /* 0x132 */ u8 unk132[5];
    /* 0x137 */ u8 unk137;
    /* 0x138 */ u8 unk138;
    /* 0x139 */ u8 unk139;
} Anim;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    u8 r1, g1, b1, p1;
    s16 x1, y1;
} LINE_G2;
typedef struct {
    /* 0x00 */ u8 unk0[0x30];
    /* 0x30 */ s16 unk30;
    /* 0x32 */ s16 unk32;
    /* 0x34 */ s16 unk34;
    /* 0x36 */ u8 unk36[0x16];
    /* 0x4C */ LINE_G2 line[2];
    /* 0x74 */ s16 unk74;
    /* 0x76 */ s16 unk76;
    /* 0x78 */ s16 unk78;
    /* 0x7A */ s16 unk7A;
    /* 0x7C */ u16 unk7C;
    /* 0x7E */ u16 unk7E;
    /* 0x80 */ u16 unk80;
    /* 0x82 */ s16 unk82;
    /* 0x84 */ s16 unk84;
    /* 0x86 */ s16 unk86;
} Particle;
typedef struct {
    /* 0x000 */ Unk13C base;
    /* 0x13C */ void *parent;
    /* 0x140 */ Particle *p;
    /* 0x144 */ u8 rgb[3];
    /* 0x147 */ u8 unk147;
    /* 0x148 */ u8 drgb[3];
    /* 0x14B */ u8 unk14B;
    /* 0x14C */ u16 frames;
    /* 0x14E */ s16 unk14E;
    /* 0x150 */ u16 unk150;
    /* 0x152 */ u16 count;
    /* 0x154 */ s16 unk154;
    /* 0x156 */ s16 unk156;
    /* 0x158 */ s8 unk158;
    /* 0x159 */ s8 own;
    /* 0x15A */ u8 unk15A;
    /* 0x15B */ s8 kind;
} Particles;

extern u8 PRIM_SIZES[];
extern s16 ATTACK_ICON_ORIGIN_X[3];
extern s16 ATTACK_ICON_ORIGIN_Y[2][3];

void renderScrollingBackground(void);
Obj32 *createRingEffect(s16 brightness, Bytes4 *innerColor, Bytes4 *midColor, Bytes4 *outerColor, Unk13C *template, s32 segments, u8 abr, u8 texDepth, s32 primType,
                     s16 innerRadius, s16 outerRadius, s16 midPercent, s16 innerZ, s16 outerZ, Bytes8 *texCoords, s32 tpage, s32 clut, s32 texAnimId, u8 u1, u8 u2,
                     s32 w, s32 x);
Particles *createStreakParticles(u8 *startColor, u8 *endColor, Unk13C *template, s16 spreadX, s16 spreadY, s16 length, s16 endLength, s16 frames, s16 speedRange, s16 reverse,
                         s16 count, s16 zOffset, s16 spin, s16 pattern, s16 kind, s16 semi, s32 flags, s32 fixedOtz);

#endif /* DCB_SCROLL_BG_H */
