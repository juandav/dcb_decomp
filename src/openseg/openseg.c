#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/card_db.h"
#include "dcb/text.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/frame_callback.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/window.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/dialog.h"
#include "dcb/memcard.h"
#include "dcb/player_rank.h"
#include "dcb/menu.h"
#include "dcb/sort.h"
#include "dcb/scroll_bg.h"
#include "dcb/prim_util.h"
#include "dcb/sound_play.h"
#include "dcb/display.h"
#include "dcb/vblank.h"
#include "dcb/pad.h"
#include "dcb/prim.h"
#include "dcb/fade.h"
#include "dcb/sound.h"
#include "dcb/player_data.h"
#include "dcb/save_checksum.h"

extern s32 D_801F07FC[3];

extern s32 D_801F4888;
extern s32 D_801F488C;
extern s32 D_801F4890;
extern s32 D_801F4880;
extern s32 D_801F4884;
typedef struct {
    s32 unk0;
    s32 page;
    s32 shownPage;
    s32 length;
    s32 done;
    s32 waitInput;
    s32 unk18;
    s32 blink;
} TextScroll;
extern TextScroll D_801F4F28;
extern UiWindow D_801F1B10;
extern UiWindow D_801F1B60;
extern UiWindow D_801F1BB0;
void func_801E5C6C(UiWindow *window);
typedef struct {
    u16 cards[30];
    char name[0x32];
} StarterDeck;
typedef struct {
    u8 unk0[0x3D04];
    StarterDeck starters[3];
} DeckFile;
extern s16 D_801F04C4[];
extern s32 D_801F5290;
extern s32 D_801F5294;
extern s32 D_801F5298;
extern s32 D_801F529C;
extern s32 D_801F0500[3];
extern s32 D_801F51A0;
extern s32 D_801F52A0;
extern s32 D_801F52A4;
extern s32 D_801F52A8;
extern s32 D_801F52AC;
extern s32 D_801F52B0;
extern s32 D_801F52B4;
extern s32 D_801F52B8;
extern u8 D_801F52BC[3];
extern u8 D_801F52C0[3];
extern s32 D_801F52C4;
extern s32 D_801F52C8;
extern POLY_FT4 D_801F51B0[][2];

extern s8 D_801F1620[2][301];
typedef struct {
    UiWindow window;
    u8 unk44[4];
} Unk801F7C88;
extern Unk801F7C88 D_801F7C88[3];
extern UiWindow D_801F7D60;
extern UiWindow D_801F53C8;

void StUnSetRing(void);
s32 DecDCTvlc2(u32 *bs, u32 *buf, u16 *table);
s32 StFreeRing(u32 *base);
typedef struct {
    UiWindow window;
    s32 player;
} PlayerWindow;
extern Menu D_801EFF30[2];
extern Menu D_801EFFB4[2];
extern char *D_801EFF88[];
extern s32 (*D_801F000C[])(s8 *, s8 *);
void func_801E2068(s32 player);
extern s32 D_801F4F08;
extern s32 D_801F4F0C;
extern s32 D_801F4F10;
extern s32 D_801F4F14;
extern s32 D_801F4F18;
extern s32 D_801F4F1C;
extern s32 D_801F4F20;
void func_801EAA54(PlayerWindow *window);
void func_801E4C18(s32 x, s32 y, s32 part);

extern s32 D_801F1AFC;
extern s32 D_801F1B00;
u16 func_801E195C(s32 player, s32 card);
extern s8 D_801F1880[2][301];
typedef struct {
    /* 0x00 */ s16 col;
    /* 0x02 */ s16 prevCol;
    /* 0x04 */ s16 row;
    /* 0x06 */ s16 prevRow;
    /* 0x08 */ u8 unk8;
    /* 0x09 */ char name[13];
    /* 0x16 */ u8 cursor;
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s8 unk18;
    /* 0x19 */ s8 unk19;
    /* 0x1A */ s8 state;
    /* 0x1B */ u8 unk1B;
} NameEntry;
typedef struct {
    u8 deck;
    s8 chosen;
} StarterSelect;
extern StarterSelect D_801F5198;
extern CursorHighlight D_801F5148;
extern NameEntry D_801F50D8;
extern u8 D_801F0464[];
s32 func_801E6FB8();
extern CursorHighlight D_801F4F98;
extern CursorHighlight D_801F5038;
extern s32 D_801F52D0;
extern s32 D_801F4F00;
extern s32 D_801F4F04;
extern s32 D_801F5370;
void func_801E96D8(s32 x, s32 y, s32 tpage, s32 v, s32 u, s32 w, s32 clutX, s32 clutY, s32 a8, s32 a9, s32 a10, s32 brightness, s32 z);

typedef struct {
    SpuVolume volume;
    s32 reverb;
    s32 mix;
} SpuExtAttr;
typedef struct {
    u32 mask;
    SpuVolume mvol;
    SpuVolume mvolmode;
    SpuVolume mvolx;
    SpuExtAttr cd;
    SpuExtAttr ext;
} SpuCommonAttr;
void SpuSetCommonAttr(SpuCommonAttr *attr);
typedef struct {
    u8 val0;
    u8 val1;
    u8 val2;
    u8 val3;
} CdAttenuation;
s32 func_8005A784(CdAttenuation *atv);
void SsSetSerialAttr(s8 s_num, s8 attr, s8 mode);
void SsSetSerialVol(s8 s_num, s16 voll, s16 volr);
typedef struct {
    u8 pos[4];
    u32 size;
    char name[16];
} CdFileEntry;
extern s32 D_801F0854;
void func_801DFA20(void *file, char *name);
typedef struct {
     u8 *vlcbuf[2];
     s32 vlcid;
     u8 *imgbuf[2];
     s32 imgid;
     Rect16 rect[2];
     s32 rectid;
     Rect16 slice;
     s32 isdone;
} DecEnv;
/* libcd's CdlLOC */
typedef struct {
    u8 minute;
    u8 second;
    u8 sector;
    u8 track;
} CdLocation;
extern DecEnv D_801F0808;
extern CdLocation D_801F0864;
extern CdLocation D_801F086C;
extern s8 D_801F0874;
extern u8 *D_801F0840;
extern u8 *D_801F0844;
extern s32 D_801EFF2C;
extern s32 D_801F0848;
extern u16 *D_801F084C;
extern u32 *D_801F085C;
extern u32 D_801F0870;
extern s8 D_801F0868;
void func_801DFD4C();
void func_801E04CC(CdLocation *loc);
void DecDCTReset(s32 mode);
void DecDCToutCallback(void (*func)());
void StSetRing(u32 *ring_addr, u32 ring_size);
void StClearRing(void);
void StSetStream(u32 mode, u32 start_frame, u32 end_frame, void (*func1)(), void (*func2)());
typedef struct {
    u16 id;
    u16 type;
    u16 secCount;
    u16 nSectors;
    u32 frameCount;
    u32 frameSize;
    u16 width;
    u16 height;
} StHeader;
extern s32 D_801F0878;
s32 StGetNext(u32 **addr, StHeader **header);
void func_801DFAC8(void);
s32 CdRead2(s32 mode);
/* DigimonCardData with a signed type */
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ s8 type;
    /* 0x03 */ char name[0x15];
    /* 0x18 */ u8 unk18;
    /* 0x19 */ u8 unk19;
    /* 0x1A */ u8 attr;
} CardEntry;
extern CardEntry *D_801F0CA0[2][301];
extern s16 D_801F1610[2][3];
extern s32 D_801F1AE8;
extern UiWindow D_801F08D0;
extern s32 D_801F1B08;
void func_801E54EC(s32 x, s32 y, s32 z);
extern UiWindow D_801F4F48;
extern UiWindow D_801F4FE8;
extern UiWindow D_801F5088;
void func_801E7638(UiWindow *window);
void func_801E7C54(UiWindow *window);
void func_801E7DC4(UiWindow *window);
extern UiWindow D_801F50F8;
void func_801E87E0(UiWindow *window);
extern PlayerWindow D_801F52E0[2];
extern s32 D_801F52D4;
typedef struct {
    s32 x;
    s32 y;
    Rect16 uv;
} MenuSprite;
typedef struct {
    UiWindow window;
    u8 slot;
    s8 message;
    u8 unk46[2];
} SlotWindow;
typedef struct {
    /* 0x00 */ char name[0xD];
    /* 0x0D */ u8 unkD;
    /* 0x0E */ u8 location;
    /* 0x0F */ u8 arena;
    /* 0x10 */ u16 unk10;
    /* 0x12 */ u16 seenCardCount;
    /* 0x14 */ u16 progress;
    /* 0x16 */ u16 size;
    /* 0x18 */ u8 unk18[0xC];
    /* 0x24 */ s32 playTime;
    /* 0x28 */ u32 unk28_0 : 10;
    /* 0x29 */ u32 unk28_10 : 1;
    /* 0x29 */ u32 unk28_11 : 21;
    /* 0x2C */ u8 unk2C[0x2A];
    /* 0x56 */ u16 unk56;
    /* 0x58 */ u8 unk58[0x28];
} SaveSlot;
typedef struct {
    u8 *color;
    s32 z;
    s16 x;
    s16 y;
    u8 slot;
} SlotDraw;
typedef struct {
    /* 0x000 */ POLY_FT4 arrows[2];
    /* 0x050 */ POLY_FT4 arrowShadows[2];
    /* 0x0A0 */ MenuSprite sprites[6];
    /* 0x100 */ SlotWindow slotWindows[3];
    /* 0x1D8 */ SlotWindow infoWindow;
    /* 0x220 */ u8 unk220[8];
    /* 0x228 */ SaveSlot slots[2][3];
    /* 0x528 */ void *buffer;
    /* 0x52C */ s8 flags[2][3];
    /* 0x532 */ s8 unk532;
    /* 0x533 */ u8 unk533;
    /* 0x534 */ s8 unk534;
    /* 0x535 */ u8 unk535;
    /* 0x536 */ u8 unk536;
    /* 0x537 */ u8 mode;
    /* 0x538 */ u8 unk538;
    /* 0x539 */ u8 unk539;
    /* 0x53A */ s8 ready;
    /* 0x53B */ s8 slot;
    /* 0x53C */ u8 card;
    /* 0x53D */ u8 unk53D;
    /* 0x53E */ u8 unk53E;
    /* 0x53F */ u8 progress;
    /* 0x540 */ u8 unk540;
    /* 0x541 */ u8 again;
    /* 0x542 */ u8 unk542;
} Unk801F7B88;
extern Unk801F7B88 D_801F7B88;
extern s8 D_801F80B4[2][3];
extern u8 D_801F80C7;
extern u8 D_801F80CA;
extern UiWindow D_801F5380;
void func_801EC728(void);
void func_801EC650(POLY_FT4 *poly, s32 shade);
void func_801ED780(s32 x, s32 y, s32 z);
void func_801EBAFC();
extern u8 D_801F80C1;
extern u8 D_801F80BF;
extern s8 D_801F80BC;
extern u8 *D_801F5378;
extern u8 D_801F80C6;
extern u8 D_801F80C4;
extern Window D_801F80D0;
extern u8 D_801F80BD;
typedef struct {
    u8 slot;
    u8 file;
    u8 unk2[2];
    s32 playTime;
} SaveInfo;
typedef struct {
    /* 0x0000 */ u8 unk0[0x1010];
    /* 0x1010 */ SaveInfo saves[2];
    /* 0x1020 */ u8 unk1020[7];
    /* 0x1027 */ u8 unk1027;
    /* 0x1028 */ s8 menuRow;
} SessionView;
extern s8 D_801F80C2;
extern PlayerProfile *D_801F80B0;
extern u8 D_801F80C5;

void func_801DFA20(void *file, char *name) {
    s32 found;

    while (1) {
        func_8005A364(0, 0);
        found = CdSearchFile(file, name);
        if (found == 0) continue;
        if (found != -1) break;
    }
}

void func_801DFA80(s32 r, s32 g, s32 b) {
    Rect16 rect;

    rect.x = 0;
    rect.y = 0;
    rect.w = 480;
    rect.h = 480;
    ClearImage(&rect, (u8)r, (u8)g, (u8)b);
}

void func_801DFAC8(void) {
    SpuCommonAttr attr;

    attr.mask = 0x200;
    attr.cd.mix = 0;
    SpuSetCommonAttr(&attr);
}

void func_801DFAF4(s32 cdVolume, s32 masterVolume) {
    SpuCommonAttr attr;
    CdAttenuation atv;

    attr.mask = 0x2C3;
    attr.mvol.left = masterVolume;
    attr.mvol.right = masterVolume;
    attr.cd.volume.left = cdVolume;
    attr.cd.volume.right = cdVolume;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
    atv.val0 = 0xFF;
    atv.val1 = 0;
    atv.val2 = 0xFF;
    atv.val3 = 0;
    func_8005A784(&atv);
    SsSetSerialAttr(0, 0, 1);
    SsSetSerialVol(0, 0x7F, 0x7F);
}

void func_801DFB70(s32 volume) {
    SpuCommonAttr attr;

    attr.mask = 0x2C3;
    attr.mvol.left = volume;
    attr.mvol.right = volume;
    attr.cd.volume.left = volume;
    attr.cd.volume.right = volume;
    attr.cd.mix = 0;
    SpuSetCommonAttr(&attr);
}

s32 func_801DFBAC(s32 *name) {
    CdFileEntry file;

    func_801DFA20(&file, name);
    return D_801F0854 = CdPosToInt(&file);
}

void func_801DFFD0(CdLocation *loc, void (*callback)());
void func_801DFF3C(DecEnv *env, s32 x0, s32 y0, s32 x1, s32 y1);
s32 func_801E02C4(DecEnv *env);
void func_801E0164(void);
void DecDCTvlcBuild(u16 *table);

void func_801DFBE0(s32 sector, s32 endFrame, s32 volume, s32 frames, s32 height) {
    s32 i;
    s32 y;

    D_801EFF2C = height;
    D_801F0878 = endFrame;
    func_801DFAC8();
    func_801DFA80(0, 0, 0);
    func_80014C08(2);
    CdIntToPos(sector + D_801F0854, (u8 *)&D_801F0864);
    func_801DFFD0(&D_801F0864, func_801E0164);
    y = (240 - D_801EFF2C) / 2;
    func_801DFF3C(&D_801F0808, 0, y, 0, y + 240);
    DecDCTvlcBuild(D_801F084C);
    for (i = 0; i < frames; i++) {
        while (func_801E02C4(&D_801F0808) == -1) {
            D_801F086C = D_801F0864;
            func_801E04CC(&D_801F086C);
        }
    }
    func_801DFAF4(volume, volume);
    D_801F0874 = 0;
}

extern s32 D_801F0858;
extern s32 D_801F0860;
extern s8 D_801F0850;
void StRingStatus(s32 *freeSectors, s32 *overSectors);
void MoveImage(Rect16 *rect, s32 x, s32 y);
void DecDCTin(u8 *buf, s32 mode);
void DecDCTout(u8 *buf, s32 size);
s32 StGetBackloc(CdLocation *loc);
void func_801E03FC(DecEnv *env, s32 unused);

void func_801DFD4C(FrameBuffer *fb, s32 start) {
    DISPENV disp;
    Rect16 copy;
    Rect16 rect;
    s32 y;
    s32 pos;

    rect.x = 0;
    rect.y = 0;
    rect.w = 480;
    rect.h = D_801EFF2C;
    copy = rect;
    StRingStatus(&D_801F0858, &D_801F0860);
    if (D_801F0874 != 0 || start != 0) {
        D_801F0874 = 1;
        D_801F0850 = start;
        if (D_801F0868 != 0) {
            GetDispEnv(&disp);
            y = disp.disp[1];
            disp.disp[1] ^= 240;
            MoveImage(&copy, 0, y + (240 - D_801EFF2C) / 2);
            DrawSync(0);
        } else {
            DecDCTin(D_801F0808.vlcbuf[D_801F0808.vlcid], 3);
            DecDCTout(D_801F0808.imgbuf[D_801F0808.imgid], D_801F0808.slice.w * D_801F0808.slice.h / 2);
            while (func_801E02C4(&D_801F0808) == -1) {
                pos = StGetBackloc(&D_801F086C);
                if (pos > D_801F0878 || pos <= 0) {
                    D_801F086C = D_801F0864;
                }
                func_801E04CC(&D_801F086C);
            }
            func_801E03FC(&D_801F0808, 0);
        }
    }
}

void func_801DFF3C(DecEnv *env, s32 x0, s32 y0, s32 x1, s32 y1) {
    env->vlcbuf[0] = D_801F0844;
    env->vlcbuf[1] = D_801F0844 + 0x28000;
    env->vlcid = 0;
    env->imgbuf[0] = D_801F0840;
    env->imgbuf[1] = D_801F0840 + 0x2D00;
    env->imgid = 0;
    env->rect[0].x = x0;
    env->rect[0].y = y0;
    env->rect[1].x = x1;
    env->rect[1].y = y1;
    env->rectid = 0;
    env->slice.x = x0;
    env->slice.y = y0;
    env->slice.w = 24;
    env->rect[0].w = env->rect[1].w = 480;
    env->slice.h = env->rect[0].h = env->rect[1].h = D_801EFF2C;
    env->isdone = 0;
}

void func_801DFFD0(CdLocation *loc, void (*callback)()) {
    s32 *callbacks;

    callbacks = &FRAME_CALLBACKS;
    D_801F0848 = 0;
    D_801F084C = allocTaskHeapBlock(0x11000);
    D_801F085C = allocTaskHeapBlock(0x20000);
    D_801F0844 = allocTaskHeapBlock(0x50000);
    D_801F0840 = allocTaskHeapBlock(0x5A00);
    D_801F0870 = 0;
    D_801F0868 = 0;
    callbacks[0] = (s32)func_801DFD4C;
    callbacks[1] = 0;
    DecDCTReset(0);
    DecDCToutCallback(callback);
    StSetRing(D_801F085C, 0x40);
    StClearRing();
    StSetStream(1, 1, -1, 0, 0);
    func_801E04CC(loc);
}

void func_801E00D4(void) {
    VSync(0);
    D_801F0868 = 1;
    func_801DFAC8();
    func_80014C08(2);
    freeHeapBlock(D_801F0840);
    freeHeapBlock(D_801F0844);
    freeHeapBlock(D_801F085C);
    freeHeapBlock(D_801F084C);
    DecDCToutCallback(0);
    StUnSetRing();
    func_801DFB70(0x3FFF);
}

extern s32 D_801D98BC;
void StCdInterrupt(void);

void func_801E0164(void) {
    Rect16 snap;
    s32 id;

    if (D_801D98BC != 0) {
        StCdInterrupt();
        D_801D98BC = 0;
    }
    id = D_801F0808.imgid;
    snap = D_801F0808.slice;
    D_801F0808.imgid = id == 0;
    D_801F0808.slice.x += D_801F0808.slice.w;
    if (D_801F0808.slice.x < D_801F0808.rect[D_801F0808.rectid].x + D_801F0808.rect[D_801F0808.rectid].w) {
        DecDCTout(D_801F0808.imgbuf[D_801F0808.imgid], D_801F0808.slice.w * D_801F0808.slice.h / 2);
    } else {
        D_801F0808.isdone = 1;
        D_801F0808.rectid = D_801F0808.rectid == 0;
        D_801F0808.slice.x = D_801F0808.rect[D_801F0808.rectid].x;
        D_801F0808.slice.y = D_801F0808.rect[D_801F0808.rectid].y;
    }
    LoadImage((s16 *)&snap, (s32)D_801F0808.imgbuf[id]);
}

s32 func_801E02C4(DecEnv *env) {
    u32 *frame;
    s32 timeout;

    timeout = 2000;
    while ((frame = func_801E0350(env)) == NULL) {
        if (--timeout == 0) {
            return -1;
        }
    }
    env->vlcid = env->vlcid == 0;
    DecDCTvlc2(frame, env->vlcbuf[env->vlcid], D_801F084C);
    StFreeRing(frame);
    return 0;
}

