#include "common.h"
#include "game.h"
#include "dcb/sug_model_effect.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/effect_object.h"
#include "dcb/anim_control.h"
#include "dcb/scroll_bg.h"
#include "dcb/sug_tex_anim.h"

/* jp's model effects (sug_model_effect.c is us's and eu's): every color of
   the shaded CLUT is semi-transparent, the CLUT row comes from the model's id
   and the effect always animates every bone */

void SUG_uploadShadedClut(ClutFade *fade) {
    u16 *src;
    u16 *dst;
    s32 brighten;
    s32 level;
    s32 color;
    s32 r;
    s32 g;
    s32 b;
    s32 i;

    brighten = fade->brighten;
    level = fade->level;
    src = fade->clut;
    dst = fade->faded;
    for (i = 0; i < fade->rect.w * fade->rect.h; i++) {
        color = *src++;
        if (color != 0) {
            r = color & 0x1F;
            g = (color >> 5) & 0x1F;
            b = (color >> 10) & 0x1F;
            if (brighten == 0) {
                r = r * level / 255;
                g = g * level / 255;
                b = b * level / 255;
            } else {
                r += (31 - r) * level / 255;
                g += (31 - g) * level / 255;
                b += (31 - b) * level / 255;
            }
            if (r < 0) {
                r = 0;
            } else if (r >= 32) {
                r = 31;
            }
            if (g < 0) {
                g = 0;
            } else if (g >= 32) {
                g = 31;
            }
            if (b < 0) {
                b = 0;
            } else if (b >= 32) {
                b = 31;
            }
            color = 0x8000 | (b << 10) | (g << 5) | r;
        }
        *dst++ = color;
    }
    LoadImage((s16 *)&fade->rect, (s32)fade->faded);
}

void *SUG_createModelEffect(s16 brightness, EffectTemplate *template, s32 modelId, s32 anim, s32 texAnimId, s32 vramSlot, u8 flags, s32 pak) {
    ModelEffect *fx;
    Rect16 rect;
    s32 slot;
    s32 id;

    fx = allocTaskHeapBlock(sizeof(ModelEffect));
    for (slot = 2; slot < 23 && SCENE_3D->modelState[slot] != 0; slot++) {
    }
    if (slot >= 23 || !loadModel(slot, modelId, vramSlot, pak)) {
        freeHeapBlock(fx);
        return NULL;
    }
    fx->modelSlot = slot;
    fx->model = SCENE_3D->models[slot];
    if (anim >= 0 && loadModelAnimation(slot, anim, 0, pak)) {
        applyAnimationFirstFrame(slot, 0);
        startModelAnimation(slot, 0, -2, 1);
    }
    if (template != NULL) {
        *(EffectTemplate *)fx = *template;
        initEffectObject(fx);
        SCENE_3D->modelState[fx->modelSlot] = -1;
        fx->active = 1;
    } else {
        SCENE_3D->modelState[fx->modelSlot] = 1;
        fx->active = 0;
    }
    fx->model->owner = fx;
    if (texAnimId >= 0 && SUG_startTexAnim(texAnimId, 0, (RingEffect *)fx->model, &fx->texAnim, pak)) {
        fx->texAnimActive = 1;
    } else {
        fx->texAnimActive = -1;
    }
    rect = SUG_MODEL_CLUT_RECT;
    id = (u16)fx->model->id;
    if (id >= 2000 && id < 9990 && !(id >= 4000 && id < 5000)) {
        rect.y = 0xF0;
    }
    rect.x += ((fx->model->tpageOffset / 0x10000 + 5) & 0xF) << 6;
    rect.y += ((fx->model->tpageOffset / 0x10000 + 5) >> 4) << 8;
    StoreImage2(&rect, (u32 *)fx->fade.clut);
    fx->fade.rect = rect;
    fx->fade.brighten = 0;
    fx->fade.level = fx->lastBrightness = brightness;
    if (brightness != 0xFF) {
        SUG_uploadShadedClut(&fx->fade);
    }
    fx->flags = flags;
    return fx;
}

void SUG_tickModelEffect(ModelEffect *obj) {
    if (obj->active != 0) {
        if (obj->suspended != 0) {
            SCENE_3D->modelState[obj->modelSlot] = -1;
            return;
        }
        obj->fade.level = updateEffectBrightness(obj, obj->fade.level);
        if (obj->fade.level != obj->lastBrightness) {
            obj->lastBrightness = obj->fade.level;
            SUG_uploadShadedClut(&obj->fade);
        }
        if (obj->fade.level == 0) {
            SCENE_3D->modelState[obj->modelSlot] = -1;
            return;
        }
        SCENE_3D->modelState[obj->modelSlot] = 3;
    }
    if (obj->texAnimActive >= 0) {
        SUG_tickTexAnim(&obj->texAnim);
    }
}

void SUG_freeModelEffect(ModelEffect *obj) {
    unloadModel(obj->modelSlot);
    if (obj->fade.level != 0xFF) {
        obj->fade.level = 0xFF;
        SUG_uploadShadedClut(&obj->fade);
    }
    freeHeapBlock(obj);
}
