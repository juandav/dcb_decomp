#include "common.h"
#include "game.h"
#include "dcb/card_db.h"
#include "dcb/loader.h"
#include "dcb/task.h"
#include "dcb/player_rank.h"

/* jp's card_db (card_db.c is us's and eu's): the card files and the card
   collection, which jp keeps for the first player only and by card type */

typedef struct {
    /* 0x0 */ u8 unk0[4];
    /* 0x4 */ s16 digimonCount;
    /* 0x6 */ s8 optionCount;
    /* 0x7 */ u8 unk7;
} CardDbHeader;

void loadCardDatabase(void) {
    CardDbHeader *file;

    spawnTask(0, -1, 0, 0x800, loadFileTagged, "B:\\card.cdd", getCurrentTaskId(), -2);
    CARD_DB_FILE = (u8 *)(file = (CardDbHeader *)waitFrames(0x7FFFFFFF));
    DIGIMON_CARDS = (u8 *)(file + 1);
    OPTION_CARDS = (u8 *)&((DigimonCardData *)DIGIMON_CARDS)[file->digimonCount];
    DIGIVOLVE_CARDS = (u8 *)&((OptionCardData *)OPTION_CARDS)[file->optionCount];
}

#define SAVED_DECK(p, i) (((PlayerProfile *)PLAYER_PROFILES)[p].savedDecks[i])

/* points each card of PLAYER's saved decks at its data */
void linkSavedDecks(s32 player) {
    s32 i;
    s32 j;

    for (i = 0; i < 3; i++) {
        if (SAVED_DECK(player, i).inUse != 0) {
            for (j = 0; j < 30; j++) {
                switch (SAVED_DECK(player, i).cards[j].type) {
                case 0:
                    SAVED_DECK(player, i).cards[j].card =
                        (s8 *)&((DigimonCardData *)DIGIMON_CARDS)[SAVED_DECK(player, i).cards[j].index];
                    break;
                case 1:
                    SAVED_DECK(player, i).cards[j].card =
                        (s8 *)&((OptionCardData *)OPTION_CARDS)[SAVED_DECK(player, i).cards[j].index];
                    break;
                case 2:
                    SAVED_DECK(player, i).cards[j].card =
                        (s8 *)&((DigivolveCardData *)DIGIVOLVE_CARDS)[SAVED_DECK(player, i).cards[j].index];
                    break;
                }
            }
        }
    }
}

/* SERIALS indexed flat, eight copies per card */
void assignCardCopySerial(s32 cardId, s32 copy, u16 (*serials)[8]) {
    s32 serial;
    s32 i;

retry:
    serial = rand();
    for (i = 0; i < copy; i++) {
        if ((*serials)[cardId * 8 + i] == serial) {
            goto retry;
        }
    }
    (*serials)[cardId * 8 + copy] = serial;
}

/* returns the copies now owned, -3 for no such card, -2 when 8 copies were
   already owned, -1 when the count overflowed to 8 */
