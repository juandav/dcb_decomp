#include "common.h"
#include "game.h"
#include "dcb/sug_battle.h"
#include "dcb/heap.h"
#include "dcb/frame_callback.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/anim_control.h"
#include "dcb/player_data.h"
#include "dcb/stage.h"
#include "dcb/model_anim.h"
#include "dcb/fade.h"
#include "dcb/sugseg.h"
#include "dcb/sug_stage_fade.h"
#include "dcb/sug_tex_anim.h"
#include "dcb/sug_effect_script.h"
#include "dcb/sug_sprite.h"
#include "dcb/sug_camera.h"
#include "dcb/sug_hud.h"
#include "dcb/prim.h"

extern s16 SUG_CURRENT_SCRIPT;
extern s32 SUG_SCRIPT_STATES[2];
extern s16 SUG_ACTIVE_MODEL;
extern s32 SUG_PREV_MODEL;
extern void *SUG_SKILL_SCRIPTS[4];
extern BattleState SUG_BATTLE_STATE;

void SUG_playSoloAnimation(s32 model, s32 anim);
void SUG_applyEatUpHp(s32 side, s32 amount);
void SUG_playNoDamageTurn(s32 model);

/* jp has no screen copy effect */
#if VERSION_US || VERSION_EU
s32 SUG_SCREEN_FX_PHASE = 0;
#endif

void SUG_placeBattleModels(void) {
    Scene3D *scene;

    scene = SCENE_3D;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn])->x = 750;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn])->rotY = 0;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn ^ 1])->x = -750;
    ((ModelData *)scene->models[SUG_BATTLE->flags.bits.turn ^ 1])->rotY = 0x800;
    SUG_TARGET_HP[SUG_BATTLE->flags.bits.turn] = SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn].hp - SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn ^ 1].damage;
    SUG_TARGET_HP[SUG_BATTLE->flags.bits.turn ^ 1] = SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn ^ 1].hp - SUG_BATTLE->players[SUG_BATTLE->flags.bits.turn].damage;
    scene->modelState[0] = -1;
    SCENE_3D->modelState[1] = -1;
}

void SUG_initBattleScene(void) {
    s32 unused[8]; /* unused, but it is in the original stack frame */

    SUG_CURRENT_SCRIPT = 0;
    addFrameCallback((s32)renderSceneModels);
    SUG_initSpriteCache();
    SUG_initTamCache();
    SUG_placeBattleModels();
    SUG_initEffectRoot();
    SUG_SCRIPT_STATES[1] = 0;
    SUG_SCRIPT_STATES[0] = 0;
}

void SUG_showWireGrid(void) {
    createWireGrid(2000, 3000, 9, 13, 1, 0);
    GRID_VISIBLE = 1;
    SCENE_3D->modelState[23] = 1;
}

void SUG_freeBattleScene(void) {
    SCENE_3D->modelState[0] = -1;
    SCENE_3D->modelState[1] = -1;
    SCENE_3D->modelState[23] = -1;
    endTask(0x1B);
    /* jp has no stage fade task to end, nor a stage brightness to restore */
#if VERSION_US || VERSION_EU
    endTask(0x1A);
#endif
    removeFrameCallback((s32)renderSceneModels);
    removeFrameCallback((s32)renderWireGrid);
    freeHeapBlock(DB(0).scenePackets);
    freeHeapBlock(DB(1).scenePackets);
    SUG_freeSprites();
    SUG_freeTamCache();
    unloadEffectAnimations();
#if VERSION_US || VERSION_EU
    SUG_resetStageBrightness();
#endif
}

void SUG_runCameraOrbit(s32 frames, s32 resetCamera) {
    Graphics *camera;

    if (resetCamera) {
        camera = (Graphics *)&GRAPHICS;
        camera->distance = 0x9C4000;
        camera->targetDistance = 4000;
        camera->snapCamera = 1;
        camera->targetPitch = 400;
        camera->pitch = 0x190000;
    }
    CAMERA_TARGET_MODEL = -1;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = 1;
    do {
        frames--;
        SUG_orbitCamera();
        waitFrames(FRAME_INTERVAL);
    } while (frames >= 0);
}

