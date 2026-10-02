#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/player_rank.h"
#include "dcb/partner_level.h"
#include "dcb/hacking_shell.h"
#include "dcb/game_exit.h"

u8 BASE_DECK_IDS[10] = { 0x55, 0x5C, 0x5A, 0x52, 0x58, 0x4F, 0xD, 0, 0x30, 0 };
u8 PARTNER_CARD_IDS[6] = { 0xAF, 0xB6, 0xBE, 0xB8, 0xB7, 0xBB };
u8 PARTNER_ARMOR_CARD_IDS[6][3] = {
    { 0xAC, 0xB9, 0xAD },
    { 0xB3, 0xBC, 0 },
    { 0xBD, 0xB0, 0 },
    { 0xB5, 0xB2, 0 },
    { 0xB4, 0xAE, 0 },
    { 0xBA, 0xB1, 0 },
};
SupportCondition PARTNER_ABILITY_CONDITIONS[18] = {
    { { 1, 0, 2 }, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 2 }, 0, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 2 }, 0, { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 2 }, 0, { 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 2 }, 0, { 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 0xD }, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 0xD }, 0, { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 0xD }, 0, { 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 3, 0, 0, 0, 0, 0, 4 }, 0, { 0 } },
    { { 1, 0, 0x12 }, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 0x12 }, 0, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 0x12 }, 0, { 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 0x16 }, 0, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3 } },
    { { 1, 0, 0x18 }, 0, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3 } },
    { { 1, 0, 0x1C }, 0, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3 } },
    { { 1, 0, 0x19 }, 0, { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5 } },
    { { 1, 0, 0x1B }, 0, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 3 } },
    { { 1, 0, 0xD }, 0, { 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 5 } },
};
SupportAction PARTNER_ABILITY_ACTIONS[36] = {
    { { 1, 0xA, 0, 0, 0xB }, 0, { 0 } },
    { { 1, 0xA, 0, 0, 0xB }, 0, { 0, 2 } },
    { { 1, 4, 0, 0, 5 }, 0, { 0 } },
    { { 1, 4, 0, 0, 5 }, 0, { 0, 2 } },
    { { 1, 6, 0, 0, 7 }, 0, { 0 } },
    { { 1, 6, 0, 0, 7 }, 0, { 0, 2 } },
    { { 1, 8, 0, 0, 9 }, 0, { 0 } },
    { { 1, 8, 0, 0, 9 }, 0, { 0, 2 } },
    { { 1, 0xA }, 0, { 0 } },
    { { 1, 0xB }, 0, { 0 } },
    { { 1, 0x33 }, 0, { 0 } },
    { { 1, 5 }, 0, { 0 } },
    { { 1, 7 }, 0, { 0 } },
    { { 1, 9 }, 0, { 0 } },
    { { 1, 0x34 }, 0, { 0 } },
    { { 1, 0x31 }, 0, { 0 } },
    { { 1, 0x2E }, 0, { 0 } },
    { { 1, 0x2A }, 0, { 0 } },
    { { 1, 0x25 }, 0, { 0 } },
    { { 1, 0x21 }, 0, { 0 } },
    { { 1, 0x22 }, 0, { 0 } },
    { { 1, 0x2C }, 0, { 0 } },
    { { 1, 0x19, 0, 0, 0, 0, 2 }, 0, { 0 } },
    { { 1, 1, 0, 0, 0, 0, 1 }, 0, { 0 } },
    { { 1, 0, 0, 0, 0, 0, 0x1A }, 0, { 0 } },
    { { 1 }, 0, { 0 } },
    { { 1, 0x11 }, 0, { 0 } },
    { { 1, 0x30 }, 0, { 0 } },
    { { 1, 0x1B }, 0, { 0 } },
    { { 1, 0x11, 0, 0, 0, 0, 0x11 }, 0, { 0 } },
    { { 1, 0xA, 0, 0, 0, 0, 0xB, 0, 0, 0, 2 }, 0, { 0, 3 } },
    { { 1, 2, 0, 0, 3 }, 0, { 0 } },
    { { 1, 0xA, 0, 0, 0, 0, 3 }, 0, { 0 } },
    { { 1, 0x35 }, 0, { 0 } },
    { { 1, 2, 0, 0, 3 }, 0, { 0 } },
    { { 1, 0xA, 0, 0, 0, 0, 0xB, 0, 0, 0, 0x64 }, 0, { 0 } },
};
PartnerAbility PARTNER_ABILITIES[128] = {
    { 0, 0, 0, 0, 0x32, 0 },
    { 0, 0, 0, 0, 0x64, 0 },
    { 0, 0, 0, 0, 0x96, 0 },
    { 0, 0, 0, 0, 0xC8, 0 },
    { 0, 0, 0, 0, 0x12C, 0 },
    { 0, 0, 0, 0, 0x190, 0 },
    { 0, 0, 0, 0, 0x1F4, 0 },
    { 1, 0, 0, 0, 0x32, 0 },
    { 1, 0, 0, 0, 0x64, 0 },
    { 1, 0, 0, 0, 0xC8, 0 },
    { 2, 0, 0, 0, 0x64, 0 },
    { 2, 0, 0, 0, 0x96, 0 },
    { 2, 0, 0, 0, 0xC8, 0 },
    { 2, 0, 0, 0, 0xFA, 0 },
    { 2, 0, 0, 0, 0x12C, 0 },
    { 3, 0, 0, 0, 0x32, 0 },
    { 3, 0, 0, 0, 0x64, 0 },
    { 3, 0, 0, 0, 0x96, 0 },
    { 3, 0, 0, 0, 0xC8, 0 },
    { 3, 0, 0, 0, 0xFA, 0 },
    { 4, 0, 0, 0, 0x32, 0 },
    { 4, 0, 0, 0, 0x64, 0 },
    { 4, 0, 0, 0, 0x96, 0 },
    { 4, 0, 0, 0, 0xC8, 0 },
    { 5, 2, 0, 0, -0x64, 0 },
    { 5, 3, 0, 0, -0x64, 0 },
    { 5, 4, 0, 0, -0x64, 0 },
    { 5, 5, 0, 0, 0, 0 },
    { 5, 6, 0, 0, 0, 0 },
    { 5, 7, 0, 0, 0, 0 },
    { 5, 0xB, 0, 0, -0xC8, 0 },
    { 5, 0xC, 0, 0, -0xC8, 0 },
    { 5, 0xD, 0, 0, -0xC8, 0 },
    { 5, 0xE, 0, 0, -0xC8, 0 },
    { 5, 0xF, 0, 0, -0xC8, 0 },
    { 5, 1, 0, 0, -0xC8, 0 },
    { 5, 0xA, 0, 0, -0x64, 0 },
    { 5, 9, 0, 0, -0xC8, 0 },
    { 6, 0, 0, 0, 0xA, 0 },
    { 6, 0, 0, 0, 0x14, 0 },
    { 6, 0, 0, 0, 0x1E, 0 },
    { 7, 0, 1, 1, 0x32, 1 },
    { 7, 0, 1, 1, 0x64, 1 },
    { 7, 0, 1, 1, 0xC8, 1 },
    { 7, 0, 1, 1, 0x12C, 1 },
    { 7, 0, 2, 1, 2, 1 },
    { 7, 0, 3, 1, 0x12C, 1 },
    { 7, 0, 3, 1, 0x190, 1 },
    { 7, 0, 3, 1, 0x1F4, 1 },
    { 7, 0, 4, 1, 2, 1 },
    { 7, 0, 4, 1, 3, 1 },
    { 7, 0, 5, 1, 0xC8, 1 },
    { 7, 0, 5, 1, 0x12C, 1 },
    { 7, 0, 5, 1, 0x190, 1 },
    { 7, 0, 6, 1, 2, 1 },
    { 7, 0, 6, 1, 3, 1 },
    { 7, 0, 7, 1, 0x64, 1 },
    { 7, 0, 7, 1, 0xC8, 1 },
    { 7, 0, 7, 1, 0x12C, 1 },
    { 7, 0, 8, 1, 2, 1 },
    { 7, 0, 8, 1, 3, 1 },
    { 7, 0, 0x21, 1, 0, 1 },
    { 7, 0, 0x22, 1, 0, 2 },
    { 7, 0, 0xB, 1, 0, 2 },
    { 7, 0, 0xC, 1, 0, 1 },
    { 7, 0, 0xD, 1, 0, 1 },
    { 7, 0, 0xE, 1, 0, 1 },
    { 7, 0xA, 0xF, 1, 0, 2 },
    { 7, 0xB, 0xF, 1, 0, 2 },
    { 7, 0xC, 0xF, 1, 0, 2 },
    { 7, 1, 2, 1, 2, 1 },
    { 7, 1, 2, 1, 3, 1 },
    { 7, 2, 2, 1, 2, 1 },
    { 7, 2, 2, 1, 3, 1 },
    { 7, 3, 2, 1, 2, 1 },
    { 7, 3, 2, 1, 3, 1 },
    { 7, 4, 2, 1, 2, 1 },
    { 7, 4, 2, 1, 3, 1 },
    { 7, 5, 2, 1, 2, 1 },
    { 7, 5, 2, 1, 3, 1 },
    { 7, 0, 0x1A, 1, 0, 2 },
    { 7, 0, 0x1A, 1, 1, 2 },
    { 7, 0, 0x1A, 1, 2, 2 },
    { 7, 0, 0x1A, 1, 3, 2 },
    { 7, 0, 0x1A, 1, 4, 2 },
    { 7, 0, 0x18, 1, 0, 2 },
    { 7, 0, 0x17, 3, 0, 2 },
    { 7, 1, 0xA, 1, 0, 1 },
    { 7, 2, 0xA, 1, 0, 1 },
    { 7, 3, 0xA, 1, 0, 1 },
    { 7, 4, 0xA, 1, 0, 1 },
    { 7, 5, 0xA, 1, 0, 1 },
    { 7, 0, 9, 2, 0, 1 },
    { 7, 6, 1, 1, 0xC8, 1 },
    { 7, 7, 1, 1, 0x12C, 1 },
    { 7, 8, 1, 1, 0x190, 1 },
    { 7, 0, 0x1B, 1, 0, 2 },
    { 7, 0, 0x1B, 1, 1, 2 },
    { 7, 0, 0x1B, 1, 2, 2 },
    { 7, 0, 0x1E, 1, 0, 2 },
    { 7, 0, 0x20, 1, 0xC8, 2 },
    { 7, 0, 0x20, 1, 0x12C, 2 },
    { 7, 0, 0x20, 1, 0x190, 2 },
    { 7, 0, 0x1F, 2, 0x190, 2 },
    { 7, 0, 0x1F, 2, 0x258, 2 },
    { 7, 9, 0x20, 1, 0x1F4, 2 },
    { 7, 9, 0x20, 1, 0x2BC, 2 },
    { 7, 0, 0x1C, 1, 0x12C, 1 },
    { 7, 0, 0x1C, 1, 0x258, 1 },
    { 7, 0, 0x1C, 1, 0x3E8, 1 },
    { 7, 0xD, 0x1D, 1, 1, 2 },
    { 7, 0xD, 0x1D, 1, 2, 2 },
    { 7, 0xE, 0x13, 1, 2, 2 },
    { 7, 0xE, 0x13, 1, 3, 2 },
    { 7, 0xE, 0x13, 1, 4, 2 },
    { 7, 0xF, 0x14, 1, 2, 2 },
    { 7, 0xF, 0x14, 1, 3, 2 },
    { 7, 0x10, 0x15, 1, 1, 2 },
    { 7, 0, 0x16, 1, 0, 3 },
    { 7, 0x11, 0x10, 1, 4, 2 },
    { 7, 0x11, 0x11, 2, 0, 2 },
    { 7, 6, 0x23, 2, 0xC8, 1 },
    { 7, 0x12, 0x23, 2, 0xC8, 1 },
    { 8, 0, 0, 0, 0xA, 0 },
    { 8, 0, 0, 0, 0x14, 0 },
    { 8, 0, 0, 0, 0x1E, 0 },
    { 8, 1, 0, 0, 0xA, 0 },
    { 8, 1, 0, 0, 0x14, 0 },
};

