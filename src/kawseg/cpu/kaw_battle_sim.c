#include "common.h"
#include "game.h"
#include "dcb/kaw_battle_sim.h"
#include "dcb/card_db.h"
#include "dcb/card_zones.h"
#include "dcb/battle_hud.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_effect.h"
#include "dcb/kaw_bonus.h"
#include "dcb/duel_util.h"
#include "dcb/sound_play.h"

#define FLAGS178(p) (&STATS(p)->flags)
#define STATS(p) ((PlayerStats *)PLAYER(p))

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

#if VERSION_JP
/* jp's Player keeps these fields from 0x0C on */
typedef struct {
    /* 0x00 */ u8 unk0[0xC];
    /* 0x0C */ s16 stats[5];
    /* 0x16 */ u8 unk16[0xA];
    /* 0x20 */ s16 hpBeforeBattle;
    /* 0x22 */ s16 hpAfterBattle;
    /* 0x24 */ s16 baseAttackPowers[3];
    /* 0x2A */ s16 damageTaken;
    /* 0x2C */ s16 hpGain;
    /* 0x2E */ u8 unk2E[2];
    /* 0x30 */ s16 attackDamage[3];
    /* 0x36 */ u8 unk36[2];
    /* 0x38 */ Flags178 flags;
} PlayerStats;
#elif VERSION_US || VERSION_EU
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
#else
#error "untested version"
#endif

typedef struct {
    u8 *card;
    s8 order;
} BattleEffect;

extern s32 KAW_SUPPORT_REGISTER;

s32 KAW_runSupportEffect(s32 self, s32 other, SupportCond *conds, SupportEffect *effects, s32 quiet);
void KAW_applyCrossEffect(s32 self, s32 other, DigimonCardData *cardData, s32 quiet);
void KAW_recordBestDamage(Player *p);

/* the controller of the CPU, which keeps no profile */
#if VERSION_JP
#define CPU_CONTROLLER 2
#elif VERSION_US || VERSION_EU
#define CPU_CONTROLLER 1
#else
#error "untested version"
#endif

#if VERSION_JP
void KAW_recordBestDamage(Player *p) {
    s32 player;
    s32 attack;
    s32 card;
    s32 index;

    player = p->controller;
    attack = p->usedAttack;
    if (DUEL->tutorial == 0 && player != CPU_CONTROLLER) {
        card = getActiveDigimonCard(player);
        index = PLAYER_CARDS(p)[card % 30].index;
        if (((PlayerStats *)p)->attackDamage[attack] > ((ProfileK *)PLAYER_PROFILES)[player].bestDamage[index][attack]) {
            ((ProfileK *)PLAYER_PROFILES)[player].bestDamage[index][attack] = ((PlayerStats *)p)->attackDamage[attack];
        }
    }
}
#elif VERSION_US || VERSION_EU
#else
#error "kawseg/cpu/kaw_battle_sim: version not checked"
#endif

#if VERSION_JP
/* jp tells the battle in its battle log (addBattleLogLine) */
/* the card the cursor points at, lit while the battle log talks about it */
#define LOG_CARD (((CardCursor *)DUEL->cursor)->id)

/* lights CARD's sprite up (0xFF) or back to normal (0x80) */
#define SET_CARD_LIGHT(card, level)          \
    CARD_SPR(card)->fade[0] = (level);       \
    CARD_SPR(card)->fade[1] = (level);       \
    CARD_SPR(card)->fade[2] = (level)

