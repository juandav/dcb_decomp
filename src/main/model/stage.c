#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/stage.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/anim_control.h"
#include "dcb/model_load.h"
#include "dcb/model_anim.h"
#include "dcb/player_data.h"
#include "dcb/prim_util.h"
#include "dcb/transform.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"
#include "dcb/overlay_calls.h"

ArenaStage ARENA_STAGES[56] = {
    { 0x50, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x51, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x52, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x53, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x58, 8, 5, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x59, 8, 2, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x5B, 8, 2, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x5D, 0x10, 0xC, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x5F, 0x10, 6, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x61, 0x10, 4, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x5E, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x13, 4, 0, 0x20, { 0, 0, 0 }, 0x80 },
    { 0x62, 0x10, 0xA, -0x80, { 0, 0, 0 }, 0xFF },
    { 0x46, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x48, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4D, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4E, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4F, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4C, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4A, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x49, 8, 0xA, 0x40, { 0, 0, 0 }, 0xFF },
    { 0x57, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x56, 0x10, 0xF, -0x7E, { 0, 0, 0 }, 0xFF },
    { 0x55, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x45, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x4B, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x5A, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x54, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x43, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x47, 0, 0, 2, { 0, 0, 0 }, 0xFF },
    { 0x42, 0, 0, 0, { 0, 0, 0 }, 0xFF },
    { 0x40, 0x10, 0xF, -0x7E, { 0, 0, 0 }, 0xFF },
    { 0, 0xD, 0, 0x68, { 0, 0, 0 }, 0x46 },
    { 1, 7, 0, 0x38, { 0, 0, 0 }, 0x46 },
    { 2, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
    { 3, 0xA, 0, 0x52, { 0, 0, 0 }, 0x50 },
    { 4, 7, 0, 0x3A, { 0, 0, 0 }, 0x80 },
    { 5, 8, 0, 0x40, { 0, 0, 0 }, 0x80 },
    { 6, 8, 0, 0x40, { 0, 0, 0 }, 0x50 },
    { 7, 0xA, 0, 0x50, { 0, 0, 0 }, 0x80 },
    { 8, 3, 0, 0x18, { 0, 0x4C, 0x8C }, 0x46 },
    { 9, 6, 0, 0x30, { 0, 0, 0 }, 0x64 },
    { 0xA, 0xE, 0, 0x72, { 0, 0, 0 }, 0x80 },
    { 0xB, 6, 0, 0x30, { 0, 0, 0 }, 0x80 },
    { 0xC, 4, 0, 0x20, { 0xC8, 0xC8, 0xC8 }, 0x50 },
    { 0xD, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
    { 0xE, 0xC, 0, 0x62, { 0, 0, 0 }, 0x80 },
    { 0xF, 0xA, 0, 0x52, { 0, 0, 0 }, 0x50 },
    { 0x10, 0x10, 0, -0x7E, { 0, 0, 0 }, 0x80 },
    { 0x11, 0xA, 0, 0x52, { 0, 0, 0 }, 0x80 },
    { 0x12, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
    { 0x13, 4, 0, 0x20, { 0, 0, 0 }, 0x80 },
    { 0x14, 5, 0, 0x28, { 0, 0, 0 }, 0x80 },
    { 0x15, 0xA, 0, 0x50, { 0, 0, 0 }, 0x80 },
    { 0x16, 7, 0, 0x3A, { 0, 0, 0 }, 0x80 },
    { 0x17, 7, 0, 0x38, { 0, 0, 0 }, 0x80 },
};

/* Heap tags: slot + 0x1F4 holds a model's PAK, slot + 0x84 its animation data,
   0x81 the skills and the stage. */
s32 loadDigimonModelPak(s32 slot, s32 id, s8 format, s32 loadAllAnims) {
    char path[32];
    s32 pak;

    if (format == 1) {
        sprintf(path, "G:\\%03d.PAK", id);
    } else {
        sprintf(path, "F:\\%03d.PAK", id);
    }
    pak = loadFileTagged((s32 *)path, getCurrentTaskId(), slot + 0x1F4);
    if (loadModel(slot, id, -1, pak, format) == 0) {
        return pak;
    }
    if (loadAllAnims != 0) {
        loadModelAnimation(slot, 0, 0, pak);
        loadModelAnimation(slot, 7, 7, pak);
        loadModelAnimation(slot, 1, 1, pak);
        loadModelAnimation(slot, 2, 2, pak);
        loadModelAnimation(slot, 3, 3, pak);
        loadModelAnimation(slot, 4, 4, pak);
        loadModelAnimation(slot, 5, 5, pak);
        loadModelAnimation(slot, 6, 6, pak);
    } else {
        setModelAnimationData(SCENE_3D->models[slot],
                              (s32 *)decompressToHeap(
                                  (s32)findPakChunk((Chunk *)((Model *)SCENE_3D->models[slot])->pak, 1, 7), slot + 0x84),
                              7);
        applyAnimationFirstFrame(slot, 7);
    }
    SCENE_3D->modelState[slot] = -1;
    truncatePakTextures((Chunk *)pak);
    return pak;
}

void syncPlayerDigimonModel(s32 player, DigimonCardData *card) {
    s32 modelId;
    s32 loadedId;
    s32 pak;
    DuelDigimonModels *models;
    DigimonCardData *cardData;

    models = &DUEL_DIGIMON_MODELS[player];
    modelId = card->modelId;
    loadedId = models->modelId;
    if (modelId != loadedId) {
        models->modelId = -2;
        if (DUEL->loadBusy == 1) {
            do {
                waitFrames(FRAME_INTERVAL);
            } while (DUEL->loadBusy == 1);
        }
        DUEL->loadBusy = 1;
        if (loadedId > 0) {
            unloadModel(player);
            freeHeapBlocksByTag(player + 0x1F4);
            freeHeapBlocksByTag(player + 0x84);
        }
        if (modelId > 0) {
            cardData = findDigimonCardByModelId(modelId);
            pak = loadDigimonModelPak(player, modelId, 0, 0);
            if (pak == 0) {
                /* leaves loadBusy set */
                return;
            }
            models->attackModels[0] = loadSkill(cardData->attack[0].skills[0], pak);
            models->attackModels[1] = loadSkill(cardData->attack[1].skills[0], pak);
            models->attackModels[2] = loadSkill(cardData->attack[2].skills[0], pak);
            models->unk14[0] = loadSkill(cardData->attack[0].skills[1], pak);
            models->unk14[1] = loadSkill(cardData->attack[1].skills[1], pak);
            models->unk14[2] = loadSkill(cardData->attack[2].skills[1], pak);
        }
        models->modelId = modelId;
        DUEL->loadBusy = 0;
    }
}

/* Loads the arena and keeps both players' Digimon models in sync with their
   battle cards until the duel sets stopStageTask. */
void runDuelStageTask(s32 stageId) {
    s32 pak;
    s32 i;
    DigimonCardData *battleCard;

    if (DUEL->loadBusy == 1) {
        do {
            waitFrames(FRAME_INTERVAL);
        } while (DUEL->loadBusy == 1);
    }
    DUEL->loadBusy = 1;
    DUEL->stopStageTask = 0;
    pak = loadFile((s32) "A:\\BATTLE.PAK", getCurrentTaskId());
    if (pak != 0) {
        uploadTimList(findPakChunk((Chunk *)pak, 5, 0x68));
        D_801D81AC = (void *)loadSkill(999, pak);
        EAT_UP_HP_SKILL = (void *)loadSkill(998, pak);
        truncatePakTextures((Chunk *)pak);
    }
    loadArenaStage(stageId);
    DUEL_DIGIMON_MODELS[0].modelId = DUEL_DIGIMON_MODELS[1].modelId = -1;
    DUEL->loadBusy = 0;
    do {
        waitFrames(FRAME_INTERVAL);
        for (i = 0; i < 2; i++) {
            battleCard = (DigimonCardData *)PLAYER(i)->battleCard;
            if (battleCard != 0 && battleCard->modelId != DUEL_DIGIMON_MODELS[i].modelId) {
                syncPlayerDigimonModel(i, battleCard);
            }
        }
    } while (DUEL->stopStageTask == 0);
    for (i = 0; i < 2; i++) {
        if (DUEL_DIGIMON_MODELS[i].modelId > 0) {
            unloadModelAnimations(i);
            unloadModel(i);
        }
    }
    unloadArenaStage();
    freeHeapBlocksByTag(0x1F4);
    freeHeapBlocksByTag(0x84);
    freeHeapBlocksByTag(0x1F5);
    freeHeapBlocksByTag(0x85);
    freeHeapBlocksByTag(0x81);
    DUEL->stopStageTask = 0;
}

void playPolygonBattle(void) {
    do {
        waitFrames(FRAME_INTERVAL);
    } while (DUEL_DIGIMON_MODELS[0].modelId <= 0 || DUEL_DIGIMON_MODELS[1].modelId <= 0 || DUEL->loadBusy == 1);
    DUEL->loadBusy = 1;
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\sugseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    playLoadedMusic(1);
    spawnTask(0, -1, 0, 0x2000, SUG_runPolygonBattle, 0, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    SCENE_3D_ENABLED = 0;
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, loadFileToAddress, "P:\\kawseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    playLoadedMusic(0);
    DUEL->loadBusy = 0;
}

void loadArenaStage(s32 stageId) {
    char path[32];

    /* a negative id picks one of the last 12 stages at random */
    if (stageId < 0) {
        stageId = rand() % 12 + 0x2C;
    }
    sprintf(path, "F:\\bg%d.pak", ARENA_STAGES[stageId].bg + 900);
    spawnTask(0, -1, 0, 0x400, loadFileTagged, path, getCurrentTaskId(), 0x81);
    STAGE_PAK = waitFrames(0x7FFFFFFF);
    /* the stage is model slot 23 */
    loadModel(0x17, ARENA_STAGES[stageId].bg + 900, 0, STAGE_PAK, 0);
    SCENE_3D->modelState[0x17] = -1;
    ((Model *)SCENE_3D->models[23])->tpageOffset = 0xA0000;
    ((Model *)SCENE_3D->models[23])->clutOffset = 0x280000;
    if (ARENA_STAGES[stageId].flags & 2) {
        loadModelAnimation(0x17, 0, 0, STAGE_PAK);
        applyAnimationFirstFrame(0x17, 0);
        startModelAnimation(0x17, 0, -2, 0);
    }
    SCENE_3D->texAnimFrame = SCENE_3D->texAnimTimer = 0;
    SCENE_3D->texAnimDelay = ARENA_STAGES[stageId].texAnimDelay;
    SCENE_3D->texAnimFrames = ARENA_STAGES[stageId].texAnimFrames;
    SCENE_3D->stageFlags = ARENA_STAGES[stageId].flags;
    STAGE_CLEAR_COLOR[0] = ARENA_STAGES[stageId].rgb[0];
    STAGE_CLEAR_COLOR[1] = ARENA_STAGES[stageId].rgb[1];
    STAGE_CLEAR_COLOR[2] = ARENA_STAGES[stageId].rgb[2];
    STAGE_FADE_LEVEL = ARENA_STAGES[stageId].fadeLevel;
}

void showArenaStage(s16 rotX) {
    Scene3D *scene;
    s32 tim;

    SCENE_3D_ENABLED = 1;
    scene = SCENE_3D;
    ((Model *)scene->models[23])->rot.vx = rotX;
    tim = decompressForTask((s32)findPakChunk((Chunk *)STAGE_PAK, 5, ((Model *)scene->models[23])->id));
    uploadTim((u32 *)tim, 0x3C0, 0, 0x3F0, 0x70);
    DrawSync(0);
    freeHeapBlock((void *)tim);
    if (rotX != 0 && (SCENE_3D->stageFlags & 2)) {
        endTask(0x1B);
        spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
        applyAnimationFirstFrame(0x17, 0);
        startModelAnimation(0x17, 0, -2, 0);
    }
    /* the background is cleared to the stage's colour */
    DB(0).draw.r0 = DB(1).draw.r0 = STAGE_CLEAR_COLOR[0];
    DB(0).draw.g0 = DB(1).draw.g0 = STAGE_CLEAR_COLOR[1];
    DB(0).draw.b0 = DB(1).draw.b0 = STAGE_CLEAR_COLOR[2];
}

void unloadArenaStage(void) {
    unloadModel(0x17);
    freeHeapBlock(STAGE_PAK);
}

/* Every texAnimDelay frames, moves the stage's CLUT to the next frame of its
   texture animation (stageFlags >> 3 frames). */
void animateStageTexture(Model *model) {
    s32 vramSlot;

    if (SCENE_3D->texAnimDelay != 0) {
        if (++SCENE_3D->texAnimTimer >= SCENE_3D->texAnimDelay) {
            vramSlot = model->tpageOffset / 0x10000 + 5;
            SCENE_3D->texAnimTimer = 0;
            if (++SCENE_3D->texAnimFrame >= (u8)SCENE_3D->stageFlags >> 3) {
                SCENE_3D->texAnimFrame = 0;
            }
            model->clutOffset =
                (((((vramSlot & 0x10) << 4) + SCENE_3D->texAnimFrame) << 6 | (vramSlot & 0xF) << 2) - 0x14) << 16;
        }
    }
}
