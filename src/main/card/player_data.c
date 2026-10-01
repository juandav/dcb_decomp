#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/player_data.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/card_db.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/anim_control.h"
#include "dcb/model_load.h"

void initPlayerData(void) {
    void *session;

    loadCardDatabase();
    PLAYER_PROFILES = allocPermanentHeapBlock(sizeof(PlayerProfile) * 2);
    SESSION_DATA = session = allocPermanentHeapBlock(0x102C);
    ((SessionData *)SESSION_DATA)->areaSession = allocPermanentHeapBlock(0x1AC);
    resetPlayerData();
}

void resetScriptProgress(void) {
    s32 i;

    ((SessionData *)SESSION_DATA)->playWithoutSaving = 0;
    ((SessionData *)SESSION_DATA)->areaSession->area = 0;
    ((SessionData *)SESSION_DATA)->areaSession->unk1A2 = 0;
    ((SessionData *)SESSION_DATA)->areaSession->resumeMode = 0;
    ((SessionData *)SESSION_DATA)->areaSession->location = 0;
    for (i = 0; i < 12; i++) {
        PLAYER_DATA(0).areaScriptFlags[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        PLAYER_DATA(0).areaScriptValues[i] = 0;
    }
    PLAYER_DATA(0).scriptFlags = 0;
    PLAYER_DATA(0).completionPoints = 0;
}

void resetPlayerData(void) {
    PlayerProfile *profile;
    s32 player;
    s32 j;
    s32 i;

    profile = (PlayerProfile *)PLAYER_PROFILES;
    for (i = 0; i < 12; i++) {
        PLAYER_DATA(0).areaScriptFlags[i] = 0;
    }
    PLAYER_DATA(0).unk28_9 = 0;
    for (player = 0; player < 2; player++, profile++) {
        profile->name[0] = 0;
        profile->battleWins = 0;
        profile->battleLosses = 0;
        profile->versusWins = 0;
        profile->versusLosses = 0;
        profile->areaId = 0;
        profile->profileId = rand();
        profile->tradeUnlocked = 0;
        profile->unk28_13 = 0;
        profile->saveCount = 0;
        profile->hasTraded = 0;
        profile->unk28_12 = 0;
        profile->tamerRank = 0;
        profile->collectorRank = 0;
        profile->battleRank = 0;
        profile->profileSize = sizeof(PlayerProfile);
        profile->cardsReceived = 0;
        profile->cardsGivenAway = 0;
        profile->fusedCards = 0;
        profile->fusionCardsUsed = 0;
        profile->fusionMutations = 0;
        profile->activePartner = 0;
        for (i = 0; i < 3; i++) {
            profile->attackCounts[i] = 0;
        }
        for (i = 0; i < 0x28; i++) {
            profile->unk58[i] = 0;
        }
        for (i = 0; i < 0x12D; i++) {
            profile->cardCollection[i] = 0;
            for (j = 0; j < 8; j++) {
                assignCardCopySerial(player, i, j);
            }
        }
        for (i = 0; i < 0xBF; i++) {
            for (j = 0; j < 3; j++) {
                profile->maxAttackPowers[i][j] = 0;
            }
            profile->cardWins[i] = 0;
            profile->cardLosses[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            profile->partners[i].cardId = 0;
        }
        for (i = 0; i < 0x10; i++) {
            profile->ownedAbilities[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            profile->savedDecks[i].inUse = 0;
            /* the original clears the record twice; GCC 2.95 drops the first */
            profile->savedDecks[i].wins = 0;
            profile->savedDecks[i].losses = 0;
            profile->savedDecks[i].saveCount = 0;
            profile->savedDecks[i].wins = 0;
            profile->savedDecks[i].losses = 0;
        }
        for (i = 0; i < 0x9F; i++) {
            profile->opponentDeckFlags[i] = 0;
            profile->opponentDeckLosses[i] = 0;
        }
        for (i = 0; i < 0x8E; i++) {
            profile->comWins[i] = 0;
            profile->comLosses[i] = 0;
        }
        for (j = 0; j < 0x20; j++) {
            profile->bonusCounts[j] = 0;
        }
        profile->monoSound = 0;
        profile->unk20_1 = 0;
        profile->unk20_2 = 0;
        profile->skipBattleAnimation = 0;
        profile->playTime = 0;
    }
    strcpy(PLAYER_DATA(0).name, "Player");
    resetScriptProgress();
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
    Model *model;

    model = SCENE_3D->models[modelSlot];
    /* reload the animation data (heap tag 0x84 + slot) only for a new clip */
    if (model->anim.clip != animId) {
        freeHeapBlocksByTag(modelSlot + 0x84);
        setModelAnimationData(model, (s32 *)decompressToHeap((s32)findPakChunk(model->pak, 1, animId), modelSlot + 0x84), animId);
    }
    startModelAnimation(modelSlot, animId, -2, 0);
}

void setModelAnimationPose(s32 modelSlot, s32 animId) {
    s32 heapTag;
    Model *model;

    model = SCENE_3D->models[modelSlot];
    heapTag = modelSlot + 0x84;
    freeHeapBlocksByTag(heapTag);
    setModelAnimationData(model, (s32 *)decompressToHeap((s32)findPakChunk(model->pak, 1, animId), heapTag), animId);
    applyAnimationFirstFrame(modelSlot, animId);
}

/* the first of the 0xBF Digimon cards that uses modelId (the last one if none does) */
void *findDigimonCardByModelId(s32 modelId) {
    DigimonCardData *card;
    s32 i;

    card = (DigimonCardData *)DIGIMON_CARDS;
    for (i = 0; i < 0xBF; i++, card++) {
        if (card->modelId == modelId) {
            break;
        }
    }
    return card;
}

s32 loadSkill(s32 skillId, s32 pak) {
    char path[32];
    s32 skill;

    skill = (s32)findPakChunk((Chunk *)pak, 2, skillId);
    if (skill == 0) {
        sprintf(path, "E:\\SKILL\\SKILL%d.MSD", skillId);
        skill = loadFileTagged(path, getCurrentTaskId(), 0x81);
    }
    return skill;
}

void loadSkillFromDisc(s32 skillId) {
    loadSkill(skillId, 0);
}
