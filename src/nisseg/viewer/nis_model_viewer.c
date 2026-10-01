#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/fade.h"
#include "dcb/sound.h"
#include "dcb/stage.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/model_anim.h"
#include "dcb/frame_callback.h"
#include "dcb/text.h"
#include "dcb/pad.h"
#include "dcb/nisseg.h"

/* The model viewer of the deck screens: a Digimon on the arena stage, the
   camera moved with the shoulder buttons and the d-pad, and Circle,
   Triangle and Cross playing its motions */

extern s16 CAMERA_TARGET_MODEL;
extern NisDeckScreens NIS_DECK_SCREENS;
void renderScrollingBackground();
void NIS_closeDeckWindow(NisWindow *window);
void NIS_drawViewerCameraHelp(NisWindow *window);
void NIS_drawViewerMotionHelp(NisWindow *window);

NisWindowDef NIS_VIEWER_CAMERA_HELP_WINDOW = { { 0x1A, 0x17, 0, 0xC }, { 0x1A, 0x17, 0x110, 0xC }, 0xA, 0, NIS_drawViewerCameraHelp, NIS_closeDeckWindow };
NisWindowDef NIS_VIEWER_MOTION_HELP_WINDOW = { { 0x1A, 0xD0, 0, 0xC }, { 0x1A, 0xD0, 0x110, 0xC }, 0xA, 0, NIS_drawViewerMotionHelp, NIS_closeDeckWindow };
/* not referenced by any code; a leftover value */
s32 D_801FC4A4 = 0x029807D2;

void NIS_drawViewerCameraHelp(NisWindow *window) {
    char text[0x40];

    /* " R1/R2 zoom in/out  L1/L2 raise/lower" */
    sprintf(text, " Ｒ１／Ｒ２　拡大／縮小　　Ｌ１／Ｌ２　上げる／下げる");
    drawIconText(0x1A, 0x18, 7, 1, window->z, (s32)text);
}

void NIS_drawViewerMotionHelp(NisWindow *window) {
    char text[0x40];

    /* " <Circle><Cross><Triangle> motion   <Square> back" */
    sprintf(text, " b0b2b1　モーション　　　b3　戻る");
    drawIconText(0x1A, 0xD1, 7, 1, window->z, (s32)text);
}

void NIS_openViewerScene(void) {
    initScene3D(1);
    createWireGrid(0xBB8, 0xBB8, 0xB, 0xB, 1, 0);
    GRID_VISIBLE = 0;
    endTask(0x19);
    spawnTask(0x19, 0x1F, 0, 0x800, runSceneCameraTask, 1);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
    loadArenaStage(-1);
}

void NIS_closeViewerScene(void) {
    endTask(0x1B);
    endTask(0x19);
    removeFrameCallback((s32)renderSceneModels);
    removeFrameCallback((s32)renderWireGrid);
    unloadArenaStage();
    unloadAllModels();
    freeHeapBlocksByTag(0x7F);
}

void NIS_showViewerModel(void) {
    ((Graphics *)&GRAPHICS)->snapCamera = 1;
    ((Graphics *)&GRAPHICS)->targetDistance = 0xBB8;
    ((Graphics *)&GRAPHICS)->targetHeight = -((Model *)SCENE_3D->models[0])->bonepos[0][1] * 3;
    SCENE_3D->modelState[0] = 1;
    showArenaStage(0);
    SCENE_3D->modelState[23] = 1; /* the stage */
    GRID_VISIBLE = 1;
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
}

void NIS_hideViewerModel(void) {
    freeHeapBlocksByTag(0x1F4);
    unloadModel(0);
    unloadModelAnimations(0);
    GRID_VISIBLE = 0;
    SCENE_3D->modelState[23] = 0;
}

