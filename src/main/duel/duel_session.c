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

void initDuelState(s32 isCpuDuel) {
    void *block;

    D_801D833C = block = allocTaskHeapBlock(0x870);
    D_801D8340 = block = allocTaskHeapBlock(0x86C);
    (*(s32 *)((s8 *)D_801D8340 + 0x7F8)) = func_801F8854();
    (*(s8 *)((s8 *)D_801D8340 + 0x817)) = (s8) (rand() % 2);
    (*(s8 *)((s8 *)D_801D8340 + 0x818)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81B)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81C)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x810)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x81F)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x825)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x823)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x822)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x824)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x820)) = 0;
    func_801F8200();
    initDuelPlayers(isCpuDuel);
    (*(s8 *)((s8 *)D_801D8340 + 0x81D)) = -1;
}

void startDuelScene(void) {
    Graphics *camera;

    initScene3D(0);
    func_800149B8(0x19, -1, 0, 0x800, &runSceneCameraTask, 0);
    func_80014C08(2);
    camera = (Graphics *)&GRAPHICS;
    camera->rotX = 0;
    camera->rotY = 0;
    camera->rotZ = 0;
    camera->posX = 0;
    camera->posY = 0;
    camera->posZ = 0;
    camera->unk8E = 0;
    camera->unk90 = 0x1C0;
    camera->unk92 = 0;
    camera->unk94 = 0;
    camera->targetModel = -1;
    camera->snapCamera = 1;
    (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
    func_80014C08(2);
}

void spawnDuelTasks(s32 isCpuDuel) {
    s32 stageId;
    s32 stageArg;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &runDuelTurnLoop, 0, 0, 0, 0);
    if ((isCpuDuel != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, runCpuDecisionTask, 0, 0, 0, 0);
    }
    func_800149B8(0, -1, 0, 0x800, &runCardArtLoader, 0, 0, 0, 0);
    if (isCpuDuel != 0) {
        stageId = (*(u8 *)((s8 *)D_8006E054 + 0x72));
        stageArg = (*(u8 *)((s8 *)D_8006E054 + 0x71));
    } else {
        stageId = -1;
        stageArg = -1;
    }
    func_800149B8(0, -1, 0, 0x1000, &runDuelStageTask, stageId, stageArg, 0, 0);
}

void teardownDuelScene(void) {
    func_80014A00(0x19);
    func_801F848C();
    func_801F88E8();
    freeHeapBlocksByTag(0x7F);
}

void renderDuelFrame(void) {
    s16 fadeLevel;

    fadeLevel = (*(s16 *)((s8 *)D_801D8340 + 0x808));
    if (fadeLevel != 0) {
        renderDuelBackground(fadeLevel);
        renderStatusMessage((*(s16 *)((s8 *)D_801D8340 + 0x808)));
        renderHelpBar((*(s16 *)((s8 *)D_801D8340 + 0x808)));
    }
    renderPhaseBanner();
    tickBattleHud();
    renderBoardCards();
    if ((*(s32 *)((s8 *)D_801D8340 + 0x83C)) == 0) {
        if ((*(s32 *)((s8 *)D_801D8340 + 0x828)) != -1) {
            func_801F97F4();
        }
        func_801EB53C(D_801D83D1);
    }
}

/* KAWSEG, the duel overlay; D_801F2A40 is a task that main only passes by address */
void func_801F003C(s32, s32);
void func_801F1AA8(s32, s32);
void func_801F9EAC(s32);
void func_801F6170(void);
void func_801F96F0(void);
void func_801F7B2C(void);
void func_801F9794(void);
void D_801F2A40();
extern s32 D_801FC734;
void func_801F8DB4(void *);
void func_801F4A24(void);
void func_801F5E50(void);
void func_801F61E4(void);
void func_801EA7E8(void);

