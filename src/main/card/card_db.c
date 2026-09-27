#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/memcard.h"
#include "dcb/shell.h"

void loadCardDatabase(void) {
    u8 *file;
    s32 i;
    s32 cardId;

    func_800149B8(0, -1, 0, 0x800, func_8001B248, "B:\\CARD2.CDD", func_800148B0(), -2);
    CARD_DB_FILE = file = (u8 *)func_80014C08(0x7FFFFFFF);
    DIGIMON_CARDS = file + 8;
    OPTION_CARDS = DIGIMON_CARDS + *(u16 *)(file + 4) * 0x13C;
    DIGIVOLVE_CARDS = OPTION_CARDS + file[6] * 0xE2;
    cardId = 0;
    for (i = 0; i < 0xBF; i++) {
        ((CardInfo *)DIGIMON_CARDS)[i].id = cardId++;
    }
    for (i = 0; i < 0x66; i++) {
        ((Unk801D8400 *)OPTION_CARDS)[i].id = cardId++;
    }
    for (i = 0; i < 8; i++) {
        ((Unk801D8404 *)DIGIVOLVE_CARDS)[i].id = cardId++;
    }
}

void func_80045968(s32 player, s32 cardId, s32 copy) {
    s32 serial;
    s32 i;

retry:
    serial = rand();
    for (i = 0; i < copy; i++) {
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk15E0[cardId][i] == serial) {
            goto retry;
        }
    }
    ((Unk8006E050 *)PLAYER_PROFILES)[player].unk15E0[cardId][copy] = serial;
}

void clearCollectionNewFlags(s32 player) {
    s32 collection;
    s32 i;
    s32 *base;
    u8 *entry;

    i = 0;
    do {
        base = &PLAYER_PROFILES;
        collection = player * 0x2774 + *base + 0x14B2;
        entry = (u8 *)(collection + i);
        *entry &= 0x7F;
        i++;
    } while (i < 0x12D);
}

void clearCollectionFirstObtainedFlags(s32 player) {
    s32 collection;
    s32 i;
    s32 *base;
    u8 *entry;

    i = 0;
    do {
        base = &PLAYER_PROFILES;
        collection = player * 0x2774 + *base + 0x14B2;
        entry = (u8 *)(collection + i);
        *entry &= 0xDF;
        i++;
    } while (i < 0x12D);
}

s8 addCardToCollection(s32 player, s32 cardId, s32 count) {
    s32 copy;

    if (cardId >= 0xAC && cardId <= 0xBE) {
        return -3;
    }
    for (copy = PLAYER_DATA(player).unk14B2[cardId] & 7; copy < 6; copy++) {
        func_80045968(player, cardId, copy);
    }
    if ((PLAYER_DATA(player).unk14B2[cardId] & 7) == 6) {
        PLAYER_DATA(player).unk14B2[cardId] |= 0x50;
        return -2;
    }
    if ((PLAYER_DATA(player).unk14B2[cardId] & 7) == 0 && !(PLAYER_DATA(player).unk14B2[cardId] & 0x40)) {
        PLAYER_DATA(player).unk14B2[cardId] |= 0x20;
    }
    if ((PLAYER_DATA(player).unk14B2[cardId] & 7) + count >= 7) {
        PLAYER_DATA(player).unk14B2[cardId] &= 0xF8;
        PLAYER_DATA(player).unk14B2[cardId] |= 0x56;
        return -1;
    }
    PLAYER_DATA(player).unk14B2[cardId] += count;
    if ((PLAYER_DATA(player).unk14B2[cardId] & 7) == 6) {
        PLAYER_DATA(player).unk14B2[cardId] |= 0x10;
    }
    if (((u8 *)getCardData(cardId))[0x19] == 0) {
        PLAYER_DATA(player).unk14B2[cardId] |= 0x10;
    }
    PLAYER_DATA(player).unk14B2[cardId] |= 0xC0;
    func_8002CC44(player);
    return PLAYER_DATA(player).unk14B2[cardId] & 7;
}

