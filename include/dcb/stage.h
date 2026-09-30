#ifndef DCB_STAGE_H
#define DCB_STAGE_H

#include "game.h"

typedef struct {
    /* 0x00 */ s32 modelId;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 attackModels[3];
    /* 0x14 */ s32 unk14[3];
} DuelDigimonModels;

extern DuelDigimonModels DUEL_DIGIMON_MODELS[2];
extern void *D_801D81AC;
extern void *D_801D81B0;
extern u8 D_801EEE90[];
extern ArenaStage ARENA_STAGES[];
extern s32 D_8006DF80;
extern u8 STAGE_CLEAR_COLOR[3];
extern s32 STAGE_PAK;
extern u8 D_801F80C1;
extern s32 D_801D8260;
extern s32 D_801D8220;
extern s8 D_801D8264;
extern s8 D_801D8266;

void animateStageTexture(u8 *model);
s32 loadDigimonModelPak(s32 slot, s32 id, s8 format, s32 loadAllAnims);
void syncPlayerDigimonModel(s32 player, void *card);
void unloadArenaStage(void);
void loadArenaStage(s32 stageId);
void runDuelStageTask(s32 stageId);
void playPolygonBattle(void);
void showArenaStage(s16 rotX);

#endif /* DCB_STAGE_H */
