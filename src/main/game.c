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
} Unk800794F8;

typedef struct {
    /* 0x00 */ s32 key;
    /* 0x04 */ u8 unk4[0xC];
    /* 0x10 */ s32 name[4];
} FileEntry;

typedef struct {
    /* 0x000 */ u8 unk0[0x11C];
    /* 0x11C */ s16 unk11C[5];
    /* 0x126 */ s16 unk126[5];
} Player;

typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1[3];
    /* 0x004 */ s32 unk4[0x43];
} Unk110;

typedef struct {
    /* 0x000 */ u8 unk0;
    /* 0x001 */ u8 unk1[3];
    /* 0x004 */ s8 unk4[0x294];
} Deck;

typedef struct {
    /* 0x0000 */ u8 unk0[0x12];
    /* 0x0012 */ u16 unk12;
    /* 0x0014 */ u8 unk14[0x10];
    /* 0x0024 */ s32 unk24;
    /* 0x0028 */ u8 unk28[0x2E0];
    /* 0x0308 */ Deck unk308[6];
    /* 0x1298 */ u8 unk1298[0x21A];
    /* 0x14B2 */ u8 unk14B2[0x12D];
    /* 0x15DF */ u8 unk15DF[0xE59];
    /* 0x2438 */ Unk110 unk2438[3];
    /* 0x2768 */ s16 unk2768[3];
    /* 0x276E */ s8 unk276E[3];
    /* 0x2771 */ u8 unk2771[3];
} Unk8006E050;

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
    /* 0x0 */ s16 id;
    /* 0x2 */ u8 unk2[6];
    /* 0x8 */ void *buf;
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

s32 VSync(s32);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800142D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014364);

extern void *D_80077A10;
extern void *D_80077A14;
extern s16 D_80077A1A;
extern s16 D_80077A1C;
extern s32 D_80077AE0;

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80014748);

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

s32 func_80015AD8(s32, s32);
s32 func_80015EAC(s32 *arg0);
s32 func_80015F34(s32, s32, s32 *);
extern s32 D_800857E0;

s32 func_80015848(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_80015AD8(arg0, 0);
    if ((temp_v0 != 0) && (func_80015F34(temp_v0, 0x4000, &D_800857E0) != 0)) {
        func_80015EAC(temp_v0);
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

s32 func_80015EAC(s32 *arg0) {
    *arg0 = 0;
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80015F34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800161D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800162F0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016500);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016724);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800168C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016948);

extern Unk800793A0 *D_800793A0;
extern s32 D_800897E8;

void func_80016BEC(void) {
    D_800897E8 = D_800793A0->unk40BC;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016C08);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80016F38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001705C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800190F4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800191C0);

void func_80019280(u8 *p, Rect16 *r) {
    if ((s8)p[0x4D] >= 6) {
        p[0x4D] = 0;
    }
    *(Rect16 *)(p + 0x30) = *(Rect16 *)(p + 0x40);
    *(Rect16 *)(p + 0x38) = *r;
}

void func_800192E0(void *arg0, Bytes4 *arg1) {
    *(Bytes4 *)((s8 *)arg0 + 0x48) = *arg1;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800192FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001963C);

s32 func_8001705C(void *, s32, s32);

void func_800197AC(void *arg0) {
    s16 r[4];
    void *img;

    img = (*(void **)((s8 *)arg0 + 0));
    func_8001705C(img, 0, (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25)) - ((*(s16 *)((s8 *)arg0 + 0xE)) - (*(u8 *)((s8 *)arg0 + 0x25))) / 2);
    r[0] = ((*(u16 *)((s8 *)img + 0xC)) - (*(u16 *)((s8 *)img + 0x34))) + (*(u8 *)((s8 *)arg0 + 0x22)) + (*(s16 *)((s8 *)arg0 + 0x10)) * (*(u8 *)((s8 *)arg0 + 0x24));
    r[1] = ((*(u16 *)((s8 *)img + 0xE)) - (*(u16 *)((s8 *)img + 0x36))) + (*(u8 *)((s8 *)arg0 + 0x23)) + (*(s16 *)((s8 *)arg0 + 0x14)) * (*(u8 *)((s8 *)arg0 + 0x25));
    r[2] = (*(u16 *)((s8 *)arg0 + 0x1A));
    r[3] = (*(u16 *)((s8 *)arg0 + 0x1C));
    func_80019280(*(u8 **)((s8 *)arg0 + 4), (Rect16 *)r);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A40C);

s32 func_8001A6B0();

