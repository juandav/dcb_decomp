#include "common.h"
#include "game.h"
#include "dcb/sug_camera.h"
#include "dcb/sugseg.h"

extern s32 SUG_CAMERA_SPIN;
extern s32 SUG_CAMERA_ORBIT_RANGE;
extern s32 SUG_CAMERA_YAW_SPIN;
extern u16 CAMERA_TARGET_PITCH;

void SUG_resetCameraPos(void) {
    CAMERA->posZ = 0;
    CAMERA->posX = 0;
    CAMERA->targetHeight = 0;
    CAMERA->posY = -150;
}

void SUG_orbitCamera(void) {
    s32 speed;
    s32 min;
    s32 max;
    s32 outStep;
    s32 inStep;
    s32 angle;

    speed = 1;
    SUG_resetCameraPos();
    CAMERA->rotY += SUG_CAMERA_SPIN * 2;
    if (SUG_CAMERA_ORBIT_RANGE == speed) {
        min = 0x100;
        max = 0x580;
        outStep = 16;
        inStep = 1;
    } else {
        min = 0x400;
        max = 0xD80;
        outStep = -1;
        inStep = -16;
    }
    angle = CAMERA_TARGET_PITCH & 0xFFF;
    if (max < angle || angle < min) {
        CAMERA->targetPitch += outStep * speed;
    } else {
        CAMERA->targetPitch += inStep * speed;
    }
    CAMERA->targetYaw += 4 / speed * SUG_CAMERA_YAW_SPIN;
}
