#ifndef DCB_PLAYER_DATA_H
#define DCB_PLAYER_DATA_H

#include "game.h"


void initPlayerData(void);
void func_8002D458(void);
void resetPlayerData(void);
void renderFullscreenBackground(void);
void playModelAnimation(s32 modelSlot, s32 animId);
void setModelAnimationPose(s32 modelSlot, s32 animId);
void *findDigimonCardByModelId(s32 modelId);
s32 loadSkill(s32 skillId, s32 pak);
void loadSkillFromDisc(s32 skillId);

#endif /* DCB_PLAYER_DATA_H */
