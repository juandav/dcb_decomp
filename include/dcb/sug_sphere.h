#ifndef DCB_SUG_SPHERE_H
#define DCB_SUG_SPHERE_H

#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/sugseg.h"

SphereEffect *SUG_createSphereEffect(s16 brightness, u8 *color, s16 pulse, s16 pulseMode, EffectTemplate *template, s16 segments, s16 slices, s32 radius, u8 semiTrans, u8 abr, u8 primKind, u8 openBottom, s16 texAnimId, Rect16 *uv, s32 tpage, s32 clut, u8 cull, s32 otz, s32 pak);
void SUG_tickSphereEffect(SphereEffect *fx);
void SUG_freeSphereEffect(SphereEffect *obj);

#endif /* DCB_SUG_SPHERE_H */
