#ifndef DCB_CARD_DB_H
#define DCB_CARD_DB_H

#include "game.h"

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 unk2[0xE0];
} OptionCardData;
typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ u8 unk2[0x6E];
} DigivolveCardData;

extern u8 PARTNER_ARMOR_CARD_IDS[6][3];
extern u8 *CARD_DB_FILE;
extern u8 *OPTION_CARDS;
extern u8 *DIGIVOLVE_CARDS;
extern u8 D_8006E50C[];
extern u8 PARTNER_CARD_IDS[];
extern u8 PARTNER_START_ABILITIES[];
extern SupportCondition PARTNER_ABILITY_CONDITIONS[];
extern SupportAction PARTNER_ABILITY_ACTIONS[];
extern PartnerAbility PARTNER_ABILITIES[];
extern u8 *PARTNER_ABILITY_TEXTS[];

void loadCardDatabase();
void func_80045968(s32 player, s32 cardId, s32 copy);
void clearCollectionNewFlags(s32 player);
void clearCollectionFirstObtainedFlags(s32 player);
s8 addCardToCollection(s32 player, s32 cardId, s32 count);
s8 removeCardFromCollection(s32 player, s32 cardId, s32 count);
s32 getOwnedCardCount(s32 player, s32 cardId);
s32 getCardId(s32 type, s32 index);
s32 getCardSpecialty(s32 cardId);
s32 getCardLevel(s32 cardId);
void *getCardData(s32 cardId);
void markDeckCardsSeen(s32 player);
void markBuildableOpponentDecks(s32 player);
void addRewardCardsToCollection(s32 player);
void countSeenCards(s32 player);
void linkSavedDecks(s32 player);
void linkDeckCardData(s32 player, PlayerDeck *deck);
void setCardSlotFromId(u8 *out, s32 id);
s32 countDeckCardsByFilter(s32 unused, PlayerDeck *deck, s32 mask);
s32 storeSavedDeck(s32 player, PlayerDeck *src, s32 slot);
s32 getSavedDeck(s32 player, PlayerDeck *out, s32 slot);
s32 deleteSavedDeck(s32 player, s32 slot);
s32 func_800471F4(s32 deckId);
void backupPartners(s32 player);
void restorePartners(s32 player);
void refreshPartners(s32 player);
void addPartner(s32 player, s32 partner, s32 obtain);
void obtainPartner(s32 player, s32 partner);
s32 getPartnerIndex(s32 cardId);
s32 getSlotPartnerIndex(s32 player, s32 slot);
s32 findPartnerSlot(s32 player, s32 cardId);
void unlockPartnerArmor(s32 player, s32 partner, s32 armor);
s32 countUnlockedPartnerArmors(s32 player, s32 partner);
void selectPartnerArmor(s32 player, s32 partner, s32 armor);
s32 getSelectedArmorIndex(s32 player, s32 partner);
s32 findArmorPartnerSlot(s32 player, s32 cardId);
s32 updatePartnerStats(s32 player, s32 slot);
void equipPartnerAbility(s32 player, s32 slot, s32 abilitySlot, s32 ability);
void unequipPartnerAbility(s32 player, s32 slot, s32 abilitySlot);
void grantPartnerAbility(s32 player, s32 ability);
s32 canEquipPartnerAbility(s32 player, s32 slot, s32 skipSlot, s32 ability);
s32 getPartnerAbilityState(s32 player, s32 ability);

#endif /* DCB_CARD_DB_H */