/* CARD2.CDD: this header, then the Digimon, Option and Digivolve cards */
typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ u16 digimonCount;
    /* 0x6 */ u8 optionCount;
    /* 0x7 */ u8 unk7;
} CardDbHeader;

void loadCardDatabase(void) {
    CardDbHeader *file;
    s32 i;
    s32 cardId;

    spawnTask(0, -1, 0, 0x800, loadFileTagged, "B:\\CARD2.CDD", getCurrentTaskId(), -2);
    CARD_DB_FILE = (u8 *)(file = (CardDbHeader *)waitFrames(0x7FFFFFFF));
    DIGIMON_CARDS = (u8 *)(file + 1);
    OPTION_CARDS = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[file->digimonCount];
    DIGIVOLVE_CARDS = (u8 *)&((OptionCardData *)OPTION_CARDS)[file->optionCount];
    cardId = 0;
    for (i = 0; i < 0xBF; i++) {
        ((DigimonCardData *)DIGIMON_CARDS)[i].id = cardId++;
    }
    for (i = 0; i < 0x66; i++) {
        ((OptionCardData *)OPTION_CARDS)[i].id = cardId++;
    }
    for (i = 0; i < 8; i++) {
        ((DigivolveCardData *)DIGIVOLVE_CARDS)[i].id = cardId++;
    }
}

