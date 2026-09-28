#ifndef DCB_DUEL_H
#define DCB_DUEL_H

#include "game.h"

#define CUR_CARD (((CardCursor *)DUEL->cursor)->id)
#define CHOICE (((Window *)&D_801D8278)->choice)
#define ME DUEL->turnPlayer
#define OPP ((s8)(DUEL->turnPlayer ^ 1))

extern s8 D_801D83D4;
extern s8 D_801D83D7;
extern s8 D_801D831D;

void runDuelTurnLoop(void);

#endif /* DCB_DUEL_H */
