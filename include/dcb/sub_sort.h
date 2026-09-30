#ifndef DCB_SUB_SORT_H
#define DCB_SUB_SORT_H

#include "game.h"
#include "dcb/menu.h"

extern Menu SUB_DECK_SORT_MENU;
extern Menu SUB_CARD_SORT_MENU;
extern Menu SUB_CARD_LIST_MENU;
extern u8 SUB_CARD_LIST_COLORS[][4];
extern const char SUB_STR_LV[];
extern const char SUB_STR_TYPE[];
extern const char SUB_FMT_CARD_NUMBER[];
extern const char SUB_FMT_COUNT[];
extern const char SUB_STR_CARDS[];
extern const char SUB_STR_QUESTION_MARK[];

extern void SUB_initCardList(void);
void SUB_drawCardSortMenu(UiWindow *window);
void SUB_drawCardList(UiWindow *window);
void SUB_drawDeckSortMenu(UiWindow *window);

#endif /* DCB_SUB_SORT_H */
