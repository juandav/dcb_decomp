#include "common.h"
#include "game.h"
#include "dcb/evo_cutscene.h"
#include "dcb/scene3d.h"
#include "dcb/model_anim.h"
#include "dcb/wire_grid.h"
#include "dcb/frame_callback.h"
#include "dcb/heap.h"
#include "dcb/model_load.h"
#include "dcb/anim_control.h"
#include "dcb/task.h"
#include "dcb/stage.h"
#include "dcb/player_data.h"
#include "dcb/fade.h"
#include "dcb/loader.h"
#include "dcb/sound_play.h"
#include "dcb/prim.h"
#include "dcb/sound.h"
#include "dcb/scroll_bg.h"
#include "gte.h"
#include "dcb/evoseg.h"
#include "dcb/evo_shatter.h"
#include "dcb/evo_fusion.h"

/* libgs's GsSortObject4J function table (_GsFCALL) */
#define GsDivMODE_NDIV 0

#define GsDivMODE_DIV 1
#define GsLMODE_NORMAL 0
#define GsLMODE_LOFF 2

typedef struct {
    s32 vx;
    s32 vy;
    s32 vz;
    u8 r;
    u8 g;
    u8 b;
    u8 pad;
} FlatLight;

typedef u32 *(*GsSortFunc)();

typedef struct {
    GsSortFunc f3[2][3];
    GsSortFunc nf3[2];
    GsSortFunc g3[2][3];
    GsSortFunc ng3[2];
    GsSortFunc tf3[2][3];
    GsSortFunc ntf3[2];
    GsSortFunc tg3[2][3];
    GsSortFunc ntg3[2];
    GsSortFunc f4[2][3];
    GsSortFunc nf4[2];
    GsSortFunc g4[2][3];
    GsSortFunc ng4[2];
    GsSortFunc tf4[2][3];
    GsSortFunc ntf4[2];
    GsSortFunc tg4[2][3];
    GsSortFunc ntg4[2];
    GsSortFunc f3g[3];
    GsSortFunc g3g[3];
    GsSortFunc f4g[3];
    GsSortFunc g4g[3];
} _GsFCALL;

/* libgs TMD primitive layouts */
typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_F3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_G3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p;
} TMD_P_F4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 r0, g0, b0, code;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_G4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 v1, v2;
} TMD_P_TF3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
} TMD_P_TG3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 r0, g0, b0, p1;
    u16 v0, v1;
    u16 v2, p2;
} TMD_P_TNF3;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 v1, v2;
    u16 v3, p2;
} TMD_P_TF4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u16 n0, v0;
    u16 n1, v1;
    u16 n2, v2;
    u16 n3, v3;
} TMD_P_TG4;

typedef struct {
    u8 out, in, dummy, cd;
    u8 tu0, tv0;
    u16 clut;
    u8 tu1, tv1;
    u16 tpage;
    u8 tu2, tv2;
    u16 p0;
    u8 tu3, tv3;
    u16 p1;
    u8 r0, g0, b0, p2;
    u16 v0, v1;
    u16 v2, v3;
} TMD_P_TNF4;

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
    s16 x2, y2;
    s16 x3, y3;
    u32 pad;
} LINE_F4;

/* the GPU packet being built: one of these, or a shattered wireframe */
typedef union {
    PolyF3 f3;
    PolyF4 f4;
    PolyG3 g3;
    PolyG4 g4;
    POLY_FT3 ft3;
    POLY_FT4 ft4;
    POLY_GT3 gt3;
    POLY_GT4 gt4;
    LINE_F4 line;
} EvoPacket;

extern s16 CAMERA_TARGET_MODEL;
extern _GsFCALL GsFCALL4;
extern u16 EVO_BANNER_CLUT[16];
extern s8 EVO_BANNER_FADE;
extern u8 EVO_BANNER_BRIGHTNESS;
extern MATRIX GsLIGHTWSMATRIX;

void GsGetLs(GsCOORDINATE2 *coord, MATRIX *m);
void GsSetLsMatrix(MATRIX *m);
void GsSetLightMatrix(MATRIX *m);
void GsSortObject4(GsDOBJ4 *obj, GsOT *ot, s32 shift, u32 *scratch);
s32 GsSetFlatLight(s32 id, FlatLight *light);
u32 *GsTMDfastF3L(), *GsTMDfastTF3L(), *GsTMDfastTF3NL(), *GsTMDfastTNF3();
u32 *GsTMDfastG3L(), *GsTMDfastTG3L(), *GsTMDfastTG3NL(), *GsTMDfastTNG3();
u32 *GsTMDfastF4L(), *GsTMDfastTF4L(), *GsTMDfastTF4NL(), *GsTMDfastTNF4();
u32 *GsTMDfastG4L(), *GsTMDfastTG4L(), *GsTMDfastTG4NL(), *GsTMDfastTNG4();
u32 *GsTMDdivTF4L(), *GsTMDdivTF4NL(), *GsTMDdivTF3NL(), *GsTMDdivTG4NL();
u32 *GsTMDdivTG3NL(), *GsTMDdivTNG4(), *GsTMDdivTNG3(), *GsTMDdivTNF4();
u32 *GsTMDdivTNF3(), *GsTMDfastF4NL(), *GsTMDfastNF4();
s32 StoreImage(Rect16 *rect, void *p);
void GsMulCoord3(MATRIX *m1, MATRIX *m2);
void MulMatrix0(MATRIX *m0, MATRIX *m1, MATRIX *m2);
void SetLightMatrix(MATRIX *m);
s32 RotNclip3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, s32 *sxy0, s32 *sxy1, s32 *sxy2, s32 *p, s32 *otz, s32 *flag);

s32 RotNclip4(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, SVECTOR *v3, s32 *sxy0, s32 *sxy1, s32 *sxy2, s32 *sxy3, s32 *p,
              s32 *otz, s32 *flag);

