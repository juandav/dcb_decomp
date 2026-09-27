#ifndef DCB_DUEL_UTIL_H
#define DCB_DUEL_UTIL_H

#include "game.h"
#include "dcb/scroll_bg.h"

void waitDuelFrames(s32 frames);
s32 func_80033D9C(void);
void waitForCpuDecision(void);
void renderAttackChoiceIcons(void);
void runDuelMessageWindow(void);

#endif /* DCB_DUEL_UTIL_H */
