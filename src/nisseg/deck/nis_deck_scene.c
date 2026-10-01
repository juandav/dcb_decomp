#include "common.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/prim.h"
#include "dcb/scene3d.h"
#include "dcb/wire_grid.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/model_anim.h"
#include "dcb/frame_callback.h"
#include "dcb/nisseg.h"

/* The deck screens' 3D scene: a Digimon model on its wire grid, drawn
   into a corner of VRAM that the screens show as a picture */

extern DR_ENV NIS_DECK_SCENE_ENVS[2];
extern DR_ENV NIS_DECK_SCREEN_ENVS[2];
extern DRAWENV NIS_DECK_SCENE_DRAWENVS[2];

void SetDrawEnv(DR_ENV *dr, DRAWENV *env);

void NIS_renderDeckScene(FrameBuffer *fb, s32 buffer) {
    buffer ^= 1;
    fillVramRect(NIS_DECK_SCENE_DRAWENVS[buffer].clip[0], NIS_DECK_SCENE_DRAWENVS[buffer].clip[1], 0x180, 0x80, 0);
    AddPrim((s32 *)&fb->ot[0xFFF], (s32)&NIS_DECK_SCENE_ENVS[buffer]);
    SetDrawEnv(&NIS_DECK_SCREEN_ENVS[buffer], &fb->draw);
    AddPrim((s32 *)&fb->ot[0x12C], (s32)&NIS_DECK_SCREEN_ENVS[buffer]);
}

void NIS_openDeckScene(s32 modelId) {
    SetDefDrawEnv(&NIS_DECK_SCENE_DRAWENVS[0], 0x140, 0x100, 0x60, 0x80);
    SetDefDrawEnv(&NIS_DECK_SCENE_DRAWENVS[1], 0x140, 0x180, 0x60, 0x80);
    NIS_DECK_SCENE_DRAWENVS[0].ofs[0] -= 0x70;
    NIS_DECK_SCENE_DRAWENVS[1].ofs[0] -= 0x70;
    NIS_DECK_SCENE_DRAWENVS[0].ofs[1] -= 0x14;
    NIS_DECK_SCENE_DRAWENVS[1].ofs[1] -= 0x14;
    SetDrawEnv(&NIS_DECK_SCENE_ENVS[0], &NIS_DECK_SCENE_DRAWENVS[0]);
    SetDrawEnv(&NIS_DECK_SCENE_ENVS[1], &NIS_DECK_SCENE_DRAWENVS[1]);
    initScene3D(1);
    addFrameCallback((s32)NIS_renderDeckScene);
    createWireGrid(0x1F4, 0x1F4, 0xB, 0xB, 0, 0);
    loadOmdModelFromDisc(0, modelId, -1);
    if (loadModelAnimationFile(0, 0, 0) != 0) {
        startModelAnimation(0, 0, -2, 0);
    }
    SCENE_3D->modelState[0] = 1;
    spawnTask(0x19, -1, 0, 0x800, runSceneCameraTask, 2);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void NIS_closeDeckScene(void) {
    if (NIS_DECK_EDIT.cardType == 0) {
        endTask(0x19);
        endTask(0x1B);
        removeFrameCallback((s32)NIS_renderDeckScene);
        removeFrameCallback((s32)renderSceneModels);
        removeFrameCallback((s32)renderWireGrid);
        waitFrames(4);
        unloadAllModels();
        freeHeapBlocksByTag(0x7F);
    }
}

/* writes three lines of text into VRAM, under the scene */
void NIS_runDeckEditor(void);

void NIS_writeSceneCaption(char *line1, char *line2, char *line3) {
    Rect16 rect = { 0x3C0, 0xDC };

    fillVramRect(0x3C0, 0xDC, 0xF0, 0x24, 0);
    uploadKanjiString(line1, &rect);
    rect.x = 0x3C0;
    rect.y += 0xC;
    uploadKanjiString(line2, &rect);
    rect.x = 0x3C0;
    rect.y += 0xC;
    uploadKanjiString(line3, &rect);
}

void NIS_startDeckEditor(void) {
    spawnTask(0, -1, 0, 0x1000, NIS_runDeckEditor, 0);
}
