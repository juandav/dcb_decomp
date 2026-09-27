#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/transform.h"
#include "dcb/prim_util.h"

void resetMatrixRotation(void *matrix) {
    (*(s16 *)((s8 *)matrix + 0)) = 0x1000;
    (*(s16 *)((s8 *)matrix + 6)) = 0;
    (*(s16 *)((s8 *)matrix + 0xC)) = 0;
    (*(s16 *)((s8 *)matrix + 2)) = 0;
    (*(s16 *)((s8 *)matrix + 8)) = 0x1000;
    (*(s16 *)((s8 *)matrix + 0xE)) = 0;
    (*(s16 *)((s8 *)matrix + 4)) = 0;
    (*(s16 *)((s8 *)matrix + 0xA)) = 0;
    (*(s16 *)((s8 *)matrix + 0x10)) = 0x1000;
}

void constrainRotationAxis(s32 axisMode, s16 *rot, void *matrix) {
    s32 mode;

    mode = axisMode & 0xFF;
    if (mode == 0) {
        return;
    }
    resetMatrixRotation(matrix);
    switch (mode) {
    case 4:
        return;
    case 3:
        rot[1] = 0;
        rot[2] = 0;
        break;
    case 2:
        rot[0] = 0;
        rot[2] = 0;
        break;
    case 1:
        rot[0] = 0;
        rot[1] = 0;
        break;
    }
    RotMatrix(rot, matrix);
}

/* old-style definition: the callers pass an int, the byte is read here */
void composeTransformMatrix(rot, trans, scale, m, axis)
    SVECTOR *rot;
    VECTOR *trans;
    VECTOR *scale;
    MATRIX *m;
    u8 axis;
{
    RotMatrix(rot, m);
    constrainRotationAxis(axis, (s16 *)rot, m);
    TransMatrix(m, trans);
    if (scale != 0 && (scale->vx != 0x1000 || scale->vy != scale->vx || scale->vz != scale->vy)) {
        ScaleMatrix(m, scale);
    }
    loadGteMatrix((s32)m);
}

void updateTransformMatrix(void *xform, s32 axisMode) {
    s16 localPos[4];
    s32 flag;
    s32 mode;
    s32 sx;
    s32 sy;
    void *rot;

    mode = axisMode & 0xFF;
    if ((*(s32 *)((s8 *)xform + 0x48)) == 0) {
        composeTransformMatrix((s8 *)xform + 0x30, (s8 *)xform + 0x20, (s8 *)xform + 0x38, xform, mode);
        return;
    }
    rot = (s8 *)xform + 0x30;
    loadGteMatrix((*(s32 *)((s8 *)xform + 0x48)));
    RotMatrix(rot, xform);
    MulMatrix2(*(MATRIX **)((s8 *)xform + 0x48), xform);
    constrainRotationAxis(mode, rot, xform);
    localPos[0] = (*(u16 *)((s8 *)xform + 0x20));
    localPos[1] = (*(u16 *)((s8 *)xform + 0x24));
    localPos[2] = (*(u16 *)((s8 *)xform + 0x28));
    RotTrans(localPos, (s8 *)xform + 0x14, &flag);
    sx = (*(s32 *)((s8 *)xform + 0x38));
    if ((sx != 0x1000 || (sy = (*(s32 *)((s8 *)xform + 0x3C))) != sx || (*(s32 *)((s8 *)xform + 0x40)) != sy) && mode != 4) {
        ScaleMatrix(xform, (s8 *)xform + 0x38);
    }
    loadGteMatrix((s32) xform);
}

void loadGteMatrix(s32 matrix) {
    func_8005C444();
    SetRotMatrix(matrix);
}

void initTransform(void *xform, s32 parent, s32 x, s32 y, s32 z, s16 rotX, s16 rotY, s16 rotZ) {
    (*(s32 *)((s8 *)xform + 0x20)) = x;
    (*(s32 *)((s8 *)xform + 0x24)) = y;
    (*(s32 *)((s8 *)xform + 0x28)) = z;
    (*(s16 *)((s8 *)xform + 0x30)) = rotX;
    (*(s16 *)((s8 *)xform + 0x32)) = rotY;
    (*(s16 *)((s8 *)xform + 0x34)) = rotZ;
    (*(s32 *)((s8 *)xform + 0x38)) = 0x1000;
    (*(s32 *)((s8 *)xform + 0x3C)) = 0x1000;
    (*(s32 *)((s8 *)xform + 0x40)) = 0x1000;
    (*(s32 *)((s8 *)xform + 0x48)) = parent;
}

void getTransformWorldPos(void *xform, void *outPos) {
    (*(u16 *)((s8 *)outPos + 0)) = (u16) (*(u16 *)((s8 *)xform + 0x14));
    (*(u16 *)((s8 *)outPos + 2)) = (u16) (*(u16 *)((s8 *)xform + 0x18));
    (*(u16 *)((s8 *)outPos + 4)) = (u16) (*(u16 *)((s8 *)xform + 0x1C));
}
