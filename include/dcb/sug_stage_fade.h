#ifndef DCB_SUG_STAGE_FADE_H
#define DCB_SUG_STAGE_FADE_H

#include "game.h"
#include "dcb/sugseg.h"

void SUG_resetStageBrightness(void);
void SUG_runStageFadeTask(void);
void SUG_saveStageClut(ModelData *model, s16 h);
void SUG_shadeModelClut(s32 slot, s32 target);

#endif /* DCB_SUG_STAGE_FADE_H */