void *func_8001ACEC(s32);
s32 PadInitDirect(void *, void *);
s32 PadStartCom(void);
extern s32 D_800897F8;
extern void *D_80089840[];
extern s32 D_8008983C;
void func_8001A600(void) {
    s32 i;
    u8 *buf;

    D_8008983C = 1;
    buf = func_8001ACEC(0x3C);
    for (i = 0; i < 2; i++) {
        D_80089840[i] = buf + i * 0x1E;
    }
    func_8001A6B0();
    PadInitDirect(&D_800897F8, (s8 *)&D_800897F8 + 0x22);
    PadStartCom();
}

extern void *D_80089840[];

void func_8001A688(s32 arg0, s16 arg1, s16 arg2) {
    (*(s16 *)((s8 *)D_80089840[arg0] + 0x16)) = arg1;
    (*(s16 *)((s8 *)D_80089840[arg0] + 0x18)) = arg2;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001A6B0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001ABCC);

void *func_8001ABCC(s32, s32);
void *func_8001ACEC(s32 arg0) {
    return func_8001ABCC(arg0, -2);
}

s32 func_800148B0();
void *func_8001ABCC(s32, s32);

void *func_8001AD0C(s32 size) {
    return func_8001ABCC(size, func_800148B0());
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AD3C);

s32 func_8001AE90(void *);

void func_8001AE70(void *arg0) {
    func_8001AE90(arg0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001AE90);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B144);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B248);

extern s32 D_8006DEF0;
extern s32 D_801D4848;

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B438);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B5BC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B634);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001B7F4);

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

void func_8001AD3C(Chunk *, s32);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BC38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BCA4);

s32 func_8001BC38();
extern s32 D_801D4878;
extern s32 D_801D4888;
extern s32 D_801D5108;

s32 func_8001BCA4(s32);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001BDEC);

void func_8001C078(s32);

void func_8001BFCC(s32 arg0, s32 arg1) {
    func_8001C078(arg0 + ((s32 *)arg0)[arg1]);
}

s32 func_8001BCA4(s32);
s32 func_8001BDEC(s32);
extern s32 D_801D4868;
extern s32 D_801D486C;
extern s32 D_801D4870;
extern s32 D_801D4874;

s32 func_8001BFF8(s32 arg0, s32 arg1) {
    s32 temp_s0;
    s32 temp_s0_2;
    s32 temp_v0;

    D_801D4868 = 0;
    D_801D486C = 0;
    D_801D4870 = arg0;
    temp_s0 = func_8001BCA4(0x10);
    temp_s0_2 = (temp_s0 << 0x10) | func_8001BCA4(0x10);
    temp_v0 = func_8001ABCC(temp_s0_2, arg1);
    D_801D4874 = temp_v0;
    func_8001BDEC(temp_s0_2);
    return temp_v0;
}

s32 func_800148B0();
s32 func_8001BFF8(s32, s32);

void func_8001C078(s32 arg0) {
    func_8001BFF8(arg0, func_800148B0());
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C4DC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C6A4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001C810);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CA54);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CCB4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CE74);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001CFDC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D1AC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D33C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D464);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001D5B4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E3C8);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001E6EC);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8001EDE0);

s32 MulMatrix2(s32, void *);
s32 RotTrans(u16 *, void *, s32 *);
s32 func_8001EDE0(void *, void *, void *, void *, s32);

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
    MulMatrix2((*(s32 *)((s8 *)arg0 + 0x48)), arg0);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800202D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80020370);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021B60);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80021C18);

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

s32 func_80021C18(void *);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022D34);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022ED0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80022F34);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023094);

s32 func_80022F34(s16, s32, s32, s32);
s32 func_80023094(void *, s32, s32);

s32 func_800230B8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp_v0;
    void *temp_s0;

    temp_s0 = D_801D6A4C->unk13C[arg0];
    temp_v0 = func_80022F34((*(s16 *)((s8 *)temp_s0 + 6)), arg1, arg0, arg3);
    if (temp_v0 != 0) {
        func_80023094(temp_s0, temp_v0, arg2);
        return 1;
    }
    return 0;
}

s32 func_800230B8(s32, s32, s32, s32);

void func_80023128(s32 arg0, s32 arg1, s32 arg2) {
    func_800230B8(arg0, arg1, arg2, 0);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023148);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023408);

void func_80023454(s32 arg0, void *arg1, s32 arg2) {
    (*(s32 *)((s8 *)arg1 + 0xC)) = (s32) (arg2 + 1);
    (*(s32 *)((s8 *)arg1 + 0)) = 0;
    (*(s32 *)((s8 *)arg1 + 8)) = arg0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80023468);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800234AC);

void func_800235C8(s32 arg0) {
    D_801D6A4C->unk114[arg0] = 0;
    D_801D6A4C->unk13C[arg0] = 0;
    func_8001AFF0(arg0 + 0x40);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002360C);

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
void func_80027228(s32, s32, s32, s32 *, u16, s32, s32);

