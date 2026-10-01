#include "common.h"
#include "game.h"
#include "dcb/sug_screen_copy.h"
#include "dcb/vblank.h"
#include "dcb/sug_battle.h"

extern s32 SUG_SCREEN_FX_X;
extern float SUG_SCREEN_FX_STEP_X;
extern float SUG_SCREEN_FX_STEP_Y;
extern s32 SUG_SCREEN_FX_FRAME;
extern u8 SUG_SCREEN_FX_BASE_RGB[3];
extern u8 SUG_SCREEN_FX_RGB_STEP[3];
extern s32 SUG_SCREEN_FX_Y;
extern s32 SUG_SCREEN_FX_MOTION;
extern s32 SUG_SCREEN_FX_HOLD;
extern s32 SUG_SCREEN_FX_FADE_TIME;
extern s32 VBLANKS_PER_FRAME;

s16 SUG_SCREEN_FX_ANGLE = 0;

void SUG_moveScreenCopy(s32 frame) {
    s32 radius;

    switch (SUG_SCREEN_FX_MOTION) {
    case 0:
        SCREEN_COPY_EFFECT.x = SUG_SCREEN_FX_X + SUG_SCREEN_FX_STEP_X * frame;
        SCREEN_COPY_EFFECT.y = SUG_SCREEN_FX_Y + SUG_SCREEN_FX_STEP_Y * frame;
        break;
    case 1:
        radius = SUG_SCREEN_FX_X + SUG_SCREEN_FX_STEP_X * frame;
        SCREEN_COPY_EFFECT.x = rcos(SUG_SCREEN_FX_Y) * radius / 4096;
        SCREEN_COPY_EFFECT.y = rsin(SUG_SCREEN_FX_Y) * radius / 4096;
        SUG_SCREEN_FX_Y = SUG_SCREEN_FX_Y + SUG_SCREEN_FX_STEP_Y;
        break;
    case 2:
        SCREEN_COPY_EFFECT.x = rand() % SUG_SCREEN_FX_X - SUG_SCREEN_FX_X / 2;
        SCREEN_COPY_EFFECT.y = rand() % SUG_SCREEN_FX_Y - SUG_SCREEN_FX_Y / 2;
        break;
    }
}

void SUG_startScreenCopyFade(u8 r, u8 g, u8 b, s32 x, s32 y) {
    SUG_SCREEN_FX_BASE_RGB[0] = SCREEN_COPY_EFFECT.r;
    SUG_SCREEN_FX_BASE_RGB[1] = SCREEN_COPY_EFFECT.g;
    SUG_SCREEN_FX_BASE_RGB[2] = SCREEN_COPY_EFFECT.b;
    SUG_SCREEN_FX_RGB_STEP[0] = (r - SCREEN_COPY_EFFECT.r) / SUG_SCREEN_FX_FADE_TIME;
    SUG_SCREEN_FX_RGB_STEP[1] = (g - SCREEN_COPY_EFFECT.g) / SUG_SCREEN_FX_FADE_TIME;
    SUG_SCREEN_FX_RGB_STEP[2] = (b - SCREEN_COPY_EFFECT.b) / SUG_SCREEN_FX_FADE_TIME;
    switch (SUG_SCREEN_FX_MOTION) {
    case 0:
        SUG_SCREEN_FX_STEP_X = (float)(x - (SUG_SCREEN_FX_X = SCREEN_COPY_EFFECT.x)) / SUG_SCREEN_FX_FADE_TIME;
        SUG_SCREEN_FX_STEP_Y = (float)(y - (SUG_SCREEN_FX_Y = SCREEN_COPY_EFFECT.y)) / SUG_SCREEN_FX_FADE_TIME;
        break;
    case 1:
        SUG_SCREEN_FX_STEP_X = (float)(x - (SUG_SCREEN_FX_X = SCREEN_COPY_EFFECT.x)) / SUG_SCREEN_FX_FADE_TIME;
        if ((x | y) != 0) {
            SUG_SCREEN_FX_STEP_Y = y;
        }
        break;
    case 2:
        if ((x | y) != 0) {
            SUG_SCREEN_FX_X = x;
            SUG_SCREEN_FX_Y = y;
        }
        break;
    }
    SUG_SCREEN_FX_FRAME = 0;
}