u32 *func_801E0350(void) {
    u32 *addr;
    StHeader *header;
    s32 timeout;

    timeout = 2000;
    while (StGetNext(&addr, &header) != 0) {
        if (--timeout == 0) {
            return NULL;
        }
    }
    if (header->frameCount >= D_801F0878 || header->frameCount < D_801F0870) {
        D_801F0868 = 1;
        func_801DFAC8();
    }
    D_801F0870 = header->frameCount;
    return addr;
}

void func_801E03FC(DecEnv *env, s32 unused) {
    volatile s32 timeout;

    timeout = 0x800000;
    if (++D_801F0848 >= D_801F0878) {
        D_801F0868 = 1;
    }
    while (env->isdone == 0) {
        if (--timeout == 0) {
            env->isdone = 1;
            env->rectid = env->rectid == 0;
            env->slice.x = env->rect[env->rectid].x;
            env->slice.y = env->rect[env->rectid].y;
        }
    }
    env->isdone = 0;
}

void func_801E04CC(CdLocation *loc) {
    u8 param[4];

    param[0] = 0x80;
    do {
        while (CdControlB(2, (u8 *)loc, 0) == 0) {
            func_8005A364(0, 0);
        }
        while (CdControlB(0xE, param, 0) == 0) {
            func_8005A364(0, 0);
        }
        VSync(3);
    } while (CdRead2(0x1E0) == 0);
}

typedef struct {
    s32 id;
    s32 sector;
    s32 endFrame;
} Movie;

extern Movie D_801EFF08[3];
void func_801E078C();
typedef struct {
    s32 heights[5];
} MovieHeights;
/* the rodata blob that holds this table also holds the sort menu's strings, so it stays as data */
extern MovieHeights D_801DDF3C;

/* game.h: s32 func_801E055C(s32); nothing is returned */
s32 func_801E055C(s32 index) {
    MovieHeights movie = D_801DDF3C;
    s32 wait;

    func_80014A00(0x1F);
    func_80014A00(0x19);
    func_80014C08(0x10);
    func_801DFA80(0, 0, 0);
    resetDisplay(320, 240, 1);
    ((Graphics *)&GRAPHICS)->unk48 = -30;
    ((Graphics *)&GRAPHICS)->vblanksPerFrame = 2;
    func_800149B8(0x1F, 0, 0, 0x1000, func_801E078C);
    func_80014C08(0x1E);
    setTaskVsyncMode(0);
    func_801DFBE0(D_801EFF08[index].sector, D_801EFF08[index].endFrame, 0x3FFF, 1, movie.heights[index]);
    while (!(PAD_STATES[0]->repeat & 0x800) && D_801F0868 == 0) {
        func_80014AC8();
    }
    if (!(PAD_STATES[0]->repeat & 0x860)) {
        wait = 29;
        do {
            func_80014AC8();
            if (PAD_STATES[0]->repeat & 0x860) {
                break;
            }
        } while (--wait != -1);
    }
    func_80014A00(0x1F);
    func_801DFA80(0, 0, 0);
    func_801E00D4();
    resetDisplay(320, 240, 0);
    func_80014C08(0x3C);
}

void func_801E078C(void) {
    Graphics *gfx;

    gfx = (Graphics *)&GRAPHICS;
    gfx->unk8[0] = 0;
    VBLANK_COUNTER = 0;
    SetDispMask(0);
    for (; gfx->unk48 <= 0; gfx->unk48++) {
        pollPads();
        func_80014C08(1);
        gfx->vblanksPerFrame = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->vblanksPerFrame = 1;
        }
        VBLANK_COUNTER = 0;
    }
    SetDispMask(1);
    FRAME_BUFFER_INDEX = 0;
    CURRENT_FRAME_BUFFER = &gfx->buffers[0];
    for (;;) {
        pollPads();
        FRAME_BUFFER_INDEX ^= 1;
        CURRENT_FRAME_BUFFER = &gfx->buffers[FRAME_BUFFER_INDEX];
        if (gfx->unk8[0] != 0) {
            func_801DFD4C(CURRENT_FRAME_BUFFER, FRAME_BUFFER_INDEX);
        }
        VSync(0);
        PutDispEnv(&CURRENT_FRAME_BUFFER->disp);
        PutDrawEnv(&CURRENT_FRAME_BUFFER->draw);
        func_80014AC8();
        gfx->vblanksPerFrame = VBLANK_COUNTER;
        if (VBLANK_COUNTER == 0) {
            gfx->vblanksPerFrame = 1;
        }
        VBLANK_COUNTER = 0;
    }
}

s32 func_801E08D8(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 0) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 0) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E0A08(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 1) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 1) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E0B38(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 2) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 2) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E0C6C(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 3) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 3) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E0DA0(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr >> 4;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr >> 4;
    }
    if (keyA == 4) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 4) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E0ED4(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    keyA = (*a)->type == 1;
    keyB = (*b)->type == 1;
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E0FBC(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    keyA = (*a)->type == 2;
    keyB = (*b)->type == 2;
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E10A4(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 0) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 0) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E11D4(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 2) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 2) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

s32 func_801E1308(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    if ((*a)->type != 0) {
        keyA = -1;
    } else {
        keyA = (*a)->attr & 0xF;
    }
    if ((*b)->type != 0) {
        keyB = -1;
    } else {
        keyB = (*b)->attr & 0xF;
    }
    if (keyA == 3) {
        keyA = 1;
    } else {
        keyA = 0;
    }
    if (keyB == 3) {
        keyB = 1;
    } else {
        keyB = 0;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

void func_801E143C(PlayerWindow *window) {
    char text[72]; /* unused, but it sizes the frame */
    Menu *menu;
    s32 player;
    s32 i;
    s32 x;
    s32 z;

    player = window->player;
    x = window->window.originX;
    z = window->window.z;
    for (i = 0; i < D_801EFFB4[player].nrows; i++) {
        if (i < window->window.view.y / D_801EFFB4[player].rowH) {
            continue;
        }
        if ((window->window.view.y + window->window.rect.h) / D_801EFFB4[player].rowH < i) {
            break;
        }
        drawText(x, window->window.originY + i * D_801EFFB4[player].rowH + 1, (s32)D_801EFF88[i], 7, z);
    }
    menu = &D_801EFFB4[player];
    updateMenuCursor(menu);
    if (menu->active && (PAD_STATES[player]->pressed & 0x40)) {
        playMenuSound(1);
        if (D_801F000C[menu->row] != NULL) {
            D_801F1B08 = player;
            sortArray((s8 *)D_801F0CA0[player], 301, 4, D_801F000C[menu->row]);
        } else {
            func_801E2068(player);
        }
        D_801EFF30[player].row = 0;
        centerMenuOnCursor(&D_801EFF30[player]);
    }
}

void func_801E16A0(void) {
    if (isSpritePoolFull() == 0) {
        if (D_801F1AFC != 0) {
            D_801F1B00 += 4;
            if (D_801F1B00 > 8) {
                D_801F1B00 = 8;
            }
        } else {
            D_801F1B00 -= 4;
            if (D_801F1B00 < -32) {
                D_801F1B00 = -32;
            }
        }
        CUR_SPRT->sp.x0 = 6;
        CUR_SPRT->sp.y0 = D_801F1B00;
        CUR_SPRT->sp.u0 = 0x58;
        CUR_SPRT->sp.v0 = 0x4A;
        CUR_SPRT->sp.clut = 0x7FB8;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x20;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

s32 func_801E1840(s32 player, s32 card) {
    s32 other;
    s32 otherCount;
    s32 count;
    s32 i;
    s32 j;
    s32 shared;

    other = player ^ 1;
    otherCount = getOwnedCardCount(other, card);
    count = getOwnedCardCount(player, card);
    shared = 0;
    for (i = 0; i < count; i++) {
        for (j = 0; j < otherCount; j++) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i] == ((PlayerProfile *)PLAYER_PROFILES)[other].cardCopySerials[card][j]) {
                shared++;
                break;
            }
        }
    }
    return shared;
}

u16 func_801E195C(s32 player, s32 card) {
    s32 other;
    s32 otherCount;
    s32 count;
    s32 i;
    s32 j;

    other = player ^ 1;
    otherCount = getOwnedCardCount(other, card);
    count = getOwnedCardCount(player, card);
    for (i = 0; i < count; i++) {
        for (j = 0; j < otherCount; j++) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i] != ((PlayerProfile *)PLAYER_PROFILES)[other].cardCopySerials[card][j]) {
                return ((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][i];
            }
        }
    }
    return ((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[card][0];
}

void func_801E1AA4(s32 player, s32 card, s32 serial) {
    PLAYER_DATA(player).cardCopySerials[card][getOwnedCardCount(player, card)] = serial;
    if (PLAYER_DATA(player).cardCollection[card] == 0) {
        PLAYER_DATA(player).cardCollection[card] |= 0x20;
    }
    PLAYER_DATA(player).cardCollection[card]++;
    if ((PLAYER_DATA(player).cardCollection[card] & 7) == 6) {
        PLAYER_DATA(player).cardCollection[card] |= 0x10;
    }
    if (((u8 *)getCardData(card))[0x19] == 0) {
        PLAYER_DATA(player).cardCollection[card] |= 0x10;
    }
    PLAYER_DATA(player).cardCollection[card] |= 0xC8;
    updatePlayerRanks(player);
    if ((u16)++PLAYER_DATA(player).unk4C >= 10000) {
        PLAYER_DATA(player).unk4C = 9999;
    }
}

u16 func_801E1C8C(s32 player, s32 card) {
    s32 count;
    u16 serial;
    s32 i;
    s32 j;

    count = getOwnedCardCount(player, card);
    serial = func_801E195C(player, card);
    for (i = 0; i < count; i++) {
        if (PLAYER_DATA(player).cardCopySerials[card][i] == serial) {
            for (j = i; j < count - 1; j++) {
                PLAYER_DATA(player).cardCopySerials[card][j] = PLAYER_DATA(player).cardCopySerials[card][j + 1];
            }
            break;
        }
    }
    PLAYER_DATA(player).cardCollection[card]--;
    updatePlayerRanks(player);
    if ((u16)++PLAYER_DATA(player).unk4E >= 10000) {
        PLAYER_DATA(player).unk4E = 9999;
    }
    return serial;
}

void func_801E1E5C(void) {
    s8 counts[3][301];
    s32 player;
    s32 i;
    s32 j;
    s32 max;

    for (player = 0; player < 2; player++) {
        for (i = 0; i < 301; i++) {
            D_801F1880[player][i] = 0;
            for (j = 0; j < 3; j++) {
                counts[j][i] = 0;
            }
        }
        for (i = 0; i < 3; i++) {
            if (PLAYER_DATA(player).savedDecks[i].inUse != 0) {
                for (j = 0; j < 30; j++) {
                    counts[i][PLAYER_DATA(player).savedDecks[i].cards[j].id]++;
                }
            }
        }
        for (i = 0; i < 301; i++) {
            max = 0;
            for (j = 0; j < 3; j++) {
                if (max < counts[j][i]) {
                    max = counts[j][i];
                }
            }
            D_801F1880[player][i] = max;
        }
    }
}

void func_801E2068(s32 player) {
    s32 n;
    s32 i;
    s32 shared;
    s32 other;

    n = 0;
    for (i = 0; i < 0xBF; i++) {
        D_801F0CA0[player][n++] = (CardEntry *)&((DigimonCardData *)DIGIMON_CARDS)[i];
    }
    for (i = 0; i < 0x66; i++) {
        D_801F0CA0[player][n++] = (CardEntry *)&((OptionCardData *)OPTION_CARDS)[i];
    }
    for (i = 0; i < 8; i++) {
        D_801F0CA0[player][n++] = (CardEntry *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[i];
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[i].cardId != 0) {
            D_801F0CA0[player][PLAYER_DATA(player).partners[i].cardId] = (CardEntry *)&PLAYER_DATA(player).partners[i];
        }
    }
    for (i = 0; i < 301; i++) {
        shared = func_801E1840(player, i);
        D_801F1620[player][i] = getOwnedCardCount(player, i) - D_801F1880[player][i];
        if (D_801F1620[player][i] > getOwnedCardCount(player, i) - shared) {
            D_801F1620[player][i] = getOwnedCardCount(player, i) - shared;
        }
        if (D_801F1620[player][i] < 0) {
            D_801F1620[player][i] = 0;
        }
        other = player ^ 1;
        if (D_801F1620[player][i] + getOwnedCardCount(other, i) >= 7) {
            D_801F1620[player][i] = 6 - getOwnedCardCount(other, i);
        }
    }
}

extern s16 D_801F1AF0[2][3];
extern u32 *D_801F161C;

void func_801E23C4(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 done;
    s32 i;
    s32 row;
    s32 col;
    s32 card;
    s32 specialty;

    x = window->originX;
    y = window->originY;
    z = window->z;
    done = 0;
    if (D_801F1AE8 != 0) {
        for (i = 0; i < 3; i++) {
            if (D_801F1AF0[0][i] < i * 44 + 0xA6) {
                D_801F1AF0[0][i] += 8;
            } else {
                D_801F1AF0[0][i] = i * 44 + 0xA6;
                done++;
            }
        }
        for (i = 0; i < 3; i++) {
            if (D_801F1AF0[1][i] > i * 44) {
                D_801F1AF0[1][i] -= 8;
            } else {
                D_801F1AF0[1][i] = i * 44;
                done++;
            }
        }
    }
    if (done == 6) {
        CUR_SPRT->sp.x0 = x + 0x54;
        CUR_SPRT->sp.y0 = y + 6;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0;
        CUR_SPRT->sp.clut = 0x7E78;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x21;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        D_801F1AE8 = 2;
    }
    for (row = 0; row < 2; row++) {
        for (col = 0; col < 3; col++) {
            if (D_801F1610[row][col] == -1) {
                continue;
            }
            card = D_801F1610[row][col];
            specialty = getCardSpecialty(card);
            if (specialty == 6) {
                specialty = 5;
            }
            uploadTim((u32 *)((u8 *)D_801F161C + D_801F161C[card]), col * 21 + 0x2C0, row * 41, 0x2C0, 0xFF - (row * 3 + col));
            if (isSpritePoolFull()) {
                return;
            }
            CUR_SPRT->sp.x0 = x + D_801F1AF0[row][col];
            CUR_SPRT->sp.y0 = y + 3;
            CUR_SPRT->sp.u0 = col * 21 * 2;
            CUR_SPRT->sp.v0 = row * 41;
            CUR_SPRT->sp.clut = getClut(0x2C0, 0xFF - (row * 3 + col));
            CUR_SPRT->sp.w = 0x28;
            CUR_SPRT->sp.h = 0x28;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, 0x2C0, 0));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            if (isSpritePoolFull()) {
                return;
            }
            CUR_SPRT->sp.x0 = x + D_801F1AF0[row][col];
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x52;
            CUR_SPRT->sp.clut = getClut(0x2C0, specialty + 0xF0);
            CUR_SPRT->sp.w = 0x28;
            CUR_SPRT->sp.h = 0x30;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, 0x2C0, 0));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
    CUR_SPRT->sp.x0 = x + 0x84;
    CUR_SPRT->sp.y0 = y + 3;
    CUR_SPRT->sp.u0 = 0x80;
    CUR_SPRT->sp.v0 = 0x20;
    CUR_SPRT->sp.clut = 0x7EF8;
    CUR_SPRT->sp.w = 0x20;
    CUR_SPRT->sp.h = 0x29;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
}

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DDF38);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DDF3C);

void func_801E2B34(UiWindow *window) {
    s32 held[2];
    s32 offset;
    char text[64];
    s32 count;
    s32 i;
    s32 j;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    count = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (D_801F1610[i][j] != -1) {
                count++;
            }
        }
    }
    if (count == 0) {
        strcpy(text, "You can't Trade between 0 Cards!");
        offset = (0x100 - measureText(text)) / 2;
        drawText(x + offset, y + 14, (s32)text, 7, z);
        return;
    }
    strcpy(text, "If these Cards are OK,\npress and hold *b2 Button.");
    offset = (0x100 - measureText(text)) / 2;
    drawText(x + offset, y + 8, (s32)text, 7, z);
    for (i = 0; i < 2; i++) {
        if ((PAD_STATES[i]->held & 0x40) || D_801F1AE8 != 0) {
            held[i] = 1;
            CUR_SPRT->sp.x0 = i * 0xD6 + x - 2;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0xA0;
            CUR_SPRT->sp.v0 = 0x20;
            CUR_SPRT->sp.clut = 0x7F78;
            CUR_SPRT->sp.w = 0x2C;
            CUR_SPRT->sp.h = 0x2A;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x3E);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        } else {
            held[i] = 0;
        }
        CUR_SPRT->sp.x0 = x + 10 + i * 0xD6;
        CUR_SPRT->sp.y0 = y + 10;
        CUR_SPRT->sp.u0 = 0xCC;
        CUR_SPRT->sp.v0 = 0x20;
        CUR_SPRT->sp.clut = 0x7F38;
        CUR_SPRT->sp.w = 0x14;
        CUR_SPRT->sp.h = 0x1D;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
    if (held[0] && held[1] && D_801F1AE8 == 0) {
        animateWindowTo(&D_801F08D0, (Rect16 *)-1);
        D_801F1AE8 = 1;
        playMenuSound(1);
    }
}

typedef struct {
    UiWindow window;
    u8 port;
    u8 message;
    u8 unk46[2];
} MessageWindow;

extern MessageWindow D_801F0A40[2];

void func_801E3024(MessageWindow *window) {
    char text[64];
    s32 port;
    s32 x;
    s32 y;
    s32 z;

    port = window->port;
    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    switch (window->message) {
    case 1:
        strcpy(text, "You can't trade\nthis Card!");
        break;
    case 2:
        strcpy(text, "You can't transfer\nany more Cards!");
        break;
    case 3:
        strcpy(text, "This Card is\nunidentified!");
        break;
    }
    drawText(x + (0x5E - measureText(text)) / 2, y, (s32)text, 7, z);
    if (PAD_STATES[port]->pressed & 0x50) {
        animateWindowTo(&D_801F0A40[port].window, (Rect16 *)-1);
        (&D_801EFF30[port])->active = 1;
        playMenuSound(4);
    }
}

extern s32 D_801F1AE0[2];
extern s32 D_801F1B04;

