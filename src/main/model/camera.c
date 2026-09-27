#include "dcb/camera.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
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

s32 stepCameraTowardTarget(u8 *camera, s32 *pos, s32 distance, s16 *target) {
    s32 speed;
    s32 steps;
    s32 delta;
    s32 step;

    if (D_8007956C != 0) {
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

INCLUDE_ASM("asm/main/nonmatchings/model/camera", runSceneCameraTask);

INCLUDE_RODATA("asm/main/nonmatchings/model/camera", D_80010190);
