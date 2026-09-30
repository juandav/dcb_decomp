#ifndef DCB_MODEL_ANIM_H
#define DCB_MODEL_ANIM_H

#include "game.h"

void applyRootMotion(Model *model);
void setupRotationCurve(AnimChan *chan, s32 length, s32 nextLength, s32 halfLength, s32 from, s32 to, s32 next);
void setupTranslationCurve(AnimChan *chan, s32 length, s32 nextLength, s32 halfLength, s32 from, s32 to, s32 next);
void accelerateModelBones(Model *model);
s32 updateModelBoneMatrices(Model *model);
s32 loadNextAnimationKeyframe(Model *model, s32 loopKey, s32 mode);
void runModelAnimationTask(void);

#endif /* DCB_MODEL_ANIM_H */
