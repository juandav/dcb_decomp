#ifndef DCB_SUG_MODEL_EFFECT_H
#define DCB_SUG_MODEL_EFFECT_H

#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/sugseg.h"

void *SUG_createModelEffect(s16 brightness, EffectTemplate *template, s32 modelId, s32 anim, s32 texAnimId, s32 vramSlot, u8 a6, s32 allBones, s32 pak, s32 a9);
void SUG_uploadShadedClut(ClutFade *fade, u16 stp);
void SUG_tickModelEffect(ModelEffect *obj);
void SUG_freeModelEffect(ModelEffect *obj);

#endif /* DCB_SUG_MODEL_EFFECT_H */
