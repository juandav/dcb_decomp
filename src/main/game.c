#include "common.h"
#include "gte.h"

typedef struct {
    /* 0x00 */ char unk0[0x14];
    /* 0x14 */ int unk14;
} Unk80077A0C;

typedef struct {
    /* 0x0000 */ int unk0;
    /* 0x0004 */ char unk4[0x102C];
} Unk80081710;

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
    /* 0x4070 */ void *unk4070;
    /* 0x4074 */ s32 unk4074;
    /* 0x4078 */ s32 unk4078[16];
    /* 0x40B8 */ s32 unk40B8;
    /* 0x40BC */ s32 unk40BC;
} Unk800793A0;

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
    /* 0x000 */ GsOT ot[2];
    /* 0x028 */ u8 unk28[0x9C];
    /* 0x0C4 */ GsRVIEW2 unkC4;
    /* 0x0E4 */ u8 unkE4[0x30];
    /* 0x114 */ s8 unk114[0x28];
    /* 0x13C */ void *unk13C[24];
    /* 0x19C */ struct {
        s32 key;
        s32 value;
    } unk19C[32];
} Unk801D6A4C;

typedef struct {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ void (*unk8[16])(Unk800793A0 *, s32);
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
    /* 0x50 */ s32 unk50;
    s16 unk54;
    s16 unk56;
    s16 unk58;
    u8 pad5A[0x1A];
    s32 unk74;
    u8 pad78[0x4];
    s32 unk7C;
    s32 unk80;
    s32 unk84;
    u8 pad88[0x4];
    s16 unk8C;
    s16 unk8E;
    s16 unk90;
    s16 unk92;
    s16 unk94;
    u8 pad96[0x2];
    Unk800793A0 unk98[2];
} Unk800794F8;

typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s8 unk10;
    /* 0x11 */ s8 unk11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u16 unk14;
    /* 0x16 */ s16 repeatDelay;
    /* 0x18 */ s16 repeatRate;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ s16 unk1C;
} PadState;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} FadePoly;

typedef struct Thread {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct Thread *next;
    /* 0x0C */ struct Thread *prev;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 stack;
    /* 0x20 */ s32 regs[40];
} Thread;

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
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u32 tpage[2];
    /* 0x1C */ u32 twin[3];
    /* 0x28 */ u32 twin0[3];
} FadeBuf;

typedef struct {
    /* 0x00 */ FadeBuf buf[2];
    /* 0x68 */ s32 tim;
    /* 0x6C */ s8 mode;
    /* 0x6D */ s8 unk6D;
    /* 0x6E */ u8 unk6E;
    /* 0x6F */ u8 unk6F;
    /* 0x70 */ s16 unk70;
    /* 0x72 */ s16 unk72;
    /* 0x74 */ s16 w;
    /* 0x76 */ s16 h;
    /* 0x78 */ s16 x;
    /* 0x7A */ s16 y;
    /* 0x7C */ u16 unk7C;
    /* 0x7E */ u16 unk7E;
} Fade;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ u8 loc[4];
    /* 0x08 */ u8 unk8[0x14];
    /* 0x1C */ s32 sector;
    /* 0x20 */ s32 remaining;
    /* 0x24 */ s32 size;
    /* 0x28 */ s32 avail;
    /* 0x2C */ u8 *cur;
    /* 0x30 */ u8 buf[0x1000];
} CdFile;

typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ s8 unk4[6];
    /* 0xA */ u8 unkA[2];
} Entry12;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ Rect16 view;
    /* 0x0C */ Rect16 rect;
    /* 0x14 */ Rect16 cur;
    /* 0x1C */ Rect16 from;
    /* 0x24 */ Rect16 delta;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s16 unk30[4];
    /* 0x38 */ u8 unk38;
    /* 0x39 */ u8 unk39;
    /* 0x3A */ s16 z;
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 unk3E;
    /* 0x3F */ u8 unk3F;
    /* 0x40 */ u8 unk40;
    /* 0x41 */ s8 unk41;
    /* 0x42 */ u8 unk42;
    /* 0x43 */ u8 unk43;
} Unk80016F38;

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
    /* 0x000 */ POLY_FT4 ft4a[4];
    /* 0x0A0 */ SPRT linea[4];
    /* 0x0F0 */ SPRT frame;
    /* 0x104 */ u8 tpage[8];
    /* 0x10C */ u8 unk10C[0xC];
    /* 0x118 */ u8 twin[0xC];
    /* 0x124 */ SPRT lineb[4];
    /* 0x174 */ POLY_FT4 ft4b[2];
    /* 0x1C4 */ SPRT linec[4];
    /* 0x214 */ POLY_FT4 ft4c[2];
    /* 0x264 */ u8 unk264[0x30];
} PanelPrims;

typedef struct {
    DR_MODE dm;
    SPRT sp;
} SprtPacket;

typedef struct {
    u32 tag;
    u32 code[2];
} DR_AREA;

typedef struct {
    /* 0x0 */ s8 unk0;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 player;
    /* 0x3 */ s8 unk3;
    /* 0x4 */ s8 next;
    /* 0x5 */ s8 cur;
    /* 0x6 */ s8 y;
    /* 0x7 */ s8 next2;
    /* 0x8 */ s8 cur2;
    /* 0x9 */ s8 y2;
    /* 0x0A */ s8 unkA;
    /* 0x0B */ s8 unkB;
    /* 0x0C */ s8 unkC;
    /* 0x0D */ s8 unkD;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s16 unk10;
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

#define setlen(p, _len) (((P_TAG *)(p))->len = (u8)(_len))
#define setaddr(p, _addr) (((P_TAG *)(p))->addr = (u32)(_addr))
#define getaddr(p) (u32)(((P_TAG *)(p))->addr)
#define getcode(p) (u8)(((P_TAG *)(p))->code)
#define setcode(p, _code) (((P_TAG *)(p))->code = (u8)(_code))
#define setSemiTrans(p, abe) \
    ((abe) ? setcode(p, getcode(p) | 0x02) : setcode(p, getcode(p) & ~0x02))
#define addPrim(ot, p) setaddr(p, getaddr(ot)), setaddr(ot, p)
#define getClut(x, y) (((y) << 6) | (((x) >> 4) & 0x3f))
#define setSprt(p) setlen(p, 4), setcode(p, 0x64)
#define setShadeTex(p, tge) \
    ((tge) ? setcode(p, getcode(p) | 1) : setcode(p, getcode(p) & ~1))
#define setRGB0(p, _r0, _g0, _b0) (p)->r0 = _r0, (p)->g0 = _g0, (p)->b0 = _b0
#define setUV0(p, _u0, _v0) (p)->u0 = (_u0), (p)->v0 = (_v0)
#define setWH(p, _w, _h) (p)->w = _w, (p)->h = _h
#define getTPage(tp, abr, x, y)                                                        \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | \
     (((y) & 0x200) << 2))
#define _get_mode(dfe, dtd, tpage) \
    ((0xe1000000) | ((dtd) ? 0x0200 : 0) | ((dfe) ? 0x0400 : 0) | ((tpage) & 0x9ff))
#define setDrawMode(p, dfe, dtd, tpage) \
    (setlen(p, 1), (p)->code[0] = _get_mode(dfe, dtd, tpage))

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 w, h;
} TILE;

typedef struct {
    /* 0x00 */ DR_MODE dm[2];
    /* 0x10 */ TILE prim[2];
    /* 0x30 */ Rect16 unk30;
    /* 0x38 */ Rect16 unk38;
    /* 0x40 */ Rect16 unk40;
    /* 0x48 */ Bytes4 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ s8 unk4D;
} Unk800190F4;

typedef struct {
    /* 0x00 */ Unk80016F38 *win;
    /* 0x04 */ Unk800190F4 *cursor;
    /* 0x08 */ Rect16 rect;
    /* 0x10 */ s16 col;
    /* 0x12 */ s16 prevCol;
    /* 0x14 */ s16 row;
    /* 0x16 */ s16 prevRow;
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ s16 cw;
    /* 0x1C */ s16 ch;
    /* 0x1E */ s16 ncols;
    /* 0x20 */ s16 nrows;
    /* 0x22 */ u8 ox;
    /* 0x23 */ u8 oy;
    /* 0x24 */ u8 colW;
    /* 0x25 */ u8 rowH;
    /* 0x26 */ u8 active;
    /* 0x27 */ u8 moved;
    /* 0x28 */ u8 pad;
} Menu;

typedef struct {
    /* 0x00 */ s32 key;
    /* 0x04 */ u8 unk4[0xC];
    /* 0x10 */ s32 name[4];
} FileEntry;

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ void *unk4;
} Unk2220;

typedef struct {
    /* 0x0000 */ u8 unk0[0x1F80];
    /* 0x1F80 */ s16 *bonepos[32];
    /* 0x2000 */ s32 scale[34][4];
    /* 0x2220 */ Unk2220 unk2220[16];
    /* 0x22A0 */ u8 unk22A0[0x430];
    /* 0x26D0 */ s32 unk26D0;
    /* 0x26D4 */ s32 unk26D4;
    /* 0x26D8 */ s32 unk26D8;
    /* 0x26DC */ u8 unk26DC[0x18];
    /* 0x26F4 */ void *unk26F4;
} Model2220;

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 val;
    /* 0x8 */ s32 d0;
    /* 0xC */ s32 d1;
} AnimChan;

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
    /* 0x13C */ u8 unk13C[0x20];
    /* 0x15C */ u8 *unk15C[2];
    /* 0x164 */ u8 *unk164[2];
    /* 0x16C */ void *unk16C;
    /* 0x170 */ Bytes8 unk170;
    /* 0x178 */ s32 unk178;
    /* 0x17C */ s32 unk17C;
    /* 0x180 */ s32 n;
    /* 0x184 */ u8 type;
    /* 0x185 */ Bytes4 unk185;
    /* 0x189 */ Bytes4 unk189;
    /* 0x18D */ Bytes4 unk18D;
    /* 0x194 */ s32 unk194;
    /* 0x198 */ s32 unk198;
    /* 0x19C */ s16 unk19C[5];
    /* 0x1A6 */ s16 unk1A6;
    /* 0x1A8 */ s16 unk1A8;
    /* 0x1AA */ u8 unk1AA;
    /* 0x1AB */ u8 unk1AB;
    /* 0x1AC */ u8 unk1AC;
    /* 0x1AD */ s8 unk1AD;
} Obj32;

typedef struct Panel {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 unkF;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
    /* 0x18 */ s16 unk18;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ u8 unk1C[4];
    /* 0x20 */ struct Panel *parent;
} Panel;

typedef struct {
    /* 0x000 */ u8 unk0[0x10C];
    /* 0x10C */ s32 unk10C[3];
} Unk80030CA8;

typedef struct {
    /* 0x00 */ u16 cards[30];
    /* 0x3C */ char name[0x28];
    /* 0x64 */ u8 unk64[4];
    /* 0x68 */ u8 unk68[5];
    /* 0x6D */ u8 unk6D;
} SavedDeck;

typedef struct {
    /* 0x00 */ s32 unk0[5];
    /* 0x14 */ s32 unk14;
} Obj18;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 flags;
    /* 0x08 */ s32 nobj;
    /* 0x0C */ Obj18 obj[1];
} Tmd18;

typedef struct {
    /* 0x00 */ u32 *data;
    /* 0x04 */ u32 *work;
    /* 0x08 */ u32 *ot;
    /* 0x0C */ u32 packet;
    /* 0x10 */ u32 unk10[4];
    /* 0x20 */ void *unk20;
} SortWork;

#define SORT_WORK ((SortWork *)0x1F800000)

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
    /* 0x0 */ u8 state;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ s16 id;
    /* 0x4 */ s8 *card;
} CardSlot;

typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1[3];
    /* 0x004 */ s32 unk4[4];
    /* 0x014 */ CardSlot cards[30];
    /* 0x104 */ s32 unk104;
    /* 0x108 */ u16 unk108[3];
    /* 0x10E */ u8 unk10E[2];
} Unk110;

typedef struct {
    /* 0x000 */ u8 unk0[0x14];
    /* 0x014 */ CardSlot cards[30];
    /* 0x104 */ u8 unk104[0xC];
    /* 0x110 */ s32 unk110;
    /* 0x114 */ s8 *unk114;
    /* 0x118 */ u8 unk118[2];
    /* 0x11A */ s16 unk11A;
    /* 0x11C */ s16 unk11C[5];
    /* 0x126 */ s16 unk126[5];
    /* 0x130 */ Popup unk130[5];
    /* 0x158 */ u8 unk158[2];
    /* 0x15A */ s16 unk15A;
    /* 0x15C */ s16 unk15C[3];
    /* 0x162 */ s16 unk162;
    /* 0x164 */ s16 unk164;
    /* 0x166 */ u8 unk166[8];
    /* 0x16E */ s16 unk16E;
    /* 0x170 */ s16 unk170[4];
    /* 0x178 */ u32 unk178_0 : 2;
    /* 0x178 */ u32 unk178_2 : 2;
    /* 0x178 */ u32 unk178_4 : 2;
    /* 0x178 */ u32 unk178_6 : 1;
    /* 0x178 */ u32 unk178_7 : 4;
    /* 0x178 */ u32 unk178_11 : 1;
    /* 0x178 */ u32 unk178_12 : 1;
    /* 0x178 */ u32 unk178_13 : 2;
    /* 0x178 */ u32 unk178_15 : 2;
    /* 0x178 */ u32 unk178_17 : 2;
    /* 0x178 */ u32 unk178_19 : 3;
    /* 0x178 */ u32 unk178_22 : 2;
    /* 0x178 */ u32 unk178_24 : 2;
    /* 0x178 */ u32 unk178_26 : 2;
    /* 0x178 */ u32 unk178_28 : 2;
    /* 0x178 */ u32 unk178_30 : 1;
    /* 0x178 */ u32 unk178_31 : 1;
    /* 0x17C */ u8 unk17C;
    /* 0x17D */ s8 unk17D[30];
    /* 0x19B */ s8 unk19B[30];
    /* 0x1B9 */ s8 unk1B9[4];
    /* 0x1BD */ u8 unk1BD[5];
    /* 0x1C2 */ s8 unk1C2[8];
    /* 0x1CA */ s8 unk1CA[3];
    /* 0x1CD */ s8 unk1CD;
    /* 0x1CE */ char unk1CE[1];
} Player;



typedef struct {
    /* 0x00 */ u8 unk0[0xE];
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 unk10[0x10];
} CardRec20;

typedef struct {
    /* 0x0 */ u8 unk0[0xC];
    /* 0xC */ s16 unkC;
    /* 0xE */ u8 unkE[2];
} CardRec10;

typedef struct {
    /* 0x00 */ s16 power;
    /* 0x02 */ u8 unk2[4];
    /* 0x06 */ char name[0x16];
} CardAttack;

typedef struct {
    /* 0x000 */ s16 id;
    /* 0x002 */ u8 unk2;
    /* 0x003 */ char name[0x17];
    /* 0x01A */ u8 attr;
    /* 0x01B */ s8 unk1B;
    /* 0x01C */ s8 level;
    /* 0x01D */ u8 unk1D;
    /* 0x01E */ s16 hp;
    /* 0x020 */ CardAttack attack[3];
    /* 0x074 */ CardRec20 unk74[2];
    /* 0x0B4 */ CardRec10 unkB4[3];
    /* 0x0E4 */ s8 unkE4;
    /* 0x0E5 */ u8 unkE5;
    /* 0x0E6 */ s8 unkE6;
    /* 0x0E7 */ u8 text[4][0x15];
    /* 0x13B */ u8 unk13B;
} CardInfo;

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 unk2[0xE0];
} Unk801D8400;

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 unk2[0x6E];
} Unk801D8404;

typedef struct {
    /* 0x000 */ CardInfo card[2];
    /* 0x278 */ u8 *unk278;
    /* 0x27C */ u8 *unk27C;
    /* 0x280 */ s16 unk280;
    /* 0x282 */ s16 unk282[3];
    /* 0x288 */ u8 unk288;
    /* 0x289 */ u8 unk289;
    /* 0x28A */ s16 unk28A;
    /* 0x28C */ s8 unk28C[3];
    /* 0x28F */ u8 unk28F[3];
    /* 0x292 */ u8 unk292[6];
} Deck;

typedef struct {
    /* 0x0000 */ char name[0xD];
    /* 0x000D */ u8 unkD;
    /* 0x000E */ u8 unkE;
    /* 0x000F */ u8 unkF;
    /* 0x0010 */ s16 unk10;
    /* 0x0012 */ u16 unk12;
    /* 0x0014 */ s16 unk14;
    /* 0x0016 */ s16 unk16;
    /* 0x0018 */ u16 unk18;
    /* 0x001A */ u16 unk1A;
    /* 0x001C */ u16 unk1C;
    /* 0x001E */ u16 unk1E;
    /* 0x0020 */ u32 unk20_0 : 1;
    /* 0x0020 */ u32 unk20_1 : 1;
    /* 0x0020 */ u32 unk20_2 : 1;
    /* 0x0020 */ u32 unk20_3 : 1;
    /* 0x0020 */ u32 unk20_4 : 28;
    /* 0x0024 */ s32 unk24;
    /* 0x0028 */ u32 rankA : 3;
    /* 0x0028 */ u32 rankB : 3;
    /* 0x0028 */ u32 rankC : 3;
    /* 0x0029 */ u32 unk28_9 : 1;
    /* 0x0029 */ u32 unk28_10 : 1;
    /* 0x0029 */ u32 unk28_11 : 1;
    /* 0x0029 */ u32 unk28_12 : 1;
    /* 0x0029 */ u32 unk28_13 : 1;
    /* 0x0029 */ u32 unk28_14 : 18;
    /* 0x002C */ s32 unk2C;
    /* 0x0030 */ u8 unk30[6];
    /* 0x0036 */ u16 unk36[3];
    /* 0x003C */ u8 unk3C[0x10];
    /* 0x004C */ s16 unk4C;
    /* 0x004E */ s16 unk4E;
    /* 0x0050 */ s16 unk50;
    /* 0x0052 */ s16 unk52;
    /* 0x0054 */ s16 unk54;
    /* 0x0056 */ u16 unk56;
    /* 0x0058 */ u8 unk58[0x28];
    /* 0x0080 */ Deck unk80[3];
    /* 0x0848 */ s16 unk848[0x20];
    /* 0x0888 */ u16 unk888[0x8E];
    /* 0x09A4 */ u16 unk9A4[0x8E];
    /* 0x0AC0 */ u16 unkAC0[0x9F];
    /* 0x0BFE */ u16 unkBFE[0x9F];
    /* 0x0D3C */ s16 unkD3C[0xBF][3];
    /* 0x11B6 */ s16 unk11B6[0xBF];
    /* 0x1334 */ s16 unk1334[0xBF];
    /* 0x14B2 */ u8 unk14B2[0x12D];
    /* 0x15DF */ u8 unk15DF;
    /* 0x15E0 */ u16 unk15E0[301][6];
    /* 0x23FC */ s32 unk23FC[12];
    /* 0x242C */ u8 unk242C[9];
    /* 0x2435 */ u8 unk2435[3];
    /* 0x2438 */ Unk110 unk2438[3];
    /* 0x2768 */ s16 unk2768[3];
    /* 0x276E */ s8 unk276E[3];
    /* 0x2771 */ u8 unk2771[3];
} Unk8006E050;

typedef struct {
    /* 0x000 */ u8 unk0[0x1A2];
    /* 0x1A2 */ s16 unk1A2;
    /* 0x1A4 */ u8 unk1A4;
    /* 0x1A5 */ u8 unk1A5[3];
    /* 0x1A8 */ u8 unk1A8;
    /* 0x1A9 */ u8 unk1A9;
} Unk8006E054Sub;

typedef struct {
    /* 0x0000 */ u8 *unk0;
    /* 0x0004 */ u8 unk4;
    /* 0x0005 */ u8 unk5[3];
    /* 0x0008 */ SavedDeck unk8;
    /* 0x0076 */ u8 unk76[2];
    /* 0x0078 */ Deck unk78[2][3];
    /* 0x1008 */ s16 unk1008[2];
    /* 0x100C */ Unk8006E054Sub *unk100C;
    /* 0x1010 */ u8 unk1010[0x17];
    /* 0x1027 */ u8 unk1027;
} Unk8006E054;

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
    /* 0x0 */ u32 attribute;
    /* 0x4 */ GsCOORDINATE2 *coord2;
    /* 0x8 */ u32 *tmd;
    /* 0xC */ u32 id;
} GsDOBJ4;

typedef struct {
    /* 0x00 */ AnimChan pos[3];
    /* 0x30 */ AnimChan rot[3];
    /* 0x60 */ AnimChan scale[3];
} BoneKeys;

typedef struct {
    /* 0x0000 */ s32 unk0;
    /* 0x0004 */ s16 nobj;
    /* 0x0006 */ s16 id;
    /* 0x0008 */ u8 unk8[0x20];
    /* 0x0028 */ GsCOORDINATE2 root;
    /* 0x0078 */ GsCOORDINATE2 coord[32];
    /* 0x0A78 */ SVECTOR rot;
    /* 0x0A80 */ SVECTOR rots[32];
    /* 0x0B80 */ GsDOBJ4 obj[32];
    /* 0x0D80 */ BoneKeys keys[32];
    /* 0x1F80 */ s16 *bonepos[32];
    /* 0x2000 */ u8 unk2000[0x6B0];
    /* 0x26B0 */ s8 parent[32];
    /* 0x26D0 */ s32 unk26D0;
    /* 0x26D4 */ s32 unk26D4;
    /* 0x26D8 */ s32 unk26D8;
    /* 0x26DC */ void *data;
    /* 0x26E0 */ s32 unk26E0;
    /* 0x26E4 */ Rect16 prect;
    /* 0x26EC */ Rect16 crect;
    /* 0x26F4 */ void *pak;
    /* 0x26F8 */ u8 clut[0x200];
} Model;

typedef struct {
    /* 0x0 */ s16 id;
    /* 0x2 */ s16 vab;
    /* 0x4 */ s32 unk4;
    /* 0x8 */ u8 *buf;
} SndSlot;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 cur;
    /* 0x04 */ s16 seq[2];
    /* 0x08 */ u8 vol[2];
    /* 0x0A */ u8 unkA[2];
    /* 0x0C */ u8 *data[2];
    /* 0x14 */ SndSlot unk14;
    /* 0x20 */ SndSlot slot[2];
} SndState;

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
    /* 0x00 */ u8 *base;
    /* 0x04 */ u8 *start;
    /* 0x08 */ u8 *pc;
    /* 0x0C */ u32 offset;
    /* 0x10 */ u32 size;
    /* 0x14 */ u8 *event;
    /* 0x18 */ u16 eventOp;
    /* 0x1A */ u16 eventArg;
    /* 0x1C */ u16 params[4];
    /* 0x24 */ s16 busy;
} Script;

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
} LINE_F2;

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

extern s32 D_801D6B24;
extern u16 D_801D6B10;
extern u16 D_801D6B12;
extern u16 D_801D6B14;
extern u16 D_801D6B20;
extern s16 D_801D6B22;
s32 func_80029990(void);
#define CUR_SPRT ((SprtPacket *)D_801D6B24)

s32 VSync(s32);
void SetSemiTrans(void *, s32);
void SetShadeTex(void *, s32);
void func_8001E76C(void *, u8, u8, u8);
s32 rand(void);
s32 sprintf(char *, const char *, ...);
s32 func_80048230(s32, s32);
extern u8 D_8006E520[6][3];
s32 DrawSync(s32);
s32 LoadImage(s16 *, s32);
s32 func_80014A90();

void ResetCallback(void);
void SetDispMask(s32);
void GsInitGraph(u16, u16, u16, u16, u16);
s32 ClearImage(Rect16 *, s32, s32, s32);
void SsInit(void);
void func_8001AA80(s32);
void func_800149A8(s32, s32, void (*)(), s32, s32, s32, s32);
void func_800155F4();

int main(void) {
    Rect16 r;

    ResetCallback();
    VSync(0);
    SetDispMask(0);
    GsInitGraph(320, 240, 0, 0, 0);
    r.x = 0;
    r.y = 0;
    r.w = 640;
    r.h = 511;
    ClearImage(&r, 0, 0, 0);
    DrawSync(0);
    SsInit();
    func_8001AA80(1);
    func_800149A8(1, 0x400, func_800155F4, 0, 0, 0, 0);
    for (;;) {
        rand();
    }
}

s32 func_8006A804();
s32 func_8006A814();
extern s32 D_80077A08;
extern s16 *D_80077AEC;
extern s16 D_80077BA0;

void func_80013F04(s32 arg0) {
    s16 *p;
    s32 i;

    func_8006A804();
    if (arg0 != 0) {
        if (D_80077A08 == 0) {
            D_80077A08 = 1;
            p = &D_80077BA0;
            for (i = 0x1F; i >= 0; i--, p += 0x60) {
                if (*p > 0) {
                    D_80077AEC = p;
                    break;
                }
            }
        }
    } else if (D_80077A08 != 0) {
        D_80077A08 = 0;
        D_80077AEC = &D_80077BA0;
    }
    func_8006A814();
}

extern s16 D_80077A18;
extern s16 D_80077A1A;
extern s16 D_80077A1C;
extern s32 D_80077AE0;
extern s32 D_80077ADC;
extern s32 D_80077AD8;
extern s32 D_80077BC0;
extern Unk80077A0C *D_80077A0C;
long func_800141B8();
long func_8006A794(unsigned long, long, long, long (*)());
long func_8006A7C4(long);
s32 SetRCnt(u32, u16, s32);
s32 StartRCnt(u32);

s32 func_80013FA4(s32 mode, s32 size, s32 pc, s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 *p;
    s32 *t;
    s32 *tcb;
    s32 *src;
    s32 *dst;
    s32 i;
    s32 j;
    s32 stack;
    s32 ev;

    func_8006A804();
    D_80077A08 = mode;
    for (p = (s32 *)&D_80077BA0, i = 0x1F; i >= 0; i--, p += 0x30) {
        *p = 0;
    }
    D_80077A18 = D_80077A1A = *(u16 *)&D_80077A1C = 0xFFFF;
    D_80077A0C = (Unk80077A0C *)((Thread *)&D_80077BA0 - 1);
    ((Thread *)&D_80077AE0)->flags = 0x8000FFFF;
    ((Thread *)&D_80077AE0)->next = (Thread *)D_80077A0C + 1;
    if (D_80077A08 != 0) {
        ((Thread *)&D_80077AE0)->prev = (Thread *)&D_80077AE0;
    } else {
        D_80077AEC = &D_80077BA0;
    }
    ((Thread *)&D_80077AE0)->unk14 = -1;
    tcb = *(s32 **)0x108;
    D_80077ADC = *tcb;
    src = (s32 *)(D_80077ADC + 8);
    dst = &D_80077BC0;
    for (j = 0x27; j >= 0; j--) {
        *dst++ = *src++;
    }
    t = (s32 *)&D_80077BA0;
    t[0] = 0xA0000000;
    t[0x28] = pc;
    t[0x2B] = 0x4000FF04;
    t[0xC] = a0;
    t[0xD] = a1;
    t[0xE] = a2;
    t[0xF] = a3;
    t[2] = (s32)&D_80077AE0;
    t[3] = (s32)&D_80077AE0;
    t[5] = 0;
    t[6] = 0;
    stack = (s32)func_8001ABCC(size, -3);
    if (stack == 0) {
        return -6;
    }
    t[7] = stack;
    t[0x25] = stack + (size & ~7) - 0x20;
    ev = func_8006A794(0xF2000003, 2, 0x1000, (long (*)())func_800141B8);
    func_8006A7C4(ev);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    D_80077AD8 = ev;
    func_8006A814();
    return 0;
}

extern void *D_80077A10;
extern void *D_80077A14;
void func_80014CF0(void);

long func_800141B8(void) {
    Thread *t;
    s32 *ctx;
    s32 *regs;
    s32 i;

    t = (Thread *)D_80077A0C;
    func_80014CF0();
    ctx = (s32 *)(D_80077ADC + 8);
    regs = t->regs;
    for (i = 0x27; i >= 0; i--) {
        *regs++ = *ctx++;
    }
    t->flags |= 0x20000000;
    if ((D_80077A1A = D_80077A18) == 0) {
        if (D_80077A08 == 0) {
            D_80077A14 = &D_80077BA0;
            D_80077A1C = 0;
        }
    } else {
        D_80077A10 = t;
        t = (Thread *)&D_80077BA0;
        D_80077A0C = (Unk80077A0C *)t;
        D_80077A18 = D_80077BA0;
        ctx = (s32 *)(D_80077ADC + 8);
        regs = t->regs;
        for (i = 0x27; i >= 0; i--) {
            *ctx++ = *regs++;
        }
    }
}

extern void *D_80077A10;
extern void *D_80077A14;
extern s16 D_80077A1A;
extern s16 D_80077A1C;
extern s32 D_80077AE0;
extern Unk80077A0C *D_80077A0C;
void *func_800142D0(void *t) {
    s16 *p = *(s16 **)((s8 *)t + 0xC);
    s16 prio = *p;

    if (*p > 0 && prio == D_80077A1A) {
        D_80077A14 = p;
        D_80077A1C = prio;
        p = D_80077A10;
        D_80077A1A = -1;
    } else {
        prio = *p;
        if ((u16)prio > (u16)D_80077A1C) {
            prio = D_80077A1C;
            p = D_80077A14;
            *(u16 *)&D_80077A1C = 0xFFFF;
        }
    }
    D_80077A0C = p;
    D_80077A18 = prio;
    return p;
}

extern s32 D_80077C30;

s32 func_80014364(s32 id, s32 where, s32 prio, s32 size, s32 unused, s32 pc, s32 a0, s32 a1, s32 a2, s32 a3) {
    Thread *t;
    Thread *prev;
    Thread *next;
    s32 ret;
    s32 f;
    s32 g;
    s32 *src;
    s32 *dst;
    s32 i;
    s32 stack;

    t = (Thread *)&D_80077BA0 + id;
    if (id != 0) {
        if ((s32)t->flags < 0) {
            return -1;
        }
    } else {
        for (id++; id < 32; id++) {
            if ((s32)(++t)->flags >= 0) {
                break;
            }
        }
        if (id >= 32) {
            return -1;
        }
    }
    ret = 0;
    if (where < 0) {
        prev = (Thread *)&D_80077BA0;
        if (prio >= *(u16 *)prev) {
            do {
                prev = prev->prev;
            } while (prio >= *(u16 *)prev);
        }
        goto after;
    }
    if (where >= 32) {
        where -= 32;
        prev = (Thread *)&D_80077BA0 + where;
        if ((s32)prev->flags >= 0) {
            return -3;
        }
        f = prev->flags;
        if ((u16)f < prio) {
            ret = -0x86;
            prio = f;
        }
    after:
        next = prev->next;
        next->prev = t;
    } else {
        next = (Thread *)&D_80077BA0 + where;
        if ((s32)next->flags >= 0) {
            return -3;
        }
        g = next->flags;
        if (prio < (u16)g) {
            ret = -0x86;
            prio = g;
        }
        prev = next->prev;
        next->prev = t;
    }
    prev->next = t;
    t->next = next;
    t->prev = prev;
    if (D_80077A08 != 0 && prio > 0 && *(s16 *)next == 0) {
        D_80077AEC = (s16 *)t;
    }
    src = (s32 *)(D_80077ADC + 8);
    dst = t->regs;
    for (i = 0x27; i >= 0; i--) {
        *dst++ = *src++;
    }
    t->flags = prio | 0xA0000000;
    t->regs[32] = pc;
    t->regs[35] = 0x4000FF04;
    t->regs[4] = a0;
    t->regs[5] = a1;
    t->regs[6] = a2;
    t->regs[7] = a3;
    t->regs[31] = (s32)func_80014A90;
    t->regs[28] = D_80077C30;
    t->unk14 = id;
    t->unk18 = 0;
    stack = (s32)func_8001ABCC(size, -3);
    if (stack == 0) {
        return -6;
    }
    t->stack = stack;
    t->regs[29] = stack + (size & ~7) - 0x20;
    return ret;
}


s32 func_8001AE90(void *);
s32 func_8001AFF0(s32 arg0);
extern Unk80077A0C *D_80077A0C;
s32 func_80014614(s32 arg0) {
    void *t;
    void *prev;
    void *next;
    void **cur;

    t = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)t + 0)) >= 0) {
        return -0x83;
    }
    if (t == D_80077A0C) {
        return -4;
    }
    if (t == &D_80077BA0) {
        return -5;
    }
    prev = (*(void **)((s8 *)t + 8));
    next = (*(void **)((s8 *)t + 0xC));
    (*(void **)((s8 *)prev + 0xC)) = next;
    (*(void **)((s8 *)next + 8)) = prev;
    if (D_80077A08 != 0) {
        cur = (void **)&D_80077AE0;
        if (cur[3] == t) {
            cur[3] = next;
        }
    }
    if ((D_80077A1A > 0) && (t == D_80077A10)) {
        D_80077A10 = next;
        D_80077A1A = (*(u16 *)((s8 *)next + 0));
    }
    if ((D_80077A1C >= 0) && (t == D_80077A14)) {
        D_80077A14 = next;
        D_80077A1C = (*(u16 *)((s8 *)next + 0));
    }
    func_8001AFF0((*(s32 *)((s8 *)t + 0x14)));
    func_8001AE90((*(void **)((s8 *)t + 0x1C)));
    (*(s32 *)((s8 *)t + 0)) = 0;
    return 0;
}

void *func_800142D0(void *);

void func_80014748(void) {
    void *t;
    void *prev;
    void *next;
    void **cur;

    t = D_80077A0C;
    prev = (*(void **)((s8 *)t + 8));
    next = (*(void **)((s8 *)t + 0xC));
    (*(void **)((s8 *)prev + 0xC)) = next;
    (*(void **)((s8 *)next + 8)) = prev;
    if (D_80077A08 != 0) {
        cur = (void **)&D_80077AE0;
        if (cur[3] == t) {
            cur[3] = next;
        }
    }
    if ((D_80077A1A > 0) && (t == D_80077A10)) {
        D_80077A10 = next;
        D_80077A1A = (*(u16 *)((s8 *)next + 0));
    }
    if ((D_80077A1C >= 0) && (t == D_80077A14)) {
        D_80077A14 = next;
        D_80077A1C = (*(u16 *)((s8 *)next + 0));
    }
    func_8001AFF0((*(s32 *)((s8 *)t + 0x14)));
    func_8001AE90((*(void **)((s8 *)t + 0x1C)));
    (*(s32 *)((s8 *)t + 0)) = 0;
    func_800142D0(t);
}

extern int func_80014A00(int);

int func_80014840(void) {
    int n = D_80077A0C->unk14;
    int i;
    int ret = 0;

    for (i = 1; i < 0x20; i++) {
        if (i == n) {
            continue;
        }
        if (func_80014A00(i) == 0) {
            ret++;
        }
    }
    return ret;
}

s32 func_800148B0(void) {
    return (*(s32 *)((s8 *)D_80077A0C + 0x14));
}

s32 func_800148C8(s32 arg0, s32 arg1) {
    void *temp_v1;

    temp_v1 = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)temp_v1 + 0)) >= 0) {
        return -3;
    }
    if (temp_v1 == D_80077A0C) {
        return -0x84;
    }
    (*(s32 *)((s8 *)temp_v1 + 0x18)) = arg1;
    (*(s32 *)((s8 *)temp_v1 + 4)) = 0;
    return 0;
}

s32 func_8001491C(s32 arg0) {
    void *temp_v1;

    temp_v1 = (s8 *)&D_80077BA0 + arg0 * 0xC0;
    if ((*(s32 *)((s8 *)temp_v1 + 0)) >= 0) {
        return -3;
    }
    if (temp_v1 == D_80077A0C) {
        return -0x84;
    }
    return (*(s32 *)((s8 *)temp_v1 + 4));
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014970);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800149A0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800149A8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800149B8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014A00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014A48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014A90);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014AC8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014C08);

extern s32 D_8006E050;
extern s32 D_800794EC;

void func_80014CF0(void) {
    s32 i;

    if (D_8006E050 != 0) {
        for (i = 0; i < 2; i++) {
            ((Unk8006E050 *)D_8006E050)[i].unk24++;
        }
    }
    D_800794EC++;
}

extern Screen D_800793A8;
void func_800677A4(POLY_FT4 *);
void SetDrawStp(DR_STP *, s32);

void func_80014D64(void) {
    s32 i;
    s32 j;
    POLY_FT4 *p;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            D_800793A8.sprt[i][j].tag = 0x05000000;
            D_800793A8.sprt[i][j].code = 0x66;
            D_800793A8.sprt[i][j].u0 = 0;
            D_800793A8.sprt[i][j].v0 = 0;
            D_800793A8.sprt[i][j].w = 256 - j * 192;
            D_800793A8.sprt[i][j].h = 240;
            p = &D_800793A8.poly[i][j];
            func_800677A4(p);
            p->u0 = j * 32;
            p->v0 = 0;
            p->u1 = j * 32 - 96;
            p->v1 = 0;
            p->u2 = j * 32;
            p->v2 = 240;
            p->u3 = j * 32 - 96;
            p->v3 = 240;
            setShadeTex(p, 0);
            SetSemiTrans(p, 1);
        }
    }
    SetDrawStp(&D_800793A8.stp[0], 1);
    SetDrawStp(&D_800793A8.stp[1], 0);
    D_800793A8.r = 0xA8;
    D_800793A8.g = 0xA8;
    D_800793A8.b = 0xA8;
    D_800793A8.x = 0;
    D_800793A8.y = 0;
    D_800793A8.mode = 0;
}

extern Screen D_800793A8;
extern Unk800793A0 *D_800793A0;
extern u8 D_800794E7;
extern u8 D_800794F4;
void func_801EAD04(void);
u32 GetTPage(s32, s32, s32, s32);

void func_80014EF0(void) {
    s32 i;
    POLY_FT4 *p;
    s32 unused[4];

    if (D_800794E7 == 0) {
        return;
    }
    if (D_800794E7 != 1) {
        func_801EAD04();
    }
    for (i = 1; i >= 0; i--) {
        if (D_800793A8.mode == 1) {
            D_800793A8.sprt[D_800794F4][i].tpage = GetTPage(2, 0, i * 0x100, 0x100 - D_800794F4 * 0x100) | 0xE1000000;
            (D_800793A8.sprt[D_800794F4] + i)->x0 = D_800793A8.x + (i << 8);
            (D_800793A8.sprt[D_800794F4] + i)->y0 = D_800793A8.y;
            (D_800793A8.sprt[D_800794F4] + i)->r0 = D_800793A8.r;
            (D_800793A8.sprt[D_800794F4] + i)->g0 = D_800793A8.g;
            (D_800793A8.sprt[D_800794F4] + i)->b0 = D_800793A8.b;
            addPrim(&D_800793A0->ot[0], &D_800793A8.sprt[D_800794F4][i]);
        } else {
            p = &D_800793A8.poly[D_800794F4][i];
            p->tpage = getTPage(2, D_800793A8.abr & 3, i * 160, 0x100 - D_800794F4 * 0x100);
            p->x0 = D_800793A8.px[i][0];
            p->y0 = D_800793A8.py[i][0];
            p->x1 = D_800793A8.px[i][1];
            p->y1 = D_800793A8.py[i][1];
            p->x2 = D_800793A8.px[i][2];
            p->y2 = D_800793A8.py[i][2];
            p->x3 = D_800793A8.px[i][3];
            p->y3 = D_800793A8.py[i][3];
            p->r0 = D_800793A8.r;
            p->g0 = D_800793A8.g;
            p->b0 = D_800793A8.b;
            addPrim(&D_800793A0->ot[0], p);
        }
    }
}

void func_80014D64(void);

void SetGraphDebug(s32);
void InitGeom(void);
extern s32 D_800794F0;
extern s32 D_800794F8;
void func_800152AC(void) {
    s32 *p;
    s32 i;

    SetGraphDebug(0);
    InitGeom();
    p = (s32 *)0x1F800000;
    for (i = 0; i < 0x100; i++) {
        *p++ = 0;
    }
    (*(s16 *)((s8 *)(&D_800794F8) + 0)) = 0;
    (*(s16 *)((s8 *)(&D_800794F8) + 2)) = 0;
    (*(s16 *)((s8 *)(&D_800794F8) + 4)) = 0x100;
    (*(s16 *)((s8 *)(&D_800794F8) + 6)) = 0x100;
    (*(s32 *)((s8 *)(&D_800794F8) + 0x50)) = 2;
    (*(s32 *)((s8 *)(&D_800794F8) + 0x48)) = 0;
    D_800794F0 = 1;
    func_80014D64();
}

extern s32 D_8006DD4C;
void func_8001A9B0(void);
s32 func_80014C08(s32);
void func_80014AC8(void);
void SetDispMask(s32);
void ClearOTagR(u32 *, s32);
void func_8002FAE4(void);
void func_800271D0(void);
void func_80016BEC(void);
void func_80014EF0(void);
s32 DrawSync(s32);
void GsSwapDispBuff(void);
void PutDispEnv(DISPENV *);
void PutDrawEnv(DRAWENV *);
void DrawOTag(u32 *);

void func_80015328(void) {
    Unk800794F8 *g;
    s32 n;
    void (**cb)(Unk800793A0 *, s32);

    g = (Unk800794F8 *)&D_800794F8;
    g->unk8[0] = 0;
    D_800794EC = 0;
    for (; g->unk48 <= 0; g->unk48++) {
        func_8001A9B0();
        func_80014C08(1);
        g->unk50 = D_800794EC;
        if (D_800794EC == 0) {
            g->unk50 = 1;
        }
        D_800794EC = 0;
    }
    SetDispMask(1);
    D_800794F4 = 0;
    D_800793A0 = &g->unk98[0];
    ClearOTagR(g->unk98[0].ot, 0x1000);
    for (;;) {
        n = D_800794F0;
        func_8001A9B0();
        while (n >= 2 || g->unk48 == 0) {
            func_80014AC8();
            n--;
        }
        D_800794F4 ^= 1;
        D_800793A0 = &g->unk98[D_800794F4];
        ClearOTagR(D_800793A0->ot, 0x1000);
        if (D_800793A8.mode != 0) {
            addPrim(&D_800793A0->ot[0], &D_800793A8.stp[1]);
            addPrim(&D_800793A0->ot[0xFFF], &D_800793A8.stp[0]);
        }
        func_8002FAE4();
        func_800271D0();
        func_80016BEC();
        if (D_8006DD4C != 0) {
            for (cb = g->unk8; *cb != 0; cb++) {
                (*cb)(D_800793A0, D_800794F4);
            }
        }
        func_80014EF0();
        func_80014AC8();
        DrawSync(0);
        if (g->unk4C != 0) {
            GsSwapDispBuff();
        }
        PutDispEnv(&D_800793A0->disp);
        PutDrawEnv(&D_800793A0->draw);
        DrawOTag(&D_800793A0->ot[0xFFF]);
        g->unk50 = D_800794EC;
        if (D_800794EC == 0) {
            g->unk50 = 1;
        }
        D_800794EC = 0;
    }
}

void func_80015328();
void func_8002BC58(void);
void func_8001A600(void);
void func_8006A884(s32);
void func_800157B0(void);
void func_800152AC(void);
int func_80015EDC(void);
void func_8002ADEC();
void func_80026E90(s32, s32, s32);
void func_80016948(s32 n);
void func_8001B90C(s32 w, s32 h, s32 interlace);
void func_8002D404(void);
void func_8002F79C(void);
void func_8002B3EC(s32 arg0, s32 arg1);
void func_8002F4F4(void);
s32 func_80014C08(s32);
void func_800168C4(void);
void func_8001F040(void);
extern s32 D_8008983C;
s32 func_800149B8();
s32 func_801E055C(s32);
void func_8002AEA4(s32);
void func_8002B688(void);

void func_800155F4(void) {
    s32 t;

    t = func_800148B0();
    func_8002BC58();
    func_8001A600();
    func_8006A884(0);
    func_800157B0();
    func_800152AC();
    func_80014840();
    func_80015EDC();
    func_8001AA80(0);
    func_8002ADEC(t);
    func_80026E90(0x3C0, 0x100, 0x3E8);
    func_80016948(0xD);
    func_8001B90C(0x140, 0xF0, 0);
    func_8002D404();
    func_8002F79C();
    for (;;) {
        func_80014840();
        func_80015EDC();
        func_8001AA80(0);
        func_80014C08(0xA);
        func_800168C4();
        func_8001F040();
        D_8008983C = 1;
        func_800149B8(0, -1, 0, 0x800, func_8002B3EC, 2, t);
        func_80014C08(0x7FFFFFFF);
        func_801E055C(0);
        func_8002AEA4(1);
        func_8002B688();
        func_8001B90C(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, func_80015328, 0, 0, 0, 0);
        func_80014C08(0xA);
        func_800149B8(0, -1, 0, 0x400, func_8002F4F4, 0, 0, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(0xA);
    }
}

extern Unk80081710 D_80081710[4];
extern s32 D_800857D0;
void ResetCallback(void);
s32 CdInit(void);
s32 CdControlB(u8, u8 *, u8 *);
void func_8005A344(s32);

void func_800157B0(void) {
    u8 param[8];
    Unk80081710 *p;
    s32 i;

    ResetCallback();
    while (CdInit() == 0) {
    }
    VSync(4);
    for (;;) {
        param[0] = 0x80;
        if (CdControlB(0xE, param, 0) != 0) {
            break;
        }
        VSync(0);
    }
    VSync(4);
    func_8005A344(0);
    p = D_80081710;
    for (i = 3; i >= 0; i--, p++) {
        p->unk0 = 0;
    }
    D_800857D0 = 0;
}

CdFile *func_80015AD8(s32, s32);
s32 func_80015EAC(CdFile *f);
s32 func_80015F34(CdFile *f, s32 size, u8 *dst);
s32 CdIntToPos(s32, u8 *);
s32 CdRead(s32, u8 *, s32);
s32 CdReadSync(s32, u8 *);
s32 func_80014C08(s32);
extern s32 D_800857E0;

s32 func_80015848(s32 arg0) {
    CdFile *f;

    f = func_80015AD8(arg0, 0);
    if (f != 0 && func_80015F34(f, 0x4000, (u8 *)&D_800857E0) != 0) {
        func_80015EAC(f);
        D_800857D0 = 1;
        return 0;
    }
    return 1;
}

extern FileEntry D_8006DD50;

FileEntry *func_800158B0(CdFile *f, char *name, s32 key) {
    u8 result[8];
    FileEntry *e;
    s32 n;
    s32 i;
    s32 r;

    if (key == 0x80 && *(s32 *)name == 0) {
        return &D_8006DD50;
    }
    for (;;) {
        if (f->remaining <= 0) {
            return 0;
        }
        CdIntToPos(f->sector, f->loc);
        e = (FileEntry *)f->buf;
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            while (CdRead(2, (u8 *)e, 0x80) == 0) {
            }
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        n = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            n = f->remaining + 0x1000;
        }
        for (; n > 0; n -= 0x20, e++) {
            if (e->key == 0) {
                return 0;
            }
            if (e->key == key) {
                for (i = 0; i < 4; i++) {
                    if (e->name[i] != *(s32 *)(name + (i << 2))) {
                        break;
                    }
                }
                if (i == 4) {
                    return e;
                }
            }
        }
    }
}

FileEntry *func_80015A3C(char *name, s32 key) {
    FileEntry *e;
    s32 n;
    s32 i;

    e = (FileEntry *)&D_800857E0;
    for (n = 0x1FF; n >= 0; n--, e++) {
        if (e->key == 0) {
            return 0;
        }
        if (e->key == key) {
            for (i = 0; i < 4; i++) {
                if (e->name[i] != *(s32 *)(name + (i << 2))) {
                    break;
                }
            }
            if (i == 4) {
                return e;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015AD8);

s32 func_8005A364(s32, s32);

s32 func_80015EAC(CdFile *f) {
    f->unk0 = 0;
    return func_8005A364(0, 0) == 5;
}

extern int func_8005A364(int, int);

int func_80015EDC(void) {
    Unk80081710 *p = D_80081710;
    int i;

    for (i = 3; i >= 0; i--, p++) {
        if (p->unk0 > 0) {
            p->unk0 = 0;
        }
    }
    return func_8005A364(0, 0) == 5;
}

s32 func_80015F34(CdFile *f, s32 size, u8 *dst) {
    u8 result[8];
    u8 result2[8];
    s32 total;
    s32 sectors;
    s32 n;
    s32 r;
    u8 *src;

    total = 0;
    n = f->avail;
    if (n > 0) {
        if (size < n) {
            n = size;
        }
        total = n;
        f->avail -= total;
        src = f->cur;
        size -= total;
        for (n = total - 4; n >= 0; n -= 4) {
            *(s32 *)dst = *(s32 *)src;
            src += 4;
            dst += 4;
        }
        f->cur = src;
    }
    if (f->remaining < size) {
        size = f->remaining;
    }
    if (size <= 0 || f->remaining <= 0) {
        return total;
    }
    sectors = size / 0x800;
    if (sectors > 0) {
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            while (CdRead(sectors, dst, 0x80) == 0) {
            }
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += sectors;
        dst += sectors << 11;
        n = sectors << 11;
        total += n;
        size -= n;
        f->remaining -= n;
    }
    if (size <= 0 || f->remaining <= 0) {
        return total;
    }
    CdIntToPos(f->sector, f->loc);
    do {
        while (CdControlB(2, f->loc, result2) == 0) {
        }
        do {
            f->cur = f->buf;
        } while (CdRead(2, f->buf, 0x80) == 0);
        while ((r = CdReadSync(1, 0)) > 0) {
            func_80014C08(1);
        }
    } while (r != 0);
    f->sector += 2;
    f->avail = 0x1000;
    if ((f->remaining -= 0x1000) < 0) {
        f->avail = f->remaining + 0x1000;
    }
    n = f->avail;
    if (n > 0) {
        if (size < n) {
            n = size;
        }
        total += n;
        f->avail -= n;
        src = f->cur;
        do {
            *(s32 *)dst = *(s32 *)src;
            src += 4;
            dst += 4;
            n -= 4;
        } while (n > 0);
        f->cur = src;
    }
    return total;
}



s32 func_800161D8(CdFile *f) {
    u8 result[8];
    s32 r;

    if (f->avail <= 0) {
        if (f->remaining <= 0) {
            return -1;
        }
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            do {
                f->cur = f->buf;
            } while (CdRead(2, f->buf, 0x80) == 0);
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        f->avail = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            f->avail = f->remaining + 0x1000;
        }
        if (f->avail <= 0) {
            return -1;
        }
    }
    f->avail--;
    return *f->cur++;
}

s32 func_800162F0(CdFile *f) {
    u8 result[8];
    s16 i;
    s16 v;
    s32 r;

    if (f->avail < 2) {
        i = 0;
        v = 0;
        while (f->avail != 0 && i++ < 2) {
            v += *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        if (f->remaining <= 0) {
            return -1;
        }
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            do {
                f->cur = f->buf;
            } while (CdRead(2, f->buf, 0x80) == 0);
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        f->avail = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            f->avail = f->remaining + 0x1000;
        }
        if (f->avail + i < 2) {
            return -1;
        }
        while (f->avail != 0 && i++ < 2) {
            v += *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        return v;
    }
    f->avail -= 2;
    /* sic: undefined order; the original reads one byte twice and advances once */
    return *f->cur++ + (*f->cur++ << 8);
}

s32 func_80016500(CdFile *f) {
    u8 result[8];
    s16 i;
    s32 v;
    s32 w;
    s32 r;

    if (f->avail < 4) {
        i = 0;
        v = 0;
        while (f->avail != 0 && i++ < 4) {
            v |= *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        if (f->remaining <= 0) {
            return -1;
        }
        CdIntToPos(f->sector, f->loc);
        do {
            while (CdControlB(2, f->loc, result) == 0) {
            }
            do {
                f->cur = f->buf;
            } while (CdRead(2, f->buf, 0x80) == 0);
            while ((r = CdReadSync(1, 0)) > 0) {
                func_80014C08(1);
            }
        } while (r != 0);
        f->sector += 2;
        f->avail = 0x1000;
        if ((f->remaining -= 0x1000) < 0) {
            f->avail = f->remaining + 0x1000;
        }
        if (f->avail + i < 4) {
            return -1;
        }
        while (f->avail != 0 && i++ < 4) {
            v |= *f->cur++ << ((i - 1) * 8);
            f->avail--;
        }
        return v;
    }
    f->avail -= 4;
    w = (((f->cur[3] << 8) + f->cur[2] << 8) + f->cur[1] << 8) + f->cur[0];
    f->cur += 4;
    return w;
}

s8 *func_80016724(s8 *buf, s32 n, CdFile *f) {
    s8 *p;
    s32 c;

    c = 0;
    p = buf;
    while (--n > 0) {
        c = func_800161D8(f);
        if (c == -1) {
            break;
        }
        if (c == 0) {
            break;
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        if (c == 0x1A) {
            break;
        }
    }
    if (n <= 0 && !(c == -1 || c == 0 || c == '\n' || c == 0x1A)) {
        do {
            c = func_800161D8(f);
        } while (!(c == -1 || c == 0 || c == '\n' || c == 0x1A));
    }
    *p = 0;
    if (*buf == 0) {
        return 0;
    }
    return buf;
}

extern s32 D_80079500;
void func_8001683C(s32 arg0) {
    s32 *var_v0;
    s32 temp_v1;

    var_v0 = &D_80079500;
    if (arg0 != 0) {
loop_1:
        temp_v1 = (*(s32 *)((s8 *)var_v0 + 0));
        if (temp_v1 != arg0) {
            if (temp_v1 != 0) {
                var_v0 += 1;
                goto loop_1;
            }
            (*(s32 *)((s8 *)var_v0 + 0)) = arg0;
            (*(s32 *)((s8 *)var_v0 + 4)) = 0;
        }
    }
}

void func_80016878(s32 arg0) {
    s32 *p;
    s32 v;

    p = &D_80079500;
    if (arg0 == 0) {
        return;
    }
loop:
    v = *p;
    if (v == arg0) {
        goto found;
    }
    p++;
    if (v == 0) {
        return;
    }
    goto loop;
found:
    if ((*p = p[1]) == 0) {
        return;
    }
    p++;
    goto found;
}

void func_800168C4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[0] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[1] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[2] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[3] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[4] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[5] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[6] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[7] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[8] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[9] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[10] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[11] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[12] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[13] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[14] = 0;
        ((Unk800794F8 *)&D_800794F8)->unk98[i].unk4078[15] = 0;
    }
}

extern Unk800793A0 *D_800793A0;
extern s32 D_800897E8;
extern u16 D_800897EC;
extern u16 D_800897EE;
extern u16 D_800897F0;
extern u16 D_800897F2;
extern u16 D_800897F4;
u32 GetTPage(s32, s32, s32, s32);
void func_8001E6EC(s32, void *, s32, s32);
void SetDrawTPage(void *, s32, s32, s32);
s32 SetTexWindow(void *, s16 *);

void func_80016948(s32 n) {
    PanelPrims *buf;
    PanelPrims *p;
    s16 r[4];
    u32 tpage;
    s32 i;
    s32 k;
    s32 j;

    D_800897EC = n;
    D_800897EE = 0x3C0;
    D_800897F0 = 0x100;
    D_800897F2 = 0x3E0;
    D_800897F4 = 0x1F8;
    buf = func_8001ACEC(D_800897EC * sizeof(PanelPrims) * 2);
    tpage = GetTPage(0, 0, D_800897EE, D_800897F0);
    for (i = 0; i < 2; i++) {
        p = (PanelPrims *)(((Unk800794F8 *)&D_800794F8)->unk98[i].unk40BC = (s32)(buf + D_800897EC * i));
        for (k = 0; k < D_800897EC; k++, p++) {
            for (j = 0; j < 4; j++) {
                func_8001E6EC(0xC, &p->ft4a[j], 0, 0);
                p->ft4a[j].tpage = tpage;
                func_8001E6EC(0xE, &p->linea[j], 0, 0);
                func_8001E6EC(0xE, &p->lineb[j], 0, 0);
                func_8001E6EC(0xE, &p->linec[j], 0, 0);
            }
            for (j = 0; j < 2; j++) {
                func_8001E6EC(0xC, &p->ft4b[j], 0, 0);
                p->ft4b[j].tpage = tpage;
                func_8001E6EC(0xC, &p->ft4c[j], 0, 0);
                p->ft4c[j].tpage = tpage;
            }
            func_8001E6EC(0xE, &p->frame, 0, 0);
            p->frame.u0 = 0;
            p->frame.v0 = 0;
            SetDrawTPage(p->tpage, 0, 1, tpage);
            r[0] = 0;
            r[1] = 0;
            r[2] = 0;
            r[3] = 0;
            SetTexWindow(p->twin, r);
        }
    }
    D_800897E8 = D_800793A0->unk40BC;
}

extern Unk800793A0 *D_800793A0;
extern s32 D_800897E8;

void func_80016BEC(void) {
    D_800897E8 = D_800793A0->unk40BC;
}

void func_80016C08(void *arg0, void *arg1, s32 arg2, s16 *arg3, s32 a4, s32 a5, s32 a6, s32 a7) {
    Unk80016F38 *w = arg0;
    Rect16 *r = arg1;
    Rect16 *from = (Rect16 *)arg2;
    Rect16 *view = (Rect16 *)arg3;

    if (r == (Rect16 *)-1) {
        w->rect.x = 0;
        w->rect.y = 0;
        w->rect.w = 0;
        w->rect.h = 0;
    } else {
        w->rect = *r;
    }
    if (from == (Rect16 *)-1) {
        w->from.x = w->rect.x + w->rect.w / 2;
        w->from.y = w->rect.y + w->rect.h / 2;
        w->from.w = 0;
        w->from.h = 0;
    } else {
        w->from = *from;
    }
    if (view == (Rect16 *)-1) {
        w->view.x = 0;
        w->view.y = 0;
        w->view.w = w->rect.w;
        w->view.h = w->rect.h;
    } else {
        w->view = *view;
        if (w->view.x < 0) {
            w->view.x = 0;
        }
        if (w->view.y < 0) {
            w->view.y = 0;
        }
        if (w->view.w == 0) {
            w->view.w = w->rect.w;
        }
        if (w->view.h == 0) {
            w->view.h = w->rect.h;
        }
        if (w->view.w < w->rect.w) {
            w->view.w = w->rect.w;
        }
        if (w->view.h < w->rect.h) {
            w->view.h = w->rect.h;
        }
        if (w->view.x >= w->view.w - w->rect.w) {
            w->view.x = w->view.w - w->rect.w;
        }
        if (w->view.y >= w->view.h - w->rect.h) {
            w->view.y = w->view.h - w->rect.h;
        }
    }
    w->cur = w->rect;
    w->delta.x = w->cur.x - w->from.x;
    w->delta.y = w->cur.y - w->from.y;
    w->delta.w = w->cur.w - w->from.w;
    w->delta.h = w->cur.h - w->from.h;
    w->unk0 = w->rect.x - w->view.x;
    w->unk2 = w->rect.y - w->view.y;
    w->unk30[0] = w->view.x;
    w->unk30[1] = w->view.y;
    w->unk30[2] = w->view.x;
    w->unk30[3] = w->view.y;
    w->unk3C = a7;
    w->unk3D = 0;
    w->unk3E = 0;
    w->unk3F = a4;
    w->unk41 = 0;
    w->unk42 = a5;
    if ((u32)a6 > 256) {
        w->unk40 = 0xFF;
    } else {
        w->unk40 = a6;
    }
    w->unk38 = 0;
    w->unk39 = 0;
    if ((a5 >> 4) < 5) {
        w->unk43 = 0;
    } else {
        w->unk43 = 1;
    }
    w->unk2C = 0;
}

void func_80016F38(Unk80016F38 *p, Rect16 *r) {
    s32 dx;
    s32 dy;

    if (r == (Rect16 *)-1) {
        dx = p->cur.w / 2;
        p->delta.x = dx;
        dy = p->cur.h / 2;
        p->delta.y = dy;
        p->delta.w = -p->cur.w;
        p->delta.h = -p->cur.h;
        p->cur.x += dx;
        p->cur.y += dy;
        p->cur.w = 0;
        p->cur.h = 0;
    } else {
        p->delta.x = r->x - p->cur.x;
        p->delta.y = r->y - p->cur.y;
        p->delta.w = r->w - p->cur.w;
        p->delta.h = r->h - p->cur.h;
        p->cur = *r;
    }
    p->unk3D = p->unk3C - p->unk3D;
    if ((s8)p->unk3D < 0) {
        p->unk3D = 0;
    }
    p->unk41 = 0;
}

void func_8001705C(s16 *arg0, s32 arg1, s32 arg2) {
    if (((s8 *)arg0)[0x3E] >= 6) {
        ((s8 *)arg0)[0x3E] = 0;
    }
    if (arg1 > arg0[4] - arg0[8]) {
        arg1 = arg0[4] - arg0[8];
    }
    if (arg2 > arg0[5] - arg0[9]) {
        arg2 = arg0[5] - arg0[9];
    }
    if (arg1 < 0) {
        arg1 = 0;
    }
    if (arg2 < 0) {
        arg2 = 0;
    }
    arg0[0x18] = arg0[2];
    arg0[0x19] = arg0[3];
    arg0[0x1A] = arg1;
    arg0[0x1B] = arg2;
}

#define WP ((PanelPrims *)D_800897E8)
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

extern WindowStyle D_8006DD70[];
s32 func_800177E8(Unk80016F38 *);
void func_800176E4(Rect16 *a, Rect16 *b);
void GetDispEnv(DISPENV *);
void SetDrawArea(DR_AREA *, Rect16 *);
void func_80027DB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80028228(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_80017B88(Rect16 *, u8, s32, s32, s32, s32);

s32 func_800170F0(Unk80016F38 *w, void (*draw)(), s32 z) {
    DISPENV env;
    Rect16 r;
    Rect16 r2;
    Rect16 r3;
    Rect16 unused;
    s32 ret;
    s32 kind;
    s32 y;

    w->z = z;
    ret = func_800177E8(w);
    if (w->from.x >= 320) {
        return ret;
    }
    if (w->from.y >= 240) {
        return ret;
    }
    if (w->from.x + w->from.w <= 0) {
        return ret;
    }
    if (w->from.y + w->from.h <= 0) {
        return ret;
    }
    if ((w->from.w | w->from.h) == 0) {
        return ret;
    }
    {
        GetDispEnv(&env);
        r.x = w->from.x + env.disp[0] - 2;
        r.y = w->from.y + env.disp[1] - 1;
        r.w = w->from.w + 4;
        r.h = w->from.h + 2;
        func_800176E4(&r, (Rect16 *)&env);
        r2.x = w->unk0 + w->view.x + env.disp[0] - 2;
        r2.y = w->unk2 + w->view.y + env.disp[1] - 1;
        r2.w = w->rect.w + 4;
        r2.h = w->rect.h + 2;
        if (w->unk3F & 2) {
            r2.w -= 8;
        }
        if (w->unk3F & 4) {
            r2.h -= 8;
        }
        func_800176E4(&r2, &r);
        SetDrawArea((DR_AREA *)&WP->unk264[0x18], (Rect16 *)&env);
        addPrim(&D_800793A0->ot[z], &WP->unk264[0x18]);
        if (w->unk2C != 0 && (w->unk3F & 8) && (kind = D_8006DD70[(w->unk42 >> 4) - 1].label) != 0) {
            switch (kind) {
            case 1:
                r3.x = r.x;
                r3.y = r.y - 7;
                r3.w = r.w;
                r3.h = 5;
                func_80027DB8(w->from.x, w->from.y - 8, w->unk2C, w->unk39, z);
                break;
            case 2:
                r3.x = r.x - 7;
                r3.y = r.y;
                r3.w = 5;
                r3.h = r.h;
                y = w->from.y;
                func_80028228(w->from.x - 9, y + strlen((u8 *)w->unk2C) * 5, w->unk2C, w->unk39, z);
                break;
            case 3:
                r3.x = r.x - 6;
                r3.y = r.y;
                r3.w = 5;
                r3.h = r.h;
                y = w->from.y;
                func_80028228(w->from.x - 8, y + strlen((u8 *)w->unk2C) * 5, w->unk2C, w->unk39, z);
                break;
            case 4:
                r3.x = r.x;
                r3.y = r.y - 10;
                r3.w = r.w;
                r3.h = 5;
                func_80027DB8(w->from.x, w->from.y - 11, w->unk2C, w->unk39, z);
                break;
            }
            func_800176E4(&r3, (Rect16 *)&env);
            SetDrawArea((DR_AREA *)&WP->unk264[0x24], &r3);
            addPrim(&D_800793A0->ot[z], &WP->unk264[0x24]);
        }
        func_80018694(w, z);
        func_80018B8C(w, z);
        SetDrawArea((DR_AREA *)&WP->unk264[0], &r);
        addPrim(&D_800793A0->ot[z], &WP->unk264[0]);
        draw(w, &D_800793A0->ot[z]);
        SetDrawArea((DR_AREA *)&WP->unk264[0xC], &r2);
        addPrim(&D_800793A0->ot[z], &WP->unk264[0xC]);
        func_80017B88(&w->from, w->unk42, w->unk3F & 1, w->unk40, w->unk38, z);
    }
    return ret;
}

void func_800176E4(Rect16 *a, Rect16 *b) {
    if (a->x < b->x) {
        a->w -= b->x - a->x;
        a->x = b->x;
    }
    if (a->y < b->y) {
        a->h -= b->y - a->y;
        a->y = b->y;
    }
    if (a->x + a->w > b->x + b->w) {
        a->w = b->x + b->w - a->x;
    }
    if (a->y + a->h > b->y + b->h) {
        a->h = b->y + b->h - a->y;
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800177E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80017B88);

extern Rect16 D_8006DE48[];
extern Rect16 D_8006DE28[];
void func_8001EC3C(u8 *, u8, u8, u8, u8);
void func_8001EA64(void *, s16, s16, s16, s16);
#define WP ((PanelPrims *)D_800897E8)

void func_80018694(Unk80016F38 *w, s32 z) {
    u16 clut;
    s32 x;
    s32 y;
    s32 len;
    s32 pos;
    s32 knob;
    s32 i;
    s32 end;
    s32 off;
    s32 top;

    clut = getClut(D_800897F2 + (w->unk38 % 2) * 16, D_800897F4 + w->unk38 / 2);
    if (!(w->unk3F & 2) || w->rect.h >= w->view.h) {
        return;
    }
    x = w->unk0 + w->view.x + w->rect.w - 8;
    y = w->unk2 + w->view.y;
    len = w->rect.h - 0x10;
    if (w->unk3F & 4) {
        len -= 8;
    }
    pos = w->view.y * len / w->view.h;
    knob = w->rect.h * len - 1;
    knob = (knob + w->view.h) / w->view.h;
    if (pos + knob < 0) {
        pos = 0;
    }
    if (pos + knob > len) {
        pos = len - knob;
    }
    WP->lineb[0].x0 = x;
    WP->lineb[0].y0 = y;
    WP->lineb[1].x0 = x;
    end = len + 8;
    WP->lineb[1].y0 = y + end;
    WP->lineb[2].x0 = x;
    off = pos + 8;
    top = y + off;
    WP->lineb[2].y0 = top;
    WP->lineb[3].x0 = x;
    WP->lineb[3].y0 = top + knob - 2;
    func_8001EA64(&WP->ft4b[0], x, top + 2, 8, knob - 4);
    func_8001EA64(&WP->ft4b[1], x, y + 8, 8, len);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->lineb[i], D_8006DE48[i + w->unk43 * 4].x, D_8006DE48[i + w->unk43 * 4].y);
        setWH(&WP->lineb[i], D_8006DE48[i + w->unk43 * 4].w, D_8006DE48[i + w->unk43 * 4].h);
        setRGB0(&WP->lineb[i], w->unk40, w->unk40, w->unk40);
        WP->lineb[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->lineb[i]);
    }
    for (i = 0; i < 2; i++) {
        func_8001EC3C((u8 *)&WP->ft4b[i], D_8006DE28[i + w->unk43 * 2].x, D_8006DE28[i + w->unk43 * 2].y, D_8006DE28[i + w->unk43 * 2].w, D_8006DE28[i + w->unk43 * 2].h);
        setRGB0(&WP->ft4b[i], w->unk40, w->unk40, w->unk40);
        WP->ft4b[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->ft4b[i]);
    }
}

extern Rect16 D_8006DEA8[];
extern Rect16 D_8006DE88[];
void func_8001EC3C(u8 *, u8, u8, u8, u8);
void func_8001EA64(void *, s16, s16, s16, s16);

void func_80018B8C(Unk80016F38 *w, s32 z) {
    u16 clut;
    s32 x;
    s32 y;
    s32 len;
    s32 pos;
    s32 knob;
    s32 i;
    s32 end;
    s32 off;
    s32 left;

    clut = getClut(D_800897F2 + (w->unk38 % 2) * 16, D_800897F4 + w->unk38 / 2);
    if (!(w->unk3F & 4) || w->rect.w >= w->view.w) {
        return;
    }
    x = w->unk0 + w->view.x;
    y = w->unk2 + w->view.y + w->rect.h - 8;
    len = w->rect.w - 0x10;
    if (w->unk3F & 2) {
        len -= 8;
    }
    pos = w->view.x * len / w->view.w;
    knob = w->rect.w * len - 1;
    knob = (knob + w->view.w) / w->view.w;
    if (pos + knob < 0) {
        pos = 0;
    }
    if (pos + knob > len) {
        pos = len - knob;
    }
    WP->linec[0].x0 = x;
    WP->linec[0].y0 = y;
    end = len + 8;
    WP->linec[1].x0 = x + end;
    WP->linec[1].y0 = y;
    off = pos + 8;
    left = x + off;
    WP->linec[2].x0 = left;
    WP->linec[2].y0 = y;
    WP->linec[3].x0 = left + knob - 2;
    WP->linec[3].y0 = y;
    func_8001EA64(&WP->ft4c[0], left + 2, y, knob - 4, 8);
    func_8001EA64(&WP->ft4c[1], x + 8, y, len, 8);
    for (i = 0; i < 4; i++) {
        setUV0(&WP->linec[i], D_8006DEA8[i + w->unk43 * 4].x, D_8006DEA8[i + w->unk43 * 4].y);
        setWH(&WP->linec[i], D_8006DEA8[i + w->unk43 * 4].w, D_8006DEA8[i + w->unk43 * 4].h);
        setRGB0(&WP->linec[i], w->unk40, w->unk40, w->unk40);
        WP->linec[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->linec[i]);
    }
    for (i = 0; i < 2; i++) {
        func_8001EC3C((u8 *)&WP->ft4c[i], D_8006DE88[i + w->unk43 * 2].x, D_8006DE88[i + w->unk43 * 2].y, D_8006DE88[i + w->unk43 * 2].w, D_8006DE88[i + w->unk43 * 2].h);
        setRGB0(&WP->ft4c[i], w->unk40, w->unk40, w->unk40);
        WP->ft4c[i].clut = clut;
        addPrim(&D_800793A0->ot[z], &WP->ft4c[i]);
    }
}

extern char D_80010008[];
extern int printf(const char *, ...);

int func_80019084(void) {
    if (D_800897E8 == D_800793A0->unk40BC + D_800897EC * 0x294) {
        printf(D_80010008);
        return -1;
    }
    return 0;
}

u32 GetTPage(s32, s32, s32, s32);
void func_8001E6EC(s32, void *, s32, s32);
void func_800191C0(Unk800190F4 *, Rect16 *, Bytes4 *);

void func_800190F4(Unk800190F4 *p, Rect16 *arg1, Bytes4 *arg2) {
    s32 i;

    for (i = 0; i < 2; i++) {
        setDrawMode(&p->dm[i], 0, 0, GetTPage(0, 1, 0, 0));
        func_8001E6EC(0x11, &p->prim[i], 1, 0);
    }
    func_800191C0(p, arg1, arg2);
}

void func_800191C0(Unk800190F4 *p, Rect16 *r, Bytes4 *c) {
    if (r == (Rect16 *)-1) {
        p->unk30.x = 0;
        p->unk30.y = 0;
        p->unk30.w = 0;
        p->unk30.h = 0;
    } else {
        p->unk30 = *r;
    }
    p->unk38 = p->unk30;
    p->unk40 = p->unk30;
    if (c == (Bytes4 *)-1) {
        p->unk48.b[0] = 0;
        p->unk48.b[1] = 0;
        p->unk48.b[2] = 0x80;
    } else {
        p->unk48 = *c;
    }
    p->unk4C = 0x80;
    p->unk4D = 0;
}

void func_80019280(Unk800190F4 *p, Rect16 *r) {
    if (p->unk4D >= 6) {
        p->unk4D = 0;
    }
    p->unk30 = p->unk40;
    p->unk38 = *r;
}

void func_800192E0(void *arg0, Bytes4 *arg1) {
    *(Bytes4 *)((s8 *)arg0 + 0x48) = *arg1;
}

extern u8 D_800794F4;

void func_800192FC(Unk800190F4 *p, s32 ot) {
    if (p->unk4D < 6) {
        p->unk40.x = p->unk30.x + (p->unk38.x - p->unk30.x) * p->unk4D / 6;
        p->unk40.y = p->unk30.y + (p->unk38.y - p->unk30.y) * p->unk4D / 6;
        p->unk40.w = p->unk30.w + (p->unk38.w - p->unk30.w) * p->unk4D / 6;
        p->unk40.h = p->unk30.h + (p->unk38.h - p->unk30.h) * p->unk4D / 6;
        p->unk4D++;
    } else {
        p->unk40 = p->unk38;
    }
    (p->prim + D_800794F4)->x0 = p->unk40.x - 2;
    (p->prim + D_800794F4)->y0 = p->unk40.y - 1;
    (p->prim + D_800794F4)->w = p->unk40.w + 4;
    (p->prim + D_800794F4)->h = p->unk40.h + 2;
    (p->prim + D_800794F4)->r0 = p->unk48.b[0] * p->unk4C / 128;
    (p->prim + D_800794F4)->g0 = p->unk48.b[1] * p->unk4C / 128;
    (p->prim + D_800794F4)->b0 = p->unk48.b[2] * p->unk4C / 128;
    addPrim(&D_800793A0->ot[ot], &p->prim[D_800794F4]);
    addPrim(&D_800793A0->ot[ot], &p->dm[D_800794F4]);
}

void func_80016C08(void *, void *, s32, s16 *, s32, s32, s32, s32);

void func_8001963C(void *arg0, void *arg1, Unk800190F4 *arg2, Bytes4 *arg3) {
    s16 r[4];

    (*(void **)((s8 *)arg0 + 0)) = arg1;
    (*(Unk800190F4 **)((s8 *)arg0 + 4)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x12)) = -1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = -1;
    (*(s8 *)((s8 *)arg0 + 0x26)) = 1;
    (*(s8 *)((s8 *)arg0 + 0x27)) = 0;
    r[0] = 0;
    r[1] = (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25)) - ((*(s16 *)((s8 *)arg0 + 0xE)) - (*(u8 *)((s8 *)arg0 + 0x25))) / 2;
    r[2] = (*(u8 *)((s8 *)arg0 + 0x24)) * (*(s16 *)((s8 *)arg0 + 0x1E));
    r[3] = (*(u8 *)((s8 *)arg0 + 0x25)) * (*(s16 *)((s8 *)arg0 + 0x20));
    func_80016C08(arg1, (s8 *)arg0 + 8, -1, r, (*(u8 *)((s8 *)arg0 + 0x18)), (*(u8 *)((s8 *)arg0 + 0x19)), 0x80, 0xC);
    r[0] = (*(u8 *)((s8 *)arg0 + 0x22)) + (*(u16 *)((s8 *)arg1 + 0));
    r[1] = (*(u8 *)((s8 *)arg0 + 0x23)) + (*(u16 *)((s8 *)arg1 + 2)) + (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25));
    r[2] = (*(u16 *)((s8 *)arg0 + 0x1A));
    r[3] = (*(u16 *)((s8 *)arg0 + 0x1C));
    func_800190F4(arg2, (Rect16 *)r, arg3);
}

void func_8001705C(s16 *, s32, s32);

void func_800197AC(void *arg0) {
    s16 r[4];
    void *img;

    img = (*(void **)((s8 *)arg0 + 0));
    func_8001705C(img, 0, (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25)) - ((*(s16 *)((s8 *)arg0 + 0xE)) - (*(u8 *)((s8 *)arg0 + 0x25))) / 2);
    r[0] = ((*(u16 *)((s8 *)img + 0xC)) - (*(u16 *)((s8 *)img + 0x34))) + (*(u8 *)((s8 *)arg0 + 0x22)) + (*(s16 *)((s8 *)arg0 + 0x10)) * (*(u8 *)((s8 *)arg0 + 0x24));
    r[1] = ((*(u16 *)((s8 *)img + 0xE)) - (*(u16 *)((s8 *)img + 0x36))) + (*(u8 *)((s8 *)arg0 + 0x23)) + (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25));
    r[2] = (*(u16 *)((s8 *)arg0 + 0x1A));
    r[3] = (*(u16 *)((s8 *)arg0 + 0x1C));
    func_80019280(*(Unk800190F4 **)((s8 *)arg0 + 4), (Rect16 *)r);
}

extern PadState *D_80089840[];
void func_800192FC(Unk800190F4 *, s32);

s32 func_800198A8(Menu *m) {
    Unk80016F38 *w;
    Unk800190F4 *c;
    s16 r[4];

    w = m->win;
    c = m->cursor;
    m->moved = 0;
    if (m->active != 0) {
        c->unk4C = 0x80;
        if (m->nrows >= 2 && m->rowH != 0) {
            if (D_80089840[m->pad]->unkE & 0x1000) {
                func_8002BB58(2);
                m->moved = 1;
                if (--m->row < 0) {
                    func_8001705C((s16 *)w, 0, w->view.h - w->rect.h);
                    m->row = m->nrows - 1;
                } else {
                if (m->row == 0) {
                    D_80089840[m->pad]->unk10 = 0;
                }
                if (m->row * m->rowH < w->unk30[3]) {
                    func_8001705C((s16 *)w, 0, m->row * m->rowH);
                }
                }
            } else if (D_80089840[m->pad]->unkE & 0x4000) {
                func_8002BB58(2);
                m->moved = 1;
                if (++m->row >= m->nrows) {
                    func_8001705C((s16 *)w, 0, 0);
                    m->row = 0;
                } else {
                if (m->row == m->nrows - 1) {
                    D_80089840[m->pad]->unk10 = 0;
                }
                if (m->row * m->rowH >= w->unk30[3] + w->rect.h) {
                    func_8001705C((s16 *)w, 0, (m->row + 1) * m->rowH - w->rect.h);
                }
                }
            } else if (D_80089840[m->pad]->unkE & 0x1) {
                if (m->row != 0) {
                    func_8002BB58(2);
                }
                m->moved = 1;
                m->row -= (w->rect.h + m->rowH - 1) / m->rowH;
                if (m->row < 0) {
                    D_80089840[m->pad]->unk10 = 0;
                    func_8001705C((s16 *)w, 0, 0);
                    m->row = 0;
                } else {
                    if (m->row == 0) {
                        D_80089840[m->pad]->unk10 = 0;
                    }
                    func_8001705C((s16 *)w, 0, w->unk30[3] - (w->rect.h + m->rowH - 1) / m->rowH * m->rowH);
                }
            } else if (D_80089840[m->pad]->unkE & 0x2) {
                if (m->row != m->nrows - 1) {
                    func_8002BB58(2);
                }
                m->moved = 1;
                m->row += (w->rect.h + m->rowH - 1) / m->rowH;
                if (m->row >= m->nrows) {
                    D_80089840[m->pad]->unk10 = 0;
                    func_8001705C((s16 *)w, 0, w->view.h - w->rect.h);
                    m->row = m->nrows - 1;
                } else {
                    if (m->row == m->nrows - 1) {
                        D_80089840[m->pad]->unk10 = 0;
                    }
                    func_8001705C((s16 *)w, 0, w->unk30[3] + (w->rect.h + m->rowH - 1) / m->rowH * m->rowH);
                }
            }
        }
    } else {
        c->unk4C = 0x40;
    }
    if (m->row != m->prevRow || m->col != m->prevCol) {
        m->prevCol = m->col;
        m->prevRow = m->row;
        r[0] = (w->rect.x - w->unk30[2]) + m->ox + m->col * m->colW;
        r[1] = (w->rect.y - w->unk30[3]) + m->oy + m->row * m->rowH;
        r[2] = m->cw;
        r[3] = m->ch;
        func_80019280(m->cursor, (Rect16 *)r);
    }
    func_800192FC(c, w->z);
    return m->col + m->row * m->ncols;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010000);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010008);

extern s32 D_801D6B18;
extern s32 D_801D6B1C;
s32 func_800293FC(u8 *);
s32 strlen(u8 *);
void func_80016C08(void *, void *, s32, s16 *, s32, s32, s32, s32);

void func_80019EA4(u8 *w, u8 *text, u32 flags) {
    Rect16 r;
    s32 m;
    s32 wd;
    s32 x;
    s32 c;

    w[0xA4] = flags & 0xF;
    w[0xB6] = flags & 0x80;
    *(u8 **)(w + 0x94) = text;
    func_800293FC(text);
    w[0xA7] = (D_801D6B18 + 1) / 2;
    *(s16 *)(w + 0xA8) = (D_801D6B18 + 1) / 2 * 2 + 4;
    *(s16 *)(w + 0xAA) = (D_801D6B1C + 1) / 2 * 2 + 4;
    if (w[0xA4] != 0) {
        if (text == 0) {
            *(s16 *)(w + 0xAA) = 0x10;
        } else {
            *(s16 *)(w + 0xAA) += 0x10;
        }
    }
    if (w[0xA4] != 2) {
        *(char **)(w + 0x98) = "Yes";
        *(char **)(w + 0x9C) = "No";
    }
    *(s16 *)(w + 0xAE) = strlen(*(u8 **)(w + 0x98)) * 6;
    *(s16 *)(w + 0xB2) = strlen(*(u8 **)(w + 0x9C)) * 6;
    m = *(s16 *)(w + 0xAE);
    if (m < *(s16 *)(w + 0xB2)) {
        m = *(s16 *)(w + 0xB2);
    }
    m = m * 2 + 0x10;
    if (*(s16 *)(w + 0xA8) < m) {
        *(s16 *)(w + 0xA8) = m;
    }
    wd = *(s16 *)(w + 0xA8);
    x = (320 - wd) / 2;
    c = x + wd / 2;
    *(s16 *)(w + 0xAC) = c - (*(s16 *)(w + 0xAE) + 4);
    *(s16 *)(w + 0xB0) = c + 4;
    r.x = x;
    r.y = (240 - *(s16 *)(w + 0xAA)) / 2;
    r.w = *(s16 *)(w + 0xA8);
    r.h = *(s16 *)(w + 0xAA);
    func_80016C08(w, &r, -1, (s16 *)-1, 8, 0x77, 0x80, 8);
    w[0x38] = 4;
    if (w[0xA4] != 0) {
        func_800190F4((Unk800190F4 *)(w + 0x44), (Rect16 *)-1, (Bytes4 *)-1);
    }
    w[0xA5] = 2;
    w[0xA6] = 0;
    *(s32 *)(w + 0xA0) = 0;
    w[0xB5] = 0;
    w[0xB4] = 0;
}

s32 func_800149B8();
void func_8001A1D8();

s32 func_80014C08(s32);
s8 func_8001A100(void *arg0) {
    func_800149B8(0, -1, 0, 0x400, &func_8001A1D8, arg0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (*(s8 *)((s8 *)arg0 + 0xA5));
}

s32 func_8001A164(s32 *arg0, s32 arg1) {
    s32 temp_v0;

    temp_v0 = func_800148B0();
    (*(s8 *)((s8 *)arg0 + 0xA6)) = (s8) arg1;
    func_800149B8(0, -1, 0, 0x400, func_8001A1D8, arg0, temp_v0, 0, 0);
    func_80014C08(0x7FFFFFFF);
    return (s32) (*(s8 *)((s8 *)arg0 + 0xA5));
}

void func_8001A40C(u8 *w);

void func_8001A1D8(u8 *w, s32 arg1) {
    Rect16 r;
    s32 x;
    s32 width;
    u16 mask;
    PadState **pads;
    s32 on;

    D_8008983C = 0;
    if ((s8)w[0xA5] == 1) {
        x = *(s16 *)(w + 0xAC);
        width = *(s16 *)(w + 0xAE);
    } else {
        x = *(s16 *)(w + 0xB0);
        width = *(s16 *)(w + 0xB2);
    }
    r.x = x;
    r.y = (240 - *(s16 *)(w + 0xAA)) / 2 + *(s16 *)(w + 0xAA) - 14;
    r.w = width;
    r.h = 12;
    func_800191C0((Unk800190F4 *)(w + 0x44), &r, (Bytes4 *)-1);
    pads = D_80089840;
    on = 1;
    do {
        func_80014C08(D_800794F0);
        func_800170F0((Unk80016F38 *)w, func_8001A40C, 0);
        if (*(void (**)(void))(w + 0xA0) != 0) {
            (*(void (**)(void))(w + 0xA0))();
        }
        if (w[0xA4] != 0) {
            if (w[0xB5] != 0) {
                mask = 0x40;
            } else {
                mask = 0x50;
            }
        } else {
            mask = 0x40;
        }
        if (pads[w[0xA6]]->unk2 & mask) {
            break;
        }
    } while (w[0xB4] == 0);
    if (w[0xB4] != 0) {
        w[0xA5] = 3;
    } else if (pads[w[0xA6]]->unk2 & 0x10) {
        w[0xA5] = 0;
        w[0xB4] = on;
        func_8002BB58(0);
    } else {
        w[0xB4] = on;
        func_8002BB58(1);
    }
    func_80016F38((Unk80016F38 *)w, (Rect16 *)-1);
    do {
        func_80014C08(D_800794F0);
        func_800170F0((Unk80016F38 *)w, func_8001A40C, 0);
        if (*(void (**)(void))(w + 0xA0) != 0) {
            (*(void (**)(void))(w + 0xA0))();
        }
    } while (*(s8 *)(w + 0x41) == 0);
    D_8008983C = 1;
    func_80014A48(arg1, (s8)w[0xA5]);
    func_80014A90();
}

void func_80028D18(s32, s32, s32, s32, s32);
void func_8002BB58(u32);
void func_800192FC(Unk800190F4 *, s32);
extern PadState *D_80089840[];

void func_8001A40C(u8 *w) {
    Rect16 r;
    s32 x;
    s32 y;

    x = *(s16 *)w + (*(s16 *)(w + 0xA8) - w[0xA7] * 2) / 2;
    y = *(s16 *)(w + 2) + 2;
    if (*(s32 *)(w + 0x94) != 0) {
        func_80028D18(x, y, *(s32 *)(w + 0x94), 7, *(s16 *)(w + 0x3A));
    }
    y = *(s16 *)(w + 2) + *(s16 *)(w + 0xAA) - 0xE;
    if (w[0xA4] != 0) {
        if (w[0xB4] == 0) {
            if ((D_80089840[w[0xA6]]->unk2 & 0x8000) && (s8)w[0xA5] != 1) {
                w[0xA5] = 1;
                r.x = *(s16 *)(w + 0xAC);
                r.y = y;
                r.w = *(s16 *)(w + 0xAE);
                r.h = 0xC;
                func_80019280((Unk800190F4 *)(w + 0x44), &r);
                func_8002BB58(2);
            }
            if ((D_80089840[w[0xA6]]->unk2 & 0x2000) && (s8)w[0xA5] != 2) {
                w[0xA5] = 2;
                r.x = *(s16 *)(w + 0xB0);
                r.y = y;
                r.w = *(s16 *)(w + 0xB2);
                r.h = 0xC;
                func_80019280((Unk800190F4 *)(w + 0x44), &r);
                func_8002BB58(2);
            }
        }
        func_80028D18(*(s16 *)(w + 0xAC), y, *(s32 *)(w + 0x98), 7, *(s16 *)(w + 0x3A));
        func_80028D18(*(s16 *)(w + 0xB0), y, *(s32 *)(w + 0x9C), 7, *(s16 *)(w + 0x3A));
        func_800192FC((Unk800190F4 *)(w + 0x44), *(s16 *)(w + 0x3A));
    }
}

void func_8001A6B0(void);

void *func_8001ACEC(s32);
s32 PadInitDirect(void *, void *);
s32 PadStartCom(void);
extern s32 D_800897F8;
extern s32 D_8008983C;
void func_8001A600(void) {
    s32 i;
    u8 *buf;

    D_8008983C = 1;
    buf = func_8001ACEC(0x3C);
    for (i = 0; i < 2; i++) {
        D_80089840[i] = (PadState *)(buf + i * 0x1E);
    }
    func_8001A6B0();
    PadInitDirect(&D_800897F8, (s8 *)&D_800897F8 + 0x22);
    PadStartCom();
}


void func_8001A688(s32 arg0, s16 arg1, s16 arg2) {
    D_80089840[arg0]->repeatDelay = arg1;
    D_80089840[arg0]->repeatRate = arg2;
}

void func_8001A6B0(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        D_80089840[i]->unk10 = 1;
        D_80089840[i]->unk11 = 0;
        D_80089840[i]->unk12 = 0;
        D_80089840[i]->unk1A = 0;
        D_80089840[i]->unk1B = 0;
        D_80089840[i]->unk0 = 0;
        D_80089840[i]->unk6 = 0;
        D_80089840[i]->unk4 = 0;
        D_80089840[i]->unk2 = 0;
        D_80089840[i]->unk8 = 0;
        D_80089840[i]->unkE = 0;
        D_80089840[i]->unkC = 0;
        D_80089840[i]->unkA = 0;
        func_8001A688(i, 0x1E, 2);
    }
}

s32 PadGetState(s32);
s32 PadInfoMode(s32, s32, s32);

s32 func_8001A7A4(s32 port, PadState *pad, u8 *buf) {
    s32 skip;
    s32 changed;
    u16 pressed;
    s32 cur;
    s16 t;

    if (buf[1] == 0x80) {
        pad->unk0 = 0;
        pad->unk6 = 0;
        pad->unk4 = 0;
        pad->unk2 = 0;
        return 0;
    }
    pad->unk1A = PadGetState(port);
    pad->unk1B = PadInfoMode(port, 1, 0);
    pad->unk1C = PadInfoMode(port, 2, 0);
    if (pad->unk1A == 0 || buf[0] != 0) {
        pad->unk0 = 0;
        pad->unk6 = 0;
        pad->unk4 = 0;
        pad->unk2 = 0;
        return 0;
    }
    skip = 0;
    switch (pad->unk1B) {
    case 1:
    case 3:
    case 6:
        skip = 1;
        break;
    case 2:
    case 4:
    case 5:
    case 7:
        break;
    }
    if (skip) {
        return 0;
    }
    changed = pad->unk0;
    pad->unk0 = ~((buf[2] << 8) | buf[3]);
    changed ^= pad->unk0;
    pressed = changed & pad->unk0;
    pad->unk2 = pressed;
    pad->unk4 = changed & ~pad->unk0;
    pad->unk6 = pressed;
    if (pad->unk10) {
        cur = pad->unk0;
        if (cur == pad->unk14 && cur != 0) {
            t = pad->unk12;
            pad->unk12 = t + ((Unk800794F8 *)&D_800794F8)->unk50;
            if (pad->unk11 == 0) {
                if (pad->unk12 < pad->repeatDelay) {
                    return 0;
                }
                if (t != 0) {
                    pad->unk11 = 1;
                    pad->unk12 = 0;
                    pad->unk6 = pressed | cur;
                }
            } else {
                if (pad->unk12 < pad->repeatRate) {
                    return 0;
                }
                pad->unk12 = 0;
                pad->unk6 |= cur;
            }
        } else {
            pad->unk14 = cur;
            pad->unk11 = 0;
            pad->unk12 = 0;
        }
    } else if (pad->unk4) {
        pad->unk10 = 1;
    }
    return 0;
}


s32 func_8001A7A4(s32, PadState *, u8 *);
extern s32 D_800897F8;
extern s32 D_8008983C;

void func_8001A9B0(void) {
    s32 var_s1;
    void *temp_s0;

    var_s1 = 0;
    do {
        temp_s0 = D_80089840[var_s1];
        func_8001A7A4(var_s1 * 0x10, temp_s0, (u8 *)&D_800897F8 + var_s1 * 0x22);
        if (D_8008983C != 0) {
            (*(u16 *)((s8 *)temp_s0 + 8)) = (u16) (*(u16 *)((s8 *)temp_s0 + 0));
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = (u16) (*(u16 *)((s8 *)temp_s0 + 6));
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = (u16) (*(u16 *)((s8 *)temp_s0 + 4));
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = (u16) (*(u16 *)((s8 *)temp_s0 + 2));
        } else {
            (*(u16 *)((s8 *)temp_s0 + 8)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = 0U;
        }
        var_s1 += 1;
    } while (var_s1 < 2);
}

extern s32 D_8008C848;

s32 func_8001AE90(void *);
extern s32 D_80089848;
void func_8001AA80(s32 arg0) {
    s32 *p;
    s32 i;

    if (arg0 != 0) {
        p = &D_80089848;
        p[0] = (s32)&D_8008C848 & 0x3FFFFFFF;
        p[1] = 0x148000;
        p[2] = -1;
        i = 0x3FF;
        do {
            p += 3;
            p[0] = 0;
            p[1] = 0;
            i--;
            p[2] = 0;
        } while (i > 0);
        return;
    }
    p = &D_80089848;
    i = 0x3FF;
    if (p[0] != 0) {
loop:
        if (p[0] < 0 && p[2] >= 0) {
            if (func_8001AE90((void *)p[0]) == 0) {
                goto loop;
            }
        }
        i--;
        p += 3;
        if (i >= 0 && p[0] != 0) {
            goto loop;
        }
    }
}

s32 func_8001AB64(void) {
    s32 *p;
    s32 i;
    s32 max;

    max = 0;
    p = &D_80089848;
    i = 0x3FF;
    if (D_80089848 != 0) {
        do {
            if (p[0] > 0 && max < p[1]) {
                max = p[1];
            }
            i--;
            p += 3;
        } while (i >= 0 && p[0] != 0);
    }
    return max;
}

void func_80014970(void);
void func_800149A0(void);

void *func_8001ABCC(s32 size, s32 tag) {
    s32 *p;
    s32 *q;
    s32 i;
    s32 addr;
    s32 avail;
    s32 res;

    size = (size + 3) & ~3;
    if (size == 0) {
        return 0;
    }
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr >= 0) {
                avail = p[1];
                if (avail >= size) {
                    res = addr | 0x80000000;
                    p[0] = res;
                    p[1] = size;
                    avail -= size;
                    p[2] = tag;
                    if (avail != 0) {
                        addr += size;
                        q = &D_80089848 + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            q[0] = q[-3];
                            q[1] = q[-2];
                            q[2] = q[-1];
                            q -= 3;
                        }
                        q[0] = addr;
                        q[1] = avail;
                        q[2] = -1;
                    }
                    func_800149A0();
                    return (void *)res;
                }
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

void *func_8001ACEC(s32 arg0) {
    return func_8001ABCC(arg0, -2);
}

s32 func_800148B0();
void *func_8001ABCC(s32, s32);

void *func_8001AD0C(s32 size) {
    return func_8001ABCC(size, func_800148B0());
}

void *func_8001AD3C(void *ptr, s32 size) {
    s32 *p;
    s32 i;
    s32 addr;
    s32 rest;

    size = (size + 3) & ~3;
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr == (s32)ptr) {
                rest = p[1] - size;
                if (rest < 0) {
                    func_800149A0();
                    return 0;
                }
                if (rest != 0 && i != 0) {
                    p[1] = size;
                    p += 3;
                    addr += size;
                    if (p[0] > 0) {
                        rest += p[1];
                    } else {
                        p = &D_80089848 + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            p[0] = p[-3];
                            p[1] = p[-2];
                            p[2] = p[-1];
                            p -= 3;
                        }
                    }
                    p[0] = addr & 0x3FFFFFFF;
                    p[1] = rest;
                    p[2] = -1;
                }
                func_800149A0();
                return ptr;
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

s32 func_8001AE90(void *);

void func_8001AE70(void *arg0) {
    func_8001AE90(arg0);
}

s32 func_8001AE90(void *ptr) {
    s32 *p;
    s32 *q;
    s32 i;
    s32 addr;
    s32 size;

    if (ptr == 0) {
        return 0;
    }
    func_80014970();
    p = &D_80089848;
    i = 0x3FF;
    if ((addr = D_80089848) != 0) {
        do {
            if (addr == (s32)ptr) {
                addr &= 0x3FFFFFFF;
                size = p[1];
                q = p;
                if (p != &D_80089848 && p[-3] > 0) {
                    p -= 3;
                    addr = p[0];
                    size += p[1];
                    i++;
                }
                if (i > 0 && q[3] > 0) {
                    if (q != p) {
                        i--;
                    }
                    q += 3;
                    size += q[1];
                }
                p[0] = addr;
                p[1] = size;
                p[2] = -1;
                if (p != q) {
                    for (i--; i > 0; i--) {
                        p += 3;
                        q += 3;
                        p[0] = q[0];
                        p[1] = q[1];
                        p[2] = q[2];
                    }
                    while (p < q) {
                        p += 3;
                        p[0] = 0;
                        p[2] = 0;
                    }
                }
                func_800149A0();
                return 0;
            }
            i--;
            p += 3;
        } while (i >= 0 && (addr = p[0]) != 0);
    }
    func_800149A0();
    return 0;
}

extern s32 D_80089848;

s32 func_8001AFF0(s32 arg0) {
    s32 *var_s0;
    s32 temp_a0;
    s32 var_s1;

    var_s0 = &D_80089848;
    var_s1 = 0x3FF;
    if (D_80089848 != 0) {
loop_1:
        temp_a0 = (*(s32 *)((s8 *)var_s0 + 0));
        if ((temp_a0 < 0) && ((*(s32 *)((s8 *)var_s0 + 8)) == arg0)) {
            if (func_8001AE90(temp_a0) == 0) {
                goto loop_1;
            }
        }
        var_s1 -= 1;
        var_s0 += 3;
        if ((var_s1 >= 0) && (*var_s0 != 0)) {
            goto loop_1;
        }
    }
    return 0;
}

long catan(long);

s32 func_8001B088(s32 y, s32 x) {
    s32 a;

    if (x == 0) {
        if (y > 0) {
            return 0x400;
        }
        if (y < 0) {
            return -0x400;
        }
        return 0;
    }
    a = catan((y << 12) / x);
    if (x < 0) {
        if (y <= 0) {
            a -= 0x800;
        } else {
            a += 0x800;
        }
    }
    return a;
}

s32 func_80014A48();
void func_8001B10C(s32 arg0, s32 arg1) {
    func_80014A48(arg1, func_80015848(arg0) == 0 ? 1 : -1);
}

extern s32 D_8006DEF0;
extern s32 D_801D4848;

s32 func_8001B144(s32 name, s32 arg1) {
    s32 size;
    CdFile *f;
    s32 buf;

    size = 0;
    while (D_8006DEF0 != 0) {
        func_80014C08(D_800794F0);
    }
    D_8006DEF0 = 1;
    f = func_80015AD8(name, 1);
    buf = 0;
    if (f != 0) {
        size = f->size;
        buf = (s32)func_8001ABCC(size, arg1);
        if (buf == 0) {
            func_80015EAC(f);
        } else {
            func_80015F34(f, size, (u8 *)buf);
            func_80015EAC(f);
        }
    }
    D_801D4848 = size;
    func_80014A48(arg1, buf);
    D_8006DEF0 = 0;
    return buf;
}

s32 func_8001B248(s32 *name, s32 arg1, s32 arg2) {
    s32 size;
    CdFile *f;
    s32 buf;

    size = 0;
    while (D_8006DEF0 != 0) {
        func_80014C08(D_800794F0);
    }
    D_8006DEF0 = 1;
    f = func_80015AD8((s32)name, 1);
    buf = 0;
    if (f != 0) {
        size = f->size;
        buf = (s32)func_8001ABCC(size, arg2);
        if (buf == 0) {
            func_80015EAC(f);
        } else {
            func_80015F34(f, size, (u8 *)buf);
            func_80015EAC(f);
        }
    }
    D_801D4848 = size;
    func_80014A48(arg1, buf);
    D_8006DEF0 = 0;
    return buf;
}

s32 func_80014A48();
extern s32 D_800794F0;
void func_8001B358(s32 arg0, s32 *arg1, s32 arg2) {
    s32 temp_v0;
    s32 var_s2;

    var_s2 = 0;
    if (D_8006DEF0 != 0) {
        do {
            func_80014C08(D_800794F0);
        } while (D_8006DEF0 != 0);
    }
    D_8006DEF0 = 1;
    temp_v0 = func_80015AD8(arg0, 1);
    if (temp_v0 != 0) {
        var_s2 = (*(s32 *)((s8 *)temp_v0 + 0x24));
        func_80015F34(temp_v0, var_s2, arg1);
        func_80015EAC((s32 *) temp_v0);
    }
    D_801D4848 = var_s2;
    func_80014A48(arg2);
    D_8006DEF0 = 0;
}

extern TIM_IMAGE D_801D4850;
s32 OpenTIM(u32 *);
TIM_IMAGE *ReadTIM(TIM_IMAGE *);

void func_8001B438(u32 *tim, s16 px, s16 py, s16 cx, s16 cy) {
    Rect16 r;

    OpenTIM(tim);
    ReadTIM(&D_801D4850);
    if (px == -1) {
        px = D_801D4850.prect->x;
        py = D_801D4850.prect->y;
    } else {
        D_801D4850.prect->x = px;
        D_801D4850.prect->y = py;
    }
    if (cx == -1) {
        cx = D_801D4850.crect->x;
        cy = D_801D4850.crect->y;
    } else if (cx != -2) {
        D_801D4850.crect->x = cx;
        D_801D4850.crect->y = cy;
    }
    r.x = px;
    r.y = py;
    r.w = D_801D4850.prect->w;
    r.h = D_801D4850.prect->h;
    LoadImage((s16 *)&r, (s32)D_801D4850.paddr);
    if ((D_801D4850.mode & 8) && cx != -2) {
        r.x = cx;
        r.y = cy;
        r.w = D_801D4850.crect->w;
        r.h = D_801D4850.crect->h;
        LoadImage((s16 *)&r, (s32)D_801D4850.caddr);
    }
}

void func_8001B5BC(u32 *addr) {
    TIM_IMAGE img;

    OpenTIM(addr);
    while (ReadTIM(&img) != 0) {
        if (img.caddr != 0) {
            LoadImage((s16 *)img.crect, (s32)img.caddr);
        }
        if (img.paddr != 0) {
            LoadImage((s16 *)img.prect, (s32)img.paddr);
        }
    }
    DrawSync(0);
}

void func_8001B634(u32 *addr, s32 dx, s32 dy) {
    TIM_IMAGE img;
    Rect16 r;

    OpenTIM(addr);
    while (ReadTIM(&img) != 0) {
        if (img.caddr != 0) {
            r.w = img.crect->w;
            r.h = img.crect->h;
            r.x = img.crect->x + dx;
            r.y = img.crect->y + dy;
            LoadImage((s16 *)&r, (s32)img.caddr);
        }
        if (img.paddr != 0) {
            r.w = img.prect->w;
            r.h = img.prect->h;
            r.x = img.prect->x + dx;
            r.y = img.prect->y + dy;
            LoadImage((s16 *)&r, (s32)img.paddr);
        }
    }
    DrawSync(0);
}

void func_8001B734(u32 *p) {
    u32 *top;
    u32 *b;
    s32 n;

    n = *p++;
    top = p;
    if ((n & 0xFFFF) == 0x7054) {
        n >>= 16;
        do {
            b = top + p[n - 1];
            if (*b++ & 8) {
                LoadImage((s16 *)(b + 1), (s32)(b + 3));
                b += *b >> 2;
            }
            LoadImage((s16 *)(b + 1), (s32)(b + 3));
            DrawSync(0);
        } while (--n > 0);
    }
}

void func_8001B7F4(u32 *p, s32 dx, s32 dy) {
    u32 *top;
    u32 *b;
    s32 n;

    n = *p++;
    top = p;
    if ((n & 0xFFFF) == 0x7054) {
        n >>= 16;
        do {
            b = top + p[n - 1];
            if (*b++ & 8) {
                ((Rect16 *)(b + 1))->x += dx;
                ((Rect16 *)(b + 1))->y += dy;
                LoadImage((s16 *)(b + 1), (s32)(b + 3));
                b += *b >> 2;
            }
            ((Rect16 *)(b + 1))->x += dx;
            ((Rect16 *)(b + 1))->y += dy;
            LoadImage((s16 *)(b + 1), (s32)(b + 3));
            DrawSync(0);
        } while (--n > 0);
    }
}

void func_8001B930();
extern s32 D_80079500;

void func_8001B90C(s32 w, s32 h, s32 interlace) {
    D_80079500 = 0;
    func_8001B930(w, h, interlace);
}

void SetDefDrawEnv(DRAWENV *, s32, s32, s32, s32);
void SetDefDispEnv(DISPENV *, s32, s32, s32, s32);

extern s32 D_80079544;

#define DB(i) (((Unk800794F8 *)&D_800794F8)->unk98[i])

void func_8001B930(s32 w, s32 h, s32 interlace) {
    s32 i;

    func_80013F04(interlace);
    for (i = 0; i < 2; i++) {
        if (h > 240) {
            SetDefDrawEnv(&DB(i).draw, 0, 0, w, h);
            SetDefDispEnv(&DB(i).disp, 0, 0, w, h);
            DB(i).disp.isinter = 1;
        } else {
            if (interlace == 0) {
                SetDefDrawEnv(&DB(i).draw, 0, i * 256, w, h);
                SetDefDispEnv(&DB(i).disp, 0, 256 - i * 256, w, h);
            } else {
                SetDefDrawEnv(&DB(i).draw, 0, i * 240, w, h);
                SetDefDispEnv(&DB(i).disp, 0, 240 - i * 240, w, h);
            }
            DB(i).disp.isinter = 0;
        }
        DB(i).draw.dtd = 0;
        DB(i).draw.dfe = 0;
        DB(i).draw.isbg = interlace ^ 1;
        DB(i).draw.tpage = GetTPage(0, 0, 0, 0);
        setRGB0(&DB(i).draw, 0, 0, 0);
        DB(i).disp.isrgb24 = interlace;
    }
    D_80079544 = 0;
}

void *func_8001BB44(Chunk *p, s32 id, s32 sub) {
    Chunk *c;

    if (p == 0) {
        return 0;
    }
    for (;;) {
        c = p++;
        if (c->id < 0) {
            return 0;
        }
        if (c->id == id && c->sub == sub) {
            return p;
        }
        p = (Chunk *)((u8 *)p + c->size);
    }
}

void *func_8001AD3C(void *, s32);

void func_8001BB94(Chunk *p, s32 id, s32 sub) {
    Chunk *base;
    Chunk *c;

    base = p;
    if (p == 0) {
        return;
    }
    for (;;) {
        c = p++;
        if (c->id < 0) {
            return;
        }
        if (c->id == id && (sub < 0 || c->sub == sub)) {
            c->id = -1;
            func_8001AD3C(base, (u8 *)p - (u8 *)base);
            return;
        }
        p = (Chunk *)((u8 *)p + c->size);
    }
}

void func_8001BC14(Chunk *arg0) {
    func_8001BB94(arg0, 5, -1);
}

extern s32 D_801D4868;
extern u32 D_801D486C;
extern u8 *D_801D4870;

s32 func_8001BC38(void) {
    if (--D_801D4868 >= 0) {
        return (D_801D486C >> D_801D4868) & 1;
    }
    D_801D4868 = 7;
    D_801D486C = *D_801D4870++;
    return D_801D486C >> 7;
}

u32 func_8001BCA4(s32 n) {
    u16 v;

    v = 0;
    while (D_801D4868 < n) {
        n -= D_801D4868;
        v |= (D_801D486C & ((1 << D_801D4868) - 1)) << n;
        D_801D486C = *D_801D4870++;
        D_801D4868 = 8;
    }
    D_801D4868 -= n;
    return v | ((D_801D486C >> D_801D4868) & ((1 << n) - 1));
}

extern s32 D_801D4878;
extern s32 D_801D4888;
extern s32 D_801D5108;

u32 func_8001BCA4(s32);
s32 func_8001BD60(void) {
    s32 i;

    if (func_8001BC38() != 0) {
        i = D_801D4878++;
        if (i >= 0x21F) {
            return -1;
        }
        (&D_801D4888)[i] = func_8001BD60();
        (&D_801D5108)[i] = func_8001BD60();
    } else {
        i = func_8001BCA4(9);
    }
    return i;
}

extern s32 D_801D487C;
extern u8 *D_801D4874;
extern u8 D_801D5988[0x1000];

void func_8001BDEC(u32 size) {
    s32 pos;
    s32 i;
    s32 k;
    u32 out;
    s32 root;
    s32 sym;
    s32 off;
    u8 c;

    D_801D487C = 0x1000;
    out = 0;
    root = 0;
    pos = 0xFEE;
    for (k = 0; k < pos; k++) {
        D_801D5988[k] = 0;
    }
    while (out < size) {
        if (D_801D487C == 0x1000) {
            D_801D4878 = 0x110;
            root = func_8001BD60();
            D_801D487C = 0;
        }
        sym = root;
        while (sym >= 0x110) {
            if (func_8001BC38() != 0) {
                sym = (&D_801D5108)[sym];
            } else {
                sym = (&D_801D4888)[sym];
            }
        }
        D_801D487C++;
        if (sym < 0x100) {
            *D_801D4874++ = sym;
            D_801D5988[pos] = sym;
            pos++;
            pos &= 0xFFF;
            out++;
        } else {
            sym -= 0xFD;
            off = func_8001BCA4(12);
            for (i = 0; i < sym; i++) {
                c = D_801D5988[(off + i) & 0xFFF];
                *D_801D4874++ = c;
                D_801D5988[pos] = c;
                pos++;
            pos &= 0xFFF;
            }
            out += sym;
        }
    }
}

s32 func_8001C078(s32);

s32 func_8001BFCC(s32 arg0, s32 arg1) {
    return func_8001C078(arg0 + ((s32 *)arg0)[arg1]);
}

u32 func_8001BCA4(s32);
void func_8001BDEC(u32);
extern s32 D_801D4868;
extern u32 D_801D486C;
extern u8 *D_801D4870;

s32 func_8001BFF8(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;

    D_801D4868 = 0;
    D_801D486C = 0;
    D_801D4870 = (u8 *)arg0;
    temp_s0 = func_8001BCA4(0x10);
    temp_s0_2 = (temp_s0 << 0x10) | func_8001BCA4(0x10);
    temp_v0 = func_8001ABCC(temp_s0_2, arg1);
    D_801D4874 = (u8 *)temp_v0;
    func_8001BDEC(temp_s0_2);
    return temp_v0;
}

s32 func_800148B0();
s32 func_8001BFF8(s32, s32);

s32 func_8001C078(s32 arg0) {
    return func_8001BFF8(arg0, func_800148B0());
}

void func_8001C1E0(s8 *, s8 *, s32);

void func_8001C0A8(s8 *base, u32 n, s32 size, s32 (*cmp)(s8 *, s8 *)) {
    u32 i;
    u32 j;
    u32 k;
    s8 *a;
    s8 *b;

    if (n < 2) {
        return;
    }
    a = base;
    if (n == 2) {
        b = a + size;
        if (cmp(a, b) > 0) {
            func_8001C1E0(a, b, size);
        }
        return;
    }
    for (i = 0; i < n; i++, a += size) {
        b = a + size;
        for (j = i; j < n - 1; j++, b += size) {
            if (cmp(a, b) > 0) {
                for (k = i; k <= j; k++) {
                    func_8001C1E0(base + size * k, b, size);
                }
            }
        }
    }
}

void func_8001C1E0(s8 *a, s8 *b, s32 size) {
    u32 i;
    s8 t;

    for (i = 0; i < size; i++) {
        t = a[i];
        a[i] = b[i];
        b[i] = t;
    }
}

s32 MargePrim(void *, void *);
s32 SetDrawMode(void *, s32, s32, s32, s32 *);

extern s32 D_800794F8;
void func_8001C220(void *arg0, s16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9) {
    s32 u;
    s32 abr;

    (*(s8 *)((s8 *)arg0 + 0xF)) = 4;
    (*(u8 *)((s8 *)arg0 + 0x13)) = 0x64;
    (*(s16 *)((s8 *)arg0 + 0x1A)) = arg3;
    (*(s16 *)((s8 *)arg0 + 0x1C)) = arg7;
    (*(s16 *)((s8 *)arg0 + 0x1E)) = arg8;
    (*(s16 *)((s8 *)arg0 + 0x14)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = arg2;
    if (arg4 != 0) {
        u = (arg5 % 64) * 2;
    } else {
        u = (arg5 % 64) * 4;
    }
    (*(u8 *)((s8 *)arg0 + 0x18)) = u;
    (*(u8 *)((s8 *)arg0 + 0x19)) = arg6;
    (*(u8 *)((s8 *)arg0 + 0x10)) = 0x80;
    (*(u8 *)((s8 *)arg0 + 0x11)) = 0x80;
    (*(u8 *)((s8 *)arg0 + 0x12)) = 0x80;
    if (arg9 >= 0) {
        (*(u8 *)((s8 *)arg0 + 0x13)) |= 2;
        abr = arg9;
    } else {
        abr = 0;
    }
    SetDrawMode(arg0, 0, 0, ((arg4 & 3) << 7) | ((abr & 3) << 5) | ((arg6 & 0x100) >> 4) | ((arg5 & 0x3C0) >> 6) | ((arg6 & 0x200) * 4), &D_800794F8);
    MargePrim(arg0, (s8 *)arg0 + 0xC);
}

void func_8001C354(s32 x, s32 y, s32 w, s32 h, u32 color) {
    Rect16 r;
    u32 *buf;
    u32 *p;
    s32 i;

    w /= 4;
    if (w == 0 || h == 0) {
        return;
    }
    r.x = x;
    r.w = w;
    /* x is reused as the fill size in bytes */
    if (func_8001AB64() < w * (h << 2)) {
        x = func_8001AB64();
    } else {
        x = w * (h << 2);
    }
    r.h = (u32)x / (w << 2);
    if (r.h <= 0) {
        return;
    }
    buf = func_8001AD0C(r.h * (w << 2));
    if (buf == NULL) {
        return;
    }
    p = buf;
    for (x -= 4; x >= 0; x -= 4) {
        *p++ = color;
    }
    r.y = y;
    for (i = 0; i < h; i += r.h, r.y += r.h) {
        r.h = (h - i < r.h) ? h - i : r.h;
        LoadImage((s16 *)&r, (s32)buf);
    }
    DrawSync(0);
    func_8001AE90(buf);
}


void func_8001C4DC(s32 x, s32 y, Rect16 *r, u16 tpage, s32 clut, s32 z, u8 c, s8 abr) {
    s32 tp = tpage;

    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = r->x;
        CUR_SPRT->sp.v0 = r->y;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = r->w;
        CUR_SPRT->sp.h = r->h;
        CUR_SPRT->sp.r0 = c;
        CUR_SPRT->sp.g0 = c;
        CUR_SPRT->sp.b0 = c;
        if (abr >= 0) {
            tp |= (abr & 3) << 5;
            setSemiTrans(&CUR_SPRT->sp, 1);
        } else {
            setSemiTrans(&CUR_SPRT->sp, 0);
        }
        setDrawMode(&CUR_SPRT->dm, 0, 0, tp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_800677A4(POLY_FT4 *);
void func_8001EC3C(u8 *, u8, u8, u8, u8);
void func_8001EA64(void *, s16, s16, s16, s16);

void func_8001C6A4(POLY_FT4 *p, POLY_FT4 *dst, u8 *rgb, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy,
                   u8 semi, u8 flat) {
    func_800677A4(p);
    p->tpage = tpage;
    p->clut = clut;
    SetShadeTex(p, flat ^ 1);
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    if (uv != 0) {
        func_8001EC3C((u8 *)p, uv->x, uv->y, uv->w, uv->h);
    }
    if (xy != 0) {
        func_8001EA64(p, xy->x, xy->y, xy->w, xy->h);
    }
    *dst = *p;
}

void func_80067724(POLY_FT3 *);

void func_8001C810(POLY_FT3 *p, s32 *dst, u8 *rgb, s32 tpage, s32 clut, Rect16 *uv, Rect16 *xy,
                   u8 semi, u8 flat) {
    func_80067724(p);
    p->tpage = tpage;
    p->clut = clut;
    SetShadeTex(p, flat ^ 1);
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    if (uv != 0) {
        p->u0 = uv->x + uv->w / 2;
        p->v0 = uv->y;
        p->u1 = uv->x;
        p->v1 = uv->y + uv->h;
        p->u2 = uv->x + uv->w;
        p->v2 = uv->y + uv->h;
    }
    if (xy != 0) {
        p->x0 = xy->x + xy->w / 2;
        p->y0 = xy->y;
        p->x1 = xy->x;
        p->y1 = xy->y + xy->h;
        p->x2 = xy->x + xy->w;
        p->y2 = xy->y + xy->h;
    }
    dst[0] = ((s32 *)p)[0];
    dst[1] = ((s32 *)p)[1];
    dst[2] = ((s32 *)p)[2];
    dst[3] = ((s32 *)p)[3];
    dst[4] = ((s32 *)p)[4];
    dst[5] = ((s32 *)p)[5];
    dst[6] = ((s32 *)p)[6];
    dst[7] = ((s32 *)p)[7];
}

void func_80067764(POLY_GT3 *);

void func_8001CA54(POLY_GT3 *p, POLY_GT3 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, s32 tpage, s32 clut,
                   Rect16 *uv, Rect16 *xy, u8 semi) {
    func_80067764(p);
    p->tpage = tpage;
    p->clut = clut;
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    if (uv != 0) {
        p->u0 = uv->x + uv->w / 2;
        p->v0 = uv->y;
        p->u1 = uv->x;
        p->v1 = uv->y + uv->h;
        p->u2 = uv->x + uv->w;
        p->v2 = uv->y + uv->h;
    }
    if (xy != 0) {
        p->x0 = xy->x + xy->w / 2;
        p->y0 = xy->y;
        p->x1 = xy->x;
        p->y1 = xy->y + xy->h;
        p->x2 = xy->x + xy->w;
        p->y2 = xy->y + xy->h;
    }
    *dst = *p;
}

void func_800677E4(POLY_GT4 *);
void func_8001E7B8(void *, u8, u8, u8);
void func_8001E804(void *, u8, u8, u8);

void func_8001CCB4(POLY_GT4 *p, POLY_GT4 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, u8 *rgb3, s32 tpage,
                   s32 clut, Rect16 *uv, Rect16 *xy, u8 semi) {
    func_800677E4(p);
    p->tpage = tpage;
    p->clut = clut;
    if (semi) {
        SetSemiTrans(p, 1);
    } else {
        SetSemiTrans(p, 0);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    if (rgb3 != 0) {
        func_8001E804(p, rgb3[0], rgb3[1], rgb3[2]);
    }
    if (uv != 0) {
        func_8001EC3C((u8 *)p, uv->x, uv->y, uv->w, uv->h);
    }
    if (xy != 0) {
        func_8001EA64(p, xy->x, xy->y, xy->w, xy->h);
    }
    *dst = *p;
}

void func_80067784(void *);
void func_8001EA64(void *, s16, s16, s16, s16);

void func_8001CE74(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, s16 *r, u8 semi) {
    func_80067784(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    if (r != 0) {
        func_8001EA64(p, r[0], r[1], r[2], r[3]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    dst[5] = p[5];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_800677C4(POLY_G4 *);

void func_8001CFDC(POLY_G4 *p, POLY_G4 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, u8 *rgb3, s32 abr,
                   void *tp0, void *tp1, Rect16 *xy, u8 semi) {
    func_800677C4(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    if (rgb3 != 0) {
        func_8001E804(p, rgb3[0], rgb3[1], rgb3[2]);
    }
    if (xy != 0) {
        func_8001EA64(p, xy->x, xy->y, xy->w, xy->h);
    }
    *dst = *p;
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_80067744(void *);

void func_8001D1AC(s32 *p, s32 *dst, u8 *rgb0, u8 *rgb1, u8 *rgb2, s32 abr, void *tp0, void *tp1,
                   u8 semi) {
    func_80067744(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    if (rgb2 != 0) {
        func_8001E7B8(p, rgb2[0], rgb2[1], rgb2[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    dst[5] = p[5];
    dst[6] = p[6];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_80067704(void *);
void func_8001E75C(void *, u8, u8, u8);
void SetDrawTPage(void *, s32, s32, s32);

void func_8001D33C(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, u8 semi) {
    func_80067704(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_80067904(void *);

void func_8001D464(s32 *p, s32 *dst, u8 *rgb0, u8 *rgb1, s32 abr, void *tp0, void *tp1, u8 semi) {
    func_80067904(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb0 != 0) {
        func_8001E75C(p, rgb0[0], rgb0[1], rgb0[2]);
    }
    if (rgb1 != 0) {
        func_8001E76C(p, rgb1[0], rgb1[1], rgb1[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    dst[4] = p[4];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

void func_800678E4(void *);
void func_8001E75C(void *, u8, u8, u8);
void SetDrawTPage(void *, s32, s32, s32);

void func_8001D5B4(s32 *p, s32 *dst, u8 *rgb, s32 abr, void *tp0, void *tp1, u8 semi) {
    func_800678E4(p);
    if (semi) {
        SetSemiTrans(p, 1);
    }
    if (rgb != 0) {
        func_8001E75C(p, rgb[0], rgb[1], rgb[2]);
    }
    dst[0] = p[0];
    dst[1] = p[1];
    dst[2] = p[2];
    dst[3] = p[3];
    if (tp0 != 0) {
        SetDrawTPage(tp0, 0, 0, GetTPage(0, abr, 0, 0));
    }
    if (tp1 != 0) {
        SetDrawTPage(tp1, 0, 0, GetTPage(0, abr, 0, 0));
    }
}

s32 AddPrim(s32 *, s32);
s32 RotAverageNclip3(s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);
s32 RotTransPers3(s32, s32, s32, s32, s32, s32, s32 *, s32 *);

void func_8001D6D0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg4 == 0) {
        sp30 = RotTransPers3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg5 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg5], arg0);
        }
    }
}

s32 RotAverageNclip4(s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *, s32 *);
s32 RotTransPers4(s32, s32, s32, s32, s32, s32, s32, s32, s32 *, s32 *);

void func_8001D7DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg5 == 0) {
        sp38 = RotTransPers4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg6 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg6], arg0);
        }
    }
}

void func_8001D900(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg5 == 0) {
        sp38 = RotTransPers4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x14, arg0 + 0x20, arg0 + 0x2C, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg1, arg2, arg3, arg4, arg0 + 8, arg0 + 0x14, arg0 + 0x20, arg0 + 0x2C, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg6 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg6], arg0);
        }
    }
}

void func_8001DA24(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 arg7) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg6 == 0) {
        sp30 = RotTransPers3(arg2, arg3, arg4, arg0 + 8, arg0 + 0xC, arg0 + 0x10, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg2, arg3, arg4, arg0 + 8, arg0 + 0xC, arg0 + 0x10, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg7 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp30], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg7], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg7], arg1);
                }
            }
        }
    }
}

void func_8001DBAC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 arg8) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg7 == 0) {
        sp38 = RotTransPers4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0xC, arg0 + 0x10, arg0 + 0x14, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0xC, arg0 + 0x10, arg0 + 0x14, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg8 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp38], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg8], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg8], arg1);
                }
            }
        }
    }
}

void func_8001DD4C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg4 == 0) {
        sp30 = RotTransPers3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x14, arg0 + 0x20, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg1, arg2, arg3, arg0 + 8, arg0 + 0x14, arg0 + 0x20, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg5 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                return;
            }
            AddPrim(&D_800793A0->ot[arg5], arg0);
        }
    }
}

void func_8001DE58(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u8 arg5, u8 arg6, s32 arg7) {
    s32 sp28;
    s32 sp2C;
    s32 sp30;

    if (arg6 == 0) {
        sp30 = RotTransPers3(arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp2C);
        goto block_3;
    }
    if (RotAverageNclip3(arg2, arg3, arg4, arg0 + 8, arg0 + 0x10, arg0 + 0x18, &sp28, &sp30, &sp2C) > 0) {
block_3:
        if ((u32) (sp30 - 2) < 0xFFFU) {
            if (arg7 == 0) {
                AddPrim(&D_800793A0->ot[sp30], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp30], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg7], arg0);
                if ((arg5 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg7], arg1);
                }
            }
        }
    }
}

void func_8001DFE0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, u8 arg6, u8 arg7, s32 arg8) {
    s32 sp30;
    s32 sp34;
    s32 sp38;

    if (arg7 == 0) {
        sp38 = RotTransPers4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp34);
        goto block_3;
    }
    if (RotAverageNclip4(arg2, arg3, arg4, arg5, arg0 + 8, arg0 + 0x10, arg0 + 0x18, arg0 + 0x20, &sp30, &sp38, &sp34) > 0) {
block_3:
        if ((u32) (sp38 - 2) < 0xFFFU) {
            if (arg8 == 0) {
                AddPrim(&D_800793A0->ot[sp38], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[sp38], arg1);
                }
            } else {
                AddPrim(&D_800793A0->ot[arg8], arg0);
                if ((arg6 != 0) && (arg1 != 0)) {
                    AddPrim(&D_800793A0->ot[arg8], arg1);
                }
            }
        }
    }
}

s32 RotTransPers(s32, s32, s32 *, s32 *);

void func_8001E180(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp10;
    s32 sp14;
    s32 temp_v0;

    RotTransPers(arg2, arg0 + 8, &sp10, &sp14);
    temp_v0 = RotTransPers(arg3, arg0 + 0xC, &sp10, &sp14);
    if ((u32) (temp_v0 - 2) < 0xFFFU) {
        if (arg5 == 0) {
            AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg1);
            }
        } else {
            AddPrim((s32 *) &D_800793A0->ot[arg5], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[arg5], arg1);
            }
        }
    }
}

void func_8001E2A4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5) {
    s32 sp10;
    s32 sp14;
    s32 temp_v0;

    RotTransPers(arg2, arg0 + 8, &sp10, &sp14);
    temp_v0 = RotTransPers(arg3, arg0 + 0x10, &sp10, &sp14);
    if ((u32) (temp_v0 - 2) < 0xFFFU) {
        if (arg5 == 0) {
            AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[temp_v0], arg1);
            }
        } else {
            AddPrim((s32 *) &D_800793A0->ot[arg5], arg0);
            if ((arg4 != 0) && (arg1 != 0)) {
                AddPrim((s32 *) &D_800793A0->ot[arg5], arg1);
            }
        }
    }
}

s32 func_8001E3C8(s32 step, u8 *r, s32 tr, u8 *g, s32 tg, u8 *b, s32 tb) {
    s16 vr;
    s16 vg;
    s16 vb;
    s32 done;

    vr = *r;
    vg = *g;
    vb = *b;
    done = 0;
    if (vr < tr) {
        vr += step;
        if (vr > tr) {
            vr = tr;
            done++;
        }
    } else if (vr > tr) {
        vr -= step;
        if (vr < tr) {
            vr = tr;
            done++;
        }
    } else {
        done++;
    }
    if (vg < tg) {
        vg += step;
        if (vg > tg) {
            vg = tg;
            done++;
        }
    } else if (vg > tg) {
        vg -= step;
        if (vg < tg) {
            vg = tg;
            done++;
        }
    } else {
        done++;
    }
    if (vb < tb) {
        vb += step;
        if (vb > tb) {
            vb = tb;
            done++;
        }
    } else if (vb > tb) {
        vb -= step;
        if (vb < tb) {
            vb = tb;
            done++;
        }
    } else {
        done++;
    }
    *r = vr;
    *g = vg;
    *b = vb;
    return done == 3;
}

void func_8001E4E8(u8 dir, s16 step, u8 *state, u8 *prim) {
    s16 c;

    switch (*state) {
    case 0:
        if (dir == 0) {
            func_8001E75C(prim, 0, 0, 0);
        } else {
            func_8001E75C(prim, 0x80, 0x80, 0x80);
        }
        SetSemiTrans(prim, 1);
        *state = 1;
        break;
    case 1:
        if (dir == 0) {
            func_8001E75C(prim, 0, 0, 0);
        } else {
            func_8001E75C(prim, 0x80, 0x80, 0x80);
        }
        SetSemiTrans(prim, 1);
        *state = 2;
        break;
    case 2:
        c = prim[4];
        if (dir == 0) {
            c += step;
            if (c >= 0x80) {
                SetSemiTrans(prim, 0);
                func_8001E75C(prim, 0x80, 0x80, 0x80);
                *state = 3;
                break;
            }
        } else {
            c -= step;
            if (c <= 0) {
                SetSemiTrans(prim, 0);
                func_8001E75C(prim, 0x80, 0x80, 0x80);
                *state = 3;
                break;
            }
        }
        func_8001E75C(prim, c, c, c);
        return;
    case 3:
        SetSemiTrans(prim, 0);
        func_8001E75C(prim, 0x80, 0x80, 0x80);
        *state = 4;
        break;
    case 4:
        *state = 5;
        break;
    }
}


void func_8001E6A4(s32 arg0, s16 arg1, s16 arg2) {
    s16 r[4];

    r[0] = arg1;
    r[1] = arg2;
    r[2] = 0x100;
    r[3] = 1;
    LoadImage(r, arg0);
    DrawSync(0);
}

extern void (*D_8006DF0C[])(void *);
void SetSemiTrans(void *, s32);
void SetShadeTex(void *, s32);

void func_8001E6EC(s32 type, void *p, s32 abe, s32 tge) {
    D_8006DF0C[type](p);
    SetSemiTrans(p, abe);
    SetShadeTex(p, tge);
}

void func_8001E75C(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(u8 *)((s8 *)arg0 + 4)) = arg1;
    (*(u8 *)((s8 *)arg0 + 5)) = arg2;
    (*(u8 *)((s8 *)arg0 + 6)) = arg3;
}

void func_8001E894(void *, u8, u8, u8);
void func_8001E8A4(void *, u8, u8, u8);
void func_8001E8B4(void *, u8, u8, u8);
void func_8001E8C4(void *, u8, u8, u8);
void func_8001E8D4(void *, u8, u8, u8);
void func_8001E8E4(void *, u8, u8, u8);

void func_8001E76C(void *arg0, u8 r, u8 g, u8 b) {
    if (*((u8 *)arg0 + 7) & 4) {
        func_8001E8A4(arg0, r, g, b);
    } else {
        func_8001E894(arg0, r, g, b);
    }
}

void func_8001E7B8(void *arg0, u8 r, u8 g, u8 b) {
    if (*((u8 *)arg0 + 7) & 4) {
        func_8001E8C4(arg0, r, g, b);
    } else {
        func_8001E8B4(arg0, r, g, b);
    }
}

void func_8001E804(void *arg0, u8 r, u8 g, u8 b) {
    if (*((u8 *)arg0 + 7) & 4) {
        func_8001E8E4(arg0, r, g, b);
    } else {
        func_8001E8D4(arg0, r, g, b);
    }
}

void func_8001E8F4(u8 *p, u8 *c);
void func_8001E9AC(u8 *p, u8 *c);

void func_8001E850(u8 *p, u8 *c) {
    if (p[7] & 4) {
        func_8001E9AC(p, c);
    } else {
        func_8001E8F4(p, c);
    }
}

void func_8001E894(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0xC)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0xD)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0xE)) = arg3;
}

void func_8001E8A4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x10)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x11)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x12)) = arg3;
}

void func_8001E8B4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x14)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x15)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x16)) = arg3;
}

void func_8001E8C4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x1C)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x1D)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x1E)) = arg3;
}

void func_8001E8D4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x1C)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x1D)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x1E)) = arg3;
}

void func_8001E8E4(void *arg0, u8 arg1, u8 arg2, u8 arg3) {
    (*(s8 *)((s8 *)arg0 + 0x28)) = arg1;
    (*(s8 *)((s8 *)arg0 + 0x29)) = arg2;
    (*(s8 *)((s8 *)arg0 + 0x2A)) = arg3;
}

void func_8001E8F4(u8 *p, u8 *c) {
    p[0x4] = *c++;
    p[0x5] = *c++;
    p[0x6] = *c++;
    p[0xC] = *c++;
    p[0xD] = *c++;
    p[0xE] = *c++;
    p[0x14] = *c++;
    p[0x15] = *c++;
    p[0x16] = *c++;
    p[0x1C] = *c++;
    p[0x1D] = *c++;
    p[0x1E] = *c;
}

void func_8001E9AC(u8 *p, u8 *c) {
    p[0x4] = *c++;
    p[0x5] = *c++;
    p[0x6] = *c++;
    p[0x10] = *c++;
    p[0x11] = *c++;
    p[0x12] = *c++;
    p[0x1C] = *c++;
    p[0x1D] = *c++;
    p[0x1E] = *c++;
    p[0x28] = *c++;
    p[0x29] = *c++;
    p[0x2A] = *c;
}

void func_8001EBF4(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001EB64(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001EBAC(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001EB1C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4);
void func_8001EA64(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 temp_v1;

    temp_v1 = (*(u8 *)((s8 *)arg0 + 7)) & 0x14;
    switch (temp_v1) {                              /* irregular */
    case 0:
        func_8001EB1C(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    case 4:
        func_8001EBAC(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    case 16:
        func_8001EB64(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    case 20:
        func_8001EBF4(arg0, arg1, arg2, arg3, (s16) (s32) arg4);
        return;
    }
}

void func_8001EB1C(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0xC)) = x1;
    (*(s16 *)((s8 *)arg0 + 0xE)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x10)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x12)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x14)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = y1;
}

void func_8001EB64(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0x10)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x12)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x18)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x1A)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x20)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x22)) = y1;
}

void func_8001EBAC(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0x10)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x12)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x18)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x1A)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x20)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x22)) = y1;
}

void func_8001EBF4(void *arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    s32 x1;
    s32 y1;

    (*(s16 *)((s8 *)arg0 + 8)) = arg1;
    (*(s16 *)((s8 *)arg0 + 0xA)) = arg2;
    x1 = arg1 + arg3;
    (*(s16 *)((s8 *)arg0 + 0x14)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x16)) = arg2;
    (*(s16 *)((s8 *)arg0 + 0x20)) = arg1;
    y1 = arg2 + arg4;
    (*(s16 *)((s8 *)arg0 + 0x22)) = y1;
    (*(s16 *)((s8 *)arg0 + 0x2C)) = x1;
    (*(s16 *)((s8 *)arg0 + 0x2E)) = y1;
}

void func_8001ECC8(u8 *, u8, u8, u8, u8);
void func_8001EC8C(u8 *, u8, u8, u8, u8);

void func_8001EC3C(u8 *p, u8 x, u8 y, u8 w, u8 h) {
    if (p[7] & 0x10) {
        func_8001ECC8(p, x, y, w, h);
    } else {
        func_8001EC8C(p, x, y, w, h);
    }
}

void func_8001EC8C(u8 *p, u8 x, u8 y, u8 w, u8 h) {
    p[0xC] = x;
    p[0xD] = y;
    p[0x14] = x + w;
    p[0x15] = y;
    p[0x1C] = x;
    p[0x1D] = y + h;
    p[0x24] = x + w;
    p[0x25] = y + h;
}

void func_8001ECC8(u8 *p, u8 x, u8 y, u8 w, u8 h) {
    p[0xC] = x;
    p[0xD] = y;
    p[0x18] = x + w;
    p[0x19] = y;
    p[0x24] = x;
    p[0x25] = y + h;
    p[0x30] = x + w;
    p[0x31] = y + h;
}

void func_8001ED04(void *arg0) {
    (*(s16 *)((s8 *)arg0 + 0)) = 0x1000;
    (*(s16 *)((s8 *)arg0 + 6)) = 0;
    (*(s16 *)((s8 *)arg0 + 0xC)) = 0;
    (*(s16 *)((s8 *)arg0 + 2)) = 0;
    (*(s16 *)((s8 *)arg0 + 8)) = 0x1000;
    (*(s16 *)((s8 *)arg0 + 0xE)) = 0;
    (*(s16 *)((s8 *)arg0 + 4)) = 0;
    (*(s16 *)((s8 *)arg0 + 0xA)) = 0;
    (*(s16 *)((s8 *)arg0 + 0x10)) = 0x1000;
}

s32 RotMatrix(void *, void *);

void func_8001ED30(s32 arg0, s16 *arg1, void *arg2) {
    s32 axis;

    axis = arg0 & 0xFF;
    if (axis == 0) {
        return;
    }
    func_8001ED04(arg2);
    switch (axis) {
    case 4:
        return;
    case 3:
        arg1[1] = 0;
        arg1[2] = 0;
        break;
    case 2:
        arg1[0] = 0;
        arg1[2] = 0;
        break;
    case 1:
        arg1[0] = 0;
        arg1[1] = 0;
        break;
    }
    RotMatrix(arg1, arg2);
}

s32 TransMatrix(MATRIX *, VECTOR *);
s32 ScaleMatrix(void *, void *);
void func_8001EFB0(s32 arg0);

void func_8001EDE0(SVECTOR *, VECTOR *, VECTOR *, MATRIX *, s32);

/* old-style definition: the callers pass an int, the byte is read here */
void func_8001EDE0(rot, trans, scale, m, axis)
    SVECTOR *rot;
    VECTOR *trans;
    VECTOR *scale;
    MATRIX *m;
    u8 axis;
{
    RotMatrix(rot, m);
    func_8001ED30(axis, (s16 *)rot, m);
    TransMatrix(m, trans);
    if (scale != 0 && (scale->vx != 0x1000 || scale->vy != scale->vx || scale->vz != scale->vy)) {
        ScaleMatrix(m, scale);
    }
    func_8001EFB0((s32)m);
}

MATRIX *MulMatrix2(MATRIX *, MATRIX *);
s32 RotTrans(u16 *, void *, s32 *);

s32 ScaleMatrix(void *, void *);
void func_8001EFB0(s32 arg0);
void func_8001EEA0(void *arg0, s32 arg1) {
    s16 v[4];
    s32 flag;
    s32 axis;
    s32 sx;
    s32 sy;
    void *rot;

    axis = arg1 & 0xFF;
    if ((*(s32 *)((s8 *)arg0 + 0x48)) == 0) {
        func_8001EDE0((s8 *)arg0 + 0x30, (s8 *)arg0 + 0x20, (s8 *)arg0 + 0x38, arg0, axis);
        return;
    }
    rot = (s8 *)arg0 + 0x30;
    func_8001EFB0((*(s32 *)((s8 *)arg0 + 0x48)));
    RotMatrix(rot, arg0);
    MulMatrix2(*(MATRIX **)((s8 *)arg0 + 0x48), arg0);
    func_8001ED30(axis, rot, arg0);
    v[0] = (*(u16 *)((s8 *)arg0 + 0x20));
    v[1] = (*(u16 *)((s8 *)arg0 + 0x24));
    v[2] = (*(u16 *)((s8 *)arg0 + 0x28));
    RotTrans(v, (s8 *)arg0 + 0x14, &flag);
    sx = (*(s32 *)((s8 *)arg0 + 0x38));
    if ((sx != 0x1000 || (sy = (*(s32 *)((s8 *)arg0 + 0x3C))) != sx || (*(s32 *)((s8 *)arg0 + 0x40)) != sy) && axis != 4) {
        ScaleMatrix(arg0, (s8 *)arg0 + 0x38);
    }
    func_8001EFB0((s32) arg0);
}

s32 SetRotMatrix(s32);
s32 func_8005C444();

void func_8001EFB0(s32 arg0) {
    func_8005C444();
    SetRotMatrix(arg0);
}

void func_8001EFDC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7) {
    (*(s32 *)((s8 *)arg0 + 0x20)) = arg2;
    (*(s32 *)((s8 *)arg0 + 0x24)) = arg3;
    (*(s32 *)((s8 *)arg0 + 0x28)) = arg4;
    (*(s16 *)((s8 *)arg0 + 0x30)) = arg5;
    (*(s16 *)((s8 *)arg0 + 0x32)) = arg6;
    (*(s16 *)((s8 *)arg0 + 0x34)) = arg7;
    (*(s32 *)((s8 *)arg0 + 0x38)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x3C)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x40)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x48)) = arg1;
}

void func_8001F01C(void *arg0, void *arg1) {
    (*(u16 *)((s8 *)arg1 + 0)) = (u16) (*(u16 *)((s8 *)arg0 + 0x14));
    (*(u16 *)((s8 *)arg1 + 2)) = (u16) (*(u16 *)((s8 *)arg0 + 0x18));
    (*(u16 *)((s8 *)arg1 + 4)) = (u16) (*(u16 *)((s8 *)arg0 + 0x1C));
}

extern s32 D_801D69E0;

void func_8001F040(void) {
    D_801D69E0 = 0;
}

extern s32 D_801D69E0;

void func_8001F04C(void) {
    D_801D69E0 = 0;
}

extern s32 D_801D69E0;

s32 func_8001F058(void) {
    return D_801D69E0;
}

extern s32 D_801D69E4;
extern s32 D_801D69E8;
extern s32 D_801D69EC;
extern s32 D_801D69F0;

void func_8001F068(s32 arg0, s32 arg1, s32 arg2) {
    D_801D69E8 = arg0;
    D_801D69EC = arg1;
    D_801D69F0 = arg2;
    D_801D69E4 = arg0 * 0xFF;
}

extern FadePoly D_801D69A0[2];
extern u32 D_801D69D0[2][2];
extern u8 D_800794F4;

void func_8001F094(s32 dir, s32 abr, s32 speed) {
    while (D_801D69E0 != 0) {
        func_80014C08(D_800794F0);
    }
    D_801D69E8 = dir;
    D_801D69EC = abr;
    D_801D69F0 = speed;
    D_801D69E0 = 1;
    D_801D69E4 = dir * 0xFF;
    while (D_801D69E0 != 0) {
        func_80014C08(D_800794F0);
        if (D_801D69E8 != 0) {
            if ((D_801D69E4 -= D_801D69F0) < 0) {
                D_801D69E4 = 0;
                break;
            }
        } else if ((D_801D69E4 += D_801D69F0) >= 0x100) {
            D_801D69E4 = 0xFF;
        }
        SetDrawTPage(D_801D69D0[D_800794F4], 0, 0, GetTPage(0, D_801D69EC, 0, 0));
        func_8001E6EC(8, &D_801D69A0[D_800794F4], 1, 0);
        func_8001E75C(&D_801D69A0[D_800794F4], (u8)D_801D69E4, (u8)D_801D69E4, (u8)D_801D69E4);
        SetSemiTrans(&D_801D69A0[D_800794F4], 1);
        D_801D69A0[D_800794F4].x0 = 0;
        D_801D69A0[D_800794F4].y0 = 0;
        D_801D69A0[D_800794F4].x1 = 320;
        D_801D69A0[D_800794F4].y1 = 0;
        D_801D69A0[D_800794F4].x2 = 0;
        D_801D69A0[D_800794F4].y2 = 240;
        D_801D69A0[D_800794F4].x3 = 320;
        D_801D69A0[D_800794F4].y3 = 240;
        AddPrim((s32 *)D_800793A0->ot, (s32)&D_801D69A0[D_800794F4]);
        AddPrim((s32 *)D_800793A0->ot, (s32)D_801D69D0[D_800794F4]);
    }
    D_801D69E0 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F3C0);

void func_8001F518(u32 i0, u32 *idx, u8 *base) {
    u8 *v0;
    u8 *v1;
    u8 *v2;
    u8 *c0;
    u8 *c1;
    u32 i1;
    u32 i2;

    v0 = base + (i0 >> 16);
    gte_lwc2(12, 0, v0);
    gte_lwc2(17, 4, v0);
    i1 = idx[1];
    i2 = idx[2];
    v1 = base + (i1 >> 16);
    v2 = base + (i2 >> 16);
    gte_lwc2(13, 0, v1);
    gte_lwc2(18, 4, v1);
    gte_lwc2(14, 0, v2);
    gte_lwc2(19, 4, v2);
    c0 = base + (i0 & 0xFFFF);
    gte_nclip();
    c1 = base + (i1 & 0xFFFF);
    base += i2 & 0xFFFF;
    gte_lwc2(20, 0, c0);
    gte_lwc2(21, 0, c1);
    gte_lwc2(22, 0, base);
}

void func_8001F580(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    base += i & 0xFFFF;
    gte_lwc2(12, 0, p);
    gte_lwc2(17, 4, p);
    gte_lwc2(20, 0, base);
}

void func_8001F5A4(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(12, 0, p);
    gte_lwc2(17, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(20, 0, base);
}

void func_8001F5CC(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    base += i & 0xFFFF;
    gte_lwc2(13, 0, p);
    gte_lwc2(18, 4, p);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
    gte_lwc2(21, 0, base);
}

void func_8001F5FC(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(13, 0, p);
    gte_lwc2(18, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(21, 0, base);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
}

void func_8001F630(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    base += i & 0xFFFF;
    gte_lwc2(14, 0, p);
    gte_lwc2(19, 4, p);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
    gte_lwc2(22, 0, base);
}

void func_8001F660(s32 flag, u32 i, u8 *base) {
    u8 *p;

    p = base + (i >> 16);
    gte_lwc2(14, 0, p);
    gte_lwc2(19, 4, p);
    base += i & 0xFFFF;
    gte_nclip();
    gte_lwc2(22, 0, base);
    if (flag == 0) {
        gte_lwc2(20, 0, base);
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F694);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F6C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F768);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F824);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F8B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F94C);

u32 *func_8001F3C0(u32 *, u32 *);
void func_8001F94C(SortWork *);
u32 *func_80020440(u32 *, u32 *);
void func_80020778(SortWork *);

u32 func_800202D8(u32 *data, u32 *ot, u32 packet, void *arg3) {
    SortWork *w;
    s32 n;

    w = SORT_WORK;
    w->data = data;
    w->ot = ot;
    w->packet = packet & 0xFFFFFF;
    w->work = (u32 *)0x1F80007C;
    w->unk20 = arg3;
    n = *data++;
    w->data = data;
    for (; n > 0; n--) {
        SORT_WORK->data = func_8001F3C0(SORT_WORK->data, SORT_WORK->work);
        func_8001F94C(w);
    }
    return SORT_WORK->packet;
}

u32 func_80020370(u32 *data, u32 *ot, u32 packet, void *arg3) {
    SortWork *w;
    s32 n;

    w = SORT_WORK;
    w->data = data;
    w->ot = ot;
    w->packet = packet & 0xFFFFFF;
    w->work = (u32 *)0x1F80007C;
    w->unk20 = arg3;
    n = *data++;
    w->data = data;
    for (; n > 0; n--) {
        if (*SORT_WORK->data++ != 0) {
            SORT_WORK->data = func_80020440(SORT_WORK->data, SORT_WORK->work);
            func_80020778(w);
        } else {
            SORT_WORK->data = func_8001F3C0(SORT_WORK->data, SORT_WORK->work);
            func_8001F94C(w);
        }
    }
    return SORT_WORK->packet;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020440);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002060C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020638);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020674);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800206B0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020778);

void *func_8001AD0C(s32);
void func_80021954(void *arg0);

void *func_80020E34(void *arg0) {
    s32 temp_v0_2;
    void *temp_v0;

    temp_v0 = func_8001AD0C(0x28);
    (*(void **)((s8 *)temp_v0 + 0)) = arg0;
    temp_v0_2 = arg0 + 0x10;
    (*(s32 *)((s8 *)temp_v0 + 4)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 8)) = temp_v0_2;
    (*(s32 *)((s8 *)temp_v0 + 0xC)) = 0;
    (*(s32 *)((s8 *)temp_v0 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(temp_v0);
    return temp_v0;
}

void func_80020E94(void *arg0, void *arg1) {
    s32 temp_v0;

    (*(void **)((s8 *)arg1 + 0)) = arg0;
    temp_v0 = arg0 + 0x10;
    (*(s32 *)((s8 *)arg1 + 4)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 8)) = temp_v0;
    (*(s32 *)((s8 *)arg1 + 0xC)) = 0;
    (*(s32 *)((s8 *)arg1 + 0x10)) = (s32) (*(s32 *)((s8 *)arg0 + 8));
    func_80021954(arg1);
}

s32 *func_80020ED4(s32 n) {
    s32 *p = func_8001AD0C(n * 4);
    s32 *q = p;
    s32 i;

    for (i = 0; i < n; i++) {
        *q++ = 0;
    }
    return p;
}

void func_80020F24(void *arg0, void *arg1) {
    func_8001AE90(arg1);
    func_8001AE90(arg0);
}

#define OP_A(p) (*(u16 *)((p) + 2))
#define OP_B(p) (*(u16 *)((p) + 4))
#define OP_MODE(p) (*(u16 *)((p) + 6))
#define OP_VAL(p) (*(s32 *)((p) + 8))

s32 func_80020F54(Script *s, s32 *regs) {
    u8 *pc;
    u16 op;
    s32 skip;
    s32 cond;
    s32 i;
    u32 next;
    u16 *arg;

    if (s->busy != 0) {
        return -1;
    }
    skip = 0;
    pc = s->pc;
    s->event = 0;
    cond = 0;
    if (s->size > s->offset) {
        do {
            op = *(u16 *)pc;
            switch (op) {
            case 6: {
                u8 *cur = pc;

                if (!skip) {
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = 0;
                }
                pc += OP_A(cur) + 4;
                break;
            }
            case 5: {
                u8 *cur = pc;

                if (!skip) {
                    pc = s->start;
                    pc += *(s32 *)(cur + 4);
                    break;
                }
                pc += 8;
                break;
            }
            case 8: {
                u8 *cur = pc;

                if (!skip) {
                    regs[OP_A(pc)] = (s32)(pc + 6);
                }
                pc += OP_B(cur) + 6;
                break;
            }
            case 7:
                if (!skip) {
                    u8 *cur = pc;

                    switch (OP_B(pc)) {
                    case 0:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] = OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] = regs[OP_VAL(cur)];
                        }
                        break;
                    case 1:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] += OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] += regs[OP_VAL(cur)];
                        }
                        break;
                    case 2:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] -= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] -= regs[OP_VAL(cur)];
                        }
                        break;
                    case 3:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] *= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] *= regs[OP_VAL(cur)];
                        }
                        break;
                    case 4:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] /= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] /= regs[OP_VAL(cur)];
                        }
                        break;
                    case 5:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] %= OP_VAL(cur);
                        } else {
                            regs[OP_A(cur)] %= regs[OP_VAL(cur)];
                        }
                        break;
                    case 6:
                        if (OP_MODE(cur) == 0) {
                            regs[OP_A(cur)] = rand() % (OP_VAL(cur) + 1);
                        } else {
                            regs[OP_A(cur)] = rand() % (regs[OP_VAL(cur)] + 1);
                        }
                        break;
                    }
                }
                pc += 12;
                break;
            case 9:
                if (!skip) {
                    u8 *cur = pc;

                    switch (OP_B(pc)) {
                    case 0:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] == OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] == regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 1:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] <= OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] <= regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 2:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] < OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] < regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 3:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] != OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] != regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 4:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] > OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] > regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    case 5:
                        if (OP_MODE(cur) == 0) {
                            if (regs[OP_A(cur)] >= OP_VAL(cur)) {
                                cond = 1;
                            }
                        } else {
                            if (regs[OP_A(cur)] >= regs[OP_VAL(cur)]) {
                                cond = 1;
                            }
                        }
                        break;
                    }
                } else {
                    cond = 1;
                }
                pc += 12;
                break;
            case 10: {
                u8 *cur = pc;

                if (!skip) {
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                }
                pc += 4;
                break;
            }
            case 11: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 1; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 8;
                break;
            }
            case 12: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 2; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 12;
                break;
            }
            case 13: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 3; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 16;
                break;
            }
            case 14: {
                u8 *cur = pc;

                if (!skip) {
                    arg = (u16 *)(cur + 4);
                    s->event = pc;
                    s->eventOp = op;
                    s->eventArg = OP_A(cur);
                    for (i = 0; i < 4; i++) {
                        if (arg[0] == 0) {
                            s->params[i] = arg[1];
                        } else {
                            s->params[i] = regs[arg[1]];
                        }
                        arg += 2;
                    }
                }
                pc += 20;
                break;
            }
            }
            skip = 0;
            if (cond) {
                cond = 0;
                skip = 1;
            }
            next = (u32)pc + 3;
            pc = (u8 *)(next & ~3);
            s->offset = pc - s->base;
        } while (s->event == 0 && s->offset < s->size);
    }
    s->pc = pc;
    return s->event != 0;
}

void func_80021954(void *arg0) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = 0;
}

void func_8002195C(void *arg0, s16 arg1) {
    (*(s16 *)((s8 *)arg0 + 0x24)) = arg1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021964);

void func_80021AA8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 start;
    s32 end;
    s32 d0;
    s32 d1;
    s32 mid;

    start = arg4 << 0x14;
    arg0[0] = start;
    end = arg5 << 0x14;
    mid = end - start;
    d0 = mid / arg1;
    d1 = (((arg6 - arg5) << 0x14) / arg2 + d0) / 2;
    mid = d0 * 2 - (d1 + arg0[1]) / 2;
    arg0[2] = (mid - arg0[1]) / arg3;
    arg0[3] = (d1 - mid) / arg3;
}

void func_80021B60(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 d1;
    s32 t;

    t = arg4 << 0x10;
    arg0[0] = t;
    t = ((arg5 << 0x10) - t) / arg1;
    d1 = (((arg6 - arg5) << 0x10) / arg2 + t) / 2;
    t = t * 2 - (d1 + arg0[1]) / 2;
    arg0[2] = (t - arg0[1]) / arg3;
    arg0[3] = (d1 - t) / arg3;
}

void func_80021C18(u8 *m) {
    BoneAnim *b;
    s32 i;
    s32 *t;

    b = (BoneAnim *)(m + 0xD80);
    t = (s32 *)(m + 0x2200);
    t[4]--;
    i = 0;
    if (*(s32 *)(m + 0x26D8) != 0) {
        i = *(s16 *)(m + 4);
        b += i;
    }
    for (; i < *(s16 *)(m + 4) + 1; b++, i++) {
        if (t[4] >= 0) {
                b->ch[0].val += b->ch[0].d0;
                b->ch[1].val += b->ch[1].d0;
                b->ch[2].val += b->ch[2].d0;
                b->ch[3].val += b->ch[3].d0;
                b->ch[4].val += b->ch[4].d0;
                b->ch[5].val += b->ch[5].d0;
                b->ch[6].val += b->ch[6].d0;
                b->ch[7].val += b->ch[7].d0;
                b->ch[8].val += b->ch[8].d0;
        } else {
                b->ch[0].val += b->ch[0].d1;
                b->ch[1].val += b->ch[1].d1;
                b->ch[2].val += b->ch[2].d1;
                b->ch[3].val += b->ch[3].d1;
                b->ch[4].val += b->ch[4].d1;
                b->ch[5].val += b->ch[5].d1;
                b->ch[6].val += b->ch[6].d1;
                b->ch[7].val += b->ch[7].d1;
                b->ch[8].val += b->ch[8].d1;
        }
    }
}

s32 RotMatrixYXZ(void *, void *);
s32 ScaleMatrix(void *, void *);

s32 func_80021DF8(void *arg0) {
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s6;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    void *temp_s1;
    void *temp_v1;
    void *var_s2;
    void *var_s3;
    void *var_s4;

    var_s2 = arg0 + 0xD80;
    var_s3 = arg0 + 0x78;
    var_s4 = arg0 + 0xA80;
    var_s6 = 0;
    if ((*(s16 *)((s8 *)arg0 + 4)) > 0) {
        do {
            var_v0 = (*(s32 *)((s8 *)var_s2 + 0));
            if (var_v0 < 0) {
                var_v0 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 0)) = (s16) (var_v0 >> 0x14);
            var_v0_2 = (*(s32 *)((s8 *)var_s2 + 0x10));
            if (var_v0_2 < 0) {
                var_v0_2 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 2)) = (s16) (var_v0_2 >> 0x14);
            var_v0_3 = (*(s32 *)((s8 *)var_s2 + 0x20));
            if (var_v0_3 < 0) {
                var_v0_3 += 0xFFFFF;
            }
            (*(s16 *)((s8 *)var_s4 + 4)) = (s16) (var_v0_3 >> 0x14);
            (*(s32 *)((s8 *)var_s3 + 0x18)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x32)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 0)));
            (*(s32 *)((s8 *)var_s3 + 0x1C)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x42)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 2)));
            (*(s32 *)((s8 *)var_s3 + 0x20)) = (s32) ((*(s16 *)((s8 *)var_s2 + 0x52)) + (*(s16 *)((s8 *)((Unk1F80 *)arg0)->unk1F80[var_s6] + 4)));
            temp_s0 = var_s6 * 0x10;
            temp_v1 = arg0 + temp_s0;
            (*(s32 *)((s8 *)temp_v1 + 0x2000)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x62));
            (*(s32 *)((s8 *)temp_v1 + 0x2004)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x72));
            (*(s32 *)((s8 *)temp_v1 + 0x2008)) = (s32) (*(s16 *)((s8 *)var_s2 + 0x82));
            temp_s1 = var_s3 + 4;
            RotMatrixYXZ(var_s4, temp_s1);
            (*(s32 *)((s8 *)var_s3 + 0)) = 0;
            ScaleMatrix(temp_s1, arg0 + (temp_s0 + 0x2000));
            (*(s32 *)((s8 *)var_s2 + 0)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0)) + (*(s32 *)((s8 *)var_s2 + 4)));
            (*(s32 *)((s8 *)var_s2 + 0x10)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x10)) + (*(s32 *)((s8 *)var_s2 + 0x14)));
            (*(s32 *)((s8 *)var_s2 + 0x20)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x20)) + (*(s32 *)((s8 *)var_s2 + 0x24)));
            (*(s32 *)((s8 *)var_s2 + 0x30)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x30)) + (*(s32 *)((s8 *)var_s2 + 0x34)));
            (*(s32 *)((s8 *)var_s2 + 0x40)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x40)) + (*(s32 *)((s8 *)var_s2 + 0x44)));
            (*(s32 *)((s8 *)var_s2 + 0x50)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x50)) + (*(s32 *)((s8 *)var_s2 + 0x54)));
            (*(s32 *)((s8 *)var_s2 + 0x60)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x60)) + (*(s32 *)((s8 *)var_s2 + 0x64)));
            (*(s32 *)((s8 *)var_s2 + 0x70)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x70)) + (*(s32 *)((s8 *)var_s2 + 0x74)));
            (*(s32 *)((s8 *)var_s2 + 0x80)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x80)) + (*(s32 *)((s8 *)var_s2 + 0x84)));
            var_s6 += 1;
            var_s2 += 0x90;
            var_s3 += 0x50;
            var_s4 += 8;
        } while (var_s6 < (*(s16 *)((s8 *)arg0 + 4)));
    }
    (*(s32 *)((s8 *)var_s2 + 0)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0)) + (*(s32 *)((s8 *)var_s2 + 4)));
    (*(s32 *)((s8 *)var_s2 + 0x10)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x10)) + (*(s32 *)((s8 *)var_s2 + 0x14)));
    (*(s32 *)((s8 *)var_s2 + 0x20)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x20)) + (*(s32 *)((s8 *)var_s2 + 0x24)));
    (*(s32 *)((s8 *)var_s2 + 0x30)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x30)) + (*(s32 *)((s8 *)var_s2 + 0x34)));
    (*(s32 *)((s8 *)var_s2 + 0x40)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x40)) + (*(s32 *)((s8 *)var_s2 + 0x44)));
    (*(s32 *)((s8 *)var_s2 + 0x50)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x50)) + (*(s32 *)((s8 *)var_s2 + 0x54)));
    (*(s32 *)((s8 *)var_s2 + 0x60)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x60)) + (*(s32 *)((s8 *)var_s2 + 0x64)));
    (*(s32 *)((s8 *)var_s2 + 0x70)) = (s32) ((*(s32 *)((s8 *)var_s2 + 0x70)) + (*(s32 *)((s8 *)var_s2 + 0x74)));
    temp_v0 = (*(s32 *)((s8 *)var_s2 + 0x80)) + (*(s32 *)((s8 *)var_s2 + 0x84));
    (*(s32 *)((s8 *)var_s2 + 0x80)) = temp_v0;
    return temp_v0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022100);

extern Unk801D6A4C *D_801D6A4C;

void func_80021C18(u8 *);
s32 func_80021DF8(void *);
s32 func_80022100(void *, s32, s32);
extern s32 D_80079544;

extern s32 D_800794F0;
void func_80022B98(void) {
    s32 temp_v0;
    s32 var_s1;
    void *temp_s0;
    void *temp_v1;

    D_80079544 = 1;
    var_s1 = 0;
loop_1:
    temp_s0 = D_801D6A4C->unk13C[var_s1];
    if (D_801D6A4C->unk114[var_s1] > 0) {
        temp_v1 = temp_s0 + 0x2200;
        if ((*(s32 *)((s8 *)temp_s0 + 0x2208)) >= 0) {
            temp_v0 = (*(s32 *)((s8 *)temp_v1 + 8)) - 1;
            (*(s32 *)((s8 *)temp_v1 + 8)) = temp_v0;
            if (temp_v0 <= 0) {
                func_80022100(temp_s0, (*(s32 *)((s8 *)temp_v1 + 0x18)), 0);
            }
            func_80021DF8(temp_s0);
            func_80021C18(temp_s0);
        }
    }
    var_s1 += 1;
    if (var_s1 < 0x18) {
        goto loop_1;
    }
    var_s1 = 0;
    func_80014C08(D_800794F0);
    goto loop_1;
}

void GsInitCoordinate2(GsCOORDINATE2 *, GsCOORDINATE2 *);
s32 bzero(Unk801D6A4C *, s32);

void func_80022C4C(void) {
    Unk801D6A4C *p;

    p = D_801D6A4C = func_8001ABCC(0x29C, 0x7F);
    bzero(p, 0x29C);
    GsInitCoordinate2(NULL, (GsCOORDINATE2 *)D_801D6A4C->unk28);
    D_80079544 = 1;
}

void func_80022CA4(s32 arg0) {
    void *temp_v1;
    void *temp_v1_2;

    temp_v1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_v1 + 0x2208)) <= 0) {
        (*(s32 *)((s8 *)temp_v1 + 0x2208)) = -1;
        return;
    }
    temp_v1_2 = D_801D6A4C->unk13C[arg0];
    (*(s32 *)((s8 *)temp_v1_2 + 0x2208)) = (s32) -(*(s32 *)((s8 *)temp_v1_2 + 0x2208));
}

void func_80022D00(s32 arg0) {
    s32 temp_v0;
    void *temp_v1;

    temp_v1 = D_801D6A4C->unk13C[arg0];
    temp_v0 = (*(s32 *)((s8 *)temp_v1 + 0x2208));
    if (temp_v0 < 0) {
        (*(s32 *)((s8 *)temp_v1 + 0x2208)) = -temp_v0;
    }
}

s32 func_80022D34(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    void *temp_a0;
    void *temp_v1;
    s32 temp_v0;

    temp_a0 = D_801D6A4C->unk13C[arg0];
    temp_v1 = (s8 *)temp_a0 + 0x2200;
    temp_v0 = ((Model2220 *)temp_a0)->unk2220[arg1].unk0;
    (*(s32 *)((s8 *)temp_a0 + 0x26D8)) = arg3;
    (*(s32 *)((s8 *)temp_v1 + 4)) = temp_v0;
    (*(s32 *)((s8 *)temp_v1 + 0x14)) = temp_v0;
    (*(s32 *)((s8 *)temp_a0 + 0x2200)) = arg1;
    if (arg2 == -2) {
        arg2 = (*(s16 *)((s8 *)((Model2220 *)temp_a0)->unk2220[arg1].unk4 + 0x1A));
    }
    (*(s32 *)((s8 *)temp_v1 + 0x18)) = arg2;
    (*(s32 *)((s8 *)temp_v1 + 0xC)) = 0;
    (*(s32 *)((s8 *)temp_v1 + 0x1C)) = 0x3F800000;
    return func_80022100(temp_a0, 0, -1);
}

void func_80022DBC(s32 arg0) {
    s32 key;
    s32 i;

    key = *(s16 *)((s8 *)D_801D6A4C->unk13C[arg0] + 6);
    func_8001AFF0(arg0 + 0x5A);
    key = (key << 8) | 0x10000000;
    for (i = 0; i < 0x20; i++) {
        if ((D_801D6A4C->unk19C[i].key & ~0xFF) == key) {
            D_801D6A4C->unk19C[i].key = 0;
            D_801D6A4C->unk19C[i].value = 0;
        }
    }
}

void func_80022E58(void) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if ((D_801D6A4C->unk19C[i].key & 0x0FFFFF00) >= 0x3E80) {
            D_801D6A4C->unk19C[i].key = 0;
            D_801D6A4C->unk19C[i].value = 0;
        }
    }
    func_8001AFF0(0x82);
}

typedef struct {
    s32 key;
    s32 value;
} KeyValue;

s32 func_80022ED0(s32 key, s32 n, KeyValue **pp) {
    KeyValue *e;
    KeyValue *free;
    s32 i;

    e = *pp;
    free = 0;
    for (i = 0; i < n; i++, e++) {
        if (e->key == key) {
            return e->value;
        }
        if (free == 0 && e->key == 0) {
            free = e;
        }
    }
    *pp = free;
    return 0;
}

s32 func_80022F34(s32 id, s32 anim, s32 slot, Chunk *pak) {
    char name[24];
    KeyValue *e;
    s32 p;
    s32 n;

    e = (KeyValue *)D_801D6A4C->unk19C;
    p = func_80022ED0((id << 8) | 0x10000000 | anim, 32, &e);
    if (p == 0) {
        if (e == 0) {
            return 0;
        }
        if (id > 1000) {
            sprintf(name, "M:\\HDF%d\\%d_%d.hdf", id / 10, id / 10, id % 10);
            n = id;
        } else {
            sprintf(name, "M:\\HDF%03d\\%c.hdf", id, anim + 'a');
            n = anim;
        }
        p = (s32)func_8001BB44(pak, 1, n);
        if (p == 0) {
            p = func_8001B248((s32 *)name, func_800148B0(), slot + 0x5A);
            if (p == 0) {
                return 0;
            }
            e->key = (id << 8) | 0x10000000 | anim;
            e->value = p;
        }
    }
    return p;
}


void func_80023094(Model2220 *m, s32 *p, s32 i) {
    m->unk2220[i].unk0 = *p++;
    m->unk2220[i].unk4 = p;
}

s32 func_80022F34(s32, s32, s32, Chunk *);
void func_80023094(Model2220 *, s32 *, s32);

s32 func_800230B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    void *temp_s0;

    temp_s0 = D_801D6A4C->unk13C[arg0];
    temp_v0 = func_80022F34((*(s16 *)((s8 *)temp_s0 + 6)), arg1, arg0, (Chunk *)arg3);
    if (temp_v0 != 0) {
        func_80023094(temp_s0, (s32 *)temp_v0, arg2);
        return 1;
    }
    return 0;
}

s32 func_800230B8(s32, s32, s32, s32);

void func_80023128(s32 arg0, s32 arg1, s32 arg2) {
    func_800230B8(arg0, arg1, arg2, 0);
}

void func_80023148(s32 slot, s32 anim) {
    u8 *m;
    BoneAnim *b;
    u8 *c;
    SVECTOR *r;
    s16 *key;
    s32 *p0;
    s32 *p3;
    s32 *p6;
    s32 i;
    s32 j;

    m = D_801D6A4C->unk13C[slot];
    b = (BoneAnim *)(m + 0xD80);
    c = m + 0x78;
    r = (SVECTOR *)(m + 0xA80);
    key = (s16 *)((u8 *)((Model2220 *)m)->unk2220[anim].unk4 + 4);
    for (i = 0; i < *(s16 *)(m + 4) + 1; i++, key += 12, b++, c += 0x50, r++) {
        for (j = 0, p0 = &b->ch[0].val, p3 = &b->ch[3].val, p6 = &b->ch[6].val; j < 3; j++) {
            *(s32 *)((u8 *)p0 + (j << 4)) = *(s32 *)((u8 *)p3 + (j << 4)) = *(s32 *)((u8 *)p6 + (j << 4)) = 0;
        }
        b->ch[0].unk0 = key[0] << 20;
        b->ch[1].unk0 = key[1] << 20;
        b->ch[2].unk0 = key[2] << 20;
        b->ch[3].unk0 = key[4] << 16;
        b->ch[4].unk0 = key[5] << 16;
        b->ch[5].unk0 = key[6] << 16;
        b->ch[6].unk0 = key[8] << 16;
        b->ch[7].unk0 = key[9] << 16;
        b->ch[8].unk0 = key[10] << 16;
        if (i < *(s16 *)(m + 4)) {
            r->vx = b->ch[0].unk0 / 0x100000;
            r->vy = b->ch[1].unk0 / 0x100000;
            r->vz = b->ch[2].unk0 / 0x100000;
            *(s32 *)(c + 0x18) = (s16)(b->ch[3].unk0 >> 16) + ((Model2220 *)m)->bonepos[i][0];
            *(s32 *)(c + 0x1C) = (s16)(b->ch[4].unk0 >> 16) + ((Model2220 *)m)->bonepos[i][1];
            *(s32 *)(c + 0x20) = (s16)(b->ch[5].unk0 >> 16) + ((Model2220 *)m)->bonepos[i][2];
            ((Model2220 *)m)->scale[i][0] = (s16)(b->ch[6].unk0 >> 16);
            ((Model2220 *)m)->scale[i][1] = (s16)(b->ch[7].unk0 >> 16);
            ((Model2220 *)m)->scale[i][2] = (s16)(b->ch[8].unk0 >> 16);
            RotMatrixYXZ(r, c + 4);
            *(s32 *)c = 0;
            ScaleMatrix(c + 4, ((Model2220 *)m)->scale[i]);
        }
    }
    func_80022CA4(slot);
}

void func_80023408(Tmd18 *t) {
    Obj18 *o;
    s32 n;
    s32 i;

    if (t->flags == 0) {
        t->flags = 1;
        n = t->nobj;
        o = t->obj;
        for (i = 0; i < n; i++) {
            o->unk14 = (s32)t + o->unk14;
            o++;
        }
    }
}

void func_80023454(s32 arg0, void *arg1, s32 arg2) {
    (*(s32 *)((s8 *)arg1 + 0xC)) = (s32) (arg2 + 1);
    (*(s32 *)((s8 *)arg1 + 0)) = 0;
    (*(s32 *)((s8 *)arg1 + 8)) = arg0;
}

s32 *func_80023468(u8 *m, s32 *p) {
    s32 i;

    for (i = 0; i < *(s16 *)(m + 4); i++) {
        ((Unk1F80 *)m)->unk1F80[i] = (s16 *)(p + 1);
        p += 3;
    }
    return p;
}

void func_800234AC(Model *m) {
    GsCOORDINATE2 *c;
    s8 *parent;
    GsDOBJ4 *o;
    s32 i;

    c = m->coord;
    GsInitCoordinate2((GsCOORDINATE2 *)D_801D6A4C->unk28, &m->root);
    for (i = 0, parent = m->parent, o = m->obj; i < m->nobj; i++, parent++, o++, c++) {
        o->coord2 = c;
        if (*parent < 0) {
            GsInitCoordinate2(&m->root, c);
        } else {
            GsInitCoordinate2(&m->coord[*parent], c);
        }
        c->coord.t[0] = m->bonepos[i][0];
        c->coord.t[1] = m->bonepos[i][1];
        c->coord.t[2] = m->bonepos[i][2];
    }
}


void func_800235C8(s32 arg0) {
    D_801D6A4C->unk114[arg0] = 0;
    D_801D6A4C->unk13C[arg0] = 0;
    func_8001AFF0(arg0 + 0x40);
}

void func_8002360C(void) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (D_801D6A4C->unk114[i] != 0) {
            func_80022DBC(i);
            D_801D6A4C->unk114[i] = 0;
            /* sic: the original clears the wrong slot */
            D_801D6A4C->unk13C[i - 0x40] = 0;
        }
    }
    func_80014C08(D_800794F0);
    for (i = 0x40; i < 0x7F; i++) {
        func_8001AFF0(i);
    }
}

void *func_800236B4(s32 id) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        if (D_801D6A4C->unk114[i] != 0 &&
            *(s16 *)((u8 *)D_801D6A4C->unk13C[i] + 6) == id) {
            return D_801D6A4C->unk13C[i];
        }
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002371C);

s32 func_8002371C(u8 *);
void func_800234AC(Model *);
void StoreImage2(Rect16 *, u32 *);
void GsMapModelingData(u32 *);
void GsLinkObject4(u32, void *, s32);

s32 func_8002386C(s32 slot, s32 id, s32 pos, s32 pak, s8 kind) {
    char buf[16];
    TIM_IMAGE tim;
    u8 *m;
    u8 *data;
    u32 *img;
    s32 u;
    s32 v;
    s32 i;
    s32 j;

    if (slot >= 0x18) {
        return 0;
    }
    if (D_801D6A4C->unk13C[slot] != 0) {
        func_80022DBC(slot);
        func_800235C8(slot);
    }
    func_80014C08(D_800794F0);
    m = D_801D6A4C->unk13C[slot] = func_8001ABCC(0x28F8, slot + 0x40);
    bzero(m, 0x28F8);
    *(s32 *)(m + 0x26F4) = pak;
    func_80022CA4(slot);
    RotMatrixYXZ(m + 0xA78, m + 0x2C);
    *(s32 *)(m + 0x28) = 0;
    *(s32 *)(m + 0x20) = 0x1000;
    *(s32 *)(m + 0x1C) = 0x1000;
    *(s32 *)(m + 0x18) = 0x1000;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 3; j++) {
            ((Model *)m)->keys[i].rot[j].unk0 = 0;
            ((Model *)m)->keys[i].pos[j].unk0 = 0;
            ((Model *)m)->keys[i].scale[j].unk0 = 0x10000000;
        }
    }
    if (pos < 0) {
        pos = (slot >> 1) + (slot & 1) * 16 + 5;
    }
    *(s32 *)(m + 0x26D4) = (pos - 5) << 16;
    *(s32 *)(m + 0x26D0) = ((((pos & 0x10) << 10) | ((pos & 0xF) * 4)) - 0x14) << 16;
    *(s16 *)(m + 6) = id;
    if (id > 1000) {
        sprintf(buf, "M:\\%d_%d.omd", id / 10, id % 10);
    } else {
        sprintf(buf, "M:\\%03d.omd", id);
    }
    data = func_8001BB44((Chunk *)pak, 0, id);
    if (data == 0) {
        data = (u8 *)func_8001B248((s32 *)buf, func_800148B0(), slot + 0x40);
        if (data == 0) {
            return 0;
        }
        *(s32 *)m = D_801D4848;
    } else {
        *(s32 *)m = ((s32 *)data)[-1];
    }
    *(u8 **)(m + 0x26DC) = data;
    if (pos != 0) {
        i = 0;
        img = func_8001BB44((Chunk *)pak, 5, id);
        if (img == 0) {
            if (func_8002371C(m) == 0) {
                goto skip;
            }
            sprintf(buf, "M:\\%s", data);
            i = 1;
            img = (u32 *)func_8001B144(buf, func_800148B0());
        }
        if (img != 0) {
            u = (pos & 0xF) << 6;
            v = (pos & 0x10) << 4;
            func_8001B634(img, u - 0x140, v);
            OpenTIM(img);
            ReadTIM(&tim);
            *(Rect16 *)(m + 0x26E4) = *tim.prect;
            *(Rect16 *)(m + 0x26EC) = *tim.crect;
            ((Rect16 *)(m + 0x26EC))->x += u - 0x140;
            ((Rect16 *)(m + 0x26EC))->y += v;
            ((Rect16 *)(m + 0x26E4))->x += u - 0x140;
            ((Rect16 *)(m + 0x26E4))->y += v;
            if (i) {
                func_8001AE90(img);
            }
        }
    }
skip:
    StoreImage2((Rect16 *)(m + 0x26EC), (u32 *)(m + 0x26F8));
    data += 0x10;
    *(s16 *)(m + 4) = *(u16 *)data;
    data += 4;
    for (i = 0; i < 32; i++) {
        ((Model *)m)->parent[i] = *data++;
    }
    data = (u8 *)func_80023468(m, (s32 *)data);
    if (kind == 0) {
        for (i = 0; i < *(s16 *)(m + 4); i++) {
            if (i != 0) {
                for (data += 4; *(s32 *)data != 0x30444D4F; data += 4) {
                }
            }
            ((Model *)m)->obj[i].tmd = 0;
            func_80023408((Tmd18 *)data);
            func_80023454((s32)(data + 12), &((Model *)m)->obj[i], i);
        }
    } else {
        for (i = 0; i < *(s16 *)(m + 4); i++) {
            if (i != 0) {
                for (data += 4; *(s32 *)data != 0x41 || ((s32 *)data)[1] != 0; data += 4) {
                }
            }
            ((Model *)m)->obj[i].tmd = 0;
            GsMapModelingData((u32 *)(data + 4));
            GsLinkObject4((u32)(data + 12), &((Model *)m)->obj[i], 0);
        }
    }
    func_800234AC((Model *)m);
    return 1;
}

void func_80023DA4(s32 arg0, s32 arg1, s32 arg2) {
    func_8002386C(arg0, arg1, arg2, 0, 0);
}

void func_80023DC8(s32 arg0, s32 arg1, s32 arg2) {
    func_8002386C(arg0, arg1, arg2, 0, 1);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023DF0);

void func_8005C484(s32, s32);
void func_8005C4A4(s32);
void func_80062484(s32);
s32 GsSetRefView2(GsRVIEW2 *);

void func_800243B0(s32 h) {
    func_8005C484(0xA0, 0x78);
    func_8005C4A4(h);
    func_80062484(h);
    D_801D6A4C->unkC4.vpx = 0;
    D_801D6A4C->unkC4.vpy = 0;
    D_801D6A4C->unkC4.vpz = 0;
    D_801D6A4C->unkC4.vrx = 0;
    D_801D6A4C->unkC4.vry = 0;
    D_801D6A4C->unkC4.vrz = 0;
    D_801D6A4C->unkC4.rz = 0;
    D_801D6A4C->unkC4.super = 0;
    GsSetRefView2(&D_801D6A4C->unkC4);
}

s32 GsSetAmbient(s32, s32, s32);
s32 GsSetLightMode(s32);
s32 func_8005C464(s32, s32, s32);

void func_80024420(void) {
    GsSetAmbient(0x40, 0x40, 0x40);
    func_8005C464(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}


void GsInit3D(void);

void func_80023DF0();
extern MATRIX D_801D6A08;
extern MATRIX D_801D6A28;

void func_80024460(s32 alloc) {
    s32 i;

    func_80022C4C();
    func_8006A804();
    for (i = 0; i < 2; i++) {
        if (alloc) {
            DB(i).unk4070 = func_8001ABCC(0xBB80, 0x7F);
        }
        D_801D6A4C->ot[i].length = 12;
        D_801D6A4C->ot[i].org = DB(i).ot;
        D_801D6A4C->ot[i].offset = 0;
        D_801D6A4C->ot[i].point = 0;
        D_801D6A4C->ot[i].tag = D_801D6A4C->ot[i].org + 0xFFF;
    }
    GsInit3D();
    func_800243B0(0x1C0);
    func_80024420();
    func_8006A814();
    {
        MATRIX lm[2] = {
            { { { 0, 0x1800, -0x1800 }, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
            { { { 0, -0x1000, -0x5DC }, { 0, 0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
        };
        MATRIX lc[2] = {
            { { { 0x800, 0, 0 }, { 0x800, 0, 0 }, { 0x800, 0, 0 } }, { 0, 0, 0 } },
            { { { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
        };

        D_801D6A08 = lm[1];
        D_801D6A28 = lc[1];
    }
    if (alloc) {
        func_8001B10C((s32) "M:", func_800148B0());
        func_8001683C((s32)func_80023DF0);
    }
}

extern void *D_801D6A58[];
extern SVECTOR *D_801D6A50;
extern s32 *D_801D6A48;
extern s16 D_801D69F8;
extern s16 D_801D69FA;
extern s16 D_801D69FC;
extern s16 D_801D69FE;
extern s16 D_801D6A00;
extern s32 D_8006DF84;
void func_800246E0();

extern u8 D_8006DF88;
s32 RotTransPers(s32, s32, s32 *, s32 *);

#define PULSE(n) (D_8006DF88 + (n) * 12)

void func_800246E0(Unk800793A0 *db, s32 n) {
    LINE_F2 *l;
    SVECTOR *v;
    SVECTOR *vv;
    s32 *xy;
    s32 *xx;
    s32 i;
    s32 j;
    u8 c;
    s32 p;
    s32 flag;

    l = D_801D6A58[n];
    if (D_8006DF84 == 0) {
        return;
    }
    v = D_801D6A50;
    xy = D_801D6A48;
    for (i = 0; i < D_801D69FC * D_801D69FE; i++, v++, xy++) {
        if ((v->pad = RotTransPers((s32)v, (s32)xy, &p, &flag)) < 0x3C) {
            v->pad = -1;
        }
    }
    v = D_801D6A50;
    xy = D_801D6A48;
    for (j = 0; j < D_801D69FE; j++, v++, xy++) {
        for (i = 0; i < D_801D69FC - 1; i++, v++, xy++) {
            c = PULSE(j + i);
            if ((s8)PULSE(j + i) < 0) {
                c = -c;
            }
            if ((u16)v[0].pad < 0x1000 && (u16)v[1].pad < 0x1000) {
                *(s32 *)&l->x0 = xy[0];
                *(s32 *)&l->x1 = xy[1];
                l->r0 = c >> 1;
                l->g0 = c + 0x40;
                l->b0 = (0x80 - c) / 2;
                AddPrim((s32 *)&db->ot[v[1].pad < v[0].pad ? v[0].pad : v[1].pad], (s32)l++);
            }
        }
    }
    v = D_801D6A50;
    xy = D_801D6A48;
    for (i = 0; i < D_801D69FC; i++, v++, xy++) {
        vv = v;
        xx = xy;
        for (j = 0; j < D_801D69FE - 1; j++, vv += D_801D69FC, xx += D_801D69FC) {
            c = PULSE(i + j);
            if ((s8)PULSE(i + j) < 0) {
                c = -c;
            }
            if ((u16)vv[0].pad < 0x1000 && (u16)vv[D_801D69FC].pad < 0x1000) {
                *(s32 *)&l->x0 = xx[0];
                *(s32 *)&l->x1 = xx[D_801D69FC];
                l->r0 = c >> 1;
                l->g0 = c + 0x40;
                l->b0 = (0x80 - c) / 2;
                AddPrim((s32 *)&db->ot[vv[1].pad < vv[0].pad ? vv[0].pad : vv[1].pad], (s32)l++);
            }
        }
    }
    D_8006DF88 += 2;
}


void func_80024B08(s32 w, s32 h, s32 cols, s32 rows, s32 unused, s32 vertical) {
    u8 *l;
    SVECTOR *v;
    s32 i;
    s32 j;
    s32 r;

    D_801D69FC = cols;
    D_801D69FE = rows;
    D_801D6A00 = ((s16)cols - 1) * (s16)rows + ((s16)rows - 1) * (s16)cols;
    D_801D69F8 = w;
    D_801D69FA = h;
    D_8006DF84 = 1;
    for (i = 0; i < 2; i++) {
        l = D_801D6A58[i] = func_8001AD0C(D_801D6A00 * 16);
        for (j = 0; j < D_801D6A00; j++) {
            func_800678E4(l);
            l[4] = 8;
            l[5] = 0x40;
            l[6] = 8;
            l += 16;
        }
    }
    D_801D6A50 = v = (SVECTOR *)func_8001AD0C(D_801D69FC * (D_801D69FE << 3));
    D_801D6A48 = (s32 *)func_8001AD0C(D_801D69FC * (D_801D69FE << 2));
    for (r = 0; r < D_801D69FE; r++) {
        for (j = 0; j < D_801D69FC; j++) {
            v->vx = D_801D69F8 / 2 - D_801D69F8 / (D_801D69FC - 1) * j;
            if (vertical) {
                v->vy = D_801D69FA / 2 - D_801D69FA / (D_801D69FE - 1) * r;
                v->vz = 0;
            } else {
                v->vz = D_801D69FA / 2 - D_801D69FA / (D_801D69FE - 1) * r;
                v->vy = 0;
            }
            v++;
        }
    }
    func_8001683C((s32)func_800246E0);
}

void func_80024DD4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_8001AE90(D_801D6A58[i]);
    }
    func_8001AE90(D_801D6A50);
    func_8001AE90(D_801D6A48);
}

extern s32 D_8007956C;

s32 func_80024E44(u8 *cam, s32 *pos, s32 cur, s16 *target) {
    s32 k;
    s32 n;
    s32 d;
    s32 step;

    if (D_8007956C != 0) {
        *(s32 *)(cam + 0x5C) = -pos[0];
        *(s32 *)(cam + 0x60) = -pos[1];
        *(s32 *)(cam + 0x64) = -pos[2];
        *(s32 *)(cam + 0x68) = target[2] << 12;
        *(s32 *)(cam + 0x6C) = target[3] << 12;
        *(s32 *)(cam + 0x70) = target[1] << 12;
        return target[4];
    }
    k = *(s32 *)(cam + 0x50);
    if (k == 0) {
        k = 1;
    }
    n = 30 / k;
    if (n == 0) {
        n = 1;
    }
    d = (target[3] << 12) - *(s32 *)(cam + 0x6C);
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(cam + 0x6C) += step;
    }
    d = (target[2] << 12) - *(s32 *)(cam + 0x68);
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(cam + 0x68) += step;
    }
    d = (target[1] << 12) - *(s32 *)(cam + 0x70);
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d > 0 ? 1 : -1;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(cam + 0x70) += step;
    }
    d = target[4] - cur;
    if (d != 0) {
        step = d / n;
        if (step == 0) {
            step = d > 0 ? 1 : -1;
        }
        cur += step;
    }
    *(s32 *)(cam + 0x5C) = -pos[0];
    *(s32 *)(cam + 0x60) = -pos[1];
    *(s32 *)(cam + 0x64) = -pos[2];
    return cur;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800250F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002583C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002584C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025854);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025874);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025BDC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025C00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025EE4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80025F08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026128);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002627C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002631C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026578);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026974);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026C70);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026D30);

void func_80026D84(void) {
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026D8C);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010190);

void func_80027044(void);

void func_80026E90(s32 x, s32 y, s32 n) {
    Rect16 r;
    u32 *tim;
    SprtPacket *p;
    s32 i;
    TIM_IMAGE *image;

    D_801D6B12 = x;
    D_801D6B14 = y;
    D_801D6B20 = x + 0x20;
    D_801D6B22 = y + 0xF8;
    D_801D6B10 = n;
    func_800149B8(0, -1, 0, 0x800, func_8001B144, "B:\\SYSTEM.TIM", func_800148B0());
    tim = (u32 *)func_80014C08(0x7FFFFFFF);
    func_8001B438(tim, D_801D6B12, D_801D6B14, -2, -2);
    image = &D_801D4850;
    r.x = D_801D6B20;
    r.y = D_801D6B22;
    r.w = 0x20;
    r.h = 8;
    LoadImage((s16 *)&r, (s32)image->caddr);
    DrawSync(0);
    func_8001AE90(tim);
    p = func_8001ACEC(D_801D6B10 * sizeof(SprtPacket) * 2);
    for (i = 0; i < 2; i++) {
        DB(i).unk40B8 = (s32)(p + D_801D6B10 * i);
    }
    func_80027044();
    D_801D6B24 = D_800793A0->unk40B8;
}


void func_80027044(void) {
    s32 i;
    s32 j;

    if (D_801D6B10 == 0) {
        return;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < D_801D6B10; j++) {
            setDrawMode(&((SprtPacket *)DB(i).unk40B8)[j].dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            setSprt(&((SprtPacket *)DB(i).unk40B8)[j].sp);
            setSemiTrans(&((SprtPacket *)DB(i).unk40B8)[j].sp, 1);
            setShadeTex(&((SprtPacket *)DB(i).unk40B8)[j].sp, 0);
            setRGB0(&((SprtPacket *)DB(i).unk40B8)[j].sp, 0x80, 0x80, 0x80);
        }
    }
}

extern s32 D_801D6B24;

void func_800271D0(void) {
    D_801D6B24 = D_800793A0->unk40B8;
}

extern s32 D_8006DF98;
void func_80027228(s32, s32, Rect16 *, u8 *, u16, s32, s32);

void func_800271EC(s32 arg0, s32 arg1, s32 arg2, u16 arg3, s32 arg4, s32 arg5) {
    func_80027228(arg0, arg1, (Rect16 *)arg2, (u8 *)&D_8006DF98, arg3, arg4, arg5);
}


void func_80027228(s32 x, s32 y, Rect16 *r, u8 *rgb, u16 tpage, s32 n, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = r->x;
        CUR_SPRT->sp.v0 = r->y;
        CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
        CUR_SPRT->sp.w = r->w;
        CUR_SPRT->sp.h = r->h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, tpage);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_80027458(s32, s32, s32, s32, s32, s32, s32, u8 *, s32);

void func_80027410(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7) {
    func_80027458(arg0, arg1, arg2, arg3, arg4, arg5, arg6, (u8 *)&D_8006DF98, arg7);
}


void func_80027458(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 n, u8 *rgb, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = u;
        CUR_SPRT->sp.v0 = v;
        CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027674);

void func_800276C8(s32 x, s32 y, u8 c, s32 n, u8 *rgb, s32 z, s32 w, s32 h, s32 bu, s32 bv) {
    if (c > 0x20 && func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = bu + (c & 0xF) * 8;
        CUR_SPRT->sp.v0 = bv + (((c - 0x20) & 0xF0) >> 1);
        CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_8002793C(s32, s32, u8 *, s32, u8 *, s32);

void func_8002790C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8002793C(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80029A0C(s32, s32, s32, s32, u8 *, s32);

void func_8002793C(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '*':
            s++;
            switch (*s) {
            case 'a':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - '0', rgb, z);
                break;
            case 'b':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - ')', rgb, z);
                break;
            case 'c':
                s++;
                n = *s - '0';
                s++;
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                break;
            case 'd':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - 0x1C, rgb, z);
                break;
            case 'e':
                s++;
                n = *s;
                if (*s < '4') {
                    n -= '#';
                } else if (*s == 'a') {
                    n = 0x11;
                } else {
                    n = *s - '"';
                }
                func_80029A0C(x, y, 3, n, rgb, z);
                s++;
                break;
            }
        case '\n':
            x = left;
            y += 6;
            s++;
            break;
        case ' ':
            x += 4;
            s++;
            break;
        default:
            c = *s++;
            if (c >= 'a') {
                c -= 0x20;
            }
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 4;
            CUR_SPRT->sp.u0 = (c & 0xF) * 4;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 5 - 0x16;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 4;
            CUR_SPRT->sp.h = 5;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_80027DE8(s32, s32, u8 *, s32, u8 *, s32);

void func_80027DB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80027DE8(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80027DE8(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;
    s32 icon;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case 1:
            s++;
            func_80029A0C(x, y, 3, *s++ - 1, rgb, z);
            x += 6;
            break;
        case '*':
            s++;
            switch (*s) {
            case 'a':
                s++;
                func_80029A0C(x, y, 3, *s++ - '0', rgb, z);
                x += 6;
                break;
            case 'b':
                s++;
                func_80029A0C(x, y, 3, *s++ - 0x29, rgb, z);
                x += 6;
                break;
            case 'c':
                s++;
                n = *s++ - '0';
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                break;
            case 'd':
                s++;
                func_80029A0C(x, y, 3, *s++ - 0x1C, rgb, z);
                x += 6;
                break;
            case 'e':
                s++;
                if (*s < 0x34) {
                    icon = *s - 0x23;
                } else if (*s == 'a') {
                    icon = 0x11;
                } else {
                    icon = *s - 0x22;
                }
                func_80029A0C(x, y, 3, icon, rgb, z);
                x += 6;
                s++;
                break;
            }
            break;
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x = left;
            y += 6;
            s++;
            break;
        case ' ':
            x += 5;
            s++;
            break;
        default:
            c = *s++;
            if (c >= 'a') {
                c -= 0x20;
            }
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 5;
            CUR_SPRT->sp.u0 = (c & 0xF) * 4 + 0x40;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 5 - 0x16;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 4;
            CUR_SPRT->sp.h = 5;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_80028258(s32, s32, u8 *, s32, u8 *, s32);

void func_80028228(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028258(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80028258(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s16 clut;
    s32 top;
    s32 c;

    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    top = y - 5;
    y = top;
    while (*s != 0) {
        switch (*s) {
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x += 6;
            y = top;
            s++;
            break;
        case ' ':
            y -= 5;
            s++;
            break;
        default:
            c = *s++;
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            y -= 5;
            CUR_SPRT->sp.u0 = (c & 0xF) * 6;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 4 - 0x26;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 5;
            CUR_SPRT->sp.h = 4;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_80028588(s32, s32, u8 *, s32, u8 *, s32);

void func_80028558(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028588(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80029A0C(s32, s32, s32, s32, u8 *, s32);

void func_80028588(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case '*':
            s++;
            switch (*s) {
            case 'a':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - '0', rgb, z);
                x += 6;
                break;
            case 'b':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - ')', rgb, z);
                x += 6;
                break;
            case 'c':
                s++;
                n = *s++ - '0';
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                break;
            case 'd':
                s++;
                n = *s++;
                func_80029A0C(x, y, 3, n - 0x1C, rgb, z);
                x += 6;
                break;
            case 'e':
                s++;
                n = *s;
                if (*s < '4') {
                    n -= '#';
                } else if (*s == 'a') {
                    n = 0x11;
                } else {
                    n = *s - '"';
                }
                func_80029A0C(x, y, 3, n, rgb, z);
                x += 6;
                s++;
                break;
            }
            break;
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x = left;
            y += 7;
            s++;
            break;
        case ' ':
            x += 6;
            s++;
            break;
        default:
            c = *s++;
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 6;
            CUR_SPRT->sp.u0 = (c & 0xF) * 6;
            CUR_SPRT->sp.v0 = (((c - 0x20) & 0xF0) >> 4) * 6 - 0x4C;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 6;
            CUR_SPRT->sp.h = 6;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

void func_800289D0(s32, s32, u8 *, s32, u8 *, s32);

void func_800289A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_800289D0(arg0, arg1, (u8 *)arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80029A0C(s32, s32, s32, s32, u8 *, s32);

void func_800289D0(s32 x, s32 y, u8 *s, s32 n, u8 *rgb, s32 z) {
    s32 left;
    s16 clut;
    s32 c;

    left = x;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case 1:
            s++;
            func_80029A0C(x, y, 1, *s++ - 1, rgb, z);
            x += 8;
            break;
        case '\f':
            s++;
            n = *s++;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        case '\n':
            x = left;
            y += 9;
            s++;
            break;
        case ' ':
            x += 8;
            s++;
            break;
        default:
            c = *s++;
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 8;
            CUR_SPRT->sp.u0 = (c & 0xF) * 8;
            CUR_SPRT->sp.v0 = ((c - 0x20) >> 4) * 7;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 7;
            CUR_SPRT->sp.h = 7;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}

s32 func_80028D48(s32, s32, u8 *, u8 *, s32, s32);

void func_80028D18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028D48(arg0, arg1, (u8 *)arg2, (u8 *)&D_8006DF98, arg3, arg4);
}

extern u8 D_8006DF9C[];

s32 func_80028D48(s32 x, s32 y, u8 *s, u8 *rgb, s32 n, s32 z) {
    s32 dx;
    s32 left;
    s32 top;
    s32 dy;
    s32 prop;
    s16 clut;
    s32 c;
    s32 t;

    dx = 0;
    left = x;
    top = y;
    dy = 0;
    prop = 1;
    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    D_801D6B18 = 0;
    D_801D6B1C = 0;
    while (*s != 0) {
        if (*s == '*') {
            s++;
            switch (*s) {
            case 'a':
                s++;
                func_80029A0C(x, y + 1, 0, *s++ - '0', rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'b':
                s++;
                func_80029A0C(x, y + 1, 0, *s++ - ')', rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'c':
                s++;
                n = *s - '0';
                s++;
                clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                continue;
            case 'd':
                s++;
                func_80029A0C(x, y + 1, 0, *s++ - 0x1C, rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'e':
                s++;
                t = *s;
                if (*s < '4') {
                    t -= '#';
                } else if (*s == 'a') {
                    t = 0x11;
                } else {
                    t = *s - '"';
                }
                func_80029A0C(x, y + 1, 0, t, rgb, z);
                x += 12 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                s++;
                continue;
            case 'g':
                s++;
                func_80029A0C(x, y, 2, *s++ - '0', rgb, z);
                x += 25 + dx;
                if (D_801D6B18 < x) {
                    D_801D6B18 = x;
                }
                continue;
            case 'h':
                s++;
                if (*s == '-') {
                    s++;
                    dy = '0' - *s++;
                } else {
                    dy = *s++ - '0';
                }
                continue;
            case 's':
                s++;
                prop = *s++ - '0';
                continue;
            case 'w':
                s++;
                if (*s == '-') {
                    s++;
                    dx = '0' - *s++;
                } else {
                    dx = *s++ - '0';
                }
                continue;
            }
        }
        switch (*s) {
        case '\\':
            s++;
            if (*s != 'n') {
                s++;
                break;
            }
            s++;
            x = left;
            y += 13 + dy;
            if (D_801D6B18 < x) {
                D_801D6B18 = x;
            }
            if (D_801D6B1C < y) {
                D_801D6B1C = y;
            }
            break;
        case '\n':
            s++;
            x = left;
            y += 13 + dy;
            if (D_801D6B18 < x) {
                D_801D6B18 = x;
            }
            if (D_801D6B1C < y) {
                D_801D6B1C = y;
            }
            break;
        default:
            c = *s++ - 0x20;
            if (func_80029990() != 0) {
                return; /* no value: the caller never reads it */
            }
            x += dx;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y + 1;
            CUR_SPRT->sp.u0 = (c % 16) * 6;
            if (prop != 0) {
                CUR_SPRT->sp.u0 += D_8006DF9C[c] >> 4;
            }
            CUR_SPRT->sp.v0 = (c / 16) * 12 + 0x30;
            CUR_SPRT->sp.clut = clut;
            if (prop != 0) {
                CUR_SPRT->sp.w = (u8)(D_8006DF9C[c] & 0xF);
            } else {
                CUR_SPRT->sp.w = 6;
            }
            CUR_SPRT->sp.h = 12;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            x += CUR_SPRT->sp.w;
            if (D_801D6B18 < x) {
                D_801D6B18 = x;
            }
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
    D_801D6B18 -= left;
    D_801D6B1C = D_801D6B1C - top + 12;
    return D_801D6B18;
}


INCLUDE_ASM("asm/main/nonmatchings/game", func_800293FC);

void func_8002961C(s32 x, s32 y, u8 *s, u8 *rgb, s32 n, s32 z) {
    s16 clut;
    s32 g;

    clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
    while (*s != 0) {
        switch (*s) {
        case ' ':
            x += 0x10;
            s++;
            break;
        case 'c':
            s++;
            n = *s++ & 0xF;
            clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            break;
        default:
            g = *s++;
            switch (g) {
            case '+':
                g = 10;
                break;
            case '-':
                g = 11;
                break;
            case '=':
                g = 12;
                break;
            default:
                g -= '0';
                break;
            }
            if (func_80029990() != 0) {
                return;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            x += 0xC;
            CUR_SPRT->sp.u0 = (g % 8) * 16 - 0x80;
            CUR_SPRT->sp.v0 = (g / 8) * 0x15;
            CUR_SPRT->sp.clut = clut;
            CUR_SPRT->sp.w = 0x10;
            CUR_SPRT->sp.h = 0x15;
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            break;
        }
    }
}


s32 func_80029990(void) {
    if (D_801D6B24 == D_800793A0->unk40B8 + D_801D6B10 * 0x1C) {
        return -1;
    }
    return 0;
}

void func_80029A0C(s32, s32, s32, s32, u8 *, s32);

void func_800299DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80029A0C(arg0, arg1, arg2, arg3, (u8 *)&D_8006DF98, arg4);
}

void func_80029A0C(s32 x, s32 y, s32 kind, s32 n, u8 *rgb, s32 z) {
    s16 w;
    s16 h;

    w = 0xB;
    h = 0xB;
    if (func_80029990() == 0) {
        switch (kind) {
        case 0:
            CUR_SPRT->sp.u0 = (n % 14) * 12;
            CUR_SPRT->sp.v0 = (n / 14) * 11 + 0x7F;
            if (n >= 0x15 && n < 0x18) {
                CUR_SPRT->sp.clut = getClut(D_801D6B20 + 16, D_801D6B22 + 5);
            } else if (n == 0x1B) {
                CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 3);
            } else if (n >= 0x1C && n < 0x25) {
                CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 7);
            } else {
                CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            }
            w = 12;
            h = 11;
            break;
        case 1:
            CUR_SPRT->sp.u0 = n * 8;
            CUR_SPRT->sp.v0 = 0x78;
            CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            w = 7;
            h = 7;
            break;
        case 2:
            if (n < 10) {
                CUR_SPRT->sp.u0 = 0xD8;
                CUR_SPRT->sp.v0 = n * 12 + 0x18;
            } else {
                CUR_SPRT->sp.u0 = 0xC0;
                CUR_SPRT->sp.v0 = (n - 10) * 12 + 0x60;
            }
            CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            w = 0x18;
            h = 0xC;
            break;
        case 3:
            CUR_SPRT->sp.u0 = (n % 14) * 6 + 0x30;
            CUR_SPRT->sp.v0 = (n / 14) * 6 - 0x60;
            CUR_SPRT->sp.clut = getClut(D_801D6B20, D_801D6B22 + 5);
            w = 5;
            h = 5;
            break;
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

s32 func_80029EFC(s32, s32, s32, s32, u8 *, s32, u8 *);

void func_80029EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80029EFC(arg0, arg1, arg2, arg3, (u8 *)&D_8006DF98, arg4, (u8 *)arg5);
}

s32 func_80029EFC(s32 x, s32 y, s32 n, s32 arg3, u8 *rgb, s32 z, u8 *s) {
    s32 left;
    s32 spacing;
    s32 lineSpacing;
    s32 space;
    s32 c;

    left = x;
    spacing = 0;
    lineSpacing = 0;
    space = 6;
    if (func_80029990() != 0) {
        return;
    }
    while (*s != 0) {
        if ((u8)(*s + 0x7F) >= 0x18) {
            switch (*s) {
            case '\\':
                s++;
                if (*s == 'n') {
                    s++;
                    x = left;
                    y += 13 + lineSpacing;
                } else {
                    s++;
                }
                break;
            case 'a':
                s++;
                c = *s++;
                func_80029A0C(x, y + 1, 0, c - '0', rgb, z);
                x += 12 + spacing;
                break;
            case 'b':
                s++;
                c = *s++;
                func_80029A0C(x, y + 1, 0, c - ')', rgb, z);
                x += 12 + spacing;
                break;
            case 'c':
                s++;
                n = *s - '0';
                s++;
                break;
            case 'd':
                s++;
                c = *s++;
                func_80029A0C(x, y + 1, 0, c - 0x1C, rgb, z);
                x += 12 + spacing;
                break;
            case 'e':
                s++;
                if (*s < '4') {
                    c = *s - '#';
                } else if (*s == 'a') {
                    c = 0x11;
                } else {
                    c = *s - '"';
                }
                func_80029A0C(x, y + 1, 0, c, rgb, z);
                x += 12 + spacing;
                s++;
                break;
            case 'g':
                s++;
                c = *s++;
                func_80029A0C(x, y, 2, c - '0', rgb, z);
                x += 25 + spacing;
                break;
            case 'h':
                s++;
                if (*s == '-') {
                    s++;
                    lineSpacing = '0' - *s;
                    s++;
                } else {
                    lineSpacing = *s - '0';
                    s++;
                }
                break;
            case 'w':
                s++;
                if (*s == '-') {
                    s++;
                    spacing = '0' - *s;
                    s++;
                } else {
                    spacing = *s - '0';
                    s++;
                }
                break;
            case 'z':
                s++;
                if (space == 6) {
                    space = 12;
                } else {
                    space = 6;
                }
                break;
            case ' ':
                s++;
                x += space + spacing;
                break;
            case '\n':
                s++;
                x = left;
                y += 13;
                y += lineSpacing;
                break;
            case 's':
                s += 2;
                break;
            default:
                if ((u32)(*s - '0') < 10) {
                    if (func_80029990() != 0) {
                        return x - left;
                    }
                    CUR_SPRT->sp.x0 = x;
                    CUR_SPRT->sp.y0 = y;
                    CUR_SPRT->sp.u0 = 0x6C;
                    CUR_SPRT->sp.v0 = 0x30;
                    CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
                    CUR_SPRT->sp.w = 12;
                    CUR_SPRT->sp.h = 12;
                    setSemiTrans(&CUR_SPRT->sp, 1);
                    CUR_SPRT->sp.r0 = 0x80;
                    CUR_SPRT->sp.g0 = 0x80;
                    CUR_SPRT->sp.b0 = 0x80;
                    setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
                    addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
                    addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
                    D_801D6B24 += sizeof(SprtPacket);
                    x += 6 + spacing;
                }
                s++;
                break;
            }
        } else {
            if (func_80029990() != 0) {
                return x - left;
            }
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0x6C;
            CUR_SPRT->sp.v0 = 0x30;
            CUR_SPRT->sp.clut = getClut(D_801D6B20 + (n % 2) * 16, D_801D6B22 + n / 2);
            CUR_SPRT->sp.w = 12;
            CUR_SPRT->sp.h = 12;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, D_801D6B12, D_801D6B14));
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
            x += 12 + spacing;
            s += 2;
        }
    }
    D_801D6B18 = x - left;
    D_801D6B1C = y + 12;
    return x - left;
}

s8 *func_8002A5B4(s8 *d, s8 *s) {
    if ((*d = *s) == 0) {
        return d;
    }
    return func_8002A5B4(d + 1, s + 1);
}

s8 *func_8002A5DC(s8 *buf, s8 pad, s32 n, s32 width) {
    s8 *q;
    s8 *r;
    s32 cnt;
    s8 c;

    buf += width;
    q = buf;
    *buf = 0;
    cnt = 0;
    do {
        q--;
        c = n % 10 + '0';
        *q = c;
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
        if (++cnt % 3 == 0) {
            if (n == 0) {
                break;
            }
            *--q = ',';
            if (--width <= 0) {
                buf++;
                for (r = buf; q < r; r--) {
                    *r = r[-1];
                }
                q++;
            }
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = pad;
    }
    return buf;
}

s8 *func_8002A710(s8 *buf, s8 pad, s32 n, s32 width) {
    s8 *q;
    s8 *r;

    buf += width;
    q = buf;
    *buf = 0;
    do {
        *--q = n % 10 + '0';
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = pad;
    }
    return buf;
}

void func_8002A7CC(s8 *arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 0) {
        *arg0++ = '+';
    } else {
        *arg0++ = '-';
        arg1 = -arg1;
    }
    func_8002A710(arg0, '0', arg1, arg2 - 1);
}

s8 *func_8002A820(s8 *buf, s32 n) {
    s8 *s;

    switch (n) {
    case 1:
        s = "1ab";
        break;
    case 2:
        s = "2cd";
        break;
    case 3:
        s = "3ef";
        break;
    default:
        if (n < 10) {
            return func_8002A5B4(func_8002A710(buf, ' ', n, 1), "gh");
        }
        return func_8002A5B4(func_8002A710(buf, ' ', n, 2), "i");
    }
    return func_8002A5B4(buf, s);
}

s32 strlen(u8 *);

s8 *func_8002A8D4(s8 *arg0, s8 *arg1, s32 arg2) {
    s32 pad;
    s32 i;
    s8 *p;

    pad = arg2 - strlen(arg1);
    if (pad < 0) {
        p = arg0;
        for (i = 0; i < arg2; i++) {
            *p++ = '*';
        }
    } else {
        pad /= 2;
        p = arg0;
        while (pad-- > 0) {
            *p++ = ' ';
            arg2--;
        }
        while ((*p = *arg1++) != 0) {
            p++;
            arg2--;
        }
        while (arg2-- > 0) {
            *p++ = ' ';
        }
    }
    *p = 0;
    return p;
}

u16 func_8002A9D4(u8 **ps) {
    u8 *s = *ps;

    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            *ps += 2;
            return ((*ps)[-2] << 8) | (*ps)[-1];
        }
    }
    return *(*ps)++;
}

u16 func_8002AA8C(u8 *s) {
    if (s[0] > 0x80 && (s[0] < 0xA0 || (s[0] >= 0xE0 && s[0] < 0xF0))) {
        if (s[1] >= 0x40 && (s[1] < 0x7F || (s[1] >= 0x80 && s[1] < 0xFD))) {
            return (s[0] << 8) | s[1];
        }
    }
    return s[0];
}

s32 func_8002AB20(s16 *s) {
    s16 *p;
    s32 w;

    p = s;
    w = 0;
loop:
    p++;
    if (*p != 0) {
        if (*p < 0) {
            w += 1;
        } else {
            w += 2;
        }
        goto loop;
    }
    *s = w;
    return w;
}

s16 *func_8002AB5C(s16 *d, u8 *s) {
    if ((*d = -*s) == 0) {
        return d;
    }
    s++;
    d++;
    return func_8002AB5C(d, s);
}

s16 *func_8002AB84(s16 *d, s16 *s) {
    if ((*d = *s) == 0) {
        return d;
    }
    return func_8002AB84(d + 1, s + 1);
}

s16 *func_8002ABAC(s16 *buf, u8 pad, s32 n, s32 width) {
    s16 *q;
    s16 *r;
    s32 fill;

    fill = -pad;
    buf += width;
    q = buf;
    *buf = 0;
    do {
        *--q = -'0' - n % 10;
        n /= 10;
        if (--width <= 0 && n != 0) {
            buf++;
            for (r = buf; q < r; r--) {
                *r = r[-1];
            }
            q++;
        }
    } while (n != 0);
    while (--width >= 0) {
        *--q = fill;
    }
    return buf;
}

void func_8002AC70(s16 *arg0, s32 arg1, s32 arg2) {
    if (arg1 >= 0) {
        *arg0++ = -0x2B;
    } else {
        *arg0++ = -0x2D;
        arg1 = -arg1;
    }
    func_8002ABAC(arg0, 0x30, arg1, arg2 - 1);
}

s16 *func_8002ACC4(s16 *buf, s32 n) {
    u8 *s;

    switch (n) {
    case 1:
        s = "1st";
        break;
    case 2:
        s = "2nd";
        break;
    case 3:
        s = "3rd";
        break;
    default:
        return func_8002AB5C(func_8002ABAC(buf, ' ', n, 1), "th");
    }
    return func_8002AB5C(buf, s);
}

s8 *func_8002AD58(s8 *buf, s32 n) {
    s8 *s;

    switch (n) {
    case 1:
        s = "1ST";
        break;
    case 2:
        s = "2ND";
        break;
    case 3:
        s = "3RD";
        break;
    default:
        return func_8002A5B4(func_8002A710(buf, ' ', n, 1), "TH");
    }
    return func_8002A5B4(buf, s);
}

s32 SsSetMVol(s32, s32);
s32 SsSetTableSize(s32 *, s32, s32);
s32 SsSetTickMode(s32);
s32 SsStart();
void func_8002AEA4(s32);
void func_8002B258(s32 arg0);
s32 func_80055740();
extern s32 D_801D6B28;

extern s32 D_801D8128;
void func_8002ADEC(void) {
    s8 *p;

    SsSetTableSize(&D_801D6B28, 0x20, 1);
    SsSetMVol(0, 0);
    SsSetTickMode(1);
    SsStart();
    func_8002B258(1);
    func_80055740();
    p = (s8 *)&D_801D8128;
    *(void **)(p + 0x1C) = func_8001ABCC(0x2100, -2);
    *(void **)(p + 0x28) = func_8001ABCC(0x9300, -2);
    *(void **)(p + 0x34) = func_8001ABCC(0x9300, -2);
    *(s16 *)(p + 0x2C) = 0xFF;
    *(s16 *)(p + 0x20) = 0xFF;
    *(s16 *)(p + 0x14) = 0xFF;
    *(s16 *)(p + 2) = -1;
    func_8002AEA4(1);
    SsSetMVol(0x7F, 0x7F);
}

extern s32 D_8006DFFC;
extern s8 *D_8006E000[];
extern s16 D_8006E048;
void SsVabClose(s16);
void bcopy(void *, void *, s32);
s32 func_8001B248(s32 *, s32, s32);
void func_8002B668(void);
s32 func_8002B300(void *, s16, s32);
void func_8002B38C(void *, s32, s32);

void func_8002AEA4(s32 id) {
    char name[32];
    u8 *pak;
    SndSlot *se;

    se = &((SndState *)&D_801D8128)->unk14;
    if (se->id != id) {
        while (D_8006DFFC != 0) {
            func_80014C08(D_800794F0);
        }
        D_8006DFFC = 1;
        if (se->id != 0xFF) {
            func_8002B668();
            SsVabClose(se->vab);
        }
        se->id = id;
        /* written as a word here, read as a halfword by the SFX players */
        *(s32 *)&D_8006E048 = D_8006E000[id][0xF];
        sprintf(name, "A:\\SE%d.PAK", id);
        pak = (u8 *)func_8001B248((s32 *)name, func_800148B0(), -2);
        if (pak == 0) {
            se->id = 0xFF;
        } else {
            bcopy(pak, se->buf, 0x2030);
            if (func_8002B300(se, 0, 0x1010) != 0) {
                func_8002B38C(se, (s32)func_8001BB44((Chunk *)pak, 8, se->id), se->vab);
            } else {
                se->id = 0xFF;
            }
            func_8001AE90(pak);
        }
        D_8006DFFC = 0;
    }
}

void func_8002B2C0(void);
void func_8002B688(void);
s16 SsSeqOpen(u8 *, s16);
void SsSeqClose(s16);

void func_8002B024(s32 n, s32 id, u8 vol) {
    char name[32];
    u8 *pak;
    SndSlot *sl;

    sl = &((SndState *)&D_801D8128)->slot[n];
    if (sl->id == id) {
        return;
    }
    while (D_8006DFFC != 0) {
        func_80014C08(D_800794F0);
    }
    D_8006DFFC = 1;
    func_8002B2C0();
    if (sl->id != 0xFF) {
        if (((SndState *)&D_801D8128)->cur == n) {
            func_8002B688();
        }
        SsSeqClose(((SndState *)&D_801D8128)->seq[n]);
        SsVabClose(sl->vab);
        func_80014C08(D_800794F0);
    }
    sl->id = id;
    ((SndState *)&D_801D8128)->vol[n] = vol;
    sprintf(name, "A:\\BGM\\BGM%02d.PAK", id);
    pak = (u8 *)func_8001B248((s32 *)name, func_800148B0(), -2);
    if (pak == 0) {
        sl->id = 0xFF;
    } else {
        bcopy(pak, sl->buf, 0x9210);
        if (func_8002B300(sl, n + 1, n * 0x1A300 + 0x49E90) == 0) {
            func_8001AE90(pak);
            sl->id = 0xFF;
        } else {
            func_8002B38C(sl, (s32)func_8001BB44((Chunk *)pak, 8, sl->id), sl->vab);
            ((SndState *)&D_801D8128)->data[n] = func_8001BB44((Chunk *)sl->buf, 6, sl->id);
            ((SndState *)&D_801D8128)->seq[n] = SsSeqOpen(((SndState *)&D_801D8128)->data[n], sl->vab);
            func_8001AE90(pak);
        }
    }
    D_8006DFFC = 0;
}

s32 SpuClearReverbWorkArea(s32);
s32 SsUtSetReverbDepth(s32, s32);
s32 SsUtSetReverbType(s16);
s32 func_80051C70();
s32 func_80051C90();

void func_8002B258(s32 arg0) {
    if (arg0 == 0) {
        func_80051C70();
        SsUtSetReverbType(0);
        SsUtSetReverbDepth(0, 0);
        SpuClearReverbWorkArea(0);
        return;
    }
    SsUtSetReverbType((s16) arg0);
    func_80051C90();
    SsUtSetReverbDepth(0x64, 0x64);
}

void SpuSetVoiceAttr(SpuVoiceAttr *);

void func_8002B2C0(void) {
    SpuVoiceAttr attr;

    attr.mask = 0x4000;
    attr.voice = 0xFFFFFF;
    attr.rr = 0;
    SpuSetVoiceAttr(&attr);
    VSync(0);
}

s16 SsVabOpenHeadSticky(u8 *, s16, s32);

s32 func_8002B300(void *arg0, s16 arg1, s32 arg2) {
    u8 *vh;

    vh = func_8001BB44(*(Chunk **)((s8 *)arg0 + 8), 7, (*(s16 *)((s8 *)arg0 + 0)));
    if (vh != 0) {
        (*(s32 *)((s8 *)arg0 + 4)) = (*(s32 *)(vh - 4));
        if (((*(s16 *)((s8 *)arg0 + 2)) = SsVabOpenHeadSticky(vh, arg1, arg2)) != -1) {
            return 1;
        }
    }
    return 0;
}

s32 SsVabTransBody(s32, s16);
s32 SsVabTransCompleted(s32);

void func_8002B38C(void *arg0, s32 arg1, s32 vab) {
    if ((arg1 == 0) || (SsVabTransBody(arg1, (*(s16 *)((s8 *)arg0 + 2))) == (*(s16 *)((s8 *)arg0 + 2)))) {
        SsVabTransCompleted(1);
    }
}

void func_8002B3DC(void) {
}

void func_8002B3E4(void) {
}

s32 func_80014A48();
s32 func_801DFBAC(s32 *);
s32 func_801E055C(s32);
extern s32 D_80010598;
extern s32 D_800105A8;
extern s32 D_80010C9C;
void func_8001B358();

void func_8002B3EC(s32 arg0, s32 arg1) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, &func_8001B358, &D_80010598, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_801DFBAC(&D_800105A8);
    func_801E055C(arg0);
    func_80014A48(arg1);
}

extern short SsUtKeyOnV(short voice, short vabId, short prog, short tone,
                        short note, short fine, short voll, short volr);
extern s32 D_8006E044;
extern s16 D_8006E048;
extern u16 D_8006E04C;
extern s16 D_801D813E;

void func_8002B498(s32 arg0) {
    s32 tone = arg0 & 0xF;

    SsUtKeyOnV(D_8006E044, D_801D813E, arg0 >> 4, tone, D_8006E048,
               D_8006E04C + tone, 0x6E, 0x6E);
    if (++D_8006E044 >= 0x16) {
        D_8006E044 = 0x12;
    }
}

void func_8002B530(s32 arg0, s32 vol) {
    s32 tone = arg0 & 0xF;

    SsUtKeyOnV(D_8006E044, D_801D813E, arg0 >> 4, tone, D_8006E048,
               D_8006E04C + tone, vol, vol);
    if (++D_8006E044 >= 0x16) {
        D_8006E044 = 0x12;
    }
}

void func_8002B5D0(s32 arg0, s32 arg1) {
    s32 tone = arg1 & 0xF;

    SsUtKeyOnV(arg0, D_801D813E, arg1 >> 4, tone, D_8006E048,
               D_8006E04C + tone, 0x6E, 0x6E);
}

s32 SsUtKeyOffV(s16);

void func_8002B644(s16 arg0) {
    SsUtKeyOffV(arg0);
}

s32 SsUtAllKeyOff(s32);

void func_8002B668(void) {
    SsUtAllKeyOff(0);
}

s32 SsSeqStop(s16);
extern s32 D_801D8128;

void func_8002B688(void) {
    s16 *p;

    func_80014A00(0x1C);
    p = (s16 *)&D_801D8128;
    if (((s16 *)&D_801D8128)[1] >= 0) {
        SsSeqStop(((s16 *)&D_801D8128)[p[1] + 2]);
        func_80014C08(4);
        ((s16 *)&D_801D8128)[1] = -1;
    }
}

void SsSeqGetVol(s16, s16, s16 *, s16 *);
s32 SsSeqStop(s16);
void SsSeqSetVol(s16, s16, s16);

void func_8002B6E4(s32 idx, s32 step) {
    s16 vl;
    s16 vr;

    for (;;) {
        func_80014C08(D_800794F0);
        if (((SndState *)&D_801D8128)->cur != idx) {
            func_80014A90();
        }
        SsSeqGetVol(((SndState *)&D_801D8128)->seq[idx], 0, &vl, &vr);
        if (vl == 0) {
            SsSeqStop(((SndState *)&D_801D8128)->seq[idx]);
            func_80014C08(4);
            ((SndState *)&D_801D8128)->cur = -1;
            func_80014A90();
        }
        vl -= step;
        if (vl < 0) {
            vl = 0;
        }
        SsSeqSetVol(((SndState *)&D_801D8128)->seq[idx], vl, vl);
    }
}

void func_8002B7DC(s32 arg0) {
    s16 *p = (s16 *)&D_801D8128;

    if (p[1] >= 0) {
        func_80014A00(0x1C);
        func_800149B8(0x1C, -1, 0, 0x1000, &func_8002B6E4, p[1], arg0);
    }
}

void func_8002B850(void) {
}

void SsSeqPlay(s16, char, s16);
void SsSeqSetVol(s16, s16, s16);

void func_8002B858(s32 arg0) {
    if (((SndState *)&D_801D8128)->slot[arg0].id != 0xFF) {
        if (((SndState *)&D_801D8128)->cur >= 0) {
            func_8002B688();
        }
        SsSeqPlay(((SndState *)&D_801D8128)->seq[arg0], 1, 0);
        SsSeqSetVol(((SndState *)&D_801D8128)->seq[arg0],
                    ((SndState *)&D_801D8128)->vol[arg0],
                    ((SndState *)&D_801D8128)->vol[arg0]);
        ((SndState *)&D_801D8128)->cur = arg0;
    }
}

extern s32 D_8006E03C;
extern s32 D_8006E040;
void func_8002B024(s32, s32, u8);

void func_8002B900(s32 seq, s32 arg1, s32 arg2, s32 load) {
    D_8006E040++;
    do {
        func_80014C08(D_800794F0);
    } while (D_8006E03C != 0);
    D_8006E03C = 1;
    if (((SndState *)&D_801D8128)->cur >= 0) {
        func_8002B7DC(2);
        while (((SndState *)&D_801D8128)->cur >= 0) {
            func_80014C08(D_800794F0);
        }
    }
    if (load) {
        func_8002B024(seq, arg1, arg2);
    }
    func_8002B858(seq);
    D_8006E03C = 0;
    D_8006E040--;
    func_80014A90();
}

s32 func_80014C08(s32);
extern s32 D_8006E040;
extern s32 D_800794F0;

void func_8002BA24(void) {
    do {
        func_80014C08(D_800794F0);
    } while (D_8006E040 != 0);
}

extern s16 D_801D812A;

void func_8002BA6C(s32 arg0, s32 arg1, s32 arg2) {
    s8 *base;

    base = (s8 *)&D_801D8128;
    if ((*(s16 *)(base + arg0 * 0xC + 0x20)) != arg1) {
        func_8002BA24();
        func_800149B8(0, -1, 0, 0x1000, &func_8002B900, arg0, arg1, arg2, 1);
        return;
    }
    if (D_801D812A != arg0) {
        func_8002BA24();
        func_800149B8(0, -1, 0, 0x1000, &func_8002B900, arg0, arg1, arg2, 0);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010598);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800105A8);

extern s16 D_801D813C;

void func_8002BB58(u32 arg0) {
    s32 var_a0;

    var_a0 = 0;
    if (D_801D813C == 0) {
        switch (arg0) {
        case 0:
            var_a0 = 0xA1;
            break;
        case 1:
            var_a0 = 0xA0;
            break;
        case 2:
            var_a0 = 0xA2;
            break;
        case 3:
            var_a0 = 0xA3;
            break;
        case 4:
            var_a0 = 0xA4;
            break;
        }
    } else {
        switch (arg0) {
        case 0:
            var_a0 = 1;
            break;
        case 1:
            var_a0 = 0;
            break;
        case 2:
            var_a0 = 2;
            break;
        case 3:
            var_a0 = 3;
            break;
        case 4:
            var_a0 = 4;
            break;
        }
    }
    func_8002B498(var_a0);
}

extern s32 D_8006E03C;
extern s32 D_8006E040;

s32 func_8002BC2C(void) {
    s32 var_v1;

    var_v1 = 0;
    if (D_8006E03C == 0) {
        var_v1 = D_8006E040 == 0;
    }
    return var_v1;
}

s32 InitCARD(s32);
void func_8002BC80(void);

void func_8002BC58(void) {
    InitCARD(0);
    func_8002BC80();
}

long func_8006A794(unsigned long, long, long, long (*)());
long func_8006A7C4(long);
void StartCARD(void);
void func_80068804(void);
extern s32 D_801D8160;
extern s32 D_801D8164;
extern s32 D_801D8168;
extern s32 D_801D816C;
extern s32 D_801D8170;
extern s32 D_801D8174;
extern s32 D_801D8178;
extern s32 D_801D817C;
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

extern CardDir *D_801D8190[2];
extern u8 *D_801D81A0;

void func_8002BC80(void) {
    s32 i;

    VSync(2);
    D_801D8160 = func_8006A794(0xF4000001, 4, 0x2000, 0);
    D_801D8164 = func_8006A794(0xF4000001, 0x8000, 0x2000, 0);
    D_801D8168 = func_8006A794(0xF4000001, 0x100, 0x2000, 0);
    D_801D816C = func_8006A794(0xF4000001, 0x2000, 0x2000, 0);
    D_801D8170 = func_8006A794(0xF0000011, 4, 0x2000, 0);
    D_801D8174 = func_8006A794(0xF0000011, 0x8000, 0x2000, 0);
    D_801D8178 = func_8006A794(0xF0000011, 0x100, 0x2000, 0);
    D_801D817C = func_8006A794(0xF0000011, 0x2000, 0x2000, 0);
    StartCARD();
    func_80068804();
    func_8006A7C4(D_801D8160);
    func_8006A7C4(D_801D8164);
    func_8006A7C4(D_801D8168);
    func_8006A7C4(D_801D816C);
    func_8006A7C4(D_801D8170);
    func_8006A7C4(D_801D8174);
    func_8006A7C4(D_801D8178);
    func_8006A7C4(D_801D817C);
    for (i = 0; i < 2; i++) {
        D_801D8190[i] = func_8001ACEC(0x260);
    }
    D_801D81A0 = func_8001ACEC(0x200);
}

s32 func_8006A7B4(s32);
extern s32 D_801D8198;
extern s32 D_801D8160;
extern s32 D_801D8164;
extern s32 D_801D8168;
extern s32 D_801D816C;
extern s32 D_801D8170;
extern s32 D_801D8174;
extern s32 D_801D8178;
extern s32 D_801D817C;
s32 func_8002BE84(s32 arg0) {
    s32 n;

    n = 0;
    D_801D8198 = 0;
    do {
        if (func_8006A7B4(D_801D8160) == 1) {
            return 0;
        }
        if (func_8006A7B4(D_801D8164) == 1) {
            return 1;
        }
        if (func_8006A7B4(D_801D8168) == 1) {
            return 2;
        }
        if (func_8006A7B4(D_801D816C) == 1) {
            return 3;
        }
        if (arg0 != 0) {
            if (n++ >= 0x1F) {
                break;
            }
            func_80014C08(arg0);
        }
    } while (D_801D8198 < 0x259);
    return 2;
}

s32 func_8006A7B4(s32);
extern s32 D_801D8160;
extern s32 D_801D8164;
extern s32 D_801D8168;
extern s32 D_801D816C;

void func_8002BF60(void) {
    func_8006A7B4(D_801D8160);
    func_8006A7B4(D_801D8164);
    func_8006A7B4(D_801D8168);
    func_8006A7B4(D_801D816C);
}

s32 func_8002BFB8(s32 arg0) {
    s32 n;

    n = 0;
    D_801D8198 = 0;
    do {
        if (func_8006A7B4(D_801D8170) == 1) {
            return 0;
        }
        if (func_8006A7B4(D_801D8174) == 1) {
            return 1;
        }
        if (func_8006A7B4(D_801D8178) == 1) {
            return 2;
        }
        if (func_8006A7B4(D_801D817C) == 1) {
            return 3;
        }
        if (arg0 != 0) {
            if (n++ >= 0x1F) {
                break;
            }
            func_80014C08(arg0);
        }
    } while (D_801D8198 < 0x259);
    return 2;
}

s32 func_8006A7B4(s32);
extern s32 D_801D8170;
extern s32 D_801D8174;
extern s32 D_801D8178;
extern s32 D_801D817C;

void func_8002C094(void) {
    func_8006A7B4(D_801D8170);
    func_8006A7B4(D_801D8174);
    func_8006A7B4(D_801D8178);
    func_8006A7B4(D_801D817C);
}

s32 _card_clear(s32);
s32 func_8002BE84(s32);
s32 func_8002BFB8(s32);
s32 func_80068814(s32);
s32 func_80068824(s32);

s32 func_8002C0EC(s32 arg0) {
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s0;

    var_s0 = 0;
loop_1:
    func_8002BF60();
    func_80068814(arg0 * 0x10);
    temp_v0 = func_8002BE84(0);
    if ((u32) (temp_v0 - 1) < 2U) {
        if (var_s0 >= 5) {
            return 1;
        }
        goto block_6;
    }
    if (temp_v0 == 3) {
        if (var_s0 < 3) {
block_6:
            var_s0 += 1;
            func_80014C08(D_800794F0);
            goto loop_1;
        }
        if (temp_v0 == 3) {
            temp_s0 = arg0 * 0x10;
            func_8002C094();
            _card_clear(temp_s0);
            func_8002BFB8(1);
            func_8002BF60();
            func_80068824(temp_s0);
            func_8002BE84(0);
        }
        /* Duplicate return node #9. Try simplifying control flow for better match */
        return 0;
    }
    return 0;
}

s32 func_8002C1C0(s32 port) {
    s32 r;
    s32 tries;
    s32 retry;

    tries = 0;
    retry = 0;
loop:
    func_8002BF60();
    func_80068814(port * 16);
    r = func_8002BE84(1);
    if (r == 1 || r == 2) {
        if (retry >= 5) {
            return 1;
        }
        retry++;
    } else {
        if (r == 3) {
            if (retry < 3) {
                retry++;
                goto wait;
            }
            retry++;
            if (r == 3) {
                func_8002C094();
                _card_clear(port * 16);
                r = func_8002BFB8(1);
                if (r == 1 || r == 2) {
                    retry = 0;
                    if (tries >= 5) {
                        return 1;
                    }
                    tries++;
                    goto wait;
                }
            }
        }
        func_8002BF60();
        func_80068824(port * 16);
        r = func_8002BE84(0);
        if (r == 0) {
            goto done;
        }
        retry = 0;
        if (tries >= 5) {
            if (r == 3) {
                return 2;
            }
            return 1;
        }
        tries++;
    }
wait:
    func_80014C08(4);
    goto loop;
done:
    return 0;
}

s32 _card_format(s32);

s32 func_8002C2E4(s32 arg0) {
    return _card_format(arg0 * 0x10) == 1;
}

s32 func_8006A824(char *, s32);
s32 func_8006A864(s32);
extern char D_800105E4;
extern s32 D_801D8180;
extern s32 D_801D8184;
extern s32 D_801D8188;
extern u8 *D_801D81A0;

typedef struct {
    u8 data[0x200];
} McHeader;

s32 func_8002C30C(s32 slot, u8 blocks, s32 arg2, s32 arg3, McHeader *hdr) {
    char name[32];
    s32 fd;

    ((u8 *)hdr)[3] = blocks;
    sprintf(name, &D_800105E4, slot, arg3);
    func_8006A864(func_8006A824(name, (((u8 *)hdr)[3] << 16) | 0x200));
    *(McHeader *)D_801D81A0 = *hdr;
    D_801D8184 = fd = func_8006A824(name, 0x8002);
    if (fd == -1) {
        return -1;
    }
    D_801D8180 = 0;
    D_801D8188 = arg2;
    if (func_8002C0EC(slot) != 0) {
        return -1;
    }
    return 0;
}

s32 func_8006A834(s32, s32, s32);
s32 func_8006A854(s32, void *, s32);
extern s32 D_801D818C;
extern s32 D_801D819C;

s32 func_8002C468(void) {
    s32 r;
    s32 start;
    s32 end;
    s32 off;

    func_8002BF60();
    switch (D_801D8180) {
    case 0:
        start = D_801D81A0[2] * 128 - 0x780;
        func_8006A834(D_801D8184, 0, 0);
        if (func_8006A854(D_801D8184, D_801D81A0, start) == -1) {
            return -1;
        }
        D_801D8180++;
    case 1:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        start = D_801D81A0[2] * 128 - 0x780;
        end = D_801D81A0[3] * 0x2000;
        D_801D819C = (end - start) / 128;
        D_801D818C = 0;
        D_801D8180++;
        return 0;
    case 2:
        func_8006A834(D_801D8184, ((D_801D81A0[2] - 0x10) << 7) + 0x80 + (D_801D818C << 7), 0);
        if (func_8006A854(D_801D8184, (void *)(D_801D8188 + (D_801D818C << 7)), 0x80) == -1) {
            return -1;
        }
        D_801D8180++;
    case 3:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        D_801D818C++;
        D_801D8180 = 2;
        if (D_801D818C == D_801D819C) {
            func_8006A864(D_801D8184);
        }
        break;
    }
    return D_801D818C * 100 / D_801D819C;
}

s32 func_8006A824(char *, s32);
extern char D_800105E4;
extern s32 D_801D8180;
extern s32 D_801D8184;
extern s32 D_801D8188;

s32 func_8002C6EC(s32 slot, s32 arg1, s32 arg2) {
    char name[32];
    s32 fd;

    sprintf(name, &D_800105E4, slot, arg2);
    D_801D8184 = fd = func_8006A824(name, 0x8001);
    if (fd == -1) {
        return -1;
    }
    D_801D8180 = 0;
    D_801D8188 = arg1;
    if (func_8002C0EC(slot) != 0) {
        return -1;
    }
    return 0;
}

s32 func_8006A834(s32, s32, s32);
s32 func_8006A844(s32, void *, s32);
extern s32 D_801D818C;
extern s32 D_801D819C;

s32 func_8002C784(void) {
    s32 r;
    s32 start;
    s32 end;
    s32 off;

    func_8002BF60();
    switch (D_801D8180) {
    case 0:
        func_8006A834(D_801D8184, 0, 0);
        if (func_8006A844(D_801D8184, D_801D81A0, 0x80) == -1) {
            return -1;
        }
        D_801D8180++;
    case 1:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        start = D_801D81A0[2] * 128 - 0x780;
        end = D_801D81A0[3] * 0x2000;
        D_801D819C = (end - start) / 128;
        D_801D818C = 0;
        D_801D8180++;
        return 0;
    case 2:
        func_8006A834(D_801D8184, ((D_801D81A0[2] - 0x10) << 7) + 0x80 + (D_801D818C << 7), 0);
        if (func_8006A844(D_801D8184, (void *)(D_801D8188 + (D_801D818C << 7)), 0x80) == -1) {
            return -1;
        }
        D_801D8180++;
    case 3:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        D_801D818C++;
        D_801D8180 = 2;
        if (D_801D818C == D_801D819C) {
            func_8006A864(D_801D8184);
        }
        break;
    }
    return D_801D818C * 100 / D_801D819C;
}

s32 func_8006A834(s32, s32, s32);
s32 func_8006A844(s32, void *, s32);
s32 func_8006A864(s32);
extern u8 *D_801D81A0;

s32 func_8002C9E8(s32 arg0, void *arg1, s32 arg2) {
    char name[32];
    s32 fd;

    sprintf(name, &D_800105E4, arg0, arg2);
    fd = func_8006A824(name, 1);
    if (fd == -1) {
        return 1;
    }
    if (func_8006A844(fd, D_801D81A0, 0x80) == -1) {
        func_8006A864(fd);
        return 1;
    }
    if (func_8006A834(fd, ((*(u8 *)((s8 *)D_801D81A0 + 2)) - 0x10) << 7, 1) == -1) {
        func_8006A864(fd);
        return 1;
    }
    if (func_8006A844(fd, arg1, 0x80) == -1) {
        func_8006A864(fd);
        return 1;
    }
    func_8006A864(fd);
    return 0;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800105E4);

DirEntry *firstfile(char *, DirEntry *);
DirEntry *func_8006A874(DirEntry *);

void func_8002CAC8(s32 port) {
    char name[8];
    DirEntry *d;
    s32 count;
    s32 total;

    count = 0;
    total = 0;
    sprintf(name, "bu%1d0:*", port);
    d = D_801D8190[port]->files;
    if (firstfile(name, d) == d) {
        do {
            total += d->size;
            count++;
            d++;
        } while (func_8006A874(d) == d);
    }
    D_801D8190[port]->count = count;
    D_801D8190[port]->blocks = total /= 8192;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800105FC);


s32 func_8002CBA0(s32 len, u8 *p) {
    s32 i;
    u8 x = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        x ^= *p;
        sum += *p;
        p++;
    }
    if (p[0] != x || p[1] != sum) {
        return 1;
    }
    return 0;
}

void func_8002CC04(s32 len, u8 *p) {
    s32 i;
    u8 x = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        x ^= *p;
        sum += *p;
        p++;
    }
    p[0] = x;
    p[1] = sum;
}

extern u8 D_8006E0B8[6];
extern u8 *D_801D8408;

#define PLAYER_DATA(p) (((Unk8006E050 *)D_8006E050)[p])

void func_8002CC44(s32 p) {
    s32 count[6];
    s32 total;
    s32 rank;
    s32 i;
    s32 n;

    rank = PLAYER_DATA(p).rankA;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(p).unk18 < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(p).unk18 < 25) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(p).unk18 < 50) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(p).unk18 < 100) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(p).unk18 < 200) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(p).unk18 < 300) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(p).unk18 < 500) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(p).rankA = rank;

    total = 0;
    for (i = 0; i < 6; i++) {
        count[i] = 0;
    }
    for (i = 0; i < 0xAC; i++) {
        n = PLAYER_DATA(p).unk14B2[i] & 7;
        if (n != 0) {
            total += n;
            count[D_801D8408[i * 0x13C + 0x1A] >> 4]++;
        }
    }
    for (i = 0xBF; i < 0x125; i++) {
        n = PLAYER_DATA(p).unk14B2[i] & 7;
        if (n != 0) {
            total += n;
            count[5]++;
        }
    }
    for (i = 0x125; i < 0x12D; i++) {
        n = PLAYER_DATA(p).unk14B2[i] & 7;
        if (n != 0) {
            total += n;
            count[5]++;
        }
    }
    n = 0;
    for (i = 0; i < 6; i++) {
        if (count[i] == D_8006E0B8[i]) {
            n++;
        }
    }

    rank = PLAYER_DATA(p).rankB;
    switch (rank) {
    case 0:
        if (total < 100) {
            break;
        }
        rank = 1;
    case 1:
        if (total < 200) {
            break;
        }
        rank = 2;
    case 2:
        if (n <= 0) {
            break;
        }
        rank = 3;
    case 3:
        if (n < 3) {
            break;
        }
        rank = 4;
    case 4:
        if (n < 5) {
            break;
        }
        rank = 5;
    case 5:
        if (n < 6) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(p).unk28_11) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(p).rankB = rank;

    rank = PLAYER_DATA(p).rankC;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(p).unk1C < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(p).unk1C < 20) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(p).unk1C < 30) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(p).unk1C < 40) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(p).unk1C < 60) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(p).unk1C < 80) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(p).unk1C < 100) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(p).rankC = rank;
}

void func_8002D51C(void);
void func_800457FC();

extern void *D_8006E054;
void func_8002D404(void) {
    void *p;

    func_800457FC();
    D_8006E050 = func_8001ACEC(0x4EE8);
    D_8006E054 = p = func_8001ACEC(0x102C);
    (*(void **)((s8 *)D_8006E054 + 0x100C)) = func_8001ACEC(0x1AC);
    func_8002D51C();
}

void func_8002D458(void) {
    s32 i;

    ((Unk8006E054 *)D_8006E054)->unk1027 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A4 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A2 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A9 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A8 = 0;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)D_8006E050)->unk23FC[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        ((Unk8006E050 *)D_8006E050)->unk242C[i] = 0;
    }
    ((Unk8006E050 *)D_8006E050)->unk2C = 0;
    ((Unk8006E050 *)D_8006E050)->unk14 = 0;
}

void func_80045968(s32 a, s32 row, s32 n);
char *strcpy(char *, const char *);

void func_8002D51C(void) {
    Unk8006E050 *e;
    s32 p;
    s32 j;
    s32 i;

    e = (Unk8006E050 *)D_8006E050;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)D_8006E050)->unk23FC[i] = 0;
    }
    ((Unk8006E050 *)D_8006E050)->unk28_9 = 0;
    for (p = 0; p < 2; p++, e++) {
        e->name[0] = 0;
        e->unk18 = 0;
        e->unk1A = 0;
        e->unk1C = 0;
        e->unk1E = 0;
        e->unkE = 0;
        e->unk10 = rand();
        e->unk28_10 = 0;
        e->unk28_13 = 0;
        e->unkD = 0;
        e->unk28_11 = 0;
        e->unk28_12 = 0;
        e->rankA = 0;
        e->rankB = 0;
        e->rankC = 0;
        e->unk16 = 0x2774;
        e->unk4C = 0;
        e->unk4E = 0;
        e->unk50 = 0;
        e->unk52 = 0;
        e->unk54 = 0;
        e->unk56 = 0;
        for (i = 0; i < 3; i++) {
            e->unk36[i] = 0;
        }
        for (i = 0; i < 0x28; i++) {
            e->unk58[i] = 0;
        }
        for (i = 0; i < 0x12D; i++) {
            e->unk14B2[i] = 0;
            for (j = 0; j < 8; j++) {
                func_80045968(p, i, j);
            }
        }
        for (i = 0; i < 0xBF; i++) {
            for (j = 0; j < 3; j++) {
                e->unkD3C[i][j] = 0;
            }
            e->unk11B6[i] = 0;
            e->unk1334[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            e->unk80[i].unk288 = 0;
        }
        for (i = 0; i < 0x10; i++) {
            e->unk3C[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            e->unk2438[i].unk0 = 0;
            e->unk2438[i].unk108[0] = 0;
            e->unk2438[i].unk108[1] = 0;
            e->unk2438[i].unk108[2] = 0;
        }
        for (i = 0; i < 0x9F; i++) {
            e->unkAC0[i] = 0;
            e->unkBFE[i] = 0;
        }
        for (i = 0; i < 0x8E; i++) {
            e->unk888[i] = 0;
            e->unk9A4[i] = 0;
        }
        for (j = 0; j < 0x20; j++) {
            e->unk848[j] = 0;
        }
        e->unk20_0 = 0;
        e->unk20_1 = 0;
        e->unk20_2 = 0;
        e->unk20_3 = 0;
        e->unk24 = 0;
    }
    strcpy(((Unk8006E050 *)D_8006E050)->name, "Player");
    func_8002D458();
}

void func_8002D898(void) {
    CUR_SPRT->sp.x0 = 0;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x100;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x85);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
    CUR_SPRT->sp.x0 = 0x100;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x40;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x87);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
}

s32 func_80022D34(s32, s32, s32, s32);

void func_8002DAAC(s32 arg0, s32 arg1) {
    void *temp_s1;

    temp_s1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_s1 + 0x2200)) != arg1) {
        func_8001AFF0(arg0 + 0x84);
        func_80023094(temp_s1, (s32 *)func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s1 + 0x26F4), 1, arg1), arg0 + 0x84), arg1);
    }
    func_80022D34(arg0, arg1, -2, 0);
}

void func_80023148(s32, s32);

void func_8002DB58(s32 arg0, s32 arg1) {
    s32 temp_s2;
    void *temp_s3;

    temp_s3 = D_801D6A4C->unk13C[arg0];
    temp_s2 = arg0 + 0x84;
    func_8001AFF0(temp_s2);
    func_80023094(temp_s3, (s32 *)func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s3 + 0x26F4), 1, arg1), temp_s2), arg1);
    func_80023148(arg0, arg1);
}

extern u8 *D_801D8408;

void *func_8002DBEC(s32 arg0) {
    u8 *p;
    s32 i;

    p = D_801D8408;
    if (p[0xE5] != arg0) {
        i = 0;
        do {
            i++;
            p += 0x13C;
            if (i >= 0xBF) {
                break;
            }
        } while (p[0xE5] != arg0);
    }
    return p;
}

s32 func_8001B248(s32 *, s32, s32);
extern s32 D_800107F8;

s32 func_8002DC30(s32 arg0, s32 arg1) {
    char sp10[32];
    s32 var_v0;

    var_v0 = (s32)func_8001BB44((Chunk *)arg1, 2, arg0);
    if (var_v0 == 0) {
        sprintf(sp10, &D_800107F8, arg0);
        var_v0 = func_8001B248(sp10, func_800148B0(), 0x81);
    }
    return var_v0;
}

s32 func_8002DC30(s32, s32);

void func_8002DC90(s32 arg0) {
    func_8002DC30(arg0, 0);
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800107F8);

s32 func_8002DCB0(s32 slot, s32 id, s8 kind, s32 anims) {
    char path[32];
    s32 pak;

    if (kind == 1) {
        sprintf(path, "G:\\%03d.PAK", id);
    } else {
        sprintf(path, "F:\\%03d.PAK", id);
    }
    pak = func_8001B248((s32 *)path, func_800148B0(), slot + 0x1F4);
    if (func_8002386C(slot, id, -1, pak, kind) == 0) {
        return pak;
    }
    if (anims != 0) {
        func_800230B8(slot, 0, 0, pak);
        func_800230B8(slot, 7, 7, pak);
        func_800230B8(slot, 1, 1, pak);
        func_800230B8(slot, 2, 2, pak);
        func_800230B8(slot, 3, 3, pak);
        func_800230B8(slot, 4, 4, pak);
        func_800230B8(slot, 5, 5, pak);
        func_800230B8(slot, 6, 6, pak);
    } else {
        func_80023094(D_801D6A4C->unk13C[slot],
                      (s32 *)func_8001BFF8(
                          (s32)func_8001BB44((Chunk *)((Model2220 *)D_801D6A4C->unk13C[slot])->unk26F4, 1, 7), slot + 0x84),
                      7);
        func_80023148(slot, 7);
    }
    D_801D6A4C->unk114[slot] = -1;
    func_8001BC14((Chunk *)pak);
    return pak;
}

void *func_8002DBEC(s32);
s32 func_8002DCB0(s32, s32, s8, s32);
extern s32 D_801D81B8;

extern void *D_801D8340;

typedef struct {
    /* 0x0 */ s16 id;
    /* 0x2 */ u8 used;
    /* 0x3 */ u8 age;
} CardCache;

typedef struct {
    /* 0x000 */ u8 unk0[0x50];
    /* 0x050 */ Player *unk50;
    /* 0x054 */ Player *unk54;
    /* 0x058 */ u8 *unk58;
    /* 0x05C */ u8 unk5C[0x784];
    /* 0x7E0 */ CardCache cache[6];
    /* 0x7F8 */ u8 unk7F8[4];
    /* 0x7FC */ s32 unk7FC;
    /* 0x800 */ s32 unk800;
    /* 0x804 */ s32 unk804;
    /* 0x808 */ s16 fade;
    /* 0x80A */ s16 unk80A;
    /* 0x80C */ s16 unk80C;
    /* 0x80E */ s16 unk80E;
    /* 0x810 */ s8 state;
    /* 0x811 */ s8 unk811;
    /* 0x812 */ s8 unk812;
    /* 0x813 */ s8 unk813;
    /* 0x814 */ s8 unk814;
    /* 0x815 */ s8 unk815;
    /* 0x816 */ s8 unk816;
    /* 0x817 */ s8 unk817;
    /* 0x818 */ s8 unk818;
    /* 0x819 */ s8 unk819;
    /* 0x81A */ s8 unk81A;
    /* 0x81B */ u8 unk81B;
    /* 0x81C */ s8 unk81C;
    /* 0x81D */ s8 unk81D;
    /* 0x81E */ u8 unk81E;
    /* 0x81F */ s8 unk81F;
    /* 0x820 */ u8 unk820[2];
    /* 0x822 */ s8 unk822;
    /* 0x823 */ s8 unk823;
    /* 0x824 */ s8 unk824;
    /* 0x825 */ u8 unk825;
    /* 0x826 */ u8 unk826;
    /* 0x827 */ u8 unk827;
    /* 0x828 */ u8 unk828[0x14];
    /* 0x83C */ s32 unk83C;
} Duel;

#define DUEL ((Duel *)D_801D8340)
void func_8002DEA0(s32 arg0, void *arg1) {
    s32 temp_s5;
    s32 temp_s6;
    s32 temp_v0;
    void *temp_s1;
    void *temp_s2;

    temp_s1 = (void *)((s8 *)&D_801D81B8 + (arg0 << 5));
    temp_s5 = (*(u8 *)((s8 *)arg1 + 0xE5));
    temp_s6 = (*(s32 *)((s8 *)temp_s1 + 0));
    if (temp_s5 != temp_s6) {
        (*(s32 *)((s8 *)temp_s1 + 0)) = -2;
        if ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1) {
            do {
                func_80014C08(D_800794F0);
            } while ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1);
        }
        (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 1;
        if (temp_s6 > 0) {
            func_800235C8(arg0);
            func_8001AFF0(arg0 + 0x1F4);
            func_8001AFF0(arg0 + 0x84);
        }
        if (temp_s5 > 0) {
            temp_s2 = func_8002DBEC(temp_s5);
            temp_v0 = func_8002DCB0(arg0, temp_s5, 0, 0);
            if (temp_v0 != 0) {
                (*(s32 *)((s8 *)temp_s1 + 8)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x22)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0xC)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x3E)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x10)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x5A)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x14)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x24)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x18)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x40)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x1C)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x5C)), temp_v0);
                goto block_9;
            }
        } else {
block_9:
            (*(s32 *)((s8 *)temp_s1 + 0)) = temp_s5;
            (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
        }
    }
}

extern u8 *D_801D8348[];
extern void *D_801D81AC;
extern void *D_801D81B0;
void func_8002E7B8(void);
void func_8002E42C(s32 n);

void func_8002E034(s32 bg) {
    s32 pak;
    s32 i;
    u8 *s;

    if (*((s8 *)D_801D8340 + 0x811) == 1) {
        do {
            func_80014C08(D_800794F0);
        } while (*((s8 *)D_801D8340 + 0x811) == 1);
    }
    *((s8 *)D_801D8340 + 0x811) = 1;
    *((s8 *)D_801D8340 + 0x813) = 0;
    pak = func_8001B144((s32) "A:\\BATTLE.PAK", func_800148B0());
    if (pak != 0) {
        func_8001B5BC(func_8001BB44((Chunk *)pak, 5, 0x68));
        D_801D81AC = (void *)func_8002DC30(999, pak);
        D_801D81B0 = (void *)func_8002DC30(998, pak);
        func_8001BC14((Chunk *)pak);
    }
    func_8002E42C(bg);
    D_801D81B8 = (&D_801D81B8)[8] = -1;
    *((s8 *)D_801D8340 + 0x811) = 0;
    do {
        func_80014C08(D_800794F0);
        for (i = 0; i < 2; i++) {
            s = *(u8 **)(D_801D8348[i] + 0x114);
            if (s != 0 && s[0xE5] != (&D_801D81B8)[i * 8]) {
                func_8002DEA0(i, s);
            }
        }
    } while (*((s8 *)D_801D8340 + 0x813) == 0);
    for (i = 0; i < 2; i++) {
        if ((&D_801D81B8)[i * 8] > 0) {
            func_80022DBC(i);
            func_800235C8(i);
        }
    }
    func_8002E7B8();
    func_8001AFF0(0x1F4);
    func_8001AFF0(0x84);
    func_8001AFF0(0x1F5);
    func_8001AFF0(0x85);
    func_8001AFF0(0x81);
    *((s8 *)D_801D8340 + 0x813) = 0;
}

extern u8 D_801EEE90[];

void func_8002E26C(void) {
    do {
        func_80014C08(D_800794F0);
    } while (D_801D81B8 <= 0 || (&D_801D81B8)[8] <= 0 || *((s8 *)D_801D8340 + 0x811) == 1);
    *((s8 *)D_801D8340 + 0x811) = 1;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\sugseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_8002B858(1);
    func_800149B8(0, -1, 0, 0x2000, D_801EEE90, 0, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    D_80079544 = 0;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\kawseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_8002B858(0);
    *((s8 *)D_801D8340 + 0x811) = 0;
}

typedef struct {
    s8 bg;
    u8 unk1;
    u8 unk2;
    s8 flags;
    u8 rgb[3];
    u8 unk7;
} BgEntry;

extern BgEntry D_8006E0C0[];
extern s32 D_8006DF80;
extern u8 D_801D6A60[3];
extern s32 D_801D81A8;

void func_8002E42C(s32 n) {
    char path[32];

    if (n < 0) {
        n = rand() % 12 + 0x2C;
    }
    sprintf(path, "F:\\bg%d.pak", D_8006E0C0[n].bg + 900);
    func_800149B8(0, -1, 0, 0x400, func_8001B248, path, func_800148B0(), 0x81);
    D_801D81A8 = func_80014C08(0x7FFFFFFF);
    func_8002386C(0x17, D_8006E0C0[n].bg + 900, 0, D_801D81A8, 0);
    D_801D6A4C->unk114[0x17] = -1;
    ((Model2220 *)D_801D6A4C->unk13C[23])->unk26D4 = 0xA0000;
    ((Model2220 *)D_801D6A4C->unk13C[23])->unk26D0 = 0x280000;
    if (D_8006E0C0[n].flags & 2) {
        func_800230B8(0x17, 0, 0, D_801D81A8);
        func_80023148(0x17, 0);
        func_80022D34(0x17, 0, -2, 0);
    }
    D_801D6A4C->unk114[0x18] = D_801D6A4C->unk114[0x19] = 0;
    D_801D6A4C->unk114[0x1B] = D_8006E0C0[n].unk2;
    D_801D6A4C->unk114[0x1A] = D_8006E0C0[n].unk1;
    *(s32 *)&D_801D6A4C->unk114[0x24] = D_8006E0C0[n].flags;
    D_801D6A60[0] = D_8006E0C0[n].rgb[0];
    D_801D6A60[1] = D_8006E0C0[n].rgb[1];
    D_801D6A60[2] = D_8006E0C0[n].rgb[2];
    D_8006DF80 = D_8006E0C0[n].unk7;
}

void func_8002E658(s16 id) {
    Unk801D6A4C *p;
    s32 tim;

    D_80079544 = 1;
    p = D_801D6A4C;
    *(s16 *)((u8 *)p->unk13C[23] + 0xA78) = id;
    tim = func_8001C078((s32)func_8001BB44((Chunk *)D_801D81A8, 5, *(s16 *)((u8 *)p->unk13C[23] + 6)));
    func_8001B438((u32 *)tim, 0x3C0, 0, 0x3F0, 0x70);
    DrawSync(0);
    func_8001AE90((void *)tim);
    if (id != 0 && (*(s32 *)&D_801D6A4C->unk114[0x24] & 2)) {
        func_80014A00(0x1B);
        func_800149B8(0x1B, -1, 0, 0x1000, func_80022B98, 1);
        func_80023148(0x17, 0);
        func_80022D34(0x17, 0, -2, 0);
    }
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.r0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.r0 = D_801D6A60[0];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.g0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.g0 = D_801D6A60[1];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.b0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.b0 = D_801D6A60[2];
}

extern s32 D_801D81A8;

void func_8002E7B8(void) {
    func_800235C8(0x17);
    func_8001AE90(D_801D81A8);
}

void func_8002E7E8(u8 *arg0) {
    s32 f;

    if (D_801D6A4C->unk114[0x1B] != 0) {
        if (++D_801D6A4C->unk114[0x19] >= D_801D6A4C->unk114[0x1B]) {
            f = *(s32 *)(arg0 + 0x26D4) / 0x10000 + 5;
            D_801D6A4C->unk114[0x19] = 0;
            if (++D_801D6A4C->unk114[0x18] >= (u8)D_801D6A4C->unk114[0x24] >> 3) {
                D_801D6A4C->unk114[0x18] = 0;
            }
            *(s32 *)(arg0 + 0x26D0) =
                (((((f & 0x10) << 4) + D_801D6A4C->unk114[0x18]) << 6 | (f & 0xF) << 2) - 0x14) << 16;
        }
    }
}

void D_801EBAFC();
void D_801F00F4();
void D_801E4D80();
extern s32 D_80010864;
extern s32 D_80010874;

void func_8002E8EC(s32 mode) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, mode, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 2:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 1, 1, func_800148B0(), 0);
        break;
    case 4:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

extern u8 D_801F80C1;
void func_8002FAC8(void);
void func_8002F4F4();

void func_8002EB1C(void) {
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 0xFF, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    if (D_801F80C1 != 0) {
        func_8002FAC8();
        func_800149B8(0, -1, 0, 0x100, func_8002F4F4, 0, 0, 0, 0);
        return;
    }
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    if (*(u8 *)(D_8006E050 + 0xF) == 0) {
        func_8002B024(0, 0x6F, 0x7F);
        func_8002B858(0);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 0, func_800148B0(), 0);
    } else {
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
    }
}

void D_801E8E88();
extern s32 D_80010884;

void func_8002ECDC(s8 arg0) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010884, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E8E88, (s32 *) arg0, 0, 0, 0);
}

void func_8002ED9C(void) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
}

void D_801E8C04();
extern s32 D_80010894;

void func_8002EE50(s32 mode) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, 0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, func_800148B0(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

void D_801E4B34();

void func_8002F074(s32 mode) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, 0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, func_800148B0(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

void func_8002F298(s32 *arg0) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, arg0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

void func_8002F3C4(s32 *arg0) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, arg0, func_800148B0(), 1, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

void D_801EA2F8();
void D_801E6454();
void D_801EB2E8();
void func_8002F920(s32, s32, s32, s32, s32);
s32 func_801EBD34(void);

void func_8002F4F4(void) {
    s32 stack;
    s32 again;
    s32 r;

    stack = func_800148B0();
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    do {
        func_800149B8(0, -1, 0, 0x800, D_801EA2F8, stack, 0, 0, 0);
        r = func_80014C08(0x7FFFFFFF);
        again = 0;
        switch (r) {
        case 0:
            func_8002F920(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, D_801E6454, stack, 0, 0, 0);
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
            break;
        case 1:
            func_8002F920(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, func_8002EB1C, 0, 0, 0, 0);
            break;
        case 2:
            func_8002F920(7, 0x380, 0, 0x380, 0x80);
            *((u8 *)D_8006E054 + 0x1028) = 0;
            again = func_801EBD34();
            if (again == 0) {
                func_800149B8(0, -1, 0, 0x800, D_801EB2E8, stack, 0, 0, 0);
            } else {
                func_8002FAC8();
            }
            break;
        }
    } while (again);
}

extern s32 D_801D8260;

void func_8002F79C(void) {
    D_801D8260 = 0;
}

s32 SetTexWindow(void *, s16 *);
void func_8001E6EC(s32, void *, s32, s32);
extern s32 D_800108A4;
extern Fade D_801D81F8;
extern s32 D_801D8220;

void func_8002F7A8(void) {
    s16 r[4];
    s32 i;
    s8 *p;
    s8 *e;

    p = (s8 *)&D_801D81F8;
    if ((*(s32 *)(p + 0x68)) != 0) {
        return;
    }
    (*(s8 *)(p + 0x6C)) = -1;
    (*(s8 *)(p + 0x6D)) = -1;
    (*(s16 *)(p + 0x72)) = 0;
    (*(s16 *)(p + 0x70)) = 0;
    (*(s8 *)(p + 0x6E)) = 0;
    (*(s8 *)(p + 0x6F)) = 5;
    for (i = 0; i < 2; i++) {
        e = (s8 *)&D_801D81F8 + i * 0x34;
        func_8001E6EC(0xE, e, 0, 0);
        (*(s16 *)(e + 0x10)) = 0x141;
        (*(s16 *)(e + 0x12)) = 0xF0;
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
        SetTexWindow((s8 *)&D_801D8220 + i * 0x34, r);
    }
    func_800149B8(0, -1, 0, 0x800, func_8001B248, &D_800108A4, func_800148B0(), -2);
    D_801D8260 = func_80014C08(0x7FFFFFFF);
}

void func_8002FAA8(void);

void func_8002F8E8(void) {
    s8 *p = (s8 *)&D_801D81F8;

    func_8001AE90(*(void **)(p + 0x68));
    *(void **)(p + 0x68) = 0;
    func_8002FAA8();
}

void func_8002F920(s32 mode, s32 x, s32 y, s32 w, s32 h) {
    s32 i;

    if (D_801D81F8.unk72 != 0 && D_801D81F8.unk72 != 0x80) {
        do {
            func_80014C08(D_800794F0);
        } while (D_801D81F8.unk72 != 0 && D_801D81F8.unk72 != 0x80);
    }
    if (D_801D81F8.unk72 == 0) {
        D_801D81F8.unk6D = -1;
    }
    D_801D81F8.mode = mode;
    if (mode >= 0) {
        D_801D81F8.x = x;
        D_801D81F8.y = y;
        D_801D81F8.w = w;
        D_801D81F8.h = h;
        for (i = 0; i < 2; i++) {
            (D_801D81F8.buf + i)->clut = getClut(w, h);
            SetDrawTPage((D_801D81F8.buf + i)->tpage, 0, 0, GetTPage(0, 0, x, y));
        }
    }
    D_801D81F8.unk6E = 0;
    D_801D81F8.unk6F = 0x1E;
}

void func_8002FAA8(void) {
    s8 *p;

    p = (s8 *)&D_801D81F8;
    p[0x6C] = -1;
    p[0x6D] = -1;
    (*(s16 *)(p + 0x72)) = 0;
    (*(s16 *)(p + 0x70)) = 0;
}

extern s8 D_801D8264;

void func_8002FAC8(void) {
    D_801D8264 = -1;
}

extern s8 D_801D8266;

void func_8002FAD8(s8 arg0) {
    D_801D8266 = arg0;
}

extern u8 D_800794F4;

void func_8002FAE4(void) {
    Fade *f;
    s32 tim;
    s16 r[4];
    u8 db;

    if (D_801D81F8.tim == 0 || *(u16 *)&D_801D81F8.mode == 0xFFFF) {
        return;
    }
    switch (D_801D81F8.unk6E) {
    case 0:
        if ((s8)D_801D81F8.unk6F < 30) {
            D_801D81F8.unk6F++;
        }
        break;
    case 1:
        if ((s8)D_801D81F8.unk6F >= -59) {
            D_801D81F8.unk6F--;
        }
        break;
    }
    f = &D_801D81F8;
    f->unk70 = (f->unk70 + (s8)f->unk6F) % 7680;
    if (f->mode != f->unk6D) {
        if (f->unk6D == -1) {
            if (f->unk72 == 0) {
                tim = func_8001BFCC(f->tim, f->mode);
                func_8001B438((u32 *)tim, f->x, f->y, f->w, f->h);
                if (f->mode != 6) {
                    f->unk7C = 0x40;
                } else {
                    D_801D81F8.unk7C = 0x80;
                }
                D_801D81F8.unk7E = 0x80;
                DrawSync(0);
                func_8001AE90((void *)tim);
            }
            D_801D81F8.unk72 += 6;
            if (D_801D81F8.unk72 > 0x80) {
                D_801D81F8.unk72 = 0x80;
                D_801D81F8.unk6D = D_801D81F8.mode;
            }
        } else {
            D_801D81F8.unk72 -= 6;
            if (D_801D81F8.unk72 < 0) {
                D_801D81F8.unk72 = 0;
                D_801D81F8.unk6D = -1;
            }
        }
    }
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].twin0);
    db = D_800794F4;
    D_801D81F8.buf[db].x0 = -((D_801D81F8.unk70 / 60) & 1);
    D_801D81F8.buf[D_800794F4].y0 = 0;
    D_801D81F8.buf[D_800794F4].u0 = (D_801D81F8.unk70 / 60) & 0xFE;
    D_801D81F8.buf[D_800794F4].v0 = D_801D81F8.unk70 / 60;
    D_801D81F8.buf[D_800794F4].r0 = D_801D81F8.unk72;
    D_801D81F8.buf[D_800794F4].g0 = D_801D81F8.unk72;
    D_801D81F8.buf[D_800794F4].b0 = D_801D81F8.unk72;
    r[0] = (D_801D81F8.x % 64) * 4;
    r[1] = D_801D81F8.y % 256;
    r[2] = D_801D81F8.unk7C;
    r[3] = D_801D81F8.unk7E;
    SetTexWindow(D_801D81F8.buf[D_800794F4].twin, r);
    addPrim(&D_800793A0->ot[0xFFF], &D_801D81F8.buf[D_800794F4]);
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].twin);
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].tpage);
}


void func_80030130(void *arg0) {
    s32 t;
    s32 sum;

    t = (*(s16 *)((s8 *)arg0 + 0x122)) * (*(s32 *)((s8 *)arg0 + 0x104)) * (*(s32 *)((s8 *)arg0 + 0x104));
    sum = (*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)) + t;
    (*(s32 *)((s8 *)arg0 + 0x20)) = (*(s16 *)((s8 *)arg0 + 0xD4)) + ((sum * (*(s32 *)((s8 *)arg0 + 0x9C))) >> 12);
    (*(s32 *)((s8 *)arg0 + 0x24)) = (*(s16 *)((s8 *)arg0 + 0xD6)) + ((sum * (*(s32 *)((s8 *)arg0 + 0xA0))) >> 12);
    (*(s32 *)((s8 *)arg0 + 0x28)) = (*(s16 *)((s8 *)arg0 + 0xD8)) + ((sum * (*(s32 *)((s8 *)arg0 + 0xA4))) >> 12);
}

void func_800301D0(void *arg0) {
    func_80030130(arg0);
    *(s32 *)((s8 *)arg0 + 0x24) +=
        -*(s16 *)((s8 *)arg0 + 0x120) * *(s32 *)((s8 *)arg0 + 0x100) +
        *(s16 *)((s8 *)arg0 + 0x120) * *(s32 *)((s8 *)arg0 + 0x100) *
            *(s32 *)((s8 *)arg0 + 0x100) / 56;
}

s32 rsin(s32);

void func_80030264(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x20)));
}

void func_800302E0(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x24)));
}

void func_8003035C(u8 *p) {
    s32 h;
    s32 sx;
    s32 cz;

    func_80030130(p);
    h = -*(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) +
        *(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) * *(s32 *)(p + 0x100) / 56;
    sx = h * rsin(*(s16 *)(p + 0xE8)) >> 12;
    cz = h * rcos(*(s16 *)(p + 0xE8)) >> 12;
    *(s32 *)(p + 0x20) -= sx;
    *(s32 *)(p + 0x24) += cz;
}

s32 rcos(s32);

void func_80030440(u8 *p) {
    s32 d;
    s32 h;
    s32 sx;
    s32 cz;

    d = *(s16 *)(p + 0x120) * *(s32 *)(p + 0x100);
    *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4) + (d * *(s32 *)(p + 0x9C) >> 12);
    *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6) + (d * *(s32 *)(p + 0xA0) >> 12);
    *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8) + (d * *(s32 *)(p + 0xA4) >> 12);
    h = -*(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) +
        *(s16 *)(p + 0x122) * *(s32 *)(p + 0x100) * *(s32 *)(p + 0x100) / 56;
    sx = h * rsin(*(s16 *)(p + 0xE8)) >> 12;
    cz = h * rcos(*(s16 *)(p + 0xE8)) >> 12;
    *(s32 *)(p + 0x20) -= sx;
    *(s32 *)(p + 0x24) += cz;
}

void func_8003058C(u8 *p) {
    s32 x;
    s32 y;
    s32 z;

    x = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    y = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    z = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4) + x;
    *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6) + y;
    *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8) + z;
}

long SquareRoot0(long);

s32 func_80030694(SVECTOR *a, SVECTOR *b) {
    VECTOR d;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    return SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz);
}

s32 func_80030718(SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    VECTOR ab;
    VECTOR cb;
    VECTOR ca;
    s32 d0;
    s32 d1;

    ab.vx = b->vx - a->vx;
    ab.vy = b->vy - a->vy;
    ab.vz = b->vz - a->vz;
    cb.vx = b->vx - c->vx;
    cb.vy = b->vy - c->vy;
    cb.vz = b->vz - c->vz;
    ca.vx = a->vx - c->vx;
    ca.vy = a->vy - c->vy;
    ca.vz = a->vz - c->vz;
    d0 = (cb.vx * ab.vx + cb.vy * ab.vy + cb.vz * ab.vz) >> 12;
    d1 = (ca.vx * ab.vx + ca.vy * ab.vy + ca.vz * ab.vz) >> 12;
    if ((d0 <= 0 && d1 >= 0) || (d0 >= 0 && d1 <= 0)) {
        return 1;
    }
    return 0;
}

void func_80030828(SVECTOR *a, SVECTOR *p, SVECTOR *b, VECTOR *out) {
    VECTOR d;
    VECTOR ap;
    VECTOR n;
    VECTOR proj;
    s32 t;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    if (SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz) < 20000) {
        VectorNormal(&d, &n);
    } else {
        d.vx >>= 4;
        d.vy >>= 4;
        d.vz >>= 4;
        VectorNormal(&d, &n);
    }
    ap.vx = p->vx - a->vx;
    ap.vy = p->vy - a->vy;
    ap.vz = p->vz - a->vz;
    t = (n.vx * ap.vx + n.vy * ap.vy + n.vz * ap.vz) >> 12;
    proj.vx = t * n.vx >> 12;
    proj.vy = t * n.vy >> 12;
    proj.vz = t * n.vz >> 12;
    out->vx = a->vx + proj.vx;
    out->vy = a->vy + proj.vy;
    out->vz = a->vz + proj.vz;
}

s32 func_800309F0(SVECTOR *arg0, SVECTOR *arg1, s32 arg2) {
    s32 v;

    v = func_80030694(arg0, arg1);
    if (-arg2 < v && v < arg2) {
        return 1;
    }
    return 0;
}

void func_80030828(SVECTOR *, SVECTOR *, SVECTOR *, VECTOR *);

s32 func_80030A34(SVECTOR *a, SVECTOR *b, SVECTOR *c, s16 r) {
    SVECTOR sv;
    VECTOR v;

    if (func_80030718(a, b, c) != 0) {
        func_80030828(a, c, b, &v);
        sv.vx = v.vx;
        sv.vy = v.vy;
        sv.vz = v.vz;
        if (func_800309F0(c, &sv, r) != 0) {
            return 1;
        }
        return -1;
    }
    return 0;
}

void *func_80030E3C(void *arg0);

Unk13C *func_80030AE4(Unk13C *src) {
    Unk13C *dst;

    dst = func_8001AD0C(0x13C);
    *dst = *src;
    func_80030E3C(dst);
    return dst;
}

s32 PopMatrix();
s32 PushMatrix();
s32 func_80030F90(s32, s32);

void func_80030B6C(s32 arg0) {
    PushMatrix();
    func_80030F90(arg0, 0);
    PopMatrix();
}

void func_80030BA4(void *arg0) {
    func_8001AE90(arg0);
}

void VectorNormal(VECTOR *, VECTOR *);

s32 func_80030BC4(SVECTOR *a, SVECTOR *b, VECTOR *out) {
    VECTOR d;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    if (SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz) < 20000) {
        VectorNormal(&d, out);
        return 1;
    }
    d.vx >>= 4;
    d.vy >>= 4;
    d.vz >>= 4;
    VectorNormal(&d, out);
    return -1;
}

void func_80030CA8(u8 *p) {
    s32 i;

    p[0x139] = *(s16 *)(p + 0x12E) >= 0x5B;
    *(s32 *)(p + 0x118) = -1;
    for (i = 0; i < 3; i++) {
        ((Unk80030CA8 *)p)->unk10C[i] = 0;
    }
    *(s32 *)(p + 0x108) = 0;
    *(s32 *)(p + 0x11C) = 0;
    *(s32 *)(p + 0x100) = 0;
    *(s32 *)(p + 0x104) = 0;
    if (*(s16 *)(p + 0x12E) != 0 && *(s16 *)(p + 0x12E) != 0x5A) {
        *(s32 *)(p + 0x38) = *(s32 *)(p + 0xAC);
        *(s32 *)(p + 0x3C) = *(s32 *)(p + 0xB0);
        *(s32 *)(p + 0x40) = *(s32 *)(p + 0xB4);
        *(s16 *)(p + 0x30) = *(s16 *)(p + 0xE4);
        *(s16 *)(p + 0x32) = *(s16 *)(p + 0xE6);
        *(s16 *)(p + 0x34) = *(s16 *)(p + 0xE8);
        *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4);
        *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6);
        *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8);
        *(s32 *)(p + 0x6C) = *(s16 *)(p + 0xDC);
        *(s32 *)(p + 0x70) = *(s16 *)(p + 0xDE);
        *(s32 *)(p + 0x74) = *(s16 *)(p + 0xE0);
        func_80030BC4((SVECTOR *)(p + 0xD4), (SVECTOR *)(p + 0xDC), (VECTOR *)(p + 0x9C));
    } else {
        *(s32 *)(p + 0xAC) = *(s32 *)(p + 0x38);
        *(s32 *)(p + 0xB0) = *(s32 *)(p + 0x3C);
        *(s32 *)(p + 0xB4) = *(s32 *)(p + 0x40);
        *(s16 *)(p + 0xE4) = *(s16 *)(p + 0x30);
        *(s16 *)(p + 0xE6) = *(s16 *)(p + 0x32);
        *(s16 *)(p + 0xE8) = *(s16 *)(p + 0x34);
        *(s16 *)(p + 0xD4) = *(s32 *)(p + 0x20);
        *(s16 *)(p + 0xD6) = *(s32 *)(p + 0x24);
        *(s16 *)(p + 0xD8) = *(s32 *)(p + 0x28);
    }
}

void func_80030CA8(u8 *);

void *func_80030E3C(void *arg0) {
    func_8001EFDC(arg0, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xD4)), (s32) (*(s16 *)((s8 *)arg0 + 0xD6)), (s32) (*(s16 *)((s8 *)arg0 + 0xD8)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE4)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE6)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE8)));
    func_8001EFDC(arg0 + 0x4C, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xDC)), (s32) (*(s16 *)((s8 *)arg0 + 0xDE)), (s32) (*(s16 *)((s8 *)arg0 + 0xE0)), 0, 0, 0);
    (*(s32 *)((s8 *)arg0 + 0x14)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x18)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x1C)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x38)) = (s32) (*(s32 *)((s8 *)arg0 + 0xAC));
    (*(s32 *)((s8 *)arg0 + 0x3C)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB0));
    (*(s32 *)((s8 *)arg0 + 0x40)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB4));
    (*(u16 *)((s8 *)arg0 + 0x30)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE4));
    (*(u16 *)((s8 *)arg0 + 0x32)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE6));
    (*(u16 *)((s8 *)arg0 + 0x34)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE8));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD4));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD6));
    (*(s32 *)((s8 *)arg0 + 0x28)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD8));
    func_80030CA8(arg0);
    (*(s32 *)((s8 *)arg0 + 0x6C)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDC));
    (*(s32 *)((s8 *)arg0 + 0x70)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDE));
    (*(s32 *)((s8 *)arg0 + 0x74)) = (s32) (*(s16 *)((s8 *)arg0 + 0xE0));
    func_80030BC4(arg0 + 0xD4, arg0 + 0xDC, arg0 + 0x9C);
    (*(s32 *)((s8 *)arg0 + 0xFC)) = 0;
    (*(s16 *)((s8 *)arg0 + 0x132)) = 0;
    (*(s8 *)((s8 *)arg0 + 0x138)) = 0;
    return arg0;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010864);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010874);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010884);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010894);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800108A4);

s32 func_80030F90(s32 arg, s32 flag) {
    Anim *o = (Anim *)arg;
    u8 f = flag;
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    s32 r;

    if (o->mode != 10 && o->mode < 90) {
        if (o->sx != o->sxT) {
            o->sx = o->dsx * o->t + o->sx0;
            if (o->dsx >= 0) {
                if (o->sx >= o->sxT) {
                    o->sx = o->sxT;
                    o->doneX = 1;
                }
            } else if (o->sx <= o->sxT) {
                o->sx = o->sxT;
                o->doneX = 1;
            }
        } else {
            o->doneX = 1;
        }
        if (o->sy != o->syT) {
            o->sy = o->dsy * o->t + o->sy0;
            if (o->dsy >= 0) {
                if (o->sy >= o->syT) {
                    o->sy = o->syT;
                    o->doneY = 1;
                }
            } else if (o->sy <= o->syT) {
                o->sy = o->syT;
                o->doneY = 1;
            }
        } else {
            o->doneY = 1;
        }
        if (o->sz != o->szT) {
            o->sz = o->dsz * o->t + o->sz0;
            if (o->dsz >= 0) {
                if (o->sz >= o->szT) {
                    o->sz = o->szT;
                    o->doneZ = 1;
                }
            } else if (o->sz <= o->szT) {
                o->sz = o->szT;
                o->doneZ = 1;
            }
        } else {
            o->doneZ = 1;
        }
        o->rotX = o->rx0 + o->drx * o->t + o->ddrx * o->t2 * o->t2 / 64;
        o->rotY = o->ry0 + o->dry * o->t + o->ddry * o->t2 * o->t2 / 64;
        o->rotZ = o->rz0 + o->drz * o->t + o->ddrz * o->t2 * o->t2 / 64;
    }
    func_8001F01C(o, &v0);
    switch (o->mode) {
    case 6:
        o->state = -1;
    case 0:
    case 47:
    case 48:
    case 49:
    case 56:
    case 69:
    case 82:
    case 90:
        o->posX = o->px;
        o->posY = o->py;
        o->posZ = o->pz;
        break;
    case 1:
    case 11:
    case 12:
    case 13:
    case 29:
    case 30:
    case 31:
    case 50:
    case 57:
    case 63:
    case 70:
    case 76:
    case 83:
        func_80030130(o);
        break;
    case 2:
    case 14:
    case 15:
    case 16:
    case 32:
    case 33:
    case 34:
    case 51:
    case 58:
    case 64:
    case 71:
    case 77:
    case 84:
        func_800301D0(o);
        break;
    case 3:
    case 17:
    case 18:
    case 19:
    case 35:
    case 36:
    case 37:
    case 52:
    case 59:
    case 65:
    case 72:
    case 78:
    case 85:
        func_80030264(o);
        break;
    case 4:
    case 20:
    case 21:
    case 22:
    case 38:
    case 39:
    case 40:
    case 53:
    case 60:
    case 66:
    case 73:
    case 79:
    case 86:
        func_800302E0(o);
        break;
    case 5:
    case 23:
    case 24:
    case 25:
    case 41:
    case 42:
    case 43:
    case 54:
    case 61:
    case 67:
    case 74:
    case 80:
    case 87:
        func_8003035C((u8 *)o);
        break;
    case 7:
    case 26:
    case 27:
    case 28:
    case 44:
    case 45:
    case 46:
    case 55:
    case 62:
    case 68:
    case 75:
    case 81:
    case 88:
        func_80030440((u8 *)o);
        break;
    case 8:
    case 89:
        func_8003058C((u8 *)o);
        break;
    }
    if (o->state != -1 && o->mode != 0 && o->mode < 90) {
        func_8001EEA0(o, f);
        func_8001EEA0((u8 *)o + 0x4C, 0);
        func_8001F01C(o, &v1);
        func_8001F01C((u8 *)o + 0x4C, &v2);
        r = func_80030A34(&v0, &v1, &v2, o->unk12C);
        if (r == 1) {
            switch (o->mode) {
            case 11:
            case 14:
            case 17:
            case 20:
            case 23:
            case 26:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                break;
            case 12:
            case 15:
            case 18:
            case 21:
            case 24:
            case 27:
                o->mode = 0;
                o->posX = o->px2;
                o->posY = o->py2;
                o->posZ = o->pz2;
                o->px = o->px2;
                o->py = o->py2;
                o->pz = o->pz2;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
                break;
            case 13:
            case 16:
            case 19:
            case 22:
            case 25:
            case 28:
                func_80030CA8((u8 *)o);
                break;
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                o->unk139 = 1;
                break;
            case 76:
            case 77:
            case 78:
            case 79:
            case 80:
            case 81:
                o->mode = 0;
                o->posX = o->px2;
                o->posY = o->py2;
                o->posZ = o->pz2;
                o->px = o->px2;
                o->py = o->py2;
                o->pz = o->pz2;
            case 63:
            case 64:
            case 65:
            case 66:
            case 67:
            case 68:
                o->unk137 = 2;
                o->speed = -abs(o->speed);
                break;
            }
            o->state = 1;
        } else if (r == -1) {
            o->state = 2;
        }
    }
    if (o->mode < 90 && o->mode != 0 && o->state == -1) {
        o->state = 0;
    }
    if (o->flag != 0) {
        switch (o->mode) {
    case 31:
    case 34:
    case 37:
    case 40:
    case 43:
    case 46:
    case 49:
            func_80030CA8((u8 *)o);
            break;
        }
    }
    if (o->mode < 90) {
        o->t++;
        o->t2++;
        if (o->period != 0 && o->flag == 0 && o->period < o->cnt++) {
            o->cnt = o->period;
            switch (o->mode) {
            case 29:
            case 32:
            case 35:
            case 38:
            case 41:
            case 44:
            case 47:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                break;
            case 30:
            case 33:
            case 36:
            case 39:
            case 42:
            case 45:
            case 48:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
                break;
            case 31:
            case 34:
            case 37:
            case 40:
            case 43:
            case 46:
            case 49:
                func_80030CA8((u8 *)o);
                break;
            case 56:
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
                o->unk139 = 1;
                break;
            case 82:
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
            case 89:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
            case 69:
            case 70:
            case 71:
            case 72:
            case 73:
            case 74:
            case 75:
                o->unk137 = 2;
                o->speed = -abs(o->speed);
                break;
            }
            o->flag = 1;
        }
    }
    func_8001EEA0(o, f);
}

void func_80031754(void *arg0) {
    if ((*(s16 *)((s8 *)arg0 + 0x12E)) >= 0x5B) {
        if ((*(s16 *)((s8 *)arg0 + 0x128)) > (*(s32 *)((s8 *)arg0 + 0x100))) {
            (*(s32 *)((s8 *)arg0 + 0x100)) += 1;
            return;
        }
        (*(s32 *)((s8 *)arg0 + 0x100)) = 0;
        (*(s8 *)((s8 *)arg0 + 0x139)) = 0;
        (*(s16 *)((s8 *)arg0 + 0x12E)) = (s16) ((u16) (*(s16 *)((s8 *)arg0 + 0x12E)) - 0x64);
    }
}

s16 func_800317A8(void *arg0, s16 arg1) {
    u8 *o;

    o = arg0;
    switch (o[0x137]) {
    case 1:
        arg1 = (*(s32 *)(o + 0x38)) / 16;
        if ((*(s32 *)(o + 0x38)) > 0x1000) {
            arg1 = 0x100 - ((*(s32 *)(o + 0x38)) - 0x1000) / 16;
        }
        break;
    case 2:
        arg1 += (*(u16 *)(o + 0x130));
        break;
    case 3:
        if (o[0x138] == 2) {
            break;
        }
        if (o[0x138] == 0) {
            arg1 += (*(u16 *)(o + 0x130));
            if (arg1 > 0x100) {
                arg1 = 0x100;
                o[0x138] = 1;
            }
        } else {
            arg1 -= (*(u16 *)(o + 0x130));
            if (arg1 < 0) {
                arg1 = 0;
                o[0x138] = 2;
            }
        }
        break;
    case 4:
        if (o[0x138] == 0) {
            arg1 += (*(u16 *)(o + 0x130));
            if (arg1 > 0x100) {
                arg1 = 0x100;
                o[0x138] = 1;
            }
        } else {
            arg1 -= (*(u16 *)(o + 0x130));
            if (arg1 < 0) {
                arg1 = 0;
                o[0x138] = 0;
            }
        }
        break;
    case 5:
        o[0x138] += (*(u16 *)(o + 0x130));
        if ((s8)o[0x138] >= 0) {
            arg1 = o[0x138] + 0x80;
        } else {
            arg1 = 0xFF - (o[0x138] & 0x7F);
        }
        break;
    }
    if (arg1 < 0) {
        arg1 = 0;
    }
    if (arg1 > 0x100) {
        arg1 = 0x100;
    }
    if ((*(s16 *)(o + 0x12E)) == 0xA) {
        arg1 = (*(s16 *)(o + 0x132));
    } else {
        (*(s16 *)(o + 0x132)) = arg1;
    }
    return arg1;
}

s32 rsin(s32);
s32 rcos(s32);

void func_80031970(Obj32 *o) {
    SVECTOR *v;
    s32 i;
    s16 x;
    s16 y;

    v = o->unk16C;
    for (i = 0; i < o->n; i++) {
        x = rsin((i << 12) / o->n) * o->unk19C[0] / 4096;
        y = rcos((i << 12) / o->n) * o->unk19C[0] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3];
        v++;
        x = rsin(((i + 1) << 12) / o->n) * o->unk19C[0] / 4096;
        y = rcos(((i + 1) << 12) / o->n) * o->unk19C[0] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3];
        v++;
        x = rsin((i << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        y = rcos((i << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3] + (o->unk19C[4] - o->unk19C[3]) * o->unk19C[2] / 100;
        v++;
        x = rsin(((i + 1) << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        y = rcos(((i + 1) << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3] + (o->unk19C[4] - o->unk19C[3]) * o->unk19C[2] / 100;
        v++;
        x = rsin((i << 12) / o->n) * o->unk19C[1] / 4096;
        y = rcos((i << 12) / o->n) * o->unk19C[1] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[4];
        v++;
        x = rsin(((i + 1) << 12) / o->n) * o->unk19C[1] / 4096;
        y = rcos(((i + 1) << 12) / o->n) * o->unk19C[1] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[4];
        v++;
    }
}

extern u8 D_8006DEF4[];
void func_80031970(Obj32 *);
s32 func_801E6C78(s32, s32, Obj32 *, u8 *, s32);

Obj32 *func_80031F58(s16 id, Bytes4 *a, Bytes4 *b, Bytes4 *c, Unk13C *src, s32 n, u8 abr, u8 tp, s32 type,
                     s16 p0, s16 p1, s16 p2, s16 p3, s16 p4, Bytes8 *q, s32 r, s32 s, s32 t, u8 u1, u8 u2,
                     s32 w, s32 x) {
    Obj32 *o;
    u8 *prim;
    s32 i;
    s32 j;

    o = func_8001AD0C(0x1B0);
    o->type = type;
    o->n = n;
    o->unk19C[0] = p0;
    o->unk19C[1] = p1;
    o->unk19C[2] = p2;
    o->unk19C[3] = p3;
    o->unk19C[4] = p4;
    o->unk16C = func_8001AD0C(n * 48);
    func_80031970(o);
    if (type == 13) {
        o->unk170 = *q;
        o->unk178 = r;
        o->unk17C = s;
        if (t >= 0 && func_801E6C78(t, 1, o, o->unk13C, x) != 0) {
            o->unk1AD = 1;
        } else {
            o->unk1AD = -1;
        }
    } else {
        o->unk1AD = -1;
    }
    o->unk198 = tp;
    o->unk1A6 = id;
    o->unk1A8 = -1;
    o->unk185 = *a;
    o->unk189 = *b;
    o->unk18D = *c;
    *(Unk13C *)o = *src;
    func_80030E3C(o);
    o->unk1AB = u2;
    o->unk194 = w;
    o->unk1AA = u1;
    o->unk1AC = abr;
    for (i = 0; i < 2; i++) {
        if (o->type < 10) {
            o->unk15C[i] = func_8001AD0C(o->n * 16);
        } else {
            o->unk15C[i] = 0;
        }
        prim = o->unk164[i] = func_8001AD0C(D_8006DEF4[o->type] * o->n * 2);
        for (j = 0; j < o->n * 2; j++) {
            func_8001E6EC(o->type, prim, abr, 0);
            if (o->type < 10) {
                SetDrawTPage(o->unk15C[i] + j * 8, 0, 0, GetTPage(0, tp, 0, 0));
            }
            prim += D_8006DEF4[o->type];
        }
    }
    return o;
}

void func_80031754(void *arg0);
s16 func_800317A8(void *arg0, s16 arg1);
void func_8001EC3C(u8 *, u8, u8, u8, u8);
void func_801E7020(u8 *);

void func_8003230C(Obj32 *o) {
    u8 c0[8];
    u8 c1[8];
    u8 c2[8];
    SVECTOR *v;
    s32 i;

    if (o->unk0[0x139] != 0) {
        func_80031754(o);
        return;
    }
    PushMatrix();
    func_80030F90((s32)o, o->unk1AA);
    o->unk1A6 = func_800317A8(o, o->unk1A6);
    if (o->unk1A6 == 0) {
        PopMatrix();
        return;
    }
    v = o->unk16C;
    if (o->unk1A6 != o->unk1A8) {
        c0[0] = o->unk185.b[0] * o->unk1A6 / 256;
        c0[1] = o->unk185.b[1] * o->unk1A6 / 256;
        c0[2] = o->unk185.b[2] * o->unk1A6 / 256;
        c1[0] = o->unk189.b[0] * o->unk1A6 / 256;
        c1[1] = o->unk189.b[1] * o->unk1A6 / 256;
        c1[2] = o->unk189.b[2] * o->unk1A6 / 256;
        c2[0] = o->unk18D.b[0] * o->unk1A6 / 256;
        c2[1] = o->unk18D.b[1] * o->unk1A6 / 256;
        c2[2] = o->unk18D.b[2] * o->unk1A6 / 256;
    }
    switch (o->type) {
    case 9: {
        u8 *p;
        u8 *q;
        u8 *tp;

        p = o->unk164[D_800794F4];
        q = o->unk164[D_800794F4 ^ 1];
        tp = o->unk15C[D_800794F4];
        for (i = 0; i < o->n; i++) {
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c0[0], c0[1], c0[2]);
                func_8001E76C(p, c0[0], c0[1], c0[2]);
                func_8001E7B8(p, c1[0], c1[1], c1[2]);
                func_8001E804(p, c1[0], c1[1], c1[2]);
                func_8001E75C(q, c0[0], c0[1], c0[2]);
                func_8001E76C(q, c0[0], c0[1], c0[2]);
                func_8001E7B8(q, c1[0], c1[1], c1[2]);
                func_8001E804(q, c1[0], c1[1], c1[2]);
            }
            func_8001DFE0((s32)p, (s32)tp, (s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], o->unk1AC, o->unk1AB, o->unk194);
            p += 0x24;
            q += 0x24;
            tp += 8;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c1[0], c1[1], c1[2]);
                func_8001E76C(p, c1[0], c1[1], c1[2]);
                func_8001E7B8(p, c2[0], c2[1], c2[2]);
                func_8001E804(p, c2[0], c2[1], c2[2]);
                func_8001E75C(q, c1[0], c1[1], c1[2]);
                func_8001E76C(q, c1[0], c1[1], c1[2]);
                func_8001E7B8(q, c2[0], c2[1], c2[2]);
                func_8001E804(q, c2[0], c2[1], c2[2]);
            }
            func_8001DFE0((s32)p, (s32)tp, (s32)&v[2], (s32)&v[3], (s32)&v[4], (s32)&v[5], o->unk1AC, o->unk1AB, o->unk194);
            p += 0x24;
            q += 0x24;
            tp += 8;
            v += 6;
        }
        break;
    }
    case 13: {
        u8 *p;
        u8 *q;

        if (o->unk1AD >= 0) {
            func_801E7020(o->unk13C);
        }
        p = o->unk164[D_800794F4];
        q = o->unk164[D_800794F4 ^ 1];
        for (i = 0; i < o->n; i++) {
            func_8001EC3C(p, o->unk170.b[0], o->unk170.b[2], o->unk170.b[4], o->unk170.b[6]);
            *(u16 *)(p + 0x1A) = o->unk178;
            *(u16 *)(p + 0xE) = o->unk17C;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c0[0], c0[1], c0[2]);
                func_8001E76C(p, c0[0], c0[1], c0[2]);
                func_8001E7B8(p, c1[0], c1[1], c1[2]);
                func_8001E804(p, c1[0], c1[1], c1[2]);
                func_8001E75C(q, c0[0], c0[1], c0[2]);
                func_8001E76C(q, c0[0], c0[1], c0[2]);
                func_8001E7B8(q, c1[0], c1[1], c1[2]);
                func_8001E804(q, c1[0], c1[1], c1[2]);
            }
            func_8001D900((s32)p, (s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], o->unk1AB, o->unk194);
            p += 0x34;
            q += 0x34;
            func_8001EC3C(p, o->unk170.b[0], o->unk170.b[2], o->unk170.b[4], o->unk170.b[6]);
            *(u16 *)(p + 0x1A) = o->unk178;
            *(u16 *)(p + 0xE) = o->unk17C;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c1[0], c1[1], c1[2]);
                func_8001E76C(p, c1[0], c1[1], c1[2]);
                func_8001E7B8(p, c2[0], c2[1], c2[2]);
                func_8001E804(p, c2[0], c2[1], c2[2]);
                func_8001E75C(q, c1[0], c1[1], c1[2]);
                func_8001E76C(q, c1[0], c1[1], c1[2]);
                func_8001E7B8(q, c2[0], c2[1], c2[2]);
                func_8001E804(q, c2[0], c2[1], c2[2]);
            }
            func_8001D900((s32)p, (s32)&v[2], (s32)&v[3], (s32)&v[4], (s32)&v[5], o->unk1AB, o->unk194);
            p += 0x34;
            q += 0x34;
            v += 6;
        }
        break;
    }
    }
    PopMatrix();
    o->unk1A8 = o->unk1A6;
}

void func_801E72D4(u8 *);

void func_80032AA0(Obj32 *p) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_8001AE90(p->unk15C[i]);
        func_8001AE90(p->unk164[i]);
    }
    if (p->unk1AD >= 0) {
        func_801E72D4(p->unk13C);
    }
    func_8001AE90(p->unk16C);
    func_8001AE90(p);
}


Particles *func_80032B44(u8 *c0, u8 *c1, Unk13C *src, s16 sx, s16 sy, s16 a5, s16 a6, s16 frames, s16 a8, s16 a9,
                         s16 count, s16 a11, s16 a12, s16 a13, s16 kind, s16 semi, s32 flags, s32 a17) {
    Particles *o;
    Particle *p;
    LINE_G2 *l;
    s32 i;
    s32 k;
    s32 angle;

    o = func_8001AD0C(0x15C);
    o->p = p = func_8001AD0C(count * 0x88);
    k = 0;
    if (src == 0) {
        o->parent = (u8 *)D_801D6A4C + 0x78;
        o->own = 0;
    } else {
        o->base = *src;
        func_80030E3C(o);
        o->parent = o;
        o->own = 1;
    }
    o->unk14E = a11;
    o->unk154 = a17;
    o->unk15A = flags & 1;
    o->count = count;
    o->unk150 = 0;
    o->frames = frames;
    o->rgb[0] = c0[0];
    o->rgb[1] = c0[1];
    o->rgb[2] = c0[2];
    o->drgb[0] = (c1[0] - o->rgb[0]) / o->frames;
    o->drgb[1] = (c1[1] - o->rgb[1]) / o->frames;
    o->drgb[2] = (c1[2] - o->rgb[2]) / o->frames;
    o->unk158 = a9 == 0 ? 1 : -1;
    o->kind = kind;
    for (i = 0; i < o->count; i++, p++) {
        if (o->kind == 0) {
            l = &p->line[0];
            func_800678E4(l);
            setSemiTrans(l, semi);
            l = &p->line[1];
            func_800678E4(l);
            setSemiTrans(l, semi);
        } else {
            l = &p->line[0];
            func_80067904(l);
            setSemiTrans(l, semi);
            l->r0 = c0[0];
            l->g0 = c0[1];
            l->b0 = c0[2];
            l->r1 = c1[0];
            l->g1 = c1[1];
            l->b1 = c1[2];
            l++;
            func_80067904(l);
            setSemiTrans(l, semi);
            l->r0 = c0[0];
            l->g0 = c0[1];
            l->b0 = c0[2];
            l->r1 = c1[0];
            l->g1 = c1[1];
            l->b1 = c1[2];
        }
        func_8001EFDC(p, (s32)o->parent, 0, 0, 0, 0, 0, 0);
        p->unk7C = a5 * 8;
        p->unk7E = rand() % a8 + 1;
        if (sy == 0) {
            sy = 1;
        }
        if (sx == 0) {
            sx = 1;
        }
        if (a13 < 3) {
            p->unk32 = rand() % sx - sx / 2;
            p->unk30 = rand() % sy - sy / 2;
            p->unk34 = 0;
            p->unk7A = 0;
        } else {
            p->unk32 = 0;
            p->unk30 = 0;
            p->unk34 = 0;
            p->unk7A = sx - 0xB4;
        }
        p->unk80 = p->unk7E * frames;
        p->unk78 = 0;
        p->unk76 = 0;
        p->unk74 = 0;
        if (a12 != 0) {
            switch ((s16)(a13 % 3)) {
            case 0:
                angle = i << 12;
                k = angle / o->count;
                p->unk76 = a12;
                break;
            case 1:
                k = rand() % 4096;
                p->unk76 = a12;
                break;
            case 2:
                k = rand() % 4096;
                p->unk76 = rand() % a12;
                break;
            }
            p->unk34 = k;
        }
        p->unk82 = rand() & 0xFFF;
        p->unk84 = rand() & 0x1FF;
        if (i & 1) {
            p->unk84 = -p->unk84;
        }
    }
    o->unk147 = flags & 2;
    if (a6 == 0) {
        o->unk156 = 0;
    } else {
        o->unk156 = (a6 - a5) * 8 / o->frames;
    }
    return o;
}


void func_80033258(Particles *o) {
    Particle *p;
    LINE_G2 *l;
    SVECTOR *v;
    s32 limit;
    s32 i;
    s32 t;
    s32 d;
    u32 z;
    s16 dx;
    s16 dz;
    s32 a;
    u8 r;
    u8 g;
    u8 bl;
    s32 b;

    p = o->p;
    limit = 10000;
    if (o->own != 0) {
        if (((u8 *)o)[0x139] != 0) {
            func_80031754(o);
            return;
        }
        PushMatrix();
        func_80030F90((s32)o, o->unk15A);
        PopMatrix();
        limit = (*(s16 *)((u8 *)o + 0x124) + o->frames - 1) / o->frames * o->frames;
    }
    PushMatrix();
    if (o->kind == 0) {
        if (o->unk147 == 0) {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 8;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vz += d;
                    z = RotTransPers((s32)v, (s32)&l->r1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        r = o->rgb[0] + o->drgb[0] * t;
                        g = o->rgb[1] + o->drgb[1] * t;
                        bl = o->rgb[2] + o->drgb[2] * t;
                        l->r0 = r;
                        l->g0 = g;
                        l->b0 = bl;
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p++;
        }
        } else {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 16;
                v->vx += dx = d * rsin(p->unk82) / 4096;
                v->vz += dz = d * rcos(p->unk82) / 4096;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vx -= dx;
                    v->vz -= dz;
                    z = RotTransPers((s32)v, (s32)&l->r1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        r = o->rgb[0] + o->drgb[0] * t;
                        g = o->rgb[1] + o->drgb[1] * t;
                        bl = o->rgb[2] + o->drgb[2] * t;
                        l->r0 = r;
                        l->g0 = g;
                        l->b0 = bl;
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p->unk82 += p->unk84;
            p++;
        }
        }
    } else {
        if (o->unk147 == 0) {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 8;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vz += d;
                    z = RotTransPers((s32)v, (s32)&l->x1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p++;
        }
        } else {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 16;
                v->vx += dx = d * rsin(p->unk82) / 4096;
                v->vz += dz = d * rcos(p->unk82) / 4096;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vx -= dx;
                    v->vz -= dz;
                    z = RotTransPers((s32)v, (s32)&l->x1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p->unk82 += p->unk84;
            p++;
        }
        }
    }
    o->unk150++;
    PopMatrix();
}

void func_80033CD4(void *arg0) {
    func_8001AE90((*(void **)((s8 *)arg0 + 0x140)));
    func_8001AE90(arg0);
}

void func_80033D08(s32 n) {
    while (n > 0) {
        func_80014C08(D_800794F0);
        if (((s8 *)D_801D8340)[0x823] == 0) {
            n--;
        }
        if (((s8 *)D_801D8340)[0x815] != 0) {
            ((s8 *)D_801D8340)[0x815] = 0;
            func_80014A90();
            return;
        }
    }
}


extern u8 *D_801D8348[];
s32 func_80033D9C(void) {
    void *var_v0_2;

    if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
        (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
        func_80014A90();
        return -1;
    }
    if ((((u32) (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178)) >> 0x11) & 3) == 1) {
        var_v0_2 = *D_80089840;
    } else {
        var_v0_2 = D_80089840[(*(s8 *)((s8 *)D_801D8340 + 0x817))];
    }
    if (!((*(u16 *)((s8 *)var_v0_2 + 0xA)) & 0x40)) {
        return 0;
    }
    func_8002B498(0xA0);
    return 1;
}

void func_80033E7C(void) {
    s32 n;

    (*(s32 *)((s8 *)D_801D8340 + 0x7FC)) = 0;
    while (1) {
        if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
            func_80014A90();
            return;
        }
        func_80014C08(D_800794F0);
        if ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x816)) = 0;
            return;
        }
        if ((*(s8 *)((s8 *)D_801D8340 + 0x816)) == 0) {
            return;
        }
        n = (*(s32 *)((s8 *)D_801D8340 + 0x7FC))++;
        if (n >= 0xF1) {
            return;
        }
    }
}

extern s32 D_801D8330;
extern s16 D_8006E280[3];
extern s16 D_8006E288[2][3];

void func_80033F34(void) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;

    if (D_801D8330 != 0) {
        D_801D8330--;
    }
    for (i = 0; i < 2; i++) {
        x = 0x80;
        y = i * -125 + 0x99;
        for (j = 0; j < 3; j++) {
            if (func_80029990() != 0) {
                return;
            }
            if (D_801D8330 == 0 && ((*(u32 *)(D_801D8348[i] + 0x178) >> 2) & 3) != j) {
                continue;
            }
            CUR_SPRT->sp.x0 = x + (x - D_8006E280[j]) * D_801D8330 / 32;
            CUR_SPRT->sp.y0 = y + (y - D_8006E288[i][j]) * D_801D8330 / 32;
            CUR_SPRT->sp.u0 = j * 64 + 64;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7FF0;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = x;
            CUR_SPRT->sp.g0 = x;
            CUR_SPRT->sp.b0 = x;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1D);
            addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        }
    }
}

s32 func_8001A164(s32 *, s32);
extern s32 D_801D8278;

extern void *D_801D8340;
extern u8 *D_801D8348[];
void func_800341EC(void) {
    s32 var_a1;
    u32 temp_v0;

    temp_v0 = (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178));
    var_a1 = (temp_v0 >> 0x11) & 1;
    if (((temp_v0 >> 0x11) & 3) == 1) {
        var_a1 = 0;
    }
    func_8001A164(&D_801D8278, var_a1);
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010C9C);

extern u8 *D_801D83EC;
extern u8 *D_801D833C;
extern MsgBar D_801D83D0;
extern u8 D_801D83D1;
extern s8 D_801D83D4;
extern s8 D_801D83D7;
extern s8 D_801D831D;
extern s32 D_8006E050;

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
s32 func_801EC8E0();
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
s32 func_801FB444();
void func_8003917C(void);
void func_80039354(s32 p, s32 v, s32 k);
s32 func_800400B4(s32 arg0, s32 arg1);
s32 func_800402CC(s32 p);
s32 func_80040220(s32 arg0);
s32 func_80040764(s32 arg0);
s32 func_80041214(s32 arg0);
s32 func_80047B84(s32 a, s32 id);
s32 func_80040468(s32 arg0);
s32 func_80040570(s32 player);
s32 func_800406BC(s32 player);
s32 func_8004080C(s32 idx, s32 p);
s32 func_80040A48(s32 p, s32 deck);
s32 func_80040D88(s32 p, s32 deck);
s32 func_800411C4(s32 arg0);
s32 func_80041340(s32 arg0);
s32 func_80041408(s32 arg0);
s32 func_80047A58(s32 arg0);
s32 func_80048014(s32 a, s32 b);
s32 func_80048150(s32 a, s32 id);
void func_80049EF8(s32 n, s32 arg1);

#define PLAYER(p) ((Player *)D_801D8348[p])
typedef struct {
    /* 0x0 */ s16 unk0;
    /* 0x2 */ s16 id;
} CardCursor;

#define CUR_CARD (((CardCursor *)DUEL->unk58)->id)
typedef struct {
    /* 0x00 */ u8 unk0[0xA5];
    /* 0xA5 */ s8 choice;
} Window;

#define CHOICE (((Window *)&D_801D8278)->choice)
#define ME DUEL->unk817
#define OPP ((s8)(DUEL->unk817 ^ 1))

void func_80034260(void) {
    char buf[0x88];
    s32 mode;
    s32 i;
    s32 j;
    s32 id;
    s8 c;
    s8 *card;
    s32 over;
    Player *a;
    Player *b;

    while (DUEL->state < 0) {
        func_80014C08(D_800794F0);
    }
    DUEL->unk815 = 0;
    for (;;) {
        func_80033D08(1);
        func_801EAB4C();
        switch (DUEL->unk818) {
        case 0:
            D_801D83EC[0x9D] = 1;
            D_801D83EC[0x175] = 1;
            D_801D83D0.next = -1;
            D_801D83D0.next2 = -1;
            func_80014C08(0x1E);
            if (PLAYER(1)->unk178_17 == 1 && ((u8 *)D_8006E054)[4] == 0x8C) {
                for (i = 0, j = 0; i < 30; i++) {
                    if ((u32)func_80047B84(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                    if ((u32)func_80048150(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                }
                if (j) {
                    for (i = 0; i < 3; i++) {
                        id = func_800402CC(0);
                        if (id == -1) {
                            break;
                        }
                        for (j = 0; j < 29; j++) {
                            PLAYER(0)->unk17D[j] = PLAYER(0)->unk17D[j + 1];
                        }
                        PLAYER(0)->unk17D[29] = id;
                    }
                    func_800149B8(0, -1, 0, 0x800, func_80049EF8, 2, func_800148B0(), 0, 0);
                    func_80014C08(0x7FFFFFFF);
                }
            }
            DUEL->unk818++;
            break;
        case 1:
            DUEL->unk822 = 0;
            if (ME == 0) {
                D_801D83EC[0xC1] = 1;
                D_801D83EC[0x199] = 6;
            } else {
                D_801D83EC[0xC1] = 6;
                D_801D83EC[0x199] = 1;
            }
            D_801D83EC[0x9D] = 1;
            D_801D83EC[0x175] = 1;
            PLAYER(0)->unk178_2 = 3;
            PLAYER(1)->unk178_2 = 3;
            DUEL->unk80A = -1;
            DUEL->unk80E = -1;
            DUEL->unk81D = -1;
            DUEL->unk818++;
            break;
        case 2:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 0;
            D_801D83D0.unk1 = PLAYER(ME)->unk178_17;
            D_801D83D0.next = 0;
            D_801D83D0.next2 = 0;
            func_80033D08(0x3C);
            while (1) {
            wait:
                if (D_801D83EC[0x9D] != 4) {
                    goto wait;
                }
                if (func_801EC570(ME) == -1) {
                    break;
                }
                func_80014C08(0x14);
                func_801FA780(ME);
            }
            DUEL->unk818++;
            break;
        case 3:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            if (func_80040764(ME) >= 0) {
                DUEL->unk818 = 4;
            } else if (func_80040570(ME) != 0) {
                if (func_80040220(ME) == 0) {
                    D_801D83D4 = 2;
                    sprintf(buf, "There are no more Cards, so %s loses!", PLAYER(ME)->unk1CE);
                    func_80019EA4((u8 *)&D_801D8278, buf, 0);
                    func_800341EC();
                    DUEL->unk81E = ME ^ 1;
                    DUEL->unk818 = 0x26;
                } else {
                    D_801D83D4 = 1;
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_80019EA4((u8 *)&D_801D8278, "Redrawing Cards because there are\nno Digimon Cards.", 0);
                        func_800341EC();
                    }
                    DUEL->unk818 = 6;
                }
            } else {
                DUEL->unk818 = 4;
            }
            break;
        case 4:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0;
            D_801D83D0.next2 = 0;
            if (func_80040220(ME) == 0) {
                DUEL->unk818 = 8;
            } else if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 1;
                func_80033E7C();
                if (DUEL->unk804 != 0) {
                    func_80033D08(0x1E);
                    DUEL->unk818 = 6;
                } else {
                    DUEL->unk818 = 8;
                }
            } else {
                DUEL->unk818++;
            }
            break;
        case 5:
            DUEL->unk822 = 1;
            D_801D83D0.next2 = 1;
            if (D_80089840[ME]->unkA & 0x10) {
                func_8002B498(0xA0);
                D_801D83D0.next = 3;
                D_801D83D0.next2 = 0;
                do {
                    func_80019EA4((u8 *)&D_801D8278, "This will discard all Cards.\nIs this OK?", 1);
                    func_800341EC();
                    switch (CHOICE) {
                    case 0:
                    case 2:
                        if (DUEL->unk81F != 0) {
                            func_801EA8B4(0x78, "Please press \"Yes\"!");
                        } else {
                            DUEL->unk818 = 4;
                        }
                        break;
                    case 1:
                        PLAYER(ME)->unk110 |= 0x20;
                        DUEL->unk818++;
                        break;
                    }
                } while (DUEL->unk818 == 5);
            } else if (D_80089840[ME]->unkA & 0x40) {
                func_8002B498(0xA0);
                DUEL->unk818 = 8;
            } else if (D_80089840[ME]->unkA & 0x80) {
                func_8002B498(0xA0);
                DUEL->unk818 = 7;
                DUEL->unk819 = 4;
                DUEL->unk81A = ME;
            }
            break;
        case 6:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0x10;
            D_801D83D0.next2 = 0;
            func_801ECC58(ME);
            DUEL->unk818 = 3;
            break;
        case 7:
            DUEL->unk822 = 1;
            D_801D83D7 = 8;
            if (DUEL->unk819 != 0x19) {
                func_801EC4CC(DUEL->unk81A);
            }
            func_801EBACC(DUEL->unk81A, 0);
            func_80033D08(0x14);
            for (;;) {
                func_80033D08(1);
                func_801EBACC(DUEL->unk81A, 0);
                if (D_80089840[DUEL->unk81A]->unkA & 0x10) {
                    func_8002B498(0xA1);
                    if (DUEL->unk819 == 0x19) {
                        D_801D83EC[0x31] = 1;
                        D_801D83EC[0x109] = 1;
                    }
                    func_801EC528(DUEL->unk81A);
                    DUEL->unk81C = -1;
                    DUEL->unk818 = DUEL->unk819;
                    func_80033D08(0x14);
                    break;
                }
            }
            break;
        case 8:
            DUEL->unk822 = 0;
            if (func_80040764(ME) >= 0) {
                if (PLAYER(ME)->unk178_17 == 1) {
                    DUEL->unk818 = 0xB;
                } else {
                    DUEL->unk818 = 0xA;
                }
            } else {
                D_801D83D4 = 4;
                if (PLAYER(ME)->unk178_17 == 1) {
                    DUEL->unk827 = ME;
                    DUEL->unk816 = 2;
                    func_80033E7C();
                    if (DUEL->unk804 == -1) {
                        for (i = 0; i < 4; i++) {
                            if (PLAYER(ME)->unk1B9[i] != -1 && PLAYER(ME)->cards[(s8)(PLAYER(ME)->unk1B9[i] % 30)].state == 0) {
                                DUEL->unk804 = PLAYER(ME)->unk1B9[i];
                                break;
                            }
                        }
                    }
                    CUR_CARD = DUEL->unk804;
                    func_801EA558(CUR_CARD, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    i = func_80047B84(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (func_80048014(ME, func_80047A58(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            func_80033D08(0x3C);
                            func_801F6214(1, ME);
                            func_80040A48(ME, i);
                        }
                    }
                    func_80033D08(0x78);
                    DUEL->unk818 = 0xB;
                } else {
                    DUEL->unk818++;
                    func_801EC4CC(ME);
                }
            }
            break;
        case 9:
            DUEL->unk822 = 1;
            if (func_80040220(ME) != 0) {
                D_801D83D7 = 5;
            } else {
                D_801D83D7 = 3;
            }
            if (func_801EBACC(ME, 1) == 0) {
                if (PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].state == 0) {
                    func_8002B498(0xA0);
                    func_801EC528(ME);
                    func_801EA558(CUR_CARD, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    i = func_80047B84(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (func_80048014(ME, func_80047A58(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            D_801D83D4 = 5;
                            func_80019EA4((u8 *)&D_801D8278, "Do you want to Armor Digivolve?", 1);
                            func_800341EC();
                            switch (CHOICE) {
                            case 0:
                                if (DUEL->unk80A >= 0) {
                                    func_801EC84C(CUR_CARD, ME, DUEL->unk80A);
                                    DUEL->unk80A = -1;
                                    DUEL->unk818 = 8;
                                } else {
                                    DUEL->unk818 = 3;
                                }
                                break;
                            case 1:
                                func_801F6214(1, ME);
                                func_80040A48(ME, i);
                                PLAYER(ME)->unk110 |= 8;
                                func_801FA4E4(ME);
                                DUEL->unk818 = 0xB;
                                break;
                            case 2:
                                DUEL->unk818++;
                                break;
                            }
                        } else {
                            DUEL->unk818++;
                        }
                    } else {
                        DUEL->unk818++;
                    }
                }
            } else if ((D_80089840[ME]->unkA & 0x10) && func_80040220(ME) != 0) {
                func_8002B498(0xA1);
                func_801EC528(ME);
                DUEL->unk818 = 4;
            }
            break;
        case 10:
            DUEL->unk822 = 0;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 6;
            func_80019EA4((u8 *)&D_801D8278, "Is it OK to end the Preparation Phase?", 1);
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Press \"Yes\" to go to the next Phase!");
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC84C(CUR_CARD, ME, DUEL->unk80A);
                        DUEL->unk80A = -1;
                    }
                    DUEL->unk818 = 4;
                }
                break;
            case 1:
                if (DUEL->unk80A >= 0) {
                    func_801FA4E4(ME);
                }
                DUEL->unk818++;
                break;
            }
            break;
        case 11:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk80A = -2;
                DUEL->unk80E = -1;
                DUEL->unk80C = -1;
                DUEL->unk827 = ME;
                DUEL->unk816 = 3;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0xE;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 7;
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801ECBCC(CUR_CARD, ME);
                    func_80033D08(0x78);
                    DUEL->unk818 = 0xE;
                }
            } else {
                DUEL->unk818++;
            }
            break;
        case 12:
            DUEL->unk822 = 0;
            DUEL->unk80A = -2;
            DUEL->unk80E = -1;
            DUEL->unk80C = -1;
            if (func_80040570(ME) != 0) {
                DUEL->unk818 = 0xE;
            } else if (func_80041214(ME) != 0) {
                func_801EC4CC(ME);
                DUEL->unk818++;
            } else {
                DUEL->unk818 = 0xE;
            }
            break;
        case 13:
            DUEL->unk822 = 1;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next = 7;
            D_801D83D0.next2 = 7;
            if (func_801EBACC(ME, 2) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                if (PLAYER(ME)->cards[i % 30].state == 0) {
                    func_8002B498(0xA0);
                    DUEL->unk80E = func_801ECBCC(CUR_CARD, ME);
                    DUEL->unk818++;
                }
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0xD) {
                func_801EC528(ME);
            }
            break;
        case 14:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 4;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x13;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 8;
                    D_801D83EC[ME * 0xD8 + 0x55] = 6;
                    func_80033D08(0x1E);
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                    func_80033D08(0x1E);
                    DUEL->unk818 = 0x10;
                }
            } else if (func_800406BC(ME) != 0) {
                DUEL->unk80A = -1;
                DUEL->unk818 = 0x13;
            } else {
                DUEL->unk818++;
                func_801EC4CC(ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 6;
                D_801D83D0.unk3 = 1;
                D_801D83D0.next = 8;
                D_801D83D0.next2 = 6;
                func_80033D08(0x1E);
            }
            break;
        case 15:
            DUEL->unk822 = 1;
            if (func_801EBACC(ME, 5) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                if (PLAYER(ME)->cards[i % 30].state == 2) {
                    func_8002B498(0xA0);
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                    if (func_801EA374(ME) != 0) {
                        func_80019EA4((u8 *)&D_801D8278, "This Digivolve Option has no Effect.\nDo you still want to use it?", 1);
                        func_800341EC();
                        switch (CHOICE) {
                        case 0:
                        case 2:
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                            break;
                        case 1:
                            func_801EC528(ME);
                            DUEL->unk818++;
                            break;
                        }
                    } else {
                        DUEL->unk818++;
                    }
                }
            } else if (D_80089840[ME]->unkA & 0x30) {
                DUEL->unk81C = -1;
                if (D_80089840[ME]->unkA & 0x20) {
                    func_8002B498(0xA0);
                    func_801EC528(ME);
                    D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    DUEL->unk818 = 0x13;
                } else if (D_80089840[ME]->unkA & 0x10) {
                    func_8002B498(0xA1);
                    if (DUEL->unk80E >= 0) {
                        func_801ECA30(func_800411C4(ME), ME, DUEL->unk80E);
                        DUEL->unk80E = -1;
                    }
                    D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    DUEL->unk818 = 0xB;
                }
            }
            break;
        case 16:
            DUEL->unk822 = 0;
            D_801D83D4 = 9;
            mode = 0;
            if (func_801EA374(ME) == 0) {
                card = PLAYER(ME)->cards[func_80041340(ME) % 30].card;
                switch (card[0x1A]) {
                case 4:
                    func_80019EA4((u8 *)&D_801D8278, "Current Digimon will be discarded,\ndo you still want to \"Digi-devolve\"?", 1);
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_800341EC();
                    } else {
                        D_801D831D = 1;
                    }
                    switch (D_801D831D) {
                    case 0:
                    case 2:
                        mode = 2;
                        func_801EC8E0(ME, DUEL->unk80A);
                        DUEL->unk80A = -2;
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                        break;
                    case 1:
                        mode = 3;
                        func_801F6214(9, ME);
                        func_801EC608(func_80040764(ME), ME);
                        PLAYER(ME)->unk178_15 = 0;
                        func_8004080C(func_80040764(ME), ME);
                        PLAYER(ME)->unk11C[0] *= 2;
                        func_80033D08(0x14);
                        break;
                    }
                    break;
                case 7:
                    func_80019EA4((u8 *)&D_801D8278, "Your Digimon's Level will become *e3,\ndo you still want to \"Armor Digi-devolve\"?", 1);
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_800341EC();
                    } else {
                        D_801D831D = 1;
                    }
                    switch (D_801D831D) {
                    case 0:
                    case 2:
                        mode = 2;
                        func_801EC8E0(ME, DUEL->unk80A);
                        DUEL->unk80A = -2;
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                        break;
                    case 1:
                        mode = 3;
                        func_801F6214(8, ME);
                        func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                        break;
                    }
                    break;
                default:
                    mode = 1;
                    break;
                }
            }
            if (mode == 0 || mode == 3) {
                DUEL->unk80C = DUEL->unk80A;
                i = func_80041408(ME);
                D_801D833C[i * 0x24 + 0x22] = 8;
                func_800400B4(i, ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 1;
                func_801EC528(ME);
            }
            switch (mode) {
            case 0:
                DUEL->unk818 = 0x13;
                break;
            case 1:
                DUEL->unk818 = 0x11;
                break;
            case 2:
                DUEL->unk818 = 0xE;
                break;
            case 3:
                PLAYER(ME)->unk110 |= 0x40000000;
                DUEL->unk818 = 0x17;
                break;
            }
            if (PLAYER(ME)->unk178_17 == 1) {
                func_80033D08(0x3C);
            }
            break;
        case 17:
            DUEL->unk822 = 0;
            DUEL->unk818 = 0x12;
            break;
        case 18:
            DUEL->unk822 = 1;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 5;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x17;
                    i = 0;
                    func_801EC528(ME);
                } else {
                    func_80033D08(0x3C);
                    CUR_CARD = DUEL->unk804;
                    i = 0;
                }
            } else {
                D_801D83D7 = 6;
                i = func_801EBACC(ME, 6);
                if (i != 0) {
                    if (D_80089840[ME]->unkA & 0x20) {
                        func_8002B498(0xA0);
                        if (DUEL->unk80A >= 0) {
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                        }
                        DUEL->unk818 = 0x13;
                        func_801EC528(ME);
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    } else if (D_80089840[ME]->unkA & 0x10) {
                        func_8002B498(0xA1);
                        if (DUEL->unk80A >= 0) {
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                        }
                        DUEL->unk818 = 0xE;
                        func_801EC528(ME);
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    }
                }
            }
            if (i == 0 && func_801E9F5C(CUR_CARD, ME) == 0) {
                func_801EC528(ME);
                card = PLAYER(ME)->cards[func_80041340(ME) % 30].card;
                switch (card[0x1A]) {
                case 0:
                    func_801F6214(3, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 1:
                    func_801F6214(4, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 2:
                    func_801F6214(5, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 3:
                    func_801F6214(7, ME);
                    func_801EC608(func_80040764(ME), ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 5:
                    func_801F6214(2, ME);
                    func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                    while (func_80040764(ME) != -1) {
                        func_801EC608(func_80040764(ME), ME);
                        func_80033D08(0x14);
                    }
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 6:
                    func_801F6214(6, ME);
                    func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                    func_801EC608(func_80040764(ME), ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                }
                PLAYER(ME)->unk110 |= 8;
                func_801FA4E4(ME);
                DUEL->unk818 = 0x17;
            }
            if (DUEL->unk818 != 0x12 && DUEL->unk818 != 0xE && DUEL->unk818 != 0x13) {
                i = func_80041408(ME);
                D_801D833C[i * 0x24 + 0x22] = 8;
                func_800400B4(i, ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 1;
                func_80033D08(0x14);
                PLAYER(ME)->unk110 |= 0x40000000;
            }
            break;
        case 19:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 5;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x17;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 9;
                    func_801F6214(0, ME);
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                }
            } else if (func_80040570(ME) != 0) {
                DUEL->unk818 = 0x16;
            } else if (func_801EA374(ME) != 0) {
                DUEL->unk818 = 0x16;
            } else {
                DUEL->unk818++;
            }
            break;
        case 20:
            DUEL->unk822 = 0;
            func_801EC4CC(ME);
            DUEL->unk818++;
            break;
        case 21:
            DUEL->unk822 = 1;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next = 9;
            if (func_801EBACC(ME, 3) == 0) {
                if (func_801E9F5C(PLAYER(ME)->unk1B9[DUEL->unk81C], ME) == 0) {
                    func_801EC528(ME);
                    func_801F6214(0, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    PLAYER(ME)->unk110 |= 8;
                    func_801FA4E4(ME);
                }
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                DUEL->unk818++;
                func_801EC528(ME);
            } else if (D_80089840[ME]->unkA & 0x10) {
                func_8002B498(0xA1);
                func_801ECD68();
                func_801EC528(ME);
            }
            break;
        case 22:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 0xA;
            D_801D83EC[ME * 0xD8 + 0x55] = 1;
            func_80019EA4((u8 *)&D_801D8278, "Is it OK to end the Digivolve Phase?", 1);
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Choose \"Yes\" to go to next Phase!");
                } else if (DUEL->unk80A == -2) {
                    DUEL->unk818 = 0xE;
                } else {
                    func_801ECD68();
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 23:
            DUEL->unk822 = 0;
            PLAYER(ME)->unk178_30 = 1;
            PLAYER(ME)->unk114 = PLAYER(ME)->cards[func_80040764(ME) % 30].card;
            if (func_80040764(OPP) == -1) {
                if (PLAYER(ME)->unk178_17 != 1) {
                    D_801D83D0.unk3 = 2;
                    D_801D83D0.next = 0xB;
                    sprintf(buf, "Since %s has no Digimon,\nthere is no Battle Phase.", PLAYER(OPP)->unk1CE);
                    func_80019EA4((u8 *)&D_801D8278, buf, 0);
                    func_800341EC();
                }
                DUEL->unk818 = 0x25;
            } else {
                DUEL->unk80A = -1;
                DUEL->unk80E = -1;
                DUEL->unk818++;
            }
            break;
        case 24:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 2;
            D_801D83D0.next = 0xC;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->unk178_17 == 1) {
                    DUEL->unk827 = i;
                    if (DUEL->unk81F != 0) {
                        DUEL->unk816 = 0;
                    } else {
                        DUEL->unk816 = 6;
                    }
                }
            }
            D_801D83EC[0x31] = 1;
            D_801D83EC[0x109] = 1;
            func_80014C08(0x1E);
            DUEL->unk818++;
            break;
        case 25:
            DUEL->unk822 = 1;
            D_801D83D7 = 2;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->unk178_17 == 1) {
                    if (DUEL->unk816 == 0 && PLAYER(i)->unk178_2 == 3) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = DUEL->unk804;
                    }
                } else if (PLAYER(i)->unk178_2 == 3) {
                    if (D_80089840[i]->unkA & 0x20) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 0;
                    } else if (D_80089840[i]->unkA & 0x10) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 1;
                    } else if (D_80089840[i]->unkA & 0x40) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 2;
                    } else if (D_80089840[i]->unkA & 0x80) {
                        func_8002B498(0xA0);
                        func_801EC4CC(i);
                        D_801D83EC[0x31] = 4;
                        D_801D83EC[0x109] = 4;
                        DUEL->unk818 = 7;
                        DUEL->unk819 = 0x19;
                        DUEL->unk81A = i;
                        break;
                    }
                }
            }
            if (PLAYER(0)->unk178_2 != 3 && PLAYER(1)->unk178_2 != 3) {
                func_80033D08(0x78);
                D_801D83EC[0x31] = 4;
                D_801D83EC[0x109] = 4;
                for (i = 0; i < 2; i++) {
                    PLAYER(i)->unk178_0 = PLAYER(i)->unk178_2;
                    if (((Unk8006E050 *)D_8006E050)[i].unk36[PLAYER(i)->unk178_0] != 0xFFFF) {
                        ((Unk8006E050 *)D_8006E050)[i].unk36[PLAYER(i)->unk178_0]++;
                    }
                }
                func_80033D08(0x78);
                DUEL->unk818++;
            }
            break;
        case 26:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0xD;
            D_801D83D0.unk1 = PLAYER(OPP)->unk178_17;
            D_801D83EC[OPP * 0xD8 + 0x55] = 6;
            func_80033D08(0x1E);
            if (PLAYER(OPP)->unk178_17 == 1) {
                D_801D83D0.next2 = 0;
                DUEL->unk827 = OPP;
                DUEL->unk816 = 7;
                func_80033E7C();
                if (DUEL->unk804 == -2) {
                    func_80033D08(0x3C);
                    func_801ECAC4(OPP);
                    func_80033D08(0x78);
                } else if (DUEL->unk804 != -1) {
                    if (PLAYER(OPP)->cards[DUEL->unk804 % 30].card[2] < 2) {
                        func_80033D08(0x3C);
                        DUEL->unk80A = func_801ECB40(DUEL->unk804, OPP);
                        func_80033D08(0x78);
                    }
                }
                DUEL->unk818 = 0x1D;
            } else {
                DUEL->unk80A = -1;
                if (func_80040468(OPP) == 4 && func_80040220(OPP) == 0) {
                    func_80019EA4((u8 *)&D_801D8278, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    func_8001A164(&D_801D8278, PLAYER(OPP)->unk178_17 & 1);
                    DUEL->unk818 = 0x1D;
                } else {
                    func_801EC4CC(OPP);
                    DUEL->unk818++;
                }
            }
            break;
        case 27:
            DUEL->unk822 = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(OPP, 4) == 0) {
                i = PLAYER(OPP)->unk1B9[DUEL->unk81C];
                c = DUEL->unk81C;
                if (c == 4) {
                    DUEL->unk80A = c;
                    func_801ECAC4(OPP);
                } else {
                    if (PLAYER(OPP)->cards[i % 30].state == 2) {
                        break;
                    }
                    DUEL->unk80A = func_801ECB40(CUR_CARD, OPP);
                }
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                DUEL->unk818++;
            } else if (D_80089840[OPP]->unkA & 0x20) {
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0x1B) {
                func_801EC528(OPP);
            }
            break;
        case 28:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            func_8001A164(&D_801D8278, PLAYER(OPP)->unk178_17 & 1);
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC8E0(OPP, DUEL->unk80A);
                    }
                    DUEL->unk818 = 0x1A;
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 29:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0xE;
            D_801D83D0.unk1 = PLAYER(ME)->unk178_17;
            D_801D83EC[ME * 0xD8 + 0x55] = 6;
            func_80033D08(0x1E);
            if (PLAYER(ME)->unk178_17 == 1) {
                D_801D83D0.next2 = 0;
                DUEL->unk827 = ME;
                DUEL->unk816 = 7;
                func_80033E7C();
                if (DUEL->unk804 == -2) {
                    func_80033D08(0x3C);
                    func_801ECAC4(ME);
                    func_80033D08(0x78);
                } else if (DUEL->unk804 != -1) {
                    if (PLAYER(ME)->cards[DUEL->unk804 % 30].card[2] < 2) {
                        func_80033D08(0x3C);
                        DUEL->unk80A = func_801ECB40(DUEL->unk804, ME);
                        func_80033D08(0x78);
                    }
                }
                DUEL->unk818 = 0x20;
            } else {
                DUEL->unk80A = -1;
                if (func_80040468(ME) == 4 && func_80040220(ME) == 0) {
                    func_80019EA4((u8 *)&D_801D8278, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    func_8001A164(&D_801D8278, PLAYER(ME)->unk178_17 & 1);
                    DUEL->unk818 = 0x20;
                } else {
                    func_801EC4CC(ME);
                    DUEL->unk818++;
                }
            }
            break;
        case 30:
            DUEL->unk822 = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(ME, 4) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                c = DUEL->unk81C;
                if (c == 4) {
                    DUEL->unk80A = c;
                    func_801ECAC4(ME);
                } else {
                    if (PLAYER(ME)->cards[i % 30].state == 2) {
                        break;
                    }
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                }
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                DUEL->unk818++;
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0x1E) {
                func_801EC528(ME);
            }
            break;
        case 31:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC8E0(ME, DUEL->unk80A);
                    }
                    DUEL->unk818 = 0x1D;
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 32:
            DUEL->unk822 = 0;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 0xF;
            func_80033D08(0x3C);
            if (((Unk8006E050 *)D_8006E050)->unk20_3) {
                D_801D8330 = 0x20;
                func_8001683C((s32)func_80033F34);
                while (D_801D8330 != 0) {
                    func_80014C08(D_800794F0);
                }
                func_80014C08(0x14);
            }
            func_801E6AA4(0);
            DUEL->unk818++;
            break;
        case 33:
            DUEL->unk822 = 0;
            D_801D83D4 = 0x11;
            DUEL->unk818++;
            break;
        case 34:
            DUEL->unk822 = 0;
            DUEL->unk81C = -1;
            if (!((Unk8006E050 *)D_8006E050)->unk20_3) {
                func_80014C08(0x3C);
                DUEL->state = 1;
                func_80014C08(2);
                DUEL->unk83C = 1;
                func_8002E26C();
                DUEL->unk83C = 0;
                func_80014C08(2);
                ((Unk800794F8 *)&D_800794F8)->unk54 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk56 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk58 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk7C = 0;
                ((Unk800794F8 *)&D_800794F8)->unk80 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk84 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk8E = 0;
                ((Unk800794F8 *)&D_800794F8)->unk90 = 0x1C0;
                ((Unk800794F8 *)&D_800794F8)->unk92 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk94 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk8C = -1;
                ((Unk800794F8 *)&D_800794F8)->unk74 = 1;
                func_80014C08(2);
                DUEL->state = 6;
            }
            DUEL->unk818++;
            break;
        case 35:
            DUEL->unk822 = 0;
            over = 0;
            if (((Unk8006E050 *)D_8006E050)->unk20_3) {
                a = DUEL->unk50;
                b = DUEL->unk54;
                j = a->unk178_17 & 1;
                func_80014C08(0x14);
                if (a->unk178_11 && !b->unk178_6) {
                    func_801F6268(0x1B, j);
                    a->unk11C[0] = 10;
                }
                if (b->unk162 == 0) {
                    func_801F6268(0x1C, j);
                } else if (a->unk178_12) {
                    func_801F6268(0x1A, j);
                    i = a->unk11C[0] + a->unk164;
                    func_80039354(j, i, 0);
                    if (i > 9990) {
                        i = 9990;
                    }
                    a->unk11C[0] = i;
                    if (i != 0 && i % 1110 == 0) {
                        func_801FB444(j, 0x1A);
                    }
                } else if (!a->unk178_11) {
                    func_801F6268(0x18, j);
                }
                i = b->unk11C[0] - b->unk162;
                if (b->unk162 != 0) {
                    func_80039354(j ^ 1, i, 0);
                    if (b->unk162 != 0 && b->unk162 % 1110 == 0) {
                        func_801FB444(j, 0x19);
                    }
                }
                if (i == 0 && a->unk17C == 2) {
                    func_801FB444(j, 0x13);
                }
                if (i < 0) {
                    i = 0;
                }
                b->unk11C[0] = i;
                if (i != 0 && i % 1110 == 0 && b->unk162 != 0) {
                    func_801FB444(j ^ 1, 0x1A);
                }
                func_8003917C();
                if (func_801ECF0C(j ^ 1) != 0) {
                    over = 1;
                }
                func_80014C08(0x14);
                if (b->unk15A != 0) {
                    if (b->unk178_11 && a->unk162 != 0) {
                        func_801F6268(0x1B, j ^ 1);
                        b->unk11C[0] = 10;
                    }
                    if (a->unk162 == 0) {
                        func_801F6268(0x1C, j ^ 1);
                    } else if (b->unk178_6 || b->unk178_12) {
                        if (b->unk178_6) {
                            func_801F6268(0x19, j ^ 1);
                        }
                        if (b->unk178_12) {
                            func_801F6268(0x1A, j ^ 1);
                            i = b->unk11C[0] + b->unk164;
                            func_80039354(j ^ 1, i, 0);
                            if (i > 9990) {
                                i = 9990;
                            }
                            b->unk11C[0] = i;
                            if (i != 0 && i % 1110 == 0) {
                                func_801FB444(j ^ 1, 0x1A);
                            }
                        }
                    } else if (!b->unk178_11) {
                        func_801F6268(0x18, j ^ 1);
                    }
                    i = a->unk11C[0] - a->unk162;
                    if (a->unk162 != 0) {
                        func_80039354(j, i, 0);
                        if (a->unk162 != 0 && a->unk162 % 1110 == 0) {
                            func_801FB444(j ^ 1, 0x19);
                        }
                    }
                    if (i == 0 && b->unk17C == 2) {
                        func_801FB444(j ^ 1, 0x13);
                    }
                    if (i < 0) {
                        i = 0;
                    }
                    a->unk11C[0] = i;
                    if (i != 0 && i % 1110 == 0 && a->unk162 != 0) {
                        func_801FB444(j, 0x1A);
                    }
                    func_8003917C();
                    if (func_801ECF0C(j) != 0) {
                        over = 1;
                    }
                }
                func_80016878((s32)func_80033F34);
                if (over) {
                    DUEL->unk818++;
                } else {
                    DUEL->unk818 = 0x25;
                }
            } else {
                func_80014C08(0x3C);
                PLAYER(0)->unk11C[0] = PLAYER(0)->unk15A;
                PLAYER(1)->unk11C[0] = PLAYER(1)->unk15A;
                if (PLAYER(ME)->unk11C[0] == PLAYER(ME)->unk126[0] && PLAYER(OPP)->unk11C[0] == PLAYER(OPP)->unk126[0]) {
                    if (func_801ECF0C(ME) != 0) {
                        DUEL->unk818++;
                    } else if (func_801ECF0C(OPP) != 0) {
                        DUEL->unk818++;
                    } else {
                        DUEL->unk818 = 0x25;
                    }
                }
            }
            break;
        case 36:
            DUEL->unk822 = 0;
            DUEL->unk818++;
            if (PLAYER(DUEL->unk81E)->unk17C == 2 && PLAYER(DUEL->unk81E ^ 1)->unk17C == 0) {
                PLAYER(DUEL->unk81E ^ 1)->unk110 |= 0x80;
                PLAYER(DUEL->unk81E)->unk110 |= 0x100;
            }
            if (func_80048150(DUEL->unk81E, PLAYER(DUEL->unk81E)->cards[func_80040764(DUEL->unk81E) % 30].id) >= 0) {
                func_801FB444(DUEL->unk81E, 0xA);
                PLAYER(DUEL->unk81E)->unk110 |= 0x4000;
            } else if (func_80047B84(DUEL->unk81E, PLAYER(DUEL->unk81E)->cards[func_80040764(DUEL->unk81E) % 30].id) >= 0) {
                func_801FB444(DUEL->unk81E, 0xA);
                PLAYER(DUEL->unk81E)->unk110 |= 0x4000;
            }
            if (PLAYER(DUEL->unk81E)->unk17C == 3) {
                if (PLAYER(DUEL->unk81E)->unk178_31) {
                    PLAYER(DUEL->unk81E)->unk110 |= 0x20000;
                    PLAYER(DUEL->unk81E ^ 1)->unk110 |= 0x40000;
                }
                sprintf(buf, "%d Wins, %d Losses-%s WINS!", PLAYER(DUEL->unk81E)->unk17C, PLAYER(DUEL->unk81E ^ 1)->unk17C, PLAYER(DUEL->unk81E)->unk1CE);
                func_80019EA4((u8 *)&D_801D8278, buf, 0);
                func_800341EC();
                DUEL->unk818 = 0x26;
            } else if (func_80040764(DUEL->unk81E ^ 1) == -1 && func_80040570(DUEL->unk81E ^ 1) != 0 && func_80040220(DUEL->unk81E ^ 1) == 0) {
                sprintf(buf, "Since %s has no more Digimon,\nthe winner is %s!", PLAYER(DUEL->unk81E ^ 1)->unk1CE, PLAYER(DUEL->unk81E)->unk1CE);
                func_80019EA4((u8 *)&D_801D8278, buf, 0);
                func_800341EC();
                DUEL->unk818 = 0x26;
            }
            break;
        case 37:
            DUEL->unk822 = 0;
            D_801D83D1 = PLAYER(ME)->unk178_17;
            DUEL->unk818++;
            for (i = 0; i < 2; i++) {
                PLAYER(i)->unk11C[1] = PLAYER(i)->unk15C[0];
                PLAYER(i)->unk11C[2] = PLAYER(i)->unk15C[1];
                PLAYER(i)->unk11C[3] = PLAYER(i)->unk15C[2];
                j = func_80041408(i);
                if (j != -1) {
                    D_801D833C[j * 0x24 + 0x22] = 8;
                    func_800400B4(j, i);
                }
                D_801D83EC[0x55] = 1;
                D_801D83EC[0x12D] = 1;
            }
            DUEL->unk817 ^= 1;
            DUEL->unk818 = 1;
            break;
        case 38:
            DUEL->unk822 = 0;
            DUEL->unk818++;
        case 39:
            DUEL->unk822 = 0;
            break;
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80038F68);

void func_8003917C(void) {
    s32 diff;
    s32 p;
    s32 i;

    do {
        func_80014C08(D_800794F0);
        diff = 0;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 5; i++) {
                if (((Player *)D_801D8348[p])->unk11C[i] !=
                    ((Player *)D_801D8348[p])->unk126[i]) {
                    diff = 1;
                }
            }
        }
    } while (diff);
}

extern u8 *D_801D833C;
extern u8 *D_801D83EC;
int abs(int);
s32 func_80040764(s32);
void func_8004480C(void *, s32);

#define PLAYER(p) ((Player *)D_801D8348[p])

void func_80039220(s32 p) {
    s32 idx;
    u8 *q;

    idx = func_800411C4(p);
    if (idx != -1) {
        q = D_801D83EC + (p * 0xD8 + 0x48);
        PLAYER(p)->unk130[4].value = (s8)PLAYER(p)->cards[idx % 30].card[0x1C];
        PLAYER(p)->unk130[4].type = 5;
        PLAYER(p)->unk130[4].timer = 0x30;
        PLAYER(p)->unk130[4].x = *(s16 *)(q + 0x10) + (s16)(p * 93 + 0x10);
        PLAYER(p)->unk130[4].y = *(u16 *)(q + 0x12) + 2;
    } else {
        PLAYER(p)->unk130[4].timer = 0;
    }
}

void func_80039354(s32 p, s32 v, s32 k) {
    s32 c;
    u8 *q;

    PLAYER(p)->unk130[k].value = v - PLAYER(p)->unk11C[k];
    if (PLAYER(p)->unk130[k].value == 0) {
        PLAYER(p)->unk130[k].type = 7;
    } else if (PLAYER(p)->unk130[k].value > 0) {
        PLAYER(p)->unk130[k].type = 5;
    } else {
        PLAYER(p)->unk130[k].type = 2;
    }
    PLAYER(p)->unk130[k].value = abs(PLAYER(p)->unk130[k].value);
    PLAYER(p)->unk130[k].timer = 0x30;
    if (k == 0) {
        c = func_80040764(p);
        func_8004480C(*(void **)(D_801D833C + c * 36), c);
        PLAYER(p)->unk130[0].x = *(u16 *)(*(u8 **)(D_801D833C + c * 36) + 0x34) + 0x19;
        PLAYER(p)->unk130[0].y = *(u16 *)(*(u8 **)(D_801D833C + c * 36) + 0x36) + 0x15;
    } else {
        q = D_801D83EC + (p * 0xD8 + 0x48);
        PLAYER(p)->unk130[k].x = *(u16 *)(q + 0x10) + p * 25 + 0x1C;
        PLAYER(p)->unk130[k].y = *(s16 *)(q + 0x12) + (s16)((k - 1) * 13 + 3);
    }
}

extern s32 D_8006E298;
void func_8002961C(s32, s32, u8 *, u8 *, s32, s32);

void func_800395A0(void) {
    char buf[24];
    s32 p;
    s32 k;
    char *sign;
    s32 size;

    for (p = 0; p < 2; p++) {
        for (k = 4; k >= 0; k--) {
            if (((Player *)D_801D8348[p])->unk130[k].timer != 0) {
                ((Player *)D_801D8348[p])->unk130[k].timer--;
                if (((Player *)D_801D8348[p])->unk130[k].type == 7) {
                    sign = "=";
                } else if (((Player *)D_801D8348[p])->unk130[k].type == 5) {
                    sign = "+";
                } else {
                    sign = "-";
                }
                size = ((Player *)D_801D8348[p])->unk130[k].timer;
                if (size < 0x2C) {
                    size = 0x2C;
                }
                sprintf(buf, "%s%d", sign, ((Player *)D_801D8348[p])->unk130[k].value);
                func_8002961C(((Player *)D_801D8348[p])->unk130[k].x + (0x30 - size),
                              ((Player *)D_801D8348[p])->unk130[k].y - (0x30 - size) * 2, (u8 *)buf, (u8 *)&D_8006E298,
                              ((Player *)D_801D8348[p])->unk130[k].type, 0);
            }
        }
    }
}

typedef struct {
    /* 0x00 */ u8 unk0[4];
    /* 0x04 */ s16 clut;
    /* 0x06 */ u8 unk6[6];
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 state;
    /* 0x0E */ u8 unkE[2];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[8];
    /* 0x1C */ s32 z;
    /* 0x20 */ u8 unk20[4];
} InfoPanel;

extern u8 *D_8006E47C[];
extern u8 *D_8006E4BC[];
extern u8 D_8006E4FC[];
s32 func_80040220(s32);
s32 func_80040124(s32);
s32 func_80041214(s32);
void func_80042BBC(s32, s32, s32, s32, u8 *);
void func_800446A4(s32, s32, s32);
void func_80044504(s32, s32, s32, s32, s32);

#define SPRITE(c) (*(void **)(D_801D833C + (c) * 36))
#define SPRITE_KIND(c) (*(s8 *)(D_801D833C + (c) * 36 + 0x22))

void func_80039730(s32 n, s32 z) {
    s32 p = n / 6;
    InfoPanel *panel = (InfoPanel *)D_801D83EC + n;
    char buf[72];
    u8 rgb[2][4] = { { 0x80, 0x80, 0x80, 0 }, { 0x40, 0x40, 0x40, 0 } };
    char buf2[40];
    u8 *cols[10];
    CardInfo *card;
    s32 color;
    s32 i;
    s32 k;
    s32 rival;
    s32 x;
    s32 y;

    switch (n) {
    case 2:
    case 8: {
        s32 idx;

        idx = func_80040764(p);
        if (idx >= 0) {
            color = PLAYER(p)->unk178_15 ? 3 : 7;
            card = (CardInfo *)PLAYER(p)->cards[idx % 30].card;
            func_80027DB8(panel->x - p * 14 + 17, panel->y + 2, (s32)card->name, 7, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[1]);
            func_80028D18(panel->x + 42 + p * 25, panel->y + 11, (s32)buf, color, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[2]);
            func_80028D18(panel->x + 42 + p * 25, panel->y + 24, (s32)buf, color, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[3]);
            func_80028D18(panel->x + 42 + p * 25, panel->y + 37, (s32)buf, color, z);
            func_80027DB8(panel->x + 24 + p * 24, panel->y + 51, (s32)D_8006E47C[card->unkE4], 7, z);
        }
        sprintf(buf, "*s0%2d", PLAYER(p)->unk126[4]);
        func_80028D18(panel->x + 6 + p * 93, panel->y + 9, (s32)buf, 7, z);
        k = 8 - func_80041214(p);
        if (k != 0) {
            CUR_SPRT->sp.x0 = panel->x + 3 + p * 94;
            CUR_SPRT->sp.y0 = panel->y + 50;
            CUR_SPRT->sp.u0 = 0xF0;
            CUR_SPRT->sp.v0 = 0x47;
            CUR_SPRT->sp.clut = getClut(800, k + 0x1F7);
            CUR_SPRT->sp.w = 16;
            CUR_SPRT->sp.h = 8;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        }
        break;
    }
    case 5:
    case 11:
        if (panel->state == 5) {
            CUR_SPRT->sp.x0 = panel->x + 5;
            CUR_SPRT->sp.y0 = panel->y - 56 + p * 64;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7CF3;
            CUR_SPRT->sp.w = 32;
            CUR_SPRT->sp.h = 62;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x3D);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        }
        break;
    case 4:
    case 10:
        sprintf(buf, "%s Deck", PLAYER(p)->unk0 + 1);
        func_80028D18(panel->x + 1 + (0x82 - func_800293FC((u8 *)buf)) / 2, panel->y + 0x33 + p * -50, (s32)buf, 7, z);
        func_80028D18(panel->x + 0x85 + (0x78 - func_800293FC((u8 *)PLAYER(p)->unk1CE)) / 2, panel->y + 0x33 + p * -50,
                      (s32)PLAYER(p)->unk1CE, 7, z);
        k = func_80040220(p) >= 8 ? 7 : 2;
        sprintf(buf, "*s0%2d", func_80040220(p));
        func_80028D18(panel->x + 4 + p * 0xEC, panel->y + 0x1E + p * 14, (s32)buf, k, z);
        sprintf(buf, "*s0%2d", func_80040124(p));
        func_80028D18(panel->x + 4 + p * 0xEC, panel->y + 6 + p * 14, (s32)buf, 7, z);
        for (k = 0; k < PLAYER(p)->unk17C; k++) {
            func_800446A4(panel->x + 0xDF + p * -0xDD, panel->y + 4 + p * 13 + k * 15, 0x4A);
        }
        break;
    case 0:
    case 6: {
        s32 back;
        s32 shift;

        if (*(s16 *)(DUEL->unk58 + 2) == -1) {
            break;
        }
        back = DUEL->cache[DUEL->unk826].used;
        if (back == 1) {
            if (func_80029990() != 0) {
                break;
            }
            CUR_SPRT->sp.x0 = panel->x;
            CUR_SPRT->sp.y0 = panel->y + 7;
            CUR_SPRT->sp.u0 = (DUEL->unk826 & 1) << 6;
            CUR_SPRT->sp.v0 = ((DUEL->unk826 >> 1) << 6) + 0x40;
            CUR_SPRT->sp.clut = (0x1FF - DUEL->unk826) << 6;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x9A);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        } else if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19 || DUEL->unk81C == 4) {
            func_80042BBC(panel->x, panel->y + 7, z, p, 0);
        } else {
            func_80042BBC(panel->x, panel->y + 7, z, p, SPRITE(*(s16 *)(DUEL->unk58 + 2)));
        }
        if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19) {
            func_80028D18(panel->x + 0x8E, panel->y + 0x10, (s32)"*h-1All-or-Nothing\nGamble!", 7, z);
            break;
        }
        if (DUEL->unk81C == 4) {
            if (DUEL->unk81D == 4) {
                sprintf(buf2, "*h-1All-or-Nothing\nGamble!\nCards left in the\nOnline Deck are %d.",
                        func_80040220(DUEL->unk81B));
            } else {
                sprintf(buf2, "*h-1Cards left in the\nOnline Deck is %d.", func_80040220(DUEL->unk81B));
            }
            func_80028D18(panel->x + 0x8E, panel->y + 0x10, (s32)buf2, 7, z);
            break;
        }
        for (i = 0; i < 10; i++) {
            cols[i] = rgb[0];
        }
        switch (DUEL->unk81D) {
        case 1:
            cols[0] = rgb[1];
            cols[1] = rgb[1];
            cols[7] = rgb[1];
            cols[8] = rgb[1];
            cols[9] = rgb[1];
            break;
        case 2:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[1] = rgb[0];
            break;
        case 3:
        case 6:
            cols[1] = rgb[1];
            cols[7] = rgb[1];
            cols[8] = rgb[1];
            cols[9] = rgb[1];
            break;
        case 4:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[7] = rgb[0];
            cols[8] = rgb[0];
            break;
        case 5:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[9] = rgb[0];
            break;
        }
        switch (PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].state) {
        case 0:
            card = (CardInfo *)PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            sprintf(buf, "*s0%2d", card->unk1B);
            func_80028D48(panel->x + 0x7A, panel->y + 0x17, (s32)buf, (s32 *)cols[0], 7, z);
            sprintf(buf, "*s0%2d", card->level);
            func_80028D48(panel->x + 0x7C, panel->y + 0x2D, (s32)buf, (s32 *)cols[1], 7, z);
            if (DUEL->unk81D == 1 || DUEL->unk81D == 3) {
                if (DUEL->unk81C < 4 && DUEL->unk81B == DUEL->unk817) {
                    if (DUEL->unk81D == 1) {
                        shift = card->attr & 0xF;
                    } else {
                        shift = PLAYER(p)->unk178_15;
                    }
                    shift--;
                    color = 3;
                    if (shift <= 0) {
                        color = 7;
                        shift = 0;
                    }
                    sprintf(buf, "*s0%4d", (card->hp >> shift) / 10 * 10);
                    func_80028D48(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], color, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(buf, "*s0%4d", (card->attack[i].power >> shift) / 10 * 10);
                        x = panel->x;
                        y = panel->y;
                        func_80028D48(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], color, z);
                    }
                    func_800299DC(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                } else if (DUEL->unk81C == 6) {
                    color = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                    for (i = 0; i < 4; i++) {
                        sprintf(buf, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                        x = panel->x;
                        y = panel->y;
                        func_80028D48(x + 0x56, i * 12 + y + 0xE, (s32)buf, (s32 *)cols[i + 2], color, z);
                    }
                    func_800299DC(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
                } else {
                    sprintf(buf, "*s0%4d", card->hp);
                    func_80028D48(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], 7, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(buf, "*s0%4d", card->attack[i].power);
                        x = panel->x;
                        y = panel->y;
                        func_80028D48(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], 7, z);
                    }
                    func_800299DC(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                }
            } else if (DUEL->unk81C == 6) {
                color = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                for (i = 0; i < 4; i++) {
                    sprintf(buf, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                    x = panel->x;
                    y = panel->y;
                    func_80028D48(x + 0x56, i * 12 + y + 0xE, (s32)buf, (s32 *)cols[i + 2], color, z);
                }
                func_800299DC(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
            } else {
                sprintf(buf, "*s0%4d", card->hp);
                func_80028D48(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], 7, z);
                for (i = 0; i < 3; i++) {
                    sprintf(buf, "*s0%4d", card->attack[i].power);
                    x = panel->x;
                    y = panel->y;
                    func_80028D48(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], 7, z);
                }
                func_800299DC(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
            }
            func_80027DE8(panel->x + 0x44, panel->y + 0x40, D_8006E47C[card->unkE4], 7, cols[6], z);
            if (D_8006E4FC[card->unkE4] != 0) {
                func_800299DC(panel->x + 0x75, panel->y + 0x3B, 0, D_8006E4FC[card->unkE4] + 0x14, z);
            }
            func_80028D18(panel->x + 0x44, panel->y + 1, (s32)card->name, 7, z);
            func_800299DC(panel->x + 0xD4, panel->y + 1, 0, (card->attr & 0xF) + 0x10, z);
            if (card->unkE6 != 0) {
                func_800299DC(panel->x + 0xE3, panel->y + 2, 0, card->unkE6 + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                func_80028D48(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)card->text[i], (s32 *)cols[7], 7, z);
            }
            break;
        case 1: {
            s8 *opt;

            opt = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            func_80028D18(panel->x + 0x44, panel->y + 1, (s32)(opt + 3), 7, z);
            func_800299DC(panel->x + 0xC3, panel->y + 1, 0, 5, z);
            if (opt[0x8C] != 0) {
                func_800299DC(panel->x + 0xE3, panel->y + 2, 0, opt[0x8C] + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                func_80028D48(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(opt + 0x8D + i * 21), (s32 *)cols[8], 7,
                              z);
            }
            break;
        }
        case 2: {
            s8 *opt;

            opt = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            func_80028D18(panel->x + 0x44, panel->y + 1, (s32)(opt + 3), 7, z);
            func_800299DC(panel->x + 0xC3, panel->y + 1, 0, 6, z);
            for (i = 0; i < 4; i++) {
                func_80028D48(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(opt + 0x1B + i * 21), (s32 *)cols[9], 7,
                              z);
            }
            break;
        }
        }
        break;
    }
    case 1:
    case 7:
        card = (CardInfo *)PLAYER(p)->cards[func_80040764(p) % 30].card;
        color = PLAYER(p)->unk178_15 ? 3 : 7;
        func_80028D18(panel->x + 4, panel->y + 1, (s32)card->name, 6, z);
        sprintf(buf, "*s0%4d", PLAYER(p)->unk11C[0]);
        func_80028D18(panel->x + 0x82, panel->y + 1, (s32)buf, color, z);
        func_80029A0C(panel->x + 0xA4, panel->y + 2, 0, (card->attr & 0xF) + 0x10, rgb[0], z);
        func_80029A0C(panel->x + 0xB6, panel->y + 2, 0, PLAYER(p)->unk178_19, rgb[0], z);
        for (k = 0; k < 3; k++) {
            func_80028D18(panel->x + 0x25, panel->y + 13 + k * 12, (s32)card->attack[k].name, 7, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk15C[k]);
            func_80028D18(panel->x + 0xA2, panel->y + 13 + k * 12, (s32)buf, color, z);
        }
        func_80028D18(panel->x + 0x47, panel->y + 0x32, (s32)D_8006E4BC[card->unkE4], 7, z);
        if (D_8006E4FC[card->unkE4] != 0) {
            func_800299DC(panel->x + 0x95, panel->y + 0x32, 0, D_8006E4FC[card->unkE4] + 0x14, z);
        }
        if (PLAYER(p)->unk178_2 != 3) {
            if (PLAYER(p)->unk178_4 != PLAYER(p)->unk178_2) {
                PLAYER(p)->unk16E = 0;
            }
            PLAYER(p)->unk178_4 = PLAYER(p)->unk178_2;
            if (PLAYER(p)->unk16E < 28) {
                PLAYER(p)->unk16E++;
                panel->clut = getClut(784, p * 8 + 0x1F0 + PLAYER(p)->unk16E / 4);
            } else {
                panel->clut = getClut(784, p * 8 + 0x1F7);
            }
        } else {
            PLAYER(p)->unk178_4 = 3;
            panel->clut = getClut(784, p * 8 + 0x1F0);
        }
        rival = DUEL->unk817 != p;
        func_80044504(panel->x + 0xA7, panel->y + 0x32, rival, 0x80, z);
        break;
    }
}

void func_80044800(void);
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

void func_80044AB0(CardSprite *, s32);

typedef struct {
    /* 0x00 */ CardSprite *spr;
    /* 0x04 */ s32 x;
    /* 0x08 */ s32 y;
    /* 0x0C */ s32 z;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s16 rx;
    /* 0x16 */ s16 ry;
    /* 0x18 */ s16 rz;
    /* 0x1A */ s16 unk1A;
    /* 0x1C */ s16 scale;
    /* 0x1E */ s16 total;
    /* 0x20 */ s16 count;
    /* 0x22 */ s8 state;
    /* 0x23 */ s8 unk23;
} CardAnim;

typedef struct {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ u8 unkC;
    /* 0x0D */ u8 unkD[3];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[0x10];
} BoardSlot;

typedef struct {
    /* 0x00 */ u8 unk0[0x48];
    /* 0x48 */ BoardSlot slot[3];
    /* 0xB4 */ u8 unkB4[0x24];
} Board;

#define ANIM_SAVE(a)                   \
    (a)->x = (a)->spr->pos.vx;         \
    (a)->y = (a)->spr->pos.vy;         \
    (a)->z = (a)->spr->pos.vz;         \
    (a)->rx = (a)->spr->rot.vx;        \
    (a)->ry = (a)->spr->rot.vy;        \
    (a)->rz = (a)->spr->rot.vz;        \
    (a)->scale = (a)->spr->scale

#define ANIM_STEP(a, TX, TY, RX, RY, RZ, SC)                                  \
    (a)->count--;                                                             \
    (a)->spr->pos.vx = TX - (TX - (a)->x) * (a)->count / (a)->total;         \
    (a)->spr->pos.vy = TY - (TY - (a)->y) * (a)->count / (a)->total;         \
    (a)->spr->pos.vz = 0 - (0 - (a)->z) * (a)->count / (a)->total;            \
    (a)->spr->rot.vx = RX - (RX - (a)->rx) * (a)->count / (a)->total;        \
    (a)->spr->rot.vy = RY - (RY - (a)->ry) * (a)->count / (a)->total;        \
    (a)->spr->rot.vz = RZ - (RZ - (a)->rz) * (a)->count / (a)->total;        \
    (a)->spr->scale = SC - (SC - (a)->scale) * (a)->count / (a)->total

#define SLOT(p, o) ((BoardSlot *)(D_801D83EC + (p) * 0xD8 + (o)))
typedef struct {
    /* 0x00 */ u8 unk0[0x28];
    /* 0x28 */ s16 rx;
    /* 0x2A */ s16 ry;
    /* 0x2C */ s16 rz;
    /* 0x2E */ u8 unk2E[0xE];
} Unk7F8;

#define UNK7F8(c) ((*(Unk7F8 **)((u8 *)D_801D8340 + 0x7F8))[c])

void func_80039220(s32 p);

void func_8003B210(s32 c, s32 p) {
    CardAnim *a;

    a = (CardAnim *)(D_801D833C + c * 36);
    a->spr->flags |= 0x80;
    switch (SPRITE_KIND(c)) {
    case 0:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x80 + p * 0xBE;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x54 + p * 0xE;
        a->spr->pos.vz = 0;
        UNK7F8(c).rx = 0x2000;
        UNK7F8(c).ry = 0x2800;
        UNK7F8(c).rz = 0x1C00;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        a->count = 0;
        break;
    case 1:
    case 21:
    case 26:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
        break;
    case 2:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x80 + p * 0xBE);
            ty = (s16)(SLOT(p, 0x90)->y - 0x54 + p * 0xE);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x1C00;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state = 0;
        }
        break;
    case 3:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
    case 4:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B);
            ty = (s16)(SLOT(p, 0x90)->y - 0x69 + p * 0x21);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            if (a->count != 0) {
                break;
            }
        }
        a->total = 4;
        a->count = 4;
        a->state++;
        func_8002B498(0xA7);
        break;
    case 5:
    case 13:
        if (--a->count == 0) {
            ANIM_SAVE(a);
            a->total = 0xE;
            a->count = 0xC;
            a->state++;
        }
        break;
    case 6:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B);
            ty = (s16)(SLOT(p, 0x90)->y - 0x61 + p * 0x11);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 7:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x61 + p * 0x11;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        break;
    case 8:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
    case 9:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x80 + p * 0xBE);
            ty = (s16)(SLOT(p, 0x90)->y - 0x6C + p * 0xE);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2400;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 10:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x80 + p * 0xBE;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x6C + p * 0xE;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2400;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        break;
    case 11:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
    case 12: {
        s32 n;
        s32 i;

            n = 0;
            if (a->count != 0) {
                s32 tx;
                s32 ty;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 sc;

                for (i = 2; i >= 0; i--) {
                    if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                        break;
                    }
                    n++;
                }
                tx = (s16)(SLOT(p, 0x48)->x + (s16)(n * 2 - 0x46) + (s16)((-0x40 - (n * 2 + 8) * 2) * p + 8));
                ty = (s16)(SLOT(p, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                sc = 0x1000;
                ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            } else {
                func_8002B498(0xA7);
                a->total = 4;
                a->count = 4;
                a->state++;
            }
            break;
    }
    case 14: {
        s32 n;
        s32 i;

            n = 0;
            if (a->count != 0) {
                s32 tx;
                s32 ty;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 sc;

                for (i = 2; i >= 0; i--) {
                    if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                        break;
                    }
                    n++;
                }
                tx = (s16)(SLOT(p, 0x48)->x + (s16)(n * 2 - 0x46) + (s16)((-0x40 - n * 4) * p));
                ty = (s16)(SLOT(p, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                sc = 0x1000;
                ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            } else {
                a->state++;
                func_8002B498(0xA7);
            }
            break;
    }
    case 15: {
        s32 n;
        s32 i;

            n = 0;
            for (i = 2; i >= 0; i--) {
                if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                    break;
                }
                n++;
            }
            a->spr->pos.vx = SLOT(p, 0x48)->x - 0x46 + n * 2 + (-0x40 - n * 4) * p;
            a->spr->pos.vy = SLOT(p, 0x48)->y - 0x54;
            a->spr->pos.vz = 0;
            a->spr->rot.vx = 0x2000;
            a->spr->rot.vy = 0x2000;
            a->spr->rot.vz = 0x2000;
            a->spr->scale = 0x1000;
            a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x48)->unkC;
            break;
    }
    case 16:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        if (a->spr->rot.vy == 0x2000) {
            func_8002B498(0xA5);
        } else {
            func_8002B498(0xA6);
        }
        a->state++;
        break;
    case 17:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x50 + p * -0x3E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            func_8002B498(0xA7);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 18:
        if (--a->count == 0) {
            ANIM_SAVE(a);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 19:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x58 + p * -0x2E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 20:
        a->spr->pos.vx = SLOT(p, 0x6C)->x - p - 0x89;
        a->spr->pos.vy = SLOT(p, 0x6C)->y - p * 0x2E - 0x58;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x6C)->unkC;
        break;
    case 22:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x50 + p * -0x3E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->total = 0x20;
            a->count = 0x20;
            a->state++;
        }
        break;
    case 23:
        if (a->count != 0) {
            a->count--;
            ANIM_SAVE(a);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 24:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x58 + p * -0x2E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 25:
        a->spr->pos.vx = SLOT(p, 0x6C)->x - p - 0x89;
        a->spr->pos.vy = SLOT(p, 0x6C)->y - p * 0x2E - 0x58;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2800;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        break;
    case 27:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x48)->x - 0x94 + p * 0x5D);
            ty = (s16)(SLOT(p, 0x48)->y - 0x54);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_80039220(p);
            func_8002B498(0xA7);
        }
        break;
    case 28:
        a->spr->pos.vx = SLOT(p, 0x48)->x - 0x94 + p * 0x5D;
        a->spr->pos.vy = SLOT(p, 0x48)->y - 0x54;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x48)->unkC;
        break;
    case 29:
        ANIM_SAVE(a);
        a->total = 0x20;
        a->count = 0x20;
        a->state++;
        break;
    case 30:
        if (a->count != 0) {
            s32 ty;
            s16 r;

            ty = (s16)(0x3C - p * 0x78);
            r = 0x2000;
            ANIM_STEP(a, 0, ty, r, r, r, r);
        } else {
            a->state++;
        }
        break;
    case 31:
        a->spr->pos.vx = 0;
        a->spr->pos.vy = 0x3C - p * 0x78;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x2000;
        break;
    case 32:
        ANIM_SAVE(a);
        a->total = 0x20;
        a->count = 0x20;
        func_8002B498(0xA6);
        a->state++;
        break;
    case 33:
        if (a->count != 0) {
            s32 ty;
            s16 ry;

            s16 r;

            ty = (s16)(0xA0 - p * 0x140);
            r = 0x2000;
            ry = 0x2800 - (p << 12);
            ANIM_STEP(a, 0, ty, r, ry, r, r);
        } else {
            a->state++;
        }
        break;
    case 34:
        a->spr->pos.vx = 0;
        a->spr->pos.vy = 0xA0 - p * 0x140;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2800 - (p << 12);
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x2000;
        break;
    }
}



void func_8003D4C4(void) {
    char buf[8];
    Rect16 rect;
    u8 rgb[4] = "@@@";
    s32 i;
    s32 j;
    s32 done;
    s8 c;
    s32 color;
    s32 z;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 30; j++) {
            func_8003B210(i * 30 + j, i);
        }
    }
    func_80044800();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            c = PLAYER(i)->unk1C2[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
                if (SPRITE_KIND(c) == 0x1C) {
                    break;
                }
            }
        }
        c = PLAYER(i)->unk1CD;
        if (c >= 0) {
            func_80044AB0(SPRITE(c), c);
        }
        done = 0;
        for (j = 0; j < 3; j++) {
            c = PLAYER(i)->unk1CA[j];
            if (c >= 0) {
                if (!done) {
                    ((u8 *)SPRITE(c))[0x14] = PLAYER(i)->unk178_19;
                    done = 1;
                    if (SPRITE_KIND(c) < 0x1D) {
                        color = PLAYER(i)->unk178_15 ? 3 : 7;
                        func_8004480C(SPRITE(c), c);
                        z = *(s32 *)((u8 *)SPRITE(c) + 0x38);
                        func_800299DC(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 2, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30, 0,
                                      0x1A, z);
                        sprintf(buf, "%4d", PLAYER(i)->unk126[0]);
                        func_80028D18(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 15, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30,
                                      (s32)buf, color, z);
                        rect.x = 0x60;
                        rect.y = 0xDB;
                        rect.w = 0x26;
                        rect.h = 0xC;
                        func_80027228(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 1, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30,
                                      &rect, rgb, getTPage(0, 2, D_801D6B12, D_801D6B14), 0xC, z);
                        done = 1;
                    }
                }
                func_80044AB0(SPRITE(c), c);
            }
        }
        for (j = 0; j < 30; j++) {
            c = PLAYER(i)->unk19B[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
                if (SPRITE_KIND(c) == 10) {
                    break;
                }
            }
        }
        for (j = 3; j >= 0; j--) {
            c = PLAYER(i)->unk1B9[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
            }
        }
        for (j = 0; j < 30; j++) {
            c = PLAYER(i)->unk17D[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
                if (SPRITE_KIND(c) == 0) {
                    break;
                }
            }
        }
    }
}

/* the original file padded its strings with an empty word here */
__asm__(".section .rodata\n\t.word 0\n\t.section .text\n");

void func_8003D9C0(Panel *p, s16 x, s16 y, s32 speed) {
    if (speed == 0) {
        speed = 1;
    }
    p->unkC |= 0x80;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        p->unk18 = p->unk10 - p->parent->unk10;
        p->unk1A = p->unk12 - p->parent->unk12;
    } else {
        p->unk18 = p->unk10;
        p->unk1A = p->unk12;
    }
    p->unk14 = x;
    p->unk16 = y;
    p->unkE = speed;
    p->unkF = speed;
    p->unkD++;
}

s32 func_8003DA64(Panel *p) {
    s16 px;
    s16 py;

    px = 0;
    py = 0;
    p->unkF--;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        px = p->parent->unk10;
        py = p->parent->unk12;
    }
    p->unk10 = px + (p->unk14 - (p->unk14 - p->unk18) * p->unkF / p->unkE);
    p->unk12 = py + (p->unk16 - (p->unk16 - p->unk1A) * p->unkF / p->unkE);
    if (p->unkF == 0) {
        p->unkD++;
    }
    return p->unkF;
}

void func_8003DB64(Panel *p) {
    s16 x;
    s16 y;

    x = p->unk14;
    y = p->unk16;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        x += p->parent->unk10;
        y += p->parent->unk12;
    }
    p->unk10 = x;
    p->unk12 = y;
}

extern u8 *D_801D83EC;
void func_8003D9C0(Panel *, s16, s16, s32);
s32 func_8003DA64();
void func_8003DB64();

void func_8003DBBC(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x90);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x20;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * -0x12F + 0xF0;
        break;
    case 1:
        (D_801D83EC + arg0 * 0xD8)[0x55] = 1;
        (D_801D83EC + arg0 * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)p + 0xD)) += 1;
        break;
    case 2:
        func_8003D9C0(p, 0x20, -(arg0 * 0x7D) + 0x99, 0x10);
        break;
    case 3:
        func_8003DA64(p);
        break;
    case 4:
        func_8003DB64(p);
        break;
    case 7:
        (D_801D83EC + arg0 * 0xD8)[0x55] = 6;
        (D_801D83EC + arg0 * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)p + 0xD)) = 2;
        break;
    case 11:
        func_8003D9C0(p, 0xE8, -(arg0 * 0x7D) + 0x99, 0x10);
        break;
    case 12:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DD9C(s32 arg0) {
    void *p;
    s32 y;

    p = D_801D83EC + (arg0 * 0xD8 + 0xB4);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x164;
        y = 0x31 - arg0 * 0x31;
        (*(s16 *)((s8 *)p + 0x12)) = y;
        func_8003D9C0(p, 0x164, y, 0);
        func_8003DA64(p);
        (*(u8 *)((s8 *)p + 0xD)) = 0;
        break;
    case 1:
        func_8003D9C0(p, 0x100, 0x31 - arg0 * 0x31, 8);
        break;
    case 2:
        if (func_8003DA64(p) == 0) {
            func_8002B498(0xA7);
        }
        break;
    case 3:
        func_8003D9C0(p, 0xF9, 0x31 - arg0 * 0x31, 8);
        break;
    case 4:
        func_8003DA64(p);
        break;
    case 5:
        func_8003DB64(p);
        break;
    case 6:
        func_8003D9C0(p, 0x164, 0x31 - arg0 * 0x31, 8);
        break;
    case 7:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DF48(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x48);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = arg0 * 0x2A0 - 0xEC;
        (*(s16 *)((s8 *)p + 0x12)) = 0x5C;
        break;
    case 1:
        func_8003D9C0(p, arg0 * 0x7C + 0x28, 0x5C, 0x10);
        (D_801D83EC + arg0 * 0xD8)[0x79] = 4;
        break;
    case 2:
        func_8003DA64(p);
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, arg0 * 0x2A0 - 0xEC, 0x5C, 0x10);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    case 6:
        func_8003D9C0(p, arg0 * 0xCC, 0x5C, 0x10);
        break;
    case 7:
        if (func_8003DA64(p) == 0) {
            (D_801D83EC + arg0 * 0xD8)[0x79] = 1;
        }
        break;
    case 8:
        func_8003DB64(p);
        break;
    }
}

void func_8003E11C(s32 arg0) {
    void *p;
    s32 x;
    s32 y;

    p = D_801D83EC + (arg0 * 0xD8 + 0x6C);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        x = -(arg0 * 0x41) + 0x44;
        (*(s16 *)((s8 *)p + 0x10)) = x;
        y = arg0 * 0x1E + 0xA;
        (*(s16 *)((s8 *)p + 0x12)) = y;
        func_8003D9C0(p, x, y, 0);
        func_8003DA64(p);
        (*(u8 *)((s8 *)p + 0xD)) = 0;
        break;
    case 1:
        func_8003D9C0(p, -(arg0 * 0xA1) + 0x74, arg0 * 0x1E + 0xA, 8);
        break;
    case 2:
        if (func_8003DA64(p) == 0) {
            func_8002B498(0xA7);
        }
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, -(arg0 * 0x41) + 0x44, arg0 * 0x1E + 0xA, 8);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E298(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x24);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x38;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * -0x12F + 0xF0;
        break;
    case 1:
        func_8003D9C0(p, 0x38, arg0 * -0x7F + 0x99, 0x10);
        break;
    case 2:
        func_8003DA64(p);
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, 0x38, arg0 * -0x12F + 0xF0, 0x10);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E3C8(s32 arg0) {
    void *p;

    p = D_801D83EC + arg0 * 0xD8;
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = -0xFF;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * 0x7E + 0x16;
        break;
    case 1:
        func_8003D9C0(p, 0x22, 0x16, 0xC);
        (*(u8 *)((s8 *)p + 0xD)) = 3;
        break;
    case 2:
        func_8003D9C0(p, 0x22, 0x94, 0xA);
        (*(u8 *)((s8 *)p + 0xD)) = 3;
        break;
    case 3:
        func_8003DA64(p);
        break;
    case 4:
        func_8003DB64(p);
        break;
    case 5:
        func_8003D9C0(p, -0xFF, (*(s16 *)((s8 *)p + 0x12)), 0xC);
        break;
    case 6:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

#define PANEL(i) (((InfoPanel *)D_801D83EC)[i])

void func_8003E4F0(void) {
    s32 p;
    s32 i;
    s32 d;
    s32 step;
    s32 count;

    for (i = 0; i < 2; i++) {
        func_8003DBBC(i);
        func_8003DD9C(i);
        func_8003E298(i);
        func_8003DF48(i);
        func_8003E11C(i);
        func_8003E3C8(i);
    }
    for (p = 0; p < 2; p++) {
        PLAYER(p)->unk11C[4] = func_8004110C(p);
        if (func_80040764(p) == -1) {
            for (i = 0; i < 4; i++) {
                PLAYER(p)->unk126[i] = 0;
                PLAYER(p)->unk11C[i] = 0;
            }
        }
        for (i = 0; i < 5; i++) {
            d = PLAYER(p)->unk126[i] - PLAYER(p)->unk11C[i];
            step = (d < 0 ? -d : d) / 16 + 1;
            if (PLAYER(p)->unk126[i] < PLAYER(p)->unk11C[i]) {
                PLAYER(p)->unk126[i] += step;
                if (PLAYER(p)->unk126[i] > PLAYER(p)->unk11C[i]) {
                    PLAYER(p)->unk126[i] = PLAYER(p)->unk11C[i];
                }
            } else if (PLAYER(p)->unk126[i] > PLAYER(p)->unk11C[i]) {
                PLAYER(p)->unk126[i] -= step;
                if (PLAYER(p)->unk126[i] < PLAYER(p)->unk11C[i]) {
                    PLAYER(p)->unk126[i] = PLAYER(p)->unk11C[i];
                }
            }
        }
    }
    count = 0;
    for (p = 0; p < 2; p++) {
        for (i = 0; i < 5; i++) {
            if (PLAYER(p)->unk126[i] != PLAYER(p)->unk11C[i]) {
                count++;
            }
        }
    }
    if (count != 0 && !(((Unk8006E050 *)D_8006E050)->unk24 & 3)) {
        func_8002B498(0xAA);
    }
    func_800395A0();
    for (i = 0; i < 12; i++) {
        if (PANEL(i).flags & 0x80) {
            func_8004269C((SprtInfo *)&PANEL(i), i, i * 2 + PANEL(i).z + 1);
            func_80039730(i, i * 2 + PANEL(i).z);
        }
    }
}


s32 func_801F8200();
s32 func_801F8854();
extern u8 *D_801D833C;
extern u8 *D_801D83EC;

void func_8003FB3C(s32);
void func_8003E844(s32 arg0) {
    void *p;

    D_801D833C = p = func_8001AD0C(0x870);
    D_801D8340 = p = func_8001AD0C(0x86C);
    (*(s32 *)((s8 *)D_801D8340 + 0x7F8)) = func_801F8854();
    (*(s8 *)((s8 *)D_801D8340 + 0x817)) = (s8) (rand() % 2);
    (*(s8 *)((s8 *)D_801D8340 + 0x818)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81B)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81C)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x810)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x81F)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x825)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x823)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x822)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x824)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x820)) = 0;
    func_801F8200();
    func_8003FB3C(arg0);
    (*(s8 *)((s8 *)D_801D8340 + 0x81D)) = -1;
}

void func_80024460(s32);
extern s32 D_800794F8;
extern s32 func_800250F4;

void func_8003E94C(void) {
    Unk800794F8 *p;

    func_80024460(0);
    func_800149B8(0x19, -1, 0, 0x800, &func_800250F4, 0);
    func_80014C08(2);
    p = (Unk800794F8 *)&D_800794F8;
    p->unk54 = 0;
    p->unk56 = 0;
    p->unk58 = 0;
    p->unk7C = 0;
    p->unk80 = 0;
    p->unk84 = 0;
    p->unk8E = 0;
    p->unk90 = 0x1C0;
    p->unk92 = 0;
    p->unk94 = 0;
    p->unk8C = -1;
    p->unk74 = 1;
    (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
    func_80014C08(2);
}

s32 func_801F8998(s32, s32, s32, s32, s32);
void func_80038F68();
extern void *D_8006E054;
void func_80034260(void);
void func_80041E00(void);

void func_8003E9F4(s32 arg0) {
    s32 var_a0;
    s32 var_v1;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &func_80034260, 0, 0, 0, 0);
    if ((arg0 != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, func_80038F68, 0, 0, 0, 0);
    }
    func_800149B8(0, -1, 0, 0x800, &func_80041E00, 0, 0, 0, 0);
    if (arg0 != 0) {
        var_a0 = (*(u8 *)((s8 *)D_8006E054 + 0x72));
        var_v1 = (*(u8 *)((s8 *)D_8006E054 + 0x71));
    } else {
        var_a0 = -1;
        var_v1 = -1;
    }
    func_800149B8(0, -1, 0, 0x1000, &func_8002E034, var_a0, var_v1, 0, 0);
}

s32 func_80014A00(s32);
s32 func_8001AFF0(s32);
s32 func_801F848C();
s32 func_801F88E8();

void func_8003EB50(void) {
    func_80014A00(0x19);
    func_801F848C();
    func_801F88E8();
    func_8001AFF0(0x7F);
}

void func_8003D4C4(void);
void func_8003E4F0(void);
void func_80042824(s32);
void func_80042E78(void);
void func_80043D00(s32);
void func_80044074(s32);
s32 func_801EB53C(u8);
s32 func_801F97F4();
extern void *D_801D8340;
extern u8 D_801D83D1;

void func_8003EB88(void) {
    s16 temp_a0;

    temp_a0 = (*(s16 *)((s8 *)D_801D8340 + 0x808));
    if (temp_a0 != 0) {
        func_80042824(temp_a0);
        func_80043D00((*(s16 *)((s8 *)D_801D8340 + 0x808)));
        func_80044074((*(s16 *)((s8 *)D_801D8340 + 0x808)));
    }
    func_80042E78();
    func_8003E4F0();
    func_8003D4C4();
    if ((*(s32 *)((s8 *)D_801D8340 + 0x83C)) == 0) {
        if ((*(s32 *)((s8 *)D_801D8340 + 0x828)) != -1) {
            func_801F97F4();
        }
        func_801EB53C(D_801D83D1);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80011350);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003EC4C);

void func_80047620(s32, s32, s32);
void func_80047C38(s32, s32, s32);
void func_80047248(s32);

void func_8003F9EC(s32 player) {
    SavedDeck *decks;
    SavedDeck *d;
    s32 i;
    s32 k;
    u16 id;

    decks = (SavedDeck *)(((Unk8006E054 *)D_8006E054)->unk0 + 8);
    if (((Unk8006E054 *)D_8006E054)->unk1008[player] != -1) {
        d = &decks[((Unk8006E054 *)D_8006E054)->unk1008[player]];
        func_80047248(player);
        strcpy(D_801D8348[player] + 1, d->name);
        for (i = 0; i < 30; i++) {
            id = d->cards[i];
            func_80046BAC(D_801D8348[player] + 0x14 + i * 8, id);
            k = func_80047A58(id);
            if (k >= 0) {
                func_80047620(player, k, 0);
                if (d->unk6D != 0) {
                    func_80047C38(player, k, d->unk6D - 1);
                }
            }
        }
        func_80046A38(player, (Unk110 *)D_801D8348[player]);
    }
}

char *strcpy(char *, const char *);

void func_8003FB3C(s32 arg) {
    s32 i;
    s32 j;
    s32 k;
    u16 id;

    for (i = 0; i < 2; i++) {
        D_801D8348[i] = func_8001AD0C(0x1E4);
        PLAYER(i)->unk178_17 = (1 - arg) * 2 + i;
        PLAYER(i)->unk0[0] = 1;
        for (j = 0; j < 30; j++) {
            PLAYER(i)->cards[j].id = 0;
            PLAYER(i)->cards[j].state = 0;
            PLAYER(i)->cards[j].unk1 = 0;
            PLAYER(i)->unk17D[j] = i * 30 + j;
            PLAYER(i)->unk19B[j] = -1;
        }
        for (j = 0; j < 4; j++) {
            PLAYER(i)->unk1B9[j] = -1;
        }
        for (j = 0; j < 8; j++) {
            PLAYER(i)->unk1C2[j] = -1;
        }
        for (j = 0; j < 3; j++) {
            PLAYER(i)->unk1CA[j] = -1;
        }
        PLAYER(i)->unk1CD = -1;
        PLAYER(i)->unk17C = 0;
        *(s32 *)(D_801D8348[i] + 0x114) = 0;
        for (j = 0; j < 5; j++) {
            PLAYER(i)->unk11C[j] = 0;
            PLAYER(i)->unk126[j] = 0;
            PLAYER(i)->unk130[j].value = 0;
            PLAYER(i)->unk130[j].type = 0;
            PLAYER(i)->unk130[j].timer = 0;
            PLAYER(i)->unk130[j].x = 0;
            PLAYER(i)->unk130[j].y = 0;
        }
    }
    if (arg != 0) {
        strcpy((char *)D_801D8348[0] + 0x1CE, (char *)D_8006E050);
        strcpy((char *)D_801D8348[1] + 0x1CE, (char *)D_8006E054 + 0x57);
        if (((Unk8006E054 *)D_8006E054)->unk4 == 0) {
            func_801EA708();
            for (i = 0; i < 2; i++) {
                func_80046A38(i, D_801D8348[i]);
            }
        } else {
            func_8003F9EC(0);
            for (i = 0; i < 3; i++) {
                PLAYER_DATA(1).unk80[i].unk288 = 0;
            }
            PLAYER(1)->unk178_22 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[0];
            PLAYER(1)->unk178_24 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[1];
            PLAYER(1)->unk178_26 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[2];
            PLAYER(1)->unk178_28 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[3];
            strcpy((char *)D_801D8348[1] + 1, ((Unk8006E054 *)D_8006E054)->unk8.name);
            for (i = 0; i < 30; i++) {
                id = ((Unk8006E054 *)D_8006E054)->unk8.cards[i];
                func_80046BAC(D_801D8348[1] + 0x14 + i * 8, id);
                k = func_80047A58(id);
                if (k >= 0) {
                    func_80047620(1, k, 0);
                    if (((Unk8006E054 *)D_8006E054)->unk8.unk6D != 0) {
                        func_80047C38(1, k, ((Unk8006E054 *)D_8006E054)->unk8.unk6D - 1);
                    }
                }
            }
            func_80046A38(1, D_801D8348[1]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            strcpy((char *)D_801D8348[i] + 0x1CE, PLAYER_DATA(i).name);
        }
    }
}

extern u8 *D_801D8348[];

s32 func_80040064(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x19B;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    v = -1;
end:
    return v;
}

s32 func_800400B4(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 29; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x19B] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x19B;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040124(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x19B;
    do {
        if (p[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 func_8004017C(s32 arg0) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 30; i++) {
        base = (s8 *)D_801D8348[arg0] + 0x19B;
        p = base + i;
        if (*p != -1) {
            s32 v = *p;
            *p = -1;
            return v;
        }
    }
    return -1;
}

s32 func_800401D0(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x17D;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    v = -1;
end:
    return v;
}

s32 func_80040220(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x17D;
    do {
        if (p[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 func_80040278(s32 arg0) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 30; i++) {
        base = (s8 *)D_801D8348[arg0] + 0x17D;
        p = base + i;
        if (*p != -1) {
            s32 v = *p;
            *p = -1;
            return v;
        }
    }
    return -1;
}

s32 func_80047B84(s32 a, s32 id);

s32 func_800402CC(s32 p) {
    s32 i;
    s32 j;
    s32 c;

    for (i = 0; i < 30; i++) {
        if (PLAYER(p)->unk17D[i] != -1) {
            c = PLAYER(p)->unk17D[i];
            if (func_80047B84(p, PLAYER(p)->cards[c % 30].id) >= 0) {
                for (j = i; j > 0; j--) {
                    PLAYER(p)->unk17D[j] = PLAYER(p)->unk17D[j - 1];
                }
                PLAYER(p)->unk17D[0] = -1;
                return c;
            }
        }
    }
    return -1;
}


s32 func_800403F8(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 29; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x17D] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x17D;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040468(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1B9;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 4);
    return count;
}

s32 func_800404C0(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 4; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1B9;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return i;
        }
    }
    return -1;
}

s32 func_80040518(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 4; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1B9;
        p = base + i;
        if (*p == arg0) {
            *p = -1;
            return i;
        }
    }
    return -1;
}

s32 func_80040570(s32 player) {
    Player *p;
    s32 i;
    s32 c;

    i = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 4; i++) {
        c = p->unk1B9[i];
        if (c != -1 && p->cards[c % 30].state == 0) {
            return 0;
        }
    }
    return -1;
}

s32 func_80040614(s32 player) {
    Player *p;
    s32 i;
    s32 c;

    i = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 4; i++) {
        c = p->unk1B9[i];
        if (c != -1 && p->cards[c % 30].state == 1) {
            return 0;
        }
    }
    return -1;
}

s32 func_800406BC(s32 player) {
    Player *p;
    s32 i;
    s32 c;

    i = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 4; i++) {
        c = p->unk1B9[i];
        if (c != -1 && p->cards[c % 30].state == 2) {
            return 0;
        }
    }
    return -1;
}

s32 func_80040764(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1CA;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 3);
    v = -1;
end:
    return v;
}

s32 func_800407B4(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1CA;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 3);
    return count;
}

s32 func_8004080C(s32 idx, s32 p) {
    s8 *card;
    s32 shift;
    s32 k;

    if (idx == -1) {
        return -1;
    }
    card = PLAYER(p)->cards[idx % 30].card;
    shift = PLAYER(p)->unk178_15 - 1;
    if (shift < 0) {
        shift = 0;
    }
    for (k = 2; k >= 0; k--) {
        if (PLAYER(p)->unk1CA[k] == -1 || PLAYER(p)->unk1CA[k] == idx) {
            PLAYER(p)->unk1CA[k] = idx;
            PLAYER(p)->unk178_19 = ((u8)card[0x1A] >> 4);
            PLAYER(p)->unk11C[0] = (*(s16 *)(card + 0x1E) >> shift) / 10 * 10;
            PLAYER(p)->unk15C[0] = (*(s16 *)(card + 0x20) >> shift) / 10 * 10;
            PLAYER(p)->unk15C[1] = (*(s16 *)(card + 0x3C) >> shift) / 10 * 10;
            PLAYER(p)->unk15C[2] = (*(s16 *)(card + 0x58) >> shift) / 10 * 10;
            PLAYER(p)->unk11C[1] = PLAYER(p)->unk15C[0];
            PLAYER(p)->unk11C[2] = PLAYER(p)->unk15C[1];
            PLAYER(p)->unk11C[3] = PLAYER(p)->unk15C[2];
            PLAYER(p)->unk178_30 = 0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040A48(s32 p, s32 deck) {
    Rect16 r;
    Rect16 unused;
    s8 *card;
    s32 c;
    s32 k;

    if (deck == -1) {
        return -1;
    }
    c = func_80040764(p);
    func_80046BAC(&PLAYER(p)->cards[c % 30], PLAYER_DATA(p).unk80[deck].unk292[0]);
    card = (s8 *)&PLAYER_DATA(p).unk80[deck] + 0x13C;
    PLAYER(p)->cards[c % 30].card = card;
    r.x = ((p << 8) + (deck + 3) * 40 >> 1) + 0x2C0;
    r.y = 0xC8;
    r.w = 0x14;
    r.h = 0x28;
    MoveImage2(&r, ((p << 8) + c % 30 % 6 * 40 >> 1) + 0x2C0, c % 30 / 6 * 40);
    for (k = 0; k < 3; k++) {
        if (PLAYER(p)->unk1CA[k] == c) {
            PLAYER(p)->unk178_19 = (u8)card[0x1A] >> 4;
            PLAYER(p)->unk11C[0] = *(s16 *)(card + 0x1E);
            PLAYER(p)->unk15C[0] = *(s16 *)(card + 0x20);
            PLAYER(p)->unk15C[1] = *(s16 *)(card + 0x3C);
            PLAYER(p)->unk15C[2] = *(s16 *)(card + 0x58);
            PLAYER(p)->unk11C[1] = PLAYER(p)->unk15C[0];
            PLAYER(p)->unk11C[2] = PLAYER(p)->unk15C[1];
            PLAYER(p)->unk11C[3] = PLAYER(p)->unk15C[2];
            PLAYER(p)->unk178_30 = 0;
            PLAYER(p)->unk170[0] = *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + c * 60 + 0x10);
            *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + c * 60 + 0x10) = PLAYER(p)->unk170[deck + 1];
            return 0;
        }
    }
    return -1;
}

s32 func_80040D88(s32 p, s32 deck) {
    Rect16 r;
    Rect16 unused;
    s8 *card;
    s32 c;
    s32 k;

    if (deck == -1) {
        return -1;
    }
    c = func_80040764(p);
    func_80046BAC(&PLAYER(p)->cards[c % 30], PLAYER_DATA(p).unk80[deck].unk288);
    card = (s8 *)&PLAYER_DATA(p).unk80[deck];
    PLAYER(p)->cards[c % 30].card = card;
    r.x = ((p << 8) + deck * 40 >> 1) + 0x2C0;
    r.y = 0xC8;
    r.w = 0x14;
    r.h = 0x28;
    MoveImage2(&r, ((p << 8) + c % 30 % 6 * 40 >> 1) + 0x2C0, c % 30 / 6 * 40);
    for (k = 0; k < 3; k++) {
        if (PLAYER(p)->unk1CA[k] == c) {
            PLAYER(p)->unk178_19 = (u8)card[0x1A] >> 4;
            PLAYER(p)->unk11C[0] = *(s16 *)(card + 0x1E);
            PLAYER(p)->unk15C[0] = *(s16 *)(card + 0x20);
            PLAYER(p)->unk15C[1] = *(s16 *)(card + 0x3C);
            PLAYER(p)->unk15C[2] = *(s16 *)(card + 0x58);
            PLAYER(p)->unk11C[1] = PLAYER(p)->unk15C[0];
            PLAYER(p)->unk11C[2] = PLAYER(p)->unk15C[1];
            PLAYER(p)->unk11C[3] = PLAYER(p)->unk15C[2];
            PLAYER(p)->unk178_30 = 0;
            *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + c * 60 + 0x10) = PLAYER(p)->unk170[0];
            return 0;
        }
    }
    return -1;
}

s32 func_800410B4(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 3; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1CA;
        p = base + i;
        if (*p == arg0) {
            *p = -1;
            return 0;
        }
    }
    return -1;
}

s32 func_8004110C(s32 player) {
    Player *p;
    s32 i;
    s32 sum;
    s32 c;

    i = 0;
    sum = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 8; i++) {
        c = p->unk1C2[i];
        if (c != -1) {
            sum += p->cards[c % 30].card[0x1C];
        }
    }
    if (sum > 90) {
        sum = 90;
    }
    return sum;
}

s32 func_800411C4(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1C2;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 8);
    v = -1;
end:
    return v;
}

s32 func_80041214(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1C2;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 8);
    return count;
}

s32 func_8004126C(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 7; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x1C2] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x1C2;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_800412DC(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 8; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1C2;
        p = base + i;
        if (*p != -1 && *p == arg0) {
            *p = -1;
            return 0;
        }
    }
    return -1;
}

s32 func_80041340(s32 arg0) {
    return ((s8 *)D_801D8348[arg0])[0x1CD];
}

s32 func_80041364(s32 arg0) {
    return ((s8 *)D_801D8348[arg0])[0x1CD] == -1;
}

s32 func_80041390(s32 id, s32 player) {
    if ((s8)D_801D8348[player][0x1CD] == id) {
        return -1;
    }
    if ((s8)D_801D8348[player][0x1CD] == -1) {
        D_801D8348[player][0x1CD] = id;
        return 0;
    }
    return -1;
}

s32 func_80041408(s32 arg0) {
    s8 *p = (s8 *)D_801D8348[arg0];
    s32 v = p[0x1CD];

    p[0x1CD] = -1;
    return v;
}

s32 func_80040220(s32);

void func_80041430(s32 player) {
    s32 n;
    s32 k;
    s32 i;
    s32 j;
    s8 t;

    n = func_80040220(player);
    if (n >= 2) {
        for (k = 0; k < ((Player *)D_801D8348[player])->unk11A; k++) {
            for (i = 30 - n; i < 30; i++) {
                j = rand() % n + (30 - n);
                t = ((Player *)D_801D8348[player])->unk17D[i];
                ((Player *)D_801D8348[player])->unk17D[i] = ((Player *)D_801D8348[player])->unk17D[j];
                ((Player *)D_801D8348[player])->unk17D[j] = t;
            }
        }
        ((Player *)D_801D8348[player])->unk11A = 0;
    }
}

void func_80041584(s32 player) {
    s32 n;
    s32 k;
    s32 i;
    s32 j;
    s8 t;

    n = func_80040124(player);
    if (n >= 2) {
        for (k = 0; k < ((Player *)D_801D8348[player])->unk11A; k++) {
            for (i = 30 - n; i < 30; i++) {
                j = rand() % n + (30 - n);
                t = ((Player *)D_801D8348[player])->unk19B[i];
                ((Player *)D_801D8348[player])->unk19B[i] = ((Player *)D_801D8348[player])->unk19B[j];
                ((Player *)D_801D8348[player])->unk19B[j] = t;
            }
        }
        ((Player *)D_801D8348[player])->unk11A = 0;
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800113C0);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800113D0);


extern s32 D_800113C0;
extern s32 D_800113D0;
void func_8003EC4C();

void func_800416D8(s32 n) {
    u8 *buf;
    SavedDeck *decks;
    s32 r;

    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_800113C0, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, func_8001B144, &D_800113D0, func_800148B0());
    buf = (u8 *)func_80014C08(0x7FFFFFFF);
    ((Unk8006E054 *)D_8006E054)->unk0 = buf;
    decks = (SavedDeck *)(buf + 8);
    ((Unk8006E054 *)D_8006E054)->unk4 = n;
    ((Unk8006E054 *)D_8006E054)->unk8 = decks[n];
    func_800149B8(0, -1, 0, 0x800, func_8003EC4C, 1, func_800148B0(), 0, 0);
    r = func_80014C08(0x7FFFFFFF);
    if (*((s8 *)D_801D8340 + 0x81F) == 0) {
        if (r != 0) {
            if (++PLAYER_DATA(0).unk1A >= 1000) {
                PLAYER_DATA(0).unk1A = 999;
            }
        } else {
            if (++PLAYER_DATA(0).unk18 >= 1000) {
                PLAYER_DATA(0).unk18 = 999;
            }
        }
        func_8002CC44(0);
    }
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\saiseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    ((u8 *)((Unk8006E054 *)D_8006E054)->unk100C)[0x1A6] = r;
    func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
}

void func_80041A1C(void) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_800113C0, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, func_8001B144, &D_800113D0, func_800148B0());
    ((Unk8006E054 *)D_8006E054)->unk0 = (u8 *)func_80014C08(0x7FFFFFFF);
    ((Unk8006E054 *)D_8006E054)->unk1010[0x12] = 0;
    func_800149B8(0, -1, 0, 0x800, func_8003EC4C, 0, func_800148B0(), 0, 0);
    if (func_80014C08(0x7FFFFFFF) != 0) {
        if (++PLAYER_DATA(0).unk1E >= 1000) {
            PLAYER_DATA(0).unk1E = 999;
        }
        if (++PLAYER_DATA(1).unk1C >= 1000) {
            PLAYER_DATA(1).unk1C = 999;
        }
    } else {
        if (++PLAYER_DATA(0).unk1C >= 1000) {
            PLAYER_DATA(0).unk1C = 999;
        }
        if (++PLAYER_DATA(1).unk1E >= 1000) {
            PLAYER_DATA(1).unk1E = 999;
        }
    }
    func_8002CC44(0);
    func_8002CC44(1);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\openseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, D_801EB2E8, func_800148B0(), 0, 0, 0);
}

extern s32 D_8006E294;
extern s32 D_801D8344;
void func_8001B438(u32 *, s16, s16, s16, s16);
s32 func_8001B144();

void func_80041CA8(u8 *s, s32 row, s32 arg2) {
    char path[64]; /* unused, but it is in the original stack frame */
    u8 *arc;
    s32 i;

    D_8006E294 = 1;
    D_801D8344 = 0;
    func_800149B8(0, -1, 0, 0x800, &func_8001B144, "B:\\FONT.ARC", func_800148B0());
    arc = (u8 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; *s != 0;) {
        func_8001B438((u32 *)(arc + ((s32 *)arc)[*s - 0x20]), i * 4 + 0x2C0, (row << 5) + 0x1C0, 0x2F0,
                      row + 0x1D7);
        DrawSync(0);
        s++;
        func_80014C08(D_800794F0);
        if (++i >= 12) {
            break;
        }
    }
    func_8001AE90(arc);
    D_8006E294 = 0;
    func_80014A48(arg2);
}

extern s32 D_801D8350;

void func_80041E00(void) {
    char path[72];
    u32 *tim;
    s32 i;
    s16 k;
    s32 id;

    D_801D8350 = -1;
    DUEL->unk812 = 0;
    DUEL->unk826 = 0;
    for (i = 0; i < 6; i++) {
        DUEL->cache[i].id = -1;
        DUEL->cache[i].used = 0;
        DUEL->cache[i].age = 100;
    }
    for (;;) {
        s32 slot;

        func_80014C08(D_800794F0);
        slot = DUEL->unk826 % 6;
        DUEL->cache[slot].used = 0;
        if (DUEL->unk812 != 0) {
            break;
        }
        if (DUEL->unk81C == -1 || DUEL->unk81C == 4) {
            continue;
        }
        k = *(s16 *)(DUEL->unk58 + 2);
        if (k == -1) {
            continue;
        }
        if (SPRITE_KIND(k) == 0x19) {
            continue;
        }
        if (k != D_801D8350) {
            id = PLAYER(DUEL->unk81B)->cards[k % 30].id;
            if (DUEL->unk811 != 0) {
                continue;
            }
            DUEL->cache[slot].used = 0;
            D_801D8350 = *(s16 *)(DUEL->unk58 + 2);
            if (DUEL->cache[slot].id != id) {
                DUEL->unk811 = 1;
                DUEL->cache[slot].id = id;
                sprintf(path, "B:\\CARD\\LC%3.3d.TIM", id);
                func_800149B8(0, -1, 0, 0x800, func_8001B144, path, func_800148B0());
                tim = (u32 *)func_80014C08(0x7FFFFFFF);
                func_8001B438(tim, slot % 2 * 32 + 0x280, slot / 2 * 64 + 0x140, 0, 0x1FF - slot);
                DrawSync(0);
                func_8001AE90(tim);
                DUEL->unk811 = 0;
            }
            DUEL->cache[slot].used = 1;
            for (i = 0; i < 6; i++) {
                if (DUEL->cache[i].age != 0) {
                    DUEL->cache[i].age--;
                }
            }
            DUEL->cache[slot].age = 100;
        } else {
            DUEL->cache[slot].used = 1;
        }
    }
    DUEL->unk812 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042174);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80011440);

void func_8004269C(SprtInfo *info, s32 arg1, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = info->x;
        CUR_SPRT->sp.y0 = info->y;
        CUR_SPRT->sp.u0 = info->u;
        CUR_SPRT->sp.v0 = info->v;
        CUR_SPRT->sp.clut = info->clut;
        CUR_SPRT->sp.w = info->w;
        CUR_SPRT->sp.h = info->h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = info->r;
        CUR_SPRT->sp.g0 = info->g;
        CUR_SPRT->sp.b0 = info->b;
        setDrawMode(&CUR_SPRT->dm, 0, 0, info->tpage);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_80042824(s32 c) {
    POLY_FT4 *p;
    u8 *buf;

    buf = (u8 *)D_800793A0->unk4078[11];
    p = (POLY_FT4 *)(buf + 0x1E0);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0;
    p->y0 = 0xB;
    p->x1 = 0xA0;
    p->y1 = 0xB;
    p->x2 = 0;
    p->y2 = 0x7A;
    p->x3 = 0xA0;
    p->y3 = 0x7A;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
    p = (POLY_FT4 *)(buf + 0x208);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0x13F;
    p->y0 = 0xB;
    p->x1 = 0x9F;
    p->y1 = 0xB;
    p->x2 = 0x13F;
    p->y2 = 0x7A;
    p->x3 = 0x9F;
    p->y3 = 0x7A;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
    p = (POLY_FT4 *)(buf + 0x230);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0;
    p->y0 = 0xE8;
    p->x1 = 0xA0;
    p->y1 = 0xE8;
    p->x2 = 0;
    p->y2 = 0x79;
    p->x3 = 0xA0;
    p->y3 = 0x79;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
    p = (POLY_FT4 *)(buf + 0x258);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0x13F;
    p->y0 = 0xE8;
    p->x1 = 0x9F;
    p->y1 = 0xE8;
    p->x2 = 0x13F;
    p->y2 = 0x79;
    p->x3 = 0x9F;
    p->y3 = 0x79;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
}

void func_80042BBC(s32 x, s32 y, s32 z, s32 n, u8 *tex) {
    POLY_FT4 *p;
    s32 u;

    p = (POLY_FT4 *)((u8 *)D_800793A0->unk4078[11] + (n * 80 + 0x280));
    u = ((((Unk8006E050 *)D_8006E050)->unk24 / 4) % 4) * 32;
    func_8001E6EC(0xC, p, 1, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->u0 = u;
    p->v0 = 0x40;
    p->u1 = u + 0x20;
    p->v1 = 0x40;
    p->u2 = u;
    p->v2 = 0x80;
    p->u3 = u + 0x20;
    p->v3 = 0x80;
    p->x0 = x;
    p->y0 = y;
    p->x1 = x + 0x40;
    p->y1 = y;
    p->x2 = x;
    p->y2 = y + 0x40;
    p->x3 = x + 0x40;
    p->y3 = y + 0x40;
    p->tpage = 0x1E;
    p->clut = 0x7FB0;
    addPrim(&D_800793A0->ot[z], p);
    if (tex != 0) {
        p++;
        func_8001E6EC(0xC, p, 1, 0);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->u0 = tex[0x16];
        p->v0 = tex[0x17];
        p->u1 = tex[0x16] + 0x28;
        p->v1 = tex[0x17];
        p->u2 = tex[0x16];
        p->v2 = tex[0x17] + 0x27;
        p->u3 = tex[0x16] + 0x28;
        p->v3 = tex[0x17] + 0x27;
        p->x0 = x;
        p->y0 = y;
        p->x1 = x + 0x40;
        p->y1 = y;
        p->x2 = x;
        p->y2 = y + 0x40;
        p->x3 = x + 0x40;
        p->y3 = y + 0x40;
        p->tpage = *(u16 *)(tex + 0x12);
        p->clut = *(u16 *)(tex + 0x10);
        addPrim(&D_800793A0->ot[z], p);
    }
}

extern s8 D_8006E2E4[];

void func_80042E78(void) {
    POLY_FT4 *p;
    s32 i;
    s32 d;
    s32 c;
    s8 k;
    s8 m;
    u8 n;

    m = D_801D83D0.unk3;
    if (m == -1) {
        return;
    }
    n = D_801D83D0.unk1;
    k = D_8006E2E4[D_801D83D0.next];
    if (D_801D83D0.unkC != n || D_801D83D0.unkB != k || D_801D83D0.unkD != m) {
        D_801D83D0.unk0 = 0;
        D_801D83D0.unkE = 0;
        D_801D83D0.unk10 = 0;
        D_801D83D0.px = 0x154;
        D_801D83D0.py = 0x66;
        D_801D83D0.unkC = n;
        D_801D83D0.unkB = k;
        D_801D83D0.unkD = m;
    }
    p = (POLY_FT4 *)(D_800793A0->unk4078[11] + 0x320);
    switch ((u8)D_801D83D0.unk0) {
    case 0:
        D_801D83D0.px -= 14;
        if (D_801D83D0.px < 0x5B) {
            D_801D83D0.px = 0x5A;
            D_801D83D0.unk0++;
        }
        break;
    case 1:
        D_801D83D0.unkE += 2;
        for (i = 0; i < 6; i++) {
            d = D_801D83D0.unkE - i * 3;
            c = 0x100 - d * 20;
            if (c >= 0) {
                func_8001E6EC(0xC, p, 1, 0);
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = 0xD0;
                p->v0 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100;
                p->u1 = 0xFF;
                p->v1 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100;
                p->u2 = 0xD0;
                p->v2 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100 + 12;
                p->u3 = 0xFF;
                p->v3 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100 + 12;
                p->x0 = D_801D83D0.px - d * 2;
                p->y0 = D_801D83D0.py - d * 2;
                p->x1 = D_801D83D0.px + 0x30;
                p->y1 = D_801D83D0.py - d * 2;
                p->x2 = D_801D83D0.px - d * 2;
                p->y2 = D_801D83D0.py + 12;
                p->x3 = D_801D83D0.px + 0x30;
                p->y3 = D_801D83D0.py + 12;
                p->tpage = 0x3E;
                p->clut = 0x7CB3;
                addPrim(&D_800793A0->ot[0x1E], p);
                p++;
                func_8001E6EC(0xC, p, 1, 0);
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = 0xD0;
                p->v0 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100;
                p->u1 = 0xFC;
                p->v1 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100;
                p->u2 = 0xD0;
                p->v2 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100 + 0x18;
                p->u3 = 0xFC;
                p->v3 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100 + 0x18;
                p->x0 = D_801D83D0.px - (s16)(d * 2 - 10);
                p->y0 = D_801D83D0.py - (s16)(d - 8);
                p->x1 = (s16)(D_801D83D0.px - (s16)(d * 2 - 10) + 0x2C) + d * 2;
                p->y1 = D_801D83D0.py - (s16)(d - 8);
                p->x2 = D_801D83D0.px - (s16)(d * 2 - 10);
                p->y2 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->x3 = (s16)(D_801D83D0.px - (s16)(d * 2 - 10) + 0x2C) + d * 2;
                p->y3 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->tpage = 0x3E;
                p->clut = 0x7CB3;
                addPrim(&D_800793A0->ot[0x1E], p);
                p++;
                func_8001E6EC(0xC, p, 1, 0);
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = 0xA0;
                p->v0 = 0xA0;
                p->u1 = 0xF0;
                p->v1 = 0xA0;
                p->u2 = 0xA0;
                p->v2 = 0xB8;
                p->u3 = 0xF0;
                p->v3 = 0xB8;
                p->x0 = D_801D83D0.px + 0x30;
                p->y0 = D_801D83D0.py - (s16)(d - 8);
                p->x1 = D_801D83D0.px + 0x80 + d * 2;
                p->y1 = D_801D83D0.py - (s16)(d - 8);
                p->x2 = D_801D83D0.px + 0x30;
                p->y2 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->x3 = D_801D83D0.px + 0x80 + d * 2;
                p->y3 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->tpage = 0x3C;
                p->clut = 0x7CB3;
                addPrim(&D_800793A0->ot[0x1E], p);
                p++;
            }
        }
        if (++D_801D83D0.unk10 > 0x10) {
            D_801D83D0.unk0++;
        }
        break;
    case 2:
        D_801D83D0.unkE = 0;
        D_801D83D0.unk10 = 0;
        D_801D83D0.tx = (D_801D83D0.unk1 % 2) * -170 + 0xB8;
        D_801D83D0.ty = (D_801D83D0.unk1 % 2) * -136 + 0xA8;
        D_801D83D0.unk0++;
        break;
    case 3:
        D_801D83D0.unk10++;
        D_801D83D0.px = (D_801D83D0.tx - 0x5A) * D_801D83D0.unk10 / 8 + 0x5A;
        D_801D83D0.py = (D_801D83D0.ty - 0x66) * D_801D83D0.unk10 / 8 + 0x66;
        if (D_801D83D0.unk10 >= 8) {
            D_801D83D0.unk0++;
        }
        break;
    case 4:
        D_801D83D0.px = D_801D83D0.tx;
        D_801D83D0.py = D_801D83D0.ty;
        break;
    }
    if (D_8006E2E4[D_801D83D0.next] != -1) {
        if (func_80029990() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = D_801D83D0.px + 0xE;
        CUR_SPRT->sp.y0 = D_801D83D0.py + 0x18;
        CUR_SPRT->sp.u0 = D_8006E2E4[D_801D83D0.next] / 4 * 100;
        CUR_SPRT->sp.v0 = ((s8)(D_8006E2E4[D_801D83D0.next] % 4) * 14 + 0x1B8) % 0x100;
        CUR_SPRT->sp.clut = 0x7DF3;
        CUR_SPRT->sp.w = 100;
        CUR_SPRT->sp.h = 14;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
    if (func_80029990() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = D_801D83D0.px;
    CUR_SPRT->sp.y0 = D_801D83D0.py;
    CUR_SPRT->sp.u0 = 0xD0;
    CUR_SPRT->sp.v0 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x2F;
    CUR_SPRT->sp.h = 12;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
    if (func_80029990() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = D_801D83D0.px + 10;
    CUR_SPRT->sp.y0 = D_801D83D0.py + 8;
    CUR_SPRT->sp.u0 = 0xD0;
    CUR_SPRT->sp.v0 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x2C;
    CUR_SPRT->sp.h = 0x18;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
    if (func_80029990() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = D_801D83D0.px + 0x30;
    CUR_SPRT->sp.y0 = D_801D83D0.py + 8;
    CUR_SPRT->sp.u0 = 0xA0;
    CUR_SPRT->sp.v0 = 0xA0;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x50;
    CUR_SPRT->sp.h = 0x18;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
}

extern DR_AREA D_801D8358[2];
extern DR_AREA D_801D8378[2];
extern u8 *D_8006E29C[];
void GetDispEnv(DISPENV *);
void SetDrawArea(DR_AREA *, Rect16 *);

void func_80043D00(s32 c) {
    DISPENV env;
    Rect16 r;
    u8 rgb[4];
    u8 buf[0x48];
    u8 *s;
    u8 *d;
    s32 i;

    if (D_801D83D0.next == -1) {
        return;
    }
    rgb[0] = c;
    rgb[1] = c;
    rgb[2] = c;
    GetDispEnv(&env);
    SetDrawArea(&D_801D8358[D_800794F4], (Rect16 *)env.disp);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D8358[D_800794F4]);
    if (D_801D83D0.cur != D_801D83D0.next) {
        if (++D_801D83D0.y > 0x10) {
            D_801D83D0.cur = D_801D83D0.next;
            D_801D83D0.player = ((u8 *)D_801D8340)[0x817];
        }
    } else if (D_801D83D0.y != 0) {
        D_801D83D0.y--;
    }
    if (D_801D83D0.cur != -1) {
        s = D_8006E29C[D_801D83D0.cur];
        d = buf;
        do {
            if (*s < 0x81 || *s >= 0x99) {
                if (*s == '*' && s[1] == 'P') {
                    s += 2;
                    i = *s++ - '0';
                    i ^= D_801D83D0.player;
                    *d = 0;
                    strcpy((char *)d, (char *)D_801D8348[i] + 0x1CE);
                    d += strlen(D_801D8348[i] + 0x1CE);
                    continue;
                }
            } else {
                *d++ = *s++;
            }
            *d++ = *s++;
        } while (s[-1] != 0);
        func_80028D48(0x10, D_801D83D0.y + 0xE, (s32)buf, (s32 *)rgb, 7, 0xFFE);
    }
    r.x = env.disp[0] + 0x10;
    r.y = env.disp[1] + 0xE;
    r.w = 0x120;
    r.h = 0xC;
    SetDrawArea(&D_801D8378[D_800794F4], &r);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D8378[D_800794F4]);
}

extern DR_AREA D_801D8398[2];
extern DR_AREA D_801D83B8[2];
extern u8 *D_8006E2F8[];
extern char D_8001174C[];

void func_80044074(s32 c) {
    DISPENV env;
    Rect16 r;
    u8 rgb[4];

    if (D_801D83D0.next2 == -1) {
        return;
    }
    rgb[0] = c;
    rgb[1] = c;
    rgb[2] = c;
    GetDispEnv(&env);
    SetDrawArea(&D_801D8398[D_800794F4], (Rect16 *)env.disp);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D8398[D_800794F4]);
    if (D_801D83D0.cur2 != D_801D83D0.next2 || D_801D83D0.unkA != D_801D83D0.unk1) {
        if (++D_801D83D0.y2 > 0x10) {
            D_801D83D0.cur2 = D_801D83D0.next2;
            D_801D83D0.unkA = D_801D83D0.unk1;
        }
    } else if (D_801D83D0.y2 != 0) {
        D_801D83D0.y2--;
    }
    if (D_801D83D0.cur2 != -1) {
        if (func_80029990() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = 0x10;
        CUR_SPRT->sp.y0 = 0xDB - D_801D83D0.y2;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = (D_801D83D0.unkA * 12 + 0x100) % 256;
        CUR_SPRT->sp.clut = 0x7C73;
        CUR_SPRT->sp.w = 0x2F;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = c;
        CUR_SPRT->sp.g0 = c;
        CUR_SPRT->sp.b0 = c;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&D_800793A0->ot[0xFFE], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[0xFFE], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
        if (D_801D83D0.unkA == 1 && D_801D83D0.cur2 != 2 && D_801D83D0.cur2 != 0) {
            func_80028D48(0x50, 0xDB - D_801D83D0.y2, (s32)D_8001174C, (s32 *)rgb, 7, 0xFFE);
        } else {
            func_80028D48(0x40, 0xDB - D_801D83D0.y2, (s32)D_8006E2F8[D_801D83D0.cur2], (s32 *)rgb, 7, 0xFFE);
        }
    }
    r.x = env.disp[0] + 0x10;
    r.y = env.disp[1] + 0xDB;
    r.w = 0x120;
    r.h = 0xC;
    SetDrawArea(&D_801D83B8[D_800794F4], &r);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D83B8[D_800794F4]);
}

void func_80044504(s32 x, s32 y, s32 n, s32 c, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = (n * 12 + 0x153) % 256;
        CUR_SPRT->sp.clut = getClut(0x300, n + 0x1FC);
        CUR_SPRT->sp.w = 0x18;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = c;
        CUR_SPRT->sp.g0 = c;
        CUR_SPRT->sp.b0 = c;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_800446A4(s32 x, s32 y, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = 0x47;
        CUR_SPRT->sp.clut = 0x7EF0;
        CUR_SPRT->sp.w = 0x20;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

extern s32 D_801D83F0;

void func_80044800(void) {
    D_801D83F0 = 0;
}

MATRIX *func_80045700(VECTOR *pos, SVECTOR *rot, MATRIX *m);
MATRIX *CompMatrix(MATRIX *, MATRIX *, MATRIX *);

void func_8004480C(void *arg0, s32 k) {
    u8 *o;
    MATRIX m;
    SVECTOR v[4];
    s32 sxy[4];
    s32 p;
    s32 otz;
    s32 flag;

    o = arg0;
    if (!(o[0x15] & 0x80)) {
        return;
    }
    PushMatrix();
    func_80045700((VECTOR *)(o + 0x18), (SVECTOR *)(o + 0x28), &m);
    CompMatrix((MATRIX *)((u8 *)D_801D6A4C + 0x78), &m, &m);
    SetRotMatrix((s32)&m);
    func_8005C444(&m);
    v[0].vx = -(*(s32 *)(o + 0x30) * 40) / 8192;
    v[0].vy = -(*(s32 *)(o + 0x30) * 48) / 8192;
    v[0].vz = 0;
    v[1].vx = (*(s32 *)(o + 0x30) * 40) / 8192;
    v[1].vy = -(*(s32 *)(o + 0x30) * 48) / 8192;
    v[1].vz = 0;
    v[2].vx = -(*(s32 *)(o + 0x30) * 40) / 8192;
    v[2].vy = (*(s32 *)(o + 0x30) * 48) / 8192;
    v[2].vz = 0;
    v[3].vx = (*(s32 *)(o + 0x30) * 40) / 8192;
    v[3].vy = (*(s32 *)(o + 0x30) * 48) / 8192;
    v[3].vz = 0;
    RotAverageNclip4((s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], (s32)&sxy[0], (s32)&sxy[1], (s32)&sxy[2],
                     (s32)&sxy[3], &p, &otz, &flag);
    *(s32 *)(o + 0x38) = 0x57 - *(s16 *)(D_801D833C + k * 36 + 0x20);
    if (*((s8 *)D_801D8340 + 0x81C) >= 0 && k == *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x58) + 2)) {
        *(s32 *)(o + 0x38) = 0x33;
    }
    *(s16 *)(o + 0x34) = sxy[0];
    *(s16 *)(o + 0x36) = sxy[0] >> 16;
    PopMatrix();
}

/* a POLY_FT4 filled in whole words */
typedef struct {
    /* 0x00 */ u32 tag;
    /* 0x04 */ u32 rgbc;
    /* 0x08 */ u32 xy0;
    /* 0x0C */ u32 uv0;
    /* 0x10 */ u32 xy1;
    /* 0x14 */ u32 uv1;
    /* 0x18 */ u32 xy2;
    /* 0x1C */ u32 uv2;
    /* 0x20 */ u32 xy3;
    /* 0x24 */ u32 uv3;
} RawPolyFT4;

s32 RotAverage4(SVECTOR *, SVECTOR *, SVECTOR *, SVECTOR *, s32 *, s32 *, s32 *, s32 *, s32 *, s32 *);
void func_801F8E34(void *, s32);

void func_80044AB0(CardSprite *o, s32 k) {
    MATRIX m;
    SVECTOR v[4];
    SVECTOR w[4];
    s32 sxy[4];
    s32 p;
    s32 otz;
    s32 flag;
    s32 nclip;
    u32 *col;
    u32 *fade;
    RawPolyFT4 *buf;
    RawPolyFT4 *pk;
    u8 *duel;
    u8 *t;
    u16 clut;
    u16 tpage;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;

    if (!(o->flags & 0x80)) {
        return;
    }
    PushMatrix();
    func_80045700(&o->pos, &o->rot, &m);
    CompMatrix((MATRIX *)((u8 *)D_801D6A4C + 0x78), &m, &m);
    SetRotMatrix((s32)&m);
    func_8005C444(&m);
    v[0].vx = -(o->scale * 40) / 8192;
    v[0].vy = -(o->scale * 48) / 8192;
    v[0].vz = 0;
    v[1].vx = (o->scale * 40) / 8192;
    v[1].vy = -(o->scale * 48) / 8192;
    v[1].vz = 0;
    v[2].vx = -(o->scale * 40) / 8192;
    v[2].vy = (o->scale * 48) / 8192;
    v[2].vz = 0;
    v[3].vx = (o->scale * 40) / 8192;
    v[3].vy = (o->scale * 48) / 8192;
    v[3].vz = 0;
    col = (u32 *)o->rgbc;
    fade = (u32 *)o->fade;
    buf = (RawPolyFT4 *)D_800793A0->unk4078[10];
    nclip = RotAverageNclip4((s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], (s32)&sxy[0], (s32)&sxy[1],
                             (s32)&sxy[2], (s32)&sxy[3], &p, &otz, &flag);
    if (nclip <= 0) {
        otz = RotAverage4(&v[1], &v[0], &v[3], &v[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
    }
    if ((o->flags & 0x20) && func_80029990() == 0) {
        CUR_SPRT->sp.x0 = sxy[0] - 10;
        CUR_SPRT->sp.y0 = (sxy[0] >> 16) + 6;
        CUR_SPRT->sp.u0 = (u8)(o->num / 5) * 60;
        CUR_SPRT->sp.v0 = (u8)(o->num % 5) * 21 - 0x80;
        CUR_SPRT->sp.clut = 0x7DF2;
        CUR_SPRT->sp.w = 60;
        CUR_SPRT->sp.h = 21;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
    o->z = 0x57 - *(s16 *)(D_801D833C + k * 36 + 0x20);
    duel = D_801D8340;
    if (*(s8 *)(duel + 0x81C) >= 0) {
        t = *(u8 **)(duel + 0x58);
        if (k == *(s16 *)(t + 2)) {
            *(CardSprite **)(t + 4) = o;
            o->z = 0x33;
            func_801F8E34(*(u8 **)(duel + 0x58), 0x33);
        }
    }
    if (o->flags & 0x40) {
        if (o->t < 16) {
            o->t++;
        } else if ((o->to[0] | o->to[1] | o->to[2]) == 0) {
            o->flags &= ~0x40;
        }
        o->fade[0] = o->from[0] + (o->to[0] - o->from[0]) * o->t / 16;
        o->fade[1] = o->from[1] + (o->to[1] - o->from[1]) * o->t / 16;
        o->fade[2] = o->from[2] + (o->to[2] - o->from[2]) * o->t / 16;
        o->sx = sxy[0];
        o->sy = sxy[0] >> 16;
        pk = &buf[D_801D83F0++];
        pk->tag = 0x09000000;
        pk->rgbc = *fade;
        pk->xy0 = sxy[0];
        pk->uv0 = 0x7DB24080;
        pk->xy1 = sxy[1];
        pk->uv1 = 0x3E40A8;
        pk->xy2 = sxy[2];
        pk->uv2 = 0x7080;
        pk->xy3 = sxy[3];
        pk->uv3 = 0x70A8;
        addPrim(&D_800793A0->ot[o->z], pk);
    }
    if (nclip <= 0) {
        otz = RotAverage4(&v[1], &v[0], &v[3], &v[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
        tpage = 0x1C;
        clut = 0x7C32;
        u0 = 0xA0;
        v0 = 0x6F;
        u1 = 0xC8;
        v1 = 0x6F;
        u2 = 0xA0;
        v2 = 0x9F;
        u3 = 0xC8;
        v3 = 0x9F;
    } else {
        w[0].vx = -(o->scale * 18) / 4096;
        w[0].vy = (o->scale * -19) / 4096;
        w[0].vz = 0;
        w[1].vx = (o->scale * 18) / 4096;
        w[1].vy = (o->scale * -19) / 4096;
        w[1].vz = 0;
        w[2].vx = -(o->scale * 18) / 4096;
        w[2].vy = (o->scale * 17) / 4096;
        w[2].vz = 0;
        w[3].vx = (o->scale * 18) / 4096;
        w[3].vy = (o->scale * 17) / 4096;
        w[3].vz = 0;
        u0 = o->u + 2;
        v0 = o->v + 2;
        u1 = o->u + 38;
        v1 = v0;
        u2 = u0;
        v2 = o->v + 38;
        u3 = u1;
        v3 = v2;
        otz = RotAverage4(&w[0], &w[1], &w[2], &w[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
        clut = o->clut;
        tpage = o->tpage;
        pk = &buf[D_801D83F0++];
        pk->tag = 0x09000000;
        pk->rgbc = *col;
        pk->xy0 = sxy[0];
        pk->uv0 = (clut << 16) | (v0 << 8) | u0;
        pk->xy1 = sxy[1];
        pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
        pk->xy2 = sxy[2];
        pk->uv2 = (v2 << 8) | u2;
        pk->xy3 = sxy[3];
        pk->uv3 = (v3 << 8) | u3;
        addPrim(&D_800793A0->ot[o->z], pk);
        otz = RotAverage4(&v[0], &v[1], &v[2], &v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
        tpage = 0x1C;
        clut = getClut(800, 497 + o->pal);
        u0 = 0xCC;
        v0 = 0x6F;
        u1 = 0xF4;
        v1 = 0x6F;
        u2 = 0xCC;
        v2 = 0x9F;
        u3 = 0xF4;
        v3 = 0x9F;
    }
    o->sx = sxy[0];
    o->sy = sxy[0] >> 16;
    pk = &buf[D_801D83F0++];
    pk->tag = 0x09000000;
    pk->rgbc = *col;
    pk->xy0 = sxy[0];
    pk->uv0 = (clut << 16) | (v0 << 8) | u0;
    pk->xy1 = sxy[1];
    pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
    pk->xy2 = sxy[2];
    pk->uv2 = (v2 << 8) | u2;
    pk->xy3 = sxy[3];
    pk->uv3 = (v3 << 8) | u3;
    addPrim(&D_800793A0->ot[o->z], pk);
    PopMatrix();
}

MATRIX *MulMatrix(MATRIX *, MATRIX *);
MATRIX *MatrixNormal(MATRIX *, MATRIX *);
MATRIX *TransposeMatrix(MATRIX *, MATRIX *);

MATRIX *func_80045700(VECTOR *pos, SVECTOR *rot, MATRIX *m) {
    MATRIX tmp;
    SVECTOR r;

    r.vx = 0;
    r.vy = rot->vy;
    r.vz = 0;
    RotMatrix(&r, m);
    r.vx = rot->vx;
    r.vy = 0;
    r.vz = 0;
    RotMatrix(&r, &tmp);
    MulMatrix(m, &tmp);
    r.vx = 0;
    r.vy = 0;
    r.vz = rot->vz;
    RotMatrix(&r, &tmp);
    MulMatrix2(&tmp, m);
    MatrixNormal(m, &tmp);
    TransposeMatrix(&tmp, m);
    m->t[0] = pos->vx;
    m->t[1] = pos->vy;
    m->t[2] = pos->vz;
    return m;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_8001174C);

extern u8 *D_801D840C;
extern u8 *D_801D8400;
extern u8 *D_801D8404;

void func_800457FC(void) {
    u8 *hdr;
    s32 i;
    s32 n;

    func_800149B8(0, -1, 0, 0x800, func_8001B248, "B:\\CARD2.CDD", func_800148B0(), -2);
    D_801D840C = hdr = (u8 *)func_80014C08(0x7FFFFFFF);
    D_801D8408 = hdr + 8;
    D_801D8400 = D_801D8408 + *(u16 *)(hdr + 4) * 0x13C;
    D_801D8404 = D_801D8400 + hdr[6] * 0xE2;
    n = 0;
    for (i = 0; i < 0xBF; i++) {
        ((CardInfo *)D_801D8408)[i].id = n++;
    }
    for (i = 0; i < 0x66; i++) {
        ((Unk801D8400 *)D_801D8400)[i].id = n++;
    }
    for (i = 0; i < 8; i++) {
        ((Unk801D8404 *)D_801D8404)[i].id = n++;
    }
}


void func_80045968(s32 a, s32 row, s32 n) {
    s32 r;
    s32 i;

retry:
    r = rand();
    for (i = 0; i < n; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk15E0[row][i] == r) {
            goto retry;
        }
    }
    ((Unk8006E050 *)D_8006E050)[a].unk15E0[row][n] = r;
}

void func_80045A58(s32 arg0) {
    s32 row;
    s32 i;
    s32 *base;
    u8 *p;

    i = 0;
    do {
        base = &D_8006E050;
        row = arg0 * 0x2774 + *base + 0x14B2;
        p = (u8 *)(row + i);
        *p &= 0x7F;
        i++;
    } while (i < 0x12D);
}

void func_80045AB8(s32 arg0) {
    s32 row;
    s32 i;
    s32 *base;
    u8 *p;

    i = 0;
    do {
        base = &D_8006E050;
        row = arg0 * 0x2774 + *base + 0x14B2;
        p = (u8 *)(row + i);
        *p &= 0xDF;
        i++;
    } while (i < 0x12D);
}

void *func_80046088(s32);

s8 func_80045B18(s32 p, s32 id, s32 n) {
    s32 k;

    if (id >= 0xAC && id <= 0xBE) {
        return -3;
    }
    for (k = PLAYER_DATA(p).unk14B2[id] & 7; k < 6; k++) {
        func_80045968(p, id, k);
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 6) {
        PLAYER_DATA(p).unk14B2[id] |= 0x50;
        return -2;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 0 && !(PLAYER_DATA(p).unk14B2[id] & 0x40)) {
        PLAYER_DATA(p).unk14B2[id] |= 0x20;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) + n >= 7) {
        PLAYER_DATA(p).unk14B2[id] &= 0xF8;
        PLAYER_DATA(p).unk14B2[id] |= 0x56;
        return -1;
    }
    PLAYER_DATA(p).unk14B2[id] += n;
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 6) {
        PLAYER_DATA(p).unk14B2[id] |= 0x10;
    }
    if (((u8 *)func_80046088(id))[0x19] == 0) {
        PLAYER_DATA(p).unk14B2[id] |= 0x10;
    }
    PLAYER_DATA(p).unk14B2[id] |= 0xC0;
    func_8002CC44(p);
    return PLAYER_DATA(p).unk14B2[id] & 7;
}

s8 func_80045E1C(s32 p, s32 id, s32 n) {
    if (id >= 0xAC && id <= 0xBE) {
        return -3;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 0) {
        return -2;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) - n < 0) {
        PLAYER_DATA(p).unk14B2[id] &= 0xF8;
        return -1;
    }
    PLAYER_DATA(p).unk14B2[id] -= n;
    func_8002CC44(p);
    return PLAYER_DATA(p).unk14B2[id] & 7;
}


s32 func_80045F5C(s32 arg0, s32 arg1) {
    return (*(u8 *)((s8 *)(((arg0 * 0x2774) + D_8006E050 + arg1)) + 0x14B2)) & 7;
}

s32 func_80045F94(s32 arg0, s32 arg1) {
    switch (arg0) {
    case 0:
        return arg1;
    case 1:
        return arg1 + 0xBF;
    case 2:
        return arg1 + 0x125;
    }
    return -1;
}

s32 func_80045FE8(s32 id) {
    if (id < 0xBF) {
        return D_801D8408[id * 0x13C + 0x1A] >> 4;
    }
    if (id < 0x125) {
        return 5;
    }
    return 6;
}

s32 func_80046038(s32 id) {
    if (id < 0xBF) {
        return D_801D8408[id * 0x13C + 0x1A] & 0xF;
    }
    if (id < 0x125) {
        return 4;
    }
    return 5;
}

extern u8 *D_801D8400;
extern u8 *D_801D8404;

void *func_80046088(s32 arg0) {
    if (arg0 < 0xBF) {
        return D_801D8408 + arg0 * 0x13C;
    }
    if (arg0 < 0x125) {
        return D_801D8400 + (arg0 * 0xE2 - 0xA89E);
    }
    return D_801D8404 + (arg0 * 0x70 - 0x8030);
}

void func_80046118(s32 p) {
    s32 i;

    for (i = 0; i < 30; i++) {
        ((Unk8006E050 *)D_8006E050)[p].unk14B2[func_80045F94(((Player *)D_801D8348[p])->cards[i].state,
                                                             ((Player *)D_801D8348[p])->cards[i].unk1)] |= 0x40;
    }
}

void func_800461C0(s32 a) {
    u8 count[0x12D];
    SavedDeck *decks;
    s32 i;
    s32 j;
    s32 missing;

    decks = (SavedDeck *)(((Unk8006E054 *)D_8006E054)->unk0 + 8);
    for (i = 0; i < 0x9F; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unkAC0[i] & 0x8000) {
            for (j = 0; j < 0x12D; j++) {
                count[j] = 0;
            }
            for (j = 0; j < 30; j++) {
                count[decks[i].cards[j]]++;
            }
            missing = 0;
            for (j = 0; j < 0x12D; j++) {
                if ((((Unk8006E050 *)D_8006E050)[a].unk14B2[j] & 7) < count[j]) {
                    missing = 1;
                    break;
                }
            }
            if (!missing) {
                ((Unk8006E050 *)D_8006E050)[a].unkAC0[i] |= 0x4000;
            }
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004635C);

s8 func_80045B18(s32, s32, s32);

void func_80046864(s32 a) {
    s32 i;

    for (i = 0; i < 3; i++) {
        ((Unk8006E050 *)D_8006E050)[a].unk276E[i] =
            func_80045B18(a, ((Unk8006E050 *)D_8006E050)[a].unk2768[i], 1);
    }
}

void func_80046908(s32 i) {
    s32 j;

    ((Unk8006E050 *)D_8006E050)[i].unk12 = 0;
    for (j = 0; j < 0x12D; j++) {
        if (((Unk8006E050 *)D_8006E050)[i].unk14B2[j] & 0x40) {
            ((Unk8006E050 *)D_8006E050)[i].unk12++;
        }
    }
}


void func_80046A38(s32, Unk110 *);

void func_800469A4(s32 a) {
    s32 i;

    for (i = 0; i < 3; i++) {
        func_80046A38(a, &((Unk8006E050 *)D_8006E050)[a].unk2438[i]);
    }
}

void func_80046A38(s32 a, Unk110 *d) {
    CardSlot *c;
    s32 i;
    s32 j;

    if (d->unk0 != 0) {
        c = d->cards;
        for (i = 0; i < 30; i++) {
            switch (c->state) {
            case 0:
                c->card = (s8 *)(D_801D8408 + c->unk1 * 0x13C);
                for (j = 0; j < 3; j++) {
                    if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 != 0 &&
                        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == c->id) {
                        c->card = (s8 *)&((Unk8006E050 *)D_8006E050)[a].unk80[j];
                        break;
                    }
                }
                break;
            case 1:
                c->card = (s8 *)(D_801D8400 + c->unk1 * 0xE2);
                break;
            case 2:
                c->card = (s8 *)(D_801D8404 + c->unk1 * 0x70);
                break;
            }
            c++;
        }
    }
}

void func_80046BAC(u8 *out, s32 id) {
    s32 type;

    type = 2;
    if (id < 0xBF) {
        type = 0;
    } else {
        id -= 0xBF;
        if (id < 0x66) {
            type = 1;
        } else {
            id -= 0x66;
        }
    }
    out[0] = type;
    out[1] = id;
    *(s16 *)(out + 2) = func_80045F94(type, id);
}

s32 func_80046C0C(s32 unused, Unk110 *deck, s32 mask) {
    s32 count;
    s32 i;
    CardInfo *info;
    s32 level;
    s32 attr;

    count = 0;
    for (i = 0; i < 30; i++) {
        switch (deck->cards[i].state) {
        case 0:
            info = (CardInfo *)(D_801D8408 + deck->cards[i].unk1 * 0x13C);
            level = info->attr & 0xF;
            attr = info->attr >> 4;
            if (mask & 0x1E00) {
                if (mask & 0x1F) {
                    if ((mask >> (level + 9)) & 1) {
                        if (!(mask & 0x20) || (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                            count++;
                        }
                    }
                } else if ((mask >> (level + 9)) & 1) {
                    count++;
                }
            } else if ((mask >> attr) & 1) {
                if (!(mask & 0x20) || (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                    count++;
                }
            } else if ((mask & 0x20) && (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                count++;
            }
            break;
        case 1:
            if (mask & 0x40) {
                count++;
            }
            break;
        case 2:
            if (mask & 0x80) {
                count++;
            }
            break;
        }
    }
    return count;
}


s32 func_80046D68(s32 a, Unk110 *src, s32 slot) {
    Unk110 *d;
    s32 i;

    if (slot == -1) {
        for (slot = 0; slot < 3; slot++) {
            if (PLAYER_DATA(a).unk2438[slot].unk0 == 0) {
                break;
            }
        }
        if (slot >= 3) {
            return -1;
        }
    }
    d = &PLAYER_DATA(a).unk2438[slot];
    *d = *src;
    d->unk0 = 1;
    d->unk108[0]++;
    if (d->unk108[1] >= 10000) {
        d->unk108[1] = 9999;
    }
    if (d->unk108[2] >= 10000) {
        d->unk108[2] = 9999;
    }
    for (i = 0; i < 30; i++) {
        switch (d->cards[i].state) {
        case 0:
            d->cards[i].card = (s8 *)(D_801D8408 + d->cards[i].unk1 * 0x13C);
            d->cards[i].id = d->cards[i].unk1;
            break;
        case 1:
            d->cards[i].card = (s8 *)(D_801D8400 + d->cards[i].unk1 * 0xE2);
            d->cards[i].id = d->cards[i].unk1 + 0xBF;
            break;
        case 2:
            d->cards[i].card = (s8 *)(D_801D8404 + d->cards[i].unk1 * 0x70);
            d->cards[i].id = d->cards[i].unk1 + 0x125;
            break;
        }
    }
    return 0;
}

s32 func_80046FB8(s32 a, Unk110 *out, s32 i) {
    if (((Unk8006E050 *)D_8006E050)[a].unk2438[i].unk0 == 0) {
        return -1;
    }
    *out = ((Unk8006E050 *)D_8006E050)[a].unk2438[i];
    return 0;
}

s32 func_8004707C(s32 a, s32 b) {
    s32 i;

    if (((Unk8006E050 *)D_8006E050)[a].unk2438[b].unk0 == 0) {
        return -1;
    }
    ((Unk8006E050 *)D_8006E050)[a].unk2438[b].unk0 = 0;
    for (i = 0; i < 2; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk2438[i].unk0 == 0) {
            ((Unk8006E050 *)D_8006E050)[a].unk2438[i] = ((Unk8006E050 *)D_8006E050)[a].unk2438[i + 1];
            ((Unk8006E050 *)D_8006E050)[a].unk2438[i + 1].unk0 = 0;
        }
    }
    return 0;
}

extern u8 D_8006E50C[];

s32 func_800471F4(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    switch (var_a0) {
    case 0x75:
    case 0x79:
    case 0x7A:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
        var_a0 = 0x72;
        break;
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x83:
        var_a0 = 0x77;
        break;
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:
    case 0x8D:
        var_a0 = D_8006E50C[var_a0 - 0x84];
        break;
    }
    return var_a0;
}

void func_80047248(s32 a) {
    s32 j;

    for (j = 0; j < 3; j++) {
        ((Unk8006E054 *)D_8006E054)->unk78[a][j] = ((Unk8006E050 *)D_8006E050)[a].unk80[j];
        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 = 0;
    }
}

void func_80047364(s32 a) {
    s32 j;

    for (j = 0; j < 3; j++) {
        ((Unk8006E050 *)D_8006E050)[a].unk80[j] =
            ((Unk8006E054 *)D_8006E054)->unk78[a][j];
    }
}

void func_80047438(s32 a) {
    s32 j;
    u8 id;
    u8 alt;

    for (j = 0; j < 3; j++) {
        id = ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288;
        if (id != 0) {
            if ((s8)((Unk8006E050 *)D_8006E050)[a].unk80[j].unk289 >= 0x63) {
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk289 = 0x63;
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28A = func_80049934(0x62);
            }
            ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk278 = D_801D8408 + id * 0x13C;
            alt = ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0];
            if (alt == 0) {
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk27C = D_801D8408 + id * 0x13C;
            } else {
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk27C = D_801D8408 + alt * 0x13C;
            }
            func_80048230(a, j);
        }
    }
}

extern u8 D_8006E518[];
extern u8 D_8006EEFC[];
void func_8004950C(s32, s32);

void func_80047620(s32 p, s32 k, s32 flag) {
    s32 i;
    s32 j;
    s32 c;

    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(p).unk80[i].unk288 == D_8006E518[k]) {
            return;
        }
        if (PLAYER_DATA(p).unk80[i].unk288 == 0) {
            PLAYER_DATA(p).unk80[i].unk278 = D_801D8408 + D_8006E518[k] * 0x13C;
            PLAYER_DATA(p).unk80[i].unk27C = D_801D8408 + D_8006E518[k] * 0x13C;
            PLAYER_DATA(p).unk80[i].unk288 = D_8006E518[k];
            PLAYER_DATA(p).unk80[i].unk289 = 1;
            PLAYER_DATA(p).unk80[i].unk28A = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[i].unk28C[j] = -1;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[i].unk28F[j] = 0;
            }
            PLAYER_DATA(p).unk80[i].unk292[0] = 0;
            PLAYER_DATA(p).unk80[i].unk292[1] = 0;
            PLAYER_DATA(p).unk80[i].unk292[2] = 0;
            PLAYER_DATA(p).unk80[i].unk280 = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[i].unk282[j] = 0;
            }
            func_80048230(p, i);
            if (flag != 0) {
                c = PLAYER_DATA(p).unk80[i].unk288;
                PLAYER_DATA(p).unk14B2[c] = 1;
                func_80045968(p, c, 0);
                func_8002CC44(p);
                func_8004950C(p, D_8006EEFC[k]);
                PLAYER_DATA(p).unk14B2[D_8006E518[k]] |= 0xF0;
            } else {
                PLAYER_DATA(p).unk14B2[D_8006E518[k]] |= 0x50;
            }
            return;
        }
    }
}

void func_80047620(s32, s32, s32);

void func_80047A38(s32 arg0, s32 arg1) {
    func_80047620(arg0, arg1, 1);
}

extern u8 D_8006E518[];

s32 func_80047A58(s32 arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (arg0 == D_8006E518[i]) {
            return i;
        }
    }
    return -1;
}

s32 func_80047A98(s32 a, s32 b) {
    s32 i;

    if (((Unk8006E050 *)D_8006E050)[a].unk80[b].unk288 == 0) {
        return -1;
    }
    for (i = 0; i < 6; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[b].unk288 == D_8006E518[i]) {
            return i;
        }
    }
    return -1;
}

s32 func_80047B84(s32 a, s32 id) {
    s32 i;
    s32 j;

    for (i = 0; i < 6; i++) {
        if (id == D_8006E518[i]) {
            for (j = 0; j < 3; j++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == id) {
                    return j;
                }
            }
            return 3;
        }
    }
    return -1;
}

void func_80047E64(s32 a, s32 b, s32 c);

void func_80047C38(s32 a, s32 b, s32 c) {
    s32 j;

    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
            if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[c] != D_8006E520[b][c]) {
                ((Unk8006E050 *)D_8006E050)[a].unk14B2[D_8006E520[b][c]] |= 0x50;
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[c] = D_8006E520[b][c];
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == 0) {
                    func_80047E64(a, b, c);
                }
            }
            return;
        }
    }
}


s32 func_80047D5C(s32 a, s32 b) {
    s32 j;
    s32 k;
    s32 n;

    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
            k = 0;
            n = 0;
            for (; k < 3; k++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[k] != 0) {
                    n++;
                }
            }
            return n;
        }
    }
    return 0;
}

void func_80047E64(s32 a, s32 b, s32 c) {
    s32 j;
    s32 k;

    if (b != -1 && D_8006E520[b][c] != 0) {
        for (j = 0; j < 3; j++) {
            if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
                for (k = 0; k < 3; k++) {
                    if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[k] == D_8006E520[b][c]) {
                        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] = D_8006E520[b][c];
                        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk27C = D_801D8408 + D_8006E520[b][c] * 0x13C;
                        func_80048230(a, j);
                        return;
                    }
                }
            }
        }
    }
}

s32 func_80048014(s32 a, s32 b) {
    s32 j;
    s32 k;

    if (b == -1) {
        return -1;
    }
    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
            if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == 0) {
                return -1;
            }
            for (k = 0; k < 3; k++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == D_8006E520[b][k]) {
                    return k;
                }
            }
        }
    }
    return -1;
}

s32 func_80048150(s32 a, s32 id) {
    s32 i;
    s32 k;
    s32 j;

    for (i = 0; i < 6; i++) {
        for (k = 0; k < 3; k++) {
            if (D_8006E520[i][k] != 0 && id == D_8006E520[i][k]) {
                for (j = 0; j < 3; j++) {
                    if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == id) {
                        return j;
                    }
                }
                return 3;
            }
        }
    }
    return -1;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800119CC);

typedef struct {
    /* 0x0 */ u8 type;
    /* 0x1 */ u8 unk1;
    /* 0x2 */ u8 unk2;
    /* 0x3 */ u8 unk3;
    /* 0x4 */ s16 value;
    /* 0x6 */ s16 unk6;
} CardEffect;

extern CardRec20 D_8006E534[];
extern CardRec10 D_8006E774[];
extern CardEffect D_8006E9B4[];
extern u8 *D_8006EDB4[];

s32 func_80048230(s32 p, s32 d) {
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 line;
    s32 col;
    s32 ret;
    u8 *s;

    PLAYER_DATA(p).unk80[d].unk292[1] = 0;
    PLAYER_DATA(p).unk80[d].unk292[2] = 0;
    ret = 0;
    PLAYER_DATA(p).unk80[d].card[0] = *(CardInfo *)PLAYER_DATA(p).unk80[d].unk278;
    PLAYER_DATA(p).unk80[d].card[1] = *(CardInfo *)PLAYER_DATA(p).unk80[d].unk27C;
    if (PLAYER_DATA(p).unk80[d].card[0].attack[2].power == 0) {
        PLAYER_DATA(p).unk80[d].card[0].attack[2].power = 100;
    }
    if (PLAYER_DATA(p).unk80[d].card[1].attack[2].power == 0) {
        PLAYER_DATA(p).unk80[d].card[1].attack[2].power = 100;
    }
    PLAYER_DATA(p).unk80[d].card[0].hp += PLAYER_DATA(p).unk80[d].unk280;
    PLAYER_DATA(p).unk80[d].card[1].hp += PLAYER_DATA(p).unk80[d].unk280;
    for (i = 0; i < 3; i++) {
        PLAYER_DATA(p).unk80[d].card[0].attack[i].power += PLAYER_DATA(p).unk80[d].unk282[i];
        PLAYER_DATA(p).unk80[d].card[1].attack[i].power += PLAYER_DATA(p).unk80[d].unk282[i];
    }
    for (i = 0; i < 3; i++) {
        k = PLAYER_DATA(p).unk80[d].unk28C[i];
        if (k == -1) {
            continue;
        }
        switch (D_8006E9B4[k].type) {
        case 0:
            PLAYER_DATA(p).unk80[d].card[0].hp += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].hp += D_8006E9B4[k].value;
            break;
        case 1:
            PLAYER_DATA(p).unk80[d].card[0].attack[0].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[0].attack[1].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[0].attack[2].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[0].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[1].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[2].power += D_8006E9B4[k].value;
            break;
        case 2:
            PLAYER_DATA(p).unk80[d].card[0].attack[0].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[0].power += D_8006E9B4[k].value;
            break;
        case 3:
            PLAYER_DATA(p).unk80[d].card[0].attack[1].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[1].power += D_8006E9B4[k].value;
            break;
        case 4:
            PLAYER_DATA(p).unk80[d].card[0].attack[2].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[2].power += D_8006E9B4[k].value;
            break;
        case 5:
            PLAYER_DATA(p).unk80[d].card[0].unkE4 = D_8006E9B4[k].unk1;
            PLAYER_DATA(p).unk80[d].card[1].unkE4 = D_8006E9B4[k].unk1;
            if (D_8006E9B4[k].value != 0) {
                PLAYER_DATA(p).unk80[d].card[0].attack[2].power += D_8006E9B4[k].value;
                PLAYER_DATA(p).unk80[d].card[1].attack[2].power += D_8006E9B4[k].value;
            } else {
                PLAYER_DATA(p).unk80[d].card[0].attack[2].power = 0;
                PLAYER_DATA(p).unk80[d].card[1].attack[2].power = 0;
            }
            break;
        case 6:
            PLAYER_DATA(p).unk80[d].card[0].level += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].level += D_8006E9B4[k].value;
            break;
        case 7:
            for (j = 0; j < 2; j++) {
                PLAYER_DATA(p).unk80[d].card[0].unk74[j].unk0[0] = 0;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[d].card[0].unkB4[j].unk0[0] = 0;
            }
            for (j = 0; j < 4; j++) {
                for (n = 0; n < 0x15; n++) {
                    PLAYER_DATA(p).unk80[d].card[0].text[j][n] = 0;
                    PLAYER_DATA(p).unk80[d].card[1].text[j][n] = 0;
                }
            }
            j = D_8006E9B4[k].unk1;
            if (j != 0) {
                PLAYER_DATA(p).unk80[d].card[0].unk74[0] = D_8006E534[j - 1];
                PLAYER_DATA(p).unk80[d].card[0].unk74[0].unkE = D_8006E9B4[k].value;
            }
            if (D_8006E9B4[k].unk2 != 0) {
                for (j = 0; j < D_8006E9B4[k].unk3; j++) {
                    PLAYER_DATA(p).unk80[d].card[0].unkB4[j] = D_8006E774[D_8006E9B4[k].unk2 - 1 + j];
                    PLAYER_DATA(p).unk80[d].card[0].unkB4[j].unkC = D_8006E9B4[k].value;
                }
            }
            s = D_8006EDB4[k - 0x29];
            line = 0;
            col = 0;
            while (*s != 0) {
                if (*s == '\n') {
                    line++;
                    col = 0;
                } else {
                    PLAYER_DATA(p).unk80[d].card[0].text[line][col] = *s;
                    PLAYER_DATA(p).unk80[d].card[1].text[line][col] = *s;
                    col++;
                }
                s++;
            }
            PLAYER_DATA(p).unk80[d].card[0].unkE6 = D_8006E9B4[k].unk6;
            PLAYER_DATA(p).unk80[d].card[1].unkE6 = D_8006E9B4[k].unk6;
            ret = 1;
            break;
        case 8:
            switch (D_8006E9B4[k].unk1) {
            case 0:
                PLAYER_DATA(p).unk80[d].unk292[1] += D_8006E9B4[k].value;
                break;
            case 1:
                PLAYER_DATA(p).unk80[d].unk292[2] += D_8006E9B4[k].value;
                break;
            }
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(p).unk80[d].card[0].attack[i].power < 0) {
            PLAYER_DATA(p).unk80[d].card[0].attack[i].power = 0;
        }
        if (PLAYER_DATA(p).unk80[d].card[1].attack[i].power < 0) {
            PLAYER_DATA(p).unk80[d].card[1].attack[i].power = 0;
        }
    }
    if (((u8)(PLAYER_DATA(p).unk80[d].card[0].unkE4 - 5) < 4) | ((u8)(PLAYER_DATA(p).unk80[d].card[1].unkE4 - 5) < 4)) {
        if ((u8)(PLAYER_DATA(p).unk80[d].card[0].unkE4 - 5) < 4) {
            PLAYER_DATA(p).unk80[d].card[0].attack[2].power = 0;
        }
        if ((u8)(PLAYER_DATA(p).unk80[d].card[1].unkE4 - 5) < 4) {
            PLAYER_DATA(p).unk80[d].card[1].attack[2].power = 0;
        }
    }
    return ret;
}

s32 func_800496E4(s32, s32);
void func_800493EC(s32 a, s32 b, s32 c, s32 v) {
    if (func_800496E4(a, v) == 1) {
        ((Unk8006E050 *)D_8006E050)[a].unk80[b].unk28C[c] = v;
        func_80048230(a, b);
    }
}

void func_8004949C(s32 arg0, s32 arg1, s32 arg2) {
    ((Unk8006E050 *)D_8006E050)[arg0].unk80[arg1].unk28C[arg2] = -1;
    func_80048230(arg0, arg1);
}

void func_8004950C(s32 a, s32 b) {
    ((Unk8006E050 *)D_8006E050)[a].unk3C[b / 8] |= 1 << (b % 8);
}


s32 func_800495B4(s32 a, s32 b, s32 skip, s32 card) {
    s32 ok;
    s32 i;
    s32 c;

    ok = 1;
    for (i = 0; i < 3; i++) {
        if (skip == i) {
            continue;
        }
        c = ((Unk8006E050 *)D_8006E050)[a].unk80[b].unk28C[i];
        if (c == -1) {
            continue;
        }
        if (D_8006E9B4[card].type == 1) {
            if (D_8006E9B4[c].type >= 1 && D_8006E9B4[c].type <= 4) {
                ok = 0;
            }
        } else if (D_8006E9B4[c].type == 1) {
            if (D_8006E9B4[card].type >= 1 && D_8006E9B4[card].type <= 4) {
                ok = 0;
            }
        } else if (D_8006E9B4[card].type == D_8006E9B4[c].type) {
            ok = 0;
        }
    }
    return ok;
}

s32 func_800496E4(s32 a, s32 id) {
    s32 j;
    s32 k;

    if (id < 0) {
        return 0;
    }
    if (!((((Unk8006E050 *)D_8006E050)[a].unk3C[id / 8] >> (id % 8)) & 1)) {
        return 0;
    }
    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 != 0) {
            for (k = 0; k < 3; k++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28C[k] == id) {
                    return 2;
                }
            }
        }
    }
    return 1;
}

s32 func_800496E4(s32, s32);

s32 func_80049840(Entry12 *tbl, s32 a, s32 b) {
    s8 v;
    s32 idx;
    s32 i;

    v = ((Unk8006E050 *)D_8006E050)[a].unk80[b].unk289;
    idx = func_80047A98(a, b);
    if (idx >= 0) {
        for (i = 0; i < 0x80; i++) {
            if (tbl[i].unk4[idx] == v) {
                if (func_800496E4(a, i) == 0) {
                    return i;
                }
                return -1;
            }
        }
    }
    return -1;
}

s32 func_80049934(s32 arg0) {
    arg0++;
    return (arg0 + 2) * arg0;
}

s32 func_8004994C(s32 a, s32 b) {
    if ((s8)((s8)((Unk8006E050 *)D_8006E050)[a].unk80[b].unk289 % 5) != 0) {
        return -1;
    }
    return rand() % 4;
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80012770);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80012D68);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80012DB8);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80012DF8);

extern u8 D_80012D68[];
extern u8 D_80012DB8[];
extern u8 D_80012DF8[];
extern s32 D_801D6B18;
extern s32 D_801D6B1C;
extern s32 D_801D8538;
extern s32 D_801D853C;
extern s32 D_801D8540;
extern s32 D_801D8544;
extern s32 D_801D854C;
extern s32 D_801D8550;
extern u8 *D_801D8554;
extern u8 *D_801D8558;
s32 func_800293FC(u8 *);

extern s32 D_801D8460;
extern s32 D_801D84F4;
extern s32 D_801D84B0;
void func_80049A14(s16 *arg0) {
    s16 r[4];
    s32 x;
    s32 y;
    s16 z;
    u8 *p;

    x = arg0[0] + 1;
    y = arg0[1];
    if (D_801D8544 >= 12) {
        y -= (D_801D8544 - 11) * 7;
    }
    z = arg0[0x1D];
    if (D_801D8538 > 0 || D_801D854C != 0) {
        D_801D8538--;
    } else {
        do {
            switch (*D_801D8558) {
            case 1:
                D_801D8540 = 1;
                break;
            case 2:
                p = D_801D8558;
                D_801D8558 = p + 1;
                D_801D8538 = p[1];
                break;
            case 4:
                func_800293FC(D_80012D68);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = 0x28;
                r[1] = 0x28;
                func_80016F38((Unk80016F38 *)&D_801D84B0, (Rect16 *)r);
                func_8002BB58(3);
                break;
            case 5:
                func_800293FC(D_80012DB8);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = 0x50;
                r[1] = 0x78;
                func_80016F38((Unk80016F38 *)&D_801D84F4, (Rect16 *)r);
                func_8002BB58(3);
                break;
            case 6:
                func_800293FC(D_80012DF8);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = (0x140 - r[2]) >> 1;
                r[1] = 0xB4 - r[3] / 2;
                func_80016F38((Unk80016F38 *)&D_801D8460, (Rect16 *)r);
                func_8002BB58(3);
                break;
            case '>':
                D_801D8540 = 1;
                goto copy;
            case '\n':
                D_801D8540 = 0;
                D_801D8538 = 20;
                D_801D8544++;
            default:
            copy:
                *D_801D8554++ = *D_801D8558;
                break;
            }
            if (*++D_801D8558 == 0) {
                D_801D854C = 1;
                break;
            }
        } while (D_801D8540 == 0 && D_801D8538 == 0);
    }
    if ((D_801D853C & 0x10) || D_801D8538 == 0) {
        *D_801D8554 = '|';
    } else {
        *D_801D8554 = ' ';
    }
    D_801D853C++;
    D_801D8554[1] = 0;
    func_80028558(x, y, D_801D8550, 4, z);
}


void func_80049DC0(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012D68, 0, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}


void func_80049E00(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012DB8, 7, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}


void func_80049E40(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012DF8, 7, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}

s32 func_800170F0(Unk80016F38 *, void (*)(), s32);
extern s32 D_801D8410;
extern s32 D_801D8460;
extern s32 D_801D84B0;
extern s32 D_801D84F4;
void func_80049A14();

void func_80049E80(void) {
    func_800170F0((Unk80016F38 *)&D_801D8460, &func_80049E40, 0xA);
    func_800170F0((Unk80016F38 *)&D_801D84F4, &func_80049E00, 0xA);
    func_800170F0((Unk80016F38 *)&D_801D84B0, &func_80049DC0, 0xA);
    func_800170F0((Unk80016F38 *)&D_801D8410, &func_80049A14, 0xA);
}

extern s32 D_801D8548;
extern u8 *D_8006EF04[];

void func_80049EF8(s32 n, s32 arg1) {
    Rect16 r;
    u8 buf[0x401];
    s32 i;
    s32 done;

    done = 0;
    D_801D8548 = n;
    D_801D8538 = 20;
    D_801D853C = 0;
    D_801D8540 = 0;
    D_801D854C = 0;
    D_801D8544 = 0;
    for (i = 0; i < 0x401; i++) {
        buf[i] = 0;
    }
    D_801D8550 = (s32)buf;
    D_801D8554 = buf;
    D_801D8558 = D_8006EF04[D_801D8548];
    r.x = 0x94;
    r.y = 0x20;
    r.w = 0xA0;
    r.h = 0x54;
    func_80016C08(&D_801D8410, &r, -1, (s16 *)-1, 8, 0x58, 0x80, 0xC);
    ((Unk80016F38 *)&D_801D8410)->unk2C = (s32)"SHELL COMMAND";
    ((Unk80016F38 *)&D_801D8410)->unk38 = 2;
    ((Unk80016F38 *)&D_801D8410)->unk39 = 8;
    func_8002BB58(3);
    func_800293FC(D_80012D68);
    r.w = (D_801D6B18 + 1) / 2 * 2;
    r.h = (D_801D6B1C + 1) / 2 * 2;
    func_80016C08(&D_801D84B0, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    func_80016F38((Unk80016F38 *)&D_801D84B0, (Rect16 *)-1);
    ((Unk80016F38 *)&D_801D84B0)->unk38 = 2;
    func_800293FC(D_80012DB8);
    r.w = (D_801D6B18 + 1) / 2 * 2;
    r.h = (D_801D6B1C + 1) / 2 * 2;
    func_80016C08((Unk80016F38 *)&D_801D84B0 + 1, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    func_80016F38((Unk80016F38 *)&D_801D84B0 + 1, (Rect16 *)-1);
    ((Unk80016F38 *)&D_801D84B0)[1].unk38 = 2;
    func_800293FC(D_80012DF8);
    r.w = (D_801D6B18 + 1) / 2 * 2;
    r.h = (D_801D6B1C + 1) / 2 * 2;
    r.x = (0x140 - r.w) >> 1;
    r.y = 0xB4 - r.h / 2;
    func_80016C08(&D_801D8460, &r, -1, (s16 *)-1, 8, 0x15, 0x80, 8);
    ((Unk80016F38 *)&D_801D8460)->unk2C = (s32)"MESSAGE";
    ((Unk80016F38 *)&D_801D8460)->unk38 = 4;
    func_80016F38((Unk80016F38 *)&D_801D8460, (Rect16 *)-1);
    func_8001683C((s32)func_80049E80);
    do {
        func_80014C08(D_800794F0);
        if (D_801D854C != 0) {
            done = 1;
        }
    } while (done == 0);
    func_8002BB58(4);
    func_80016F38((Unk80016F38 *)&D_801D8410, (Rect16 *)-1);
    func_80016F38((Unk80016F38 *)&D_801D84B0, (Rect16 *)-1);
    func_80016F38((Unk80016F38 *)&D_801D84F4, (Rect16 *)-1);
    func_80016F38((Unk80016F38 *)&D_801D8460, (Rect16 *)-1);
    func_80014C08(20);
    func_80016878((s32)func_80049E80);
    func_80014A48(arg1);
}

void func_8002F8E8(void);
void func_8002B3EC(s32 arg0, s32 arg1);
void func_80015328();
s8 func_8001A100(void *arg0);
void D_801DF47C();
s32 ClearImage(Rect16 *, s32, s32, s32);
void func_8001B90C(s32, s32, s32);
/* the same text as in func_800416D8, kept as its own copy */
extern char D_80012FAC[];

void func_8004A2DC(s32 mode) {
    u8 dlg[0xB8];
    Rect16 r = { 0, 0, 480, 512 };
    s32 stack;
    s32 done;

    stack = func_800148B0();
    if (mode == 0) {
        func_8002F8E8();
        func_80014C08(10);
        ClearImage(&r, 0, 0, 0);
        DrawSync(0);
        func_80014C08(10);
        done = 0;
        func_8002B688();
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x800, func_8002B3EC, 1, stack);
        func_80014C08(0x7FFFFFFF);
        func_8001B90C(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, func_80015328, 0, 0, 0, 0);
        func_80014C08(2);
        do {
            func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 8, stack, 0, 0);
            func_80014C08(0x7FFFFFFF);
            func_8002BB58(3);
            func_80019EA4(dlg,
                          "*c6 Is it OK to return to Title Screen?\n*c3(Unless you save the game now,\nyou won't be able "
                          "to continue.)",
                          1);
            func_8001A100(dlg);
            switch ((s8)dlg[0xA5]) {
            case 1:
                done = 1;
                break;
            case 0:
            case 2:
                done = 0;
                break;
            }
        } while (!done);
        func_80014C08(20);
        func_80014A48(0);
        func_80014A90();
    } else {
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\endseg.bin", D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x800, D_801DF47C, stack, mode, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(10);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, D_80012FAC, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, stack, 0, 0);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80012FAC);
