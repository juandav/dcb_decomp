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

/* jp's tutorial duel: its own turn loop, which steps through the lessons
   (us runs the tutorial inside runDuelTurnLoop, with KAWSEG's
   KAW_tickTutorial), and the pointer it draws over what to press */

extern s32 TUTORIAL_POINTER_TIMER;

void drawTutorialPointer(s32 x, s32 y, s32 mode, s32 otz) {
    switch (mode) {
    case 0:
        KAW_drawSprite(x, y, 0x378, 0x1C0, 0x20, 0x20, 0x360, 0x1E8, 0, 0, 0, 0x80, otz);
        break;
    case 1:
        switch (++TUTORIAL_POINTER_TIMER / 8) {
        case 0:
            KAW_drawSprite(x, y, 0x360, 0x1C0, 0x28, 0x28, 0x360, 0x1E9, 0, 0, 0, 0x80, otz);
            break;
        case 1:
            KAW_drawSprite(x + 4, y + 4, 0x36A, 0x1C0, 0x20, 0x20, 0x360, 0x1E9, 0, 0, 0, 0x80, otz);
            break;
        default:
            KAW_drawSprite(x + 8, y + 8, 0x372, 0x1C0, 0x18, 0x18, 0x360, 0x1E9, 0, 0, 0, 0x80, otz);
            TUTORIAL_POINTER_TIMER = 100;
            break;
        }
        break;
    case 2:
        TUTORIAL_POINTER_TIMER++;
    case 3:
        switch (TUTORIAL_POINTER_TIMER / 8) {
        case 0:
            KAW_drawSprite(x + 8, y + 8, 0x372, 0x1C0, 0x18, 0x18, 0x360, 0x1E9, 0, 0, 0, 0x80, otz);
            break;
        case 1:
            KAW_drawSprite(x + 4, y + 4, 0x36A, 0x1C0, 0x20, 0x20, 0x360, 0x1E9, 0, 0, 0, 0x80, otz);
            break;
        default:
            KAW_drawSprite(x, y, 0x360, 0x1C0, 0x28, 0x28, 0x360, 0x1E9, 0, 0, 0, 0x80, otz);
            TUTORIAL_POINTER_TIMER = 100;
            break;
        }
        break;
    }
}

void moveTutorialCursor(s32 slot, s32 card, s32 player) {
    DUEL->cursorSlot = slot;
    CUR_CARD = card;
    DUEL->cursorPlayer = player;
    KAW_pickCardArtSlot();
}

#define LESSON DUEL->unk449
#define MESSAGE DUEL->tutorialMessage
#define PAD0_PRESSED (PAD_STATES[0]->rawPressed)

