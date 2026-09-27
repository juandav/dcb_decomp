#include "dcb/player_rank.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"

void updatePlayerRanks(s32 player) {
    s32 specialtyCounts[6];
    s32 ownedCards;
    s32 rank;
    s32 i;
    s32 completedSets;

    rank = PLAYER_DATA(player).rankA;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(player).unk18 < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(player).unk18 < 25) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(player).unk18 < 50) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(player).unk18 < 100) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(player).unk18 < 200) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(player).unk18 < 300) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(player).unk18 < 500) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(player).rankA = rank;

    ownedCards = 0;
    for (i = 0; i < 6; i++) {
        specialtyCounts[i] = 0;
    }
    for (i = 0; i < 0xAC; i++) {
        completedSets = PLAYER_DATA(player).unk14B2[i] & 7;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[DIGIMON_CARDS[i * 0x13C + 0x1A] >> 4]++;
        }
    }
    for (i = 0xBF; i < 0x125; i++) {
        completedSets = PLAYER_DATA(player).unk14B2[i] & 7;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[5]++;
        }
    }
    for (i = 0x125; i < 0x12D; i++) {
        completedSets = PLAYER_DATA(player).unk14B2[i] & 7;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[5]++;
        }
    }
    completedSets = 0;
    for (i = 0; i < 6; i++) {
        if (specialtyCounts[i] == COMPLETE_SET_CARD_COUNTS[i]) {
            completedSets++;
        }
    }

    rank = PLAYER_DATA(player).rankB;
    switch (rank) {
    case 0:
        if (ownedCards < 100) {
            break;
        }
        rank = 1;
    case 1:
        if (ownedCards < 200) {
            break;
        }
        rank = 2;
    case 2:
        if (completedSets <= 0) {
            break;
        }
        rank = 3;
    case 3:
        if (completedSets < 3) {
            break;
        }
        rank = 4;
    case 4:
        if (completedSets < 5) {
            break;
        }
        rank = 5;
    case 5:
        if (completedSets < 6) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(player).unk28_11) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(player).rankB = rank;

    rank = PLAYER_DATA(player).rankC;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(player).unk1C < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(player).unk1C < 20) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(player).unk1C < 30) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(player).unk1C < 40) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(player).unk1C < 60) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(player).unk1C < 80) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(player).unk1C < 100) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(player).rankC = rank;
}
