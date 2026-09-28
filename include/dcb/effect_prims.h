#ifndef DCB_EFFECT_PRIMS_H
#define DCB_EFFECT_PRIMS_H

#include "game.h"
#include "dcb/effect_object.h"

void buildRingEffectMesh(Obj32 *ring);
void renderRingEffect(Obj32 *ring);
void freeRingEffect(Obj32 *ring);
void renderStreakParticles(Particles *fx);
void freeStreakParticles(void *fx);

#endif /* DCB_EFFECT_PRIMS_H */
