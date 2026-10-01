#ifndef DCB_KAWSEG_H
#define DCB_KAWSEG_H

#include "game.h"
#include "dcb/script.h"
#include "dcb/duel_launch.h"
#include "dcb/battle_hud.h"
#include "dcb/menu.h"

/* the duel state (DUEL) as KAWSEG sees it */
#define KAW_DUEL ((DuelK *)DUEL_STATE)

#define setRGB1(p, _r1, _g1, _b1) (p)->r1 = _r1, (p)->g1 = _g1, (p)->b1 = _b1
#define setRGB2(p, _r2, _g2, _b2) (p)->r2 = _r2, (p)->g2 = _g2, (p)->b2 = _b2
#define setRGB3(p, _r3, _g3, _b3) (p)->r3 = _r3, (p)->g3 = _g3, (p)->b3 = _b3
#define DUEL_AI ((DuelAi *)DUEL_STATE)
#define CARD_SPR(c) (((CardAnim *)(CARD_ANIMS + (c) * CARD_ANIM_SIZE))->spr)

#define setXYWH(p, _x0, _y0, _w, _h)                                                            \
    (p)->x0 = (_x0), (p)->y0 = (_y0), (p)->x1 = (_x0) + (_w), (p)->y1 = (_y0), (p)->x2 = (_x0), \
    (p)->y2 = (_y0) + (_h), (p)->x3 = (_x0) + (_w), (p)->y3 = (_y0) + (_h)

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
    /* 0x088 */ UiWindow partsWindow; /* "DIGI-PARTS RECEIVED" */
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

/* what the CPU can do with one hand card: kind 1 digivolves into it now,
   2 once it has more DP (need), 0 nothing */
typedef struct {
    /* 0x0 */ s8 kind;
    /* 0x1 */ u8 need;
    /* 0x2 */ s16 card;
    /* 0x4 */ s16 option; /* the Digivolve card that makes it possible */
} DigivolvePlan;

#if VERSION_JP
/* jp's duel state: only the CPU's digivolve plans are known, after the
   battle simulations (DuelAi) */
typedef struct {
    /* 0x000 */ u8 unk0[0x3E8];
    /* 0x3E8 */ DigivolvePlan *selected;
    /* 0x3EC */ DigivolvePlan slots[4];
    /* 0x404 */ u8 unk404[0x3D];
    /* 0x441 */ s8 cursorMode; /* which cards the card cursor offers, -1: none */
    /* 0x442 */ u8 winner;
    /* 0x443 */ s8 tutorial;
} DuelK;
#elif VERSION_US || VERSION_EU
typedef struct {
    /* 0x000 */ struct ScriptRunner *tutorialScript;
    /* 0x004 */ struct RingPrims *ringPrims;
    /* 0x008 */ u8 unk8[0x40];
    /* 0x048 */ struct HudPrims *hudPrims;
    /* 0x04C */ s32 effectArchive; /* CBTL_EFF.ARC */
    /* 0x050 */ u8 unk50[0x774];
    /* 0x7C4 */ DigivolvePlan *selected;
    /* 0x7C8 */ DigivolvePlan slots[4];
    /* 0x7E0 */ u8 unk7E0[0x3D];
    /* 0x81D */ s8 cursorMode; /* which cards the card cursor offers, -1: none */
    /* 0x81E */ u8 winner;
    /* 0x81F */ s8 tutorial;
    /* 0x820 */ s8 tutorialBusy; /* set while the tutorial script runs */
    /* 0x821 */ s8 menuPlayer;
    /* 0x822 */ s8 awaitingInput;
    /* 0x823 */ s8 menuOpen;
    /* 0x824 */ s8 quit; /* 2 + the winner when the duel ends early (tutorial, Give Up) */
    /* 0x825 */ u8 unk825[0x1B];
    /* 0x840 */ u8 bonusFlags[32];
    /* 0x860 */ s16 rewardCluts[3];
    /* 0x866 */ s16 partnerCluts[3];
} DuelK;
#endif

#if VERSION_JP
/* jp's tutorial block (SessionData.unk8): its message window, the decks
   both players duel with and the text */