void assignCardCopySerial(s32 player, s32 cardId, s32 copy) {
    s32 serial;
    s32 i;

retry:
    serial = rand();
    for (i = 0; i < copy; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[cardId][i] == serial) {
            goto retry;
        }
    }
    ((PlayerProfile *)PLAYER_PROFILES)[player].cardCopySerials[cardId][copy] = serial;
}

void clearCollectionNewFlags(s32 player) {
    s32 i;

    for (i = 0; i < 0x12D; i++) {
        PLAYER_DATA(player).cardCollection[i] &= 0x7F;
    }
}

void clearCollectionFirstObtainedFlags(s32 player) {
    s32 i;

    for (i = 0; i < 0x12D; i++) {
        PLAYER_DATA(player).cardCollection[i] &= 0xDF;
    }
}

/* returns the copies now owned, or -3 for a partner or armor card, -2 when 6
   copies were already owned, -1 when the count overflowed to 6 */
s32 addCardToCollection(s32 player, s32 cardId, s32 count) {
    s32 copy;

    if (cardId >= 0xAC && cardId <= 0xBE) {
        return -3;
    }
    for (copy = PLAYER_DATA(player).cardCollection[cardId] & 7; copy < 6; copy++) {
        assignCardCopySerial(player, cardId, copy);
    }
    if ((PLAYER_DATA(player).cardCollection[cardId] & 7) == 6) {
        PLAYER_DATA(player).cardCollection[cardId] |= 0x50;
        return -2;
    }
    if ((PLAYER_DATA(player).cardCollection[cardId] & 7) == 0 && !(PLAYER_DATA(player).cardCollection[cardId] & 0x40)) {
        PLAYER_DATA(player).cardCollection[cardId] |= 0x20;
    }
    if ((PLAYER_DATA(player).cardCollection[cardId] & 7) + count >= 7) {
        PLAYER_DATA(player).cardCollection[cardId] &= 0xF8;
        PLAYER_DATA(player).cardCollection[cardId] |= 0x56;
        return -1;
    }
    PLAYER_DATA(player).cardCollection[cardId] += count;
    if ((PLAYER_DATA(player).cardCollection[cardId] & 7) == 6) {
        PLAYER_DATA(player).cardCollection[cardId] |= 0x10;
    }
    if (((DigimonCardData *)getCardData(cardId))->rewardRank == 0) {
        PLAYER_DATA(player).cardCollection[cardId] |= 0x10;
    }
    PLAYER_DATA(player).cardCollection[cardId] |= 0xC0;
    updatePlayerRanks(player);
    return PLAYER_DATA(player).cardCollection[cardId] & 7;
}

s8 removeCardFromCollection(s32 player, s32 cardId, s32 count) {
    if (cardId >= 0xAC && cardId <= 0xBE) {
        return -3;
    }
    if ((PLAYER_DATA(player).cardCollection[cardId] & 7) == 0) {
        return -2;
    }
    if ((PLAYER_DATA(player).cardCollection[cardId] & 7) - count < 0) {
        PLAYER_DATA(player).cardCollection[cardId] &= 0xF8;
        return -1;
    }
    PLAYER_DATA(player).cardCollection[cardId] -= count;
    updatePlayerRanks(player);
    return PLAYER_DATA(player).cardCollection[cardId] & 7;
}

s32 getOwnedCardCount(s32 player, s32 cardId) {
    return PLAYER_DATA(player).cardCollection[cardId] & 7;
}

s32 getCardId(s32 type, s32 index) {
    switch (type) {
    case 0:
        return index;
    case 1:
        return index + 0xBF;
    case 2:
        return index + 0x125;
    }
    return -1;
}

s32 getCardSpecialty(s32 cardId) {
    if (cardId < 0xBF) {
        return ((DigimonCardData *)DIGIMON_CARDS)[cardId].attr >> 4;
    }
    if (cardId < 0x125) {
        return 5;
    }
    return 6;
}

s32 getCardLevel(s32 cardId) {
    if (cardId < 0xBF) {
        return ((DigimonCardData *)DIGIMON_CARDS)[cardId].attr & 0xF;
    }
    if (cardId < 0x125) {
        return 4;
    }
    return 5;
}

void *getCardData(s32 cardId) {
    if (cardId < 0xBF) {
        return &((DigimonCardData *)DIGIMON_CARDS)[cardId];
    }
    if (cardId < 0x125) {
        return &((OptionCardData *)OPTION_CARDS)[cardId - 0xBF];
    }
    return &((DigivolveCardData *)DIGIVOLVE_CARDS)[cardId - 0x125];
}

void markDeckCardsSeen(s32 player) {
    s32 i;

    for (i = 0; i < 30; i++) {
        ((PlayerProfile *)PLAYER_PROFILES)[player].cardCollection[getCardId(((Player *)DUEL_PLAYERS[player])->cards[i].type,
                                                             ((Player *)DUEL_PLAYERS[player])->cards[i].index)] |= 0x40;
    }
}

void markBuildableOpponentDecks(s32 player) {
    u8 needed[0x12D];
    PresetDeck *decks;
    s32 i;
    s32 j;
    s32 missing;

    decks = (PresetDeck *)(((SessionData *)SESSION_DATA)->npcDeckFile + 8);
    for (i = 0; i < 0x9F; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].opponentDeckFlags[i] & 0x8000) {
            for (j = 0; j < 0x12D; j++) {
                needed[j] = 0;
            }
            for (j = 0; j < 30; j++) {
                needed[decks[i].cards[j]]++;
            }
            missing = 0;
            for (j = 0; j < 0x12D; j++) {
                if ((((PlayerProfile *)PLAYER_PROFILES)[player].cardCollection[j] & 7) < needed[j]) {
                    missing = 1;
                    break;
                }
            }
            if (!missing) {
                ((PlayerProfile *)PLAYER_PROFILES)[player].opponentDeckFlags[i] |= 0x4000;
            }
        }
    }
}

/* picks the 3 reward cards: cards whose rewardRank falls in the pack's range
   for their specialty and level, and with the partners' rewardBonus as percent
   chance, one of them from just above that range */
