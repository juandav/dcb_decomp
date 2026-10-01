#include "common.h"
#include "game.h"
#include "dcb/sug_stage_fade.h"
#include "dcb/stage.h"
#include "dcb/sug_model_effect.h"
#include "dcb/sug_tex_anim.h"

extern ClutFade SUG_STAGE_CLUT;

void SUG_setStageBrightness(u8 level) {
    u8 r;
    u8 g;
    u8 b;

    SUG_STAGE_CLUT.level = level;
    if (SUG_STAGE_CLUT.level == 0xFF) {
        LoadImage((s16 *)&SUG_STAGE_CLUT.rect, (s32)SUG_STAGE_CLUT.clut);
        DrawSync(0);
    } else {
        SUG_uploadShadedClut(&SUG_STAGE_CLUT, 0);
    }
    r = STAGE_CLEAR_COLOR[0] * level / 255;
    g = STAGE_CLEAR_COLOR[1] * level / 255;
    b = STAGE_CLEAR_COLOR[2] * level / 255;
    DB(0).draw.r0 = DB(1).draw.r0 = r;
    DB(0).draw.g0 = DB(1).draw.g0 = g;
    DB(0).draw.b0 = DB(1).draw.b0 = b;
}

#define FADE_LEVEL (((u8 *)SCENE_3D)[0x130])

void SUG_runStageFadeTask(void) {
    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (FADE_LEVEL > FADE_TARGET) {
            if (FADE_LEVEL < 3) {
                FADE_LEVEL = 0;
            } else {
                FADE_LEVEL -= 3;
            }
            if (FADE_LEVEL < FADE_TARGET) {
                FADE_LEVEL = FADE_TARGET;
            }
        } else if (FADE_LEVEL < FADE_TARGET) {
            if (FADE_LEVEL >= 0xFD) {
                FADE_LEVEL = 0xFF;
            } else {
                FADE_LEVEL += 3;
            }
            if (FADE_LEVEL > FADE_TARGET) {
                FADE_LEVEL = FADE_TARGET;
            }
        } else {
            continue;
        }
        SUG_setStageBrightness(FADE_LEVEL);
    }
}

void SUG_saveStageClut(ModelData *model, s16 h) {
    Rect16 rect;

    rect = SUG_MODEL_CLUT_RECT;
    rect.h = h;
    rect.x += ((model->tpageOffset / 0x10000 + 5) & 0xF) << 6;
    rect.y += ((model->tpageOffset / 0x10000 + 5) >> 4) << 8;
    StoreImage2(&rect, (u32 *)SUG_STAGE_CLUT.clut);
    SUG_STAGE_CLUT.rect = rect;
    SUG_STAGE_CLUT.brighten = 0;
    FADE_LEVEL = FADE_TARGET = 0xFF;
}

void SUG_resetStageBrightness(void) {
    SUG_setStageBrightness(0xFF);
    DB(1).draw.r0 = 0;
    DB(0).draw.r0 = 0;
    DB(1).draw.g0 = 0;
    DB(0).draw.g0 = 0;
    DB(1).draw.b0 = 0;
    DB(0).draw.b0 = 0;
}

void SUG_shadeModelClut(s32 slot, s32 target) {
    ClutFade fade;
    ModelData *model;
    s32 i;

    model = SCENE_3D->models[slot];
    if (target != 0xFF) {
        fade.rect = model->clutRect;
        fade.brighten = 0;
        fade.level = target;
        for (i = 0; i < 256; i++) {
            fade.clut[i] = model->clut[i];
        }
        SUG_uploadShadedClut(&fade, 0x8000);
    } else {
        LoadImage((s16 *)&model->clutRect, (s32)model->clut);
        DrawSync(0);
    }
}
