#include "common.h"
#include "gte.h"
#include "game.h"

void func_8001ED30(s32 arg0, s16 *arg1, void *arg2) {
    s32 axis;

    axis = arg0 & 0xFF;
    if (axis == 0) {
        return;
    }
    func_8001ED04(arg2);
    switch (axis) {
    case 4:
        return;
    case 3:
        arg1[1] = 0;
        arg1[2] = 0;
        break;
    case 2:
        arg1[0] = 0;
        arg1[2] = 0;
        break;
    case 1:
        arg1[0] = 0;
        arg1[1] = 0;
        break;
    }
    RotMatrix(arg1, arg2);
}

/* old-style definition: the callers pass an int, the byte is read here */
void func_8001EDE0(rot, trans, scale, m, axis)
    SVECTOR *rot;
    VECTOR *trans;
    VECTOR *scale;
    MATRIX *m;
    u8 axis;
{
    RotMatrix(rot, m);
    func_8001ED30(axis, (s16 *)rot, m);
    TransMatrix(m, trans);
    if (scale != 0 && (scale->vx != 0x1000 || scale->vy != scale->vx || scale->vz != scale->vy)) {
        ScaleMatrix(m, scale);
    }
    func_8001EFB0((s32)m);
}

void func_8001EEA0(void *arg0, s32 arg1) {
    s16 v[4];
    s32 flag;
    s32 axis;
    s32 sx;
    s32 sy;
    void *rot;

    axis = arg1 & 0xFF;
    if ((*(s32 *)((s8 *)arg0 + 0x48)) == 0) {
        func_8001EDE0((s8 *)arg0 + 0x30, (s8 *)arg0 + 0x20, (s8 *)arg0 + 0x38, arg0, axis);
        return;
    }
    rot = (s8 *)arg0 + 0x30;
    func_8001EFB0((*(s32 *)((s8 *)arg0 + 0x48)));
    RotMatrix(rot, arg0);
    MulMatrix2(*(MATRIX **)((s8 *)arg0 + 0x48), arg0);
    func_8001ED30(axis, rot, arg0);
    v[0] = (*(u16 *)((s8 *)arg0 + 0x20));
    v[1] = (*(u16 *)((s8 *)arg0 + 0x24));
    v[2] = (*(u16 *)((s8 *)arg0 + 0x28));
    RotTrans(v, (s8 *)arg0 + 0x14, &flag);
    sx = (*(s32 *)((s8 *)arg0 + 0x38));
    if ((sx != 0x1000 || (sy = (*(s32 *)((s8 *)arg0 + 0x3C))) != sx || (*(s32 *)((s8 *)arg0 + 0x40)) != sy) && axis != 4) {
        ScaleMatrix(arg0, (s8 *)arg0 + 0x38);
    }
    func_8001EFB0((s32) arg0);
}

void func_8001EFB0(s32 arg0) {
    func_8005C444();
    SetRotMatrix(arg0);
}

void func_8001EFDC(void *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s16 arg5, s16 arg6, s16 arg7) {
    (*(s32 *)((s8 *)arg0 + 0x20)) = arg2;
    (*(s32 *)((s8 *)arg0 + 0x24)) = arg3;
    (*(s32 *)((s8 *)arg0 + 0x28)) = arg4;
    (*(s16 *)((s8 *)arg0 + 0x30)) = arg5;
    (*(s16 *)((s8 *)arg0 + 0x32)) = arg6;
    (*(s16 *)((s8 *)arg0 + 0x34)) = arg7;
    (*(s32 *)((s8 *)arg0 + 0x38)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x3C)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x40)) = 0x1000;
    (*(s32 *)((s8 *)arg0 + 0x48)) = arg1;
}

void func_8001F01C(void *arg0, void *arg1) {
    (*(u16 *)((s8 *)arg1 + 0)) = (u16) (*(u16 *)((s8 *)arg0 + 0x14));
    (*(u16 *)((s8 *)arg1 + 2)) = (u16) (*(u16 *)((s8 *)arg0 + 0x18));
    (*(u16 *)((s8 *)arg1 + 4)) = (u16) (*(u16 *)((s8 *)arg0 + 0x1C));
}
