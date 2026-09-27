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
        func_80014C08(FRAME_INTERVAL);
    }
    SCREEN_FADE_DIRECTION = fadeIn;
    SCREEN_FADE_BLEND_MODE = blendMode;
    SCREEN_FADE_SPEED = speed;
    SCREEN_FADE_ACTIVE = 1;
    SCREEN_FADE_LEVEL = fadeIn * 0xFF;
    while (SCREEN_FADE_ACTIVE != 0) {
        func_80014C08(FRAME_INTERVAL);
        if (SCREEN_FADE_DIRECTION != 0) {
            if ((SCREEN_FADE_LEVEL -= SCREEN_FADE_SPEED) < 0) {
                SCREEN_FADE_LEVEL = 0;
                break;
            }
        } else if ((SCREEN_FADE_LEVEL += SCREEN_FADE_SPEED) >= 0x100) {
            SCREEN_FADE_LEVEL = 0xFF;
        }
        SetDrawTPage(SCREEN_FADE_TPAGES[FRAME_BUFFER_INDEX], 0, 0, GetTPage(0, SCREEN_FADE_BLEND_MODE, 0, 0));
        initPrimByType(8, &SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX], 1, 0);
        setPrimRgb0(&SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX], (u8)SCREEN_FADE_LEVEL, (u8)SCREEN_FADE_LEVEL, (u8)SCREEN_FADE_LEVEL);
        SetSemiTrans(&SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX], 1);
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].x0 = 0;
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].y0 = 0;
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].x1 = 320;
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].y1 = 0;
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].x2 = 0;
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].y2 = 240;
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].x3 = 320;
        SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX].y3 = 240;
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)&SCREEN_FADE_POLYS[FRAME_BUFFER_INDEX]);
        AddPrim((s32 *)CURRENT_FRAME_BUFFER->ot, (s32)SCREEN_FADE_TPAGES[FRAME_BUFFER_INDEX]);
    }
    SCREEN_FADE_ACTIVE = 0;
}
