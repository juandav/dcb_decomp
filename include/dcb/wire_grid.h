#ifndef DCB_WIRE_GRID_H
#define DCB_WIRE_GRID_H

#include "game.h"
#include "dcb/scene3d.h"

void renderWireGrid();
void createWireGrid(s32 width, s32 depth, s32 cols, s32 rows, s32 unused, s32 vertical);
void freeWireGrid(void);

#endif /* DCB_WIRE_GRID_H */
