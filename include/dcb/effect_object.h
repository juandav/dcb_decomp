#ifndef DCB_EFFECT_OBJECT_H
#define DCB_EFFECT_OBJECT_H

#include "game.h"
#include "dcb/scroll_bg.h"

s32 tickEffectMotion(s32 fxAddr, s32 applyFlag);
void updateEffectLinearMotion(void *fx);
void updateEffectArcMotion(void *fx);
void updateEffectWaveXMotion(void *fx);
void updateEffectWaveYMotion(void *fx);
void updateEffectTiltedArcMotion(u8 *fx);
void func_80030440(u8 *fx);
void updateEffectShakeMotion(u8 *fx);
s32 getVectorDistance(SVECTOR *from, SVECTOR *to);
s32 isPointAlongSegment(SVECTOR *start, SVECTOR *end, SVECTOR *point);
s32 isWithinDistance(SVECTOR *a, SVECTOR *b, s32 radius);
void projectPointOntoLine(SVECTOR *start, SVECTOR *point, SVECTOR *end, VECTOR *out);
s32 checkEffectHitTarget(SVECTOR *prevPos, SVECTOR *curPos, SVECTOR *target, s16 radius);
void *initEffectObject(void *fx);
EffectTemplate *cloneEffectObject(EffectTemplate *template);
void updateEffectObject(s32 fx);
void freeEffectObject(void *fx);
s32 getDirectionVector(SVECTOR *from, SVECTOR *to, VECTOR *dir);
void restartEffectMotion(u8 *fx);
void tickEffectStartDelay(void *fx);
s16 updateEffectBrightness(void *fxObj, s16 brightness);

#endif /* DCB_EFFECT_OBJECT_H */
