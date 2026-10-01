#ifndef DCB_SUG_SPRITE_EFFECT_H
#define DCB_SUG_SPRITE_EFFECT_H

#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/sugseg.h"

#if VERSION_JP
void *SUG_createSpriteEffect(s32 brightness, EffectTemplate *template, s32 key, s32 otz, s32 subKey);
#elif VERSION_US || VERSION_EU
void *SUG_createSpriteEffect(s32 brightness, EffectTemplate *template, s32 key, s32 flipX, s32 flipY, s32 a5, s32 useOrigin, s32 otz, s32 subKey);
#endif
void SUG_tickSpriteEffect(SpriteEffect *fx);
void SUG_freeSpriteEffect(void *ptr);

#endif /* DCB_SUG_SPRITE_EFFECT_H */
