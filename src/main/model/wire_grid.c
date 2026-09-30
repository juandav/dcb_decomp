#include "dcb/wire_grid.h"
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

s32 GRID_VISIBLE = 1;
u8 GRID_PULSE_PHASE = 0;
u8 UNUSED_GRID_BYTES[12] = { 0 };

void renderWireGrid(FrameBuffer *buffer, s32 bufferIndex) {
    LINE_F2 *line;
    SVECTOR *vertex;
    SVECTOR *columnVertex;
    s32 *screenXY;
    s32 *columnXY;
    s32 i;
    s32 j;
    u8 color;
    s32 interp;
    s32 flag;

    line = GRID_LINE_PRIMS[bufferIndex];
    if (GRID_VISIBLE == 0) {
        return;
    }
    vertex = GRID_VERTICES;
    screenXY = GRID_SCREEN_XY;
    for (i = 0; i < GRID_COLUMNS * GRID_ROWS; i++, vertex++, screenXY++) {
        if ((vertex->pad = RotTransPers((s32)vertex, (s32)screenXY, &interp, &flag)) < 0x3C) {
            vertex->pad = -1;
        }
    }
    vertex = GRID_VERTICES;
    screenXY = GRID_SCREEN_XY;
    for (j = 0; j < GRID_ROWS; j++, vertex++, screenXY++) {
        for (i = 0; i < GRID_COLUMNS - 1; i++, vertex++, screenXY++) {
            color = PULSE(j + i);
            if ((s8)PULSE(j + i) < 0) {
                color = -color;
            }
            if ((u16)vertex[0].pad < 0x1000 && (u16)vertex[1].pad < 0x1000) {
                *(s32 *)&line->x0 = screenXY[0];
                *(s32 *)&line->x1 = screenXY[1];
                line->r0 = color >> 1;
                line->g0 = color + 0x40;
                line->b0 = (0x80 - color) / 2;
                AddPrim((s32 *)&buffer->ot[vertex[1].pad < vertex[0].pad ? vertex[0].pad : vertex[1].pad], (s32)line++);
            }
        }
    }
    vertex = GRID_VERTICES;
    screenXY = GRID_SCREEN_XY;
    for (i = 0; i < GRID_COLUMNS; i++, vertex++, screenXY++) {
        columnVertex = vertex;
        columnXY = screenXY;
        for (j = 0; j < GRID_ROWS - 1; j++, columnVertex += GRID_COLUMNS, columnXY += GRID_COLUMNS) {
            color = PULSE(i + j);
            if ((s8)PULSE(i + j) < 0) {
                color = -color;
            }
            if ((u16)columnVertex[0].pad < 0x1000 && (u16)columnVertex[GRID_COLUMNS].pad < 0x1000) {
                *(s32 *)&line->x0 = columnXY[0];
                *(s32 *)&line->x1 = columnXY[GRID_COLUMNS];
                line->r0 = color >> 1;
                line->g0 = color + 0x40;
                line->b0 = (0x80 - color) / 2;
                AddPrim((s32 *)&buffer->ot[columnVertex[1].pad < columnVertex[0].pad ? columnVertex[0].pad : columnVertex[1].pad], (s32)line++);
            }
        }
    }
    GRID_PULSE_PHASE += 2;
}

void createWireGrid(s32 width, s32 depth, s32 cols, s32 rows, s32 unused, s32 vertical) {
    u8 *line;
    SVECTOR *vertex;
    s32 i;
    s32 j;
    s32 row;

    GRID_COLUMNS = cols;
    GRID_ROWS = rows;
    GRID_LINE_COUNT = ((s16)cols - 1) * (s16)rows + ((s16)rows - 1) * (s16)cols;
    GRID_WIDTH = width;
    GRID_DEPTH = depth;
    GRID_VISIBLE = 1;
    for (i = 0; i < 2; i++) {
        line = GRID_LINE_PRIMS[i] = allocTaskHeapBlock(GRID_LINE_COUNT * 16);
        for (j = 0; j < GRID_LINE_COUNT; j++) {
            SetLineF2(line);
            line[4] = 8;
            line[5] = 0x40;
            line[6] = 8;
            line += 16;
        }
    }
    GRID_VERTICES = vertex = (SVECTOR *)allocTaskHeapBlock(GRID_COLUMNS * (GRID_ROWS << 3));
    GRID_SCREEN_XY = (s32 *)allocTaskHeapBlock(GRID_COLUMNS * (GRID_ROWS << 2));
    for (row = 0; row < GRID_ROWS; row++) {
        for (j = 0; j < GRID_COLUMNS; j++) {
            vertex->vx = GRID_WIDTH / 2 - GRID_WIDTH / (GRID_COLUMNS - 1) * j;
            if (vertical) {
                vertex->vy = GRID_DEPTH / 2 - GRID_DEPTH / (GRID_ROWS - 1) * row;
                vertex->vz = 0;
            } else {
                vertex->vz = GRID_DEPTH / 2 - GRID_DEPTH / (GRID_ROWS - 1) * row;
                vertex->vy = 0;
            }
            vertex++;
        }
    }
    addFrameCallback((s32)renderWireGrid);
}

void freeWireGrid(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        freeHeapBlock(GRID_LINE_PRIMS[i]);
    }
    freeHeapBlock(GRID_VERTICES);
    freeHeapBlock(GRID_SCREEN_XY);
}
