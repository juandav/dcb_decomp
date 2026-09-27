#include "common.h"

typedef struct {
    /* 0x00 */ char unk0[0x14];
    /* 0x14 */ int unk14;
} Unk80077A0C;

typedef struct {
    /* 0x0000 */ int unk0;
    /* 0x0004 */ char unk4[0x102C];
} Unk80081710;

typedef struct {
    /* 0x0000 */ u8 unk0[0x70];
    /* 0x0070 */ u32 ot[0x1000];
    /* 0x4070 */ u8 unk4070[0x48];
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
    /* 0x000 */ u8 unk0[0x28];
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
    u8 pad0[0x54];
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
    struct {
        u8 pad0[0x19];
        u8 r0;
        u8 g0;
        u8 b0;
        u8 pad1C[0x405C];
        s32 unk4078[16];
        u8 pad40B8[0x8];
    } unk98[2];
} Unk800794F8;

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ u8 unk10;
    /* 0x11 */ u8 unk11;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 repeatDelay;
    /* 0x18 */ s16 repeatRate;
    /* 0x1A */ u8 unk1A;
    /* 0x1B */ u8 unk1B;
    /* 0x1C */ u8 unk1C[2];
} PadState;

typedef struct {
    /* 0x00 */ u8 unk0[0xE];
    /* 0x0E */ u16 clut;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ u8 tpage[0x14];
    /* 0x28 */ u8 twin[0xC];
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
    /* 0x00 */ u8 unk0[0x14];
    /* 0x14 */ Rect16 cur;
    /* 0x1C */ u8 unk1C[8];
    /* 0x24 */ Rect16 delta;
    /* 0x2C */ u8 unk2C[0x10];
    /* 0x3C */ u8 unk3C;
    /* 0x3D */ u8 unk3D;
    /* 0x3E */ u8 unk3E[3];
    /* 0x41 */ s8 unk41;
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
    DR_MODE dm;
    SPRT sp;
} SprtPacket;

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
#define getTPage(tp, abr, x, y)                                                        \
    ((((tp) & 0x3) << 7) | (((abr) & 0x3) << 5) | (((y) & 0x100) >> 4) | (((x) & 0x3ff) >> 6) | \
     (((y) & 0x200) << 2))
#define _get_mode(dfe, dtd, tpage) \
    ((0xe1000000) | ((dtd) ? 0x0200 : 0) | ((dfe) ? 0x0400 : 0) | ((tpage) & 0x9ff))
#define setDrawMode(p, dfe, dtd, tpage) \
    (setlen(p, 1), (p)->code[0] = _get_mode(dfe, dtd, tpage))

typedef struct {
    /* 0x00 */ DR_MODE dm[2];
    /* 0x10 */ u8 prim[2][0x10];
    /* 0x30 */ Rect16 unk30;
    /* 0x38 */ Rect16 unk38;
    /* 0x40 */ Rect16 unk40;
    /* 0x48 */ Bytes4 unk48;
    /* 0x4C */ u8 unk4C;
    /* 0x4D */ s8 unk4D;
} Unk800190F4;

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
    /* 0x0000 */ u8 unk0[0x2220];
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
    /* 0x000 */ u8 unk0[0x13C];
    /* 0x13C */ u8 unk13C[0x20];
    /* 0x15C */ void *unk15C[2];
    /* 0x164 */ void *unk164[2];
    /* 0x16C */ void *unk16C;
    /* 0x170 */ u8 unk170[0x3D];
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
    /* 0x3C */ char name[0x31];
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
    /* 0x104 */ s32 unk104[3];
} Unk110;

typedef struct {
    /* 0x000 */ u8 unk0[0x14];
    /* 0x014 */ CardSlot cards[30];
    /* 0x104 */ u8 unk104[0x16];
    /* 0x11A */ s16 unk11A;
    /* 0x11C */ s16 unk11C[5];
    /* 0x126 */ s16 unk126[5];
    /* 0x130 */ Popup unk130[5];
    /* 0x158 */ u8 unk158[0x25];
    /* 0x17D */ s8 unk17D[30];
    /* 0x19B */ s8 unk19B[30];
    /* 0x1B9 */ s8 unk1B9[4];
    /* 0x1BD */ u8 unk1BD[5];
    /* 0x1C2 */ s8 unk1C2[8];
    /* 0x1CA */ s8 unk1CA[3];
    /* 0x1CD */ s8 unk1CD;
} Player;



