#ifndef DCB_SUG_MODEL_EFFECT_H
#define DCB_SUG_MODEL_EFFECT_H

#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/sugseg.h"

#if VERSION_JP
void *SUG_createModelEffect(s16 brightness, EffectTemplate *template, s32 modelId, s32 anim, s32 texAnimId, s32 vramSlot, u8 flags, s32 pak);
void SUG_uploadShadedClut(ClutFade *fade);
#elif VERSION_US || VERSION_EU
void *SUG_createModelEffect(s16 brightness, EffectTemplate *template, s32 modelId, s32 anim, s32 texAnimId, s32 vramSlot, u8 flags, s32 allBones, s32 pak, s32 clutBank);
void SUG_uploadShadedClut(ClutFade *fade, u16 stp);
#endif
void SUG_tickModelEffect(ModelEffect *obj);
void SUG_freeModelEffect(ModelEffect *obj);

#endif /* DCB_SUG_MODEL_EFFECT_H */