void func_801E3214(PlayerWindow *window) {
    char text[72];
    u8 rgb[3];
    Rect16 rect;
    Rect16 rect2;
    s32 full;
    Menu *menu;
    UiWindow *win;
    s32 player;
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 card;
    s32 palette;
    s32 count;
    s32 shade;

    win = &window->window;
    player = window->player;
    x = win->originX;
    z = win->z;
    full = 0;
    if (D_801F1AE0[player] != 0) {
        CUR_SPRT->sp.x0 = player * 0x9C + 0x12;
        CUR_SPRT->sp.y0 = 0x4B;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x21;
        CUR_SPRT->sp.clut = 0x7E38;
        CUR_SPRT->sp.w = 0x80;
        CUR_SPRT->sp.h = 0x21;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        shade = 0x40;
    } else {
        shade = 0x80;
    }
    rgb[0] = shade;
    rgb[1] = shade;
    rgb[2] = shade;
    for (i = 0; i < D_801EFF30[player].nrows; i++) {
        if (i < win->view.y / D_801EFF30[player].rowH) {
            continue;
        }
        if ((win->view.y + win->rect.h) / D_801EFF30[player].rowH < i) {
            break;
        }
        y = win->originY + i * D_801EFF30[player].rowH + 1;
        card = D_801F0CA0[player][i]->id;
        palette = 8;
        if (D_801F1620[player][card] > 0) {
            palette = 7;
        }
        if (D_801F0CA0[player][i]->unk18 == 0) {
            palette = 3;
        }
        if (PLAYER_DATA(player).cardCollection[D_801F0CA0[player][i]->id] & 0x40) {
            drawTextColored(x + 0x12, y, D_801F0CA0[player][i]->name, rgb, palette, z);
        } else {
            palette = 9;
            drawTextColored(x + 0x12, y, "-----------------", rgb, 9, z);
        }
        sprintf(text, "%3.3d", D_801F0CA0[player][i]->id);
        drawSmallTextColored(x, y + 6, text, palette, rgb, z);
        sprintf(text, "%d", D_801F1620[player][card]);
        drawTextColored(x + 0x7E, y, text, rgb, palette, z);
    }
    menu = &D_801EFF30[player];
    updateMenuCursor(menu);
    if (menu->active) {
        card = D_801F0CA0[player][menu->row]->id;
        if (PAD_STATES[player]->pressed & 0x40) {
            if (PLAYER_DATA(player).cardCollection[card] & 0x40) {
                if (D_801F0CA0[player][menu->row]->unk18 == 0) {
                    rect.x = player * 0x9C + 0x21;
                    rect.y = 0x5A;
                    rect.w = 0x5E;
                    rect.h = 0x18;
                    animateWindowTo(&D_801F0A40[player].window, &rect);
                    D_801F0A40[player].message = 1;
                    menu->active = 0;
                    playMenuSound(3);
                } else if (D_801F1620[player][card] == 0) {
                    rect2.x = player * 0x9C + 0x21;
                    rect2.y = 0x5A;
                    rect2.w = 0x5E;
                    rect2.h = 0x18;
                    animateWindowTo(&D_801F0A40[player].window, &rect2);
                    D_801F0A40[player].message = 2;
                    D_801EFF30[player].active = 0;
                    playMenuSound(3);
                } else {
                    for (i = 0; i < 3; i++) {
                        if (D_801F1610[player][i] == -1) {
                            D_801F1610[player][i] = card;
                            D_801F1620[player][card]--;
                            if (i == 2) {
                                full = 1;
                            } else {
                                playMenuSound(1);
                            }
                            break;
                        }
                    }
                    if (i == 3) {
                        full = 1;
                    }
                }
            } else {
                rect.x = player * 0x9C + 0x21;
                rect.y = 0x5A;
                rect.w = 0x5E;
                rect.h = 0x18;
                animateWindowTo(&D_801F0A40[player].window, &rect);
                D_801F0A40[player].message = 3;
                D_801EFF30[player].active = 0;
                playMenuSound(3);
            }
        }
        if (PAD_STATES[player]->pressed & 0x10) {
            playMenuSound(0);
            for (i = 0, count = 0; i < 3; i++) {
                if (D_801F1610[player][i] != -1) {
                    count++;
                }
            }
            if (count == 0) {
                D_801F1B04 = player + 2;
                return;
            }
            for (i = 2; i >= 0; i--) {
                if (D_801F1610[player][i] != -1) {
                    D_801F1620[player][D_801F1610[player][i]]++;
                    D_801F1610[player][i] = -1;
                    break;
                }
            }
        }
        if ((PAD_STATES[player]->pressed & 0x800) || full) {
            D_801EFF30[player].active = 0;
            playMenuSound(1);
            D_801F1AE0[player] = 1;
            if (D_801F1AE0[0] != 0 && D_801F1AE0[1] != 0) {
                rect.x = 0x20;
                rect.y = 0x78;
                rect.w = 0x100;
                rect.h = 0x28;
                animateWindowTo(&D_801F08D0, &rect);
            }
        }
    } else if (D_801F1AE0[player] != 0 && D_801F1AE8 == 0 && (PAD_STATES[player]->pressed & 0x10)) {
        playMenuSound(0);
        D_801EFF30[player].active = 1;
        D_801F1AE0[player] = 0;
        animateWindowTo(&D_801F08D0, (Rect16 *)-1);
    }
}

void func_801E3CA8(PlayerWindow *window) {
    char text[72];
    s32 player;
    s32 row;
    s32 x;
    s32 y;
    s32 z;

    player = window->player;
    x = window->window.originX + 1;
    y = window->window.originY + 1;
    z = window->window.z;
    row = D_801EFF30[player].row;
    if (PLAYER_DATA(player).cardCollection[D_801F0CA0[player][row]->id] & 0x40) {
        switch (D_801F0CA0[player][row]->type) {
        case 0:
            drawIcon(x, y, 0, D_801F0CA0[player][row]->attr >> 4, z);
            drawIcon(x + 14, y, 0, (D_801F0CA0[player][row]->attr & 0xF) + 16, z);
            break;
        case 1:
            drawIcon(x, y, 0, 5, z);
            break;
        case 2:
            drawIcon(x, y, 0, 6, z);
            break;
        }
        sprintf(text, "in Stock. \f\a%d\f\x06 Cards", getOwnedCardCount(player, D_801F0CA0[player][row]->id));
        drawSmallText(x + 0x22, y, (s32)text, 6, z);
        sprintf(text, "in a Deck. \f\a%d\f\x06 Cards", D_801F1880[player][D_801F0CA0[player][row]->id]);
        drawSmallText(x + 0x31, y + 6, (s32)text, 6, z);
    } else {
        strcpy(text, "Unidentified Card");
        drawText(x + (0x90 - measureText(text)) / 2, y, (s32)text, 7, z);
    }
}

extern UiWindow D_801F0880;
extern PlayerWindow D_801F0B70[2];
extern PlayerWindow D_801F09B0[2];
extern PlayerWindow D_801F0920[2];
void func_801E2B34(UiWindow *window);
void func_801E23C4(UiWindow *window);
void func_801E3214(PlayerWindow *window);

void func_801E3FCC(void) {
    s32 count;
    s32 i;
    s32 j;

    func_801E16A0();
    count = 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            if (D_801F1610[i][j] != -1) {
                count++;
            }
        }
    }
    if (count == 0) {
        D_801F08D0.label = (s32)"WARNING";
        D_801F08D0.palette = 2;
    } else {
        D_801F08D0.label = (s32)"TRADE OK?";
        D_801F08D0.palette = 1;
    }
    drawWindow(&D_801F08D0, func_801E2B34, 1);
    drawWindow(&D_801F0880, func_801E23C4, 1);
    for (i = 0; i < 2; i++) {
        drawWindow(&D_801F0B70[i].window, func_801E143C, 1);
        drawWindow(&D_801F09B0[i].window, func_801E3214, 2);
        drawWindow(&D_801F0920[i].window, func_801E3CA8, 2);
        drawWindow(&D_801F0A40[i].window, func_801E3024, 1);
    }
}

extern CursorHighlight D_801F0C00[2];
extern CursorHighlight D_801F0AD0[2];
/* "Do you want to Quit Trading?": the string is followed by leftover bytes, so it stays as data */
extern char D_801DE164[];

void func_801E4170(s32 parentTask) {
    s32 open[2];
    Rect16 rect;
    u8 dialog[0xB8];
    s32 i;
    s32 j;
    s16 card;

    D_801F1AFC = 1;
    D_801F1B00 = -32;
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\TRADE.ARC", getCurrentTaskId());
    D_801F161C = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(D_801F161C[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)D_801F161C + D_801F161C[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(D_801F161C);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\CARD_F.TIM", getCurrentTaskId());
    D_801F161C = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTim(D_801F161C, 0x2C0, 0x52, 0x2C0, 0xF0);
    DrawSync(0);
    func_80014C08(FRAME_INTERVAL);
    freeHeapBlock(D_801F161C);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\M_CARD.ARC", getCurrentTaskId());
    D_801F161C = (u32 *)func_80014C08(0x7FFFFFFF);
    rect.x = 0x20;
    rect.y = 0x78;
    rect.w = 0x100;
    rect.h = 0x28;
    openWindow(&D_801F08D0, &rect, -1, (s16 *)-1, 8, 0x25, 0x80, 0xC);
    animateWindowTo(&D_801F08D0, (Rect16 *)-1);
    D_801F08D0.label = (s32)"TRADE OK?";
    rect.x = 0xC;
    rect.y = 0xB6;
    rect.w = 0x128;
    rect.h = 0x2E;
    openWindow(&D_801F0880, &rect, -1, (s16 *)-1, 8, 0x25, 0x80, 0xC);
    D_801F0880.label = (s32)"TRADE LIST";
    for (i = 0; i < 2; i++) {
        openMenu(&D_801EFFB4[i], &D_801F0B70[i].window, &D_801F0C00[i], (Bytes4 *)-1);
        animateWindowTo(&D_801F0B70[i].window, (Rect16 *)-1);
        D_801F0B70[i].window.label = (s32)"SORT MENU";
        D_801F0B70[i].player = i;
        rect.x = i * 0x9C + 0x21;
        rect.y = 0x5A;
        rect.w = 0x5E;
        rect.h = 0x18;
        openWindow(&D_801F0A40[i].window, &rect, -1, (s16 *)-1, 8, 0x11, 0x80, 0xC);
        animateWindowTo(&D_801F0A40[i].window, (Rect16 *)-1);
        D_801F0A40[i].window.label = (s32)"WARNING";
        D_801F0A40[i].window.palette = 2;
        D_801F0A40[i].port = i;
        rect.x = i * 0x9C + 0xA;
        rect.y = 0x9A;
        rect.w = 0x90;
        rect.h = 0xE;
        openWindow(&D_801F0920[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 0xC);
        D_801F0920[i].player = i;
        openMenu(&D_801EFF30[i], &D_801F09B0[i].window, &D_801F0AD0[i], (Bytes4 *)-1);
        if (i == 0) {
            D_801F09B0[i].window.label = (s32)"1P CARD LIST";
        } else {
            D_801F09B0[i].window.label = (s32)"2P CARD LIST";
        }
        D_801F09B0[i].player = i;
        D_801F1AE0[i] = 0;
        for (j = 0; j < 3; j++) {
            D_801F1610[i][j] = -1;
            D_801F1AF0[i][j] = j * 44 + i * 166;
        }
        open[i] = 0;
    }
    func_801E1E5C();
    for (i = 0; i < 2; i++) {
        func_801E2068(i);
        D_801EFF30[i].row = 0;
        centerMenuOnCursor(&D_801EFF30[i]);
    }
    D_801F1AE8 = 0;
    D_801F1B04 = 0;
    playMenuSound(3);
    addFrameCallback((s32)func_801E3FCC);
    do {
        func_80014C08(FRAME_INTERVAL);
        for (i = 0; i < 2; i++) {
            if (D_801F1AE0[i] == 0) {
                if (open[i]) {
                    if (PAD_STATES[i]->pressed & 0x110) {
                        open[i] = 0;
                        playMenuSound(4);
                        D_801EFF30[i].active = 1;
                        animateWindowTo(&D_801F0B70[i].window, (Rect16 *)-1);
                    }
                } else if (PAD_STATES[i]->pressed & 0x100) {
                    open[i] = 1;
                    playMenuSound(3);
                    D_801EFF30[i].active = 0;
                    animateWindowTo(&D_801F0B70[i].window, &D_801EFFB4[i].rect);
                }
            }
        }
        if (D_801F1AE8 == 2 && ((PAD_STATES[0]->pressed & 0x40) || (PAD_STATES[1]->pressed & 0x40))) {
            playMenuSound(1);
            PLAYER_DATA(0).unk28_11 = 1;
            PLAYER_DATA(1).unk28_11 = 1;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    card = D_801F1610[i][j];
                    if (card >= 0) {
                        func_801E1AA4(i ^ 1, card, func_801E1C8C(i, card));
                    }
                }
            }
            func_801E1E5C();
            for (i = 0; i < 2; i++) {
                func_801E2068(i);
                D_801EFF30[i].row = 0;
                centerMenuOnCursor(&D_801EFF30[i]);
            }
            D_801F1AE8 = 0;
            for (i = 0; i < 2; i++) {
                D_801EFF30[i].active = 1;
                D_801F1AE0[i] = 0;
                for (j = 0; j < 3; j++) {
                    D_801F1610[i][j] = -1;
                    D_801F1AF0[i][j] = j * 44 + i * 166;
                }
            }
        }
        if (D_801F1B04 >= 2) {
            initDialog(dialog, D_801DE164, 1);
            runDialogForPad((s32 *)dialog, D_801F1B04 - 2);
            switch ((s8)dialog[0xA5]) {
            case 1:
                D_801F1B04 = 1;
                break;
            case 0:
            case 2:
                D_801F1B04 = 0;
                break;
            }
        }
    } while (D_801F1B04 != 1);
    animateWindowTo(&D_801F08D0, (Rect16 *)-1);
    animateWindowTo(&D_801F0880, (Rect16 *)-1);
    for (i = 0; i < 2; i++) {
        animateWindowTo(&D_801F0B70[i].window, (Rect16 *)-1);
        animateWindowTo(&D_801F09B0[i].window, (Rect16 *)-1);
        animateWindowTo(&D_801F0920[i].window, (Rect16 *)-1);
        animateWindowTo(&D_801F0A40[i].window, (Rect16 *)-1);
    }
    playMenuSound(4);
    D_801F1AFC = 0;
    func_80014C08(20);
    removeFrameCallback((s32)func_801E3FCC);
    func_80014C08(2);
    freeHeapBlock(D_801F161C);
    func_80014A48(parentTask);
}

typedef struct {
    /* 0x00 */ s16 texX;
    /* 0x02 */ s16 texY;
    /* 0x04 */ s16 w;
    /* 0x06 */ s16 h;
    /* 0x08 */ s16 clutX;
    /* 0x0A */ s16 clutY;
    /* 0x0C */ u16 dx;
    /* 0x0E */ u16 dy;
    /* 0x10 */ u16 dw;
    /* 0x12 */ u16 dh;
    /* 0x14 */ u8 semiTrans;
    /* 0x15 */ u8 abr;
} TitlePart;
extern TitlePart D_801F0280[];
extern POLY_FT4 D_801F3C00[2][40];

/* the title part's primitive; the tpage and clut stores index the array directly */
#define PRIM (&D_801F3C00[FRAME_BUFFER_INDEX][D_801F4F20])

void func_801E4C18(s32 x, s32 y, s32 part) {
    initPrimByType(0xC, PRIM, D_801F0280[part].semiTrans, 0);
    PRIM->r0 = 0x80;
    PRIM->g0 = 0x80;
    PRIM->b0 = 0x80;
    PRIM->x0 = x += D_801F0280[part].dx;
    PRIM->y0 = y += D_801F0280[part].dy;
    PRIM->x1 = x + D_801F0280[part].dw;
    PRIM->y1 = y;
    PRIM->x2 = x;
    PRIM->y2 = y + D_801F0280[part].dh;
    PRIM->x3 = x + D_801F0280[part].dw;
    PRIM->y3 = y + D_801F0280[part].dh;
    PRIM->u0 = D_801F0280[part].texX % 64 * 4;
    PRIM->v0 = D_801F0280[part].texY % 256;
    PRIM->u1 = D_801F0280[part].texX % 64 * 4 + (D_801F0280[part].w - 1);
    PRIM->v1 = D_801F0280[part].texY % 256;
    PRIM->u2 = D_801F0280[part].texX % 64 * 4;
    PRIM->v2 = D_801F0280[part].texY % 256 + (D_801F0280[part].h - 1);
    PRIM->u3 = D_801F0280[part].texX % 64 * 4 + (D_801F0280[part].w - 1);
    PRIM->v3 = D_801F0280[part].texY % 256 + (D_801F0280[part].h - 1);
    D_801F3C00[FRAME_BUFFER_INDEX][D_801F4F20].tpage = getTPage(0, D_801F0280[part].abr, D_801F0280[part].texX, D_801F0280[part].texY);
    D_801F3C00[FRAME_BUFFER_INDEX][D_801F4F20].clut = getClut(D_801F0280[part].clutX, D_801F0280[part].clutY);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0xFF6], PRIM);
    D_801F4F20++;
}

#undef PRIM

