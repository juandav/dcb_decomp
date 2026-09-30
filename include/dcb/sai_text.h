#ifndef DCB_SAI_TEXT_H
#define DCB_SAI_TEXT_H

#include "game.h"
#include "dcb/saiseg.h"

void SAI_drawMessageWindow(UiWindow *window);
void SAI_clearTextLines(TextLine *lines);
s32 SAI_addTextLine(u8 *src);

#endif /* DCB_SAI_TEXT_H */
