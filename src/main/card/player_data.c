#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/player_data.h"
#include "dcb/archive.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/model.h"

void initPlayerData(void) {
    void *session;

    loadCardDatabase();
    PLAYER_PROFILES = allocPermanentHeapBlock(0x4EE8);
    D_8006E054 = session = allocPermanentHeapBlock(0x102C);
    (*(void **)((s8 *)D_8006E054 + 0x100C)) = allocPermanentHeapBlock(0x1AC);
    resetPlayerData();
}

void func_8002D458(void) {
    s32 i;

    ((Unk8006E054 *)D_8006E054)->unk1027 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A4 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A2 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A9 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A8 = 0;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)PLAYER_PROFILES)->unk23FC[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        ((Unk8006E050 *)PLAYER_PROFILES)->unk242C[i] = 0;
    }
    ((Unk8006E050 *)PLAYER_PROFILES)->unk2C = 0;
    ((Unk8006E050 *)PLAYER_PROFILES)->unk14 = 0;
}

void resetPlayerData(void) {
    Unk8006E050 *profile;
    s32 player;
    s32 j;
    s32 i;

    profile = (Unk8006E050 *)PLAYER_PROFILES;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)PLAYER_PROFILES)->unk23FC[i] = 0;
    }
    ((Unk8006E050 *)PLAYER_PROFILES)->unk28_9 = 0;
    for (player = 0; player < 2; player++, profile++) {
        profile->name[0] = 0;
        profile->unk18 = 0;
        profile->unk1A = 0;
        profile->unk1C = 0;
        profile->unk1E = 0;
        profile->unkE = 0;
        profile->unk10 = rand();
        profile->unk28_10 = 0;
        profile->unk28_13 = 0;
        profile->unkD = 0;
        profile->unk28_11 = 0;
        profile->unk28_12 = 0;
        profile->rankA = 0;
        profile->rankB = 0;
        profile->rankC = 0;
        profile->unk16 = 0x2774;
        profile->unk4C = 0;
        profile->unk4E = 0;
        profile->unk50 = 0;
        profile->unk52 = 0;
        profile->unk54 = 0;
        profile->unk56 = 0;
        for (i = 0; i < 3; i++) {
            profile->unk36[i] = 0;
        }
        for (i = 0; i < 0x28; i++) {
            profile->unk58[i] = 0;
        }
        for (i = 0; i < 0x12D; i++) {
            profile->unk14B2[i] = 0;
            for (j = 0; j < 8; j++) {
                func_80045968(player, i, j);
            }
        }
        for (i = 0; i < 0xBF; i++) {
            for (j = 0; j < 3; j++) {
                profile->unkD3C[i][j] = 0;
            }
            profile->unk11B6[i] = 0;
            profile->unk1334[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            profile->unk80[i].unk288 = 0;
        }
        for (i = 0; i < 0x10; i++) {
            profile->unk3C[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            profile->unk2438[i].unk0 = 0;
            profile->unk2438[i].unk108[0] = 0;
            profile->unk2438[i].unk108[1] = 0;
            profile->unk2438[i].unk108[2] = 0;
        }
        for (i = 0; i < 0x9F; i++) {
            profile->unkAC0[i] = 0;
            profile->unkBFE[i] = 0;
        }
        for (i = 0; i < 0x8E; i++) {
            profile->unk888[i] = 0;
            profile->unk9A4[i] = 0;
        }
        for (j = 0; j < 0x20; j++) {
            profile->unk848[j] = 0;
        }
        profile->unk20_0 = 0;
        profile->unk20_1 = 0;
        profile->unk20_2 = 0;
        profile->unk20_3 = 0;
        profile->unk24 = 0;
    }
    strcpy(((Unk8006E050 *)PLAYER_PROFILES)->name, "Player");
    func_8002D458();
}

void renderFullscreenBackground(void) {
    CUR_SPRT->sp.x0 = 0;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x100;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x85);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
    CUR_SPRT->sp.x0 = 0x100;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x40;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x87);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->sp);
    addPrim(&CURRENT_FRAME_BUFFER->ot[0], &CUR_SPRT->dm);
    SPRITE_POOL_CURSOR += sizeof(SprtPacket);
}

void playModelAnimation(s32 modelSlot, s32 animId) {
    void *model;

    model = SCENE_3D->unk13C[modelSlot];
    if ((*(s32 *)((s8 *)model + 0x2200)) != animId) {
        freeHeapBlocksByTag(modelSlot + 0x84);
        setModelAnimationData(model, (s32 *)decompressToHeap((s32)findPakChunk(*(Chunk **)((s8 *)model + 0x26F4), 1, animId), modelSlot + 0x84), animId);
    }
    startModelAnimation(modelSlot, animId, -2, 0);
}

void setModelAnimationPose(s32 modelSlot, s32 animId) {
    s32 heapTag;
    void *model;

    model = SCENE_3D->unk13C[modelSlot];
    heapTag = modelSlot + 0x84;
    freeHeapBlocksByTag(heapTag);
    setModelAnimationData(model, (s32 *)decompressToHeap((s32)findPakChunk(*(Chunk **)((s8 *)model + 0x26F4), 1, animId), heapTag), animId);
    applyAnimationFirstFrame(modelSlot, animId);
}

void *findDigimonCardByModelId(s32 modelId) {
    u8 *card;
    s32 i;

    card = DIGIMON_CARDS;
    if (card[0xE5] != modelId) {
        i = 0;
        do {
            i++;
            card += 0x13C;
            if (i >= 0xBF) {
                break;
            }
        } while (card[0xE5] != modelId);
    }
    return card;
}

s32 loadSkill(s32 skillId, s32 pak) {
    char path[32];
    s32 skill;

    skill = (s32)findPakChunk((Chunk *)pak, 2, skillId);
    if (skill == 0) {
        sprintf(path, &FMT_SKILL_PATH, skillId);
        skill = loadFileTagged(path, getCurrentTaskId(), 0x81);
    }
    return skill;
}

void loadSkillFromDisc(s32 skillId) {
    loadSkill(skillId, 0);
}

INCLUDE_RODATA("asm/main/nonmatchings/card/player_data", FMT_SKILL_PATH);
