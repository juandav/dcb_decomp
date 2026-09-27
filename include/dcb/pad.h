#ifndef DCB_PAD_H
#define DCB_PAD_H

#include "game.h"

extern s32 PAD_RECEIVE_BUFFERS;

void pollPads(void);
void initPads(void);
void resetPadStates(void);
void setPadRepeatRate(s32 port, s16 repeatDelay, s16 repeatRate);
s32 updatePadState(s32 port, PadState *pad, u8 *rawData);

#endif /* DCB_PAD_H */
