#ifndef DCB_EVO_CARD_LIST_H
#define DCB_EVO_CARD_LIST_H

#include "game.h"
#include "dcb/menu.h"

extern Menu EVO_CARD_LIST_MENU;
extern Menu EVO_SORT_MENU;
extern Rect16 EVO_RANK_UP_RECT;
extern const char EVO_FMT_CARD_NUMBER[];

/* defined after the table: GCC emitted the table's strings in reverse order */
extern const char EVO_STR_NUMBER[];

void EVO_initCardList(void);
void EVO_drawSortMenu(UiWindow *w);
void EVO_loadUnitTextures(void);
void EVO_initFusionScene(void);
void EVO_countSpareCards(void);
void EVO_drawCardList(UiWindow *w);

#endif /* DCB_EVO_CARD_LIST_H */