typedef struct {
    /* 0x000 */ s32 unk0[0x9E];
    /* 0x278 */ u8 *unk278;
    /* 0x27C */ u8 *unk27C;
    /* 0x280 */ s32 unk280[2];
    /* 0x288 */ u8 unk288;
    /* 0x289 */ u8 unk289;
    /* 0x28A */ s16 unk28A;
    /* 0x28C */ s8 unk28C[3];
    /* 0x28F */ u8 unk28F[3];
    /* 0x292 */ u8 unk292[6];
} Deck;

typedef struct {
    /* 0x0000 */ u8 unk0[0x12];
    /* 0x0012 */ u16 unk12;
    /* 0x0014 */ s16 unk14;
    /* 0x0016 */ u8 unk16[0xE];
    /* 0x0024 */ s32 unk24;
    /* 0x0028 */ s32 unk28;
    /* 0x002C */ s32 unk2C;
    /* 0x0030 */ u8 unk30[0xC];
    /* 0x003C */ u8 unk3C[0x44];
    /* 0x0080 */ Deck unk80[3];
    /* 0x0848 */ u8 unk848[0x278];
    /* 0x0AC0 */ u16 unkAC0[0x9F];
    /* 0x0BFE */ u8 unkBFE[0x8B4];
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
    /* 0x0004 */ u8 unk4[0x74];
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
    /* 0x0A */ u8 unkA[0xA];
    /* 0x14 */ SndSlot unk14;
    /* 0x20 */ SndSlot slot[2];
} SndState;

typedef struct {
    s32 data[0x4F];
} Unk13C;

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
void func_80048230(s32, s32);
extern u8 D_8006E520[6][3];
s32 DrawSync(s32);
s32 LoadImage(s16 *, s32);
s32 func_80014A90();

INCLUDE_ASM("asm/main/nonmatchings/game", main);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80013FA4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800141B8);

extern void *D_80077A10;
extern void *D_80077A14;
extern s16 D_80077A1A;
extern s16 D_80077A1C;
extern s32 D_80077AE0;
extern Unk80077A0C *D_80077A0C;
INCLUDE_ASM("asm/main/nonmatchings/game", func_800142D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014364);


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

void func_800142D0(void *);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014D64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014EF0);

s32 func_80014D64();

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015328);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800158B0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016948);

extern Unk800793A0 *D_800793A0;
extern s32 D_800897E8;

void func_80016BEC(void) {
    D_800897E8 = D_800793A0->unk40BC;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016C08);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800170F0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80018694);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80018B8C);

extern int D_800897E8;
extern unsigned short D_800897EC;
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
        func_8001E6EC(0x11, p->prim[i], 1, 0);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800192FC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800198A8);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010000);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80010008);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80019EA4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A1D8);

void func_80028D18(s32, s32, s32, s32, s32);
void func_8002BB58(u32);
void func_800192FC(void *, s32);
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
        func_800192FC(w + 0x44, *(s16 *)(w + 0x3A));
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A7A4);

s32 func_8001A7A4(s32, void *, void *);
extern s32 D_800897F8;
extern s32 D_8008983C;

void func_8001A9B0(void) {
    s32 var_s1;
    void *temp_s0;

    var_s1 = 0;
    do {
        temp_s0 = D_80089840[var_s1];
        func_8001A7A4(var_s1 * 0x10, temp_s0, (s8 *)&D_800897F8 + var_s1 * 0x22);
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

s32 func_8001B930();
extern s32 D_80079500;

void func_8001B90C(void) {
    D_80079500 = 0;
    func_8001B930();
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B930);

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

void func_8001BFCC(s32 arg0, s32 arg1) {
    func_8001C078(arg0 + ((s32 *)arg0)[arg1]);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C354);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E4E8);


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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F094);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F3C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F518);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F580);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F5A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F5CC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F5FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F630);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001F660);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020F54);

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

s32 GsInitCoordinate2(s32, u8 *);
s32 bzero(Unk801D6A4C *, s32);

void func_80022C4C(void) {
    Unk801D6A4C *p;

    p = D_801D6A4C = func_8001ABCC(0x29C, 0x7F);
    bzero(p, 0x29C);
    GsInitCoordinate2(0, D_801D6A4C->unk28);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022F34);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023148);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800234AC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002386C);

