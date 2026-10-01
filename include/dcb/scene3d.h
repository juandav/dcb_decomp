#ifndef DCB_SCENE3D_H
#define DCB_SCENE3D_H

#include "game.h"

#define PULSE(n) (GRID_PULSE_PHASE + (n) * 12)

typedef struct {
    /* 0x000 */ MATRIX m;
    /* 0x020 */ VECTOR pos;
    /* 0x030 */ SVECTOR rot;
#if VERSION_JP
    /* 0x038 */ u8 unk38[0x108]; /* jp's EffectObject: the fields below are 4 bytes further */
#elif VERSION_US || VERSION_EU
    /* 0x038 */ u8 unk38[0x104];
#endif
    /* 0x13C */ Model *model;
    /* 0x140 */ u8 unk140[0x42F];
    /* 0x56F */ u8 axisMode;
    /* 0x570 */ u8 unk570;
    /* 0x571 */ s8 enabled;
} ModelLink;

extern MATRIX SCENE_LIGHT_MATRIX;
extern MATRIX SCENE_LIGHT_COLORS;
extern SVECTOR SCENE_WORLD_ROTATION;
extern void *GRID_LINE_PRIMS[];
extern SVECTOR *GRID_VERTICES;
extern s32 *GRID_SCREEN_XY;
extern s16 GRID_WIDTH;
extern s16 GRID_DEPTH;
extern s16 GRID_COLUMNS;
extern s16 GRID_ROWS;
extern s16 GRID_LINE_COUNT;
extern s32 GRID_VISIBLE;
extern u8 GRID_PULSE_PHASE;
extern s32 CAMERA_SNAP;

void setupSceneProjection(s32 projection);
void setupSceneLighting(void);
void renderSceneModels();
void initScene3D(s32 allocBuffers);

#endif /* DCB_SCENE3D_H */
