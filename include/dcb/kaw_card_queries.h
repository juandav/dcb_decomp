#ifndef DCB_KAW_CARD_QUERIES_H
#define DCB_KAW_CARD_QUERIES_H

#include "game.h"

s32 KAW_countHandDigimonOfSpecialty(s32 player, s32 attr);
s32 KAW_countHandDigimon(s32 player);
s32 KAW_countDeckDigimonOfLevel(s32 player, s32 level);
s32 KAW_countHandDigimonOfLevel(s32 player, s32 level);
s32 KAW_countDeckDigimonOfSpecialty(s32 player, s32 attr);
s32 KAW_countDeckDigimonOfSpecialtyAndLevel(s32 player, s32 attr, s32 level);
s32 KAW_countHandDigimonOfSpecialtyAndLevel(s32 player, s32 attr, s32 level);
s32 KAW_hasDigivolveInHand(s32 id, s32 player);
s32 KAW_countStrongerDigimonInHand(s32 player);
s32 KAW_isVoidingCard(s32 player, s32 card);
s32 KAW_findVoidingCardInHand(s32 player);
s32 KAW_isPileEffectCard(s32 player, s32 card);
s32 KAW_isRecoveryCard(s32 player, s32 card);
s32 KAW_findRecoveryCardInHand(s32 player);
s32 KAW_getActiveCrossEffect(s32 player);
s32 KAW_checkSupportCard(s32 player, s32 card);

#endif /* DCB_KAW_CARD_QUERIES_H */