void NIS_playViewerMotion(void) {
    s32 motion;

    CAMERA_TARGET_MODEL = 0;
    motion = 1;
    if (NIS_PRESSED() & PAD_CIRCLE) {
        motion = 1;
    } else if (NIS_PRESSED() & PAD_TRIANGLE) {
        motion = 2;
    } else if (NIS_PRESSED() & PAD_CROSS) {
        motion = 3;
    }
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x20, 0);
    applyAnimationFirstFrame(0, motion);
    startModelAnimation(0, motion, -2, 0);
    waitFrames(8);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (((Model *)SCENE_3D->models[0])->anim.keyTimer >= 0);
    waitFrames(0x1E);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x20, 0);
    applyAnimationFirstFrame(0, 0);
    startModelAnimation(0, 0, -2, 0);
    CAMERA_TARGET_MODEL = -1;
    waitFrames(8);
}

void NIS_runViewerControls(s32 parentTask) {
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_VIEWER_CAMERA_HELP_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.helpWindow = waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x1000, runWindowTask, &NIS_VIEWER_MOTION_HELP_WINDOW, getCurrentTaskId());
    NIS_DECK_SCREENS.motionWindow = waitFrames(0x7FFFFFFF);
    do {
        waitFrames(FRAME_INTERVAL);
        if ((NIS_HELD() & PAD_R1) && ((Graphics *)&GRAPHICS)->targetDistance > 1000) {
            ((Graphics *)&GRAPHICS)->targetDistance -= 0x19;
        } else if ((NIS_HELD() & PAD_R2) && ((Graphics *)&GRAPHICS)->targetDistance < 5000) {
            ((Graphics *)&GRAPHICS)->targetDistance += 0x19;
        } else if (NIS_HELD() & PAD_RIGHT) {
            ((Graphics *)&GRAPHICS)->rotY -= 0x10;
        } else if (NIS_HELD() & PAD_LEFT) {
            ((Graphics *)&GRAPHICS)->rotY += 0x10;
        } else if (NIS_HELD() & PAD_UP) {
            ((Graphics *)&GRAPHICS)->targetPitch -= 0x10;
        } else if (NIS_HELD() & PAD_DOWN) {
            ((Graphics *)&GRAPHICS)->targetPitch += 0x10;
        } else if ((NIS_HELD() & PAD_L1) && ((Graphics *)&GRAPHICS)->targetHeight > -1000) {
            ((Graphics *)&GRAPHICS)->targetHeight -= 0x19;
        } else if ((NIS_HELD() & PAD_L2) && ((Graphics *)&GRAPHICS)->targetHeight < 1000) {
            ((Graphics *)&GRAPHICS)->targetHeight += 0x19;
        } else if (NIS_PRESSED() & (PAD_TRIANGLE | PAD_CIRCLE | PAD_CROSS)) {
            NIS_playViewerMotion();
        }
    } while (!(NIS_HELD() & PAD_SQUARE));
    NIS_WINDOW(NIS_DECK_SCREENS.helpWindow)->state = 4;
    NIS_WINDOW(NIS_DECK_SCREENS.motionWindow)->state = 4;
    waitFrames(0xA);
    resumeTask(parentTask);
}

void NIS_runModelViewer(s32 digimonId) {
    s32 scrollMode;
    s32 mode;

    scrollMode = SCROLLING_BACKGROUND->scrollMode;
    mode = scrollMode + 1;
    if (mode >= 5) {
        mode = 2;
    }
    setBackgroundScrollMode(mode);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SCROLLING_BACKGROUND->unk1C8 != 0);
    removeFrameCallback((s32)renderScrollingBackground);
    loadSoundEffectBank(0);
    loadDigimonModelPak(0, digimonId);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 1, 2, 0x10, 0);
    NIS_showViewerModel();
    waitFrames(0x10);
    spawnTask(0, -1, 0, 0x400, NIS_runViewerControls, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 2, 0x10, 0);
    waitFrames(0x10);
    NIS_hideViewerModel();
    waitFrames(0xA);
    stopScreenFade();
    loadSoundEffectBank(1);
    setBackgroundScrollMode(scrollMode);
    addFrameCallback((s32)renderScrollingBackground);
    do {
        waitFrames(FRAME_INTERVAL);
    } while (SCROLLING_BACKGROUND->unk1C8 != 0x80);
}
