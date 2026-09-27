#ifndef DCB_VRAM_UPLOAD_H
#define DCB_VRAM_UPLOAD_H

#include "game.h"
#include "dcb/loader.h"

void uploadTimList(u32 *tims);
void uploadTimListOffset(u32 *tims, s32 dx, s32 dy);
void uploadTexturePack(u32 *pack);
void uploadTexturePackOffset(u32 *pack, s32 dx, s32 dy);
void uploadTim(u32 *tim, s16 pixelX, s16 pixelY, s16 clutX, s16 clutY);

#endif /* DCB_VRAM_UPLOAD_H */
