#ifndef DCB_CARD_DB_H
#define DCB_CARD_DB_H

#include "game.h"

typedef struct {
#if VERSION_JP
    /* 0x00 */ s16 price; /* in hundreds of Bits (jp's card shop); jp keeps no id */
#elif VERSION_US || VERSION_EU
    /* 0x00 */ s16 id;
#endif
    /* 0x02 */ u8 type;
#if VERSION_JP
    /* 0x03 */ char name[0x10]; /* jp: the fields below are 6 bytes lower */
#elif VERSION_US || VERSION_EU
    /* 0x03 */ char name[0x16];
#endif
    /* 0x19 */ u8 unk19[0x73];
    /* 0x8C */ s8 supportIcon;
#if VERSION_JP
    /* 0x8D */ u8 text[4][0x13];
#elif VERSION_US || VERSION_EU
    /* 0x8D */ u8 text[4][0x15];
#endif
    /* 0xE1 */ u8 unkE1;
} OptionCardData;
typedef struct {
#if VERSION_JP
    /* 0x00 */ s16 price; /* in hundreds of Bits (jp's card shop); jp keeps no id */
#elif VERSION_US || VERSION_EU
    /* 0x00 */ s16 id;
#endif
    /* 0x02 */ u8 type;
#if VERSION_JP
    /* 0x03 */ char name[0x10]; /* jp: the fields below are 6 bytes lower */
#elif VERSION_US || VERSION_EU
    /* 0x03 */ char name[0x16];
#endif
    /* 0x19 */ u8 rewardRank;
    /* 0x1A */ s8 effect; /* what the Digivolve Option does (runDuelTurnLoop) */
#if VERSION_JP
    /* 0x1B */ u8 text[4][0x13];
#elif VERSION_US || VERSION_EU
    /* 0x1B */ u8 text[4][0x15];
#endif
    /* 0x6F */ u8 unk6F;
} DigivolveCardData;

#if VERSION_JP
/* a booster pack of jp's card shop, laid out as a card */
typedef struct {
    /* 0x00 */ s16 price; /* in hundreds of Bits */
    /* 0x02 */ u8 type; /* 3 */
    /* 0x03 */ char name[0x11];
    /* 0x14 */ char text[4][0x13]; /* "１" to "４": not shown */
} BoosterPack;
#endif

extern u8 PARTNER_ARMOR_CARD_IDS[6][3];
/* per pack: for each colour and the options, a card id range (first, span, …) */
extern s8 REWARD_CARD_RANGES[16][6][3];
extern u8 *CARD_DB_FILE;
extern u8 *OPTION_CARDS;
extern u8 *DIGIVOLVE_CARDS;
extern u8 BASE_DECK_IDS[];
extern u8 PARTNER_CARD_IDS[];
extern u8 PARTNER_START_ABILITIES[];
extern SupportCondition PARTNER_ABILITY_CONDITIONS[];
extern SupportAction PARTNER_ABILITY_ACTIONS[];
extern PartnerAbility PARTNER_ABILITIES[];
extern u8 *PARTNER_ABILITY_TEXTS[];

void loadCardDatabase();
#if VERSION_JP
/* jp's draws a serial no earlier copy of the card has, in the table given */
void assignCardCopySerial(s32 cardId, s32 copy, u16 (*serials)[8]);
#elif VERSION_US || VERSION_EU
void assignCardCopySerial(s32 player, s32 cardId, s32 copy);
#else
#error "card_db.h: version not checked"
#endif
void clearCollectionNewFlags(s32 player);
void clearCollectionFirstObtainedFlags(s32 player);
#if VERSION_JP
/* jp's keep the first player's collection, by card type (0-2) and index */
s32 addCardToCollection(s32 type, s32 index, s32 count);
s32 removeCardFromCollection(s32 type, s32 index, s32 count);
#elif VERSION_US || VERSION_EU
s32 addCardToCollection(s32 player, s32 cardId, s32 count);
s8 removeCardFromCollection(s32 player, s32 cardId, s32 count);
#else
#error "card_db.h: version not checked"
#endif
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
s32 getBaseDeckId(s32 deckId);
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
#if VERSION_JP
/* jp: the battle log, up to 7 lines for each player */
void clearBattleLog(void);
s32 countBattleLogLines(s32 player);
/* jp: adds TEXT to the lines PLAYER's battle log shows (up to 7) */
void addBattleLogLine(s32 player, char *text);
void drawBattleLog(s32 player, s32 x, s32 y, s32 z);
#endif

#endif /* DCB_CARD_DB_H */