void func_801E54EC(s32 x, s32 y, s32 z) {
    Rect16 rect; /* unused, but it sizes the frame */
    s32 next;
    s32 cur;
    s32 semiTrans;

    if (D_801F4888 != -1 || D_801F488C != D_801F4888) {
        if (D_801F4888 != D_801F488C) {
            cur = D_801F488C;
            next = D_801F4888;
            D_801F4890 += 4;
            if (D_801F4890 > 0x80) {
                D_801F4890 = 0;
                D_801F488C = next;
                cur = next;
                next = -1;
            }
        } else {
            cur = D_801F4888;
            next = -1;
        }
        if (next != -1) {
            semiTrans = D_801F4890 != 0x80;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = next % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, next + 0x1F1);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = D_801F4890;
            CUR_SPRT->sp.g0 = D_801F4890;
            CUR_SPRT->sp.b0 = D_801F4890;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 1, next / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = next % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, 0x1FF);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = D_801F4890;
            CUR_SPRT->sp.g0 = D_801F4890;
            CUR_SPRT->sp.b0 = D_801F4890;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 2, next / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
        if (cur != -1) {
            semiTrans = D_801F4890 != 0;
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = cur % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, cur + 0x1F1);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = 0x80 - D_801F4890;
            CUR_SPRT->sp.g0 = 0x80 - D_801F4890;
            CUR_SPRT->sp.b0 = 0x80 - D_801F4890;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 1, cur / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = cur % 3 * 0x52;
            CUR_SPRT->sp.clut = getClut(0, 0x1FF);
            CUR_SPRT->sp.w = 0xDC;
            CUR_SPRT->sp.h = 0x52;
            setSemiTrans(&CUR_SPRT->sp, semiTrans);
            CUR_SPRT->sp.r0 = 0x80 - D_801F4890;
            CUR_SPRT->sp.g0 = 0x80 - D_801F4890;
            CUR_SPRT->sp.b0 = 0x80 - D_801F4890;
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 2, cur / 3 * 128 + 0x180, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

void func_801E5C30(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    drawText(x, y, PLAYER_PROFILES, 7, z);
}

typedef struct {
    /* 0x0 */ char *text;
    /* 0x4 */ u8 kind;
    /* 0x5 */ u8 next;
    /* 0x6 */ u8 alt;
    /* 0x7 */ s8 image;
} ScrollPage;
extern ScrollPage D_801F0038[];

void func_801E5C6C(UiWindow *window) {
    char text[136];
    char *src;
    char *dst;
    s32 stop;
    s32 i;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (D_801F4F28.page != D_801F4F28.shownPage) {
        D_801F4F28.shownPage = D_801F4F28.page;
        D_801F4F28.length = 0;
    }
    D_801F4F28.done = 0;
    for (i = 0; i < 133; i++) {
        text[i] = 0;
    }
    text[0] = '*';
    text[1] = 's';
    text[2] = '0';
    dst = &text[3];
    src = D_801F0038[D_801F4F28.page].text;
    for (i = 0; i < D_801F4F28.length; i++) {
        stop = 0;
        while (!stop) {
            if (*src == '*') {
                *dst++ = *src++;
                *dst++ = *src++;
            } else {
                *dst++ = *src++;
                stop = 1;
            }
            if (dst[-1] == 0) {
                D_801F4F28.done = 1;
                break;
            }
        }
    }
    if (D_801F4F28.done == 0) {
        D_801F4F28.length++;
    }
    dst = text;
    drawText(x + 2, y + 1, (s32)dst, 7, z);
    if (D_801F4F28.done != 0 && D_801F4F28.waitInput != 0) {
        if (++D_801F4F28.blink & 0x10) {
            drawIcon(x + 0x119, y + 0x1C, 0, 0x1B, z);
        }
    } else {
        D_801F4F28.blink = 0;
    }
}

void func_801E5E7C(UiWindow *window) {
    func_801E54EC(window->originX, window->originY, window->z);
}

void func_801E5EAC(void) {
    Rect16 rect;
    s32 i;

    if (D_801F4888 != D_801F488C) {
        if (D_801F488C == -1) {
            rect.x = 0x32;
            rect.y = 0x48;
            rect.w = 0xDC;
            rect.h = 0x52;
            animateWindowTo(&D_801F1BB0, &rect);
            playMenuSound(3);
            D_801F4890 = 0;
            D_801F488C = D_801F4888;
        } else if (D_801F4888 == -1) {
            if (D_801F4890 == 0) {
                animateWindowTo(&D_801F1BB0, (Rect16 *)-1);
                playMenuSound(4);
            } else if (D_801F1BB0.from.w == 0) {
                D_801F4890 = 0;
                D_801F488C = -1;
            }
        }
    }
    drawWindow(&D_801F1B60, func_801E5C30, 0x19);
    drawWindow(&D_801F1B10, func_801E5C6C, 0x19);
    drawWindow(&D_801F1BB0, func_801E5E7C, 0x19);
    D_801F4F0C -= 16;
    if (D_801F4F0C < 16) {
        D_801F4F0C = 16;
    }
    D_801F4F10 += 8;
    if (D_801F4F10 > 0) {
        D_801F4F10 = 0;
    }
    D_801F4F04 += 4;
    if (D_801F4F04 > 28) {
        D_801F4F04 = 28;
    }
    D_801F4880 -= 12;
    if (D_801F4880 < 0xAA) {
        D_801F4880 = 0xAA;
    }
    D_801F4F1C -= 16;
    if (D_801F4F1C < 20) {
        D_801F4F1C = 20;
    }
    D_801F4F20 = 0;
    func_801E4C18(D_801F4F08, D_801F4F0C, 0);
    func_801E4C18(D_801F4F08, D_801F4F0C, 1);
    func_801E4C18(D_801F4F08, D_801F4F0C, 2);
    for (i = 0; i < D_801F4F28.unk0; i++) {
        func_801E4C18(i * 38 + D_801F4F00, D_801F4F04, 3);
    }
    func_801E4C18(D_801F4F00, D_801F4F04, 4);
    func_801E4C18(D_801F4F00, D_801F4F04, 5);
    func_801E4C18(D_801F4F00 + 0x26, D_801F4F04, 5);
    func_801E4C18(D_801F4F00 + 0x4C, D_801F4F04, 5);
    func_801E4C18(D_801F4F00 + 0x72, D_801F4F04, 5);
    func_801E4C18(D_801F4F18, D_801F4F1C, 10);
    func_801E4C18(D_801F4F18, D_801F4F1C, 11);
    func_801E4C18(D_801F4F18, D_801F4F1C, 12);
    func_801E4C18(D_801F4F18, D_801F4F1C, 13);
    func_801E4C18(D_801F4F18, D_801F4F1C, 14);
    func_801E4C18(D_801F4F18, D_801F4F1C, 15);
    func_801E4C18(D_801F4F18, D_801F4F1C, 16);
    func_801E4C18(D_801F4F18, D_801F4F1C, 17);
    func_801E4C18(D_801F4F18, D_801F4F1C, 18);
    func_801E4C18(D_801F4F18, D_801F4F1C, 19);
    func_801E4C18(D_801F4880, D_801F4884, 6);
    func_801E4C18(D_801F4880, D_801F4884, 7);
    func_801E4C18(D_801F4880, D_801F4884, 8);
    func_801E4C18(D_801F4880, D_801F4884, 9);
    func_801E4C18(D_801F4F10, D_801F4F14, 20);
    func_801E4C18(D_801F4F10, D_801F4F14, 21);
}

void func_801E62D8(void) {
    initScene3D(1);
    func_80014A00(0x19);
    func_800149B8(0x19, 0x1F, 0, 0x800, &runSceneCameraTask, 1);
    func_80014A00(0x1B);
    func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void func_801E6358(void) {
    func_80014A00(0x1B);
    func_80014A00(0x19);
    removeFrameCallback((s32)renderSceneModels);
    unloadAllModels();
    freeHeapBlocksByTag(0x7F);
}

void func_801E639C(void) {
    ((Graphics *)&GRAPHICS)->snapCamera = 0;
    ((Graphics *)&GRAPHICS)->unk90 = 3000;
    ((Graphics *)&GRAPHICS)->unk92 = -((Model2220 *)SCENE_3D->models[0])->bonepos[0][1] * 3;
    SCENE_3D->modelState[0] = 1;
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
}

void func_801E6424(void) {
    freeHeapBlocksByTag(500);
    unloadModel(0);
    unloadModelAnimations(0);
}

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE164);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E6454);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9BC);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9C4);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9CC);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9D4);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9DC);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9EC);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9F0);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE9F4);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", jtbl_801DEA04);

/* moves a keyboard row by d, wrapping inside its page of 9 rows */
#define WRAP_ROW(row, d) (((s16)((row) + (d)) - (d)) / 9 * 9 + ((s16)((row) + (d)) + 9) % 9)

/* declared s32 although nothing is returned: v0 stays live at the exits */
s32 func_801E6FB8(void) {
    UiWindow *window;
    Rect16 rect;

    window = &D_801F4F48;
    if (D_801F50D8.unk8 != 0) {
        if (D_801F50D8.unk17 == 0) {
            if ((u16)PAD_STATES[0]->repeat & 0xF000) {
                playMenuSound(2);
            }
            do {
                if (PAD_STATES[0]->repeat & 0x1000) {
                    D_801F50D8.row = WRAP_ROW(D_801F50D8.row, -1);
                    if (D_801F50D8.row % 9 == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x4000) {
                    D_801F50D8.row = WRAP_ROW(D_801F50D8.row, 1);
                    if (D_801F50D8.row % 9 == 8) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if ((u16)PAD_STATES[0]->repeat & 0x8000) {
                    if (--D_801F50D8.col < 0) {
                        D_801F50D8.col = 9;
                        D_801F50D8.unk17 = 1;
                    } else if (D_801F50D8.col == 0) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                } else if (PAD_STATES[0]->repeat & 0x2000) {
                    if (++D_801F50D8.col >= 10) {
                        D_801F50D8.col = 0;
                        D_801F50D8.unk17 = 1;
                    } else if (D_801F50D8.col == 9) {
                        PAD_STATES[0]->repeatEnabled = 0;
                    }
                }
            } while (D_801F0464[D_801F50D8.row * 10 + D_801F50D8.col] == ' ');
        } else {
            if ((u16)PAD_STATES[0]->pressed & 0xA000) {
                playMenuSound(2);
            }
            if ((u16)PAD_STATES[0]->pressed & 0x8000) {
                D_801F50D8.unk17 = 0;
                D_801F50D8.unk19 = -1;
                D_801F50D8.prevCol = -1;
                D_801F50D8.prevRow = -1;
                D_801F50D8.col = 9;
                while (D_801F0464[D_801F50D8.row * 10 + D_801F50D8.col] == ' ') {
                    D_801F50D8.col--;
                }
            } else if (PAD_STATES[0]->pressed & 0x2000) {
                D_801F50D8.unk17 = 0;
                D_801F50D8.unk19 = -1;
                D_801F50D8.prevCol = -1;
                D_801F50D8.prevRow = -1;
                D_801F50D8.col = 0;
                while (D_801F0464[D_801F50D8.row * 10 + D_801F50D8.col] == ' ') {
                    D_801F50D8.col++;
                }
            }
        }
    }
    if (D_801F50D8.unk17 == 0) {
        if (D_801F50D8.row != D_801F50D8.prevRow || D_801F50D8.col != D_801F50D8.prevCol) {
            D_801F50D8.prevCol = D_801F50D8.col;
            D_801F50D8.prevRow = D_801F50D8.row;
            rect.x = window->rect.x - window->scroll[2] + D_801F50D8.col * 17 + 4;
            rect.y = window->rect.y - window->scroll[3] + D_801F50D8.row * 14 + 1;
            rect.w = 12;
            rect.h = 12;
            if (D_801F50D8.col >= 5) {
                rect.x = window->rect.x - window->scroll[2] + D_801F50D8.col * 17 + 15;
            }
            moveCursorHighlight(&D_801F4F98, &rect);
        }
    } else if (D_801F50D8.unk18 != D_801F50D8.unk19) {
        D_801F50D8.unk19 = D_801F50D8.unk18;
        switch (D_801F50D8.unk18) {
        case 0:
        case 1:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + D_801F50D8.unk18 * 14 + 1;
            rect.w = 0x30;
            rect.h = 12;
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + D_801F50D8.unk18 * 14 + 1;
            rect.w = 0x24;
            rect.h = 12;
            break;
        case 7:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x63;
            rect.w = 0x18;
            rect.h = 12;
            break;
        case 8:
            rect.x = window->rect.x + 0xCC;
            rect.y = window->rect.y + 0x71;
            rect.w = 0x30;
            rect.h = 12;
            break;
        }
        moveCursorHighlight(&D_801F4F98, &rect);
    }
}

#undef WRAP_ROW


void func_801E7638(UiWindow *window) {
    char text[64];
    s8 rows[7];
    s32 i;
    s32 pad;
    s32 col;
    s32 item;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX + 4;
    y = window->originY + 1;
    z = window->z;
    for (i = 0; i < 90; i++) {
        sprintf(text, "%c", D_801F0464[i]);
        pad = 0;
        if (i % 10 >= 5) {
            pad = 11;
        }
        col = i % 10 * 17 + 4;
        drawText(x + col + pad, y + i / 10 * 14, (s32)text, 7, z);
    }
    for (i = 0; i < 7; i++) {
        rows[i] = 4;
    }
    rows[window->view.y / window->rect.h] = 5;
    drawText(window->rect.x + 0xD4, window->rect.y + 0x63, (s32)"OK", 6, z);
    func_801E6FB8();
    if (D_801F50D8.unk17 == 0) {
        if (PAD_STATES[0]->pressed & 0x40) {
            D_801F50D8.name[D_801F50D8.cursor] = D_801F0464[D_801F50D8.row * 10 + D_801F50D8.col];
            playMenuSound(1);
            if (D_801F50D8.cursor == 0 && D_801F50D8.unk1B == 1) {
                for (i = 1; i < 13; i++) {
                    D_801F50D8.name[i] = 0;
                }
            }
            if (D_801F50D8.cursor < 11) {
                D_801F50D8.cursor++;
            } else {
                D_801F50D8.unk17 = 1;
                D_801F50D8.unk18 = 7;
            }
            D_801F50D8.unk1B = 0;
        } else if (PAD_STATES[0]->pressed & 0x20) {
            for (i = 11; i >= D_801F50D8.cursor + 1; i--) {
                D_801F50D8.name[i] = D_801F50D8.name[i - 1];
            }
            D_801F50D8.name[D_801F50D8.cursor] = D_801F0464[D_801F50D8.row * 10 + D_801F50D8.col];
            playMenuSound(1);
            if (D_801F50D8.cursor < 11) {
                D_801F50D8.cursor++;
            } else {
                D_801F50D8.unk17 = 1;
                D_801F50D8.unk18 = 7;
            }
            D_801F50D8.unk1B = 0;
        } else if (PAD_STATES[0]->pressed & 0x10) {
            if (D_801F50D8.name[0] != 0) {
                playMenuSound(1);
            }
            if (D_801F50D8.cursor == 0) {
                D_801F50D8.cursor++;
            }
            for (i = D_801F50D8.cursor; i < 13; i++) {
                D_801F50D8.name[i - 1] = D_801F50D8.name[i];
            }
            D_801F50D8.cursor--;
            D_801F50D8.unk1B = 0;
        }
    } else if (PAD_STATES[0]->pressed & 0x40) {
        playMenuSound(1);
        item = D_801F50D8.unk18;
        if (item >= 0) {
            if (item < 7) {
                D_801F50D8.row = item * 9;
                D_801F50D8.col = 0;
                scrollWindowTo((s16 *)window, 0, D_801F50D8.row * 14);
            } else if (item < 9) {
                D_801F50D8.state = D_801F50D8.unk18;
            }
        }
    } else if (PAD_STATES[0]->pressed & 0x10) {
        if (D_801F50D8.name[0] != 0) {
            playMenuSound(1);
        }
        if (D_801F50D8.cursor == 0) {
            D_801F50D8.cursor++;
        }
        for (i = D_801F50D8.cursor; i < 13; i++) {
            D_801F50D8.name[i - 1] = D_801F50D8.name[i];
        }
        D_801F50D8.cursor--;
        D_801F50D8.unk1B = 0;
    }
    if (PAD_STATES[0]->pressed & 0x800) {
        if (D_801F50D8.unk17 != 1 || D_801F50D8.unk18 != 7) {
            playMenuSound(1);
            D_801F50D8.unk17 = 1;
            D_801F50D8.unk18 = 7;
        }
    }
    drawCursorHighlight(&D_801F4F98, z);
}

void func_801E7C54(UiWindow *window) {
    Rect16 target;
    char text[64];
    s32 x;
    s32 y;
    s32 z;

    x = window->originX + 1;
    y = window->originY;
    z = window->z;
    sprintf(text, "*s0%s", D_801F50D8.name);
    drawText(x, y, (s32)text, 7, z);
    if (PAD_STATES[0]->repeat & 4) {
        if (D_801F50D8.cursor != 0) {
            playMenuSound(2);
            D_801F50D8.cursor--;
        }
    } else if (PAD_STATES[0]->repeat & 8) {
        if (D_801F50D8.cursor != 11 && D_801F50D8.name[D_801F50D8.cursor] != 0) {
            playMenuSound(2);
            D_801F50D8.cursor++;
        }
    }
    target.x = x + D_801F50D8.cursor * 6;
    target.y = y + 13;
    target.w = 6;
    target.h = 0;
    moveCursorHighlight(&D_801F5038, &target);
    drawCursorHighlight(&D_801F5038, z);
}

void func_801E7DC4(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    Rect16 rect; /* unused, but it sizes the frame */

    x = window->originX + 1;
    y = window->originY + 1;
    z = window->z;
    drawText(x, y, (s32)"*b1 Delete", 7, z);
    drawText(x + 8, y + 13, (s32)"*b2 OK", 7, z);
    drawText(x, y + 26, (s32)"*b0 Insert", 7, z);
}

void func_801E7E64(void) {
    drawWindow(&D_801F4F48, func_801E7638, 1);
    drawWindow(&D_801F4FE8, func_801E7C54, 1);
    drawWindow(&D_801F5088, func_801E7DC4, 1);
}

/* "Is this name OK?": the string is followed by leftover bytes in the ROM, so it stays as data */
extern char D_801DEAC8[];
void func_801E7EC4(char *name, s32 parentTask) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];

    strcpy(D_801F50D8.name, name);
    D_801F50D8.col = 0;
    D_801F50D8.row = 0;
    D_801F50D8.prevCol = -1;
    D_801F50D8.prevRow = -1;
    D_801F50D8.unk8 = 1;
    D_801F50D8.cursor = 0;
    D_801F50D8.unk17 = 0;
    D_801F50D8.unk18 = 7;
    D_801F50D8.unk19 = -1;
    D_801F50D8.state = 0;
    D_801F50D8.unk1B = 1;
    rect.x = 0x18;
    rect.y = 0x5E;
    rect.w = 0x110;
    rect.h = 0x7E;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0x7E;
    openWindow(&D_801F4F48, &rect, -1, (s16 *)&view, 10, 0x81, 0x80, 0xC);
    D_801F4F48.label = (s32)"NAME ENTRY";
    D_801F4F48.labelPalette = 7;
    cursor.x = D_801F4F48.originX + 4;
    cursor.y = D_801F4F48.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&D_801F4F98, &cursor, (Bytes4 *)-1);
    rect.x = 0x1E;
    rect.y = 0x1C;
    rect.w = 0x36;
    rect.h = 0x28;
    openWindow(&D_801F5088, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 0xC);
    D_801F5088.label = (s32)"HELP";
    rect.x = 0xDE;
    rect.y = 0x1C;
    rect.w = 0x4A;
    rect.h = 0xE;
    openWindow(&D_801F4FE8, &rect, -1, (s16 *)-1, 8, 0x26, 0x80, 0xC);
    D_801F4FE8.label = (s32)"PLAYER NAME";
    cursor.x = D_801F4FE8.originX;
    cursor.y = D_801F4FE8.originY + 13;
    cursor.w = 12;
    cursor.h = 0;
    /* passes rect, not the cursor rect it just filled in */
    initCursorHighlight(&D_801F5038, &rect, (Bytes4 *)-1);
    playMenuSound(3);
    addFrameCallback((s32)func_801E7E64);
    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (D_801F50D8.state == 0) {
            continue;
        }
        if (D_801F50D8.name[0] == 0) {
            initDialog(dialog, "A name has not been entered!", 0);
            runDialog(dialog);
            D_801F50D8.state = 0;
        } else {
            initDialog(dialog, D_801DEAC8, 1);
            runDialog(dialog);
            switch ((s8)dialog[0xA5]) {
            case 1:
                break;
            case 0:
            case 2:
                D_801F50D8.state = 0;
                break;
            }
        }
        if (D_801F50D8.state != 0) {
            break;
        }
    }
    if (D_801F50D8.state == 7) {
        strcpy(name, D_801F50D8.name);
    }
    playMenuSound(4);
    animateWindowTo(&D_801F5088, (Rect16 *)-1);
    animateWindowTo(&D_801F4F48, (Rect16 *)-1);
    animateWindowTo(&D_801F4FE8, (Rect16 *)-1);
    func_80014C08(20);
    removeFrameCallback((s32)func_801E7E64);
    func_80014A48(parentTask);
}

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DEAC8);

void func_801E826C(s32 deck) {
    u8 *file;
    DeckFile *decks;
    s32 i;
    u16 card;

    func_800149B8(0, -1, 0, 0x800, loadFileTagged, "B:\\DECK2.DEK", getCurrentTaskId(), -2);
    file = (u8 *)func_80014C08(0x7FFFFFFF);
    decks = (DeckFile *)(file + 8);
    obtainPartner(0, deck);
    for (i = 0; i < 30; i++) {
        card = (&decks->starters[deck])->cards[i];
        setCardSlotFromId((u8 *)&PLAYER_DATA(0).savedDecks[0].cards[i], card);
        if (findPartnerSlot(0, card) == -1) {
            addCardToCollection(0, card, 1);
        }
    }
    strcpy((char *)PLAYER_DATA(0).savedDecks[0].unk1, decks->starters[deck].name);
    storeSavedDeck(0, &PLAYER_DATA(0).savedDecks[0], 0);
    PLAYER_DATA(0).opponentDeckFlags[deck + 0x8E] |= 0x8000;
    for (i = 0; i < 5; i++) {
        addCardToCollection(0, D_801F04C4[deck * 10 + i * 2 + rand() % 2], 1);
    }
    PLAYER_DATA(0).unk56 = deck;
    changeScrollingBackground(PLAYER_DATA(0).unk56, 0x380, 0, 0x380, 0x80);
    func_801EBA74(deck);
    freeHeapBlock(file);
}

