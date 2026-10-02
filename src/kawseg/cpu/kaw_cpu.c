#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_card_queries.h"

/* the bytes of a Player the battle simulations save and restore */
typedef struct {
#if VERSION_JP
    s32 words[0xA4 / 4];
#elif VERSION_US || VERSION_EU
    s32 words[0x1E4 / 4];
#else
#error "untested version"
#endif
} PlayerSnapshot;

/* not referenced by any code */
const s32 D_801DDF38 = 5;

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

    /* a copy of self: the match depends on it, for the register allocation */
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
            waitFrames(FRAME_INTERVAL);
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
                if (played[player] < 0 || CARD_BYTE(PLAYER_CARDS(PLAYER(player))[played[player] % 30].card, type) == 2) {
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
                    if (played[opponent] < 0 || CARD_BYTE(PLAYER_CARDS(PLAYER(opponent))[played[opponent] % 30].card, type) == 2) {
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
                              CARD_BYTE(PLAYER_CARDS(PLAYER(player))[PLAYER(player)->hand[card] % 30].card, type) != 2)) {
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
#if VERSION_JP || VERSION_EU
    u8 level;
#elif VERSION_US
    s32 level;
#else
#error "untested version"
#endif
    s32 need;
    s32 other;

    if (getActiveDigimonCard(player) == -1) {
        return -1;
    }
    KAW_DUEL->selected = NULL;
    for (i = 0; i < 4; i++) {
        KAW_DUEL->slots[i].card = PLAYER(player)->hand[i];
        KAW_DUEL->slots[i].kind = -1;
        KAW_DUEL->slots[i].need = 100;
        KAW_DUEL->slots[i].option = -1;
        card = PLAYER(player)->hand[i];
        if (card != -1 && PLAYER_CARDS(PLAYER(player))[card % 30].type == 0) {
            KAW_DUEL->slots[i].kind = 0;
            specialty = PLAYER(player)->specialty;
            level = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr) & 0xF;
            need = CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpCost) - sumDigivolvePoints(player);
            if (need < 0) {
                KAW_DUEL->slots[i].need = 0;
            } else {
                KAW_DUEL->slots[i].need = need;
            }
#if VERSION_JP
            if ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) >> 4 == specialty) {
#elif VERSION_US || VERSION_EU
            /* us and eu: a level-1 Digimon can't digivolve, and one of level 0
               counts as 1 */
            if ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) >> 4 == specialty && level != 1) {
                if (level == 0) {
                    level = 1;
                }
#else
#error "untested version"
#endif
                if (((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF) == level + 1) {
                    if (need <= 0) {
                        KAW_DUEL->slots[i].kind = 1;
                    } else {
                        for (j = 0; j < 4; j++) {
                            if (j == i) {
                                continue;
                            }
                            other = PLAYER(player)->hand[j];
                            if (other != -1 && PLAYER_CARDS(PLAYER(player))[other % 30].type == 0) {
                                if (CARD_BYTE(PLAYER_CARDS(PLAYER(player))[other % 30].card, dpBonus) >= need) {
                                    KAW_DUEL->slots[i].kind = 1;
                                    break;
                                }
                                KAW_DUEL->slots[i].kind = 2;
                            }
                        }
                    }
                }
            }
        }
    }
    j = 0;
    for (i = 0; i < 4; i++) {
        if (KAW_DUEL->slots[i].kind == 1) {
            return 1;
        }
        if (KAW_DUEL->slots[i].kind == 2) {
            j = 2;
        }
    }
    return j;
}

/* jp's digivolve options are numbered differently, and its levels count
   from 0 */
#if VERSION_JP
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
        for (i = 0; i < 6; i++) {
            if (KAW_hasDigivolveInHand(i, self) && KAW_countHandDigimon(self) >= 2) {
                switch (i) {
                case 0:
                    for (j = 1; j < 3; j++) {
                        if (sumDigivolvePoints(self) >= j * 20 && KAW_countHandDigimonOfLevel(self, j) >= 2) {
                            return 0;
                        }
                    }
                    break;
                case 1:
                    if (sumDigivolvePoints(self) >= 20 && KAW_countHandDigimonOfLevel(self, 1) != 0) {
                        return 0;
                    }
                    break;
                case 3:
                    for (j = 0; j < 5; j++) {
                        if (KAW_countDeckDigimonOfSpecialtyAndLevel(self, j, 1) != 0 && KAW_countDeckDigimonOfSpecialtyAndLevel(self, j, 2) != 0) {
                            return 0;
                        }
                    }
                    break;
                case 5:
                    return 0;
                case 2:
                case 4:
                    break;
                }
            }
        }
        if (KAW_countDeckDigimonOfLevel(player, 0) != 0) {
            switch (PLAYER(player)->cpuRedrawStyle) {
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
                level = CARD_BYTE(PLAYER_CARDS(PLAYER(self))[getActiveDigimonCard(self) % 30].card, attr) & 0xF;
                switch (i) {
                case 0:
                    if (KAW_countStrongerDigimonInHand(self) != 0) {
                        return 0;
                    }
                    break;
                case 1:
                    if (KAW_countHandDigimonOfLevel(self, level + 1) != 0) {
                        return 0;
                    }
                    break;
                case 2:
                    if (level == 0 && KAW_countHandDigimonOfLevel(self, 2) != 0) {
                        return 0;
                    }
                    break;
                case 3:
                    if (PLAYER(self)->statPenalty == 0 &&
                        KAW_countDeckDigimonOfSpecialtyAndLevel(self, PLAYER(self)->specialty, level + 1) != 0) {
                        return 0;
                    }
                    break;
                case 5:
                    return 0;
                }
            }
        }
        if (PLAYER(opponent)->wins == 2
            && (KAW_findRecoveryCardInHand(player) == 0 || (KAW_findVoidingCardInHand(opponent) != 0 && KAW_getActiveCrossEffect(opponent) == 10))
            && KAW_simulateBattles(player) != 0) {
            if (KAW_planDigivolves(player) == 1) {
                return 0;
            }
            switch (PLAYER(player)->cpuRedrawStyle) {
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
#elif VERSION_US || VERSION_EU
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
            switch (PLAYER(player)->cpuRedrawStyle) {
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
                level = CARD_BYTE(PLAYER_CARDS(PLAYER(self))[getActiveDigimonCard(self) % 30].card, attr) & 0xF;
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
            switch (PLAYER(player)->cpuRedrawStyle) {
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
#else
#error "untested version"
#endif
