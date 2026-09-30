#ifndef DCB_SAI_WORLD_MAP_H
#define DCB_SAI_WORLD_MAP_H

#include "game.h"
#include "dcb/menu.h"
#include "dcb/saiseg.h"

void SAI_setSpriteImage8Bit(Sprite3D *sprite, Rect16 *rect, s8 flip);
void SAI_setSpriteImage4Bit(Sprite3D *sprite, Rect16 *rect);
void SAI_loadMapTextures(s32 useMap);
void SAI_runCornerIcon(s32 state);
void SAI_initCamera(void);
void SAI_runWorldMap(s32 resume, s32 openMenu);

#endif /* DCB_SAI_WORLD_MAP_H */
