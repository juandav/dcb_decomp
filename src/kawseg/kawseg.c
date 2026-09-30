#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/loader.h"
#include "dcb/script.h"
#include "dcb/text.h"
#include "dcb/duel_util.h"
#include "dcb/window.h"
#include "dcb/duel_launch.h"
#include "dcb/battle_hud.h"
#include "dcb/transform.h"
#include "dcb/effect_object.h"
#include "dcb/decompress.h"
#include "dcb/scroll_bg.h"
#include "dcb/card_db.h"
#include "dcb/vram_upload.h"
#include "dcb/card_render.h"
#include "dcb/menu.h"
#include "dcb/sound_play.h"
#include "dcb/prim_util.h"
#include "dcb/frame_callback.h"
#include "dcb/partner_level.h"
#include "dcb/vblank.h"
#include "dcb/duel.h"
#include "dcb/dialog.h"
#include "dcb/libmath.h"
#include "dcb/duel_setup.h"
#include "dcb/sound.h"

typedef struct {
     s16 id;
     u8 attrCount;
     u8 levelCount;
     u8 dpCost;
     u8 deckCount;
     u8 supports;
     u8 pad7;
} Candidate;

s32 rand(void);

typedef struct {
    UiWindow window;
    u16 partner;
    u16 clut;
} ExpWindow;
typedef struct {
    UiWindow window;
    s32 rank;
} RankUpWindow;
typedef struct {
    /* 0x000 */ UiWindow window;
    /* 0x044 */ UiWindow titleWindow;
    /* 0x088 */ UiWindow unk88;
    /* 0x0CC */ ExpWindow expWindows[3];
    /* 0x1A4 */ RankUpWindow rankWindows[3];
    /* 0x27C */ s32 progress;
    /* 0x280 */ s32 done;
    /* 0x284 */ s32 speed;
    /* 0x288 */ u16 gains[3][4];
    /* 0x2A0 */ u16 pendingExp[3];
    /* 0x2A6 */ u8 partFlags[16];
    /* 0x2B6 */ u8 partnerShown[3];
} ExpScreen;
typedef struct {
     char *name;
     s32 unk4;
     s32 unk8;
} PartInfo;
extern ExpScreen *D_801FC738;
extern PartInfo D_801FBB38[];

typedef struct {
     s8 active;
     s16 lhs[6];
     s16 rhs[6];
     s8 cmp;
     s8 ops[4];
} SupportCond;
typedef struct {
     s8 active;
     s8 kind;
     s16 lhs[3];
     s16 rhs[3];
     s8 ops[2];
} SupportEffect;
s32 func_801E7DD4(s32 arg0, s32 arg1, s32 lhs, s32 rhs, s32 slot);
s32 func_801E81DC(s32 arg0, s32 arg1, s32 kind, s32 value, s32 slot, s32 arg5);
extern s32 D_801FC458;
extern s32 D_801FCA28;
extern s32 D_801FCA2C;
extern u8 *D_801D485C;
extern s32 D_801FBA30[2][4];
extern s32 D_801FBA50[2][4];
extern s32 D_801FBA70[2][4];
extern s32 D_801FBA90[2][4];

typedef struct {
    u32 f0 : 1;
    u32 f1 : 1;
    u32 f2 : 1;
    u32 f3 : 1;
    u32 f4 : 1;
    u32 f5 : 1;
    u32 f6 : 1;
    u32 f7 : 1;
    u32 f8 : 1;
    u32 f9 : 1;
    u32 f10 : 1;
    u32 f11 : 1;
    u32 count : 2;
    u32 f14 : 1;
    u32 f15 : 1;
    u32 f16 : 1;
    u32 f17 : 1;
    u32 f18 : 1;
    u32 f19 : 2;
    u32 f21 : 1;
    u32 f22 : 1;
    u32 f23 : 1;
    u32 f24 : 1;
    u32 f25 : 1;
    u32 f26 : 1;
    u32 f27 : 1;
    u32 f28 : 1;
    u32 f29 : 1;
    u32 f30 : 1;
    u32 f31 : 1;
} Flags110;

extern char *D_801FC168[];

typedef struct {
    s16 id;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Entry8;
s32 func_801E02D8(s32 player, s32 attr);
s32 func_801E9F5C(s32 card, s32 player);
extern s8 D_801FC87C;
extern s8 D_801FC87D;
extern s8 D_801FC87E;
void func_801F7760(void *data, s32 task);
typedef struct {
    /* 0x00 */ s16 mode;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ CardSprite *sprite;
    /* 0x08 */ s16 offsetX;
    /* 0x0A */ s16 offsetY;
    /* 0x0C */ s16 x;
    /* 0x0E */ s16 y;
    /* 0x10 */ s32 index;
    /* 0x14 */ u8 rgb[3];
    /* 0x17 */ u8 unk17;
    /* 0x18 */ s16 cur[64];
    /* 0x98 */ s16 points[64];
} Shape;

s32 func_801E0558(s32 id, s32 player, s32 card);
extern s16 D_801FB9D8[];
s32 func_801E05CC(s32 id, s32 player);
extern s16 D_801FB9A8[];
extern s16 D_801FBA28[];
extern s16 D_801FB9E4[];
typedef struct {
    /* 0x0 */ s8 kind;
    /* 0x1 */ u8 need;
    /* 0x2 */ s16 unk2;
    /* 0x4 */ s16 id;
} Slot7C8;
typedef struct {
    /* 0x000 */ u8 unk0[0x7C4];
    /* 0x7C4 */ Slot7C8 *selected;
    /* 0x7C8 */ Slot7C8 slots[4];
    /* 0x7E0 */ u8 unk7E0[0x3F];
    /* 0x81F */ s8 tutorial;
    /* 0x820 */ s8 unk820;
    /* 0x821 */ s8 menuPlayer;
    /* 0x822 */ s8 awaitingInput;
    /* 0x823 */ s8 menuOpen;
    /* 0x824 */ s8 quit;
    /* 0x825 */ u8 unk825[0x1B];
    /* 0x840 */ u8 bonusFlags[32];
    /* 0x860 */ s16 rewardCluts[3];
    /* 0x866 */ s16 partnerCluts[3];
} DuelK;
typedef struct {
     void *data;
     Script *script;
     s32 *regs;
     s32 unkC;
     s32 unk10;
} ScriptRunner;
s32 func_801EAB4C();
s32 func_801EC9A4(s32 player, s32 slot);
s32 func_801ECA30(s32 card, s32 player, s32 slot);
void func_801ED65C(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 arg8, s32 arg9, s32 argA, s32 brightness, s32 z);
typedef struct {
    /* 0x00 */ u8 r0;
    /* 0x01 */ u8 g0;
    /* 0x02 */ u8 b0;
    /* 0x03 */ u8 code;
    /* 0x04 */ u16 clut;
    /* 0x06 */ u16 tpage;
    /* 0x08 */ u8 u;
    /* 0x09 */ u8 v;
    /* 0x0A */ u8 unkA[2];
    /* 0x0C */ VECTOR pos;
    /* 0x1C */ SVECTOR rot;
} Icon3D;
typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
} PolyF4;
typedef struct {
    /* 0x00 */ PolyF4 bars[2];
    /* 0x30 */ DR_MODE barMode;
    /* 0x38 */ PolyF4 fade;
    /* 0x50 */ DR_MODE fadeMode;
    /* 0x58 */ POLY_FT4 logo;
    /* 0x80 */ POLY_FT4 intro;
    /* 0xA8 */ RawPolyFT4 cards[2];
} VersusPrims;
typedef struct {
    UiWindow window;
    s32 player;
} ListWindow;
typedef struct {
    /* 0x000 */ s16 *cursor;
    /* 0x004 */ ListWindow lists[2];
    /* 0x094 */ CursorHighlight highlights[2];
    /* 0x134 */ ListWindow frames[2];
    /* 0x1C4 */ u8 dialog[0xB8];
    /* 0x27C */ u16 deckIds[2][0xA2];
    /* 0x504 */ s32 unk504[2];
    /* 0x50C */ s32 introState;
    /* 0x510 */ s32 unk510;
    /* 0x514 */ s32 introZoom;
    /* 0x518 */ s32 introBrightness;
    /* 0x51C */ s32 logoShown;
    /* 0x520 */ s32 logoScale;
    /* 0x524 */ s32 pulse;
    /* 0x528 */ VersusPrims prims[2];
    /* 0x718 */ Icon3D cards[2];
    /* 0x760 */ s16 wins[2];
    /* 0x764 */ s16 losses[2];
    /* 0x768 */ s32 choice;
    /* 0x76C */ s16 barW;
    /* 0x76E */ s16 barH;
    /* 0x770 */ s16 unk770;
    /* 0x772 */ s16 deckId;
    /* 0x774 */ s16 chosen;
    /* 0x776 */ s16 timer;
} DeckScreen;
extern DeckScreen *D_801FC454;
extern Menu D_801FBAB0[];
void func_801EDD88(ListWindow *w);
void func_801EFB78(ListWindow *w);
typedef struct {
    UiWindow window;
    s32 index;
} RewardWindow;
typedef struct {
    UiWindow window;
    u16 cardId;
    u16 index;
    u16 clut;
} PrizeWindow;
typedef struct {
     UiWindow window;
     PrizeWindow prizes[3];
     RewardWindow rewards[3];
     s32 showRewards;
} PrizeScreen;
extern PrizeScreen *D_801FC73C;
void func_801F5C14(UiWindow *window);
void func_801F5B94(RewardWindow *w);
void func_801F54BC(PrizeWindow *w);
void func_801F6294(s32 entry, s32 player1, s32 player2, s32 mode1, s32 mode2);
typedef struct {
     s16 kind;
     s16 active;
     u8 *obj;
} EffectTableEntry;
typedef struct {
     void *data;
     Script *script;
     s32 *regs;
     EffectTableEntry entries[16];
     s32 count;
} EffectTable;
extern void (*D_801FC158[])(u8 *);
void func_801F893C(CardSprite *sprite, u8 *to);
/* SUGSEG's colour quad drawer: in KAWSEG this address is inside func_801E5710 */
void func_801E6424(Rect16 *rect, u8 *rgb, u8 *rgb2, u8 arg3, s32 arg4, u8 arg5);
void func_801F7128(EffectTemplate *template, u8 *fx, EffectTable *table);
extern u8 *(*D_801FC148[])(s32, EffectTable *);
extern UiWindow D_801FC884;
void func_801F7A64(UiWindow *window);
typedef struct {
    u8 data[0x14F0];
} Unk14F0;
extern Unk14F0 *D_801D83F8;
typedef struct {
    /* 0x00 */ DR_MODE dm;
    /* 0x08 */ POLY_G4 prims[8];
} GradPacket;
extern GradPacket *D_801D83F4;
void func_801F8E14(void *arg0);
void func_801F8E34(void *arg0, s32 arg1);
typedef struct {
    /* 0x000 */ DR_MODE dm;
    /* 0x008 */ PolyF4 edges[32];
    /* 0x308 */ POLY_G4 fades[32];
} RingPrims;
typedef struct {
    /* 0x000 */ u8 unk0[0x828];
    /* 0x828 */ s32 mode;
    /* 0x82C */ s32 cx;
    /* 0x830 */ s32 cy;
    /* 0x834 */ s32 radius;
    /* 0x838 */ s32 width;
} DuelRing;
#define RING ((DuelRing *)D_801D8340)
#define setRGB1(p, _r1, _g1, _b1) (p)->r1 = _r1, (p)->g1 = _g1, (p)->b1 = _b1
#define setRGB2(p, _r2, _g2, _b2) (p)->r2 = _r2, (p)->g2 = _g2, (p)->b2 = _b2
#define setRGB3(p, _r3, _g3, _b3) (p)->r3 = _r3, (p)->g3 = _g3, (p)->b3 = _b3
typedef struct {
    /* 0x0000 */ u8 unk0[0x848];
    /* 0x0848 */ u16 counts[32];
    /* 0x0888 */ u8 unk888[0xD3C - 0x888];
    /* 0x0D3C */ u16 bestDamage[0xBF][3];
    /* 0x11B6 */ u8 unk11B6[0x2774 - 0x11B6];
} ProfileK;
typedef struct {
    u32 usedAttack : 2;
    u32 attackChoice : 2;
    u32 unk4 : 2;
    u32 f6 : 1;
    u32 f7 : 1;
    u32 f8 : 1;
    u32 f9 : 2;
    u32 f11 : 1;
    u32 f12 : 1;
    u32 f13 : 1;
    u32 f14 : 1;
    u32 unk15 : 16;
    u32 f31 : 1;
} Flags178;
typedef struct {
    /* 0x000 */ u8 unk0[0x110];
    /* 0x110 */ s32 unk110;
    /* 0x114 */ u8 unk114[8];
    /* 0x11C */ s16 stats[5];
    /* 0x126 */ u8 unk126[0x32];
    /* 0x158 */ s16 hpBeforeBattle;
    /* 0x15A */ s16 hpAfterBattle;
    /* 0x15C */ s16 baseAttackPowers[3];
    /* 0x162 */ s16 damageTaken;
    /* 0x164 */ s16 hpGain;
    /* 0x166 */ u8 unk166[2];
    /* 0x168 */ s16 attackDamage[3];
    /* 0x16E */ u8 unk16E[10];
    /* 0x178 */ Flags178 flags;
    /* 0x17C */ u8 wins;
} PlayerStats;

#define FLAGS178(p) ((Flags178 *)((u8 *)PLAYER(p) + 0x178))
#define STATS(p) ((PlayerStats *)PLAYER(p))

s32 func_801DFE84(s32 player) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    i = 0;
    p = (Player *)DUEL_PLAYERS[player];
    for (; i < 30; i++) {
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            count++;
        }
    }
    return count;
}

s32 func_801DFF2C(s32 player) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    i = 0;
    p = (Player *)DUEL_PLAYERS[player];
    for (; i < 4; i++) {
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            count++;
        }
    }
    return count;
}

s32 func_801DFFD4(s32 player) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    i = 0;
    p = (Player *)DUEL_PLAYERS[player];
    for (; i < 4; i++) {
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 2) {
            count++;
        }
    }
    return count;
}

s32 func_801E0080(s32 player, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 30; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0 && (p->cards[card % 30].card[0x1A] & 0xF) == level) {
            count++;
        }
    }
    return count;
}

s32 func_801E0148(s32 player, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0 && (p->cards[card % 30].card[0x1A] & 0xF) == level) {
            count++;
        }
    }
    return count;
}

s32 func_801E0210(s32 player, s32 attr) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 30; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0 && ((u8)p->cards[card % 30].card[0x1A] >> 4) == attr) {
            count++;
        }
    }
    return count;
}

s32 func_801E02D8(s32 player, s32 attr) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;

    count = 0;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0 && ((u8)p->cards[card % 30].card[0x1A] >> 4) == attr) {
            count++;
        }
    }
    return count;
}

s32 func_801E03A0(s32 player, s32 attr, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;
    u8 *data;

    count = 0;
    for (i = 0; i < 30; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->onlineDeck[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            data = (u8 *)p->cards[card % 30].card;
            if ((data[0x1A] >> 4) == attr && (data[0x1A] & 0xF) == level) {
                count++;
            }
        }
    }
    return count;
}

s32 func_801E047C(s32 player, s32 attr, s32 level) {
    s32 count;
    s32 i;
    Player *p;
    s8 card;
    u8 *data;

    count = 0;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            data = (u8 *)p->cards[card % 30].card;
            if ((data[0x1A] >> 4) == attr && (data[0x1A] & 0xF) == level) {
                count++;
            }
        }
    }
    return count;
}

s32 func_801E0558(s32 id, s32 player, s32 card) {
    if (card != -1 && ((Player *)DUEL_PLAYERS[player])->cards[card % 30].id == id) {
        return 1;
    }
    return 0;
}

s32 func_801E05CC(s32 id, s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (func_801E0558(id, player, ((Player *)DUEL_PLAYERS[player])->hand[i])) {
            return i + 1;
        }
    }
    return 0;
}

s32 func_801E0650(s32 id, s32 player) {
    s32 i;
    Player *p;
    s8 card;

    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 2 && p->cards[card % 30].index == id) {
            return 1;
        }
    }
    return 0;
}

s32 func_801E0708(s32 player) {
    s32 count;
    s32 level;
    s32 i;
    Player *p;
    s8 card;
    u8 *data;

    count = 0;
    level = ((u8 *)((Player *)DUEL_PLAYERS[player])->cards[getActiveDigimonCard(player) % 30].card)[0x1A] & 0xF;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && p->cards[card % 30].type == 0) {
            data = (u8 *)p->cards[card % 30].card;
            if ((data[0x1A] & 0xF) == level && *(s16 *)(data + 0x1E) > p->displayedStats[0]) {
                count++;
            }
        }
    }
    return count;
}

