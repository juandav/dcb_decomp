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

extern s16 SUG_CURRENT_SCRIPT;
extern s32 SUG_SCRIPT_STATES[2];
extern s16 SUG_ACTIVE_MODEL;
extern s32 SUG_PREV_MODEL;
extern void *SUG_SKILL_SCRIPTS[4];
extern BattleState SUG_BATTLE_STATE;

void SUG_playSoloAnimation(s32 model, s32 anim);
void SUG_applyEatUpHp(s32 side, s32 amount);
void SUG_playNoDamageTurn(s32 model);

s32 SUG_SCREEN_FX_PHASE = 0;

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
    s32 unused[8];

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
    endTask(0x1A);
    removeFrameCallback((s32)renderSceneModels);
    removeFrameCallback((s32)renderWireGrid);
    freeHeapBlock(DB(0).scenePackets);
    freeHeapBlock(DB(1).scenePackets);
    SUG_freeSprites();
    SUG_freeTamCache();
    func_80022E58();
    SUG_resetStageBrightness();
}

void SUG_runCameraOrbit(s32 frames, s32 resetCamera) {
    Graphics *camera;

    if (resetCamera) {
        camera = (Graphics *)&GRAPHICS;
        *(s32 *)&camera->pad5A[0xE] = 0x9C4000;
        camera->unk90 = 4000;
        camera->snapCamera = 1;
        camera->unk8E = 400;
        *(s32 *)&camera->pad5A[0x16] = 0x190000;
    }
    D_80079584 = -1;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = 1;
    do {
        frames--;
        SUG_orbitCamera();
        waitFrames(FRAME_INTERVAL);
    } while (frames >= 0);
}

void SUG_playAttackTurn(s32 model, s32 a1, void *script, void (*fn)(s32), s32 switchModel) {
    s16 other;

    other = SUG_CURRENT_SCRIPT ^ 1;
    if (switchModel) {
        playModelAnimation(SUG_ACTIVE_MODEL, 0);
        SCENE_3D->modelState[SUG_ACTIVE_MODEL] = 1;
        if (SUG_ACTIVE_MODEL != SUG_PREV_MODEL) {
            SCENE_3D->modelState[SUG_PREV_MODEL] = -1;
            applyAnimationFirstFrame(SUG_PREV_MODEL, 0);
        }
        D_80079584 = SUG_ACTIVE_MODEL;
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
    spawnTask(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, D_801D81AC, SUG_BATTLE->flags.bits.turn, 1, &SUG_SCRIPT_STATES[SUG_CURRENT_SCRIPT]);
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
    ((ModelData *)SCENE_3D->models[23])->unkA78 = 0;
    setScreenFadeParams(1, 1, 6);
    SUG_playAttackTurn(SUG_BATTLE->flags.bits.turn ^ 1, 1, D_801D81AC, SUG_showHpBanner, 0);
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
        } while (((ModelData *)SCENE_3D->models[second])->unk2208 >= 0);
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
        } while (((ModelData *)SCENE_3D->models[first])->unk2208 >= 0);
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
        D_80079584 = winner;
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

void SUG_playSoloAnimation(s32 model, s32 anim) {
    SCENE_3D->modelState[model] = 1;
    SCENE_3D->modelState[model ^ 1] = -1;
    D_80079584 = model;
    playModelAnimation(model, anim);
}

void SUG_applyEatUpHp(s32 side, s32 amount) {
    s32 state;

    state = -1;
    SCENE_3D->modelState[side] = 1;
    D_80079584 = side;
    spawnTask(0, 0x1F, 0, 0x2000, SUG_runEffectScriptTask, D_801D81B0, side, 0, &state);
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
    playModelAnimation(model, 0);
    D_80079584 = model;
    SCENE_3D->modelState[model ^ 1] = -1;
    SUG_showAttackBanner(~model);
}

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
