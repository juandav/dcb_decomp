#include "dcb/duel_setup.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"

void loadPresetDeckForPlayer(s32 player) {
    PresetDeck *presetDecks;
    PresetDeck *deck;
    s32 i;
    s32 partner;
    u16 cardId;

    presetDecks = (PresetDeck *)(((SessionData *)D_8006E054)->npcDeckFile + 8);
    if (((SessionData *)D_8006E054)->npcDeckIndex[player] != -1) {
        deck = &presetDecks[((SessionData *)D_8006E054)->npcDeckIndex[player]];
        backupPartners(player);
        strcpy(DUEL_PLAYERS[player] + 1, deck->name);
        for (i = 0; i < 30; i++) {
            cardId = deck->cards[i];
            setCardSlotFromId(DUEL_PLAYERS[player] + 0x14 + i * 8, cardId);
            partner = getPartnerIndex(cardId);
            if (partner >= 0) {
                addPartner(player, partner, 0);
                if (deck->partnerArmor != 0) {
                    unlockPartnerArmor(player, partner, deck->partnerArmor - 1);
                }
            }
        }
        linkDeckCardData(player, (PlayerDeck *)DUEL_PLAYERS[player]);
    }
}

void initDuelPlayers(s32 isCpuDuel) {
    s32 i;
    s32 j;
    s32 partner;
    u16 cardId;

    for (i = 0; i < 2; i++) {
        DUEL_PLAYERS[i] = allocTaskHeapBlock(0x1E4);
        PLAYER(i)->controller = (1 - isCpuDuel) * 2 + i;
        PLAYER(i)->unk0 = 1;
        for (j = 0; j < 30; j++) {
            PLAYER(i)->cards[j].id = 0;
            PLAYER(i)->cards[j].type = 0;
            PLAYER(i)->cards[j].index = 0;
            PLAYER(i)->onlineDeck[j] = i * 30 + j;
            PLAYER(i)->offlineDeck[j] = -1;
        }
        for (j = 0; j < 4; j++) {
            PLAYER(i)->hand[j] = -1;
        }
        for (j = 0; j < 8; j++) {
            PLAYER(i)->dpSlots[j] = -1;
        }
        for (j = 0; j < 3; j++) {
            PLAYER(i)->digimonStack[j] = -1;
        }
        PLAYER(i)->playedCard = -1;
        PLAYER(i)->wins = 0;
        *(s32 *)(DUEL_PLAYERS[i] + 0x114) = 0;
        for (j = 0; j < 5; j++) {
            PLAYER(i)->stats[j] = 0;
            PLAYER(i)->displayedStats[j] = 0;
            PLAYER(i)->statPopups[j].value = 0;
            PLAYER(i)->statPopups[j].type = 0;
            PLAYER(i)->statPopups[j].timer = 0;
            PLAYER(i)->statPopups[j].x = 0;
            PLAYER(i)->statPopups[j].y = 0;
        }
    }
    if (isCpuDuel != 0) {
        strcpy((char *)DUEL_PLAYERS[0] + 0x1CE, (char *)PLAYER_PROFILES);
        strcpy((char *)DUEL_PLAYERS[1] + 0x1CE, (char *)D_8006E054 + 0x57);
        if (((SessionData *)D_8006E054)->opponentDeckIndex == 0) {
            func_801EA708();
            for (i = 0; i < 2; i++) {
                linkDeckCardData(i, DUEL_PLAYERS[i]);
            }
        } else {
            loadPresetDeckForPlayer(0);
            for (i = 0; i < 3; i++) {
                PLAYER_DATA(1).partners[i].cardId = 0;
            }
            PLAYER(1)->unk178_22 = ((SessionData *)D_8006E054)->opponentDeck.unk64[0];
            PLAYER(1)->unk178_24 = ((SessionData *)D_8006E054)->opponentDeck.unk64[1];
            PLAYER(1)->unk178_26 = ((SessionData *)D_8006E054)->opponentDeck.unk64[2];
            PLAYER(1)->unk178_28 = ((SessionData *)D_8006E054)->opponentDeck.unk64[3];
            strcpy((char *)DUEL_PLAYERS[1] + 1, ((SessionData *)D_8006E054)->opponentDeck.name);
            for (i = 0; i < 30; i++) {
                cardId = ((SessionData *)D_8006E054)->opponentDeck.cards[i];
                setCardSlotFromId(DUEL_PLAYERS[1] + 0x14 + i * 8, cardId);
                partner = getPartnerIndex(cardId);
                if (partner >= 0) {
                    addPartner(1, partner, 0);
                    if (((SessionData *)D_8006E054)->opponentDeck.partnerArmor != 0) {
                        unlockPartnerArmor(1, partner, ((SessionData *)D_8006E054)->opponentDeck.partnerArmor - 1);
                    }
                }
            }
            linkDeckCardData(1, DUEL_PLAYERS[1]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            strcpy((char *)DUEL_PLAYERS[i] + 0x1CE, PLAYER_DATA(i).name);
        }
    }
}
