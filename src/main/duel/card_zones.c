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
    s8 *pile;
    s32 cardIndex;

    i = 0;
    pile = PLAYER(player)->offlineDeck;
    do {
        cardIndex = pile[i];
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
    s8 *pile;
    s8 *entry;

    for (i = 29; i >= 0; i--) {
        if (PLAYER(player)->offlineDeck[i] == cardIndex) {
            return -1;
        }
        pile = PLAYER(player)->offlineDeck;
        entry = pile + i;
        if (*entry == -1) {
            *entry = cardIndex;
            return 0;
        }
    }
    return -1;
}

s32 countOfflineDeckCards(s32 player) {
    s32 i;
    s32 count;
    s8 *pile;

    i = 0;
    count = 0;
    pile = PLAYER(player)->offlineDeck;
    do {
        if (pile[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 takeOfflineDeckTopCard(s32 player) {
    s32 i;
    s8 *pile;
    s8 *entry;

    for (i = 0; i < 30; i++) {
        pile = PLAYER(player)->offlineDeck;
        entry = pile + i;
        if (*entry != -1) {
            s32 cardIndex = *entry;
            *entry = -1;
            return cardIndex;
        }
    }
    return -1;
}

s32 peekOnlineDeckTop(s32 player) {
    s32 i;
    s8 *pile;
    s32 cardIndex;

    i = 0;
    pile = PLAYER(player)->onlineDeck;
    do {
        cardIndex = pile[i];
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
    s8 *pile;

    i = 0;
    count = 0;
    pile = PLAYER(player)->onlineDeck;
    do {
        if (pile[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 drawOnlineDeckCard(s32 player) {
    s32 i;
    s8 *pile;
    s8 *entry;

    for (i = 0; i < 30; i++) {
        pile = PLAYER(player)->onlineDeck;
        entry = pile + i;
        if (*entry != -1) {
            s32 cardIndex = *entry;
            *entry = -1;
            return cardIndex;
        }
    }
    return -1;
}

s32 takePartnerCardFromOnlineDeck(s32 player) {
    s32 i;
    s32 j;
    s32 cardIndex;

    for (i = 0; i < 30; i++) {
        if (PLAYER(player)->onlineDeck[i] != -1) {
            cardIndex = PLAYER(player)->onlineDeck[i];
            if (findPartnerSlot(player, PLAYER(player)->cards[cardIndex % 30].id) >= 0) {
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

s32 returnCardToOnlineDeck(s32 cardIndex, s32 player) {
    s32 i;
    s8 *pile;
    s8 *entry;

    for (i = 29; i >= 0; i--) {
        if (PLAYER(player)->onlineDeck[i] == cardIndex) {
            return -1;
        }
        pile = PLAYER(player)->onlineDeck;
        entry = pile + i;
        if (*entry == -1) {
            *entry = cardIndex;
            return 0;
        }
    }
    return -1;
}

s32 countEmptyHandSlots(s32 player) {
    s32 i;
    s32 count;
    s8 *hand;

    i = 0;
    count = 0;
    hand = PLAYER(player)->hand;
    do {
        if (hand[i] == -1) {
            count++;
        }
        i++;
    } while (i < 4);
    return count;
}

s32 addCardToHand(s32 cardIndex, s32 player) {
    s32 i;
    s8 *hand;
    s8 *entry;

    for (i = 0; i < 4; i++) {
        hand = PLAYER(player)->hand;
        entry = hand + i;
        if (*entry == -1) {
            *entry = cardIndex;
            return i;
        }
    }
    return -1;
}

s32 removeCardFromHand(s32 cardIndex, s32 player) {
    s32 i;
    s8 *hand;
    s8 *entry;

    for (i = 0; i < 4; i++) {
        hand = PLAYER(player)->hand;
        entry = hand + i;
        if (*entry == cardIndex) {
            *entry = -1;
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
        if (cardIndex != -1 && duelPlayer->cards[cardIndex % 30].type == 0) {
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
        if (cardIndex != -1 && duelPlayer->cards[cardIndex % 30].type == 1) {
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
        if (cardIndex != -1 && duelPlayer->cards[cardIndex % 30].type == 2) {
            return 0;
        }
    }
    return -1;
}

s32 getActiveDigimonCard(s32 player) {
    s32 i;
    s8 *stack;
    s32 cardIndex;

    i = 0;
    stack = PLAYER(player)->digimonStack;
    do {
        cardIndex = stack[i];
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
    s8 *stack;

    i = 0;
    count = 0;
    stack = PLAYER(player)->digimonStack;
    do {
        if (stack[i] == -1) {
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
    card = (DigimonCardData *)PLAYER(player)->cards[cardIndex % 30].card;
    /* a stat penalty of n > 1 divides the card's stats by 2^(n-1), rounded down to tens */
    statShift = PLAYER(player)->statPenalty - 1;
    if (statShift < 0) {
        statShift = 0;
    }
    for (stackSlot = 2; stackSlot >= 0; stackSlot--) {
        if (PLAYER(player)->digimonStack[stackSlot] == -1 || PLAYER(player)->digimonStack[stackSlot] == cardIndex) {
            PLAYER(player)->digimonStack[stackSlot] = cardIndex;
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

s32 armorDigivolvePartner(s32 player, s32 partnerSlot) {
    Rect16 artRect;
    Rect16 unused;
    DigimonCardData *armorCard;
    s32 cardIndex;
    s32 stackSlot;

    if (partnerSlot == -1) {
        return -1;
    }
    cardIndex = getActiveDigimonCard(player);
    setCardSlotFromId(&PLAYER(player)->cards[cardIndex % 30], PLAYER_DATA(player).partners[partnerSlot].armorCardId);
    armorCard = &PLAYER_DATA(player).partners[partnerSlot].card[1];
    PLAYER(player)->cards[cardIndex % 30].card = (s8 *)armorCard;
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
            /* unk170[0] keeps the base card's value for armorDevolvePartner */
            PLAYER(player)->unk170[0] = DUEL->sprites[cardIndex].clut;
            DUEL->sprites[cardIndex].clut = PLAYER(player)->unk170[partnerSlot + 1];
            return 0;
        }
    }
    return -1;
}

s32 armorDevolvePartner(s32 player, s32 partnerSlot) {
    Rect16 artRect;
    Rect16 unused;
    DigimonCardData *baseCard;
    s32 cardIndex;
    s32 stackSlot;

    if (partnerSlot == -1) {
        return -1;
    }
    cardIndex = getActiveDigimonCard(player);
    setCardSlotFromId(&PLAYER(player)->cards[cardIndex % 30], PLAYER_DATA(player).partners[partnerSlot].cardId);
    baseCard = &PLAYER_DATA(player).partners[partnerSlot].card[0];
    PLAYER(player)->cards[cardIndex % 30].card = (s8 *)baseCard;
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
            DUEL->sprites[cardIndex].clut = PLAYER(player)->unk170[0];
            return 0;
        }
    }
    return -1;
}

s32 removeCardFromDigimonStack(s32 cardIndex, s32 player) {
    s32 i;
    s8 *stack;
    s8 *entry;

    for (i = 0; i < 3; i++) {
        stack = PLAYER(player)->digimonStack;
        entry = stack + i;
        if (*entry == cardIndex) {
            *entry = -1;
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
    for (; i < 8; i++) {
        cardIndex = duelPlayer->dpSlots[i];
        if (cardIndex != -1) {
            points += ((DigimonCardData *)duelPlayer->cards[cardIndex % 30].card)->dpBonus;
        }
    }
    if (points > 90) {
        points = 90;
    }
    return points;
}

s32 peekDpSlotTop(s32 player) {
    s32 i;
    s8 *dpSlots;
    s32 cardIndex;

    i = 0;
    dpSlots = PLAYER(player)->dpSlots;
    do {
        cardIndex = dpSlots[i];
        if (cardIndex != -1) {
            goto end;
        }
        i++;
    } while (i < 8);
    cardIndex = -1;
end:
    return cardIndex;
}

s32 countEmptyDpSlots(s32 player) {
    s32 i;
    s32 count;
    s8 *dpSlots;

    i = 0;
    count = 0;
    dpSlots = PLAYER(player)->dpSlots;
    do {
        if (dpSlots[i] == -1) {
            count++;
        }
        i++;
    } while (i < 8);
    return count;
}

s32 addCardToDpSlots(s32 cardIndex, s32 player) {
    s32 i;
    s8 *dpSlots;
    s8 *entry;

    for (i = 7; i >= 0; i--) {
        if (PLAYER(player)->dpSlots[i] == cardIndex) {
            return -1;
        }
        dpSlots = PLAYER(player)->dpSlots;
        entry = dpSlots + i;
        if (*entry == -1) {
            *entry = cardIndex;
            return 0;
        }
    }
    return -1;
}

s32 removeCardFromDpSlots(s32 cardIndex, s32 player) {
    s32 i;
    s8 *dpSlots;
    s8 *entry;

    for (i = 0; i < 8; i++) {
        dpSlots = PLAYER(player)->dpSlots;
        entry = dpSlots + i;
        if (*entry != -1 && *entry == cardIndex) {
            *entry = -1;
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
    s8 swap;

    cardCount = countOnlineDeckCards(player);
    if (cardCount >= 2) {
        for (pass = 0; pass < PLAYER(player)->shufflePasses; pass++) {
            for (i = 30 - cardCount; i < 30; i++) {
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
    s8 swap;

    cardCount = countOfflineDeckCards(player);
    if (cardCount >= 2) {
        for (pass = 0; pass < PLAYER(player)->shufflePasses; pass++) {
            for (i = 30 - cardCount; i < 30; i++) {
                swapIndex = rand() % cardCount + (30 - cardCount);
                swap = PLAYER(player)->offlineDeck[i];
                PLAYER(player)->offlineDeck[i] = PLAYER(player)->offlineDeck[swapIndex];
                PLAYER(player)->offlineDeck[swapIndex] = swap;
            }
        }
        PLAYER(player)->shufflePasses = 0;
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/duel/card_zones", PATH_KAWSEG_BIN);

INCLUDE_RODATA("asm/main/nonmatchings/duel/card_zones", PATH_DECK2_DEK);