void rollRewardCards(s32 player, s32 pack) {
    DigimonCardData **cards;
    s16 *inRange;
    s16 *nearRange;
    DigimonCardData **data;
    s16 *in;
    s16 *near;
    s32 bonusChance;
    s16 inCount;
    s16 nearCount;
    s32 i;
    s8 upper;
    s8 lower;
    s32 row;

    clearCollectionFirstObtainedFlags(player);
    cards = allocTaskHeapBlock(0x4B4);
    inRange = allocTaskHeapBlock(0x25A);
    nearRange = allocTaskHeapBlock(0x25A);
    bonusChance = 0;
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[i].cardId != 0) {
            bonusChance += PLAYER_DATA(player).partners[i].rewardBonus;
        }
    }
    inCount = 0;
    nearCount = 0;
    in = inRange;
    near = nearRange;
    data = cards;
    for (i = 0; i < 0x12D; i++) {
        *in = -1;
        *near = -1;
        if (i < 0xBF) {
            *data = &((DigimonCardData *)DIGIMON_CARDS)[i];
        } else if (i < 0x125) {
            *data = (DigimonCardData *)&((OptionCardData *)OPTION_CARDS)[i - 0xBF];
        } else {
            *data = (DigimonCardData *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[i - 0x125];
        }
        data++;
        in++;
        near++;
    }
    in = inRange;
    near = nearRange;
    data = cards;
    for (i = 0; i < 0x12D; i++, data++) {
        upper = 0;
        lower = 0;
        switch ((s8)(*data)->type) {
        case 0:
            row = 0;
            switch ((*data)->attr & 0xF) {
            case 0:
                row = 2;
                break;
            case 2:
                row = 1;
                break;
            case 3:
                row = 0;
                break;
            }
            upper = REWARD_CARD_RANGES[pack][(*data)->attr >> 4][row];
            lower = 0;
            if (upper < 0) {
                lower = abs(upper);
                upper = 99;
            }
            break;
        case 1:
        case 2:
            upper = REWARD_CARD_RANGES[pack][5][0];
            lower = REWARD_CARD_RANGES[pack][5][1];
            break;
        }
        if ((*data)->rewardRank != 0) {
            if ((*data)->rewardRank <= upper && (*data)->rewardRank >= lower) {
                *in++ = i;
                inCount++;
            } else if (upper != 0 && (*data)->rewardRank <= upper + REWARD_CARD_RANGES[pack][5][2] && (*data)->rewardRank >= lower) {
                *near++ = i;
                nearCount++;
            }
        }
    }
    if (rand() % 100 < bonusChance) {
        PLAYER_DATA(player).rewardCards[0] = nearRange[rand() % nearCount];
        PLAYER_DATA(player).rewardCards[1] = inRange[rand() % inCount];
        PLAYER_DATA(player).rewardCards[2] = inRange[rand() % inCount];
    } else {
        PLAYER_DATA(player).rewardCards[0] = inRange[rand() % inCount];
        PLAYER_DATA(player).rewardCards[1] = inRange[rand() % inCount];
        PLAYER_DATA(player).rewardCards[2] = inRange[rand() % inCount];
    }
    freeHeapBlock(cards);
    freeHeapBlock(inRange);
    freeHeapBlock(nearRange);
}

void addRewardCardsToCollection(s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        ((PlayerProfile *)PLAYER_PROFILES)[player].rewardResults[i] =
            addCardToCollection(player, ((PlayerProfile *)PLAYER_PROFILES)[player].rewardCards[i], 1);
    }
}

void countSeenCards(s32 player) {
    s32 cardId;

    ((PlayerProfile *)PLAYER_PROFILES)[player].seenCardCount = 0;
    for (cardId = 0; cardId < 0x12D; cardId++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].cardCollection[cardId] & 0x40) {
            ((PlayerProfile *)PLAYER_PROFILES)[player].seenCardCount++;
        }
    }
}

