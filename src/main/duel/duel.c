#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/duel.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/hud_panels.h"
#include "dcb/duel_session.h"
#include "dcb/card_db.h"
#include "dcb/duel_setup.h"
#include "dcb/card_zones.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/duel_util.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/menu.h"
#include "dcb/dialog.h"
#include "dcb/partner_level.h"
#include "dcb/hacking_shell.h"
#include "dcb/game_exit.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"
#include "dcb/pad.h"

void runDuelTurnLoop(void) {
    char message[0x88];
    s32 devolveOutcome;
    s32 i;
    s32 j;
    s32 cardId;
    s8 handSlot;
    DigivolveCardData *option;
    s32 knockedOut;
    Player *firstAttacker;
    Player *secondAttacker;

    while (DUEL->state < 0) {
        waitFrames(FRAME_INTERVAL);
    }
    DUEL->stopTurnLoop = 0;
    for (;;) {
        waitDuelFrames(1);
        func_801EAB4C();
        /* steps 1-10: Preparation Phase (draw, place a Digimon), 11-13: DP
           slots, 14-22: Digivolve Phase, 23-36: Battle Phase, 37: end of
           turn, 38-39: duel over */
        /* pad bits: 0x10 triangle, 0x20 circle, 0x40 cross, 0x80 square */
        switch (DUEL->step) {
        case 0:
            PLAYER_PANEL(0, HUD_DECK)->state = 1;
            PLAYER_PANEL(1, HUD_DECK)->state = 1;
            DUEL_MSG_BAR.next = -1;
            DUEL_MSG_BAR.next2 = -1;
            waitFrames(0x1E);
            /* against opponent 0x8C, partner cards in the player's deck move to
               the bottom of the Online Deck and the hacking sequence plays */
            if (PLAYER(1)->controller == 1 && ((SessionData *)SESSION_DATA)->opponentDeckIndex == 0x8C) {
                for (i = 0, j = 0; i < 30; i++) {
                    if ((u32)findPartnerSlot(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                    if ((u32)findArmorPartnerSlot(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                }
                if (j) {
                    for (i = 0; i < 3; i++) {
                        cardId = takePartnerCardFromOnlineDeck(0);
                        if (cardId == -1) {
                            break;
                        }
                        for (j = 0; j < 29; j++) {
                            PLAYER(0)->onlineDeck[j] = PLAYER(0)->onlineDeck[j + 1];
                        }
                        PLAYER(0)->onlineDeck[29] = cardId;
                    }
                    spawnTask(0, -1, 0, 0x800, runHackingSequence, 2, getCurrentTaskId(), 0, 0);
                    waitFrames(0x7FFFFFFF);
                }
            }
            DUEL->step++;
            break;
        case 1:
            DUEL->awaitingInput = 0;
            if (ME == 0) {
                PLAYER_PANEL(0, HUD_PANEL_5)->state = 1;
                PLAYER_PANEL(1, HUD_PANEL_5)->state = 6;
            } else {
                PLAYER_PANEL(0, HUD_PANEL_5)->state = 6;
                PLAYER_PANEL(1, HUD_PANEL_5)->state = 1;
            }
            PLAYER_PANEL(0, HUD_DECK)->state = 1;
            PLAYER_PANEL(1, HUD_DECK)->state = 1;
            PLAYER(0)->attackChoice = 3;
            PLAYER(1)->attackChoice = 3;
            DUEL->playedFromSlot = -1;
            DUEL->dpFromSlot = -1;
            DUEL->cursorMode = -1;
            DUEL->step++;
            break;
        case 2:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.phase = 0;
            DUEL_MSG_BAR.playerLabel = PLAYER(ME)->controller;
            DUEL_MSG_BAR.next = 0;
            DUEL_MSG_BAR.next2 = 0;
            waitDuelFrames(0x3C);
            while (1) {
            wait:
                if (PLAYER_PANEL(0, HUD_DECK)->state != 4) {
                    goto wait;
                }
                if (func_801EC570(ME) == -1) {
                    break;
                }
                waitFrames(0x14);
                func_801FA780(ME);
            }
            DUEL->step++;
            break;
        case 3:
            DUEL->awaitingInput = 0;
            D_801D83D7 = 0;
            if (getActiveDigimonCard(ME) >= 0) {
                DUEL->step = 4;
            } else if (checkHandHasDigimonCard(ME) != 0) {
                if (countOnlineDeckCards(ME) == 0) {
                    D_801D83D4 = 2;
                    sprintf(message, "There are no more Cards, so %s loses!", PLAYER(ME)->name);
                    initDialog((u8 *)&DUEL_DIALOG, message, 0);
                    runDuelMessageWindow();
                    DUEL->winner = ME ^ 1;
                    DUEL->step = 0x26;
                } else {
                    D_801D83D4 = 1;
                    if (PLAYER(ME)->controller != 1) {
                        initDialog((u8 *)&DUEL_DIALOG, "Redrawing Cards because there are\nno Digimon Cards.", 0);
                        runDuelMessageWindow();
                    }
                    DUEL->step = 6;
                }
            } else {
                DUEL->step = 4;
            }
            break;
        case 4:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.next = 0;
            DUEL_MSG_BAR.next2 = 0;
            if (countOnlineDeckCards(ME) == 0) {
                DUEL->step = 8;
            } else if (PLAYER(ME)->controller == 1) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 1;
                waitForCpuDecision();
                if (DUEL->cpuResult != 0) {
                    waitDuelFrames(0x1E);
                    DUEL->step = 6;
                } else {
                    DUEL->step = 8;
                }
            } else {
                DUEL->step++;
            }
            break;
        case 5:
            DUEL->awaitingInput = 1;
            DUEL_MSG_BAR.next2 = 1;
            if (PAD_STATES[ME]->pressed & PAD_TRIANGLE) {
                playSoundEffect(0xA0);
                DUEL_MSG_BAR.next = 3;
                DUEL_MSG_BAR.next2 = 0;
                do {
                    initDialog((u8 *)&DUEL_DIALOG, "This will discard all Cards.\nIs this OK?", 1);
                    runDuelMessageWindow();
                    switch (CHOICE) {
                    case 0:
                    case 2:
                        if (DUEL->tutorial != 0) {
                            func_801EA8B4(0x78, "Please press \"Yes\"!");
                        } else {
                            DUEL->step = 4;
                        }
                        break;
                    case 1:
                        PLAYER(ME)->bonusFlags |= 0x20;
                        DUEL->step++;
                        break;
                    }
                } while (DUEL->step == 5);
            } else if (PAD_STATES[ME]->pressed & PAD_CROSS) {
                playSoundEffect(0xA0);
                DUEL->step = 8;
            } else if (PAD_STATES[ME]->pressed & PAD_SQUARE) {
                playSoundEffect(0xA0);
                DUEL->step = 7;
                DUEL->returnStep = 4;
                DUEL->viewPlayer = ME;
            }
            break;
        case 6:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.next = 0x10;
            DUEL_MSG_BAR.next2 = 0;
            func_801ECC58(ME);
            DUEL->step = 3;
            break;
        case 7:
            DUEL->awaitingInput = 1;
            D_801D83D7 = 8;
            if (DUEL->returnStep != 0x19) {
                func_801EC4CC(DUEL->viewPlayer);
            }
            func_801EBACC(DUEL->viewPlayer, 0);
            waitDuelFrames(0x14);
            for (;;) {
                waitDuelFrames(1);
                func_801EBACC(DUEL->viewPlayer, 0);
                if (PAD_STATES[DUEL->viewPlayer]->pressed & PAD_TRIANGLE) {
                    playSoundEffect(0xA1);
                    if (DUEL->returnStep == 0x19) {
                        PLAYER_PANEL(0, HUD_ATTACK)->state = 1;
                        PLAYER_PANEL(1, HUD_ATTACK)->state = 1;
                    }
                    func_801EC528(DUEL->viewPlayer);
                    DUEL->cursorSlot = -1;
                    DUEL->step = DUEL->returnStep;
                    waitDuelFrames(0x14);
                    break;
                }
            }
            break;
        case 8:
            DUEL->awaitingInput = 0;
            if (getActiveDigimonCard(ME) >= 0) {
                if (PLAYER(ME)->controller == 1) {
                    DUEL->step = 0xB;
                } else {
                    DUEL->step = 0xA;
                }
            } else {
                D_801D83D4 = 4;
                if (PLAYER(ME)->controller == 1) {
                    DUEL->cpuPlayer = ME;
                    DUEL->cpuRequest = 2;
                    waitForCpuDecision();
                    if (DUEL->cpuResult == -1) {
                        for (i = 0; i < 4; i++) {
                            if (PLAYER(ME)->hand[i] != -1 && PLAYER(ME)->cards[(s8)(PLAYER(ME)->hand[i] % 30)].type == 0) {
                                DUEL->cpuResult = PLAYER(ME)->hand[i];
                                break;
                            }
                        }
                    }
                    CUR_CARD = DUEL->cpuResult;
                    func_801EA558(CUR_CARD, ME);
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    i = findPartnerSlot(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (getSelectedArmorIndex(ME, getPartnerIndex(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            waitDuelFrames(0x3C);
                            func_801F6214(1, ME);
                            armorDigivolvePartner(ME, i);
                        }
                    }
                    waitDuelFrames(0x78);
                    DUEL->step = 0xB;
                } else {
                    DUEL->step++;
                    func_801EC4CC(ME);
                }
            }
            break;
        case 9:
            DUEL->awaitingInput = 1;
            if (countOnlineDeckCards(ME) != 0) {
                D_801D83D7 = 5;
            } else {
                D_801D83D7 = 3;
            }
            if (func_801EBACC(ME, 1) == 0) {
                if (PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].type == 0) {
                    playSoundEffect(0xA0);
                    func_801EC528(ME);
                    func_801EA558(CUR_CARD, ME);
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    i = findPartnerSlot(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (getSelectedArmorIndex(ME, getPartnerIndex(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            D_801D83D4 = 5;
                            initDialog((u8 *)&DUEL_DIALOG, "Do you want to Armor Digivolve?", 1);
                            runDuelMessageWindow();
                            switch (CHOICE) {
                            case 0:
                                if (DUEL->playedFromSlot >= 0) {
                                    func_801EC84C(CUR_CARD, ME, DUEL->playedFromSlot);
                                    DUEL->playedFromSlot = -1;
                                    DUEL->step = 8;
                                } else {
                                    DUEL->step = 3;
                                }
                                break;
                            case 1:
                                func_801F6214(1, ME);
                                armorDigivolvePartner(ME, i);
                                PLAYER(ME)->bonusFlags |= 8;
                                func_801FA4E4(ME);
                                DUEL->step = 0xB;
                                break;
                            case 2:
                                DUEL->step++;
                                break;
                            }
                        } else {
                            DUEL->step++;
                        }
                    } else {
                        DUEL->step++;
                    }
                }
            } else if ((PAD_STATES[ME]->pressed & PAD_TRIANGLE) && countOnlineDeckCards(ME) != 0) {
                playSoundEffect(0xA1);
                func_801EC528(ME);
                DUEL->step = 4;
            }
            break;
        case 10:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.next2 = 0;
            DUEL_MSG_BAR.next = 6;
            initDialog((u8 *)&DUEL_DIALOG, "Is it OK to end the Preparation Phase?", 1);
            runDuelMessageWindow();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->tutorial != 0) {
                    func_801EA8B4(0x78, "Press \"Yes\" to go to the next Phase!");
                } else {
                    if (DUEL->playedFromSlot >= 0) {
                        func_801EC84C(CUR_CARD, ME, DUEL->playedFromSlot);
                        DUEL->playedFromSlot = -1;
                    }
                    DUEL->step = 4;
                }
                break;
            case 1:
                if (DUEL->playedFromSlot >= 0) {
                    func_801FA4E4(ME);
                }
                DUEL->step++;
                break;
            }
            break;
        case 11:
            DUEL->awaitingInput = 0;
            if (PLAYER(ME)->controller == 1) {
                DUEL->playedFromSlot = -2;
                DUEL->dpFromSlot = -1;
                DUEL->discardedFromSlot = -1;
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 3;
                waitForCpuDecision();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0xE;
                } else {
                    DUEL_MSG_BAR.phase = 1;
                    DUEL_MSG_BAR.next = 7;
                    CUR_CARD = DUEL->cpuResult;
                    DUEL->playedFromSlot = func_801ECBCC(CUR_CARD, ME);
                    waitDuelFrames(0x78);
                    DUEL->step = 0xE;
                }
            } else {
                DUEL->step++;
            }
            break;
        case 12:
            DUEL->awaitingInput = 0;
            DUEL->playedFromSlot = -2;
            DUEL->dpFromSlot = -1;
            DUEL->discardedFromSlot = -1;
            if (checkHandHasDigimonCard(ME) != 0) {
                DUEL->step = 0xE;
            } else if (countEmptyDpSlots(ME) != 0) {
                func_801EC4CC(ME);
                DUEL->step++;
            } else {
                DUEL->step = 0xE;
            }
            break;
        case 13:
            DUEL->awaitingInput = 1;
            DUEL_MSG_BAR.phase = 1;
            DUEL_MSG_BAR.next = 7;
            DUEL_MSG_BAR.next2 = 7;
            if (func_801EBACC(ME, 2) == 0) {
                i = PLAYER(ME)->hand[DUEL->cursorSlot];
                if (PLAYER(ME)->cards[i % 30].type == 0) {
                    playSoundEffect(0xA0);
                    DUEL->dpFromSlot = func_801ECBCC(CUR_CARD, ME);
                    DUEL->step++;
                }
            } else if (PAD_STATES[ME]->pressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
            }
            if (DUEL->step != 0xD) {
                func_801EC528(ME);
            }
            break;
        case 14:
            DUEL->awaitingInput = 0;
            if (PLAYER(ME)->controller == 1) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 4;
                waitForCpuDecision();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0x13;
                } else {
                    DUEL_MSG_BAR.phase = 1;
                    DUEL_MSG_BAR.next = 8;
                    PLAYER_PANEL(ME, HUD_STATUS)->state = 6;
                    waitDuelFrames(0x1E);
                    CUR_CARD = DUEL->cpuResult;
                    DUEL->playedFromSlot = func_801ECB40(CUR_CARD, ME);
                    waitDuelFrames(0x1E);
                    DUEL->step = 0x10;
                }
            } else if (checkHandHasDigivolveCard(ME) != 0) {
                DUEL->playedFromSlot = -1;
                DUEL->step = 0x13;
            } else {
                DUEL->step++;
                func_801EC4CC(ME);
                PLAYER_PANEL(ME, HUD_STATUS)->state = 6;
                DUEL_MSG_BAR.phase = 1;
                DUEL_MSG_BAR.next = 8;
                DUEL_MSG_BAR.next2 = 6;
                waitDuelFrames(0x1E);
            }
            break;
        case 15:
            DUEL->awaitingInput = 1;
            if (func_801EBACC(ME, 5) == 0) {
                i = PLAYER(ME)->hand[DUEL->cursorSlot];
                if (PLAYER(ME)->cards[i % 30].type == 2) {
                    playSoundEffect(0xA0);
                    DUEL->playedFromSlot = func_801ECB40(CUR_CARD, ME);
                    if (func_801EA374(ME) != 0) {
                        initDialog((u8 *)&DUEL_DIALOG, "This Digivolve Option has no Effect.\nDo you still want to use it?", 1);
                        runDuelMessageWindow();
                        switch (CHOICE) {
                        case 0:
                        case 2:
                            func_801EC8E0(ME, DUEL->playedFromSlot);
                            DUEL->playedFromSlot = -2;
                            break;
                        case 1:
                            func_801EC528(ME);
                            DUEL->step++;
                            break;
                        }
                    } else {
                        DUEL->step++;
                    }
                }
            } else if (PAD_STATES[ME]->pressed & (PAD_TRIANGLE | PAD_CIRCLE)) {
                DUEL->cursorSlot = -1;
                if (PAD_STATES[ME]->pressed & PAD_CIRCLE) {
                    playSoundEffect(0xA0);
                    func_801EC528(ME);
                    PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                    DUEL->step = 0x13;
                } else if (PAD_STATES[ME]->pressed & PAD_TRIANGLE) {
                    playSoundEffect(0xA1);
                    if (DUEL->dpFromSlot >= 0) {
                        func_801ECA30(peekDpSlotTop(ME), ME, DUEL->dpFromSlot);
                        DUEL->dpFromSlot = -1;
                    }
                    PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                    DUEL->step = 0xB;
                }
            }
            break;
        case 16:
            DUEL->awaitingInput = 0;
            D_801D83D4 = 9;
            devolveOutcome = 0;
            if (func_801EA374(ME) == 0) {
                option = (DigivolveCardData *)PLAYER(ME)->cards[getPlayedCard(ME) % 30].card;
                switch (option->effect) {
                case 4:
                    initDialog((u8 *)&DUEL_DIALOG, "Current Digimon will be discarded,\ndo you still want to \"Digi-devolve\"?", 1);
                    if (PLAYER(ME)->controller != 1) {
                        runDuelMessageWindow();
                    } else {
                        CHOICE = 1;
                    }
                    switch (CHOICE) {
                    case 0:
                    case 2:
                        devolveOutcome = 2;
                        func_801EC8E0(ME, DUEL->playedFromSlot);
                        DUEL->playedFromSlot = -2;
                        PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                        break;
                    case 1:
                        devolveOutcome = 3;
                        func_801F6214(9, ME);
                        func_801EC608(getActiveDigimonCard(ME), ME);
                        PLAYER(ME)->statPenalty = 0;
                        placeActiveDigimon(getActiveDigimonCard(ME), ME);
                        PLAYER(ME)->stats[0] *= 2;
                        waitDuelFrames(0x14);
                        break;
                    }
                    break;
                case 7:
                    initDialog((u8 *)&DUEL_DIALOG, "Your Digimon's Level will become *e3,\ndo you still want to \"Armor Digi-devolve\"?", 1);
                    if (PLAYER(ME)->controller != 1) {
                        runDuelMessageWindow();
                    } else {
                        CHOICE = 1;
                    }
                    switch (CHOICE) {
                    case 0:
                    case 2:
                        devolveOutcome = 2;
                        func_801EC8E0(ME, DUEL->playedFromSlot);
                        DUEL->playedFromSlot = -2;
                        PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                        break;
                    case 1:
                        devolveOutcome = 3;
                        func_801F6214(8, ME);
                        armorDevolvePartner(ME, findArmorPartnerSlot(ME, PLAYER(ME)->cards[getActiveDigimonCard(ME) % 30].id));
                        break;
                    }
                    break;
                default:
                    devolveOutcome = 1;
                    break;
                }
            }
            if (devolveOutcome == 0 || devolveOutcome == 3) {
                DUEL->discardedFromSlot = DUEL->playedFromSlot;
                i = takePlayedCard(ME);
                SPRITE_KIND(i) = 8;
                discardCardToOfflineDeck(i, ME);
                PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                func_801EC528(ME);
            }
            switch (devolveOutcome) {
            case 0:
                DUEL->step = 0x13;
                break;
            case 1:
                DUEL->step = 0x11;
                break;
            case 2:
                DUEL->step = 0xE;
                break;
            case 3:
                PLAYER(ME)->bonusFlags |= 0x40000000;
                DUEL->step = 0x17;
                break;
            }
            if (PLAYER(ME)->controller == 1) {
                waitDuelFrames(0x3C);
            }
            break;
        case 17:
            DUEL->awaitingInput = 0;
            DUEL->step = 0x12;
            break;
        case 18:
            DUEL->awaitingInput = 1;
            if (PLAYER(ME)->controller == 1) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 5;
                waitForCpuDecision();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0x17;
                    i = 0;
                    func_801EC528(ME);
                } else {
                    waitDuelFrames(0x3C);
                    CUR_CARD = DUEL->cpuResult;
                    i = 0;
                }
            } else {
                D_801D83D7 = 6;
                i = func_801EBACC(ME, 6);
                if (i != 0) {
                    if (PAD_STATES[ME]->pressed & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (DUEL->playedFromSlot >= 0) {
                            func_801EC8E0(ME, DUEL->playedFromSlot);
                            DUEL->playedFromSlot = -2;
                        }
                        DUEL->step = 0x13;
                        func_801EC528(ME);
                        PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                    } else if (PAD_STATES[ME]->pressed & PAD_TRIANGLE) {
                        playSoundEffect(0xA1);
                        if (DUEL->playedFromSlot >= 0) {
                            func_801EC8E0(ME, DUEL->playedFromSlot);
                            DUEL->playedFromSlot = -2;
                        }
                        DUEL->step = 0xE;
                        func_801EC528(ME);
                        PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                    }
                }
            }
            if (i == 0 && func_801E9F5C(CUR_CARD, ME) == 0) {
                func_801EC528(ME);
                option = (DigivolveCardData *)PLAYER(ME)->cards[getPlayedCard(ME) % 30].card;
                switch (option->effect) {
                case 0:
                    func_801F6214(3, ME);
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 1:
                    func_801F6214(4, ME);
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 2:
                    func_801F6214(5, ME);
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 3:
                    func_801F6214(7, ME);
                    func_801EC608(getActiveDigimonCard(ME), ME);
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 5:
                    func_801F6214(2, ME);
                    armorDevolvePartner(ME, findArmorPartnerSlot(ME, PLAYER(ME)->cards[getActiveDigimonCard(ME) % 30].id));
                    while (getActiveDigimonCard(ME) != -1) {
                        func_801EC608(getActiveDigimonCard(ME), ME);
                        waitDuelFrames(0x14);
                    }
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 6:
                    func_801F6214(6, ME);
                    armorDevolvePartner(ME, findArmorPartnerSlot(ME, PLAYER(ME)->cards[getActiveDigimonCard(ME) % 30].id));
                    func_801EC608(getActiveDigimonCard(ME), ME);
                    PLAYER(ME)->statPenalty = 0;
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                }
                PLAYER(ME)->bonusFlags |= 8;
                func_801FA4E4(ME);
                DUEL->step = 0x17;
            }
            if (DUEL->step != 0x12 && DUEL->step != 0xE && DUEL->step != 0x13) {
                i = takePlayedCard(ME);
                SPRITE_KIND(i) = 8;
                discardCardToOfflineDeck(i, ME);
                PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
                waitDuelFrames(0x14);
                PLAYER(ME)->bonusFlags |= 0x40000000;
            }
            break;
        case 19:
            DUEL->awaitingInput = 0;
            if (PLAYER(ME)->controller == 1) {
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 5;
                waitForCpuDecision();
                if (DUEL->cpuResult == -1) {
                    DUEL->step = 0x17;
                } else {
                    DUEL_MSG_BAR.phase = 1;
                    DUEL_MSG_BAR.next = 9;
                    func_801F6214(0, ME);
                    CUR_CARD = DUEL->cpuResult;
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                }
            } else if (checkHandHasDigimonCard(ME) != 0) {
                DUEL->step = 0x16;
            } else if (func_801EA374(ME) != 0) {
                DUEL->step = 0x16;
            } else {
                DUEL->step++;
            }
            break;
        case 20:
            DUEL->awaitingInput = 0;
            func_801EC4CC(ME);
            DUEL->step++;
            break;
        case 21:
            DUEL->awaitingInput = 1;
            DUEL_MSG_BAR.phase = 1;
            DUEL_MSG_BAR.next = 9;
            if (func_801EBACC(ME, 3) == 0) {
                if (func_801E9F5C(PLAYER(ME)->hand[DUEL->cursorSlot], ME) == 0) {
                    func_801EC528(ME);
                    func_801F6214(0, ME);
                    DUEL->playedFromSlot = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    PLAYER(ME)->bonusFlags |= 8;
                    func_801FA4E4(ME);
                }
            } else if (PAD_STATES[ME]->pressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                DUEL->step++;
                func_801EC528(ME);
            } else if (PAD_STATES[ME]->pressed & PAD_TRIANGLE) {
                playSoundEffect(0xA1);
                func_801ECD68();
                func_801EC528(ME);
            }
            break;
        case 22:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.phase = 1;
            DUEL_MSG_BAR.next2 = 0;
            DUEL_MSG_BAR.next = 0xA;
            PLAYER_PANEL(ME, HUD_STATUS)->state = 1;
            initDialog((u8 *)&DUEL_DIALOG, "Is it OK to end the Digivolve Phase?", 1);
            runDuelMessageWindow();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->tutorial != 0) {
                    func_801EA8B4(0x78, "Choose \"Yes\" to go to next Phase!");
                } else if (DUEL->playedFromSlot == -2) {
                    DUEL->step = 0xE;
                } else {
                    func_801ECD68();
                }
                break;
            case 1:
                DUEL->step++;
                break;
            }
            break;
        case 23:
            DUEL->awaitingInput = 0;
            PLAYER(ME)->hasBattled = 1;
            PLAYER(ME)->battleCard = PLAYER(ME)->cards[getActiveDigimonCard(ME) % 30].card;
            if (getActiveDigimonCard(OPP) == -1) {
                if (PLAYER(ME)->controller != 1) {
                    DUEL_MSG_BAR.phase = 2;
                    DUEL_MSG_BAR.next = 0xB;
                    sprintf(message, "Since %s has no Digimon,\nthere is no Battle Phase.", PLAYER(OPP)->name);
                    initDialog((u8 *)&DUEL_DIALOG, message, 0);
                    runDuelMessageWindow();
                }
                DUEL->step = 0x25;
            } else {
                DUEL->playedFromSlot = -1;
                DUEL->dpFromSlot = -1;
                DUEL->step++;
            }
            break;
        case 24:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.phase = 2;
            DUEL_MSG_BAR.next = 0xC;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->controller == 1) {
                    DUEL->cpuPlayer = i;
                    if (DUEL->tutorial != 0) {
                        DUEL->cpuRequest = 0;
                    } else {
                        DUEL->cpuRequest = 6;
                    }
                }
            }
            PLAYER_PANEL(0, HUD_ATTACK)->state = 1;
            PLAYER_PANEL(1, HUD_ATTACK)->state = 1;
            waitFrames(0x1E);
            DUEL->step++;
            break;
        case 25:
            DUEL->awaitingInput = 1;
            D_801D83D7 = 2;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->controller == 1) {
                    if (DUEL->cpuRequest == 0 && PLAYER(i)->attackChoice == 3) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = DUEL->cpuResult;
                    }
                } else if (PLAYER(i)->attackChoice == 3) {
                    /* circle, triangle and cross pick the attack, square views the cards */
                    if (PAD_STATES[i]->pressed & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = 0;
                    } else if (PAD_STATES[i]->pressed & PAD_TRIANGLE) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = 1;
                    } else if (PAD_STATES[i]->pressed & PAD_CROSS) {
                        playSoundEffect(0xA0);
                        PLAYER(i)->attackChoice = 2;
                    } else if (PAD_STATES[i]->pressed & PAD_SQUARE) {
                        playSoundEffect(0xA0);
                        func_801EC4CC(i);
                        PLAYER_PANEL(0, HUD_ATTACK)->state = 4;
                        PLAYER_PANEL(1, HUD_ATTACK)->state = 4;
                        DUEL->step = 7;
                        DUEL->returnStep = 0x19;
                        DUEL->viewPlayer = i;
                        break;
                    }
                }
            }
            if (PLAYER(0)->attackChoice != 3 && PLAYER(1)->attackChoice != 3) {
                waitDuelFrames(0x78);
                PLAYER_PANEL(0, HUD_ATTACK)->state = 4;
                PLAYER_PANEL(1, HUD_ATTACK)->state = 4;
                for (i = 0; i < 2; i++) {
                    PLAYER(i)->usedAttack = PLAYER(i)->attackChoice;
                    if (((PlayerProfile *)PLAYER_PROFILES)[i].attackCounts[PLAYER(i)->usedAttack] != 0xFFFF) {
                        ((PlayerProfile *)PLAYER_PROFILES)[i].attackCounts[PLAYER(i)->usedAttack]++;
                    }
                }
                waitDuelFrames(0x78);
                DUEL->step++;
            }
            break;
        case 26:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.next = 0xD;
            DUEL_MSG_BAR.playerLabel = PLAYER(OPP)->controller;
            PLAYER_PANEL(OPP, HUD_STATUS)->state = 6;
            waitDuelFrames(0x1E);
            if (PLAYER(OPP)->controller == 1) {
                DUEL_MSG_BAR.next2 = 0;
                DUEL->cpuPlayer = OPP;
                DUEL->cpuRequest = 7;
                waitForCpuDecision();
                if (DUEL->cpuResult == -2) {
                    waitDuelFrames(0x3C);
                    func_801ECAC4(OPP);
                    waitDuelFrames(0x78);
                } else if (DUEL->cpuResult != -1) {
                    if (PLAYER(OPP)->cards[DUEL->cpuResult % 30].card[2] < 2) {
                        waitDuelFrames(0x3C);
                        DUEL->playedFromSlot = func_801ECB40(DUEL->cpuResult, OPP);
                        waitDuelFrames(0x78);
                    }
                }
                DUEL->step = 0x1D;
            } else {
                DUEL->playedFromSlot = -1;
                if (countEmptyHandSlots(OPP) == 4 && countOnlineDeckCards(OPP) == 0) {
                    initDialog((u8 *)&DUEL_DIALOG, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    runDialogForPad(&DUEL_DIALOG, PLAYER(OPP)->controller & 1);
                    DUEL->step = 0x1D;
                } else {
                    func_801EC4CC(OPP);
                    DUEL->step++;
                }
            }
            break;
        case 27:
            DUEL->awaitingInput = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(OPP, 4) == 0) {
                i = PLAYER(OPP)->hand[DUEL->cursorSlot];
                handSlot = DUEL->cursorSlot;
                if (handSlot == 4) {
                    DUEL->playedFromSlot = handSlot;
                    func_801ECAC4(OPP);
                } else {
                    if (PLAYER(OPP)->cards[i % 30].type == 2) {
                        break;
                    }
                    DUEL->playedFromSlot = func_801ECB40(CUR_CARD, OPP);
                }
                playSoundEffect(0xA0);
                initDialog((u8 *)&DUEL_DIALOG, "Do you want to use this Support Card?", 1);
                DUEL->step++;
            } else if (PAD_STATES[OPP]->pressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                initDialog((u8 *)&DUEL_DIALOG, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->step++;
            }
            if (DUEL->step != 0x1B) {
                func_801EC528(OPP);
            }
            break;
        case 28:
            DUEL->awaitingInput = 0;
            D_801D83D7 = 0;
            runDialogForPad(&DUEL_DIALOG, PLAYER(OPP)->controller & 1);
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->tutorial != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    initDialog((u8 *)&DUEL_DIALOG, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->playedFromSlot >= 0) {
                        func_801EC8E0(OPP, DUEL->playedFromSlot);
                    }
                    DUEL->step = 0x1A;
                }
                break;
            case 1:
                DUEL->step++;
                break;
            }
            break;
        case 29:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.next = 0xE;
            DUEL_MSG_BAR.playerLabel = PLAYER(ME)->controller;
            PLAYER_PANEL(ME, HUD_STATUS)->state = 6;
            waitDuelFrames(0x1E);
            if (PLAYER(ME)->controller == 1) {
                DUEL_MSG_BAR.next2 = 0;
                DUEL->cpuPlayer = ME;
                DUEL->cpuRequest = 7;
                waitForCpuDecision();
                if (DUEL->cpuResult == -2) {
                    waitDuelFrames(0x3C);
                    func_801ECAC4(ME);
                    waitDuelFrames(0x78);
                } else if (DUEL->cpuResult != -1) {
                    if (PLAYER(ME)->cards[DUEL->cpuResult % 30].card[2] < 2) {
                        waitDuelFrames(0x3C);
                        DUEL->playedFromSlot = func_801ECB40(DUEL->cpuResult, ME);
                        waitDuelFrames(0x78);
                    }
                }
                DUEL->step = 0x20;
            } else {
                DUEL->playedFromSlot = -1;
                if (countEmptyHandSlots(ME) == 4 && countOnlineDeckCards(ME) == 0) {
                    initDialog((u8 *)&DUEL_DIALOG, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    runDialogForPad(&DUEL_DIALOG, PLAYER(ME)->controller & 1);
                    DUEL->step = 0x20;
                } else {
                    func_801EC4CC(ME);
                    DUEL->step++;
                }
            }
            break;
        case 30:
            DUEL->awaitingInput = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(ME, 4) == 0) {
                i = PLAYER(ME)->hand[DUEL->cursorSlot];
                handSlot = DUEL->cursorSlot;
                if (handSlot == 4) {
                    DUEL->playedFromSlot = handSlot;
                    func_801ECAC4(ME);
                } else {
                    if (PLAYER(ME)->cards[i % 30].type == 2) {
                        break;
                    }
                    DUEL->playedFromSlot = func_801ECB40(CUR_CARD, ME);
                }
                playSoundEffect(0xA0);
                initDialog((u8 *)&DUEL_DIALOG, "Do you want to use this Support Card?", 1);
                DUEL->step++;
            } else if (PAD_STATES[ME]->pressed & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                initDialog((u8 *)&DUEL_DIALOG, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->step++;
            }
            if (DUEL->step != 0x1E) {
                func_801EC528(ME);
            }
            break;
        case 31:
            DUEL->awaitingInput = 0;
            D_801D83D7 = 0;
            runDuelMessageWindow();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->tutorial != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    initDialog((u8 *)&DUEL_DIALOG, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->playedFromSlot >= 0) {
                        func_801EC8E0(ME, DUEL->playedFromSlot);
                    }
                    DUEL->step = 0x1D;
                }
                break;
            case 1:
                DUEL->step++;
                break;
            }
            break;
        case 32:
            DUEL->awaitingInput = 0;
            DUEL_MSG_BAR.next2 = 0;
            DUEL_MSG_BAR.next = 0xF;
            waitDuelFrames(0x3C);
            if (((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation) {
                ATTACK_ICON_TIMER = 0x20;
                addFrameCallback((s32)renderAttackChoiceIcons);
                while (ATTACK_ICON_TIMER != 0) {
                    waitFrames(FRAME_INTERVAL);
                }
                waitFrames(0x14);
            }
            func_801E6AA4(0);
            DUEL->step++;
            break;
        case 33:
            DUEL->awaitingInput = 0;
            D_801D83D4 = 0x11;
            DUEL->step++;
            break;
        case 34:
            DUEL->awaitingInput = 0;
            DUEL->cursorSlot = -1;
            if (!((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation) {
                waitFrames(0x3C);
                DUEL->state = 1;
                waitFrames(2);
                DUEL->inPolygonBattle = 1;
                playPolygonBattle();
                DUEL->inPolygonBattle = 0;
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
            }
            DUEL->step++;
            break;
        case 35:
            DUEL->awaitingInput = 0;
            knockedOut = 0;
            if (((PlayerProfile *)PLAYER_PROFILES)->skipBattleAnimation) {
                firstAttacker = DUEL->firstAttacker;
                secondAttacker = DUEL->secondAttacker;
                j = firstAttacker->controller & 1;
                waitFrames(0x14);
                if (firstAttacker->crash && !secondAttacker->counter) {
                    func_801F6268(0x1B, j);
                    firstAttacker->stats[0] = 10;
                }
                if (secondAttacker->damageTaken == 0) {
                    func_801F6268(0x1C, j);
                } else if (firstAttacker->eatUpHp) {
                    func_801F6268(0x1A, j);
                    i = firstAttacker->stats[0] + firstAttacker->hpGain;
                    showStatChangePopup(j, i, 0);
                    /* HP caps at 9990; a repdigit HP (1110, 2220...) earns a bonus */
                    if (i > 9990) {
                        i = 9990;
                    }
                    firstAttacker->stats[0] = i;
                    if (i != 0 && i % 1110 == 0) {
                        func_801FB444(j, 0x1A);
                    }
                } else if (!firstAttacker->crash) {
                    func_801F6268(0x18, j);
                }
                i = secondAttacker->stats[0] - secondAttacker->damageTaken;
                if (secondAttacker->damageTaken != 0) {
                    showStatChangePopup(j ^ 1, i, 0);
                    if (secondAttacker->damageTaken != 0 && secondAttacker->damageTaken % 1110 == 0) {
                        func_801FB444(j, 0x19);
                    }
                }
                if (i == 0 && firstAttacker->wins == 2) {
                    func_801FB444(j, 0x13);
                }
                if (i < 0) {
                    i = 0;
                }
                secondAttacker->stats[0] = i;
                if (i != 0 && i % 1110 == 0 && secondAttacker->damageTaken != 0) {
                    func_801FB444(j ^ 1, 0x1A);
                }
                waitForStatCountersToSettle();
                if (func_801ECF0C(j ^ 1) != 0) {
                    knockedOut = 1;
                }
                waitFrames(0x14);
                if (secondAttacker->hpAfterBattle != 0) {
                    if (secondAttacker->crash && firstAttacker->damageTaken != 0) {
                        func_801F6268(0x1B, j ^ 1);
                        secondAttacker->stats[0] = 10;
                    }
                    if (firstAttacker->damageTaken == 0) {
                        func_801F6268(0x1C, j ^ 1);
                    } else if (secondAttacker->counter || secondAttacker->eatUpHp) {
                        if (secondAttacker->counter) {
                            func_801F6268(0x19, j ^ 1);
                        }
                        if (secondAttacker->eatUpHp) {
                            func_801F6268(0x1A, j ^ 1);
                            i = secondAttacker->stats[0] + secondAttacker->hpGain;
                            showStatChangePopup(j ^ 1, i, 0);
                            if (i > 9990) {
                                i = 9990;
                            }
                            secondAttacker->stats[0] = i;
                            if (i != 0 && i % 1110 == 0) {
                                func_801FB444(j ^ 1, 0x1A);
                            }
                        }
                    } else if (!secondAttacker->crash) {
                        func_801F6268(0x18, j ^ 1);
                    }
                    i = firstAttacker->stats[0] - firstAttacker->damageTaken;
                    if (firstAttacker->damageTaken != 0) {
                        showStatChangePopup(j, i, 0);
                        if (firstAttacker->damageTaken != 0 && firstAttacker->damageTaken % 1110 == 0) {
                            func_801FB444(j ^ 1, 0x19);
                        }
                    }
                    if (i == 0 && secondAttacker->wins == 2) {
                        func_801FB444(j ^ 1, 0x13);
                    }
                    if (i < 0) {
                        i = 0;
                    }
                    firstAttacker->stats[0] = i;
                    if (i != 0 && i % 1110 == 0 && firstAttacker->damageTaken != 0) {
                        func_801FB444(j, 0x1A);
                    }
                    waitForStatCountersToSettle();
                    if (func_801ECF0C(j) != 0) {
                        knockedOut = 1;
                    }
                }
                removeFrameCallback((s32)renderAttackChoiceIcons);
                if (knockedOut) {
                    DUEL->step++;
                } else {
                    DUEL->step = 0x25;
                }
            } else {
                waitFrames(0x3C);
                PLAYER(0)->stats[0] = PLAYER(0)->hpAfterBattle;
                PLAYER(1)->stats[0] = PLAYER(1)->hpAfterBattle;
                if (PLAYER(ME)->stats[0] == PLAYER(ME)->displayedStats[0] && PLAYER(OPP)->stats[0] == PLAYER(OPP)->displayedStats[0]) {
                    if (func_801ECF0C(ME) != 0) {
                        DUEL->step++;
                    } else if (func_801ECF0C(OPP) != 0) {
                        DUEL->step++;
                    } else {
                        DUEL->step = 0x25;
                    }
                }
            }
            break;
        case 36:
            DUEL->awaitingInput = 0;
            DUEL->step++;
            if (PLAYER(DUEL->winner)->wins == 2 && PLAYER(DUEL->winner ^ 1)->wins == 0) {
                PLAYER(DUEL->winner ^ 1)->bonusFlags |= 0x80;
                PLAYER(DUEL->winner)->bonusFlags |= 0x100;
            }
            if (findArmorPartnerSlot(DUEL->winner, PLAYER(DUEL->winner)->cards[getActiveDigimonCard(DUEL->winner) % 30].id) >= 0) {
                func_801FB444(DUEL->winner, 0xA);
                PLAYER(DUEL->winner)->bonusFlags |= 0x4000;
            } else if (findPartnerSlot(DUEL->winner, PLAYER(DUEL->winner)->cards[getActiveDigimonCard(DUEL->winner) % 30].id) >= 0) {
                func_801FB444(DUEL->winner, 0xA);
                PLAYER(DUEL->winner)->bonusFlags |= 0x4000;
            }
            if (PLAYER(DUEL->winner)->wins == 3) {
                if (PLAYER(DUEL->winner)->unk178_31) {
                    PLAYER(DUEL->winner)->bonusFlags |= 0x20000;
                    PLAYER(DUEL->winner ^ 1)->bonusFlags |= 0x40000;
                }
                sprintf(message, "%d Wins, %d Losses-%s WINS!", PLAYER(DUEL->winner)->wins, PLAYER(DUEL->winner ^ 1)->wins, PLAYER(DUEL->winner)->name);
                initDialog((u8 *)&DUEL_DIALOG, message, 0);
                runDuelMessageWindow();
                DUEL->step = 0x26;
            } else if (getActiveDigimonCard(DUEL->winner ^ 1) == -1 && checkHandHasDigimonCard(DUEL->winner ^ 1) != 0 && countOnlineDeckCards(DUEL->winner ^ 1) == 0) {
                sprintf(message, "Since %s has no more Digimon,\nthe winner is %s!", PLAYER(DUEL->winner ^ 1)->name, PLAYER(DUEL->winner)->name);
                initDialog((u8 *)&DUEL_DIALOG, message, 0);
                runDuelMessageWindow();
                DUEL->step = 0x26;
            }
            break;
        case 37:
            DUEL->awaitingInput = 0;
            D_801D83D1 = PLAYER(ME)->controller;
            DUEL->step++;
            for (i = 0; i < 2; i++) {
                PLAYER(i)->stats[1] = PLAYER(i)->baseAttackPowers[0];
                PLAYER(i)->stats[2] = PLAYER(i)->baseAttackPowers[1];
                PLAYER(i)->stats[3] = PLAYER(i)->baseAttackPowers[2];
                j = takePlayedCard(i);
                if (j != -1) {
                    SPRITE_KIND(j) = 8;
                    discardCardToOfflineDeck(j, i);
                }
                PLAYER_PANEL(0, HUD_STATUS)->state = 1;
                PLAYER_PANEL(1, HUD_STATUS)->state = 1;
            }
            DUEL->turnPlayer ^= 1;
            DUEL->step = 1;
            break;
        case 38:
            DUEL->awaitingInput = 0;
            DUEL->step++;
        case 39:
            DUEL->awaitingInput = 0;
            break;
        }
    }
}
