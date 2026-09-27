#ifndef DCB_LOADER_H
#define DCB_LOADER_H

#include "game.h"

extern s32 FILE_LOADER_BUSY;

void mountDriveTask(s32 path, s32 parentTask);
s32 loadFileTagged(s32 *path, s32 parentTask, s32 heapTag);
void loadFileToAddress();
s32 loadFile();

#endif /* DCB_LOADER_H */
