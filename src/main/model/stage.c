#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/stage.h"
#include "dcb/archive.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/model.h"
#include "dcb/model_anim.h"
#include "dcb/player_data.h"
#include "dcb/prim_util.h"
#include "dcb/sound.h"

BgEntry ARENA_STAGES[56] = {
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
        setModelAnimationData(SCENE_3D->unk13C[slot],
                      (s32 *)decompressToHeap(
                          (s32)findPakChunk((Chunk *)((Model2220 *)SCENE_3D->unk13C[slot])->unk26F4, 1, 7), slot + 0x84),
                      7);
        applyAnimationFirstFrame(slot, 7);
    }
    SCENE_3D->unk114[slot] = -1;
    truncatePakTextures((Chunk *)pak);
    return pak;
}

void syncPlayerDigimonModel(s32 player, void *card) {
    s32 modelId;
    s32 loadedId;
    s32 pak;
    void *models;
    void *cardData;

    models = (void *)((s8 *)&DUEL_DIGIMON_MODELS + (player << 5));
    modelId = (*(u8 *)((s8 *)card + 0xE5));
    loadedId = (*(s32 *)((s8 *)models + 0));
    if (modelId != loadedId) {
        (*(s32 *)((s8 *)models + 0)) = -2;
        if ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1) {
            do {
                func_80014C08(FRAME_INTERVAL);
            } while ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1);
        }
        (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 1;
        if (loadedId > 0) {
            unloadModel(player);
            freeHeapBlocksByTag(player + 0x1F4);
            freeHeapBlocksByTag(player + 0x84);
        }
        if (modelId > 0) {
            cardData = findDigimonCardByModelId(modelId);
            pak = loadDigimonModelPak(player, modelId, 0, 0);
            if (pak != 0) {
                (*(s32 *)((s8 *)models + 8)) = loadSkill((s32) (*(s16 *)((s8 *)cardData + 0x22)), pak);
                (*(s32 *)((s8 *)models + 0xC)) = loadSkill((s32) (*(s16 *)((s8 *)cardData + 0x3E)), pak);
                (*(s32 *)((s8 *)models + 0x10)) = loadSkill((s32) (*(s16 *)((s8 *)cardData + 0x5A)), pak);
                (*(s32 *)((s8 *)models + 0x14)) = loadSkill((s32) (*(s16 *)((s8 *)cardData + 0x24)), pak);
                (*(s32 *)((s8 *)models + 0x18)) = loadSkill((s32) (*(s16 *)((s8 *)cardData + 0x40)), pak);
                (*(s32 *)((s8 *)models + 0x1C)) = loadSkill((s32) (*(s16 *)((s8 *)cardData + 0x5C)), pak);
                goto block_9;
            }
        } else {
block_9:
            (*(s32 *)((s8 *)models + 0)) = modelId;
            (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
        }
    }
}

