#ifndef DCB_EVO_TEXT_H
#define DCB_EVO_TEXT_H

#include "game.h"
#include "dcb/evoseg.h"

s32 EVO_addTextLine(u8 *src);
void EVO_clearTextLines(EvoText *slot);
void EVO_drawMessageWindow(UiWindow *w);

#endif /* DCB_EVO_TEXT_H */