void SUG_stepScreenCopyFade(void) {
    SCREEN_COPY_EFFECT.r = SUG_SCREEN_FX_BASE_RGB[0] + SUG_SCREEN_FX_RGB_STEP[0] * SUG_SCREEN_FX_FRAME;
    SCREEN_COPY_EFFECT.g = SUG_SCREEN_FX_BASE_RGB[1] + SUG_SCREEN_FX_RGB_STEP[1] * SUG_SCREEN_FX_FRAME;
    SCREEN_COPY_EFFECT.b = SUG_SCREEN_FX_BASE_RGB[2] + SUG_SCREEN_FX_RGB_STEP[2] * SUG_SCREEN_FX_FRAME;
    SUG_moveScreenCopy(SUG_SCREEN_FX_FRAME);
    SUG_SCREEN_FX_FRAME++;
}

void SUG_startScreenCopyEffect(EffectParams *params, s32 clearColor) {
    SUG_SCREEN_FX_FRAME = 0;
    SUG_SCREEN_FX_ANGLE = SUG_SCREEN_FX_Y = 0;
    SUG_SCREEN_FX_MOTION = params->mode;
    SUG_SCREEN_FX_PHASE = 1;
    SUG_SCREEN_FX_HOLD = params->hold;
    SUG_SCREEN_FX_FADE_TIME = params->rows;
    SCREEN_COPY_EFFECT.mode = 1;
    if (clearColor) {
        SCREEN_COPY_EFFECT.r = 0;
        SCREEN_COPY_EFFECT.g = 0;
        SCREEN_COPY_EFFECT.b = 0;
    }
    SUG_startScreenCopyFade(params->r0, params->g0, params->b0, params->x, params->y);
}

void SUG_tickScreenCopyEffect(void) {
    if (SCREEN_COPY_MODE == 0) {
        return;
    }
    switch (SUG_SCREEN_FX_PHASE) {
    case 1:
        if (SUG_SCREEN_FX_FRAME < SUG_SCREEN_FX_FADE_TIME) {
            SUG_stepScreenCopyFade();
            return;
        }
        /* read as a signed halfword here, unlike main */
        if ((s16)SCREEN_COPY_EFFECT.abr != 0) {
            SUG_startScreenCopyFade(0, 0, 0, 0, 0);
        } else {
            SUG_startScreenCopyFade(0xA8, 0xA8, 0xA8, 0, 0);
        }
        SUG_SCREEN_FX_PHASE++;
        /* fallthrough */
    case 2:
        if (++SUG_SCREEN_FX_FRAME < SUG_SCREEN_FX_HOLD) {
            if (SUG_SCREEN_FX_MOTION != 0) {
                SUG_moveScreenCopy(0);
            }
            return;
        }
        SUG_SCREEN_FX_FRAME = 0;
        SUG_SCREEN_FX_PHASE++;
        /* fallthrough */
    case 3:
        if (SUG_SCREEN_FX_FRAME < SUG_SCREEN_FX_FADE_TIME) {
            SUG_stepScreenCopyFade();
            return;
        }
        SCREEN_COPY_EFFECT.abr = SUG_SCREEN_FX_FRAME = SUG_SCREEN_FX_PHASE = SCREEN_COPY_EFFECT.x = SCREEN_COPY_EFFECT.y = SCREEN_COPY_EFFECT.mode = 0;
        SCREEN_COPY_EFFECT.r = 0xA8;
        SCREEN_COPY_EFFECT.g = 0xA8;
        SCREEN_COPY_EFFECT.b = 0xA8;
        break;
    }
}

