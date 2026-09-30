#include "dcb/duel_session.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/battle_hud.h"
#include "dcb/card_motion.h"
#include "dcb/hud_panels.h"
#include "dcb/duel_launch.h"
#include "dcb/card_render.h"
#include "dcb/duel.h"
#include "dcb/cpu_decision.h"
#include "dcb/duel_setup.h"
#include "dcb/card_zones.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/camera.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/text.h"
#include "dcb/str_util.h"
#include "dcb/fade.h"
#include "dcb/overlay_calls.h"

void initDuelState(s32 isCpuDuel) {
    void *block;

    CARD_ANIMS = block = allocTaskHeapBlock(0x870);
    DUEL_STATE = block = allocTaskHeapBlock(0x86C);
    DUEL->sprites = (void *)KAW_allocCardPolys();
    DUEL->turnPlayer = rand() % 2;
    DUEL->step = 0;
    DUEL->cursorPlayer = 0;
    DUEL->cursorSlot = -1;
    DUEL->state = -1;
    DUEL->tutorial = 0;
    DUEL->unk825 = 0;
    DUEL->menuOpen = 0;
    DUEL->awaitingInput = 0;
    DUEL->quit = 0;
    DUEL->tutorialBusy = 0;
    KAW_initHudPanels();
    initDuelPlayers(isCpuDuel);
    DUEL->cursorMode = -1;
}

