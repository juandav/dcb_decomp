#ifndef DCB_SAI_SPRITE_H
#define DCB_SAI_SPRITE_H

#include "game.h"
#include "dcb/saiseg.h"

void SAI_drawSprite(Sprite3D *sprite);
Sprite3D *SAI_createSprite(s32 id);
void SAI_setSpriteSize(Sprite3D *sprite, s16 width, s16 height);
void SAI_setSpriteBlendMode(Sprite3D *sprite, s32 abr);
void SAI_setSpriteDepth(Sprite3D *sprite, s32 otz);
void SAI_freeSprite(void *ptr);
void SAI_setSpriteBrightness(Unk801EBD94 *quads, u8 value);
void SAI_loadAreaPak(void);
void SAI_moveSpriteCorners(Sprite3D *sprite, s16 dx, s16 dy);

#endif /* DCB_SAI_SPRITE_H */