void SUG_updateScreenCopyQuads(void) {
    switch (SCREEN_COPY_MODE) {
    case 2:
        SUG_SCREEN_FX_ANGLE = 0;
        SCREEN_COPY_EFFECT.px[0][1] = SCREEN_COPY_EFFECT.px[0][3] = SCREEN_COPY_EFFECT.px[1][0] = SCREEN_COPY_EFFECT.px[1][2] = SCREEN_COPY_EFFECT.x + 0xA0;
        if (SCREEN_COPY_EFFECT.x < 0) {
            SCREEN_COPY_EFFECT.px[0][0] = SCREEN_COPY_EFFECT.px[0][2] = SCREEN_COPY_EFFECT.x;
            SCREEN_COPY_EFFECT.px[1][1] = SCREEN_COPY_EFFECT.px[1][3] = 0x140;
        } else {
            SCREEN_COPY_EFFECT.px[0][0] = SCREEN_COPY_EFFECT.px[0][2] = 0;
            SCREEN_COPY_EFFECT.px[1][1] = SCREEN_COPY_EFFECT.px[1][3] = SCREEN_COPY_EFFECT.x + 0x140;
        }
        if (SCREEN_COPY_EFFECT.y < 0) {
            SCREEN_COPY_EFFECT.py[0][0] = SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = SCREEN_COPY_EFFECT.py[1][1] = SCREEN_COPY_EFFECT.y;
            SCREEN_COPY_EFFECT.py[0][2] = SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = SCREEN_COPY_EFFECT.py[1][3] = 0xF0;
        } else {
            SCREEN_COPY_EFFECT.py[0][0] = SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = SCREEN_COPY_EFFECT.py[1][1] = 0;
            SCREEN_COPY_EFFECT.py[0][2] = SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = SCREEN_COPY_EFFECT.py[1][3] = SCREEN_COPY_EFFECT.y + 0xF0;
        }
        break;
    case 3:
        SCREEN_COPY_EFFECT.px[0][1] = SCREEN_COPY_EFFECT.px[0][3] = SCREEN_COPY_EFFECT.px[1][0] = SCREEN_COPY_EFFECT.px[1][2] = 0xA0;
        SCREEN_COPY_EFFECT.px[0][0] = SCREEN_COPY_EFFECT.px[0][2] = -SCREEN_COPY_EFFECT.x;
        SCREEN_COPY_EFFECT.px[1][1] = SCREEN_COPY_EFFECT.px[1][3] = SCREEN_COPY_EFFECT.x + 0x140;
        SCREEN_COPY_EFFECT.py[0][0] = SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = SCREEN_COPY_EFFECT.py[1][1] = -SCREEN_COPY_EFFECT.y;
        SCREEN_COPY_EFFECT.py[0][2] = SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = SCREEN_COPY_EFFECT.py[1][3] = SCREEN_COPY_EFFECT.y + 0xF0;
        break;
    case 4:
        SCREEN_COPY_EFFECT.px[0][1] = SCREEN_COPY_EFFECT.px[1][0] = (0 * rcos(SUG_SCREEN_FX_ANGLE) - (-SCREEN_COPY_EFFECT.x - 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[0][3] = SCREEN_COPY_EFFECT.px[1][2] = (0 * rcos(SUG_SCREEN_FX_ANGLE) - (SCREEN_COPY_EFFECT.x + 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[0][0] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (-SCREEN_COPY_EFFECT.x - 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[0][2] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (SCREEN_COPY_EFFECT.x + 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[1][1] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (-SCREEN_COPY_EFFECT.x - 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.px[1][3] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rcos(SUG_SCREEN_FX_ANGLE) - (SCREEN_COPY_EFFECT.x + 0x78) * rsin(SUG_SCREEN_FX_ANGLE)) / 4096 + 0xA0;
        SCREEN_COPY_EFFECT.py[0][0] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (-SCREEN_COPY_EFFECT.x - 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[0][1] = SCREEN_COPY_EFFECT.py[1][0] = (0 * rsin(SUG_SCREEN_FX_ANGLE) + (-SCREEN_COPY_EFFECT.x - 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[1][1] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (-SCREEN_COPY_EFFECT.x - 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[0][2] = ((-SCREEN_COPY_EFFECT.x - 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (SCREEN_COPY_EFFECT.x + 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[0][3] = SCREEN_COPY_EFFECT.py[1][2] = (0 * rsin(SUG_SCREEN_FX_ANGLE) + (SCREEN_COPY_EFFECT.x + 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SCREEN_COPY_EFFECT.py[1][3] = ((SCREEN_COPY_EFFECT.x + 0xA0) * rsin(SUG_SCREEN_FX_ANGLE) + (SCREEN_COPY_EFFECT.x + 0x78) * rcos(SUG_SCREEN_FX_ANGLE)) / 4096 + 0x78;
        SUG_SCREEN_FX_ANGLE += SCREEN_COPY_EFFECT.y * VBLANKS_PER_FRAME;
        break;
    }
}
