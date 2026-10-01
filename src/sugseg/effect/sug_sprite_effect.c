#include "common.h"
#include "game.h"
#include "dcb/sug_sprite_effect.h"
#include "dcb/heap.h"
#include "dcb/effect_object.h"
#include "dcb/scroll_bg.h"
#include "dcb/sug_sprite.h"

#if VERSION_JP
void *SUG_createSpriteEffect(s32 brightness, EffectTemplate *template, s32 key, s32 otz, s32 subKey) {
    SpriteEffect *fx;

    fx = allocTaskHeapBlock(sizeof(SpriteEffect));
    SUG_initSprite(&fx->sprite, key, 4, 4, 0, 0, 0, otz, subKey);
#elif VERSION_US || VERSION_EU
void *SUG_createSpriteEffect(s32 brightness, EffectTemplate *template, s32 key, s32 flipX, s32 flipY, s32 a5, s32 useOrigin, s32 otz, s32 subKey) {
    SpriteEffect *fx;

    fx = allocTaskHeapBlock(sizeof(SpriteEffect));
    fx->sprite.flipX = flipX;
    fx->sprite.flipY = flipY;
    fx->sprite.unk8A = a5 == 0;
    SUG_initSprite(&fx->sprite, key, 4, 4, 0, 0, 0, otz, useOrigin, subKey);
#else
#error "sugseg/effect/sug_sprite_effect: version not checked"
#endif
    fx->brightness = brightness;
    if (template != NULL) {
        *(EffectTemplate *)fx = *template;
        initEffectObject(fx);
        fx->active = 1;
    } else {
        fx->active = 0;
        fx->brightness = 0xFF;
    }
    return fx;
}

void SUG_tickSpriteEffect(SpriteEffect *fx) {
    PushMatrix();
    if (fx->active != 0) {
        if (fx->suspended != 0 || (fx->brightness = updateEffectBrightness(fx, fx->brightness)) == 0) {
            PopMatrix();
            TICK_START_DELAY(fx);
            return;
        }
#if VERSION_JP
        tickEffectMotion((s32)fx, 1);
#elif VERSION_US || VERSION_EU
        tickEffectMotion((s32)fx, fx->sprite.unk8A);
#endif
    } else {
        SetRotMatrix((s32)&GsIDMATRIX);
        SetTransMatrix(&GsIDMATRIX);
    }
    SUG_drawSprite(&fx->sprite, fx->brightness);
    PopMatrix();
}

void SUG_freeSpriteEffect(void *ptr) {
    freeHeapBlock(ptr);
}