void linkSavedDecks(s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        linkDeckCardData(player, &((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[i]);
    }
}

void linkDeckCardData(s32 player, PlayerDeck *deck) {
    CardSlot *cardSlot;
    s32 i;
    s32 slot;

    if (deck->inUse != 0) {
        cardSlot = deck->cards;
        for (i = 0; i < 30; i++) {
            switch (cardSlot->type) {
            case 0:
                cardSlot->card = (s8 *)&((DigimonCardData *)DIGIMON_CARDS)[cardSlot->index];
                for (slot = 0; slot < 3; slot++) {
                    if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId != 0 &&
                        ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == cardSlot->id) {
                        cardSlot->card = (s8 *)&((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot];
                        break;
                    }
                }
                break;
            case 1:
                cardSlot->card = (s8 *)&((OptionCardData *)OPTION_CARDS)[cardSlot->index];
                break;
            case 2:
                cardSlot->card = (s8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[cardSlot->index];
                break;
            }
            cardSlot++;
        }
    }
}

void setCardSlotFromId(u8 *out, s32 id) {
    s32 type;

    type = 2;
    if (id < 0xBF) {
        type = 0;
    } else {
        id -= 0xBF;
        if (id < 0x66) {
            type = 1;
        } else {
            id -= 0x66;
        }
    }
    ((CardSlot *)out)->type = type;
    ((CardSlot *)out)->index = id;
    ((CardSlot *)out)->id = getCardId(type, id);
}

s32 countDeckCardsByFilter(s32 unused, PlayerDeck *deck, s32 mask) {
    s32 count;
    u8 type;
    s32 i;
    DigimonCardData *card;
    s32 level;
    s32 specialty;

    count = 0;
    for (i = 0; i < 30; i++) {
        type = deck->cards[i].type;
        switch (type) {
        case 0:
            card = &((DigimonCardData *)DIGIMON_CARDS)[deck->cards[i].index];
            level = card->attr;
            level &= 0xF;
            specialty = card->attr >> 4;
            if (mask & 0x1E00) {
                if (mask & 0x1F) {
                    if ((mask >> (level + 9)) & 1) {
                        if (!(mask & 0x20) || (deck->cards[i].index >= 0xAC && deck->cards[i].index < 0xBF)) {
                            count++;
                        }
                    }
                } else if ((mask >> (level + 9)) & 1) {
                    count++;
                }
            } else if ((mask >> specialty) & 1) {
                if (!(mask & 0x20) || (deck->cards[i].index >= 0xAC && deck->cards[i].index < 0xBF)) {
                    count++;
                }
            } else if ((mask & 0x20) && (deck->cards[i].index >= 0xAC && deck->cards[i].index < 0xBF)) {
                count++;
            }
            break;
        case 1:
            if (mask & 0x40) {
                count++;
            }
            break;
        case 2:
            if (mask & 0x80) {
                count++;
            }
            break;
        }
    }
    return count;
}

s32 storeSavedDeck(s32 player, PlayerDeck *src, s32 slot) {
    PlayerDeck *deck;
    s32 i;

    if (slot == -1) {
        for (slot = 0; slot < 3; slot++) {
            if (PLAYER_DATA(player).savedDecks[slot].inUse == 0) {
                break;
            }
        }
        if (slot >= 3) {
            return -1;
        }
    }
    deck = &PLAYER_DATA(player).savedDecks[slot];
    *deck = *src;
    deck->inUse = 1;
    deck->saveCount++;
    if (deck->wins >= 10000) {
        deck->wins = 9999;
    }
    if (deck->losses >= 10000) {
        deck->losses = 9999;
    }
    for (i = 0; i < 30; i++) {
        switch (deck->cards[i].type) {
        case 0:
            deck->cards[i].card = (s8 *)&((DigimonCardData *)DIGIMON_CARDS)[deck->cards[i].index];
            deck->cards[i].id = deck->cards[i].index;
            break;
        case 1:
            deck->cards[i].card = (s8 *)&((OptionCardData *)OPTION_CARDS)[deck->cards[i].index];
            deck->cards[i].id = deck->cards[i].index + 0xBF;
            break;
        case 2:
            deck->cards[i].card = (s8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[deck->cards[i].index];
            deck->cards[i].id = deck->cards[i].index + 0x125;
            break;
        }
    }
    return 0;
}

s32 getSavedDeck(s32 player, PlayerDeck *out, s32 slot) {
    if (((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[slot].inUse == 0) {
        return -1;
    }
    *out = ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[slot];
    return 0;
}

s32 deleteSavedDeck(s32 player, s32 slot) {
    s32 i;

    if (((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[slot].inUse == 0) {
        return -1;
    }
    ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[slot].inUse = 0;
    for (i = 0; i < 2; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[i].inUse == 0) {
            ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[i] = ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[i + 1];
            ((PlayerProfile *)PLAYER_PROFILES)[player].savedDecks[i + 1].inUse = 0;
        }
    }
    return 0;
}

/* the opponent deck whose win record a duel against deckId counts for:
   variant decks count for their base deck */
s32 getBaseDeckId(s32 deckId) {
    s32 baseDeckId;

    baseDeckId = deckId;
    switch (baseDeckId) {
    case 0x75:
    case 0x79:
    case 0x7A:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
        baseDeckId = 0x72;
        break;
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x83:
        baseDeckId = 0x77;
        break;
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:
    case 0x8D:
        baseDeckId = BASE_DECK_IDS[baseDeckId - 0x84];
        break;
    }
    return baseDeckId;
}

void backupPartners(s32 player) {
    s32 slot;

    for (slot = 0; slot < 3; slot++) {
        ((SessionData *)SESSION_DATA)->partnerBackup[player][slot] = ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot];
        ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId = 0;
    }
}

void restorePartners(s32 player) {
    s32 slot;

    for (slot = 0; slot < 3; slot++) {
        ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot] =
            ((SessionData *)SESSION_DATA)->partnerBackup[player][slot];
    }
}

void refreshPartners(s32 player) {
    s32 slot;
    s32 cardId;
    s32 armorCardId;

    for (slot = 0; slot < 3; slot++) {
        cardId = ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId;
        if (cardId != 0) {
            if ((s8)((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].level >= 0x63) {
                ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].level = 0x63;
                ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].exp = getExpForNextLevel(0x62);
            }
            ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].baseCard = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[cardId];
            armorCardId = ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCardId;
            if (armorCardId == 0) {
                ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCard = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[cardId];
            } else {
                ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCard = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[armorCardId];
            }
            updatePartnerStats(player, slot);
        }
    }
}

void addPartner(s32 player, s32 partner, s32 obtain) {
    s32 slot;
    s32 j;
    s32 cardId;

    for (slot = 0; slot < 3; slot++) {
        if (PLAYER_DATA(player).partners[slot].cardId == PARTNER_CARD_IDS[partner]) {
            return;
        }
        if (PLAYER_DATA(player).partners[slot].cardId == 0) {
            PLAYER_DATA(player).partners[slot].baseCard = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[PARTNER_CARD_IDS[partner]];
            PLAYER_DATA(player).partners[slot].armorCard = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[PARTNER_CARD_IDS[partner]];
            PLAYER_DATA(player).partners[slot].cardId = PARTNER_CARD_IDS[partner];
            PLAYER_DATA(player).partners[slot].level = 1;
            PLAYER_DATA(player).partners[slot].exp = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).partners[slot].equippedAbilities[j] = -1;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).partners[slot].unlockedArmors[j] = 0;
            }
            PLAYER_DATA(player).partners[slot].armorCardId = 0;
            PLAYER_DATA(player).partners[slot].expBonus = 0;
            PLAYER_DATA(player).partners[slot].rewardBonus = 0;
            PLAYER_DATA(player).partners[slot].hpBonus = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).partners[slot].attackBonus[j] = 0;
            }
            updatePartnerStats(player, slot);
            if (obtain != 0) {
                cardId = PLAYER_DATA(player).partners[slot].cardId;
                PLAYER_DATA(player).cardCollection[cardId] = 1;
                assignCardCopySerial(player, cardId, 0);
                updatePlayerRanks(player);
                grantPartnerAbility(player, PARTNER_START_ABILITIES[partner]);
                PLAYER_DATA(player).cardCollection[PARTNER_CARD_IDS[partner]] |= 0xF0;
            } else {
                PLAYER_DATA(player).cardCollection[PARTNER_CARD_IDS[partner]] |= 0x50;
            }
            return;
        }
    }
}

void obtainPartner(s32 player, s32 partner) {
    addPartner(player, partner, 1);
}

s32 getPartnerIndex(s32 cardId) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (cardId == PARTNER_CARD_IDS[i]) {
            return i;
        }
    }
    return -1;
}

s32 getSlotPartnerIndex(s32 player, s32 slot) {
    s32 i;

    if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == 0) {
        return -1;
    }
    for (i = 0; i < 6; i++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == PARTNER_CARD_IDS[i]) {
            return i;
        }
    }
    return -1;
}

/* the partner slot holding cardId, 3 for a partner not obtained yet, -1 for
   a card that isn't a partner */
s32 findPartnerSlot(s32 player, s32 cardId) {
    s32 i;
    s32 slot;

    for (i = 0; i < 6; i++) {
        if (cardId == PARTNER_CARD_IDS[i]) {
            for (slot = 0; slot < 3; slot++) {
                if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == cardId) {
                    return slot;
                }
            }
            return 3;
        }
    }
    return -1;
}