void startDuelScene(void) {
    Graphics *camera;

    initScene3D(0);
    spawnTask(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    waitFrames(2);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->targetPitch = 0;
    camera->targetDistance = 0x1C0;
    camera->targetHeight = 0;
    camera->targetYaw = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
    DUEL->loadBusy = 0;
    waitFrames(2);
}

void spawnDuelTasks(s32 isCpuDuel) {
    s32 stageId;
    s32 stageArg;

    DUEL->cursor = (u8 *)KAW_createCursor(0, 0x26, 0x2E, 0xA, 1);
    spawnTask(0x1E, -1, 0, 0x800, &runDuelTurnLoop, 0, 0, 0, 0);
    if ((isCpuDuel != 0) && (DUEL->tutorial == 0)) {
        spawnTask(0, -1, 0, 0x800, runCpuDecisionTask, 0, 0, 0, 0);
    }
    spawnTask(0, -1, 0, 0x800, &runCardArtLoader, 0, 0, 0, 0);
    if (isCpuDuel != 0) {
        stageId = ((SessionData *)SESSION_DATA)->opponentDeck.stageId;
        stageArg = ((SessionData *)SESSION_DATA)->opponentDeck.unk68[1];
    } else {
        stageId = -1;
        stageArg = -1;
    }
    spawnTask(0, -1, 0, 0x1000, &runDuelStageTask, stageId, stageArg, 0, 0);
}

void teardownDuelScene(void) {
    endTask(0x19);
    KAW_freeHudPanels();
    KAW_freeCardPolys();
    freeHeapBlocksByTag(0x7F);
}

void renderDuelFrame(void) {
    s16 fadeLevel;

    fadeLevel = DUEL->fade;
    if (fadeLevel != 0) {
        renderDuelBackground(fadeLevel);
        renderStatusMessage(DUEL->fade);
        renderHelpBar(DUEL->fade);
    }
    renderPhaseBanner();
    tickBattleHud();
    renderBoardCards();
    if (DUEL->inPolygonBattle == 0) {
        /* the expanding/shrinking ring, drawn by KAWSEG */
        if (DUEL->ringMode != -1) {
            KAW_renderRing();
        }
        KAW_drawHandHints(MSG_BAR_PLAYER_LABEL);
    }
}

void runDuel(s32 mode, s32 parent) {
    Rect16 r = { 320, 0, 640, 512 };
    s32 i;
    s32 j;
    s32 timer;
    s32 winner;
    s32 k;

    ClearImage(&r, 0, 0, 0);
    DrawSync(0);
    loadSoundEffectBank(0);
    initDuelState(mode);
    startDuelScene();
    if (mode) {
        i = PLAYER_DATA(0).activePartner;
    } else {
        i = 7;
    }
    changeScrollingBackground(i, 0x280, 0, 0x280, 0x80);
    KAW_runDeckSelect(mode, ((SessionData *)SESSION_DATA)->opponentDeckIndex);
    KAW_runVersusIntro(mode, ((SessionData *)SESSION_DATA)->opponentDeckIndex);
    spawnDuelTasks(mode);
    for (i = 0; i < 2; i++) {
        KAW_resetBonusFlags(i);
    }
    timer = 0;
    KAW_loadEffectArchive();
    fadeOutScrollingBackground();
    freeScrollingBackground();
    KAW_initRing();
    setScreenFadeParams(1, 2, 6);
    DUEL->fade = 0x80;
    DUEL->inPolygonBattle = 0;
    for (;;) {
        waitFrames(FRAME_INTERVAL);
        switch (DUEL->state) {
        case -1:
            addFrameCallback((s32)renderDuelFrame);
            DUEL->state++;
            break;
        case 0:
            KAW_tickDuelMenu();
            break;
        case 1:
            PLAYER_PANEL(0, HUD_DECK)->state = 11;
            PLAYER_PANEL(1, HUD_DECK)->state = 11;
            PLAYER_PANEL(0, HUD_STATUS)->state = 4;
            PLAYER_PANEL(1, HUD_STATUS)->state = 4;
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    s8 c = PLAYER(i)->digimonStack[j];

                    if (c != -1) {
                        SPRITE_KIND(c) = 0x1D;
                        break;
                    }
                }
            }
            DUEL->state++;
            break;
        case 2:
            if (PLAYER_PANEL(0, HUD_DECK)->state == 0) {
                timer = 0;
                DUEL->fade = 0x80;
                DUEL->state = 20;
            }
            break;
        case 20:
            DUEL->fade -= 4;
            if (DUEL->fade < 0) {
                DUEL->fade = 0;
            }
            if (timer++ >= 10) {
                createWireGrid(400, 600, 9, 13, 0, 1);
                addFrameCallback((s32)renderWireGrid);
                timer = 0;
                DUEL->state++;
            }
            break;
        case 21:
            DUEL->fade -= 4;
            if (DUEL->fade < 0) {
                DUEL->fade = 0;
            }
            if (DUEL->fade == 0) {
                DB(0).scenePackets = allocHeapBlock(0xBB80, 0x7F);
                DB(1).scenePackets = allocHeapBlock(0xBB80, 0x7F);
                showArenaStage(0x400);
                addFrameCallback((s32)renderSceneModels);
                SCENE_3D->modelState[0x17] = 1;
                timer = 0;
                DUEL->state++;
            }
            break;
        case 22:
            DUEL->fade -= 4;
            if (DUEL->fade < 0) {
                DUEL->fade = 0;
            }
            ((Graphics *)&GRAPHICS)->targetDistance += 4;
            ((Graphics *)&GRAPHICS)->targetPitch -= 10;
            ((Graphics *)&GRAPHICS)->rotZ += 6;
            if (timer++ >= 60) {
                for (i = 0; i < 2; i++) {
                    for (j = 0; j < 3; j++) {
                        s8 c = PLAYER(i)->digimonStack[j];

                        if (c != -1) {
                            SPRITE_KIND(c) = 0x20;
                            break;
                        }
                    }
                }
                timer = 0;
                DUEL->state++;
            }
            break;
        case 23:
            ((Graphics *)&GRAPHICS)->rotZ += 8;
            if (++timer == 60) {
                spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
            }
            if (timer == 120) {
                removeFrameCallback((s32)renderWireGrid);
                removeFrameCallback((s32)renderDuelFrame);
            }
            if (timer >= 122) {
                timer = 0;
                DUEL->state = 3;
                freeWireGrid();
                ((Graphics *)&GRAPHICS)->targetDistance = 0x1C0;
                ((Graphics *)&GRAPHICS)->targetPitch = 0;
                ((Graphics *)&GRAPHICS)->rotZ = 0;
            }
            break;
        case 4:
            DUEL->state++;
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 4, 0);
            break;
        case 6:
            stopScreenFade();
            for (i = 0; i < 2; i++) {
                for (j = 0; j < 3; j++) {
                    s8 c = PLAYER(i)->digimonStack[j];

                    if (c != -1) {
                        SPRITE_KIND(c) = 15;
                    }
                }
            }
            PLAYER_PANEL(0, HUD_DECK)->state = 7;
            PLAYER_PANEL(1, HUD_DECK)->state = 7;
            addFrameCallback((s32)renderDuelFrame);
            DUEL->state++;
            break;
        case 7:
            if (DUEL->fade != 0x80) {
                DUEL->fade += 4;
                if (DUEL->fade > 0x80) {
                    DUEL->fade = 0x80;
                }
            } else if (PLAYER_PANEL(0, HUD_DECK)->state == 4) {
                DUEL->state = -1;
            }
            break;
        }
        if (DUEL->step == 0x27) {
            winner = DUEL->winner;
            break;
        }
        if (DUEL->quit >= 2) {
            DUEL->winner = DUEL->quit & 1;
            winner = DUEL->quit - 2;
            break;
        }
    }
    KAW_freeRing();
    PAD_INPUT_ENABLED = 1;
    DUEL->stopTurnLoop = -1;
    DUEL->stopArtLoader = -1;
    DUEL->stopStageTask = -1;
    DUEL->stopCpuTask = -1;
    DUEL->fade = 0x80;
    while (DUEL->stopStageTask | DUEL->stopArtLoader) {
        waitFrames(FRAME_INTERVAL);
    }
    loadScrollingBackground();
    spawnTask(0, -1, 0, 0x800, KAW_runResultScreen, mode, winner, ((SessionData *)SESSION_DATA)->opponentDeckIndex, 0);
    waitFrames(FRAME_INTERVAL);
    while (KAW_RESULT_SCREEN_STATE != 0) {
        waitFrames(FRAME_INTERVAL);
        if (KAW_RESULT_SCREEN_STATE == 1) {
            removeFrameCallback((s32)renderDuelFrame);
        }
    }
    while (DUEL->stopTurnLoop != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    KAW_freeCursor(DUEL->cursor);
    if (DUEL->tutorial == 0 && mode != 0) {
        while (DUEL->stopCpuTask != 0) {
            waitFrames(FRAME_INTERVAL);
        }
        if (((SessionData *)SESSION_DATA)->npcDeckIndex[0] != -1) {
            restorePartners(0);
        }
        /* j: the player's deck holds one of the partner cards (or their armor) */
        i = 0;
        j = 0;
        for (; i < 30; i++) {
            k = findPartnerSlot(0, PLAYER(0)->cards[i].id);
            if (k >= 0 && k < 3) {
                j = 1;
            }
            k = findArmorPartnerSlot(0, PLAYER(0)->cards[i].id);
            if (k >= 0 && k < 3) {
                j = 1;
            }
        }
        if (j || winner == 0) {
            changeScrollingBackground(7, 0x280, 0, 0x280, 0x80);
        }
        if (j) {
            KAW_runExpScreen();
        }
        if (winner == 0) {
            KAW_runPrizeScreen();
        }
        fadeOutScrollingBackground();
    } else if (mode == 0) {
        for (i = 0; i < 2; i++) {
            if (((SessionData *)SESSION_DATA)->npcDeckIndex[i] != -1) {
                restorePartners(i);
            }
        }
    }
    if (DUEL->tutorial != 0) {
        PLAYER_DATA(0).opponentDeckFlags[0] |= 0x8000;
        PLAYER_DATA(0).opponentDeckFlags[0x9E] |= 0x8000;
    } else {
        for (i = 0; i < 2; i++) {
            k = getBaseDeckId(((SessionData *)SESSION_DATA)->npcDeckIndex[i]);
            if (k != -1) {
                /* bit 15: deck met; low 14 bits: wins, capped at 999 */
                if (winner == i) {
                    PLAYER_DATA(winner).opponentDeckFlags[k] = (PLAYER_DATA(winner).opponentDeckFlags[k] | 0x8000) + 1;
                    if ((PLAYER_DATA(winner).opponentDeckFlags[k] & 0x3FFF) >= 1000) {
                        PLAYER_DATA(winner).opponentDeckFlags[k] = (PLAYER_DATA(winner).opponentDeckFlags[k] & 0xC000) + 999;
                    }
                } else if (++PLAYER_DATA(i).opponentDeckLosses[k] >= 1000) {
                    PLAYER_DATA(i).opponentDeckLosses[k] = 999;
                }
            } else {
                s8 c = PLAYER_DATA(i).deckChoice;

                if (c != -1) {
                    PlayerDeck *e = &PLAYER_DATA(i).savedDecks[c];

                    if (i == winner) {
                        if (++e->wins >= 1000) {
                            e->wins = 999;
                        }
                    } else if (++e->losses >= 1000) {
                        e->losses = 999;
                    }
                }
            }
        }
        if (mode != 0) {
            k = getBaseDeckId(((SessionData *)SESSION_DATA)->opponentDeckIndex);
            PLAYER_DATA(0).opponentDeckFlags[k] |= 0x8000;
            if (winner == 0) {
                if (++PLAYER_DATA(0).comWins[k] >= 1000) {
                    PLAYER_DATA(0).comWins[k] = 999;
                }
            } else if (++PLAYER_DATA(0).comLosses[k] >= 1000) {
                PLAYER_DATA(0).comLosses[k] = 999;
            }
        }
    }
    KAW_freeEffectArchive();
    teardownDuelScene();
    KAW_freeTutorial();
    waitFrames(4);
    stopMusic();
    resumeTask(parent, winner);
}

