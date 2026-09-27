#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/fade.h"
#include "dcb/prim_util.h"

void func_8001F040(void) {
    D_801D69E0 = 0;
}

void func_8001F04C(void) {
    D_801D69E0 = 0;
}

s32 func_8001F058(void) {
    return D_801D69E0;
}

void func_8001F068(s32 arg0, s32 arg1, s32 arg2) {
    D_801D69E8 = arg0;
    D_801D69EC = arg1;
    D_801D69F0 = arg2;
    D_801D69E4 = arg0 * 0xFF;
}

void func_8001F094(s32 dir, s32 abr, s32 speed) {
    while (D_801D69E0 != 0) {
        func_80014C08(D_800794F0);
    }
    D_801D69E8 = dir;
    D_801D69EC = abr;
    D_801D69F0 = speed;
    D_801D69E0 = 1;
    D_801D69E4 = dir * 0xFF;
    while (D_801D69E0 != 0) {
        func_80014C08(D_800794F0);
        if (D_801D69E8 != 0) {
            if ((D_801D69E4 -= D_801D69F0) < 0) {
                D_801D69E4 = 0;
                break;
            }
        } else if ((D_801D69E4 += D_801D69F0) >= 0x100) {
            D_801D69E4 = 0xFF;
        }
        SetDrawTPage(D_801D69D0[D_800794F4], 0, 0, GetTPage(0, D_801D69EC, 0, 0));
        func_8001E6EC(8, &D_801D69A0[D_800794F4], 1, 0);
        func_8001E75C(&D_801D69A0[D_800794F4], (u8)D_801D69E4, (u8)D_801D69E4, (u8)D_801D69E4);
        SetSemiTrans(&D_801D69A0[D_800794F4], 1);
        D_801D69A0[D_800794F4].x0 = 0;
        D_801D69A0[D_800794F4].y0 = 0;
        D_801D69A0[D_800794F4].x1 = 320;
        D_801D69A0[D_800794F4].y1 = 0;
        D_801D69A0[D_800794F4].x2 = 0;
        D_801D69A0[D_800794F4].y2 = 240;
        D_801D69A0[D_800794F4].x3 = 320;
        D_801D69A0[D_800794F4].y3 = 240;
        AddPrim((s32 *)D_800793A0->ot, (s32)&D_801D69A0[D_800794F4]);
        AddPrim((s32 *)D_800793A0->ot, (s32)D_801D69D0[D_800794F4]);
    }
    D_801D69E0 = 0;
}
