#ifndef DCB_EVO_FUSION_RESULT_H
#define DCB_EVO_FUSION_RESULT_H

#include "game.h"
#include "dcb/evoseg.h"

void EVO_startSecondCardPick(void);
void EVO_resetFusion(void);
void EVO_cancelFirstCard(void);
void EVO_uploadShadedClut(EvoClut *clut, u16 flags);
s16 EVO_findFusionResult(void);

#endif /* DCB_EVO_FUSION_RESULT_H */
