#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/transform.h"
#include "dcb/prim_util.h"

void resetMatrixRotation(MATRIX *m) {
    m->m[0][0] = 0x1000;
    m->m[1][0] = 0;
    m->m[2][0] = 0;
    m->m[0][1] = 0;
    m->m[1][1] = 0x1000;
    m->m[2][1] = 0;
    m->m[0][2] = 0;
    m->m[1][2] = 0;
    m->m[2][2] = 0x1000;
}

/* rebuilds m's rotation from one axis of rot only: 1 keeps z, 2 keeps y,
   3 keeps x, 4 leaves no rotation and 0 leaves m untouched */
void constrainRotationAxis(s32 axisMode, SVECTOR *rot, MATRIX *m) {
    u8 mode;

    mode = axisMode & 0xFF;
    if (mode == 0) {
        return;
    }
    resetMatrixRotation(m);
    switch (mode) {
    case 4:
        return;
    case 3:
        rot->vy = 0;
        rot->vz = 0;
        break;
    case 2:
        rot->vx = 0;
        rot->vz = 0;
        break;
    case 1:
        rot->vx = 0;
        rot->vy = 0;
        break;
    }
    RotMatrix(rot, m);
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
    constrainRotationAxis(axis, rot, m);
    TransMatrix(m, trans);
    if (scale != 0 && (scale->vx != 0x1000 || scale->vy != scale->vx || scale->vz != scale->vy)) {
        ScaleMatrix(m, scale);
    }
    loadGteMatrix((s32)m);
}

void updateTransformMatrix(void *xform, s32 axisMode) {
    Transform *t = xform;
    s16 localPos[4];
    s32 flag;
    u8 mode;
    s32 sx;
    s32 sy;
    SVECTOR *rot;

    mode = axisMode & 0xFF;
    if (t->parent == NULL) {
        composeTransformMatrix(&t->rot, &t->trans, &t->scale, &t->matrix, mode);
        return;
    }
    rot = &t->rot;
    /* the parent's matrix goes into the GTE: the child is built inside it */
    loadGteMatrix((s32)t->parent);
    RotMatrix(rot, &t->matrix);
    MulMatrix2(&t->parent->matrix, &t->matrix);
    constrainRotationAxis(mode, rot, &t->matrix);
    localPos[0] = t->trans.vx;
    localPos[1] = t->trans.vy;
    localPos[2] = t->trans.vz;
    RotTrans(localPos, t->matrix.t, &flag);
    sx = t->scale.vx;
    if ((sx != 0x1000 || (sy = t->scale.vy) != sx || t->scale.vz != sy) && mode != 4) {
        ScaleMatrix(&t->matrix, &t->scale);
    }
    loadGteMatrix((s32)t);
}

void loadGteMatrix(s32 matrix) {
    SetTransMatrix();
    SetRotMatrix(matrix);
}

void initTransform(void *xform, s32 parent, s32 x, s32 y, s32 z, s16 rotX, s16 rotY, s16 rotZ) {
    Transform *t = xform;

    t->trans.vx = x;
    t->trans.vy = y;
    t->trans.vz = z;
    t->rot.vx = rotX;
    t->rot.vy = rotY;
    t->rot.vz = rotZ;
    t->scale.vx = 0x1000;
    t->scale.vy = 0x1000;
    t->scale.vz = 0x1000;
    t->parent = (Transform *)parent;
}

void getTransformWorldPos(void *xform, void *outPos) {
    Transform *t = xform;
    SVECTOR *pos = outPos;

    pos->vx = t->matrix.t[0];
    pos->vy = t->matrix.t[1];
    pos->vz = t->matrix.t[2];
}
