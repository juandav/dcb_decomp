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

s32 peekOfflineDeckTop(s32 player) {
    s32 i;
    s8 *pile;
    s32 cardIndex;

    i = 0;
    pile = (s8 *)DUEL_PLAYERS[player] + 0x19B;
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
        if ((((s8 *)DUEL_PLAYERS[player]) + i)[0x19B] == cardIndex) {
            return -1;
        }
        pile = (s8 *)DUEL_PLAYERS[player] + 0x19B;
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
    pile = (s8 *)DUEL_PLAYERS[player] + 0x19B;
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
        pile = (s8 *)DUEL_PLAYERS[player] + 0x19B;
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
    pile = (s8 *)DUEL_PLAYERS[player] + 0x17D;
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
    pile = (s8 *)DUEL_PLAYERS[player] + 0x17D;
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
        pile = (s8 *)DUEL_PLAYERS[player] + 0x17D;
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
        if (PLAYER(player)->unk17D[i] != -1) {
            cardIndex = PLAYER(player)->unk17D[i];
            if (findPartnerSlot(player, PLAYER(player)->cards[cardIndex % 30].id) >= 0) {
                for (j = i; j > 0; j--) {
                    PLAYER(player)->unk17D[j] = PLAYER(player)->unk17D[j - 1];
                }
                PLAYER(player)->unk17D[0] = -1;
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
        if ((((s8 *)DUEL_PLAYERS[player]) + i)[0x17D] == cardIndex) {
            return -1;
        }
        pile = (s8 *)DUEL_PLAYERS[player] + 0x17D;
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
    hand = (s8 *)DUEL_PLAYERS[player] + 0x1B9;
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
        hand = (s8 *)DUEL_PLAYERS[player] + 0x1B9;
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
        hand = (s8 *)DUEL_PLAYERS[player] + 0x1B9;
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
    duelPlayer = (Player *)DUEL_PLAYERS[player];
    for (; i < 4; i++) {
        cardIndex = duelPlayer->unk1B9[i];
        if (cardIndex != -1 && duelPlayer->cards[cardIndex % 30].state == 0) {
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
    duelPlayer = (Player *)DUEL_PLAYERS[player];
    for (; i < 4; i++) {
        cardIndex = duelPlayer->unk1B9[i];
        if (cardIndex != -1 && duelPlayer->cards[cardIndex % 30].state == 1) {
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
    duelPlayer = (Player *)DUEL_PLAYERS[player];
    for (; i < 4; i++) {
        cardIndex = duelPlayer->unk1B9[i];
        if (cardIndex != -1 && duelPlayer->cards[cardIndex % 30].state == 2) {
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
    stack = (s8 *)DUEL_PLAYERS[player] + 0x1CA;
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
    stack = (s8 *)DUEL_PLAYERS[player] + 0x1CA;
    do {
        if (stack[i] == -1) {
            count++;
        }
        i++;
    } while (i < 3);
    return count;
}

s32 placeActiveDigimon(s32 cardIndex, s32 player) {
    s8 *card;
    s32 statShift;
    s32 stackSlot;

    if (cardIndex == -1) {
        return -1;
    }
    card = PLAYER(player)->cards[cardIndex % 30].card;
    statShift = PLAYER(player)->unk178_15 - 1;
    if (statShift < 0) {
        statShift = 0;
    }
    for (stackSlot = 2; stackSlot >= 0; stackSlot--) {
        if (PLAYER(player)->unk1CA[stackSlot] == -1 || PLAYER(player)->unk1CA[stackSlot] == cardIndex) {
            PLAYER(player)->unk1CA[stackSlot] = cardIndex;
            PLAYER(player)->unk178_19 = ((u8)card[0x1A] >> 4);
            PLAYER(player)->unk11C[0] = (*(s16 *)(card + 0x1E) >> statShift) / 10 * 10;
            PLAYER(player)->unk15C[0] = (*(s16 *)(card + 0x20) >> statShift) / 10 * 10;
            PLAYER(player)->unk15C[1] = (*(s16 *)(card + 0x3C) >> statShift) / 10 * 10;
            PLAYER(player)->unk15C[2] = (*(s16 *)(card + 0x58) >> statShift) / 10 * 10;
            PLAYER(player)->unk11C[1] = PLAYER(player)->unk15C[0];
            PLAYER(player)->unk11C[2] = PLAYER(player)->unk15C[1];
            PLAYER(player)->unk11C[3] = PLAYER(player)->unk15C[2];
            PLAYER(player)->unk178_30 = 0;
            return 0;
        }
    }
    return -1;
}

s32 armorDigivolvePartner(s32 player, s32 partnerSlot) {
    Rect16 artRect;
    Rect16 unused;
    s8 *armorCard;
    s32 cardIndex;
    s32 stackSlot;

    if (partnerSlot == -1) {
        return -1;
    }
    cardIndex = getActiveDigimonCard(player);
    setCardSlotFromId(&PLAYER(player)->cards[cardIndex % 30], PLAYER_DATA(player).unk80[partnerSlot].unk292[0]);
    armorCard = (s8 *)&PLAYER_DATA(player).unk80[partnerSlot] + 0x13C;
    PLAYER(player)->cards[cardIndex % 30].card = armorCard;
    artRect.x = ((player << 8) + (partnerSlot + 3) * 40 >> 1) + 0x2C0;
    artRect.y = 0xC8;
    artRect.w = 0x14;
    artRect.h = 0x28;
    MoveImage2(&artRect, ((player << 8) + cardIndex % 30 % 6 * 40 >> 1) + 0x2C0, cardIndex % 30 / 6 * 40);
    for (stackSlot = 0; stackSlot < 3; stackSlot++) {
        if (PLAYER(player)->unk1CA[stackSlot] == cardIndex) {
            PLAYER(player)->unk178_19 = (u8)armorCard[0x1A] >> 4;
            PLAYER(player)->unk11C[0] = *(s16 *)(armorCard + 0x1E);
            PLAYER(player)->unk15C[0] = *(s16 *)(armorCard + 0x20);
            PLAYER(player)->unk15C[1] = *(s16 *)(armorCard + 0x3C);
            PLAYER(player)->unk15C[2] = *(s16 *)(armorCard + 0x58);
            PLAYER(player)->unk11C[1] = PLAYER(player)->unk15C[0];
            PLAYER(player)->unk11C[2] = PLAYER(player)->unk15C[1];
            PLAYER(player)->unk11C[3] = PLAYER(player)->unk15C[2];
            PLAYER(player)->unk178_30 = 0;
            PLAYER(player)->unk170[0] = *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + cardIndex * 60 + 0x10);
            *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + cardIndex * 60 + 0x10) = PLAYER(player)->unk170[partnerSlot + 1];
            return 0;
        }
    }
    return -1;
}

s32 armorDevolvePartner(s32 player, s32 partnerSlot) {
    Rect16 artRect;
    Rect16 unused;
    s8 *baseCard;
    s32 cardIndex;
    s32 stackSlot;

    if (partnerSlot == -1) {
        return -1;
    }
    cardIndex = getActiveDigimonCard(player);
    setCardSlotFromId(&PLAYER(player)->cards[cardIndex % 30], PLAYER_DATA(player).unk80[partnerSlot].unk288);
    baseCard = (s8 *)&PLAYER_DATA(player).unk80[partnerSlot];
    PLAYER(player)->cards[cardIndex % 30].card = baseCard;
    artRect.x = ((player << 8) + partnerSlot * 40 >> 1) + 0x2C0;
    artRect.y = 0xC8;
    artRect.w = 0x14;
    artRect.h = 0x28;
    MoveImage2(&artRect, ((player << 8) + cardIndex % 30 % 6 * 40 >> 1) + 0x2C0, cardIndex % 30 / 6 * 40);
    for (stackSlot = 0; stackSlot < 3; stackSlot++) {
        if (PLAYER(player)->unk1CA[stackSlot] == cardIndex) {
            PLAYER(player)->unk178_19 = (u8)baseCard[0x1A] >> 4;
            PLAYER(player)->unk11C[0] = *(s16 *)(baseCard + 0x1E);
            PLAYER(player)->unk15C[0] = *(s16 *)(baseCard + 0x20);
            PLAYER(player)->unk15C[1] = *(s16 *)(baseCard + 0x3C);
            PLAYER(player)->unk15C[2] = *(s16 *)(baseCard + 0x58);
            PLAYER(player)->unk11C[1] = PLAYER(player)->unk15C[0];
            PLAYER(player)->unk11C[2] = PLAYER(player)->unk15C[1];
            PLAYER(player)->unk11C[3] = PLAYER(player)->unk15C[2];
            PLAYER(player)->unk178_30 = 0;
            *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + cardIndex * 60 + 0x10) = PLAYER(player)->unk170[0];
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
        stack = (s8 *)DUEL_PLAYERS[player] + 0x1CA;
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
    duelPlayer = (Player *)DUEL_PLAYERS[player];
    for (; i < 8; i++) {
        cardIndex = duelPlayer->unk1C2[i];
        if (cardIndex != -1) {
            points += duelPlayer->cards[cardIndex % 30].card[0x1C];
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
    dpSlots = (s8 *)DUEL_PLAYERS[player] + 0x1C2;
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
    dpSlots = (s8 *)DUEL_PLAYERS[player] + 0x1C2;
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
        if ((((s8 *)DUEL_PLAYERS[player]) + i)[0x1C2] == cardIndex) {
            return -1;
        }
        dpSlots = (s8 *)DUEL_PLAYERS[player] + 0x1C2;
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
        dpSlots = (s8 *)DUEL_PLAYERS[player] + 0x1C2;
        entry = dpSlots + i;
        if (*entry != -1 && *entry == cardIndex) {
            *entry = -1;
            return 0;
        }
    }
    return -1;
}

s32 getPlayedCard(s32 player) {
    return ((s8 *)DUEL_PLAYERS[player])[0x1CD];
}

s32 isPlayedCardSlotEmpty(s32 player) {
    return ((s8 *)DUEL_PLAYERS[player])[0x1CD] == -1;
}

s32 setPlayedCard(s32 cardIndex, s32 player) {
    if ((s8)DUEL_PLAYERS[player][0x1CD] == cardIndex) {
        return -1;
    }
    if ((s8)DUEL_PLAYERS[player][0x1CD] == -1) {
        DUEL_PLAYERS[player][0x1CD] = cardIndex;
        return 0;
    }
    return -1;
}

s32 takePlayedCard(s32 player) {
    s8 *duelPlayer = (s8 *)DUEL_PLAYERS[player];
    s32 cardIndex = duelPlayer[0x1CD];

    duelPlayer[0x1CD] = -1;
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
        for (pass = 0; pass < ((Player *)DUEL_PLAYERS[player])->unk11A; pass++) {
            for (i = 30 - cardCount; i < 30; i++) {
                swapIndex = rand() % cardCount + (30 - cardCount);
                swap = ((Player *)DUEL_PLAYERS[player])->unk17D[i];
                ((Player *)DUEL_PLAYERS[player])->unk17D[i] = ((Player *)DUEL_PLAYERS[player])->unk17D[swapIndex];
                ((Player *)DUEL_PLAYERS[player])->unk17D[swapIndex] = swap;
            }
        }
        ((Player *)DUEL_PLAYERS[player])->unk11A = 0;
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
        for (pass = 0; pass < ((Player *)DUEL_PLAYERS[player])->unk11A; pass++) {
            for (i = 30 - cardCount; i < 30; i++) {
                swapIndex = rand() % cardCount + (30 - cardCount);
                swap = ((Player *)DUEL_PLAYERS[player])->unk19B[i];
                ((Player *)DUEL_PLAYERS[player])->unk19B[i] = ((Player *)DUEL_PLAYERS[player])->unk19B[swapIndex];
                ((Player *)DUEL_PLAYERS[player])->unk19B[swapIndex] = swap;
            }
        }
        ((Player *)DUEL_PLAYERS[player])->unk11A = 0;
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/duel/card_zones", PATH_KAWSEG_BIN);

INCLUDE_RODATA("asm/main/nonmatchings/duel/card_zones", PATH_DECK2_DEK);
