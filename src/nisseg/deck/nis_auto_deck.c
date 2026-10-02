#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/nisseg.h"

/* The deck screens' card lists and the auto deck: counting the cards a
   player owns or has in a deck, building sorted lists of them, and
   filling a deck with 30 cards of a main element and a style */

/* the copies of each card the auto deck can still use */
extern u8 *NIS_DIGIMON_LEFT;
extern u8 *NIS_OPTIONS_LEFT;
extern u8 *NIS_OTHERS_LEFT;

#if JP_DEBUG_BUILD
/* the debug build names what it allocates */
#define allocTaskHeapBlock(size) allocNamedTaskHeapBlock(size, NIS_STR_DECK_EDIT, 7)
#endif

void NIS_fixDeckCards(s8 deck);
void NIS_dropDeckCopiesFromList(s32 deck, NisCardList *list, s32 count);

/* the option cards of an attack deck (0) and of a defense deck (1) */
s8 NIS_OPTION_STYLES[48] = {
    0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0,
    1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1,
    1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 0, -1, 0, 0,
};

void NIS_closeDeckWindow(NisWindow *window) {
}

s32 NIS_getCardId(s32 type, s32 index) {
    s32 id = 0;

    switch (type) {
    case 2:
        id += NIS_OPTION_COUNT;
    case 1:
        id += NIS_DIGIMON_COUNT;
    case 0:
        id += index;
    }
    return id;
}

s32 NIS_countOwnedDigimon(void) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        count += NIS_PROFILE(0)->digimonCards[i] & 0xF;
    }
    return count;
}

s32 NIS_countOwnedOptions(void) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        count += NIS_PROFILE(0)->optionCards[i] & 0xF;
    }
    return count;
}

s32 NIS_countOwnedOthers(void) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        count += NIS_PROFILE(0)->otherCards[i] & 0xF;
    }
    return count;
}

s32 NIS_countOwnedCards(void) {
    s32 digimon = NIS_countOwnedDigimon();
    s32 options = NIS_countOwnedOptions();

    return digimon + options + NIS_countOwnedOthers();
}

/* the Digimon cards owned of an element and a level (-1: any) */
s32 NIS_countOwnedDigimonOf(s32 element, s32 level) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        if ((NIS_PROFILE(0)->digimonCards[i] & 0xF) && (element < 0 || NIS_CARD_ELEMENT(i) == element) && (level < 0 || NIS_CARD_LEVEL(i) == level)) {
            count += NIS_PROFILE(0)->digimonCards[i] & 0xF;
        }
    }
    return count;
}

s32 NIS_countDeckDigimonOf(s32 deck, s32 element, s32 level) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if (NIS_PROFILE(0)->savedDecks[deck].cards[i].type == 0 && (element < 0 || NIS_CARD_ELEMENT(NIS_PROFILE(0)->savedDecks[deck].cards[i].index) == element) && (level < 0 || NIS_CARD_LEVEL(NIS_PROFILE(0)->savedDecks[deck].cards[i].index) == level)) {
            count++;
        }
    }
    return count;
}

s32 NIS_countDeckOptions(s32 deck) {
    s32 count = 0;
    s32 i;

    for (i = 0; i < 30; i++) {
        if ((u32)(NIS_PROFILE(0)->savedDecks[deck].cards[i].type - 1) < 2) {
            count++;
        }
    }
    return count;
}

/* group 0: the cards owned, 1-3: a saved deck's; kind 0: Digimon cards,
   else the others, negative: all */
s32 NIS_countCards(s32 group, s32 kind, s32 element, s32 level) {
    s32 count = 0;

    if (kind < 0) {
        if (group == 0) {
            count = NIS_countOwnedCards();
        }
    } else if (group == 0) {
        if (kind == 0) {
            count = NIS_countOwnedDigimonOf(element, level);
        } else {
            count = NIS_countOwnedOptions() + NIS_countOwnedOthers();
        }
    } else if (kind == 0) {
        count = NIS_countDeckDigimonOf(group - 1, element, level);
    } else {
        count = NIS_countDeckOptions(group - 1);
    }
    return count;
}