void NormalColorCol3(SVECTOR *v0, SVECTOR *v1, SVECTOR *v2, Color *in, Color *out0, Color *out1, Color *out2);
void NormalColorCol(SVECTOR *n, Color *in, Color *out);
void EVO_renderDissolvingObject(GsDOBJ4 *obj, s32 mode);
void EVO_initShatterScene(s32 arg);
void EVO_renderCutsceneModels();
void EVO_remapPartTextures(EvoPart *part, s32 enable);
void EVO_runFusedDigimonTask(s32 parentTask);
void EVO_initGsSortTable(void);
void EVO_drawFusionBanner(void);

const char D_801DDF38[] = "\t";

void EVO_initCutsceneScene(s8 evolved) {
    if (evolved == 0) {
        EVO_initShatterScene(1);
        createWireGrid(3000, 3000, 11, 11, 1, 0);
    } else {
        initScene3D(1);
        addFrameCallback((s32)renderWireGrid);
    }
    GRID_VISIBLE = 0;
    endTask(0x19);
    spawnTask(0x19, 0x1F, 0, 0x800, &runSceneCameraTask, 1);
    endTask(0x1B);
    spawnTask(0x1B, -1, 0, 0x1000, runModelAnimationTask, 1);
}

void EVO_freeCutsceneScene(s8 evolved) {
    endTask(0x1B);
    endTask(0x19);
    removeFrameCallback((s32)renderWireGrid);
    removeFrameCallback((s32)EVO_tickShatter);
    if (evolved == 0) {
        removeFrameCallback((s32)EVO_renderCutsceneModels);
    } else {
        removeFrameCallback((s32)renderSceneModels);
    }
    waitFrames(1);
    unloadAllModels();
    GRID_VISIBLE = 0;
    freeHeapBlocksByTag(0x7F);
    if (evolved == 0) {
        freeHeapBlocksByTag(0x1F5);
        freeHeapBlocksByTag(0x41);
    } else {
        freeHeapBlocksByTag(0x84);
    }
    freeHeapBlocksByTag(0x1F4);
    freeHeapBlocksByTag(0x40);
}

void EVO_restartModelAnimation(s16 slot) {
    applyAnimationFirstFrame(slot, 0);
    startModelAnimation(slot, 0, -2, 0);
}

void EVO_placeFusionModels(void) {
    Graphics *camera;
    EvoModel *model;
    EvoPart *part;
    s32 i;

    camera = (Graphics *)&GRAPHICS;
    camera->snapCamera = 1;
    camera->targetDistance = 3000;
    camera->targetHeight = -((EvoModel *)SCENE_3D->models[0])->pose->y * 3;
    camera->targetModel = 0;
    SCENE_3D->modelState[0] = 1;
    SCENE_3D->modelState[1] = -1;
    GRID_VISIBLE = 1;
    EVO_restartModelAnimation(0);
    model = SCENE_3D->models[1];
    for (i = 0, part = model->parts; i < model->partCount; i++, part++) {
        EVO_remapPartTextures(part, 1);
    }
}

