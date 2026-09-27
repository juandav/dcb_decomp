#ifndef DCB_CARD_ZONES_H
#define DCB_CARD_ZONES_H

#include "game.h"
#include "dcb/duel_setup.h"

s32 discardCardToOfflineDeck(s32 cardIndex, s32 player);
s32 takePartnerCardFromOnlineDeck(s32 player);
s32 countOnlineDeckCards(s32 player);
s32 getActiveDigimonCard(s32 player);
s32 countEmptyDpSlots(s32 player);
s32 countEmptyHandSlots(s32 player);
s32 checkHandHasDigimonCard(s32 player);
s32 checkHandHasDigivolveCard(s32 player);
s32 placeActiveDigimon(s32 cardIndex, s32 player);
s32 armorDigivolvePartner(s32 player, s32 partnerSlot);
s32 armorDevolvePartner(s32 player, s32 partnerSlot);
s32 peekDpSlotTop(s32 player);
s32 getPlayedCard(s32 player);
s32 takePlayedCard(s32 player);
s32 getActiveDigimonCard(s32);
s32 countOnlineDeckCards(s32);
s32 countOfflineDeckCards(s32 player);
s32 countEmptyDpSlots(s32);
s32 peekOfflineDeckTop(s32 player);
s32 takeOfflineDeckTopCard(s32 player);
s32 peekOnlineDeckTop(s32 player);
s32 drawOnlineDeckCard(s32 player);
s32 returnCardToOnlineDeck(s32 cardIndex, s32 player);
s32 addCardToHand(s32 cardIndex, s32 player);
s32 removeCardFromHand(s32 cardIndex, s32 player);
s32 checkHandHasOptionCard(s32 player);
s32 countEmptyDigimonStackSlots(s32 player);
s32 removeCardFromDigimonStack(s32 cardIndex, s32 player);
s32 sumDigivolvePoints(s32 player);
s32 addCardToDpSlots(s32 cardIndex, s32 player);
s32 removeCardFromDpSlots(s32 cardIndex, s32 player);
s32 isPlayedCardSlotEmpty(s32 player);
s32 setPlayedCard(s32 cardIndex, s32 player);
void shuffleOnlineDeck(s32 player);
void shuffleOfflineDeck(s32 player);

#endif /* DCB_CARD_ZONES_H */