void SUG_playAttackTurn(s32 model, s32 a1, void *script, void (*fn)(s32), s32 switchModel) {
    s32 other;

    other = (s16)(SUG_CURRENT_SCRIPT ^ 1);
    if (switchModel) {
#if VERSION_JP
        startModelAnimation(SUG_ACTIVE_MODEL, 0, -2, 0);
#elif VERSION_US || VERSION_EU
        playModelAnimation(SUG_ACTIVE_MODEL, 0);
#endif
        SCENE_3D->modelState[SUG_ACTIVE_MODEL] = 1;
        if (SUG_ACTIVE_MODEL != SUG_PREV_MODEL) {
            SCENE_3D->modelState[SUG_PREV_MODEL] = -1;
            applyAnimationFirstFrame(SUG_PREV_MODEL, 0);
        }
        CAMERA_TARGET_MODEL = SUG_ACTIVE_MODEL;
    }
    while (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] != 1) {
        waitFrames(FRAME_INTERVAL);
    }
    if (script != NULL) {
        if (SUG_SCRIPT_STATES[other] != 0) {
            if (SUG_SCRIPT_STATES[other] < 0) {
                while (SUG_SCRIPT_STATES[other] != 1) {
                    waitFrames(FRAME_INTERVAL);
                }
            }
            SUG_SCRIPT_STATES[other] = 3;
            while (SUG_SCRIPT_STATES[other] != 0) {
                waitFrames(FRAME_INTERVAL);
            }
        }
        SUG_SCRIPT_STATES[other] = -1;
        spawnTask(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, script, model, a1, &SUG_SCRIPT_STATES[other]);
    }
    SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] = 2;
    if (fn != NULL) {
        fn(SUG_ACTIVE_MODEL);
    }
    if (fn == SUG_showAttackBanner && SUG_BATTLE->players[SUG_ACTIVE_MODEL].crash) {
        waitFrames(30);
        SUG_TARGET_HP[SUG_ACTIVE_MODEL] = 10;
        SUG_animateHpCounter(~SUG_ACTIVE_MODEL);
        if (!SUG_BATTLE->flags.bits.counter) {
            SUG_TARGET_HP[SUG_ACTIVE_MODEL] -= SUG_BATTLE->players[(s16)(SUG_ACTIVE_MODEL ^ 1)].damage;
        }
    }
    if (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] == 1) {
        SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] = 2;
    }
    while (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    SUG_CURRENT_SCRIPT = other;
    SUG_PREV_MODEL = SUG_ACTIVE_MODEL;
    SUG_ACTIVE_MODEL = model;
}

/* jp has no stage fade or clear colour during the exchange, and its models
   keep their animations loaded */