s32 func_801E0868(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (func_801E0558(D_801FB9D8[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 func_801E08E4(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 5; i++) {
        found = func_801E05CC(D_801FB9D8[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E094C(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (func_801E0558(D_801FB9A8[i], player, card)) {
            if (D_801FB9A8[i] != 0x97) {
                return 1;
            }
            if (countOfflineDeckCards(player) >= 8) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_801E09FC(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 24; i++) {
        found = func_801E05CC(D_801FB9A8[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E0A64(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 4; i++) {
        found = func_801E05CC(D_801FBA28[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E0ACC(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 33; i++) {
        if (func_801E0558(D_801FB9E4[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 func_801E0B48(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 33; i++) {
        found = func_801E05CC(D_801FB9E4[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 func_801E0BB0(s32 player) {
    Player *p;

    if (getActiveDigimonCard(player) == -1) {
        return 0;
    }
    p = (Player *)DUEL_PLAYERS[player];
    return p->cards[getActiveDigimonCard(player) % 30].card[0xE4];
}

s32 func_801E0C50(s32 player, s32 card) {
    if (card == -1) {
        return -1;
    }
    if (((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[2] == 2) {
        return -1;
    }
    return 0;
}

typedef struct {
    s32 own;
    s32 opponent;
} SimDamage;
typedef struct {
    s8 outcome;
    u8 unk1;
    u8 wins;
    u8 losses;
    SimDamage damage[5][3];
} SimCard;
typedef struct {
    s8 outcome;
    u8 unk1;
    u8 wins;
    u8 losses;
    s32 totalOwn;
    s32 totalOpponent;
    SimCard cards[5];
} AttackSim;
typedef struct {
    s32 words[0x1E4 / 4];
} PlayerSnapshot;
typedef struct {
    u8 unk0[0x5C];
    AttackSim sims[3];
} DuelAi;
#define DUEL_AI ((DuelAi *)D_801D8340)

s32 func_801E0CCC(s32 self) {
    s32 played[2];
    s32 player;
    PlayerSnapshot saved[2];
    s32 opponent;
    s32 i;
    s32 attack;
    s32 card;
    s32 oppCard;
    s32 oppAttack;
    s8 handCard;

    /* a copy of self: it only changes the register allocation */
    player = self;
    opponent = self ^ 1;
    for (i = 0; i < 2; i++) {
        if (getActiveDigimonCard(i) == -1) {
            return -1;
        }
        saved[i] = *(PlayerSnapshot *)DUEL_PLAYERS[i];
    }
    for (attack = 0; attack < 3; attack++) {
        DUEL_AI->sims[attack].totalOwn = 0;
        DUEL_AI->sims[attack].totalOpponent = 0;
        DUEL_AI->sims[attack].outcome = 0;
        DUEL_AI->sims[attack].wins = 0;
        DUEL_AI->sims[attack].losses = 0;
        if (PLAYER(player)->attackChoice != 3 && PLAYER(player)->attackChoice != attack) {
            continue;
        }
        for (card = 0; card < 5; card++) {
            func_80014C08(FRAME_INTERVAL);
            DUEL_AI->sims[attack].cards[card].outcome = -1;
            DUEL_AI->sims[attack].cards[card].wins = 0;
            DUEL_AI->sims[attack].cards[card].losses = 0;
            played[player] = -1;
            if (card == 4) {
                if (PLAYER(player)->playedCard >= 0) {
                    played[player] = PLAYER(player)->playedCard;
                }
            } else {
                played[player] = PLAYER(player)->hand[card];
                if (played[player] < 0 || PLAYER(player)->cards[played[player] % 30].card[2] == 2) {
                    continue;
                }
            }
            for (oppCard = 0; oppCard < 5; oppCard++) {
                played[opponent] = -1;
                if (oppCard == 4) {
                    if (PLAYER(opponent)->playedCard >= 0) {
                        played[opponent] = PLAYER(opponent)->playedCard;
                    }
                } else {
                    played[opponent] = PLAYER(opponent)->hand[oppCard];
                    if (played[opponent] < 0 || PLAYER(opponent)->cards[played[opponent] % 30].card[2] == 2) {
                        continue;
                    }
                }
                for (oppAttack = 0; oppAttack < 3; oppAttack++) {
                    PLAYER(player)->usedAttack = attack;
                    PLAYER(opponent)->usedAttack = oppAttack;
                    PLAYER(player)->playedCard = played[player];
                    PLAYER(opponent)->playedCard = played[opponent];
                    func_801E6AA4(1);
                    DUEL_AI->sims[attack].cards[card].damage[oppCard][oppAttack].own = PLAYER(player)->hpAfterBattle;
                    DUEL_AI->sims[attack].cards[card].damage[oppCard][oppAttack].opponent =
                        PLAYER(opponent)->hpAfterBattle;
                    DUEL_AI->sims[attack].totalOwn += PLAYER(player)->hpAfterBattle;
                    DUEL_AI->sims[attack].totalOpponent += PLAYER(opponent)->hpAfterBattle;
                    if (PLAYER(opponent)->hpAfterBattle <= 0) {
                        DUEL_AI->sims[attack].wins++;
                        DUEL_AI->sims[attack].cards[card].wins++;
                    } else if (PLAYER(player)->hpAfterBattle <= 0) {
                        DUEL_AI->sims[attack].losses++;
                        DUEL_AI->sims[attack].cards[card].losses++;
                        DUEL_AI->sims[attack].outcome = -1;
                        DUEL_AI->sims[attack].cards[card].outcome = -1;
                    } else {
                        if (DUEL_AI->sims[attack].outcome == 0) {
                            DUEL_AI->sims[attack].outcome = 1;
                        }
                        if (DUEL_AI->sims[attack].cards[card].outcome == 0) {
                            DUEL_AI->sims[attack].cards[card].outcome = 1;
                        }
                    }
                    for (i = 0; i < 2; i++) {
                        *(PlayerSnapshot *)DUEL_PLAYERS[i] = saved[i];
                    }
                }
            }
        }
    }
    oppCard = -1;
    for (attack = 0; attack < 3; attack++) {
        for (card = 0; card < 5; card++) {
            if (card == 4 || (PLAYER(player)->hand[card] != -1 &&
                              PLAYER(player)->cards[PLAYER(player)->hand[card] % 30].card[2] != 2)) {
                if (DUEL_AI->sims[attack].cards[card].outcome == 1) {
                    return 0;
                }
                if (DUEL_AI->sims[attack].cards[card].outcome == 2) {
                    oppCard = 1;
                }
            }
        }
    }
    return oppCard;
}

s32 func_801E157C(s32 player) {
    s32 i;
    s32 j;
    s32 card;
    u8 specialty;
    s32 level;
    s32 need;
    s32 other;

    if (getActiveDigimonCard(player) == -1) {
        return -1;
    }
    ((DuelK *)D_801D8340)->selected = NULL;
    for (i = 0; i < 4; i++) {
        ((DuelK *)D_801D8340)->slots[i].unk2 = PLAYER(player)->hand[i];
        ((DuelK *)D_801D8340)->slots[i].kind = -1;
        ((DuelK *)D_801D8340)->slots[i].need = 100;
        ((DuelK *)D_801D8340)->slots[i].id = -1;
        card = PLAYER(player)->hand[i];
        if (card != -1 && PLAYER(player)->cards[card % 30].type == 0) {
            ((DuelK *)D_801D8340)->slots[i].kind = 0;
            specialty = PLAYER(player)->specialty;
            level = (u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF;
            need = PLAYER(player)->cards[card % 30].card[0x1B] - sumDigivolvePoints(player);
            if (need < 0) {
                ((DuelK *)D_801D8340)->slots[i].need = 0;
            } else {
                ((DuelK *)D_801D8340)->slots[i].need = need;
            }
            if ((u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4 == specialty && level != 1) {
                if (level == 0) {
                    level = 1;
                }
                if (((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF) == level + 1) {
                    if (need <= 0) {
                        ((DuelK *)D_801D8340)->slots[i].kind = 1;
                    } else {
                        for (j = 0; j < 4; j++) {
                            if (j == i) {
                                continue;
                            }
                            other = PLAYER(player)->hand[j];
                            if (other != -1 && PLAYER(player)->cards[other % 30].type == 0) {
                                if (PLAYER(player)->cards[other % 30].card[0x1C] >= need) {
                                    ((DuelK *)D_801D8340)->slots[i].kind = 1;
                                    break;
                                }
                                ((DuelK *)D_801D8340)->slots[i].kind = 2;
                            }
                        }
                    }
                }
            }
        }
    }
    j = 0;
    for (i = 0; i < 4; i++) {
        if (((DuelK *)D_801D8340)->slots[i].kind == 1) {
            return 1;
        }
        if (((DuelK *)D_801D8340)->slots[i].kind == 2) {
            j = 2;
        }
    }
    return j;
}

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DDF38);

s32 func_801E19EC(s32 player) {
    s32 self;
    s32 opponent;
    s32 cards;
    s32 i;
    s32 j;
    s32 level;

    self = player;
    opponent = player ^ 1;
    cards = countOnlineDeckCards(player);
    if (cards == 0) {
        return 0;
    }
    if (getActiveDigimonCard(player) == -1) {
        if (func_801DFF2C(player) == 0) {
            return 1;
        }
        if (func_801E0148(player, 0) != 0) {
            return 0;
        }
        for (i = 0; i < 8; i++) {
            if (func_801E0650(i, self) && func_801DFF2C(self) >= 2) {
                switch (i) {
                case 0:
                    if (sumDigivolvePoints(self) >= 20 && func_801E0148(self, 2) != 0) {
                        return 0;
                    }
                    break;
                case 2:
                    for (j = 0; j < 5; j++) {
                        if (func_801E03A0(self, j, 2) != 0 && func_801E03A0(self, j, 3) != 0) {
                            return 0;
                        }
                    }
                    break;
                case 3:
                    for (j = 2; j < 4; j++) {
                        if (sumDigivolvePoints(self) >= j * 20 && func_801E0148(self, j) >= 2) {
                            return 0;
                        }
                    }
                    break;
                case 5:
                    return 0;
                case 1:
                case 4:
                case 6:
                case 7:
                    break;
                }
            }
        }
        if (func_801E0080(player, 0) != 0) {
            switch (PLAYER(player)->unk178_26) {
            case 0:
                return cards >= (3 - PLAYER(player)->wins) * 3;
            case 1:
                return cards >= (3 - PLAYER(player)->wins) * 4;
            case 2:
                if (rand() % 3 == 0) {
                    return 1;
                }
                return 0;
            }
            return 0;
        }
    } else {
        for (i = 0; i < 6; i++) {
            if (func_801E0650(i, self) && func_801DFF2C(self) != 0) {
                level = PLAYER(self)->cards[getActiveDigimonCard(self) % 30].card[0x1A] & 0xF;
                switch (i) {
                case 0:
                    j = level;
                    if (j == 0) {
                        j = 1;
                    }
                    if (func_801E0148(self, j + 1) != 0) {
                        return 0;
                    }
                    break;
                case 1:
                    if (level == 0 && func_801E0148(self, 3) != 0) {
                        return 0;
                    }
                    break;
                case 2:
                    if (PLAYER(self)->statPenalty == 0) {
                        j = level;
                        if (j == 0) {
                            j = 1;
                        }
                        if (func_801E03A0(self, PLAYER(self)->specialty, j + 1) != 0) {
                            return 0;
                        }
                    }
                    break;
                case 3:
                    if (func_801E0708(self) != 0) {
                        return 0;
                    }
                    break;
                case 5:
                    return 0;
                case 6:
                    if (level == 1) {
                        if (func_801E03A0(self, PLAYER(self)->specialty, 2) != 0 && PLAYER(self)->displayedStats[0] < 300) {
                            return 0;
                        }
                        if (func_801E03A0(self, PLAYER(self)->specialty, 3) != 0) {
                            return 0;
                        }
                    }
                    break;
                case 7:
                    if (level == 1 && PLAYER(self)->displayedStats[0] < 300) {
                        return 0;
                    }
                    break;
                }
            }
        }
        if (PLAYER(opponent)->wins == 2
            && (func_801E0B48(player) == 0 || (func_801E08E4(opponent) != 0 && func_801E0BB0(opponent) == 10))
            && func_801E0CCC(player) != 0) {
            if (func_801E157C(player) == 1) {
                return 0;
            }
            switch (PLAYER(player)->unk178_26) {
            case 0:
                return cards >= (3 - PLAYER(player)->wins) * 3;
            case 1:
                return cards >= (3 - PLAYER(player)->wins) * 4;
            case 2:
                if (rand() % 3 == 0) {
                    return 1;
                }
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

s32 func_801E201C(Entry8 *entries, s32 n) {
    s32 i;
    s32 j;
    s32 count;

    count = 0;
    for (i = 0; i < n; i++) {
        if (entries[i].id != -1) {
            count++;
        }
    }
    for (i = 0; i < count; i++) {
    retry:
        if (entries[i].id == -1) {
            for (j = i; j < n - 1; j++) {
                entries[j] = entries[j + 1];
            }
            entries[n - 1].id = -1;
            if (entries[j].id == -1) {
                goto retry;
            }
        }
    }
    return count;
}

s32 func_801E2100(s32 player) {
    Candidate cands[4];
    s32 level;
    s32 n;
    s32 i;
    s32 j;
    s32 best;
    s32 count;
    s32 card;
    u8 attr;

    for (level = 0; level < 4; level++) {
        if (func_801E0148(player, level) == 0) {
            continue;
        }
        n = 0;
        for (i = 0; i < 4; i++) {
            cands[i].id = -1;
        }
        for (i = 0; i < 4; i++) {
            card = PLAYER(player)->hand[i];
            if (card == -1) {
                continue;
            }
            if (PLAYER(player)->cards[card % 30].card[2] != 0) {
                continue;
            }
            if (getPartnerIndex(PLAYER(player)->cards[card % 30].id) >= 0) {
                return card;
            }
            attr = (u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4;
            if (((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF) != level) {
                continue;
            }
            cands[n].id = card;
            cands[n].attrCount = func_801E02D8(player, attr);
            cands[n].levelCount = func_801E047C(player, attr, 1);
            cands[n].dpCost = PLAYER(player)->cards[card % 30].card[0x1B];
            cands[n].deckCount = func_801E0210(player, attr);
            for (j = 0; j < 3; j++) {
                if ((s8)((DigimonCardData *)PLAYER(player)->cards[card % 30].card)->supportActions[j].unk0[0] != 0) {
                    cands[n].supports++;
                }
            }
            n++;
        }
        if (n == 0) {
            return -1;
        }
        if (n == 1) {
            return cands[0].id;
        }
        switch (level) {
        case 0:
            switch (PLAYER(player)->unk178_22) {
            case 0:
            case 1:
                best = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].levelCount >= best) {
                        best = cands[i].levelCount;
                    }
                }
                count = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].levelCount >= best) {
                        count++;
                    }
                }
                if (count > 0) {
                    for (i = 0; i < n; i++) {
                        if (cands[i].levelCount < best) {
                            cands[i].id = -1;
                        }
                    }
                    n = func_801E201C((Entry8 *)cands, 4);
                }
                if (n == 1) {
                    return cands[0].id;
                }
                best = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].deckCount >= best) {
                        best = cands[i].deckCount;
                    }
                }
                count = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].deckCount >= best) {
                        count++;
                    }
                }
                if (count > 0) {
                    for (i = 0; i < n; i++) {
                        if (cands[i].deckCount < best) {
                            cands[i].id = -1;
                        }
                    }
                    n = func_801E201C((Entry8 *)cands, 4);
                }
                if (n == 1) {
                    return cands[0].id;
                }
                count = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].supports == 0) {
                        count++;
                    }
                }
                if (count > 0) {
                    for (i = 0; i < n; i++) {
                        if (cands[i].supports != 0) {
                            cands[i].id = -1;
                        }
                    }
                    n = func_801E201C((Entry8 *)cands, 4);
                }
                if (n == 1) {
                    return cands[0].id;
                }
                return cands[rand() % n].id;
            case 2:
                return cands[rand() % n].id;
            default:
                return -1;
            }
        case 2:
        case 3:
            switch (PLAYER(player)->unk178_22) {
            case 0:
            case 1:
                best = 30;
                for (i = 0; i < n; i++) {
                    if (cands[i].deckCount <= best) {
                        best = cands[i].deckCount;
                    }
                }
                count = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].deckCount <= best) {
                        count++;
                    }
                }
                if (count > 0) {
                    for (i = 0; i < n; i++) {
                        if (cands[i].deckCount > best) {
                            cands[i].id = -1;
                        }
                    }
                    n = func_801E201C((Entry8 *)cands, 4);
                }
                if (n == 1) {
                    return cands[0].id;
                }
                count = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].attrCount == 0) {
                        count++;
                    }
                }
                if (count > 0) {
                    for (i = 0; i < n; i++) {
                        if (cands[i].attrCount != 0) {
                            cands[i].id = -1;
                        }
                    }
                    n = func_801E201C((Entry8 *)cands, 4);
                }
                if (n == 1) {
                    return cands[0].id;
                }
                count = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].supports == 0) {
                        count++;
                    }
                }
                if (count > 0) {
                    for (i = 0; i < n; i++) {
                        if (cands[i].supports != 0) {
                            cands[i].id = -1;
                        }
                    }
                    n = func_801E201C((Entry8 *)cands, 4);
                }
                if (n == 1) {
                    return cands[0].id;
                }
                best = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].dpCost >= best) {
                        best = cands[i].dpCost;
                    }
                }
                count = 0;
                for (i = 0; i < n; i++) {
                    if (cands[i].dpCost >= best) {
                        count++;
                    }
                }
                if (count > 0) {
                    for (i = 0; i < n; i++) {
                        if (cands[i].dpCost < best) {
                            cands[i].id = -1;
                        }
                    }
                    n = func_801E201C((Entry8 *)cands, 4);
                }
                if (n == 1) {
                    return cands[0].id;
                }
                return cands[rand() % n].id;
            case 2:
                return cands[rand() % n].id;
            default:
                return -1;
            }
        }
    }
    return -1;
}

s32 func_801E2A50(s16 *cards, s32 player, s32 min) {
    s32 best;
    s32 count;
    s32 i;
    s16 card;
    s32 dp;

    best = 100;
    for (i = 0; i < 4; i++) {
        s16 id = cards[i];

        if (id != -1) {
            dp = PLAYER(player)->cards[id % 30].card[0x1C];
            if (dp >= min && dp < best) {
                best = dp;
            }
        }
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best == PLAYER(player)->cards[card % 30].card[0x1C]) {
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best != PLAYER(player)->cards[card % 30].card[0x1C]) {
            cards[i] = -1;
        }
    }
    if (count == 1) {
        for (i = 0; i < 4; i++) {
            if (cards[i] != -1) {
                return cards[i];
            }
        }
    }
    return -2;
}

s32 func_801E2C98(s16 *cards, s32 player, s32 min) {
    s32 best;
    s32 count;
    s32 i;
    s16 card;

    best = -1;
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best < PLAYER(player)->cards[card % 30].card[0x1C]) {
            best = PLAYER(player)->cards[card % 30].card[0x1C];
        }
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best == PLAYER(player)->cards[card % 30].card[0x1C]) {
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best != PLAYER(player)->cards[card % 30].card[0x1C]) {
            cards[i] = -1;
        }
    }
    if (count == 1) {
        for (i = 0; i < 4; i++) {
            if (cards[i] != -1) {
                return cards[i];
            }
        }
    }
    return -2;
}

s32 func_801E2ED8(s16 *cards, s32 player, s32 min) {
    s32 count;
    s32 i;
    s16 card;

    count = 0;
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && func_801E02D8(player, (u8)((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[0x1A] >> 4) == 1) {
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && func_801E02D8(player, (u8)((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[0x1A] >> 4) != 1) {
            cards[i] = -1;
        }
    }
    if (count == 1) {
        for (i = 0; i < 4; i++) {
            if (cards[i] != -1) {
                return cards[i];
            }
        }
    }
    return -2;
}

s32 func_801E30D0(s16 *cards, s32 player, s32 min) {
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 4; i++) {
        if (cards[i] != -1) {
            u8 level;
            s16 card;

            level = PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A];
            card = cards[i];
            if ((level & 0xF) == ((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF)) {
                count++;
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        if (cards[i] != -1) {
            u8 level;
            s16 card;

            level = PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A];
            card = cards[i];
            if ((level & 0xF) != ((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF)) {
                cards[i] = -1;
            }
        }
    }
    if (count == 1) {
        for (i = 0; i < 4; i++) {
            if (cards[i] != -1) {
                return cards[i];
            }
        }
    }
    return -2;
}

s32 func_801E3364(s16 *cards, s32 player, s32 min) {
    s32 count;
    s32 i;
    s32 j;
    s32 n;

    count = 0;
    for (i = 0; i < 4; i++) {
        n = 0;
        if (cards[i] != -1) {
            for (j = 0; j < 3; j++) {
                Player *p = PLAYER(player);
                s16 card = cards[i];

                if ((s8)((DigimonCardData *)p->cards[card % 30].card)->supportActions[j].unk0[0] != 0) {
                    n++;
                }
            }
            if (n != 0) {
                count++;
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        n = 0;
        if (cards[i] != -1) {
            for (j = 0; j < 3; j++) {
                Player *p = PLAYER(player);
                s16 card = cards[i];

                if ((s8)((DigimonCardData *)p->cards[card % 30].card)->supportActions[j].unk0[0] != 0) {
                    n++;
                }
            }
            if (n == 0) {
                cards[i] = -1;
            }
        }
    }
    if (count == 1) {
        for (i = 0; i < 4; i++) {
            if (cards[i] != -1) {
                return cards[i];
            }
        }
    }
    return -2;
}

s32 func_801E3574(s16 *ids, s32 player) {
    s32 count;
    s32 i;
    s32 pick;

    count = 0;
    for (i = 0; i < 4; i++) {
        if (ids[i] != -1) {
            count++;
        }
    }
    if (count != 0) {
        pick = rand() % count;
        count = 0;
        for (i = 0; i < 4; i++) {
            if (ids[i] != -1) {
                if (count == pick) {
                    return ids[i];
                }
                count++;
            }
        }
    }
    return -1;
}

s32 func_801E363C(s32 player) {
    s16 ids[4];
    s32 need;
    s32 count;
    s32 i;
    s32 result;
    s32 self;

    if (getActiveDigimonCard(player) == -1) {
        return -1;
    }
    if (((u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF) != 1) {
        need = 50 - sumDigivolvePoints(player);
    } else {
        need = 60 - sumDigivolvePoints(player);
    }
    if (((DuelK *)D_801D8340)->selected != NULL) {
        if (((DuelK *)D_801D8340)->selected->kind == 1) {
            if (((DuelK *)D_801D8340)->selected->need == 0) {
                return -1;
            }
        }
        if (((DuelK *)D_801D8340)->selected->kind > 0) {
            need = ((DuelK *)D_801D8340)->selected->need;
        }
    }
    /* a copy of player for the rest: it only changes the register allocation */
    self = player;
    count = 0;
    if (need <= 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        ids[i] = PLAYER(self)->hand[i];
        if (ids[i] != -1) {
            if (PLAYER(self)->cards[ids[i] % 30].card[2] != 0) {
                ids[i] = -1;
            } else if (PLAYER(self)->cards[ids[i] % 30].card[0x1C] == 0) {
                ids[i] = -1;
            } else {
                if (((DuelK *)D_801D8340)->selected != NULL && ((DuelK *)D_801D8340)->selected->unk2 == ids[i]) {
                    ids[i] = -1;
                }
                if (ids[i] != -1) {
                    count++;
                }
            }
        }
    }
    if (count == 0) {
        return -1;
    }
    if (count == 1) {
        for (i = 0; i < 4; i++) {
            if (ids[i] != -1) {
                return ids[i];
            }
        }
    }
    switch (PLAYER(self)->unk178_22) {
    case 0:
        if ((result = func_801E2A50(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = func_801E2C98(ids, self, need)) > 0) {
            return result;
        }
        if ((result = func_801E2ED8(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = func_801E30D0(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = func_801E3364(ids, self, need)) >= 0) {
            return result;
        }
        return func_801E3574(ids, self);
    case 1:
        if (((u8)PLAYER(self)->cards[getActiveDigimonCard(self) % 30].card[0x1A] & 0xF) == 0 && func_801DFF2C(self) < 2) {
            return -1;
        }
        if ((result = func_801E2A50(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = func_801E2ED8(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = func_801E3364(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = func_801E2C98(ids, self, need)) > 0) {
            return result;
        }
        if ((result = func_801E30D0(ids, self, need)) >= 0) {
            return result;
        }
        return func_801E3574(ids, self);
    case 2:
        return func_801E3574(ids, self);
    }
    return -1;
}

Slot7C8 *func_801E3AF8(s32 kind) {
    s32 i;

    if (((DuelK *)D_801D8340)->selected == NULL) {
        switch (kind) {
        case 0:
            ((DuelK *)D_801D8340)->selected = NULL;
            break;
        case 1:
            for (i = 0; i < 4; i++) {
                if (((DuelK *)D_801D8340)->slots[i].kind == 1) {
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[i];
                }
            }
            break;
        case 2:
            for (i = 0; i < 4; i++) {
                if (((DuelK *)D_801D8340)->slots[i].kind == 2) {
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[i];
                }
            }
            break;
        }
    }
    return ((DuelK *)D_801D8340)->selected;
}

s32 func_801E3C00(s32 player) {
    Player *p;
    s8 *card;
    s32 specialty;
    s32 level;
    s32 points;
    s32 cost;
    s32 target;
    s32 targetSpecialty;
    s32 sel;
    s32 self;
    s32 need;

    /* a copy of player: it only changes the register allocation */
    self = player;
    if (getActiveDigimonCard(self) == -1) {
        return -1;
    }
    if (((DuelK *)D_801D8340)->selected == NULL) {
        return -1;
    }
    if (PLAYER(self)->cards[((DuelK *)D_801D8340)->selected->unk2 % 30].card[2] != 0) {
        return -1;
    }
    sel = ((DuelK *)D_801D8340)->selected->unk2;
    if (getPlayedCard(player) == -1) {
        need = PLAYER(self)->cards[sel % 30].card[0x1B];
        if (sumDigivolvePoints(self) < need) {
            return -1;
        }
    } else {
        specialty = PLAYER(self)->specialty;
        level = (u8)PLAYER(self)->cards[getActiveDigimonCard(self) % 30].card[0x1A] & 0xF;
        points = sumDigivolvePoints(self);
        p = PLAYER(self);
        card = p->cards[sel % 30].card;
        targetSpecialty = (u8)card[0x1A] >> 4;
        target = (u8)card[0x1A] & 0xF;
        cost = card[0x1B];
        switch (p->cards[getPlayedCard(self) % 30].card[0x1A]) {
        case 0:
            if (level == 1) {
                return -1;
            }
            if (level == 0) {
                level = 1;
            }
            if (level + 1 != target) {
                return -1;
            }
            if (points + 20 < cost) {
                return -1;
            }
            break;
        case 1:
            if (level != 0) {
                return -1;
            }
            if (target != 3) {
                return -1;
            }
            if (targetSpecialty != specialty) {
                return -1;
            }
            if (points < cost) {
                return -1;
            }
            break;
        case 2:
            if (level == 1) {
                return -1;
            }
            if (level == 0) {
                level = 1;
            }
            if (PLAYER(self)->statPenalty != 0) {
                return -1;
            }
            if (level + 1 != target) {
                return -1;
            }
            if (targetSpecialty != specialty) {
                return -1;
            }
            break;
        case 3:
            if (points < cost) {
                return -1;
            }
            if (target != level) {
                return -1;
            }
            break;
        case 4:
        case 5:
        case 6:
        case 7:
            break;
        }
    }
    return ((DuelK *)D_801D8340)->selected->unk2;
}

void func_801E3FF4(s32 player) {
    s32 minNeed[4];
    s32 i;
    s32 j;
    s32 k;
    s32 card;
    s32 option;
    s32 hand;
    s32 specialty;
    s32 level;
    s32 target;
    s32 targetSpecialty;
    s32 flag;

    for (i = 0; i < 4; i++) {
        card = ((DuelK *)D_801D8340)->slots[i].unk2;
        if (card != -1 && PLAYER(player)->cards[card % 30].type == 0 && ((DuelK *)D_801D8340)->slots[i].kind == 1 &&
            ((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF) == 2) {
            return;
        }
    }
    for (i = 0; i < 4; i++) {
        option = ((DuelK *)D_801D8340)->slots[i].unk2;
        if (option == -1 || PLAYER(player)->cards[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            card = ((DuelK *)D_801D8340)->slots[j].unk2;
            if (card == -1 || PLAYER(player)->cards[card % 30].type != 0) {
                continue;
            }
            specialty = PLAYER(player)->specialty;
            level = (u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF;
            targetSpecialty = (u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4;
            target = (u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF;
            switch (PLAYER(player)->cards[option % 30].card[0x1A]) {
            case 0:
                if (level == 1) {
                    break;
                }
                if (level == 0) {
                    level = 1;
                }
                if (level + 1 != target) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    minNeed[k] = 100;
                    if (((DuelK *)D_801D8340)->slots[j].need - 20 > 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < ((DuelK *)D_801D8340)->slots[j].need - 20) {
                        continue;
                    }
                    if (((DuelK *)D_801D8340)->slots[j].need - 20 < 0) {
                        minNeed[k] = 0;
                    } else {
                        minNeed[k] = ((DuelK *)D_801D8340)->slots[j].need - 20;
                    }
                    ((DuelK *)D_801D8340)->slots[j].kind = 1;
                    ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                }
                for (k = 0; k < 4; k++) {
                    if (minNeed[k] < ((DuelK *)D_801D8340)->slots[j].need) {
                        ((DuelK *)D_801D8340)->slots[j].need = minNeed[k];
                    }
                }
                break;
            case 1:
                if (level != 0 || target != 3 || targetSpecialty != specialty) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    if (((DuelK *)D_801D8340)->slots[j].need != 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < ((DuelK *)D_801D8340)->slots[j].need) {
                        continue;
                    }
                    ((DuelK *)D_801D8340)->slots[j].kind = 1;
                    ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                }
                break;
            case 2:
                if (level == 1) {
                    break;
                }
                if (level == 0) {
                    level = 1;
                }
                if (PLAYER(player)->statPenalty != 0 || level + 1 != target || targetSpecialty != specialty) {
                    break;
                }
                ((DuelK *)D_801D8340)->slots[j].kind = 1;
                ((DuelK *)D_801D8340)->slots[j].need = 0;
                ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                break;
            case 3:
                if (level != target) {
                    break;
                }
                flag = PLAYER(player)->displayedStats[0] < ((DigimonCardData *)PLAYER(player)->cards[card % 30].card)->hp;
                if (PLAYER(player)->statPenalty != 0) {
                    flag = 1;
                }
                if (!PLAYER(player)->unk178_30 || flag != 1 || ((DuelK *)D_801D8340)->selected != NULL) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    if (((DuelK *)D_801D8340)->slots[j].need != 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < ((DuelK *)D_801D8340)->slots[j].need) {
                        continue;
                    }
                    ((DuelK *)D_801D8340)->slots[j].kind = 1;
                    ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                }
                break;
            case 4:
                if (countEmptyDigimonStackSlots(player) < 2 && PLAYER(player)->displayedStats[0] < 300) {
                    ((DuelK *)D_801D8340)->slots[j].kind = 1;
                    ((DuelK *)D_801D8340)->slots[j].need = 0;
                    ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                }
                break;
            case 5:
                if (level < target) {
                    ((DuelK *)D_801D8340)->slots[j].kind = 1;
                    ((DuelK *)D_801D8340)->slots[j].need = 0;
                    ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                }
                break;
            case 6:
                if (level != 1 || targetSpecialty != specialty) {
                    break;
                }
                if ((target == 2 && PLAYER(player)->displayedStats[0] < 300) || target == 3) {
                    for (k = 0; k < 4; k++) {
                        if (((DuelK *)D_801D8340)->slots[j].need != 0 && k == j) {
                            continue;
                        }
                        hand = PLAYER(player)->hand[k];
                        if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                            continue;
                        }
                        if (PLAYER(player)->cards[hand % 30].card[0x1C] < ((DuelK *)D_801D8340)->slots[j].need) {
                            continue;
                        }
                        ((DuelK *)D_801D8340)->slots[j].kind = 1;
                        ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                        ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                    }
                }
                break;
            case 7:
                if (level == 1 && PLAYER(player)->displayedStats[0] < 300) {
                    ((DuelK *)D_801D8340)->slots[j].kind = 1;
                    ((DuelK *)D_801D8340)->slots[j].need = 0;
                    ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
                    ((DuelK *)D_801D8340)->selected = &((DuelK *)D_801D8340)->slots[j];
                }
                break;
            }
        }
    }
    if (((DuelK *)D_801D8340)->selected != NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        option = ((DuelK *)D_801D8340)->slots[i].unk2;
        if (option == -1 || PLAYER(player)->cards[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            }
            card = ((DuelK *)D_801D8340)->slots[j].unk2;
            if (card == -1 || PLAYER(player)->cards[card % 30].type != 2) {
                continue;
            }
            if (PLAYER(player)->cards[option % 30].card[0x1A] == PLAYER(player)->cards[card % 30].card[0x1A]) {
                ((DuelK *)D_801D8340)->slots[j].id = ((DuelK *)D_801D8340)->slots[i].unk2;
            }
        }
    }
}

s32 func_801E4E08(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((DuelK *)D_801D8340)->slots[i].id != -1) {
            return ((DuelK *)D_801D8340)->slots[i].id;
        }
    }
    return -1;
}

#define SIM(i) (DUEL_AI->sims[i])

void func_801E4E58(s32 player) {
    s32 draws;
    s32 wins;
    s32 i;
    s32 weight;
    s32 pick;
    s32 same;

    draws = 0;
    wins = 0;
    DUEL->cpuResult = 0;
    for (i = 0; i < 3; i++) {
        if (SIM(i).outcome == 0) {
            draws++;
        }
        if (SIM(i).outcome == 1) {
            wins++;
        }
    }
    if (draws != 0) {
        if (draws == 1) {
            for (i = 0; i < 3; i++) {
                if (SIM(i).outcome == 0) {
                    DUEL->cpuResult = i;
                    return;
                }
            }
            DUEL->cpuResult = 0;
        } else {
            weight = 0;
            pick = 0;
            if (SIM(0).outcome == 0) {
                weight += 2;
            }
            if (SIM(1).outcome == 0) {
                weight += 3;
            }
            if (SIM(2).outcome == 0) {
                weight += 5;
            }
            if (weight != 0) {
                pick = rand() % weight;
            }
            if (SIM(0).outcome == 0) {
                if (pick < 2) {
                    DUEL->cpuResult = 0;
                } else if (SIM(1).outcome == 0 && pick < 5) {
                    DUEL->cpuResult = 1;
                } else {
                    DUEL->cpuResult = 2;
                }
            } else if (SIM(1).outcome == 0 && pick < 3) {
                DUEL->cpuResult = 1;
            } else {
                DUEL->cpuResult = 2;
            }
        }
    } else if (wins != 0) {
        if (wins == 1) {
            for (i = 0; i < 3; i++) {
                if (SIM(i).outcome == 1) {
                    DUEL->cpuResult = i;
                    break;
                }
            }
            if (i == 3) {
                DUEL->cpuResult = 0;
            }
        } else {
            same = 3;
            if (PLAYER(player)->unk178_24 != 2 && (rand() & 1)) {
                if (DUEL->turnPlayer == player) {
                    DUEL->cpuResult = 0;
                    if (SIM(0).totalOwn < SIM(1).totalOwn) {
                        DUEL->cpuResult = 1;
                    }
                    if (DUEL->cpuResult == 0) {
                        if (SIM(0).totalOwn < SIM(2).totalOwn) {
                            DUEL->cpuResult = 2;
                        }
                    } else if (SIM(1).totalOwn < SIM(2).totalOwn) {
                        DUEL->cpuResult = 2;
                    }
                    same = 0;
                    for (i = 0; i < 3; i++) {
                        if (SIM(i).totalOwn == SIM(DUEL->cpuResult).totalOwn) {
                            same++;
                        }
                    }
                } else {
                    DUEL->cpuResult = 0;
                    if (SIM(0).totalOpponent > SIM(1).totalOpponent) {
                        DUEL->cpuResult = 1;
                    }
                    if (DUEL->cpuResult == 0) {
                        if (SIM(0).totalOpponent > SIM(2).totalOpponent) {
                            DUEL->cpuResult = 2;
                        }
                    } else if (SIM(1).totalOpponent > SIM(2).totalOpponent) {
                        DUEL->cpuResult = 2;
                    }
                    same = 0;
                    for (i = 0; i < 3; i++) {
                        if (SIM(i).totalOpponent == SIM(DUEL->cpuResult).totalOpponent) {
                            same++;
                        }
                    }
                }
            }
            if (same >= 2) {
                weight = 0;
                if (SIM(0).outcome == 1) {
                    weight += 2;
                }
                if (SIM(1).outcome == 1) {
                    weight += 3;
                }
                if (SIM(2).outcome == 1) {
                    weight += 5;
                }
                pick = rand() % weight;
                if (SIM(0).outcome == 1) {
                    if (pick < 2) {
                        DUEL->cpuResult = 0;
                    } else if (SIM(1).outcome == 1 && pick < 5) {
                        DUEL->cpuResult = 1;
                    } else {
                        DUEL->cpuResult = 2;
                    }
                } else if (SIM(1).outcome == 1 && pick < 3) {
                    DUEL->cpuResult = 1;
                } else {
                    DUEL->cpuResult = 2;
                }
            }
        }
    } else {
        same = 3;
        if (PLAYER(player)->unk178_24 != 2 && (rand() & 1)) {
            if (DUEL->turnPlayer == player) {
                DUEL->cpuResult = 0;
                if (SIM(0).totalOwn < SIM(1).totalOwn) {
                    DUEL->cpuResult = 1;
                }
                if (DUEL->cpuResult == 0) {
                    if (SIM(0).totalOwn < SIM(2).totalOwn) {
                        DUEL->cpuResult = 2;
                    }
                } else if (SIM(1).totalOwn < SIM(2).totalOwn) {
                    DUEL->cpuResult = 2;
                }
                same = 0;
                for (i = 0; i < 3; i++) {
                    if (SIM(i).totalOwn == SIM(DUEL->cpuResult).totalOwn) {
                        same++;
                    }
                }
            } else {
                DUEL->cpuResult = 0;
                if (SIM(0).totalOpponent > SIM(1).totalOpponent) {
                    DUEL->cpuResult = 1;
                }
                if (DUEL->cpuResult == 0) {
                    if (SIM(0).totalOpponent > SIM(2).totalOpponent) {
                        DUEL->cpuResult = 2;
                    }
                } else if (SIM(1).totalOpponent > SIM(2).totalOpponent) {
                    DUEL->cpuResult = 2;
                }
                same = 0;
                for (i = 0; i < 3; i++) {
                    if (SIM(i).totalOpponent == SIM(DUEL->cpuResult).totalOpponent) {
                        same++;
                    }
                }
            }
        }
        if (same >= 2) {
            DUEL->cpuResult = rand() % 3;
        }
    }
    if (DUEL->cpuResult < 0) {
        DUEL->cpuResult = rand() % 3;
    }
}

typedef struct {
    s32 own;
    s32 opponent;
    s8 kills;
    s8 survives;
    s8 dies;
} CardScore;

/* The CPU picks the card to play with its attack: it scores each hand card
 * against the opponent's cards and prefers one that kills, then one that
 * survives, then the least bad. Its choice goes to DUEL->cpuResult. */
void func_801E5710(void) {
    CardScore scores[5];
    s32 self;
    s32 opponent;
    s32 attack;
    s32 i;
    s32 j; /* also the best score, the chosen slot and the random start */
    s32 k;
    s32 kills;
    s32 survives;
    s32 count;
    s32 cards;

    self = DUEL->cpuPlayer;
    opponent = self ^ 1;
    attack = PLAYER(self)->attackChoice;
    for (i = 0; i < 5; i++) {
        scores[i].own = 0;
        scores[i].opponent = 0;
        scores[i].kills = 0;
        scores[i].survives = 0;
        scores[i].dies = 0;
    }
    if (PLAYER(opponent)->playedCard >= 0) {
        for (i = 0; i < 5; i++) {
            if (i != 4 && func_801E0C50(self, PLAYER(self)->hand[i]) != 0) {
                continue;
            }
            for (k = 0; k < 3; k++) {
                if (DUEL_AI->sims[attack].cards[i].damage[4][k].own > 0) {
                    scores[i].survives++;
                } else {
                    scores[i].dies++;
                }
                if (DUEL_AI->sims[attack].cards[i].damage[4][k].opponent <= 0) {
                    scores[i].kills++;
                }
                scores[i].own += DUEL_AI->sims[attack].cards[i].damage[4][k].own;
                scores[i].opponent += DUEL_AI->sims[attack].cards[i].damage[4][k].opponent;
            }
        }
    } else {
        for (i = 0; i < 5; i++) {
            if (i != 4 && func_801E0C50(self, PLAYER(self)->hand[i]) != 0) {
                continue;
            }
            for (j = 0; j < 5; j++) {
                if (j != 4 && func_801E0C50(opponent, PLAYER(opponent)->hand[j]) != 0) {
                    continue;
                }
                for (k = 0; k < 3; k++) {
                    if (DUEL_AI->sims[attack].cards[i].damage[j][k].own > 0) {
                        scores[i].survives++;
                    } else {
                        scores[i].dies++;
                    }
                    if (DUEL_AI->sims[attack].cards[i].damage[j][k].opponent <= 0) {
                        scores[i].kills++;
                    }
                    scores[i].own += DUEL_AI->sims[attack].cards[i].damage[j][k].own;
                    scores[i].opponent += DUEL_AI->sims[attack].cards[i].damage[j][k].opponent;
                }
            }
        }
    }
    kills = 0;
    survives = 0;
    for (i = 0; i < 5; i++) {
        kills += scores[i].kills;
        survives += scores[i].survives;
    }
    if (kills != 0) {
        if (kills == 1) {
            for (i = 0; i < 4; i++) {
                if (scores[i].kills != 0) {
                    DUEL->cpuResult = PLAYER(self)->hand[i];
                    return;
                }
            }
            DUEL->cpuResult = -1;
            return;
        }
        if ((s8)PLAYER(opponent)->unk1BD[0] != -1) {
            for (i = 0; i < 4; i++) {
                if (scores[i].kills != 0 && func_801E094C(self, PLAYER(self)->hand[i]) != 0) {
                    DUEL->cpuResult = PLAYER(self)->hand[i];
                    return;
                }
            }
        }
        j = 0;
        for (i = 0; i < 5; i++) {
            if (scores[i].kills != 0 && j < scores[i].own) {
                j = scores[i].own;
            }
        }
        for (i = 0; i < 5; i++) {
            if (scores[i].kills != 0 && scores[i].own != j) {
                scores[i].kills = 0;
            }
        }
        kills = 0;
        for (i = 0; i < 5; i++) {
            if (scores[i].kills != 0) {
                kills++;
                j = i;
            }
        }
        if (kills == 1) {
            DUEL->cpuResult = PLAYER(self)->hand[j];
            return;
        }
        if (scores[4].kills != 0) {
            DUEL->cpuResult = -1;
            return;
        }
        j = rand() % 5;
        for (i = j; i < j + 5; i++) {
            if (scores[i % 5].kills != 0) {
                DUEL->cpuResult = PLAYER(self)->hand[i % 5];
                return;
            }
        }
        DUEL->cpuResult = -2;
        return;
    }
    if (survives != 0) {
        cards = countOnlineDeckCards(self);
        switch (PLAYER(self)->unk178_28) {
        case 0:
            if ((s8)PLAYER(opponent)->unk1BD[0] != -1) {
                for (i = 0; i < 4; i++) {
                    if (scores[i].survives != 0 && func_801E094C(self, PLAYER(self)->hand[i]) != 0) {
                        DUEL->cpuResult = PLAYER(self)->hand[i];
                        return;
                    }
                }
            }
            j = 10000;
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0 && scores[i].opponent < j) {
                    j = scores[i].opponent;
                }
            }
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0 && scores[i].opponent != j) {
                    scores[i].survives = 0;
                }
            }
            survives = 0;
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0) {
                    survives++;
                    j = i;
                }
            }
            if (survives == 1) {
                DUEL->cpuResult = PLAYER(self)->hand[j];
                return;
            }
            if (cards < (3 - PLAYER(self)->wins) * 2 || (PLAYER(self)->wins == 2 && PLAYER(opponent)->wins == 0)) {
                j = rand() % 4;
                for (i = j; i < j + 4; i++) {
                    if (scores[i % 4].survives != 0) {
                        DUEL->cpuResult = PLAYER(self)->hand[i % 4];
                        return;
                    }
                }
                DUEL->cpuResult = -1;
                return;
            }
            DUEL->cpuResult = -2;
            return;
        case 1:
            j = 0;
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0 && j < scores[i].own) {
                    j = scores[i].own;
                }
            }
            for (i = 0; i < 5; i++) {
                /* clears kills, not survives: the filter does nothing */
                if (scores[i].survives != 0 && scores[i].own != j) {
                    scores[i].kills = 0;
                }
            }
            survives = 0;
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0) {
                    survives++;
                    j = i;
                }
            }
            if (survives == 1) {
                DUEL->cpuResult = PLAYER(self)->hand[j];
                return;
            }
            j = 10000;
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0 && scores[i].opponent < j) {
                    j = scores[i].opponent;
                }
            }
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0 && scores[i].opponent != j) {
                    scores[i].survives = 0;
                }
            }
            survives = 0;
            for (i = 0; i < 5; i++) {
                if (scores[i].survives != 0) {
                    survives++;
                    j = i;
                }
            }
            if (survives == 1) {
                DUEL->cpuResult = PLAYER(self)->hand[j];
                return;
            }
            if ((s8)PLAYER(opponent)->unk1BD[0] != -1) {
                for (i = 0; i < 4; i++) {
                    if (scores[i].survives != 0 && func_801E0868(self, PLAYER(self)->hand[i]) != 0) {
                        if (func_801E0ACC(opponent, (s8)PLAYER(opponent)->unk1BD[0]) |
                            func_801E094C(opponent, (s8)PLAYER(opponent)->unk1BD[0])) {
                            DUEL->cpuResult = PLAYER(self)->hand[i];
                        }
                        return;
                    }
                }
            }
            break;
        case 2:
            break;
        default:
            return;
        }
    } else {
        cards = countOnlineDeckCards(self);
        switch (PLAYER(self)->unk178_28) {
        case 0:
            if ((s8)PLAYER(opponent)->unk1BD[0] != -1 && PLAYER(self)->wins != 2) {
                for (i = 0; i < 4; i++) {
                    if (scores[i].dies != 0 && func_801E094C(self, PLAYER(self)->hand[i]) != 0) {
                        DUEL->cpuResult = PLAYER(self)->hand[i];
                        return;
                    }
                }
            }
            j = 10000;
            for (i = 0; i < 5; i++) {
                if (scores[i].dies != 0 && scores[i].opponent < j) {
                    j = scores[i].opponent;
                }
            }
            for (i = 0; i < 5; i++) {
                /* clears survives, not dies (here and in case 1) */
                if (scores[i].dies != 0 && scores[i].opponent != j) {
                    scores[i].survives = 0;
                }
            }
            count = 0;
            for (i = 0; i < 5; i++) {
                if (scores[i].dies != 0) {
                    count++;
                    j = i;
                }
            }
            if (count == 1) {
                DUEL->cpuResult = PLAYER(self)->hand[j];
                return;
            }
            if (cards >= (3 - PLAYER(self)->wins) * 2) {
                DUEL->cpuResult = -2;
                return;
            }
            j = rand() % 5;
            for (i = j; i < j + 5; i++) {
                /* the original tests scores[i], not scores[i % 5] */
                if (scores[i].dies != 0) {
                    DUEL->cpuResult = PLAYER(self)->hand[i % 5];
                    return;
                }
            }
            DUEL->cpuResult = -1;
            return;
        case 1:
            j = 10000;
            for (i = 0; i < 5; i++) {
                if (scores[i].dies != 0 && scores[i].opponent < j) {
                    j = scores[i].opponent;
                }
            }
            for (i = 0; i < 5; i++) {
                if (scores[i].dies != 0 && scores[i].opponent != j) {
                    scores[i].survives = 0;
                }
            }
            count = 0;
            for (i = 0; i < 5; i++) {
                if (scores[i].dies != 0) {
                    count++;
                    j = i;
                }
            }
            if (count == 1) {
                DUEL->cpuResult = PLAYER(self)->hand[j];
                return;
            }
            if ((s8)PLAYER(opponent)->unk1BD[0] != -1) {
                for (i = 0; i < 4; i++) {
                    if (scores[i].dies != 0 && func_801E094C(self, PLAYER(self)->hand[i]) != 0) {
                        DUEL->cpuResult = PLAYER(self)->hand[i];
                        return;
                    }
                }
            }
            break;
        case 2:
            break;
        default:
            return;
        }
    }
    DUEL->cpuResult = rand() % 6 - 2;
    if (DUEL->cpuResult >= 0) {
        DUEL->cpuResult = PLAYER(self)->hand[DUEL->cpuResult];
    }
}


