#include "dcb/camera.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
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

s32 stepCameraTowardTarget(u8 *camera, s32 *pos, s32 distance, s16 *target) {
    s32 speed;
    s32 steps;
    s32 delta;
    s32 step;

    if (CAMERA_SNAP != 0) {
        *(s32 *)(camera + 0x5C) = -pos[0];
        *(s32 *)(camera + 0x60) = -pos[1];
        *(s32 *)(camera + 0x64) = -pos[2];
        *(s32 *)(camera + 0x68) = target[2] << 12;
        *(s32 *)(camera + 0x6C) = target[3] << 12;
        *(s32 *)(camera + 0x70) = target[1] << 12;
        return target[4];
    }
    speed = *(s32 *)(camera + 0x50);
    if (speed == 0) {
        speed = 1;
    }
    steps = 30 / speed;
    if (steps == 0) {
        steps = 1;
    }
    delta = (target[3] << 12) - *(s32 *)(camera + 0x6C);
    if (delta != 0) {
        step = delta / steps;
        if (step == 0) {
            step = delta;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(camera + 0x6C) += step;
    }
    delta = (target[2] << 12) - *(s32 *)(camera + 0x68);
    if (delta != 0) {
        step = delta / steps;
        if (step == 0) {
            step = delta;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(camera + 0x68) += step;
    }
    delta = (target[1] << 12) - *(s32 *)(camera + 0x70);
    if (delta != 0) {
        step = delta / steps;
        if (step == 0) {
            step = delta > 0 ? 1 : -1;
        } else if (step > 0x10000) {
            step = 0x8000;
        } else if (step < -0x10000) {
            step = -0x8000;
        }
        *(s32 *)(camera + 0x70) += step;
    }
    delta = target[4] - distance;
    if (delta != 0) {
        step = delta / steps;
        if (step == 0) {
            step = delta > 0 ? 1 : -1;
        }
        distance += step;
    }
    *(s32 *)(camera + 0x5C) = -pos[0];
    *(s32 *)(camera + 0x60) = -pos[1];
    *(s32 *)(camera + 0x64) = -pos[2];
    return distance;
}

/* A camera preset: the model to follow and where the camera sits around it */
typedef struct {
    /* 0x0 */ s16 model; /* index into SCENE_3D->models, or -1 for a free camera */
    /* 0x2 */ s16 pitch;
    /* 0x4 */ s16 distance;
    /* 0x6 */ s16 height;
    /* 0x8 */ s16 yaw;
    /* 0xA */ s16 facedModel; /* the model that a big (id >= 2000) model faces */
} CameraPreset;

extern VECTOR CAMERA_LOOK_AT;
/* The presets runSceneCameraTask(preset) starts from (preset 0 keeps the camera as it is) */
const CameraPreset CAMERA_PRESETS[] = {
    { -1, 1024, 2560, 336, 0, 0 },
    { -1, 400, 512, 0, 0, 0 },
    { -1, 400, 3000, 0, 0, 0 },
    { -1, 400, 512, 0, 0, 0 },
};

/*
 * Task that places the 3D camera every frame: either eased toward the current
 * preset (free camera), or attached to the root bone of the followed model.
 * The GTE work area lives in the scratchpad.
 */
void runSceneCameraTask(s32 preset) {
    s32 yaw;
    s32 depth;
    s32 facing;
    s32 sign;
    Graphics *camera;
    Model *model;
    BoneKeys *root; /* the root bone's channels: angles 12.20, translation 16.16 */
    VECTOR *scale;
    VECTOR *viewTrans;
    MATRIX *view;
    SVECTOR *offset;
    SVECTOR *angles;
    s32 *gteFlag;
    Scene3D *scene;
    Graphics *graphics;
    CameraPreset *target;
    VECTOR *lookAt;
    s32 *lookAtCoords;
#if VERSION_JP
    s32 y;
#endif

    yaw = 0;
    /* GRAPHICS.targetModel starts the current CameraPreset */
    target = (CameraPreset *)&((Graphics *)&GRAPHICS)->targetModel;
    scene = SCENE_3D;
    scene->unkC0 = 0;
    scene->unk98.vx = scene->unk98.vy = scene->unk98.vz = 0;
    scene->unkA8.vx = scene->unkA8.vy = scene->unkA8.vz = 0;
    scene->unkB0.vx = scene->unkB0.vy = scene->unkB0.vz = 0;
    camera = (Graphics *)&GRAPHICS;
    lookAt = &CAMERA_LOOK_AT;
    if (preset != 0) {
        lookAt->vx = lookAt->vy = lookAt->vz = 0;
        camera->posX = camera->posY = camera->posZ = 0;
        *target = CAMERA_PRESETS[preset];
        camera->height = target->height << 12;
        camera->distance = target->distance << 12;
        camera->pitch = target->pitch << 12;
        camera->originX = 0;
        camera->originY = 0;
        camera->originZ = 0;
    }
    target->model = -1;
    target->facedModel = 0;
    lookAtCoords = (s32 *)&CAMERA_LOOK_AT;
    while (1) {
        /* GTE work area in the scratchpad */
        viewTrans = (VECTOR *)0x1F800004;
        view = (MATRIX *)0x1F800014;
        offset = (SVECTOR *)0x1F80003C;
        angles = (SVECTOR *)0x1F800034;
        gteFlag = (s32 *)0x1F800000;
        view->t[0] = view->t[1] = view->t[2] = 0;
        gte_SetTransMatrix(view);
        if (target->model < 0 || SCENE_3D->modelState[target->model] <= 0) {
            /* Free camera: ease toward the preset and orbit the look-at point */
            graphics = (Graphics *)&GRAPHICS;
            *lookAt = *(VECTOR *)&graphics->posX;
            yaw = stepCameraTowardTarget((u8 *)camera, lookAtCoords, yaw, (s16 *)target);
#if VERSION_JP
            /* jp also turns the camera's height and distance by the camera angles
               and keeps the x it gets, less the look-at y, in unk5A */
            {
                SVECTOR rot;

                y = -camera->originY;
                rot.vx = camera->rotY + yaw;
                rot.vy = camera->pitch / 4096;
                rot.vz = camera->rotZ;
                angles->vx = camera->height / 4096;
                angles->vy = 0;
                angles->vz = camera->distance / 4096;
                RotMatrix(&rot, view);
                gte_SetRotMatrix(view);
                gte_ldv0(angles);
                gte_rtv0tr();
                gte_stlvnl(viewTrans);
                gte_stflg(gteFlag);
                camera->unk5A = viewTrans->vx - y;
                SCENE_WORLD_ROTATION.vx = angles->vx = camera->rotX = rot.vy;
            }
#elif VERSION_US || VERSION_EU
            SCENE_WORLD_ROTATION.vx = angles->vx = camera->rotX = camera->pitch / 4096;
#endif
            SCENE_WORLD_ROTATION.vy = angles->vy = camera->rotY + yaw;
            SCENE_WORLD_ROTATION.vz = angles->vz = camera->rotZ;
            RotMatrix(angles, view);
            gte_SetRotMatrix(view);
            offset->vx = camera->originX;
            offset->vy = camera->originY;
            offset->vz = camera->originZ;
            /* Rotate the origin offset into view space */
            gte_ldv0(offset);
            gte_rtv0tr();
            gte_stlvnl(viewTrans);
            gte_stflg(gteFlag);
#if VERSION_JP
            viewTrans->vz += camera->distance / 4096;
            y = camera->height / 4096;
            viewTrans->vy += y;
#elif VERSION_US || VERSION_EU
            viewTrans->vz += camera->distance / 4096;
            viewTrans->vy += camera->height / 4096;
#endif
            TransMatrix(view, viewTrans);
            gte_SetTransMatrix(view);
        } else {
            /* Follow the root bone of the target model */
            model = SCENE_3D->models[target->model];
            root = &model->keys[model->nobj];
            scale = &model->scale;
            if (model->id < 2000) {
                facing = model->rot.vy;
                /* A model turned around (rot.vy != 0) mirrors the offset */
                sign = facing == 0 ? 1 : -1;
                depth = model->pos.vz;
            } else {
                /* A big model: look from the side of the model it faces */
                facing = ((Model *)SCENE_3D->models[target->facedModel])->rot.vy;
                sign = facing == 0 ? 1 : -1;
                depth = ((Model *)SCENE_3D->models[target->facedModel])->pos.vz + sign * model->pos.vz;
            }
            SCENE_WORLD_ROTATION.vx = -root->rot[0].value / 0x100000;
            SCENE_WORLD_ROTATION.vy = -root->rot[1].value / 0x100000 + facing;
            SCENE_WORLD_ROTATION.vz = -root->rot[2].value / 0x100000;
            RotMatrix(&SCENE_WORLD_ROTATION, view);
            gte_SetRotMatrix(view);
            /* Offset by the root bone's translation, scaled by the model scale (4096 = 1.0) */
            offset->vx = (float)-(s16)(root->pos[0].value >> 16) * ((float)scale->vx * (1.0f / 4096)) * (float)sign;
            offset->vy = (float)-(s16)(root->pos[1].value >> 16) * ((float)scale->vy * (1.0f / 4096)) - (float)model->pos.vy;
            offset->vz = (float)-(s16)(root->pos[2].value >> 16) * ((float)scale->vz * (1.0f / 4096)) * (float)sign - (float)depth;
            gte_ldv0(offset);
            gte_rtv0tr();
            gte_stlvnl(viewTrans);
            gte_stflg(gteFlag);
            TransMatrix(view, viewTrans);
            gte_SetTransMatrix(view);
        }
        /* Publish the view matrix to the scene */
        *(MATRIX *)SCENE_3D->viewMatrix = *view;
        waitFrames(FRAME_INTERVAL);
    }
}
