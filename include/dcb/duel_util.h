#ifndef DCB_DUEL_UTIL_H
#define DCB_DUEL_UTIL_H

#include "game.h"
#include "dcb/scroll_bg.h"

void waitDuelFrames(s32 frames);
#if VERSION_JP
void isCrossPressedByTurnPlayer(void); /* jp: waits for the press */
#elif VERSION_US || VERSION_EU
s32 isCrossPressedByTurnPlayer(void);
#endif
void waitForCpuDecision(void);
void renderAttackChoiceIcons(void);
void runDuelMessageWindow(void);

#endif /* DCB_DUEL_UTIL_H */