void func_801E8494(s32 x, s32 y, s32 texX, s32 texY, s32 palette, u8 *rgb, s32 otIndex) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = (texX % 64) * 2 + 2;
        CUR_SPRT->sp.v0 = texY % 256 + 2;
        CUR_SPRT->sp.clut = getClut(0x2C0, palette + 0xF9);
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb[0];
        CUR_SPRT->sp.g0 = rgb[1];
        CUR_SPRT->sp.b0 = rgb[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, texX, texY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0x80;
            CUR_SPRT->sp.clut = getClut(0x2C0, palette + 0xB1);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb[0];
            CUR_SPRT->sp.g0 = rgb[1];
            CUR_SPRT->sp.b0 = rgb[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0xB);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

extern u8 D_801F04C0[3];
extern u8 *CROSS_EFFECT_SHORT_NAMES[];

void func_801E87E0(UiWindow *window) {
    Rect16 rect;
    char text[16];
    u8 rgb[3];
    s32 i;
    s32 card;
    s32 x;
    s32 y;
    s32 z;
    s32 j;
    s32 shade;
    s32 tx;

    z = window->z;
    y = window->originY + 1;
    for (i = 0; i < 3; i++) {
        card = D_801F04C0[i];
        shade = D_801F5198.deck;
        if (shade == i) {
            shade = 0x80;
        } else {
            shade = 0x40;
        }
        rgb[0] = shade;
        rgb[1] = shade;
        rgb[2] = shade;
        x = window->originX;
        tx = x + 4;
        func_801E8494(tx, y + 4, i * 20 + 0x2C0, 0, getCardSpecialty(card), rgb, z);
        tx = x + 0x30;
        drawTextColored(tx, y + 1, ((DigimonCardData *)DIGIMON_CARDS)[card].name, rgb, 7, z);
        drawIconColored(tx, y + 13, 0, 7, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[0].power);
        drawTextColored(x + 0x3E, y + 13, text, rgb, 7, z);
        drawIconColored(tx, y + 0x19, 0, 8, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[1].power);
        drawTextColored(x + 0x3E, y + 0x19, text, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 9, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].attack[2].power);
        drawTextColored(x + 0x3E, y + 0x25, text, rgb, 7, z);
        sprintf(text, "(%s)", CROSS_EFFECT_SHORT_NAMES[((DigimonCardData *)DIGIMON_CARDS)[card].crossEffect]);
        drawLargeTextColored(tx, y + 0x31, text, 7, rgb, z);
        tx = x + 0x5C;
        drawIconColored(tx, y + 0x19, 0, 0x19, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].dpBonus);
        drawTextColored(x + 0x68, y + 0x19, text, rgb, 7, z);
        drawIconColored(tx, y + 0x25, 0, 0x1A, rgb, z);
        sprintf(text, "%4d", ((DigimonCardData *)DIGIMON_CARDS)[card].hp);
        drawTextColored(x + 0x6A, y + 0x25, text, rgb, 7, z);
        tx = x + 0x88;
        drawTextColored(tx, y, "Support Effect", rgb, 6, z);
        for (j = 0; j < 4; j++) {
            drawTextColored(tx, y + 15 + j * 12, ((DigimonCardData *)DIGIMON_CARDS)[card].supportText[j], rgb, 7, z);
        }
        rect.x = tx;
        rect.y = y + 15;
        rect.w = 0x6E;
        rect.h = 0x30;
        drawWindowFrame(&rect, 0x31, 0, 0x80, 1, z);
        y += 0x48;
    }
    if (D_801F4F28.page != 5) {
        if (PAD_STATES[0]->repeat & 0x1000) {
            if (D_801F5198.deck != 0) {
                playMenuSound(2);
                D_801F5198.deck--;
                scrollWindowTo((s16 *)window, 0, D_801F5198.deck * 0x48);
            }
        }
        if (PAD_STATES[0]->repeat & 0x4000) {
            if (D_801F5198.deck < 2) {
                playMenuSound(2);
                D_801F5198.deck++;
                scrollWindowTo((s16 *)window, 0, D_801F5198.deck * 0x48);
            }
        }
        if (PAD_STATES[0]->pressed & 0x40) {
            playMenuSound(1);
            D_801F5198.chosen = 1;
        }
        D_801F4F28.page = D_801F5198.deck + 6;
        rect.x = window->rect.x - window->scroll[2] + 1;
        rect.y = window->rect.y - window->scroll[3] + D_801F5198.deck * 0x48;
        rect.w = 0x104;
        rect.h = 0x48;
        moveCursorHighlight(&D_801F5148, &rect);
        drawCursorHighlight(&D_801F5148, z);
    }
}

void func_801E8E6C(void) {
    drawWindow(&D_801F50F8, func_801E87E0, 1);
}

/* "Is this Deck OK?": the string is followed by leftover bytes in the ROM, so it stays as data */
extern char D_801DEB38[];

void func_801E8E9C(s32 parentTask) {
    Rect16 cursor;
    Rect16 rect;
    Rect16 view;
    u8 dialog[0xB8];
    u32 *arc;
    s32 i;

    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\BCARD.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\P_CARD.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < 3; i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), i * 20 + 0x2C0, 0, -1, -1);
        DrawSync(0);
    }
    D_801F5198.chosen = 0;
    D_801F5198.deck = 0;
    rect.x = 0x1E;
    rect.y = 0x44;
    rect.w = 0x104;
    rect.h = 0x5A;
    view.x = 0;
    view.y = 0;
    view.w = 0x10E;
    view.h = 0xD8;
    openWindow(&D_801F50F8, &rect, -1, (s16 *)&view, 10, 0x81, 0x80, 0xC);
    D_801F50F8.label = (s32)"STARTER SELECT";
    D_801F50F8.labelPalette = 7;
    cursor.x = D_801F50F8.originX + 4;
    cursor.y = D_801F50F8.originY + 1;
    cursor.w = 12;
    cursor.h = 12;
    initCursorHighlight(&D_801F5148, &cursor, (Bytes4 *)-1);
    func_80014C08(FRAME_INTERVAL);
    playMenuSound(3);
    addFrameCallback((s32)func_801E8E6C);
    D_801F4F28.waitInput = 1;
    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (D_801F4F28.page == 5 && (PAD_STATES[0]->pressed & 0x40)) {
            playMenuSound(1);
            D_801F4F28.waitInput = 0;
            D_801F4F28.page = 6;
        }
        if (D_801F5198.chosen == 0) {
            continue;
        }
        initDialog(dialog, D_801DEB38, 1);
        runDialog(dialog);
        switch ((s8)dialog[0xA5]) {
        case 1:
            break;
        case 0:
        case 2:
            D_801F5198.chosen = 0;
            break;
        }
        if (D_801F5198.chosen != 0) {
            break;
        }
    }
    animateWindowTo(&D_801F50F8, (Rect16 *)-1);
    playMenuSound(4);
    freeHeapBlock(arc);
    func_80014C08(20);
    removeFrameCallback((s32)func_801E8E6C);
    func_801E826C(D_801F5198.deck);
    func_80014A48(parentTask);
}

void func_801E9244(s32 idx) {
    POLY_FT4 *poly;
    s32 cx;
    s32 cy;
    s32 angle;

    D_801F5294 += 32;
    cx = rsin(D_801F5294) * D_801F5290 / 4096 + 160;
    cy = D_801F5290 * (rcos(D_801F5294) << 1) / 4096 + 60;
    if (idx & 1) {
        angle = D_801F5294;
    } else {
        angle = -D_801F5294;
    }
    poly = &D_801F51B0[idx][FRAME_BUFFER_INDEX];
    initPrimByType(0xC, poly, 1, 0);
    poly->r0 = D_801F529C;
    poly->g0 = D_801F529C;
    poly->b0 = D_801F529C;
    poly->u0 = 0;
    poly->v0 = 0;
    poly->u1 = 0xEF;
    poly->v1 = 0;
    poly->u2 = 0;
    poly->v2 = 0xEF;
    poly->u3 = 0xEF;
    poly->v3 = 0xEF;
    poly->x0 = cx + D_801F5298 * 120 / 100 * rsin(angle) / 4096;
    poly->y0 = cy + D_801F5298 * 120 / 100 * rcos(angle) / 4096;
    poly->x1 = cx + D_801F5298 * 120 / 100 * rsin(angle + 0x400) / 4096;
    poly->y1 = cy + D_801F5298 * 120 / 100 * rcos(angle + 0x400) / 4096;
    poly->x2 = cx + D_801F5298 * 120 / 100 * rsin(angle + 0xC00) / 4096;
    poly->y2 = cy + D_801F5298 * 120 / 100 * rcos(angle + 0xC00) / 4096;
    poly->x3 = cx + D_801F5298 * 120 / 100 * rsin(angle + 0x800) / 4096;
    poly->y3 = cy + D_801F5298 * 120 / 100 * rcos(angle + 0x800) / 4096;
    poly->tpage = 0x2A;
    poly->clut = 0x3E64;
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], poly);
}

void func_801E96D8(s32 x, s32 y, s32 texX, s32 texY, s32 w, s32 h, s32 clutX, s32 clutY, s32 texMode,
                   s32 semiTrans, s32 blendMode, s32 shade, s32 otIndex) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = (texX % 64) * (4 >> texMode);
        CUR_SPRT->sp.v0 = texY % 256;
        CUR_SPRT->sp.clut = getClut(clutX, clutY);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, semiTrans);
        CUR_SPRT->sp.r0 = shade;
        CUR_SPRT->sp.g0 = shade;
        CUR_SPRT->sp.b0 = shade;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(texMode, blendMode, texX, texY));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otIndex], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DEB38);

/* libgpu's POLY_F4 */
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} POLY_F4;
extern POLY_F4 D_801F5250[2];
extern DR_MODE D_801F5280[2];

void func_801E9938(void) {
    s32 shade;
    s32 i; /* also the shade of the dark band */

    switch (D_801F51A0) {
    case 7:
        if (D_801F52B4 > 0x80) {
            shade = 0x80;
        } else {
            shade = D_801F52B4;
        }
        func_801E96D8(0x48, 0xAC, 0x200, 0x98, 0xB0, 0x10, 0x240, 0xF8, 0, 1, 1, shade, 0xA);
        func_801E96D8(0x48, 0xAC, 0x200, 0x98, 0xB0, 0x10, 0x250, 0xF8, 0, 1, 2, shade, 0xA);
        break;
    case 8:
        for (i = 0; i < 3; i++) {
            func_801E96D8(i * 100 + 0xC, 0x9C, 0x2C0, i * 0x1C + 0x80, 0x60, 0x1C, D_801F0500[i] * 16 + 0x2C0, 0xFA, 0, 0, 0, 0x80, 5);
        }
        break;
    }
    switch (D_801F51A0) {
    case 8:
        D_801F52A8 += 2;
        if (D_801F52A8 > 16) {
            D_801F52A8 = 16;
        }
    case 7:
        if (D_801F51A0 == 7) {
            D_801F52A8 -= 2;
            if (D_801F52A8 < 0) {
                D_801F52A8 = 0;
            }
        }
        i = D_801F52A8 * 8;
        initPrimByType(8, &D_801F5250[FRAME_BUFFER_INDEX], 1, 0);
        D_801F5250[FRAME_BUFFER_INDEX].r0 = i;
        D_801F5250[FRAME_BUFFER_INDEX].g0 = i;
        D_801F5250[FRAME_BUFFER_INDEX].b0 = i;
        D_801F5250[FRAME_BUFFER_INDEX].x0 = 0;
        D_801F5250[FRAME_BUFFER_INDEX].y0 = 0x9A;
        D_801F5250[FRAME_BUFFER_INDEX].x1 = 0x140;
        D_801F5250[FRAME_BUFFER_INDEX].y1 = 0x9A;
        D_801F5250[FRAME_BUFFER_INDEX].x2 = 0;
        D_801F5250[FRAME_BUFFER_INDEX].y2 = 0xBA;
        D_801F5250[FRAME_BUFFER_INDEX].x3 = 0x140;
        D_801F5250[FRAME_BUFFER_INDEX].y3 = 0xBA;
        setlen(&D_801F5280[FRAME_BUFFER_INDEX], 1);
        D_801F5280[FRAME_BUFFER_INDEX].code[0] = _get_mode(0, 0, 0x40);
        addPrim(&CURRENT_FRAME_BUFFER->ot[6], &D_801F5250[FRAME_BUFFER_INDEX]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[6], &D_801F5280[FRAME_BUFFER_INDEX]);
    case 6:
        if (D_801F51A0 == 6) {
            if (++D_801F52A0 > 60) {
                D_801F51A0 = 7;
                D_801F52A0 = 0;
            }
        }
        D_801F52AC += 4;
        if (D_801F52AC > 0x80) {
            D_801F52AC = 0x80;
        }
        func_801E96D8(0x20, 0xC8, 0x200, 0xA8, 0xFF, 0x20, 0x240, 0xFA, 0, 1, 1, D_801F52AC, 0xA);
        func_801E96D8(0x20, 0xC8, 0x200, 0xA8, 0xFF, 0x20, 0x250, 0xFA, 0, 1, 2, D_801F52AC, 0xA);
    case 5:
        if (D_801F51A0 == 5) {
            D_801F52A4 -= 20;
            if (D_801F52A4 < 0) {
                D_801F51A0 = 6;
                D_801F52A0 = 0;
                D_801F52A4 = 0;
            }
        }
        func_801E96D8(D_801F52A4, 0x78 - D_801F52A8, 0x2C0, 0, 0x100, 0x30, 0x140, 0xF8, 1, 1, 0, 0x80, 5);
        func_801E96D8(D_801F52A4 + 0x100, 0x78 - D_801F52A8, 0x340, 0, 0x40, 0x30, 0x140, 0xF8, 1, 1, 0, 0x80, 5);
    case 4:
        if (D_801F51A0 == 4) {
            D_801F529C -= 16;
            if (D_801F529C < 0) {
                D_801F529C = 0;
            }
            D_801F5298 += 25;
            if (++D_801F52A0 > 60) {
                D_801F51A0 = 5;
                D_801F52A0 = 0;
                D_801F52A4 = 0x140;
                D_801F52A8 = 0;
            }
        }
        func_801E96D8(0x20, 0x10 - D_801F52A8, 0x200, 0, 0x100, 0x88, 0x140, 0xF9, 1, 1, 0, 0x80, 0xA);
    case 2:
    case 3:
        if (D_801F51A0 == 3) {
            if (++D_801F52A0 > 60) {
                D_801F51A0 = 4;
                D_801F52A0 = 0;
            }
        }
        func_801E96D8(0, 0, 0x140, 0, 0x100, 0xF0, 0x140, 0xFA, 1, 0, 0, 0x80, 0xA);
        func_801E96D8(0x100, 0, 0x1C0, 0, 0x40, 0xF0, 0x140, 0xFA, 1, 0, 0, 0x80, 0xA);
    case 1:
        if (D_801F51A0 == 1) {
            if (++D_801F52A0 > 100) {
                setScreenFadeParams(1, 1, 8);
                D_801F51A0 = 3;
                D_801F52A0 = 0;
            }
        }
    case 0:
        if (D_801F51A0 == 0) {
            D_801F52A0++;
            D_801F529C += 8;
            if (D_801F529C > 0x40) {
                D_801F529C = 0x40;
            }
            D_801F5298 += 2;
            if (D_801F5298 > 100) {
                D_801F5298 = 100;
            }
            D_801F5290 -= 2;
            if (D_801F5290 < 0) {
                D_801F5290 = 0;
                func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 4, 0);
                D_801F52A0 = 0;
                D_801F51A0 = 1;
            }
        }
        func_801E9244(0);
        func_801E9244(1);
        break;
    }
}

void func_801E9938();

void func_801EA2F8(s32 parentTask) {
    char text[192]; /* unused, but it sizes the frame */
    char *name;     /* never read: only its empty string is left in .rodata */
    u32 *arc;
    s32 i;
    s32 choice;
    s32 idle;
    s32 fade;
    s32 done;

    loadMusicTrack(0, 0x66, 0x7F);
    i = 0;
    hideScrollingBackground();
    loadScrollingBackground();
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\TITLE.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    name = "";
    choice = 1;
    for (i = 0; i < 3; i++) {
        D_801F0500[i] = 1;
    }
    D_801F0500[choice] = 0;
    fade = 0;
    playLoadedMusic(0);
    D_801F52AC = 0;
    D_801F52B0 = 0;
    D_801F52B4 = 0x80;
    D_801F52B8 = 4;
    D_801F52A0 = 0;
    D_801F5290 = 0xA0;
    D_801F5294 = 0;
    D_801F5298 = 0;
    D_801F529C = 0;
    D_801F51A0 = 0;
    done = 0;
    D_801F52BC[0] = 0;
    D_801F52BC[1] = 0x80;
    D_801F52BC[2] = 0x80;
    D_801F52C0[0] = 0;
    D_801F52C0[1] = 0x80;
    D_801F52C0[2] = 0x80;
    D_801F52C4 = 0;
    D_801F52C8 = 1;
    resetPlayerData();
    addFrameCallback((s32)func_801E9938);
    idle = 0;
    D_801F52A8 = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
        switch (D_801F51A0) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            if (PAD_STATES[0]->pressed & 0x800) {
                idle = 0;
                fade = 1;
                D_801F52A4 = 0;
                D_801F52A8 = 0x20;
                D_801F529C = 0;
                D_801F51A0 = 8;
                playMenuSound(1);
            }
            break;
        case 6:
        case 7:
            D_801F52B4 += D_801F52B8;
            if (D_801F52B4 >= 0x100) {
                D_801F52B4 = 0xFF;
                D_801F52B8 = -4;
            } else if (D_801F52B4 < 0) {
                D_801F52B4 = 0;
                D_801F52B8 = 4;
            }
            if (PAD_STATES[0]->pressed & 0x800) {
                idle = 0;
                D_801F51A0 = 8;
                playMenuSound(1);
            }
            idle++;
            break;
        case 8:
            if (fade) {
                if (isScreenFadeActive()) {
                    setScreenFadeParams(1, 1, 8);
                }
                fade = 0;
            }
            if (isScreenFadeActive() == 0) {
                idle++;
                if ((u16)PAD_STATES[0]->pressed & 0x8000) {
                    idle = 0;
                    playMenuSound(2);
                    choice = (choice + 2) % 3;
                } else if (PAD_STATES[0]->pressed & 0x2000) {
                    idle = 0;
                    playMenuSound(2);
                    choice = (choice + 4) % 3;
                }
                for (i = 0; i < 3; i++) {
                    D_801F0500[i] = 1;
                }
                D_801F0500[choice] = 0;
                if (PAD_STATES[0]->pressed & 0x10) {
                    idle = 0;
                    playMenuSound(0);
                    D_801F51A0 = 7;
                    D_801F52B4 = 0;
                    D_801F52B8 = 4;
                } else if (PAD_STATES[0]->pressed & 0x40) {
                    idle = 0;
                    playMenuSound(1);
                    done = 1;
                }
            }
            break;
        }
    } while (idle < 0xE11 && !done);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, NULL, 2, 8, 0);
    func_80014C08(20);
    removeFrameCallback((s32)func_801E9938);
    stopScreenFade();
    func_80014C08(10);
    if (idle >= 0xE11) {
        stopMusic();
        func_80014A48(0);
        func_80014A90();
    }
    func_80014A48(parentTask, choice);
}

void func_801EA874(void) {
    s32 i;
    s32 same;
    s32 clutY;

    if (D_801F52D0 == 1) {
        D_801F4F00 += 8;
        if (D_801F4F00 > 8) {
            D_801F4F00 = 8;
        }
    } else {
        D_801F4F00 -= 8;
        if (D_801F4F00 < -140) {
            D_801F4F00 = -140;
        }
    }
    func_801E96D8(D_801F4F00, D_801F4F04, 0x300, 0, 0x8C, 0x96, 0x300, 0x96, 0, 1, 1, 0x80, 2);
    same = (u16)PLAYER_DATA(0).unk10 == (u16)PLAYER_DATA(1).unk10;
    if ((D_801F5370 & 3) != 3) {
        same = 1;
    }
    for (i = 0; i < 7; i++) {
        if (*((s8 *)D_8006E054 + 0x1028) == i) {
            clutY = 0x98;
            if (i == 1 && same) {
                clutY = 0x99;
            }
        } else {
            clutY = 0x97;
            if (i == 1 && same) {
                clutY = 0x9A;
            }
        }
        func_801E96D8(D_801F4F00 + 0x19, D_801F4F04 + 0x16 + i * 16, 0x323, i * 16, 0x58, 0x10, 0x300, clutY, 0, 1, 0, 0x80, 2);
    }
}

char *strcat(char *dst, const char *src);

