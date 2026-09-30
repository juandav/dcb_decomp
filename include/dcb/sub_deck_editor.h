#ifndef DCB_SUB_DECK_EDITOR_H
#define DCB_SUB_DECK_EDITOR_H

#include "game.h"

extern const char SUB_STR_TYPE[];
extern const char SUB_STR_DECK_TITLE[];
extern const char SUB_STR_DISABLE[];

extern void SUB_editDeck(PlayerDeck *deck);
extern void SUB_openCenteredWindow(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style);
void SUB_runDeckMenu(void);

#endif /* DCB_SUB_DECK_EDITOR_H */