void func_800271EC(s32 arg0, s32 arg1, s32 arg2, u16 arg3, s32 arg4, s32 arg5) {
    func_80027228(arg0, arg1, arg2, &D_8006DF98, arg3, arg4, arg5);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027228);

void func_80027458(s32, s32, s32, s32, s32, s32, s32, s32 *, s32);

void func_80027410(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5,
                   s32 arg6, s32 arg7) {
    func_80027458(arg0, arg1, arg2, arg3, arg4, arg5, arg6, &D_8006DF98, arg7);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027458);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027674);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800276C8);

void func_8002793C(s32, s32, s32, s32, s32 *, s32);

void func_8002790C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_8002793C(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002793C);

void func_80027DE8(s32, s32, s32, s32, s32 *, s32);

void func_80027DB8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80027DE8(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80027DE8);

void func_80028258(s32, s32, s32, s32, s32 *, s32);

void func_80028228(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028258(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028258);

void func_80028588(s32, s32, s32, s32, s32 *, s32);

void func_80028558(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028588(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028588);

void func_800289D0(s32, s32, s32, s32, s32 *, s32);

void func_800289A0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_800289D0(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_800289D0);

extern void func_80028D48(s32, s32, s32, s32 *, s32, s32);

void func_80028D18(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80028D48(arg0, arg1, arg2, &D_8006DF98, arg3, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80028D48);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800293FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002961C);

extern u16 D_801D6B10;

s32 func_80029990(void) {
    if (D_801D6B24 == D_800793A0->unk40B8 + D_801D6B10 * 0x1C) {
        return -1;
    }
    return 0;
}

void func_80029A0C(s32, s32, s32, s32, s32 *, s32);

void func_800299DC(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    func_80029A0C(arg0, arg1, arg2, arg3, &D_8006DF98, arg4);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80029A0C);

void func_80029EFC(s32, s32, s32, s32, s32 *, s32, s32);

void func_80029EC4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_80029EFC(arg0, arg1, arg2, arg3, &D_8006DF98, arg4, arg5);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80029EFC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002A5B4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AB20);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AB5C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AB84);

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
s32 func_8002AEA4(s32);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002AEA4);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B300);

s32 SsVabTransBody(s32, s16);
s32 SsVabTransCompleted(s32);

void func_8002B38C(void *arg0, s32 arg1) {
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B6E4);

void func_8002B6E4();

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002B900);

s32 func_80014C08(s32);
extern s32 D_8006E040;
extern s32 D_800794F0;

void func_8002BA24(void) {
    do {
        func_80014C08(D_800794F0);
    } while (D_8006E040 != 0);
}

extern s16 D_801D812A;
extern s32 func_8002B900;

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
s32 func_8002BC80();

void func_8002BC58(void) {
    InitCARD(0);
    func_8002BC80();
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002BC80);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C30C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002C468);

s32 func_8006A824(char *, s32);
extern char D_800105E4;
s32 sprintf(char *, const char *, ...);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002CBA0);

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
s32 func_800457FC();

extern void *D_8006E054;
void func_8002D404(void) {
    void *p;

    func_800457FC();
    D_8006E050 = func_8001ACEC(0x4EE8);
    D_8006E054 = p = func_8001ACEC(0x102C);
    (*(void **)((s8 *)D_8006E054 + 0x100C)) = func_8001ACEC(0x1AC);
    func_8002D51C();
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D458);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D51C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002D898);

s32 func_80022D34(s32, s32, s32, s32);

void func_8002DAAC(s32 arg0, s32 arg1) {
    void *temp_s1;

    temp_s1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_s1 + 0x2200)) != arg1) {
        func_8001AFF0(arg0 + 0x84);
        func_80023094(temp_s1, func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s1 + 0x26F4), 1, arg1), arg0 + 0x84), arg1);
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
    func_80023094(temp_s3, func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s3 + 0x26F4), 1, arg1), temp_s2), arg1);
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
s32 sprintf(char *, const char *, ...);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002DCB0);

void *func_8002DBEC(s32);
s32 func_8002DCB0(s32, s32, s32, s32);
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E034);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E26C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E42C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E658);

extern s32 D_801D81A8;

void func_8002E7B8(void) {
    func_800235C8(0x17);
    func_8001AE90(D_801D81A8);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E7E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002E8EC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002EB1C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002ED9C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002EE50);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F074);

void D_801E8C04();
extern s32 D_80010864;
extern s32 D_80010894;

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

void D_801E4B34();

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F4F4);

extern s32 D_801D8260;

void func_8002F79C(void) {
    D_801D8260 = 0;
}

