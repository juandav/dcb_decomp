#ifndef DCB_KAW_HAND_H
#define DCB_KAW_HAND_H

#include "game.h"
#include "dcb/duel_launch.h"
#include "dcb/kawseg.h"

void KAW_drawSprite(s32 x, s32 y, s32 u, s32 v, s32 w, s32 h, s32 clutX, s32 clutY, s32 tp, s32 semi, s32 abr, s32 brightness,
                   s32 otz);

void KAW_drawCard3D(Icon3D *icon, s32 z, RawPolyFT4 *pk);
void KAW_waitForCross(void);

#endif /* DCB_KAW_HAND_H */