void runDuelStageTask(s32 stageId) {
    s32 pak;
    s32 i;
    u8 *activeCard;

    if (*((s8 *)D_801D8340 + 0x811) == 1) {
        do {
            func_80014C08(FRAME_INTERVAL);
        } while (*((s8 *)D_801D8340 + 0x811) == 1);
    }
    *((s8 *)D_801D8340 + 0x811) = 1;
    *((s8 *)D_801D8340 + 0x813) = 0;
    pak = loadFile((s32) "A:\\BATTLE.PAK", getCurrentTaskId());
    if (pak != 0) {
        uploadTimList(findPakChunk((Chunk *)pak, 5, 0x68));
        D_801D81AC = (void *)loadSkill(999, pak);
        D_801D81B0 = (void *)loadSkill(998, pak);
        truncatePakTextures((Chunk *)pak);
    }
    loadArenaStage(stageId);
    DUEL_DIGIMON_MODELS = (&DUEL_DIGIMON_MODELS)[8] = -1;
    *((s8 *)D_801D8340 + 0x811) = 0;
    do {
        func_80014C08(FRAME_INTERVAL);
        for (i = 0; i < 2; i++) {
            activeCard = *(u8 **)(DUEL_PLAYERS[i] + 0x114);
            if (activeCard != 0 && activeCard[0xE5] != (&DUEL_DIGIMON_MODELS)[i * 8]) {
                syncPlayerDigimonModel(i, activeCard);
            }
        }
    } while (*((s8 *)D_801D8340 + 0x813) == 0);
    for (i = 0; i < 2; i++) {
        if ((&DUEL_DIGIMON_MODELS)[i * 8] > 0) {
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
    *((s8 *)D_801D8340 + 0x813) = 0;
}

void playPolygonBattle(void) {
    do {
        func_80014C08(FRAME_INTERVAL);
    } while (DUEL_DIGIMON_MODELS <= 0 || (&DUEL_DIGIMON_MODELS)[8] <= 0 || *((s8 *)D_801D8340 + 0x811) == 1);
    *((s8 *)D_801D8340 + 0x811) = 1;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, "P:\\sugseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    playLoadedMusic(1);
    func_800149B8(0, -1, 0, 0x2000, D_801EEE90, 0, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    SCENE_3D_ENABLED = 0;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, "P:\\kawseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    playLoadedMusic(0);
    *((s8 *)D_801D8340 + 0x811) = 0;
}

void loadArenaStage(s32 stageId) {
    char path[32];

    if (stageId < 0) {
        stageId = rand() % 12 + 0x2C;
    }
    sprintf(path, "F:\\bg%d.pak", ARENA_STAGES[stageId].bg + 900);
    func_800149B8(0, -1, 0, 0x400, loadFileTagged, path, getCurrentTaskId(), 0x81);
    STAGE_PAK = func_80014C08(0x7FFFFFFF);
    loadModel(0x17, ARENA_STAGES[stageId].bg + 900, 0, STAGE_PAK, 0);
    SCENE_3D->unk114[0x17] = -1;
    ((Model2220 *)SCENE_3D->unk13C[23])->unk26D4 = 0xA0000;
    ((Model2220 *)SCENE_3D->unk13C[23])->unk26D0 = 0x280000;
    if (ARENA_STAGES[stageId].flags & 2) {
        loadModelAnimation(0x17, 0, 0, STAGE_PAK);
        applyAnimationFirstFrame(0x17, 0);
        startModelAnimation(0x17, 0, -2, 0);
    }
    SCENE_3D->unk114[0x18] = SCENE_3D->unk114[0x19] = 0;
    SCENE_3D->unk114[0x1B] = ARENA_STAGES[stageId].unk2;
    SCENE_3D->unk114[0x1A] = ARENA_STAGES[stageId].unk1;
    *(s32 *)&SCENE_3D->unk114[0x24] = ARENA_STAGES[stageId].flags;
    STAGE_CLEAR_COLOR[0] = ARENA_STAGES[stageId].rgb[0];
    STAGE_CLEAR_COLOR[1] = ARENA_STAGES[stageId].rgb[1];
    STAGE_CLEAR_COLOR[2] = ARENA_STAGES[stageId].rgb[2];
    D_8006DF80 = ARENA_STAGES[stageId].unk7;
}

void showArenaStage(s16 rotX) {
    Unk801D6A4C *scene;
    s32 tim;

    SCENE_3D_ENABLED = 1;
    scene = SCENE_3D;
    *(s16 *)((u8 *)scene->unk13C[23] + 0xA78) = rotX;
    tim = decompressForTask((s32)findPakChunk((Chunk *)STAGE_PAK, 5, *(s16 *)((u8 *)scene->unk13C[23] + 6)));
    uploadTim((u32 *)tim, 0x3C0, 0, 0x3F0, 0x70);
    DrawSync(0);
    freeHeapBlock((void *)tim);
    if (rotX != 0 && (*(s32 *)&SCENE_3D->unk114[0x24] & 2)) {
        func_80014A00(0x1B);
        func_800149B8(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
        applyAnimationFirstFrame(0x17, 0);
        startModelAnimation(0x17, 0, -2, 0);
    }
    ((Unk800794F8 *)&GRAPHICS)->unk98[0].draw.r0 = ((Unk800794F8 *)&GRAPHICS)->unk98[1].draw.r0 = STAGE_CLEAR_COLOR[0];
    ((Unk800794F8 *)&GRAPHICS)->unk98[0].draw.g0 = ((Unk800794F8 *)&GRAPHICS)->unk98[1].draw.g0 = STAGE_CLEAR_COLOR[1];
    ((Unk800794F8 *)&GRAPHICS)->unk98[0].draw.b0 = ((Unk800794F8 *)&GRAPHICS)->unk98[1].draw.b0 = STAGE_CLEAR_COLOR[2];
}

void unloadArenaStage(void) {
    unloadModel(0x17);
    freeHeapBlock(STAGE_PAK);
}

void animateStageTexture(u8 *model) {
    s32 vramSlot;

    if (SCENE_3D->unk114[0x1B] != 0) {
        if (++SCENE_3D->unk114[0x19] >= SCENE_3D->unk114[0x1B]) {
            vramSlot = *(s32 *)(model + 0x26D4) / 0x10000 + 5;
            SCENE_3D->unk114[0x19] = 0;
            if (++SCENE_3D->unk114[0x18] >= (u8)SCENE_3D->unk114[0x24] >> 3) {
                SCENE_3D->unk114[0x18] = 0;
            }
            *(s32 *)(model + 0x26D0) =
                (((((vramSlot & 0x10) << 4) + SCENE_3D->unk114[0x18]) << 6 | (vramSlot & 0xF) << 2) - 0x14) << 16;
        }
    }
}

void openSaveScreenFromMap(s32 saveMode) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_OPENSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, saveMode, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (saveMode) {
    case 2:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 1, 1, getCurrentTaskId(), 0);
        break;
    case 4:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
        break;
    }
}

void continueSavedGame(void) {
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 0xFF, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    if (D_801F80C1 != 0) {
        fadeOutScrollingBackground();
        func_800149B8(0, -1, 0, 0x100, runTitleMenu, 0, 0, 0, 0);
        return;
    }
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    if (*(u8 *)(PLAYER_PROFILES + 0xF) == 0) {
        loadMusicTrack(0, 0x6F, 0x7F);
        playLoadedMusic(0);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 0, getCurrentTaskId(), 0);
    } else {
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
    }
}

void openPartnerFusion(s8 mode) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_EVOSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E8E88, (s32 *) mode, 0, 0, 0);
}

