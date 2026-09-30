#ifndef DCB_SUG_FADE_RECT_H
#define DCB_SUG_FADE_RECT_H

#include "game.h"
#include "dcb/sugseg.h"

ColorQuad *SUG_createFadeRect(Rect16 *rect, Color *color, Color *color2, u8 blend, s16 step, u8 mode);
s32 SUG_tickFadeRect(ColorQuad *quad);

#endif /* DCB_SUG_FADE_RECT_H */