#if VERSION_JP
void SUG_playBattleExchange(void) {
    s32 first;
    s32 second;
    s32 other;
    s32 winner;
    s32 hp;

    winner = -1;
    first = SUG_BATTLE->flags.bits.turn ^ (SUG_BATTLE->flags.bits.flag1 | SUG_BATTLE->flags.bits.flag3);
    other = first ^ SUG_BATTLE->flags.bits.counter;
    second = first ^ 1;
    if (SUG_BATTLE->players[other].damage == 0) {
        other ^= 1;
    }
    SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] = -1;
    SUG_ACTIVE_MODEL = SUG_BATTLE->flags.bits.turn;
    spawnTask(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, BATTLE_START_SKILL, SUG_BATTLE->flags.bits.turn, 1, &SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT]);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (DUEL->state != 3);
    while (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] != 1) {
        waitFrames(FRAME_INTERVAL);
    }
    endTask(0x19);
    spawnTask(0x19, -1, 0, 0x800, &runSceneCameraTask, 1);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    SUG_showWireGrid();
    ((ModelData *)SCENE_3D->models[23])->rotX = 0;
    setScreenFadeParams(1, 1, 6);
    SUG_playAttackTurn(SUG_BATTLE->flags.bits.turn ^ 1, 1, BATTLE_START_SKILL, SUG_showHpBanner, 0);
    SUG_playAttackTurn(other, 0, SUG_SKILL_SCRIPTS[other * 2], SUG_showHpBanner, 0);
    SUG_runCameraOrbit(0x78, 1);
    SCENE_3D->modelState[first ^ 1] = -1;
    CAMERA_TARGET_MODEL = first;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SUG_SCRIPT_STATES[0] != 1);
    if (SUG_BATTLE->flags.bits.counter) {
        SUG_playSoloAnimation(first, SUG_BATTLE->players[first].attack + 1);
        SUG_showAttackLabel(first);
    } else if (SUG_BATTLE->players[first].damage == 0) {
        SUG_playNoDamageTurn(first);
    } else {
        SUG_playAttackTurn(second, 1, SUG_SKILL_SCRIPTS[first * 2 + 1], SUG_showAttackBanner, 1);
        hp = SUG_BATTLE->players[second].hp;
        SUG_playAttackTurn(second, 0, SUG_SKILL_SCRIPTS[second * 2], SUG_animateHpCounter, 1);
        applyAnimationFirstFrame(first, 0);
        startModelAnimation(first, 0, -2, 0);
        do {
            waitFrames(FRAME_INTERVAL);
        } while (((ModelData *)SCENE_3D->models[second])->animKeyTimer >= 0);
        if (SUG_BATTLE->players[first].eatUpHp) {
            SUG_applyEatUpHp(first, hp);
        }
        if (SUG_TARGET_HP[second] <= 0) {
            goto firstWins;
        }
        startModelAnimation(second, 0, -2, 0);
        waitFrames(20);
    }
    if (SUG_BATTLE->players[second].damage == 0) {
        SUG_playNoDamageTurn(second);
    } else {
        SUG_playAttackTurn(first, 1, SUG_SKILL_SCRIPTS[second * 2 + 1], SUG_showAttackBanner, 1);
        hp = SUG_BATTLE->players[first].hp;
        SUG_playAttackTurn(second, 0, NULL, SUG_animateHpCounter, 1);
        applyAnimationFirstFrame(second, 0);
        startModelAnimation(second, 0, -2, 0);
        do {
            waitFrames(FRAME_INTERVAL);
        } while (((ModelData *)SCENE_3D->models[first])->animKeyTimer >= 0);
        if (SUG_BATTLE->players[second].eatUpHp) {
            SUG_applyEatUpHp(second, hp);
        }
    }
    if (SUG_TARGET_HP[first] > 0) {
        startModelAnimation(first, 0, -2, 0);
        goto done;
    }
    winner = second;
    goto done;
firstWins:
    winner = first;