NisCardPicture NIS_getCardPicture(s32 type, s32 index) {
    NisCardPicture picture;
    s32 id;
    s32 x;

    id = NIS_getCardId(type, index);
    x = 384 + (id % 36) / 6 * 20 + id / 36 * 128;
    picture.u = (x * 2) & 0xFF;
    picture.v = ((id % 6) * 40) & 0xFF;
    picture.tpage = ((x & 0x380) >> 6) | 0x80;
    if (type == 0) {
        picture.frame = NIS_CARD_ELEMENT(id);
    } else {
        picture.frame = type + 4;
    }
    if (type == 0 && index >= 0x6C) {
        picture.frame = picture.frame * -1 - 1;
    }
    if (type == 1 && (u32)(index - 0x23) < 8) {
        picture.frame++;
    }
    return picture;
}

s32 NIS_countSavedDecks(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (!NIS_PROFILE(0)->savedDecks[i].inUse) {
            break;
        }
    }
    return i;
}

/* puts copies of a card in the deck being edited (count > 0), or takes
   them out (count < 0) */
void NIS_addCardsToDeck(s32 type, s32 index, s32 count) {
    CardSlot *slot;
    s32 i;
#if JP_DEBUG_BUILD
    char text[8];
#endif

    if (count == 0) {
        return;
    }
    slot = NIS_PROFILE(0)->savedDecks[NIS_DECK_EDIT.deck].cards;
    if (count < 0) {
        for (i = 0; i < 30; i++, slot++) {
            if (type == slot->type && index == slot->index) {
                slot->type = 0xFF;
                if (++count >= 0) {
                    return;
                }
            }
        }
    } else {
        for (i = 0; i < 30; i++, slot++) {
            if (slot->type == 0xFF) {
                slot->type = type;
                slot->index = index;
                if (--count <= 0) {
                    return;
                }
            }
        }
    }
#if JP_DEBUG_BUILD
    /* the deck had no room for them, or not the copies to take: "failed
       to update the deck" */
    sprintf(text, "FLG = %d", count);
    NIS_DEBUG_NAME_TASK(0, "ERROR TASK");
    spawnTask(0, -1, 0, 0x200, func_80019CE8, "デッキの内容を更新失敗", count, getCurrentTaskId(), 0);
    waitFrames(0x7FFFFFFF);
#endif
}

/* quicksort by card id */
void NIS_sortCardEntries(NisCardEntry *entries, s32 lo, s32 hi) {
    NisCardEntry swap;
    s32 pivot;
    s32 j;
    s32 i;

    pivot = NIS_getCardId(entries[(lo + hi) / 2].type, entries[(lo + hi) / 2].index);
    i = lo;
    j = hi;
    while (1) {
        while (NIS_getCardId(entries[i].type, entries[i].index) < pivot && i < hi) {
            i++;
        }
        while (pivot < NIS_getCardId(entries[j].type, entries[j].index) && lo < j) {
            j--;
        }
        if (i >= j) {
            break;
        }
        swap = entries[i];
        entries[i] = entries[j];
        entries[j] = swap;
        i++;
        j--;
    }
    if (lo < i - 1) {
        NIS_sortCardEntries(entries, lo, i - 1);
    }
    if (j + 1 < hi) {
        NIS_sortCardEntries(entries, j + 1, hi);
    }
}

void NIS_countListInDeck(NisCardList *list, s32 deck) {
    CardSlot *slot;
    s32 i;
    s32 j;

    for (i = 0; i < list->count; i++) {
        slot = NIS_PROFILE(0)->savedDecks[deck].cards;
        for (j = 0; j < 30; j++, slot++) {
            if (list->entries[i].type == slot->type && list->entries[i].index == slot->index) {
                list->entries[i].inDeck++;
            }
        }
    }
}