void unlockPartnerArmor(s32 player, s32 partner, s32 armor) {
    s32 slot;

    for (slot = 0; slot < 3; slot++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == PARTNER_CARD_IDS[partner]) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].unlockedArmors[armor] != PARTNER_ARMOR_CARD_IDS[partner][armor]) {
                ((PlayerProfile *)PLAYER_PROFILES)[player].cardCollection[PARTNER_ARMOR_CARD_IDS[partner][armor]] |= 0x50;
                ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].unlockedArmors[armor] = PARTNER_ARMOR_CARD_IDS[partner][armor];
                if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCardId == 0) {
                    selectPartnerArmor(player, partner, armor);
                }
            }
            return;
        }
    }
}

s32 countUnlockedPartnerArmors(s32 player, s32 partner) {
    s32 slot;
    s32 armor;
    s32 count;

    for (slot = 0; slot < 3; slot++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == PARTNER_CARD_IDS[partner]) {
            armor = 0;
            count = 0;
            for (; armor < 3; armor++) {
                if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].unlockedArmors[armor] != 0) {
                    count++;
                }
            }
            return count;
        }
    }
    return 0;
}

void selectPartnerArmor(s32 player, s32 partner, s32 armor) {
    s32 slot;
    s32 k;

    if (partner != -1 && PARTNER_ARMOR_CARD_IDS[partner][armor] != 0) {
        for (slot = 0; slot < 3; slot++) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == PARTNER_CARD_IDS[partner]) {
                for (k = 0; k < 3; k++) {
                    if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].unlockedArmors[k] == PARTNER_ARMOR_CARD_IDS[partner][armor]) {
                        ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCardId = PARTNER_ARMOR_CARD_IDS[partner][armor];
                        ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCard = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[PARTNER_ARMOR_CARD_IDS[partner][armor]];
                        updatePartnerStats(player, slot);
                        return;
                    }
                }
            }
        }
    }
}

s32 getSelectedArmorIndex(s32 player, s32 partner) {
    s32 slot;
    s32 armor;

    if (partner == -1) {
        return -1;
    }
    for (slot = 0; slot < 3; slot++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId == PARTNER_CARD_IDS[partner]) {
            if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCardId == 0) {
                return -1;
            }
            for (armor = 0; armor < 3; armor++) {
                if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCardId == PARTNER_ARMOR_CARD_IDS[partner][armor]) {
                    return armor;
                }
            }
        }
    }
    return -1;
}

s32 findArmorPartnerSlot(s32 player, s32 cardId) {
    s32 partner;
    s32 armor;
    s32 slot;

    for (partner = 0; partner < 6; partner++) {
        for (armor = 0; armor < 3; armor++) {
            if (PARTNER_ARMOR_CARD_IDS[partner][armor] != 0 && cardId == PARTNER_ARMOR_CARD_IDS[partner][armor]) {
                for (slot = 0; slot < 3; slot++) {
                    if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].armorCardId == cardId) {
                        return slot;
                    }
                }
                return 3;
            }
        }
    }
    return -1;
}

/* the support effect text of each ability from 0x29 on */
u8 *PARTNER_ABILITY_TEXTS[82] = {
    "Boost Attack\n"
    "Power +50.",
    "Boost Attack\n"
    "Power +100.",
    "Boost Attack\n"
    "Power +200.",
    "Boost Attack\n"
    "Power +300.",
    "Attack Power\n"
    "is Doubled.",
    "Boost *b0 Attack\n"
    "Power +300.",
    "Boost *b0 Attack\n"
    "Power +400.",
    "Boost *b0 Attack\n"
    "Power +500.",
    "*b0 Attack\n"
    "Power is Doubled.",
    "*b0 Attack\n"
    "Power is Tripled.",
    "Boost *b1 Attack\n"
    "Power +200.",
    "Boost *b1 Attack\n"
    "Power +300.",
    "Boost *b1 Attack\n"
    "Power +400.",
    "*b1 Attack\n"
    "Power is Doubled.",
    "*b1 Attack\n"
    "Power is Tripled.",
    "Boost *b2 Attack\n"
    "Power +100.",
    "Boost *b2 Attack\n"
    "Power +200.",
    "Boost *b2 Attack\n"
    "Power +300.",
    "*b2 Attack\n"
    "Power is Doubled.",
    "*b2 Attack\n"
    "Power is Tripled.",
    "Attack Power\n"
    "becomes the same\n"
    "as HP.",
    "Attack becomes\n"
    "\"1st Attack.\"",
    "Attack is\n"
    "\"Eat-up HP.\"",
    "Lower Opponent's\n"
    "*b0 Attack\n"
    "Power to 0.",
    "Lower Opponent's\n"
    "*b1 Attack\n"
    "Power to 0.",
    "Lower Opponent's\n"
    "*b2 Attack\n"
    "Power to 0.",
    "*b0 Counterattack:\n"
    "(Attack Second).",
    "*b1 Counterattack:\n"
    "(Attack Second).",
    "*b2 Counterattack:\n"
    "(Attack Second).",
    "If Opponent's\n"
    "Specialty is *a0,\n"
    "own Attack Power\n"
    "is Doubled.",
    "If Opponent's\n"
    "Specialty is *a0,\n"
    "own Attack power\n"
    "is Tripled.",
    "If Opponent's\n"
    "Specialty is *a1,\n"
    "own Attack power\n"
    "is Doubled.",
    "If Opponent's\n"
    "Specialty is *a1,\n"
    "own Attack power\n"
    "is Tripled.",
    "If Opponent's\n"
    "Specialty is *a2,\n"
    "own Attack power\n"
    "is Doubled.",
    "If Opponent's\n"
    "Specialty is *a2,\n"
    "own Attack power\n"
    "is Tripled.",
    "If Opponent's\n"
    "Specialty is *a3,\n"
    "own Attack power\n"
    "is Doubled.",
    "If Opponent's\n"
    "Specialty is *a3,\n"
    "own Attack power\n"
    "is Tripled.",
    "If Opponent's\n"
    "Specialty is *a4,\n"
    "own Attack power\n"
    "is Doubled.",
    "If Opponent's\n"
    "Specialty is *a4,\n"
    "own Attack power\n"
    "is Tripled.",
    "Change own\n"
    "Speciality to *a0.",
    "Change own\n"
    "Speciality to *a1.",
    "Change own\n"
    "Speciality to *a2.",
    "Change own\n"
    "Speciality to *a3.",
    "Change own\n"
    "Speciality to *a4.",
    "Opponent's\n"
    "Speciality becomes\n"
    "same as Player's.",
    "Swap Specialities\n"
    "with Opponent's.",
    "If Opponent's\n"
    "Specialty is *a0,\n"
    "lower his Attack\n"
    "power to 0.",
    "If Opponent's\n"
    "Specialty is *a1,\n"
    "lower his Attack\n"
    "power to 0.",
    "If Opponent's\n"
    "Specialty is *a2,\n"
    "lower his Attack\n"
    "power to 0.",
    "If Opponent's\n"
    "Specialty is *a3,\n"
    "lower his Attack\n"
    "power to 0.",
    "If Opponent's\n"
    "Specialty is *a4,\n"
    "lower his Attack\n"
    "power to 0.",
    "Lower both\n"
    "Players' Attack\n"
    "Power to 0.",
    "If *e3 Level,\n"
    "boost Attack\n"
    "Power +200.",
    "If *e4 Level,\n"
    "boost Attack\n"
    "Power +300.",
    "If *e5 Level,\n"
    "boost Attack\n"
    "Power +400.",
    "Opponent will use\n"
    "*b0 Button.",
    "Opponent will use\n"
    "*b1 Button.",
    "Opponent will use\n"
    "*b2 Button.",
    "Opponent will use\n"
    "the same Attack.",
    "Recover HP +200.",
    "Recover HP +300.",
    "Recover HP +400.",
    "Attack Power is\n"
    "halved.\n"
    "Recover HP +400.",
    "Attack Power is\n"
    "halved.\n"
    "Recover HP +600.",
    "If HP is less\n"
    "than Opponent's,\n"
    "recover HP +500.",
    "If HP is less\n"
    "than Opponent's,\n"
    "recover HP +700.",
    "If KO'd in\n"
    "a Round, revive\n"
    "with 300 HP.",
    "If KO'd in\n"
    "a Round, revive\n"
    "with 600 HP.",
    "If KO'd in\n"
    "a Round, revive\n"
    "with 1000 HP.",
    "Discard 1 Card\n"
    "from Opponent's\n"
    "Hand at random.",
    "Discard 2 Cards\n"
    "from Opponent's\n"
    "Hand at random.",
    "Discard Opponent's\n"
    "Top 2 DP Cards\n"
    "shown.",
    "Discard Opponent's\n"
    "Top 3 DP Cards\n"
    "shown.",
    "Discard Opponent's\n"
    "Top 4 DP Cards\n"
    "shown.",
    "Discard Top\n"
    "2 Cards\n"
    "from Opponent's\n"
    "Online Deck.",
    "Discard Top\n"
    "3 Cards\n"
    "from Opponent's\n"
    "Online Deck.",
    "Move the Top\n"
    "Card from own\n"
    "Offline Pile to\n"
    "own Offline Deck.",
    "Opponent's Support\n"
    "Effect has\n"
    "been Voided.",
    "Draw until there\n"
    "are 4 Cards.",
    "Draw Partner\n"
    "Card from\n"
    "the Online Deck.\n"
    "Shuffle the Deck.",
    "If *e3 Level,\n"
    "boost HP +200.\n"
    "Boost all Attack\n"
    "Powers +100.",
    "If *ea Level,\n"
    "boost HP +200.\n"
    "Boost all Attack\n"
    "Powers +100.",
};
u8 PARTNER_START_ABILITIES[6] = { 0xA, 0xF, 0, 0x26, 0x64, 0x14 };

