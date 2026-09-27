#ifndef DCB_SYSTEM_H
#define DCB_SYSTEM_H

#include "game.h"

extern s32 VBLANK_COUNTER;
extern Screen SCREEN_COPY_EFFECT;
extern u8 D_800794E7;
extern s32 RENDER_CALLBACKS_ENABLED;

void tickVblankCounters(void);
void initScreenCopyEffect(void);
void renderScreenCopyEffect(void);

#endif /* DCB_SYSTEM_H */