void NIS_countListOwned(NisCardList *list) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        switch (list->entries[i].type) {
        case 0:
            list->entries[i].count = NIS_PROFILE(0)->digimonCards[list->entries[i].index] & 0xF;
            break;
        case 1:
            list->entries[i].count = NIS_PROFILE(0)->optionCards[list->entries[i].index] & 0xF;
            break;
        case 2:
            list->entries[i].count = NIS_PROFILE(0)->otherCards[list->entries[i].index] & 0xF;
            break;
        }
    }
}

/* returns 1 if the card is in the list already */
s32 NIS_addCardToList(NisCardList *list, u8 type, u8 index) {
    s32 i;

    for (i = 0; i < list->count; i++) {
        if (list->entries[i].type == type && list->entries[i].index == index) {
            return 1;
        }
    }
    list->entries[i].brightness = 0x40;
    list->entries[i].picture = NIS_getCardPicture(type, index);
    list->entries[i].type = type;
    list->entries[i].index = index;
    list->count++;
    return 0;
}

void NIS_listDeckCards(NisCardList *list, s32 deck) {
    CardSlot *slot;
    s32 i;

    slot = NIS_PROFILE(0)->savedDecks[deck].cards;
    for (i = 0; i < 30; i++, slot++) {
        if (slot->type != 0xFF) {
            NIS_addCardToList(list, slot->type, slot->index);
        }
    }
    NIS_sortCardEntries(list->entries, 0, list->count - 1);
}

void NIS_listOwnedDigimon(NisCardList *list, s32 element, s32 level) {
    s32 i;

    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        if (NIS_PROFILE(0)->digimonCards[i] & 0xF) {
            if (NIS_CARD_ELEMENT(i) == element && (level < 0 || NIS_CARD_LEVEL(i) == level)) {
                NIS_addCardToList(list, 0, i);
            }
        }
    }
}

void NIS_listOwnedOptionsAndOthers(NisCardList *list) {
    s32 i;

    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        if (NIS_PROFILE(0)->optionCards[i] & 0xF) {
            NIS_addCardToList(list, 1, i);
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        if (NIS_PROFILE(0)->otherCards[i] & 0xF) {
            NIS_addCardToList(list, 2, i);
        }
    }
}

void NIS_listOwnedOptionsOfStyle(NisCardList *list, s8 style) {
    s32 i;

    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        if ((NIS_PROFILE(0)->optionCards[i] & 0xF) && NIS_OPTION_STYLES[i] == style) {
            NIS_addCardToList(list, 1, i);
        }
    }
}

void NIS_listOwnedOthers(NisCardList *list) {
    s32 i;

    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        if (NIS_PROFILE(0)->otherCards[i] & 0xF) {
            NIS_addCardToList(list, 2, i);
        }
    }
}

void NIS_listNewCards(NisCardList *list) {
    s32 i;

    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        if (NIS_PROFILE(0)->digimonCards[i] & 0x80) {
            NIS_addCardToList(list, 0, i);
        }
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        if (NIS_PROFILE(0)->optionCards[i] & 0x80) {
            NIS_addCardToList(list, 1, i);
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        if (NIS_PROFILE(0)->otherCards[i] & 0x80) {
            NIS_addCardToList(list, 2, i);
        }
    }
}

void NIS_listAllCards(NisCardList *list) {
    s32 i;

    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        NIS_addCardToList(list, 0, i);
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        NIS_addCardToList(list, 1, i);
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        NIS_addCardToList(list, 2, i);
    }
}

#if JP_DEBUG_BUILD
const char NIS_STR_DECK_EDIT[] = "DECK EDIT";
#endif

/* kind 0: the deck's cards, 1-5: the Digimon of element kind - 1, 6: the
   options and others, 7-8: every card, 9: the options of a style, 10: the
   others */
