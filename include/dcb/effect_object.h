#ifndef DCB_EFFECT_OBJECT_H
#define DCB_EFFECT_OBJECT_H

#include "game.h"
#include "dcb/scroll_bg.h"

s32 tickEffectMotion(s32 fxAddr, s32 applyFlag);
void updateEffectLinearMotion(EffectObject *fx);
void updateEffectArcMotion(EffectObject *fx);
void updateEffectWaveXMotion(EffectObject *fx);
void updateEffectWaveYMotion(EffectObject *fx);
void updateEffectTiltedArcMotion(EffectObject *fx);
void updateEffectTiltedAccelArcMotion(EffectObject *fx);
void updateEffectShakeMotion(EffectObject *fx);
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
void restartEffectMotion(void *fx);
void tickEffectStartDelay(void *fx);
s16 updateEffectBrightness(void *fxObj, s16 brightness);

/* A suspended effect waits out its start delay in us and eu; jp's effects
   have none, they only skip the frame */
#if VERSION_JP
#define TICK_START_DELAY(fx)
#elif VERSION_US || VERSION_EU
#define TICK_START_DELAY(fx) tickEffectStartDelay(fx)
#endif

#endif /* DCB_EFFECT_OBJECT_H */
