#include "common.h"
#include "game.h"
#include "dcb/kaw_card_queries.h"
#include "dcb/card_zones.h"

/* The card ids of each kind of option card the CPU looks for, and how
   many of them it checks */
#if VERSION_JP
#define VOIDING_CARD_COUNT 4
#define PILE_EFFECT_CARD_COUNT 10
#define REVIVE_CARD_COUNT 3
#define RECOVERY_CARD_COUNT 6

s16 KAW_VOIDING_CARDS[4] = {
    0x2C, 0x6B, 0x84, 0x95,
};

s16 KAW_PILE_EFFECT_CARDS[10] = {
    0x61, 0x63, 0x64, 0x65, 0x67, 0x71, 0x83, 0x86,
    0x97, 0x98,
};

s16 KAW_REVIVE_CARDS[4] = {
    2, 0x47, 0x7E, 0,
};

s16 KAW_RECOVERY_CARDS[6] = {
    0x1D, 0x1E, 0x6F, 0x81, 0x90, 0x92,
};
#elif VERSION_US || VERSION_EU
#define VOIDING_CARD_COUNT 5
#define PILE_EFFECT_CARD_COUNT 24
#define REVIVE_CARD_COUNT 4
#define RECOVERY_CARD_COUNT 33

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
#else
#error "untested version"
#endif

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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 2) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0 && (CARD_BYTE(PLAYER_CARDS(p)[card % 30].card, attr) & 0xF) == level) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0 && (CARD_BYTE(PLAYER_CARDS(p)[card % 30].card, attr) & 0xF) == level) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0 && ((u8)CARD_BYTE(PLAYER_CARDS(p)[card % 30].card, attr) >> 4) == attr) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0 && ((u8)CARD_BYTE(PLAYER_CARDS(p)[card % 30].card, attr) >> 4) == attr) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0) {
            data = (u8 *)PLAYER_CARDS(p)[card % 30].card;
            if ((CARD_BYTE(data, attr) >> 4) == attr && (CARD_BYTE(data, attr) & 0xF) == level) {
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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0) {
            data = (u8 *)PLAYER_CARDS(p)[card % 30].card;
            if ((CARD_BYTE(data, attr) >> 4) == attr && (CARD_BYTE(data, attr) & 0xF) == level) {
                count++;
            }
        }
    }
    return count;
}

#if VERSION_JP
s32 KAW_isCardId(s32 id, s32 player, s32 card) {
    s32 type;

    /* the id counts through the Digimon, then the options from 0x6E and the
       digivolve cards from 0x99: the type and index of a CardSlot */
    type = 0;
    if (id >= 0x99) {
        type = 2;
        id -= 0x99;
    } else if (id >= 0x6E) {
        type = 1;
        id -= 0x6E;
    }
    if (card != -1 && PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[card % 30].type == type &&
        PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[card % 30].index == id) {
        return 1;
    }
    return 0;
}
#elif VERSION_US || VERSION_EU
s32 KAW_isCardId(s32 id, s32 player, s32 card) {
    if (card != -1 && PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[card % 30].id == id) {
        return 1;
    }
    return 0;
}
#else
#error "untested version"
#endif

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
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 2 && PLAYER_CARDS(p)[card % 30].index == id) {
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
    level = CARD_BYTE((u8 *)PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[getActiveDigimonCard(player) % 30].card, attr) & 0xF;
    for (i = 0; i < 4; i++) {
        p = (Player *)DUEL_PLAYERS[player];
        card = p->hand[i];
        if (card != -1 && PLAYER_CARDS(p)[card % 30].type == 0) {
            data = (u8 *)PLAYER_CARDS(p)[card % 30].card;
            if ((CARD_BYTE(data, attr) & 0xF) == level && ((DigimonCardData *)data)->hp > p->displayedStats[0]) {
                count++;
            }
        }
    }
    return count;
}

s32 KAW_isVoidingCard(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < VOIDING_CARD_COUNT; i++) {
        if (KAW_isCardId(KAW_VOIDING_CARDS[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 KAW_findVoidingCardInHand(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < VOIDING_CARD_COUNT; i++) {
        found = KAW_findCardIdInHand(KAW_VOIDING_CARDS[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 KAW_isPileEffectCard(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < PILE_EFFECT_CARD_COUNT; i++) {
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

    for (i = 0; i < PILE_EFFECT_CARD_COUNT; i++) {
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

    for (i = 0; i < REVIVE_CARD_COUNT; i++) {
        found = KAW_findCardIdInHand(KAW_REVIVE_CARDS[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

s32 KAW_isRecoveryCard(s32 player, s32 card) {
    s32 i;

    for (i = 0; i < RECOVERY_CARD_COUNT; i++) {
        if (KAW_isCardId(KAW_RECOVERY_CARDS[i], player, card)) {
            return 1;
        }
    }
    return 0;
}

s32 KAW_findRecoveryCardInHand(s32 player) {
    s32 i;
    s32 found;

    for (i = 0; i < RECOVERY_CARD_COUNT; i++) {
        found = KAW_findCardIdInHand(KAW_RECOVERY_CARDS[i], player);
        if (found) {
            return found;
        }
    }
    return 0;
}

#if VERSION_JP
s32 KAW_getActiveCrossEffect(s32 player) {
    PlayerDeck *deck;

    if (getActiveDigimonCard(player) == -1) {
        return 0;
    }
    deck = ((Player *)DUEL_PLAYERS[player])->deck;
    return CARD_BYTE(deck->cards[getActiveDigimonCard(player) % 30].card, crossEffect);
}
#elif VERSION_US || VERSION_EU
s32 KAW_getActiveCrossEffect(s32 player) {
    Player *p;

    if (getActiveDigimonCard(player) == -1) {
        return 0;
    }
    p = (Player *)DUEL_PLAYERS[player];
    return CARD_BYTE(PLAYER_CARDS(p)[getActiveDigimonCard(player) % 30].card, crossEffect);
}
#else
#error "untested version"
#endif

s32 KAW_checkSupportCard(s32 player, s32 card) {
    if (card == -1) {
        return -1;
    }
    if (CARD_BYTE(PLAYER_CARDS((Player *)DUEL_PLAYERS[player])[card % 30].card, type) == 2) {
        return -1;
    }
    return 0;
}