void returnToWorldMap(void) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
}

void openDeckEditor(s32 returnTo) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SUBSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, 0, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (returnTo) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, getCurrentTaskId(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
        break;
    }
}

void openPartnerEquipment(s32 returnTo) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SUBSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, 0, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (returnTo) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, getCurrentTaskId(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
        break;
    }
}

void func_8002F298(s32 *param) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SUBSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, param, getCurrentTaskId(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_OPENSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

void func_8002F3C4(s32 *param) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SUBSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, param, getCurrentTaskId(), 1, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_OPENSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

void runTitleMenu(void) {
    s32 stack;
    s32 again;
    s32 choice;

    stack = getCurrentTaskId();
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_OPENSEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    do {
        func_800149B8(0, -1, 0, 0x800, D_801EA2F8, stack, 0, 0, 0);
        choice = func_80014C08(0x7FFFFFFF);
        again = 0;
        switch (choice) {
        case 0:
            changeScrollingBackground(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, D_801E6454, stack, 0, 0, 0);
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1000, loadFileToAddress, &PATH_SAISEG, OVERLAY_LOAD_ADDR, getCurrentTaskId());
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, getCurrentTaskId(), 0, 0);
            break;
        case 1:
            changeScrollingBackground(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, continueSavedGame, 0, 0, 0, 0);
            break;
        case 2:
            changeScrollingBackground(7, 0x380, 0, 0x380, 0x80);
            *((u8 *)D_8006E054 + 0x1028) = 0;
            again = func_801EBD34();
            if (again == 0) {
                func_800149B8(0, -1, 0, 0x800, D_801EB2E8, stack, 0, 0, 0);
            } else {
                fadeOutScrollingBackground();
            }
            break;
        }
    } while (again);
}