void func_801EAA54(PlayerWindow *window) {
    /* its own copy: a "Name" literal would be merged with the one in func_801ED3B8 */
    static const char nameLabel[] = "Name";
    char text[64];
    s32 total;
    s32 i;
    s32 x;
    s32 y;
    s32 z;

    x = window->window.originX;
    y = window->window.originY;
    z = window->window.z;
    total = 0;
    for (i = 0; i < 301; i++) {
        total += getOwnedCardCount(window->player, i);
    }
    drawText(x + 1, y, (s32)nameLabel, 6, z);
    drawText(x + 0x43, y, (s32)PLAYER_DATA(window->player).name, 7, z);
    drawText(x + 1, y + 12, (s32)"2 Player Battle", 6, z);
    sprintf(text, "*s0%3d    %3d", PLAYER_DATA(window->player).versusWins, PLAYER_DATA(window->player).versusLosses);
    drawText(x + 0x52, y + 12, (s32)text, 7, z);
    drawSmallText(x + 0x65, y + 19, (s32)"WINS", 6, z);
    drawSmallText(x + 0x90, y + 19, (s32)"LOSSES", 6, z);
    countSeenCards(window->player);
    drawText(x + 1, y + 24, (s32)"Cards in Stock.", 6, z);
    i = PLAYER_DATA(window->player).seenCardCount * 1000 / 301;
    sprintf(text, "*s0%3d.%1d*w4*c6%%", i / 10, i % 10);
    drawText(x + 0x67, y + 24, (s32)text, 7, z);
    drawText(x + 1, y + 36, (s32)"Cards in Possession.", 6, z);
    sprintf(text, "*s0%4d", total);
    drawText(x + 0x6D, y + 36, (s32)text, 7, z);
    drawSmallText(x + 0x87, y + 0x2B, (s32)"CARDS", 6, z);
    drawText(x + 1, y + 0x30, (s32)"Deck", 6, z);
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(window->player).savedDecks[i].inUse) {
            strcpy(text, (char *)PLAYER_DATA(window->player).savedDecks[i].unk1);
            strcat(text, "Deck");
        } else {
            strcpy(text, "Unused Deck");
        }
        drawText(x + 0x37, y + (i + 4) * 12, (s32)text, 7, z);
    }
    if (PLAYER_DATA(window->player).unk28_10) {
        drawIcon(x + 8, y + 0x44, 2, 11, z);
    }
}

void func_801EAE84(void) {
    s32 i;

    func_801EA874();
    for (i = 0; i < 2; i++) {
        drawWindow(&D_801F52E0[i].window, func_801EAA54, 4);
    }
    if (D_801F52D0 == 0) {
        D_801F4F0C += 16;
        if (D_801F4F0C > 240) {
            D_801F4F0C = 240;
        }
        D_801F4F10 -= 8;
        if (D_801F4F10 < -96) {
            D_801F4F10 -= 96;
        }
        D_801F4F1C += 16;
        if (D_801F4F1C > 240) {
            D_801F4F1C = 240;
        }
    } else {
        D_801F4F0C -= 16;
        if (D_801F4F0C < 16) {
            D_801F4F0C = 16;
        }
        D_801F4F10 += 8;
        if (D_801F4F10 > 0) {
            D_801F4F10 = 0;
        }
        D_801F4F1C -= 16;
        if (D_801F4F1C < 20) {
            D_801F4F1C = 20;
        }
    }
    D_801F4F20 = 0;
    func_801E4C18(D_801F4F08, D_801F4F0C, 0);
    func_801E4C18(D_801F4F08, D_801F4F0C, 1);
    func_801E4C18(D_801F4F08, D_801F4F0C, 2);
    func_801E4C18(D_801F4F18, D_801F4F1C, 10);
    func_801E4C18(D_801F4F18, D_801F4F1C, 11);
    func_801E4C18(D_801F4F18, D_801F4F1C, 12);
    func_801E4C18(D_801F4F18, D_801F4F1C, 13);
    func_801E4C18(D_801F4F18, D_801F4F1C, 14);
    func_801E4C18(D_801F4F18, D_801F4F1C, 15);
    func_801E4C18(D_801F4F18, D_801F4F1C, 16);
    func_801E4C18(D_801F4F18, D_801F4F1C, 17);
    func_801E4C18(D_801F4F18, D_801F4F1C, 18);
    func_801E4C18(D_801F4F18, D_801F4F1C, 19);
    func_801E4C18(D_801F4F10, D_801F4F14, 21);
}

void func_801EB0D0(void) {
    Rect16 to;
    Rect16 from;
    s32 i;

    D_801F4F08 = 12;
    D_801F4F0C = 240;
    D_801F4F10 = -96;
    D_801F4F14 = 0;
    D_801F4F18 = 14;
    D_801F4F1C = 240;
    D_801F4F00 = -140;
    D_801F4F04 = 80;
    D_801F52D0 = 1;
    D_801F52D4 = 0;
    for (i = 0; i < 2; i++) {
        from.x = 140;
        from.y = i * 100 + 40;
        from.w = 172;
        from.h = 84;
        to.x = 320;
        to.y = i * 100 + 40;
        to.w = 172;
        to.h = 84;
        openWindow(&D_801F52E0[i].window, &from, (s32)&to, (s16 *)-1, 8, 0x25, 0x80, 0xC);
        D_801F52E0[i].player = i;
        if (i == 0) {
            D_801F52E0[0].window.label = (s32)"PLAYER 1";
        } else {
            D_801F52E0[1].window.label = (s32)"PLAYER 2";
        }
    }
}

void func_801EB24C(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 2; i++) {
        rect.x = 320;
        rect.y = i * 100 + 40;
        rect.w = 172;
        rect.h = 84;
        animateWindowTo(&D_801F52E0[i].window, &rect);
    }
}

void func_801EAE84(void);
void func_801E4170();
s32 func_801EBE10(void);
void func_8002F298(s32 player);
void func_8002F3C4(s32 player);
void startVersusDuel(void);
void runTitleMenu(void);

void func_801EB2E8(void) {
    Rect16 rect;
    u8 dialog[0xB8];
    u32 *arc;
    char *message;
    s32 i;
    s32 ok;

    changeScrollingBackground(7, 0x380, 0, 0x380, 0x80);
    loadMusicTrack(0, 0x6D, 0x7F);
    playLoadedMusic(0);
    i = 0;
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\FRIEND.ARC", getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    func_801EB0D0();
    playMenuSound(3);
    addFrameCallback((s32)func_801EAE84);
    do {
        func_80014C08(FRAME_INTERVAL);
        D_801F5370 = PLAYER_DATA(0).unk28_10 | (PLAYER_DATA(1).unk28_10 << 1);
        if (PAD_STATES[0]->pressed & 0x40) {
            switch (((SessionView *)D_8006E054)->menuRow) {
            case 1:
                message = NULL;
                if ((u16)PLAYER_DATA(0).unk10 == (u16)PLAYER_DATA(1).unk10) {
                    message = "You can't trade the same Data!";
                } else if (!(D_801F5370 & 3)) {
                    message = "Trade is disabled.";
                } else if (!(D_801F5370 & 1)) {
                    message = "Player 1's trade is disabled.";
                } else if (!(D_801F5370 & 2)) {
                    message = "Player 2's trade is disabled.";
                }
                if (message == NULL) {
                    playMenuSound(4);
                    D_801F52D0 = 0;
                    func_801EB24C();
                    func_80014C08(20);
                    removeFrameCallback((s32)func_801EAE84);
                    func_800149B8(0, -1, 0, 0x800, func_801E4170, getCurrentTaskId(), 0, 0, 0);
                    func_80014C08(0x7FFFFFFF);
                    func_801EB0D0();
                    addFrameCallback((s32)func_801EAE84);
                } else {
                    playMenuSound(1);
                    initDialog(dialog, message, 0);
                    runDialog(dialog);
                }
                break;
            case 2:
                playMenuSound(4);
                D_801F52D0 = 0;
                func_801EB24C();
                func_80014C08(20);
                removeFrameCallback((s32)func_801EAE84);
                func_8002F298(0);
                func_801EB0D0();
                addFrameCallback((s32)func_801EAE84);
                break;
            case 3:
                playMenuSound(4);
                D_801F52D0 = 0;
                func_801EB24C();
                func_80014C08(20);
                removeFrameCallback((s32)func_801EAE84);
                func_8002F298(1);
                func_801EB0D0();
                addFrameCallback((s32)func_801EAE84);
                break;
            case 4:
                playMenuSound(4);
                D_801F52D0 = 0;
                func_801EB24C();
                func_80014C08(20);
                removeFrameCallback((s32)func_801EAE84);
                func_8002F3C4(0);
                func_801EB0D0();
                addFrameCallback((s32)func_801EAE84);
                break;
            case 5:
                playMenuSound(4);
                D_801F52D0 = 0;
                func_801EB24C();
                func_80014C08(20);
                removeFrameCallback((s32)func_801EAE84);
                func_8002F3C4(1);
                func_801EB0D0();
                addFrameCallback((s32)func_801EAE84);
                break;
            default:
                playMenuSound(4);
                D_801F52D4 = 1;
                break;
            }
        } else if (PAD_STATES[0]->pressed & 0x10) {
            playMenuSound(4);
            ((SessionView *)D_8006E054)->menuRow = 6;
            D_801F52D4 = 1;
        } else if (PAD_STATES[0]->repeat & 0x1000) {
            playMenuSound(2);
            ((SessionView *)D_8006E054)->menuRow--;
        } else if (PAD_STATES[0]->repeat & 0x4000) {
            playMenuSound(2);
            ((SessionView *)D_8006E054)->menuRow++;
        }
        ((SessionView *)D_8006E054)->menuRow = (((SessionView *)D_8006E054)->menuRow + 7) % 7;
        if (D_801F52D4 != 0) {
            D_801F52D0 = 2;
            func_801EB24C();
            if (((SessionView *)D_8006E054)->menuRow == 6) {
                initDialog((u8 *)&D_801F80D0, "Save \"Battle with Friend\" game?", 1);
                runDialog(&D_801F80D0);
                ok = 1;
                switch (D_801F80D0.choice) {
                case 1:
                    ok = func_801EBE10();
                    break;
                case 0:
                case 2:
                    initDialog((u8 *)&D_801F80D0, "Return to the Title Screen?", 1);
                    runDialog(&D_801F80D0);
                    if (D_801F80D0.choice == 1) {
                        ok = 0;
                    }
                    break;
                }
                if (ok) {
                    playMenuSound(3);
                    D_801F52D0 = 1;
                    D_801F52D4 = 0;
                    for (i = 0; i < 2; i++) {
                        rect.x = 0x8C;
                        rect.y = i * 100 + 0x28;
                        rect.w = 0xAC;
                        rect.h = 0x54;
                        animateWindowTo(&D_801F52E0[i].window, &rect);
                    }
                }
            }
        }
    } while (D_801F52D4 == 0);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    func_80014C08(20);
    removeFrameCallback((s32)func_801EAE84);
    hideScrollingBackground();
    stopScreenFade();
    switch (((SessionView *)D_8006E054)->menuRow) {
    case 0:
        func_800149B8(0, -1, 0, 0x200, startVersusDuel, 0, 0, 0, 0);
        break;
    case 6:
        func_800149B8(0, -1, 0, 0x100, runTitleMenu, 0, 0, 0, 0);
        break;
    }
}

s32 func_801EBA74(s32 kind) {
    s32 id;
    s32 word;
    s32 shift;
    s32 bit;

    if (kind == 0) {
        id = 0x126;
    } else if (kind == 1) {
        id = 0x12A;
    } else if (kind == 2) {
        id = 0x12D;
    } else {
        id = 0x126;
    }
    id -= 12;
    word = id / 32;
    shift = id % 32;
    ((PlayerProfile *)PLAYER_PROFILES)->unk23FC[word] |= bit = 1 << shift;
    return (((PlayerProfile *)PLAYER_PROFILES)->unk23FC[word] & bit) != 0;
}

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DED2C);

void func_801EC558(void);
void func_801EBEDC(s32 port);
void func_801EBF20(s32 port);
void func_801ECF48();
void func_801EEB9C();
void func_801EDC08();

void func_801EBAFC(s32 mode, s32 parentTask, s32 port) {
    D_801F80BF = mode;
    D_801F5378 = NULL;
    func_801EC558();
    func_801EBEDC(port);
    addFrameCallback((s32)func_801ECF48);
    do {
        func_801EBF20(port);
        func_800149B8(0, -1, 4, 0x800, func_801EEB9C, 0, 0, 0, 0);
        func_800149B8(0, -1, 0, 0x800, func_801EDC08, 0, 0, 0, 0);
        while (D_801F7B88.ready != 1) {
            func_80014C08(FRAME_INTERVAL);
        }
        if (D_801F7B88.unk539 != 0) {
            break;
        }
        refreshPartners(port);
        linkSavedDecks(port);
        port++;
    } while (D_801F7B88.again != 0);
    animateWindowTo(&D_801F5380, (Rect16 *)-1);
    playMenuSound(4);
    func_80014C08(20);
    removeFrameCallback((s32)func_801ECF48);
    switch (D_801F80CA) {
    case 0:
        break;
    case 1:
        initDialog((u8 *)&D_801F80D0, "Quit the game?", 1);
        runDialog(&D_801F80D0);
        if (D_801F80D0.choice == 1) {
            stopMusic();
            fadeOutScrollingBackground();
            func_80014C08(20);
            func_80014A48(0);
            func_80014A90();
        }
        break;
    }
    freeHeapBlock(D_801F5378);
    func_80014C08(20);
    freeHeapBlocksByTag(0x63);
    func_80014A48(parentTask);
}

s32 func_801EBD34(void) {
    s32 task;

    task = getCurrentTaskId();
    func_800149B8(0, -1, 0, 0x600, func_801EBAFC, 7, task, 0, 0);
    func_80014C08(0x7FFFFFFF);
    if (D_801F7B88.unk539 == 0) {
        func_800149B8(0, -1, 0, 0x600, func_801EBAFC, 7, task, 1, 0);
        func_80014C08(0x7FFFFFFF);
        if (D_801F7B88.unk539 == 0) {
            return 0;
        }
    }
    return 1;
}

s32 func_801EBE10(void) {
    func_800149B8(0, -1, 0, 0x600, func_801EBAFC, 6, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    return D_801F80C1 != 0;
}

void func_801EBE80(void) {
    func_800149B8(0, -1, 0, 0x600, func_801EBAFC, 0, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
}

void func_801EBEDC(s32 port) {
    D_801F7B88.unk533 = 0;
    D_801F7B88.unk534 = -1;
    D_801F7B88.unk532 = 0;
    D_801F7B88.buffer = allocHeapBlock(0x4000, 0x63);
}

/* libgpu's setRECT */
#define setRECT(r, _x, _y, _w, _h) (r)->x = (_x), (r)->y = (_y), (r)->w = (_w), (r)->h = (_h)

void func_801EBF20(s32 port) {
    Rect16 rect;
    s32 i;
    s32 j;

    if (D_801F7B88.mode == 6) {
        D_801F7B88.progress = port * 50;
    } else {
        D_801F80C7 = 0;
    }
    D_801F7B88.unk540 = 0;
    D_801F7B88.unk539 = 1;
    D_801F7B88.ready = 0;
    D_801F7B88.card = port;
    D_801F7B88.slot = 0;
    D_801F7B88.again = 0;
    switch (D_801F7B88.mode) {
    case 0:
        countSeenCards(port);
        D_801F7B88.unk53E = 0;
        ((SessionView *)D_8006E054)->saves[port].slot = 0;
        ((SessionView *)D_8006E054)->saves[port].file = 0;
        D_801F7B88.unk535 = 1;
        D_801F7B88.unk542 = 0;
        break;
    case 2:
    case 4:
    case 5:
        countSeenCards(port);
        D_801F7B88.unk53E = 0;
        D_801F7B88.unk535 = 0x11;
        D_801F7B88.unk542 = 1;
        break;
    case 8:
        countSeenCards(port);
        D_801F7B88.unk53E = 0;
        D_801F7B88.unk535 = 0x11;
        D_801F7B88.unk542 = 2;
        PLAYER_DATA(0).unkE = 0;
        PLAYER_DATA(0).unkF = 1;
        break;
    case 7:
        D_801F7B88.unk53E = 1;
        ((SessionView *)D_8006E054)->saves[port].slot = port;
        ((SessionView *)D_8006E054)->saves[port].file = 0;
        D_801F7B88.unk535 = 1;
        D_801F7B88.unk542 = 0;
        break;
    case 6:
        countSeenCards(port);
        D_801F7B88.unk53E = 0;
        D_801F7B88.unk535 = 0x11;
        if (port == 0) {
            D_801F7B88.again = 1;
        }
        D_801F80CA = 0;
        break;
    case 0xFF:
        D_801F7B88.unk53E = 1;
        ((SessionView *)D_8006E054)->saves[port].slot = 0;
        ((SessionView *)D_8006E054)->saves[port].file = 0;
        D_801F7B88.unk535 = 1;
        D_801F7B88.unk542 = 0;
        break;
    }
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 3; j++) {
            D_801F80B4[i][j] = 1;
        }
    }
    for (i = 0; i < 2; i++) {
        func_801EC650(&D_801F7B88.arrows[i], 0x80);
        func_801EC650(&D_801F7B88.arrowShadows[i], 0x40);
    }
    func_801EC728();
    D_801F7B88.sprites[1].x = 0x37;
    D_801F7B88.sprites[1].y = 0x70;
    D_801F7B88.sprites[0].x = 0x37;
    D_801F7B88.sprites[0].y = 0x56;
    D_801F7B88.sprites[2].x = 0x38;
    D_801F7B88.sprites[2].y = 0x76;
    D_801F7B88.sprites[3].x = 0xFC;
    D_801F7B88.sprites[3].y = 0x76;
    D_801F7B88.sprites[4].x = 0x77;
    D_801F7B88.sprites[4].y = 0x85;
    D_801F7B88.sprites[5].x = 0x32;
    D_801F7B88.sprites[5].y = 0x4C;
    setRECT(&D_801F7B88.sprites[0].uv, 0, 0x54, 0x20, 0x20);
    setRECT(&D_801F7B88.sprites[1].uv, 0x40, 4, 0x20, 8);
    setRECT(&D_801F7B88.sprites[2].uv, 0, 0x48, 0xC, 0xC);
    setRECT(&D_801F7B88.sprites[3].uv, 0xC, 0x48, 0xC, 0xC);
    setRECT(&D_801F7B88.sprites[4].uv, D_801F7B88.unk53E * 88 + 0x18, 0x48, 0x58, 8);
    setRECT(&D_801F7B88.sprites[5].uv, 0, 0, 0xDC, 0x48);
    rect.x = 0xC;
    rect.y = 0xBC;
    rect.w = 0x128;
    rect.h = 0x2A;
    openWindow(&D_801F5380, &rect, -1, (s16 *)-1, 8, 0x51, 0x80, 0xC);
    D_801F5380.labelPalette = 8;
    D_801F5380.label = (s32)"MESSAGE";
    playMenuSound(3);
}

extern char *D_801F0794[];

void func_801EC37C(UiWindow *window) {
    char text[136];
    char *src;
    char *dst;
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    if (D_801F80BC >= 0) {
        src = D_801F0794[D_801F80BC];
        dst = text;
        while (*src != 0) {
            if (*src == '*') {
                src++;
                switch (*src) {
                case 'a':
                case 'b':
                case 'c':
                case 'd':
                case 'e':
                case 'g':
                case 's':
                    *dst++ = '*';
                    *dst++ = *src++;
                    break;
                case 'h':
                case 'w':
                    *dst++ = '*';
                    *dst++ = *src++;
                    if (*src == '-') {
                        *dst++ = *src++;
                    }
                    break;
                case 'S':
                    src++;
                    *dst++ = D_801F7B88.card + '1';
                    continue;
                case 'E':
                    src++;
                    *dst++ = D_801F7B88.unk53D + '1';
                    continue;
                /* '\n' and a code above 'w' (which one is a guess) had cases of their
                   own with the default's code: they shape the switch's compare tree */
                case '\n':
                    *dst++ = '*';
                    break;
                case 'x':
                    *dst++ = '*';
                    break;
                default:
                    *dst++ = '*';
                    break;
                }
            }
            *dst++ = *src++;
        }
        *dst = 0;
        drawText(x + 2, y + 1, (s32)text, 7, z);
    }
}

