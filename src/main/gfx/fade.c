#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/fade.h"
#include "dcb/prim_util.h"

void initScreenFade(void) {
    SCREEN_FADE_ACTIVE = 0;
}

void stopScreenFade(void) {
    SCREEN_FADE_ACTIVE = 0;
}

s32 isScreenFadeActive(void) {
    return SCREEN_FADE_ACTIVE;
}

void setScreenFadeParams(s32 fadeIn, s32 blendMode, s32 speed) {
    SCREEN_FADE_DIRECTION = fadeIn;
    SCREEN_FADE_BLEND_MODE = blendMode;
    SCREEN_FADE_SPEED = speed;
    SCREEN_FADE_LEVEL = fadeIn * 0xFF;
}

void screenFadeTask(s32 fadeIn, s32 blendMode, s32 speed) {
    while (SCREEN_FADE_ACTIVE != 0) {
        func_80014C08(D_800794F0);
    }
    SCREEN_FADE_DIRECTION = fadeIn;
    SCREEN_FADE_BLEND_MODE = blendMode;
    SCREEN_FADE_SPEED = speed;
    SCREEN_FADE_ACTIVE = 1;
    SCREEN_FADE_LEVEL = fadeIn * 0xFF;
    while (SCREEN_FADE_ACTIVE != 0) {
        func_80014C08(D_800794F0);
        if (SCREEN_FADE_DIRECTION != 0) {
            if ((SCREEN_FADE_LEVEL -= SCREEN_FADE_SPEED) < 0) {
                SCREEN_FADE_LEVEL = 0;
                break;
            }
        } else if ((SCREEN_FADE_LEVEL += SCREEN_FADE_SPEED) >= 0x100) {
            SCREEN_FADE_LEVEL = 0xFF;
        }
        SetDrawTPage(SCREEN_FADE_TPAGES[D_800794F4], 0, 0, GetTPage(0, SCREEN_FADE_BLEND_MODE, 0, 0));
        initPrimByType(8, &SCREEN_FADE_POLYS[D_800794F4], 1, 0);
        setPrimRgb0(&SCREEN_FADE_POLYS[D_800794F4], (u8)SCREEN_FADE_LEVEL, (u8)SCREEN_FADE_LEVEL, (u8)SCREEN_FADE_LEVEL);
        SetSemiTrans(&SCREEN_FADE_POLYS[D_800794F4], 1);
        SCREEN_FADE_POLYS[D_800794F4].x0 = 0;
        SCREEN_FADE_POLYS[D_800794F4].y0 = 0;
        SCREEN_FADE_POLYS[D_800794F4].x1 = 320;
        SCREEN_FADE_POLYS[D_800794F4].y1 = 0;
        SCREEN_FADE_POLYS[D_800794F4].x2 = 0;
        SCREEN_FADE_POLYS[D_800794F4].y2 = 240;
        SCREEN_FADE_POLYS[D_800794F4].x3 = 320;
        SCREEN_FADE_POLYS[D_800794F4].y3 = 240;
        AddPrim((s32 *)D_800793A0->ot, (s32)&SCREEN_FADE_POLYS[D_800794F4]);
        AddPrim((s32 *)D_800793A0->ot, (s32)SCREEN_FADE_TPAGES[D_800794F4]);
    }
    SCREEN_FADE_ACTIVE = 0;
}
