#ifndef DCB_SCENE3D_H
#define DCB_SCENE3D_H

#include "game.h"

#define PULSE(n) (GRID_PULSE_PHASE + (n) * 12)

typedef struct {
    u32 tag;
    u8 r0, g0, b0, code;
    s16 x0, y0;
    s16 x1, y1;
} LINE_F2;
typedef struct {
    /* 0x000 */ MATRIX m;
    /* 0x020 */ VECTOR pos;
    /* 0x030 */ SVECTOR rot;
    /* 0x038 */ u8 unk38[0x104];
    /* 0x13C */ Model *model;
    /* 0x140 */ u8 unk140[0x42F];
    /* 0x56F */ u8 unk56F;
    /* 0x570 */ u8 unk570;
    /* 0x571 */ s8 unk571;
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
extern s32 D_8007956C;

void setupSceneProjection(s32 projection);
void setupSceneLighting(void);
void renderSceneModels();
void renderWireGrid();
void createWireGrid(s32 width, s32 depth, s32 cols, s32 rows, s32 unused, s32 vertical);
void freeWireGrid(void);
void initScene3D(s32 allocBuffers);

#endif /* DCB_SCENE3D_H */