void runTutorialTurnLoop(void) {
    s32 i;
    s32 j;

    DUEL->stopTurnLoop = 0;
    while (1) {
        waitDuelFrames(1);
        if (DUEL->stopTurnLoop != 0) {
            break;
        }
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
            waitDuelFrames(0x1E);
            DUEL->unk481 = -1;
            switch (LESSON) {
            case 0:
                MESSAGE = 1;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 3) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                break;
            case 1:
                MESSAGE = 0xF;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                break;
            case 2:
                MESSAGE = 0x3B;
                TUTORIAL_POINTER_TIMER = 0;
                while (1) {
                    waitDuelFrames(1);
                    if (MESSAGE >= 0x3C) {
                        drawTutorialPointer(0x116, 0x78, 2, 0);
                        drawTutorialPointer(0x116, 0x50, 3, 0);
                    }
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x3D) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                break;
            case 3:
                MESSAGE = 0x58;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                break;
            }
            while (KAW_drawCardToHand(ME) != -1) {
                waitDuelFrames(0x14);
            }
            switch (LESSON) {
            case 0:
                MESSAGE = 4;
                TUTORIAL_POINTER_TIMER = 0;
                while (1) {
                    if (MESSAGE == 5) {
                        drawTutorialPointer(0xA4, 0x24, 1, 0);
                    }
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 5) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->step++;
                break;
            case 1:
                MESSAGE = 0x10;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x11) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->step++;
                break;
            case 2:
                MESSAGE = 0x3E;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x3F) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->step = 0xF;
                break;
            case 3:
                MESSAGE = 0x59;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x5A) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->step++;
                break;
            }
            break;
        case 4:
            DUEL->unk481 = -1;
            DUEL->step++;
            DUEL->returnStep = 0;
            break;
        case 5:
            DUEL->unk482 = 0;
            switch (LESSON) {
            case 1:
                DUEL->messageWanted = 2;
                DUEL->step++;
                break;
            case 3:
                if (countOnlineDeckCards(0) == 0x12) {
                    MESSAGE = 0x5D;
                    while (1) {
                        waitDuelFrames(1);
                        if (PAD0_PRESSED & PAD_CIRCLE) {
                            playSoundEffect(0xA0);
                            if (MESSAGE == 0x5E) {
                                DUEL->tutorialVisible = 0;
                                break;
                            }
                            MESSAGE++;
                        }
                    }
                }
                DUEL->messageWanted = 2;
                DUEL->step++;
                break;
            case 0:
            case 2:
                DUEL->step = 0xA;
                break;
            }
            break;
        case 6:
            DUEL->unk481 = 0;
            switch (LESSON) {
            case 1:
                if (DUEL->returnStep == 0) {
                    if (PAD0_PRESSED & (PAD_CIRCLE | PAD_CROSS)) {
                        playSoundEffect(0xA0);
                        DUEL->tutorialVisible = 1;
                        MESSAGE = 0x12;
                    } else if (PAD_PRESSED(ME) & PAD_SQUARE) {
                        DUEL->tutorialVisible = 0;
                        playSoundEffect(0xA0);
                        DUEL->step = 9;
                        DUEL->returnStep = 5;
                        DUEL->viewPlayer = ME;
                    }
                } else if (PAD0_PRESSED & (PAD_CROSS | PAD_SQUARE)) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x14;
                } else if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                    DUEL->step = 0xA;
                }
                break;
            case 3:
                if (countOnlineDeckCards(0) == 0x12) {
                    if (PAD0_PRESSED & (PAD_CROSS | PAD_SQUARE)) {
                        playSoundEffect(0xA0);
                        DUEL->tutorialVisible = 1;
                        MESSAGE = 0x14;
                    } else if (PAD_PRESSED(ME) & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        DUEL->tutorialVisible = 0;
                        DUEL->step = 0xA;
                    }
                } else if (DUEL->returnStep == 0) {
                    if (PAD0_PRESSED & (PAD_CIRCLE | PAD_CROSS)) {
                        playSoundEffect(0xA0);
                        DUEL->tutorialVisible = 1;
                        MESSAGE = 0x12;
                    } else if (PAD_PRESSED(ME) & PAD_SQUARE) {
                        DUEL->tutorialVisible = 0;
                        playSoundEffect(0xA0);
                        DUEL->step = 9;
                        DUEL->returnStep = 5;
                        DUEL->viewPlayer = ME;
                    }
                } else if (PAD0_PRESSED & PAD_CROSS) {
                    DUEL->tutorialVisible = 0;
                    playSoundEffect(0xA0);
                    DUEL->messageWanted = 3;
                    DUEL->step++;
                } else if (PAD_PRESSED(ME) & (PAD_CIRCLE | PAD_SQUARE)) {
                    playSoundEffect(0xA0);
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x5C;
                }
                break;
            }
            break;
        case 7:
            DUEL->unk481 = 4;
            DUEL->messageWanted = 4;
            DUEL->step++;
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
            switch (LESSON) {
            case 2:
                if (DUEL->returnStep == 0x24) {
                    playSoundEffect(0xA0);
                    MESSAGE = 0x54;
                    while (1) {
                        waitDuelFrames(1);
                        if (PAD0_PRESSED & (PAD_CIRCLE | PAD_CROSS)) {
                            playSoundEffect(0xA0);
                            if (MESSAGE == 0x55) {
                                DUEL->tutorialVisible = 0;
                                break;
                            }
                            MESSAGE++;
                        }
                    }
                }
                break;
            case 3:
                if (DUEL->returnStep == 5) {
                    playSoundEffect(0xA0);
                    MESSAGE = 0x5B;
                    do {
                        waitDuelFrames(1);
                    } while (!(PAD0_PRESSED & (PAD_CIRCLE | PAD_CROSS)));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                }
                break;
            }
            while (1) {
                waitDuelFrames(1);
                KAW_tickCardCursor(DUEL->viewPlayer, 0);
                if (PAD_PRESSED(DUEL->viewPlayer) & PAD_CROSS) {
                    if (LESSON == 1 && DUEL->returnStep == 5) {
                        playSoundEffect(0xA0);
                        MESSAGE = 0x13;
                        do {
                            waitDuelFrames(1);
                            drawTutorialPointer(0x6E, 0x5C, 0, 0);
                        } while (!(PAD0_PRESSED & (PAD_CIRCLE | PAD_CROSS)));
                        playSoundEffect(0xA0);
                        DUEL->tutorialVisible = 0;
                    }
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
                    break;
                }
            }
            break;
        case 0xA:
            DUEL->unk482 = 1;
            if (getActiveDigimonCard(ME) >= 0) {
                if (PLAYER(ME)->controller == 2) {
                    DUEL->step = 0xF;
                } else {
                    DUEL->step = 0xD;
                }
            } else {
                switch (LESSON) {
                case 0:
                    CUR_CARD = 0x21;
                    KAW_setStatPenalty(CUR_CARD, ME);
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    waitDuelFrames(0x78);
                    MESSAGE = 6;
                    do {
                        waitDuelFrames(1);
                    } while (!(PAD0_PRESSED & PAD_CIRCLE));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                    DUEL->step = 0xF;
                    break;
                case 1:
                    KAW_openCardSelect(ME);
                    moveTutorialCursor(1, 1, 0);
                    MESSAGE = 0x15;
                    do {
                        waitDuelFrames(1);
                    } while (!(PAD0_PRESSED & PAD_CIRCLE));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                    DUEL->messageWanted = 6;
                    DUEL->step = 0xC;
                    break;
                }
            }
            break;
        case 0xC:
            DUEL->unk481 = 4;
            if (KAW_tickCardCursor(ME, 1) == 0) {
                if (CUR_CARD != 1) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x17;
                } else {
                    playSoundEffect(0xA0);
                    KAW_setStatPenalty(CUR_CARD, ME);
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    DUEL->step++;
                }
            } else if (PAD_PRESSED(ME) & PAD_CROSS) {
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x16;
            }
            if (DUEL->step != 0xC) {
                KAW_closeCardSelect(ME);
            }
            break;
        case 0xD:
            DUEL->unk481 = -1;
            DUEL->messageWanted = 7;
            MESSAGE = 0x5F;
            do {
                waitDuelFrames(1);
            } while (!(PAD0_PRESSED & PAD_CIRCLE));
            playSoundEffect(0xA0);
            DUEL->tutorialVisible = 0;
            DUEL->step++;
        case 0xE:
        case 0x1F:
        case 0x26:
        case 0x2A:
            DUEL->unk481 = 4;
            DUEL->step++;
            break;
        case 0xF:
            DUEL->playedFromSlot = -2;
            DUEL->dpFromSlot = -1;
            DUEL->unk47F = 1;
            DUEL->unk482 = 2;
            DUEL->unk481 = -1;
            DUEL->messageWanted = 8;
            switch (LESSON) {
            case 0:
                playSoundEffect(0xA0);
                KAW_openCardSelect(ME);
                moveTutorialCursor(1, 0x1F, 1);
                waitDuelFrames(0x1E);
                MESSAGE = 7;
                TUTORIAL_POINTER_TIMER = 0;
                while (1) {
                    waitDuelFrames(1);
                    if (MESSAGE >= 8) {
                        drawTutorialPointer(0x59, 0x30, 1, 0);
                    }
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 9) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->playedFromSlot = KAW_chargeDpCard(CUR_CARD, ME);
                moveTutorialCursor(0, 0x1E, 1);
                waitDuelFrames(0x78);
                DUEL->step = 0x13;
                break;
            case 1:
                MESSAGE = 0x18;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                KAW_openCardSelect(ME);
                moveTutorialCursor(3, 3, 0);
                waitDuelFrames(0x1E);
                MESSAGE = 0x19;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x1A) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->step = 0x11;
                DUEL->step = 0x11;
                break;
            case 2:
                playSoundEffect(0xA0);
                KAW_openCardSelect(ME);
                moveTutorialCursor(2, 0x23, 1);
                waitDuelFrames(0x1E);
                MESSAGE = 0x40;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x41) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                waitDuelFrames(0x78);
                DUEL->playedFromSlot = KAW_chargeDpCard(CUR_CARD, ME);
                KAW_closeCardSelect(ME);
                waitDuelFrames(0x78);
                DUEL->step = 0x13;
                break;
            case 3:
                MESSAGE = 0x60;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                KAW_openCardSelect(ME);
                moveTutorialCursor(1, 9, 0);
                waitDuelFrames(0x1E);
                MESSAGE = 0x61;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                waitDuelFrames(0x1E);
                DUEL->step = 0x13;
                break;
            }
            break;
        case 0x11:
            KAW_openCardSelect(ME);
            DUEL->step++;
            break;
        case 0x12:
            DUEL->unk481 = 8;
            if (KAW_tickCardCursor(ME, 2) == 0) {
                if (CUR_CARD != 3) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x1B;
                } else {
                    playSoundEffect(0xA0);
                    DUEL->dpFromSlot = KAW_chargeDpCard(CUR_CARD, ME);
                    moveTutorialCursor(0, 0, 0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x1C;
                    DUEL->cursorMode = 3;
                    TUTORIAL_POINTER_TIMER = 0;
                    while (1) {
                        waitDuelFrames(1);
                        if (MESSAGE == 0x1D) {
                            drawTutorialPointer(0x59, 0x8A, 1, 0);
                        }
                        if (PAD0_PRESSED & (PAD_CIRCLE | PAD_CROSS)) {
                            playSoundEffect(0xA0);
                            if (MESSAGE == 0x1E) {
                                DUEL->tutorialVisible = 0;
                                break;
                            }
                            MESSAGE++;
                        }
                    }
                    DUEL->step++;
                }
            } else if (PAD_PRESSED(ME) & (PAD_TRIANGLE | PAD_CROSS)) {
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x16;
            }
            if (DUEL->step != 0x12) {
                KAW_closeCardSelect(ME);
            }
            break;
        case 0x13:
            DUEL->unk482 = 3;
            switch (LESSON) {
            case 0:
                DUEL->unk482 = 5;
                MESSAGE = 0xA;
                TUTORIAL_POINTER_TIMER = 0;
                while (1) {
                    waitDuelFrames(1);
                    if (MESSAGE == 0xB) {
                        drawTutorialPointer(0x59, 0x3C, 1, 0);
                    }
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0xC) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                KAW_closeCardSelect(ME);
                DUEL->step = 0x20;
                break;
            case 1:
            case 2:
            case 3:
                DUEL->playedFromSlot = -1;
                DUEL->step = 0x1A;
                break;
            }
            break;
        case 0x1A:
            DUEL->unk482 = 5;
            switch (LESSON) {
            case 0:
            case 1:
            case 3:
                if (checkHandHasDigimonCard(ME) != 0 || KAW_checkAnyDigivolve(ME) != 0) {
                    DUEL->step = 0x1E;
                } else {
                    DUEL->messageWanted = 0xA;
                    DUEL->step = 0x1C;
                }
                break;
            case 2:
                DUEL->messageWanted = 0xA;
                KAW_openCardSelect(ME);
                moveTutorialCursor(0, 0x1E, 1);
                waitDuelFrames(0x1E);
                MESSAGE = 0x42;
                while (1) {
                    waitDuelFrames(1);
                    KAW_drawHandHints(1, 3);
                    if (MESSAGE >= 0x43) {
                        drawTutorialPointer(0x40, 0x8E, 0, 0);
                        drawTutorialPointer(0x69, 0x8E, 0, 0);
                    }
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x45) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                for (i = 0; i < 0x1E; i++) {
                    waitDuelFrames(1);
                    KAW_drawHandHints(1, 3);
                }
                playSoundEffect(0xAB);
                playSoundEffect(0xAB);
                DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                while (peekDpSlotTop(ME) != -1) {
                    discardCardToOfflineDeck(peekDpSlotTop(ME), ME);
                    SPRITE_KIND(peekDpSlotTop(ME)) = 6;
                    removeCardFromDpSlots(peekDpSlotTop(ME), ME);
                }
                KAW_closeCardSelect(ME);
                waitDuelFrames(0x78);
                MESSAGE = 0x46;
                TUTORIAL_POINTER_TIMER = 0;
                while (1) {
                    waitDuelFrames(1);
                    if (MESSAGE == 0x46) {
                        drawTutorialPointer(0x116, 0x50, 2, 0);
                    }
                    if (MESSAGE == 0x47) {
                        drawTutorialPointer(0xFE, 7, 1, 0);
                    }
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        TUTORIAL_POINTER_TIMER = 0;
                        if (MESSAGE == 0x48) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->step = 0x20;
                break;
            }
            break;
        case 0x1C:
            KAW_openCardSelect(ME);
            moveTutorialCursor(1, 9, 0);
            MESSAGE = 0x62;
            while (1) {
                waitDuelFrames(1);
                KAW_drawHandHints(ME, 3);
                if (PAD0_PRESSED & PAD_CIRCLE) {
                    playSoundEffect(0xA0);
                    if (MESSAGE == 0x63) {
                        DUEL->tutorialVisible = 0;
                        break;
                    }
                    MESSAGE++;
                }
            }
            DUEL->step++;
            break;
        case 0x1D:
            if (KAW_tickCardCursor(ME, 3) == 0) {
                if (CUR_CARD != 9) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x64;
                } else {
                    playSoundEffect(0xAB);
                    playSoundEffect(0xAB);
                    KAW_closeCardSelect(ME);
                    DUEL->playedFromSlot = KAW_placeDigimonFromHand(CUR_CARD, ME);
                    DUEL->tutorialVisible = 0;
                    while (peekDpSlotTop(ME) != -1) {
                        discardCardToOfflineDeck(peekDpSlotTop(ME), ME);
                        SPRITE_KIND(peekDpSlotTop(ME)) = 6;
                        removeCardFromDpSlots(peekDpSlotTop(ME), ME);
                    }
                    waitDuelFrames(0x78);
                    MESSAGE = 0x65;
                    do {
                        waitDuelFrames(1);
                        KAW_drawHandHints(ME, 3);
                    } while (!(PAD0_PRESSED & PAD_CIRCLE));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                    DUEL->step = 0x20;
                }
            } else if (PAD_PRESSED(ME) & (PAD_TRIANGLE | PAD_CROSS)) {
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x16;
            }
            if (DUEL->step != 0x1D) {
                KAW_closeCardSelect(ME);
            }
            break;
        case 0x1E:
            DUEL->messageWanted = 0xB;
            DUEL->step++;
            break;
        case 0x20:
            waitDuelFrames(0x1E);
            DUEL->unk47F = 2;
            DUEL->playedFromSlot = -1;
            DUEL->dpFromSlot = -1;
            PLAYER(ME)->battleCard = CARD_OF(ME, getActiveDigimonCard(ME)).card;
            if (getActiveDigimonCard(OPP) == -1) {
                MESSAGE = 0xD;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0xE) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                DUEL->step = 0x31;
            } else {
                DUEL->step = 0x21;
            }
            break;
        case 0x21:
            DUEL->unk47F = -1;
            DUEL->humanPlayer = -1;
            DUEL->unk481 = -1;
            DUEL->unk482 = -1;
            DUEL->messageWanted = 0xD;
            PLAYER_PANEL(0, 3)->state = 0xE;
            PLAYER_PANEL(1, 3)->state = 0xE;
            switch (LESSON) {
            case 1:
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x20;
                while (1) {
                    waitDuelFrames(1);
                    if (MESSAGE == 0x24) {
                        drawTutorialPointer(0xA5, 0xA0, 0, 0);
                    }
                    if (MESSAGE == 0x25) {
                        drawTutorialPointer(0xA5, 0xAC, 0, 0);
                    }
                    if (MESSAGE == 0x26) {
                        drawTutorialPointer(0xA5, 0xB8, 0, 0);
                    }
                    if (MESSAGE == 0x27) {
                        drawTutorialPointer(0x4C, 0xC4, 0, 0);
                    }
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x28) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                waitDuelFrames(0x78);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x29;
                PLAYER(1)->attackChoice = 1;
                playSoundEffect(0xA0);
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x2C) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                break;
            case 2:
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x49;
                while (1) {
                    do {
                        waitDuelFrames(1);
                    } while (!(PAD0_PRESSED & PAD_CIRCLE));
                    playSoundEffect(0xA0);
                    if (MESSAGE == 0x4B) {
                        playSoundEffect(0xA0);
                        PLAYER(1)->attackChoice = 0;
                    } else if (MESSAGE == 0x4D) {
                        DUEL->tutorialVisible = 0;
                        break;
                    }
                    MESSAGE++;
                }
                break;
            case 3:
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x67;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x6A) {
                            break;
                        }
                        MESSAGE++;
                    }
                }
                PLAYER(1)->attackChoice = 1;
                DUEL->tutorialVisible = 0;
                break;
            }
            DUEL->step++;
            DUEL->unk424 = 0;
            break;
        case 0x22:
            DUEL->unk47F = -1;
            DUEL->humanPlayer = -1;
            DUEL->unk481 = -1;
            DUEL->unk482 = -1;
            switch (LESSON) {
            case 1:
            case 3:
                if (PLAYER(0)->attackChoice == 3) {
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        PLAYER(0)->attackChoice = 0;
                    } else if (PAD0_PRESSED & (PAD_TRIANGLE | PAD_CROSS | PAD_SQUARE)) {
                        playSoundEffect(0xA0);
                        DUEL->tutorialVisible = 1;
                        MESSAGE = 0x2C;
                    }
                }
                break;
            case 2:
                if (PLAYER(0)->attackChoice == 3) {
                    if (PAD0_PRESSED & PAD_CROSS) {
                        playSoundEffect(0xA0);
                        PLAYER(0)->attackChoice = 2;
                    } else if (PAD0_PRESSED & (PAD_TRIANGLE | PAD_CIRCLE | PAD_SQUARE)) {
                        playSoundEffect(0xA0);
                        DUEL->tutorialVisible = 1;
                        MESSAGE = 0x4E;
                    }
                }
                break;
            }
            if (PLAYER(0)->attackChoice != 3 && PLAYER(1)->attackChoice != 3) {
                PLAYER(0)->usedAttack = PLAYER(0)->attackChoice;
                PLAYER(1)->usedAttack = PLAYER(1)->attackChoice;
                DUEL->tutorialVisible = 0;
                waitDuelFrames(0x78);
                switch (LESSON) {
                case 1:
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x2D;
                    playSoundEffect(0xA0);
                    do {
                        waitDuelFrames(1);
                    } while (!(PAD0_PRESSED & PAD_CIRCLE));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                    break;
                case 2:
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x4F;
                    while (1) {
                        waitDuelFrames(1);
                        if (PAD0_PRESSED & PAD_CIRCLE) {
                            playSoundEffect(0xA0);
                            if (MESSAGE == 0x50) {
                                DUEL->tutorialVisible = 0;
                                break;
                            }
                            MESSAGE++;
                        }
                    }
                    break;
                }
                DUEL->tutorialVisible = 0;
                PLAYER_PANEL(0, 8)->state = 6;
                PLAYER_PANEL(1, 8)->state = 6;
                waitDuelFrames(0x3C);
                DUEL->step++;
            }
            break;
        case 0x23:
            DUEL->unk47F = 2;
            DUEL->unk482 = 7;
            DUEL->humanPlayer = PLAYER(OPP)->controller;
            PLAYER_PANEL(OPP, 2)->state = 1;
            DUEL->messageWanted = 0xF;
            switch (LESSON) {
            case 1:
                DUEL->unk481 = -1;
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x2E;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                moveTutorialCursor(0, 0x1E, 1);
                KAW_openCardSelect(OPP);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x2F;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                waitDuelFrames(0x50);
                moveTutorialCursor(2, 0x20, 1);
                playSoundEffect(0xA2);
                waitDuelFrames(0x50);
                moveTutorialCursor(0, 0x1E, 1);
                playSoundEffect(0xA2);
                waitDuelFrames(0x50);
                moveTutorialCursor(2, 0x20, 1);
                playSoundEffect(0xA2);
                waitDuelFrames(0x50);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x30;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                waitDuelFrames(0x3C);
                DUEL->playedFromSlot = KAW_playCardFromHand(CUR_CARD, OPP);
                KAW_closeCardSelect(OPP);
                waitDuelFrames(0x78);
                DUEL->step = 0x27;
                break;
            case 2:
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x51;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                DUEL->playedFromSlot = -1;
                DUEL->step++;
                DUEL->returnStep = 0;
                break;
            case 3:
                DUEL->unk481 = -1;
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x6B;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                moveTutorialCursor(4, 0x25, 1);
                KAW_openCardSelect(OPP);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x6C;
                while (1) {
                    waitDuelFrames(1);
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x70) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
                waitDuelFrames(0x3C);
                KAW_playOnlineDeckTop(OPP);
                KAW_closeCardSelect(OPP);
                waitDuelFrames(0x78);
                DUEL->step = 0x27;
                break;
            }
            break;
        case 0x24:
            DUEL->unk481 = 2;
            DUEL->unk482 = 7;
            if (DUEL->returnStep == 0) {
                if (PAD0_PRESSED & (PAD_TRIANGLE | PAD_CIRCLE)) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x52;
                } else if (PAD0_PRESSED & PAD_SQUARE) {
                    DUEL->tutorialVisible = 0;
                    playSoundEffect(0xA0);
                    moveTutorialCursor(0, 0, 0);
                    DUEL->step = 9;
                    DUEL->returnStep = 0x24;
                    DUEL->viewPlayer = ME ^ 1;
                }
            } else if (PAD0_PRESSED & (PAD_TRIANGLE | PAD_SQUARE)) {
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x53;
            } else if (PAD0_PRESSED & PAD_CIRCLE) {
                playSoundEffect(0xA0);
                KAW_openCardSelect(OPP);
                DUEL->step++;
            }
            break;
        case 0x25:
            DUEL->unk481 = 8;
            if (KAW_tickCardCursor(OPP, 4) == 0) {
                if (DUEL->cursorSlot != 0) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x77;
                } else {
                    DUEL->playedFromSlot = KAW_playCardFromHand(CUR_CARD, OPP);
                    playSoundEffect(0xA0);
                    DUEL->messageWanted = 0x16;
                    DUEL->step++;
                }
            } else if (PAD_PRESSED(OPP) & (PAD_TRIANGLE | PAD_CROSS)) {
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x16;
            }
            if (DUEL->step != 0x25) {
                KAW_closeCardSelect(OPP);
            }
            break;
        case 0x27:
            DUEL->unk482 = 7;
            DUEL->humanPlayer = PLAYER(ME)->controller;
            PLAYER_PANEL(ME, 2)->state = 1;
            DUEL->messageWanted = 0xF;
            switch (LESSON) {
            case 1:
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x31;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                DUEL->playedFromSlot = -1;
                DUEL->step++;
                break;
            case 2:
                DUEL->unk481 = -1;
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x56;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                DUEL->step = 0x2B;
                break;
            case 3:
                DUEL->tutorialVisible = 1;
                MESSAGE = 0x71;
                do {
                    waitDuelFrames(1);
                } while (!(PAD0_PRESSED & PAD_CIRCLE));
                playSoundEffect(0xA0);
                DUEL->tutorialVisible = 0;
                DUEL->playedFromSlot = -1;
                DUEL->step++;
                break;
            }
            break;
        case 0x28:
            DUEL->unk481 = 2;
            switch (LESSON) {
            case 1:
                if (PAD0_PRESSED & PAD_CIRCLE) {
                    playSoundEffect(0xA0);
                    KAW_openCardSelect(ME);
                    DUEL->unk481 = 8;
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x32;
                    moveTutorialCursor(0, 0, 0);
                    waitDuelFrames(0x10);
                    do {
                        waitDuelFrames(1);
                        drawTutorialPointer(0x3E, 0x5C, 0, 0);
                    } while (!(PAD0_PRESSED & PAD_CIRCLE));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                    MESSAGE = 0x33;
                    moveTutorialCursor(2, 2, 0);
                    do {
                        waitDuelFrames(1);
                        drawTutorialPointer(0x96, 0x5C, 0, 0);
                    } while (!(PAD0_PRESSED & PAD_CIRCLE));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                    waitDuelFrames(0x50);
                    moveTutorialCursor(0, 0, 0);
                    playSoundEffect(0xA2);
                    waitDuelFrames(0x50);
                    moveTutorialCursor(2, 2, 0);
                    playSoundEffect(0xA2);
                    waitDuelFrames(0x50);
                    MESSAGE = 0x34;
                    while (1) {
                        waitDuelFrames(1);
                        if (MESSAGE == 0x35) {
                            drawTutorialPointer(0x96, 0x5C, 0, 0);
                        }
                        if (PAD0_PRESSED & PAD_CIRCLE) {
                            playSoundEffect(0xA0);
                            if (MESSAGE == 0x36) {
                                moveTutorialCursor(1, 0x20, 1);
                                playSoundEffect(0xA2);
                            } else if (MESSAGE == 0x38) {
                                DUEL->tutorialVisible = 0;
                                break;
                            }
                            MESSAGE++;
                        }
                    }
                    moveTutorialCursor(2, 2, 0);
                    DUEL->step++;
                } else if (PAD_PRESSED(ME) & (PAD_TRIANGLE | PAD_SQUARE)) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x16;
                    do {
                        waitDuelFrames(1);
                    } while (!(PAD0_PRESSED & (PAD_TRIANGLE | PAD_CIRCLE | PAD_SQUARE)));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                }
                break;
            case 2:
                DUEL->step = 0x2B;
                break;
            case 3:
                if (PAD0_PRESSED & PAD_CIRCLE) {
                    playSoundEffect(0xA0);
                    moveTutorialCursor(4, 0xC, 0);
                    KAW_openCardSelect(ME);
                    DUEL->step++;
                } else if (PAD_PRESSED(ME) & (PAD_TRIANGLE | PAD_SQUARE)) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x16;
                    do {
                        waitDuelFrames(1);
                    } while (!(PAD0_PRESSED & (PAD_TRIANGLE | PAD_CIRCLE | PAD_SQUARE)));
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 0;
                }
                break;
            }
            break;
        case 0x29:
            DUEL->unk481 = 8;
            switch (LESSON) {
            case 1:
                if (KAW_tickCardCursor(ME, 4) == 0) {
                    i = PLAYER(ME)->hand[DUEL->cursorSlot];
                    if (DUEL->cursorSlot == 2) {
                        DUEL->playedFromSlot = KAW_playCardFromHand(CUR_CARD, ME);
                        playSoundEffect(0xA0);
                        DUEL->messageWanted = 0x16;
                        DUEL->step++;
                    } else {
                        DUEL->tutorialVisible = 1;
                        MESSAGE = 0x39;
                    }
                } else if (PAD_PRESSED(ME) & (PAD_TRIANGLE | PAD_CROSS)) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x39;
                }
                break;
            case 3:
                if (KAW_tickCardCursor(ME, 4) == 0) {
                    if (DUEL->cursorSlot == 4) {
                        DUEL->playedFromSlot = 4;
                        KAW_playOnlineDeckTop(ME);
                        playSoundEffect(0xA0);
                        DUEL->messageWanted = 0x16;
                        DUEL->step++;
                    } else {
                        DUEL->tutorialVisible = 1;
                        MESSAGE = 0x72;
                    }
                } else if (PAD_PRESSED(ME) & (PAD_TRIANGLE | PAD_CROSS)) {
                    playSoundEffect(0xA0);
                    DUEL->tutorialVisible = 1;
                    MESSAGE = 0x72;
                }
                break;
            }
            if (DUEL->step != 0x29) {
                KAW_closeCardSelect(ME);
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
            switch (LESSON) {
            case 1:
                MESSAGE = 0x3A;
                break;
            case 2:
                MESSAGE = 0x57;
                break;
            case 3:
                MESSAGE = 0x73;
                break;
            }
            DUEL->tutorialVisible = 1;
            do {
                waitDuelFrames(1);
            } while (!(PAD0_PRESSED & PAD_CIRCLE));
            playSoundEffect(0xA0);
            DUEL->tutorialVisible = 0;
            waitDuelFrames(0x3C);
            KAW_resolveBattle(0);
            DUEL->step++;
            break;
        case 0x2D:
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
            DUEL->messageWanted = 0x12;
            isCrossPressedByTurnPlayer();
            DUEL->step = 0x30;
            break;
        case 0x30:
            DUEL->unk47F = 2;
            DUEL->humanPlayer = PLAYER(ME)->controller;
            DUEL->unk482 = 8;
            DUEL->unk481 = 9;
            DUEL->step++;
            break;
        case 0x31:
            if (LESSON == 3) {
                MESSAGE = 0x74;
                while (1) {
                    waitDuelFrames(1);
                    if (MESSAGE == 0x75) {
                        drawTutorialPointer(0x8E, 0xB4, 1, 0);
                    }
                    if (PAD0_PRESSED & PAD_CIRCLE) {
                        playSoundEffect(0xA0);
                        if (MESSAGE == 0x76) {
                            DUEL->tutorialVisible = 0;
                            break;
                        }
                        MESSAGE++;
                    }
                }
            }
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
            if (LESSON == 3) {
                waitDuelFrames(0x1E);
                DUEL->step = 0x32;
            } else {
                DUEL->turnPlayer ^= 1;
                LESSON++;
                DUEL->step = 2;
            }
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
    DUEL->stopTurnLoop = 0;
}
