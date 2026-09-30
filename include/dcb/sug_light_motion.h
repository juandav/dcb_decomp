#ifndef DCB_SUG_LIGHT_MOTION_H
#define DCB_SUG_LIGHT_MOTION_H

#include "game.h"
#include "dcb/sugseg.h"

LightMotion *SUG_createLightMotion(VECTOR *pos, VECTOR *posTo, VECTOR *color, VECTOR *colorTo, s32 light, s32 period, s32 duration);
void SUG_tickLightMotion(LightMotion *motion);
void SUG_freeLightMotion(void *obj);

#endif /* DCB_SUG_LIGHT_MOTION_H */