s32 func_8002386C(s32, s32, s32, s32, s32);

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

INCLUDE_RODATA("asm/main/nonmatchings/game", D_8001010C);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_8001014C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024460);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800246E0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024B08);

extern void *D_801D6A58[];
extern void *D_801D6A50;
extern void *D_801D6A48;

void func_80024DD4(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_8001AE90(D_801D6A58[i]);
    }
    func_8001AE90(D_801D6A50);
    func_8001AE90(D_801D6A48);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80024E44);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80026E90);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027044);

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

void func_8002793C(s32, s32, s32, s32, s32 *, s32);

void func_8002790C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8002793C(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002793C);

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

void func_80028588(s32, s32, s32, s32, s32 *, s32);

void func_80028558(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028588(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028588);

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

extern void func_80028D48(s32, s32, s32, s32 *, s32, s32);

void func_80028D18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028D48(arg0, arg1, arg2, &D_8006DF98, arg3, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028D48);

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

void func_80029EFC(s32, s32, s32, s32, s32 *, s32, s32);

void func_80029EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80029EFC(arg0, arg1, arg2, arg3, &D_8006DF98, arg4, arg5);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80029EFC);

s8 *func_8002A5B4(s8 *d, s8 *s) {
    if ((*d = *s) == 0) {
        return d;
    }
    return func_8002A5B4(d + 1, s + 1);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A5DC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A820);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ACC4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AD58);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B024);

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
extern void *D_801D8190[2];
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C1C0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C468);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C784);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002CAC8);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002CC44);

s32 func_8002D51C();
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D51C);

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

s32 func_80023148(s32, s32);

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
    ((Unk800794F8 *)&D_800794F8)->unk98[0].r0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].r0 = D_801D6A60[0];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].g0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].g0 = D_801D6A60[1];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].b0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].b0 = D_801D6A60[2];
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002FAE4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030F90);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80031970);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80031F58);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003230C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80032B44);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033258);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80033F34);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80034260);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80039220);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80039354);

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

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800110F8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80039730);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003B210);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003D4C4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003E4F0);

s32 func_801F8200();
s32 func_801F8854();
extern void *D_801D833C;

s32 func_8003FB3C();
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

s32 func_80024460(s32);
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
extern s32 D_80038F68;
extern void *D_8006E054;
extern s32 func_80034260;
extern s32 func_80041E00;

void func_8003E9F4(s32 arg0) {
    s32 var_a0;
    s32 var_v1;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &func_80034260, 0, 0, 0, 0);
    if ((arg0 != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, &D_80038F68, 0, 0, 0, 0);
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

s32 func_8003D4C4();
s32 func_8003E4F0();
s32 func_80042824(s16);
s32 func_80042E78();
s32 func_80043D00(s16);
s32 func_80044074(s16);
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

s32 func_80047620(s32, s32, s32);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003FB3C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800402CC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004080C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040A48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80040D88);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800416D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041A1C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041E00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042174);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042824);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042BBC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042E78);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80043D00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044074);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004480C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044AB0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800457FC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045B18);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045E1C);


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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046C0C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046D68);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047620);

s32 func_80047620(s32, s32, s32);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047C38);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80048230);

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

extern u8 D_8006E9B4[][8];

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
        if (D_8006E9B4[card][0] == 1) {
            if (D_8006E9B4[c][0] >= 1 && D_8006E9B4[c][0] <= 4) {
                ok = 0;
            }
        } else if (D_8006E9B4[c][0] == 1) {
            if (D_8006E9B4[card][0] >= 1 && D_8006E9B4[card][0] <= 4) {
                ok = 0;
            }
        } else if (D_8006E9B4[card][0] == D_8006E9B4[c][0]) {
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

s32 func_800170F0(s32 *, s32 *, s32);
extern s32 D_801D8410;
extern s32 D_801D8460;
extern s32 D_801D84B0;
extern s32 D_801D84F4;
void func_80049A14();

void func_80049E80(void) {
    func_800170F0(&D_801D8460, &func_80049E40, 0xA);
    func_800170F0(&D_801D84F4, &func_80049E00, 0xA);
    func_800170F0(&D_801D84B0, &func_80049DC0, 0xA);
    func_800170F0(&D_801D8410, &func_80049A14, 0xA);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049EF8);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_80012F28);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004A2DC);
