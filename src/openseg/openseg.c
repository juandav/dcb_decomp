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
extern void *D_801F0CA0[2][301];
void func_801E2068(s32 player);
extern s32 D_801F4F08;
extern s32 D_801F4F0C;
extern s32 D_801F4F10;
extern s32 D_801F4F14;
extern s32 D_801F4F18;
extern s32 D_801F4F1C;
extern s32 D_801F4F20;
void func_801EAA54(UiWindow *window);
void func_801E4C18(s32 x, s32 y, s32 part);

extern s32 D_801F1AFC;
extern s32 D_801F1B00;
u16 func_801E195C(s32 player, s32 card);
extern s8 D_801F1880[2][301];
typedef struct {
    u8 unk0[9];
    char name[13];
    u8 cursor;
} NameEntry;
extern NameEntry D_801F50D8;
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
extern u8 *D_801F0840;
extern u8 *D_801F0844;
extern u16 D_801EFF2C;
extern s32 D_801F0848;
extern u16 *D_801F084C;
extern u32 *D_801F085C;
extern u32 D_801F0870;
extern u8 D_801F0868;
void func_801DFD4C();
void func_801E04CC(u8 *loc);
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
typedef struct {
    s16 id;
    s8 unk2;
} CardEntry;
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
typedef struct {
    UiWindow window;
    u8 unk44[4];
} Unk801F52E0;
extern Unk801F52E0 D_801F52E0[2];
typedef struct {
    u8 unk0[0x528];
    void *unk528;
    u8 pad52C[0x532 - 0x52C];
    u8 unk532;
    u8 unk533;
    s8 unk534;
    u8 pad535[0x539 - 0x535];
    u8 unk539;
} Unk801F7B88;
extern Unk801F7B88 D_801F7B88;
void func_801EBAFC();
extern u8 D_801F80C1;
extern u8 D_801F80BF;
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
    u8 unk0[0x1010];
    SaveInfo saves[2];
} SessionView;
extern u8 D_801F80C2;
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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801DFBE0);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801DFD4C);

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

void func_801DFFD0(u8 *loc, void (*callback)()) {
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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E0164);

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

void func_801E03FC(DecEnv *env) {
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

void func_801E04CC(u8 *loc) {
    u8 param[4];

    param[0] = 0x80;
    do {
        while (CdControlB(2, loc, 0) == 0) {
            func_8005A364(0, 0);
        }
        while (CdControlB(0xE, param, 0) == 0) {
            func_8005A364(0, 0);
        }
        VSync(3);
    } while (CdRead2(0x1E0) == 0);
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E055C);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E078C);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E0A08);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E0B38);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E0C6C);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E0DA0);

s32 func_801E0ED4(CardEntry **a, CardEntry **b) {
    s32 keyA;
    s32 keyB;

    keyA = (*a)->unk2 == 1;
    keyB = (*b)->unk2 == 1;
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

    keyA = (*a)->unk2 == 2;
    keyB = (*b)->unk2 == 2;
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*a)->id] & 0x40)) {
        keyA = -1;
    }
    if (!(((PlayerProfile *)PLAYER_PROFILES)[D_801F1B08].cardCollection[(*b)->id] & 0x40)) {
        keyB = -1;
    }
    return keyB - keyA;
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E10A4);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E11D4);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E1308);

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
        D_801F0CA0[player][n++] = &((DigimonCardData *)DIGIMON_CARDS)[i];
    }
    for (i = 0; i < 0x66; i++) {
        D_801F0CA0[player][n++] = &((OptionCardData *)OPTION_CARDS)[i];
    }
    for (i = 0; i < 8; i++) {
        D_801F0CA0[player][n++] = &((DigivolveCardData *)DIGIVOLVE_CARDS)[i];
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[i].cardId != 0) {
            D_801F0CA0[player][PLAYER_DATA(player).partners[i].cardId] = &PLAYER_DATA(player).partners[i];
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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E23C4);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E2B34);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DDF38);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DDF3C);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DDFD0);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DDFF4);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E3024);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE044);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE068);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E3214);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE0A4);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE0BC);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E3CA8);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E3FCC);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE0E8);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DE0F0);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E4170);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E4C18);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E54EC);

void func_801E5C30(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;

    x = window->originX;
    y = window->originY;
    z = window->z;
    drawText(x, y, PLAYER_PROFILES, 7, z);
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E5C6C);

void func_801E5E7C(UiWindow *window) {
    func_801E54EC(window->originX, window->originY, window->z);
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E5EAC);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E6FB8);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E7638);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E7EC4);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DEAC8);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E826C);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E87E0);

void func_801E8E6C(void) {
    drawWindow(&D_801F50F8, func_801E87E0, 1);
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E8E9C);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E9244);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801E9938);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EA2F8);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EAA54);

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

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DEC34);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EB0D0);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EB2E8);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EBAFC);

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

void func_801EBEDC(void) {
    D_801F7B88.unk533 = 0;
    D_801F7B88.unk534 = -1;
    D_801F7B88.unk532 = 0;
    D_801F7B88.unk528 = allocHeapBlock(0x4000, 0x63);
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EBF20);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EC37C);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EC728);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EC994);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801ECA38);

void func_801ECBE8(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        animateWindowTo(&D_801F7C88[i].window, (Rect16 *)-1);
    }
    animateWindowTo(&D_801F7D60, (Rect16 *)-1);
    animateWindowTo(&D_801F53C8, (Rect16 *)-1);
    playMenuSound(4);
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801ECC64);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801ECDC0);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DF6F0);

void func_801ECEAC(UiWindow *window) {
    char text[40];
    s32 x;

    sprintf(text, "Player %d : Slot %d", D_801F80C4 + 1, ((SessionData *)D_8006E054)->unk1010[D_801F80C4 * 8] + 1);
    x = (0x84 - strlen(text) * 6) / 2;
    drawText(window->originX + x, window->originY + 1, (s32)text, 7, window->z);
}

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DF70C);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801ECF48);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801ED3B8);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801ED780);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DF78C);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EDC08);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EE3DC);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EE488);

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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EEB9C);

void func_801EF4AC(s32 port) {
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

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EF568);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EF774);

s32 func_801EF880(s32 port) {
    D_801F80C5 = port;
    return ensureMemoryCardReady(port);
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EF8A4);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EF900);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EF9E8);

s16 func_801EFA90(s32 port) {
    return 15 - MEMORY_CARD_DIRECTORIES[port]->blocks;
}

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EFAC0);

INCLUDE_ASM("asm/openseg/nonmatchings/openseg", func_801EFE04);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DFA04);

INCLUDE_RODATA("asm/openseg/nonmatchings/openseg", D_801DFA0C);