void EVO_showFusedDigimon(s32 id) {
    loadDigimonModelPak(0, id, 0, 0);
    waitFrames(2);
    waitFrames(20);
    setModelAnimationPose(0, 0);
    CAMERA_TARGET_MODEL = 0;
    waitFrames(1);
    SCENE_3D->modelState[0] = 1;
    playModelAnimation(0, 0);
    GRID_VISIBLE = 1;
    spawnTask(0, -1, 0, 0x400, EVO_runFusedDigimonTask, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(10);
}

void EVO_initFusionBanner(void) {
    Rect16 rect = { 320, 240, 32, 1 };
    s8 i;

    StoreImage(&rect, EVO_BANNER_CLUT);
    for (i = 0; i < 16; i++) {
        if (EVO_BANNER_CLUT[i] != 0) {
            EVO_BANNER_CLUT[i] |= 0x8000;
        } else {
            EVO_BANNER_CLUT[i] = 0;
        }
    }
    LoadImage((s16 *)&rect, (s32)EVO_BANNER_CLUT);
    for (i = 0; i < 16; i++) {
        if (EVO_BANNER_CLUT[i] != 0) {
            EVO_BANNER_CLUT[i] = 0xFFFF;
        }
    }
    rect.y = 241;
    LoadImage((s16 *)&rect, (s32)EVO_BANNER_CLUT);
    EVO_BANNER_FADE = 2;
    EVO_BANNER_BRIGHTNESS = 0;
    addFrameCallback((s32)EVO_drawFusionBanner);
}

const u8 EVO_OLD_FUSION_RECIPES[20][3] = {
    { 0x01, 0x04, 0xEC },
    { 0x04, 0x23, 0xEC },
    { 0x02, 0x25, 0xED },
    { 0x06, 0x27, 0xED },
    { 0x8E, 0x28, 0xEE },
    { 0x2A, 0x2B, 0xEF },
    { 0x4C, 0x07, 0xF0 },
    { 0x4D, 0x4B, 0xF1 },
    { 0x4B, 0x8F, 0xF2 },
    { 0x51, 0x03, 0xF3 },
    { 0x70, 0x4A, 0xF4 },
    { 0x4E, 0x6F, 0xF5 },
    { 0x6E, 0x6F, 0xF6 },
    { 0x6D, 0x90, 0xF7 },
    { 0x93, 0x73, 0xF8 },
    { 0x96, 0x74, 0xF9 },
    { 0x91, 0x26, 0xFA },
    { 0x0C, 0x75, 0xFB },
    { 0x53, 0x0D, 0xFC },
    { 0x52, 0x97, 0xFD },
};

s16 EVO_WIRE_SHADE_MIN = 0x37;
s16 EVO_WIRE_SHADE_MAX = 0xFF;
s8 EVO_DISSOLVE_PATTERN[16] = { 6, 3, 11, 8, 10, 13, 1, 4, 14, 7, 12, 15, 5, 2, 9, 0 };
s16 EVO_PART_DRAW_MODES[40] = {
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
    16, 16, 16, 16, 16, 16, 16, 16, 16, 16,
};

/* not referenced by any code; the last two bytes are leftovers, not the same
   in every version */
#if VERSION_US
u8 D_801EF8C4[16] = { 0x40, 0x48, 0x4C, 0x50, 0x58, 0x5C, 0x20, 0x28, 0x24, 0x2C, 0x30, 0x38, 0x34, 0x3C, 0xF6, 0x57 };
#elif VERSION_EU
u8 D_801EF8C4[16] = { 0x40, 0x48, 0x4C, 0x50, 0x58, 0x5C, 0x20, 0x28, 0x24, 0x2C, 0x30, 0x38, 0x34, 0x3C, 0x62, 0x14 };
#else
#error "evoseg/cutscene/evo_cutscene: version not checked"
#endif

void EVO_drawFusionBanner(void) {
    Rect16 uv;

    if (EVO_BANNER_FADE == 1) {
        EVO_BANNER_BRIGHTNESS += 8;
        if (EVO_BANNER_BRIGHTNESS > 0x80) {
            EVO_BANNER_BRIGHTNESS = 0x80;
        }
    } else if (EVO_BANNER_FADE == 2) {
        if (EVO_BANNER_BRIGHTNESS >= 8) {
            EVO_BANNER_BRIGHTNESS -= 8;
        } else {
            EVO_BANNER_BRIGHTNESS = 0;
        }
    }
    uv.x = 0;
    uv.y = 0x80;
    uv.w = 0xFF;
    uv.h = 0x27;
    drawTexturedSprite(0x28, 0xB4, &uv, 0x25, 0x3C14, 0, EVO_BANNER_BRIGHTNESS, 0);
    drawTexturedSprite(0x28, 0xB4, &uv, 0x45, 0x3C54, 0, EVO_BANNER_BRIGHTNESS, 0);
}

void EVO_runFusedDigimonTask(s32 parentTask) {
    s32 frames;

    EVO_initFusionBanner();
    setScreenFadeParams(1, 1, 8);
    playSoundEffect(0x8D);
    waitFrames(120);
    EVO_BANNER_FADE = 1;
    waitFrames(180);
    frames = 0;
    do {
        waitFrames(FRAME_INTERVAL);
        frames++;
        if ((PAD_STATES[0]->pressed & 0x40) || frames > 180) {
            EVO_BANNER_FADE = 2;
            EVO_CUTSCENE_STEP = 6;
        }
    } while (EVO_CUTSCENE_STEP != 6);
    EVO_SCREEN_FLASH.on = 1;
    waitFrames(10);
    resumeTask(parentTask);
}

void EVO_runShatterTask(s32 parentTask) {
    do {
        waitFrames(FRAME_INTERVAL);
        switch (EVO_CUTSCENE_STEP) {
        case 1:
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 8, 0);
            waitFrames(60);
            EVO_CUTSCENE_STEP = 2;
            SCENE_3D->modelState[0] = -1;
            break;
        case 2:
            playSoundEffect(0x8D);
            EVO_startShatter(1);
            SCENE_3D->modelState[1] = 1;
            applyAnimationFirstFrame(1, 0);
            startModelAnimation(1, 0, -2, 0);
            CAMERA_TARGET_MODEL = 1;
            waitFrames(5);
            setScreenFadeParams(1, 1, 8);
            EVO_CUTSCENE_STEP = 3;
            break;
        case 4:
            spawnTask(0, -1, 0, 0x200, screenFadeTask, 0, 1, 8, 0);
            waitFrames(60);
            EVO_CUTSCENE_STEP = 5;
            break;
        }
    } while (EVO_CUTSCENE_STEP != 5);
    waitFrames(10);
    resumeTask(parentTask);
}

