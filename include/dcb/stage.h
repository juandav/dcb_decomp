#ifndef DCB_STAGE_H
#define DCB_STAGE_H

#include "game.h"

typedef struct {
    /* 0x00 */ s32 modelId;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 attackModels[3]; /* skills[0] of each attack */
    /* 0x14 */ s32 unk14[3];        /* skills[1] of each attack */
} DuelDigimonModels;

extern DuelDigimonModels DUEL_DIGIMON_MODELS[2];
extern void *BATTLE_START_SKILL;
extern void *EAT_UP_HP_SKILL;
extern ArenaStage ARENA_STAGES[];
extern s32 STAGE_FADE_LEVEL;
extern u8 STAGE_CLEAR_COLOR[3];
extern s32 STAGE_PAK;

void animateStageTexture(Model *model);
#if VERSION_JP
s32 loadDigimonModelPak(s32 slot, s32 id);
#elif VERSION_US || VERSION_EU
s32 loadDigimonModelPak(s32 slot, s32 id, s8 format, s32 loadAllAnims);
#endif
void syncPlayerDigimonModel(s32 player, DigimonCardData *card);
void unloadArenaStage(void);
void loadArenaStage(s32 stageId);
#if VERSION_JP
void runDuelStageTask(s32 stageId, s32 music);
#elif VERSION_US || VERSION_EU
void runDuelStageTask(s32 stageId);
#endif
void playPolygonBattle(void);
void showArenaStage(s16 rotX);

#endif /* DCB_STAGE_H */