void func_801EC558(void) {
    u32 *pack;

    func_800149B8(0, -1, 0, 0x800, loadFile, "C:\\OBJECT\\saveload.TIS", getCurrentTaskId(), 0, 0);
    pack = (u32 *)func_80014C08(0x7FFFFFFF);
    uploadTexturePack(pack);
    freeHeapBlock(pack);
    switch (D_801F80BF) {
    case 0:
    case 7:
    case 0xFF:
        func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\SAVE.ARC", getCurrentTaskId());
        D_801F5378 = (u8 *)func_80014C08(0x7FFFFFFF);
        break;
    }
}

void func_801EC650(POLY_FT4 *poly, s32 shade) {
    s32 u0;
    s32 u1;

    func_800677A4(poly);
    poly->r0 = shade;
    poly->g0 = shade;
    poly->b0 = shade;
    SetSemiTrans(poly, 0);
    poly->clut = 0x7F57;
    poly->tpage = 0x15;
    if (D_801F80C6 == 0) {
        poly->u0 = u0 = 0xE2;
        poly->v0 = 0;
        u1 = 0xE6;
    } else {
        poly->u0 = u0 = 0xEA;
        poly->v0 = 0;
        u1 = 0xEE;
    }
    poly->u1 = u1;
    poly->v1 = 0;
    poly->u2 = u0;
    poly->v2 = 8;
    poly->u3 = u1;
    poly->v3 = 8;
    poly->x0 = 0x46;
    poly->y0 = 0x78;
    poly->x1 = 0xF7;
    poly->y1 = 0x78;
    poly->x2 = 0x46;
    poly->y2 = 0x80;
    poly->x3 = 0xF7;
    poly->y3 = 0x80;
}

void func_801EC728(void) {
    Rect16 rect;
    s32 i;

    for (i = 0; i < 3; i++) {
        D_801F7B88.slotWindows[i].slot = i;
        D_801F7B88.slotWindows[i].message = 0x1D;
        rect.x = 16;
        rect.y = i * 42 + 0x3E;
        rect.w = 0x90;
        rect.h = 0x1A;
        openWindow(&D_801F7B88.slotWindows[i].window, &rect, -1, (s16 *)-1, 8, 0x21, 0x80, 0xC);
        switch (i) {
        case 0:
            D_801F7B88.slotWindows[i].window.label = (s32)"FILE 1";
            break;
        case 1:
            D_801F7B88.slotWindows[i].window.label = (s32)"FILE 2";
            break;
        case 2:
            D_801F7B88.slotWindows[i].window.label = (s32)"FILE 3";
            break;
        }
        animateWindowTo(&D_801F7B88.slotWindows[i].window, (Rect16 *)-1);
    }
    rect.x = 0xAC;
    rect.y = 0x5E;
    rect.w = 0x84;
    rect.h = 0x4E;
    openWindow(&D_801F7B88.infoWindow.window, &rect, -1, (s16 *)-1, 8, 0x21, 0x80, 0xC);
    D_801F7B88.infoWindow.window.label = (s32)"INFO.";
    animateWindowTo(&D_801F7B88.infoWindow.window, (Rect16 *)-1);
    rect.x = 0xAC;
    rect.y = 0x3E;
    rect.w = 0x84;
    rect.h = 0xE;
    openWindow(&D_801F53C8, &rect, -1, (s16 *)-1, 8, 0x21, 0x80, 0xC);
    D_801F53C8.label = (s32)"OPERATION";
    animateWindowTo(&D_801F53C8, (Rect16 *)-1);
}

void func_801EC994(s32 slot) {
    SaveSlot *save;

    if (D_801F5378 != NULL && D_801F7B88.flags[D_801F7B88.card][slot] == 0) {
        save = &D_801F7B88.slots[D_801F7B88.card][slot];
        if (save->location < 16) {
            uploadTim((u32 *)(D_801F5378 + ((s32 *)D_801F5378)[save->location + 16]), 0x180, 0x14E, 0x180, 0x1FD);
        }
    }
}

void func_801ECA38(void) {
    Rect16 rect;
    SaveSlot *save;
    s32 i;

    for (i = 0; i < 3; i++) {
        if (D_801F5378 != NULL && D_801F7B88.flags[D_801F7B88.card][i] == 0) {
            save = &D_801F7B88.slots[D_801F7B88.card][i];
            if (save->location < 16) {
                uploadTim((u32 *)(D_801F5378 + ((s32 *)D_801F5378)[save->location]), 0x180, i * 26 + 0x100, 0x180, i + 0x1FA);
            }
        }
        rect.x = 16;
        rect.y = i * 42 + 0x3E;
        rect.w = 0x90;
        rect.h = 0x1A;
        animateWindowTo(&D_801F7B88.slotWindows[i].window, &rect);
    }
    func_801EC994(D_801F7B88.slot);
    rect.x = 0xAC;
    rect.y = 0x5E;
    rect.w = 0x84;
    rect.h = 0x4E;
    animateWindowTo(&D_801F7B88.infoWindow.window, &rect);
    rect.x = 0xAC;
    rect.y = 0x3E;
    rect.w = 0x84;
    rect.h = 0xE;
    animateWindowTo(&D_801F53C8, &rect);
    playMenuSound(3);
}

void func_801ECBE8(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        animateWindowTo(&D_801F7C88[i].window, (Rect16 *)-1);
    }
    animateWindowTo(&D_801F7D60, (Rect16 *)-1);
    animateWindowTo(&D_801F53C8, (Rect16 *)-1);
    playMenuSound(4);
}

extern u8 D_801F050C[];
extern u8 D_801F0510[];
void func_801ED3B8(SlotDraw draw);

void func_801ECC64(SlotWindow *window) {
    SlotDraw draw;
    SaveSlot *save;

    if (window->window.brightness == 0x80) {
        draw.color = D_801F050C;
    } else {
        draw.color = D_801F0510;
    }
    draw.x = window->window.originX;
    draw.y = window->window.originY;
    draw.z = window->window.z;
    draw.slot = window->slot;
    if (draw.slot >= 3) {
        draw.slot = 0;
    }
    if (D_801F7B88.flags[D_801F7B88.card][draw.slot] == 0) {
        save = &D_801F7B88.slots[D_801F7B88.card][draw.slot];
        if (save->size == 0x2774) {
            func_801ED3B8(draw);
        } else {
            drawTextColored(draw.x + 0xC, draw.y + 7, "File is corrupted!", draw.color, 7, draw.z);
        }
    } else {
        drawTextColored(draw.x + 0x30, draw.y + 7, "NO DATA", draw.color, 7, draw.z);
    }
}

void func_801ECDC0(UiWindow *window) {
    SaveSlot *save;

    if (D_801F7B88.flags[D_801F7B88.card][D_801F7B88.slot] == 0) {
        save = &D_801F7B88.slots[D_801F7B88.card][D_801F7B88.slot];
        if (save->size == 0x2774) {
            func_801ED780(window->originX, window->originY, window->z);
        } else {
            drawText(window->originX + 0x2C, window->originY + 0x20, (s32)"NO DATA", 7, window->z);
        }
    } else {
        drawText(window->originX + 0x2C, window->originY + 0x20, (s32)"NO DATA", 7, window->z);
    }
}

void func_801ECEAC(UiWindow *window) {
    char text[40];
    s32 x;

    sprintf(text, "Player %d : Slot %d", D_801F80C4 + 1, ((SessionData *)D_8006E054)->unk1010[D_801F80C4 * 8] + 1);
    x = (0x84 - strlen(text) * 6) / 2;
    drawText(window->originX + x, window->originY + 1, (s32)text, 7, window->z);
}

typedef struct {
    /* 0x00 */ Rect16 uv;
    /* 0x08 */ s32 duration;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 y;
} CardAnimFrame;
extern CardAnimFrame D_801F0514[];

void func_801ECF48(void) {
    s8 firstFrames[7] = { 0, 6, 9, 17, 19, 21, 23 };
    char text[24];
    MenuSprite *sprite;
    POLY_FT4 *arrow;
    POLY_FT4 *shadow;
    s32 time;
    s32 i;
    s8 frame;

    sprite = D_801F7B88.sprites;
    arrow = &D_801F7B88.arrows[FRAME_BUFFER_INDEX];
    shadow = &D_801F7B88.arrowShadows[FRAME_BUFFER_INDEX];
    drawWindow(&D_801F5380, func_801EC37C, 1);
    for (i = 0; i < 3; i++) {
        drawWindow(&D_801F7B88.slotWindows[i].window, func_801ECC64, D_801F7B88.slotWindows[i].message);
    }
    drawWindow(&D_801F7B88.infoWindow.window, func_801ECDC0, 1);
    drawWindow(&D_801F53C8, func_801ECEAC, 1);
    frame = -1;
    if (D_801F7B88.progress != 0) {
        D_801F7B88.unk532++;
        do {
            time = D_801F7B88.unk532;
            for (i = firstFrames[D_801F7B88.unk533]; i < firstFrames[D_801F7B88.unk533 + 1]; i++) {
                if (time < D_801F0514[i].duration) {
                    frame = i;
                    break;
                }
                time -= D_801F0514[i].duration;
            }
            if (frame == -1) {
                D_801F7B88.unk532 = time;
                if (D_801F7B88.unk533 == 1) {
                    D_801F7B88.unk533 = 2;
                }
                if (D_801F7B88.unk533 == 3) {
                    D_801F7B88.unk533 = 4;
                }
                if (D_801F7B88.unk533 == 5) {
                    D_801F7B88.unk533 = 0;
                }
            }
        } while (frame == -1);
        sprite[0].uv = D_801F0514[frame].uv;
        sprite[0].x = ((100 - D_801F80C7) * 55 + D_801F80C7 * 232) / 100;
        sprite[0].y = D_801F0514[frame].y + 0x56;
        sprite[1].x = sprite[0].x;
        if (frame >= 11 && frame < 16) {
            sprite[1].uv.y = 0xBC;
        } else {
            sprite[1].uv.y = 0xB4;
        }
        arrow->x1 = arrow->x3 = sprite[0].x + 15;
        addPrim(&CURRENT_FRAME_BUFFER->ot[31], arrow);
        addPrim(&CURRENT_FRAME_BUFFER->ot[31], shadow);
        for (i = 0; i < 6; i++, sprite++) {
            if (i != 4 || (D_801F7B88.progress >= 1 && D_801F7B88.progress < 100)) {
                if (i == 0) {
                    drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, 0x15, 0x7F97, 0x1E, 0x80, -1);
                } else {
                    drawTexturedSprite(sprite->x, sprite->y, &sprite->uv, 0x15, 0x7FD7, 0x1E, 0x80, -1);
                }
            }
        }
        sprintf(text, "*s0%3d*w3%%", D_801F80C7);
        drawText(0xE6, 0x82, (s32)text, 6, 0x1D);
    }
}

void func_801ED3B8(SlotDraw draw) {
    char text[24];
    SaveSlot *save;
    s32 time;
    s32 hour;
    s32 minute;
    s32 second;

    save = &D_801F7B88.slots[D_801F7B88.card][draw.slot];
    drawTextColored(draw.x + 1, draw.y + 1, "Name", draw.color, 6, draw.z);
    drawTextColored(draw.x + 0x43, draw.y + 1, save->name, draw.color, 7, draw.z);
    time = save->playTime;
    hour = time / 216000;
    minute = (time - hour * 216000) / 3600;
    second = (time - hour * 216000 - minute * 3600) / 60;
    if (hour >= 1000) {
        hour = 999;
        minute = 59;
        second = 59;
    }
    drawTextColored(draw.x + 1, draw.y + 13, "Playing Time", draw.color, 6, draw.z);
    sprintf(text, "%3d:%2.2d:%2.2d", hour, minute, second);
    drawTextColored(draw.x + 0x43, draw.y + 13, text, draw.color, 7, draw.z);
    if (save->location < 16) {
        CUR_SPRT->sp.x0 = draw.x;
        CUR_SPRT->sp.y0 = draw.y;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = draw.slot * 26;
        CUR_SPRT->sp.clut = getClut(0x180, draw.slot + 0x1FA);
        CUR_SPRT->sp.w = 0x90;
        CUR_SPRT->sp.h = 0x1A;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = draw.color[0];
        CUR_SPRT->sp.g0 = draw.color[1];
        CUR_SPRT->sp.b0 = draw.color[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x96);
        addPrim(&CURRENT_FRAME_BUFFER->ot[draw.z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[draw.z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

extern char *D_801F0684[];
extern char *D_801F06C4[];
extern char *D_801F06D4[];
/* "Arena": the string is followed by two leftover bytes (E0 03) in the ROM, so it stays as data */
extern char D_801DF78C[];

void func_801ED780(s32 x, s32 y, s32 z) {
    char text[24];
    SaveSlot *save;
    s32 completion;
    s32 collection;
    s32 slot;

    slot = D_801F7B88.slot;
    save = &D_801F7B88.slots[D_801F7B88.card][slot];
    completion = save->progress * 1000 / 166;
    collection = save->seenCardCount * 1000 / 301;
    drawText(x + 2, y + 1, (s32)"Game Completion", 6, z);
    sprintf(text, "%3d.%1d*w3*c6%%", completion / 10, completion % 10);
    drawText(x + 0x5B, y + 1, (s32)text, 7, z);
    drawText(x + 2, y + 14, (s32)"Card Collection", 6, z);
    sprintf(text, "%3d.%1d*w3*c6%%", collection / 10, collection % 10);
    drawText(x + 0x5B, y + 14, (s32)text, 7, z);
    if (save->unk28_10) {
        drawIcon(x + 0x68, y + 0x1A, 2, 11, z);
    }
    if (save->location < 16) {
        drawText(x + 2, y + 0x1B, (s32)"Current Position", 6, z);
        drawText(x + 13, y + 0x28, (s32)D_801F0684[save->location], 7, z);
        if (save->location >= 12) {
            drawText(x + 13, y + 0x35, (s32)D_801F06C4[save->location - 12], 7, z);
        } else if (save->arena >= 2 && save->arena < 6) {
            drawText(x + 2, y + 0x35, (s32)D_801DF78C, 6, z);
            drawText(x + 13, y + 0x42, (s32)D_801F06D4[save->location * 4 + save->arena - 2], 7, z);
        }
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0;
        CUR_SPRT->sp.v0 = 0x4E;
        CUR_SPRT->sp.clut = 0x7F58;
        CUR_SPRT->sp.w = 0x84;
        CUR_SPRT->sp.h = 0x4E;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x96);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DF78C);

extern s32 D_801D8198;
void func_801EE254(s32 port);
void func_801EE318(s32 port);
void func_801EE3DC(s32 pad);
void func_801EE488(s32 port);

void func_801EDC08(void) {
    s32 port;

    do {
        func_80014C08(FRAME_INTERVAL);
        D_801D8198++;
        port = D_801F7B88.card;
        switch (D_801F7B88.unk535) {
        case 17:
            if (D_801F7B88.mode == 6) {
                D_801F7B88.unk535 = 0x17;
            } else {
                initDialog((u8 *)&D_801F80D0, "Save the game up to this point?", 1);
                D_801F80D0.choice = 1;
                runDialogForPad((s32 *)&D_801F80D0, port);
                switch (D_801F80D0.choice) {
                case 1:
                    D_801F80BD = 0x17;
                    break;
                case 0:
                case 2:
                    D_801F80BD = 0x1B;
                    break;
                }
            }
            break;
        case 24:
            if (D_801F80BF != 6) {
                func_801EE254(port);
            } else {
                D_801F7B88.unk535 = 0x12;
                if (port != 0) {
                    D_801F7B88.unk533 = 5;
                }
            }
            break;
        case 10:
            D_801F80BC = 0x10;
            func_801EE488(port);
            break;
        case 3:
            D_801F80BC = 0x11;
            func_801EE488(port);
            break;
        case 16:
            D_801F80BC = 6;
            initDialog((u8 *)&D_801F80D0, NULL, 1);
            runDialogForPad((s32 *)&D_801F80D0, port);
            switch (D_801F80D0.choice) {
            case 1:
                D_801F80BD = 0x1B;
                break;
            case 0:
            case 2:
                D_801F80BD = 1;
                break;
            }
            break;
        case 22:
            D_801F80BC = 0x19;
            initDialog((u8 *)&D_801F80D0, NULL, 1);
            runDialogForPad((s32 *)&D_801F80D0, port);
            switch (D_801F80D0.choice) {
            case 1:
                D_801F80BD = 0x1B;
                break;
            case 0:
            case 2:
                D_801F80BD = 0x11;
                break;
            }
            break;
        case 29:
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                if (D_801F7B88.mode == 0) {
                    D_801F7B88.unk534 = 4;
                    D_801F7B88.unk535 = 2;
                } else {
                    D_801F80BD = 0x10;
                }
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F80BD = 1;
            }
            break;
        case 19:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F80BD = 0x11;
            } else if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                D_801F80BD = 0x16;
            }
            break;
        case 28:
            D_801F7B88.unk534 = 1;
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                D_801F7B88.unk535 = 0x10;
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F80BD = 1;
            }
            break;
        case 21:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F80BD = 0x17;
            }
            break;
        case 11:
            if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                D_801F80BD = 0x10;
            } else if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F80BD = 1;
            }
            break;
        case 14:
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F80BD = 1;
            }
            break;
        case 7:
            D_801F80BC = 3;
            func_801EE3DC(port);
            break;
        case 2:
            func_801EE318(port);
            break;
        case 5:
            D_801F7B88.progress = 0;
            D_801F7B88.unk534 = 5;
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F7B88.unk535 = 1;
            }
            break;
        case 6:
            D_801F7B88.unk534 = 9;
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F7B88.unk535 = 1;
            } else if (PAD_STATES[port]->pressed & 0x10) {
                playMenuSound(0);
                D_801F7B88.unk534 = 0x18;
                D_801F7B88.unk535 = 2;
            }
            break;
        case 18:
            D_801F80BC = 0xB;
            break;
        case 4:
            D_801F80BC = 0xE;
            break;
        case 9:
            D_801F80BC = 0x12;
            break;
        case 15:
            D_801F7B88.unk534 = 0x13;
            if (PAD_STATES[port]->pressed & 0x40) {
                playMenuSound(1);
                D_801F7B88.unk535 = 1;
            }
            break;
        case 12:
            if (PAD_STATES[port]->pressed & 0x40) {
                D_801F7B88.unk539 = 0;
                D_801F7B88.ready = 1;
            }
            break;
        case 26:
            if (D_801F80BF == 6) {
                port = 0;
            }
            if (PAD_STATES[port]->pressed & 0x40) {
                D_801F80C2 = 1;
            }
            break;
        case 27:
            D_801F80C2 = 1;
            break;
        case 8:
            D_801F80BC = 7;
            break;
        case 1:
            break;
        }
    } while (D_801F80C2 != 1);
    func_80014C08(FRAME_INTERVAL);
}

void func_801EE254(s32 port) {
    char text[136];

    sprintf(text, "Will write over File%d.\nIs this OK?", ((SessionView *)D_8006E054)->saves[port].file + 1);
    initDialog((u8 *)&D_801F80D0, text, 1);
    runDialogForPad((s32 *)&D_801F80D0, port);
    switch (D_801F80D0.choice) {
    case 1:
        D_801F80BD = 0x12;
        break;
    case 0:
    case 2:
        D_801F80BD = 0x11;
        break;
    }
}

void func_801EE318(s32 port) {
    initDialog((u8 *)&D_801F80D0, NULL, 1);
    runDialogForPad((s32 *)&D_801F80D0, port);
    switch (D_801F80D0.choice) {
    case 1:
        D_801F80C2 = 1;
        ((SessionData *)D_8006E054)->unk1027 = 1;
        func_80014A90();
        break;
    case 0:
    case 2:
        D_801F80BD = 1;
        ((SessionData *)D_8006E054)->unk1027 = 0;
        break;
    }
}