NisCardList *NIS_buildCardList(s32 deck, u32 kind, s32 arg) {
    NisCardList *list;

    list = allocTaskHeapBlock(sizeof(NisCardList));
    bzero((void *)list, sizeof(NisCardList));
    switch (kind) {
    case 0:
        NIS_listDeckCards(list, deck);
        break;
    case 1:
        NIS_listOwnedDigimon(list, 0, arg);
        break;
    case 2:
        NIS_listOwnedDigimon(list, 1, arg);
        break;
    case 3:
        NIS_listOwnedDigimon(list, 2, arg);
        break;
    case 4:
        NIS_listOwnedDigimon(list, 3, arg);
        break;
    case 5:
        NIS_listOwnedDigimon(list, 4, arg);
        break;
    case 6:
        NIS_listOwnedOptionsAndOthers(list);
        break;
    case 7:
    case 8:
        NIS_listAllCards(list);
        break;
    case 9:
        NIS_listOwnedOptionsOfStyle(list, arg);
        break;
    case 10:
        NIS_listOwnedOthers(list);
        break;
    }
    NIS_countListInDeck(list, deck);
    NIS_countListOwned(list);
    NIS_CARD_LIST = list;
    return list;
}

/* the copies a list offers, at most 4 of each card */
s32 NIS_countListCopies(NisCardList *list) {
    s32 total = 0;
    s32 i;

    for (i = 0; i < list->count; i++) {
        if (list->entries[i].count < 5) {
            total += list->entries[i].count;
        } else {
            total += 4;
        }
    }
    return total;
}

s32 NIS_takeAllListCopies(s32 deck, NisCardList *list) {
    s32 taken = 0;
    s32 i;
    s32 j;

    if (NIS_DECK_EDIT.cardCount >= 30) {
        return taken;
    }
    for (i = 0; i < list->count; i++) {
        for (j = 0; j < list->entries[i].count; j++) {
            NIS_addCardsToDeck(list->entries[i].type, list->entries[i].index, 1);
            taken++;
            if (++NIS_DECK_EDIT.cardCount >= 30) {
                return taken;
            }
        }
    }
    return taken;
}

/* puts count random copies from the list in the deck (at most 4 of each
   card, all of them if the list offers no more than count). i also holds
   the copies over count, as the original's registers show: the match
   depends on it */
s32 NIS_takeListCopies(s32 deck, NisCardList *list, s32 count) {
    s32 taken;
    s32 i;
    s32 k;

    if (count <= 0 || NIS_DECK_EDIT.cardCount >= 30) {
        return 0;
    }
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].count >= 5) {
            list->entries[i].count = 4;
        }
    }
    i = NIS_countListCopies(list) - count;
    if (i > 0) {
        if (count < i) {
            taken = 0;
            for (i = 0; i < count; i++) {
                while (1) {
                    k = rand() % list->count;
                    if (list->entries[k].count != 0) {
                        list->entries[k].count--;
                        NIS_addCardsToDeck(list->entries[k].type, list->entries[k].index, 1);
                        taken++;
                        break;
                    }
                }
                if (++NIS_DECK_EDIT.cardCount >= 30) {
                    break;
                }
            }
            return taken;
        }
        while (i > 0) {
            while (1) {
                k = rand() % list->count;
                if (list->entries[k].count != 0) {
                    i--;
                    list->entries[k].count--;
                    break;
                }
            }
        }
    }
    return NIS_takeAllListCopies(deck, list);
}

/* the Digimon cards of an element and a level in a saved deck */
s32 NIS_countDeckDigimonOfLevel(s32 deck, s32 element, s32 level) {
    CardSlot *slot;
    s32 count;
    s32 key;
    s32 i;

    slot = NIS_PROFILE(0)->savedDecks[deck].cards;
    count = 0;
    key = (element << 4) | level;
    for (i = 0; i < 30; i++, slot++) {
        if (slot->type == 0 && NIS_DIGIMON_CARDS[slot->index].elementLevel == key) {
            count++;
        }
    }
    return count;
}

