#ifndef DCB_DUEL_H
#define DCB_DUEL_H

#include "game.h"

#define CUR_CARD (((CardCursor *)DUEL->cursor)->id)
#define CHOICE (((Window *)&DUEL_DIALOG)->choice)
#define ME DUEL->turnPlayer
#define OPP ((s8)(DUEL->turnPlayer ^ 1))

extern s8 MSG_BAR_NEXT;
extern s8 MSG_BAR_NEXT2;

void runDuelTurnLoop(void);

#endif /* DCB_DUEL_H */
