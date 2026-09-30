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
#include "dcb/effect_prims.h"
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
     u8 unk4[8];
} PartInfo;
extern ExpScreen *KAW_EXP_SCREEN;
extern PartInfo KAW_DIGI_PARTS[];

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
s32 KAW_getSupportOperand(s32 arg0, s32 arg1, s32 lhs, s32 rhs, s32 slot);
s32 KAW_applySupportAction(s32 arg0, s32 arg1, s32 kind, s32 value, s32 slot, s32 arg5);
extern s32 KAW_MATCH_LOADING;
extern s32 KAW_BONUS_ROW;
extern s32 KAW_BONUS_EXP;
extern u8 *D_801D485C;
extern s32 KAW_VS_PANEL_POS[2][4];
extern s32 KAW_VS_NAME_POS[2][4];
extern s32 KAW_VS_INNER_LINE_POS[2][4];
extern s32 KAW_VS_OUTER_LINE_POS[2][4];

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

extern char *KAW_EFFECT_HELP_LINES[];

typedef struct {
    s16 id;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Entry8;
s32 KAW_countHandDigimonOfSpecialty(s32 player, s32 attr);
s32 KAW_checkDigivolveTarget(s32 card, s32 player);
extern s8 KAW_EFFECT_PLAYER;
extern s8 KAW_EFFECT_CARD;
extern s8 KAW_EFFECT_TARGET_CARD;
void KAW_runEffectScriptTask(void *data, s32 task);
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

s32 KAW_isCardId(s32 id, s32 player, s32 card);
extern s16 KAW_VOIDING_CARDS[];
s32 KAW_findCardIdInHand(s32 id, s32 player);
extern s16 KAW_PILE_EFFECT_CARDS[];
extern s16 KAW_REVIVE_CARDS[];
extern s16 KAW_RECOVERY_CARDS[];
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
s32 KAW_tickTutorial();
s32 KAW_returnDiscardToHand(s32 player, s32 slot);
s32 KAW_returnDpCardToHand(s32 card, s32 player, s32 slot);
void KAW_drawSprite(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 arg8, s32 arg9, s32 argA, s32 brightness, s32 z);
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
extern DeckScreen *KAW_MATCH_SCREEN;
extern Menu KAW_DECK_LIST_MENUS[];
void KAW_drawDeckList(ListWindow *w);
void KAW_drawDeckInfo(ListWindow *w);
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
extern PrizeScreen *KAW_PRIZE_SCREEN;
void KAW_drawPrizeTitle(UiWindow *window);
void KAW_drawPrizeResult(RewardWindow *w);
void KAW_drawPrizeCard(PrizeWindow *w);
void KAW_playEffectScript(s32 entry, s32 player1, s32 player2, s32 mode1, s32 mode2);
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
extern void (*KAW_EFFECT_FREE_FUNCS[])(u8 *);
void KAW_fadeCardSprite(CardSprite *sprite, u8 *to);
/* SUGSEG's colour quad drawer: in KAWSEG this address is inside KAW_chooseSupportCard */
void func_801E6424(Rect16 *rect, u8 *rgb, u8 *rgb2, u8 arg3, s32 arg4, u8 arg5);
void KAW_initEffectFromParams(EffectTemplate *template, u8 *fx, EffectTable *table);
extern u8 *(*KAW_EFFECT_CREATE_FUNCS[])(s32, EffectTable *);
extern UiWindow KAW_DUEL_MENU_WINDOW;
void KAW_drawDuelMenu(UiWindow *window);
typedef struct {
    u8 data[0x14F0];
} Unk14F0;
extern Unk14F0 *D_801D83F8;
typedef struct {
    /* 0x00 */ DR_MODE dm;
    /* 0x08 */ POLY_G4 prims[8];
} GradPacket;
extern GradPacket *D_801D83F4;
void KAW_drawCursor(void *arg0);
void KAW_renderCursor(void *arg0, s32 arg1);
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

s32 KAW_countDeckDigimon(s32 player) {
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

s32 KAW_countHandDigimon(s32 player) {
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

s32 KAW_countHandDigivolves(s32 player) {
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

s32 KAW_countDeckDigimonOfLevel(s32 player, s32 level) {
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

s32 KAW_countHandDigimonOfLevel(s32 player, s32 level) {
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

s32 KAW_countDeckDigimonOfSpecialty(s32 player, s32 attr) {
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

s32 KAW_countHandDigimonOfSpecialty(s32 player, s32 attr) {
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

s32 KAW_countDeckDigimonOfSpecialtyAndLevel(s32 player, s32 attr, s32 level) {
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

s32 KAW_countHandDigimonOfSpecialtyAndLevel(s32 player, s32 attr, s32 level) {
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

s32 KAW_isCardId(s32 id, s32 player, s32 card) {
    if (card != -1 && ((Player *)DUEL_PLAYERS[player])->cards[card % 30].id == id) {
        return 1;
    }
    return 0;
}

s32 KAW_findCardIdInHand(s32 id, s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (KAW_isCardId(id, player, ((Player *)DUEL_PLAYERS[player])->hand[i])) {
            return i + 1;
        }
    }
    return 0;
}

s32 KAW_hasDigivolveInHand(s32 id, s32 player) {
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

s32 KAW_countStrongerDigimonInHand(s32 player) {
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

s32 KAW_isVoidingCard(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 5; i++) {
        if (KAW_isCardId(KAW_VOIDING_CARDS[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 KAW_findVoidingCardInHand(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 5; i++) {
        found = KAW_findCardIdInHand(KAW_VOIDING_CARDS[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 KAW_isPileEffectCard(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (KAW_isCardId(KAW_PILE_EFFECT_CARDS[i], player, card)) {
            if (KAW_PILE_EFFECT_CARDS[i] != 0x97) {
                return 1;
            }
            if (countOfflineDeckCards(player) >= 8) {
                return 1;
            }
        }
    }
    return 0;
}

s32 KAW_findPileEffectCardInHand(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 24; i++) {
        found = KAW_findCardIdInHand(KAW_PILE_EFFECT_CARDS[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 KAW_findReviveCardInHand(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 4; i++) {
        found = KAW_findCardIdInHand(KAW_REVIVE_CARDS[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 KAW_isRecoveryCard(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < 33; i++) {
        if (KAW_isCardId(KAW_RECOVERY_CARDS[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 KAW_findRecoveryCardInHand(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < 33; i++) {
        found = KAW_findCardIdInHand(KAW_RECOVERY_CARDS[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 KAW_getActiveCrossEffect(s32 player) {
    Player *p;

    if (getActiveDigimonCard(player) == -1) {
        return 0;
    }
    p = (Player *)DUEL_PLAYERS[player];
    return p->cards[getActiveDigimonCard(player) % 30].card[0xE4];
}

s32 KAW_checkSupportCard(s32 player, s32 card) {
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

s32 KAW_simulateBattles(s32 self) {
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
                    KAW_resolveBattle(1);
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

s32 KAW_planDigivolves(s32 player) {
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

/* not referenced by any code */
const s32 D_801DDF38 = 5;

s32 KAW_decideRedraw(s32 player) {
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
        if (KAW_countHandDigimon(player) == 0) {
            return 1;
        }
        if (KAW_countHandDigimonOfLevel(player, 0) != 0) {
            return 0;
        }
        for (i = 0; i < 8; i++) {
            if (KAW_hasDigivolveInHand(i, self) && KAW_countHandDigimon(self) >= 2) {
                switch (i) {
                case 0:
                    if (sumDigivolvePoints(self) >= 20 && KAW_countHandDigimonOfLevel(self, 2) != 0) {
                        return 0;
                    }
                    break;
                case 2:
                    for (j = 0; j < 5; j++) {
                        if (KAW_countDeckDigimonOfSpecialtyAndLevel(self, j, 2) != 0 && KAW_countDeckDigimonOfSpecialtyAndLevel(self, j, 3) != 0) {
                            return 0;
                        }
                    }
                    break;
                case 3:
                    for (j = 2; j < 4; j++) {
                        if (sumDigivolvePoints(self) >= j * 20 && KAW_countHandDigimonOfLevel(self, j) >= 2) {
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
        if (KAW_countDeckDigimonOfLevel(player, 0) != 0) {
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
            if (KAW_hasDigivolveInHand(i, self) && KAW_countHandDigimon(self) != 0) {
                level = PLAYER(self)->cards[getActiveDigimonCard(self) % 30].card[0x1A] & 0xF;
                switch (i) {
                case 0:
                    j = level;
                    if (j == 0) {
                        j = 1;
                    }
                    if (KAW_countHandDigimonOfLevel(self, j + 1) != 0) {
                        return 0;
                    }
                    break;
                case 1:
                    if (level == 0 && KAW_countHandDigimonOfLevel(self, 3) != 0) {
                        return 0;
                    }
                    break;
                case 2:
                    if (PLAYER(self)->statPenalty == 0) {
                        j = level;
                        if (j == 0) {
                            j = 1;
                        }
                        if (KAW_countDeckDigimonOfSpecialtyAndLevel(self, PLAYER(self)->specialty, j + 1) != 0) {
                            return 0;
                        }
                    }
                    break;
                case 3:
                    if (KAW_countStrongerDigimonInHand(self) != 0) {
                        return 0;
                    }
                    break;
                case 5:
                    return 0;
                case 6:
                    if (level == 1) {
                        if (KAW_countDeckDigimonOfSpecialtyAndLevel(self, PLAYER(self)->specialty, 2) != 0 && PLAYER(self)->displayedStats[0] < 300) {
                            return 0;
                        }
                        if (KAW_countDeckDigimonOfSpecialtyAndLevel(self, PLAYER(self)->specialty, 3) != 0) {
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
            && (KAW_findRecoveryCardInHand(player) == 0 || (KAW_findVoidingCardInHand(opponent) != 0 && KAW_getActiveCrossEffect(opponent) == 10))
            && KAW_simulateBattles(player) != 0) {
            if (KAW_planDigivolves(player) == 1) {
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

s32 KAW_compactCandidates(Entry8 *entries, s32 n) {
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

s32 KAW_chooseDigimonToPlace(s32 player) {
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
        if (KAW_countHandDigimonOfLevel(player, level) == 0) {
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
            cands[n].attrCount = KAW_countHandDigimonOfSpecialty(player, attr);
            cands[n].levelCount = KAW_countHandDigimonOfSpecialtyAndLevel(player, attr, 1);
            cands[n].dpCost = PLAYER(player)->cards[card % 30].card[0x1B];
            cands[n].deckCount = KAW_countDeckDigimonOfSpecialty(player, attr);
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
                    n = KAW_compactCandidates((Entry8 *)cands, 4);
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
                    n = KAW_compactCandidates((Entry8 *)cands, 4);
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
                    n = KAW_compactCandidates((Entry8 *)cands, 4);
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
                    n = KAW_compactCandidates((Entry8 *)cands, 4);
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
                    n = KAW_compactCandidates((Entry8 *)cands, 4);
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
                    n = KAW_compactCandidates((Entry8 *)cands, 4);
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
                    n = KAW_compactCandidates((Entry8 *)cands, 4);
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

s32 KAW_keepLowestDpBonus(s16 *cards, s32 player, s32 min) {
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

s32 KAW_keepHighestDpBonus(s16 *cards, s32 player, s32 min) {
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

s32 KAW_keepLoneSpecialty(s16 *cards, s32 player, s32 min) {
    s32 count;
    s32 i;
    s16 card;

    count = 0;
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && KAW_countHandDigimonOfSpecialty(player, (u8)((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[0x1A] >> 4) == 1) {
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && KAW_countHandDigimonOfSpecialty(player, (u8)((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[0x1A] >> 4) != 1) {
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

s32 KAW_keepActiveLevel(s16 *cards, s32 player, s32 min) {
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

s32 KAW_keepWithSupport(s16 *cards, s32 player, s32 min) {
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

s32 KAW_pickRandomCard(s16 *ids, s32 player) {
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

s32 KAW_chooseDpCard(s32 player) {
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
        if ((result = KAW_keepLowestDpBonus(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = KAW_keepHighestDpBonus(ids, self, need)) > 0) {
            return result;
        }
        if ((result = KAW_keepLoneSpecialty(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = KAW_keepActiveLevel(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = KAW_keepWithSupport(ids, self, need)) >= 0) {
            return result;
        }
        return KAW_pickRandomCard(ids, self);
    case 1:
        if (((u8)PLAYER(self)->cards[getActiveDigimonCard(self) % 30].card[0x1A] & 0xF) == 0 && KAW_countHandDigimon(self) < 2) {
            return -1;
        }
        if ((result = KAW_keepLowestDpBonus(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = KAW_keepLoneSpecialty(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = KAW_keepWithSupport(ids, self, need)) >= 0) {
            return result;
        }
        if ((result = KAW_keepHighestDpBonus(ids, self, need)) > 0) {
            return result;
        }
        if ((result = KAW_keepActiveLevel(ids, self, need)) >= 0) {
            return result;
        }
        return KAW_pickRandomCard(ids, self);
    case 2:
        return KAW_pickRandomCard(ids, self);
    }
    return -1;
}

Slot7C8 *KAW_selectDigivolvePlan(s32 kind) {
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

s32 KAW_chooseDigivolveTarget(s32 player) {
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

void KAW_planDigivolveOptions(s32 player) {
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
                if (!PLAYER(player)->hasBattled || flag != 1 || ((DuelK *)D_801D8340)->selected != NULL) {
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

s32 KAW_chooseDigivolveOption(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((DuelK *)D_801D8340)->slots[i].id != -1) {
            return ((DuelK *)D_801D8340)->slots[i].id;
        }
    }
    return -1;
}

#define SIM(i) (DUEL_AI->sims[i])

void KAW_chooseAttack(s32 player) {
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
void KAW_chooseSupportCard(void) {
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
            if (i != 4 && KAW_checkSupportCard(self, PLAYER(self)->hand[i]) != 0) {
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
            if (i != 4 && KAW_checkSupportCard(self, PLAYER(self)->hand[i]) != 0) {
                continue;
            }
            for (j = 0; j < 5; j++) {
                if (j != 4 && KAW_checkSupportCard(opponent, PLAYER(opponent)->hand[j]) != 0) {
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
                if (scores[i].kills != 0 && KAW_isPileEffectCard(self, PLAYER(self)->hand[i]) != 0) {
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
                    if (scores[i].survives != 0 && KAW_isPileEffectCard(self, PLAYER(self)->hand[i]) != 0) {
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
                    if (scores[i].survives != 0 && KAW_isVoidingCard(self, PLAYER(self)->hand[i]) != 0) {
                        if (KAW_isRecoveryCard(opponent, (s8)PLAYER(opponent)->unk1BD[0]) |
                            KAW_isPileEffectCard(opponent, (s8)PLAYER(opponent)->unk1BD[0])) {
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
                    if (scores[i].dies != 0 && KAW_isPileEffectCard(self, PLAYER(self)->hand[i]) != 0) {
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
                    if (scores[i].dies != 0 && KAW_isPileEffectCard(self, PLAYER(self)->hand[i]) != 0) {
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

s32 KAW_runSupportEffect(s32 arg0, s32 arg1, SupportCond *conds, SupportEffect *effects, s32 arg4);
void KAW_applyCrossEffect(s32 self, s32 other, DigimonCardData *cardData, s32 quiet);
void KAW_recordBestDamage(Player *p);
void KAW_playCardEffect(s32 entry, s32 player, s32 mode);

#define CARD_SPR(c) (((CardAnim *)(D_801D833C + (c) * 36))->spr)

s32 KAW_resolveBattle(s32 quiet) {
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
                    KAW_playCardEffect(0x13, player, 1);
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
                    result = KAW_runSupportEffect(player, player ^ 1, (SupportCond *)(effects[k].card + 0x1C),
                                           (SupportEffect *)(effects[k].card + 0x5C), quiet);
                    if (!quiet && result) {
                        KAW_playCardEffect(0x13, player, 1);
                    }
                } else if (!quiet) {
                    KAW_playCardEffect(0x13, player, 1);
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
                    result = KAW_runSupportEffect(player, player ^ 1, (SupportCond *)(effects[k].card + 0x74),
                                           (SupportEffect *)(effects[k].card + 0xB4), quiet);
                    if (!quiet && result) {
                        KAW_playCardEffect(0x13, player, 1);
                    }
                } else if (!quiet) {
                    KAW_playCardEffect(0x13, player, 1);
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
                    KAW_applyCrossEffect(player, player ^ 1, (DigimonCardData *)effects[k].card, quiet);
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
                KAW_playCardEffect(0x13, player, 1);
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
        KAW_recordBestDamage((Player *)attacker);
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
            KAW_recordBestDamage((Player *)defender);
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

extern s32 KAW_SUPPORT_REGISTER;
s32 KAW_getSupportOperand(s32 self, s32 other, s32 kind, s32 value, s32 slot) {
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
        return KAW_SUPPORT_REGISTER;
    case 27:
        return countOnlineDeckCards(self);
    case 28:
        return countOnlineDeckCards(other);
    }
    return 0;
}


extern s32 KAW_SUPPORT_REGISTER;
void KAW_trackSpecialties(s32 player);

#define SHOW_EFFECT_FAILED(player) \
    do {                          \
        KAW_playCardEffect(0x13, player, 1); \
    } while (0)

s32 KAW_applySupportAction(s32 self, s32 other, s32 kind, s32 value, s32 slot, s32 quiet) {
    s32 cards[4];
    u8 unused[0x90];
    s32 i;
    s32 j;
    s32 n;
    s32 card;

    switch (kind) {
    case 0:
        if (!quiet) {
            KAW_playEffectScript(0xF, self, self, 1, 0);
        }
        PLAYER(self)->specialty = value % 5;
        if (!quiet) {
            KAW_trackSpecialties(self);
        }
        break;
    case 1:
        if (!quiet) {
            KAW_playEffectScript(0xF, self, other, 1, 0);
        }
        PLAYER(other)->specialty = value % 5;
        if (!quiet) {
            KAW_trackSpecialties(other);
        }
        break;
    case 2:
        if (!quiet) {
            KAW_playEffectScript(0x14, self, self, 1, 0);
            showStatChangePopup(self, value, 0);
        }
        STATS(self)->hpBeforeBattle = value;
        STATS(self)->stats[0] = value;
        if (!quiet && value != 0 && value % 1110 == 0) {
            KAW_showBonusBanner(self, 0x1A);
            STATS(self)->unk110 |= 0x400;
        }
        break;
    case 3:
        if (!quiet) {
            KAW_playEffectScript(0x14, self, other, 1, 0);
            showStatChangePopup(other, value, 0);
        }
        STATS(other)->hpBeforeBattle = value;
        STATS(other)->stats[0] = value;
        if (!quiet && value != 0 && value % 1110 == 0) {
            KAW_showBonusBanner(other, 0x1A);
            STATS(other)->unk110 |= 0x400;
        }
        break;
    case 4:
    case 6:
    case 8:
        if (!quiet) {
            KAW_playEffectScript(0x14, self, self, 1, 0);
            showStatChangePopup(self, value, (kind - 4) / 2 + 1);
        }
        STATS(self)->attackDamage[(kind - 4) / 2] = value;
        STATS(self)->stats[(kind - 4) / 2 + 1] = value;
        break;
    case 5:
    case 7:
    case 9:
        if (!quiet) {
            KAW_playEffectScript(0x14, self, other, 1, 0);
            showStatChangePopup(other, value, (kind - 5) / 2 + 1);
        }
        STATS(other)->attackDamage[(kind - 5) / 2] = value;
        STATS(other)->stats[(kind - 5) / 2 + 1] = value;
        break;
    case 10:
        if (!quiet) {
            if (slot == 0) {
                KAW_playEffectScript(0x14, self, self, 1, 0);
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
                KAW_playEffectScript(0x14, self, other, 1, 0);
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
            KAW_playEffectScript(0x10, self, self, 1, 0);
            PLAYER(self)->attackChoice = value;
        }
        break;
    case 17:
        PLAYER(other)->usedAttack = value;
        if (!quiet) {
            KAW_playEffectScript(0x10, self, other, 1, 0);
            PLAYER(other)->attackChoice = value;
        }
        break;
    case 25:
        KAW_SUPPORT_REGISTER = value;
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
                KAW_playEffectScript(0x11, self, other, 1, 1);
            } else {
                SHOW_EFFECT_FAILED(self);
            }
        }
        break;
    case 45:
        FLAGS178(other)->f9 = 3;
        if (!quiet) {
            if (getPlayedCard(other) != -1) {
                KAW_playEffectScript(0x12, self, other, 1, 1);
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
                KAW_checkHandBonuses(self);
            } else {
                SHOW_EFFECT_FAILED(self);
            }
        }
        break;
    case 47:
        if (!quiet) {
            KAW_playEffectScript(0x10, self, other, 1, 0);
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
            KAW_playEffect(0xC, self);
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
                KAW_checkHandBonuses(self);
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
                KAW_checkHandBonuses(self);
            }
        }
        break;
    case 51:
        FLAGS178(self)->f12 = 1;
        if (!quiet) {
            KAW_playEffect(0xB, self);
        }
        break;
    case 52:
        FLAGS178(self)->f6 = 1;
        FLAGS178(other)->f6 = 0;
        if (!quiet) {
            KAW_playEffect(0xE, self);
        }
        break;
    case 53:
        FLAGS178(self)->f8 = 1;
        if (!quiet) {
            KAW_playEffect(0xA, self);
        }
        break;
    }
}

void KAW_applyCrossEffect(s32 self, s32 other, DigimonCardData *cardData, s32 quiet) {
    u8 unused[0xB0];
    s32 card;
    s32 value;

    switch (cardData->crossEffect) {
    case 0:
        break;
    case 1:
        FLAGS178(self)->f8 = 1;
        if (!quiet) {
            KAW_playEffect(10, self);
        }
        break;
    case 2:
    case 3:
    case 4:
        if (!quiet) {
            showStatChangePopup(other, 0, cardData->crossEffect - 1);
            KAW_playEffectScript(0x14, self, other, 0, 0);
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
            KAW_playEffect(0xE, self);
        }
        break;
    case 8:
        FLAGS178(self)->f11 = 1;
        if (!quiet) {
            KAW_playEffect(0xD, self);
        }
        break;
    case 9:
        FLAGS178(self)->f12 = 1;
        if (!quiet) {
            KAW_playEffect(0xB, self);
        }
        break;
    case 10:
        FLAGS178(other)->f9 |= 1;
        if (!quiet) {
            card = getPlayedCard(other);
            if (card != -1 && PLAYER(other)->cards[card % 30].card[2] == 0) {
                KAW_playEffectScript(0x11, self, other, 0, 1);
            } else {
                KAW_playEffect(0x13, self);
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
                KAW_playEffect(0x14, self);
            }
            if (value > 9990) {
                value = 9990;
            }
            STATS(self)->attackDamage[2] = value;
            STATS(self)->stats[3] = value;
            FLAGS178(self)->f13 = 1;
        } else if (!quiet) {
            KAW_playEffect(0x13, self);
        }
        break;
    }
}

s32 KAW_calcSupportValue(s32 a, s32 op, s32 b) {
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

s32 KAW_compareSupportValues(s32 a, s32 op, s32 b) {
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

s32 KAW_runSupportEffect(s32 arg0, s32 arg1, SupportCond *conds, SupportEffect *effects, s32 arg4) {
    s32 vals[6];
    s16 slotVals[3][3];
    s32 i;
    s32 j;
    s32 k;
    s32 a;

    for (i = 0; i < 2; i++) {
        if (conds[i].active != 0) {
            for (j = 0; j < 6; j++) {
                vals[j] = KAW_getSupportOperand(arg0, arg1, conds[i].lhs[j], conds[i].rhs[j], 0);
            }
            a = KAW_calcSupportValue(KAW_calcSupportValue(vals[0], conds[i].ops[0], vals[1]), conds[i].ops[1], vals[2]);
            if (KAW_compareSupportValues(a, conds[i].cmp, KAW_calcSupportValue(KAW_calcSupportValue(vals[3], conds[i].ops[2], vals[4]), conds[i].ops[3], vals[5])) == 0) {
                return -1;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        if (effects[i].active != 0) {
            if (effects[i].kind == 10 || effects[i].kind == 11) {
                for (j = 0; j < 3; j++) {
                    for (k = 0; k < 3; k++) {
                        slotVals[j][k] = KAW_getSupportOperand(arg0, arg1, effects[i].lhs[j], effects[i].rhs[j], k);
                    }
                }
                for (k = 0; k < 3; k++) {
                    KAW_applySupportAction(arg0, arg1, effects[i].kind,
                                  KAW_calcSupportValue(KAW_calcSupportValue(slotVals[2][k], effects[i].ops[1], slotVals[1][k]), effects[i].ops[0], slotVals[0][k]),
                                  k, arg4);
                }
            } else {
                for (j = 0; j < 3; j++) {
                    vals[j] = KAW_getSupportOperand(arg0, arg1, effects[i].lhs[j], effects[i].rhs[j], 0);
                }
                KAW_applySupportAction(arg0, arg1, effects[i].kind,
                              KAW_calcSupportValue(KAW_calcSupportValue(vals[2], effects[i].ops[1], vals[1]), effects[i].ops[0], vals[0]), 0, arg4);
            }
        }
    }
    return 0;
}

s32 KAW_checkDigivolveTarget(s32 card, s32 player) {
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

s32 KAW_checkAnyDigivolve(s32 player) {
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
        if (KAW_checkDigivolveTarget(((Player *)DUEL_PLAYERS[player])->hand[i], player) == 0) {
            return 0;
        }
    }
    return -1;
}

s32 KAW_setStatPenalty(s32 card, s32 player) {
    Player *p = (Player *)DUEL_PLAYERS[player];

    p->statPenalty = ((u8 *)p->cards[card % 30].card)[0x1A];
    ((Player *)DUEL_PLAYERS[player])->bonusFlags &= ~0x40000000;
}

void KAW_recordBestDamage(Player *p) {
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

void KAW_startTutorial(void) {
    DUEL->tutorial = 1;
    *(ScriptRunner **)D_801D8340 = allocTaskHeapBlock(sizeof(ScriptRunner));
    func_800149B8(0, -1, 0, 0x800, loadFile, "B:\\BETA.MSD", getCurrentTaskId());
    (*(ScriptRunner **)D_801D8340)->data = (void *)func_80014C08(0x7FFFFFFF);
    (*(ScriptRunner **)D_801D8340)->script = createScriptContext((*(ScriptRunner **)D_801D8340)->data);
    (*(ScriptRunner **)D_801D8340)->regs = allocScriptRegisters(10);
    (*(ScriptRunner **)D_801D8340)->unkC = 0;
    KAW_tickTutorial();
}

void KAW_freeTutorial(void) {
    if (DUEL->tutorial) {
        freeScriptContext((*(ScriptRunner **)D_801D8340)->script, (*(ScriptRunner **)D_801D8340)->regs);
        freeHeapBlock((*(ScriptRunner **)D_801D8340)->data);
        freeHeapBlock(*(ScriptRunner **)D_801D8340);
    }
}

void KAW_drawTutorialText(UiWindow *window) {
    drawText(window->originX, window->originY, *(s32 *)(*(u8 **)D_801D8340 + 0x10), 7, window->z);
}

extern UiWindow KAW_TUTORIAL_WINDOW;

s32 KAW_showTutorialMessage(s32 y, u8 *src) {
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
    openWindow(&KAW_TUTORIAL_WINDOW, &rect, -1, (s16 *)-1, 8, 0x15, 0x80, 8);
    KAW_TUTORIAL_WINDOW.label = (s32)"TUTORIAL";
    KAW_TUTORIAL_WINDOW.palette = 4;
    playSoundEffect(0xA3);
    PAD_INPUT_ENABLED = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (drawWindow(&KAW_TUTORIAL_WINDOW, KAW_drawTutorialText, 0) == 0 || ((PAD_STATES[0]->rawPressed & 0x40) >> 6) == 0);
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_TUTORIAL_WINDOW, (Rect16 *)-1);
    for (i = 0; i < 16; i++) {
        func_80014C08(FRAME_INTERVAL);
        drawWindow(&KAW_TUTORIAL_WINDOW, KAW_drawTutorialText, 0);
    }
    PAD_INPUT_ENABLED = 0;
}

void KAW_closeRing(s32 arg0, s32 arg1, s32 arg2);
void KAW_openRing(void);

s32 KAW_tickTutorial(void) {
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
                    KAW_showTutorialMessage((*(ScriptRunner **)D_801D8340)->regs[8], (u8 *)(*(ScriptRunner **)D_801D8340)->regs[0]);
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
                    KAW_openRing();
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
                    KAW_openCardSelect((s16)(*(ScriptRunner **)D_801D8340)->script->params[0]);
                    break;
                case 5:
                    vars[9] = -1;
                    DUEL->unk81D = -1;
                    KAW_closeCardSelect((s16)(*(ScriptRunner **)D_801D8340)->script->params[0]);
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
                    KAW_closeRing((s16)(*(ScriptRunner **)D_801D8340)->script->params[0], (s16)(*(ScriptRunner **)D_801D8340)->script->params[1],
                                  (s16)(*(ScriptRunner **)D_801D8340)->script->params[2]);
                }
                break;
            }
        }
        clearScriptBusy((*(ScriptRunner **)D_801D8340)->script);
    } while (result != 0);
    DUEL->unk820[0] = 0;
}

void KAW_pickCardArtSlot(void) {
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

s32 KAW_drawHandHints(s32 player) {
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
            if (KAW_checkDigivolveTarget(card, player) != 0) {
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

s32 KAW_tickCardCursor(s32 player, s32 mode) {
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
    KAW_pickCardArtSlot();
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

s32 KAW_openCardSelect(s32 player) {
    D_801D83D1 = (*(u32 *)(DUEL_PLAYERS[player] + 0x178) >> 17) & 3;
    D_801D83EC[player * 0xD8 + 0xD] = player + 1;
}

s32 KAW_closeCardSelect(s32 index) {
    DUEL->cursorSlot = -1;
    DUEL->unk81D = -1;
    D_801D83EC[index * 0xD8 + 0xD] = 5;
}

s32 KAW_drawCardToHand(s32 player) {
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

s32 KAW_discardCard(s32 card, s32 player) {
    if (removeCardFromHand(card, player) != -1) {
        SPRITE_KIND(card) = 8;
    } else if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = 8;
    }
    ((CardAnim *)(D_801D833C + card * 36))->spr->pal = (u8)((Player *)DUEL_PLAYERS[player])->cards[card % 30].card[0x1A] >> 4;
    discardCardToOfflineDeck(card, player);
}

s32 KAW_discardHand(s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (((Player *)DUEL_PLAYERS[player])->hand[i] != -1) {
            SPRITE_KIND(((Player *)DUEL_PLAYERS[player])->hand[i]) = 8;
            discardCardToOfflineDeck(((Player *)DUEL_PLAYERS[player])->hand[i], player);
            removeCardFromHand(((Player *)DUEL_PLAYERS[player])->hand[i], player);
        }
    }
}

s32 KAW_placeDigimonFromHand(s32 card, s32 player) {
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

s32 KAW_returnDigimonToHand(s32 card, s32 player, s32 slot) {
    if (removeCardFromDigimonStack(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

void KAW_returnPlayedCard(s32 player, s32 slot) {
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

s32 KAW_returnDiscardToHand(s32 player, s32 slot) {
    s32 card;

    card = takeOfflineDeckTopCard(player);
    if (card != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

s32 KAW_returnDpCardToHand(s32 card, s32 player, s32 slot) {
    if (removeCardFromDpSlots(card, player) != -1) {
        SPRITE_KIND(card) = 3;
        ((Player *)DUEL_PLAYERS[player])->hand[slot] = card;
        *(s8 *)(D_801D833C + card * 36 + 0x23) = slot;
    }
}

s32 KAW_playOnlineDeckTop(s32 player) {
    s32 card;

    if (countOnlineDeckCards(player) && isPlayedCardSlotEmpty(player)) {
        card = drawOnlineDeckCard(player);
        setPlayedCard(card, player);
        SPRITE_KIND(card) = 21;
    }
}

s32 KAW_playCardFromHand(s32 card, s32 player) {
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

s32 KAW_chargeDpCard(s32 card, s32 player) {
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

s32 KAW_redrawHand(s32 player) {
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
    while (KAW_drawCardToHand(player) != -1) {
        func_80014C08(20);
        KAW_checkHandBonuses(player);
    }
}

s32 KAW_undoDigivolve(void) {
    if (DUEL->discardedFromSlot >= 0) {
        KAW_returnDiscardToHand(DUEL->turnPlayer, DUEL->discardedFromSlot);
        DUEL->discardedFromSlot = -1;
    }
    waitDuelFrames(30);
    if (DUEL->dpFromSlot >= 0) {
        KAW_returnDpCardToHand(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer, DUEL->dpFromSlot);
        DUEL->dpFromSlot = -1;
    }
    waitDuelFrames(30);
    DUEL->step = 11;
}

s32 KAW_discardDpSlots(void) {
    while (peekDpSlotTop(DUEL->turnPlayer) != -1) {
        discardCardToOfflineDeck(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
        SPRITE_KIND(peekDpSlotTop(DUEL->turnPlayer)) = 8;
        removeCardFromDpSlots(peekDpSlotTop(DUEL->turnPlayer), DUEL->turnPlayer);
        waitDuelFrames(20);
    }
    waitDuelFrames(30);
    DUEL->step = 23;
}

s32 KAW_checkKnockout(s32 player) {
    s32 opponent;
    s32 card;
    s32 slot;
    u8 *data;

    opponent = player ^ 1;
    if (PLAYER(player)->stats[0] == 0) {
        if (((u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF) == 3) {
            KAW_showBonusBanner(opponent, 0xF);
        }
        DUEL->winner = opponent;
        if (((*(u32 *)((u8 *)PLAYER(player) + 0x178) >> 14) & 1) && PLAYER(opponent)->wins != 2) {
            KAW_playEffect(0x1D, player);
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
                KAW_playEffect(0x1E, player);
                armorDevolvePartner(player, slot);
            }
            while (getActiveDigimonCard(player) != -1) {
                card = getActiveDigimonCard(player);
                ((CardAnim *)(D_801D833C + card * 36))->spr->pal = (u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4;
                KAW_discardCard(card, player);
                waitDuelFrames(20);
            }
        }
        PLAYER(opponent)->wins++;
        return 1;
    }
    return 0;
}

void KAW_drawCard3D(Icon3D *icon, s32 z, RawPolyFT4 *pk) {
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

void KAW_waitForCross(void) {
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (!(PAD_STATES[0]->pressed & 0x40));
}

void KAW_drawSprite(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 tp, s32 semi, s32 abr, s32 brightness,
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

void KAW_drawDeckName(s32 x, s32 y, char *name) {
    char buf[64];

    sprintf(buf, "%s Deck", name);
    drawText(x + 0x18, y + 3, (s32)buf, 7, 1);
    KAW_drawSprite(x, y, 0x1D0, 0xCA, 0xC0, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

void KAW_drawBattleRecord(s32 x, s32 y, s32 wins, s32 losses) {
    char buf[64];

    sprintf(buf, "*s0%4d        %3d      %3d", wins + losses, wins, losses);
    drawSmallText(x + 0x24, y + 9, (s32)"BATTLES", 6, 1);
    drawSmallText(x + 0x66, y + 9, (s32)"WINS", 6, 1);
    drawSmallText(x + 0x9C, y + 9, (s32)"LOSSES", 6, 1);
    drawText(x + 8, y + 3, (s32)buf, 7, 1);
    KAW_drawSprite(x, y, 0x1D0, 0xB8, 0xC0, 0x12, 0x190, 0xF9, 0, 0, 0, 0x80, 1);
}

void KAW_loadMatchGraphics(s32 isVersus, s32 match, s32 task) {
    char path[64];
    s32 count;
    s32 i;
    u32 *arc;
    s32 width0;
    s32 width1;

    KAW_MATCH_LOADING = 1;
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
    KAW_VS_PANEL_POS[0][0] = 100;
    KAW_VS_PANEL_POS[0][1] = 0xF1;
    KAW_VS_PANEL_POS[0][2] = 0xB8;
    KAW_VS_PANEL_POS[0][3] = 0x79;
    KAW_VS_PANEL_POS[1][0] = 0x4C;
    KAW_VS_PANEL_POS[1][1] = -0x71;
    KAW_VS_PANEL_POS[1][2] = 8;
    KAW_VS_PANEL_POS[1][3] = 7;
    KAW_VS_NAME_POS[0][0] = 0x140;
    KAW_VS_NAME_POS[0][1] = 0xC3;
    KAW_VS_NAME_POS[1][0] = -width1;
    KAW_VS_NAME_POS[1][1] = 0x10;
    KAW_VS_NAME_POS[1][2] = 0x138 - width1;
    KAW_VS_INNER_LINE_POS[0][0] = 0x140;
    KAW_VS_INNER_LINE_POS[0][1] = 0x9F;
    KAW_VS_INNER_LINE_POS[1][0] = -0xC0;
    KAW_VS_INNER_LINE_POS[1][1] = 0x42;
    KAW_VS_OUTER_LINE_POS[0][0] = 0x140;
    KAW_VS_OUTER_LINE_POS[0][1] = 0xB1;
    KAW_VS_OUTER_LINE_POS[1][0] = -0xC0;
    KAW_VS_OUTER_LINE_POS[1][1] = 0x30;
    func_80014C08(10);
    KAW_MATCH_LOADING = 0;
    func_80014A48(task);
}

char *strcat(char *dst, const char *src);

void KAW_drawDeckList(ListWindow *w) {
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
    if (KAW_MATCH_SCREEN->unk504[player] != 0) {
        decks = (PresetDeck *)(((SessionData *)D_8006E054)->npcDeckFile + 8);
        for (i = 0; i < KAW_DECK_LIST_MENUS[player].nrows; i++) {
            if (i < w->window.view.y / KAW_DECK_LIST_MENUS[player].rowH) {
                continue;
            }
            if ((w->window.view.y + w->window.rect.h) / KAW_DECK_LIST_MENUS[player].rowH < i) {
                break;
            }
            y = w->window.originY + i * KAW_DECK_LIST_MENUS[player].rowH;
            y++;
            deck = KAW_MATCH_SCREEN->deckIds[player][i];
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
        updateMenuCursor(&KAW_DECK_LIST_MENUS[player]);
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

extern POLY_G4 KAW_DECK_CHART_POLYS[2][2][3];
extern TILE KAW_DECK_LEVEL_BARS[2][2][4];
extern DR_MODE KAW_DECK_CHART_MODES[2][2];
#define setXY0(p, _x0, _y0) (p)->x0 = _x0, (p)->y0 = _y0
#define setDrawTPage(p, dfe, dtd, tpage) (setlen(p, 1), ((u32 *)(p))[1] = _get_mode(dfe, dtd, tpage))

void KAW_drawDeckChart(s32 x, s32 y, s32 player, s32 z) {
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
    deck = KAW_MATCH_SCREEN->deckIds[player][KAW_DECK_LIST_MENUS[player].row];
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
        initPrimByType(9, &KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i], 0, 0);
        setRGB1(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i], 0xFF, 0xFF, 0xFF);
        KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i].x1 = x + cx;
        KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i].y1 = y + cy;
    }
    setRGB0(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0], 0xFF, 0, 0);
    setRGB2(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0], 0xFF, 0xFF, 0);
    setRGB3(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0], 0, 0, 0);
    setRGB0(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1], 0, 0, 0);
    setRGB2(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1], 0, 0xFF, 0xFF);
    setRGB3(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1], 0, 0xFF, 0);
    setRGB0(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2], 0, 0xFF, 0);
    setRGB2(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2], 0xFF, 0xFF, 0xFF);
    setRGB3(&KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2], 0xFF, 0, 0);
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].x0 = x + cx + rsin(0) * counts[0] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].y0 = y + cy + rcos(0) * counts[0] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].x2 = x + cx + rsin(0x2AA) * counts[1] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].y2 = y + cy + rcos(0x2AA) * counts[1] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].x3 = x + cx + rsin(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][0].y3 = y + cy + rcos(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].x0 = x + cx + rsin(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].y0 = y + cy + rcos(0x554) * counts[2] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].x2 = x + cx + rsin(0x7FE) * counts[3] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].y2 = y + cy + rcos(0x7FE) * counts[3] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].x3 = x + cx + rsin(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][1].y3 = y + cy + rcos(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].x0 = x + cx + rsin(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].y0 = y + cy + rcos(0xAA8) * counts[4] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].x2 = x + cx + rsin(0xD52) * counts[5] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].y2 = y + cy + rcos(0xD52) * counts[5] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].x3 = x + cx + rsin(0) * counts[0] / 4096;
    KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][2].y3 = y + cy + rcos(0) * counts[0] / 4096;
    drawIcon(x - 6 + cx + rsin(0) * 26 / 4096, y - 6 + cy + rcos(0) * 26 / 4096, 0, 0, z);
    drawIcon(x - 6 + cx + rsin(0x2AA) * 26 / 4096, y - 6 + cy + rcos(0x2AA) * 26 / 4096, 0, 4, z);
    drawIcon(x - 6 + cx + rsin(0x554) * 26 / 4096, y - 6 + cy + rcos(0x554) * 26 / 4096, 0, 3, z);
    drawIcon(x - 6 + cx + rsin(0x7FE) * 26 / 4096, y - 6 + cy + rcos(0x7FE) * 26 / 4096, 0, 1, z);
    drawIcon(x - 6 + cx + rsin(0xAA8) * 26 / 4096, y - 6 + cy + rcos(0xAA8) * 26 / 4096, 0, 2, z);
    drawIcon(x - 6 + cx + rsin(0xD52) * 26 / 4096, y - 6 + cy + rcos(0xD52) * 26 / 4096, 0, 5, z);
    for (i = 0; i < 3; i++) {
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &KAW_DECK_CHART_POLYS[player][FRAME_BUFFER_INDEX][i]);
    }
    KAW_drawSprite(x + cx - 0x16, y + cy - 0x1A, 0x3E1, 0x19A, 0x2C, 0x32, 0x3F0, 0x1FD, 0, 0, 0, 0x80, z);
    drawIcon(x + 0x44, y + 0x34, 1, 0x10, z);
    drawIcon(x + 0x50, y + 0x34, 1, 0x12, z);
    drawIcon(x + 0x5C, y + 0x34, 1, 0x13, z);
    drawIcon(x + 0x68, y + 0x34, 1, 5, z);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][0], 0xFF, 0xFF, 0);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][1], 0xFF, 0, 0);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][2], 0, 0, 0xFF);
    setRGB0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][3], 0xFF, 0xFF, 0xFF);
    for (i = 0; i < 4; i++) {
        func_800678C4(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i]);
        setXY0(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i], x + 0x44 + i * 12, y - (bars[i] - 0x32));
        setWH(&KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i], 8, bars[i]);
        addPrim(&CURRENT_FRAME_BUFFER->ot[z], &KAW_DECK_LEVEL_BARS[player][FRAME_BUFFER_INDEX][i]);
    }
    setDrawTPage(&KAW_DECK_CHART_MODES[player][FRAME_BUFFER_INDEX], 0, 0, 0);
    addPrim(&CURRENT_FRAME_BUFFER->ot[z], &KAW_DECK_CHART_MODES[player][FRAME_BUFFER_INDEX]);
}

void KAW_drawDeckInfo(ListWindow *w) {
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
    KAW_drawDeckChart(x, y, player, z);
    deck = KAW_MATCH_SCREEN->deckIds[player][KAW_DECK_LIST_MENUS[player].row];
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

extern Rect16 KAW_DECK_INFO_RECTS[];

void KAW_openDeckList(s32 player) {
    s32 i;
    s32 n;

    markBuildableOpponentDecks(player);
    for (i = 0; i < 0xA2; i++) {
        KAW_MATCH_SCREEN->deckIds[player][i] = 0xFFFF;
    }
    n = 0;
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).savedDecks[i].inUse) {
            KAW_MATCH_SCREEN->deckIds[player][n++] = i;
        }
    }
    for (i = 0; i < 0x9F; i++) {
        if (PLAYER_DATA(player).opponentDeckFlags[i] & 0x4000) {
            KAW_MATCH_SCREEN->deckIds[player][n++] = i + 3;
        }
    }
    KAW_DECK_LIST_MENUS[player].nrows = n;
    KAW_MATCH_SCREEN->lists[player].window.view.h = n * KAW_DECK_LIST_MENUS[player].rowH;
    KAW_MATCH_SCREEN->unk504[player] = 1;
    animateWindowTo(&KAW_MATCH_SCREEN->frames[player].window, &KAW_DECK_INFO_RECTS[player]);
    playSoundEffect(0xA3);
}

void KAW_closeDeckList(s32 i) {
    KAW_DECK_LIST_MENUS[i].nrows = 3;
    KAW_DECK_LIST_MENUS[i].row = 0;
    KAW_MATCH_SCREEN->lists[i].window.scroll[3] = 0;
    KAW_MATCH_SCREEN->lists[i].window.view.y = 0;
    KAW_MATCH_SCREEN->lists[i].window.view.h = KAW_DECK_LIST_MENUS[0].rowH * 3;
    KAW_MATCH_SCREEN->unk504[i] = 0;
    animateWindowTo(&KAW_MATCH_SCREEN->frames[i].window, (Rect16 *)-1);
    playSoundEffect(0xA4);
}

void KAW_renderDeckSelect(void) {
    drawWindow(&KAW_MATCH_SCREEN->lists[0].window, KAW_drawDeckList, 10);
    drawWindow(&KAW_MATCH_SCREEN->frames[0].window, KAW_drawDeckInfo, 10);
    if (KAW_MATCH_SCREEN->unk770 == 0) {
        drawWindow(&KAW_MATCH_SCREEN->lists[1].window, KAW_drawDeckList, 10);
        drawWindow(&KAW_MATCH_SCREEN->frames[1].window, KAW_drawDeckInfo, 10);
    }
}

#define DECK_CHOICE(p) (*(s8 *)&PLAYER_DATA(p).unk30[4])

void KAW_runDeckSelect(s32 isVersus, s32 match) {
    s32 i;
    s32 done;
    u16 pressed;

    KAW_MATCH_SCREEN = allocTaskHeapBlock(0x778);
    func_800149B8(0, -1, 0, 0x800, KAW_loadMatchGraphics, isVersus, match, getCurrentTaskId(), 0);
    KAW_MATCH_SCREEN->unk504[0] = 0;
    KAW_MATCH_SCREEN->unk504[1] = 0;
    KAW_MATCH_SCREEN->unk770 = isVersus;
    if ((DUEL->tutorial == 0 && ((SessionData *)D_8006E054)->npcDeckIndex[0] == -1) || isVersus == 0) {
        ((SessionData *)D_8006E054)->npcDeckIndex[0] = -1;
        ((SessionData *)D_8006E054)->npcDeckIndex[1] = -1;
        if (isVersus != 0) {
            openMenu(&KAW_DECK_LIST_MENUS[0], &KAW_MATCH_SCREEN->lists[0].window, &KAW_MATCH_SCREEN->highlights[0], (Bytes4 *)-1);
            KAW_MATCH_SCREEN->lists[0].player = 0;
            KAW_MATCH_SCREEN->lists[0].window.labelPalette = 7;
            KAW_MATCH_SCREEN->lists[0].window.label = (s32) "PLAYER DECK LIST";
            openWindow(&KAW_MATCH_SCREEN->frames[0], &KAW_DECK_INFO_RECTS[0], -1, (s16 *)-1, 8, 0x55, 0x80, 8);
            animateWindowTo(&KAW_MATCH_SCREEN->frames[0].window, (Rect16 *)-1);
            KAW_MATCH_SCREEN->frames[0].window.labelPalette = 8;
            KAW_MATCH_SCREEN->frames[0].player = 0;
            KAW_MATCH_SCREEN->frames[0].window.label = (s32) "PLAYER DECK INFO.";
            done = 2;
        } else {
            for (i = 0, done = 0; i < 2; i++) {
                openMenu(&KAW_DECK_LIST_MENUS[i], &KAW_MATCH_SCREEN->lists[i].window, &KAW_MATCH_SCREEN->highlights[i], (Bytes4 *)-1);
                KAW_MATCH_SCREEN->lists[i].player = i;
                openWindow(&KAW_MATCH_SCREEN->frames[i], &KAW_DECK_INFO_RECTS[i], -1, (s16 *)-1, 8, 0x55, 0x80, 8);
                animateWindowTo(&KAW_MATCH_SCREEN->frames[i].window, (Rect16 *)-1);
                KAW_MATCH_SCREEN->frames[i].window.labelPalette = 8;
                KAW_MATCH_SCREEN->frames[i].player = i;
                if (i == 0) {
                    KAW_MATCH_SCREEN->lists[0].window.label = (s32) "1P DECK LIST";
                    KAW_MATCH_SCREEN->frames[0].window.label = (s32) "1P DECK INFO.";
                } else {
                    KAW_MATCH_SCREEN->lists[i].window.label = (s32) "2P DECK LIST";
                    KAW_MATCH_SCREEN->frames[i].window.label = (s32) "2P DECK INFO.";
                }
                KAW_MATCH_SCREEN->lists[i].window.labelPalette = 7;
            }
        }
        playSoundEffect(0xA3);
        addFrameCallback((s32)KAW_renderDeckSelect);
        func_80014C08(0x10);
        do {
            func_80014C08(FRAME_INTERVAL);
            if (!(done & 1)) {
                if (KAW_MATCH_SCREEN->unk504[0] != 0) {
                    if (PAD_STATES[0]->pressed & 0x40) {
                        i = KAW_MATCH_SCREEN->deckIds[0][KAW_DECK_LIST_MENUS[0].row];
                        if (((SessionData *)D_8006E054)->unk1010[0x12] != 0) {
                            if (i < 3) {
                                if (((SessionData *)((u8 *)D_8006E054 + i))->unk1010[0x13] == 0) {
                                    playSoundEffect(0xA0);
                                    initDialog(KAW_MATCH_SCREEN->dialog, "This Deck can't be used in this Arena.", 0);
                                    runDialog(KAW_MATCH_SCREEN->dialog);
                                } else {
                                    DECK_CHOICE(0) = i;
                                    done |= 1;
                                }
                            } else {
                                playSoundEffect(0xA0);
                                initDialog(KAW_MATCH_SCREEN->dialog, "Base Deck can't be used in this Arena.", 0);
                                runDialog(KAW_MATCH_SCREEN->dialog);
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
                        KAW_closeDeckList(0);
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
                                initDialog(KAW_MATCH_SCREEN->dialog, "This Deck can't be used in this Arena.", 0);
                                runDialog(KAW_MATCH_SCREEN->dialog);
                                i = -1;
                            }
                            if (i != -1 && PLAYER_DATA(0).savedDecks[i].inUse) {
                                DECK_CHOICE(0) = i;
                                done |= 1;
                            }
                        } else {
                            KAW_openDeckList(0);
                        }
                    }
                }
                if (done & 1) {
                    animateWindowTo(&KAW_MATCH_SCREEN->lists[0].window, (Rect16 *)-1);
                    animateWindowTo(&KAW_MATCH_SCREEN->frames[0].window, (Rect16 *)-1);
                    playSoundEffect(0xA0);
                }
            }
            if (isVersus == 0 && !(done & 2)) {
                if (KAW_MATCH_SCREEN->unk504[1] != 0) {
                    if (PAD_STATES[1]->pressed & 0x40) {
                        playSoundEffect(0xA0);
                        i = KAW_MATCH_SCREEN->deckIds[1][KAW_DECK_LIST_MENUS[1].row];
                        if (i < 3) {
                            DECK_CHOICE(1) = i;
                        } else {
                            ((SessionData *)D_8006E054)->npcDeckIndex[1] = i - 3;
                            DECK_CHOICE(1) = -1;
                        }
                        done |= 2;
                    } else if (PAD_STATES[1]->pressed & 0x10) {
                        KAW_closeDeckList(1);
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
                            KAW_openDeckList(1);
                        }
                    }
                }
                if (done & 2) {
                    animateWindowTo(&KAW_MATCH_SCREEN->lists[1].window, (Rect16 *)-1);
                    animateWindowTo(&KAW_MATCH_SCREEN->frames[1].window, (Rect16 *)-1);
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
    removeFrameCallback((s32)KAW_renderDeckSelect);
    markDeckCardsSeen(0);
    freeHeapBlock(((SessionData *)D_8006E054)->npcDeckFile);
    func_80014C08(0x1E);
    while (KAW_MATCH_LOADING != 0) {
        func_80014C08(FRAME_INTERVAL);
    }
    func_80014C08(2);
    freeHeapBlock(KAW_MATCH_SCREEN);
    func_80014C08(2);
}


#define setXYWH(p, _x0, _y0, _w, _h)                                                            \
    (p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0), (p)->x2 = (_x0), \
    (p)->y2 = (_y0) + (_h), (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h)
#define setUVWH(p, _u0, _v0, _w, _h)                                                            \
    (p)->u0 = (_u0), (p)->v0 = (_v0), (p)->u1 = (_u0) + (_w), (p)->v1 = (_v0), (p)->u2 = (_u0), \
    (p)->v2 = (_v0) + (_h), (p)->u3 = (_u0) + (_w), (p)->v3 = (_v0) + (_h)

void KAW_renderVersusScreen(void) {
    char buf[64];
    VersusPrims *prims;
    s32 i;

    prims = (VersusPrims *)CURRENT_FRAME_BUFFER->primSlots[15];
    if (KAW_MATCH_SCREEN->introState != 0) {
        switch (KAW_MATCH_SCREEN->introState) {
        case 1:
            KAW_MATCH_SCREEN->unk510 = 0;
            KAW_MATCH_SCREEN->introZoom = 0;
            KAW_MATCH_SCREEN->introBrightness = 0x80;
            KAW_MATCH_SCREEN->introState++;
        case 2:
            KAW_MATCH_SCREEN->introZoom += 10;
            if (KAW_MATCH_SCREEN->introZoom > 150) {
                KAW_MATCH_SCREEN->introState++;
            }
            break;
        case 3:
            KAW_MATCH_SCREEN->introZoom -= 5;
            if (KAW_MATCH_SCREEN->introZoom < 100) {
                KAW_MATCH_SCREEN->introZoom = 100;
                KAW_MATCH_SCREEN->introState++;
            }
            break;
        case 4:
            KAW_MATCH_SCREEN->introBrightness -= 12;
            if (KAW_MATCH_SCREEN->introBrightness <= 0) {
                KAW_MATCH_SCREEN->introBrightness = 0;
                KAW_MATCH_SCREEN->introState = 0;
            }
            break;
        }
        setRGB0(&prims->intro, KAW_MATCH_SCREEN->introBrightness, KAW_MATCH_SCREEN->introBrightness, KAW_MATCH_SCREEN->introBrightness);
        setXYWH(&prims->intro, 160 - (KAW_MATCH_SCREEN->introZoom * 64) / 100, 120 - (KAW_MATCH_SCREEN->introZoom * 64) / 100,
                (KAW_MATCH_SCREEN->introZoom * 128) / 100, (KAW_MATCH_SCREEN->introZoom * 128) / 100);
        setUVWH(&prims->intro, 0x60, 0x28, 0x80, 0x80);
        prims->intro.tpage = 0x26;
        prims->intro.clut = 0x3E19;
        addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->intro);
    }
    i = (KAW_MATCH_SCREEN->barH * 192) / 40;
    setRGB0(&prims->fade, i, i, i);
    setXYWH(&prims->fade, 0, 120 - KAW_MATCH_SCREEN->barH, 320, KAW_MATCH_SCREEN->barH * 2);
    setlen(&prims->fadeMode, 1);
    prims->fadeMode.code[0] = 0xE1000040;
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->fade);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->fadeMode);
    setRGB0(&prims->bars[0], 0xFF, 0xFF, 0);
    setXYWH(&prims->bars[0], 160 - KAW_MATCH_SCREEN->barW, 119 - KAW_MATCH_SCREEN->barH, KAW_MATCH_SCREEN->barW * 2, 1);
    setRGB0(&prims->bars[1], 0xFF, 0xFF, 0);
    setXYWH(&prims->bars[1], 160 - KAW_MATCH_SCREEN->barW, KAW_MATCH_SCREEN->barH + 120, KAW_MATCH_SCREEN->barW * 2, 1);
    setlen(&prims->barMode, 1);
    prims->barMode.code[0] = 0xE1000000;
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->bars[0]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->bars[1]);
    addPrim(&CURRENT_FRAME_BUFFER->ot[2], &prims->barMode);
    if (KAW_MATCH_SCREEN->logoShown != 0) {
        setRGB0(&prims->logo, 0x80, 0x80, 0x80);
        setXYWH(&prims->logo, 160 - (KAW_MATCH_SCREEN->logoScale * 32) / 100, 120 - KAW_MATCH_SCREEN->logoScale / 5,
                (KAW_MATCH_SCREEN->logoScale * 64) / 100, (KAW_MATCH_SCREEN->logoScale * 40) / 100);
        setUVWH(&prims->logo, 0x60, 0, 0x40, 0x28);
        prims->logo.tpage = 6;
        prims->logo.clut = 0x3E18;
        addPrim(&CURRENT_FRAME_BUFFER->ot[1], &prims->logo);
    }
    for (i = 0; i < 2; i++) {
        KAW_drawCard3D(&KAW_MATCH_SCREEN->cards[i], 1, &prims->cards[i]);
    }
    i = (((PlayerProfile *)PLAYER_PROFILES)->playTime * 8) % 256;
    if (i >= 0x80) {
        KAW_MATCH_SCREEN->pulse = 0x17F - i;
    } else {
        KAW_MATCH_SCREEN->pulse = i + 0x80;
    }
    i = 0;
    KAW_drawSprite(KAW_VS_NAME_POS[0][0], KAW_VS_NAME_POS[0][1], 0x2C0, 0x1C0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 1);
    KAW_drawSprite(KAW_VS_PANEL_POS[0][0] - 8, KAW_VS_PANEL_POS[0][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[0][0] + 0x78, KAW_VS_PANEL_POS[0][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[0][0] - 0x20, KAW_VS_PANEL_POS[0][1], 0x180, 0, 0x20, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
    KAW_drawSprite(KAW_VS_PANEL_POS[0][0] + 0x80, KAW_VS_PANEL_POS[0][1], 0x188, 0, 8, 0x70, 0x180, 0xFB, 0, 0, 0, 0x80, 3);
    KAW_drawDeckName(KAW_VS_INNER_LINE_POS[0][0], KAW_VS_INNER_LINE_POS[0][1], (char *)DUEL_PLAYERS[i] + 1);
    KAW_drawBattleRecord(KAW_VS_OUTER_LINE_POS[0][0], KAW_VS_OUTER_LINE_POS[0][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
    KAW_drawSprite(KAW_VS_PANEL_POS[0][0], KAW_VS_PANEL_POS[0][1], 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
    i = 1;
    KAW_drawSprite(KAW_VS_NAME_POS[1][0], KAW_VS_NAME_POS[1][1], 0x2C0, 0x1E0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 1);
    KAW_drawSprite(KAW_VS_PANEL_POS[1][0] - 6, KAW_VS_PANEL_POS[1][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[1][0] + 0x7A, KAW_VS_PANEL_POS[1][1], 0x194, 0, 0x10, 0x70, 0x180, 0xFA, 0, 1, 1, KAW_MATCH_SCREEN->pulse, 2);
    KAW_drawSprite(KAW_VS_PANEL_POS[1][0] - 8, KAW_VS_PANEL_POS[1][1], 0x18A, 0, 8, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
    KAW_drawSprite(KAW_VS_PANEL_POS[1][0] + 0x80, KAW_VS_PANEL_POS[1][1], 0x18C, 0, 0x20, 0x70, 0x180, 0xFC, 0, 0, 0, 0x80, 3);
    KAW_drawDeckName(KAW_VS_OUTER_LINE_POS[1][0], KAW_VS_OUTER_LINE_POS[1][1], (char *)DUEL_PLAYERS[i] + 1);
    KAW_drawBattleRecord(KAW_VS_INNER_LINE_POS[1][0], KAW_VS_INNER_LINE_POS[1][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
    KAW_drawSprite(KAW_VS_PANEL_POS[1][0], KAW_VS_PANEL_POS[1][1], 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
    if (KAW_MATCH_SCREEN->timer > 0x20) {
        if (KAW_MATCH_SCREEN->cards[KAW_MATCH_SCREEN->chosen - 2].u != 0) {
            KAW_drawSprite(KAW_VS_PANEL_POS[0][0] + 0x4C, KAW_VS_PANEL_POS[0][1] + 0x4C, 0x1B4, 0, 0x30, 0x20, 0x190, 0xFD, 0, 1, 1, 0x80, 3);
            KAW_drawSprite(KAW_VS_PANEL_POS[1][0] + 4, KAW_VS_PANEL_POS[1][1] + 4, 0x1A8, 0, 0x30, 0x20, 0x190, 0xFC, 0, 1, 1, 0x80, 3);
        } else {
            KAW_drawSprite(KAW_VS_PANEL_POS[0][0] + 0x4C, KAW_VS_PANEL_POS[0][1] + 0x4C, 0x1A8, 0, 0x30, 0x20, 0x190, 0xFC, 0, 1, 1, 0x80, 3);
            KAW_drawSprite(KAW_VS_PANEL_POS[1][0] + 4, KAW_VS_PANEL_POS[1][1] + 4, 0x1B4, 0, 0x30, 0x20, 0x190, 0xFD, 0, 1, 1, 0x80, 3);
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

extern u8 KAW_DARKNESS_WAVE_ORDER[32];
void loadDuelCardGraphics();
void KAW_freeCursor(void *ptr);
void KAW_drawCursorAt();

/* libgte's setVector */
#define setVector(v, _x, _y, _z) (v)->vx = (_x), (v)->vy = (_y), (v)->vz = (_z)

void KAW_runVersusIntro(s32 mode, s32 deckId) {
    s32 i;
    s32 j;
    s32 frame;
    s32 step;
    s32 k;
    char buf[64];

    KAW_MATCH_SCREEN = allocTaskHeapBlock(sizeof(DeckScreen));
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
            KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
            KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
        } else {
            KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
            KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
        }
    }
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[15] = (s32)&KAW_MATCH_SCREEN->prims[i];
        initPrimByType(0xC, &KAW_MATCH_SCREEN->prims[i].intro, 1, 0);
        initPrimByType(0xC, &KAW_MATCH_SCREEN->prims[i].logo, 1, 0);
        initPrimByType(8, &KAW_MATCH_SCREEN->prims[i].fade, 1, 0);
        for (j = 0; j < 2; j++) {
            initPrimByType(8, &KAW_MATCH_SCREEN->prims[i].bars[j], 0, 0);
        }
    }
    j = rand() % 2;
    for (i = 0; i < 2; i++) {
        KAW_MATCH_SCREEN->cards[i].code = 0x2C;
        setRGB0(&KAW_MATCH_SCREEN->cards[i], 0x80, 0x80, 0x80);
        k = i ^ j;
        if (((DuelK *)D_801D8340)->tutorial) {
            k = 1;
        }
        KAW_MATCH_SCREEN->cards[i].tpage = ((k * 10 + 0x180) & 0x3FF) >> 6;
        KAW_MATCH_SCREEN->cards[i].clut = ((k + 0xFA) << 6) | 0x19;
        KAW_MATCH_SCREEN->cards[i].u = (k * 10 + 0x180) % 64 * 4;
        KAW_MATCH_SCREEN->cards[i].v = 0x70;
        setVector(&KAW_MATCH_SCREEN->cards[i].pos, i * 400 - 200, 0, 0);
        setVector(&KAW_MATCH_SCREEN->cards[i].rot, 0x2000, 0x2800 - (i << 12), 0x2000);
        PLAYER(i)->shufflePasses = 0;
    }
    KAW_MATCH_SCREEN->unk770 = mode;
    KAW_MATCH_SCREEN->deckId = deckId;
    KAW_MATCH_SCREEN->introState = 0;
    KAW_MATCH_SCREEN->logoShown = 0;
    KAW_MATCH_SCREEN->logoScale = 0;
    KAW_MATCH_SCREEN->choice = 0;
    KAW_MATCH_SCREEN->pulse = 0;
    KAW_MATCH_SCREEN->chosen = 0;
    KAW_MATCH_SCREEN->timer = 0;
    KAW_MATCH_SCREEN->barW = 0;
    KAW_MATCH_SCREEN->barH = 0;
    KAW_MATCH_SCREEN->cursor = (s16 *)KAW_createCursor(1, 0x12, 0x16, 6, 1);
    addFrameCallback((s32)KAW_renderVersusScreen);
    step = ((DuelK *)D_801D8340)->tutorial;
    do {
        func_80014C08(FRAME_INTERVAL);
        frame++;
        for (i = 0; i < 2; i++) {
            if (frame > 0) {
                STEP_TOWARD(KAW_VS_PANEL_POS[i][1], KAW_VS_PANEL_POS[i][3], 12);
            }
            if (frame > 20) {
                STEP_TOWARD(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][2], 8);
            }
            if (frame > 30) {
                STEP_TOWARD(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][2], 24);
            }
            if (frame > 40) {
                STEP_TOWARD(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][2], 24);
            }
            if (frame > 50) {
                STEP_TOWARD(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][2], 24);
            }
        }
        if (frame == 8) {
            KAW_MATCH_SCREEN->introState = 1;
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
            if (KAW_MATCH_SCREEN->timer < 60) {
                if ((KAW_MATCH_SCREEN->barH += 2) > 35) {
                    KAW_MATCH_SCREEN->barH = 35;
                }
            } else {
                if ((KAW_MATCH_SCREEN->barH -= 2) < 0) {
                    KAW_MATCH_SCREEN->barH = 0;
                }
            }
        }
        if (frame > 20) {
            if ((KAW_MATCH_SCREEN->barW += 16) > 160) {
                KAW_MATCH_SCREEN->barW = 160;
            }
        }
        if (KAW_MATCH_SCREEN->timer == 80) {
            KAW_MATCH_SCREEN->introState = 1;
            playSoundEffect(0x83);
        }
        if (KAW_MATCH_SCREEN->timer > 80) {
            KAW_MATCH_SCREEN->logoShown = 1;
            if (KAW_MATCH_SCREEN->timer < 92) {
                KAW_MATCH_SCREEN->logoScale += 10;
            } else {
                if ((KAW_MATCH_SCREEN->logoScale -= 5) < 100) {
                    KAW_MATCH_SCREEN->logoScale = 100;
                }
            }
        }
        if (frame == 80) {
            playSoundEffect(0xA5);
        }
        if (KAW_MATCH_SCREEN->timer == 51) {
            playSoundEffect(0xA5);
        }
        if (frame > 80) {
            if (KAW_MATCH_SCREEN->chosen >= 2) {
                i = KAW_MATCH_SCREEN->chosen - 2;
                if (KAW_MATCH_SCREEN->cards[i].rot.vy != 0x2000) {
                    if (KAW_MATCH_SCREEN->cards[i].rot.vy < 0x2000) {
                        KAW_MATCH_SCREEN->cards[i].rot.vy += 0x40;
                    } else {
                        KAW_MATCH_SCREEN->cards[i].rot.vy -= 0x40;
                    }
                }
                if (KAW_MATCH_SCREEN->timer >= 50) {
                    if (step == 2) {
                        KAW_showTutorialMessage(0x34, "It looks like I go first!");
                        PAD_INPUT_ENABLED = 1;
                        step = 3;
                    }
                    for (i = 0; i < 2; i++) {
                        if (abs(KAW_MATCH_SCREEN->cards[i].pos.vx) >= 200) {
                            if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                                KAW_MATCH_SCREEN->cards[i].pos.vx = -200;
                            } else {
                                KAW_MATCH_SCREEN->cards[i].pos.vx = 200;
                            }
                        } else if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                            KAW_MATCH_SCREEN->cards[i].pos.vx -= 10;
                        } else {
                            KAW_MATCH_SCREEN->cards[i].pos.vx += 10;
                        }
                    }
                }
            } else {
                for (i = 0; i < 2; i++) {
                    if (abs(KAW_MATCH_SCREEN->cards[i].pos.vx) <= 80) {
                        if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                            KAW_MATCH_SCREEN->cards[i].pos.vx = -80;
                        } else {
                            KAW_MATCH_SCREEN->cards[i].pos.vx = 80;
                        }
                        KAW_MATCH_SCREEN->chosen = 1;
                    } else if (KAW_MATCH_SCREEN->cards[i].pos.vx < 0) {
                        KAW_MATCH_SCREEN->cards[i].pos.vx += 8;
                    } else {
                        KAW_MATCH_SCREEN->cards[i].pos.vx -= 8;
                    }
                }
            }
        }
        if (KAW_MATCH_SCREEN->chosen == 1) {
            if (step == 1) {
                KAW_showTutorialMessage(0x34, "Let's decide who gets 1st Turn.\nChoose a Card with the directional\nbuttons and press the *b2 button.");
                PAD_INPUT_ENABLED = 1;
                step = 2;
            }
            if ((u16)PAD_STATES[0]->pressed & 0x2000) {
                if (KAW_MATCH_SCREEN->choice == 0) {
                    playSoundEffect(0xA2);
                    KAW_MATCH_SCREEN->choice = 1;
                }
            }
            if ((u16)(PAD_STATES[0]->pressed & 0x8000)) {
                if (KAW_MATCH_SCREEN->choice == 1) {
                    playSoundEffect(0xA2);
                    KAW_MATCH_SCREEN->choice = 0;
                }
            }
            KAW_drawCursorAt(KAW_MATCH_SCREEN->cursor, KAW_MATCH_SCREEN->choice * 160 + 0x4F, 0x78);
            if (PAD_STATES[0]->pressed & 0x40) {
                playSoundEffect(0xA6);
                KAW_MATCH_SCREEN->chosen = KAW_MATCH_SCREEN->choice + 2;
                KAW_MATCH_SCREEN->timer = 1;
            }
        }
        if (KAW_MATCH_SCREEN->timer != 0) {
            KAW_MATCH_SCREEN->timer++;
        }
    } while (!DUEL_VRAM_READY || KAW_MATCH_SCREEN->timer < 181);
    func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 2, 8, 0);
    func_80014C08(40);
    if (!((DuelK *)D_801D8340)->tutorial) {
        for (i = 0; i < 2; i++) {
            PLAYER(i)->shufflePasses += 600;
            shuffleOnlineDeck(i);
        }
        if (deckId == 0x8C) {
            for (i = 0; i < 30; i++) {
                PLAYER(1)->onlineDeck[i] = KAW_DARKNESS_WAVE_ORDER[i] + 0x1D;
            }
        }
    }
    if (KAW_MATCH_SCREEN->cards[KAW_MATCH_SCREEN->chosen - 2].u != 0) {
        ((u8 *)D_801D8340)[0x817] = 1;
    } else {
        ((u8 *)D_801D8340)[0x817] = 0;
    }
    removeFrameCallback((s32)KAW_renderVersusScreen);
    KAW_freeCursor(KAW_MATCH_SCREEN->cursor);
    func_80014C08(2);
    freeHeapBlock(KAW_MATCH_SCREEN);
    func_80014C08(2);
}


extern s32 KAW_RESULT_SCREEN_STATE;
extern const char KAW_FMT_WIN_ARC_PATH[];

void KAW_runResultScreen(s32 mode, s32 winner, s32 deckId) {
    char path[64];
    s32 scale;
    u32 *arc;
    s32 i;
    s32 frame;
    VersusPrims *prims;

    KAW_RESULT_SCREEN_STATE = -1;
    if (mode == 0) {
        deckId = 999;
    }
    sprintf(path, KAW_FMT_WIN_ARC_PATH, deckId);
    i = 0;
    func_800149B8(0, -1, 0, 0x800, loadFile, path, getCurrentTaskId());
    arc = (u32 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; i < (s32)(arc[0] / 4); i++) {
        uploadTim((u32 *)((u8 *)arc + arc[i]), -1, -1, -1, -1);
        func_80014C08(FRAME_INTERVAL);
        DrawSync(0);
    }
    freeHeapBlock(arc);
    KAW_MATCH_SCREEN = allocTaskHeapBlock(sizeof(DeckScreen));
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[15] = (s32)&KAW_MATCH_SCREEN->prims[i];
    }
    KAW_MATCH_SCREEN->unk770 = mode;
    KAW_MATCH_SCREEN->deckId = deckId;
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
    KAW_VS_PANEL_POS[0][0] = 0x140;
    KAW_VS_PANEL_POS[0][1] = 0x78;
    KAW_VS_PANEL_POS[0][2] = 0;
    KAW_VS_PANEL_POS[0][3] = 0x78;
    KAW_VS_PANEL_POS[1][0] = -0x140;
    KAW_VS_PANEL_POS[1][1] = 0;
    KAW_VS_PANEL_POS[1][2] = 0;
    KAW_VS_PANEL_POS[1][3] = 0;
    KAW_VS_NAME_POS[0][0] = 0x140;
    KAW_VS_NAME_POS[0][1] = 0xC3;
    KAW_VS_NAME_POS[1][0] = -*(s16 *)PLAYER(1)->unk118;
    KAW_VS_NAME_POS[1][1] = 0x10;
    KAW_VS_NAME_POS[1][2] = 0x138 - *(s16 *)PLAYER(1)->unk118;
    KAW_VS_INNER_LINE_POS[0][0] = 0x140;
    KAW_VS_INNER_LINE_POS[0][1] = 0x9F;
    KAW_VS_INNER_LINE_POS[1][0] = -0xC0;
    KAW_VS_INNER_LINE_POS[1][1] = 0x42;
    KAW_VS_OUTER_LINE_POS[0][0] = 0x140;
    KAW_VS_OUTER_LINE_POS[0][1] = 0xB1;
    KAW_VS_OUTER_LINE_POS[1][0] = -0xC0;
    KAW_VS_OUTER_LINE_POS[1][1] = 0x30;
    frame = 0;
    scale = 200;
    for (i = 0; i < 2; i++) {
        if (mode != 0) {
            if (winner == i) {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    KAW_MATCH_SCREEN->wins[i]++;
                }
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
            } else {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleWins;
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].battleLosses;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    KAW_MATCH_SCREEN->losses[i]++;
                }
            }
        } else {
            if (winner == i) {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    KAW_MATCH_SCREEN->wins[i]++;
                }
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
            } else {
                KAW_MATCH_SCREEN->wins[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusWins;
                KAW_MATCH_SCREEN->losses[i] = ((PlayerProfile *)PLAYER_PROFILES)[i].versusLosses;
                if (!((DuelK *)D_801D8340)->tutorial) {
                    KAW_MATCH_SCREEN->losses[i]++;
                }
            }
        }
        if (KAW_MATCH_SCREEN->wins[i] >= 1000) {
            KAW_MATCH_SCREEN->wins[i] = 999;
        }
        if (KAW_MATCH_SCREEN->losses[i] >= 1000) {
            KAW_MATCH_SCREEN->losses[i] = 999;
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
                setXYWH(&prims->intro, KAW_VS_PANEL_POS[1][0] - (s16)(scale * 64 / 100 - 74), KAW_VS_PANEL_POS[1][1] - (s16)(scale * 56 / 100 - 60),
                        scale * 128 / 100, scale * 112 / 100);
            } else {
                setXYWH(&prims->intro, KAW_VS_PANEL_POS[0][0] - (s16)(scale * 64 / 100 - 248), KAW_VS_PANEL_POS[0][1] - (s16)(scale * 56 / 100 - 60),
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
                STEP_TOWARD(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][2], 16);
            }
            if (frame > 20) {
                STEP_TOWARD(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][2], 24);
            }
            if (frame > 30) {
                STEP_TOWARD(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][2], 24);
            }
            if (frame > 40) {
                STEP_TOWARD(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][2], 24);
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
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1C0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D7, 0, 1, 0, 0x80, 0);
        KAW_drawDeckName(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], (char *)DUEL_PLAYERS[i] + 1);
        KAW_drawBattleRecord(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0xB8, KAW_VS_PANEL_POS[i][1] + 4, 0x140, 0, 0x80, 0x70, 0x140, 0xFE, 1, 0, 0, 0x80, 4);
        if (!((DuelK *)D_801D8340)->tutorial) {
            if (winner == 0) {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
            } else {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
            }
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x180, 0x78, 0x100, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x100, KAW_VS_PANEL_POS[i][1], 0x1C0, 0x78, 0x40, 0x78, 0x180, 0xF9, 0, 0, 0, 0x80, 4);
        i = 1;
        KAW_drawSprite(KAW_VS_NAME_POS[i][0], KAW_VS_NAME_POS[i][1], 0x2C0, 0x1E0, *(s16 *)PLAYER(i)->unk118, 0x20, 0x2F0, 0x1D8, 0, 1, 0, 0x80, 0);
        KAW_drawDeckName(KAW_VS_OUTER_LINE_POS[i][0], KAW_VS_OUTER_LINE_POS[i][1], (char *)DUEL_PLAYERS[i] + 1);
        KAW_drawBattleRecord(KAW_VS_INNER_LINE_POS[i][0], KAW_VS_INNER_LINE_POS[i][1], KAW_MATCH_SCREEN->wins[i], KAW_MATCH_SCREEN->losses[i]);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 8, KAW_VS_PANEL_POS[i][1] + 4, 0x140, 0x70, 0x80, 0x70, 0x140, 0xFF, 1, 0, 0, 0x80, 4);
        if (!((DuelK *)D_801D8340)->tutorial) {
            if (winner != 0) {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x96, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0, 0xA0, 0x5C, 0x180, 0xF8, 0, 0, 0, 0x80, 4);
            } else {
                KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x96, KAW_VS_PANEL_POS[i][1] + 0x14, 0x1D0, 0x5C, 0xA0, 0x5C, 0x180, 0xFC, 0, 0, 0, 0x80, 4);
            }
        }
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0], KAW_VS_PANEL_POS[i][1], 0x180, 0, 0x100, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        KAW_drawSprite(KAW_VS_PANEL_POS[i][0] + 0x100, KAW_VS_PANEL_POS[i][1], 0x1C0, 0, 0x40, 0x78, 0x180, 0xFB, 0, 0, 0, 0x80, 4);
        if (frame > 30) {
            KAW_RESULT_SCREEN_STATE = 1;
        }
    } while (frame < 100 || !(PAD_STATES[0]->pressed & 0x40));
    KAW_RESULT_SCREEN_STATE = 0;
    func_80014C08(2);
    freeHeapBlock(KAW_MATCH_SCREEN);
    func_80014C08(2);
}

/* the last three bytes are leftovers in the original, not zero padding */
const char KAW_FMT_WIN_ARC_PATH[20] = "B:\\WIN\\%3.3d.ARC\0\x02\x24\x41";

s16 KAW_PILE_EFFECT_CARDS[24] = {
    0x6B, 0x71, 0x76, 0x8B, 0x99, 0x9A, 0x9B, 0x9C,
    0x9D, 0x9E, 0x9F, 0xA0, 0xB6, 0xBE, 0xC9, 0xD4,
    0xD5, 0xD6, 0xE1, 0xF7, 0xFF, 0x101, 0x123, 0x124,
};

s16 KAW_VOIDING_CARDS[6] = {
    0x50, 0x98, 0xA8, 0xFD, 0x121, 0,
};

s16 KAW_RECOVERY_CARDS[34] = {
    0x23, 0x26, 0x27, 0x2F, 0x30, 0x34, 0x37, 0x46,
    0x4E, 0x57, 0x86, 0x8E, 0x93, 0xA6, 0xC5, 0xCE,
    0xCF, 0xD0, 0xD1, 0xD2, 0xDD, 0xDE, 0xDF, 0xE0,
    0xF2, 0xF3, 0xF4, 0xF6, 0xF8, 0x103, 0x10A, 0x112,
    0x11E, 0,
};

s16 KAW_REVIVE_CARDS[4] = {
    3, 0x6F, 0xEF, 0x113,
};

s32 KAW_VS_PANEL_POS[2][4] = {
    { 0x64, 0xF1, 0xB8, 0x79 },
    { 0x4C, -0x71, 8, 7 },
};

s32 KAW_VS_NAME_POS[2][4] = {
    { 0x140, 0xC3, 8, 0xC3 },
    { -0x100, 0x10, 0, 0x10 },
};

s32 KAW_VS_INNER_LINE_POS[2][4] = {
    { 0x140, 0x9F, 8, 0x9F },
    { -0xC0, 0x42, 0x78, 0x42 },
};

s32 KAW_VS_OUTER_LINE_POS[2][4] = {
    { 0x140, 0xB1, 8, 0xB1 },
    { -0xC0, 0x30, 0x78, 0x30 },
};

Menu KAW_DECK_LIST_MENUS[2] = {
    { NULL, NULL, { 24, 40, 132, 56 }, 0, -1, 0, -1, 0xA, 0x81, 132, 12, 1, 1, 2, 1, 0, 14, 0, 0, 0 },
    { NULL, NULL, { 24, 140, 132, 56 }, 0, -1, 0, -1, 0xA, 0x81, 132, 12, 1, 1, 2, 1, 0, 14, 0, 0, 1 },
};

Rect16 KAW_DECK_INFO_RECTS[2] = {
    { 0xBC, 0x1E, 0x76, 0x4C },
    { 0xBC, 0x82, 0x76, 0x4C },
};

/* the online deck of deck 0x8C, as card ids less 0x1D; the code reads 30 */
u8 KAW_DARKNESS_WAVE_ORDER[32] = {
    0x1D, 0xA, 1, 0x16, 4, 0x1C, 0xE, 0x18, 0xB, 0xF, 5, 0x1A, 0x19, 0x10, 0x15, 3,
    0x12, 6, 0x13, 0x17, 0xC, 0x1E, 7, 2, 0x14, 0x11, 0x1B, 0xD, 9, 8, 0x11, 0xE,
};

/* the partner abilities; the code here only reads their texts */
PartInfo KAW_DIGI_PARTS[128] = {
    { "HP+50.", { 3, 5, 1, 99, 3, 7, 0, 0 } },
    { "HP+100.", { 17, 16, 8, 12, 14, 19, 0, 0 } },
    { "HP+150.", { 29, 32, 19, 25, 32, 33, 0, 0 } },
    { "HP+200.", { 45, 48, 31, 40, 52, 59, 0, 0 } },
    { "HP+300.", { 61, 67, 51, 57, 68, 78, 0, 0 } },
    { "HP+400.", { 96, 89, 71, 81, 91, 88, 0, 0 } },
    { "HP+500.", { 99, 0xFF, 75, 91, 0xFF, 95, 0, 0 } },
    { "All Attack Powers +50.", { 18, 39, 54, 30, 57, 28, 0, 0 } },
    { "All Attack Powers +100.", { 39, 86, 72, 50, 89, 51, 0, 0 } },
    { "All Attack Powers +200.", { 75, 0xFF, 90, 93, 0xFF, 84, 0, 0 } },
    { "*b0 Attack Power +100.", { 1, 9, 15, 5, 4, 2, 0, 0 } },
    { "*b0 Attack Power +150.", { 10, 21, 38, 22, 16, 15, 0, 0 } },
    { "*b0 Attack Power +200.", { 27, 54, 62, 41, 39, 30, 0, 0 } },
    { "*b0 Attack Power +250.", { 48, 0xFF, 78, 61, 62, 61, 0, 0 } },
    { "*b0 Attack Power +300.", { 67, 0xFF, 0xFF, 85, 82, 79, 0, 0 } },
    { "*b1 Attack Power +50.", { 4, 1, 12, 2, 6, 13, 0, 0 } },
    { "*b1 Attack Power +100.", { 12, 7, 22, 16, 18, 29, 0, 0 } },
    { "*b1 Attack Power +150.", { 32, 28, 45, 36, 45, 43, 0, 0 } },
    { "*b1 Attack Power +200.", { 51, 44, 0xFF, 56, 67, 66, 0, 0 } },
    { "*b1 Attack Power +250.", { 77, 0xFF, 0xFF, 76, 86, 96, 0, 0 } },
    { "*b2 Attack Power +50.", { 6, 6, 2, 4, 10, 1, 0, 0 } },
    { "*b2 Attack Power +100.", { 19, 36, 20, 19, 27, 9, 0, 0 } },
    { "*b2 Attack Power +150.", { 35, 69, 42, 38, 53, 24, 0, 0 } },
    { "*b2 Attack Power +200.", { 82, 0xFF, 63, 67, 0xFF, 48, 0, 0 } },
    { "*b0 to 0, *b2 Attack Power -100.", { 24, 12, 4, 14, 0xFF, 34, 0, 0 } },
    { "*b1 to 0, *b2 Attack Power -100.", { 46, 23, 9, 35, 0xFF, 17, 0, 0 } },
    { "*b2 to 0, *b2 Attack Power -100.", { 58, 71, 36, 10, 0xFF, 71, 0, 0 } },
    { "Counter *b0,*b2 Attack Power to 0.", { 11, 26, 0xFF, 0xFF, 40, 62, 0, 0 } },
    { "Counter *b1,*b2 Attack Power to 0.", { 36, 14, 0xFF, 0xFF, 24, 49, 0, 0 } },
    { "Counter *b2,*b2 Attack Power to 0.", { 40, 57, 0xFF, 0xFF, 11, 21, 0, 0 } },
    { "Opponent *a0 X3, *b2 Attack Power -200.", { 87, 92, 39, 69, 31, 50, 0, 0 } },
    { "Opponent *a1 X3, *b2 Attack Power -200.", { 25, 97, 0xFF, 42, 77, 73, 0, 0 } },
    { "Opponent *a2 X3, *b2 Attack Power -200.", { 37, 0xFF, 66, 0xFF, 83, 4, 0, 0 } },
    { "Opponent *a3 X3, *b2 Attack Power -200.", { 68, 33, 46, 21, 5, 97, 0, 0 } },
    { "Opponent *a4 X3, *b2 Attack Power -200.", { 52, 41, 6, 0xFF, 0xFF, 25, 0, 0 } },
    { "1st Attack, *b2 Attack Power -200.", { 76, 50, 95, 62, 0xFF, 0xFF, 0, 0 } },
    { "Jamming Support, *b2 Attack Power -100.", { 0xFF, 74, 27, 0xFF, 54, 10, 0, 0 } },
    { "Eat-up HP, *b2 Attack Power -200.", { 0xFF, 87, 50, 0xFF, 73, 0xFF, 0, 0 } },
    { "Add + 10 DP.", { 42, 4, 61, 1, 9, 0xFF, 0, 0 } },
    { "Add + 20 DP.", { 91, 29, 79, 27, 21, 0xFF, 0, 0 } },
    { "Add + 30 DP.", { 0xFF, 98, 97, 60, 78, 85, 0, 0 } },
    { "Boost Attack Power +50.", { 2, 10, 13, 3, 7, 11, 0, 0 } },
    { "Boost Attack Power +100.", { 21, 42, 56, 33, 36, 35, 0, 0 } },
    { "Boost Attack Power +200.", { 26, 81, 73, 46, 84, 44, 0, 0 } },
    { "Boost Attack Power +300.", { 69, 0xFF, 84, 82, 0xFF, 80, 0, 0 } },
    { "Attack Power is Doubled.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Boost *b0 Attack Power +300.", { 8, 30, 47, 17, 26, 12, 0, 0 } },
    { "Boost *b0 Attack Power +400.", { 34, 77, 74, 47, 46, 39, 0, 0 } },
    { "Boost *b0 Attack Power +500.", { 55, 0xFF, 88, 88, 66, 74, 0, 0 } },
    { "*b0 Attack Power is Doubled.", { 13, 64, 67, 58, 33, 63, 0, 0 } },
    { "*b0 Attack Power is Tripled.", { 59, 0xFF, 94, 74, 0xFF, 86, 0, 0 } },
    { "Boost *b1 Attack Power +200.", { 15, 2, 0xFF, 7, 22, 18, 0, 0 } },
    { "Boost *b1 Attack Power +300.", { 28, 19, 0xFF, 23, 41, 36, 0, 0 } },
    { "Boost *b1 Attack Power +400.", { 43, 70, 0xFF, 92, 59, 53, 0, 0 } },
    { "*b1 Attack Power is Doubled.", { 22, 13, 59, 34, 0xFF, 67, 0, 0 } },
    { "*b1 Attack Power is Tripled.", { 83, 0xFF, 91, 94, 0xFF, 89, 0, 0 } },
    { "Boost *b2 Attack Power +100.", { 23, 40, 10, 18, 0xFF, 8, 0, 0 } },
    { "Boost *b2 Attack Power +200.", { 38, 58, 40, 43, 0xFF, 20, 0, 0 } },
    { "Boost *b2 Attack Power +300.", { 71, 0xFF, 60, 63, 0xFF, 90, 0, 0 } },
    { "*b2 Attack Power is Doubled.", { 44, 72, 48, 51, 0xFF, 40, 0, 0 } },
    { "*b2 Attack Power is Tripled.", { 88, 0xFF, 85, 95, 0xFF, 65, 0, 0 } },
    { "Attack Power becomes same as HP.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Get 1st Attack.", { 53, 55, 98, 71, 85, 0xFF, 0, 0 } },
    { "Attack becomes Eat-up HP.", { 0xFF, 93, 80, 0xFF, 61, 55, 0, 0 } },
    { "Lower Opponent's *b0 Attack Power to 0.", { 54, 0xFF, 17, 0xFF, 28, 75, 0, 0 } },
    { "Lower Opponent's *b1 Attack Power to 0.", { 84, 24, 28, 0xFF, 15, 0xFF, 0, 0 } },
    { "Lower Opponent's *b2 Attack Power to 0.", { 0xFF, 66, 24, 0xFF, 13, 45, 0, 0 } },
    { "*b0 Counterattack (Attack 2nd).", { 7, 0xFF, 33, 0xFF, 47, 14, 0, 0 } },
    { "*b1 Counterattack (Attack 2nd).", { 33, 17, 0xFF, 0xFF, 37, 26, 0, 0 } },
    { "*b2 Counterattack (Attack 2nd).", { 47, 43, 5, 0xFF, 0xFF, 37, 0, 0 } },
    { "If *a0 Opponent, X2 own Attack Power.", { 74, 79, 0xFF, 77, 25, 38, 0, 0 } },
    { "If *a0 Opponent, X3 own Attack Power.", { 0xFF, 91, 0xFF, 65, 74, 82, 0, 0 } },
    { "If *a1 Opponent, X2 own Attack Power.", { 5, 47, 64, 26, 80, 76, 0, 0 } },
    { "If *a1 Opponent, X3 own Attack Power.", { 60, 0xFF, 89, 83, 98, 91, 0, 0 } },
    { "If *a2 Opponent, X2 own Attack Power.", { 50, 27, 58, 59, 42, 6, 0, 0 } },
    { "If *a2 Opponent, X3 own Attack Power.", { 79, 0xFF, 93, 0xFF, 93, 70, 0, 0 } },
    { "If *a3 Opponent, X2 own Attack Power.", { 56, 37, 44, 6, 19, 93, 0, 0 } },
    { "If *a3 Opponent, X3 own Attack Power.", { 92, 60, 87, 79, 63, 0xFF, 0, 0 } },
    { "If *a4 Opponent, X2 own Attack Power.", { 78, 76, 26, 72, 69, 46, 0, 0 } },
    { "If *a4 Opponent, X3 own Attack Power.", { 89, 96, 55, 0xFF, 0xFF, 68, 0, 0 } },
    { "Change own Specialty to *a0.", { 9, 35, 0xFF, 48, 96, 0xFF, 0, 0 } },
    { "Change own Specialty to *a1.", { 0xFF, 99, 30, 87, 58, 0xFF, 0, 0 } },
    { "Change own Specialty to *a2.", { 97, 3, 41, 8, 12, 94, 0, 0 } },
    { "Change own Specialty to *a3.", { 30, 0xFF, 68, 98, 81, 3, 0, 0 } },
    { "Change own Specialty to *a4.", { 95, 82, 14, 52, 76, 99, 0, 0 } },
    { "Switch Opponent's Specialty to own.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Swap Specialty with Opponent's.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *a0 Opponent, lower its AP to 0.", { 66, 52, 0xFF, 97, 43, 0xFF, 0, 0 } },
    { "If *a1 Opponent, lower its AP to 0.", { 41, 61, 34, 84, 8, 0xFF, 0, 0 } },
    { "If *a2 Opponent, lower its AP to 0.", { 0xFF, 11, 52, 0xFF, 64, 22, 0, 0 } },
    { "If *a3 Opponent, lower its AP to 0.", { 0xFF, 25, 37, 13, 2, 72, 0, 0 } },
    { "If *a4 Opponent, lower its AP to 0.", { 0xFF, 75, 16, 0xFF, 34, 52, 0, 0 } },
    { "Reduce both Players' Atk Pwr to 0.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "If *e3, boost Attack Power +200.", { 14, 31, 0xFF, 9, 48, 23, 0, 0 } },
    { "If *e4, boost Attack Power +300.", { 31, 53, 0xFF, 20, 38, 57, 0, 0 } },
    { "If *e5, boost Attack Power +400.", { 57, 0xFF, 81, 70, 95, 77, 0, 0 } },
    { "Opponent uses *b0 Attack.", { 16, 62, 0xFF, 28, 97, 31, 0, 0 } },
    { "Opponent uses *b1 Attack.", { 62, 18, 11, 39, 0xFF, 47, 0, 0 } },
    { "Opponent uses *b2 Attack.", { 94, 8, 21, 0xFF, 44, 58, 0, 0 } },
    { "Opponent uses same Attack.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Recover HP +200.", { 72, 15, 32, 11, 1, 81, 0, 0 } },
    { "Recover HP +300.", { 90, 38, 53, 31, 17, 0xFF, 0, 0 } },
    { "Recover HP +400.", { 0xFF, 68, 92, 73, 55, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +400.", { 93, 20, 43, 15, 23, 0xFF, 0, 0 } },
    { "Halve Attack Power, recover HP +600.", { 0xFF, 46, 86, 44, 60, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +500.", { 98, 59, 23, 37, 29, 0xFF, 0, 0 } },
    { "If HP < Opponent's HP, add HP +700.", { 0xFF, 83, 69, 53, 56, 0xFF, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 300.", { 49, 34, 0xFF, 24, 30, 42, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 600.", { 0xFF, 63, 0xFF, 49, 49, 64, 0, 0 } },
    { "If KO'd in battle, revive w/ HP 1000.", { 0xFF, 94, 0xFF, 64, 65, 98, 0, 0 } },
    { "Drop 1 Card in Opponent's Hand.", { 63, 22, 3, 45, 75, 27, 0, 0 } },
    { "Drop 2 Cards in Opponent's Hand.", { 0xFF, 78, 25, 0xFF, 92, 56, 0, 0 } },
    { "Drop Opponent's Top 2 DP Cards shown.", { 0xFF, 56, 18, 86, 35, 5, 0, 0 } },
    { "Drop Opponent's Top 3 DP Cards shown.", { 0xFF, 88, 49, 0xFF, 90, 32, 0, 0 } },
    { "Drop Opponent's Top 4 DP Cards shown.", { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0, 0 } },
    { "Drop 2 Cards in Opponent's Online Deck.", { 0xFF, 51, 7, 29, 0xFF, 16, 0, 0 } },
    { "Drop 3 Cards in Opponent's Online Deck.", { 0xFF, 95, 35, 89, 0xFF, 54, 0, 0 } },
    { "Move Offline Top Card to Online Deck.", { 86, 0xFF, 70, 78, 50, 0xFF, 0, 0 } },
    { "Void Opponent's Support Effect.", { 0xFF, 84, 65, 0xFF, 87, 69, 0, 0 } },
    { "Draw until there are 4 Cards.", { 64, 45, 29, 54, 20, 0xFF, 0, 0 } },
    { "Draw Online Partner Card, then Shuffle.", { 65, 80, 82, 68, 72, 0xFF, 0, 0 } },
    { "If *e3, HP + 200 & all Attack Powers +100.", { 73, 73, 0xFF, 55, 94, 83, 0, 0 } },
    { "If *ea, HP + 200 & all Attack Powers +100.", { 81, 0xFF, 76, 66, 79, 87, 0, 0 } },
    { "Boost Battle Experience by 10%.", { 20, 49, 57, 32, 71, 41, 0, 0 } },
    { "Boost Battle Experience by 20%.", { 85, 65, 77, 75, 88, 92, 0, 0 } },
    { "Boost Battle Experience by 30%.", { 80, 0xFF, 96, 90, 99, 60, 0, 0 } },
    { "Rare Card might appear after battle.", { 70, 85, 83, 80, 51, 0xFF, 0, 0 } },
    { "Rare Card even more likely to appear.", { 0xFF, 90, 99, 96, 70, 0xFF, 0, 0 } },
};

void KAW_drawExpTitle(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned Experience Points", 7, 0);
}

void KAW_drawPortrait(s32 x, s32 y, s32 arg2, s32 arg3, s32 arg4, u16 arg5);

void KAW_drawPartnerExp(ExpWindow *w) {
    s32 x;
    s32 y;
    s32 z;
    s32 next;
    s32 attr;
    char buf[72];

    x = w->window.originX;
    y = w->window.originY;
    z = w->window.z;
    if (KAW_EXP_SCREEN->partnerShown[w->partner]) {
        attr = ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attr >> 4;
        KAW_drawPortrait(x + 0x22, y + 1, w->partner * 20 + 0x2C0, 0x128, attr, w->clut);
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
        if (KAW_EXP_SCREEN->gains[w->partner][0]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][0]);
            drawText(x + 0x2E, y + 1, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0xD, 0, 7, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[0].power);
        drawText(x + 0xE, y + 0xD, (s32)buf, 7, z);
        if (KAW_EXP_SCREEN->gains[w->partner][1]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][1]);
            drawText(x + 0x2E, y + 0xD, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0x19, 0, 8, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[1].power);
        drawText(x + 0xE, y + 0x19, (s32)buf, 7, z);
        if (KAW_EXP_SCREEN->gains[w->partner][2]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][2]);
            drawText(x + 0x2E, y + 0x19, (s32)buf, 5, z);
        }
        drawIcon(x, y + 0x25, 0, 9, z);
        sprintf(buf, "*s0%4d", ((PlayerProfile *)PLAYER_PROFILES)->partners[w->partner].card[0].attack[2].power);
        drawText(x + 0xE, y + 0x25, (s32)buf, 7, z);
        if (KAW_EXP_SCREEN->gains[w->partner][3]) {
            sprintf(buf, "*s0+%d", KAW_EXP_SCREEN->gains[w->partner][3]);
            drawText(x + 0x2E, y + 0x25, (s32)buf, 5, z);
        }
    }
}

s32 KAW_drawBonuses(s32 x, s32 y, s32 count, s32 z, s32 exp);

void KAW_drawBonusList(UiWindow *w) {
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
    KAW_EXP_SCREEN->done = KAW_drawBonuses(x, y + 0x1C, KAW_EXP_SCREEN->progress / 32, z, exp);
    if (PAD_STATES[0]->pressed & 0x40) {
        KAW_EXP_SCREEN->speed = 0x20;
    }
    if (KAW_EXP_SCREEN->done == 0) {
        KAW_EXP_SCREEN->progress += KAW_EXP_SCREEN->speed;
        w->view.h = KAW_EXP_SCREEN->progress / 32 * 13 + 0x26;
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

void KAW_drawEarnedParts(UiWindow *w) {
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
        if ((KAW_EXP_SCREEN->partFlags[i / 8] >> (i % 8)) & 1) {
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
            drawText(x + 0x30, y + 15 + (n - 1) * 13, (s32)KAW_DIGI_PARTS[i].name, palette, z);
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

void KAW_drawRankUp(RankUpWindow *w) {
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

void KAW_renderExpScreen(void) {
    s32 i;

    drawWindow(&KAW_EXP_SCREEN->unk88, KAW_drawEarnedParts, 0);
    drawWindow(&KAW_EXP_SCREEN->window, KAW_drawBonusList, 0);
    drawWindow(&KAW_EXP_SCREEN->titleWindow, KAW_drawExpTitle, 0);
    for (i = 0; i < 3; i++) {
        drawWindow(&KAW_EXP_SCREEN->rankWindows[i].window, KAW_drawRankUp, 0);
        drawWindow(&KAW_EXP_SCREEN->expWindows[i].window, KAW_drawPartnerExp, 0);
    }
}

void KAW_uploadPartnerPortraits(u8 *archive) {
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

void KAW_countEarnedBonuses(void);

void KAW_runExpScreen(void) {
    Rect16 rect;
    s32 i;
    s32 j;
    s32 n;
    s32 gained;

    KAW_EXP_SCREEN = allocPermanentHeapBlock(sizeof(ExpScreen));
    KAW_EXP_SCREEN->progress = 0;
    KAW_EXP_SCREEN->speed = 1;
    KAW_EXP_SCREEN->done = 0;
    for (i = 0; i < 3; i++) {
        KAW_EXP_SCREEN->partnerShown[i] = 0;
        for (j = 0; j < 4; j++) {
            KAW_EXP_SCREEN->gains[i][j] = 0;
        }
    }
    for (i = 0; i < 30; i++) {
        n = findPartnerSlot(0, PLAYER(0)->cards[i].id);
        if (n >= 0 && n < 3) {
            KAW_EXP_SCREEN->partnerShown[n] = 1;
        }
        n = findArmorPartnerSlot(0, PLAYER(0)->cards[i].id);
        if (n >= 0 && n < 3) {
            KAW_EXP_SCREEN->partnerShown[n] = 1;
        }
    }
    for (i = 0; i < 16; i++) {
        KAW_EXP_SCREEN->partFlags[i] = 0;
    }
    rect.x = 0x30;
    rect.y = 0x2C;
    rect.w = 0xE0;
    rect.h = 0xA8;
    openWindow(&KAW_EXP_SCREEN->window, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    KAW_EXP_SCREEN->window.label = (s32)"BONUS LIST";
    animateWindowTo(&KAW_EXP_SCREEN->window, (Rect16 *)-1);
    rect.x = 0x10;
    rect.y = 0x2C;
    rect.w = 0x120;
    rect.h = 0xA8;
    openWindow(&KAW_EXP_SCREEN->unk88, &rect, -1, (s16 *)-1, 10, 0x16, 0x80, 12);
    KAW_EXP_SCREEN->unk88.label = (s32)"DIGI-PARTS RECEIVED";
    animateWindowTo(&KAW_EXP_SCREEN->unk88, (Rect16 *)-1);
    rect.x = 0x10;
    rect.y = 0x14;
    rect.w = 0x120;
    rect.h = 0xE;
    openWindow(&KAW_EXP_SCREEN->titleWindow, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        KAW_EXP_SCREEN->expWindows[i].partner = i;
        rect.x = 0x10;
        rect.y = i * 60 + 0x2C;
        rect.w = 0x120;
        rect.h = 0x32;
        openWindow(&KAW_EXP_SCREEN->expWindows[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        rect.x = 0x14;
        rect.y = i * 60 + 0x40;
        rect.w = 0x60;
        rect.h = 9;
        openWindow(&KAW_EXP_SCREEN->rankWindows[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        KAW_EXP_SCREEN->rankWindows[i].rank = 0;
        KAW_EXP_SCREEN->rankWindows[i].window.palette = 2;
        animateWindowTo(&KAW_EXP_SCREEN->rankWindows[i].window, (Rect16 *)-1);
        if (((PlayerProfile *)PLAYER_PROFILES)->partners[i].cardId != 0) {
            KAW_EXP_SCREEN->expWindows[i].clut = ((DuelK *)D_801D8340)->partnerCluts[i];
        }
    }
    playSoundEffect(0xA3);
    addFrameCallback((s32)KAW_renderExpScreen);
    func_80014C08(20);
    playSoundEffect(0xA3);
    rect.x = 0x30;
    rect.y = 0x2C;
    rect.w = 0xE0;
    rect.h = 0xA8;
    animateWindowTo(&KAW_EXP_SCREEN->window, &rect);
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (KAW_EXP_SCREEN->done == 0 || !(PAD_STATES[0]->pressed & 0x40));
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_EXP_SCREEN->window, (Rect16 *)-1);
    func_80014C08(20);
    for (i = 0; i < 3; i++) {
        KAW_EXP_SCREEN->pendingExp[i] = 0;
        if (KAW_EXP_SCREEN->partnerShown[i] && (s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level < 99) {
            KAW_EXP_SCREEN->pendingExp[i] = KAW_BONUS_EXP + ((PlayerProfile *)PLAYER_PROFILES)->partners[i].expBonus * KAW_BONUS_EXP / 100;
        }
    }
    gained = 0;
    if (KAW_EXP_SCREEN->pendingExp[0] + KAW_EXP_SCREEN->pendingExp[1] + KAW_EXP_SCREEN->pendingExp[2] != 0) {
        do {
            func_80014C08(3);
            for (i = 0; i < 3; i++) {
                if (KAW_EXP_SCREEN->partnerShown[i] && KAW_EXP_SCREEN->pendingExp[i] != 0) {
                    if ((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level < 99) {
                        ((PlayerProfile *)PLAYER_PROFILES)->partners[i].exp++;
                        if ((u16)((PlayerProfile *)PLAYER_PROFILES)->partners[i].exp >= getExpForNextLevel((s8)((PlayerProfile *)PLAYER_PROFILES)->partners[i].level)) {
                            ((PlayerProfile *)PLAYER_PROFILES)->partners[i].level++;
                            rect.x = 0x14;
                            rect.y = i * 60 + 0x40;
                            rect.w = 0x60;
                            rect.h = 9;
                            animateWindowTo(&KAW_EXP_SCREEN->rankWindows[i].window, &rect);
                            KAW_EXP_SCREEN->rankWindows[i].rank++;
                            n = findNewPartnerAbility((AbilityLearnEntry *)KAW_DIGI_PARTS, 0, i);
                            if (n >= 0) {
                                KAW_EXP_SCREEN->partFlags[n / 8] |= 1 << (n % 8);
                                grantPartnerAbility(0, n);
                            }
                            n = func_8004994C(0, i);
                            if (n >= 0) {
                                KAW_EXP_SCREEN->gains[i][n] += 10;
                                gained = 1;
                            }
                        }
                        KAW_EXP_SCREEN->pendingExp[i]--;
                    } else {
                        KAW_EXP_SCREEN->pendingExp[i] = 0;
                    }
                }
            }
            playSoundEffect(0xAA);
        } while (KAW_EXP_SCREEN->pendingExp[0] + KAW_EXP_SCREEN->pendingExp[1] + KAW_EXP_SCREEN->pendingExp[2] != 0);
    }
    KAW_waitForCross();
    if (gained) {
        do {
            func_80014C08(3);
            n = 0;
            for (i = 0; i < 3; i++) {
                if (KAW_EXP_SCREEN->partnerShown[i]
                    && KAW_EXP_SCREEN->gains[i][0] + KAW_EXP_SCREEN->gains[i][1] + KAW_EXP_SCREEN->gains[i][2] + KAW_EXP_SCREEN->gains[i][3] != 0) {
                    n++;
                    if (KAW_EXP_SCREEN->gains[i][0] != 0) {
                        ((PlayerProfile *)PLAYER_PROFILES)->partners[i].hpBonus++;
                        KAW_EXP_SCREEN->gains[i][0]--;
                    }
                    for (j = 0; j < 3; j++) {
                        if (KAW_EXP_SCREEN->gains[i][j + 1] != 0) {
                            ((PlayerProfile *)PLAYER_PROFILES)->partners[i].attackBonus[j]++;
                            KAW_EXP_SCREEN->gains[i][j + 1]--;
                        }
                    }
                    updatePartnerStats(0, i);
                }
            }
            playSoundEffect(0xAA);
        } while (n != 0);
        KAW_waitForCross();
    }
    playSoundEffect(0xA3);
    rect.x = 0x10;
    rect.y = 0x2C;
    rect.w = 0x120;
    rect.h = 0xA8;
    animateWindowTo(&KAW_EXP_SCREEN->unk88, &rect);
    KAW_waitForCross();
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_EXP_SCREEN->unk88, (Rect16 *)-1);
    animateWindowTo(&KAW_EXP_SCREEN->titleWindow, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&KAW_EXP_SCREEN->rankWindows[i].window, (Rect16 *)-1);
        animateWindowTo(&KAW_EXP_SCREEN->expWindows[i].window, (Rect16 *)-1);
    }
    func_80014C08(30);
    KAW_countEarnedBonuses();
    removeFrameCallback((s32)KAW_renderExpScreen);
    func_80014C08(2);
    freeHeapBlock(KAW_EXP_SCREEN);
    func_80014C08(2);
}

void KAW_drawPortraitColored(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb);

void KAW_drawPrizeCard(PrizeWindow *win) {
    /* arrays, not literals: GCC would share KAW_drawPartnerExp's "*s0%4d", and the
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

    if (KAW_PRIZE_SCREEN->showRewards != 0) {
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
    KAW_drawPortraitColored(x + 1, y + 0xC, win->index * 20 + 0x2C0, 0x100, specialty, win->clut, &rgb);
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

void KAW_drawPrizeResult(RewardWindow *w) {
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

void KAW_drawPrizeTitle(UiWindow *window) {
    drawText(window->originX + 2, window->originY + 1, (s32)"Earned a Prize Pack", 7, 0);
    drawText(window->originX + 0x92, window->originY + 1, (s32)D_8006E31C[((u8 *)D_8006E054)[0x73]], 6, 0);
}

void KAW_renderPrizeScreen(void) {
    s32 i;

    drawWindow(&KAW_PRIZE_SCREEN->window, KAW_drawPrizeTitle, 0);
    for (i = 0; i < 3; i++) {
        if (KAW_PRIZE_SCREEN->showRewards) {
            drawWindow(&KAW_PRIZE_SCREEN->rewards[i].window, KAW_drawPrizeResult, 0);
        }
        drawWindow(&KAW_PRIZE_SCREEN->prizes[i].window, KAW_drawPrizeCard, 0);
    }
}

void rollRewardCards(s32 player, s32 level);

void KAW_rollPrizeCards(u8 *archive) {
    s32 i;

    rollRewardCards(0, ((u8 *)D_8006E054)[0x73]);
    for (i = 0; i < 3; i++) {
        uploadTim((u32 *)(archive + ((s32 *)archive)[((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i]]), i * 20 + 0x2C0, 0x100, -1, -1);
        ((DuelK *)D_801D8340)->rewardCluts[i] = getClut(LOADED_TIM.crect->x, LOADED_TIM.crect->y);
    }
}

void KAW_runPrizeScreen(void) {
    Rect16 rect;
    s32 i;

    KAW_PRIZE_SCREEN = allocPermanentHeapBlock(sizeof(PrizeScreen));
    addRewardCardsToCollection(0);
    KAW_PRIZE_SCREEN->showRewards = 0;
    rect.x = 16;
    rect.y = 16;
    rect.w = 0x120;
    rect.h = 14;
    openWindow(&KAW_PRIZE_SCREEN->window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    for (i = 0; i < 3; i++) {
        KAW_PRIZE_SCREEN->prizes[i].index = i;
        KAW_PRIZE_SCREEN->prizes[i].cardId = ((PlayerProfile *)PLAYER_PROFILES)->rewardCards[i];
        KAW_PRIZE_SCREEN->prizes[i].clut = ((DuelK *)D_801D8340)->rewardCluts[i];
        rect.x = 16;
        rect.y = i * 65 + 0x26;
        rect.w = 0x120;
        rect.h = 0x3C;
        openWindow(&KAW_PRIZE_SCREEN->prizes[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
    }
    playSoundEffect(0xA3);
    addFrameCallback((s32)KAW_renderPrizeScreen);
    func_80014C08(20);
    KAW_waitForCross();
    playSoundEffect(0xA0);
    KAW_PRIZE_SCREEN->showRewards = 1;
    for (i = 0; i < 3; i++) {
        rect.x = 200;
        rect.y = i * 65 + 0x40;
        rect.w = 0x48;
        rect.h = 9;
        if (((PlayerProfile *)PLAYER_PROFILES)->rewardResults[i] < 0) {
            rect.x = 0xC4;
            rect.w = 0x50;
        }
        openWindow(&KAW_PRIZE_SCREEN->rewards[i].window, &rect, -1, (s16 *)-1, 0, 0x36, 0x80, 12);
        KAW_PRIZE_SCREEN->rewards[i].index = i;
        KAW_PRIZE_SCREEN->rewards[i].window.palette = 2;
    }
    KAW_waitForCross();
    playSoundEffect(0xA4);
    animateWindowTo(&KAW_PRIZE_SCREEN->window, (Rect16 *)-1);
    for (i = 0; i < 3; i++) {
        animateWindowTo(&KAW_PRIZE_SCREEN->rewards[i].window, (Rect16 *)-1);
        animateWindowTo(&KAW_PRIZE_SCREEN->prizes[i].window, (Rect16 *)-1);
    }
    func_80014C08(30);
    removeFrameCallback((s32)KAW_renderPrizeScreen);
    func_80014C08(2);
    freeHeapBlock(KAW_PRIZE_SCREEN);
    func_80014C08(2);
}

void KAW_loadEffectArchive(void) {
    func_800149B8(0, -1, 0, 0x800, loadFileTagged, "B:\\CBTL_EFF.ARC", getCurrentTaskId(), 0x38E);
    *(s32 *)((u8 *)D_801D8340 + 0x4C) = func_80014C08(0x7FFFFFFF);
}

void KAW_freeEffectArchive(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x4C));
}

s32 KAW_playEffect(s32 entry, s32 player) {
    KAW_playEffectScript(entry, player, player, 0, 0);
}

void KAW_playCardEffect(s32 entry, s32 player, s32 mode) {
    KAW_playEffectScript(entry, player, player, mode, mode);
}

s32 KAW_playEffectOnOpponent(s32 entry, s32 player) {
    KAW_playEffectScript(entry, player, player ^ 1, 0, 0);
}

void KAW_playEffectScript(s32 entry, s32 player1, s32 player2, s32 mode1, s32 mode2) {
    s32 data;

    KAW_EFFECT_PLAYER = player1;
    switch (mode1) {
    case 0:
        KAW_EFFECT_CARD = getActiveDigimonCard(player1);
        break;
    case 1:
        KAW_EFFECT_CARD = getPlayedCard(player1);
        break;
    }
    switch (mode2) {
    case 0:
        KAW_EFFECT_TARGET_CARD = getActiveDigimonCard(player2);
        break;
    case 1:
        KAW_EFFECT_TARGET_CARD = getPlayedCard(player2);
        break;
    }
    data = decompressArchiveEntry(*(s32 *)((u8 *)D_801D8340 + 0x4C), entry);
    func_800149B8(0, 0x1F, 0, 0x800, KAW_runEffectScriptTask, data, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    freeHeapBlock((void *)data);
}

extern EffectObject KAW_EFFECT_ROOT;
extern void (*KAW_EFFECT_TICK_FUNCS[])(u8 *);
void KAW_runEffectScript(EffectTable *table);

s32 KAW_tickEffectScript(EffectTable *table) {
    s32 i;

    PushMatrix();
    tickEffectMotion((s32)&KAW_EFFECT_ROOT, 0);
    PopMatrix();
    table->regs[0] = 1;
    KAW_runEffectScript(table);
    for (i = 0; i < 16; i++) {
        if (table->entries[i].active != 0 && KAW_EFFECT_TICK_FUNCS[table->entries[i].kind] != NULL) {
            KAW_EFFECT_TICK_FUNCS[table->entries[i].kind](table->entries[i].obj);
            if (table->entries[i].kind > 0) {
                table->regs[i + 0x52] = *(s32 *)(table->entries[i].obj + 0x118);
                table->regs[i + 0x72] = *(s32 *)(table->entries[i].obj + 0x11C);
            }
        }
    }
    return table->regs[0];
}

void KAW_freeEffectEntries(EffectTable *table) {
    s32 i;

    func_80014C08(FRAME_INTERVAL);
    for (i = 0; i < 16; i++) {
        if (table->entries[i].kind != -1) {
            KAW_EFFECT_FREE_FUNCS[table->entries[i].kind](table->entries[i].obj);
        }
    }
}

void KAW_getCardPosition(s32 index, u8 *fx) {
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

void KAW_setCardSpriteColor(s32 index, u8 *fx) {
    u8 rgb[3];

    if (index >= 0) {
        rgb[0] = *(s32 *)(fx + 0x94);
        rgb[1] = *(s32 *)(fx + 0x98);
        rgb[2] = *(s32 *)(fx + 0x9C);
        KAW_fadeCardSprite(*(CardSprite **)(D_801D833C + index * 36), rgb);
    }
}

extern u8 D_800795A8;
void KAW_getEffectParams(EffectObject *o, u8 *fx, s32 current);
void KAW_setEffectParams(EffectObject *o, u8 *fx);
void KAW_getEffectWorldPos(void *xform, u8 *fx);
void KAW_createEffectEntry(s32 index, s32 kind, s32 arg2, EffectTable *table);
void KAW_showCardLabel(CardSprite *sprite, s32 num);
void KAW_hideCardLabel(CardSprite *sprite);

void KAW_runEffectScript(EffectTable *table) {
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
                    KAW_getEffectParams(&KAW_EFFECT_ROOT, (u8 *)vars, 0);
                    break;
                case 1:
                    KAW_setEffectParams(&KAW_EFFECT_ROOT, (u8 *)vars);
                    restartEffectMotion((u8 *)&KAW_EFFECT_ROOT);
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
                    KAW_getCardPosition(KAW_EFFECT_CARD, (u8 *)vars);
                    break;
                case 7:
                    KAW_getCardPosition(KAW_EFFECT_TARGET_CARD, (u8 *)vars);
                    break;
                case 8:
                    KAW_setCardSpriteColor(KAW_EFFECT_CARD, (u8 *)vars);
                    break;
                case 9:
                    KAW_setCardSpriteColor(KAW_EFFECT_TARGET_CARD, (u8 *)vars);
                    break;
                case 10:
                    index = KAW_EFFECT_CARD;
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 11:
                    index = KAW_EFFECT_TARGET_CARD;
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
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
                    KAW_getEffectParams((EffectObject *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, 0);
                    break;
                case 2:
                    KAW_initEffectFromParams((EffectTemplate *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, table);
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
                    KAW_getEffectWorldPos(table->entries[(s16)table->script->params[0]].obj, (u8 *)vars);
                    break;
                case 8:
                    KAW_getEffectParams((EffectObject *)table->entries[(s16)table->script->params[0]].obj, (u8 *)vars, 1);
                    break;
                case 9:
                    KAW_getCardPosition(getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 10:
                    KAW_getCardPosition(getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 11:
                    KAW_getCardPosition(peekOnlineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 12:
                    KAW_getCardPosition(peekOfflineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 13:
                    KAW_setCardSpriteColor(getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 14:
                    KAW_setCardSpriteColor(getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 15:
                    KAW_setCardSpriteColor(peekOnlineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 16:
                    KAW_setCardSpriteColor(peekOfflineDeckTop(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]), (u8 *)vars);
                    break;
                case 17:
                    index = getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 18:
                    index = getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_hideCardLabel(SPRITE(index));
                    }
                    break;
                case 19:
                    index = KAW_EFFECT_CARD;
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[0]);
                    }
                    break;
                case 20:
                    index = KAW_EFFECT_TARGET_CARD;
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[0]);
                    }
                    break;
                }
                break;
            case 12:
                switch (table->script->eventArg) {
                case 0:
                    KAW_createEffectEntry((s16)table->script->params[0], (s16)table->script->params[1], (s32)vars, table);
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
                    index = getActiveDigimonCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[1]);
                    }
                    break;
                case 5:
                    index = getPlayedCard(KAW_EFFECT_PLAYER ^ (s16)table->script->params[0]);
                    if (index >= 0) {
                        KAW_showCardLabel(SPRITE(index), (s16)table->script->params[1]);
                    }
                    break;
                }
                break;
            }
        }
        clearScriptBusy(table->script);
    } while (result != 0);
}

void KAW_getEffectParams(EffectObject *o, u8 *fx, s32 current) {
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

void KAW_setEffectParams(EffectObject *o, u8 *fx) {
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

void KAW_getEffectWorldPos(void *xform, u8 *fx) {
    SVECTOR pos;

    getTransformWorldPos(xform, &pos);
    *(s32 *)(fx + 0x248) = pos.vx;
    *(s32 *)(fx + 0x24C) = pos.vy;
    *(s32 *)(fx + 0x250) = pos.vz;
}

void KAW_initEffectFromParams(EffectTemplate *template, u8 *fx, EffectTable *table) {
    KAW_setEffectParams((EffectObject *)template, fx);
    if (*(s32 *)(fx + 0x2D0) == -2) {
        template->data[0x26] = 0;
    } else if (*(s32 *)(fx + 0x2D0) == -1) {
        template->data[0x26] = (s32)&KAW_EFFECT_ROOT;
    } else {
        template->data[0x26] = (s32)table->entries[*(s32 *)(fx + 0x2D0)].obj;
    }
}

void KAW_createFadeRectFromParams(u8 *fx) {
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

void KAW_createRingFromParams(u8 *fx, EffectTable *table) {
    Bytes4 inner;
    Bytes4 mid;
    Bytes4 outer;
    EffectTemplate template;

    KAW_initEffectFromParams(&template, fx, table);
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

void KAW_createEffectObjectFromParams(u8 *fx, EffectTable *table) {
    EffectTemplate template;

    KAW_initEffectFromParams(&template, fx, table);
    cloneEffectObject(&template);
}

void KAW_createStreaksFromParams(u8 *fx, EffectTable *table) {
    EffectTemplate template;
    u8 startColor[3];
    u8 endColor[3];

    startColor[0] = *(s32 *)(fx + 0x94);
    startColor[1] = *(s32 *)(fx + 0x98);
    startColor[2] = *(s32 *)(fx + 0x9C);
    endColor[0] = *(s32 *)(fx + 0xA0);
    endColor[1] = *(s32 *)(fx + 0xA4);
    endColor[2] = *(s32 *)(fx + 0xA8);
    KAW_initEffectFromParams(&template, fx, table);
    createStreakParticles(startColor, endColor, &template, *(s32 *)(fx + 0x2DC), *(s32 *)(fx + 0x2D8), *(s32 *)(fx + 0x2E0),
                          *(s32 *)(fx + 0x2F4), *(s32 *)(fx + 0x2E4), *(s32 *)(fx + 0x2E8), *(s32 *)(fx + 0x2EC), *(s32 *)(fx + 0x2F0),
                          *(s32 *)(fx + 0x13C), *(s32 *)(fx + 0x140), *(s32 *)(fx + 0x138), *(s32 *)(fx + 0x144), *(s16 *)(fx + 0x68),
                          *(s32 *)(fx + 0x114), *(s32 *)(fx + 0x124));
}

void KAW_freeFadeRect(void *ptr) {
    freeHeapBlock(ptr);
}

void KAW_createEffectEntry(s32 index, s32 kind, s32 arg2, EffectTable *table) {
    if (KAW_EFFECT_CREATE_FUNCS[kind] != NULL) {
        table->entries[index].kind = kind;
        table->entries[index].active = 0;
        table->entries[index].obj = KAW_EFFECT_CREATE_FUNCS[kind](arg2, table);
        if ((++table->count & 0xF) == 0) {
            func_80014C08(FRAME_INTERVAL);
        }
    }
}

EffectTable *KAW_createEffectScript(void *data) {
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
    KAW_EFFECT_ROOT = fx;
    initEffectObject(&KAW_EFFECT_ROOT);
    KAW_runEffectScript(table);
    return table;
}

void KAW_runEffectScriptTask(void *data, s32 task) {
    EffectTable *table;

    table = KAW_createEffectScript(data);
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (KAW_tickEffectScript(table));
    KAW_freeEffectEntries(table);
    freeScriptContext(table->script, table->regs);
    freeHeapBlock(table);
    func_80014A48(task);
}

void KAW_renderDuelMenu(void) {
    drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
}

void KAW_drawEffectHelp(UiWindow *w) {
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
        drawText(x, y + i * 12, (s32)KAW_EFFECT_HELP_LINES[i], 7, z);
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

/* per effect kind: what updates it each frame, creates it and frees it */
void (*KAW_EFFECT_TICK_FUNCS[4])(u8 *) = {
    /* SUGSEG's colour quad renderer, func_801E651C: in KAWSEG this address
       is inside KAW_chooseSupportCard, and nothing here relocates it */
    (void (*)(u8 *))0x801E651C,
    (void (*)(u8 *))renderRingEffect,
    (void (*)(u8 *))updateEffectObject,
    (void (*)(u8 *))renderStreakParticles,
};

u8 *(*KAW_EFFECT_CREATE_FUNCS[4])(s32, EffectTable *) = {
    (u8 *(*)(s32, EffectTable *))KAW_createFadeRectFromParams,
    (u8 *(*)(s32, EffectTable *))KAW_createRingFromParams,
    (u8 *(*)(s32, EffectTable *))KAW_createEffectObjectFromParams,
    (u8 *(*)(s32, EffectTable *))KAW_createStreaksFromParams,
};

void (*KAW_EFFECT_FREE_FUNCS[4])(u8 *) = {
    (void (*)(u8 *))KAW_freeFadeRect,
    (void (*)(u8 *))freeRingEffect,
    (void (*)(u8 *))freeEffectObject,
    (void (*)(u8 *))freeStreakParticles,
};

/* the lines of the special effect descriptions */
char *KAW_EFFECT_HELP_LINES[51] = {
    "          *c6*b2 Special Effect Descriptions *h-6*c7*h0",
    "",
    "*c5*d3\"Jamming Support\" *c7",
    " Opponent's Support Effect is Voided.",
    " Can't Void Option Effect.",
    "*c5*d2\"1st Attack\"*c7",
    " Attack first, regardless of Turn.",
    " If you're 1st, Void Foe's \"1st Attack.\"",
    "*c5*d2\"Eat-up HP\" *c7",
    " Recover the same amount of HP as",
    " the damage inflicted.",
    "*c5*d1\"Crash\"*c7",
    " Attack Power becomes same as HP.",
    " HP becomes 10.",
    " No Effect until just before Attack.",
    "",
    "*c5*d1\"Opponent *a0 X3\"*c7",
    " If Opponent's Specialty is *a0,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a1 X3\"*c7",
    " If Opponent's Specialty is *a1,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a2 X3\"*c7",
    " If Opponent's Specialty is *a2,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a3 X3\"*c7",
    " If Opponent's Specialty is *a3,",
    " own *b2 Attack is Tripled.",
    "*c5*d1\"Opponent *a4 X3\"*c7",
    " If Opponent's Specialty is *a4,",
    " own *b2 Attack is Tripled.",
    "",
    "*c5*d1\"*b0 Counter Attack\"*c7",
    " If Opponent uses *b0 Attack, it will miss.",
    " Then you Counter with Opponent's ",
    " *b0 Attack power.",
    "*c5*d1\"*b1 Counter Attack\"*c7",
    " If Opponent uses *b1 Attack, it will miss.",
    " Then you Counter with Opponent's ",
    " *b1 Attack power.",
    "*c5*d1\"*b2 Counter Attack\"*c7",
    " If Opponent uses *b2 Attack, it will miss.",
    " Then you Counter with Opponent's ",
    " *b2 Attack power.",
    "",
    "*c5*d1\"*b0 to 0\"*c7",
    " Lowers Opponent's *b0 Attack Power to 0.",
    "*c5*d1\"*b1 to 0\"*c7",
    " Lowers Opponent's *b1 Attack Power to 0.",
    "*c5*d1\"*b2 to 0\"*c7",
    " Lowers Opponent's *b2 Attack Power to 0.",
};

/* the options of the duel menu, KAW_DUEL_MENU */
char *KAW_DUEL_MENU_LABELS[4] = {
    "Sound",
    "Polygon Battle",
    "*b2 Special Effect Descriptions",
    "Give Up",
};

extern char *KAW_DUEL_MENU_LABELS[];
extern Menu KAW_DUEL_MENU;

void KAW_drawDuelMenu(UiWindow *window) {
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
        text = KAW_DUEL_MENU_LABELS[i];
        if (DUEL->tutorial && i == 3) {
            text = "Quit";
        }
        drawText(x, y + i * 14, (s32)text, 7, z);
    }
    updateMenuCursor(&KAW_DUEL_MENU);
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
extern CursorHighlight KAW_DUEL_MENU_CURSOR;
extern DialogK KAW_MENU_DIALOG;
extern UiWindow KAW_HELP_WINDOW;
extern const char KAW_STR_GIVE_UP[];
void func_80055730(void);

void KAW_tickDuelMenu(void) {
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
    KAW_DUEL_MENU.pad = player;
    openMenu(&KAW_DUEL_MENU, &KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU_CURSOR, (Bytes4 *)-1);
    KAW_DUEL_MENU_WINDOW.label = (s32)"MENU";
    for (;;) {
        func_80014C08(FRAME_INTERVAL);
        drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
        if (PAD_STATES[((DuelK *)D_801D8340)->menuPlayer]->pressed & 0x40) {
            playSoundEffect(0xA0);
            switch (KAW_DUEL_MENU.row) {
            case 0:
                KAW_MENU_DIALOG.yes = "Stereo";
                KAW_MENU_DIALOG.no = "Mono";
                initDialog((u8 *)&KAW_MENU_DIALOG, "Sound Settings", 2);
                KAW_MENU_DIALOG.pad = ((DuelK *)D_801D8340)->menuPlayer;
                KAW_MENU_DIALOG.draw = KAW_renderDuelMenu;
                KAW_MENU_DIALOG.result = ((PlayerProfile *)PLAYER_PROFILES)->unk20_0 + 1;
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                runDialog(&KAW_MENU_DIALOG);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                switch (KAW_MENU_DIALOG.result) {
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
                KAW_MENU_DIALOG.yes = "On";
                KAW_MENU_DIALOG.no = "Off";
                initDialog((u8 *)&KAW_MENU_DIALOG, "Polygon Battle", 2);
                KAW_MENU_DIALOG.pad = ((DuelK *)D_801D8340)->menuPlayer;
                KAW_MENU_DIALOG.draw = KAW_renderDuelMenu;
                KAW_MENU_DIALOG.result = ((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation + 1;
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                runDialog(&KAW_MENU_DIALOG);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                switch (KAW_MENU_DIALOG.result) {
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
                openWindow(&KAW_HELP_WINDOW, &rect, -1, (s16 *)&view, 10, 0x16, 0x80, 12);
                KAW_HELP_WINDOW.label = (s32)"HELP";
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                do {
                    func_80014C08(FRAME_INTERVAL);
                    drawWindow(&KAW_HELP_WINDOW, KAW_drawEffectHelp, 0);
                    drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
                } while (!(PAD_STATES[((DuelK *)D_801D8340)->menuPlayer]->pressed & 0x10));
                playSoundEffect(0xA4);
                animateWindowTo(&KAW_HELP_WINDOW, (Rect16 *)-1);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                for (i = 0; i < 16; i++) {
                    func_80014C08(FRAME_INTERVAL);
                    drawWindow(&KAW_HELP_WINDOW, KAW_drawEffectHelp, 0);
                    drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
                }
                break;
            case 3:
                if (((DuelK *)D_801D8340)->tutorial) {
                    initDialog((u8 *)&KAW_MENU_DIALOG, "Quit Tutorial?", 1);
                } else {
                    initDialog((u8 *)&KAW_MENU_DIALOG, KAW_STR_GIVE_UP, 1);
                }
                KAW_MENU_DIALOG.pad = ((DuelK *)D_801D8340)->menuPlayer;
                KAW_MENU_DIALOG.draw = KAW_renderDuelMenu;
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
                runDialog(&KAW_MENU_DIALOG);
                animateWindowTo(&KAW_DUEL_MENU_WINDOW, &KAW_DUEL_MENU.rect);
                switch (KAW_MENU_DIALOG.result) {
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
            animateWindowTo(&KAW_DUEL_MENU_WINDOW, (Rect16 *)-1);
            for (i = 0; i < 16; i++) {
                func_80014C08(FRAME_INTERVAL);
                drawWindow(&KAW_DUEL_MENU_WINDOW, KAW_drawDuelMenu, 0);
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

extern HudPanelInit KAW_HUD_PANEL_INITS[];
s32 KAW_initHudPanels(void) {
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
        ((HudPanelK *)D_801D83EC)[i].unk8[0] = KAW_HUD_PANEL_INITS[i].unk0[0];
        ((HudPanelK *)D_801D83EC)[i].unk8[1] = KAW_HUD_PANEL_INITS[i].unk0[1];
        ((HudPanelK *)D_801D83EC)[i].unk8[2] = KAW_HUD_PANEL_INITS[i].unk0[2];
        ((HudPanelK *)D_801D83EC)[i].unk8[3] = KAW_HUD_PANEL_INITS[i].unk0[3];
        ((HudPanelK *)D_801D83EC)[i].unk6 = KAW_HUD_PANEL_INITS[i].unk4;
        ((HudPanelK *)D_801D83EC)[i].unk4 = KAW_HUD_PANEL_INITS[i].unk6;
        ((HudPanelK *)D_801D83EC)[i].flags = 0;
        ((HudPanelK *)D_801D83EC)[i].state = 0;
        if (KAW_HUD_PANEL_INITS[i].parent != -1) {
            ((HudPanelK *)D_801D83EC)[i].parent = &((HudPanelK *)D_801D83EC)[KAW_HUD_PANEL_INITS[i].parent];
        } else {
            ((HudPanelK *)D_801D83EC)[i].parent = NULL;
        }
        ((HudPanelK *)D_801D83EC)[i].z = KAW_HUD_PANEL_INITS[i].z;
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

s32 KAW_freeHudPanels(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x48));
    freeHeapBlock(D_801D83EC);
}

extern const Bytes4 KAW_PORTRAIT_COLOR;
void KAW_drawPortraitColored(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb);

void KAW_drawPortrait(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, u16 a5) {
    Bytes4 color;

    color = KAW_PORTRAIT_COLOR;
    KAW_drawPortraitColored(a0, a1, a2, a3, a4, a5, &color);
}

void KAW_drawPortraitColored(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb) {
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

s32 KAW_allocCardPolys(void) {
    s32 i;

    D_801D83F8 = allocTaskHeapBlock(sizeof(Unk14F0) * 2);
    for (i = 0; i < 2; i++) {
        ((Graphics *)&GRAPHICS)->buffers[i].primSlots[10] = (s32)&D_801D83F8[i];
    }
    allocTaskHeapBlock(0xE10);
}

s32 KAW_freeCardPolys(void) {
    freeHeapBlock(D_801D83F8);
}

void KAW_showCardLabel(CardSprite *sprite, s32 num) {
    sprite->flags |= 0x20;
    sprite->num = num;
}

void KAW_hideCardLabel(CardSprite *sprite) {
    sprite->flags &= ~0x20;
}

void KAW_fadeCardSprite(CardSprite *sprite, u8 *to) {
    sprite->flags |= 0x40;
    sprite->t = 0;
    sprite->from[0] = sprite->fade[0];
    sprite->from[1] = sprite->fade[1];
    sprite->from[2] = sprite->fade[2];
    sprite->to[0] = to[0];
    sprite->to[1] = to[1];
    sprite->to[2] = to[2];
}

void KAW_initCursorShape(Shape *shape, s32 x, s32 y, s32 d);

s32 KAW_createCursor(s32 arg0, s32 x, s32 y, s32 d, s32 count) {
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
        KAW_initCursorShape(&shapes[i], x, y, d);
    }
    return (s32)shapes;
}

void KAW_initCursorShape(Shape *shape, s32 x, s32 y, s32 d) {
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

void KAW_freeCursor(void *ptr) {
    if (ptr != NULL) {
        freeHeapBlock(D_801D83F4);
        freeHeapBlock(ptr);
    }
}

void KAW_drawCursorAt(arg0, x, y)
    s16 *arg0;
    s16 x;
    s16 y;
{
    arg0[4] = x;
    arg0[5] = y;
    KAW_drawCursor(arg0);
}

void KAW_drawCursor(void *arg0) {
    KAW_renderCursor(arg0, 1);
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

void KAW_renderCursor(void *arg0, s32 otz) {
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

void KAW_initRing(void) {
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

void KAW_freeRing(void) {
    freeHeapBlock(*(void **)((u8 *)D_801D8340 + 0x4));
}

void KAW_closeRing(s32 arg0, s32 arg1, s32 arg2) {
    u8 *duel = D_801D8340;

    *(s32 *)(duel + 0x828) = 1;
    *(s32 *)(duel + 0x82C) = arg0;
    *(s32 *)(duel + 0x830) = arg1;
    *(s32 *)(duel + 0x838) = arg2;
}

void KAW_openRing(void) {
    *(s32 *)((u8 *)D_801D8340 + 0x828) = 0;
}

s32 KAW_renderRing(void) {
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

#define FLAGS110(p) ((Flags110 *)&PLAYER(p)->bonusFlags)

void KAW_resetBonusFlags(s32 player) {
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

void KAW_countEarnedBonuses(void) {
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

/* the last three bytes are leftovers in the original, not zero padding */
const char KAW_STR_GIVE_UP[12] = "Give Up?\0\xD0\x12\x2B";

typedef struct {
    /* 0x0 */ u8 id;
    /* 0x1 */ u8 bonus;
    /* 0x4 */ char *name;
} BonusEntry;

const Bytes4 KAW_PORTRAIT_COLOR = { { 0x80, 0x80, 0x80, 0 } };

Menu KAW_DUEL_MENU = { NULL, NULL, { 20, 40, 180, 56 }, 0, -1, 0, -1, 8, 0x16, 180, 12, 1, 4, 0, 0, 0, 14, 0, 0, 0 };

HudPanelInit KAW_HUD_PANEL_INITS[12] = {
    { { 0, 0, 0xFF, 0x47 }, 0x1C, 0x7E30, 0, -1, 0xA },
    { { 0, 0, 0xD0, 0x40 }, 0x1E, 0x7C31, 0, -1, 0xA },
    { { 0, 0x7E, 0x74, 0x3C }, 0x1D, 0x7EF4, 0, -1, 0x64 },
    { { 0xA0, 0x47, 0x30, 0x14 }, 0x1C, 0x7E70, 0, 2, 0x64 },
    { { 0, 0x3F, 0xFF, 0x3F }, 0x1D, 0x7E74, 0, -1, 0x64 },
    { { 0xA8, 0x4E, 0x28, 0xE }, 0x1E, 0x7DB3, 0, 4, 0x64 },
    { { 0, 0, 0xFF, 0x47 }, 0x1C, 0x7E30, 0, -1, 0xA },
    { { 0, 0, 0xD0, 0x40 }, 0x1E, 0x7E31, 0, -1, 0xA },
    { { 0x74, 0x7E, 0x74, 0x3C }, 0x1D, 0x7EB4, 0, -1, 0x64 },
    { { 0xA0, 0x5B, 0x30, 0x14 }, 0x1C, 0x7EB0, 0, 8, 0x64 },
    { { 0, 0, 0xFF, 0x3F }, 0x1D, 0x7E34, 0, -1, 0x64 },
    { { 0xA8, 0x40, 0x28, 0xE }, 0x1E, 0x7D73, 0, 10, 0x64 },
};

/* the bonuses a duel can give, with their experience */
BonusEntry KAW_BONUSES[32] = {
    { 0, 3, "All *b0 Attack Win" },
    { 1, 3, "All *b1 Attack Win" },
    { 2, 3, "All *b2 Attack Win" },
    { 3, 5, "All or Nothing Gamble Win" },
    { 4, 2, "Last Chance Gamble Win" },
    { 5, 5, "No Support Card Win" },
    { 6, 3, "No Digivolve Win" },
    { 7, 1, "No Discard Win" },
    { 8, 5, "4-of-a-Kind Win" },
    { 9, 2, "0 Online Card Left win" },
    { 10, 1, "Partner Win" },
    { 11, 3, "No Loss Win" },
    { 12, 3, "Come-Back Win" },
    { 13, 7, "Desperate Win" },
    { 14, 2, "All Gone Win" },
    { 15, 3, "Ultimate Level Win" },
    { 16, 5, "Option Maniac Win" },
    { 17, 8, "8 DP Cards Win" },
    { 18, 1, "Lucky Seven Win" },
    { 19, 3, "Just Enough Attack Win" },
    { 20, 10, "12 S-Jewel Cards Win" },
    { 21, 2, "Choked Loss" },
    { 22, 2, "Loss by Gamble" },
    { 23, 1, "Total Loss" },
    { 24, 2, "Rainbow" },
    { 25, 10, "Damage Fever" },
    { 26, 7, "HP Fever" },
    { 27, 2, "3 Partners" },
    { 28, 2, "3 Partners Plus" },
    { 29, 2, "Partner Normal Digivolve" },
    { 30, 1, "Lucky Name" },
    { 31, 10, "Super Bonus" },
};

/* the u8 arrays among these are not referenced by any code */
u8 D_801FC400[4] = { 0 };
s32 KAW_SUPPORT_REGISTER = 0;
u8 D_801FC408[8] = { 0 };
UiWindow KAW_TUTORIAL_WINDOW = { 0 };
DeckScreen *KAW_MATCH_SCREEN = NULL;
s32 KAW_MATCH_LOADING = 0;
u8 D_801FC45C[8] = { 0 };
POLY_G4 KAW_DECK_CHART_POLYS[2][2][3] = { { { { 0 } } } };
TILE KAW_DECK_LEVEL_BARS[2][2][4] = { { { { 0 } } } };
DR_MODE KAW_DECK_CHART_MODES[2][2] = { { { 0 } } };
s32 KAW_RESULT_SCREEN_STATE = 0;
ExpScreen *KAW_EXP_SCREEN = NULL;
PrizeScreen *KAW_PRIZE_SCREEN = NULL;
EffectObject KAW_EFFECT_ROOT = { { { { 0 } } } };
s8 KAW_EFFECT_PLAYER = 0;
s8 KAW_EFFECT_CARD = 0;
s8 KAW_EFFECT_TARGET_CARD = 0;
u8 D_801FC880[4] = { 0 };
UiWindow KAW_DUEL_MENU_WINDOW = { 0 };
u8 D_801FC8C8[12] = { 0 };
CursorHighlight KAW_DUEL_MENU_CURSOR = { { { 0 } } };
DialogK KAW_MENU_DIALOG = { { 0 } };
u8 D_801FC9CC[0x18] = { 0 };
UiWindow KAW_HELP_WINDOW = { 0 };
s32 KAW_BONUS_ROW = 0;
s32 KAW_BONUS_EXP = 0;

void KAW_trackSpecialties(s32 player) {
    Player *p;
    u32 flags;

    if (getActiveDigimonCard(player) == -1) {
        return;
    }
    p = (Player *)DUEL_PLAYERS[player];
    if (((u32)p->bonusFlags >> 15) & 1) {
        return;
    }
    switch (p->specialty) {
    case 0:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x800000;
        break;
    case 1:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x1000000;
        break;
    case 2:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x2000000;
        break;
    case 3:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x4000000;
        break;
    case 4:
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x8000000;
        break;
    }
    flags = ((Player *)DUEL_PLAYERS[player])->bonusFlags;
    if (((flags >> 23) & 1) + ((flags >> 24) & 1) + ((flags >> 25) & 1) + ((flags >> 26) & 1) + ((flags >> 27) & 1) == 5) {
        KAW_showBonusBanner(player, 0x18);
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x8000;
    }
}

s32 KAW_checkDigimonBonuses(s32 player) {
    s32 card;
    s16 hp;

    card = getActiveDigimonCard(player);
    if (card == -1) {
        return;
    }
    hp = PLAYER(player)->stats[0];
    if (hp != 0 && hp % 1110 == 0) {
        KAW_showBonusBanner(player, 0x1A);
        PLAYER(0)->bonusFlags |= 0x400;
    }
    if (findPartnerSlot(player, PLAYER(player)->cards[card % 30].id) >= 0) {
        KAW_showBonusBanner(player, 0x1C);
        ((Flags110 *)&PLAYER(player)->bonusFlags)->count++;
    } else if (findArmorPartnerSlot(player, PLAYER(player)->cards[card % 30].id) >= 0) {
        KAW_showBonusBanner(player, 0x1C);
        ((Flags110 *)&PLAYER(player)->bonusFlags)->count++;
    }
    if (!(((u32)PLAYER(player)->bonusFlags >> 30) & 1) && PLAYER(player)->digimonStack[0] >= 0 &&
        findPartnerSlot(player, PLAYER(player)->cards[PLAYER(player)->digimonStack[2] % 30].id) >= 0) {
        KAW_showBonusBanner(player, 0x1D);
        PLAYER(player)->bonusFlags |= 0x20000000;
    }
    KAW_trackSpecialties(player);
}

s32 KAW_checkHandBonuses(s32 player) {
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
        KAW_showBonusBanner(player, 8);
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x40;
    }
    if (partners == 3) {
        KAW_showBonusBanner(player, 0x1B);
        ((Player *)DUEL_PLAYERS[player])->bonusFlags |= 0x800;
    }
}


s32 KAW_addBonusLine(BonusEntry *entry, s32 x, s32 y, s32 last, s32 z) {
    char buf[72];

    ((DuelK *)D_801D8340)->bonusFlags[entry->id] = 1;
    drawText(x + 6, y + KAW_BONUS_ROW * 13, (s32)entry->name, 7, z);
    sprintf(buf, "*s0+%3d*c7(%3d)", entry->bonus, ((ProfileK *)PLAYER_PROFILES)->counts[entry->id] + 1);
    drawText(x + 0xA2, y + KAW_BONUS_ROW * 13, (s32)buf, 5, z);
    KAW_BONUS_ROW++;
    KAW_BONUS_EXP += entry->bonus;
    return KAW_BONUS_ROW == last;
}

s32 KAW_drawBonuses(s32 x, s32 y, s32 count, s32 z, s32 exp) {
    BonusEntry *entry;
    s32 i;
    char buf[72];

    entry = KAW_BONUSES;
    if (count == 0) {
        return 0;
    }
    KAW_BONUS_EXP = exp;
    KAW_BONUS_ROW = 0;
    if (((u8 *)D_801D8340)[0x81E] == 0) {
        if (!(FLAGS110(0)->f1 | FLAGS110(0)->f2) && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!(FLAGS110(0)->f0 | FLAGS110(0)->f2) && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!(FLAGS110(0)->f0 | FLAGS110(0)->f1) && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f16 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (((DuelK *)D_801D8340)->bonusFlags[3] == 0 && FLAGS110(0)->f17 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f4 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f3 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (!FLAGS110(0)->f5 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f6 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countOnlineDeckCards(0) > 0 && countOnlineDeckCards(1) == 0 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f14 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(1)->wins == 0 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f7 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(0)->wins == 3 && PLAYER(1)->wins == 2
            && countOnlineDeckCards(0) + countOnlineDeckCards(1) - countEmptyHandSlots(0) + 8 == countEmptyHandSlots(1)
            && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (((DuelK *)D_801D8340)->bonusFlags[13] == 0 && countOnlineDeckCards(0) + 4 == countEmptyHandSlots(0)
            && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f19 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countDeckCardsByFilter(0, (PlayerDeck *)PLAYER(0), 0xC0) >= 25 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countEmptyDpSlots(0) == 0 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (countOnlineDeckCards(0) == 7 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f22 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f28 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry += 4;
    } else {
        entry += 21;
        if (FLAGS110(0)->f8 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (FLAGS110(0)->f18 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
        if (PLAYER(0)->wins == 0 && PLAYER(1)->wins == 3 && KAW_addBonusLine(entry, x, y, count, z)) {
            return 0;
        }
        entry++;
    }
    if (FLAGS110(0)->f15 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f9 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f10 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f11 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->count == 3 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry++;
    if (FLAGS110(0)->f29 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    entry += 2;
    if (KAW_BONUS_ROW >= 7 && KAW_addBonusLine(entry, x, y, count, z)) {
        return 0;
    }
    i = 46;
    do {
        drawText(x + i * 5, y + KAW_BONUS_ROW * 13, (s32)"-", 7, z);
    } while (--i >= 0);
    sprintf(buf, "+%3d", KAW_BONUS_EXP);
    drawText(x + 0xA2, y + KAW_BONUS_ROW * 13 + 11, (s32)buf, 5, z);
    return 1;
}

typedef struct {
    u8 unk0[8];
    PolyF4 banner[2];
    DR_MODE bannerMode[2];
} DuelBanner;
#define BANNER ((DuelBanner *)D_801D8340)

void KAW_showBonusBanner(s32 player, s32 id) {
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
        drawText(x - measureText(KAW_BONUSES[id].name) / 2, y - 6, (s32)KAW_BONUSES[id].name, 7, 0);
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