/* the element that goes best with the main one, for the style */
s8 NIS_pickSecondElement(s32 deck, s8 element, s8 style, s32 minCount) {
    s8 order[2][5] = { { 0, 3, 2, 1, 4 }, { 1, 4, 2, 3, 0 } };
    s32 totals[5] = { 0 };
    NisCardList *list;
    s32 best;
    s32 i;

    for (i = 0; i < 5; i++) {
        if (order[style][i] != element) {
            list = NIS_buildCardList(deck, order[style][i] + 1, 0);
            totals[i] = NIS_countListCopies(list);
            if (totals[i] >= minCount) {
                freeHeapBlock(list);
                return order[style][i];
            }
            freeHeapBlock(list);
        }
    }
    best = 0;
    for (i = 1; i < 5; i++) {
        if (order[style][i] != element && totals[best] < totals[i]) {
            best = i;
        }
    }
    if (order[style][best] == element) {
        for (i = 0; i < 5; i++) {
            if (order[style][i] != element) {
                best = i;
                break;
            }
        }
    }
    return order[style][best];
}

s32 NIS_countDeckFreeSlots(s32 deck) {
    CardSlot *slot;
    s32 count;
    s32 i;

    slot = NIS_PROFILE(0)->savedDecks[deck].cards;
    count = 0;
    for (i = 0; i < 30; i++, slot++) {
        if (slot->type == 0xFF) {
            count++;
        }
    }
    return count;
}

/* puts count copies of the element's Digimon of a level that the deck
   doesn't have yet */
s32 NIS_takeDigimonOfLevel(s32 deck, s32 element, s32 level, s32 count) {
    NisCardList *list;
    s32 i;

    if (count <= 0 || NIS_DECK_EDIT.cardCount >= 30) {
        return 0;
    }
    list = NIS_buildCardList(deck, element + 1, level);
    for (i = 0; i < list->count; i++) {
        if (list->entries[i].count >= 6) {
            list->entries[i].count = 4;
        }
    }
    for (i = 0; i < list->count; i++) {
        list->entries[i].count -= list->entries[i].inDeck;
    }
    /* i now counts the copies taken */
    i = NIS_takeListCopies(deck, list, count);
    freeHeapBlock(list);
    return i;
}

/* fills the deck being edited with 30 cards: options of the style, the
   main element's Digimon of each level, a second element's, and whatever
   is left */