s32 SetTexWindow(void *, s16 *);
s32 func_8001E6EC(s32, void *, s32, s32);
extern s32 D_800108A4;
extern s32 D_801D81F8;
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F8E8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8002F920);

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

s32 rsin(s32, s32);

void func_80030264(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)), temp_a1)) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x20)));
}

void func_800302E0(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)), temp_a1)) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x24)));
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003035C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030440);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003058C);

long SquareRoot0(long);

s32 func_80030694(SVECTOR *a, SVECTOR *b) {
    VECTOR d;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    return SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030718);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030828);

s32 func_800309F0(SVECTOR *arg0, SVECTOR *arg1, s32 arg2) {
    s32 v;

    v = func_80030694(arg0, arg1);
    if (-arg2 < v && v < arg2) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030A34);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030BC4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80030CA8);

s32 func_80030BC4(void *, void *, void *);
s32 func_80030CA8(void *);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80032AA0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_800395A0);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800110F8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80039730);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003B210);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003D4C4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003D9C0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003DA64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003DB64);

extern u8 *D_801D83EC;
s32 func_8003D9C0(void *, s16, s16, s32);
s32 func_8003DA64();
s32 func_8003DB64();

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
s32 rand();
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
extern s32 func_8002E034;
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8003F9EC);

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
    u8 *p;
    s8 *slots;
    s32 i;
    s32 c;

    i = 0;
    p = D_801D8348[player];
    slots = (s8 *)p + 0x1B9;
    for (; i < 4; i++) {
        c = slots[i];
        if (c != -1 && (p + ((c % 30) << 3))[0x14] == 0) {
            return 0;
        }
    }
    return -1;
}

s32 func_80040614(s32 player) {
    u8 *p;
    s8 *slots;
    s32 i;
    s32 c;

    i = 0;
    p = D_801D8348[player];
    slots = (s8 *)p + 0x1B9;
    for (; i < 4; i++) {
        c = slots[i];
        if (c != -1 && (p + ((c % 30) << 3))[0x14] == 1) {
            return 0;
        }
    }
    return -1;
}

s32 func_800406BC(s32 player) {
    u8 *p;
    s8 *slots;
    s32 i;
    s32 c;

    i = 0;
    p = D_801D8348[player];
    slots = (s8 *)p + 0x1B9;
    for (; i < 4; i++) {
        c = slots[i];
        if (c != -1 && (p + ((c % 30) << 3))[0x14] == 2) {
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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004110C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041430);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041584);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800113C0);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800113D0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800416D8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041A1C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041CA8);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80041E00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042174);

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004269C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042824);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042BBC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80042E78);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80043D00);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044074);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044504);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800446A4);

extern s32 D_801D83F0;

void func_80044800(void) {
    D_801D83F0 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004480C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80044AB0);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045700);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_8001174C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800457FC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80045968);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046118);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800461C0);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80046A38);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004707C);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047248);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047364);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047438);

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

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047A98);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047B84);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047C38);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047D5C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80047E64);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80048014);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80048150);

INCLUDE_RODATA("asm/main/nonmatchings/game", D_800119CC);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80048230);

s32 func_800496E4(s32, s32);
void func_80048230(s32, s32);

void func_800493EC(s32 a, s32 b, s32 c, s32 v) {
    if (func_800496E4(a, v) == 1) {
        ((Unk8006E050 *)D_8006E050)[a].unk308[b].unk4[c] = v;
        func_80048230(a, b);
    }
}

void func_8004949C(s32 arg0, s32 arg1, s32 arg2) {
    ((Unk8006E050 *)D_8006E050)[arg0].unk308[arg1].unk4[arg2] = -1;
    func_80048230(arg0, arg1);
}

INCLUDE_ASM("asm/main/nonmatchings/game", func_8004950C);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800495B4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_800496E4);

INCLUDE_ASM("asm/main/nonmatchings/game", func_80049840);

s32 func_80049934(s32 arg0) {
    arg0++;
    return (arg0 + 2) * arg0;
}

s32 rand(void);

s32 func_8004994C(s32 a, s32 b) {
    if ((s8)((s8)((Unk8006E050 *)D_8006E050)[a].unk308[b].unk1[0] % 5) != 0) {
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
s32 func_80016F38(s32 *, s16 *);
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
                func_80016F38(&D_801D84B0, r);
                func_8002BB58(3);
                break;
            case 5:
                func_800293FC(D_80012DB8);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = 0x50;
                r[1] = 0x78;
                func_80016F38(&D_801D84F4, r);
                func_8002BB58(3);
                break;
            case 6:
                func_800293FC(D_80012DF8);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = (0x140 - r[2]) >> 1;
                r[1] = 0xB4 - r[3] / 2;
                func_80016F38(&D_801D8460, r);
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