s8 removeCardFromCollection(s32 player, s32 cardId, s32 count) {
    if (cardId >= 0xAC && cardId <= 0xBE) {
        return -3;
    }
    if ((PLAYER_DATA(player).unk14B2[cardId] & 7) == 0) {
        return -2;
    }
    if ((PLAYER_DATA(player).unk14B2[cardId] & 7) - count < 0) {
        PLAYER_DATA(player).unk14B2[cardId] &= 0xF8;
        return -1;
    }
    PLAYER_DATA(player).unk14B2[cardId] -= count;
    func_8002CC44(player);
    return PLAYER_DATA(player).unk14B2[cardId] & 7;
}

s32 getOwnedCardCount(s32 player, s32 cardId) {
    return (*(u8 *)((s8 *)(((player * 0x2774) + PLAYER_PROFILES + cardId)) + 0x14B2)) & 7;
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
        return DIGIMON_CARDS[cardId * 0x13C + 0x1A] >> 4;
    }
    if (cardId < 0x125) {
        return 5;
    }
    return 6;
}

s32 getCardLevel(s32 cardId) {
    if (cardId < 0xBF) {
        return DIGIMON_CARDS[cardId * 0x13C + 0x1A] & 0xF;
    }
    if (cardId < 0x125) {
        return 4;
    }
    return 5;
}

void *getCardData(s32 cardId) {
    if (cardId < 0xBF) {
        return DIGIMON_CARDS + cardId * 0x13C;
    }
    if (cardId < 0x125) {
        return OPTION_CARDS + (cardId * 0xE2 - 0xA89E);
    }
    return DIGIVOLVE_CARDS + (cardId * 0x70 - 0x8030);
}

void markDeckCardsSeen(s32 player) {
    s32 i;

    for (i = 0; i < 30; i++) {
        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk14B2[getCardId(((Player *)D_801D8348[player])->cards[i].state,
                                                             ((Player *)D_801D8348[player])->cards[i].unk1)] |= 0x40;
    }
}

void markBuildableOpponentDecks(s32 player) {
    u8 needed[0x12D];
    SavedDeck *decks;
    s32 i;
    s32 j;
    s32 missing;

    decks = (SavedDeck *)(((Unk8006E054 *)D_8006E054)->unk0 + 8);
    for (i = 0; i < 0x9F; i++) {
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unkAC0[i] & 0x8000) {
            for (j = 0; j < 0x12D; j++) {
                needed[j] = 0;
            }
            for (j = 0; j < 30; j++) {
                needed[decks[i].cards[j]]++;
            }
            missing = 0;
            for (j = 0; j < 0x12D; j++) {
                if ((((Unk8006E050 *)PLAYER_PROFILES)[player].unk14B2[j] & 7) < needed[j]) {
                    missing = 1;
                    break;
                }
            }
            if (!missing) {
                ((Unk8006E050 *)PLAYER_PROFILES)[player].unkAC0[i] |= 0x4000;
            }
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/card/card_db", rollRewardCards);

void addRewardCardsToCollection(s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk276E[i] =
            addCardToCollection(player, ((Unk8006E050 *)PLAYER_PROFILES)[player].unk2768[i], 1);
    }
}

void countSeenCards(s32 player) {
    s32 cardId;

    ((Unk8006E050 *)PLAYER_PROFILES)[player].unk12 = 0;
    for (cardId = 0; cardId < 0x12D; cardId++) {
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk14B2[cardId] & 0x40) {
            ((Unk8006E050 *)PLAYER_PROFILES)[player].unk12++;
        }
    }
}

