#include "common.h"
#include "game.h"
#include "dcb/sug_light_motion.h"
#include "dcb/heap.h"
#include "dcb/scene3d.h"

/* the light matrix and colours SUG_tickLightMotion sets */
/* jp's two lights point the other way up, and their green is a little lower */
#if VERSION_JP
MATRIX SUG_DEFAULT_LIGHT_MATRIX = { { { 0, 0x1000, -0x5DC }, { 0, -0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } };
MATRIX SUG_DEFAULT_LIGHT_COLORS = { { { 0x1000, 0x5DC, 0 }, { 0xFF5, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } };
#elif VERSION_US || VERSION_EU
MATRIX SUG_DEFAULT_LIGHT_MATRIX = { { { 0, -0x1000, -0x5DC }, { 0, 0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } };
MATRIX SUG_DEFAULT_LIGHT_COLORS = { { { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } };
#else
#error "sugseg/effect/sug_light_motion: version not checked"
#endif

LightMotion *SUG_createLightMotion(VECTOR *pos, VECTOR *posTo, VECTOR *color, VECTOR *colorTo, s32 light, s32 period, s32 duration) {
    LightMotion *motion;

    motion = allocTaskHeapBlock(sizeof(LightMotion));
    /* jp lets a zero period divide by zero */
#if VERSION_US || VERSION_EU
    if (period == 0) {
        period = 1;
    }
#endif
    motion->pos = *pos;
    motion->posStep.vx = (posTo->vx - pos->vx) / period;
    motion->posStep.vy = (posTo->vy - pos->vy) / period;
    motion->posStep.vz = (posTo->vz - pos->vz) / period;
    motion->color = *color;
    motion->colorStep.vx = (colorTo->vx - color->vx) / period;
    motion->colorStep.vy = (colorTo->vy - color->vy) / period;
    motion->colorStep.vz = (colorTo->vz - color->vz) / period;
    motion->light = light;
    motion->duration = duration;
    motion->period = period;
    motion->frame = 0;
    return motion;
}

void SUG_tickLightMotion(LightMotion *motion) {
    s32 t;

    if (motion->frame < motion->duration) {
        t = motion->frame % motion->period;
        SCENE_LIGHT_MATRIX.m[motion->light][0] = motion->pos.vx + t * motion->posStep.vx;
        SCENE_LIGHT_MATRIX.m[motion->light][1] = motion->pos.vy + t * motion->posStep.vy;
        SCENE_LIGHT_MATRIX.m[motion->light][2] = motion->pos.vz + t * motion->posStep.vz;
        SCENE_LIGHT_COLORS.m[0][motion->light] = motion->color.vx + t * motion->colorStep.vx;
        SCENE_LIGHT_COLORS.m[1][motion->light] = motion->color.vy + t * motion->colorStep.vy;
        SCENE_LIGHT_COLORS.m[2][motion->light] = motion->color.vz + t * motion->colorStep.vz;
        if (++motion->frame == motion->duration) {
            SCENE_LIGHT_MATRIX = SUG_DEFAULT_LIGHT_MATRIX;
            SCENE_LIGHT_COLORS = SUG_DEFAULT_LIGHT_COLORS;
        }
    }
}

void SUG_freeLightMotion(void *obj) {
    SCENE_LIGHT_MATRIX = SUG_DEFAULT_LIGHT_MATRIX;
    SCENE_LIGHT_COLORS = SUG_DEFAULT_LIGHT_COLORS;
    freeHeapBlock(obj);
}