void NIS_buildAutoDeck(void) {
    /* the cards of each kind, for an attack deck (0) and a defense deck
       (1): options and Digimon of level R, C and U */
    s8 counts[2][4] = { { 10, 10, 6, 4 }, { 5, 12, 8, 6 } };
    NisCardList *list;
    s32 total;
    s32 taken;
    s32 i;
    s32 count;
    s32 deck;
    s8 second;
    CardSlot *slot;

    deck = NIS_DECK_EDIT.deck;
    slot = NIS_PROFILE(0)->savedDecks[deck].cards;
    for (i = 0; i < 30; i++, slot++) {
        slot->type = 0xFF;
    }
    NIS_DECK_EDIT.cardCount = 0;
    /* i now holds the copies wanted of each kind */
    i = counts[NIS_DECK_EDIT.unkA][0];
    list = NIS_buildCardList(deck, 9, NIS_DECK_EDIT.cardIndex);
    total = NIS_takeListCopies(deck, list, i);
    if (total < i) {
        freeHeapBlock(list);
        list = NIS_buildCardList(deck, 9, (s16)(NIS_DECK_EDIT.cardIndex ^ 1));
        total += NIS_takeListCopies(deck, list, i - total);
    }
    freeHeapBlock(list);
    i = counts[NIS_DECK_EDIT.unkA][1];
    second = NIS_pickSecondElement(deck, NIS_DECK_EDIT.cardType, NIS_DECK_EDIT.cardIndex, (i / 2) & 0xE);
    list = NIS_buildCardList(deck, NIS_DECK_EDIT.cardType + 1, 0);
    taken = NIS_takeListCopies(deck, list, i);
    if (taken < i) {
        freeHeapBlock(list);
        list = NIS_buildCardList(deck, second + 1, 0);
        taken += NIS_takeListCopies(deck, list, i - taken);
    }
    freeHeapBlock(list);
    total += taken;
    i = counts[NIS_DECK_EDIT.unkA][2];
    list = NIS_buildCardList(deck, NIS_DECK_EDIT.cardType + 1, 1);
    taken = NIS_takeListCopies(deck, list, i);
    if (taken < i) {
        if (NIS_countDeckDigimonOfLevel(deck, second, 0) >= 3) {
            goto second_level1;
        }
    } else {
        count = NIS_countDeckDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 0);
        if (NIS_DECK_EDIT.unkA == 0) {
            count -= 2;
        } else {
            count = count * 3 / 4;
        }
        taken += NIS_takeDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 1, count - taken);
        if (taken < 6) {
        second_level1:
            freeHeapBlock(list);
            list = NIS_buildCardList(deck, second + 1, 1);
            if (NIS_countListCopies(list) < 2) {
                taken += NIS_takeListCopies(deck, list, i - taken);
            } else {
                taken += NIS_takeListCopies(deck, list, 2);
            }
        }
    }
    freeHeapBlock(list);
    taken += NIS_takeDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 0, i - taken);
    taken += NIS_takeDigimonOfLevel(deck, second, 0, i - taken);
    total += taken;
    i = counts[NIS_DECK_EDIT.unkA][3];
    list = NIS_buildCardList(deck, NIS_DECK_EDIT.cardType + 1, 2);
    if (NIS_countListCopies(list) < i) {
        taken = NIS_takeListCopies(deck, list, i);
        freeHeapBlock(list);
        list = NIS_buildCardList(deck, second + 1, 2);
        if (NIS_countDeckDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 1) >= i) {
            if (NIS_countDeckDigimonOfLevel(deck, second, 1) >= 2) {
                taken += NIS_takeListCopies(deck, list, i - taken);
            }
        } else if (NIS_countDeckDigimonOfLevel(deck, second, 1) >= i / 6 + 1) {
            taken += NIS_takeListCopies(deck, list, i / 6 + 1);
        }
    } else if (NIS_countDeckDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 1) >= i) {
        taken = NIS_takeListCopies(deck, list, i);
    } else {
        taken = NIS_takeListCopies(deck, list, NIS_countDeckDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 1) / 2);
        freeHeapBlock(list);
        list = NIS_buildCardList(deck, second + 1, 2);
        taken += NIS_takeListCopies(deck, list, NIS_countDeckDigimonOfLevel(deck, second, 1) / 2);
    }
    freeHeapBlock(list);
    total += taken;
    taken = 0;
    count = NIS_countDeckDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 2) + NIS_countDeckDigimonOfLevel(deck, second, 2);
    list = NIS_buildCardList(deck, 10, -1);
    if (count < 4) {
        if (NIS_countDeckDigimonOfLevel(deck, NIS_DECK_EDIT.cardType, 1) != 0 && NIS_countDeckDigimonOfLevel(deck, second, 1) != 0) {
            taken = NIS_takeListCopies(deck, list, 30 - total);
        }
    } else if (NIS_countDeckFreeSlots(deck) >= 3) {
        taken = NIS_takeListCopies(deck, list, 2);
    } else {
        taken = NIS_takeListCopies(deck, list, 30 - total);
    }
    freeHeapBlock(list);
    total += taken;
    taken = 0;
    if (total < 30) {
        list = allocTaskHeapBlock(sizeof(NisCardList));
        bzero((void *)list, sizeof(NisCardList));
        for (i = 0; i < 5; i++) {
            if (i != NIS_DECK_EDIT.cardType && i != second) {
                NIS_listOwnedDigimon(list, i, 0);
            }
        }
        NIS_countListOwned(list);
        taken = NIS_takeListCopies(deck, list, 30 - total);
        freeHeapBlock(list);
    }
    total += taken;
    taken = 0;
    if (total < 30) {
        list = allocTaskHeapBlock(sizeof(NisCardList));
        bzero((void *)list, sizeof(NisCardList));
        for (i = 0; i < 5; i++) {
            if (i != NIS_DECK_EDIT.cardType && i != second) {
                NIS_listOwnedDigimon(list, i, 1);
            }
        }
        NIS_countListOwned(list);
        taken = NIS_takeListCopies(deck, list, 30 - total);
        freeHeapBlock(list);
    }
    total += taken;
    taken = 0;
    if (total < 30) {
        list = allocTaskHeapBlock(sizeof(NisCardList));
        bzero((void *)list, sizeof(NisCardList));
        for (i = 0; i < 5; i++) {
            if (i != NIS_DECK_EDIT.cardType && i != second) {
                NIS_listOwnedDigimon(list, i, 2);
            }
        }
        NIS_countListOwned(list);
        taken = NIS_takeListCopies(deck, list, 30 - total);
        freeHeapBlock(list);
    }
    total += taken;
    if (total < 30) {
        list = allocTaskHeapBlock(sizeof(NisCardList));
        bzero((void *)list, sizeof(NisCardList));
        for (i = 0; i < 5; i++) {
            NIS_listOwnedDigimon(list, i, 0);
        }
        for (i = 0; i < 5; i++) {
            NIS_listOwnedDigimon(list, i, 1);
        }
        for (i = 0; i < 5; i++) {
            NIS_listOwnedDigimon(list, i, 2);
        }
        NIS_listOwnedOptionsAndOthers(list);
        NIS_countListOwned(list);
        NIS_dropDeckCopiesFromList(deck, list, total);
        NIS_takeListCopies(deck, list, 30 - total);
        freeHeapBlock(list);
    }
    NIS_fixDeckCards(deck);
}