s32 KAW_resolveBattle(s32 quiet) {
    BattleEffect effects[6];
    char text[64];
    s32 i;
    s32 k;
    s32 player;
    s32 card;
    s32 result;
    s32 shown;
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
        card = getPlayedCard(player);
        if (card != -1) {
            switch (PLAYER_CARDS(PLAYER(player))[card % 30].card[2]) {
            case 0:
                effects[i + 2].card = (u8 *)PLAYER_CARDS(PLAYER(player))[card % 30].card;
                effects[i + 2].order = ((DigimonCardData *)effects[i + 2].card)->supportIcon;
                break;
            case 1:
                effects[i].card = (u8 *)PLAYER_CARDS(PLAYER(player))[card % 30].card;
                effects[i].order = ((OptionCardData *)effects[i].card)->supportIcon;
                break;
            case 2:
                effects[i].order = -1;
                effects[i + 2].order = -1;
                if (!quiet) {
                    LOG_CARD = getPlayedCard(player);
                    DUEL->cursorSlot = 4;
                    SET_CARD_LIGHT(getPlayedCard(player), 0xFF);
                    clearBattleLog();
                    addBattleLogLine(player, "オプションスロットは");
                    addBattleLogLine(player, "進化カードなのでc2発動しません！");
                    if (SPRITE_KIND(getPlayedCard(player)) == 0x17) {
                        SPRITE_KIND(getPlayedCard(player)) = 0xE;
                        waitFrames(0x10);
                    }
                    playSoundEffect(0x6A);
                    waitForCpuDecision();
                    LOG_CARD = -1;
                    DUEL->cursorSlot = -1;
                    SET_CARD_LIGHT(getPlayedCard(player), 0x80);
                }
                break;
            }
        }
    }
    for (i = 0; i < 2; i++) {
        player = DUEL->turnPlayer ^ i;
        card = getActiveDigimonCard(player);
        if (card != -1) {
            effects[i + 4].card = (u8 *)PLAYER_CARDS(PLAYER(player))[card % 30].card;
            effects[i + 4].order = CROSS_EFFECT_ICONS[((DigimonCardData *)effects[i + 4].card)->crossEffect];
        }
    }
    for (i = 3; i > 0; i--) {
        for (k = 0; k < 6; k++) {
            player = DUEL->turnPlayer ^ (k % 2);
            if (!quiet && k < 4 && effects[k].order == i && getPlayedCard(player) != -1) {
                if (SPRITE_KIND(getPlayedCard(player)) == 0x17) {
                    SPRITE_KIND(getPlayedCard(player)) = 0xE;
                    waitFrames(0x10);
                }
                LOG_CARD = getPlayedCard(player);
                DUEL->cursorSlot = 4;
                SET_CARD_LIGHT(LOG_CARD, 0xFF);
            }
            if (effects[k].order != i) {
                continue;
            }
            shown = 0;
            clearBattleLog();
            switch (k) {
            case 0:
            case 1:
                if (!(STATS(player)->flags.f9 & 2)) {
                    result = KAW_runSupportEffect(player, player ^ 1, (SupportCond *)(effects[k].card + 0x16),
                                                  (SupportEffect *)(effects[k].card + 0x56), quiet);
                    if (!quiet) {
                        shown = 1;
                        if (result) {
                            sprintf(text, "c6%sカードc7はc2不発！", effects[k].card + 3);
                            addBattleLogLine(player, text);
                            playSoundEffect(0x6A);
                        } else {
                            playSoundEffect(0x67);
                        }
                    }
                } else if (!quiet) {
                    shown = 1;
                    addBattleLogLine(player, "「ぼうがい」により");
                    addBattleLogLine(player, "オプション攻撃はc2無効！");
                    playSoundEffect(0x6A);
                }
                break;
            case 2:
            case 3:
                if (!(STATS(player)->flags.f9 & 1)) {
                    result = KAW_runSupportEffect(player, player ^ 1, (SupportCond *)(effects[k].card + 0x62),
                                                  (SupportEffect *)(effects[k].card + 0xA2), quiet);
                    if (!quiet) {
                        shown = 1;
                        if (result) {
                            sprintf(text, "c6%sカードc7はc2不発！", effects[k].card + 3);
                            addBattleLogLine(player, text);
                            playSoundEffect(0x6A);
                        } else {
                            playSoundEffect(0x67);
                        }
                    }
                } else if (!quiet) {
                    shown = 1;
                    addBattleLogLine(player, "「ぼうがい」により");
                    addBattleLogLine(player, "援護攻撃はc2無効！");
                    playSoundEffect(0x6A);
                }
                break;
            case 4:
            case 5:
                if (STATS(player)->flags.attackChoice == 2) {
                    if (!quiet) {
                        shown = 1;
                        LOG_CARD = getActiveDigimonCard(player);
                        DUEL->cursorSlot = 4;
                        SET_CARD_LIGHT(LOG_CARD, 0xFF);
                        sprintf(text, "c6%sc7のb2特殊攻撃！", effects[k].card + 3);
                        addBattleLogLine(player, text);
                    }
                    KAW_applyCrossEffect(player, player ^ 1, (DigimonCardData *)effects[k].card, quiet);
                }
                break;
            }
            if (shown && countBattleLogLines(player) != 0) {
                waitForCpuDecision();
                if (!quiet && LOG_CARD != -1) {
                    SET_CARD_LIGHT(LOG_CARD, 0x80);
                    LOG_CARD = -1;
                    DUEL->cursorSlot = -1;
                }
            }
        }
    }
    if (!quiet) {
        for (i = 0; i < 2; i++) {
            player = DUEL->turnPlayer ^ i;
            if (getPlayedCard(player) != -1 && (effects[i].order | effects[i + 2].order) == 0) {
                clearBattleLog();
                if (SPRITE_KIND(getPlayedCard(player)) == 0x17) {
                    SPRITE_KIND(getPlayedCard(player)) = 0xE;
                    waitFrames(0x10);
                }
                LOG_CARD = getPlayedCard(player);
                DUEL->cursorSlot = 4;
                SET_CARD_LIGHT(getPlayedCard(player), 0xFF);
                addBattleLogLine(player, "オプションスロットの");
                sprintf(text, "c6%sカードc7はc2効果なし！",
                        PLAYER_CARDS(PLAYER(player))[getPlayedCard(player) % 30].card + 3);
                addBattleLogLine(player, text);
                playSoundEffect(0x6A);
                waitForCpuDecision();
                DUEL->cursorSlot = -1;
                SET_CARD_LIGHT(getPlayedCard(player), 0x80);
            }
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
        if (!defender->flags.f6) {
            attacker->attackDamage[2] = attacker->stats[0];
        } else {
            defender->attackDamage[defender->flags.usedAttack] = attacker->stats[0];
        }
        attacker->stats[0] = 10;
    }
    defender->damageTaken = attacker->attackDamage[attacker->flags.usedAttack];
    if (!quiet) {
        KAW_recordBestDamage((Player *)attacker);
    }
    if (attacker->flags.f12) {
        if (defender->hpBeforeBattle <= defender->damageTaken) {
            attacker->hpGain = defender->hpBeforeBattle;
        } else {
            attacker->hpGain = defender->damageTaken;
        }
        attacker->stats[0] += attacker->hpGain;
        if (attacker->stats[0] > 9990) {
            attacker->stats[0] = 9990;
        }
    }
    defender->stats[0] -= defender->damageTaken;
    if (defender->stats[0] <= 0) {
        defender->stats[0] = 0;
    } else {
        if (defender->flags.f11) {
            if (!defender->flags.f6) {
                defender->attackDamage[2] = defender->stats[0];
            }
            defender->stats[0] = 10;
        }
        attacker->damageTaken = defender->attackDamage[defender->flags.usedAttack];
        if (!quiet) {
            KAW_recordBestDamage((Player *)defender);
        }
        if (defender->flags.f12) {
            if (attacker->hpBeforeBattle <= attacker->damageTaken) {
                defender->hpGain = attacker->hpBeforeBattle;
            } else {
                defender->hpGain = attacker->damageTaken;
            }
            defender->stats[0] += defender->hpGain;
            if (defender->stats[0] > 9990) {
                defender->stats[0] = 9990;
            }
        }
        attacker->stats[0] -= attacker->damageTaken;
        if (attacker->stats[0] <= 0) {
            attacker->stats[0] = 0;
        }
    }
    attacker->hpAfterBattle = attacker->stats[0];
    defender->hpAfterBattle = defender->stats[0];
    attacker->stats[0] = attacker->hpBeforeBattle;
    defender->stats[0] = defender->hpBeforeBattle;
}
#elif VERSION_US || VERSION_EU
s32 KAW_resolveBattle(s32 quiet) {
    BattleEffect effects[6];
    u8 unused[0x88]; /* unused, but it is in the original stack frame */
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
            switch (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, type)) {
            case 0:
                effects[i + 2].card = (u8 *)PLAYER_CARDS(PLAYER(player))[card % 30].card;
                effects[i + 2].order = CARD_BYTE(effects[i + 2].card, supportIcon);
                break;
            case 1:
                effects[i].card = (u8 *)PLAYER_CARDS(PLAYER(player))[card % 30].card;
                effects[i].order = CARD_BYTE(effects[i].card, supportConditions[0].unk10[8]);
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
                        waitFrames(0x10);
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
            effects[i + 4].card = (u8 *)PLAYER_CARDS(PLAYER(player))[card % 30].card;
            effects[i + 4].order = CROSS_EFFECT_ICONS[((DigimonCardData *)effects[i + 4].card)->crossEffect];
        }
    }
    for (i = 3; i > 0; i--) {
        for (k = 0; k < 6; k++) {
            player = DUEL->turnPlayer ^ (k % 2);
            if (!quiet && k < 4 && effects[k].order == i && getPlayedCard(player) != -1) {
                if (SPRITE_KIND(getPlayedCard(player)) == 0x19) {
                    SPRITE_KIND(getPlayedCard(player)) = 0x10;
                    waitFrames(0x10);
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
                    waitFrames(0x10);
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
#else
#error "untested version"
#endif

/* what an operand that doesn't apply reads as */
#if VERSION_JP
#define NO_OPERAND 2
#elif VERSION_US || VERSION_EU
#define NO_OPERAND -1
#else
#error "untested version"
#endif

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
        return CARD_BYTE(PLAYER_CARDS(PLAYER(self))[card % 30].card, attr) & 0xF;
    case 14:
        card = getActiveDigimonCard(other);
        return CARD_BYTE(PLAYER_CARDS(PLAYER(other))[card % 30].card, attr) & 0xF;
    case 15:
        switch (countEmptyDigimonStackSlots(self)) {
        case 0:
            return 1;
        case 1:
            card = PLAYER(self)->digimonStack[2];
            if (CARD_BYTE(PLAYER_CARDS(PLAYER(self))[card % 30].card, attr) & 0xF) {
                return 0;
            }
        default:
            return NO_OPERAND;
        }
    case 16:
        switch (countEmptyDigimonStackSlots(other)) {
        case 0:
            return 1;
        case 1:
            card = PLAYER(other)->digimonStack[2];
            if (CARD_BYTE(PLAYER_CARDS(PLAYER(other))[card % 30].card, attr) & 0xF) {
                return 0;
            }
        default:
            return NO_OPERAND;
        }
    case 17:
        return PLAYER(self)->usedAttack;
    case 18:
        return PLAYER(other)->usedAttack;
    case 19:
        card = getPlayedCard(other);
        switch (CARD_BYTE(PLAYER_CARDS(PLAYER(other))[card % 30].card, type)) {
        case 0:
            return 0;
        case 1:
            return 1;
        default:
            return NO_OPERAND;
        }
    case 20:
        return self != DUEL->turnPlayer;
    case 21:
        return 4 - countEmptyHandSlots(self);
    case 22:
        return 4 - countEmptyHandSlots(other);
    case 23:
        return DP_SLOT_COUNT - countEmptyDpSlots(self);
    case 24:
        return DP_SLOT_COUNT - countEmptyDpSlots(other);
    case 25:
        return countOfflineDeckCards(self) == 0;
    case 26:
        return KAW_SUPPORT_REGISTER;
#if VERSION_US || VERSION_EU
    case 27:
        return countOnlineDeckCards(self);
    case 28:
        return countOnlineDeckCards(other);
#elif VERSION_JP
#else
#error "kawseg/cpu/kaw_battle_sim: version not checked"
#endif
    }
    return 0;
}

#define SHOW_EFFECT_FAILED(player) \
    do {                          \
        KAW_playCardEffect(0x13, player, 1); \
    } while (0)

#if VERSION_JP
/* jp tells each support action in the battle log */
/* the cards move while the log waits: the panels switch modes before and after */
#define BEGIN_CARD_MOVES()                   \
    waitForCpuDecision();                    \
    PLAYER_PANEL(0, 9)->state = 4;           \
    PLAYER_PANEL(1, 9)->state = 4;           \
    PLAYER_PANEL(0, 3)->state = 0xD;         \
    PLAYER_PANEL(1, 3)->state = 0xD;         \
    waitFrames(20)
#define END_CARD_MOVES()                     \
    PLAYER_PANEL(0, 3)->state = 0x11;        \
    PLAYER_PANEL(1, 3)->state = 0x11

/* says there was nothing to move, and leaves */
#define LOG_NOTHING(message)                 \
    sprintf(text, message, 0);               \
    addBattleLogLine(self, text);            \
    waitForCpuDecision();                    \
    return

/* picks one of PLAYER's hand cards at random */
#define PICK_HAND_CARD(player)                       \
    for (j = 0, n = 0; j < 4; j++) {                 \
        cards[n] = PLAYER(player)->hand[j];          \
        if (cards[n] != -1) {                        \
            n++;                                     \
        }                                            \
    }                                                \
    card = cards[rand() % n]

s32 KAW_applySupportAction(s32 self, s32 other, s32 kind, s32 value, s32 slot, s32 quiet) {
    s32 cards[4];
    char text[72];
    s32 i;
    s32 j;
    s32 n;
    s32 card;

    switch (kind) {
    case 0:
        if (!quiet) {
            sprintf(text, "c5自分c7の属性を変更！");
            addBattleLogLine(self, text);
        }
        PLAYER(self)->specialty = value % 5;
        break;
    case 1:
        if (!quiet) {
            sprintf(text, "c4相手c7の属性を変更！");
            addBattleLogLine(self, text);
        }
        PLAYER(other)->specialty = value % 5;
        break;
    case 2:
        if (!quiet) {
            sprintf(text, "c5自分c7のＨＰをc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(self)->hpBeforeBattle = value;
        STATS(self)->stats[0] = value;
        break;
    case 3:
        if (!quiet) {
            sprintf(text, "c4相手c7のＨＰをc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(other)->hpBeforeBattle = value;
        STATS(other)->stats[0] = value;
        break;
    case 4:
        if (!quiet) {
            sprintf(text, "c5自分c7のb0攻撃力をc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(self)->attackDamage[(kind - 4) / 2] = value;
        STATS(self)->stats[(kind - 4) / 2 + 1] = value;
        break;
    case 6:
        if (!quiet) {
            sprintf(text, "c5自分c7のb1攻撃力をc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(self)->attackDamage[(kind - 4) / 2] = value;
        STATS(self)->stats[(kind - 4) / 2 + 1] = value;
        break;
    case 8:
        if (!quiet) {
            sprintf(text, "c5自分c7のb2攻撃力をc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(self)->attackDamage[(kind - 4) / 2] = value;
        STATS(self)->stats[(kind - 4) / 2 + 1] = value;
        break;
    case 5:
        if (!quiet) {
            sprintf(text, "c4相手c7のb0攻撃力をc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(other)->attackDamage[(kind - 5) / 2] = value;
        STATS(other)->stats[(kind - 5) / 2 + 1] = value;
        break;
    case 7:
        if (!quiet) {
            sprintf(text, "c4相手c7のb1攻撃力をc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(other)->attackDamage[(kind - 5) / 2] = value;
        STATS(other)->stats[(kind - 5) / 2 + 1] = value;
        break;
    case 9:
        if (!quiet) {
            sprintf(text, "c4相手c7のb2攻撃力をc6%dc7に！", value);
            addBattleLogLine(self, text);
        }
        STATS(other)->attackDamage[(kind - 5) / 2] = value;
        STATS(other)->stats[(kind - 5) / 2 + 1] = value;
        break;
    case 10:
        if (!quiet) {
            sprintf(text, "c5自分c7のb%d攻撃力をc6%dc7に！", slot, value);
            addBattleLogLine(self, text);
        }
        STATS(self)->attackDamage[slot] = value;
        STATS(self)->stats[slot + 1] = value;
        break;
    case 11:
        if (!quiet) {
            sprintf(text, "c4相手c7のb%d攻撃力をc6%dc7に！", slot, value);
            addBattleLogLine(self, text);
        }
        STATS(other)->attackDamage[slot] = value;
        STATS(other)->stats[slot + 1] = value;
        break;
    case 16:
        if (!quiet) {
            sprintf(text, "c5自分c7の攻撃をc6b%dc7に！", value % 3);
            PLAYER(self)->attackChoice = value % 3;
            addBattleLogLine(self, text);
        }
        PLAYER(self)->usedAttack = value % 3;
        break;
    case 17:
        if (!quiet) {
            sprintf(text, "c4相手c7の攻撃をc6b%dc7に！", value % 3);
            PLAYER(other)->attackChoice = value % 3;
            addBattleLogLine(self, text);
        }
        PLAYER(other)->usedAttack = value % 3;
        break;
    case 19:
        if (!quiet) {
            if (value != 1) {
                sprintf(text, "%sが先攻に！", PLAYER(other)->name);
                addBattleLogLine(self, text);
            } else {
                sprintf(text, "%sが後攻に！", PLAYER(other)->name);
                addBattleLogLine(self, text);
            }
        }
        FLAGS178(self)->f8 = value ^ 1;
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
                LOG_NOTHING("c5自分c7の手札がなかった！");
            }
            sprintf(text, "c5自分c7の手札を%d枚捨てる！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && countEmptyHandSlots(self) != 4; i++) {
                PICK_HAND_CARD(self);
                if (removeCardFromHand(card, self) != -1) {
                    SPRITE_KIND(card) = 6;
                    discardCardToOfflineDeck(card, self);
                    waitFrames(20);
                }
            }
            END_CARD_MOVES();
        }
        break;
    case 27:
        if (!quiet) {
            if (4 - countEmptyHandSlots(other) < value) {
                value = 4 - countEmptyHandSlots(other);
            }
            if (value == 0) {
                LOG_NOTHING("c4相手c7の手札がなかった！");
            }
            sprintf(text, "c4相手c7の手札を%d枚捨てる！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && countEmptyHandSlots(other) != 4; i++) {
                PICK_HAND_CARD(other);
                if (removeCardFromHand(card, other) != -1) {
                    SPRITE_KIND(card) = 6;
                    discardCardToOfflineDeck(card, other);
                    waitFrames(20);
                }
            }
            END_CARD_MOVES();
        }
        break;
    case 28:
        value = 0;
        if (!quiet) {
            for (i = 0; i < 4; i++) {
                if (PLAYER(self)->hand[i] != -1 && PLAYER_CARDS(PLAYER(self))[PLAYER(self)->hand[i] % 30].card[2] != 0) {
                    value++;
                }
            }
            if (value == 0) {
                addBattleLogLine(self, "c5自分c7の手札に");
                LOG_NOTHING("オプションカードがなかった！");
            }
            addBattleLogLine(self, "c5自分c7の手札のオプションカード");
            addBattleLogLine(self, "を全て捨てる！");
            BEGIN_CARD_MOVES();
            for (i = 0; i < 4; i++) {
                if (PLAYER(self)->hand[i] != -1 && PLAYER_CARDS(PLAYER(self))[PLAYER(self)->hand[i] % 30].card[2] != 0 &&
                    removeCardFromHand(PLAYER(self)->hand[i], self) != -1) {
                    SPRITE_KIND(PLAYER(self)->hand[i]) = 6;
                    discardCardToOfflineDeck(PLAYER(self)->hand[i], self);
                    waitFrames(20);
                }
            }
            END_CARD_MOVES();
        }
        break;
    case 29:
        value = 0;
        if (!quiet) {
            for (i = 0; i < 4; i++) {
                if (PLAYER(other)->hand[i] != -1 && PLAYER_CARDS(PLAYER(other))[PLAYER(other)->hand[i] % 30].card[2] != 0) {
                    value++;
                }
            }
            if (value == 0) {
                addBattleLogLine(self, "c4相手c7の手札に");
                LOG_NOTHING("オプションカードがなかった！");
            }
            addBattleLogLine(self, "c4相手c7の手札のオプションカード");
            addBattleLogLine(self, "を全て捨てる！");
            for (i = 0; i < 4; i++) {
                if (PLAYER(other)->hand[i] != -1 && PLAYER_CARDS(PLAYER(other))[PLAYER(other)->hand[i] % 30].card[2] != 0 &&
                    removeCardFromHand(PLAYER(other)->hand[i], other) != -1) {
                    SPRITE_KIND(PLAYER(other)->hand[i]) = 6;
                    discardCardToOfflineDeck(PLAYER(other)->hand[i], other);
                    waitFrames(20);
                }
            }
            END_CARD_MOVES();
        }
        break;
    case 30:
        if (!quiet) {
            if (4 - countEmptyHandSlots(self) < value) {
                value = 4 - countEmptyHandSlots(self);
            }
            if (value == 0) {
                LOG_NOTHING("c5自分c7の手札がなかった！");
            }
            sprintf(text, "c5自分c7の手札を%d枚山札へ！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && countEmptyHandSlots(self) != 4; i++) {
                PICK_HAND_CARD(self);
                returnCardToOnlineDeck(card, self);
                SPRITE_KIND(card) = 1;
                removeCardFromHand(card, self);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 31:
        if (!quiet) {
            if (4 - countEmptyHandSlots(other) < value) {
                value = 4 - countEmptyHandSlots(other);
            }
            if (value == 0) {
                LOG_NOTHING("c4相手c7の手札がなかった！");
            }
            sprintf(text, "c4相手c7の手札を%d枚山札へ！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && countEmptyHandSlots(other) != 4; i++) {
                PICK_HAND_CARD(other);
                returnCardToOnlineDeck(card, self);
                SPRITE_KIND(card) = 1;
                removeCardFromHand(card, other);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 32:
        if (!quiet) {
            if (countOnlineDeckCards(self) < value) {
                value = countOnlineDeckCards(self);
            }
            if (value == 0) {
                LOG_NOTHING("c5自分c7の山札がなかった！");
            }
            sprintf(text, "c5自分c7の山札から%d枚捨てる！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && (card = peekOnlineDeckTop(self)) != -1; i++) {
                discardCardToOfflineDeck(card, self);
                SPRITE_KIND(card) = 6;
                drawOnlineDeckCard(self);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 33:
        if (!quiet) {
            if (countOnlineDeckCards(other) < value) {
                value = countOnlineDeckCards(other);
            }
            if (value == 0) {
                LOG_NOTHING("c4相手c7の山札がなかった！");
            }
            sprintf(text, "c4相手c7の山札から%d枚捨てる！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && (card = peekOnlineDeckTop(other)) != -1; i++) {
                discardCardToOfflineDeck(card, other);
                SPRITE_KIND(card) = 6;
                drawOnlineDeckCard(other);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 34:
        if (!quiet) {
            if (countOfflineDeckCards(self) < value) {
                value = countOfflineDeckCards(self);
            }
            if (value == 0) {
                LOG_NOTHING("c5自分c7の捨てカードがなかった！");
            }
            sprintf(text, "c5自分c7の捨て場から%d枚山札へ！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && (card = peekOfflineDeckTop(self)) != -1; i++) {
                returnCardToOnlineDeck(card, self);
                SPRITE_KIND(card) = 1;
                takeOfflineDeckTopCard(self);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 35:
        if (!quiet) {
            if (countOfflineDeckCards(other) < value) {
                value = countOfflineDeckCards(other);
            }
            if (value == 0) {
                LOG_NOTHING("c4相手c7の捨てカードがなかった！");
            }
            sprintf(text, "c4相手c7の捨て場から%d枚山札へ！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && (card = peekOfflineDeckTop(other)) != -1; i++) {
                returnCardToOnlineDeck(card, other);
                SPRITE_KIND(card) = 1;
                takeOfflineDeckTopCard(other);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 36:
        if (!quiet) {
            if (9 - countEmptyDpSlots(self) < value) {
                value = 9 - countEmptyDpSlots(self);
            }
            if (value == 0) {
                LOG_NOTHING("c5自分c7のＰＯＷカードがなかった！");
            }
            sprintf(text, "c5自分c7の進化置場から%d枚捨てる！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && (card = peekDpSlotTop(self)) != -1; i++) {
                discardCardToOfflineDeck(card, self);
                SPRITE_KIND(card) = 6;
                removeCardFromDpSlots(card, self);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 37:
        if (!quiet) {
            if (9 - countEmptyDpSlots(other) < value) {
                value = 9 - countEmptyDpSlots(other);
            }
            if (value == 0) {
                LOG_NOTHING("c4相手c7のＰＯＷカードがなかった！");
            }
            sprintf(text, "c4相手c7の進化置場から%d枚捨てる！", value);
            addBattleLogLine(self, text);
            BEGIN_CARD_MOVES();
            for (i = 0; i < value && (card = peekDpSlotTop(other)) != -1; i++) {
                discardCardToOfflineDeck(card, other);
                SPRITE_KIND(card) = 6;
                removeCardFromDpSlots(card, other);
                waitFrames(20);
            }
            END_CARD_MOVES();
        }
        break;
    case 42:
        if (!quiet) {
            addBattleLogLine(self, "c5自分c7の山札シャッフル！");
            PLAYER(self)->shufflePasses = 300;
            shuffleOnlineDeck(self);
        }
        break;
    case 43:
        if (!quiet) {
            addBattleLogLine(self, "c4相手c7の山札シャッフル！");
            PLAYER(other)->shufflePasses = 300;
            shuffleOnlineDeck(other);
        }
        break;
    case 44:
        if (!quiet) {
            addBattleLogLine(self, "c4相手c7の援護カード無効！");
        }
        FLAGS178(other)->f9 |= 1;
        break;
    case 45:
        if (!quiet) {
            addBattleLogLine(self, "c4相手c7のオプションスロット無効！");
        }
        FLAGS178(other)->f9 = 3;
        break;
    case 47:
        if (!quiet) {
            sprintf(text, "c4相手c7の攻撃をb%dからb%dに変更！",
                    PLAYER(other)->usedAttack, (PLAYER(other)->usedAttack + 1) % 3);
            PLAYER(other)->attackChoice = (PLAYER(other)->usedAttack + 1) % 3;
            addBattleLogLine(self, text);
        }
        PLAYER(other)->usedAttack = (PLAYER(other)->usedAttack + 1) % 3;
        break;
    case 48:
        if (!quiet) {
            addBattleLogLine(self, "c5自分c7の復活！");
        }
        FLAGS178(self)->f14 = 1;
        PLAYER(self)->reviveHp = value;
        break;
    case 51:
        if (!quiet) {
            addBattleLogLine(self, "c5自分c7に「すいとる」効果！");
        }
        FLAGS178(self)->f12 = 1;
        break;
    case 52:
        if (!quiet) {
            addBattleLogLine(self, "c5自分c7に「カウンター」効果！");
        }
        FLAGS178(self)->f6 = 1;
        FLAGS178(other)->f6 = 0;
        break;
    case 53:
        if (!quiet) {
            addBattleLogLine(self, "c5自分c7に「せんせい」効果！");
        }
        FLAGS178(self)->f8 = 1;
        break;
    }
}
#elif VERSION_US || VERSION_EU
void KAW_applySupportAction(s32 self, s32 other, s32 kind, s32 value, s32 slot, s32 quiet) {
    s32 cards[4];
    u8 unused[0x90]; /* unused, but it is in the original stack frame */
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
                waitFrames(6);
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
                waitFrames(6);
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
                    waitFrames(20);
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
                    waitFrames(20);
                }
            }
        }
        break;
    case 28:
        if (!quiet) {
            value = 0;
            for (i = 0; i < 4; i++) {
                if (PLAYER(self)->hand[i] != -1 && CARD_BYTE(PLAYER_CARDS(PLAYER(self))[PLAYER(self)->hand[i] % 30].card, type) != 0) {
                    value++;
                }
            }
            if (value == 0) {
                SHOW_EFFECT_FAILED(self);
            } else {
                for (i = 0; i < 4; i++) {
                    if (PLAYER(self)->hand[i] != -1 && CARD_BYTE(PLAYER_CARDS(PLAYER(self))[PLAYER(self)->hand[i] % 30].card, type) != 0 &&
                        removeCardFromHand(PLAYER(self)->hand[i], self) != -1) {
                        SPRITE_KIND(PLAYER(self)->hand[i]) = 8;
                        discardCardToOfflineDeck(PLAYER(self)->hand[i], self);
                        waitFrames(20);
                    }
                }
            }
        }
        break;
    case 29:
        if (!quiet) {
            value = 0;
            for (i = 0; i < 4; i++) {
                if (PLAYER(other)->hand[i] != -1 && CARD_BYTE(PLAYER_CARDS(PLAYER(other))[PLAYER(other)->hand[i] % 30].card, type) != 0) {
                    value++;
                }
            }
            if (value == 0) {
                SHOW_EFFECT_FAILED(self);
            } else {
                for (i = 0; i < 4; i++) {
                    if (PLAYER(other)->hand[i] != -1 && CARD_BYTE(PLAYER_CARDS(PLAYER(other))[PLAYER(other)->hand[i] % 30].card, type) != 0 &&
                        removeCardFromHand(PLAYER(other)->hand[i], other) != -1) {
                        SPRITE_KIND(PLAYER(other)->hand[i]) = 8;
                        discardCardToOfflineDeck(PLAYER(other)->hand[i], other);
                        waitFrames(20);
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
                waitFrames(20);
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
                waitFrames(20);
            }
        }
        break;
    case 32:
        if (!quiet) {
            for (i = 0; i < value; i++) {
                card = peekOnlineDeckTop(self);
                if (card == -1) {
                    break;
                }
                discardCardToOfflineDeck(card, self);
                drawOnlineDeckCard(self);
                SPRITE_KIND(card) = 8;
                waitFrames(20);
            }
        }
        break;
    case 33:
        if (!quiet) {
            for (i = 0; i < value; i++) {
                card = peekOnlineDeckTop(other);
                if (card == -1) {
                    break;
                }
                discardCardToOfflineDeck(card, other);
                drawOnlineDeckCard(other);
                SPRITE_KIND(card) = 8;
                waitFrames(20);
            }
        }
        break;
    case 34:
        if (!quiet) {
            for (i = 0; i < value; i++) {
                card = peekOfflineDeckTop(self);
                if (card == -1) {
                    break;
                }
                returnCardToOnlineDeck(card, self);
                takeOfflineDeckTopCard(self);
                SPRITE_KIND(card) = 1;
                waitFrames(20);
            }
        }
        break;
    case 35:
        if (!quiet) {
            for (i = 0; i < value; i++) {
                card = peekOfflineDeckTop(other);
                if (card == -1) {
                    break;
                }
                returnCardToOnlineDeck(card, other);
                takeOfflineDeckTopCard(other);
                SPRITE_KIND(card) = 1;
                waitFrames(20);
            }
        }
        break;
    case 36:
        if (!quiet) {
            for (i = 0; i < value; i++) {
                card = peekDpSlotTop(self);
                if (card == -1) {
                    break;
                }
                discardCardToOfflineDeck(card, self);
                removeCardFromDpSlots(card, self);
                SPRITE_KIND(card) = 8;
                waitFrames(20);
            }
        }
        break;
    case 37:
        if (!quiet) {
            for (i = 0; i < value; i++) {
                card = peekDpSlotTop(other);
                if (card == -1) {
                    break;
                }
                discardCardToOfflineDeck(card, other);
                removeCardFromDpSlots(card, other);
                SPRITE_KIND(card) = 8;
                waitFrames(20);
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
            if (card != -1 && CARD_BYTE(PLAYER_CARDS(PLAYER(other))[card % 30].card, type) == 0) {
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
                CARD_ANIM(card)->handSlot = n;
                waitFrames(20);
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
        PLAYER(self)->reviveHp = value;
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
                CARD_ANIM(card)->handSlot = n;
                waitFrames(20);
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
                CARD_ANIM(card)->handSlot = n;
                waitFrames(20);
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
#else
#error "untested version"
#endif

#if VERSION_JP
void KAW_applyCrossEffect(s32 self, s32 other, DigimonCardData *cardData, s32 quiet) {
    char text[0x40];

    switch (cardData->crossEffect) {
    case 0:
        break;
    case 1:
        if (!quiet) {
            /* 「せんせい」発動！！ */
            addBattleLogLine(self, "「せんせい」発動！！");
            playSoundEffect(0x67);
        }
        FLAGS178(self)->f8 = 1;
        break;
    case 2:
        if (!quiet) {
            /* 「b0を０に」発動！！ */
            addBattleLogLine(self, "「b0を０に」発動！！");
            playSoundEffect(0x67);
        }
        STATS(other)->attackDamage[0] = 0;
        STATS(other)->stats[1] = 0;
        break;
    case 3:
        if (!quiet) {
            /* 「b1を０に」発動！！ */
            addBattleLogLine(self, "「b1を０に」発動！！");
            playSoundEffect(0x67);
        }
        STATS(other)->attackDamage[1] = 0;
        STATS(other)->stats[2] = 0;
        break;
    case 4:
        if (!quiet) {
            /* 「b2を０に」発動！！ */
            addBattleLogLine(self, "「b2を０に」発動！！");
            playSoundEffect(0x67);
        }
        STATS(other)->attackDamage[2] = 0;
        STATS(other)->stats[3] = 0;
        break;
    case 5:
        if (!quiet) {
            /* 「b0カウンター」準備！ */
            addBattleLogLine(self, "「b0カウンター」準備！");
            playSoundEffect(0x67);
        }
        if (PLAYER(other)->usedAttack == 0) {
            FLAGS178(self)->f6 = 1;
            FLAGS178(other)->f6 = 0;
        }
        break;
    case 6:
        if (!quiet) {
            /* 「b1カウンター」準備！ */
            addBattleLogLine(self, "「b1カウンター」準備！");
            playSoundEffect(0x67);
        }
        if (PLAYER(other)->usedAttack == 1) {
            FLAGS178(self)->f6 = 1;
            FLAGS178(other)->f6 = 0;
        }
        break;
    case 7:
        if (!quiet) {
            /* 「b2カウンター」準備！ */
            addBattleLogLine(self, "「b2カウンター」準備！");
            playSoundEffect(0x67);
        }
        if (PLAYER(other)->usedAttack == 2) {
            FLAGS178(self)->f6 = 1;
            FLAGS178(other)->f6 = 0;
        }
        break;
    case 8:
        if (!quiet) {
            /* 「自爆」準備ＯＫ！ */
            addBattleLogLine(self, "「自爆」準備ＯＫ！");
            playSoundEffect(0x67);
        }
        FLAGS178(self)->f11 = 1;
        break;
    case 9:
        if (!quiet) {
            /* 「すいとる」発動！！ */
            addBattleLogLine(self, "「すいとる」発動！！");
            playSoundEffect(0x67);
        }
        FLAGS178(self)->f12 = 1;
        break;
    case 10:
        if (!quiet) {
            /* 「ぼうがい」発動！！ */
            addBattleLogLine(self, "「ぼうがい」発動！！");
            playSoundEffect(0x67);
        }
        FLAGS178(other)->f9 |= 1;
        break;
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        if (cardData->crossEffect - 11 == PLAYER(other)->specialty) {
            if (!quiet) {
                /* 「対a%d×３」発動！！ */
                sprintf(text, "「対a%d×３」発動！！", cardData->crossEffect - 11);
                addBattleLogLine(self, text);
                playSoundEffect(0x67);
            }
            FLAGS178(self)->f13 = 1;
            STATS(self)->attackDamage[2] *= 3;
            STATS(self)->stats[3] *= 3;
        } else if (!quiet) {
            /* 「対a%d×３」失敗！！ */
            sprintf(text, "「対a%d×３」失敗！！", cardData->crossEffect - 11);
            addBattleLogLine(self, text);
            playSoundEffect(0x6A);
        }
        break;
    }
}
#elif VERSION_US || VERSION_EU
void KAW_applyCrossEffect(s32 self, s32 other, DigimonCardData *cardData, s32 quiet) {
    u8 unused[0xB0]; /* unused, but it is in the original stack frame */
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
            if (card != -1 && CARD_BYTE(PLAYER_CARDS(PLAYER(other))[card % 30].card, type) == 0) {
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
#else
#error "untested version"
#endif

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

s32 KAW_runSupportEffect(s32 self, s32 other, SupportCond *conds, SupportEffect *effects, s32 quiet) {
    s32 vals[6];
    s16 slotVals[3][3];
    s32 i;
    s32 j;
    s32 k;
    s32 a;

    for (i = 0; i < 2; i++) {
        if (conds[i].active != 0) {
            for (j = 0; j < 6; j++) {
                vals[j] = KAW_getSupportOperand(self, other, conds[i].lhs[j], conds[i].rhs[j], 0);
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
                        slotVals[j][k] = KAW_getSupportOperand(self, other, effects[i].lhs[j], effects[i].rhs[j], k);
                    }
                }
                for (k = 0; k < 3; k++) {
                    KAW_applySupportAction(self, other, effects[i].kind,
                                  KAW_calcSupportValue(KAW_calcSupportValue(slotVals[2][k], effects[i].ops[1], slotVals[1][k]), effects[i].ops[0], slotVals[0][k]),
                                  k, quiet);
                }
            } else {
                for (j = 0; j < 3; j++) {
                    vals[j] = KAW_getSupportOperand(self, other, effects[i].lhs[j], effects[i].rhs[j], 0);
                }
                KAW_applySupportAction(self, other, effects[i].kind,
                              KAW_calcSupportValue(KAW_calcSupportValue(vals[2], effects[i].ops[1], vals[1]), effects[i].ops[0], vals[0]), 0, quiet);
            }
        }
    }
    return 0;
}

#if VERSION_JP
s32 KAW_checkDigivolveTarget(s32 card, s32 player) {
    s32 specialty;
    s32 level;
    s32 dp;

    if (card == -1) {
        return -1;
    }
    if (getActiveDigimonCard(player) == -1) {
        return -1;
    }
    if (PLAYER_CARDS(PLAYER(player))[card % 30].type != 0) {
        return -1;
    }
    specialty = PLAYER(player)->specialty;
    level = CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr) & 0xF;
    dp = sumDigivolvePoints(player);
    /* the card CARD, which the active Digimon would digivolve into */
    {
        s8 *data = PLAYER_CARDS(PLAYER(player))[card % 30].card;
        s32 cardSpecialty = (u8)CARD_BYTE(data, attr) >> 4;
        s32 cardLevel = CARD_BYTE(data, attr) & 0xF;
        s32 cost = CARD_BYTE(data, dpCost);

        if (getPlayedCard(player) != -1) {
            PlayerDeck *deck = PLAYER(player)->deck;

            if (CARD_BYTE(deck->cards[getPlayedCard(player) % 30].card, type) == 2) {
                PlayerDeck *same = PLAYER(player)->deck;

                switch (CARD_BYTE(same->cards[getPlayedCard(player) % 30].card, attr)) {
                case 0:
                    if (cardLevel == level + 1 && dp + 30 >= cost) {
                        return 0;
                    }
                    break;
                case 1:
                    if (level == 0 && cardSpecialty == specialty && cardLevel == 2 && dp >= cost) {
                        return 0;
                    }
                    break;
                case 2:
                    if (PLAYER(player)->statPenalty == 0 && cardSpecialty == specialty && cardLevel == level + 1) {
                        return 0;
                    }
                    break;
                case 5:
                    return 0;
                case 3:
                    if (cardLevel == level && dp >= cost) {
                        return 0;
                    }
                    break;
                case 4:
                    if (countEmptyDigimonStackSlots(player) < 2) {
                        return 0;
                    }
                    break;
                }
            }
        } else if (cardSpecialty == specialty && cardLevel == level + 1 && dp >= cost) {
            return 0;
        }
    }
    return -1;
}
#elif VERSION_US || VERSION_EU
s32 KAW_checkDigivolveTarget(s32 card, s32 player) {
    s32 specialty;
#if VERSION_US
    u8 level;
#elif VERSION_EU
    s32 level;
#endif
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
    if (PLAYER_CARDS(PLAYER(player))[card % 30].type != 0) {
        return -1;
    }
    specialty = PLAYER(player)->specialty;
    level = CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr) & 0xF;
    dp = sumDigivolvePoints(player);
    data = PLAYER_CARDS(PLAYER(player))[card % 30].card;
    cardSpecialty = (u8)CARD_BYTE(data, attr) >> 4;
    cardLevel = CARD_BYTE(data, attr) & 0xF;
    cost = CARD_BYTE(data, dpCost);
    if (getPlayedCard(player) != -1) {
        p = DUEL_PLAYERS[player];
        p += (getPlayedCard(player) % 30) * sizeof(CardSlot);
        played = PLAYER_CARDS((Player *)p);
        if (CARD_BYTE(played->card, type) == 2) {
            p = DUEL_PLAYERS[player];
            p += (getPlayedCard(player) % 30) * sizeof(CardSlot);
            switch (CARD_BYTE(PLAYER_CARDS((Player *)p)->card, attr)) {
            case 0:
                if (level == 1) {
                    break;
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
                    break;
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
#else
#error "untested version"
#endif

#if VERSION_JP
s32 KAW_checkAnyDigivolve(s32 player) {
    s32 i;
    if (getPlayedCard(player) != -1) {
        PlayerDeck *deck = PLAYER(player)->deck;

        if (CARD_BYTE(deck->cards[getPlayedCard(player) % 30].card, type) == 2) {
            PlayerDeck *same = PLAYER(player)->deck;

            if (CARD_BYTE(same->cards[getPlayedCard(player) % 30].card, attr) == 4 && countEmptyDigimonStackSlots(player) < 2) {
                return 0;
            }
        }
    }
    for (i = 0; i < 4; i++) {
        if (KAW_checkDigivolveTarget(PLAYER(player)->hand[i], player) == 0) {
            return 0;
        }
    }
    return -1;
}
#elif VERSION_US || VERSION_EU
s32 KAW_checkAnyDigivolve(s32 player) {
    s32 i;
    u8 *data;

    if (getPlayedCard(player) != -1) {
        data = DUEL_PLAYERS[player];
        data += (getPlayedCard(player) % 30) * sizeof(CardSlot);
        if (CARD_BYTE(PLAYER_CARDS((Player *)data)[0].card, type) == 2) {
            data = DUEL_PLAYERS[player];
            data += (getPlayedCard(player) % 30) * sizeof(CardSlot);
            switch (CARD_BYTE(PLAYER_CARDS((Player *)data)[0].card, attr)) {
            case 4:
                if (countEmptyDigimonStackSlots(player) < 2) {
                    return 0;
                }
                break;
            case 7:
                if ((CARD_BYTE(PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[getActiveDigimonCard(player) % 30].card, attr) & 0xF) == 1) {
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
#else
#error "untested version"
#endif

s32 KAW_setStatPenalty(s32 card, s32 player) {
    Player *p = (Player *)DUEL_PLAYERS[player];

    p->statPenalty = CARD_BYTE((u8 *)PLAYER_CARDS(p)[card % 30].card, attr);
#if VERSION_US || VERSION_EU
    ((Player *)DUEL_PLAYERS[player])->bonusFlags &= ~0x40000000;
#elif VERSION_JP
#else
#error "kawseg/cpu/kaw_battle_sim: version not checked"
#endif
}

#if VERSION_US || VERSION_EU
void KAW_recordBestDamage(Player *p) {
    s32 player;
    s32 attack;
    s32 card;
    s32 index;

    player = p->controller;
    attack = p->usedAttack;
    if (DUEL->tutorial == 0 && player != CPU_CONTROLLER) {
        card = getActiveDigimonCard(player);
        index = PLAYER_CARDS(p)[card % 30].index;
        if (((PlayerStats *)p)->attackDamage[attack] > ((ProfileK *)PLAYER_PROFILES)[player].bestDamage[index][attack]) {
            ((ProfileK *)PLAYER_PROFILES)[player].bestDamage[index][attack] = ((PlayerStats *)p)->attackDamage[attack];
        }
    }
}
#elif VERSION_JP
#else
#error "kawseg/cpu/kaw_battle_sim: version not checked"
#endif
