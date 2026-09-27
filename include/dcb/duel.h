#ifndef DCB_DUEL_H
#define DCB_DUEL_H

#include "game.h"

#define CUR_CARD (((CardCursor *)DUEL->unk58)->id)
#define CHOICE (((Window *)&D_801D8278)->choice)
#define ME DUEL->unk817
#define OPP ((s8)(DUEL->unk817 ^ 1))

extern s8 D_801D83D4;
extern s8 D_801D83D7;
extern s8 D_801D831D;

void runCpuDecisionTask();
void runDuelTurnLoop(void);

#endif /* DCB_DUEL_H */
