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

/* jp keeps the player profiles, the rank titles and the complete set counts
   here (us and eu have them in memcard.c) */
#if VERSION_JP
s32 PLAYER_PROFILES = 0;
void *SESSION_DATA = 0;

/* the rank titles, lowest first */
char *STR_TAMER_RANKS[8] = {
    "かけだしテイマー", "一人前テイマー", "中級テイマー", "上級テイマー",
    "名人テイマー", "達人テイマー", "天才テイマー", "超絶偉大無敵テイマー",
};
char *STR_COLLECTOR_RANKS[8] = {
    "一般ピープル", "趣味コレクター", "こだわりコレクター", "一流コレクター",
    "有名コレクター", "天下一コレクター", "完全無欠コレクター", "超伝説究極コレクター",
};
char *STR_BATTLE_RANKS[8] = {
    "対戦ビギナー", "対戦エキスパート", "対戦スペシャリスト", "対戦マスター",
    "対戦チャンピオン", "対戦キング", "対戦エンペラー", "対戦の神様",
};
u8 COMPLETE_SET_CARD_COUNTS[6] = { 0x15, 0x16, 0x1C, 0x16, 0x11, 0x31 };
#elif VERSION_US || VERSION_EU
#else
#error "main/card/player_rank: version not checked"
#endif

void updatePlayerRanks(s32 player) {
    s32 specialtyCounts[6];
    s32 ownedCards;
    s32 rank;
    s32 i;
    s32 completedSets;

#if VERSION_JP
    /* jp's tamer ranks take fewer wins */
    rank = PLAYER_DATA(player).tamerRank;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(player).battleWins < 6) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(player).battleWins < 15) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(player).battleWins < 30) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(player).battleWins < 50) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(player).battleWins < 80) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(player).battleWins < 120) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(player).battleWins < 200) {
            break;
        }
        rank = 7;
    }
#elif VERSION_US || VERSION_EU
    rank = PLAYER_DATA(player).tamerRank;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(player).battleWins < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(player).battleWins < 25) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(player).battleWins < 50) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(player).battleWins < 100) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(player).battleWins < 200) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(player).battleWins < 300) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(player).battleWins < 500) {
            break;
        }
        rank = 7;
    }
#else
#error "main/card/player_rank: version not checked"
#endif
    PLAYER_DATA(player).tamerRank = rank;

    ownedCards = 0;
    for (i = 0; i < 6; i++) {
        specialtyCounts[i] = 0;
    }
#if VERSION_JP
    /* jp has fewer cards, in three arrays, and counts up to 15 copies of each */
    for (i = 0; i < 0x6E; i++) {
        completedSets = PLAYER_DATA(player).cardCollection[i] & 0xF;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[DIGIMON_CARDS[i * 0x122 + 0x14] >> 4]++;
        }
    }
    for (i = 0; i < 0x2B; i++) {
        completedSets = PLAYER_DATA(player).optionCollection[i] & 0xF;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[5]++;
        }
    }
    for (i = 0; i < 6; i++) {
        completedSets = PLAYER_DATA(player).digivolveCollection[i] & 0xF;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[5]++;
        }
    }
#elif VERSION_US || VERSION_EU
    for (i = 0; i < 0xAC; i++) {
        completedSets = PLAYER_DATA(player).cardCollection[i] & 7;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[DIGIMON_CARDS[i * 0x13C + 0x1A] >> 4]++;
        }
    }
    for (i = 0xBF; i < 0x125; i++) {
        completedSets = PLAYER_DATA(player).cardCollection[i] & 7;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[5]++;
        }
    }
    for (i = 0x125; i < 0x12D; i++) {
        completedSets = PLAYER_DATA(player).cardCollection[i] & 7;
        if (completedSets != 0) {
            ownedCards += completedSets;
            specialtyCounts[5]++;
        }
    }
#else
#error "main/card/player_rank: version not checked"
#endif
    completedSets = 0;
    for (i = 0; i < 6; i++) {
        if (specialtyCounts[i] == COMPLETE_SET_CARD_COUNTS[i]) {
            completedSets++;
        }
    }

    rank = PLAYER_DATA(player).collectorRank;
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
        if (PLAYER_DATA(player).hasTraded) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(player).collectorRank = rank;

    rank = PLAYER_DATA(player).battleRank;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(player).versusWins < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(player).versusWins < 20) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(player).versusWins < 30) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(player).versusWins < 40) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(player).versusWins < 60) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(player).versusWins < 80) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(player).versusWins < 100) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(player).battleRank = rank;
#if VERSION_JP
    /* jp opens the card trades as soon as the player has a collector rank */
    if (PLAYER_DATA(player).collectorRank != 0) {
        PLAYER_DATA(player).tradeUnlocked = 1;
    }
#elif VERSION_US || VERSION_EU
    /* us and eu leave that to an area script */
#else
#error "main/card/player_rank: version not checked"
#endif
}