void EVO_shatterFusionModels(s32 unused) {
    s32 models[2];

    models[0] = loadDigimonModelPak(0, EVO_CUTSCENE_MODELS[0], 1, 1);
    models[1] = loadDigimonModelPak(1, EVO_CUTSCENE_MODELS[1], 1, 1);
    EVO_placeFusionModels();
    CAMERA_TARGET_MODEL = 0;
    waitFrames(20);
    EVO_SCREEN_FLASH.on = 0;
    playSoundEffect(0x8D);
    spawnTask(0, -1, 0, 0x400, EVO_runShatterTask, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(10);
}

void EVO_runFusionCutscene(void) {
    loadSoundEffectBank(0);
    playMusic(0, 0x88, 100);
    EVO_initCutsceneScene(0);
    EVO_shatterFusionModels(0x73);
    EVO_freeCutsceneScene(0);
    waitFrames(2);
    playMusic(0, 0x89, 100);
    waitFrames(2);
    EVO_initCutsceneScene(1);
    EVO_showFusedDigimon(EVO_CUTSCENE_MODELS[2]);
    EVO_freeCutsceneScene(1);
    waitFrames(10);
    do {
        waitFrames(1);
    } while (PAD_STATES[0]->pressed & 0x40);
    removeFrameCallback((s32)EVO_drawFusionBanner);
    loadSoundEffectBank(1);
    changeScrollingBackground(((PlayerProfile *)PLAYER_PROFILES)->activePartner, 0x380, 0, 0x380, 0x80);
    spawnTask(0, -1, 0, 0x400, EVO_runFusion, -1, getCurrentTaskId(), 0, 0);
}

void EVO_setGteMatrix(MATRIX *m) {
    gte_SetRotMatrix(m);
    gte_SetTransMatrix(m);
}

void EVO_renderCutsceneModels(FrameBuffer *buffer, s32 bufferIndex) {
    MATRIX localScreen;
    SVECTOR position;
    MATRIX unused; /* unused, but it is in the original stack frame */
    MATRIX lightMatrix;
    MATRIX colorMatrix;
    s32 flag;
    Model *model;
    GsDOBJ4 *obj;
    s32 i;
    s32 j;
    s32 *scratch;

    if (SCENE_3D_ENABLED != 0) {
        GsSetWorkBase((long)buffer->scenePackets);
        colorMatrix = SCENE_LIGHT_COLORS;
        for (i = 0; i < 24; i++) {
            if (SCENE_3D->modelState[i] <= 0) {
                continue;
            }
            model = SCENE_3D->models[i];
            lightMatrix = SCENE_LIGHT_MATRIX;
            gte_SetColorMatrix(&colorMatrix);
            PushMatrix();
            position.vx = model->pos.vx;
            position.vy = model->pos.vy;
            position.vz = model->pos.vz;
            RotTrans((u16 *)&position, localScreen.t, &flag);
            RotMatrix(&SCENE_WORLD_ROTATION, &SCENE_3D->root.coord);
            SCENE_3D->root.flg = 0;
            SCENE_3D->root.coord.t[0] = localScreen.t[0];
            SCENE_3D->root.coord.t[1] = localScreen.t[1];
            SCENE_3D->root.coord.t[2] = localScreen.t[2];
            RotMatrixYXZ(&model->rot, &model->root.coord);
            model->root.flg = 0;
            ScaleMatrix(&model->root.coord, &model->scale);
            if (EVO_SHATTER_STARTED == 0) {
                EVO_startShatter(0);
                EVO_SHATTER_STARTED = 1;
            }
            scratch = (s32 *)0x1F800000;
            scratch[12] = model->tpageOffset;
            scratch[13] = model->clutOffset;
            obj = model->obj;
            for (j = 0; j < model->nobj; j++, obj++) {
                obj->coord2->flg = 0;
                if (obj->id != -1 && obj->tmd != NULL && obj->attribute == 0) {
                    GsGetLws(obj->coord2, &model->lw[j], &localScreen);
                    GsGetLs(obj->coord2, &localScreen);
                    GsSetLsMatrix(&localScreen);
                    GsSetLightMatrix(&localScreen);
                    if (EVO_PART_DRAW_MODES[j] != 16) {
                        EVO_renderDissolvingObject(obj, EVO_PART_DRAW_MODES[j]);
                    } else {
                        GsSortObject4(obj, &SCENE_3D->ot[bufferIndex], 2, (u32 *)0x1F800000);
                    }
                }
            }
            PopMatrix();
        }
    }
}

void EVO_initView(s32 projection) {
    SetGeomOffset(160, 120);
    SetGeomScreen(projection);
    GsSetProjection(projection);
    SCENE_3D->view.vpx = 0;
    SCENE_3D->view.vpy = 0;
    SCENE_3D->view.vpz = 0;
    SCENE_3D->view.vrx = 0;
    SCENE_3D->view.vry = 0;
    SCENE_3D->view.vrz = 0;
    SCENE_3D->view.rz = 0;
    SCENE_3D->view.super = NULL;
    GsSetRefView2(&SCENE_3D->view);
}

void EVO_initLights(void) {
    ((FlatLight *)SCENE_3D->flatLights)[0].vx = 0;
    ((FlatLight *)SCENE_3D->flatLights)[0].vy = -100;
    ((FlatLight *)SCENE_3D->flatLights)[0].vz = 100;
    ((FlatLight *)SCENE_3D->flatLights)[0].r = 0xFF;
    ((FlatLight *)SCENE_3D->flatLights)[0].g = 0xFF;
    ((FlatLight *)SCENE_3D->flatLights)[0].b = 0xFF;
    GsSetFlatLight(0, &((FlatLight *)SCENE_3D->flatLights)[0]);
    ((FlatLight *)SCENE_3D->flatLights)[1].vx = 0;
    ((FlatLight *)SCENE_3D->flatLights)[1].vy = 100;
    ((FlatLight *)SCENE_3D->flatLights)[1].vz = 100;
    ((FlatLight *)SCENE_3D->flatLights)[1].r = 0x80;
    ((FlatLight *)SCENE_3D->flatLights)[1].g = 0x80;
    ((FlatLight *)SCENE_3D->flatLights)[1].b = 0x80;
    GsSetFlatLight(1, &((FlatLight *)SCENE_3D->flatLights)[1]);
    ((FlatLight *)SCENE_3D->flatLights)[2].vx = 0;
    ((FlatLight *)SCENE_3D->flatLights)[2].vy = 0;
    ((FlatLight *)SCENE_3D->flatLights)[2].vz = 0;
    ((FlatLight *)SCENE_3D->flatLights)[2].r = 0;
    ((FlatLight *)SCENE_3D->flatLights)[2].g = 0;
    ((FlatLight *)SCENE_3D->flatLights)[2].b = 0;
    GsSetFlatLight(2, &((FlatLight *)SCENE_3D->flatLights)[2]);
    GsSetAmbient(0x40, 0x40, 0x40);
    SetBackColor(0x30, 0x30, 0x40);
    GsSetLightMode(0);
}

void EVO_initShatterScene(s32 allocBuffers) {
    s32 i;

    EVO_initGsSortTable();
    initModelScene();
    EnterCriticalSection();
    for (i = 0; i < 2; i++) {
        if (allocBuffers) {
            DB(i).scenePackets = allocHeapBlock(0x1F400, 0x7F);
        }
        SCENE_3D->ot[i].length = 12;
        SCENE_3D->ot[i].org = DB(i).ot;
        SCENE_3D->ot[i].offset = 0;
        SCENE_3D->ot[i].point = 0;
        SCENE_3D->ot[i].tag = SCENE_3D->ot[i].org + 0xFFF;
    }
    EVO_SHARDS = allocHeapBlock(0x4B0, 0x7F);
    EVO_SHARD_VERTEX_POOL = allocHeapBlock(0x3E80, 0x7F);
    EVO_SHATTER_STARTED = 0;
    EVO_CUTSCENE_STEP = 0;
    GsInit3D();
    EVO_initView(0x1C0);
    EVO_initLights();
    ExitCriticalSection();
    {
        MATRIX lightMatrices[2] = {
            { { { 0, 0x1800, -0x1800 }, { 0, 0, 0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
            { { { 0, 0x1000, -0x5DC }, { 0, -0x1000, -0x7D0 }, { 0, 0, 0 } }, { 0, 0, 0 } },
        };
        MATRIX colorMatrices[2] = {
            { { { 0x800, 0, 0 }, { 0x800, 0, 0 }, { 0x800, 0, 0 } }, { 0, 0, 0 } },
            { { { 0x1000, 0x5DC, 0 }, { 0xFF5, 0x5DC, 0 }, { 0x1000, 0x5DC, 0 } }, { 0, 0, 0 } },
        };

        SCENE_LIGHT_MATRIX = lightMatrices[1];
        SCENE_LIGHT_COLORS = colorMatrices[1];
    }
    if (allocBuffers) {
        mountDriveTask((s32)"M:", getCurrentTaskId());
        addFrameCallback((s32)EVO_renderCutsceneModels);
    }
}

void EVO_remapPartTextures(EvoPart *part, s32 enable) {
    TmdObject *tmd;
    s32 count;
    TmdPrim *prim;
    u8 *next;
    s32 i;

    tmd = part->tmd;
    count = tmd->nprim;
    prim = tmd->prims;
    if (enable == 0) {
        return;
    }
    for (i = 0; i < count; i++) {
        if ((prim->mode & 0x3D) == 0x2C) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x20;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x3C) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x24;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x34) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x1C;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x24) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x18;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x2D) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x20;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x28) {
            next += 0x14;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x25) {
            prim->cba |= 0x4000;
            prim->tsb = (prim->tsb & ~0x1F) | 0x15;
            next = (u8 *)prim + 0x1C;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x3D) == 0x38) {
            next += 0x18;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x30) {
            next += 0x14;
            prim = (TmdPrim *)next;
        } else if ((prim->mode & 0x35) == 0x20) {
            next += 0x10;
            prim = (TmdPrim *)next;
        }
    }
}

