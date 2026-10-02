#include "dcb/card_zones.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/duel_setup.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/battle_hud.h"

/* A duel player's card zones (online and offline decks, hand, DP slots,
   Digimon stack, played card) hold indices into its cards[]; -1 is an empty
   slot. */

s32 peekOfflineDeckTop(s32 player) {
    s32 i;
    Player *p;
    s32 cardIndex;

    i = 0;
    p = PLAYER(player);
    do {
        cardIndex = p->offlineDeck[i];
        if (cardIndex != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    cardIndex = -1;
end:
    return cardIndex;
}

s32 discardCardToOfflineDeck(s32 cardIndex, s32 player) {
    s32 i;

    for (i = 29; i >= 0; i--) {
        if (PLAYER(player)->offlineDeck[i] == cardIndex) {
            return -1;
        }
        if (PLAYER(player)->offlineDeck[i] == -1) {
            PLAYER(player)->offlineDeck[i] = cardIndex;
            return 0;
        }
    }
    return -1;
}

s32 countOfflineDeckCards(s32 player) {
    s32 i;
    s32 count;
    Player *p;

    i = 0;
    count = 0;
    p = PLAYER(player);
    do {
        if (p->offlineDeck[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 takeOfflineDeckTopCard(s32 player) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (PLAYER(player)->offlineDeck[i] != -1) {
            s32 cardIndex = PLAYER(player)->offlineDeck[i];
            PLAYER(player)->offlineDeck[i] = -1;
            return cardIndex;
        }
    }
    return -1;
}

s32 peekOnlineDeckTop(s32 player) {
    s32 i;
    Player *p;
    s32 cardIndex;

    i = 0;
    p = PLAYER(player);
    do {
        cardIndex = p->onlineDeck[i];
        if (cardIndex != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    cardIndex = -1;
end:
    return cardIndex;
}

s32 countOnlineDeckCards(s32 player) {
    s32 i;
    s32 count;
    Player *p;

    i = 0;
    count = 0;
    p = PLAYER(player);
    do {
        if (p->onlineDeck[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 drawOnlineDeckCard(s32 player) {
    s32 i;

    for (i = 0; i < 30; i++) {
        if (PLAYER(player)->onlineDeck[i] != -1) {
            s32 cardIndex = PLAYER(player)->onlineDeck[i];
            PLAYER(player)->onlineDeck[i] = -1;
            return cardIndex;
        }
    }
    return -1;
}

#if VERSION_US || VERSION_EU
s32 takePartnerCardFromOnlineDeck(s32 player) {
    s32 i;
    s32 j;
    s32 cardIndex;

    for (i = 0; i < 30; i++) {
        if (PLAYER(player)->onlineDeck[i] != -1) {
            cardIndex = PLAYER(player)->onlineDeck[i];
            if (findPartnerSlot(player, PLAYER_CARDS(PLAYER(player))[cardIndex % 30].id) >= 0) {
                for (j = i; j > 0; j--) {
                    PLAYER(player)->onlineDeck[j] = PLAYER(player)->onlineDeck[j - 1];
                }
                PLAYER(player)->onlineDeck[0] = -1;
                return cardIndex;
            }
        }
    }
    return -1;
}
#endif

s32 returnCardToOnlineDeck(s32 cardIndex, s32 player) {
    s32 i;

    for (i = 29; i >= 0; i--) {
        if (PLAYER(player)->onlineDeck[i] == cardIndex) {
            return -1;
        }
        if (PLAYER(player)->onlineDeck[i] == -1) {
            PLAYER(player)->onlineDeck[i] = cardIndex;
            return 0;
        }
    }
    return -1;
}

s32 countEmptyHandSlots(s32 player) {
    s32 i;
    s32 count;
    Player *p;

    i = 0;
    count = 0;
    p = PLAYER(player);
    do {
        if (p->hand[i] == -1) {
            count++;
        }
        i++;
    } while (i < 4);
    return count;
}

s32 addCardToHand(s32 cardIndex, s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (PLAYER(player)->hand[i] == -1) {
            PLAYER(player)->hand[i] = cardIndex;
            return i;
        }
    }
    return -1;
}

s32 removeCardFromHand(s32 cardIndex, s32 player) {
    s32 i;

    for (i = 0; i < 4; i++) {
        if (PLAYER(player)->hand[i] == cardIndex) {
            PLAYER(player)->hand[i] = -1;
            return i;
        }
    }
    return -1;
}

s32 checkHandHasDigimonCard(s32 player) {
    Player *duelPlayer;
    s32 i;
    s32 cardIndex;

    i = 0;
    duelPlayer = PLAYER(player);
    for (; i < 4; i++) {
        cardIndex = duelPlayer->hand[i];
        if (cardIndex != -1 && PLAYER_CARDS(duelPlayer)[cardIndex % 30].type == 0) {
            return 0;
        }
    }
    return -1;
}

s32 checkHandHasOptionCard(s32 player) {
    Player *duelPlayer;
    s32 i;
    s32 cardIndex;

    i = 0;
    duelPlayer = PLAYER(player);
    for (; i < 4; i++) {
        cardIndex = duelPlayer->hand[i];
        if (cardIndex != -1 && PLAYER_CARDS(duelPlayer)[cardIndex % 30].type == 1) {
            return 0;
        }
    }
    return -1;
}

s32 checkHandHasDigivolveCard(s32 player) {
    Player *duelPlayer;
    s32 i;
    s32 cardIndex;

    i = 0;
    duelPlayer = PLAYER(player);
    for (; i < 4; i++) {
        cardIndex = duelPlayer->hand[i];
        if (cardIndex != -1 && PLAYER_CARDS(duelPlayer)[cardIndex % 30].type == 2) {
            return 0;
        }
    }
    return -1;
}

s32 getActiveDigimonCard(s32 player) {
    s32 i;
    Player *p;
    s32 cardIndex;

    i = 0;
    p = PLAYER(player);
    do {
        cardIndex = p->digimonStack[i];
        if (cardIndex != -1) {
            goto end;
        }
        i++;
    } while (i < 3);
    cardIndex = -1;
end:
    return cardIndex;
}

s32 countEmptyDigimonStackSlots(s32 player) {
    s32 i;
    s32 count;
    Player *p;

    i = 0;
    count = 0;
    p = PLAYER(player);
    do {
        if (p->digimonStack[i] == -1) {
            count++;
        }
        i++;
    } while (i < 3);
    return count;
}

s32 placeActiveDigimon(s32 cardIndex, s32 player) {
    DigimonCardData *card;
    s32 statShift;
    s32 stackSlot;

    if (cardIndex == -1) {
        return -1;
    }
    card = (DigimonCardData *)PLAYER_CARDS(PLAYER(player))[cardIndex % 30].card;
#if VERSION_JP
    /* a stat penalty of n divides the card's stats by 2^n, rounded down to tens */
    statShift = PLAYER(player)->statPenalty;
    for (stackSlot = 2; stackSlot >= 0; stackSlot--) {
        if (PLAYER(player)->digimonStack[stackSlot] == -1) {
            PLAYER(player)->digimonStack[stackSlot] = cardIndex;
        }
        if (PLAYER(player)->digimonStack[stackSlot] == cardIndex) {
#elif VERSION_US || VERSION_EU
    /* a stat penalty of n > 1 divides the card's stats by 2^(n-1), rounded down to tens */
    statShift = PLAYER(player)->statPenalty - 1;
    if (statShift < 0) {
        statShift = 0;
    }
    for (stackSlot = 2; stackSlot >= 0; stackSlot--) {
        if (PLAYER(player)->digimonStack[stackSlot] == -1 || PLAYER(player)->digimonStack[stackSlot] == cardIndex) {
            PLAYER(player)->digimonStack[stackSlot] = cardIndex;
#endif
            PLAYER(player)->specialty = card->attr >> 4;
            PLAYER(player)->stats[0] = (card->hp >> statShift) / 10 * 10;
            PLAYER(player)->baseAttackPowers[0] = (card->attack[0].power >> statShift) / 10 * 10;
            PLAYER(player)->baseAttackPowers[1] = (card->attack[1].power >> statShift) / 10 * 10;
            PLAYER(player)->baseAttackPowers[2] = (card->attack[2].power >> statShift) / 10 * 10;
            PLAYER(player)->stats[1] = PLAYER(player)->baseAttackPowers[0];
            PLAYER(player)->stats[2] = PLAYER(player)->baseAttackPowers[1];
            PLAYER(player)->stats[3] = PLAYER(player)->baseAttackPowers[2];
            PLAYER(player)->hasBattled = 0;
            return 0;
        }
    }
    return -1;
}

#if VERSION_US || VERSION_EU
s32 armorDigivolvePartner(s32 player, s32 partnerSlot) {
    Rect16 artRect;
    Rect16 unused; /* unused, but it is in the original stack frame */
    DigimonCardData *armorCard;
    s32 cardIndex;
    s32 stackSlot;

    if (partnerSlot == -1) {
        return -1;
    }
    cardIndex = getActiveDigimonCard(player);
    setCardSlotFromId(&PLAYER_CARDS(PLAYER(player))[cardIndex % 30], PLAYER_DATA(player).partners[partnerSlot].armorCardId);
    armorCard = &PLAYER_DATA(player).partners[partnerSlot].card[1];
    PLAYER_CARDS(PLAYER(player))[cardIndex % 30].card = (s8 *)armorCard;
    /* copy the armor's art over the card's art in VRAM */
    artRect.x = ((player << 8) + (partnerSlot + 3) * 40 >> 1) + 0x2C0;
    artRect.y = 0xC8;
    artRect.w = 0x14;
    artRect.h = 0x28;
    MoveImage2(&artRect, ((player << 8) + cardIndex % 30 % 6 * 40 >> 1) + 0x2C0, cardIndex % 30 / 6 * 40);
    for (stackSlot = 0; stackSlot < 3; stackSlot++) {
        if (PLAYER(player)->digimonStack[stackSlot] == cardIndex) {
            PLAYER(player)->specialty = armorCard->attr >> 4;
            PLAYER(player)->stats[0] = armorCard->hp;
            PLAYER(player)->baseAttackPowers[0] = armorCard->attack[0].power;
            PLAYER(player)->baseAttackPowers[1] = armorCard->attack[1].power;
            PLAYER(player)->baseAttackPowers[2] = armorCard->attack[2].power;
            PLAYER(player)->stats[1] = PLAYER(player)->baseAttackPowers[0];
            PLAYER(player)->stats[2] = PLAYER(player)->baseAttackPowers[1];
            PLAYER(player)->stats[3] = PLAYER(player)->baseAttackPowers[2];
            PLAYER(player)->hasBattled = 0;
            /* armorCluts[0] keeps the base card's CLUT for armorDevolvePartner */
            PLAYER(player)->armorCluts[0] = DUEL->sprites[cardIndex].clut;
            DUEL->sprites[cardIndex].clut = PLAYER(player)->armorCluts[partnerSlot + 1];
            return 0;
        }
    }
    return -1;
}
#endif

#if VERSION_US || VERSION_EU
s32 armorDevolvePartner(s32 player, s32 partnerSlot) {
    Rect16 artRect;
    Rect16 unused; /* unused, but it is in the original stack frame */
    DigimonCardData *baseCard;
    s32 cardIndex;
    s32 stackSlot;

    if (partnerSlot == -1) {
        return -1;
    }
    cardIndex = getActiveDigimonCard(player);
    setCardSlotFromId(&PLAYER_CARDS(PLAYER(player))[cardIndex % 30], PLAYER_DATA(player).partners[partnerSlot].cardId);
    baseCard = &PLAYER_DATA(player).partners[partnerSlot].card[0];
    PLAYER_CARDS(PLAYER(player))[cardIndex % 30].card = (s8 *)baseCard;
    /* copy the partner's art back over the card's art in VRAM */
    artRect.x = ((player << 8) + partnerSlot * 40 >> 1) + 0x2C0;
    artRect.y = 0xC8;
    artRect.w = 0x14;
    artRect.h = 0x28;
    MoveImage2(&artRect, ((player << 8) + cardIndex % 30 % 6 * 40 >> 1) + 0x2C0, cardIndex % 30 / 6 * 40);
    for (stackSlot = 0; stackSlot < 3; stackSlot++) {
        if (PLAYER(player)->digimonStack[stackSlot] == cardIndex) {
            PLAYER(player)->specialty = baseCard->attr >> 4;
            PLAYER(player)->stats[0] = baseCard->hp;
            PLAYER(player)->baseAttackPowers[0] = baseCard->attack[0].power;
            PLAYER(player)->baseAttackPowers[1] = baseCard->attack[1].power;
            PLAYER(player)->baseAttackPowers[2] = baseCard->attack[2].power;
            PLAYER(player)->stats[1] = PLAYER(player)->baseAttackPowers[0];
            PLAYER(player)->stats[2] = PLAYER(player)->baseAttackPowers[1];
            PLAYER(player)->stats[3] = PLAYER(player)->baseAttackPowers[2];
            PLAYER(player)->hasBattled = 0;
            DUEL->sprites[cardIndex].clut = PLAYER(player)->armorCluts[0];
            return 0;
        }
    }
    return -1;
}
#endif

s32 removeCardFromDigimonStack(s32 cardIndex, s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (PLAYER(player)->digimonStack[i] == cardIndex) {
            PLAYER(player)->digimonStack[i] = -1;
            return 0;
        }
    }
    return -1;
}

s32 sumDigivolvePoints(s32 player) {
    Player *duelPlayer;
    s32 i;
    s32 points;
    s32 cardIndex;

    i = 0;
    points = 0;
    duelPlayer = PLAYER(player);
    for (; i < DP_SLOT_COUNT; i++) {
        cardIndex = duelPlayer->dpSlots[i];
        if (cardIndex != -1) {
            points += ((DigimonCardData *)PLAYER_CARDS(duelPlayer)[cardIndex % 30].card)->dpBonus;
        }
    }
    /* us and eu cap them at 90 */
#if VERSION_US || VERSION_EU
    if (points > 90) {
        points = 90;
    }
#endif
    return points;
}

s32 peekDpSlotTop(s32 player) {
    s32 i;
    Player *p;
    s32 cardIndex;

    i = 0;
    p = PLAYER(player);
    do {
        cardIndex = p->dpSlots[i];
        if (cardIndex != -1) {
            goto end;
        }
        i++;
    } while (i < DP_SLOT_COUNT);
    cardIndex = -1;
end:
    return cardIndex;
}

s32 countEmptyDpSlots(s32 player) {
    s32 i;
    s32 count;
    Player *p;

    i = 0;
    count = 0;
    p = PLAYER(player);
    do {
        if (p->dpSlots[i] == -1) {
            count++;
        }
        i++;
    } while (i < DP_SLOT_COUNT);
    return count;
}

s32 addCardToDpSlots(s32 cardIndex, s32 player) {
    s32 i;

    for (i = DP_SLOT_COUNT - 1; i >= 0; i--) {
        if (PLAYER(player)->dpSlots[i] == cardIndex) {
            return -1;
        }
        if (PLAYER(player)->dpSlots[i] == -1) {
            PLAYER(player)->dpSlots[i] = cardIndex;
            return 0;
        }
    }
    return -1;
}

s32 removeCardFromDpSlots(s32 cardIndex, s32 player) {
    s32 i;

    for (i = 0; i < DP_SLOT_COUNT; i++) {
        if (PLAYER(player)->dpSlots[i] != -1 && PLAYER(player)->dpSlots[i] == cardIndex) {
            PLAYER(player)->dpSlots[i] = -1;
            return 0;
        }
    }
    return -1;
}

s32 getPlayedCard(s32 player) {
    return PLAYER(player)->playedCard;
}

s32 isPlayedCardSlotEmpty(s32 player) {
    return PLAYER(player)->playedCard == -1;
}

s32 setPlayedCard(s32 cardIndex, s32 player) {
    if (PLAYER(player)->playedCard == cardIndex) {
        return -1;
    }
    if (PLAYER(player)->playedCard == -1) {
        PLAYER(player)->playedCard = cardIndex;
        return 0;
    }
    return -1;
}

s32 takePlayedCard(s32 player) {
    Player *duelPlayer = PLAYER(player);
    s32 cardIndex = duelPlayer->playedCard;

    duelPlayer->playedCard = -1;
    return cardIndex;
}

void shuffleOnlineDeck(s32 player) {
    s32 cardCount;
    s32 pass;
    s32 i;
    s32 swapIndex;
    s32 swap;

    cardCount = countOnlineDeckCards(player);
    if (cardCount >= 2) {
        for (pass = 0; pass < PLAYER(player)->shufflePasses; pass++) {
            /* us and eu shuffle the last cardCount slots, jp the first ones */
#if VERSION_JP
            for (i = 0; i < cardCount; i++) {
#elif VERSION_US || VERSION_EU
            for (i = 30 - cardCount; i < 30; i++) {
#endif
                swapIndex = rand() % cardCount + (30 - cardCount);
                swap = PLAYER(player)->onlineDeck[i];
                PLAYER(player)->onlineDeck[i] = PLAYER(player)->onlineDeck[swapIndex];
                PLAYER(player)->onlineDeck[swapIndex] = swap;
            }
        }
        PLAYER(player)->shufflePasses = 0;
    }
}

void shuffleOfflineDeck(s32 player) {
    s32 cardCount;
    s32 pass;
    s32 i;
    s32 swapIndex;
    s32 swap;

    cardCount = countOfflineDeckCards(player);
    if (cardCount >= 2) {
        for (pass = 0; pass < PLAYER(player)->shufflePasses; pass++) {
            /* us and eu shuffle the last cardCount slots, jp the first ones */
#if VERSION_JP
            for (i = 0; i < cardCount; i++) {
#elif VERSION_US || VERSION_EU
            for (i = 30 - cardCount; i < 30; i++) {
#endif
                swapIndex = rand() % cardCount + (30 - cardCount);
                swap = PLAYER(player)->offlineDeck[i];
                PLAYER(player)->offlineDeck[i] = PLAYER(player)->offlineDeck[swapIndex];
                PLAYER(player)->offlineDeck[swapIndex] = swap;
            }
        }
        PLAYER(player)->shufflePasses = 0;
    }
}
