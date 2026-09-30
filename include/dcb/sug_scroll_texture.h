#ifndef DCB_SUG_SCROLL_TEXTURE_H
#define DCB_SUG_SCROLL_TEXTURE_H

#include "game.h"
#include "dcb/sugseg.h"

ScrollTex *SUG_createScrollTexture(Rect16 *rect, s32 depth, s32 mode, s16 speed);
void SUG_scrollTexture(ScrollTex *tex);
void SUG_freeScrollTexture(void **obj);

#endif /* DCB_SUG_SCROLL_TEXTURE_H */
