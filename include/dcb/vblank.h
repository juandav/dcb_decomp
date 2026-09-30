#ifndef DCB_VBLANK_H
#define DCB_VBLANK_H

#include "game.h"

extern s32 VBLANK_COUNTER;
extern Screen SCREEN_COPY_EFFECT;
extern u8 SCREEN_COPY_MODE;
extern s32 RENDER_CALLBACKS_ENABLED;

void tickVblankCounters(void);

#endif /* DCB_VBLANK_H */
