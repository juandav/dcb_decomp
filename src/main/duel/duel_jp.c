#include "common.h"
#include "game.h"
#include "dcb/duel.h"
#include "dcb/battle_hud.h"
#include "dcb/card_db.h"
#include "dcb/card_zones.h"
#include "dcb/duel_util.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/text.h"
#include "dcb/pad.h"
#include "dcb/overlay_calls.h"
#include "dcb/kaw_hand.h"

/* jp's turn loop (duel.c is us's and eu's; jp runs the tutorial duel in a
   loop of its own, tutorial_jp.c). The message bar shows
   DUEL->messageWanted */

extern u8 OVERLAY_AREA[];
/* see duel_util.c: us keeps it there */
const s32 OVERLAY_LOAD_ADDR = (s32)OVERLAY_AREA;

void runDuelTurnLoop(void) {
    s32 i;
    s32 j;

    DUEL->stopTurnLoop = 0;
    while (1) {
        waitDuelFrames(1);
        switch (DUEL->step) {
        case 1:
            PLAYER_PANEL(0, 3)->state = 1;
            PLAYER_PANEL(1, 3)->state = 1;
            PLAYER_PANEL(0, 0)->state = 1;
            PLAYER_PANEL(1, 0)->state = 1;
            DUEL->unk47F = -1;
            DUEL->unk481 = -1;
            DUEL->unk482 = -1;
            DUEL->humanPlayer = -1;
            DUEL->step++;
            break;
        case 2:
            PLAYER(0)->attackChoice = 3;
            PLAYER(1)->attackChoice = 3;
            DUEL->messageWanted = 0;
            DUEL->playedFromSlot = -1;
            DUEL->dpFromSlot = -1;
            DUEL->unk47F = 0;
            DUEL->humanPlayer = PLAYER(ME)->controller;
            DUEL->unk481 = -1;
            DUEL->unk482 = 0;
            do {
                waitDuelFrames(1);
            } while (HUD_PANEL(22)->state != 3);
            DUEL->step++;
            break;
        case 3:
            DUEL->unk481 = -1;
            while (KAW_drawCardToHand(ME) != -1) {
                waitDuelFrames(0x14);
            }
            DUEL->step++;
            break;
        case 4:
            DUEL->unk481 = -1;
            if (getActiveDigimonCard(ME) < 0 && checkHandHasDigimonCard(ME) != 0) {
                if (countOnlineDeckCards(ME) == 0) {
                    DUEL->messageWanted = 0x19;
                    DUEL->winner = ME ^ 1;
                    DUEL->step = 0x32;
                } else {
                    DUEL->messageWanted = 5;
                    DUEL->step = 8;
                }
            } else {
                DUEL->step = 5;
            }
            break;
        case 5:
            DUEL->unk482 = 0;
            if (countOnlineDeckCards(ME) == 0) {
                DUEL->step = 0xA;
                break;
            }
            if (PLAYER(ME)->controller == 2) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 1;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult != 0) {
                    waitDuelFrames(0x1E);
                    DUEL->step = 8;
                } else {
                    DUEL->step = 0xA;
                }
                break;
            }
            DUEL->messageWanted = 2;
            DUEL->step++;
            break;
        case 6:
            DUEL->unk481 = 0;
            if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA0);
                DUEL->messageWanted = 3;
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step = 0xA;
            } else if (PAD_PRESSED(ME) & PAD_SQUARE) {
                playSoundEffect(0xA0);
                DUEL->step = 9;
                DUEL->returnStep = 5;
                DUEL->viewPlayer = ME;
            }
            break;
        case 7:
            DUEL->unk481 = 4;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->messageWanted = 4;
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                DUEL->step = 5;
            }
            break;
        case 8:
            DUEL->unk481 = -1;
            KAW_redrawHand(ME);
            DUEL->step = 4;
            break;
        case 9:
            DUEL->humanPlayer = PLAYER(DUEL->viewPlayer)->controller;
            DUEL->unk481 = 5;
            DUEL->unk482 = 6;
            HUD_PANEL(22)->state = 4;
            if (DUEL->returnStep != 0x22) {
                PLAYER_PANEL(0, 3)->state = DUEL->viewPlayer * 3 + 5;
                PLAYER_PANEL(1, 3)->state = DUEL->viewPlayer * 3 + 5;
            }
            waitDuelFrames(0x14);
            while (1) {
                waitDuelFrames(1);
                KAW_tickCardCursor(DUEL->viewPlayer, 0);
                if (PAD_PRESSED(DUEL->viewPlayer) & PAD_CROSS) {
                    playSoundEffect(0xA1);
                    if (DUEL->returnStep == 0x22) {
                        PLAYER_PANEL(0, 3)->state = 0xE;
                        PLAYER_PANEL(1, 3)->state = 0xE;
                    } else {
                        PLAYER_PANEL(0, 3)->state = 2;
                        PLAYER_PANEL(1, 3)->state = 2;
                    }
                    CLOSE_CARD_SELECT(DUEL->viewPlayer);
                    DUEL->step = DUEL->returnStep;
                    waitDuelFrames(0x14);
                    break;
                }
            }
            break;
        case 0xA:
            DUEL->unk482 = 1;
            if (getActiveDigimonCard(ME) >= 0) {
                if (PLAYER(ME)->controller != 2) {
                    DUEL->step = 0xD;
                } else {
                    DUEL->step = 0xF;
                }
                break;
            }
            if (PLAYER(ME)->controller == 2) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 2;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult == -1) {
                    for (i = 0; i < 4; i++) {
                        if (PLAYER(ME)->hand[i] != -1 && HAND_CARD(ME, i).type == 0) {
                            DUEL->cpuResult = PLAYER(ME)->hand[i];
                            break;
                        }
                    }
                }
                CUR_CARD = DUEL->cpuResult;
                KAW_setStatPenalty(CUR_CARD, ME);
                DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                waitDuelFrames(0x78);
                DUEL->step = 0xF;
                break;
            }
            DUEL->messageWanted = 6;
            DUEL->step++;
            break;
        case 0xB:
            DUEL->unk481 = 4;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
            case 0x1C:
                KAW_openCardSelect(ME);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                DUEL->step = 5;
            }
            break;
        case 0xC:
            DUEL->unk481 = 4;
            if (KAW_tickCardCursor(ME, 1) == 0) {
                if (CARD_OF(ME, CUR_CARD).type == 0) {
                    playSoundEffect(0xA0);
                    KAW_setStatPenalty(CUR_CARD, ME);
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    DUEL->step++;
                }
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                DUEL->step = 5;
            }
            if (DUEL->step != 0xC) {
                KAW_closeCardSelect(ME);
            }
            break;
        case 0xD:
            DUEL->unk481 = -1;
            DUEL->messageWanted = 7;
            DUEL->step++;
        case 0xE:
            DUEL->unk481 = 4;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                if (DUEL->playedFromSlot >= 0) {
                    KAW_returnDigimonToHand(CUR_CARD, ME, DUEL->playedFromSlot);
                    DUEL->playedFromSlot = -1;
                    DUEL->step = 0xA;
                } else {
                    DUEL->step = 4;
                }
            }
            break;
        case 0xF:
            DUEL->playedFromSlot = -2;
            DUEL->dpFromSlot = -1;
            DUEL->discardedFromSlot = -1;
            DUEL->unk47F = 1;
            DUEL->unk482 = 2;
            if (PLAYER(ME)->controller == 2) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 3;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0x13;
                    break;
                }
                CUR_CARD = DUEL->cpuResult;
                DUEL->playedFromSlot = KAW_chargeDpCard(CUR_CARD, ME);
                waitDuelFrames(0x78);
                DUEL->step = 0x13;
                break;
            }
            DUEL->messageWanted = 8;
            DUEL->step++;
            break;
        case 0x10:
            DUEL->unk481 = 7;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->step = 0x13;
            }
            break;
        case 0x11:
            if (checkHandHasDigimonCard(ME) != 0 || countEmptyDpSlots(ME) == 0) {
                DUEL->step = 0x13;
                break;
            }
            KAW_openCardSelect(ME);
            DUEL->step++;
            break;
        case 0x12:
            DUEL->unk481 = 8;
            if (KAW_tickCardCursor(ME, 2) == 0) {
                i = PLAYER(ME)->hand[DUEL->cursorSlot];
                if (CARD_OF(ME, i).type == 0) {
                    playSoundEffect(0xA0);
                    DUEL->dpFromSlot = KAW_chargeDpCard(CUR_CARD, ME);
                    DUEL->step++;
                }
            } else if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                DUEL->step = 0xF;
            }
            if (DUEL->step != 0x12) {
                KAW_closeCardSelect(ME);
            }
            break;
        case 0x13:
            DUEL->unk482 = 3;
            if (PLAYER(ME)->controller == 2) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 4;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0x1A;
                } else {
                    PLAYER_PANEL(ME, 2)->state = 1;
                    CUR_CARD = DUEL->cpuResult;
                    DUEL->playedFromSlot = KAW_playCardFromHand(CUR_CARD, ME);
                    waitDuelFrames(0x78);
                    DUEL->step = 0x17;
                }
                break;
            }
            if (checkHandHasDigivolveCard(ME) != 0) {
                DUEL->playedFromSlot = -1;
                DUEL->step = 0x1A;
                break;
            }
            DUEL->messageWanted = 9;
            DUEL->step++;
            break;
        case 0x14:
            DUEL->unk481 = 8;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                KAW_openCardSelect(ME);
                PLAYER_PANEL(ME, 2)->state = 1;
                DUEL->step++;
                waitDuelFrames(0x1E);
            } else if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->step = 0x1A;
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                if (DUEL->dpFromSlot >= 0) {
                    KAW_returnDpCardToHand(peekDpSlotTop(ME), ME, DUEL->dpFromSlot);
                    DUEL->dpFromSlot = -1;
                }
                DUEL->step = 0xF;
            }
            break;
        case 0x15:
            if (KAW_tickCardCursor(ME, 5) == 0) {
                i = PLAYER(ME)->hand[DUEL->cursorSlot];
                if (CARD_OF(ME, i).type == 2) {
                    playSoundEffect(0xA0);
                    DUEL->playedFromSlot = KAW_playCardFromHand(CUR_CARD, ME);
                    DUEL->messageWanted = 0x16;
                    DUEL->step++;
                }
            } else if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->step = 0x1A;
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                DUEL->step = 0x13;
            }
            if (DUEL->step != 0x15) {
                DUEL->cursorSlot = -1;
                if (DUEL->step != 0x16) {
                    PLAYER_PANEL(0, 3)->state = 2;
                    PLAYER_PANEL(1, 3)->state = 2;
                } else {
                    if (KAW_checkAnyDigivolve(ME) == 0) {
                        DUEL->step = 0x17;
                        break;
                    }
                    clearBattleLog();
                    addBattleLogLine(ME, "この進化オプションカードは");
                    addBattleLogLine(ME, "効果がありません。");
                    addBattleLogLine(ME, "それでも使用しますか？");
                    PLAYER_PANEL(ME, 9)->state = 1;
                }
                PLAYER_PANEL(ME, 4)->state = 4;
                PLAYER_PANEL(ME, 5)->state = 4;
                if (PLAYER_PANEL(ME, 6)->state < 4) {
                    PLAYER_PANEL(ME, 6)->state = 4;
                }
                if (PLAYER_PANEL(ME, 7)->state < 4) {
                    PLAYER_PANEL(ME, 7)->state = 4;
                }
                if (DUEL->messageWanted != 0x16) {
                    if (PLAYER_PANEL(ME, 2)->state < 4) {
                        PLAYER_PANEL(ME, 2)->state = 4;
                    }
                }
            }
            break;
        case 0x16:
            DUEL->unk481 = 4;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                PLAYER_PANEL(ME, 9)->state = 4;
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                if (DUEL->playedFromSlot >= 0) {
                    KAW_returnPlayedCard(ME, DUEL->playedFromSlot);
                    DUEL->playedFromSlot = -2;
                }
                DUEL->step = 0x13;
                PLAYER_PANEL(0, 3)->state = 2;
                PLAYER_PANEL(1, 3)->state = 2;
                PLAYER_PANEL(ME, 9)->state = 4;
                if (PLAYER_PANEL(ME, 2)->state < 4) {
                    PLAYER_PANEL(ME, 2)->state = 4;
                }
            }
            break;
        case 0x17:
            DUEL->unk482 = 5;
            j = 0;
            if (KAW_checkAnyDigivolve(ME) == 0) {
                if (((DigivolveCardData *)CARD_OF(ME, getPlayedCard(ME)).card)->effect != 4) {
                    DUEL->step++;
                    break;
                }
                playSoundEffect(0xA0);
                j = 1;
                waitDuelFrames(0x78);
                KAW_discardCard(getActiveDigimonCard(ME), ME);
                waitDuelFrames(0x78);
                PLAYER(ME)->statPenalty = 0;
                placeActiveDigimon(getActiveDigimonCard(ME), ME);
                PLAYER(ME)->stats[0] *= 2;
            }
            DUEL->discardedFromSlot = DUEL->playedFromSlot;
            i = takePlayedCard(ME);
            SPRITE_KIND(i) = 6;
            discardCardToOfflineDeck(i, ME);
            if (PLAYER_PANEL(ME, 2)->state < 4) {
                PLAYER_PANEL(ME, 2)->state = 4;
            }
            KAW_closeCardSelect(ME);
            if (PLAYER(ME)->controller == 2) {
                DUEL->step = 0x20;
                waitDuelFrames(0x78);
            } else if (j) {
                DUEL->step = 0x20;
            } else {
                DUEL->step = 0x1A;
            }
            break;
        case 0x18:
            if (PLAYER(ME)->controller != 2) {
                KAW_openCardSelect(ME);
            }
            DUEL->step = 0x19;
            break;
        case 0x19:
            if (PLAYER(ME)->controller == 2) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 5;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0x20;
                } else {
                    waitDuelFrames(0x3C);
                    CUR_CARD = DUEL->cpuResult;
                }
                i = 0;
            } else {
                DUEL->unk481 = 8;
                i = KAW_tickCardCursor(ME, 3);
                if (i != 0) {
                    if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                        playSoundEffect(0xA0);
                        if (DUEL->playedFromSlot >= 0) {
                            KAW_returnPlayedCard(ME, DUEL->playedFromSlot);
                            DUEL->playedFromSlot = -2;
                        }
                        DUEL->step = 0x1A;
                    } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                        playSoundEffect(0xA1);
                        if (DUEL->playedFromSlot >= 0) {
                            KAW_returnPlayedCard(ME, DUEL->playedFromSlot);
                            DUEL->playedFromSlot = -2;
                        }
                        DUEL->step = 0x13;
                        if (PLAYER_PANEL(ME, 2)->state < 4) {
                            PLAYER_PANEL(ME, 2)->state = 4;
                        }
                    }
                }
            }
            if (i == 0 && KAW_checkDigivolveTarget(CUR_CARD, ME) == 0) {
                playSoundEffect(0xAB);
                playSoundEffect(0xAB);
                switch (((DigivolveCardData *)CARD_OF(ME, getPlayedCard(ME)).card)->effect) {
                case 3:
                    KAW_discardCard(getActiveDigimonCard(ME), ME);
                case 0:
                case 1:
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    KAW_discardDpSlots();
                    break;
                case 4:
                    PLAYER(ME)->statPenalty = 0;
                    break;
                case 5:
                    while (getActiveDigimonCard(ME) != -1) {
                        KAW_discardCard(getActiveDigimonCard(ME), ME);
                    }
                case 2:
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    break;
                }
                DUEL->step = 0x20;
            }
            if (DUEL->step != 0x19) {
                KAW_closeCardSelect(ME);
                if (DUEL->step != 0x13 && DUEL->step != 0x1A) {
                    i = takePlayedCard(ME);
                    SPRITE_KIND(i) = 6;
                    discardCardToOfflineDeck(i, ME);
                    if (PLAYER_PANEL(ME, 2)->state < 4) {
                        PLAYER_PANEL(ME, 2)->state = 4;
                    }
                    waitDuelFrames(0x78);
                }
            }
            break;
        case 0x1A:
            DUEL->unk482 = 5;
            if (PLAYER(ME)->controller == 2) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 5;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0x20;
                } else {
                    waitDuelFrames(0x3C);
                    playSoundEffect(0xAB);
                    playSoundEffect(0xAB);
                    CUR_CARD = DUEL->cpuResult;
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    KAW_discardDpSlots();
                }
                break;
            }
            if (checkHandHasDigimonCard(ME) == 0 && KAW_checkAnyDigivolve(ME) == 0) {
                DUEL->messageWanted = 0xA;
                DUEL->step++;
                break;
            }
            DUEL->step = 0x1E;
            break;
        case 0x1B:
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->step = 0x1E;
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                KAW_undoDigivolve();
            }
            break;
        case 0x1D:
            if (KAW_tickCardCursor(ME, 3) == 0) {
                if (KAW_checkDigivolveTarget(PLAYER(ME)->hand[DUEL->cursorSlot], ME) == 0) {
                    playSoundEffect(0xAB);
                    playSoundEffect(0xAB);
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    KAW_discardDpSlots();
                }
            } else if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                KAW_undoDigivolve();
            }
            if (DUEL->step != 0x1D) {
                KAW_closeCardSelect(ME);
            }
            break;
        case 0x1E:
            DUEL->messageWanted = 0xB;
            DUEL->step++;
            break;
        case 0x1F:
            DUEL->unk481 = 4;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                if (DUEL->playedFromSlot == -2) {
                    DUEL->step = 0x13;
                } else {
                    KAW_undoDigivolve();
                }
            }
            break;
        case 0x20:
            PLAYER(ME)->hasBattled = 1;
            DUEL->unk47F = 2;
            DUEL->playedFromSlot = -1;
            DUEL->dpFromSlot = -1;
            PLAYER(ME)->battleCard = CARD_OF(ME, getActiveDigimonCard(ME)).card;
            if (PLAYER(ME)->controller == 2) {
                if (getActiveDigimonCard(OPP) == -1) {
                    DUEL->step = 0x31;
                } else {
                    DUEL->step = 0x21;
                }
            } else if (getActiveDigimonCard(OPP) == -1) {
                DUEL->messageWanted = 0xC;
                DUEL->step = 0x30;
            } else {
                DUEL->step = 0x21;
            }
            break;
        case 0x21:
            DUEL->messageWanted = 0xD;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->controller == 2) {
                    DUEL->cpuPlayer = i;
                    DUEL->cpuRequest = 6;
                }
            }
            PLAYER_PANEL(0, 3)->state = 0xE;
            PLAYER_PANEL(1, 3)->state = 0xE;
            waitFrames(0x1E);
            DUEL->step++;
            break;
        case 0x22:
            DUEL->unk47F = -1;
            DUEL->humanPlayer = -1;
            DUEL->unk481 = -1;
            DUEL->unk482 = -1;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->controller == 2) {
                    if (DUEL->cpuRequest == 0 && PLAYER(i)->attackChoice == 3) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = DUEL->cpuResult;
                    }
                } else if (PLAYER(i)->attackChoice == 3) {
                    if (PAD_PRESSED(i) & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = 0;
                    } else if (PAD_PRESSED(i) & PAD_TRIANGLE) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = 1;
                    } else if (PAD_PRESSED(i) & PAD_CROSS) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = 2;
                    } else if (PAD_PRESSED(i) & PAD_SQUARE) {
                        playSoundEffect(0xA0);
                        PLAYER_PANEL(0, 8)->state = 4;
                        PLAYER_PANEL(1, 8)->state = 4;
                        DUEL->step = 9;
                        DUEL->returnStep = 0x22;
                        DUEL->viewPlayer = i;
                        DUEL->unk47F = 2;
                    }
                }
            }
            if (PLAYER(0)->attackChoice != 3 && PLAYER(1)->attackChoice != 3) {
                waitDuelFrames(0x78);
                PLAYER_PANEL(0, 8)->state = 6;
                PLAYER_PANEL(1, 8)->state = 6;
                for (i = 0; i < 2; i++) {
                    PLAYER(i)->usedAttack = PLAYER(i)->attackChoice;
                    if (PLAYER_DATA(i).attackCounts[PLAYER(i)->usedAttack] != 0xFFFF) {
                        PLAYER_DATA(i).attackCounts[PLAYER(i)->usedAttack]++;
                    }
                    if (PLAYER(i)->deck->attackCounts[PLAYER(i)->usedAttack] != 0xFFFF) {
                        PLAYER(i)->deck->attackCounts[PLAYER(i)->usedAttack]++;
                    }
                }
                waitDuelFrames(0x78);
                DUEL->step++;
                break;
            }
            break;
        case 0x23:
            DUEL->unk47F = 2;
            DUEL->unk482 = 7;
            DUEL->humanPlayer = PLAYER(OPP)->controller;
            PLAYER_PANEL(OPP, 2)->state = 1;
            if (PLAYER(OPP)->controller == 2) {
                DUEL->unk481 = -1;
                DUEL->messageWanted = 1;
                DUEL->cpuPlayer = ME ^ 1;
                DUEL->cpuRequest = 7;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult == -2) {
                    waitDuelFrames(0x3C);
                    KAW_playOnlineDeckTop(OPP);
                    waitDuelFrames(0x78);
                } else if (DUEL->cpuResult != -1 && CARD_OF(OPP, DUEL->cpuResult).card[2] < 2) {
                    waitDuelFrames(0x3C);
                    DUEL->playedFromSlot = KAW_playCardFromHand(DUEL->cpuResult, OPP);
                    waitDuelFrames(0x78);
                }
                DUEL->step = 0x27;
                break;
            }
            DUEL->playedFromSlot = -1;
            DUEL->messageWanted = 0xF;
            DUEL->step++;
            break;
        case 0x24:
            DUEL->unk481 = 2;
            DUEL->unk482 = 7;
            if (PAD_PRESSED(OPP) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                KAW_openCardSelect(OPP);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(OPP) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->messageWanted = 0x17;
                DUEL->step = 0x26;
            } else if (PAD_PRESSED(OPP) & PAD_SQUARE) {
                playSoundEffect(0xA0);
                DUEL->step = 9;
                DUEL->returnStep = 0x24;
                DUEL->viewPlayer = OPP;
            }
            break;
        case 0x25:
            DUEL->unk481 = 8;
            if (KAW_tickCardCursor(OPP, 4) == 0) {
                i = PLAYER(OPP)->hand[DUEL->cursorSlot];
                if (DUEL->cursorSlot == 4) {
                    DUEL->playedFromSlot = 4;
                    KAW_playOnlineDeckTop(OPP);
                } else {
                    if (CARD_OF(OPP, i).type == 2) {
                        break;
                    }
                    DUEL->playedFromSlot = KAW_playCardFromHand(CUR_CARD, OPP);
                }
                playSoundEffect(0xA0);
                DUEL->messageWanted = 0x16;
                DUEL->step++;
            } else if (PAD_PRESSED(OPP) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->messageWanted = 0x17;
                DUEL->step++;
            } else if (PAD_PRESSED(OPP) & PAD_CROSS) {
                playSoundEffect(0xA1);
                DUEL->step = 0x24;
            }
            if (DUEL->step != 0x25) {
                KAW_closeCardSelect(OPP);
            }
            break;
        case 0x26:
            DUEL->unk481 = 4;
            if (PAD_PRESSED(OPP) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(OPP) & PAD_CROSS) {
                playSoundEffect(0xA1);
                if (DUEL->playedFromSlot >= 0) {
                    KAW_returnPlayedCard(OPP, DUEL->playedFromSlot);
                }
                DUEL->step = 0x23;
            }
            break;
        case 0x27:
            DUEL->unk482 = 7;
            DUEL->humanPlayer = PLAYER(ME)->controller;
            PLAYER_PANEL(ME, 2)->state = 1;
            if (PLAYER(ME)->controller == 2) {
                DUEL->unk481 = -1;
                DUEL->messageWanted = 1;
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 7;
                waitForCpuDecisionNoTutorial();
                if (DUEL->cpuResult == -2) {
                    waitDuelFrames(0x3C);
                    KAW_playOnlineDeckTop(ME);
                    waitDuelFrames(0x78);
                } else if (DUEL->cpuResult != -1 && CARD_OF(ME, DUEL->cpuResult).card[2] < 2) {
                    waitDuelFrames(0x3C);
                    DUEL->playedFromSlot = KAW_playCardFromHand(DUEL->cpuResult, ME);
                    waitDuelFrames(0x78);
                }
                DUEL->step = 0x2B;
                break;
            }
            DUEL->playedFromSlot = -1;
            DUEL->messageWanted = 0xF;
            DUEL->step++;
            break;
        case 0x28:
            DUEL->unk481 = 2;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                KAW_openCardSelect(ME);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->messageWanted = 0x17;
                DUEL->step = 0x2A;
            } else if (PAD_PRESSED(ME) & PAD_SQUARE) {
                playSoundEffect(0xA0);
                DUEL->step = 9;
                DUEL->returnStep = 0x28;
                DUEL->viewPlayer = ME;
            }
            break;
        case 0x29:
            DUEL->unk481 = 8;
            if (KAW_tickCardCursor(ME, 4) == 0) {
                i = PLAYER(ME)->hand[DUEL->cursorSlot];
                if (DUEL->cursorSlot == 4) {
                    DUEL->playedFromSlot = 4;
                    KAW_playOnlineDeckTop(ME);
                } else {
                    if (CARD_OF(ME, i).type == 2) {
                        break;
                    }
                    DUEL->playedFromSlot = KAW_playCardFromHand(CUR_CARD, ME);
                }
                playSoundEffect(0xA0);
                DUEL->messageWanted = 0x16;
                DUEL->step++;
            } else if (PAD_PRESSED(ME) & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL->messageWanted = 0x17;
                DUEL->step++;
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                DUEL->step = 0x28;
            }
            if (DUEL->step != 0x29) {
                KAW_closeCardSelect(ME);
            }
            break;
        case 0x2A:
            DUEL->unk481 = 4;
            if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
                break;
            }
            if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA1);
                if (DUEL->playedFromSlot >= 0) {
                    KAW_returnPlayedCard(ME, DUEL->playedFromSlot);
                }
                DUEL->step = 0x27;
            }
            break;
        case 0x2B:
            DUEL->unk47F = -1;
            DUEL->humanPlayer = -1;
            DUEL->unk481 = -1;
            DUEL->unk482 = -1;
            DUEL->messageWanted = 0x18;
            clearBattleLog();
            PLAYER_PANEL(0, 3)->state = 0x11;
            PLAYER_PANEL(1, 3)->state = 0x11;
            waitDuelFrames(0x3C);
            KAW_resolveBattle(0);
            DUEL->step++;
            break;
        case 0x2D:
            DUEL->cursorSlot = -1;
            if (!PLAYER_DATA(0).skipBattleAnimation) {
                DUEL->state = 1;
                waitFrames(2);
                playPolygonBattle();
                waitFrames(2);
                ((Graphics *)&GRAPHICS)->rotX = 0;
                ((Graphics *)&GRAPHICS)->rotY = 0;
                ((Graphics *)&GRAPHICS)->rotZ = 0;
                ((Graphics *)&GRAPHICS)->posX = 0;
                ((Graphics *)&GRAPHICS)->posY = 0;
                ((Graphics *)&GRAPHICS)->posZ = 0;
                ((Graphics *)&GRAPHICS)->targetPitch = 0;
                ((Graphics *)&GRAPHICS)->targetDistance = 0x1C0;
                ((Graphics *)&GRAPHICS)->targetHeight = 0;
                ((Graphics *)&GRAPHICS)->targetYaw = 0;
                ((Graphics *)&GRAPHICS)->targetModel = -1;
                ((Graphics *)&GRAPHICS)->snapCamera = 1;
                waitFrames(2);
                DUEL->state = 6;
                clearKanjiPage(0xF);
            } else {
                PLAYER_PANEL(0, 9)->state = 4;
                PLAYER_PANEL(1, 9)->state = 4;
                PLAYER_PANEL(0, 3)->state = 0xD;
                PLAYER_PANEL(1, 3)->state = 0xD;
            }
            DUEL->step++;
            break;
        case 0x2E:
            PLAYER(0)->stats[0] = PLAYER(0)->hpAfterBattle;
            PLAYER(1)->stats[0] = PLAYER(1)->hpAfterBattle;
            if (PLAYER(ME)->stats[0] != PLAYER(ME)->displayedStats[0] ||
                PLAYER(OPP)->stats[0] != PLAYER(OPP)->displayedStats[0]) {
                break;
            }
            if (KAW_checkKnockout(ME) == 0 && KAW_checkKnockout(OPP) == 0) {
                DUEL->step = 0x30;
                break;
            }
            DUEL->step++;
            break;
        case 0x2F:
            if (PLAYER(DUEL->winner)->wins == 2) {
                DUEL->messageWanted = 0x13;
                isCrossPressedByTurnPlayer();
                DUEL->step = 0x30;
            } else if (PLAYER(DUEL->winner)->wins == 3) {
                DUEL->messageWanted = 0x11;
                isCrossPressedByTurnPlayer();
                DUEL->step = 0x32;
            } else {
                DUEL->messageWanted = 0x12;
                isCrossPressedByTurnPlayer();
                DUEL->step = 0x30;
            }
            break;
        case 0x30:
            DUEL->unk47F = 2;
            DUEL->humanPlayer = PLAYER(ME)->controller;
            DUEL->unk482 = 8;
            DUEL->unk481 = 9;
            DUEL->step++;
            break;
        case 0x31:
            for (i = 0; i < 2; i++) {
                PLAYER(i)->stats[1] = PLAYER(i)->baseAttackPowers[0];
                PLAYER(i)->stats[2] = PLAYER(i)->baseAttackPowers[1];
                PLAYER(i)->stats[3] = PLAYER(i)->baseAttackPowers[2];
                j = takePlayedCard(i);
                if (j != -1) {
                    SPRITE_KIND(j) = 6;
                    discardCardToOfflineDeck(j, i);
                }
                if (PLAYER_PANEL(i, 2)->state < 4) {
                    PLAYER_PANEL(i, 2)->state = 4;
                }
            }
            DUEL->turnPlayer ^= 1;
            DUEL->step = 2;
            break;
        case 0:
        case 0x2C:
        case 0x32:
        case 0x33:
            DUEL->step++;
            break;
        case 0x34:
            break;
        }
    }
}
