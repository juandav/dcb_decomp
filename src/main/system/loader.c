#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/loader.h"
#include "dcb/cd_file.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"

s32 FILE_LOADER_BUSY = 0;

void mountDriveTask(s32 path, s32 parentTask) {
    resumeTask(parentTask, mountDrive(path) == 0 ? 1 : -1);
}

s32 loadFile(s32 path, s32 parentTask) {
    s32 size;
    CdFile *file;
    s32 buf;

    size = 0;
    while (FILE_LOADER_BUSY != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    FILE_LOADER_BUSY = 1;
    file = openDiscFile((s8 *)path, 1);
    buf = 0;
    if (file != 0) {
        size = file->size;
        buf = (s32)allocHeapBlock(size, parentTask);
        if (buf == 0) {
            closeDiscFile(file);
        } else {
            readDiscFile(file, size, (u8 *)buf);
            closeDiscFile(file);
        }
    }
    LOADED_FILE_SIZE = size;
    resumeTask(parentTask, buf);
    FILE_LOADER_BUSY = 0;
    return buf;
}

s32 loadFileTagged(s32 *path, s32 parentTask, s32 heapTag) {
    s32 size;
    CdFile *file;
    s32 buf;

    size = 0;
#if VERSION_EU
    printf("GMload_heap_file2(%s)\n", path);
#endif
    while (FILE_LOADER_BUSY != 0) {
        waitFrames(FRAME_INTERVAL);
    }
    FILE_LOADER_BUSY = 1;
    file = openDiscFile((s8 *)path, 1);
    buf = 0;
    if (file != 0) {
        size = file->size;
        buf = (s32)allocHeapBlock(size, heapTag);
        if (buf == 0) {
            closeDiscFile(file);
        } else {
            readDiscFile(file, size, (u8 *)buf);
            closeDiscFile(file);
        }
    }
#if VERSION_EU
    printf("end\n");
#endif
    LOADED_FILE_SIZE = size;
    resumeTask(parentTask, buf);
    FILE_LOADER_BUSY = 0;
    return buf;
}

void loadFileToAddress(s32 path, s32 *dst, s32 parentTask) {
    s32 file;
    s32 size;

    size = 0;
    if (FILE_LOADER_BUSY != 0) {
        do {
            waitFrames(FRAME_INTERVAL);
        } while (FILE_LOADER_BUSY != 0);
    }
    FILE_LOADER_BUSY = 1;
    file = (s32)openDiscFile((s8 *)path, 1);
    if (file != 0) {
        size = (*(s32 *)((s8 *)file + 0x24));
        readDiscFile(file, size, dst);
        closeDiscFile((s32 *) file);
    }
    LOADED_FILE_SIZE = size;
    resumeTask(parentTask);
    FILE_LOADER_BUSY = 0;
}