done:
    SUG_SCRIPT_STATES[0] = SUG_SCRIPT_STATES[1] = 3;
    if (winner >= 0) {
        SCENE_3D->modelState[winner] = 1;
        startModelAnimation(winner, 6, -2, 0);
        CAMERA_TARGET_MODEL = winner;
        waitFrames(FRAME_INTERVAL);
        SCENE_3D->modelState[winner ^ 1] = -1;
        SUG_showWinnerBanner(winner);
        waitFrames(8);
    } else {
        SCENE_3D->modelState[0] = 1;
        SCENE_3D->modelState[1] = 1;
    }
    spawnTask(0, 0x1F, 0, 0x800, SUG_runCameraOrbit, 0xA0, 0);
    waitFrames(0x82);
    DUEL->state = 4;
    waitFrames(30);
}
#elif VERSION_US || VERSION_EU
void SUG_playBattleExchange(void) {
    s32 first;
    s32 second;
    s32 other;
    s32 winner;
    s32 hp;
    BattleFlags flags;

    winner = -1;
    flags = SUG_BATTLE->flags.bits;
    first = flags.turn ^ (flags.flag1 | flags.flag3);
    other = first ^ flags.counter;
    second = first ^ 1;
    if (SUG_BATTLE->players[other].damage == 0) {
        other ^= 1;
    }
    SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] = -1;
    SUG_ACTIVE_MODEL = SUG_BATTLE->flags.bits.turn;
    spawnTask(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, BATTLE_START_SKILL, SUG_BATTLE->flags.bits.turn, 1, &SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT]);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (DUEL->state != 3);
    SUG_saveStageClut(SCENE_3D->models[23], SCENE_3D->texAnimFrames);
    spawnTask(0x1A, 0x1F, 0, 0x400, SUG_runStageFadeTask);
    while (SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT] != 1) {
        waitFrames(FRAME_INTERVAL);
    }
    endTask(0x19);
    spawnTask(0x19, -1, 0, 0x800, &runSceneCameraTask, 1);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    DB(0).draw.r0 = DB(1).draw.r0 = STAGE_CLEAR_COLOR[0];
    DB(0).draw.g0 = DB(1).draw.g0 = STAGE_CLEAR_COLOR[1];
    DB(0).draw.b0 = DB(1).draw.b0 = STAGE_CLEAR_COLOR[2];
    SUG_showWireGrid();
    ((ModelData *)SCENE_3D->models[23])->rotX = 0;
    setScreenFadeParams(1, 1, 6);
    SUG_playAttackTurn(SUG_BATTLE->flags.bits.turn ^ 1, 1, BATTLE_START_SKILL, SUG_showHpBanner, 0);
    SUG_playAttackTurn(other, 0, SUG_SKILL_SCRIPTS[other * 2], SUG_showHpBanner, 0);
    SUG_runCameraOrbit(0x78, 1);
    SCENE_3D->modelState[first ^ 1] = -1;
    CAMERA->targetModel = first;
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SUG_SCRIPT_STATES[0] != 1);
    if (SUG_BATTLE->flags.bits.counter) {
        SUG_playSoloAnimation(first, SUG_BATTLE->players[first].attack + 1);
        SUG_showAttackLabel(first);
    } else if (SUG_BATTLE->players[first].damage == 0) {
        SUG_playNoDamageTurn(first);
    } else {
        FADE_TARGET = STAGE_FADE_LEVEL;
        SUG_playAttackTurn(second, 1, SUG_SKILL_SCRIPTS[first * 2 + 1], SUG_showAttackBanner, 1);
        FADE_TARGET = 0xFF;
        hp = SUG_BATTLE->players[second].hp;
        SUG_playAttackTurn(second, 0, SUG_SKILL_SCRIPTS[second * 2], SUG_animateHpCounter, 1);
        applyAnimationFirstFrame(first, 0);
        playModelAnimation(first, 0);
        do {
            waitFrames(FRAME_INTERVAL);
        } while (((ModelData *)SCENE_3D->models[second])->animKeyTimer >= 0);
        if (SUG_BATTLE->players[first].eatUpHp) {
            SUG_applyEatUpHp(first, hp);
        }
        if (SUG_TARGET_HP[second] <= 0) {
            goto firstWins;
        }
        playModelAnimation(second, 0);
        waitFrames(20);
    }
    if (SUG_BATTLE->players[second].damage == 0) {
        SUG_playNoDamageTurn(second);
    } else {
        FADE_TARGET = STAGE_FADE_LEVEL;
        SUG_playAttackTurn(first, 1, SUG_SKILL_SCRIPTS[second * 2 + 1], SUG_showAttackBanner, 1);
        FADE_TARGET = 0xFF;
        hp = SUG_BATTLE->players[first].hp;
        SUG_playAttackTurn(second, 0, NULL, SUG_animateHpCounter, 1);
        applyAnimationFirstFrame(second, 0);
        playModelAnimation(second, 0);
        do {
            waitFrames(FRAME_INTERVAL);
        } while (((ModelData *)SCENE_3D->models[first])->animKeyTimer >= 0);
        if (SUG_BATTLE->players[second].eatUpHp) {
            SUG_applyEatUpHp(second, hp);
        } else {
            if (SUG_TARGET_HP[first] <= 0) {
                goto secondWins;
            }
            playModelAnimation(first, 0);
            waitFrames(30);
        }
    }
    if (SUG_TARGET_HP[first] > 0) {
        playModelAnimation(first, 0);
        goto done;
    }
secondWins:
    winner = second;
    goto done;
firstWins:
    winner = first;
done:
    SUG_SCRIPT_STATES[0] = SUG_SCRIPT_STATES[1] = 3;
    if (winner >= 0) {
        SCENE_3D->modelState[winner] = 1;
        playModelAnimation(winner, 6);
        CAMERA_TARGET_MODEL = winner;
        waitFrames(FRAME_INTERVAL);
        SCENE_3D->modelState[winner ^ 1] = -1;
        SUG_showWinnerBanner(winner);
        waitFrames(8);
    } else {
        SCENE_3D->modelState[0] = 1;
        SCENE_3D->modelState[1] = 1;
    }
    spawnTask(0, 0x1F, 0, 0x800, SUG_runCameraOrbit, 0xA0, 0);
    waitFrames(0x82);
    DUEL->state = 4;
    waitFrames(30);
}
#endif

