#include "common.h"
#include "game.h"
#include "dcb/card_zones.h"
#include "dcb/card_db.h"
#include "dcb/kawseg.h"
#include "dcb/kaw_card_queries.h"

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
    s32 self;
#if VERSION_JP || VERSION_EU
    s32 attr;
#elif VERSION_US
    u8 attr;
#else
#error "untested version"
#endif

    /* a copy of player: the match depends on it, for the register allocation */
    self = player;
    /* jp's Digimon levels go from 0 to 2, us's from 0 to 3 */
#if VERSION_JP
    for (level = 0; level < 3; level++) {
#elif VERSION_US || VERSION_EU
    for (level = 0; level < 4; level++) {
#else
#error "untested version"
#endif
        if (KAW_countHandDigimonOfLevel(self, level) == 0) {
            continue;
        }
        n = 0;
        for (i = 0; i < 4; i++) {
            cands[i].id = -1;
        }
        for (i = 0; i < 4; i++) {
            card = PLAYER(self)->hand[i];
            if (card == -1) {
                continue;
            }
            if (CARD_BYTE(PLAYER_CARDS(PLAYER(self))[card % 30].card, type) != 0) {
                continue;
            }
            /* a partner card goes first (jp has no partners) */
#if VERSION_US || VERSION_EU
            if (getPartnerIndex(PLAYER_CARDS(PLAYER(self))[card % 30].id) >= 0) {
                return card;
            }
#elif VERSION_JP
#else
#error "/tmp/claude-1000/-home-juandav-code-dw-decomp/d2b36c10-5503-4300-af75-672dcd3b6641/scratchpad/eu2/kaw_cpu_placement: version not checked"
#endif
            attr = (u8)CARD_BYTE(PLAYER_CARDS(PLAYER(self))[card % 30].card, attr) >> 4;
            if (((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(self))[card % 30].card, attr) & 0xF) != level) {
                continue;
            }
            cands[n].id = card;
            cands[n].attrCount = KAW_countHandDigimonOfSpecialty(self, attr);
            cands[n].levelCount = KAW_countHandDigimonOfSpecialtyAndLevel(self, attr, 1);
            cands[n].dpCost = CARD_BYTE(PLAYER_CARDS(PLAYER(self))[card % 30].card, dpCost);
            cands[n].deckCount = KAW_countDeckDigimonOfSpecialty(self, attr);
            for (j = 0; j < 3; j++) {
                if ((s8)((DigimonCardData *)PLAYER_CARDS(PLAYER(self))[card % 30].card)->supportActions[j].unk0[0] != 0) {
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
            switch (PLAYER(self)->cpuPlaceStyle) {
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
#if VERSION_JP
        case 1:
        case 2:
#elif VERSION_US || VERSION_EU
        case 2:
        case 3:
#else
#error "untested version"
#endif
            switch (PLAYER(self)->cpuPlaceStyle) {
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
            dp = CARD_BYTE(PLAYER_CARDS(PLAYER(player))[id % 30].card, dpBonus);
            if (dp >= min && dp < best) {
                best = dp;
            }
        }
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best == CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpBonus)) {
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best != CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpBonus)) {
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
        if (card != -1 && best < CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpBonus)) {
            best = CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpBonus);
        }
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best == CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpBonus)) {
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && best != CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, dpBonus)) {
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
        if (card != -1 && KAW_countHandDigimonOfSpecialty(player, (u8)CARD_BYTE(PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[card % 30].card, attr) >> 4) == 1) {
            count++;
        }
    }
    if (count == 0) {
        return -1;
    }
    for (i = 0; i < 4; i++) {
        card = cards[i];
        if (card != -1 && KAW_countHandDigimonOfSpecialty(player, (u8)CARD_BYTE(PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[card % 30].card, attr) >> 4) != 1) {
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

            level = CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr);
            card = cards[i];
            if ((level & 0xF) == ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF)) {
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

            level = CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr);
            card = cards[i];
            if ((level & 0xF) != ((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[card % 30].card, attr) & 0xF)) {
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
        if (cards[i] != -1) {
            n = 0;
            for (j = 0; j < 3; j++) {
#if VERSION_JP
                PlayerDeck *deck = PLAYER(player)->deck;
                s16 card = cards[i];

                if ((s8)((DigimonCardData *)deck->cards[card % 30].card)->supportActions[j].unk0[0] != 0) {
#elif VERSION_US || VERSION_EU
                Player *p = PLAYER(player);
                s16 card = cards[i];

                if ((s8)((DigimonCardData *)PLAYER_CARDS(p)[card % 30].card)->supportActions[j].unk0[0] != 0) {
#else
#error "untested version"
#endif
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
        if (cards[i] != -1) {
            n = 0;
            for (j = 0; j < 3; j++) {
#if VERSION_JP
                PlayerDeck *deck = PLAYER(player)->deck;
                s16 card = cards[i];

                if ((s8)((DigimonCardData *)deck->cards[card % 30].card)->supportActions[j].unk0[0] != 0) {
#elif VERSION_US || VERSION_EU
                Player *p = PLAYER(player);
                s16 card = cards[i];

                if ((s8)((DigimonCardData *)PLAYER_CARDS(p)[card % 30].card)->supportActions[j].unk0[0] != 0) {
#else
#error "untested version"
#endif
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
    if (((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(player))[getActiveDigimonCard(player) % 30].card, attr) & 0xF) != 1) {
        need = 50 - sumDigivolvePoints(player);
    } else {
#if VERSION_JP
        need = 80 - sumDigivolvePoints(player);
#elif VERSION_US || VERSION_EU
        need = 60 - sumDigivolvePoints(player);
#else
#error "untested version"
#endif
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
    /* a copy of player for the rest: the match depends on it, for the
       register allocation */
    self = player;
    if (need <= 0) {
        return -1;
    }
    count = 0;
    for (i = 0; i < 4; i++) {
        ids[i] = PLAYER(self)->hand[i];
        if (ids[i] != -1) {
            if (CARD_BYTE(PLAYER_CARDS(PLAYER(self))[ids[i] % 30].card, type) != 0) {
                ids[i] = -1;
            } else if (CARD_BYTE(PLAYER_CARDS(PLAYER(self))[ids[i] % 30].card, dpBonus) == 0) {
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
    switch (PLAYER(self)->cpuPlaceStyle) {
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
        if (((u8)CARD_BYTE(PLAYER_CARDS(PLAYER(self))[getActiveDigimonCard(self) % 30].card, attr) & 0xF) == 0 && KAW_countHandDigimon(self) < 2) {
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