void func_801EE3DC(s32 pad) {
    initDialog((u8 *)&D_801F80D0, "Format the Memory Card in Slot 1?", 1);
    runDialogForPad((s32 *)&D_801F80D0, pad);
    switch (D_801F80D0.choice) {
    case 1:
        D_801F80BD = 8;
        break;
    case 0:
    case 2:
        D_801F7B88.unk534 = 0x18;
        D_801F7B88.unk535 = 2;
        break;
    }
}

extern s8 D_801F80C3;
extern u8 D_801F80BE;

void func_801EE488(s32 port) {
    char text[136];
    s32 i; /* first the dialog's yes/no flag, then the slot loop's index */

    if (PAD_STATES[port]->repeat & 0x4000) {
        playMenuSound(2);
        if (++D_801F7B88.slot == 2) {
            PAD_STATES[port]->repeatEnabled = 0;
        }
        if (D_801F7B88.slot >= 3) {
            D_801F7B88.slot = 0;
        }
        func_801EC994(D_801F80C3);
    } else if (PAD_STATES[port]->repeat & 0x1000) {
        playMenuSound(2);
        if (--D_801F7B88.slot == 0) {
            PAD_STATES[port]->repeatEnabled = 0;
        }
        if (D_801F7B88.slot < 0) {
            D_801F7B88.slot = 2;
        }
        func_801EC994(D_801F80C3);
    } else if (PAD_STATES[port]->pressed & 0x40) {
        if (D_801F7B88.flags[port][D_801F7B88.slot] == 0 || D_801F7B88.mode == 0) {
            playMenuSound(1);
            i = 0;
            if (D_801F7B88.mode == 0) {
                if (D_801F7B88.flags[port][D_801F7B88.slot] == 0) {
                    sprintf(text, "Will write over File %d.\nCreate a New File?", D_801F7B88.slot + 1);
                    i = 1;
                } else if (D_801F80BE < 2) {
                    sprintf(text, "Not enough Free Blocks.\nYou need 2 Blocks to save.");
                } else {
                    sprintf(text, "Creating a \"Digital Card Battle\" File.\nIs this OK?");
                    i = 1;
                }
            } else if ((D_801F7B88.slots[port] + D_801F7B88.slot)->size != 0x2774) {
                sprintf(text, "File %d is corrupted!", D_801F7B88.slot + 1);
            } else {
                sprintf(text, "Do you want to Load File%d?", D_801F80C3 + 1);
                i = 1;
            }
            initDialog((u8 *)&D_801F80D0, text, i);
            runDialogForPad((s32 *)&D_801F80D0, port);
            if (i == 1 && D_801F80D0.choice == 1) {
                ((SessionView *)D_8006E054)->saves[port].file = D_801F7B88.slot;
                if (D_801F7B88.mode == 0) {
                    D_801F7B88.unk535 = 4;
                } else {
                    D_801F7B88.unk535 = 9;
                    if ((D_801F7B88.slots[port] + D_801F7B88.slot)->unk56 < 6) {
                        changeScrollingBackground((D_801F7B88.slots[port] + D_801F7B88.slot)->unk56, 0x380, 0, 0x380, 0x80);
                    }
                }
                func_801ECBE8();
            }
        }
    } else if (PAD_STATES[port]->pressed & 0x10) {
        playMenuSound(0);
        if (D_801F7B88.mode == 0) {
            D_801F7B88.unk534 = 10;
        } else {
            D_801F80BC = 6;
        }
        initDialog((u8 *)&D_801F80D0, NULL, 1);
        runDialogForPad((s32 *)&D_801F80D0, port);
        switch (D_801F80D0.choice) {
        case 1:
            D_801F7B88.ready = 1;
            func_801ECBE8();
            D_801F7B88.unk535 = 0x1A;
            ((SessionData *)D_8006E054)->unk1027 = 1;
            break;
        case 0:
        case 2:
            ((SessionData *)D_8006E054)->unk1027 = 0;
            if (D_801F7B88.mode == 0) {
                D_801F7B88.unk534 = 0x11;
            } else {
                D_801F80BC = 0x10;
            }
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (D_801F7B88.slot == i) {
            D_801F7B88.slotWindows[i].message = 0x1B;
            D_801F7B88.slotWindows[i].window.brightness = 0x80;
        } else {
            D_801F7B88.slotWindows[i].message = 0x1C;
            D_801F7B88.slotWindows[i].window.brightness = 0x40;
        }
    }
}

/* the save file names of the table at D_801F07FC (in .data) */
const char D_801DF938[] = "BASLUS-01328_C";
const char D_801DF948[] = "BASLUS-01328_B";
const char D_801DF958[] = "BASLUS-01328_A";

u8 *func_801EE998(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;
    s32 minus;

    minus = 0;
    if (width < 0) {
        if (value < 0) {
            *dst++ = 0x81;
            *dst++ = 0x7C;
        } else {
            *dst++ = 0x81;
            *dst++ = 0x7B;
        }
    } else if (value < 0) {
        minus = 1;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x81;
        *dst++ = 0x40;
    }
    *dst = 0;
    if (minus) {
        width--;
    }
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    if (minus) {
        *--dst = 0x7C;
        *--dst = 0x81;
    }
    return dst;
}

u8 *func_801EEAB4(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;

    if (value < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7C;
    } else if (width < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7B;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x82;
        *dst++ = 0x4F;
    }
    *dst = 0;
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    return dst;
}

extern u8 D_801F8184;
void func_80055730(void);
void func_801EF4AC();
void func_801EF568(s32 port);
s32 func_801EF774(s32 port, s32 slot);
s32 func_801EF880(s32 port);
s32 func_801EF8A4(s32 port);
s32 func_801EF900(s32 part, s32 port);
s32 func_801EF9E8(s32 unused, s32 port);
s16 func_801EFA90(s32 port);
void func_801EFAC0(s32 port, s32 slot);
u8 func_801EFE04(s32 player, s32 port, s32 slot);

void func_801EEB9C(void) {
    s32 player;
    s32 port;
    s32 slot;
    s32 status;
    s32 result;
    s32 i;
    s32 j;

    D_801F80BC = -1;
    do {
        func_80014C08(FRAME_INTERVAL);
        player = D_801F7B88.card;
        port = ((SessionView *)D_8006E054)->saves[player].slot;
        slot = ((SessionView *)D_8006E054)->saves[player].file;
        status = 0;
        switch (D_801F7B88.unk535) {
        case 1:
            if (D_801F80BF == 7 && port == 0) {
                for (slot = 0; slot < 2; slot++) {
                    status = func_801EF8A4(slot);
                    if (status != 0) {
                        break;
                    }
                }
            } else {
                status = func_801EF8A4(port);
            }
            D_801F7B88.unk540 = 0;
            if (status == 1) {
                if (D_801F7B88.mode == 0) {
                    D_801F7B88.unk534 = 4;
                    D_801F7B88.unk535 = 2;
                } else {
                    D_801F7B88.unk534 = 12;
                    D_801F7B88.unk535 = 29;
                }
            } else if (status == 2) {
                if (D_801F7B88.mode == 0) {
                    D_801F7B88.unk535 = 7;
                } else {
                    D_801F7B88.unk534 = 3;
                    D_801F7B88.unk535 = 11;
                }
            } else {
                if (D_801F80BF == 7 && port == 0) {
                    for (slot = 0; slot < 2; slot++) {
                        scanMemoryCardFiles(slot);
                        for (i = 0; i < 3; i++) {
                            func_801EF774(slot, i);
                        }
                        status = (s8)(D_801F7B88.flags[slot][0] & D_801F7B88.flags[slot][1] & D_801F7B88.flags[slot][2]);
                        if (status == 1) {
                            D_801F7B88.unk53D = slot;
                            status = func_801EF8A4(slot);
                            if (status == 1) {
                                D_801F7B88.unk534 = 12;
                                D_801F7B88.unk535 = 29;
                            } else if (status == 2) {
                                D_801F7B88.unk534 = 3;
                                D_801F7B88.unk535 = 11;
                            } else {
                                D_801F7B88.unk535 = 28;
                            }
                            break;
                        }
                    }
                    if (D_801F80BD != 1) {
                        break;
                    }
                } else {
                    scanMemoryCardFiles(port);
                    for (i = 0; i < 3; i++) {
                        func_801EF774(port, i);
                    }
                }
                D_801F7B88.unk536 = func_801EFA90(port);
                switch (D_801F7B88.mode) {
                case 0:
                    status = D_801F80BE;
                    for (i = 0; i < 3; i++) {
                        if (D_801F7B88.flags[port][i] == 0) {
                            status += 2;
                        }
                    }
                    if (status < 2) {
                        D_801F80BD = 6;
                    } else {
                        func_801ECA38();
                        D_801F80BD = 3;
                    }
                    break;
                case 7:
                case 0xFF:
                    status = (s8)(D_801F7B88.flags[port][0] & D_801F7B88.flags[port][1] & D_801F7B88.flags[port][2]);
                    if (status == 1) {
                        D_801F7B88.unk53D = port;
                        D_801F7B88.unk535 = 28;
                    } else {
                        func_801ECA38();
                        D_801F80BD = 10;
                    }
                    break;
                }
            }
            break;
        case 17:
            D_801F80BC = -1;
            break;
        case 23:
            if (D_801F80BF == 6 && port == 0) {
                for (slot = 0; slot < 2; slot++) {
                    status = func_801EFE04(slot, slot, ((SessionView *)D_8006E054)->saves[slot].file);
                    if (status != 3) {
                        break;
                    }
                }
            } else {
                status = func_801EFE04(player, port, slot);
            }
            switch (status) {
            case 0:
            case 2:
                D_801F7B88.unk534 = 13;
                D_801F7B88.unk535 = 19;
                break;
            case 1:
                D_801F7B88.unk534 = 12;
                D_801F7B88.unk535 = 19;
                break;
            case 3:
                D_801F7B88.unk534 = -1;
                D_801F7B88.unk535 = 24;
                break;
            }
            break;
        case 24:
            status = func_801EF880(port);
            if (status == 1) {
                D_801F7B88.unk534 = 12;
                D_801F7B88.unk535 = 19;
                D_801F8184 = status;
            }
            break;
        case 4:
            func_801EF568(player);
            func_801EFAC0(player, slot);
            writeSaveChecksum(0x2774, D_801F7B88.buffer);
            if (startMemoryCardSave(port, 2, (s32)D_801F7B88.buffer, D_801F07FC[slot], (McHeader *)MEMORY_CARD_SAVE_HEADER) == -1) {
                D_801F7B88.unk535 = 5;
            } else if (func_801EF900(player, port) == -1) {
                D_801F80BD = 5;
            } else {
                D_801F7B88.unk534 = 21;
                D_801F7B88.unk535 = 25;
            }
            break;
        case 18:
            func_801EF568(player);
            func_801EFAC0(player, slot);
            writeSaveChecksum(0x2774, D_801F7B88.buffer);
            if (startMemoryCardSave(port, 2, (s32)D_801F7B88.buffer, D_801F07FC[slot], (McHeader *)MEMORY_CARD_SAVE_HEADER) == -1) {
                D_801F7B88.unk535 = 20;
            } else if (func_801EF900(player, port) == -1) {
                D_801F80BD = 20;
            } else {
                D_801F7B88.unk539 = 0;
                if (D_801F7B88.mode != 6 || player != 0) {
                    playMenuSound(1);
                    D_801F7B88.unk534 = 22;
                    D_801F7B88.unk533 = 1;
                    D_801F7B88.unk535 = 26;
                } else {
                    D_801F7B88.unk533 = 3;
                    D_801F7B88.unk535 = 27;
                }
            }
            break;
        case 3:
        case 10:
            result = func_801EF880(port);
            if (result == 1) {
                D_801F8184 = result;
                D_801F80BD = result;
                func_801ECBE8();
            }
            break;
        case 7:
            result = func_801EF880(port);
            if (result == 1) {
                D_801F8184 = result;
                D_801F80BD = result;
            }
            break;
        case 8:
            formatMemoryCard(port);
            D_801F80BD = 1;
            break;
        case 9:
            if (startMemoryCardLoad(port, (s32)D_801F7B88.buffer, D_801F07FC[slot]) == -1) {
                D_801F7B88.unk535 = 13;
                break;
            }
            D_801F7B88.unk533 = 0;
            if (func_801EF9E8(player, port) == -1) {
                D_801F7B88.unk535 = 13;
                break;
            }
            if (verifySaveChecksum(((SaveSlot *)D_801F7B88.buffer)->size, D_801F7B88.buffer) == 1) {
                D_801F7B88.progress = 0;
                D_801F7B88.unk535 = 15;
            } else {
                D_801F7B88.unk534 = 20;
                D_801F7B88.unk535 = 25;
                if (player == 0) {
                    if ((((PlayerProfile *)D_801F7B88.buffer)->unk20_0) == 1) {
                        func_80055730();
                    } else {
                        func_80055740();
                    }
                }
            }
            break;
        case 25:
            D_801F7B88.unk533 = 1;
            playMenuSound(1);
            func_801EF4AC(player, port, slot);
            D_801F7B88.unk535 = 12;
            break;
        case 20:
            D_801F8184 = 1;
            D_801F7B88.unk534 = 15;
            D_801F7B88.unk535 = 21;
            break;
        case 13:
            D_801F8184 = 1;
            D_801F7B88.unk534 = 19;
            D_801F7B88.unk535 = 14;
            break;
        case 29:
            break;
        }
    } while (D_801F80C2 != 1);
    func_80014C08(FRAME_INTERVAL);
}

void func_801EF4AC(s32 port, s32 slot, s32 file) {
    PlayerProfile *src;

    switch (D_801F80BF) {
    case 0:
        break;
    case 7:
    case 0xFF:
        src = D_801F80B0;
        ((PlayerProfile *)PLAYER_PROFILES)[port] = *src;
        ((SessionView *)D_8006E054)->saves[port].playTime = src->playTime;
        break;
    }
}

extern u8 D_801F80C0;

void func_801EF568(s32 port) {
    PlayerProfile *buffer;

    buffer = D_801F7B88.buffer;
    if (D_801F7B88.mode == 6) {
        D_801F7B88.progress = port * 50;
    } else {
        D_801F80C7 = 0;
    }
    switch (D_801F80BF) {
    case 0:
        PLAYER_DATA(port).unkF = 1;
        PLAYER_DATA(port).unk28_13 = D_801F80C0;
        ((SessionView *)D_8006E054)->saves[port].playTime = PLAYER_DATA(port).playTime;
        *buffer = PLAYER_DATA(port);
        return;
    case 2:
    case 4:
    case 5:
    case 8:
        if (PLAYER_DATA(port).unkD < 255) {
            PLAYER_DATA(port).unkD++;
        }
    case 6:
        ((SessionView *)D_8006E054)->saves[port].playTime = PLAYER_DATA(port).playTime;
        *buffer = PLAYER_DATA(port);
        break;
    }
}

extern s32 D_801F07FC[3];

s32 func_801EF774(s32 port, s32 slot) {
    s16 i;

    D_801F7B88.flags[port][slot] = 1;
    for (i = 0; i < 5; i++) {
        if ((D_801F7B88.flags[port][slot] = readMemoryCardSavePreview(port, &D_801F7B88.slots[port][slot], D_801F07FC[slot])) == 0) {
            D_801F7B88.unk540++;
            return 0;
        }
    }
    return 1;
}

s32 func_801EF880(s32 port) {
    D_801F80C5 = port;
    return ensureMemoryCardReady(port);
}

s32 func_801EF8A4(s32 port) {
    switch (D_801F7B88.mode) {
    case 6:
    case 7:
        D_801F7B88.unk534 = 2;
        break;
    default:
        D_801F80BC = 0;
        break;
    }
    D_801F80C5 = port;
    return getMemoryCardStatus(port);
}

s32 func_801EF900(s32 part, s32 port) {
    s32 result;

    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (func_801EF880(port) != 0) {
            D_801F7B88.progress = 0;
            return -1;
        }
        result = stepMemoryCardSave();
        if (result == -1) {
            D_801F7B88.progress = 0;
            return -1;
        }
        if (D_801F7B88.mode == 6) {
            D_801F7B88.progress = part * 50 + result / 2;
        } else {
            D_801F7B88.progress = result;
        }
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            return 0;
        }
    }
}

s32 func_801EF9E8(s32 unused, s32 port) {
    s32 result;

    while (1) {
        func_80014C08(FRAME_INTERVAL);
        if (func_801EF880(port) != 0) {
            D_801F7B88.progress = 0;
            return -1;
        }
        result = stepMemoryCardLoad();
        if (result == -1) {
            D_801F7B88.progress = 0;
            return -1;
        }
        D_801F7B88.progress = result;
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            return 0;
        }
    }
}

s16 func_801EFA90(s32 port) {
    return 15 - MEMORY_CARD_DIRECTORIES[port]->blocks;
}

typedef struct {
    u8 data[0x20];
} IconClut;
typedef struct {
    u8 data[0x180];
} IconImage;
typedef struct {
    /* 0x00 */ char magic[2];
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 blocks;
    /* 0x04 */ char title[64];
    /* 0x44 */ u8 reserve[28];
    /* 0x60 */ IconClut clut;
    /* 0x80 */ IconImage icon;
} SaveHeader;
void StoreImage(Rect16 *rect, void *p);

void func_801EFAC0(s32 port, s32 slot) {
    SaveHeader *header = (SaveHeader *)MEMORY_CARD_SAVE_HEADER;
    IconImage icon;
    IconClut clut;
    s16 iconX[3] = { 0x158, 0x15C, 0x160 };
    Rect16 iconRect;
    Rect16 clutRect;
    char title[72];
    u8 file[16];
    u8 hours[16];
    u8 minutes[16];
    s32 time;
    s32 hour;
    s32 minute;
    s32 i;

    iconRect.x = iconX[slot];
    iconRect.y = 0x154;
    iconRect.w = 4;
    iconRect.h = 0x30;
    StoreImage(&iconRect, &icon);
    clutRect.x = 0x170;
    clutRect.y = slot + 0x1FA;
    clutRect.w = 0x10;
    clutRect.h = 1;
    StoreImage(&clutRect, &clut);
    DrawSync(0);
    bzero((Scene3D *)title, 0x42);
    time = ((SessionView *)D_8006E054)->saves[port].playTime;
    hour = time / 216000;
    minute = (time - hour * 216000) / 3600;
    if (hour >= 1000) {
        hour = 999;
        minute = 59;
    }
    func_801EE998(slot + 1, 1, file);
    func_801EE998(hour, 3, hours);
    func_801EEAB4(minute, 2, minutes);
    /* "ＤＣＢ［%s］%s：%s" */
    sprintf(title, "\x82" "c" "\x82" "b" "\x82" "a" "\x81" "m%s" "\x81" "n%s" "\x81" "F%s", file, hours, minutes);
    header->magic[0] = 'S';
    header->magic[1] = 'C';
    header->type = 0x13;
    header->blocks = 2;
    for (i = 0; i < 64; i++) {
        header->title[i] = 0;
    }
    strcpy(header->title, title);
    for (i = 0; i < 28; i++) {
        header->reserve[i] = 0;
    }
    header->clut = clut;
    header->icon = icon;
}

u8 func_801EFE04(s32 player, s32 port, s32 slot) {
    SaveSlot *save;
    s32 status;

    save = &D_801F7B88.slots[port][slot];
    status = func_801EF8A4(port);
    if (status == 0) {
        scanMemoryCardFiles(port);
        func_801EF774(port, slot);
        if (D_801F7B88.flags[port][slot] == 0) {
            if ((u16)((PlayerProfile *)PLAYER_PROFILES)[player].unk10 != save->unk10) {
                return 0;
            }
            return 3;
        }
        D_801F80C5 = port;
    }
    return status;
}
