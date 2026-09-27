#ifndef DCB_TRANSFORM_H
#define DCB_TRANSFORM_H

#include "game.h"

void constrainRotationAxis(s32 axisMode, s16 *rot, void *matrix);
void loadGteMatrix(s32 matrix);
void updateTransformMatrix(void *xform, s32 axisMode);
void initTransform(void *xform, s32 parent, s32 x, s32 y, s32 z, s16 rotX, s16 rotY, s16 rotZ);
void getTransformWorldPos(void *xform, void *outPos);

#include "dcb/prim_util.h"

void resetMatrixRotation(void *matrix);

#endif /* DCB_TRANSFORM_H */
