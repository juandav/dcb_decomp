#ifndef DCB_TRANSFORM_H
#define DCB_TRANSFORM_H

#include "game.h"

/* an object's place in the scene: its local position, rotation and scale,
   the parent it hangs from, and the matrix built from them */
typedef struct Transform {
    /* 0x00 */ MATRIX matrix; /* the world matrix; t is the world position */
    /* 0x20 */ VECTOR trans;
    /* 0x30 */ SVECTOR rot;
    /* 0x38 */ VECTOR scale; /* 0x1000 = 1.0 */
    /* 0x48 */ struct Transform *parent;
} Transform;

void constrainRotationAxis(s32 axisMode, SVECTOR *rot, MATRIX *m);
void loadGteMatrix(s32 matrix);
void updateTransformMatrix(void *xform, s32 axisMode);
void initTransform(void *xform, s32 parent, s32 x, s32 y, s32 z, s16 rotX, s16 rotY, s16 rotZ);
void getTransformWorldPos(void *xform, void *outPos);

#include "dcb/prim_util.h"

void resetMatrixRotation(MATRIX *m);

#endif /* DCB_TRANSFORM_H */
