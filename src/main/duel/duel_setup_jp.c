#include "dcb/duel_setup.h"
#include "common.h"
#include "game.h"
#include "dcb/heap.h"

/* jp's duel setup (duel_setup.c is us's and eu's): each player takes a
   saved deck from its profile (the CPU its deck from the session), and the
   tutorial sets itself up in KAWSEG */

void linkSavedDecks(s32 player);
void KAW_startTutorial();

void initDuelPlayers(s32 isCpuDuel) {
    s32 i;
    s32 j;

    if (isCpuDuel != 0 && ((SessionData *)SESSION_DATA)->tutorial != 0) {
        DUEL->tutorial = 1;
    }
    for (i = 0; i < 2; i++) {
        DUEL_PLAYERS[i] = allocTaskHeapBlock(sizeof(Player));
        strcpy(PLAYER(i)->name, PLAYER_DATA(i).name);
        PLAYER(i)->controller = i;
        PLAYER(i)->deck = &PLAYER_DATA(i).savedDecks[((SessionData *)SESSION_DATA)->deckChoices[i]];
        for (j = 0; j < 30; j++) {
            PLAYER(i)->onlineDeck[j] = i * 30 + j;
            PLAYER(i)->offlineDeck[j] = -1;
        }
        for (j = 0; j < 4; j++) {
            PLAYER(i)->hand[j] = -1;
        }
        for (j = 0; j < DP_SLOT_COUNT; j++) {
            PLAYER(i)->dpSlots[j] = -1;
        }
        for (j = 0; j < 3; j++) {
            PLAYER(i)->digimonStack[j] = -1;
        }
        PLAYER(i)->dpGainTimer = 0;
        PLAYER(i)->playedCard = -1;
        PLAYER(i)->wins = 0;
        PLAYER(i)->battleCard = 0;
        for (j = 0; j < 5; j++) {
            PLAYER(i)->stats[j] = 0;
            PLAYER(i)->displayedStats[j] = 0;
        }
        linkSavedDecks(i);
    }
    if (isCpuDuel != 0) {
        PLAYER(1)->controller = 2;
        strcpy(PLAYER(1)->name, ((SessionData *)SESSION_DATA)->opponentName);
        if (DUEL->tutorial == 0) {
            PLAYER(0)->deck = &PLAYER_DATA(0).savedDecks[((SessionData *)SESSION_DATA)->deckChoice];
            PLAYER(1)->deck = &((SessionData *)SESSION_DATA)->opponentDeck;
            PLAYER(1)->cpuPlaceStyle = ((SessionData *)SESSION_DATA)->cpuStyle[0];
            PLAYER(1)->cpuAttackStyle = ((SessionData *)SESSION_DATA)->cpuStyle[1];
            PLAYER(1)->cpuRedrawStyle = ((SessionData *)SESSION_DATA)->cpuStyle[2];
            PLAYER(1)->cpuSupportStyle = ((SessionData *)SESSION_DATA)->cpuStyle[3];
        } else {
            KAW_startTutorial();
        }
    }
}