/* Wireframe of a quad that is shattering: a closed 4-point polyline v0 v1 v3 v2 */
#define SHATTER_QUAD(pk, shade, otz)                        \
    {                                                       \
        LINE_F2 *line;                                      \
        setlen(&(pk)->line, 6);                             \
        setcode(&(pk)->line, 0x4C);                         \
        (pk)->line.pad = 0x55555555;                        \
        (pk)->line.r0 = (pk)->line.g0 = (pk)->line.b0 = shade; \
        otz >>= 2;                                          \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &(pk)->line); \
        line = (LINE_F2 *)(&(pk)->line + 1);                \
        setlen(line, 3);                                    \
        setcode(line, 0x40);                                \
        line->r0 = line->g0 = line->b0 = shade;             \
        line->x0 = (pk)->line.x3;                           \
        line->y0 = (pk)->line.y3;                           \
        line->x1 = (pk)->line.x0;                           \
        line->y1 = (pk)->line.y0;                           \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], line);      \
        pk = (EvoPacket *)(line + 1);                       \
    }

/* Wireframe of a triangle that is shattering: a closed polyline v0 v1 v2 v0 */
#define SHATTER_TRI(pk, shade, otz)                           \
    {                                                         \
        (pk)->line.x3 = (pk)->line.x0;                        \
        (pk)->line.y3 = (pk)->line.y0;                        \
        setlen(&(pk)->line, 6);                               \
        setcode(&(pk)->line, 0x4C);                           \
        (pk)->line.pad = 0x55555555;                          \
        (pk)->line.r0 = (pk)->line.g0 = (pk)->line.b0 = shade; \
        otz >>= 2;                                            \
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &(pk)->line); \
        pk = (EvoPacket *)(&(pk)->line + 1);                  \
    }

