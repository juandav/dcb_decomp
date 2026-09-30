#ifndef DCB_MODEL_ANIM_H
#define DCB_MODEL_ANIM_H

#include "game.h"

void applyRootMotion(u8 *model);
void setupRotationCurve(s32 *chan, s32 span, s32 nextSpan, s32 halfSpan, s32 key, s32 nextKey, s32 afterKey);
void setupTranslationCurve(s32 *chan, s32 span, s32 nextSpan, s32 halfSpan, s32 key, s32 nextKey, s32 afterKey);
void accelerateModelBones(u8 *model);
s32 updateModelBoneMatrices(void *model);
s32 loadNextAnimationKeyframe(Model2220 *, s32, s32);
void runModelAnimationTask(void);

#endif /* DCB_MODEL_ANIM_H */