s32 updatePartnerStats(s32 player, s32 slot) {
    s32 i;
    s32 j;
    s32 ability;
    s32 n;
    s32 line;
    s32 col;
    s32 ret;
    u8 *supportText;

    PLAYER_DATA(player).partners[slot].expBonus = 0;
    PLAYER_DATA(player).partners[slot].rewardBonus = 0;
    ret = 0;
    PLAYER_DATA(player).partners[slot].card[0] = *(DigimonCardData *)PLAYER_DATA(player).partners[slot].baseCard;
    PLAYER_DATA(player).partners[slot].card[1] = *(DigimonCardData *)PLAYER_DATA(player).partners[slot].armorCard;
    if (PLAYER_DATA(player).partners[slot].card[0].attack[2].power == 0) {
        PLAYER_DATA(player).partners[slot].card[0].attack[2].power = 100;
    }
    if (PLAYER_DATA(player).partners[slot].card[1].attack[2].power == 0) {
        PLAYER_DATA(player).partners[slot].card[1].attack[2].power = 100;
    }
    PLAYER_DATA(player).partners[slot].card[0].hp += PLAYER_DATA(player).partners[slot].hpBonus;
    PLAYER_DATA(player).partners[slot].card[1].hp += PLAYER_DATA(player).partners[slot].hpBonus;
    for (i = 0; i < 3; i++) {
        PLAYER_DATA(player).partners[slot].card[0].attack[i].power += PLAYER_DATA(player).partners[slot].attackBonus[i];
        PLAYER_DATA(player).partners[slot].card[1].attack[i].power += PLAYER_DATA(player).partners[slot].attackBonus[i];
    }
    for (i = 0; i < 3; i++) {
        ability = PLAYER_DATA(player).partners[slot].equippedAbilities[i];
        /* kept on one line: the match depends on it, since GCC 2.8.1's line
           notes decide where slot*83 is computed */
        if (ability == -1) continue;
        switch (PARTNER_ABILITIES[ability].type) {
        case 0:
            PLAYER_DATA(player).partners[slot].card[0].hp += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].hp += PARTNER_ABILITIES[ability].value;
            break;
        case 1:
            PLAYER_DATA(player).partners[slot].card[0].attack[0].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[0].attack[1].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[0].attack[2].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].attack[0].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].attack[1].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].attack[2].power += PARTNER_ABILITIES[ability].value;
            break;
        case 2:
            PLAYER_DATA(player).partners[slot].card[0].attack[0].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].attack[0].power += PARTNER_ABILITIES[ability].value;
            break;
        case 3:
            PLAYER_DATA(player).partners[slot].card[0].attack[1].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].attack[1].power += PARTNER_ABILITIES[ability].value;
            break;
        case 4:
            PLAYER_DATA(player).partners[slot].card[0].attack[2].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].attack[2].power += PARTNER_ABILITIES[ability].value;
            break;
        case 5:
            PLAYER_DATA(player).partners[slot].card[0].crossEffect = PARTNER_ABILITIES[ability].param;
            PLAYER_DATA(player).partners[slot].card[1].crossEffect = PARTNER_ABILITIES[ability].param;
            if (PARTNER_ABILITIES[ability].value != 0) {
                PLAYER_DATA(player).partners[slot].card[0].attack[2].power += PARTNER_ABILITIES[ability].value;
                PLAYER_DATA(player).partners[slot].card[1].attack[2].power += PARTNER_ABILITIES[ability].value;
            } else {
                PLAYER_DATA(player).partners[slot].card[0].attack[2].power = 0;
                PLAYER_DATA(player).partners[slot].card[1].attack[2].power = 0;
            }
            break;
        case 6:
            PLAYER_DATA(player).partners[slot].card[0].dpBonus += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).partners[slot].card[1].dpBonus += PARTNER_ABILITIES[ability].value;
            break;
        case 7:
            for (j = 0; j < 2; j++) {
                PLAYER_DATA(player).partners[slot].card[0].supportConditions[j].unk0[0] = 0;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).partners[slot].card[0].supportActions[j].unk0[0] = 0;
            }
            for (j = 0; j < 4; j++) {
                for (n = 0; n < 0x15; n++) {
                    PLAYER_DATA(player).partners[slot].card[0].supportText[j][n] = 0;
                    PLAYER_DATA(player).partners[slot].card[1].supportText[j][n] = 0;
                }
            }
            if (PARTNER_ABILITIES[ability].param != 0) {
                PLAYER_DATA(player).partners[slot].card[0].supportConditions[0] = PARTNER_ABILITY_CONDITIONS[PARTNER_ABILITIES[ability].param - 1];
                PLAYER_DATA(player).partners[slot].card[0].supportConditions[0].value = PARTNER_ABILITIES[ability].value;
            }
            if (PARTNER_ABILITIES[ability].actionStart != 0) {
                for (j = 0; j < PARTNER_ABILITIES[ability].actionCount; j++) {
                    PLAYER_DATA(player).partners[slot].card[0].supportActions[j] = PARTNER_ABILITY_ACTIONS[PARTNER_ABILITIES[ability].actionStart - 1 + j];
                    PLAYER_DATA(player).partners[slot].card[0].supportActions[j].value = PARTNER_ABILITIES[ability].value;
                }
            }
            supportText = PARTNER_ABILITY_TEXTS[ability - 0x29];
            line = 0;
            col = 0;
            while (*supportText != 0) {
                if (*supportText == '\n') {
                    line++;
                    col = 0;
                } else {
                    PLAYER_DATA(player).partners[slot].card[0].supportText[line][col] = *supportText;
                    PLAYER_DATA(player).partners[slot].card[1].supportText[line][col] = *supportText;
                    col++;
                }
                supportText++;
            }
            PLAYER_DATA(player).partners[slot].card[0].supportIcon = PARTNER_ABILITIES[ability].supportIcon;
            PLAYER_DATA(player).partners[slot].card[1].supportIcon = PARTNER_ABILITIES[ability].supportIcon;
            ret = 1;
            break;
        case 8:
            switch (PARTNER_ABILITIES[ability].param) {
            case 0:
                PLAYER_DATA(player).partners[slot].expBonus += PARTNER_ABILITIES[ability].value;
                break;
            case 1:
                PLAYER_DATA(player).partners[slot].rewardBonus += PARTNER_ABILITIES[ability].value;
                break;
            }
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).partners[slot].card[0].attack[i].power < 0) {
            PLAYER_DATA(player).partners[slot].card[0].attack[i].power = 0;
        }
        if (PLAYER_DATA(player).partners[slot].card[1].attack[i].power < 0) {
            PLAYER_DATA(player).partners[slot].card[1].attack[i].power = 0;
        }
    }
    if (((u8)(PLAYER_DATA(player).partners[slot].card[0].crossEffect - 5) < 4) | ((u8)(PLAYER_DATA(player).partners[slot].card[1].crossEffect - 5) < 4)) {
        if ((u8)(PLAYER_DATA(player).partners[slot].card[0].crossEffect - 5) < 4) {
            PLAYER_DATA(player).partners[slot].card[0].attack[2].power = 0;
        }
        if ((u8)(PLAYER_DATA(player).partners[slot].card[1].crossEffect - 5) < 4) {
            PLAYER_DATA(player).partners[slot].card[1].attack[2].power = 0;
        }
    }
    return ret;
}

