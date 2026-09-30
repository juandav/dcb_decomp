#ifndef DCB_SUB_DECK_EDITOR_H
#define DCB_SUB_DECK_EDITOR_H

#include "game.h"

extern void SUB_drawSprite(s16 x, s16 y, s16 clut, s32 u, s32 v, s16 w, s16 h, s8 tp, u8 brightness, s8 abr, s32 otIndex);
extern void SUB_drawCardIcon(s16 cardId, s16 x, s16 y, u8 brightness, s32 otIndex);
extern s32 SUB_findCachedCardImage(s16 id);
extern void SUB_openCenteredWindow(UiWindow *window, Rect16 area, s32 label, s32 flags, s32 style);

#endif /* DCB_SUB_DECK_EDITOR_H */
