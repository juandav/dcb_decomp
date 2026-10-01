#ifndef DCB_OVERLAY_CALLS_H
#define DCB_OVERLAY_CALLS_H

#include "game.h"

/*
 * Overlay functions the executable calls, or hands to spawnTask, by address
 * while that overlay is loaded at OVERLAY_LOAD_ADDR. They carry the overlay's
 * own names: config/<version>/symbols_overlay_calls.txt gives those names to
 * the addresses for the executable alone (its splat config reads that file,
 * the overlays' configs don't), so another overlay loaded at the same address
 * keeps its own names. The overlays don't include this header; their own
 * headers hold the real prototypes.
 */

/* ENDSEG: the player records */
void END_runPlayerRecords();

/* EVOSEG: partner fusion */
void EVO_runFusion();

/* KAWSEG: the duel */
s32 KAW_resolveBattle();
s32 KAW_checkDigivolveTarget();
s32 KAW_checkAnyDigivolve();
s32 KAW_setStatPenalty();
s32 KAW_showTutorialMessage();
s32 KAW_tickTutorial();
s32 KAW_tickCardCursor();
s32 KAW_openCardSelect();
s32 KAW_closeCardSelect();
s32 KAW_drawCardToHand();
s32 KAW_discardCard();
s32 KAW_placeDigimonFromHand();
s32 KAW_returnDigimonToHand();
void KAW_returnPlayedCard(s32 player, s32 slot);
s32 KAW_returnDpCardToHand();
s32 KAW_playOnlineDeckTop();
s32 KAW_playCardFromHand();
s32 KAW_chargeDpCard();
s32 KAW_redrawHand();
s32 KAW_undoDigivolve();
s32 KAW_discardDpSlots();
s32 KAW_checkKnockout();
s32 KAW_playEffect();
s32 KAW_playEffectOnOpponent();
s32 KAW_checkDigimonBonuses();
s32 KAW_checkHandBonuses();
void KAW_showBonusBanner();
s32 KAW_initHudPanels();
s32 KAW_allocCardPolys();
s32 KAW_createCursor(s32, s32, s32, s32, s32);
s32 KAW_freeHudPanels();
s32 KAW_freeCardPolys();
#if VERSION_JP
s32 KAW_drawHandHints(s32 player, s32 mode);
#elif VERSION_US || VERSION_EU
s32 KAW_drawHandHints(s32);
#endif
s32 KAW_renderRing();
void KAW_renderCursor(void *, s32);
void KAW_uploadPartnerPortraits(u8 *archive);
void KAW_rollPrizeCards(u8 *archive);
s32 KAW_decideRedraw(s32);
s32 KAW_chooseDigimonToPlace(s32);
s32 KAW_planDigivolves(s32);
s32 KAW_planDigivolveOptions(s32);
s32 KAW_selectDigivolvePlan(s32);
s32 KAW_chooseDpCard(s32);
s32 KAW_countHandDigivolves(s32);
s32 KAW_chooseDigivolveOption(s32);
s32 KAW_chooseDigivolveTarget(s32);
s32 KAW_simulateBattles(s32);
s32 KAW_chooseAttack(s32);
void KAW_chooseSupportCard(void);
void KAW_runDeckSelect(s32, s32);
void KAW_runVersusIntro(s32, s32);
void KAW_resetBonusFlags(s32);
void KAW_loadEffectArchive(void);
void KAW_initRing(void);
void KAW_tickDuelMenu(void);
void KAW_freeRing(void);
void KAW_runResultScreen(); /* a task main only hands to spawnTask */
extern s32 KAW_RESULT_SCREEN_STATE;
void KAW_freeCursor(void *);
void KAW_runExpScreen(void);
void KAW_runPrizeScreen(void);
void KAW_freeEffectArchive(void);
void KAW_freeTutorial(void);
#if VERSION_JP
void KAW_pickCardArtSlot(void);
#endif

/* OPENSEG: the title, movies and memory card screens */
s32 OPEN_playMovie(s32);
#if VERSION_JP
/* jp's title screen, in NISSEG */
void NIS_runTitleScreen();
#endif
s32 OPEN_findMovieFile(char *);
void OPEN_runMemcardScreen();
void OPEN_runTitleScreen();
void OPEN_runUserRegistration();
void OPEN_runBattleWithFriend();
s32 OPEN_loadFriendSaves(void);
extern u8 OPEN_MEMCARD_CANCELLED; /* OPEN_MEMCARD.cancelled */

/* SAISEG: the world map and its areas */
void SAI_runWorldMap();
void SAI_runArea();

/* SUBSEG: the deck editor and partner equipment */
void SUB_runDeckEditor();
void SUB_runPartnerEquipment();

/* SUGSEG: the battle scenes */
void SUG_updateScreenCopyQuads(void);
s32 SUG_startTexAnim(s32, s32, RingEffect *, u8 *, s32);
void SUG_tickTexAnim(u8 *);
void SUG_freeTexAnim(u8 *);
void SUG_runPolygonBattle();

#endif /* DCB_OVERLAY_CALLS_H */