typedef struct {
    /* 0x000 */ UiWindow window;
    /* 0x050 */ PlayerDeck decks[2];
    /* 0x268 */ char text[0x101];
    /* 0x369 */ u8 unk369[3];
} TutorialK;
#define KAW_TUTORIAL ((TutorialK *)((SessionData *)SESSION_DATA)->unk8)

/* jp's card prize (SessionData.unkC): after a duel against the CPU the
   player takes one of three of its cards (kaw_prize_jp's KAW_runCardPrize) */
typedef struct {
    /* 0x00 */ s32 cards[3]; /* the cards' sprites: 30 + their deck slots */
    /* 0x0C */ Rect16 windows[3];
    /* 0x24 */ s32 brightness[3];
    /* 0x30 */ Rect16 message;
    /* 0x38 */ s32 state; /* 0 choosing, 2 asked to leave, 3 eight owned, 5 asked to take one; 1 taken, 4 left */
    /* 0x3C */ s32 choice; /* -1 once left */
} CardPrizeK;
#define KAW_CARD_PRIZE ((CardPrizeK *)((SessionData *)SESSION_DATA)->unkC)
#endif

typedef struct ScriptRunner {
     void *data;
     Script *script;
     s32 *regs;
     s32 delay; /* frames to wait before running the script again */
     s32 text; /* the message being shown */
} ScriptRunner;

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
    /* 0x504 */ s32 deckListOpen[2];
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
    /* 0x770 */ s16 mode; /* the mode the screen was opened with; 0 also draws the second player's deck lists */
    /* 0x772 */ s16 deckId;
    /* 0x774 */ s16 chosen;
    /* 0x776 */ s16 timer;
} DeckScreen;

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

typedef struct RingPrims {
    /* 0x000 */ DR_MODE dm;
    /* 0x008 */ PolyF4 edges[32];
    /* 0x308 */ POLY_G4 fades[32];
} RingPrims;

#if VERSION_JP
/* jp keeps a profile for each of the two players (PLAYER_PROFILES points to
   both), in another layout: only the fields its matched code reads are placed */
typedef struct {
    /* 0x0000 */ u8 unk0[0x18];
    /* 0x0018 */ u16 battleWins;
    /* 0x001A */ u16 battleLosses;
    /* 0x001C */ u16 versusWins;
    /* 0x001E */ u16 versusLosses;
    /* 0x0020 */ u8 unk20[0x10];
    /* 0x0030 */ u16 bestDamage[0x6E][3]; /* per Digimon card: jp has 0x6E */
    /* 0x02C4 */ u16 cardWins[0x6E];
    /* 0x03A0 */ u16 cardLosses[0x6E];
    /* 0x047C */ u8 unk47C[0x145C - 0x47C];
} ProfileK;
#elif VERSION_US || VERSION_EU
typedef struct {
    /* 0x0000 */ u8 unk0[0x848];
    /* 0x0848 */ u16 counts[32];
    /* 0x0888 */ u8 unk888[0xD3C - 0x888];
    /* 0x0D3C */ u16 bestDamage[0xBF][3];
    /* 0x11B6 */ u8 unk11B6[0x2774 - 0x11B6];
} ProfileK;
#endif

typedef struct {
#if VERSION_JP
    s16 own;
    s16 opponent;
#elif VERSION_US || VERSION_EU
    s32 own;
    s32 opponent;
#endif
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
#if VERSION_JP
    u8 unk0[4];
#elif VERSION_US || VERSION_EU
    u8 unk0[0x5C];
#endif
    AttackSim sims[3];
} DuelAi;

typedef struct {
    /* 0x00 */ u8 unk0[0x98];
    /* 0x98 */ char *yes;
    /* 0x9C */ char *no;
    /* 0xA0 */ void (*draw)(void);
    /* 0xA4 */ u8 unkA4;
    /* 0xA5 */ s8 result;
    /* 0xA6 */ u8 pad;
} DialogK;

typedef struct HudPrims {
    u8 data[0x5F0];
} HudPrims;

extern s32 KAW_BONUS_EXP;
extern DeckScreen *KAW_MATCH_SCREEN;

s32 rand(void);

#endif /* DCB_KAWSEG_H */
