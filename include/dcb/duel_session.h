#ifndef DCB_DUEL_SESSION_H
#define DCB_DUEL_SESSION_H

#include "game.h"

void initDuelState(s32 isCpuDuel);
void startDuelScene(void);
void spawnDuelTasks(s32 isCpuDuel);
void teardownDuelScene(void);
void renderDuelFrame(void);
void runDuel();

#endif /* DCB_DUEL_SESSION_H */
