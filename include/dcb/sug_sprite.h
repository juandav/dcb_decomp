#ifndef DCB_SUG_SPRITE_H
#define DCB_SUG_SPRITE_H

#include "game.h"
#include "dcb/sugseg.h"

void SUG_initSprite(Sprite *sprite, s32 key, u16 scaleX, u16 scaleY, s16 x, s16 y, s16 z, s32 otz, s32 useOrigin, s32 subKey);
void SUG_drawSprite(Sprite *sprite, s16 brightness);
void SUG_loadSprite(s32 id, s32 x, s32 y, s32 subKey);
void SUG_initSpriteCache(void);
void SUG_freeSprites(void);

#endif /* DCB_SUG_SPRITE_H */