void SUG_playSoloAnimation(s32 model, s32 anim) {
    SCENE_3D->modelState[model] = 1;
    SCENE_3D->modelState[model ^ 1] = -1;
    CAMERA_TARGET_MODEL = model;
#if VERSION_JP
    startModelAnimation(model, anim, -2, 0);
#elif VERSION_US || VERSION_EU
    playModelAnimation(model, anim);
#endif
}

void SUG_applyEatUpHp(s32 side, s32 amount) {
    s32 state;

    state = -1;
    /* jp orbits the camera around the scene instead of facing the side */
#if VERSION_US || VERSION_EU
    SCENE_3D->modelState[side] = 1;
    CAMERA_TARGET_MODEL = side;
#endif
    spawnTask(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, EAT_UP_HP_SKILL, side, 0, &state);
#if VERSION_JP
    spawnTask(0, 0x1F, 0, 0x800, SUG_runCameraOrbit, 0x50, 1);
#endif
    spawnTask(0, -1, 0, 0x800, SUG_showEatUpHpBanner);
    while (state != 1) {
        waitFrames(FRAME_INTERVAL);
    }
    state = 2;
    if (SUG_BATTLE->players[side].damage < amount) {
        amount = SUG_BATTLE->players[side].damage;
    }
    SUG_TARGET_HP[side] = SUG_BATTLE->players[side].hp + amount;
    if (SUG_TARGET_HP[side] >= 0x2707) {
        SUG_TARGET_HP[side] = 0x2706;
    }
    SUG_animateHpCounter(~side);
    SUG_TARGET_HP[side] -= SUG_BATTLE->players[side ^ 1].damage;
    while (state != 0) {
        waitFrames(FRAME_INTERVAL);
    }
}

void SUG_playNoDamageTurn(s32 model) {
    SCENE_3D->modelState[model] = 1;
    /* jp's models keep their animations loaded */
#if VERSION_JP
    startModelAnimation(model, 0, -2, 0);
#elif VERSION_US || VERSION_EU
    playModelAnimation(model, 0);
#endif
    CAMERA_TARGET_MODEL = model;
    SCENE_3D->modelState[model ^ 1] = -1;
    SUG_showAttackBanner(~model);
}

