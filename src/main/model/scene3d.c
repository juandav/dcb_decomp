#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scene3d.h"
#include "dcb/effect_object.h"
#include "dcb/duel_util.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/model.h"
#include "dcb/model_load.h"
#include "dcb/stage.h"
#include "dcb/game_flow.h"
#include "dcb/scroll_bg.h"
#include "dcb/tmd_sort.h"
#include "dcb/frame_callback.h"
#include "dcb/window.h"

s32 D_8006DF80 = 0xFF;
s32 GRID_VISIBLE = 1;
u8 GRID_PULSE_PHASE = 0;
u8 D_8006DF8C[12] = { 0 };

void renderSceneModels(Unk800793A0 *buffer, s32 bufferIndex) {
    MATRIX localScreen;
    SVECTOR position;
    MATRIX lightMatrix;
    MATRIX colorMatrix;
    MATRIX unused;
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
        packet = (u32)buffer->unk4070;
        colorMatrix = SCENE_LIGHT_COLORS;
        for (i = 0; i < 24; i++) {
            model = SCENE_3D->unk13C[i];
            if (SCENE_3D->unk114[i] <= 0 || model->unk26D4 < 0) {
                continue;
            }
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
                animateStageTexture((u8 *)model);
            }
            if (SCENE_3D->unk114[i] == 1) {
                RotMatrix(&SCENE_WORLD_ROTATION, &SCENE_3D->root.coord);
                SCENE_3D->root.flg = 0;
                SCENE_3D->root.coord.t[0] = localScreen.t[0];
                SCENE_3D->root.coord.t[1] = localScreen.t[1];
                SCENE_3D->root.coord.t[2] = localScreen.t[2];
                RotMatrixYXZ(&model->rot, &model->root.coord);
                model->root.flg = 0;
                ScaleMatrix(&model->root.coord, &model->scale);
            } else if (SCENE_3D->unk114[i] == 2) {
                PopMatrix();
                continue;
            } else {
                link = (ModelLink *)model->unk26E0;
                memset(&rot, 0, 8);
                if (link->unk571 != 0) {
                    tickEffectMotion((s32)link, link->unk56F);
                    model->root.coord = link->m;
                    link->model->rot = link->rot;
                    link->model->pos = link->pos;
                }
                RotMatrix(&rot, &SCENE_3D->root.coord);
                SCENE_3D->root.flg = 0;
                SCENE_3D->root.coord.t[2] = 0;
                SCENE_3D->root.coord.t[1] = 0;
                SCENE_3D->root.coord.t[0] = 0;
                SCENE_3D->unk114[i] = 2;
            }
            scratch = (s32 *)0x1F800000;
            scratch[12] = model->unk26D4;
            scratch[13] = model->unk26D0;
            obj = model->obj;
            for (j = 0; j < model->nobj; j++, obj++) {
                obj->coord2->flg = 0;
                if (obj->id != -1 && obj->tmd != NULL) {
                    GsGetLws(obj->coord2, &model->lw[j], &localScreen);
                    if (obj->attribute == 0) {
                        gte_SetLightMatrix(&lightMatrix);
                        gte_SetRotMatrix(&localScreen);
                        gte_SetTransMatrix(&localScreen);
                        if (obj->tmd[0] != 0) {
                            packet = sortEnvMappedModelObject((u32 *)obj->tmd[5], ot + 1, packet, (void *)otDepth);
                        } else {
                            packet = sortModelObject((u32 *)obj->tmd[5], ot + 1, packet, (void *)otDepth);
                        }
                    }
                }
            }
            PopMatrix();
        }
    }
}

void setupSceneProjection(s32 projection) {
    func_8005C484(0xA0, 0x78);
    func_8005C4A4(projection);
    func_80062484(projection);
    SCENE_3D->unkC4.vpx = 0;
    SCENE_3D->unkC4.vpy = 0;
    SCENE_3D->unkC4.vpz = 0;
    SCENE_3D->unkC4.vrx = 0;
    SCENE_3D->unkC4.vry = 0;
    SCENE_3D->unkC4.vrz = 0;
    SCENE_3D->unkC4.rz = 0;
    SCENE_3D->unkC4.super = 0;
    GsSetRefView2(&SCENE_3D->unkC4);
}

void setupSceneLighting(void) {
    GsSetAmbient(0x40, 0x40, 0x40);
    func_8005C464(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

void initScene3D(s32 allocBuffers) {
    s32 i;

    initModelScene();
    func_8006A804();
    for (i = 0; i < 2; i++) {
        if (allocBuffers) {
            DB(i).unk4070 = allocHeapBlock(0xBB80, 0x7F);
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
    func_8006A814();
    {
        MATRIX lightMatrices[2] = {
            { { { 0, 0x1800, -0x1800 }, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
            { { { 0, -0x1000, -0x5DC }, { 0, 0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
        };
        MATRIX colorMatrices[2] = {
            { { { 0x800, 0, 0 }, { 0x800, 0, 0 }, { 0x800, 0, 0 } }, { 0, 0, 0 } },
            { { { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
        };

        SCENE_LIGHT_MATRIX = lightMatrices[1];
        SCENE_LIGHT_COLORS = colorMatrices[1];
    }
    if (allocBuffers) {
        mountDriveTask((s32) "M:", getCurrentTaskId());
        addFrameCallback((s32)renderSceneModels);
    }
}
