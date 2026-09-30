#ifndef DCB_EFFECT_PRIMS_H
#define DCB_EFFECT_PRIMS_H

#include "game.h"
#include "dcb/effect_object.h"

void buildRingEffectMesh(RingEffect *ring);
void renderRingEffect(RingEffect *ring);
void freeRingEffect(RingEffect *ring);
void renderStreakParticles(StreakParticles *fx);
void freeStreakParticles(StreakParticles *fx);

#endif /* DCB_EFFECT_PRIMS_H */
