#ifndef DCB_PAD_H
#define DCB_PAD_H

#include "game.h"

/* Button bits of PadState's masks: the controller's two data bytes, inverted,
   the d-pad/Start/Select byte in the high half. */
#define PAD_L2 0x0001
#define PAD_R2 0x0002
#define PAD_L1 0x0004
#define PAD_R1 0x0008
#define PAD_TRIANGLE 0x0010
#define PAD_CIRCLE 0x0020
#define PAD_CROSS 0x0040
#define PAD_SQUARE 0x0080
#define PAD_SELECT 0x0100
#define PAD_L3 0x0200
#define PAD_R3 0x0400
#define PAD_START 0x0800
#define PAD_UP 0x1000
#define PAD_RIGHT 0x2000
#define PAD_DOWN 0x4000
#define PAD_LEFT 0x8000

/* PadInitDirect's receive buffers, one per port */
extern u8 PAD_RECEIVE_BUFFERS[2][0x22];

void pollPads(void);
void initPads(void);
void resetPadStates(void);
void setPadRepeatRate(s32 port, s16 repeatDelay, s16 repeatRate);
s32 updatePadState(s32 port, PadState *pad, u8 *rawData);

#endif /* DCB_PAD_H */
