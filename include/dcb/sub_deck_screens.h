#ifndef DCB_SUB_DECK_SCREENS_H
#define DCB_SUB_DECK_SCREENS_H

#include "game.h"

extern const char SUB_STR_DECK_TITLE[];
extern const char SUB_STR_DISABLE[];

extern void SUB_editDeck(PlayerDeck *deck);
void SUB_runCardList(void);
void SUB_runDeckMenu(void);

#endif /* DCB_SUB_DECK_SCREENS_H */