void EVO_renderDissolvingObject(GsDOBJ4 *obj, s32 mode) {
    Color rgb;
    MATRIX m;
    s32 p;
    s32 otz;
    s32 flag;
    s32 i;
    s32 nprim;
    SVECTOR *norm;
    SVECTOR *vert;
    TmdPrim *prim;
    EvoPacket *pk;
    TmdObject *tmd;
    GsCOORDINATE2 *coord;
    s8 shade;
    union {
        TMD_P_F3 *f3;
        TMD_P_G3 *g3;
        TMD_P_F4 *f4;
        TMD_P_G4 *g4;
        TMD_P_TF3 *tf3;
        TMD_P_TG3 *tg3;
        TMD_P_TNF3 *tnf3;
        TMD_P_TF4 *tf4;
        TMD_P_TG4 *tg4;
        TMD_P_TNF4 *tnf4;
    } op;
    /* the match depends on code's type, which gives it its register in each
       version */
#if VERSION_US
    s32 code;
#elif VERSION_EU
    s8 code;
#else
#error "evoseg/cutscene/evo_cutscene: version not checked"
#endif

    shade = EVO_WIRE_SHADE_MIN + rand() % (EVO_WIRE_SHADE_MAX - EVO_WIRE_SHADE_MIN);
    tmd = (TmdObject *)obj->tmd;
    vert = tmd->vertTop;
    norm = tmd->normTop;
    nprim = tmd->nprim;
    prim = tmd->prims;
    pk = (EvoPacket *)GsGetWorkBase();
    rgb.r = rgb.g = rgb.b = 0x80;
    coord = obj->coord2;
    if (coord->flg == 0) {
        coord->flg = 1;
        MulMatrix0(&coord->coord, &coord->super->workm, &coord->workm);
    }
    MulMatrix0(&GsLIGHTWSMATRIX, &coord->workm, &m);
    SetLightMatrix(&m);
    CompMatrix(&GsWSMATRIX, &coord->workm, &m);
    EVO_setGteMatrix(&m);
    for (i = 0; i < nprim; i++) {
        if ((code = prim->mode & 0x3D) == 0x2C) {
            op.tf4 = (TMD_P_TF4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tf4->v0], &vert[op.tf4->v1], &vert[op.tf4->v2], &vert[op.tf4->v3],
                              (s32 *)&pk->ft4.x0, (s32 *)&pk->ft4.x1, (s32 *)&pk->ft4.x2, (s32 *)&pk->ft4.x3, &p, &otz,
                              &flag) > 0) {
                    NormalColorCol(&norm[op.tf4->n0], &rgb, (Color *)&pk->ft4.r0);
                    pk->ft4.u0 = op.tf4->tu0;
                    pk->ft4.v0 = op.tf4->tv0;
                    pk->ft4.u1 = op.tf4->tu1;
                    pk->ft4.v1 = op.tf4->tv1;
                    pk->ft4.u2 = op.tf4->tu2;
                    pk->ft4.v2 = op.tf4->tv2;
                    pk->ft4.u3 = op.tf4->tu3;
                    pk->ft4.v3 = op.tf4->tv3;
                    pk->ft4.clut = op.tf4->clut;
                    pk->ft4.tpage = op.tf4->tpage;
                    setlen(&pk->ft4, 9);
                    setcode(&pk->ft4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft4);
                    pk = (EvoPacket *)(&pk->ft4 + 1);
                }
                prim = (TmdPrim *)(op.tf4 + 1);
            } else {
                if (RotNclip4(&vert[op.tf4->v0], &vert[op.tf4->v1], &vert[op.tf4->v2], &vert[op.tf4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tf4 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x3C) {
            op.tg4 = (TMD_P_TG4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tg4->v0], &vert[op.tg4->v1], &vert[op.tg4->v2], &vert[op.tg4->v3],
                              (s32 *)&pk->gt4.x0, (s32 *)&pk->gt4.x1, (s32 *)&pk->gt4.x2, (s32 *)&pk->gt4.x3, &p, &otz,
                              &flag) > 0) {
                    NormalColorCol3(&norm[op.tg4->n0], &norm[op.tg4->n1], &norm[op.tg4->n2], &rgb, (Color *)&pk->gt4.r0,
                                    (Color *)&pk->gt4.r1, (Color *)&pk->gt4.r2);
                    NormalColorCol(&norm[op.tg4->n3], &rgb, (Color *)&pk->gt4.r3);
                    pk->gt4.u0 = op.tg4->tu0;
                    pk->gt4.v0 = op.tg4->tv0;
                    pk->gt4.u1 = op.tg4->tu1;
                    pk->gt4.v1 = op.tg4->tv1;
                    pk->gt4.u2 = op.tg4->tu2;
                    pk->gt4.v2 = op.tg4->tv2;
                    pk->gt4.u3 = op.tg4->tu3;
                    pk->gt4.v3 = op.tg4->tv3;
                    pk->gt4.clut = op.tg4->clut;
                    pk->gt4.tpage = op.tg4->tpage;
                    setlen(&pk->gt4, 12);
                    setcode(&pk->gt4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->gt4);
                    pk = (EvoPacket *)(&pk->gt4 + 1);
                }
                prim = (TmdPrim *)(op.tg4 + 1);
            } else {
                if (RotNclip4(&vert[op.tg4->v0], &vert[op.tg4->v1], &vert[op.tg4->v2], &vert[op.tg4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tg4 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x34) {
            op.tg3 = (TMD_P_TG3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tg3->v0], &vert[op.tg3->v1], &vert[op.tg3->v2], (s32 *)&pk->gt3.x0,
                              (s32 *)&pk->gt3.x1, (s32 *)&pk->gt3.x2, &p, &otz, &flag) > 0) {
                    NormalColorCol3(&norm[op.tg3->n0], &norm[op.tg3->n1], &norm[op.tg3->n2], &rgb, (Color *)&pk->gt3.r0,
                                    (Color *)&pk->gt3.r1, (Color *)&pk->gt3.r2);
                    pk->gt3.u0 = op.tg3->tu0;
                    pk->gt3.v0 = op.tg3->tv0;
                    pk->gt3.u1 = op.tg3->tu1;
                    pk->gt3.v1 = op.tg3->tv1;
                    pk->gt3.u2 = op.tg3->tu2;
                    pk->gt3.v2 = op.tg3->tv2;
                    pk->gt3.clut = op.tg3->clut;
                    pk->gt3.tpage = op.tg3->tpage;
                    setlen(&pk->gt3, 9);
                    setcode(&pk->gt3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->gt3);
                    pk = (EvoPacket *)(&pk->gt3 + 1);
                }
                prim = (TmdPrim *)(op.tg3 + 1);
            } else {
                if (RotNclip3(&vert[op.tg3->v0], &vert[op.tg3->v1], &vert[op.tg3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tg3 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x24) {
            op.tf3 = (TMD_P_TF3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tf3->v0], &vert[op.tf3->v1], &vert[op.tf3->v2], (s32 *)&pk->ft3.x0,
                              (s32 *)&pk->ft3.x1, (s32 *)&pk->ft3.x2, &p, &otz, &flag) > 0) {
                    NormalColorCol(&norm[op.tf3->n0], &rgb, (Color *)&pk->ft3.r0);
                    pk->ft3.u0 = op.tf3->tu0;
                    pk->ft3.v0 = op.tf3->tv0;
                    pk->ft3.u1 = op.tf3->tu1;
                    pk->ft3.v1 = op.tf3->tv1;
                    pk->ft3.u2 = op.tf3->tu2;
                    pk->ft3.v2 = op.tf3->tv2;
                    pk->ft3.clut = op.tf3->clut;
                    pk->ft3.tpage = op.tf3->tpage;
                    setlen(&pk->ft3, 7);
                    setcode(&pk->ft3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft3);
                    pk = (EvoPacket *)(&pk->ft3 + 1);
                }
                prim = (TmdPrim *)(op.tf3 + 1);
            } else {
                if (RotNclip3(&vert[op.tf3->v0], &vert[op.tf3->v1], &vert[op.tf3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tf3 + 1);
            }
        } else if ((prim->mode & 0x3D) == 0x2D) {
            op.tnf4 = (TMD_P_TNF4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.tnf4->v0], &vert[op.tnf4->v1], &vert[op.tnf4->v2], &vert[op.tnf4->v3],
                              (s32 *)&pk->ft4.x0, (s32 *)&pk->ft4.x1, (s32 *)&pk->ft4.x2, (s32 *)&pk->ft4.x3, &p, &otz,
                              &flag) > 0) {
                    pk->ft4.u0 = op.tnf4->tu0;
                    pk->ft4.v0 = op.tnf4->tv0;
                    pk->ft4.u1 = op.tnf4->tu1;
                    pk->ft4.v1 = op.tnf4->tv1;
                    pk->ft4.u2 = op.tnf4->tu2;
                    pk->ft4.v2 = op.tnf4->tv2;
                    pk->ft4.u3 = op.tnf4->tu3;
                    pk->ft4.v3 = op.tnf4->tv3;
                    pk->ft4.clut = op.tnf4->clut;
                    pk->ft4.tpage = op.tnf4->tpage;
                    setlen(&pk->ft4, 9);
                    setcode(&pk->ft4, 0x2C);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft4);
                    pk = (EvoPacket *)(&pk->ft4 + 1);
                }
                prim = (TmdPrim *)(op.tnf4 + 1);
            } else {
                if (RotNclip4(&vert[op.tnf4->v0], &vert[op.tnf4->v1], &vert[op.tnf4->v2], &vert[op.tnf4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tnf4 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x28) {
            op.f4 = (TMD_P_F4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.f4->v0], &vert[op.f4->v1], &vert[op.f4->v2], &vert[op.f4->v3], (s32 *)&pk->f4.x0,
                              (s32 *)&pk->f4.x1, (s32 *)&pk->f4.x2, (s32 *)&pk->f4.x3, &p, &otz, &flag) > 0) {
                    NormalColorCol(&norm[op.f4->n0], &rgb, (Color *)&pk->f4.r0);
                    setlen(&pk->f4, 5);
                    setcode(&pk->f4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->f4);
                    pk = (EvoPacket *)(&pk->f4 + 1);
                }
                prim = (TmdPrim *)(op.f4 + 1);
            } else {
                if (RotNclip4(&vert[op.f4->v0], &vert[op.f4->v1], &vert[op.f4->v2], &vert[op.f4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.f4 + 1);
            }
        } else if ((prim->mode & 0x35) == 0x25) {
            op.tnf3 = (TMD_P_TNF3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.tnf3->v0], &vert[op.tnf3->v1], &vert[op.tnf3->v2], (s32 *)&pk->ft3.x0,
                              (s32 *)&pk->ft3.x1, (s32 *)&pk->ft3.x2, &p, &otz, &flag) > 0) {
                    pk->ft3.u0 = op.tnf3->tu0;
                    pk->ft3.v0 = op.tnf3->tv0;
                    pk->ft3.u1 = op.tnf3->tu1;
                    pk->ft3.v1 = op.tnf3->tv1;
                    pk->ft3.u2 = op.tnf3->tu2;
                    pk->ft3.v2 = op.tnf3->tv2;
                    pk->ft3.clut = op.tnf3->clut;
                    pk->ft3.tpage = op.tnf3->tpage;
                    setlen(&pk->ft3, 7);
                    setcode(&pk->ft3, 0x24);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->ft3);
                    pk = (EvoPacket *)(&pk->ft3 + 1);
                }
                prim = (TmdPrim *)(op.tnf3 + 1);
            } else {
                if (RotNclip3(&vert[op.tnf3->v0], &vert[op.tnf3->v1], &vert[op.tnf3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.tnf3 + 1);
            }
        } else if ((code = prim->mode & 0x3D) == 0x38) {
            op.g4 = (TMD_P_G4 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip4(&vert[op.g4->v0], &vert[op.g4->v1], &vert[op.g4->v2], &vert[op.g4->v3], (s32 *)&pk->g4.x0,
                              (s32 *)&pk->g4.x1, (s32 *)&pk->g4.x2, (s32 *)&pk->g4.x3, &p, &otz, &flag) > 0) {
                    NormalColorCol3(&norm[op.g4->n0], &norm[op.g4->n1], &norm[op.g4->n2], &rgb, (Color *)&pk->g4.r0,
                                    (Color *)&pk->g4.r1, (Color *)&pk->g4.r2);
                    NormalColorCol(&norm[op.g4->n3], &rgb, (Color *)&pk->g4.r3);
                    setlen(&pk->g4, 8);
                    setcode(&pk->g4, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->g4);
                    pk = (EvoPacket *)(&pk->g4 + 1);
                }
                prim = (TmdPrim *)(op.g4 + 1);
            } else {
                if (RotNclip4(&vert[op.g4->v0], &vert[op.g4->v1], &vert[op.g4->v2], &vert[op.g4->v3],
                              (s32 *)&pk->line.x0, (s32 *)&pk->line.x1, (s32 *)&pk->line.x3, (s32 *)&pk->line.x2, &p,
                              &otz, &flag) > 0) {
                    SHATTER_QUAD(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.g4 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x30) {
            op.g3 = (TMD_P_G3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.g3->v0], &vert[op.g3->v1], &vert[op.g3->v2], (s32 *)&pk->g3.x0,
                              (s32 *)&pk->g3.x1, (s32 *)&pk->g3.x2, &p, &otz, &flag) > 0) {
                    NormalColorCol(&norm[op.g3->n0], &rgb, (Color *)&pk->g3.r0);
                    setlen(&pk->g3, 6);
                    setcode(&pk->g3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->g3);
                    pk = (EvoPacket *)(&pk->g3 + 1);
                }
                prim = (TmdPrim *)(op.g3 + 1);
            } else {
                if (RotNclip3(&vert[op.g3->v0], &vert[op.g3->v1], &vert[op.g3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.g3 + 1);
            }
        } else if ((code = prim->mode & 0x35) == 0x20) {
            op.f3 = (TMD_P_F3 *)prim;
            if (EVO_DISSOLVE_PATTERN[i & 0xF] < mode) {
                if (RotNclip3(&vert[op.f3->v0], &vert[op.f3->v1], &vert[op.f3->v2], (s32 *)&pk->f3.x0,
                              (s32 *)&pk->f3.x1, (s32 *)&pk->f3.x2, &p, &otz, &flag) > 0) {
                    NormalColorCol(&norm[op.f3->n0], &rgb, (Color *)&pk->f3.r0);
                    setlen(&pk->f3, 4);
                    setcode(&pk->f3, code);
                    otz >>= 2;
                    addPrim(&CURRENT_FRAME_BUFFER->ot[otz], &pk->f3);
                    pk = (EvoPacket *)(&pk->f3 + 1);
                }
                prim = (TmdPrim *)(op.f3 + 1);
            } else {
                if (RotNclip3(&vert[op.f3->v0], &vert[op.f3->v1], &vert[op.f3->v2], (s32 *)&pk->line.x0,
                              (s32 *)&pk->line.x1, (s32 *)&pk->line.x2, &p, &otz, &flag) > 0) {
                    SHATTER_TRI(pk, shade, otz);
                }
                prim = (TmdPrim *)(op.f3 + 1);
            }
        }
    }
    GsSetWorkBase((long)pk);
}

