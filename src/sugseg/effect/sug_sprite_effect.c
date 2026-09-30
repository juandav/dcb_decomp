#include "common.h"
#include "game.h"
#include "dcb/sug_sprite_effect.h"
#include "dcb/heap.h"
#include "dcb/effect_object.h"
#include "dcb/scroll_bg.h"
#include "dcb/sug_sprite.h"

void *SUG_createSpriteEffect(s32 brightness, EffectTemplate *template, s32 key, s32 flipX, s32 flipY, s32 a5, s32 useOrigin, s32 otz, s32 subKey) {
    SpriteEffect *fx;

    fx = allocTaskHeapBlock(sizeof(SpriteEffect));
    fx->sprite.flipX = flipX;
    fx->sprite.flipY = flipY;
    fx->sprite.unk8A = a5 == 0;
    SUG_initSprite(&fx->sprite, key, 4, 4, 0, 0, 0, otz, useOrigin, subKey);
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
            tickEffectStartDelay(fx);
            return;
        }
        tickEffectMotion((s32)fx, fx->sprite.unk8A);
    } else {
        SetRotMatrix((s32)&D_801DBEC0);
        func_8005C444(&D_801DBEC0);
    }
    SUG_drawSprite(&fx->sprite, fx->brightness);
    PopMatrix();
}

void SUG_freeSpriteEffect(void *ptr) {
    freeHeapBlock(ptr);
}
