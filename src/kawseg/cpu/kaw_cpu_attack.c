#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_card_queries.h"

typedef struct {
#if VERSION_JP
    s16 own;
    s16 opponent;
#elif VERSION_US || VERSION_EU
    s32 own;
    s32 opponent;
#else
#error "untested version"
#endif
    s8 kills;
    s8 survives;
    s8 dies;
} CardScore;

#define SIM(i) (DUEL_AI->sims[i])
/* us only looks for a card against the opponent's top card when it has one;
   jp always looks */
#if VERSION_JP
#define HAS_TOP_CARD(player) 1
#elif VERSION_US || VERSION_EU
#define HAS_TOP_CARD(player) ((s8)PLAYER(player)->topCards[0] != -1)
#else
#error "untested version"
#endif

void KAW_chooseAttack(s32 player) {
    s32 draws;
    s32 wins;
    s32 i;
    s32 weight;
    s32 pick;
    s32 same;

    draws = 0;
    wins = 0;
#if VERSION_US || VERSION_EU
    DUEL->cpuResult = 0;
#elif VERSION_JP
#else
#error "kawseg/cpu/kaw_cpu_attack: version not checked"
#endif
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
#if VERSION_JP
            for (i = 0; i < 3; i++) {
                if (SIM(i).outcome == 0) {
                    DUEL->cpuResult = i;
                    break;
                }
            }
            if (i == 3) {
                DUEL->cpuResult = 0;
            }
#elif VERSION_US || VERSION_EU
            for (i = 0; i < 3; i++) {
                if (SIM(i).outcome == 0) {
                    DUEL->cpuResult = i;
                    return;
                }
            }
            DUEL->cpuResult = 0;
#else
#error "untested version"
#endif
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
            /* jp divides even by a zero weight */
#if VERSION_JP
            pick = rand() % weight;
#elif VERSION_US || VERSION_EU
            if (weight != 0) {
                pick = rand() % weight;
            }
#else
#error "untested version"
#endif
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
            if (PLAYER(player)->cpuAttackStyle != 2 && (rand() & 1)) {
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
        if (PLAYER(player)->cpuAttackStyle != 2 && (rand() & 1)) {
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
        if (HAS_TOP_CARD(opponent)) {
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
        switch (PLAYER(self)->cpuSupportStyle) {
        case 0:
            if (HAS_TOP_CARD(opponent)) {
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
            if (HAS_TOP_CARD(opponent)) {
                for (i = 0; i < 4; i++) {
                    if (scores[i].survives != 0 && KAW_isVoidingCard(self, PLAYER(self)->hand[i]) != 0) {
                        if (KAW_isRecoveryCard(opponent, (s8)PLAYER(opponent)->topCards[0]) |
                            KAW_isPileEffectCard(opponent, (s8)PLAYER(opponent)->topCards[0])) {
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
        switch (PLAYER(self)->cpuSupportStyle) {
        case 0:
            if (HAS_TOP_CARD(opponent) && PLAYER(self)->wins != 2) {
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
            if (HAS_TOP_CARD(opponent)) {
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