void runDuel(s32 mode, s32 parent) {
    Rect16 r = { 320, 0, 640, 512 };
    s32 i;
    s32 j;
    s32 n;
    s32 winner;
    s32 k;

    ClearImage(&r, 0, 0, 0);
    DrawSync(0);
    loadSoundEffectBank(0);
    initDuelState(mode);
    startDuelScene();
    if (mode) {
        i = PLAYER_DATA(0).unk56;
    } else {
        i = 7;
    }
    changeScrollingBackground(i, 0x280, 0, 0x280, 0x80);
    func_801F003C(mode, ((SessionData *)D_8006E054)->opponentDeckIndex);
    func_801F1AA8(mode, ((SessionData *)D_8006E054)->opponentDeckIndex);
    spawnDuelTasks(mode);
    for (i = 0; i < 2; i++) {
        func_801F9EAC(i);
    }
    n = 0;
    func_801F6170();
    fadeOutScrollingBackground();
    freeScrollingBackground();
    func_801F96F0();
    setScreenFadeParams(1, 2, 6);
    DUEL->fade = 0x80;
    DUEL->inPolygonBattle = 0;
    for (;;) {
        func_80014C08(FRAME_INTERVAL);
        switch (DUEL->state) {
        case -1:
            addFrameCallback((s32)renderDuelFrame);
            DUEL->state++;
            break;
        case 0:
            func_801F7B2C();
            break;
        case 1:
            D_801D83EC[0x9D] = 11;
            D_801D83EC[0x175] = 11;
            D_801D83EC[0x55] = 4;
            D_801D83EC[0x12D] = 4;
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
            if (D_801D83EC[0x9D] == 0) {
                n = 0;
                DUEL->fade = 0x80;
                DUEL->state = 20;
            }
            break;
        case 20:
            DUEL->fade -= 4;
            if (DUEL->fade < 0) {
                DUEL->fade = 0;
            }
            if (n++ >= 10) {
                createWireGrid(400, 600, 9, 13, 0, 1);
                addFrameCallback((s32)renderWireGrid);
                n = 0;
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
                n = 0;
                DUEL->state++;
            }
            break;
        case 22:
            DUEL->fade -= 4;
            if (DUEL->fade < 0) {
                DUEL->fade = 0;
            }
            ((Graphics *)&GRAPHICS)->unk90 += 4;
            ((Graphics *)&GRAPHICS)->unk8E -= 10;
            ((Graphics *)&GRAPHICS)->rotZ += 6;
            if (n++ >= 60) {
                for (i = 0; i < 2; i++) {
                    for (j = 0; j < 3; j++) {
                        s8 c = PLAYER(i)->digimonStack[j];

                        if (c != -1) {
                            SPRITE_KIND(c) = 0x20;
                            break;
                        }
                    }
                }
                n = 0;
                DUEL->state++;
            }
            break;
        case 23:
            ((Graphics *)&GRAPHICS)->rotZ += 8;
            if (++n == 60) {
                func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 1, 6, 0);
            }
            if (n == 120) {
                removeFrameCallback((s32)renderWireGrid);
                removeFrameCallback((s32)renderDuelFrame);
            }
            if (n >= 122) {
                n = 0;
                DUEL->state = 3;
                freeWireGrid();
                ((Graphics *)&GRAPHICS)->unk90 = 0x1C0;
                ((Graphics *)&GRAPHICS)->unk8E = 0;
                ((Graphics *)&GRAPHICS)->rotZ = 0;
            }
            break;
        case 4:
            DUEL->state++;
            func_800149B8(0, -1, 0, 0x200, screenFadeTask, 0, 2, 4, 0);
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
            D_801D83EC[0x9D] = 7;
            D_801D83EC[0x175] = 7;
            addFrameCallback((s32)renderDuelFrame);
            DUEL->state++;
            break;
        case 7:
            if (DUEL->fade != 0x80) {
                DUEL->fade += 4;
                if (DUEL->fade > 0x80) {
                    DUEL->fade = 0x80;
                }
            } else if (D_801D83EC[0x9D] == 4) {
                DUEL->state = -1;
            }
            break;
        }
        if (DUEL->step == 0x27) {
            winner = DUEL->winner;
            break;
        }
        if (DUEL->unk824 >= 2) {
            DUEL->winner = DUEL->unk824 & 1;
            winner = DUEL->unk824 - 2;
            break;
        }
    }
    func_801F9794();
    PAD_INPUT_ENABLED = 1;
    DUEL->stopTurnLoop = -1;
    DUEL->stopArtLoader = -1;
    DUEL->stopStageTask = -1;
    DUEL->stopCpuTask = -1;
    DUEL->fade = 0x80;
    while (DUEL->stopStageTask | DUEL->stopArtLoader) {
        func_80014C08(FRAME_INTERVAL);
    }
    loadScrollingBackground();
    func_800149B8(0, -1, 0, 0x800, D_801F2A40, mode, winner, ((SessionData *)D_8006E054)->opponentDeckIndex, 0);
    func_80014C08(FRAME_INTERVAL);
    while (D_801FC734 != 0) {
        func_80014C08(FRAME_INTERVAL);
        if (D_801FC734 == 1) {
            removeFrameCallback((s32)renderDuelFrame);
        }
    }
    while (DUEL->stopTurnLoop != 0) {
        func_80014C08(FRAME_INTERVAL);
    }
    func_801F8DB4(DUEL->cursor);
    if (DUEL->tutorial == 0 && mode != 0) {
        while (DUEL->stopCpuTask != 0) {
            func_80014C08(FRAME_INTERVAL);
        }
        if (((SessionData *)D_8006E054)->npcDeckIndex[0] != -1) {
            restorePartners(0);
        }
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
            func_801F4A24();
        }
        if (winner == 0) {
            func_801F5E50();
        }
        fadeOutScrollingBackground();
    } else if (mode == 0) {
        for (i = 0; i < 2; i++) {
            if (((SessionData *)D_8006E054)->npcDeckIndex[i] != -1) {
                restorePartners(i);
            }
        }
    }
    if (DUEL->tutorial != 0) {
        PLAYER_DATA(0).opponentDeckFlags[0] |= 0x8000;
        PLAYER_DATA(0).opponentDeckFlags[0x9E] |= 0x8000;
    } else {
        for (i = 0; i < 2; i++) {
            k = func_800471F4(((SessionData *)D_8006E054)->npcDeckIndex[i]);
            if (k != -1) {
                if (winner == i) {
                    PLAYER_DATA(winner).opponentDeckFlags[k] = (PLAYER_DATA(winner).opponentDeckFlags[k] | 0x8000) + 1;
                    if ((PLAYER_DATA(winner).opponentDeckFlags[k] & 0x3FFF) >= 1000) {
                        PLAYER_DATA(winner).opponentDeckFlags[k] = (PLAYER_DATA(winner).opponentDeckFlags[k] & 0xC000) + 999;
                    }
                } else if (++PLAYER_DATA(i).unkBFE[k] >= 1000) {
                    PLAYER_DATA(i).unkBFE[k] = 999;
                }
            } else {
                s8 c = PLAYER_DATA(i).unk30[4];

                if (c != -1) {
                    PlayerDeck *e = &PLAYER_DATA(i).savedDecks[c];

                    if (i == winner) {
                        if (++e->unk108[1] >= 1000) {
                            e->unk108[1] = 999;
                        }
                    } else if (++e->unk108[2] >= 1000) {
                        e->unk108[2] = 999;
                    }
                }
            }
        }
        if (mode != 0) {
            k = func_800471F4(((SessionData *)D_8006E054)->opponentDeckIndex);
            PLAYER_DATA(0).opponentDeckFlags[k] |= 0x8000;
            if (winner == 0) {
                if (++PLAYER_DATA(0).unk888[k] >= 1000) {
                    PLAYER_DATA(0).unk888[k] = 999;
                }
            } else if (++PLAYER_DATA(0).unk9A4[k] >= 1000) {
                PLAYER_DATA(0).unk9A4[k] = 999;
            }
        }
    }
    func_801F61E4();
    teardownDuelScene();
    func_801EA7E8();
    func_80014C08(4);
    stopMusic();
    func_80014A48(parent, winner);
}