void EVO_initGsSortTable(void) {
    GsFCALL4.f3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF3L;
    GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF3L;
    GsFCALL4.tf3[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTF3NL;
    GsFCALL4.ntf3[GsDivMODE_NDIV] = GsTMDfastTNF3;
    GsFCALL4.g3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG3L;
    GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG3L;
    GsFCALL4.tg3[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTG3NL;
    GsFCALL4.ntg3[GsDivMODE_NDIV] = GsTMDfastTNG3;
    GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastF4L;
    GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTF4L;
    GsFCALL4.tf4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTF4NL;
    GsFCALL4.ntf4[GsDivMODE_NDIV] = GsTMDfastTNF4;
    GsFCALL4.g4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastG4L;
    GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_NORMAL] = GsTMDfastTG4L;
    GsFCALL4.tg4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastTG4NL;
    GsFCALL4.ntg4[GsDivMODE_NDIV] = GsTMDfastTNG4;
    GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_NORMAL] = GsTMDdivTF4L;
    GsFCALL4.tf4[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTF4NL;
    GsFCALL4.tf3[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTF3NL;
    GsFCALL4.tg4[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTG4NL;
    GsFCALL4.tg3[GsDivMODE_DIV][GsLMODE_LOFF] = GsTMDdivTG3NL;
    GsFCALL4.ntg4[GsDivMODE_DIV] = GsTMDdivTNG4;
    GsFCALL4.ntg3[GsDivMODE_DIV] = GsTMDdivTNG3;
    GsFCALL4.ntf4[GsDivMODE_DIV] = GsTMDdivTNF4;
    GsFCALL4.ntf3[GsDivMODE_DIV] = GsTMDdivTNF3;
    GsFCALL4.f4[GsDivMODE_NDIV][GsLMODE_LOFF] = GsTMDfastF4NL;
    GsFCALL4.nf4[GsDivMODE_NDIV] = GsTMDfastNF4;
}

void EVO_getCoordWorldMatrix(GsCOORDINATE2 *coord, MATRIX *m) {
    GsCOORDINATE2 *chain[100];
    GsCOORDINATE2 **p;

    chain[0] = coord;
    p = &chain[1];
    while (coord->super != NULL) {
        coord = coord->super;
        *p++ = coord;
    }
    p--;
    *m = (*p)->coord;
    while (chain < p) {
        p--;
        GsMulCoord3(m, &(*p)->coord);
    }
}

void EVO_getPartWorldMatrix(s32 part, MATRIX *m) {
    GsCOORDINATE2 root;
    GsCOORDINATE2 node;
    VECTOR trans;

    trans = *(VECTOR *)((Model *)SCENE_3D->models[0])->bonepos[0];
    root = ((Model *)SCENE_3D->models[0])->coord[0];
    node = ((Model *)SCENE_3D->models[0])->coord[part];
    RotMatrix(&((Model *)SCENE_3D->models[0])->rots[0], &root.coord);
    ScaleMatrix(&root.coord, ((Model *)SCENE_3D->models[0])->boneScale);
    TransMatrix(&root.coord, &trans);
    EVO_getCoordWorldMatrix(&node, m);
}
