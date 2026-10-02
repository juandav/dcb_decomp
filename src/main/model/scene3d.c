#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scene3d.h"
#include "dcb/effect_object.h"
#include "dcb/effect_prims.h"
#include "dcb/duel_util.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/anim_control.h"
#include "dcb/model_load.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/tmd_sort.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

#if VERSION_US || VERSION_EU
s32 STAGE_FADE_LEVEL = 0xFF;
#elif VERSION_JP
/* jp's stages have no fade level */
#endif

void renderSceneModels(FrameBuffer *buffer, s32 bufferIndex) {
    MATRIX localScreen;
    SVECTOR position;
    MATRIX lightMatrix;
    MATRIX colorMatrix;
    MATRIX unused; /* unused, but it is in the original stack frame */
    SVECTOR rot;
    s32 flag;
    u32 *ot;
    u32 otDepth;
    u32 packet;
    Model *model;
    GsDOBJ4 *obj;
    ModelLink *link;
    s32 i;
    s32 j;
    s32 *scratch;

    ot = SCENE_3D->ot[bufferIndex].org;
    otDepth = 0xFFF;
    if (SCENE_3D_ENABLED != 0) {
        packet = (u32)buffer->scenePackets;
        colorMatrix = SCENE_LIGHT_COLORS;
        for (i = 0; i < 24; i++) {
#if VERSION_JP
            /* jp draws a model without checking its texture page. */
            if (SCENE_3D->modelState[i] <= 0) {
                continue;
            }
            model = SCENE_3D->models[i];
#elif VERSION_US || VERSION_EU
            model = SCENE_3D->models[i];
            if (SCENE_3D->modelState[i] <= 0 || model->tpageOffset < 0) {
                continue;
            }
#endif
            lightMatrix = SCENE_LIGHT_MATRIX;
            gte_SetColorMatrix(&colorMatrix);
            PushMatrix();
            if (i != 0x17) {
                position.vx = model->pos.vx;
                position.vy = model->pos.vy;
                position.vz = model->pos.vz;
                gte_ldv0(&position);
                gte_rtv0tr();
                gte_stlvnl(localScreen.t);
                gte_stflg(&flag);
            } else {
                localScreen.t[0] = localScreen.t[1] = localScreen.t[2] = 0;
                otDepth >>= 2;
                ot += otDepth * 3;
                animateStageTexture(model);
            }
            if (SCENE_3D->modelState[i] == 1) {
                RotMatrix(&SCENE_WORLD_ROTATION, &SCENE_3D->root.coord);
                SCENE_3D->root.flg = 0;
                SCENE_3D->root.coord.t[0] = localScreen.t[0];
                SCENE_3D->root.coord.t[1] = localScreen.t[1];
                SCENE_3D->root.coord.t[2] = localScreen.t[2];
                RotMatrixYXZ(&model->rot, &model->root.coord);
                model->root.flg = 0;
                ScaleMatrix(&model->root.coord, &model->scale);
            } else if (SCENE_3D->modelState[i] == 2) {
                PopMatrix();
                continue;
            } else {
                link = (ModelLink *)model->link;
                memset(&rot, 0, 8);
                if (link->enabled != 0) {
                    tickEffectMotion((s32)link, link->axisMode);
                    model->root.coord = link->m;
                    link->model->rot = link->rot;
                    link->model->pos = link->pos;
                }
                RotMatrix(&rot, &SCENE_3D->root.coord);
                SCENE_3D->root.flg = 0;
                SCENE_3D->root.coord.t[2] = 0;
                SCENE_3D->root.coord.t[1] = 0;
                SCENE_3D->root.coord.t[0] = 0;
                SCENE_3D->modelState[i] = 2;
            }
            scratch = (s32 *)0x1F800000;
            scratch[12] = model->tpageOffset;
            scratch[13] = model->clutOffset;
            obj = model->obj;
            for (j = 0; j < model->nobj; j++, obj++) {
                obj->coord2->flg = 0;
#if VERSION_JP
                /* jp only computes the matrices of the objects it draws. */
                if (obj->id == -1 || obj->tmd == NULL || obj->attribute != 0) {
                    continue;
                }
                GsGetLws(obj->coord2, &model->lw[j], &localScreen);
#elif VERSION_US || VERSION_EU
                if (obj->id == -1 || obj->tmd == NULL) {
                    continue;
                }
                GsGetLws(obj->coord2, &model->lw[j], &localScreen);
                if (obj->attribute != 0) {
                    continue;
                }
#endif
                gte_SetLightMatrix(&lightMatrix);
                gte_SetRotMatrix(&localScreen);
                gte_SetTransMatrix(&localScreen);
                if (obj->tmd[0] != 0) {
                    packet = sortEnvMappedModelObject((u32 *)obj->tmd[5], ot + 1, packet, (void *)otDepth);
                } else {
                    packet = sortModelObject((u32 *)obj->tmd[5], ot + 1, packet, (void *)otDepth);
                }
            }
            PopMatrix();
        }
    }
}

void setupSceneProjection(s32 projection) {
    SetGeomOffset(0xA0, 0x78);
    SetGeomScreen(projection);
    GsSetProjection(projection);
    SCENE_3D->view.vpx = 0;
    SCENE_3D->view.vpy = 0;
    SCENE_3D->view.vpz = 0;
    SCENE_3D->view.vrx = 0;
    SCENE_3D->view.vry = 0;
    SCENE_3D->view.vrz = 0;
    SCENE_3D->view.rz = 0;
    SCENE_3D->view.super = 0;
    GsSetRefView2(&SCENE_3D->view);
}

void setupSceneLighting(void) {
    GsSetAmbient(0x40, 0x40, 0x40);
    SetBackColor(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

void initScene3D(s32 allocBuffers) {
    s32 i;

    initModelScene();
    EnterCriticalSection();
    for (i = 0; i < 2; i++) {
        if (allocBuffers) {
            DB(i).scenePackets = allocHeapBlock(0xBB80, 0x7F);
        }
        SCENE_3D->ot[i].length = 12;
        SCENE_3D->ot[i].org = DB(i).ot;
        SCENE_3D->ot[i].offset = 0;
        SCENE_3D->ot[i].point = 0;
        SCENE_3D->ot[i].tag = SCENE_3D->ot[i].org + 0xFFF;
    }
    GsInit3D();
    setupSceneProjection(0x1C0);
    setupSceneLighting();
    ExitCriticalSection();
    {
        MATRIX lightMatrices[2] = {
            { { { 0, 0x1800, -0x1800 }, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
#if VERSION_JP
            /* jp's two lights come from the other sides */
            { { { 0, 0x1000, -0x5DC }, { 0, -0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
#elif VERSION_US || VERSION_EU
            { { { 0, -0x1000, -0x5DC }, { 0, 0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
#endif
        };
        MATRIX colorMatrices[2] = {
            { { { 0x800, 0, 0 }, { 0x800, 0, 0 }, { 0x800, 0, 0 } }, { 0, 0, 0 } },
#if VERSION_JP
            /* and the first light has a little less green */
            { { { 0x1000, 0x5DC, 0 }, { 0xFF5, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
#elif VERSION_US || VERSION_EU
            { { { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
#endif
        };

        SCENE_LIGHT_MATRIX = lightMatrices[1];
        SCENE_LIGHT_COLORS = colorMatrices[1];
    }
    if (allocBuffers) {
        mountDriveTask((s32) "M:", getCurrentTaskId());
        addFrameCallback((s32)renderSceneModels);
    }
}
