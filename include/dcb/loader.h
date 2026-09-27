#ifndef DCB_LOADER_H
#define DCB_LOADER_H

#include "game.h"

extern s32 FILE_LOADER_BUSY;

void mountDriveTask(s32 path, s32 parentTask);
void uploadTimList(u32 *tims);
void uploadTimListOffset(u32 *tims, s32 dx, s32 dy);
void uploadTexturePack(u32 *pack);
void uploadTexturePackOffset(u32 *pack, s32 dx, s32 dy);
s32 loadFileTagged(s32 *path, s32 parentTask, s32 heapTag);
void loadFileToAddress();
void uploadTim(u32 *tim, s16 pixelX, s16 pixelY, s16 clutX, s16 clutY);
s32 loadFile();

#endif /* DCB_LOADER_H */