typedef struct {
    u8 *card;
    s8 order;
} BattleEffect;

s32 func_801E9C1C(s32 arg0, s32 arg1, SupportCond *conds, SupportEffect *effects, s32 arg4);
void func_801E9700(s32 self, s32 other, DigimonCardData *cardData, s32 quiet);
void func_801EA5F4(Player *p);
void func_801F623C(s32 entry, s32 player, s32 mode);

#define CARD_SPR(c) (((CardAnim *)(D_801D833C + (c) * 36))->spr)

s32 func_801E6AA4(s32 quiet) {
    BattleEffect effects[6];
    u8 unused[0x88];
    s32 i;
    s32 k;
    s32 player;
    s32 card;
    s32 result;
    PlayerStats *attacker;
    PlayerStats *defender;
    PlayerStats *tmp;

    for (i = 0; i < 2; i++) {
        STATS(i)->hpBeforeBattle = STATS(i)->stats[0];
        STATS(i)->damageTaken = 0;
        for (k = 0; k < 3; k++) {
            STATS(i)->attackDamage[k] = STATS(i)->baseAttackPowers[k];
        }
        STATS(i)->flags.f6 = 0;
        STATS(i)->flags.f8 = 0;
        STATS(i)->flags.f9 = 0;
        STATS(i)->flags.f11 = 0;
        STATS(i)->flags.f12 = 0;
        STATS(i)->flags.f13 = 0;
        STATS(i)->flags.f14 = 0;
        STATS(i)->flags.f7 = 0;
    }
    for (i = 0; i < 6; i++) {
        effects[i].order = 0;
    }
    for (i = 0; i < 2; i++) {
        player = DUEL->turnPlayer ^ i;
        STATS(player)->flags.f31 = 0;
        card = getPlayedCard(player);
        if (card != -1) {
            if (SPRITE_KIND(card) == 0x19) {
                STATS(player)->flags.f31 = 1;
            } else {
                STATS(player)->unk110 |= 0x10000;
            }
            STATS(player)->unk110 |= 0x10;
            switch (PLAYER(player)->cards[card % 30].card[2]) {
            case 0:
                effects[i + 2].card = (u8 *)PLAYER(player)->cards[card % 30].card;
                effects[i + 2].order = effects[i + 2].card[0xE6];
                break;
            case 1:
                effects[i].card = (u8 *)PLAYER(player)->cards[card % 30].card;
                effects[i].order = effects[i].card[0x8C];
                break;
            case 2:
                effects[i].order = -1;
                effects[i + 2].order = -1;
                if (!quiet) {
                    CARD_SPR(card)->rgbc[0] = 0xFF;
                    CARD_SPR(card)->rgbc[1] = 0xFF;
                    CARD_SPR(card)->rgbc[2] = 0xFF;
                    if (SPRITE_KIND(card) == 0x19) {
                        SPRITE_KIND(card) = 0x10;
                        func_80014C08(0x10);
                    }
                    func_801F623C(0x13, player, 1);
                    CARD_SPR(card)->rgbc[0] = 0x80;
                    CARD_SPR(card)->rgbc[1] = 0x80;
                    CARD_SPR(card)->rgbc[2] = 0x80;
                }
                break;
            }
        } else {
            STATS(player)->unk110 |= 0x10000;
        }
    }
    for (i = 0; i < 2; i++) {
        player = DUEL->turnPlayer ^ i;
        card = getActiveDigimonCard(player);
        if (card != -1) {
            effects[i + 4].card = (u8 *)PLAYER(player)->cards[card % 30].card;
            effects[i + 4].order = D_8006E4FC[((DigimonCardData *)effects[i + 4].card)->crossEffect];
        }
    }
    for (i = 3; i > 0; i--) {
        for (k = 0; k < 6; k++) {
            player = DUEL->turnPlayer ^ (k % 2);
            if (!quiet && k < 4 && effects[k].order == i && getPlayedCard(player) != -1) {
                if (SPRITE_KIND(getPlayedCard(player)) == 0x19) {
                    SPRITE_KIND(getPlayedCard(player)) = 0x10;
                    func_80014C08(0x10);
                }
                card = getPlayedCard(player);
                CARD_SPR(card)->rgbc[0] = 0xFF;
                CARD_SPR(card)->rgbc[1] = 0xFF;
                CARD_SPR(card)->rgbc[2] = 0xFF;
            }
            if (effects[k].order != i) {
                continue;
            }
            switch (k) {
            case 0:
            case 1:
                if (!(STATS(player)->flags.f9 & 2)) {
                    result = func_801E9C1C(player, player ^ 1, (SupportCond *)(effects[k].card + 0x1C),
                                           (SupportEffect *)(effects[k].card + 0x5C), quiet);
                    if (!quiet && result) {
                        func_801F623C(0x13, player, 1);
                    }
                } else if (!quiet) {
                    func_801F623C(0x13, player, 1);
                }
                if (!quiet) {
                    card = getPlayedCard(player);
                    CARD_SPR(card)->rgbc[0] = 0x80;
                    CARD_SPR(card)->rgbc[1] = 0x80;
                    CARD_SPR(card)->rgbc[2] = 0x80;
                }
                break;
            case 2:
            case 3:
                if (!(STATS(player)->flags.f9 & 1)) {
                    result = func_801E9C1C(player, player ^ 1, (SupportCond *)(effects[k].card + 0x74),
                                           (SupportEffect *)(effects[k].card + 0xB4), quiet);
                    if (!quiet && result) {
                        func_801F623C(0x13, player, 1);
                    }
                } else if (!quiet) {
                    func_801F623C(0x13, player, 1);
                }
                if (!quiet) {
                    card = getPlayedCard(player);
                    CARD_SPR(card)->rgbc[0] = 0x80;
                    CARD_SPR(card)->rgbc[1] = 0x80;
                    CARD_SPR(card)->rgbc[2] = 0x80;
                }
                break;
            case 4:
            case 5:
                if (STATS(player)->flags.attackChoice == 2) {
                    if (!quiet) {
                        card = getActiveDigimonCard(player);
                        CARD_SPR(card)->rgbc[0] = 0xFF;
                        CARD_SPR(card)->rgbc[1] = 0xFF;
                        CARD_SPR(card)->rgbc[2] = 0xFF;
                    }
                    func_801E9700(player, player ^ 1, (DigimonCardData *)effects[k].card, quiet);
                    if (!quiet) {
                        CARD_SPR(card)->rgbc[0] = 0x80;
                        CARD_SPR(card)->rgbc[1] = 0x80;
                        CARD_SPR(card)->rgbc[2] = 0x80;
                    }
                }
                break;
            }
            if (!quiet) {
                waitForStatCountersToSettle();
            }
        }
    }
    if (!quiet) {
        for (i = 0; i < 2; i++) {
            player = DUEL->turnPlayer ^ i;
            if (getPlayedCard(player) != -1 && (effects[i].order | effects[i + 2].order) == 0) {
                if (SPRITE_KIND(getPlayedCard(player)) == 0x19) {
                    SPRITE_KIND(getPlayedCard(player)) = 0x10;
                    func_80014C08(0x10);
                }
                card = getPlayedCard(player);
                CARD_SPR(card)->rgbc[0] = 0xFF;
                CARD_SPR(card)->rgbc[1] = 0xFF;
                CARD_SPR(card)->rgbc[2] = 0xFF;
                func_801F623C(0x13, player, 1);
                CARD_SPR(card)->rgbc[0] = 0x80;
                CARD_SPR(card)->rgbc[1] = 0x80;
                CARD_SPR(card)->rgbc[2] = 0x80;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        if (STATS(i)->hpBeforeBattle != 0 && STATS(i)->hpBeforeBattle % 1110 == 0) {
            STATS(i)->unk110 |= 0x400;
        }
    }
    attacker = STATS(DUEL->turnPlayer);
    defender = STATS((s8)(DUEL->turnPlayer ^ 1));
    if (defender->flags.f8) {
        if (!attacker->flags.f8) {
            tmp = attacker;
            attacker = defender;
            defender = tmp;
        } else {
            attacker->flags.f8 = 0;
            defender->flags.f8 = 0;
        }
    }
    if (attacker->flags.f6) {
        if (!defender->flags.f6) {
            attacker->flags.f7 = 1;
            attacker->flags.f8 = 0;
            tmp = attacker;
            attacker = defender;
            defender = tmp;
        } else {
            attacker->flags.f7 = 0;
            defender->flags.f7 = 0;
            for (k = 0; k < 3; k++) {
                attacker->attackDamage[k] = 0;
                defender->attackDamage[k] = 0;
            }
        }
    }
    DUEL->firstAttacker = (Player *)attacker;
    DUEL->secondAttacker = (Player *)defender;
    if (defender->flags.f6) {
        defender->attackDamage[defender->flags.usedAttack] = attacker->attackDamage[attacker->flags.usedAttack];
        for (i = 0; i < 3; i++) {
            attacker->attackDamage[i] = 0;
        }
    }
    for (i = 0; i < 2; i++) {
        STATS(i)->stats[0] = STATS(i)->hpBeforeBattle;
    }
    if (attacker->flags.f11) {
        if (defender->flags.f6) {
            defender->attackDamage[defender->flags.usedAttack] = attacker->stats[0];
        } else {
            attacker->attackDamage[2] = attacker->stats[0];
        }
        attacker->stats[0] = 10;
    }
    defender->damageTaken = attacker->attackDamage[attacker->flags.usedAttack];
    if (!quiet) {
        func_801EA5F4((Player *)attacker);
    }
    if (attacker->flags.f12) {
        if (defender->stats[0] <= defender->damageTaken) {
            attacker->hpGain = defender->stats[0];
        } else {
            attacker->hpGain = defender->damageTaken;
        }
        attacker->stats[0] += attacker->hpGain;
        if (attacker->stats[0] > 9990) {
            attacker->stats[0] = 9990;
        }
        if (attacker->stats[0] != 0 && attacker->stats[0] % 1110 == 0) {
            attacker->unk110 |= 0x400;
        }
    }
    defender->stats[0] -= defender->damageTaken;
    if (defender->stats[0] <= 0) {
        if (attacker->wins == 2 && defender->stats[0] == 0) {
            attacker->unk110 |= 0x400000;
        }
        defender->stats[0] = 0;
    } else {
        if (defender->flags.f11) {
            if (!defender->flags.f6) {
                defender->attackDamage[2] = defender->stats[0];
            }
            if (defender->attackDamage[2] != 0) {
                defender->stats[0] = 10;
            }
        }
        attacker->damageTaken = defender->attackDamage[defender->flags.usedAttack];
        if (!quiet) {
            func_801EA5F4((Player *)defender);
        }
        if (defender->flags.f12) {
            if (attacker->stats[0] <= attacker->damageTaken) {
                defender->hpGain = attacker->stats[0];
            } else {
                defender->hpGain = attacker->damageTaken;
            }
            defender->stats[0] += defender->hpGain;
            if (defender->stats[0] > 9990) {
                defender->stats[0] = 9990;
            }
            if (defender->stats[0] != 0 && defender->stats[0] % 1110 == 0) {
                defender->unk110 |= 0x400;
            }
        }
        attacker->stats[0] -= attacker->damageTaken;
        if (attacker->stats[0] <= 0) {
            if (defender->wins == 2 && attacker->stats[0] == 0) {
                defender->unk110 |= 0x400000;
            }
            attacker->stats[0] = 0;
        }
    }
    attacker->hpAfterBattle = attacker->stats[0];
    defender->hpAfterBattle = defender->stats[0];
    attacker->stats[0] = attacker->hpBeforeBattle;
    defender->stats[0] = defender->hpBeforeBattle;
    for (i = 0; i < 2; i++) {
        if (STATS(i)->stats[0] != 0 && STATS(i)->stats[0] % 1110 == 0) {
            STATS(i)->unk110 |= 0x400;
        }
        if (STATS(i)->damageTaken != 0 && STATS(i)->damageTaken % 1110 == 0) {
            STATS(i ^ 1)->unk110 |= 0x200;
        }
        switch (STATS(i)->flags.usedAttack) {
        case 0:
            STATS(i)->unk110 |= 1;
            break;
        case 1:
            STATS(i)->unk110 |= 2;
            break;
        case 2:
            STATS(i)->unk110 |= 4;
            break;
        }
    }
}

extern s32 D_801FC404;
s32 func_801E7DD4(s32 self, s32 other, s32 kind, s32 value, s32 slot) {
    s32 card;
    s32 n;

    switch (kind) {
    case 0:
        return value;
    case 1:
        return PLAYER(self)->specialty;
    case 2:
        return PLAYER(other)->specialty;
    case 3:
        return STATS(self)->hpBeforeBattle;
    case 4:
        return STATS(other)->hpBeforeBattle;
    case 5:
    case 7:
    case 9:
        return STATS(self)->attackDamage[(kind - 5) / 2];
    case 6:
    case 8:
    case 10:
        return STATS(other)->attackDamage[(kind - 6) / 2];
    case 11:
        return STATS(self)->attackDamage[slot];
    case 12:
        return STATS(other)->attackDamage[slot];
    case 13:
        card = getActiveDigimonCard(self);
        return PLAYER(self)->cards[card % 30].card[0x1A] & 0xF;
    case 14:
        card = getActiveDigimonCard(other);
        return PLAYER(other)->cards[card % 30].card[0x1A] & 0xF;
    case 15:
        switch (countEmptyDigimonStackSlots(self)) {
        case 0:
            return 1;
        case 1:
            card = PLAYER(self)->digimonStack[2];
            if (PLAYER(self)->cards[card % 30].card[0x1A] & 0xF) {
                return 0;
            }
        default:
            return -1;
        }
    case 16:
        switch (countEmptyDigimonStackSlots(other)) {
        case 0:
            return 1;
        case 1:
            card = PLAYER(other)->digimonStack[2];
            if (PLAYER(other)->cards[card % 30].card[0x1A] & 0xF) {
                return 0;
            }
        default:
            return -1;
        }
    case 17:
        return PLAYER(self)->usedAttack;
    case 18:
        return PLAYER(other)->usedAttack;
    case 19:
        card = getPlayedCard(other);
        switch (PLAYER(other)->cards[card % 30].card[2]) {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            return -1;
        }
    case 20:
        return self != DUEL->turnPlayer;
    case 21:
        return 4 - countEmptyHandSlots(self);
    case 22:
        return 4 - countEmptyHandSlots(other);
    case 23:
        return 8 - countEmptyDpSlots(self);
    case 24:
        return 8 - countEmptyDpSlots(other);
    case 25:
        return countOfflineDeckCards(self) == 0;
    case 26:
        return D_801FC404;
    case 27:
        return countOnlineDeckCards(self);
    case 28:
        return countOnlineDeckCards(other);
    }
    return 0;
}


extern s32 D_801FC404;
void func_801FA30C(s32 player);

#define SHOW_EFFECT_FAILED(player) \
    do {                          \
        func_801F623C(0x13, player, 1); \
    } while (0)

s32 func_801E81DC(s32 self, s32 other, s32 kind, s32 value, s32 slot, s32 quiet) {
    s32 cards[4];
    u8 unused[0x90];
    s32 i;
    s32 j;
    s32 n;
    s32 card;

    switch (kind) {
    case 0:
        if (!quiet) {
            func_801F6294(0xF, self, self, 1, 0);
        }
        PLAYER(self)->specialty = value % 5;
        if (!quiet) {
            func_801FA30C(self);
        }
        break;
    case 1:
        if (!quiet) {
            func_801F6294(0xF, self, other, 1, 0);
        }
        PLAYER(other)->specialty = value % 5;
        if (!quiet) {
            func_801FA30C(other);
        }
        break;
    case 2:
        if (!quiet) {
            func_801F6294(0x14, self, self, 1, 0);
            showStatChangePopup(self, value, 0);
        }
        STATS(self)->hpBeforeBattle = value;
        STATS(self)->stats[0] = value;
        if (!quiet && value != 0 && value % 1110 == 0) {
            func_801FB444(self, 0x1A);
            STATS(self)->unk110 |= 0x400;
        }
        break;
    case 3:
        if (!quiet) {
            func_801F6294(0x14, self, other, 1, 0);
            showStatChangePopup(other, value, 0);
        }
        STATS(other)->hpBeforeBattle = value;
        STATS(other)->stats[0] = value;
        if (!quiet && value != 0 && value % 1110 == 0) {
            func_801FB444(other, 0x1A);
            STATS(other)->unk110 |= 0x400;
        }
        break;
    case 4:
    case 6:
    case 8:
        if (!quiet) {
            func_801F6294(0x14, self, self, 1, 0);
            showStatChangePopup(self, value, (kind - 4) / 2 + 1);
        }
        STATS(self)->attackDamage[(kind - 4) / 2] = value;
        STATS(self)->stats[(kind - 4) / 2 + 1] = value;
        break;
    case 5:
    case 7:
    case 9:
        if (!quiet) {
            func_801F6294(0x14, self, other, 1, 0);
            showStatChangePopup(other, value, (kind - 5) / 2 + 1);
        }
        STATS(other)->attackDamage[(kind - 5) / 2] = value;
        STATS(other)->stats[(kind - 5) / 2 + 1] = value;
        break;
    case 10:
        if (!quiet) {
            if (slot == 0) {
                func_801F6294(0x14, self, self, 1, 0);
            }
            if (!quiet) {
                showStatChangePopup(self, value, slot + 1);
                func_80014C08(6);
            }
        }
        STATS(self)->attackDamage[slot] = value;
        STATS(self)->stats[slot + 1] = value;
        break;
    case 11:
        if (!quiet) {
            if (slot == 0) {
                func_801F6294(0x14, self, other, 1, 0);
            }
            if (!quiet) {
                showStatChangePopup(other, value, slot + 1);
                func_80014C08(6);
            }
        }
        STATS(other)->attackDamage[slot] = value;
        STATS(other)->stats[slot + 1] = value;
        break;
    case 16:
        PLAYER(self)->usedAttack = value;
        if (!quiet) {
            func_801F6294(0x10, self, self, 1, 0);
            PLAYER(self)->attackChoice = value;
        }
        break;
    case 17:
        PLAYER(other)->usedAttack = value;
        if (!quiet) {
            func_801F6294(0x10, self, other, 1, 0);
            PLAYER(other)->attackChoice = value;
        }
        break;
    case 25:
        D_801FC404 = value;
        break;
    case 26:
        if (!quiet) {
            if (4 - countEmptyHandSlots(self) < value) {
                value = 4 - countEmptyHandSlots(self);
            }
            if (value == 0) {
                break;
            }
            for (i = 0; i < value && countEmptyHandSlots(self) != 4; i++) {
            for (j = 0, n = 0; j < 4; j++) {
                cards[n] = PLAYER(self)->hand[j];
                if (cards[n] != -1) {
                    n++;
                }
            }
            card = cards[rand() % n];
                if (removeCardFromHand(card, self) != -1) {
                    SPRITE_KIND(card) = 8;
                    discardCardToOfflineDeck(card, self);
                    func_80014C08(20);
                }
            }
        }
        break;
    case 27:
        if (!quiet) {
            for (i = 0; i < value && countEmptyHandSlots(other) != 4; i++) {
            for (j = 0, n = 0; j < 4; j++) {
                cards[n] = PLAYER(other)->hand[j];
                if (cards[n] != -1) {
                    n++;
                }
            }
            card = cards[rand() % n];
                if (removeCardFromHand(card, other) != -1) {
                    SPRITE_KIND(card) = 8;
                    discardCardToOfflineDeck(card, other);
                    func_80014C08(20);
                }
            }
        }
        break;
    case 28:
        if (!quiet) {
            value = 0;
            for (i = 0; i < 4; i++) {
                if (PLAYER(self)->hand[i] != -1 && PLAYER(self)->cards[PLAYER(self)->hand[i] % 30].card[2] != 0) {
                    value++;
                }
            }
            if (value == 0) {
                SHOW_EFFECT_FAILED(self);
            } else {
                for (i = 0; i < 4; i++) {
                    if (PLAYER(self)->hand[i] != -1 && PLAYER(self)->cards[PLAYER(self)->hand[i] % 30].card[2] != 0 &&
                        removeCardFromHand(PLAYER(self)->hand[i], self) != -1) {
                        SPRITE_KIND(PLAYER(self)->hand[i]) = 8;
                        discardCardToOfflineDeck(PLAYER(self)->hand[i], self);
                        func_80014C08(20);
                    }
                }
            }
        }
        break;
    case 29:
        if (!quiet) {
            value = 0;
            for (i = 0; i < 4; i++) {
                if (PLAYER(other)->hand[i] != -1 && PLAYER(other)->cards[PLAYER(other)->hand[i] % 30].card[2] != 0) {
                    value++;
                }
            }
            if (value == 0) {
                SHOW_EFFECT_FAILED(self);
            } else {
                for (i = 0; i < 4; i++) {
                    if (PLAYER(other)->hand[i] != -1 && PLAYER(other)->cards[PLAYER(other)->hand[i] % 30].card[2] != 0 &&
                        removeCardFromHand(PLAYER(other)->hand[i], other) != -1) {
                        SPRITE_KIND(PLAYER(other)->hand[i]) = 8;
                        discardCardToOfflineDeck(PLAYER(other)->hand[i], other);
                        func_80014C08(20);
                    }
                }
            }
        }
        break;
    case 30:
        if (!quiet) {
            for (i = 0; i < value && countEmptyHandSlots(self) != 4; i++) {
            for (j = 0, n = 0; j < 4; j++) {
                cards[n] = PLAYER(self)->hand[j];
                if (cards[n] != -1) {
                    n++;
                }
            }
            card = cards[rand() % n];
                returnCardToOnlineDeck(card, self);
                removeCardFromHand(card, self);
                SPRITE_KIND(card) = 1;
                func_80014C08(20);
            }
        }
        break;
    case 31:
        if (!quiet) {
            for (i = 0; i < value && countEmptyHandSlots(other) != 4; i++) {
            for (j = 0, n = 0; j < 4; j++) {
                cards[n] = PLAYER(other)->hand[j];
                if (cards[n] != -1) {
                    n++;
                }
            }
            card = cards[rand() % n];
                returnCardToOnlineDeck(card, self);
                removeCardFromHand(card, other);
                SPRITE_KIND(card) = 1;
                func_80014C08(20);
            }
        }
        break;
    case 32:
        if (!quiet) {
            for (i = 0; i < value && (card = peekOnlineDeckTop(self)) != -1; i++) {
                discardCardToOfflineDeck(card, self);
                drawOnlineDeckCard(self);
                SPRITE_KIND(card) = 8;
                func_80014C08(20);
            }
        }
        break;
    case 33:
        if (!quiet) {
            for (i = 0; i < value && (card = peekOnlineDeckTop(other)) != -1; i++) {
                discardCardToOfflineDeck(card, other);
                drawOnlineDeckCard(other);
                SPRITE_KIND(card) = 8;
                func_80014C08(20);
            }
        }
        break;
    case 34:
        if (!quiet) {
            for (i = 0; i < value && (card = peekOfflineDeckTop(self)) != -1; i++) {
                returnCardToOnlineDeck(card, self);
                takeOfflineDeckTopCard(self);
                SPRITE_KIND(card) = 1;
                func_80014C08(20);
            }
        }
        break;
    case 35:
        if (!quiet) {
            for (i = 0; i < value && (card = peekOfflineDeckTop(other)) != -1; i++) {
                returnCardToOnlineDeck(card, other);
                takeOfflineDeckTopCard(other);
                SPRITE_KIND(card) = 1;
                func_80014C08(20);
            }
        }
        break;
    case 36:
        if (!quiet) {
            for (i = 0; i < value && (card = peekDpSlotTop(self)) != -1; i++) {
                discardCardToOfflineDeck(card, self);
                removeCardFromDpSlots(card, self);
                SPRITE_KIND(card) = 8;
                func_80014C08(20);
            }
        }
        break;
    case 37:
        if (!quiet) {
            for (i = 0; i < value && (card = peekDpSlotTop(other)) != -1; i++) {
                discardCardToOfflineDeck(card, other);
                removeCardFromDpSlots(card, other);
                SPRITE_KIND(card) = 8;
                func_80014C08(20);
            }
        }
        break;
    case 42:
        if (!quiet) {
            PLAYER(self)->shufflePasses = 300;
            shuffleOnlineDeck(self);
        }
        break;
    case 43:
        if (!quiet) {
            PLAYER(other)->shufflePasses = 300;
            shuffleOnlineDeck(other);
        }
        break;
    case 44:
        FLAGS178(other)->f9 |= 1;
        if (!quiet) {
            card = getPlayedCard(other);
            if (card != -1 && PLAYER(other)->cards[card % 30].card[2] == 0) {
                func_801F6294(0x11, self, other, 1, 1);
            } else {
                SHOW_EFFECT_FAILED(self);
            }
        }
        break;
    case 45:
        FLAGS178(other)->f9 = 3;
        if (!quiet) {
            if (getPlayedCard(other) != -1) {
                func_801F6294(0x12, self, other, 1, 1);
            } else {
                SHOW_EFFECT_FAILED(self);
            }
        }
        break;
    case 46:
        if (!quiet && countOnlineDeckCards(self) != 0 && countEmptyHandSlots(self) != 0) {
            card = takePartnerCardFromOnlineDeck(self);
            if (card != -1) {
                n = addCardToHand(card, self);
                SPRITE_KIND(card) = 3;
                *(s8 *)(D_801D833C + card * 36 + 0x23) = n;
                func_80014C08(20);
                func_801FA780(self);
            } else {
                SHOW_EFFECT_FAILED(self);
            }
        }
        break;
    case 47:
        if (!quiet) {
            func_801F6294(0x10, self, other, 1, 0);
        }
        PLAYER(other)->usedAttack = (PLAYER(other)->usedAttack + 1) % 3;
        if (!quiet) {
            PLAYER(other)->attackChoice = PLAYER(other)->usedAttack;
        }
        break;
    case 48:
        FLAGS178(self)->f14 = 1;
        *(s16 *)PLAYER(self)->unk166 = value;
        if (!quiet) {
            func_801F6214(0xC, self);
        }
        break;
    case 49:
        if (!quiet) {
            for (i = 0; i < value && countOnlineDeckCards(self) != 0 && countEmptyHandSlots(self) != 0; i++) {
                card = drawOnlineDeckCard(self);
                n = addCardToHand(card, self);
                SPRITE_KIND(card) = 3;
                *(s8 *)(D_801D833C + card * 36 + 0x23) = n;
                func_80014C08(20);
                func_801FA780(self);
            }
        }
        break;
    case 50:
        if (!quiet) {
            for (i = 0; i < value && countOnlineDeckCards(other) != 0 && countEmptyHandSlots(other) != 0; i++) {
                card = drawOnlineDeckCard(other);
                n = addCardToHand(card, other);
                SPRITE_KIND(card) = 3;
                *(s8 *)(D_801D833C + card * 36 + 0x23) = n;
                func_80014C08(20);
                func_801FA780(self);
            }
        }
        break;
    case 51:
        FLAGS178(self)->f12 = 1;
        if (!quiet) {
            func_801F6214(0xB, self);
        }
        break;
    case 52:
        FLAGS178(self)->f6 = 1;
        FLAGS178(other)->f6 = 0;
        if (!quiet) {
            func_801F6214(0xE, self);
        }
        break;
    case 53:
        FLAGS178(self)->f8 = 1;
        if (!quiet) {
            func_801F6214(0xA, self);
        }
        break;
    }
}

void func_801E9700(s32 self, s32 other, DigimonCardData *cardData, s32 quiet) {
    u8 unused[0xB0];
    s32 card;
    s32 value;

    switch (cardData->crossEffect) {
    case 0:
        break;
    case 1:
        FLAGS178(self)->f8 = 1;
        if (!quiet) {
            func_801F6214(10, self);
        }
        break;
    case 2:
    case 3:
    case 4:
        if (!quiet) {
            showStatChangePopup(other, 0, cardData->crossEffect - 1);
            func_801F6294(0x14, self, other, 0, 0);
        }
        STATS(other)->attackDamage[cardData->crossEffect - 2] = 0;
        STATS(other)->stats[cardData->crossEffect - 1] = 0;
        break;
    case 5:
    case 6:
    case 7:
        if (PLAYER(other)->usedAttack == cardData->crossEffect - 5) {
            FLAGS178(self)->f6 = 1;
            FLAGS178(other)->f6 = 0;
        }
        if (!quiet) {
            func_801F6214(0xE, self);
        }
        break;
    case 8:
        FLAGS178(self)->f11 = 1;
        if (!quiet) {
            func_801F6214(0xD, self);
        }
        break;
    case 9:
        FLAGS178(self)->f12 = 1;
        if (!quiet) {
            func_801F6214(0xB, self);
        }
        break;
    case 10:
        FLAGS178(other)->f9 |= 1;
        if (!quiet) {
            card = getPlayedCard(other);
            if (card != -1 && PLAYER(other)->cards[card % 30].card[2] == 0) {
                func_801F6294(0x11, self, other, 0, 1);
            } else {
                func_801F6214(0x13, self);
            }
        }
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        if (cardData->crossEffect - 11 == PLAYER(other)->specialty) {
            value = STATS(self)->attackDamage[2] * 3;
            if (!quiet) {
                showStatChangePopup(self, value, 3);
                func_801F6214(0x14, self);
            }
            if (value > 9990) {
                value = 9990;
            }
            STATS(self)->attackDamage[2] = value;
            STATS(self)->stats[3] = value;
            FLAGS178(self)->f13 = 1;
        } else if (!quiet) {
            func_801F6214(0x13, self);
        }
        break;
    }
}

s32 func_801E9ABC(s32 a, s32 op, s32 b) {
    switch (op) {
    case 0:
        a += b;
        if (a > 9990) {
            a = 9990;
        }
        return a;
    case 1:
        if (a - b < 0) {
            return 0;
        }
        return a - b;
    case 2:
        a *= b;
        if (a > 9990) {
            a = 9990;
        }
        return a;
    case 3:
        if ((a == 0) | (b == 0)) {
            return 0;
        }
        a /= b;
        if (a < 10) {
            a = 10;
        }
        return a / 10 * 10;
    }
    return 0;
}

s32 func_801E9BAC(s32 a, s32 op, s32 b) {
    switch (op) {
    case 0:
        return a < b;
    case 1:
        return a <= b;
    case 2:
        return a > b;
    case 3:
        return a >= b;
    case 4:
        return a != b;
    case 5:
        return a == b;
    }
    return 0;
}

s32 func_801E9C1C(s32 arg0, s32 arg1, SupportCond *conds, SupportEffect *effects, s32 arg4) {
    s32 vals[6];
    s16 slotVals[3][3];
    s32 i;
    s32 j;
    s32 k;
    s32 a;

    for (i = 0; i < 2; i++) {
        if (conds[i].active != 0) {
            for (j = 0; j < 6; j++) {
                vals[j] = func_801E7DD4(arg0, arg1, conds[i].lhs[j], conds[i].rhs[j], 0);
            }
            a = func_801E9ABC(func_801E9ABC(vals[0], conds[i].ops[0], vals[1]), conds[i].ops[1], vals[2]);
            if (func_801E9BAC(a, conds[i].cmp, func_801E9ABC(func_801E9ABC(vals[3], conds[i].ops[2], vals[4]), conds[i].ops[3], vals[5])) == 0) {
                return -1;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (effects[i].active != 0) {
            if (effects[i].kind == 10 || effects[i].kind == 11) {
                for (j = 0; j < 3; j++) {
                    for (k = 0; k < 3; k++) {
                        slotVals[j][k] = func_801E7DD4(arg0, arg1, effects[i].lhs[j], effects[i].rhs[j], k);
                    }
                }
                for (k = 0; k < 3; k++) {
                    func_801E81DC(arg0, arg1, effects[i].kind,
                                  func_801E9ABC(func_801E9ABC(slotVals[2][k], effects[i].ops[1], slotVals[1][k]), effects[i].ops[0], slotVals[0][k]),
                                  k, arg4);
                }
            } else {
                for (j = 0; j < 3; j++) {
                    vals[j] = func_801E7DD4(arg0, arg1, effects[i].lhs[j], effects[i].rhs[j], 0);
                }
                func_801E81DC(arg0, arg1, effects[i].kind,
                              func_801E9ABC(func_801E9ABC(vals[2], effects[i].ops[1], vals[1]), effects[i].ops[0], vals[0]), 0, arg4);
            }
        }
    }
    return 0;
}

s32 func_801E9F5C(s32 card, s32 player) {
    s32 specialty;
    u8 level;
    s32 dp;
    s32 cardSpecialty;
    s32 cardLevel;
    s32 cost;
    s8 *data;
    u8 *p;
    CardSlot *played;

    if (card == -1) {
        return -1;
    }
    if (getActiveDigimonCard(player) == -1) {
        return -1;
    }
    if (PLAYER(player)->cards[card % 30].type != 0) {
        return -1;
    }
    specialty = PLAYER(player)->specialty;
    level = PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF;
    dp = sumDigivolvePoints(player);
    data = PLAYER(player)->cards[card % 30].card;
    cardSpecialty = (u8)data[0x1A] >> 4;
    cardLevel = data[0x1A] & 0xF;
    cost = data[0x1B];
    if (getPlayedCard(player) != -1) {
        p = DUEL_PLAYERS[player];
        p += (getPlayedCard(player) % 30) * sizeof(CardSlot);
        played = ((Player *)p)->cards;
        if (played->card[2] == 2) {
            p = DUEL_PLAYERS[player];
            p += (getPlayedCard(player) % 30) * sizeof(CardSlot);
            played = ((Player *)p)->cards;
            switch (played->card[0x1A]) {
            case 0:
                if (level == 1) {
                    return -1;
                }
                if (level == 0) {
                    level = 1;
                }
                if (cardLevel == level + 1) {
                    if (dp + 20 >= cost) {
                        return 0;
                    }
                }
                break;
            case 1:
                if (level != 0) {
                    return -1;
                }
                if (cardSpecialty != specialty) {
                    return -1;
                }
                if (cardLevel == 3 && dp >= cost) {
                    return 0;
                }
                break;
            case 2:
                if (level == 1) {
                    return -1;
                }
                if (level == 0) {
                    level = 1;
                }
                if (PLAYER(player)->statPenalty == 0 && cardSpecialty == specialty && cardLevel == level + 1) {
                    return 0;
                }
                break;
            case 5:
                return 0;
            case 3:
                if (cardLevel != level) {
                    return -1;
                }
                if (dp >= cost) {
                    return 0;
                }
                break;
            case 4:
                if (level == 1) {
                    return -1;
                }
                if (countEmptyDigimonStackSlots(player) < 2) {
                    return 0;
                }
                break;
            case 6:
                if (level != 1) {
                    return -1;
                }
                if (cardLevel < 2) {
                    return -1;
                }
                if (cardSpecialty != specialty) {
                    return -1;
                }
                if (dp >= cost) {
                    return 0;
                }
                break;
            case 7:
                if (level == 1) {
                    return 0;
                }
                break;
            }
        }
    } else if (level != 1) {
        if (level == 0) {
            level = 1;
        }
        if (cardSpecialty != specialty) {
            return -1;
        }
        if (cardLevel != level + 1) {
            return -1;
        }
        if (dp >= cost) {
            return 0;
        }
    }
    return -1;
}

s32 func_801EA374(s32 player) {
    s32 i;
    u8 *data;

    if (getPlayedCard(player) != -1) {
        data = DUEL_PLAYERS[player];
        data += (getPlayedCard(player) % 30) * sizeof(CardSlot);
        if (((Player *)data)->cards[0].card[2] == 2) {
            data = DUEL_PLAYERS[player];
            data += (getPlayedCard(player) % 30) * sizeof(CardSlot);
            switch (((Player *)data)->cards[0].card[0x1A]) {
            case 4:
                if (countEmptyDigimonStackSlots(player) < 2) {
                    return 0;
                }
                break;
            case 7:
                if ((((Player *)DUEL_PLAYERS[player])->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF) == 1) {
                    return 0;
                }
                break;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (func_801E9F5C(((Player *)DUEL_PLAYERS[player])->hand[i], player) == 0) {
            return 0;
        }
    }
    return -1;
}

s32 func_801EA558(s32 card, s32 player) {
    Player *p = (Player *)DUEL_PLAYERS[player];

    p->statPenalty = ((u8 *)p->cards[card % 30].card)[0x1A];
    ((Player *)DUEL_PLAYERS[player])->unk110 &= ~0x40000000;
}

void func_801EA5F4(Player *p) {
    s32 player;
    s32 attack;
    s32 index;

    player = p->controller;
    attack = p->usedAttack;
    if (DUEL->tutorial == 0 && player != 1) {
        index = p->cards[getActiveDigimonCard(player) % 30].index;
        if (((PlayerStats *)p)->attackDamage[attack] > ((ProfileK *)PLAYER_PROFILES)[player].bestDamage[index][attack]) {
            ((ProfileK *)PLAYER_PROFILES)[player].bestDamage[index][attack] = ((PlayerStats *)p)->attackDamage[attack];
        }
    }
}

void func_801EA708(void) {
    DUEL->tutorial = 1;
    *(ScriptRunner **)D_801D8340 = allocTaskHeapBlock(sizeof(ScriptRunner));
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\BETA.MSD", getCurrentTaskId());
    (*(ScriptRunner **)D_801D8340)->data = (void *)func_80014C08(0x7FFFFFFF);
    (*(ScriptRunner **)D_801D8340)->script = createScriptContext((*(ScriptRunner **)D_801D8340)->data);
    (*(ScriptRunner **)D_801D8340)->regs = allocScriptRegisters(10);
    (*(ScriptRunner **)D_801D8340)->unkC = 0;
    func_801EAB4C();
}

void func_801EA7E8(void) {
    if (DUEL->tutorial) {
        freeScriptContext((*(ScriptRunner **)D_801D8340)->script, (*(ScriptRunner **)D_801D8340)->regs);
        freeHeapBlock((*(ScriptRunner **)D_801D8340)->data);
        freeHeapBlock(*(ScriptRunner **)D_801D8340);
    }
}

void func_801EA868(UiWindow *window) {
    drawText(window->originX, window->originY, *(s32 *)(*(u8 **)D_801D8340 + 0x10), 7, window->z);
}

extern UiWindow D_801FC410;

s32 func_801EA8B4(s32 y, u8 *src) {
    Rect16 rect;
    u8 text[200];
    u8 *dst;
    s32 w;
    s32 h;
    s32 i;

    text[0] = '*';
    text[1] = 's';
    text[2] = '0';
    dst = &text[3];
    DUEL->awaitingInput = 0;
    do {
        if (*src < 0x81 || *src > 0x98) {
            if (src[0] == '*' && src[1] == 'p') {
                src += 2;
                *dst = 0;
                strcpy(dst, PLAYER(0)->name);
                dst += strlen(PLAYER(0)->name);
                continue;
            }
        } else {
            *dst++ = *src++;
        }
        *dst++ = *src++;
    } while (src[-1] != 0);
    (*(ScriptRunner **)D_801D8340)->unk10 = (s32)text;
    measureText((u8 *)(*(ScriptRunner **)D_801D8340)->unk10);
    w = (TEXT_WIDTH + 1) / 2;
    rect.w = w * 2;
    h = (TEXT_HEIGHT + 1) / 2;
    rect.h = h * 2;
    rect.x = (320 - rect.w) >> 1;
    rect.y = y - rect.h / 2;
    openWindow(&D_801FC410, &rect, -1, (s16 *)-1, 8, 0x15, 0x80, 8);
    D_801FC410.label = (s32)"TUTORIAL";
    D_801FC410.palette = 4;
    playSoundEffect(0xA3);
    PAD_INPUT_ENABLED = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (drawWindow(&D_801FC410, func_801EA868, 0) == 0 || ((PAD_STATES[0]->rawPressed & 0x40) >> 6) == 0);
    playSoundEffect(0xA4);
    animateWindowTo(&D_801FC410, (Rect16 *)-1);
    for (i = 0; i < 16; i++) {
        func_80014C08(FRAME_INTERVAL);
        drawWindow(&D_801FC410, func_801EA868, 0);
    }
    PAD_INPUT_ENABLED = 0;
}

void func_801F97C4(s32 arg0, s32 arg1, s32 arg2);
void func_801F97E4(void);

s32 func_801EAB4C(void) {
    s32 *vars;
    s32 result;
    s32 player;
    s32 slot;
    s32 id;

    if (DUEL->unk823 != 0 || DUEL->tutorial == 0) {
        return;
    }
    if ((*(ScriptRunner **)D_801D8340)->unkC != 0) {
        (*(ScriptRunner **)D_801D8340)->unkC--;
        return;
    }
    vars = (*(ScriptRunner **)D_801D8340)->regs;
    vars[2] = DUEL->step;
    if (DUEL->cursorPlayer == 0) {
        vars[3] = DUEL->cursorSlot;
    } else {
        vars[3] = -1;
    }
    vars[4] = 0;
    vars[5] = 0;
    vars[6] = 0;
    vars[7] = 0;
    if (PAD_STATES[0]->pressed & 0x20) {
        vars[4] = 1;
    }
    if (PAD_STATES[0]->pressed & 0x80) {
        vars[5] = 1;
    }
    if (PAD_STATES[0]->pressed & 0x10) {
        vars[6] = 1;
    }
    if (PAD_STATES[0]->pressed & 0x40) {
        vars[7] = 1;
    }
    do {
        DUEL->unk820[0] = 1;
        result = runScriptToNextEvent((*(ScriptRunner **)D_801D8340)->script, vars);
        if (result == 1) {
            DUEL->unk81D = vars[9];
            switch ((*(ScriptRunner **)D_801D8340)->script->eventOp) {
            case 10:
                switch ((*(ScriptRunner **)D_801D8340)->script->eventArg) {
                case 0:
                    func_801EA8B4((*(ScriptRunner **)D_801D8340)->regs[8], (u8 *)(*(ScriptRunner **)D_801D8340)->regs[0]);
                    break;
                case 1:
                    DUEL->unk820[0] = 0;
                    return;
                case 2:
                    break;
                case 3:
                    player = vars[1];
                    strcpy((char *)DUEL_PLAYERS[player] + 1, (char *)vars[0]);
                    linkDeckCardData(player, (PlayerDeck *)DUEL_PLAYERS[player]);
                    break;
                case 4:
                    DUEL->unk824 = 2;
                    DUEL->unk820[0] = 0;
                    return;
                case 5:
                    DUEL->cursorSlot = -1;
                    ((CardCursor *)DUEL->cursor)->id = -1;
                    break;
                case 6:
                    func_801F97E4();
                    break;
                case 7:
                    PAD_INPUT_ENABLED = 1;
                    break;
                case 8:
                    PAD_INPUT_ENABLED = 0;
                    break;
                }
                break;
            case 11:
                switch ((*(ScriptRunner **)D_801D8340)->script->eventArg) {
                case 0:
                    func_80014C08((s16)(*(ScriptRunner **)D_801D8340)->script->params[0]);
                    break;
                case 1:
                    DUEL->cpuResult = (s16)(*(ScriptRunner **)D_801D8340)->script->params[0];
                    break;
                case 2:
                    D_801D83D4 = (*(ScriptRunner **)D_801D8340)->script->params[0];
                    break;
                case 3:
                    playSoundEffect((s16)(*(ScriptRunner **)D_801D8340)->script->params[0]);
                    break;
                case 4:
                    DUEL->unk81D = vars[9];
                    func_801EC4CC((s16)(*(ScriptRunner **)D_801D8340)->script->params[0]);
                    break;
                case 5:
                    vars[9] = -1;
                    DUEL->unk81D = -1;
                    func_801EC528((s16)(*(ScriptRunner **)D_801D8340)->script->params[0]);
                    break;
                case 6:
                    D_801D83D1 = (*(ScriptRunner **)D_801D8340)->script->params[0];
                    break;
                }
                break;
            case 12:
                switch ((*(ScriptRunner **)D_801D8340)->script->eventArg) {
                case 0:
                    DUEL_MSG_BAR.playerLabel = (*(ScriptRunner **)D_801D8340)->script->params[0];
                    DUEL_MSG_BAR.phase = (*(ScriptRunner **)D_801D8340)->script->params[1];
                    func_80014C08(60);
                    break;
                case 1:
                    player = vars[1];
                    slot = (s16)(*(ScriptRunner **)D_801D8340)->script->params[0];
                    id = (s16)(*(ScriptRunner **)D_801D8340)->script->params[1];
                    PLAYER(player)->cards[slot].id = id;
                    if (id < 0xBF) {
                        PLAYER(player)->cards[slot].type = 0;
                        PLAYER(player)->cards[slot].index = id;
                    } else if (id < 0x125) {
                        PLAYER(player)->cards[slot].type = 1;
                        PLAYER(player)->cards[slot].index = id - 0xBF;
                    } else {
                        PLAYER(player)->cards[slot].type = 2;
                        PLAYER(player)->cards[slot].index = id - 0x125;
                    }
                    PLAYER(player)->unk0[0] = 1;
                    break;
                case 2:
                    DUEL_MSG_BAR.playerLabel = (*(ScriptRunner **)D_801D8340)->script->params[0];
                    DUEL_MSG_BAR.next2 = (*(ScriptRunner **)D_801D8340)->script->params[1];
                    break;
                case 3:
                    ((CardCursor *)DUEL->cursor)->id = (*(ScriptRunner **)D_801D8340)->script->params[0];
                    DUEL->cursorSlot = (*(ScriptRunner **)D_801D8340)->script->params[1];
                    if (((CardCursor *)DUEL->cursor)->id < 30) {
                        DUEL->cursorPlayer = 0;
                    } else {
                        DUEL->cursorPlayer = 1;
                    }
                    break;
                }
                break;
            case 13:
                if ((*(ScriptRunner **)D_801D8340)->script->eventArg == 0) {
                    func_801F97C4((s16)(*(ScriptRunner **)D_801D8340)->script->params[0], (s16)(*(ScriptRunner **)D_801D8340)->script->params[1],
                                  (s16)(*(ScriptRunner **)D_801D8340)->script->params[2]);
                }
                break;
            }
        }
        clearScriptBusy((*(ScriptRunner **)D_801D8340)->script);
    } while (result != 0);
    DUEL->unk820[0] = 0;
}

void func_801EB32C(void) {
    s32 id;
    s32 i;

    id = PLAYER(DUEL->cursorPlayer)->cards[((CardCursor *)DUEL->cursor)->id % 30].index;
    switch (PLAYER(DUEL->cursorPlayer)->cards[((CardCursor *)DUEL->cursor)->id % 30].type) {
    case 1:
        id += 0xBF;
        break;
    case 2:
        id += 0x125;
        break;
    case 0:
        break;
    }
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].id == id) {
            DUEL->artSlot = i;
            return;
        }
    }
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].id == -1) {
            DUEL->artSlot = i;
            DUEL->cache[i].used = 0;
            return;
        }
    }
    id = 100;
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].age < id) {
            id = DUEL->cache[i].age;
        }
    }
    for (i = 0; i < 6; i++) {
        if (DUEL->cache[i].age == id) {
            DUEL->artSlot = i;
            return;
        }
    }
    DUEL->artSlot = (DUEL->artSlot + 1) % 6;
}

