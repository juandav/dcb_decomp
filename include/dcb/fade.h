#ifndef DCB_FADE_H
#define DCB_FADE_H

#include "game.h"

extern s32 SCREEN_FADE_ACTIVE;
extern s32 SCREEN_FADE_LEVEL;
extern s32 SCREEN_FADE_DIRECTION;
extern s32 SCREEN_FADE_BLEND_MODE;
extern s32 SCREEN_FADE_SPEED;
extern FadePoly SCREEN_FADE_POLYS[2];
extern u32 SCREEN_FADE_TPAGES[2][2];

void initScreenFade(void);
void stopScreenFade(void);
s32 isScreenFadeActive(void);
void setScreenFadeParams(s32 fadeIn, s32 blendMode, s32 speed);
void screenFadeTask(s32 fadeIn, s32 blendMode, s32 speed);

#endif /* DCB_FADE_H */
