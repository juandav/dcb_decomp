#include "dcb/duel_setup.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/duel_rules.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/main.h"

void loadPresetDeckForPlayer(s32 player) {
    SavedDeck *presetDecks;
    SavedDeck *deck;
    s32 i;
    s32 partner;
    u16 cardId;

    presetDecks = (SavedDeck *)(((Unk8006E054 *)D_8006E054)->unk0 + 8);
    if (((Unk8006E054 *)D_8006E054)->unk1008[player] != -1) {
        deck = &presetDecks[((Unk8006E054 *)D_8006E054)->unk1008[player]];
        backupPartners(player);
        strcpy(DUEL_PLAYERS[player] + 1, deck->name);
        for (i = 0; i < 30; i++) {
            cardId = deck->cards[i];
            setCardSlotFromId(DUEL_PLAYERS[player] + 0x14 + i * 8, cardId);
            partner = getPartnerIndex(cardId);
            if (partner >= 0) {
                addPartner(player, partner, 0);
                if (deck->unk6D != 0) {
                    unlockPartnerArmor(player, partner, deck->unk6D - 1);
                }
            }
        }
        linkDeckCardData(player, (Unk110 *)DUEL_PLAYERS[player]);
    }
}

void initDuelPlayers(s32 isCpuDuel) {
    s32 i;
    s32 j;
    s32 partner;
    u16 cardId;

    for (i = 0; i < 2; i++) {
        DUEL_PLAYERS[i] = allocTaskHeapBlock(0x1E4);
        PLAYER(i)->unk178_17 = (1 - isCpuDuel) * 2 + i;
        PLAYER(i)->unk0[0] = 1;
        for (j = 0; j < 30; j++) {
            PLAYER(i)->cards[j].id = 0;
            PLAYER(i)->cards[j].state = 0;
            PLAYER(i)->cards[j].unk1 = 0;
            PLAYER(i)->unk17D[j] = i * 30 + j;
            PLAYER(i)->unk19B[j] = -1;
        }
        for (j = 0; j < 4; j++) {
            PLAYER(i)->unk1B9[j] = -1;
        }
        for (j = 0; j < 8; j++) {
            PLAYER(i)->unk1C2[j] = -1;
        }
        for (j = 0; j < 3; j++) {
            PLAYER(i)->unk1CA[j] = -1;
        }
        PLAYER(i)->unk1CD = -1;
        PLAYER(i)->unk17C = 0;
        *(s32 *)(DUEL_PLAYERS[i] + 0x114) = 0;
        for (j = 0; j < 5; j++) {
            PLAYER(i)->unk11C[j] = 0;
            PLAYER(i)->unk126[j] = 0;
            PLAYER(i)->unk130[j].value = 0;
            PLAYER(i)->unk130[j].type = 0;
            PLAYER(i)->unk130[j].timer = 0;
            PLAYER(i)->unk130[j].x = 0;
            PLAYER(i)->unk130[j].y = 0;
        }
    }
    if (isCpuDuel != 0) {
        strcpy((char *)DUEL_PLAYERS[0] + 0x1CE, (char *)PLAYER_PROFILES);
        strcpy((char *)DUEL_PLAYERS[1] + 0x1CE, (char *)D_8006E054 + 0x57);
        if (((Unk8006E054 *)D_8006E054)->unk4 == 0) {
            func_801EA708();
            for (i = 0; i < 2; i++) {
                linkDeckCardData(i, DUEL_PLAYERS[i]);
            }
        } else {
            loadPresetDeckForPlayer(0);
            for (i = 0; i < 3; i++) {
                PLAYER_DATA(1).unk80[i].unk288 = 0;
            }
            PLAYER(1)->unk178_22 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[0];
            PLAYER(1)->unk178_24 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[1];
            PLAYER(1)->unk178_26 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[2];
            PLAYER(1)->unk178_28 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[3];
            strcpy((char *)DUEL_PLAYERS[1] + 1, ((Unk8006E054 *)D_8006E054)->unk8.name);
            for (i = 0; i < 30; i++) {
                cardId = ((Unk8006E054 *)D_8006E054)->unk8.cards[i];
                setCardSlotFromId(DUEL_PLAYERS[1] + 0x14 + i * 8, cardId);
                partner = getPartnerIndex(cardId);
                if (partner >= 0) {
                    addPartner(1, partner, 0);
                    if (((Unk8006E054 *)D_8006E054)->unk8.unk6D != 0) {
                        unlockPartnerArmor(1, partner, ((Unk8006E054 *)D_8006E054)->unk8.unk6D - 1);
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