void resetScrollingBackground(void) {
    D_801D8260 = 0;
}

void loadScrollingBackground(void) {
    s16 texWindow[4];
    s32 i;
    s8 *bg;
    s8 *buf;

    bg = (s8 *)&SCROLL_BACKGROUND;
    if ((*(s32 *)(bg + 0x68)) != 0) {
        return;
    }
    (*(s8 *)(bg + 0x6C)) = -1;
    (*(s8 *)(bg + 0x6D)) = -1;
    (*(s16 *)(bg + 0x72)) = 0;
    (*(s16 *)(bg + 0x70)) = 0;
    (*(s8 *)(bg + 0x6E)) = 0;
    (*(s8 *)(bg + 0x6F)) = 5;
    for (i = 0; i < 2; i++) {
        buf = (s8 *)&SCROLL_BACKGROUND + i * 0x34;
        initPrimByType(0xE, buf, 0, 0);
        (*(s16 *)(buf + 0x10)) = 0x141;
        (*(s16 *)(buf + 0x12)) = 0xF0;
        texWindow[0] = 0;
        texWindow[1] = 0;
        texWindow[2] = 0;
        texWindow[3] = 0;
        SetTexWindow((s8 *)&D_801D8220 + i * 0x34, texWindow);
    }
    func_800149B8(0, -1, 0, 0x800, loadFileTagged, &PATH_BG_ARC, getCurrentTaskId(), -2);
    D_801D8260 = func_80014C08(0x7FFFFFFF);
}

void freeScrollingBackground(void) {
    s8 *bg = (s8 *)&SCROLL_BACKGROUND;

    freeHeapBlock(*(void **)(bg + 0x68));
    *(void **)(bg + 0x68) = 0;
    hideScrollingBackground();
}

void changeScrollingBackground(s32 image, s32 x, s32 y, s32 w, s32 h) {
    s32 i;

    if (SCROLL_BACKGROUND.unk72 != 0 && SCROLL_BACKGROUND.unk72 != 0x80) {
        do {
            func_80014C08(FRAME_INTERVAL);
        } while (SCROLL_BACKGROUND.unk72 != 0 && SCROLL_BACKGROUND.unk72 != 0x80);
    }
    if (SCROLL_BACKGROUND.unk72 == 0) {
        SCROLL_BACKGROUND.unk6D = -1;
    }
    SCROLL_BACKGROUND.mode = image;
    if (image >= 0) {
        SCROLL_BACKGROUND.x = x;
        SCROLL_BACKGROUND.y = y;
        SCROLL_BACKGROUND.w = w;
        SCROLL_BACKGROUND.h = h;
        for (i = 0; i < 2; i++) {
            (SCROLL_BACKGROUND.buf + i)->clut = getClut(w, h);
            SetDrawTPage((SCROLL_BACKGROUND.buf + i)->tpage, 0, 0, GetTPage(0, 0, x, y));
        }
    }
    SCROLL_BACKGROUND.unk6E = 0;
    SCROLL_BACKGROUND.unk6F = 0x1E;
}

void hideScrollingBackground(void) {
    s8 *bg;

    bg = (s8 *)&SCROLL_BACKGROUND;
    bg[0x6C] = -1;
    bg[0x6D] = -1;
    (*(s16 *)(bg + 0x72)) = 0;
    (*(s16 *)(bg + 0x70)) = 0;
}

void fadeOutScrollingBackground(void) {
    D_801D8264 = -1;
}

void setBackgroundScrollMode(s8 scrollMode) {
    D_801D8266 = scrollMode;
}
