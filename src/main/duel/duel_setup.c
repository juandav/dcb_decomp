#include "dcb/duel_setup.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/overlay_calls.h"

void loadPresetDeckForPlayer(s32 player) {
    PresetDeck *presetDecks;
    PresetDeck *deck;
    s32 i;
    s32 partner;
    s32 cardId;

    presetDecks = (PresetDeck *)(((SessionData *)SESSION_DATA)->npcDeckFile + 8);
    if (((SessionData *)SESSION_DATA)->npcDeckIndex[player] != -1) {
        deck = &presetDecks[((SessionData *)SESSION_DATA)->npcDeckIndex[player]];
        backupPartners(player);
        strcpy(PLAYER(player)->deckName, deck->name);
        for (i = 0; i < 30; i++) {
            cardId = deck->cards[i];
            setCardSlotFromId((u8 *)&PLAYER(player)->cards[i], cardId);
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
    s32 cardId;

    for (i = 0; i < 2; i++) {
        DUEL_PLAYERS[i] = allocTaskHeapBlock(0x1E4); /* a Player */
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
        PLAYER(i)->battleCard = 0;
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
        strcpy(PLAYER(0)->name, PLAYER_DATA(0).name);
        strcpy(PLAYER(1)->name, ((SessionData *)SESSION_DATA)->opponentDeck.ownerName);
        if (((SessionData *)SESSION_DATA)->opponentDeckIndex == 0) {
            KAW_startTutorial();
            for (i = 0; i < 2; i++) {
                linkDeckCardData(i, DUEL_PLAYERS[i]);
            }
        } else {
            loadPresetDeckForPlayer(0);
            for (i = 0; i < 3; i++) {
                PLAYER_DATA(1).partners[i].cardId = 0;
            }
            PLAYER(1)->cpuPlaceStyle = ((SessionData *)SESSION_DATA)->opponentDeck.cpuStyle[0];
            PLAYER(1)->cpuAttackStyle = ((SessionData *)SESSION_DATA)->opponentDeck.cpuStyle[1];
            PLAYER(1)->cpuRedrawStyle = ((SessionData *)SESSION_DATA)->opponentDeck.cpuStyle[2];
            PLAYER(1)->cpuSupportStyle = ((SessionData *)SESSION_DATA)->opponentDeck.cpuStyle[3];
            strcpy(PLAYER(1)->deckName, ((SessionData *)SESSION_DATA)->opponentDeck.name);
            for (i = 0; i < 30; i++) {
                cardId = ((SessionData *)SESSION_DATA)->opponentDeck.cards[i];
                setCardSlotFromId((u8 *)&PLAYER(1)->cards[i], cardId);
                partner = getPartnerIndex(cardId);
                if (partner >= 0) {
                    addPartner(1, partner, 0);
                    if (((SessionData *)SESSION_DATA)->opponentDeck.partnerArmor != 0) {
                        unlockPartnerArmor(1, partner, ((SessionData *)SESSION_DATA)->opponentDeck.partnerArmor - 1);
                    }
                }
            }
            linkDeckCardData(1, DUEL_PLAYERS[1]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            strcpy(PLAYER(i)->name, PLAYER_DATA(i).name);
        }
    }
}