#if VERSION_JP
void SUG_runPolygonBattle(s32 arg, s32 parentTask) {
    /* jp skips the battle while either player holds one of the two lowest
       buttons, showing a banner until the duel moves on */
    if (!((PAD_STATES[0]->rawHeld | PAD_STATES[1]->rawHeld) & 3)) {
        SUG_BATTLE = &SUG_BATTLE_STATE;
        SUG_BATTLE_STATE.flags.bits.flag1 = PLAYER((s8)(DUEL->turnPlayer ^ 1))->unk178_8;
        SUG_BATTLE_STATE.flags.bits.counter = PLAYER(0)->counter | PLAYER(1)->counter;
        SUG_BATTLE_STATE.flags.bits.turn = DUEL->turnPlayer;
        SUG_BATTLE_STATE.flags.bits.flag3 = PLAYER(DUEL->turnPlayer)->unk178_7;
        SUG_BATTLE_STATE.players[0].hp = PLAYER(0)->unk158;
        SUG_BATTLE_STATE.players[1].hp = PLAYER(1)->unk158;
        SUG_BATTLE_STATE.players[0].damage = PLAYER(1)->damageTaken;
        SUG_BATTLE_STATE.players[1].damage = PLAYER(0)->damageTaken;
        SUG_BATTLE_STATE.players[0].element = PLAYER(0)->controller;
        SUG_BATTLE_STATE.players[1].element = PLAYER(1)->controller;
        SUG_BATTLE_STATE.players[0].attack = PLAYER(0)->usedAttack;
        SUG_BATTLE_STATE.players[1].attack = PLAYER(1)->usedAttack;
        SUG_BATTLE_STATE.players[0].eatUpHp = PLAYER(0)->eatUpHp;
        SUG_BATTLE_STATE.players[1].eatUpHp = PLAYER(1)->eatUpHp;
        SUG_BATTLE_STATE.players[0].crash = PLAYER(0)->crash;
        SUG_BATTLE_STATE.players[1].crash = PLAYER(1)->crash;
        SUG_BATTLE_STATE.players[0].unk8_6 = PLAYER(0)->unk178_13 & 1;
        SUG_BATTLE_STATE.players[1].unk8_6 = PLAYER(1)->unk178_13 & 1;
        SUG_SKILL_SCRIPTS[0] = (void *)DUEL_DIGIMON_MODELS[0].attackModels[SUG_BATTLE_STATE.players[0].attack];
        SUG_SKILL_SCRIPTS[1] = (void *)DUEL_DIGIMON_MODELS[0].unk14[SUG_BATTLE_STATE.players[0].attack];
        SUG_SKILL_SCRIPTS[2] = (void *)DUEL_DIGIMON_MODELS[1].attackModels[SUG_BATTLE_STATE.players[1].attack];
        SUG_SKILL_SCRIPTS[3] = (void *)DUEL_DIGIMON_MODELS[1].unk14[SUG_BATTLE_STATE.players[1].attack];
        SUG_initBattleScene();
        SUG_playBattleExchange();
        SUG_freeBattleScene();
    } else {
        Rect16 uv = { 0, 0xCE, 0xB0, 0x10 };

        do {
            waitFrames(FRAME_INTERVAL);
            drawTexturedSprite(0x48, 0x70, &uv, 0xB, 0x336C, 1, 0xFF, 1);
        } while (DUEL->state != 3);
        SUG_freeBattleScene();
        waitFrames(FRAME_INTERVAL);
        setScreenFadeParams(1, 1, 6);
        waitFrames(0x32);
        DUEL->state = 4;
    }
    resumeTask(parentTask);
}
#elif VERSION_US || VERSION_EU
void SUG_runPolygonBattle(s32 arg, s32 parentTask) {
    s32 i;

    SUG_BATTLE = &SUG_BATTLE_STATE;
    SUG_BATTLE_STATE.flags.bits.flag1 = PLAYER((s8)(DUEL->turnPlayer ^ 1))->unk178_8;
    SUG_BATTLE_STATE.flags.bits.counter = PLAYER(0)->counter | PLAYER(1)->counter;
    SUG_BATTLE_STATE.flags.bits.flag3 = PLAYER(DUEL->turnPlayer)->unk178_7;
    SUG_BATTLE_STATE.flags.bits.turn = DUEL->turnPlayer;
    for (i = 0; i < 2; i++) {
        SUG_BATTLE->players[i].hp = PLAYER(i)->unk158;
        SUG_BATTLE->players[i].damage = PLAYER(i ^ 1)->damageTaken;
        switch (PLAYER(i)->controller) {
        case 0:
        case 2:
            SUG_BATTLE->players[i].element = 0;
            break;
        case 1:
            SUG_BATTLE->players[i].element = 2;
            break;
        case 3:
            SUG_BATTLE->players[i].element = 1;
            break;
        }
        SUG_BATTLE->players[i].attack = PLAYER(i)->usedAttack;
        SUG_BATTLE->players[i].eatUpHp = PLAYER(i)->eatUpHp;
        SUG_BATTLE->players[i].crash = PLAYER(i)->crash;
        SUG_BATTLE->players[i].unk8_6 = PLAYER(i)->unk178_13;
        SUG_SKILL_SCRIPTS[i * 2] = (void *)DUEL_DIGIMON_MODELS[i].attackModels[SUG_BATTLE->players[i].attack];
        SUG_SKILL_SCRIPTS[i * 2 + 1] = (void *)DUEL_DIGIMON_MODELS[i].unk14[SUG_BATTLE->players[i].attack];
    }
    SUG_initBattleScene();
    SUG_playBattleExchange();
    SUG_freeBattleScene();
    DB(0).draw.b0 = DB(1).draw.b0 = 0;
    DB(0).draw.g0 = DB(1).draw.g0 = 0;
    DB(0).draw.r0 = DB(1).draw.r0 = 0;
    resumeTask(parentTask);
}
#endif