#define DRAW_HAND_MARK(u)                                                                               \
    rect.x = (u);                                                                                        \
    rect.y = player * 24 + 0x30;                                                                         \
    rect.w = 0x1B;                                                                                       \
    rect.h = 0x18;                                                                                       \
    drawPageSprite(((CardAnim *)(D_801D833C + card * 36))->spr->sx + dx,                                 \
                   ((CardAnim *)(D_801D833C + card * 36))->spr->sy + dy, (s32)&rect,                     \
                   getTPage(0, 2, SYSTEM_TEX_X, SYSTEM_TEX_Y), 0xC, 0x32)

s32 func_801EB53C(s32 player) {
    Rect16 rect;
    s32 dx;
    s32 dy;
    s32 i;
    s32 card;
    s32 level;
    s32 cardLevel;

    player %= 2;
    if (player == 0) {
        dx = 0x16;
        dy = -12;
    } else {
        dx = -12;
        dy = 0x24;
    }
    getActiveDigimonCard(player);
    for (i = 0; i < 4; i++) {
        card = PLAYER(player)->hand[i];
        if (card == -1) {
            continue;
        }
        switch (DUEL->unk81D) {
        case 1:
            if (PLAYER(player)->cards[card % 30].type != 0) {
                break;
            }
            cardLevel = (u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF;
            level = cardLevel - 1;
            if (level < 0) {
                level = 0;
            }
            DRAW_HAND_MARK(level * 28 + 0x80);
            break;
        case 2:
            if (PLAYER(player)->cards[card % 30].type != 0) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        case 3:
        case 6:
            if (PLAYER(player)->cards[card % 30].type != 0) {
                break;
            }
            if (func_801E9F5C(card, player) != 0) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        case 4:
            if (PLAYER(player)->cards[card % 30].type == 2) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        case 5:
            if (PLAYER(player)->cards[card % 30].type != 2) {
                break;
            }
            DRAW_HAND_MARK(0x80);
            break;
        }
    }
    if (DUEL->unk81D == 4) {
        card = peekOnlineDeckTop(player);
        if (card != -1) {
            if (player == 0) {
                dx = 8;
                dy = -36;
            } else {
                dx = -10;
                dy = -6;
            }
            DRAW_HAND_MARK(0x80);
        }
    }
}

double func_80026264(s32 x); /* the __floatsidf stub at the end of __cmpdf2 */

s32 func_801EBACC(s32 player, s32 mode) {
    s32 i;
    s32 p;
    s32 card;
    s32 x;
    s32 y;
    s32 cx;
    s32 cy;
    s32 dist;
    s32 best;
    s16 cur;
    CardSprite *spr;

    DUEL->unk81D = mode;
    for (i = 0; i < 2; i++) {
        PLAYER(i)->unk1BD[0] = peekOnlineDeckTop(i);
        PLAYER(i)->unk1BD[1] = peekOfflineDeckTop(i);
        PLAYER(i)->unk1BD[2] = getActiveDigimonCard(i);
        PLAYER(i)->unk1BD[3] = getPlayedCard(i);
        PLAYER(i)->unk1BD[4] = peekDpSlotTop(i);
    }
    if (DUEL->cursorSlot == -1) {
        DUEL->cursorPlayer = player;
        for (i = 0; i < 9; i++) {
            if (PLAYER(player)->hand[i] != -1) {
                ((CardCursor *)DUEL->cursor)->id = PLAYER(player)->hand[i];
                DUEL->cursorSlot = i;
                break;
            }
        }
    }
    cur = ((CardCursor *)DUEL->cursor)->id;
    best = 0x280;
    if (PAD_STATES[player]->repeat & 0x1000) {
        cx = CARD_SPR(cur)->pos.vx;
        spr = CARD_SPR(cur);
        cy = spr->pos.vy - spr->scale * 24 / 4096;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (y < cy) {
                        dist = func_80026D8C(func_80026264((abs(x - cx) ^ 2) + (abs(y - cy) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    } else if (PAD_STATES[player]->repeat & 0x4000) {
        cx = CARD_SPR(cur)->pos.vx;
        spr = CARD_SPR(cur);
        cy = spr->pos.vy + spr->scale * 24 / 4096;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (cy < y) {
                        dist = func_80026D8C(func_80026264((abs(x - cx) ^ 2) + (abs(y - cy) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    } else if ((u16)PAD_STATES[player]->repeat & 0x8000) {
        spr = CARD_SPR(cur);
        cx = spr->pos.vx - spr->scale * 20 / 4096;
        cy = CARD_SPR(cur)->pos.vy;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (x < cx) {
                        dist = func_80026D8C(func_80026264((abs(x - cx) ^ 2) + ((abs(y - cy) / 48 * 480) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    } else if (PAD_STATES[player]->repeat & 0x2000) {
        spr = CARD_SPR(cur);
        cx = spr->pos.vx + spr->scale * 20 / 4096;
        cy = CARD_SPR(cur)->pos.vy;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 9; i++) {
                card = PLAYER(p)->hand[i];
                if (card != -1) {
                    x = CARD_SPR(card)->pos.vx;
                    y = CARD_SPR(card)->pos.vy;
                    if (cx < x) {
                        dist = func_80026D8C(func_80026264((abs(x - cx) ^ 2) + ((abs(y - cy) / 48 * 480) ^ 2)));
                        if (dist <= best) {
                            best = dist;
                            ((CardCursor *)DUEL->cursor)->id = card;
                            DUEL->cursorSlot = i;
                            DUEL->cursorPlayer = p;
                        }
                    }
                }
            }
        }
    }
    if (cur != ((CardCursor *)DUEL->cursor)->id) {
        playSoundEffect(0xA2);
    }
    if (DUEL->cursorSlot < 6) {
        if (DUEL->cursorPlayer == 0) {
            if (D_801D83EC[player * 0xD8 + 0xD] == 4) {
                D_801D83EC[player * 0xD8 + 0xD] = 1;
            }
        } else {
            if (D_801D83EC[player * 0xD8 + 0xD] == 4) {
                D_801D83EC[player * 0xD8 + 0xD] = 2;
            }
        }
    }
    func_801EB32C();
    if (DUEL->cursorPlayer != player) {
        return -1;
    }
    if (mode == 4) {
        if (DUEL->cursorSlot >= 5) {
            return -1;
        }
    } else if (DUEL->cursorSlot >= 4) {
        return -1;
    }
    if (PAD_STATES[player]->pressed & 0x40) {
        return 0;
    }
    return -1;
}

s32 func_801EC4CC(s32 player) {
    D_801D83D1 = (*(u32 *)(DUEL_PLAYERS[player] + 0x178) >> 17) & 3;
    D_801D83EC[player * 0xD8 + 0xD] = player + 1;
}

s32 func_801EC528(s32 index) {
    DUEL->cursorSlot = -1;
    DUEL->unk81D = -1;
    D_801D83EC[index * 0xD8 + 0xD] = 5;
}

s32 func_801EC570(s32 player) {
    s32 card;
    s32 slot;

    if (countOnlineDeckCards(player) == 0) {
        return -1;
    }
    if (countEmptyHandSlots(player) == 0) {
        return -1;
    }
    card = drawOnlineDeckCard(player);
    slot = addCardToHand(card, player);
    SPRITE_KIND(card) = 3;
    *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    return card;
}

s32 func_801EC608(s32 card, s32 player) {
    if (removeCardFromHand(card, player) != -1) {
        SPRITE_KIND(card) = 8;
    } else if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = 8;
    }
    ((CardAnim *)(D_801D833C + card * 36))->spr->pal = (u8)((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[0x1A] >> 4;
    discardCardToOfflineDeck(card, player);
}

s32 func_801EC704(s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
            SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = 8;
            discardCardToOfflineDeck(((Player *)DUEL_PLAYERS[player])->hand[i], player);
            removeCardFromHand(((Player *)DUEL_PLAYERS[player])->hand[i], player);
        }
    }
}

s32 func_801EC7C0(s32 card, s32 player) {
    s32 result;

    result = -1;
    if (countEmptyDigimonStackSlots(player)) {
        result = removeCardFromHand(card, player);
        if (result != -1) {
            SPRITE_KIND(card) = 11;
            placeActiveDigimon(card, player);
        }
    }
    return result;
}

s32 func_801EC84C(s32 card, s32 player, s32 slot) {
    if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

void func_801EC8E0(s32 player, s32 slot) {
    s32 card;

    card = takePlayedCard(player);
    if (card != -1) {
        if (slot != 4) {
            SPRITE_KIND(card) = 3;
            ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
            *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
        } else {
            SPRITE_KIND(card) = 1;
            returnCardToOnlineDeck(card, player);
        }
    }
}

s32 func_801EC9A4(s32 player, s32 slot) {
    s32 card;

    card = takeOfflineDeckTopCard(player);
    if (card != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

s32 func_801ECA30(s32 card, s32 player, s32 slot) {
    if (removeCardFromDpSlots(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

s32 func_801ECAC4(s32 player) {
    s32 card;

    if (countOnlineDeckCards(player) && isPlayedCardSlotEmpty(player)) {
        card = drawOnlineDeckCard(player);
        setPlayedCard(card, player);
        SPRITE_KIND(card) = 21;
    }
}

s32 func_801ECB40(s32 card, s32 player) {
    s32 result;

    result = -1;
    if (isPlayedCardSlotEmpty(player)) {
        result = removeCardFromHand(card, player);
        if (result != -1) {
            SPRITE_KIND(card) = 16;
            setPlayedCard(card, player);
        }
    }
    return result;
}

s32 func_801ECBCC(s32 card, s32 player) {
    s32 result;

    result = -1;
    if (countEmptyDpSlots(player)) {
        result = removeCardFromHand(card, player);
        if (result != -1) {
            SPRITE_KIND(card) = 26;
            addCardToDpSlots(card, player);
        }
    }
    return result;
}

s32 func_801ECC58(s32 player) {
    s32 i;

    while (func_80014C08(20), countEmptyHandSlots(player) != 4) {
        for (i = 0; i < 4; i++) {
            if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
                SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = 8;
                discardCardToOfflineDeck(((Player *)DUEL_PLAYERS[player])->hand[i], player);
                removeCardFromHand(((Player *)DUEL_PLAYERS[player])->hand[i], player);
                break;
            }
        }
    }
    while (func_801EC570(player) != -1) {
        func_80014C08(20);
        func_801FA780(player);
    }
}

s32 func_801ECD68(void) {
    if (DUEL->unk80C >= 0) {
        func_801EC9A4(DUEL->turnPlayer, DUEL->unk80C);
        DUEL->unk80C = -1;
    }
    waitDuelFrames(30);
    if (DUEL->unk80E >= 0) {
        func_801ECA30(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer, DUEL->unk80E);
        DUEL->unk80E = -1;
    }
    waitDuelFrames(30);
    DUEL->step = 11;
}

s32 func_801ECE24(void) {
    while (peekDpSlotTop(DUEL->turnPlayer) != -1) {
        discardCardToOfflineDeck(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
        SPRITE_KIND(peekDpSlotTop(DUEL->turnPlayer)) = 8;
        removeCardFromDpSlots(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
        waitDuelFrames(20);
    }
    waitDuelFrames(30);
    DUEL->step = 23;
}

s32 func_801ECF0C(s32 player) {
    s32 opponent;
    s32 card;
    s32 slot;
    u8 *data;

    opponent = player ^ 1;
    if (PLAYER(player)->stats[0] == 0) {
        if (((u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF) == 3) {
            func_801FB444(opponent, 0xF);
        }
        DUEL->winner = opponent;
        if (((*(u32 *)((u8 *)PLAYER(player) + 0x178) >> 14) & 1) && PLAYER(opponent)->wins != 2) {
            func_801F6214(0x1D, player);
            showStatChangePopup(player, *(s16 *)PLAYER(player)->unk166, 0);
            PLAYER(player)->stats[0] = *(s16 *)PLAYER(player)->unk166;
            waitForStatCountersToSettle();
        } else {
            data = DUEL_PLAYERS[opponent];
            data += (getActiveDigimonCard(opponent) % 30) * sizeof(CardSlot);
            card = ((Player *)data)->cards[0].index;
            if (++PLAYER_DATA(opponent).unk11B6[card] >= 1000) {
                PLAYER_DATA(opponent).unk11B6[card] = 999;
            }
            data = DUEL_PLAYERS[player];
            data += (getActiveDigimonCard(player) % 30) * sizeof(CardSlot);
            card = ((Player *)data)->cards[0].index;
            if (++PLAYER_DATA(player).unk1334[card] >= 1000) {
                PLAYER_DATA(player).unk1334[card] = 999;
            }
            data = DUEL_PLAYERS[player];
            data += (getActiveDigimonCard(player) % 30) * sizeof(CardSlot);
            slot = findArmorPartnerSlot(player, ((Player *)data)->cards[0].id);
            if (slot != -1) {
                func_801F6214(0x1E, player);
                armorDevolvePartner(player, slot);
            }
            while (getActiveDigimonCard(player) != -1) {
                card = getActiveDigimonCard(player);
                ((CardAnim *)(D_801D833C + card * 36))->spr->pal = (u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4;
                func_801EC608(card, player);
                waitDuelFrames(20);
            }
        }
        PLAYER(opponent)->wins++;
        return 1;
    }
    return 0;
}

void func_801ED334(Icon3D *icon, s32 z, RawPolyFT4 *pk) {
    MATRIX matrix;
    SVECTOR vertices[4];
    s32 sxy[4];
    s32 depthCue;
    s32 otz;
    s32 flag;
    u32 *col;
    u16 clut;
    u16 tpage;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;

    PushMatrix();
    buildRotTransMatrix(&icon->pos, &icon->rot, &matrix);
    CompMatrix((MATRIX *)((u8 *)SCENE_3D + 0x78), &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    func_8005C444(&matrix);
    vertices[0].vx = -20;
    vertices[0].vy = -24;
    vertices[0].vz = 0;
    vertices[1].vx = 20;
    vertices[1].vy = -24;
    vertices[1].vz = 0;
    vertices[2].vx = -20;
    vertices[2].vy = 24;
    vertices[2].vz = 0;
    vertices[3].vx = 20;
    vertices[3].vy = 24;
    vertices[3].vz = 0;
    col = (u32 *)&icon->r0;
    if (RotAverageNclip4((s32)&vertices[0], (s32)&vertices[1], (s32)&vertices[2], (s32)&vertices[3], (s32)&sxy[0], (s32)&sxy[1],
                         (s32)&sxy[2], (s32)&sxy[3], &depthCue, &otz, &flag) <= 0) {
        otz = RotAverage4(&vertices[1], &vertices[0], &vertices[3], &vertices[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
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
        tpage = icon->tpage;
        clut = icon->clut;
        u0 = icon->u;
        v0 = icon->v;
        u1 = icon->u + 40;
        v1 = icon->v;
        u2 = icon->u;
        v2 = icon->v + 48;
        u3 = u1;
        v3 = v2;
    }
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
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], pk);
    PopMatrix();
}

void func_801ED608(void) {
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->pressed & 0x40));
}

void func_801ED65C(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 tp, s32 semi, s32 abr, s32 brightness,
                   s32 otz) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = (u % 64) * (4 >> tp);
        CUR_SPRT->sp.v0 = v % 256;
        CUR_SPRT->sp.clut = getClut(clutX, clutY);
        CUR_SPRT->sp.w = w;
        CUR_SPRT->sp.h = h;
        setSemiTrans(&CUR_SPRT->sp, semi);
        CUR_SPRT->sp.r0 = brightness;
        CUR_SPRT->sp.g0 = brightness;
        CUR_SPRT->sp.b0 = brightness;
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(tp, abr, u, v));
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    }
}

void func_801ED8BC(s32 x, s32 y, char *name) {
    char buf[64];

    sprintf(buf, "%s Deck", name);
    drawText(x + 0x18, y + 3, (s32)buf, 7, 1);
    func_801ED65C(x, y, 0x1D0, 0xCA, 0xC0, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

void func_801ED968(s32 x, s32 y, s32 wins, s32 losses) {
    char buf[64];

    sprintf(buf, "*s0%4d        %3d      %3d", wins + losses, wins, losses);
    drawSmallText(x + 0x24, y + 9, (s32)"BATTLES", 6, 1);
    drawSmallText(x + 0x66, y + 9, (s32)"WINS", 6, 1);
    drawSmallText(x + 0x9C, y + 9, (s32)"LOSSES", 6, 1);
    drawText(x + 8, y + 3, (s32)buf, 7, 1);
    func_801ED65C(x, y, 0x1D0, 0xB8, 0xC0, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

void func_801EDA84(s32 isVersus, s32 match, s32 task) {
    char path[64];
    s32 count;
    s32 i;
    u32 *arc;
    s32 width0;
    s32 width1;

    D_801FC458 = 1;
    if (isVersus == 0) {
        match = 999;
        count = 2;
    } else {
        count = 1;
    }
    for (i = 0; i < count; i++) {
        func_800149B8(0, -1, 0, 0x800, uploadStringGlyphs, PLAYER_DATA(i).name, i, getCurrentTaskId(), 0);
        func_80014C08(0x7FFFFFFF);
    }
    sprintf(path, "B:\\MATCH\\%3.3d.ARC", match);
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    width0 = strlen(PLAYER(0)->name) * 16;
    if (isVersus != 0) {
        width1 = *(s16 *)(D_801D485C + 4) * 4;
    } else {
        width1 = strlen(PLAYER(1)->name) * 16;
    }
    *(s16 *)PLAYER(0)->unk118 = width0;
    *(s16 *)PLAYER(1)->unk118 = width1;
    D_801FBA30[0][0] = 100;
    D_801FBA30[0][1] = 0xF1;
    D_801FBA30[0][2] = 0xB8;
    D_801FBA30[0][3] = 0x79;
    D_801FBA30[1][0] = 0x4C;
    D_801FBA30[1][1] = -0x71;
    D_801FBA30[1][2] = 8;
    D_801FBA30[1][3] = 7;
    D_801FBA50[0][0] = 0x140;
    D_801FBA50[0][1] = 0xC3;
    D_801FBA50[1][0] = -width1;
    D_801FBA50[1][1] = 0x10;
    D_801FBA50[1][2] = 0x138 - width1;
    D_801FBA70[0][0] = 0x140;
    D_801FBA70[0][1] = 0x9F;
    D_801FBA70[1][0] = -0xC0;
    D_801FBA70[1][1] = 0x42;
    D_801FBA90[0][0] = 0x140;
    D_801FBA90[0][1] = 0xB1;
    D_801FBA90[1][0] = -0xC0;
    D_801FBA90[1][1] = 0x30;
    func_80014C08(10);
    D_801FC458 = 0;
    func_80014A48(task);
}

char *strcat(char *dst, const char *src);

void func_801EDD88(ListWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 player;
    s32 i;
    s32 deck;
    PresetDeck *decks;
    char buf[64];

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    player = w->player;
    y++;
    if (D_801FC454->unk504[player] != 0) {
        decks = (PresetDeck *)(((SessionData *)D_8006E054)->npcDeckFile + 8);
        for (i = 0; i < D_801FBAB0[player].nrows; i++) {
            if (i < w->window.view.y / D_801FBAB0[player].rowH) {
                continue;
            }
            if ((w->window.view.y + w->window.rect.h) / D_801FBAB0[player].rowH < i) {
                break;
            }
            y = w->window.originY + i * D_801FBAB0[player].rowH;
            y++;
            deck = D_801FC454->deckIds[player][i];
            if (deck < 3) {
                strcpy(buf, (char *)PLAYER_DATA(player).savedDecks[deck].unk1);
                strcat(buf, " Deck");
                drawText(x + 2, y, (s32)buf, 7, z);
            } else {
                strcpy(buf, decks[deck - 3].name);
                strcat(buf, " Deck");
                drawText(x + 2, y, (s32)buf, 5, z);
            }
        }
        updateMenuCursor(&D_801FBAB0[player]);
    } else {
        for (i = 0; i < 3; i++) {
            drawIcon(x, y + i * 14, 0, i + 7, z);
            if (PLAYER_DATA(player).savedDecks[i].inUse) {
                strcpy(buf, (char *)PLAYER_DATA(player).savedDecks[i].unk1);
                strcat(buf, " Deck");
            } else {
                strcpy(buf, "Unused Deck");
            }
            drawText(x + 0xE, y + i * 14, (s32)buf, 7, z);
        }
        drawIcon(x, y + 0x2A, 0, 10, z);
        drawText(x + 0xE, y + 0x2A, (s32)"Choose from List", 7, z);
    }
}

extern POLY_G4 D_801FC464[2][2][3];
extern TILE D_801FC614[2][2][4];
extern DR_MODE D_801FC714[2][2];
#define setXY0(p, _x0, _y0) (p)->x0 = _x0, (p)->y0 = _y0
#define setDrawTPage(p, dfe, dtd, tpage) (setlen(p, 1), ((u32 *)(p))[1] = _get_mode(dfe, dtd, tpage))

void func_801EE170(s32 x, s32 y, s32 player, s32 z) {
    u8 counts[6];
    u8 bars[4];
    PresetDeck *decks;
    s32 i;
    s32 specialty;
    s32 level;
    s32 deck;
    s32 cx;
    s32 cy;

    decks = (PresetDeck *)(((SessionData *)D_8006E054)->npcDeckFile + 8);
    cx = 0x22;
    cy = 0x1F;
    for (i = 0; i < 6; i++) {
        counts[i] = 2;
    }
    for (i = 0; i < 3; i++) {
        bars[i] = 0;
    }
    deck = D_801FC454->deckIds[player][D_801FBAB0[player].row];
    for (i = 0; i < 30; i++) {
        if (deck < 3) {
            specialty = getCardSpecialty(PLAYER_DATA(player).savedDecks[deck].cards[i].id);
            level = getCardLevel(PLAYER_DATA(player).savedDecks[deck].cards[i].id);
        } else {
            specialty = getCardSpecialty(decks[deck - 3].cards[i]);
            level = getCardLevel(decks[deck - 3].cards[i]);
        }
        switch (specialty) {
        case 0:
            counts[0]++;
            break;
        case 1:
            counts[3]++;
            break;
        case 2:
            counts[4]++;
            break;
        case 3:
            counts[2]++;
            break;
        case 4:
            counts[1]++;
            break;
        case 5:
        case 6:
            counts[5]++;
            break;
        }
        switch (level) {
        case 0:
            bars[0] += 2;
            break;
        case 2:
            bars[1] += 2;
            break;
        case 3:
            bars[2] += 2;
            break;
        case 4:
        case 5:
            bars[3] += 2;
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        initPrimByType(9, &D_801FC464[player][FRAME_BUFFER_INDEX][i], 0, 0);
        setRGB1(&D_801FC464[player][FRAME_BUFFER_INDEX][i], 0xFF, 0xFF, 0xFF);
        D_801FC464[player][FRAME_BUFFER_INDEX][i].x1 = x + cx;
        D_801FC464[player][FRAME_BUFFER_INDEX][i].y1 = y + cy;
    }
    setRGB0(&D_801FC464[player][FRAME_BUFFER_INDEX][0], 0xFF, 0, 0);
    setRGB2(&D_801FC464[player][FRAME_BUFFER_INDEX][0], 0xFF, 0xFF, 0);
    setRGB3(&D_801FC464[player][FRAME_BUFFER_INDEX][0], 0, 0, 0);
    setRGB0(&D_801FC464[player][FRAME_BUFFER_INDEX][1], 0, 0, 0);
    setRGB2(&D_801FC464[player][FRAME_BUFFER_INDEX][1], 0, 0xFF, 0xFF);
    setRGB3(&D_801FC464[player][FRAME_BUFFER_INDEX][1], 0, 0xFF, 0);
    setRGB0(&D_801FC464[player][FRAME_BUFFER_INDEX][2], 0, 0xFF, 0);
    setRGB2(&D_801FC464[player][FRAME_BUFFER_INDEX][2], 0xFF, 0xFF, 0xFF);
    setRGB3(&D_801FC464[player][FRAME_BUFFER_INDEX][2], 0xFF, 0, 0);
    D_801FC464[player][FRAME_BUFFER_INDEX][0].x0 = x + cx + rsin(0) * counts[0] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][0].y0 = y + cy + rcos(0) * counts[0] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][0].x2 = x + cx + rsin(0x2AA) * counts[1] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][0].y2 = y + cy + rcos(0x2AA) * counts[1] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][0].x3 = x + cx + rsin(0x554) * counts[2] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][0].y3 = y + cy + rcos(0x554) * counts[2] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][1].x0 = x + cx + rsin(0x554) * counts[2] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][1].y0 = y + cy + rcos(0x554) * counts[2] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][1].x2 = x + cx + rsin(0x7FE) * counts[3] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][1].y2 = y + cy + rcos(0x7FE) * counts[3] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][1].x3 = x + cx + rsin(0xAA8) * counts[4] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][1].y3 = y + cy + rcos(0xAA8) * counts[4] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][2].x0 = x + cx + rsin(0xAA8) * counts[4] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][2].y0 = y + cy + rcos(0xAA8) * counts[4] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][2].x2 = x + cx + rsin(0xD52) * counts[5] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][2].y2 = y + cy + rcos(0xD52) * counts[5] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][2].x3 = x + cx + rsin(0) * counts[0] / 4096;
    D_801FC464[player][FRAME_BUFFER_INDEX][2].y3 = y + cy + rcos(0) * counts[0] / 4096;
    drawIcon(x - 6 + cx + rsin(0) * 26 / 4096, y - 6 + cy + rcos(0) * 26 / 4096, 0, 0, z);
    drawIcon(x - 6 + cx + rsin(0x2AA) * 26 / 4096, y - 6 + cy + rcos(0x2AA) * 26 / 4096, 0, 4, z);
    drawIcon(x - 6 + cx + rsin(0x554) * 26 / 4096, y - 6 + cy + rcos(0x554) * 26 / 4096, 0, 3, z);
    drawIcon(x - 6 + cx + rsin(0x7FE) * 26 / 4096, y - 6 + cy + rcos(0x7FE) * 26 / 4096, 0, 1, z);
    drawIcon(x - 6 + cx + rsin(0xAA8) * 26 / 4096, y - 6 + cy + rcos(0xAA8) * 26 / 4096, 0, 2, z);
    drawIcon(x - 6 + cx + rsin(0xD52) * 26 / 4096, y - 6 + cy + rcos(0xD52) * 26 / 4096, 0, 5, z);
    for (i = 0; i < 3; i++) {
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &D_801FC464[player][FRAME_BUFFER_INDEX][i]);
    }
    func_801ED65C(x + cx - 0x16, y + cy - 0x1A, 0x3E1, 0x19A, 0x2C, 0x32, 0x3F0, 0x1FD, 0, 0, 0, 0x80, z);
    drawIcon(x + 0x44, y + 0x34, 1, 0x10, z);
    drawIcon(x + 0x50, y + 0x34, 1, 0x12, z);
    drawIcon(x + 0x5C, y + 0x34, 1, 0x13, z);
    drawIcon(x + 0x68, y + 0x34, 1, 5, z);
    setRGB0(&D_801FC614[player][FRAME_BUFFER_INDEX][0], 0xFF, 0xFF, 0);
    setRGB0(&D_801FC614[player][FRAME_BUFFER_INDEX][1], 0xFF, 0, 0);
    setRGB0(&D_801FC614[player][FRAME_BUFFER_INDEX][2], 0, 0, 0xFF);
    setRGB0(&D_801FC614[player][FRAME_BUFFER_INDEX][3], 0xFF, 0xFF, 0xFF);
    for (i = 0; i < 4; i++) {
        func_800678C4(&D_801FC614[player][FRAME_BUFFER_INDEX][i]);
        setXY0(&D_801FC614[player][FRAME_BUFFER_INDEX][i], x + 0x44 + i * 12, y - (bars[i] - 0x32));
        setWH(&D_801FC614[player][FRAME_BUFFER_INDEX][i], 8, bars[i]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &D_801FC614[player][FRAME_BUFFER_INDEX][i]);
    }
    setDrawTPage(&D_801FC714[player][FRAME_BUFFER_INDEX], 0, 0, 0);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &D_801FC714[player][FRAME_BUFFER_INDEX]);
}

void func_801EFB78(ListWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 player;
    s32 deck;
    s32 wins;
    s32 losses;
    char buf[40];

    x = w->window.originX;
    y = w->window.originY + 1;
    z = w->window.z;
    player = w->player;
    func_801EE170(x, y, player, z);
    deck = D_801FC454->deckIds[player][D_801FBAB0[player].row];
    if (deck < 3) {
        wins = ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[deck].unk108[1];
        losses = ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[deck].unk108[2];
    } else {
        wins = ((PlayerProfile *)PLAYER_PROFILES)[player].opponentDeckFlags[deck - 3] & 0x3FFF;
        losses = ((PlayerProfile *)PLAYER_PROFILES)[player].unkBFE[deck - 3];
    }
    sprintf(buf, "*s0%3d *c6Wins *c7%3d *c6Losses", wins, losses);
    drawText(x + 6, y + 0x3E, (s32)buf, 7, z);
}

extern Rect16 D_801FBB08[];

void func_801EFCEC(s32 player) {
    s32 i;
    s32 n;

    markBuildableOpponentDecks(player);
    for (i = 0; i < 0xA2; i++) {
        D_801FC454->deckIds[player][i] = 0xFFFF;
    }
    n = 0;
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).savedDecks[i].inUse) {
            D_801FC454->deckIds[player][n++] = i;
        }
    }
    for (i = 0; i < 0x9F; i++) {
        if (PLAYER_DATA(player).opponentDeckFlags[i] & 0x4000) {
            D_801FC454->deckIds[player][n++] = i + 3;
        }
    }
    D_801FBAB0[player].nrows = n;
    D_801FC454->lists[player].window.view.h = n * D_801FBAB0[player].rowH;
    D_801FC454->unk504[player] = 1;
    animateWindowTo(&D_801FC454->frames[player].window, &D_801FBB08[player]);
    playSoundEffect(0xA3);
}

