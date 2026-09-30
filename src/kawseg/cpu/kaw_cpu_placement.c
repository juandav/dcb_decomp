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
