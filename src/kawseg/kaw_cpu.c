#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/card_db.h"
#include "dcb/kawseg.h"

typedef struct {
     s16 id;
     u8 attrCount;
     u8 levelCount;
     u8 dpCost;
     u8 deckCount;
     u8 supports;
     u8 pad7;
} Candidate;

typedef struct {
    s16 id;
    s16 unk2;
    s16 unk4;
    s16 unk6;
} Entry8;

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

typedef struct {
    s32 own;
    s32 opponent;
    s8 kills;
    s8 survives;
    s8 dies;
} CardScore;

/* not referenced by any code */
const s32 D_801DDF38 = 5;

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
    KAW_DUEL->selected = NULL;
    for (i = 0; i < 4; i++) {
        KAW_DUEL->slots[i].card = PLAYER(player)->hand[i];
        KAW_DUEL->slots[i].kind = -1;
        KAW_DUEL->slots[i].need = 100;
        KAW_DUEL->slots[i].option = -1;
        card = PLAYER(player)->hand[i];
        if (card != -1 && PLAYER(player)->cards[card % 30].type == 0) {
            KAW_DUEL->slots[i].kind = 0;
            specialty = PLAYER(player)->specialty;
            level = (u8)PLAYER(player)->cards[getActiveDigimonCard(player) % 30].card[0x1A] & 0xF;
            need = PLAYER(player)->cards[card % 30].card[0x1B] - sumDigivolvePoints(player);
            if (need < 0) {
                KAW_DUEL->slots[i].need = 0;
            } else {
                KAW_DUEL->slots[i].need = need;
            }
            if ((u8)PLAYER(player)->cards[card % 30].card[0x1A] >> 4 == specialty && level != 1) {
                if (level == 0) {
                    level = 1;
                }
                if (((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF) == level + 1) {
                    if (need <= 0) {
                        KAW_DUEL->slots[i].kind = 1;
                    } else {
                        for (j = 0; j < 4; j++) {
                            if (j == i) {
                                continue;
                            }
                            other = PLAYER(player)->hand[j];
                            if (other != -1 && PLAYER(player)->cards[other % 30].type == 0) {
                                if (PLAYER(player)->cards[other % 30].card[0x1C] >= need) {
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
    if (KAW_DUEL->selected != NULL) {
        if (KAW_DUEL->selected->kind == 1) {
            if (KAW_DUEL->selected->need == 0) {
                return -1;
            }
        }
        if (KAW_DUEL->selected->kind > 0) {
            need = KAW_DUEL->selected->need;
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
                if (KAW_DUEL->selected != NULL && KAW_DUEL->selected->card == ids[i]) {
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

DigivolvePlan *KAW_selectDigivolvePlan(s32 kind) {
    s32 i;

    if (KAW_DUEL->selected == NULL) {
        switch (kind) {
        case 0:
            KAW_DUEL->selected = NULL;
            break;
        case 1:
            for (i = 0; i < 4; i++) {
                if (KAW_DUEL->slots[i].kind == 1) {
                    KAW_DUEL->selected = &KAW_DUEL->slots[i];
                }
            }
            break;
        case 2:
            for (i = 0; i < 4; i++) {
                if (KAW_DUEL->slots[i].kind == 2) {
                    KAW_DUEL->selected = &KAW_DUEL->slots[i];
                }
            }
            break;
        }
    }
    return KAW_DUEL->selected;
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
    if (KAW_DUEL->selected == NULL) {
        return -1;
    }
    if (PLAYER(self)->cards[KAW_DUEL->selected->card % 30].card[2] != 0) {
        return -1;
    }
    sel = KAW_DUEL->selected->card;
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
    return KAW_DUEL->selected->card;
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
        card = KAW_DUEL->slots[i].card;
        if (card != -1 && PLAYER(player)->cards[card % 30].type == 0 && KAW_DUEL->slots[i].kind == 1 &&
            ((u8)PLAYER(player)->cards[card % 30].card[0x1A] & 0xF) == 2) {
            return;
        }
    }
    for (i = 0; i < 4; i++) {
        option = KAW_DUEL->slots[i].card;
        if (option == -1 || PLAYER(player)->cards[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            card = KAW_DUEL->slots[j].card;
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
                    if (KAW_DUEL->slots[j].need - 20 > 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need - 20) {
                        continue;
                    }
                    if (KAW_DUEL->slots[j].need - 20 < 0) {
                        minNeed[k] = 0;
                    } else {
                        minNeed[k] = KAW_DUEL->slots[j].need - 20;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                for (k = 0; k < 4; k++) {
                    if (minNeed[k] < KAW_DUEL->slots[j].need) {
                        KAW_DUEL->slots[j].need = minNeed[k];
                    }
                }
                break;
            case 1:
                if (level != 0 || target != 3 || targetSpecialty != specialty) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    if (KAW_DUEL->slots[j].need != 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need) {
                        continue;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
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
                KAW_DUEL->slots[j].kind = 1;
                KAW_DUEL->slots[j].need = 0;
                KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                KAW_DUEL->selected = &KAW_DUEL->slots[j];
                break;
            case 3:
                if (level != target) {
                    break;
                }
                flag = PLAYER(player)->displayedStats[0] < ((DigimonCardData *)PLAYER(player)->cards[card % 30].card)->hp;
                if (PLAYER(player)->statPenalty != 0) {
                    flag = 1;
                }
                if (!PLAYER(player)->hasBattled || flag != 1 || KAW_DUEL->selected != NULL) {
                    break;
                }
                for (k = 0; k < 4; k++) {
                    if (KAW_DUEL->slots[j].need != 0 && k == j) {
                        continue;
                    }
                    hand = PLAYER(player)->hand[k];
                    if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                        continue;
                    }
                    if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need) {
                        continue;
                    }
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 4:
                if (countEmptyDigimonStackSlots(player) < 2 && PLAYER(player)->displayedStats[0] < 300) {
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].need = 0;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 5:
                if (level < target) {
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].need = 0;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            case 6:
                if (level != 1 || targetSpecialty != specialty) {
                    break;
                }
                if ((target == 2 && PLAYER(player)->displayedStats[0] < 300) || target == 3) {
                    for (k = 0; k < 4; k++) {
                        if (KAW_DUEL->slots[j].need != 0 && k == j) {
                            continue;
                        }
                        hand = PLAYER(player)->hand[k];
                        if (hand == -1 || PLAYER(player)->cards[hand % 30].type != 0) {
                            continue;
                        }
                        if (PLAYER(player)->cards[hand % 30].card[0x1C] < KAW_DUEL->slots[j].need) {
                            continue;
                        }
                        KAW_DUEL->slots[j].kind = 1;
                        KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                        KAW_DUEL->selected = &KAW_DUEL->slots[j];
                    }
                }
                break;
            case 7:
                if (level == 1 && PLAYER(player)->displayedStats[0] < 300) {
                    KAW_DUEL->slots[j].kind = 1;
                    KAW_DUEL->slots[j].need = 0;
                    KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
                    KAW_DUEL->selected = &KAW_DUEL->slots[j];
                }
                break;
            }
        }
    }
    if (KAW_DUEL->selected != NULL) {
        return;
    }
    for (i = 0; i < 4; i++) {
        option = KAW_DUEL->slots[i].card;
        if (option == -1 || PLAYER(player)->cards[option % 30].type != 2) {
            continue;
        }
        for (j = 0; j < 4; j++) {
            if (i == j) {
                continue;
            }
            card = KAW_DUEL->slots[j].card;
            if (card == -1 || PLAYER(player)->cards[card % 30].type != 2) {
                continue;
            }
            if (PLAYER(player)->cards[option % 30].card[0x1A] == PLAYER(player)->cards[card % 30].card[0x1A]) {
                KAW_DUEL->slots[j].option = KAW_DUEL->slots[i].card;
            }
        }
    }
}

s32 KAW_chooseDigivolveOption(void) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (KAW_DUEL->slots[i].option != -1) {
            return KAW_DUEL->slots[i].option;
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