void linkSavedDecks(s32 player) {
    s32 i;

    for (i = 0; i < 3; i++) {
        linkDeckCardData(player, &((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[i]);
    }
}

void linkDeckCardData(s32 player, Unk110 *deck) {
    CardSlot *cardSlot;
    s32 i;
    s32 slot;

    if (deck->unk0 != 0) {
        cardSlot = deck->cards;
        for (i = 0; i < 30; i++) {
            switch (cardSlot->state) {
            case 0:
                cardSlot->card = (s8 *)(DIGIMON_CARDS + cardSlot->unk1 * 0x13C);
                for (slot = 0; slot < 3; slot++) {
                    if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 != 0 &&
                        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == cardSlot->id) {
                        cardSlot->card = (s8 *)&((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot];
                        break;
                    }
                }
                break;
            case 1:
                cardSlot->card = (s8 *)(OPTION_CARDS + cardSlot->unk1 * 0xE2);
                break;
            case 2:
                cardSlot->card = (s8 *)(DIGIVOLVE_CARDS + cardSlot->unk1 * 0x70);
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
    out[0] = type;
    out[1] = id;
    *(s16 *)(out + 2) = getCardId(type, id);
}

s32 countDeckCardsByFilter(s32 unused, Unk110 *deck, s32 mask) {
    s32 count;
    s32 i;
    CardInfo *card;
    s32 level;
    s32 specialty;

    count = 0;
    for (i = 0; i < 30; i++) {
        switch (deck->cards[i].state) {
        case 0:
            card = (CardInfo *)(DIGIMON_CARDS + deck->cards[i].unk1 * 0x13C);
            level = card->attr & 0xF;
            specialty = card->attr >> 4;
            if (mask & 0x1E00) {
                if (mask & 0x1F) {
                    if ((mask >> (level + 9)) & 1) {
                        if (!(mask & 0x20) || (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                            count++;
                        }
                    }
                } else if ((mask >> (level + 9)) & 1) {
                    count++;
                }
            } else if ((mask >> specialty) & 1) {
                if (!(mask & 0x20) || (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                    count++;
                }
            } else if ((mask & 0x20) && (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
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

s32 storeSavedDeck(s32 player, Unk110 *src, s32 slot) {
    Unk110 *deck;
    s32 i;

    if (slot == -1) {
        for (slot = 0; slot < 3; slot++) {
            if (PLAYER_DATA(player).unk2438[slot].unk0 == 0) {
                break;
            }
        }
        if (slot >= 3) {
            return -1;
        }
    }
    deck = &PLAYER_DATA(player).unk2438[slot];
    *deck = *src;
    deck->unk0 = 1;
    deck->unk108[0]++;
    if (deck->unk108[1] >= 10000) {
        deck->unk108[1] = 9999;
    }
    if (deck->unk108[2] >= 10000) {
        deck->unk108[2] = 9999;
    }
    for (i = 0; i < 30; i++) {
        switch (deck->cards[i].state) {
        case 0:
            deck->cards[i].card = (s8 *)(DIGIMON_CARDS + deck->cards[i].unk1 * 0x13C);
            deck->cards[i].id = deck->cards[i].unk1;
            break;
        case 1:
            deck->cards[i].card = (s8 *)(OPTION_CARDS + deck->cards[i].unk1 * 0xE2);
            deck->cards[i].id = deck->cards[i].unk1 + 0xBF;
            break;
        case 2:
            deck->cards[i].card = (s8 *)(DIGIVOLVE_CARDS + deck->cards[i].unk1 * 0x70);
            deck->cards[i].id = deck->cards[i].unk1 + 0x125;
            break;
        }
    }
    return 0;
}

s32 getSavedDeck(s32 player, Unk110 *out, s32 slot) {
    if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[slot].unk0 == 0) {
        return -1;
    }
    *out = ((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[slot];
    return 0;
}

s32 deleteSavedDeck(s32 player, s32 slot) {
    s32 i;

    if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[slot].unk0 == 0) {
        return -1;
    }
    ((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[slot].unk0 = 0;
    for (i = 0; i < 2; i++) {
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[i].unk0 == 0) {
            ((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[i] = ((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[i + 1];
            ((Unk8006E050 *)PLAYER_PROFILES)[player].unk2438[i + 1].unk0 = 0;
        }
    }
    return 0;
}

s32 func_800471F4(s32 deckId) {
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
        baseDeckId = D_8006E50C[baseDeckId - 0x84];
        break;
    }
    return baseDeckId;
}

void backupPartners(s32 player) {
    s32 slot;

    for (slot = 0; slot < 3; slot++) {
        ((Unk8006E054 *)D_8006E054)->unk78[player][slot] = ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot];
        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 = 0;
    }
}

void restorePartners(s32 player) {
    s32 slot;

    for (slot = 0; slot < 3; slot++) {
        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot] =
            ((Unk8006E054 *)D_8006E054)->unk78[player][slot];
    }
}

void refreshPartners(s32 player) {
    s32 slot;
    u8 cardId;
    u8 armorCardId;

    for (slot = 0; slot < 3; slot++) {
        cardId = ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288;
        if (cardId != 0) {
            if ((s8)((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk289 >= 0x63) {
                ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk289 = 0x63;
                ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28A = func_80049934(0x62);
            }
            ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk278 = DIGIMON_CARDS + cardId * 0x13C;
            armorCardId = ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk292[0];
            if (armorCardId == 0) {
                ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk27C = DIGIMON_CARDS + cardId * 0x13C;
            } else {
                ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk27C = DIGIMON_CARDS + armorCardId * 0x13C;
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
        if (PLAYER_DATA(player).unk80[slot].unk288 == PARTNER_CARD_IDS[partner]) {
            return;
        }
        if (PLAYER_DATA(player).unk80[slot].unk288 == 0) {
            PLAYER_DATA(player).unk80[slot].unk278 = DIGIMON_CARDS + PARTNER_CARD_IDS[partner] * 0x13C;
            PLAYER_DATA(player).unk80[slot].unk27C = DIGIMON_CARDS + PARTNER_CARD_IDS[partner] * 0x13C;
            PLAYER_DATA(player).unk80[slot].unk288 = PARTNER_CARD_IDS[partner];
            PLAYER_DATA(player).unk80[slot].unk289 = 1;
            PLAYER_DATA(player).unk80[slot].unk28A = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).unk80[slot].unk28C[j] = -1;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).unk80[slot].unk28F[j] = 0;
            }
            PLAYER_DATA(player).unk80[slot].unk292[0] = 0;
            PLAYER_DATA(player).unk80[slot].unk292[1] = 0;
            PLAYER_DATA(player).unk80[slot].unk292[2] = 0;
            PLAYER_DATA(player).unk80[slot].unk280 = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).unk80[slot].unk282[j] = 0;
            }
            updatePartnerStats(player, slot);
            if (obtain != 0) {
                cardId = PLAYER_DATA(player).unk80[slot].unk288;
                PLAYER_DATA(player).unk14B2[cardId] = 1;
                func_80045968(player, cardId, 0);
                func_8002CC44(player);
                grantPartnerAbility(player, PARTNER_START_ABILITIES[partner]);
                PLAYER_DATA(player).unk14B2[PARTNER_CARD_IDS[partner]] |= 0xF0;
            } else {
                PLAYER_DATA(player).unk14B2[PARTNER_CARD_IDS[partner]] |= 0x50;
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

    if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == 0) {
        return -1;
    }
    for (i = 0; i < 6; i++) {
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == PARTNER_CARD_IDS[i]) {
            return i;
        }
    }
    return -1;
}

s32 findPartnerSlot(s32 player, s32 cardId) {
    s32 i;
    s32 slot;

    for (i = 0; i < 6; i++) {
        if (cardId == PARTNER_CARD_IDS[i]) {
            for (slot = 0; slot < 3; slot++) {
                if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == cardId) {
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
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == PARTNER_CARD_IDS[partner]) {
            if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28F[armor] != PARTNER_ARMOR_CARD_IDS[partner][armor]) {
                ((Unk8006E050 *)PLAYER_PROFILES)[player].unk14B2[PARTNER_ARMOR_CARD_IDS[partner][armor]] |= 0x50;
                ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28F[armor] = PARTNER_ARMOR_CARD_IDS[partner][armor];
                if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk292[0] == 0) {
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
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == PARTNER_CARD_IDS[partner]) {
            armor = 0;
            count = 0;
            for (; armor < 3; armor++) {
                if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28F[armor] != 0) {
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
            if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == PARTNER_CARD_IDS[partner]) {
                for (k = 0; k < 3; k++) {
                    if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28F[k] == PARTNER_ARMOR_CARD_IDS[partner][armor]) {
                        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk292[0] = PARTNER_ARMOR_CARD_IDS[partner][armor];
                        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk27C = DIGIMON_CARDS + PARTNER_ARMOR_CARD_IDS[partner][armor] * 0x13C;
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
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 == PARTNER_CARD_IDS[partner]) {
            if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk292[0] == 0) {
                return -1;
            }
            for (armor = 0; armor < 3; armor++) {
                if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk292[0] == PARTNER_ARMOR_CARD_IDS[partner][armor]) {
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
                    if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk292[0] == cardId) {
                        return slot;
                    }
                }
                return 3;
            }
        }
    }
    return -1;
}

INCLUDE_RODATA("asm/main/nonmatchings/card/card_db", D_800119CC);

s32 updatePartnerStats(s32 player, s32 slot) {
    s32 i;
    s32 j;
    s32 ability;
    s32 n;
    s32 line;
    s32 col;
    s32 ret;
    u8 *supportText;

    PLAYER_DATA(player).unk80[slot].unk292[1] = 0;
    PLAYER_DATA(player).unk80[slot].unk292[2] = 0;
    ret = 0;
    PLAYER_DATA(player).unk80[slot].card[0] = *(CardInfo *)PLAYER_DATA(player).unk80[slot].unk278;
    PLAYER_DATA(player).unk80[slot].card[1] = *(CardInfo *)PLAYER_DATA(player).unk80[slot].unk27C;
    if (PLAYER_DATA(player).unk80[slot].card[0].attack[2].power == 0) {
        PLAYER_DATA(player).unk80[slot].card[0].attack[2].power = 100;
    }
    if (PLAYER_DATA(player).unk80[slot].card[1].attack[2].power == 0) {
        PLAYER_DATA(player).unk80[slot].card[1].attack[2].power = 100;
    }
    PLAYER_DATA(player).unk80[slot].card[0].hp += PLAYER_DATA(player).unk80[slot].unk280;
    PLAYER_DATA(player).unk80[slot].card[1].hp += PLAYER_DATA(player).unk80[slot].unk280;
    for (i = 0; i < 3; i++) {
        PLAYER_DATA(player).unk80[slot].card[0].attack[i].power += PLAYER_DATA(player).unk80[slot].unk282[i];
        PLAYER_DATA(player).unk80[slot].card[1].attack[i].power += PLAYER_DATA(player).unk80[slot].unk282[i];
    }
    for (i = 0; i < 3; i++) {
        ability = PLAYER_DATA(player).unk80[slot].unk28C[i];
        if (ability == -1) {
            continue;
        }
        switch (PARTNER_ABILITIES[ability].type) {
        case 0:
            PLAYER_DATA(player).unk80[slot].card[0].hp += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].hp += PARTNER_ABILITIES[ability].value;
            break;
        case 1:
            PLAYER_DATA(player).unk80[slot].card[0].attack[0].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[0].attack[1].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[0].attack[2].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].attack[0].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].attack[1].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].attack[2].power += PARTNER_ABILITIES[ability].value;
            break;
        case 2:
            PLAYER_DATA(player).unk80[slot].card[0].attack[0].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].attack[0].power += PARTNER_ABILITIES[ability].value;
            break;
        case 3:
            PLAYER_DATA(player).unk80[slot].card[0].attack[1].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].attack[1].power += PARTNER_ABILITIES[ability].value;
            break;
        case 4:
            PLAYER_DATA(player).unk80[slot].card[0].attack[2].power += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].attack[2].power += PARTNER_ABILITIES[ability].value;
            break;
        case 5:
            PLAYER_DATA(player).unk80[slot].card[0].unkE4 = PARTNER_ABILITIES[ability].unk1;
            PLAYER_DATA(player).unk80[slot].card[1].unkE4 = PARTNER_ABILITIES[ability].unk1;
            if (PARTNER_ABILITIES[ability].value != 0) {
                PLAYER_DATA(player).unk80[slot].card[0].attack[2].power += PARTNER_ABILITIES[ability].value;
                PLAYER_DATA(player).unk80[slot].card[1].attack[2].power += PARTNER_ABILITIES[ability].value;
            } else {
                PLAYER_DATA(player).unk80[slot].card[0].attack[2].power = 0;
                PLAYER_DATA(player).unk80[slot].card[1].attack[2].power = 0;
            }
            break;
        case 6:
            PLAYER_DATA(player).unk80[slot].card[0].level += PARTNER_ABILITIES[ability].value;
            PLAYER_DATA(player).unk80[slot].card[1].level += PARTNER_ABILITIES[ability].value;
            break;
        case 7:
            for (j = 0; j < 2; j++) {
                PLAYER_DATA(player).unk80[slot].card[0].unk74[j].unk0[0] = 0;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(player).unk80[slot].card[0].unkB4[j].unk0[0] = 0;
            }
            for (j = 0; j < 4; j++) {
                for (n = 0; n < 0x15; n++) {
                    PLAYER_DATA(player).unk80[slot].card[0].text[j][n] = 0;
                    PLAYER_DATA(player).unk80[slot].card[1].text[j][n] = 0;
                }
            }
            j = PARTNER_ABILITIES[ability].unk1;
            if (j != 0) {
                PLAYER_DATA(player).unk80[slot].card[0].unk74[0] = PARTNER_ABILITY_CONDITIONS[j - 1];
                PLAYER_DATA(player).unk80[slot].card[0].unk74[0].unkE = PARTNER_ABILITIES[ability].value;
            }
            if (PARTNER_ABILITIES[ability].unk2 != 0) {
                for (j = 0; j < PARTNER_ABILITIES[ability].unk3; j++) {
                    PLAYER_DATA(player).unk80[slot].card[0].unkB4[j] = PARTNER_ABILITY_ACTIONS[PARTNER_ABILITIES[ability].unk2 - 1 + j];
                    PLAYER_DATA(player).unk80[slot].card[0].unkB4[j].unkC = PARTNER_ABILITIES[ability].value;
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
                    PLAYER_DATA(player).unk80[slot].card[0].text[line][col] = *supportText;
                    PLAYER_DATA(player).unk80[slot].card[1].text[line][col] = *supportText;
                    col++;
                }
                supportText++;
            }
            PLAYER_DATA(player).unk80[slot].card[0].unkE6 = PARTNER_ABILITIES[ability].unk6;
            PLAYER_DATA(player).unk80[slot].card[1].unkE6 = PARTNER_ABILITIES[ability].unk6;
            ret = 1;
            break;
        case 8:
            switch (PARTNER_ABILITIES[ability].unk1) {
            case 0:
                PLAYER_DATA(player).unk80[slot].unk292[1] += PARTNER_ABILITIES[ability].value;
                break;
            case 1:
                PLAYER_DATA(player).unk80[slot].unk292[2] += PARTNER_ABILITIES[ability].value;
                break;
            }
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(player).unk80[slot].card[0].attack[i].power < 0) {
            PLAYER_DATA(player).unk80[slot].card[0].attack[i].power = 0;
        }
        if (PLAYER_DATA(player).unk80[slot].card[1].attack[i].power < 0) {
            PLAYER_DATA(player).unk80[slot].card[1].attack[i].power = 0;
        }
    }
    if (((u8)(PLAYER_DATA(player).unk80[slot].card[0].unkE4 - 5) < 4) | ((u8)(PLAYER_DATA(player).unk80[slot].card[1].unkE4 - 5) < 4)) {
        if ((u8)(PLAYER_DATA(player).unk80[slot].card[0].unkE4 - 5) < 4) {
            PLAYER_DATA(player).unk80[slot].card[0].attack[2].power = 0;
        }
        if ((u8)(PLAYER_DATA(player).unk80[slot].card[1].unkE4 - 5) < 4) {
            PLAYER_DATA(player).unk80[slot].card[1].attack[2].power = 0;
        }
    }
    return ret;
}

void equipPartnerAbility(s32 player, s32 slot, s32 abilitySlot, s32 ability) {
    if (getPartnerAbilityState(player, ability) == 1) {
        ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28C[abilitySlot] = ability;
        updatePartnerStats(player, slot);
    }
}

void unequipPartnerAbility(s32 player, s32 slot, s32 abilitySlot) {
    ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28C[abilitySlot] = -1;
    updatePartnerStats(player, slot);
}

void grantPartnerAbility(s32 player, s32 ability) {
    ((Unk8006E050 *)PLAYER_PROFILES)[player].unk3C[ability / 8] |= 1 << (ability % 8);
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
        equipped = ((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28C[i];
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

s32 getPartnerAbilityState(s32 player, s32 ability) {
    s32 slot;
    s32 abilitySlot;

    if (ability < 0) {
        return 0;
    }
    if (!((((Unk8006E050 *)PLAYER_PROFILES)[player].unk3C[ability / 8] >> (ability % 8)) & 1)) {
        return 0;
    }
    for (slot = 0; slot < 3; slot++) {
        if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk288 != 0) {
            for (abilitySlot = 0; abilitySlot < 3; abilitySlot++) {
                if (((Unk8006E050 *)PLAYER_PROFILES)[player].unk80[slot].unk28C[abilitySlot] == ability) {
                    return 2;
                }
            }
        }
    }
    return 1;
}
