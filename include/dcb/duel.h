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
#if VERSION_JP
/* jp's turn loops (duel_jp.c, tutorial_jp.c) */
#define PAD_PRESSED(p) (PAD_STATES[p]->rawPressed)
#define HAND_CARD(p, slot) (PLAYER_CARDS(PLAYER(p))[PLAYER(p)->hand[slot] % 30])
#define CARD_OF(p, i) (PLAYER_CARDS(PLAYER(p))[(i) % 30])
/* the HUD panels the card select moves out of the way */
#define CLOSE_CARD_SELECT(p)                                                                             \
    DUEL->cursorSlot = -1;                                                                               \
    PLAYER_PANEL(p, 4)->state = 4;                                                                       \
    PLAYER_PANEL(p, 5)->state = 4;                                                                       \
    if (PLAYER_PANEL(p, 6)->state < 4) {                                                                 \
        PLAYER_PANEL(p, 6)->state = 4;                                                                   \
    }                                                                                                    \
    if (PLAYER_PANEL(p, 7)->state < 4) {                                                                 \
        PLAYER_PANEL(p, 7)->state = 4;                                                                   \
    }

void runTutorialTurnLoop(void);
#endif

#endif /* DCB_DUEL_H */