void equipPartnerAbility(s32 player, s32 slot, s32 abilitySlot, s32 ability) {
    if (getPartnerAbilityState(player, ability) == 1) {
        ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].equippedAbilities[abilitySlot] = ability;
        updatePartnerStats(player, slot);
    }
}

void unequipPartnerAbility(s32 player, s32 slot, s32 abilitySlot) {
    ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].equippedAbilities[abilitySlot] = -1;
    updatePartnerStats(player, slot);
}

void grantPartnerAbility(s32 player, s32 ability) {
    ((PlayerProfile *)PLAYER_PROFILES)[player].ownedAbilities[ability / 8] |= 1 << (ability % 8);
}

s32 canEquipPartnerAbility(s32 player, s32 slot, s32 skipSlot, s32 ability) {
    s32 ok;
    s32 i;
    s32 equipped;

    ok = 1;
    for (i = 0; i < 3; i++) {
        if (skipSlot == i) {
            continue;
        }
        equipped = ((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].equippedAbilities[i];
        if (equipped == -1) {
            continue;
        }
        if (PARTNER_ABILITIES[ability].type == 1) {
            if (PARTNER_ABILITIES[equipped].type >= 1 && PARTNER_ABILITIES[equipped].type <= 4) {
                ok = 0;
            }
        } else if (PARTNER_ABILITIES[equipped].type == 1) {
            if (PARTNER_ABILITIES[ability].type >= 1 && PARTNER_ABILITIES[ability].type <= 4) {
                ok = 0;
            }
        } else if (PARTNER_ABILITIES[ability].type == PARTNER_ABILITIES[equipped].type) {
            ok = 0;
        }
    }
    return ok;
}

/* 0: not owned, 1: owned, 2: equipped on a partner */
s32 getPartnerAbilityState(s32 player, s32 ability) {
    s32 slot;
    s32 abilitySlot;

    if (ability < 0) {
        return 0;
    }
    if (!((((PlayerProfile *)PLAYER_PROFILES)[player].ownedAbilities[ability / 8] >> (ability % 8)) & 1)) {
        return 0;
    }
    for (slot = 0; slot < 3; slot++) {
        if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].cardId != 0) {
            for (abilitySlot = 0; abilitySlot < 3; abilitySlot++) {
                if (((PlayerProfile *)PLAYER_PROFILES)[player].partners[slot].equippedAbilities[abilitySlot] == ability) {
                    return 2;
                }
            }
        }
    }
    return 1;
}