void func_801EFEFC(s32 i) {
    D_801FBAB0[i].nrows = 3;
    D_801FBAB0[i].row = 0;
    D_801FC454->lists[i].window.scroll[3] = 0;
    D_801FC454->lists[i].window.view.y = 0;
    D_801FC454->lists[i].window.view.h = D_801FBAB0[0].rowH * 3;
    D_801FC454->unk504[i] = 0;
    animateWindowTo(&D_801FC454->frames[i].window, (Rect16 *)-1);
    playSoundEffect(0xA4);
}

void func_801EFF98(void) {
    drawWindow(&D_801FC454->lists[0].window, func_801EDD88, 10);
    drawWindow(&D_801FC454->frames[0].window, func_801EFB78, 10);
    if (D_801FC454->unk770 == 0) {
        drawWindow(&D_801FC454->lists[1].window, func_801EDD88, 10);
        drawWindow(&D_801FC454->frames[1].window, func_801EFB78, 10);
    }
}

#define DECK_CHOICE(p) (*(s8 *)&PLAYER_DATA(p).unk30[4])

void func_801F003C(s32 isVersus, s32 match) {
    s32 i;
    s32 done;
    u16 pressed;

    D_801FC454 = allocTaskHeapBlock(0x778);
    func_800149B8(0, -1, 0, 0x800, func_801EDA84, isVersus, match, getCurrentTaskId(), 0);
    D_801FC454->unk504[0] = 0;
    D_801FC454->unk504[1] = 0;
    D_801FC454->unk770 = isVersus;
    if ((DUEL->tutorial == 0 && ((SessionData *)D_8006E054)->npcDeckIndex[0] == -1) || isVersus == 0) {
        ((SessionData *)D_8006E054)->npcDeckIndex[0] = -1;
        ((SessionData *)D_8006E054)->npcDeckIndex[1] = -1;
        if (isVersus != 0) {
            openMenu(&D_801FBAB0[0], &D_801FC454->lists[0].window, &D_801FC454->highlights[0], (Bytes4 *)-1);
            D_801FC454->lists[0].player = 0;
            D_801FC454->lists[0].window.labelPalette = 7;
            D_801FC454->lists[0].window.label = (s32) "PLAYER DECK LIST";
            openWindow(&D_801FC454->frames[0], &D_801FBB08[0], -1, (s16 *)-1, 8, 0x55, 0x80, 8);
            animateWindowTo(&D_801FC454->frames[0].window, (Rect16 *)-1);
            D_801FC454->frames[0].window.labelPalette = 8;
            D_801FC454->frames[0].player = 0;
            D_801FC454->frames[0].window.label = (s32) "PLAYER DECK INFO.";
            done = 2;
        } else {
            for (i = 0, done = 0; i < 2; i++) {
                openMenu(&D_801FBAB0[i], &D_801FC454->lists[i].window, &D_801FC454->highlights[i], (Bytes4 *)-1);
                D_801FC454->lists[i].player = i;
                openWindow(&D_801FC454->frames[i], &D_801FBB08[i], -1, (s16 *)-1, 8, 0x55, 0x80, 8);
                animateWindowTo(&D_801FC454->frames[i].window, (Rect16 *)-1);
                D_801FC454->frames[i].window.labelPalette = 8;
                D_801FC454->frames[i].player = i;
                if (i == 0) {
                    D_801FC454->lists[0].window.label = (s32) "1P DECK LIST";
                    D_801FC454->frames[0].window.label = (s32) "1P DECK INFO.";
                } else {
                    D_801FC454->lists[i].window.label = (s32) "2P DECK LIST";
                    D_801FC454->frames[i].window.label = (s32) "2P DECK INFO.";
                }
                D_801FC454->lists[i].window.labelPalette = 7;
            }
        }
        playSoundEffect(0xA3);
        addFrameCallback((s32)func_801EFF98);
        func_80014C08(0x10);
        do {
            func_80014C08(FRAME_INTERVAL);
            if (!(done & 1)) {
                if (D_801FC454->unk504[0] != 0) {
                    if (PAD_STATES[0]->pressed & 0x40) {
                        i = D_801FC454->deckIds[0][D_801FBAB0[0].row];
                        if (((SessionData *)D_8006E054)->unk1010[0x12] != 0) {
                            if (i < 3) {
                                if (((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] == 0) {
                                    playSoundEffect(0xA0);
                                    initDialog(D_801FC454->dialog, "This Deck can't be used in this Arena.", 0);
                                    runDialog(D_801FC454->dialog);
                                } else {
                                    DECK_CHOICE(0) = i;
                                    done |= 1;
                                }
                            } else {
                                playSoundEffect(0xA0);
                                initDialog(D_801FC454->dialog, "Base Deck can't be used in this Arena.", 0);
                                runDialog(D_801FC454->dialog);
                            }
                        } else {
                            if (i < 3) {
                                DECK_CHOICE(0) = i;
                            } else {
                                ((SessionData *)D_8006E054)->npcDeckIndex[0] = i - 3;
                                DECK_CHOICE(0) = -1;
                            }
                            done |= 1;
                        }
                    } else if (PAD_STATES[0]->pressed & 0x10) {
                        func_801EFEFC(0);
                    }
                } else {
                    pressed = PAD_STATES[0]->pressed;
                    if (pressed & 0xF0) {
                        if (pressed & 0x20) {
                            i = 0;
                        } else if (PAD_STATES[0]->pressed & 0x10) {
                            i = 1;
                        } else if (PAD_STATES[0]->pressed & 0x40) {
                            i = 2;
                        } else {
                            i = 3;
                        }
                        if (i < 3) {
                            if (((SessionData *)D_8006E054)->unk1010[0x12] != 0 &&
                                ((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] == 0) {
                                playSoundEffect(0xA0);
                                initDialog(D_801FC454->dialog, "This Deck can't be used in this Arena.", 0);
                                runDialog(D_801FC454->dialog);
                                i = -1;
                            }
                            if (i != -1 && PLAYER_DATA(0).savedDecks[i].inUse) {
                                DECK_CHOICE(0) = i;
                                done |= 1;
                            }
                        } else {
                            func_801EFCEC(0);
                        }
                    }
                }
                if (done & 1) {
                    animateWindowTo(&D_801FC454->lists[0].window, (Rect16 *)-1);
                    animateWindowTo(&D_801FC454->frames[0].window, (Rect16 *)-1);
                    playSoundEffect(0xA0);
                }
            }
            if (isVersus == 0 && !(done & 2)) {
                if (D_801FC454->unk504[1] != 0) {
                    if (PAD_STATES[1]->pressed & 0x40) {
                        playSoundEffect(0xA0);
                        i = D_801FC454->deckIds[1][D_801FBAB0[1].row];
                        if (i < 3) {
                            DECK_CHOICE(1) = i;
                        } else {
                            ((SessionData *)D_8006E054)->npcDeckIndex[1] = i - 3;
                            DECK_CHOICE(1) = -1;
                        }
                        done |= 2;
                    } else if (PAD_STATES[1]->pressed & 0x10) {
                        func_801EFEFC(1);
                    }
                } else {
                    pressed = PAD_STATES[1]->pressed;
                    if (pressed & 0xF0) {
                        if (pressed & 0x20) {
                            if (PLAYER_DATA(1).savedDecks[0].inUse) {
                                DECK_CHOICE(1) = 0;
                                done |= 2;
                            }
                        } else if (PAD_STATES[1]->pressed & 0x10) {
                            if (PLAYER_DATA(1).savedDecks[1].inUse) {
                                DECK_CHOICE(1) = 1;
                                done |= 2;
                            }
                        } else if (PAD_STATES[1]->pressed & 0x40) {
                            if (PLAYER_DATA(1).savedDecks[2].inUse) {
                                DECK_CHOICE(1) = 2;
                                done |= 2;
                            }
                        } else {
                            func_801EFCEC(1);
                        }
                    }
                }
                if (done & 2) {
                    animateWindowTo(&D_801FC454->lists[1].window, (Rect16 *)-1);
                    animateWindowTo(&D_801FC454->frames[1].window, (Rect16 *)-1);
                    playSoundEffect(0xA0);
                }
            }
        } while (done != 3);
        func_80014C08(0x10);
        if (((SessionData *)D_8006E054)->npcDeckIndex[0] == -1) {
            *(PlayerDeck *)DUEL_PLAYERS[0] = PLAYER_DATA(0).savedDecks[DECK_CHOICE(0)];
            linkDeckCardData(0, (PlayerDeck *)DUEL_PLAYERS[0]);
        } else {
            loadPresetDeckForPlayer(0);
        }
        if (isVersus == 0) {
            if (((SessionData *)D_8006E054)->npcDeckIndex[1] == -1) {
                *(PlayerDeck *)DUEL_PLAYERS[1] = PLAYER_DATA(1).savedDecks[DECK_CHOICE(1)];
                linkDeckCardData(1, (PlayerDeck *)DUEL_PLAYERS[1]);
            } else {
                loadPresetDeckForPlayer(1);
            }
        }
    } else {
        for (i = 0; i < 2; i++) {
            DECK_CHOICE(i) = -1;
        }
    }
    removeFrameCallback((s32)func_801EFF98);
    markDeckCardsSeen(0);
    freeHeapBlock(((SessionData *)D_8006E054)->npcDeckFile);
    func_80014C08(0x1E);
    while (D_801FC458 != 0) {
        func_80014C08(FRAME_INTERVAL);
    }
    func_80014C08(2);
    freeHeapBlock(D_801FC454);
    func_80014C08(2);
}


#define setXYWH(p, _x0, _y0, _w, _h)                                                            \
    (p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0), (p)->x2 = (_x0), \
    (p)->y2 = (_y0) + (_h), (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h)
#define setUVWH(p, _u0, _v0, _w, _h)                                                            \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u0) + (_w), (p)->v1 = (_v0), (p)->u2 = (_u0), \
    (p)->v2 = (_v0) + (_h), (p)->u3 = (_u0) + (_w), (p)->v3 = (_v0) + (_h)

void func_801F0A30(void) {
    char buf[64];
    VersusPrims *prims;
    s32 i;

    prims = (VersusPrims *)CURRENT_FRAME_BUFFER->primSlots[15];
    if (D_801FC454->introState != 0) {
        switch (D_801FC454->introState) {
        case 1:
            D_801FC454->unk510 = 0;
            D_801FC454->introZoom = 0;
            D_801FC454->introBrightness = 0x80;
            D_801FC454->introState++;
        case 2:
            D_801FC454->introZoom += 10;
            if (D_801FC454->introZoom > 150) {
                D_801FC454->introState++;
            }
            break;
        case 3:
            D_801FC454->introZoom -= 5;
            if (D_801FC454->introZoom < 100) {
                D_801FC454->introZoom = 100;
                D_801FC454->introState++;
            }
            break;
        case 4:
            D_801FC454->introBrightness -= 12;
            if (D_801FC454->introBrightness <= 0) {
                D_801FC454->introBrightness = 0;
                D_801FC454->introState = 0;
            }
            break;
        }
        setRGB0(&prims->intro, D_801FC454->introBrightness, D_801FC454->introBrightness, D_801FC454->introBrightness);
        setXYWH(&prims->intro, 160 - (D_801FC454->introZoom * 64) / 100, 120 - (D_801FC454->introZoom * 64) / 100,
                (D_801FC454->introZoom * 128) / 100, (D_801FC454->introZoom * 128) / 100);
        setUVWH(&prims->intro, 0x60, 0x28, 0x80, 0x80);
        prims->intro.tpage = 0x26;
        prims->intro.clut = 0x3E19;
        addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->intro);
    }
    i = (D_801FC454->barH * 192) / 40;
    setRGB0(&prims->fade, i, i, i);
    setXYWH(&prims->fade, 0, 120 - D_801FC454->barH, 320, D_801FC454->barH * 2);
    setlen(&prims->fadeMode, 1);
    prims->fadeMode.code[0] = 0xE1000040;
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->fade);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->fadeMode);
    setRGB0(&prims->bars[0], 0xFF, 0xFF, 0);
    setXYWH(&prims->bars[0], 160 - D_801FC454->barW, 119 - D_801FC454->barH, D_801FC454->barW * 2, 1);
    setRGB0(&prims->bars[1], 0xFF, 0xFF, 0);
    setXYWH(&prims->bars[1], 160 - D_801FC454->barW, D_801FC454->barH + 120, D_801FC454->barW * 2, 1);
    setlen(&prims->barMode, 1);
    prims->barMode.code[0] = 0xE1000000;
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->bars[0]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->bars[1]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->barMode);
    if (D_801FC454->logoShown != 0) {
        setRGB0(&prims->logo, 0x80, 0x80, 0x80);
        setXYWH(&prims->logo, 160 - (D_801FC454->logoScale * 32) / 100, 120 - D_801FC454->logoScale / 5,
                (D_801FC454->logoScale * 64) / 100, (D_801FC454->logoScale * 40) / 100);
        setUVWH(&prims->logo, 0x60, 0, 0x40, 0x28);
        prims->logo.tpage = 6;
        prims->logo.clut = 0x3E18;
        addPrim(&CURRENT_FRAME_BUFFER->ot[1], &prims->logo);
    }
    for (i = 0; i < 2; i++) {
        func_801ED334(&D_801FC454->cards[i], 1, &prims->cards[i]);
    }
    i = (((PlayerProfile *)PLAYER_PROFILES)->playTime * 8) % 256;
    if (i >= 0x80) {
        D_801FC454->pulse = 0x17F - i;
    } else {
        D_801FC454->pulse = i + 0x80;
    }
    i = 0;
    func_801ED65C(D_801FBA50[0][0], D_801FBA50[0][1], 0x2C0, 0x1C0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 1);
    func_801ED65C(D_801FBA30[0][0] - 8, D_801FBA30[0][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, D_801FC454->pulse, 2);
    func_801ED65C(D_801FBA30[0][0] + 0x78, D_801FBA30[0][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, D_801FC454->pulse, 2);
    func_801ED65C(D_801FBA30[0][0] - 0x20, D_801FBA30[0][1], 0x180, 0, 0x20, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
    func_801ED65C(D_801FBA30[0][0] + 0x80, D_801FBA30[0][1], 0x188, 0, 8, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
    func_801ED8BC(D_801FBA70[0][0], D_801FBA70[0][1], (char *)DUEL_PLAYERS[i] + 1);
    func_801ED968(D_801FBA90[0][0], D_801FBA90[0][1], D_801FC454->wins[i], D_801FC454->losses[i]);
    func_801ED65C(D_801FBA30[0][0], D_801FBA30[0][1], 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
    i = 1;
    func_801ED65C(D_801FBA50[1][0], D_801FBA50[1][1], 0x2C0, 0x1E0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 1);
    func_801ED65C(D_801FBA30[1][0] - 6, D_801FBA30[1][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, D_801FC454->pulse, 2);
    func_801ED65C(D_801FBA30[1][0] + 0x7A, D_801FBA30[1][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, D_801FC454->pulse, 2);
    func_801ED65C(D_801FBA30[1][0] - 8, D_801FBA30[1][1], 0x18A, 0, 8, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
    func_801ED65C(D_801FBA30[1][0] + 0x80, D_801FBA30[1][1], 0x18C, 0, 0x20, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
    func_801ED8BC(D_801FBA90[1][0], D_801FBA90[1][1], (char *)DUEL_PLAYERS[i] + 1);
    func_801ED968(D_801FBA70[1][0], D_801FBA70[1][1], D_801FC454->wins[i], D_801FC454->losses[i]);
    func_801ED65C(D_801FBA30[1][0], D_801FBA30[1][1], 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
    if (D_801FC454->timer > 0x20) {
        if (D_801FC454->cards[D_801FC454->chosen - 2].u != 0) {
            func_801ED65C(D_801FBA30[0][0] + 0x4C, D_801FBA30[0][1] + 0x4C, 0x1B4, 0, 0x30, 0x20, 0x190, 0xFD, 0, 1, 1, 0x80, 3);
            func_801ED65C(D_801FBA30[1][0] + 4, D_801FBA30[1][1] + 4, 0x1A8, 0, 0x30, 0x20, 0x190, 0xFC, 0, 1, 1, 0x80, 3);
        } else {
            func_801ED65C(D_801FBA30[0][0] + 0x4C, D_801FBA30[0][1] + 0x4C, 0x1A8, 0, 0x30, 0x20, 0x190, 0xFC, 0, 1, 1, 0x80, 3);
            func_801ED65C(D_801FBA30[1][0] + 4, D_801FBA30[1][1] + 4, 0x1B4, 0, 0x30, 0x20, 0x190, 0xFD, 0, 1, 1, 0x80, 3);
        }
    }
}

/* moves *cur toward *target by step without overshooting */
#define STEP_TOWARD(cur, target, step) \
    if ((cur) < (target)) {            \
        (cur) += (step);               \
        if ((target) < (cur))          \
            (cur) = (target);          \
    } else {                           \
        (cur) -= (step);               \
        if ((cur) < (target))          \
            (cur) = (target);          \
    }

#include "dcb/fade.h"

extern u8 D_801FBB18[30];
void loadDuelCardGraphics();
void func_801F8DB4(void *ptr);
void func_801F8DF0();

/* libgte's setVector */
#define setVector(v, _x, _y, _z) (v)->vx = (_x), (v)->vy = (_y), (v)->vz = (_z)

void func_801F1AA8(s32 mode, s32 deckId) {
    s32 i;
    s32 j;
    s32 frame;
    s32 step;
    s32 k;
    char buf[64];

    D_801FC454 = allocTaskHeapBlock(sizeof(DeckScreen));
    waitForMusicChange();
    if (mode != 0) {
        loadMusicTrack(0, ((u8 *)D_8006E054)[0x70], 0x7F);
        loadMusicTrack(1, ((u8 *)D_8006E054)[0x71], 0x64);
    } else {
        loadMusicTrack(0, rand() % 2 + 0x8F, 0x7F);
        loadMusicTrack(1, rand() % 2 + 0x93, 0x64);
    }
    playLoadedMusic(0);
    frame = 0;
    func_800149B8(0, -1, 0, 0x1000, loadDuelCardGraphics, mode, getCurrentTaskId(), 0, 0);
    if (mode != 0) {
        k = func_800471F4(deckId);
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleWins = ((PlayerProfile *)PLAYER_PROFILES)->unk9A4[k];
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleLosses = ((PlayerProfile *)PLAYER_PROFILES)->unk888[k];
    }
    for (i = 0; i < 2; i++) {
        if (mode != 0) {
            D_801FC454->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
            D_801FC454->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
        } else {
            D_801FC454->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
            D_801FC454->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
        }
    }
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[15] = (s32)&D_801FC454->prims[i];
        initPrimByType(0xC, &D_801FC454->prims[i].intro, 1, 0);
        initPrimByType(0xC, &D_801FC454->prims[i].logo, 1, 0);
        initPrimByType(8, &D_801FC454->prims[i].fade, 1, 0);
        for (j = 0; j < 2; j++) {
            initPrimByType(8, &D_801FC454->prims[i].bars[j], 0, 0);
        }
    }
    j = rand() % 2;
    for (i = 0; i < 2; i++) {
        D_801FC454->cards[i].code = 0x2C;
        setRGB0(&D_801FC454->cards[i], 0x80, 0x80, 0x80);
        k = i ^ j;
        if (((DuelK *)D_801D8340)->tutorial) {
            k = 1;
        }
        D_801FC454->cards[i].tpage = ((k * 10 + 0x180) & 0x3FF) >> 6;
        D_801FC454->cards[i].clut = ((k + 0xFA) << 6) | 0x19;
        D_801FC454->cards[i].u = (k * 10 + 0x180) % 64 * 4;
        D_801FC454->cards[i].v = 0x70;
        setVector(&D_801FC454->cards[i].pos, i * 400 - 200, 0, 0);
        setVector(&D_801FC454->cards[i].rot, 0x2000, 0x2800 - (i << 12), 0x2000);
        PLAYER(i)->shufflePasses = 0;
    }
    D_801FC454->unk770 = mode;
    D_801FC454->deckId = deckId;
    D_801FC454->introState = 0;
    D_801FC454->logoShown = 0;
    D_801FC454->logoScale = 0;
    D_801FC454->choice = 0;
    D_801FC454->pulse = 0;
    D_801FC454->chosen = 0;
    D_801FC454->timer = 0;
    D_801FC454->barW = 0;
    D_801FC454->barH = 0;
    D_801FC454->cursor = (s16 *)func_801F8998(1, 0x12, 0x16, 6, 1);
    addFrameCallback((s32)func_801F0A30);
    step = ((DuelK *)D_801D8340)->tutorial;
    do {
        func_80014C08(FRAME_INTERVAL);
        frame++;
        for (i = 0; i < 2; i++) {
            if (frame > 0) {
                STEP_TOWARD(D_801FBA30[i][1], D_801FBA30[i][3], 12);
            }
            if (frame > 20) {
                STEP_TOWARD(D_801FBA30[i][0], D_801FBA30[i][2], 8);
            }
            if (frame > 30) {
                STEP_TOWARD(D_801FBA50[i][0], D_801FBA50[i][2], 24);
            }
            if (frame > 40) {
                STEP_TOWARD(D_801FBA90[i][0], D_801FBA90[i][2], 24);
            }
            if (frame > 50) {
                STEP_TOWARD(D_801FBA70[i][0], D_801FBA70[i][2], 24);
            }
        }
        if (frame == 8) {
            D_801FC454->introState = 1;
            playSoundEffect(0x83);
        }
        if (frame == 26) {
            playSoundEffect(0x8D);
            playSoundEffect(0x8D);
        }
        if (frame == 30) {
            playSoundEffect(0xA7);
        }
        if (frame == 40) {
            playSoundEffect(0xA7);
        }
        if (frame == 50) {
            playSoundEffect(0xA7);
        }
        if (frame > 70) {
            if (D_801FC454->timer < 60) {
                if ((D_801FC454->barH += 2) > 35) {
                    D_801FC454->barH = 35;
                }
            } else {
                if ((D_801FC454->barH -= 2) < 0) {
                    D_801FC454->barH = 0;
                }
            }
        }
        if (frame > 20) {
            if ((D_801FC454->barW += 16) > 160) {
                D_801FC454->barW = 160;
            }
        }
        if (D_801FC454->timer == 80) {
            D_801FC454->introState = 1;
            playSoundEffect(0x83);
        }
        if (D_801FC454->timer > 80) {
            D_801FC454->logoShown = 1;
            if (D_801FC454->timer < 92) {
                D_801FC454->logoScale += 10;
            } else {
                if ((D_801FC454->logoScale -= 5) < 100) {
                    D_801FC454->logoScale = 100;
                }
            }
        }
        if (frame == 80) {
            playSoundEffect(0xA5);
        }
        if (D_801FC454->timer == 51) {
            playSoundEffect(0xA5);
        }
        if (frame > 80) {
            if (D_801FC454->chosen >= 2) {
                i = D_801FC454->chosen - 2;
                if (D_801FC454->cards[i].rot.vy != 0x2000) {
                    if (D_801FC454->cards[i].rot.vy < 0x2000) {
                        D_801FC454->cards[i].rot.vy += 0x40;
                    } else {
                        D_801FC454->cards[i].rot.vy -= 0x40;
                    }
                }
                if (D_801FC454->timer >= 50) {
                    if (step == 2) {
                        func_801EA8B4(0x34, "It looks like I go first!");
                        PAD_INPUT_ENABLED = 1;
                        step = 3;
                    }
                    for (i = 0; i < 2; i++) {
                        if (abs(D_801FC454->cards[i].pos.vx) >= 200) {
                            if (D_801FC454->cards[i].pos.vx < 0) {
                                D_801FC454->cards[i].pos.vx = -200;
                            } else {
                                D_801FC454->cards[i].pos.vx = 200;
                            }
                        } else if (D_801FC454->cards[i].pos.vx < 0) {
                            D_801FC454->cards[i].pos.vx -= 10;
                        } else {
                            D_801FC454->cards[i].pos.vx += 10;
                        }
                    }
                }
            } else {
                for (i = 0; i < 2; i++) {
                    if (abs(D_801FC454->cards[i].pos.vx) <= 80) {
                        if (D_801FC454->cards[i].pos.vx < 0) {
                            D_801FC454->cards[i].pos.vx = -80;
                        } else {
                            D_801FC454->cards[i].pos.vx = 80;
                        }
                        D_801FC454->chosen = 1;
                    } else if (D_801FC454->cards[i].pos.vx < 0) {
                        D_801FC454->cards[i].pos.vx += 8;
                    } else {
                        D_801FC454->cards[i].pos.vx -= 8;
                    }
                }
            }
        }
        if (D_801FC454->chosen == 1) {
            if (step == 1) {
                func_801EA8B4(0x34, "Let's decide who gets 1st Turn.\nChoose a Card with the directional\nbuttons and press the *b2 button.");
                PAD_INPUT_ENABLED = 1;
                step = 2;
            }
            if ((u16)PAD_STATES[0]->pressed & 0x2000) {
                if (D_801FC454->choice == 0) {
                    playSoundEffect(0xA2);
                    D_801FC454->choice = 1;
                }
            }
            if ((u16)(PAD_STATES[0]->pressed & 0x8000)) {
                if (D_801FC454->choice == 1) {
                    playSoundEffect(0xA2);
                    D_801FC454->choice = 0;
                }
            }
            func_801F8DF0(D_801FC454->cursor, D_801FC454->choice * 160 + 0x4F, 0x78);
            if (PAD_STATES[0]->pressed & 0x40) {
                playSoundEffect(0xA6);
                D_801FC454->chosen = D_801FC454->choice + 2;
                D_801FC454->timer = 1;
            }
        }
        if (D_801FC454->timer != 0) {
            D_801FC454->timer++;
        }
    } while (!DUEL_VRAM_READY || D_801FC454->timer < 181);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    func_80014C08(40);
    if (!((DuelK *)D_801D8340)->tutorial) {
        for (i = 0; i < 2; i++) {
            PLAYER(i)->shufflePasses += 600;
            shuffleOnlineDeck(i);
        }
        if (deckId == 0x8C) {
            for (i = 0; i < 30; i++) {
                PLAYER(1)->onlineDeck[i] = D_801FBB18[i] + 0x1D;
            }
        }
    }
    if (D_801FC454->cards[D_801FC454->chosen - 2].u != 0) {
        ((u8 *)D_801D8340)[0x817] = 1;
    } else {
        ((u8 *)D_801D8340)[0x817] = 0;
    }
    removeFrameCallback((s32)func_801F0A30);
    func_801F8DB4(D_801FC454->cursor);
    func_80014C08(2);
    freeHeapBlock(D_801FC454);
    func_80014C08(2);
}


extern s32 D_801FC734;
extern char D_801DE41C[]; /* "B:\\WIN\\%3.3d.ARC", still in the INCLUDE_RODATA block below */

void func_801F2A40(s32 mode, s32 winner, s32 deckId) {
    char path[64];
    s32 scale;
    u32 *arc;
    s32 i;
    s32 frame;
    VersusPrims *prims;

    D_801FC734 = -1;
    if (mode == 0) {
        deckId = 999;
    }
    sprintf(path, D_801DE41C, deckId);
    i = 0;
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    D_801FC454 = allocTaskHeapBlock(sizeof(DeckScreen));
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[15] = (s32)&D_801FC454->prims[i];
    }
    D_801FC454->unk770 = mode;
    D_801FC454->deckId = deckId;
    if (mode != 0) {
        i = func_800471F4(deckId);
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleWins = ((PlayerProfile *)PLAYER_PROFILES)->unk9A4[i];
        ((PlayerProfile *)PLAYER_PROFILES)[1].battleLosses = ((PlayerProfile *)PLAYER_PROFILES)->unk888[i];
        if (!((DuelK *)D_801D8340)->tutorial && winner != 0) {
            loadMusicTrack(0, 0x96, 0x7F);
        } else {
            loadMusicTrack(0, 0x95, 0x7F);
        }
    } else {
        loadMusicTrack(0, 0x95, 0x7F);
    }
    playLoadedMusic(0);
    D_801FBA30[0][0] = 0x140;
    D_801FBA30[0][1] = 0x78;
    D_801FBA30[0][2] = 0;
    D_801FBA30[0][3] = 0x78;
    D_801FBA30[1][0] = -0x140;
    D_801FBA30[1][1] = 0;
    D_801FBA30[1][2] = 0;
    D_801FBA30[1][3] = 0;
    D_801FBA50[0][0] = 0x140;
    D_801FBA50[0][1] = 0xC3;
    D_801FBA50[1][0] = -*(s16 *)PLAYER(1)->unk118;
    D_801FBA50[1][1] = 0x10;
    D_801FBA50[1][2] = 0x138 - *(s16 *)PLAYER(1)->unk118;
    D_801FBA70[0][0] = 0x140;
    D_801FBA70[0][1] = 0x9F;
    D_801FBA70[1][0] = -0xC0;
    D_801FBA70[1][1] = 0x42;
    D_801FBA90[0][0] = 0x140;
    D_801FBA90[0][1] = 0xB1;
    D_801FBA90[1][0] = -0xC0;
    D_801FBA90[1][1] = 0x30;
    frame = 0;
    scale = 200;
    for (i = 0; i < 2; i++) {
        if (mode != 0) {
            if (winner == i) {
                D_801FC454->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    D_801FC454->wins[i]++;
                }
                D_801FC454->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
            } else {
                D_801FC454->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
                D_801FC454->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    D_801FC454->losses[i]++;
                }
            }
        } else {
            if (winner == i) {
                D_801FC454->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    D_801FC454->wins[i]++;
                }
                D_801FC454->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
            } else {
                D_801FC454->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
                D_801FC454->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    D_801FC454->losses[i]++;
                }
            }
        }
        if (D_801FC454->wins[i] >= 1000) {
            D_801FC454->wins[i] = 999;
        }
        if (D_801FC454->losses[i] >= 1000) {
            D_801FC454->losses[i] = 999;
        }
    }
    do {
        func_80014C08(FRAME_INTERVAL);
        frame++;
        prims = (VersusPrims *)CURRENT_FRAME_BUFFER->primSlots[15];
        if (!((DuelK *)D_801D8340)->tutorial && frame >= 56) {
            scale -= 8;
            if (scale < 100) {
                scale = 100;
            }
            initPrimByType(0xC, &prims->intro, 0, 0);
            setRGB0(&prims->intro, 0x80, 0x80, 0x80);
            if (winner == 0) {
                setXYWH(&prims->intro, D_801FBA30[1][0] - (s16)(scale * 64 / 100 - 74), D_801FBA30[1][1] - (s16)(scale * 56 / 100 - 60),
                        scale * 128 / 100, scale * 112 / 100);
            } else {
                setXYWH(&prims->intro, D_801FBA30[0][0] - (s16)(scale * 64 / 100 - 248), D_801FBA30[0][1] - (s16)(scale * 56 / 100 - 60),
                        scale * 128 / 100, scale * 112 / 100);
            }
            prims->intro.u0 = 0;
            prims->intro.v0 = 0;
            prims->intro.u1 = 0x80;
            prims->intro.v1 = 0;
            prims->intro.u2 = 0;
            prims->intro.v2 = 0x70;
            prims->intro.u3 = 0x80;
            prims->intro.v3 = 0x70;
            prims->intro.tpage = 8;
            prims->intro.clut = 0x3E98;
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &prims->intro);
        }
        for (i = 0; i < 2; i++) {
            if (frame > 0) {
                STEP_TOWARD(D_801FBA30[i][0], D_801FBA30[i][2], 16);
            }
            if (frame > 20) {
                STEP_TOWARD(D_801FBA50[i][0], D_801FBA50[i][2], 24);
            }
            if (frame > 30) {
                STEP_TOWARD(D_801FBA90[i][0], D_801FBA90[i][2], 24);
            }
            if (frame > 40) {
                STEP_TOWARD(D_801FBA70[i][0], D_801FBA70[i][2], 24);
            }
        }
        if (frame == 26) {
            playSoundEffect(0x8D);
            playSoundEffect(0x8D);
        }
        if (frame == 30) {
            playSoundEffect(0xA7);
        }
        if (frame == 40) {
            playSoundEffect(0xA7);
        }
        if (frame == 50) {
            playSoundEffect(0xA7);
        }
        if (!((DuelK *)D_801D8340)->tutorial && frame == 70) {
            playSoundEffect(0x83);
        }
        i = 0;
        func_801ED65C(D_801FBA50[i][0], D_801FBA50[i][1], 0x2C0, 0x1C0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 0);
        func_801ED8BC(D_801FBA70[i][0], D_801FBA70[i][1], (char *)DUEL_PLAYERS[i] + 1);
        func_801ED968(D_801FBA90[i][0], D_801FBA90[i][1], D_801FC454->wins[i], D_801FC454->losses[i]);
        func_801ED65C(D_801FBA30[i][0] + 0xB8, D_801FBA30[i][1] + 4, 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
        if (!((DuelK *)D_801D8340)->tutorial) {
            if (winner == 0) {
                func_801ED65C(D_801FBA30[i][0] + 8, D_801FBA30[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
            } else {
                func_801ED65C(D_801FBA30[i][0] + 8, D_801FBA30[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
            }
        }
        func_801ED65C(D_801FBA30[i][0], D_801FBA30[i][1], 0x180, 0x78, 0x100, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        func_801ED65C(D_801FBA30[i][0] + 0x100, D_801FBA30[i][1], 0x1C0, 0x78, 0x40, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        i = 1;
        func_801ED65C(D_801FBA50[i][0], D_801FBA50[i][1], 0x2C0, 0x1E0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 0);
        func_801ED8BC(D_801FBA90[i][0], D_801FBA90[i][1], (char *)DUEL_PLAYERS[i] + 1);
        func_801ED968(D_801FBA70[i][0], D_801FBA70[i][1], D_801FC454->wins[i], D_801FC454->losses[i]);
        func_801ED65C(D_801FBA30[i][0] + 8, D_801FBA30[i][1] + 4, 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
        if (!((DuelK *)D_801D8340)->tutorial) {
            if (winner != 0) {
                func_801ED65C(D_801FBA30[i][0] + 0x96, D_801FBA30[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
            } else {
                func_801ED65C(D_801FBA30[i][0] + 0x96, D_801FBA30[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
            }
        }
        func_801ED65C(D_801FBA30[i][0], D_801FBA30[i][1], 0x180, 0, 0x100, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        func_801ED65C(D_801FBA30[i][0] + 0x100, D_801FBA30[i][1], 0x1C0, 0, 0x40, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        if (frame > 30) {
            D_801FC734 = 1;
        }
    } while (frame < 100 || !(PAD_STATES[0]->pressed & 0x40));
    D_801FC734 = 0;
    func_80014C08(2);
    freeHeapBlock(D_801FC454);
    func_80014C08(2);
}

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DE41C);

void func_801F3BAC(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned Experience Points", 7, 0);
}

void func_801F84CC(s32 x, s32 y, s32 arg2, s32 arg3, s32 arg4, u16 arg5);

void func_801F3BE8(ExpWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 next;
    s32 attr;
    char buf[72];

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    if (D_801FC738->partnerShown[w->partner]) {
        attr = ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attr >> 4;
        func_801F84CC(x + 0x22, y + 1, w->partner * 20 + 0x2C0, 0x128, attr, w->clut);
        x += 0x6E;
        drawText(x, y + 1, (s32)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].name, 7, z);
        sprintf(buf, "RANK   \x0c\x07%2d", (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].level);
        drawLargeText(x, y + 0xE, (s32)buf, 6, z);
        sprintf(buf, "EXP  \x0c\x07%4d", (u16)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].exp);
        drawLargeText(x, y + 0x16, (s32)buf, 6, z);
        next = 0;
        if ((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].level < 99) {
            next = getExpForNextLevel((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].level) -
                   (u16)((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].exp;
        }
        sprintf(buf, "NEXT  \x0c\x07%3d", next);
        drawLargeText(x, y + 0x1E, (s32)buf, 6, z);
        x += 0x64;
        drawIcon(x, y + 1, 0, 0x1A, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].hp);
        drawText(x + 0xE, y + 1, (s32)buf, 7, z);
        if (D_801FC738->gains[w->partner][0]) {
            sprintf(buf, "*s0+%d", D_801FC738->gains[w->partner][0]);
            drawText(x + 0x2E, y + 1, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0xD, 0, 7, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[0].power);
        drawText(x + 0xE, y + 0xD, (s32)buf, 7, z);
        if (D_801FC738->gains[w->partner][1]) {
            sprintf(buf, "*s0+%d", D_801FC738->gains[w->partner][1]);
            drawText(x + 0x2E, y + 0xD, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0x19, 0, 8, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[1].power);
        drawText(x + 0xE, y + 0x19, (s32)buf, 7, z);
        if (D_801FC738->gains[w->partner][2]) {
            sprintf(buf, "*s0+%d", D_801FC738->gains[w->partner][2]);
            drawText(x + 0x2E, y + 0x19, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0x25, 0, 9, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[2].power);
        drawText(x + 0xE, y + 0x25, (s32)buf, 7, z);
        if (D_801FC738->gains[w->partner][3]) {
            sprintf(buf, "*s0+%d", D_801FC738->gains[w->partner][3]);
            drawText(x + 0x2E, y + 0x25, (s32)buf, 5, z);
        }
    }
}

s32 func_801FAA54(s32 x, s32 y, s32 count, s32 z, s32 exp);

void func_801F4174(UiWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 exp;
    s32 rowY;
    char buf[24];

    x = w->originX;
    y = w->originY;
    z = w->z;
    drawText(x + 0x28, y + 1, (s32)"Detail of Earned Experience Points", 6, 0);
    exp = DUEL->winner == 0 ? ((u8 *)D_8006E054)[0x74] : 0;
    rowY = y + 15;
    drawText(x + 6, rowY, (s32)"Experience Points from Opponent", 7, z);
    sprintf(buf, "*s0+%3d", exp);
    drawText(x + 0xA2, rowY, (s32)buf, 5, z);
    D_801FC738->done = func_801FAA54(x, y + 0x1C, D_801FC738->progress / 32, z, exp);
    if (PAD_STATES[0]->pressed & 0x40) {
        D_801FC738->speed = 0x20;
    }
    if (D_801FC738->done == 0) {
        D_801FC738->progress += D_801FC738->speed;
        w->view.h = D_801FC738->progress / 32 * 13 + 0x26;
        if (w->view.h - w->rect.h >= 0) {
            scrollWindowTo((s16 *)w, 0, w->view.h - w->rect.h);
        }
    } else {
        if (PAD_STATES[0]->repeat & 1) {
            scrollWindowTo((s16 *)w, 0, 0);
        }
        if (PAD_STATES[0]->repeat & 2) {
            scrollWindowTo((s16 *)w, 0, w->view.h - w->rect.h);
        }
        if (PAD_STATES[0]->repeat & 0x1000) {
            scrollWindowTo((s16 *)w, 0, w->view.y - 13);
        }
        if (PAD_STATES[0]->repeat & 0x4000) {
            scrollWindowTo((s16 *)w, 0, w->view.y + 13);
        }
    }
}

void func_801F4408(UiWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    s32 n;
    s32 icon;
    s32 palette;
    char buf[24];

    x = w->originX;
    y = w->originY;
    z = w->z;
    drawText(x + 0x5A, y + 1, (s32)"Earned Digi-Parts", 6, 0);
    n = 0;
    for (i = 0; i < 128; i++) {
        if ((D_801FC738->partFlags[i / 8] >> (i % 8)) & 1) {
            n++;
            if (n < (w->view.y - 15) / 13) {
                continue;
            }
            if ((w->view.y + w->rect.h) / 13 < n) {
                continue;
            }
            sprintf(buf, "*s0%3.3d", i);
            drawText(x + 2, y + 15 + (n - 1) * 13, (s32)buf, 5, z);
            if (i < 7) {
                icon = 0;
            } else if (i < 10) {
                icon = 1;
            } else if (i < 15) {
                icon = 2;
            } else if (i < 20) {
                icon = 3;
            } else if (i < 24) {
                icon = 4;
            } else if (i < 38) {
                icon = 5;
            } else if (i < 41) {
                icon = 6;
            } else if (i < 123) {
                icon = 7;
            } else {
                icon = 8;
            }
            drawIcon(x + 0x16, y + 15 + (n - 1) * 13, 2, icon, z);
            palette = 7;
            if (i >= 24 && i < 38) {
                palette = 4;
            }
            if (i >= 41 && i < 123) {
                palette = 5;
            }
            drawText(x + 0x30, y + 15 + (n - 1) * 13, (s32)D_801FBB38[i].name, palette, z);
        }
    }
    w->view.h = n * 13 + 15;
    if (w->view.h - w->rect.h >= 0) {
        if (PAD_STATES[0]->repeat & 1) {
            scrollWindowTo((s16 *)w, 0, w->view.y - w->rect.h);
        }
        if (PAD_STATES[0]->repeat & 2) {
            scrollWindowTo((s16 *)w, 0, w->view.y + w->rect.h);
        }
        if (PAD_STATES[0]->repeat & 0x1000) {
            scrollWindowTo((s16 *)w, 0, w->view.y - 13);
        }
        if (PAD_STATES[0]->repeat & 0x4000) {
            scrollWindowTo((s16 *)w, 0, w->view.y + 13);
        }
    }
    if (n == 0) {
        drawText(x + 0x1A, y + 0xE, (s32)"None", 7, 0);
    }
}

void func_801F4794(RankUpWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    char buf[24];

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    sprintf(buf, "%2d RANK UP!", w->rank);
    drawLargeText(x + 1, y + 1, (s32)buf, 7, z);
}

void func_801F47FC(void) {
    s32 i;

    drawWindow(&D_801FC738->unk88, func_801F4408, 0);
    drawWindow(&D_801FC738->window, func_801F4174, 0);
    drawWindow(&D_801FC738->titleWindow, func_801F3BAC, 0);
    for (i = 0; i < 3; i++) {
        drawWindow(&D_801FC738->rankWindows[i].window, func_801F4794, 0);
        drawWindow(&D_801FC738->expWindows[i].window, func_801F3BE8, 0);
    }
}

void func_801F48E0(u8 *archive) {
    s32 i;
    s32 id;

    for (i = 0; i < 3; i++) {
        if (((SessionData *)D_8006E054)->npcDeckIndex[0] != -1) {
            id = ((SessionData *)D_8006E054)->partnerBackup[0][i].cardId;
        } else {
            id = ((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId;
        }
        if (id != 0) {
            uploadTim((u32 *)(archive + ((s32 *)archive)[id]), i * 20 + 0x2C0, 0x128, -1, -1);
            ((DuelK *)D_801D8340)->partnerCluts[i] = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
        }
    }
}

void func_801FA290(void);

void func_801F4A24(void) {
    Rect16 rect;
    s32 i;
    s32 j;
    s32 n;
    s32 gained;

    D_801FC738 = allocPermanentHeapBlock(sizeof(ExpScreen));
    D_801FC738->progress = 0;
    D_801FC738->speed = 1;
    D_801FC738->done = 0;
    for (i = 0; i < 3; i++) {
        D_801FC738->partnerShown[i] = 0;
        for (j = 0; j < 4; j++) {
            D_801FC738->gains[i][j] = 0;
        }
    }
    for (i = 0; i < 30; i++) {
        n = findPartnerSlot(0, PLAYER(0)->cards[i].id);
        if (n >= 0 && n < 3) {
            D_801FC738->partnerShown[n] = 1;
        }
        n = findArmorPartnerSlot(0, PLAYER(0)->cards[i].id);
        if (n >= 0 && n < 3) {
            D_801FC738->partnerShown[n] = 1;
        }
    }
    for (i = 0; i < 16; i++) {
        D_801FC738->partFlags[i] = 0;
    }
    rect.x = 0x30;
    rect.y = 0x2C;
    rect.w = 0xE0;
    rect.h = 0xA8;
    openWindow(&D_801FC738->window, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    D_801FC738->window.label = (s32)"BONUS LIST";
    animateWindowTo(&D_801FC738->window, (Rect16 *)-1);
    rect.x = 0x10;
    rect.y = 0x2C;
    rect.w = 0x120;
    rect.h = 0xA8;
    openWindow(&D_801FC738->unk88, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    D_801FC738->unk88.label = (s32)"DIGI-PARTS RECEIVED";
    animateWindowTo(&D_801FC738->unk88, (Rect16 *)-1);
    rect.x = 0x10;
    rect.y = 0x14;
    rect.w = 0x120;
    rect.h = 0xE;
    openWindow(&D_801FC738->titleWindow, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        D_801FC738->expWindows[i].partner = i;
        rect.x = 0x10;
        rect.y = i * 60 + 0x2C;
        rect.w = 0x120;
        rect.h = 0x32;
        openWindow(&D_801FC738->expWindows[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        rect.x = 0x14;
        rect.y = i * 60 + 0x40;
        rect.w = 0x60;
        rect.h = 9;
        openWindow(&D_801FC738->rankWindows[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        D_801FC738->rankWindows[i].rank = 0;
        D_801FC738->rankWindows[i].window.palette = 2;
        animateWindowTo(&D_801FC738->rankWindows[i].window, (Rect16 *)-1);
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
            D_801FC738->expWindows[i].clut = ((DuelK *)D_801D8340)->partnerCluts[i];
        }
    }
    playSoundEffect(0xA3);
    addFrameCallback((s32)func_801F47FC);
    func_80014C08(20);
    playSoundEffect(0xA3);
    rect.x = 0x30;
    rect.y = 0x2C;
    rect.w = 0xE0;
    rect.h = 0xA8;
    animateWindowTo(&D_801FC738->window, &rect);
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (D_801FC738->done == 0 || !(PAD_STATES[0]->pressed & 0x40));
    playSoundEffect(0xA4);
    animateWindowTo(&D_801FC738->window, (Rect16 *)-1);
    func_80014C08(20);
    for (i = 0; i < 3; i++) {
        D_801FC738->pendingExp[i] = 0;
        if (D_801FC738->partnerShown[i] && (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level < 99) {
            D_801FC738->pendingExp[i] = D_801FCA2C + ((PlayerProfile *)PLAYER_PROFILES)->partners[i].unk292[1] * D_801FCA2C / 100;
        }
    }
    gained = 0;
    if (D_801FC738->pendingExp[0] + D_801FC738->pendingExp[1] + D_801FC738->pendingExp[2] != 0) {
        do {
            func_80014C08(3);
            for (i = 0; i < 3; i++) {
                if (D_801FC738->partnerShown[i] && D_801FC738->pendingExp[i] != 0) {
                    if ((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level < 99) {
                        ((PlayerProfile *)PLAYER_PROFILES)->partners[i].exp++;
                        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->partners[i].exp >= getExpForNextLevel((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level)) {
                            ((PlayerProfile *)PLAYER_PROFILES)->partners[i].level++;
                            rect.x = 0x14;
                            rect.y = i * 60 + 0x40;
                            rect.w = 0x60;
                            rect.h = 9;
                            animateWindowTo(&D_801FC738->rankWindows[i].window, &rect);
                            D_801FC738->rankWindows[i].rank++;
                            n = findNewPartnerAbility((AbilityLearnEntry *)D_801FBB38, 0, i);
                            if (n >= 0) {
                                D_801FC738->partFlags[n / 8] |= 1 << (n % 8);
                                grantPartnerAbility(0, n);
                            }
                            n = func_8004994C(0, i);
                            if (n >= 0) {
                                D_801FC738->gains[i][n] += 10;
                                gained = 1;
                            }
                        }
                        D_801FC738->pendingExp[i]--;
                    } else {
                        D_801FC738->pendingExp[i] = 0;
                    }
                }
            }
            playSoundEffect(0xAA);
        } while (D_801FC738->pendingExp[0] + D_801FC738->pendingExp[1] + D_801FC738->pendingExp[2] != 0);
    }
    func_801ED608();
    if (gained) {
        do {
            func_80014C08(3);
            n = 0;
            for (i = 0; i < 3; i++) {
                if (D_801FC738->partnerShown[i]
                    && D_801FC738->gains[i][0] + D_801FC738->gains[i][1] + D_801FC738->gains[i][2] + D_801FC738->gains[i][3] != 0) {
                    n++;
                    if (D_801FC738->gains[i][0] != 0) {
                        ((PlayerProfile *)PLAYER_PROFILES)->partners[i].hpBonus++;
                        D_801FC738->gains[i][0]--;
                    }
                    for (j = 0; j < 3; j++) {
                        if (D_801FC738->gains[i][j + 1] != 0) {
                            ((PlayerProfile *)PLAYER_PROFILES)->partners[i].attackBonus[j]++;
                            D_801FC738->gains[i][j + 1]--;
                        }
                    }
                    updatePartnerStats(0, i);
                }
            }
            playSoundEffect(0xAA);
        } while (n != 0);
        func_801ED608();
    }
    playSoundEffect(0xA3);
    rect.x = 0x10;
    rect.y = 0x2C;
    rect.w = 0x120;
    rect.h = 0xA8;
    animateWindowTo(&D_801FC738->unk88, &rect);
    func_801ED608();
    playSoundEffect(0xA4);
    animateWindowTo(&D_801FC738->unk88, (Rect16 *)-1);
    animateWindowTo(&D_801FC738->titleWindow, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&D_801FC738->rankWindows[i].window, (Rect16 *)-1);
        animateWindowTo(&D_801FC738->expWindows[i].window, (Rect16 *)-1);
    }
    func_80014C08(30);
    func_801FA290();
    removeFrameCallback((s32)func_801F47FC);
    func_80014C08(2);
    freeHeapBlock(D_801FC738);
    func_80014C08(2);
}

void func_801F851C(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb);

void func_801F54BC(PrizeWindow *win) {
    /* arrays, not literals: GCC would share func_801F3BE8's "*s0%4d", and the
       strings before it have to be arrays too to keep their order in .rodata */
    static const char numberLabel[] = "No.";
    static const char idFormat[] = "*s0%3d";
    static const char valueFormat[] = "*s0%4d";
    char buf[72];
    Bytes4 rgb;
    s32 x;
    s32 y;
    s32 z;
    s32 specialty;
    s32 i;
    DigimonCardData *card;
    u8 *data;

    if (D_801FC73C->showRewards != 0) {
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[win->index] < 0) {
            rgb.b[0] = 0x40;
            rgb.b[1] = 0x40;
            rgb.b[2] = 0x40;
            win->window.brightness = 0x40;
        } else {
            rgb.b[0] = 0x80;
            rgb.b[1] = 0x80;
            rgb.b[2] = 0x80;
            win->window.brightness = 0x80;
        }
    } else {
        rgb.b[0] = 0x80;
        rgb.b[1] = 0x80;
        rgb.b[2] = 0x80;
        win->window.brightness = 0x80;
    }
    x = win->window.originX;
    y = win->window.originY;
    z = win->window.z;
    specialty = getCardSpecialty(win->cardId);
    if (((PlayerProfile *)PLAYER_PROFILES)->cardCollection[win->cardId] & 0x20) {
        drawIcon(x + 0xF, y + 0x2A, 2, 9, 0);
    }
    func_801F851C(x + 1, y + 0xC, win->index * 20 + 0x2C0, 0x100, specialty, win->clut, &rgb);
    drawTextColored(x + 3, y, numberLabel, rgb.b, 7, z);
    sprintf(buf, idFormat, win->cardId);
    drawTextColored(x + 0x17, y, buf, rgb.b, 7, z);
    if (specialty < 5) {
        card = &((DigimonCardData *)DIGIMON_CARDS)[win->cardId];
        drawTextColored(x + 0x32, y, card->name, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0xE, 0, 0x1A, rgb.b, z);
        sprintf(buf, valueFormat, card->hp);
        drawTextColored(x + 0x3C, y + 0xD, buf, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0x1A, 0, 7, rgb.b, z);
        sprintf(buf, valueFormat, card->attack[0].power);
        drawTextColored(x + 0x3C, y + 0x19, buf, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0x26, 0, 8, rgb.b, z);
        sprintf(buf, valueFormat, card->attack[1].power);
        drawTextColored(x + 0x3C, y + 0x25, buf, rgb.b, 7, z);
        drawIconColored(x + 0x2C, y + 0x32, 0, 9, rgb.b, z);
        sprintf(buf, valueFormat, card->attack[2].power);
        drawTextColored(x + 0x3C, y + 0x31, buf, rgb.b, 7, z);
        sprintf(buf, "(%s)", CROSS_EFFECT_SHORT_NAMES[card->crossEffect]);
        drawSmallTextColored(x + 0x56, y + 0x33, buf, 7, rgb.b, z);
        if (D_8006E4FC[card->crossEffect] != 0) {
            drawIconColored(x + 0x95, y + 0x31, 0, D_8006E4FC[card->crossEffect] + 0x14, rgb.b, z);
        }
        drawIconColored(x + 0x74, y + 0x1A, 0, 0x18, rgb.b, z);
        sprintf(buf, "*s0%2d", card->dpCost);
        drawTextColored(x + 0x86, y + 0x19, buf, rgb.b, 7, z);
        drawIconColored(x + 0x74, y + 0x26, 0, 0x19, rgb.b, z);
        sprintf(buf, "*s0%2d", card->dpBonus);
        drawTextColored(x + 0x86, y + 0x25, buf, rgb.b, 7, z);
        drawTextColored(x + 0xA8, y, "Support Effect", rgb.b, 6, z);
        if (card->supportIcon != 0) {
            drawIconColored(x + 0x114, y, 0, card->supportIcon + 0x14, rgb.b, z);
        }
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, card->supportText[i], rgb.b, 7, z);
        }
    } else if (specialty == 5) {
        data = (u8 *)&((OptionCardData *)OPTION_CARDS)[win->cardId - 0xBF];
        drawTextColored(x + 0x32, y, data + 3, rgb.b, 7, z);
        drawTextColored(x + 0xA8, y, "Option Description", rgb.b, 6, z);
        if (*(s8 *)(data + 0x8C) != 0) {
            drawIconColored(x + 0x114, y, 0, *(s8 *)(data + 0x8C) + 0x14, rgb.b, z);
        }
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, data + (i * 0x15 + 0x8D), rgb.b, 7, z);
        }
    } else {
        data = (u8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[win->cardId - 0x125];
        drawTextColored(x + 0x32, y, data + 3, rgb.b, 7, z);
        drawTextColored(x + 0xA8, y, "Option Description", rgb.b, 6, z);
        for (i = 0; i < 4; i++) {
            drawTextColored(x + 0xA8, y + 0xD + i * 12, data + (i * 0x15 + 0x1B), rgb.b, 7, z);
        }
    }
}

void func_801F5B94(RewardWindow *w) {
    s32 x;
    s32 y;
    s32 z;

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[w->index] < 0) {
        drawLargeText(x + 1, y + 1, (s32)"FULL SET!", 7, z);
    } else {
        drawLargeText(x + 1, y + 1, (s32)"RECEIVED!", 7, z);
    }
}

void func_801F5C14(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned a Prize Pack", 7, 0);
    drawText(window->originX + 0x92, window->originY + 1, (s32)D_8006E31C[((u8 *)D_8006E054)[0x73]], 6, 0);
}

void func_801F5C9C(void) {
    s32 i;

    drawWindow(&D_801FC73C->window, func_801F5C14, 0);
    for (i = 0; i < 3; i++) {
        if (D_801FC73C->showRewards) {
            drawWindow(&D_801FC73C->rewards[i].window, func_801F5B94, 0);
        }
        drawWindow(&D_801FC73C->prizes[i].window, func_801F54BC, 0);
    }
}

void rollRewardCards(s32 player, s32 level);

void func_801F5D58(u8 *archive) {
    s32 i;

    rollRewardCards(0, ((u8 *)D_8006E054)[0x73]);
    for (i = 0; i < 3; i++) {
        uploadTim((u32 *)(archive + ((s32 *)archive)[((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i]]), i * 20 + 0x2C0, 0x100, -1, -1);
        ((DuelK *)D_801D8340)->rewardCluts[i] = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
    }
}

void func_801F5E50(void) {
    Rect16 rect;
    s32 i;

    D_801FC73C = allocPermanentHeapBlock(sizeof(PrizeScreen));
    addRewardCardsToCollection(0);
    D_801FC73C->showRewards = 0;
    rect.x = 16;
    rect.y = 16;
    rect.w = 0x120;
    rect.h = 14;
    openWindow(&D_801FC73C->window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        D_801FC73C->prizes[i].index = i;
        D_801FC73C->prizes[i].cardId = ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i];
        D_801FC73C->prizes[i].clut = ((DuelK *)D_801D8340)->rewardCluts[i];
        rect.x = 16;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&D_801FC73C->prizes[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    }
    playSoundEffect(0xA3);
    addFrameCallback((s32)func_801F5C9C);
    func_80014C08(20);
    func_801ED608();
    playSoundEffect(0xA0);
    D_801FC73C->showRewards = 1;
    for (i = 0; i < 3; i++) {
        rect.x = 200;
        rect.y = i * 65 + 0x40;
        rect.w = 0x48;
        rect.h = 9;
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] < 0) {
            rect.x = 0xC4;
            rect.w = 0x50;
        }
        openWindow(&D_801FC73C->rewards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        D_801FC73C->rewards[i].index = i;
        D_801FC73C->rewards[i].window.palette = 2;
    }
    func_801ED608();
    playSoundEffect(0xA4);
    animateWindowTo(&D_801FC73C->window, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&D_801FC73C->rewards[i].window, (Rect16 *)-1);
        animateWindowTo(&D_801FC73C->prizes[i].window, (Rect16 *)-1);
    }
    func_80014C08(30);
    removeFrameCallback((s32)func_801F5C9C);
    func_80014C08(2);
    freeHeapBlock(D_801FC73C);
    func_80014C08(2);
}

void func_801F6170(void) {
    func_800149B8(0, -1, 0, 0x800, loadFileTagged, "B:\\CBTL_EFF.ARC", getCurrentTaskId(), 0x38E);
    *(s32 *)((u8 *)D_801D8340 + 0x4C) = func_80014C08(0x7FFFFFFF);
}

void func_801F61E4(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x4C));
}

s32 func_801F6214(s32 entry, s32 player) {
    func_801F6294(entry, player, player, 0, 0);
}

void func_801F623C(s32 entry, s32 player, s32 mode) {
    func_801F6294(entry, player, player, mode, mode);
}

s32 func_801F6268(s32 entry, s32 player) {
    func_801F6294(entry, player, player ^ 1, 0, 0);
}

void func_801F6294(s32 entry, s32 player1, s32 player2, s32 mode1, s32 mode2) {
    s32 data;

    D_801FC87C = player1;
    switch (mode1) {
    case 0:
        D_801FC87D = getActiveDigimonCard(player1);
        break;
    case 1:
        D_801FC87D = getPlayedCard(player1);
        break;
    }
    switch (mode2) {
    case 0:
        D_801FC87E = getActiveDigimonCard(player2);
        break;
    case 1:
        D_801FC87E = getPlayedCard(player2);
        break;
    }
    data = decompressArchiveEntry(*(s32 *)((u8 *)D_801D8340 + 0x4C), entry);
    func_800149B8(0, 0x1F, 0, 0x800, func_801F7760, data, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    freeHeapBlock((void *)data);
}

extern EffectObject D_801FC740;
extern void (*D_801FC138[])(u8 *);
void func_801F663C(EffectTable *table);

s32 func_801F63AC(EffectTable *table) {
    s32 i;

    PushMatrix();
    tickEffectMotion((s32)&D_801FC740, 0);
    PopMatrix();
    table->regs[0] = 1;
    func_801F663C(table);
    for (i = 0; i < 16; i++) {
        if (table->entries[i].active != 0 && D_801FC138[table->entries[i].kind] != NULL) {
            D_801FC138[table->entries[i].kind](table->entries[i].obj);
            if (table->entries[i].kind > 0) {
                table->regs[i + 0x52] = *(s32 *)(table->entries[i].obj + 0x118);
                table->regs[i + 0x72] = *(s32 *)(table->entries[i].obj + 0x11C);
            }
        }
    }
    return table->regs[0];
}

void func_801F64D8(EffectTable *table) {
    s32 i;

    func_80014C08(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (table->entries[i].kind != -1) {
            D_801FC158[table->entries[i].kind](table->entries[i].obj);
        }
    }
}

void func_801F6578(s32 index, u8 *fx) {
    CardAnim *anim;
    s32 base;

    if (index >= 0) {
        base = (s32)D_801D833C;
        anim = (CardAnim *)(index * 36 + base);
        *(s32 *)(fx + 0x248) = anim->spr->pos.vx;
        *(s32 *)(fx + 0x24C) = anim->spr->pos.vy;
        *(s32 *)(fx + 0x250) = anim->spr->pos.vz;
    }
}

void func_801F65D8(s32 index, u8 *fx) {
    u8 rgb[3];

    if (index >= 0) {
        rgb[0] = *(s32 *)(fx + 0x94);
        rgb[1] = *(s32 *)(fx + 0x98);
        rgb[2] = *(s32 *)(fx + 0x9C);
        func_801F893C(*(CardSprite **)(D_801D833C + index * 36), rgb);
    }
}

extern u8 D_800795A8;
void func_801F6D38(EffectObject *o, u8 *fx, s32 current);
void func_801F6F44(EffectObject *o, u8 *fx);
void func_801F70DC(void *xform, u8 *fx);
void func_801F7550(s32 index, s32 kind, s32 arg2, EffectTable *table);
void func_801F8910(CardSprite *sprite, s32 num);
void func_801F8928(CardSprite *sprite);

void func_801F663C(EffectTable *table) {
    s32 *vars;
    s32 result;
    s32 index;

    if (table->count != 0) {
        table->count--;
        return;
    }
    vars = table->regs;
    do {
        result = runScriptToNextEvent(table->script, vars);
        if (result == 1) {
            switch (table->script->eventOp) {
            case 10:
                switch (table->script->eventArg) {
                case 0:
                    func_801F6D38(&D_801FC740, (u8 *)vars, 0);
                    break;
                case 1:
                    func_801F6F44(&D_801FC740, (u8 *)vars);
                    restartEffectMotion((u8 *)&D_801FC740);
                    break;
                case 2:
                    D_800794E7 = 1;
                    break;
                case 3:
                    D_800794E7 = 0;
                    break;
                case 4:
                    D_800795A8 = 0;
                    break;
                case 5:
                    D_800795A8 = 1;
                    break;
                case 6:
                    func_801F6578(D_801FC87D, (u8 *)vars);
                    break;
                case 7:
                    func_801F6578(D_801FC87E, (u8 *)vars);
                    break;
                case 8:
                    func_801F65D8(D_801FC87D, (u8 *)vars);
                    break;
                case 9:
                    func_801F65D8(D_801FC87E, (u8 *)vars);
                    break;
                case 10:
                    index = D_801FC87D;
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                case 11:
                    index = D_801FC87E;
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                }
                break;
            case 11:
                switch (table->script->eventArg) {
                case 0:
                    playSoundEffect((s16)table->script->params[0]);
                    break;
                case 1:
                    func_801F6D38((EffectObject *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, 0);
                    break;
                case 2:
                    func_801F7128((EffectTemplate *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, table);
                    initEffectObject(table->entries[(s16)table->script->params[0]].obj);
                    break;
                case 3:
                    stopSoundVoice(table->script->params[0]);
                    break;
                case 4:
                    if (table->entries[(s16)table->script->params[0]].obj != NULL) {
                        table->entries[(s16)table->script->params[0]].active = 1;
                    }
                    break;
                case 5:
                    table->entries[(s16)table->script->params[0]].active = 0;
                    break;
                case 6:
                    table->count = (s16)table->script->params[0] - 1;
                    return;
                case 7:
                    func_801F70DC(table->entries[(s16)table->script->params[0]].obj, (u8 *)vars);
                    break;
                case 8:
                    func_801F6D38((EffectObject *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, 1);
                    break;
                case 9:
                    func_801F6578(getActiveDigimonCard(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 10:
                    func_801F6578(getPlayedCard(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 11:
                    func_801F6578(peekOnlineDeckTop(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 12:
                    func_801F6578(peekOfflineDeckTop(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 13:
                    func_801F65D8(getActiveDigimonCard(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 14:
                    func_801F65D8(getPlayedCard(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 15:
                    func_801F65D8(peekOnlineDeckTop(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 16:
                    func_801F65D8(peekOfflineDeckTop(D_801FC87C ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 17:
                    index = getActiveDigimonCard(D_801FC87C ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                case 18:
                    index = getPlayedCard(D_801FC87C ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        func_801F8928(SPRITE(index));
                    }
                    break;
                case 19:
                    index = D_801FC87D;
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)table->script->params[0]);
                    }
                    break;
                case 20:
                    index = D_801FC87E;
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)table->script->params[0]);
                    }
                    break;
                }
                break;
            case 12:
                switch (table->script->eventArg) {
                case 0:
                    func_801F7550((s16)table->script->params[0], (s16)table->script->params[1], (s32)vars, table);
                    break;
                case 1:
                    vars[1] = rsin((s16)table->script->params[1]) * (s16)table->script->params[0] / 4096;
                    break;
                case 2:
                    vars[1] = rcos((s16)table->script->params[1]) * (s16)table->script->params[0] / 4096;
                    break;
                case 3:
                    playSoundEffectOnVoice((s16)table->script->params[0], (s16)table->script->params[1]);
                    break;
                case 4:
                    index = getActiveDigimonCard(D_801FC87C ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)table->script->params[1]);
                    }
                    break;
                case 5:
                    index = getPlayedCard(D_801FC87C ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        func_801F8910(SPRITE(index), (s16)table->script->params[1]);
                    }
                    break;
                }
                break;
            }
        }
        clearScriptBusy(table->script);
    } while (result != 0);
}

void func_801F6D38(EffectObject *o, u8 *fx, s32 current) {
    if (current == 0) {
        *(s32 *)(fx + 0x248) = o->px;
        *(s32 *)(fx + 0x24C) = o->py;
        *(s32 *)(fx + 0x250) = o->pz;
        *(s32 *)(fx + 0x268) = (s16)o->rx0;
        *(s32 *)(fx + 0x26C) = (s16)o->ry0;
        *(s32 *)(fx + 0x270) = (s16)o->rz0;
        *(s32 *)(fx + 0x28C) = o->sx0;
        *(s32 *)(fx + 0x290) = o->sy0;
        *(s32 *)(fx + 0x294) = o->sz0;
    } else {
        *(s32 *)(fx + 0x248) = o->posX;
        *(s32 *)(fx + 0x24C) = o->posY;
        *(s32 *)(fx + 0x250) = o->posZ;
        *(s32 *)(fx + 0x268) = o->rotX;
        *(s32 *)(fx + 0x26C) = o->rotY;
        *(s32 *)(fx + 0x270) = o->rotZ;
        *(s32 *)(fx + 0x28C) = o->sx;
        *(s32 *)(fx + 0x290) = o->sy;
        *(s32 *)(fx + 0x294) = o->sz;
    }
    *(s32 *)(fx + 0x254) = o->px2;
    *(s32 *)(fx + 0x258) = o->py2;
    *(s32 *)(fx + 0x25C) = o->pz2;
    *(s32 *)(fx + 0x260) = *(s16 *)((u8 *)o + 0x120);
    *(s32 *)(fx + 0x264) = *(s16 *)((u8 *)o + 0x122);
    *(s32 *)(fx + 0x274) = o->drx;
    *(s32 *)(fx + 0x278) = o->dry;
    *(s32 *)(fx + 0x27C) = o->drz;
    *(s32 *)(fx + 0x280) = o->ddrx;
    *(s32 *)(fx + 0x284) = o->ddry;
    *(s32 *)(fx + 0x288) = o->ddrz;
    *(s32 *)(fx + 0x298) = o->sxT;
    *(s32 *)(fx + 0x29C) = o->syT;
    *(s32 *)(fx + 0x2A0) = o->szT;
    *(s32 *)(fx + 0x2A4) = o->dsx;
    *(s32 *)(fx + 0x2A8) = o->dsy;
    *(s32 *)(fx + 0x2AC) = o->dsz;
    *(s32 *)(fx + 0x2B0) = o->hitRadius;
    *(s32 *)(fx + 0x2B4) = o->period;
    *(s32 *)(fx + 0x2B8) = o->fadeMode;
    *(s32 *)(fx + 0x2BC) = o->speed;
    *(s32 *)(fx + 0x2C4) = *(s16 *)((u8 *)o + 0x128);
    *(s32 *)(fx + 0x2C8) = *(s16 *)((u8 *)o + 0x12A);
    *(s32 *)(fx + 0x2CC) = *(s16 *)((u8 *)o + 0x126);
    *(s32 *)(fx + 0x2C0) = o->mode;
}

void func_801F6F44(EffectObject *o, u8 *fx) {
    o->px = *(s32 *)(fx + 0x248);
    o->py = *(s32 *)(fx + 0x24C);
    o->pz = *(s32 *)(fx + 0x250);
    o->px2 = *(s32 *)(fx + 0x254);
    o->py2 = *(s32 *)(fx + 0x258);
    o->pz2 = *(s32 *)(fx + 0x25C);
    *(s16 *)((u8 *)o + 0x120) = *(s32 *)(fx + 0x260);
    *(s16 *)((u8 *)o + 0x122) = *(s32 *)(fx + 0x264);
    o->rx0 = *(s32 *)(fx + 0x268);
    o->ry0 = *(s32 *)(fx + 0x26C);
    o->rz0 = *(s32 *)(fx + 0x270);
    o->drx = *(s32 *)(fx + 0x274);
    o->dry = *(s32 *)(fx + 0x278);
    o->drz = *(s32 *)(fx + 0x27C);
    o->ddrx = *(s32 *)(fx + 0x280);
    o->ddry = *(s32 *)(fx + 0x284);
    o->ddrz = *(s32 *)(fx + 0x288);
    o->sx0 = *(s32 *)(fx + 0x28C);
    o->sy0 = *(s32 *)(fx + 0x290);
    o->sz0 = *(s32 *)(fx + 0x294);
    o->sxT = *(s32 *)(fx + 0x298);
    o->syT = *(s32 *)(fx + 0x29C);
    o->szT = *(s32 *)(fx + 0x2A0);
    o->dsx = *(s32 *)(fx + 0x2A4);
    o->dsy = *(s32 *)(fx + 0x2A8);
    o->dsz = *(s32 *)(fx + 0x2AC);
    o->hitRadius = *(s32 *)(fx + 0x2B0);
    o->period = *(s32 *)(fx + 0x2B4);
    o->fadeMode = *(s32 *)(fx + 0x2B8);
    o->speed = *(s32 *)(fx + 0x2BC);
    *(s16 *)((u8 *)o + 0x128) = *(s32 *)(fx + 0x2C4);
    *(s16 *)((u8 *)o + 0x12A) = *(s32 *)(fx + 0x2C8);
    *(s16 *)((u8 *)o + 0x126) = *(s32 *)(fx + 0x2CC);
    o->mode = *(s32 *)(fx + 0x2C0);
}

void func_801F70DC(void *xform, u8 *fx) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    *(s32 *)(fx + 0x248) = pos.vx;
    *(s32 *)(fx + 0x24C) = pos.vy;
    *(s32 *)(fx + 0x250) = pos.vz;
}

void func_801F7128(EffectTemplate *template, u8 *fx, EffectTable *table) {
    func_801F6F44((EffectObject *)template, fx);
    if (*(s32 *)(fx + 0x2D0) == -2) {
        template->data[0x26] = 0;
    } else if (*(s32 *)(fx + 0x2D0) == -1) {
        template->data[0x26] = (s32)&D_801FC740;
    } else {
        template->data[0x26] = (s32)table->entries[*(s32 *)(fx + 0x2D0)].obj;
    }
}

void func_801F71A8(u8 *fx) {
    Rect16 rect;
    u8 rgb[3];
    u8 rgb2[3];

    rect.x = *(s32 *)(fx + 0x128);
    rect.y = *(s32 *)(fx + 0x12C);
    rect.w = *(s32 *)(fx + 0x130);
    rect.h = *(s32 *)(fx + 0x134);
    rgb[0] = *(s32 *)(fx + 0x94);
    rgb[1] = *(s32 *)(fx + 0x98);
    rgb[2] = *(s32 *)(fx + 0x9C);
    rgb2[0] = *(s32 *)(fx + 0xA0);
    rgb2[1] = *(s32 *)(fx + 0xA4);
    rgb2[2] = *(s32 *)(fx + 0xA8);
    func_801E6424(&rect, rgb, rgb2, *(s32 *)(fx + 0x6C), *(s16 *)(fx + 0x70), *(s32 *)(fx + 0x110));
}

void func_801F7264(u8 *fx, EffectTable *table) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    EffectTemplate template;

    func_801F7128(&template, fx, table);
    inner.b[0] = *(s32 *)(fx + 0x94);
    inner.b[1] = *(s32 *)(fx + 0x98);
    inner.b[2] = *(s32 *)(fx + 0x9C);
    mid.b[0] = *(s32 *)(fx + 0xA0);
    mid.b[1] = *(s32 *)(fx + 0xA4);
    mid.b[2] = *(s32 *)(fx + 0xA8);
    outer.b[0] = *(s32 *)(fx + 0xAC);
    outer.b[1] = *(s32 *)(fx + 0xB0);
    outer.b[2] = *(s32 *)(fx + 0xB4);
    createRingEffect(*(s32 *)(fx + 0xDC), &inner, &mid, &outer, &template, *(s32 *)(fx + 0x54), *(s32 *)(fx + 0x68),
                     *(s32 *)(fx + 0x6C), *(s32 *)(fx + 0x118), *(s32 *)(fx + 0xF4), *(s32 *)(fx + 0xF8), *(s32 *)(fx + 0x104),
                     *(s32 *)(fx + 0xFC), *(s32 *)(fx + 0x100), NULL, 0, 0, 0, *(s32 *)(fx + 0x114), *(s32 *)(fx + 0x120),
                     *(s32 *)(fx + 0x124), 0);
}

void func_801F73C8(u8 *fx, EffectTable *table) {
    EffectTemplate template;

    func_801F7128(&template, fx, table);
    cloneEffectObject(&template);
}

void func_801F73FC(u8 *fx, EffectTable *table) {
    EffectTemplate template;
    u8 startColor[3];
    u8 endColor[3];

    startColor[0] = *(s32 *)(fx + 0x94);
    startColor[1] = *(s32 *)(fx + 0x98);
    startColor[2] = *(s32 *)(fx + 0x9C);
    endColor[0] = *(s32 *)(fx + 0xA0);
    endColor[1] = *(s32 *)(fx + 0xA4);
    endColor[2] = *(s32 *)(fx + 0xA8);
    func_801F7128(&template, fx, table);
    createStreakParticles(startColor, endColor, &template, *(s32 *)(fx + 0x2DC), *(s32 *)(fx + 0x2D8), *(s32 *)(fx + 0x2E0),
                          *(s32 *)(fx + 0x2F4), *(s32 *)(fx + 0x2E4), *(s32 *)(fx + 0x2E8), *(s32 *)(fx + 0x2EC), *(s32 *)(fx + 0x2F0),
                          *(s32 *)(fx + 0x13C), *(s32 *)(fx + 0x140), *(s32 *)(fx + 0x138), *(s32 *)(fx + 0x144), *(s16 *)(fx + 0x68),
                          *(s32 *)(fx + 0x114), *(s32 *)(fx + 0x124));
}

void func_801F7530(void *ptr) {
    freeHeapBlock(ptr);
}

void func_801F7550(s32 index, s32 kind, s32 arg2, EffectTable *table) {
    if (D_801FC148[kind] != NULL) {
        table->entries[index].kind = kind;
        table->entries[index].active = 0;
        table->entries[index].obj = D_801FC148[kind](arg2, table);
        if ((++table->count & 0xF) == 0) {
            func_80014C08(FRAME_INTERVAL);
        }
    }
}

EffectTable *func_801F75E4(void *data) {
    EffectObject fx;
    EffectTable *table;
    s32 i;

    table = allocTaskHeapBlock(sizeof(EffectTable));
    table->data = data;
    table->script = createScriptContext(data);
    table->regs = allocScriptRegisters(0xBE);
    for (i = 0; i < 16; i++) {
        table->entries[i].kind = -1;
        table->entries[i].active = 0;
        table->entries[i].obj = NULL;
    }
    table->count = 0;
    fx.px = 0;
    fx.py = 0;
    fx.pz = 0;
    fx.px2 = 0;
    fx.py2 = 0;
    fx.pz2 = 0;
    fx.rx0 = 0;
    fx.ry0 = 0;
    fx.rz0 = 0;
    fx.drx = 0;
    fx.dry = 0;
    fx.drz = 0;
    fx.ddrx = 0;
    fx.ddry = 0;
    fx.ddrz = 0;
    fx.sx0 = 0x1000;
    fx.sy0 = 0x1000;
    fx.sz0 = 0x1000;
    fx.sxT = 0x1000;
    fx.syT = 0x1000;
    fx.szT = 0x1000;
    fx.dsx = 0;
    fx.dsy = 0;
    fx.dsz = 0;
    fx.fadeMode = 0;
    fx.speed = 0;
    fx.hitRadius = 0x80;
    *(GsCOORDINATE2 **)((u8 *)&fx + 0x98) = (GsCOORDINATE2 *)SCENE_3D->unk78;
    fx.mode = 0;
    D_801FC740 = fx;
    initEffectObject(&D_801FC740);
    func_801F663C(table);
    return table;
}

void func_801F7760(void *data, s32 task) {
    EffectTable *table;

    table = func_801F75E4(data);
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (func_801F63AC(table));
    func_801F64D8(table);
    freeScriptContext(table->script, table->regs);
    freeHeapBlock(table);
    func_80014A48(task);
}

void func_801F77E0(void) {
    drawWindow(&D_801FC884, func_801F7A64, 0);
}

void func_801F7810(UiWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    char unused[8];

    x = w->originX;
    y = w->originY;
    z = w->z;
    for (i = 0; i < 51; i++) {
        if (i < (s16)(w->view.y / 12)) {
            continue;
        }
        if ((w->view.y + w->rect.h) / 12 < i) {
            break;
        }
        drawText(x, y + i * 12, (s32)D_801FC168[i], 7, z);
    }
    if (PAD_STATES[(s8)DUEL->unk820[1]]->repeat & 2) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] + w->rect.h);
    }
    if (PAD_STATES[(s8)DUEL->unk820[1]]->repeat & 1) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] - w->rect.h);
    }
    if (PAD_STATES[(s8)DUEL->unk820[1]]->repeat & 0x1000) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] - 12);
    }
    if (PAD_STATES[(s8)DUEL->unk820[1]]->repeat & 0x4000) {
        scrollWindowTo((s16 *)w, w->scroll[2], w->scroll[3] + 12);
    }
}

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DF604);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DFB54);

extern char *D_801FC234[];
extern Menu D_801FC244;

void func_801F7A64(UiWindow *window) {
    s32 x;
    s32 y;
    s32 z;
    s32 i;
    char *text;
    char unused[8];

    x = window->originX;
    y = window->originY;
    z = window->z;
    for (i = 0; i < 4; i++) {
        text = D_801FC234[i];
        if (DUEL->tutorial && i == 3) {
            text = "Quit";
        }
        drawText(x, y + i * 14, (s32)text, 7, z);
    }
    updateMenuCursor(&D_801FC244);
}

typedef struct {
    /* 0x00 */ u8 unk0[0x98];
    /* 0x98 */ char *yes;
    /* 0x9C */ char *no;
    /* 0xA0 */ void (*draw)(void);
    /* 0xA4 */ u8 unkA4;
    /* 0xA5 */ s8 result;
    /* 0xA6 */ u8 pad;
} DialogK;
extern CursorHighlight D_801FC8D4;
extern DialogK D_801FC924;
extern UiWindow D_801FC9E4;
extern char D_801DFB54[];
extern char D_801DFBBC[];
void func_80055730(void);

void func_801F7B2C(void) {
    Rect16 rect;
    Rect16 view;
    s32 player;
    s32 i;

    if (DUEL_MSG_BAR.playerLabel == 1 || ((DuelK *)D_801D8340)->awaitingInput == 0 || ((DuelK *)D_801D8340)->unk820 != 0) {
        return;
    }
    player = DUEL_MSG_BAR.playerLabel & 1;
    if (!(PAD_STATES[player]->pressed & 0x800)) {
        return;
    }
    playSoundEffect(0xA3);
    ((DuelK *)D_801D8340)->menuOpen = 1;
    ((DuelK *)D_801D8340)->menuPlayer = player;
    D_801FC244.pad = player;
    openMenu(&D_801FC244, &D_801FC884, &D_801FC8D4, (Bytes4 *)-1);
    D_801FC884.label = (s32)"MENU";
    for (;;) {
        func_80014C08(FRAME_INTERVAL);
        drawWindow(&D_801FC884, func_801F7A64, 0);
        if (PAD_STATES[((DuelK *)D_801D8340)->menuPlayer]->pressed & 0x40) {
            playSoundEffect(0xA0);
            switch (D_801FC244.row) {
            case 0:
                D_801FC924.yes = "Stereo";
                D_801FC924.no = "Mono";
                initDialog((u8 *)&D_801FC924, "Sound Settings", 2);
                D_801FC924.pad = ((DuelK *)D_801D8340)->menuPlayer;
                D_801FC924.draw = func_801F77E0;
                D_801FC924.result = ((PlayerProfile *)PLAYER_PROFILES)->unk20_0 + 1;
                animateWindowTo(&D_801FC884, (Rect16 *)-1);
                runDialog(&D_801FC924);
                animateWindowTo(&D_801FC884, &D_801FC244.rect);
                switch (D_801FC924.result) {
                case 2:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk20_0 = 1;
                    func_80055730();
                    break;
                case 1:
                    ((PlayerProfile *)PLAYER_PROFILES)->unk20_0 = 0;
                    func_80055740();
                    break;
                case 0:
                    break;
                }
                break;
            case 1:
                D_801FC924.yes = "On";
                D_801FC924.no = "Off";
                initDialog((u8 *)&D_801FC924, D_801DFB54, 2);
                D_801FC924.pad = ((DuelK *)D_801D8340)->menuPlayer;
                D_801FC924.draw = func_801F77E0;
                D_801FC924.result = ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation + 1;
                animateWindowTo(&D_801FC884, (Rect16 *)-1);
                runDialog(&D_801FC924);
                animateWindowTo(&D_801FC884, &D_801FC244.rect);
                switch (D_801FC924.result) {
                case 2:
                    ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation = 1;
                    break;
                case 1:
                    ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation = 0;
                    break;                    break;
                case 0:
                    break;
                }
                break;
            case 2:
                rect.x = 0x32;
                rect.y = 0x1A;
                rect.w = 0xDC;
                rect.h = 0xBE;
                view.x = 0;
                view.y = 0;
                view.w = 0xDC;
                view.h = 0x264;
                openWindow(&D_801FC9E4, &rect, -1, (s16 *)&view, 10, 0x16, 0x80, 12);
                D_801FC9E4.label = (s32)"HELP";
                animateWindowTo(&D_801FC884, (Rect16 *)-1);
                do {
                    func_80014C08(FRAME_INTERVAL);
                    drawWindow(&D_801FC9E4, func_801F7810, 0);
                    drawWindow(&D_801FC884, func_801F7A64, 0);
                } while (!(PAD_STATES[((DuelK *)D_801D8340)->menuPlayer]->pressed & 0x10));
                playSoundEffect(0xA4);
                animateWindowTo(&D_801FC9E4, (Rect16 *)-1);
                animateWindowTo(&D_801FC884, &D_801FC244.rect);
                for (i = 0; i < 16; i++) {
                    func_80014C08(FRAME_INTERVAL);
                    drawWindow(&D_801FC9E4, func_801F7810, 0);
                    drawWindow(&D_801FC884, func_801F7A64, 0);
                }
                break;
            case 3:
                if (((DuelK *)D_801D8340)->tutorial) {
                    initDialog((u8 *)&D_801FC924, "Quit Tutorial?", 1);
                } else {
                    initDialog((u8 *)&D_801FC924, D_801DFBBC, 1);
                }
                D_801FC924.pad = ((DuelK *)D_801D8340)->menuPlayer;
                D_801FC924.draw = func_801F77E0;
                animateWindowTo(&D_801FC884, (Rect16 *)-1);
                runDialog(&D_801FC924);
                animateWindowTo(&D_801FC884, &D_801FC244.rect);
                switch (D_801FC924.result) {
                case 2:
                case 0:
                    ((DuelK *)D_801D8340)->quit = 0;
                    break;
                case 1:
                    ((DuelK *)D_801D8340)->quit = ((D_801D83D1 & 1) + 2) ^ 1;
                    break;
                }
                break;
            }
        }
        if (!(PAD_STATES[((DuelK *)D_801D8340)->menuPlayer]->pressed & 0x810)) {
            if (((DuelK *)D_801D8340)->quit == 0) {
                continue;
            }
        } else {
            playSoundEffect(0xA4);
            animateWindowTo(&D_801FC884, (Rect16 *)-1);
            for (i = 0; i < 16; i++) {
                func_80014C08(FRAME_INTERVAL);
                drawWindow(&D_801FC884, func_801F7A64, 0);
            }
        }
        ((DuelK *)D_801D8340)->menuOpen = 0;
        return;
    }
}

typedef struct HudPanelK {
    /* 0x00 */ u8 rgb[4];
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ u8 unk8[4];
    /* 0x0C */ u8 flags;
    /* 0x0D */ u8 state;
    /* 0x0E */ u8 unkE[2];
    /* 0x10 */ s16 x;
    /* 0x12 */ s16 y;
    /* 0x14 */ u8 unk14[8];
    /* 0x1C */ s32 z;
    /* 0x20 */ struct HudPanelK *parent;
} HudPanelK;
typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ s16 unk4;
    /* 0x6 */ s16 unk6;
    /* 0x8 */ u8 unk8;
    /* 0x9 */ s8 parent;
    /* 0xA */ s16 z;
} HudPanelInit;
typedef struct {
    u8 data[0x5F0];
} Unk5F0;

extern HudPanelInit D_801FC270[];
s32 func_801F8200(void) {
    s32 i;

    *(Unk5F0 **)((u8 *)D_801D8340 + 0x48) = allocTaskHeapBlock(sizeof(Unk5F0) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[11] = (s32)&(*(Unk5F0 **)((u8 *)D_801D8340 + 0x48))[i];
    }
    D_801D83EC = allocTaskHeapBlock(sizeof(HudPanelK) * 12);
    for (i = 0; i < 12; i++) {
        ((HudPanelK *)D_801D83EC)[i].rgb[0] = 0x80;
        ((HudPanelK *)D_801D83EC)[i].rgb[1] = 0x80;
        ((HudPanelK *)D_801D83EC)[i].rgb[2] = 0x80;
        ((HudPanelK *)D_801D83EC)[i].x = 0;
        ((HudPanelK *)D_801D83EC)[i].y = 0;
        ((HudPanelK *)D_801D83EC)[i].unk8[0] = D_801FC270[i].unk0[0];
        ((HudPanelK *)D_801D83EC)[i].unk8[1] = D_801FC270[i].unk0[1];
        ((HudPanelK *)D_801D83EC)[i].unk8[2] = D_801FC270[i].unk0[2];
        ((HudPanelK *)D_801D83EC)[i].unk8[3] = D_801FC270[i].unk0[3];
        ((HudPanelK *)D_801D83EC)[i].unk6 = D_801FC270[i].unk4;
        ((HudPanelK *)D_801D83EC)[i].unk4 = D_801FC270[i].unk6;
        ((HudPanelK *)D_801D83EC)[i].flags = 0;
        ((HudPanelK *)D_801D83EC)[i].state = 0;
        if (D_801FC270[i].parent != -1) {
            ((HudPanelK *)D_801D83EC)[i].parent = &((HudPanelK *)D_801D83EC)[D_801FC270[i].parent];
        } else {
            ((HudPanelK *)D_801D83EC)[i].parent = NULL;
        }
        ((HudPanelK *)D_801D83EC)[i].z = D_801FC270[i].z;
    }
    DUEL_MSG_BAR.bannerState = 0;
    DUEL_MSG_BAR.playerLabel = 0;
    DUEL_MSG_BAR.phase = -1;
    DUEL_MSG_BAR.next = 0;
    DUEL_MSG_BAR.cur = -1;
    DUEL_MSG_BAR.y = 0;
    DUEL_MSG_BAR.next2 = 0;
    DUEL_MSG_BAR.cur2 = -1;
    DUEL_MSG_BAR.y2 = 0;
    DUEL_MSG_BAR.bannerLabel = -1;
    DUEL_MSG_BAR.bannerStep = -1;
    DUEL_MSG_BAR.bannerPhase = -1;
    DUEL_MSG_BAR.echoAge = 0;
    DUEL_MSG_BAR.px = 0;
    DUEL_MSG_BAR.py = 0;
    DUEL_MSG_BAR.tx = 0;
    DUEL_MSG_BAR.ty = 0;
}

s32 func_801F848C(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x48));
    freeHeapBlock(D_801D83EC);
}

extern Bytes4 D_801DFBC8;
void func_801F851C(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb);

void func_801F84CC(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, u16 a5) {
    Bytes4 color;

    color = D_801DFBC8;
    func_801F851C(a0, a1, a2, a3, a4, a5, &color);
}

void func_801F851C(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb) {
    if (isSpritePoolFull() == 0) {
        CUR_SPRT->sp.x0 = x + 2;
        CUR_SPRT->sp.y0 = y + 5;
        CUR_SPRT->sp.u0 = (u % 64) * 2 + 2;
        CUR_SPRT->sp.v0 = v % 256 + 2;
        CUR_SPRT->sp.clut = clut;
        CUR_SPRT->sp.w = 36;
        CUR_SPRT->sp.h = 36;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = rgb->b[0];
        CUR_SPRT->sp.g0 = rgb->b[1];
        CUR_SPRT->sp.b0 = rgb->b[2];
        setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(1, 0, u, v));
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
        addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
        SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        if (frame >= 6) {
            frame = 5;
        }
        if (isSpritePoolFull() == 0) {
            CUR_SPRT->sp.x0 = x;
            CUR_SPRT->sp.y0 = y;
            CUR_SPRT->sp.u0 = 0xCC;
            CUR_SPRT->sp.v0 = 0x6F;
            CUR_SPRT->sp.clut = getClut(0x320, frame + 0x1F1);
            CUR_SPRT->sp.w = 40;
            CUR_SPRT->sp.h = 48;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = rgb->b[0];
            CUR_SPRT->sp.g0 = rgb->b[1];
            CUR_SPRT->sp.b0 = rgb->b[2];
            setDrawMode(&CUR_SPRT->dm, 0, 0, getTPage(0, 0, 0x300, 0x100));
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
            addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
            SPRITE_POOL_CURSOR += sizeof(SprtPacket);
        }
    }
}

s32 func_801F8854(void) {
    s32 i;

    D_801D83F8 = allocTaskHeapBlock(sizeof(Unk14F0) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[10] = (s32)&D_801D83F8[i];
    }
    allocTaskHeapBlock(0xE10);
}

s32 func_801F88E8(void) {
    freeHeapBlock(D_801D83F8);
}

void func_801F8910(CardSprite *sprite, s32 num) {
    sprite->flags |= 0x20;
    sprite->num = num;
}

void func_801F8928(CardSprite *sprite) {
    sprite->flags &= ~0x20;
}

void func_801F893C(CardSprite *sprite, u8 *to) {
    sprite->flags |= 0x40;
    sprite->t = 0;
    sprite->from[0] = sprite->fade[0];
    sprite->from[1] = sprite->fade[1];
    sprite->from[2] = sprite->fade[2];
    sprite->to[0] = to[0];
    sprite->to[1] = to[1];
    sprite->to[2] = to[2];
}

void func_801F8C58(Shape *shape, s32 x, s32 y, s32 d);

s32 func_801F8998(s32 arg0, s32 x, s32 y, s32 d, s32 count) {
    s32 i;
    s32 j;
    s32 k;
    Shape *shapes;
    u8 *rgb;

    D_801D83F4 = allocTaskHeapBlock(count * sizeof(GradPacket) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[12] = (s32)&D_801D83F4[i * count];
        for (j = 0; j < count; j++) {
            setDrawMode(&D_801D83F4[i * count + j].dm, 0, 0, GetTPage(0, 1, 0, 0));
            for (k = 0; k < 8; k++) {
                initPrimByType(9, &D_801D83F4[i * count + j].prims[k], 1, 0);
                (D_801D83F4[i * count + j].prims + k)->r0 = 0;
                (D_801D83F4[i * count + j].prims + k)->g0 = 0;
                (D_801D83F4[i * count + j].prims + k)->b0 = 0;
                (D_801D83F4[i * count + j].prims + k)->r1 = 0;
                (D_801D83F4[i * count + j].prims + k)->g1 = 0;
                (D_801D83F4[i * count + j].prims + k)->b1 = 0;
            }
        }
    }
    shapes = allocTaskHeapBlock(count * sizeof(Shape));
    for (i = 0; i < count; i++) {
        shapes[i].unk2 = 0;
        shapes[i].mode = arg0;
        shapes[i].index = i;
        rgb = shapes[i].rgb;
        rgb[0] = 0xFF;
        rgb[1] = 0xFF;
        rgb[2] = 0;
        func_801F8C58(&shapes[i], x, y, d);
    }
    return (s32)shapes;
}

void func_801F8C58(Shape *shape, s32 x, s32 y, s32 d) {
    s32 k;

    k = d * 14 / 10;
    shape->x = x;
    shape->y = y;
    shape->points[0] = -x;
    shape->points[1] = -y - k;
    shape->points[2] = x;
    shape->points[3] = -y - k;
    shape->points[4] = -x;
    shape->points[5] = -y;
    shape->points[6] = x;
    shape->points[7] = -y;
    shape->points[8] = x + k;
    shape->points[9] = -y;
    shape->points[10] = x + k;
    shape->points[11] = y;
    shape->points[12] = x;
    shape->points[13] = -y;
    shape->points[14] = x;
    shape->points[15] = y;
    shape->points[16] = -x - k;
    shape->points[17] = -y;
    shape->points[18] = -x - k;
    shape->points[19] = y;
    shape->points[20] = -x;
    shape->points[21] = -y;
    shape->points[22] = -x;
    shape->points[23] = y;
    shape->points[24] = -x;
    shape->points[25] = y + k;
    shape->points[26] = x;
    shape->points[27] = y + k;
    shape->points[28] = -x;
    shape->points[29] = y;
    shape->points[30] = x;
    shape->points[31] = y;
    shape->points[32] = -x - k;
    shape->points[33] = -y;
    shape->points[34] = -x - d;
    shape->points[35] = -y - d;
    shape->points[36] = -x;
    shape->points[37] = -y;
    shape->points[38] = -x;
    shape->points[39] = -y - k;
    shape->points[40] = x + k;
    shape->points[41] = -y;
    shape->points[42] = x + d;
    shape->points[43] = -y - d;
    shape->points[44] = x;
    shape->points[45] = -y;
    shape->points[46] = x;
    shape->points[47] = -y - k;
    shape->points[48] = x + k;
    shape->points[49] = y;
    shape->points[50] = x + d;
    shape->points[51] = y + d;
    shape->points[52] = x;
    shape->points[53] = y;
    shape->points[54] = x;
    shape->points[55] = y + k;
    shape->points[56] = -x - k;
    shape->points[57] = y;
    shape->points[58] = -x - d;
    shape->points[59] = y + d;
    shape->points[60] = -x;
    shape->points[61] = y;
    shape->points[62] = -x;
    shape->points[63] = y + k;
}

void func_801F8DB4(void *ptr) {
    if (ptr != NULL) {
        freeHeapBlock(D_801D83F4);
        freeHeapBlock(ptr);
    }
}

void func_801F8DF0(arg0, x, y)
    s16 *arg0;
    s16 x;
    s16 y;
{
    arg0[4] = x;
    arg0[5] = y;
    func_801F8E14(arg0);
}

void func_801F8E14(void *arg0) {
    func_801F8E34(arg0, 1);
}

#define ABS(x) ((x) < 0 ? -(x) : (x))
#define setXY4(p, _x0, _y0, _x1, _y1, _x2, _y2, _x3, _y3)                                  \
    (p)->x0 = _x0, (p)->y0 = _y0, (p)->x1 = _x1, (p)->y1 = _y1, (p)->x2 = _x2, (p)->y2 = _y2, \
    (p)->x3 = _x3, (p)->y3 = _y3
#define SCALE_COORD(dst, src, scale) \
    do {                                \
        (dst) = (src) * (scale) / 8192; \
    } while (0)
#define SET_SCALED_VERTEX(v, px, py, scale) \
    do {                                    \
        SCALE_COORD((v).vx, px, scale);     \
        SCALE_COORD((v).vy, py, scale);     \
        (v).vz = 0;                         \
    } while (0)
#define SET_SPRITE_MATRIX(sprite, m)                                  \
    do {                                                              \
        buildRotTransMatrix(&(sprite)->pos, &(sprite)->rot, m);       \
        CompMatrix((MATRIX *)((u8 *)SCENE_3D + 0x78), m, m);          \
        SetRotMatrix((s32)(m));                                       \
        func_8005C444(m);                                             \
    } while (0)

void func_801F8E34(void *arg0, s32 otz) {
    Shape *shape;
    GradPacket *pk;
    MATRIX matrix;
    SVECTOR v[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;
    s32 level;
    s32 i;
    CardSprite *sprite;

    shape = arg0;
    pk = (GradPacket *)CURRENT_FRAME_BUFFER->primSlots[12] + shape->index;
    i = (((PlayerProfile *)PLAYER_PROFILES)->playTime * 8) % 200;
    level = (u8)(i >= 100 ? 300 - i : i + 100);
    for (i = 0; i < 64; i += 2) {
        if (ABS(shape->points[i]) == shape->x) {
            shape->cur[i] = shape->points[i];
        } else {
            shape->cur[i] = shape->x + (ABS(shape->points[i]) - shape->x) * level / 100;
            if (shape->points[i] < 0) {
                shape->cur[i] *= -1;
            }
        }
        if (ABS(shape->points[i + 1]) == shape->y) {
            shape->cur[i + 1] = shape->points[i + 1];
        } else {
            shape->cur[i + 1] = shape->y + (ABS(shape->points[i + 1]) - shape->y) * level / 100;
            if (shape->points[i + 1] < 0) {
                shape->cur[i + 1] *= -1;
            }
        }
    }
    switch (shape->mode) {
    case 0:
        sprite = shape->sprite;
        SET_SPRITE_MATRIX(sprite, &matrix);
        for (i = 0; i < 8; i++) {
            SET_SCALED_VERTEX(v[0], shape->cur[i * 8 + 0], shape->cur[i * 8 + 1], sprite->scale);
            SET_SCALED_VERTEX(v[1], shape->cur[i * 8 + 2], shape->cur[i * 8 + 3], sprite->scale);
            SET_SCALED_VERTEX(v[2], shape->cur[i * 8 + 4], shape->cur[i * 8 + 5], sprite->scale);
            SET_SCALED_VERTEX(v[3], shape->cur[i * 8 + 6], shape->cur[i * 8 + 7], sprite->scale);
            RotAverage4(&v[0], &v[1], &v[2], &v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
            setXY4(&pk->prims[i], sxy[0], sxy[0] >> 16, sxy[1], sxy[1] >> 16, sxy[2], sxy[2] >> 16, sxy[3],
                   sxy[3] >> 16);
            setRGB2(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            if (i / 4 != 0) {
                setRGB3(&pk->prims[i], 0, 0, 0);
            } else {
                setRGB3(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            }
            addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->prims[i]);
        }
        break;
    case 1:
        for (i = 0; i < 8; i++) {
            pk->prims[i].x0 = shape->cur[i * 8 + 0] + shape->offsetX;
            pk->prims[i].y0 = shape->cur[i * 8 + 1] + shape->offsetY;
            pk->prims[i].x1 = shape->cur[i * 8 + 2] + shape->offsetX;
            pk->prims[i].y1 = shape->cur[i * 8 + 3] + shape->offsetY;
            pk->prims[i].x2 = shape->cur[i * 8 + 4] + shape->offsetX;
            pk->prims[i].y2 = shape->cur[i * 8 + 5] + shape->offsetY;
            pk->prims[i].x3 = shape->cur[i * 8 + 6] + shape->offsetX;
            pk->prims[i].y3 = shape->cur[i * 8 + 7] + shape->offsetY;
            setRGB2(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            if (i / 4 != 0) {
                setRGB3(&pk->prims[i], 0, 0, 0);
            } else {
                setRGB3(&pk->prims[i], shape->rgb[0], shape->rgb[1], shape->rgb[2]);
            }
            addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->prims[i]);
        }
        break;
    }
    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->dm);
}

void func_801F96F0(void) {
    s32 i;
    u8 *duel;

    *(RingPrims **)((u8 *)D_801D8340 + 0x4) = allocTaskHeapBlock(sizeof(RingPrims) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[13] = (s32)&(*(RingPrims **)((u8 *)D_801D8340 + 0x4))[i];
    }
    duel = D_801D8340;
    *(s32 *)(duel + 0x828) = -1;
    *(s32 *)(duel + 0x834) = 0x140;
}

void func_801F9794(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x4));
}

void func_801F97C4(s32 arg0, s32 arg1, s32 arg2) {
    u8 *duel = D_801D8340;

    *(s32 *)(duel + 0x828) = 1;
    *(s32 *)(duel + 0x82C) = arg0;
    *(s32 *)(duel + 0x830) = arg1;
    *(s32 *)(duel + 0x838) = arg2;
}

void func_801F97E4(void) {
    *(s32 *)((u8 *)D_801D8340 + 0x828) = 0;
}

s32 func_801F97F4(void) {
    PolyF4 *f4;
    POLY_G4 *g4;
    DR_MODE *dm;
    s32 cx;
    s32 cy;
    s32 r;
    s32 i;

    switch (RING->mode) {
    case 0:
        if ((RING->radius += 16) > 0x140) {
            RING->mode = -1;
            RING->radius = 0x140;
            return;
        }
        break;
    case 1:
        if ((RING->radius -= 16) < 0) {
            RING->radius = 0;
        }
        break;
    }
    cx = RING->cx;
    cy = RING->cy;
    r = RING->radius;
    f4 = ((RingPrims *)CURRENT_FRAME_BUFFER->primSlots[13])->edges;
    for (i = 0; i < 32; i++, f4++) {
        initPrimByType(8, f4, 1, 0);
        setRGB0(f4, 0xA0, 0xA0, 0xA0);
        f4->x0 = cx + (r + RING->width + 32 + r) * rsin(i << 7) / 4096;
        f4->y0 = cy + (r + RING->width + 32 + r) * rcos(i << 7) / 4096;
        f4->x1 = cx + (r + RING->width + 32 + r) * rsin((i + 1) << 7) / 4096;
        f4->y1 = cy + (r + RING->width + 32 + r) * rcos((i + 1) << 7) / 4096;
        f4->x2 = cx + rsin(i << 7) * 400 / 4096;
        f4->y2 = cy + rcos(i << 7) * 400 / 4096;
        f4->x3 = cx + rsin((i + 1) << 7) * 400 / 4096;
        f4->y3 = cy + rcos((i + 1) << 7) * 400 / 4096;
        addPrim(&CURRENT_FRAME_BUFFER->ot[1], f4);
    }
    g4 = ((RingPrims *)CURRENT_FRAME_BUFFER->primSlots[13])->fades;
    for (i = 0; i < 32; i++, g4++) {
        initPrimByType(9, g4, 1, 0);
        setRGB0(g4, 0, 0, 0);
        setRGB1(g4, 0, 0, 0);
        setRGB2(g4, 0xA0, 0xA0, 0xA0);
        setRGB3(g4, 0xA0, 0xA0, 0xA0);
        g4->x0 = cx + (r + RING->width) * rsin(i << 7) / 4096;
        g4->y0 = cy + (r + RING->width) * rcos(i << 7) / 4096;
        g4->x1 = cx + (r + RING->width) * rsin((i + 1) << 7) / 4096;
        g4->y1 = cy + (r + RING->width) * rcos((i + 1) << 7) / 4096;
        g4->x2 = cx + (r + RING->width + 32 + r) * rsin(i << 7) / 4096;
        g4->y2 = cy + (r + RING->width + 32 + r) * rcos(i << 7) / 4096;
        g4->x3 = cx + (r + RING->width + 32 + r) * rsin((i + 1) << 7) / 4096;
        g4->y3 = cy + (r + RING->width + 32 + r) * rcos((i + 1) << 7) / 4096;
        addPrim(&CURRENT_FRAME_BUFFER->ot[1], g4);
    }
    dm = &((RingPrims *)CURRENT_FRAME_BUFFER->primSlots[13])->dm;
    SetDrawTPage(dm, 0, 0, GetTPage(0, 2, 0, 0));
    addPrim(&CURRENT_FRAME_BUFFER->ot[1], dm);
}

#define FLAGS110(p) ((Flags110 *)&PLAYER(p)->unk110)

void func_801F9EAC(s32 player) {
    s32 count;
    s32 id;
    s32 i;

    FLAGS110(player)->f0 = 0;
    FLAGS110(player)->f1 = 0;
    FLAGS110(player)->f2 = 0;
    FLAGS110(player)->f3 = 0;
    FLAGS110(player)->f4 = 0;
    FLAGS110(player)->f5 = 0;
    FLAGS110(player)->f6 = 0;
    FLAGS110(player)->f7 = 0;
    FLAGS110(player)->f8 = 0;
    FLAGS110(player)->f9 = 0;
    FLAGS110(player)->f10 = 0;
    FLAGS110(player)->f11 = 0;
    FLAGS110(player)->count = 0;
    FLAGS110(player)->f14 = 0;
    FLAGS110(player)->f15 = 0;
    FLAGS110(player)->f16 = 0;
    FLAGS110(player)->f19 = 0;
    FLAGS110(player)->f21 = 0;
    FLAGS110(player)->f22 = 0;
    FLAGS110(player)->f23 = 0;
    FLAGS110(player)->f24 = 0;
    FLAGS110(player)->f25 = 0;
    FLAGS110(player)->f26 = 0;
    FLAGS110(player)->f27 = 0;
    FLAGS110(player)->f17 = 0;
    FLAGS110(player)->f18 = 0;
    FLAGS110(player)->f29 = 0;
    FLAGS110(player)->f30 = 0;
    count = 0;
    for (id = 0x111; id < 0x11D; id++) {
        for (i = 0; i < 30; i++) {
            if (PLAYER(player)->cards[i].id == id) {
                count++;
                break;
            }
        }
    }
    if (count == 12) {
        FLAGS110(player)->f28 = 1;
    } else {
        FLAGS110(player)->f28 = 0;
    }
    for (i = 0; i < 32; i++) {
        ((DuelK *)D_801D8340)->bonusFlags[i] = 0;
    }
}

void func_801FA290(void) {
    s32 i;
    u8 *flags;
    ProfileK *profile;

    i = 0;
    flags = (u8 *)D_801D8340 + 0x840;
    profile = (ProfileK *)PLAYER_PROFILES;
    for (; i < 32; i++) {
        if (flags[i] != 0) {
            if (++profile->counts[i] >= 1000) {
                profile->counts[i] = 999;
            }
        }
    }
}

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DFBBC);

INCLUDE_RODATA("asm/kawseg/nonmatchings/kawseg", D_801DFBC8);

void func_801FA30C(s32 player) {
    Player *p;
    u32 flags;

    if (getActiveDigimonCard(player) == -1) {
        return;
    }
    p = (Player *)DUEL_PLAYERS[player];
    if (((u32)p->unk110 >> 15) & 1) {
        return;
    }
    switch (p->specialty) {
    case 0:
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x800000;
        break;
    case 1:
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x1000000;
        break;
    case 2:
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x2000000;
        break;
    case 3:
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x4000000;
        break;
    case 4:
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x8000000;
        break;
    }
    flags = ((Player *)DUEL_PLAYERS[player])->unk110;
    if (((flags >> 23) & 1) + ((flags >> 24) & 1) + ((flags >> 25) & 1) + ((flags >> 26) & 1) + ((flags >> 27) & 1) == 5) {
        func_801FB444(player, 0x18);
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x8000;
    }
}

s32 func_801FA4E4(s32 player) {
    s32 card;
    s16 hp;

    card = getActiveDigimonCard(player);
    if (card == -1) {
        return;
    }
    hp = PLAYER(player)->stats[0];
    if (hp != 0 && hp % 1110 == 0) {
        func_801FB444(player, 0x1A);
        PLAYER(0)->unk110 |= 0x400;
    }
    if (findPartnerSlot(player, PLAYER(player)->cards[card % 30].id) >= 0) {
        func_801FB444(player, 0x1C);
        ((Flags110 *)&PLAYER(player)->unk110)->count++;
    } else if (findArmorPartnerSlot(player, PLAYER(player)->cards[card % 30].id) >= 0) {
        func_801FB444(player, 0x1C);
        ((Flags110 *)&PLAYER(player)->unk110)->count++;
    }
    if (!(((u32)PLAYER(player)->unk110 >> 30) & 1) && PLAYER(player)->digimonStack[0] >= 0 &&
        findPartnerSlot(player, PLAYER(player)->cards[PLAYER(player)->digimonStack[2] % 30].id) >= 0) {
        func_801FB444(player, 0x1D);
        PLAYER(player)->unk110 |= 0x20000000;
    }
    func_801FA30C(player);
}

s32 func_801FA780(s32 player) {
    s32 ids[4];
    s32 same;
    s32 partners;
    s32 i;
    s32 card;

    same = 0;
    partners = 0;
    for (i = 0; i < 4; i++) {
        card = ((Player *)DUEL_PLAYERS[player])->hand[i];
        if (card == -1) {
            ids[i] = card;
            continue;
        }
        ids[i] = ((Player *)DUEL_PLAYERS[player])->cards[card % 30].id;
        if (ids[0] == ids[i]) {
            same++;
        }
        if (findPartnerSlot(player, ids[i]) >= 0) {
            partners++;
        }
    }
    if (same == 4) {
        func_801FB444(player, 8);
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x40;
    }
    if (partners == 3) {
        func_801FB444(player, 0x1B);
        ((Player *)DUEL_PLAYERS[player])->unk110 |= 0x800;
    }
}

typedef struct {
    /* 0x0 */ u8 id;
    /* 0x1 */ u8 bonus;
    /* 0x4 */ char *name;
} BonusEntry;


s32 func_801FA918(BonusEntry *entry, s32 x, s32 y, s32 last, s32 z) {
    char buf[72];

    ((DuelK *)D_801D8340)->bonusFlags[entry->id] = 1;
    drawText(x + 6, y + D_801FCA28 * 13, (s32)entry->name, 7, z);
    sprintf(buf, "*s0+%3d*c7(%3d)", entry->bonus, ((ProfileK *)PLAYER_PROFILES)->counts[entry->id] + 1);
    drawText(x + 0xA2, y + D_801FCA28 * 13, (s32)buf, 5, z);
    D_801FCA28++;
    D_801FCA2C += entry->bonus;
    return D_801FCA28 == last;
}

extern BonusEntry D_801FC300[];

s32 func_801FAA54(s32 x, s32 y, s32 count, s32 z, s32 exp) {
    BonusEntry *entry;
    s32 i;
    char buf[72];

    entry = D_801FC300;
    if (count == 0) {
        return 0;
    }
    D_801FCA2C = exp;
    D_801FCA28 = 0;
    if (((u8 *)D_801D8340)[0x81E] == 0) {
        if (!(FLAGS110(0)->f1 | FLAGS110(0)->f2) && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!(FLAGS110(0)->f0 | FLAGS110(0)->f2) && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!(FLAGS110(0)->f0 | FLAGS110(0)->f1) && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f16 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (((DuelK *)D_801D8340)->bonusFlags[3] == 0 && FLAGS110(0)->f17 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f4 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f3 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f5 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f6 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countOnlineDeckCards(0) > 0 && countOnlineDeckCards(1) == 0 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f14 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(1)->wins == 0 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f7 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(0)->wins == 3 && PLAYER(1)->wins == 2
            && countOnlineDeckCards(0) + countOnlineDeckCards(1) - countEmptyHandSlots(0) + 8 == countEmptyHandSlots(1)
            && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (((DuelK *)D_801D8340)->bonusFlags[13] == 0 && countOnlineDeckCards(0) + 4 == countEmptyHandSlots(0)
            && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f19 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countDeckCardsByFilter(0, (PlayerDeck *)PLAYER(0), 0xC0) >= 25 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countEmptyDpSlots(0) == 0 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countOnlineDeckCards(0) == 7 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f22 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f28 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry += 4;
    } else {
        entry += 21;
        if (FLAGS110(0)->f8 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f18 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(0)->wins == 0 && PLAYER(1)->wins == 3 && func_801FA918(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
    }
    if (FLAGS110(0)->f15 && func_801FA918(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f9 && func_801FA918(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f10 && func_801FA918(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f11 && func_801FA918(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->count == 3 && func_801FA918(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f29 && func_801FA918(entry, x, y, count, z)) {
        return 0;
    }
    entry += 2;
    if (D_801FCA28 >= 7 && func_801FA918(entry, x, y, count, z)) {
        return 0;
    }
    i = 46;
    do {
        drawText(x + i * 5, y + D_801FCA28 * 13, (s32)"-", 7, z);
    } while (--i >= 0);
    sprintf(buf, "+%3d", D_801FCA2C);
    drawText(x + 0xA2, y + D_801FCA28 * 13 + 11, (s32)buf, 5, z);
    return 1;
}

typedef struct {
    u8 unk0[8];
    PolyF4 banner[2];
    DR_MODE bannerMode[2];
} DuelBanner;
#define BANNER ((DuelBanner *)D_801D8340)

void func_801FB444(s32 player, s32 id) {
    s32 show;
    s32 x;
    s32 y;
    s32 h;
    s32 frame;

    if (PLAYER(1)->controller != 1 || player != 0) {
        return;
    }
    show = 0;
    switch (id) {
    case 24:
        if (!FLAGS110(player)->f15) {
            show = 1;
        }
        break;
    case 28:
        if (FLAGS110(player)->count == 2) {
            show = 1;
        }
        break;
    case 29:
        if (!FLAGS110(player)->f29) {
            show = 1;
        }
        break;
    case 10:
        if (!FLAGS110(player)->f14) {
            show = 1;
        }
        break;
    case 27:
        if (!FLAGS110(player)->f11) {
            show = 1;
        }
        break;
    case 8:
        if (!FLAGS110(player)->f6) {
            show = 1;
        }
        break;
    case 15:
        show = 1;
        FLAGS110(player)->f19 = 1;
        break;
    case 19:
        show = 1;
        FLAGS110(player)->f22 = 1;
        break;
    case 25:
        show = 1;
        FLAGS110(player)->f9 = 1;
        break;
    case 26:
        show = 1;
        FLAGS110(player)->f10 = 1;
        break;
    }
    if (show == 0) {
        return;
    }
    x = -200;
    y = 0x6E;
    h = 0;
    frame = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
        if (frame < 100) {
            x += 16;
            if (x > 160) {
                x = 160;
            }
            h++;
            if (h >= 9) {
                h = 8;
            }
        } else {
            x += 16;
            if (x > 0x208) {
                x = 0x208;
            }
            h--;
            if (h < 0) {
                h = 0;
            }
        }
        frame++;
        drawText(x - measureText(D_801FC300[id].name) / 2, y - 6, (s32)D_801FC300[id].name, 7, 0);
        SetDrawTPage(&BANNER->bannerMode[FRAME_BUFFER_INDEX], 0, 0, GetTPage(0, 2, 0, 0));
        initPrimByType(8, &BANNER->banner[FRAME_BUFFER_INDEX], 1, 0);
        setPrimRgb0(&BANNER->banner[FRAME_BUFFER_INDEX], 0xC0, 0xC0, 0xC0);
        SetSemiTrans(&BANNER->banner[FRAME_BUFFER_INDEX], 1);
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x0 = 0;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y0 = y - h;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x1 = 0x140;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y1 = y - h;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x2 = 0;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y2 = y - h + h * 2;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->x3 = 0x140;
        (&BANNER->banner[FRAME_BUFFER_INDEX])->y3 = y - h + h * 2;
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&BANNER->banner[FRAME_BUFFER_INDEX]);
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&BANNER->bannerMode[FRAME_BUFFER_INDEX]);
    } while (frame < 120);
}