/* takes out of the deck the cards the player doesn't have enough copies
   of, and fills its empty slots with cards that are left */
void NIS_fixDeckCards(s8 deck) {
    s32 i;
    s8 found;
    s32 j;

    NIS_DIGIMON_LEFT = allocTaskHeapBlock(NIS_DIGIMON_COUNT);
    NIS_OPTIONS_LEFT = allocTaskHeapBlock(NIS_OPTION_COUNT);
    NIS_OTHERS_LEFT = allocTaskHeapBlock(NIS_OTHER_COUNT);
    for (i = 0; i < NIS_DIGIMON_COUNT; i++) {
        NIS_DIGIMON_LEFT[i] = NIS_PROFILE(0)->digimonCards[i] & 0xF;
        if (NIS_DIGIMON_LEFT[i] >= 5) {
            NIS_DIGIMON_LEFT[i] = 4;
        }
    }
    for (i = 0; i < NIS_OPTION_COUNT; i++) {
        NIS_OPTIONS_LEFT[i] = NIS_PROFILE(0)->optionCards[i] & 0xF;
        if (NIS_OPTIONS_LEFT[i] >= 5) {
            NIS_OPTIONS_LEFT[i] = 4;
        }
    }
    for (i = 0; i < NIS_OTHER_COUNT; i++) {
        NIS_OTHERS_LEFT[i] = NIS_PROFILE(0)->otherCards[i] & 0xF;
        if (NIS_OTHERS_LEFT[i] >= 5) {
            NIS_OTHERS_LEFT[i] = 4;
        }
    }
    for (i = 0; i < 30; i++) {
        switch (NIS_PROFILE(0)->savedDecks[deck].cards[i].type) {
        case 0:
            if (NIS_DIGIMON_LEFT[NIS_PROFILE(0)->savedDecks[deck].cards[i].index] != 0) {
                NIS_DIGIMON_LEFT[NIS_PROFILE(0)->savedDecks[deck].cards[i].index]--;
            } else {
                NIS_PROFILE(0)->savedDecks[deck].cards[i].index = 0xFF;
            }
            break;
        case 1:
            if (NIS_OPTIONS_LEFT[NIS_PROFILE(0)->savedDecks[deck].cards[i].index] != 0) {
                NIS_OPTIONS_LEFT[NIS_PROFILE(0)->savedDecks[deck].cards[i].index]--;
            } else {
                NIS_PROFILE(0)->savedDecks[deck].cards[i].index = 0xFF;
            }
            break;
        case 2:
            if (NIS_OTHERS_LEFT[NIS_PROFILE(0)->savedDecks[deck].cards[i].index] != 0) {
                NIS_OTHERS_LEFT[NIS_PROFILE(0)->savedDecks[deck].cards[i].index]--;
            } else {
                NIS_PROFILE(0)->savedDecks[deck].cards[i].index = 0xFF;
            }
            break;
        }
    }
    for (i = 0; i < 30; i++) {
        found = 0;
        if (NIS_PROFILE(0)->savedDecks[deck].cards[i].index == 0xFF) {
            for (j = 0; j < NIS_DIGIMON_COUNT; j++) {
                if (NIS_DIGIMON_LEFT[j] != 0) {
                    NIS_PROFILE(0)->savedDecks[deck].cards[i].index = j;
                    NIS_PROFILE(0)->savedDecks[deck].cards[i].type = 0;
                    found = 1;
                    break;
                }
            }
            if (!found) {
                for (j = 0; j < NIS_OPTION_COUNT; j++) {
                    if (NIS_OPTIONS_LEFT[j] != 0) {
                        NIS_PROFILE(0)->savedDecks[deck].cards[i].index = j;
                        NIS_PROFILE(0)->savedDecks[deck].cards[i].type = 1;
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    for (j = 0; j < NIS_OTHER_COUNT; j++) {
                        if (NIS_OTHERS_LEFT[j] != 0) {
                            NIS_PROFILE(0)->savedDecks[deck].cards[i].index = j;
                            NIS_PROFILE(0)->savedDecks[deck].cards[i].type = 2;
                            break;
                        }
                    }
                }
            }
        }
    }
    freeHeapBlock(NIS_DIGIMON_LEFT);
    freeHeapBlock(NIS_OPTIONS_LEFT);
    freeHeapBlock(NIS_OTHERS_LEFT);
}

/* takes the deck's first cards out of the list's copies, then keeps only
   the cards that have copies left */
void NIS_dropDeckCopiesFromList(s32 deck, NisCardList *list, s32 count) {
    NisCardList copy;
    CardSlot *slot;
    NisCardEntry *entry;
    s32 kept;
    s32 i;
    s32 j;

    slot = NIS_PROFILE(0)->savedDecks[deck].cards;
    copy = *list;
    for (i = 0; i < count; i++, slot++) {
        entry = list->entries;
        for (j = 0; j < copy.count; j++, entry++) {
            if (*(u16 *)slot == *(u16 *)&entry->type) {
                if (entry->count >= 5) {
                    entry->count = 4;
                }
                if (entry->count != 0) {
                    entry->count--;
                }
                j = copy.count;
            }
        }
    }
    copy = *list;
    bzero((void *)list, sizeof(NisCardList));
    kept = 0;
    for (i = 0; i < copy.count; i++) {
        if (copy.entries[i].count != 0) {
            NIS_addCardToList(list, copy.entries[i].type, copy.entries[i].index);
            list->entries[kept].count = copy.entries[i].count;
            kept++;
        }
    }
}
