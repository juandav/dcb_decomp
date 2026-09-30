#ifndef DCB_KAW_HUD_H
#define DCB_KAW_HUD_H

#include "game.h"

void KAW_fadeCardSprite(CardSprite *sprite, u8 *to);
void KAW_closeRing(s32 x, s32 y, s32 width);
void KAW_openRing(void);
void KAW_freeCursor(void *ptr);
void KAW_drawPortrait(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut);
void KAW_drawPortraitColored(s32 x, s32 y, s32 u, s32 v, s32 frame, u16 clut, Bytes4 *rgb);
void KAW_showCardLabel(CardSprite *sprite, s32 num);
void KAW_hideCardLabel(CardSprite *sprite);
void KAW_drawCursorAt();

#endif /* DCB_KAW_HUD_H */