s32 addCardToCollection(s32 type, s32 index, s32 count) {
    s32 owned;
    s32 copy;

    switch (type) {
    case 0:
        if (index >= 0x6E) {
            break;
        }
        if ((PLAYER_DATA(0).cardCollection[index] & 0xF) == 8) {
            return -2;
        }
        if ((PLAYER_DATA(0).cardCollection[index] & 0xF) + count >= 9) {
            PLAYER_DATA(0).cardCollection[index] = 8;
            return -1;
        }
        owned = PLAYER_DATA(0).cardCollection[index] & 0xF;
        for (copy = owned; copy < owned + count; copy++) {
            assignCardCopySerial(index, copy, PLAYER_DATA(0).cardSerials);
        }
        PLAYER_DATA(0).cardCollection[index] += count;
        PLAYER_DATA(0).cardCollection[index] |= 0x80;
        updatePlayerRanks(0);
        return PLAYER_DATA(0).cardCollection[index] & 0xF;
    case 1:
        if (index >= 0x2B) {
            break;
        }
        if ((PLAYER_DATA(0).optionCollection[index] & 0xF) == 8) {
            return -2;
        }
        if ((PLAYER_DATA(0).optionCollection[index] & 0xF) + count >= 9) {
            PLAYER_DATA(0).optionCollection[index] = 8;
            return -1;
        }
        owned = PLAYER_DATA(0).optionCollection[index] & 0xF;
        for (copy = owned; copy < owned + count; copy++) {
            assignCardCopySerial(index, copy, PLAYER_DATA(0).optionSerials);
        }
        PLAYER_DATA(0).optionCollection[index] += count;
        PLAYER_DATA(0).optionCollection[index] |= 0x80;
        updatePlayerRanks(0);
        return PLAYER_DATA(0).optionCollection[index] & 0xF;
    case 2:
        if (index >= 6) {
            break;
        }
        if ((PLAYER_DATA(0).digivolveCollection[index] & 0xF) == 8) {
            return -2;
        }
        if ((PLAYER_DATA(0).digivolveCollection[index] & 0xF) + count >= 9) {
            PLAYER_DATA(0).digivolveCollection[index] = 8;
            return -1;
        }
        owned = PLAYER_DATA(0).digivolveCollection[index] & 0xF;
        for (copy = owned; copy < owned + count; copy++) {
            assignCardCopySerial(index, copy, PLAYER_DATA(0).digivolveSerials);
        }
        PLAYER_DATA(0).digivolveCollection[index] += count;
        PLAYER_DATA(0).digivolveCollection[index] |= 0x80;
        updatePlayerRanks(0);
        return PLAYER_DATA(0).digivolveCollection[index] & 0xF;
    }
    return -3;
}

/* returns the copies left, -3 for no such card, -2 when none was owned, -1
   when fewer than COUNT were */
s32 removeCardFromCollection(s32 type, s32 index, s32 count) {
    switch (type) {
    case 0:
        if (index >= 0x6E) {
            break;
        }
        if ((PLAYER_DATA(0).cardCollection[index] & 0xF) == 0) {
            return -2;
        }
        if ((PLAYER_DATA(0).cardCollection[index] & 0xF) - count < 0) {
            return -1;
        }
        PLAYER_DATA(0).cardCollection[index] -= count;
        updatePlayerRanks(0);
        return PLAYER_DATA(0).cardCollection[index] & 0xF;
    case 1:
        if (index >= 0x2B) {
            break;
        }
        if ((PLAYER_DATA(0).optionCollection[index] & 0xF) == 0) {
            return -2;
        }
        if ((PLAYER_DATA(0).optionCollection[index] & 0xF) - count < 0) {
            return -1;
        }
        PLAYER_DATA(0).optionCollection[index] -= count;
        updatePlayerRanks(0);
        return PLAYER_DATA(0).optionCollection[index] & 0xF;
    case 2:
        if (index >= 6) {
            break;
        }
        if ((PLAYER_DATA(0).digivolveCollection[index] & 0xF) == 0) {
            return -2;
        }
        if ((PLAYER_DATA(0).digivolveCollection[index] & 0xF) - count < 0) {
            return -1;
        }
        PLAYER_DATA(0).digivolveCollection[index] -= count;
        updatePlayerRanks(0);
        return PLAYER_DATA(0).digivolveCollection[index] & 0xF;
    }
    return -3;
}

s32 getOwnedCardCount(s32 type, s32 index) {
    switch (type) {
    case 0:
        if (index < 0x6E) {
            return PLAYER_DATA(0).cardCollection[index] & 0xF;
        }
        break;
    case 1:
        if (index < 0x2B) {
            return PLAYER_DATA(0).optionCollection[index] & 0xF;
        }
        break;
    case 2:
        if (index < 6) {
            return PLAYER_DATA(0).digivolveCollection[index] & 0xF;
        }
        break;
    default:
        return 0;
    }
    return 0;
}
