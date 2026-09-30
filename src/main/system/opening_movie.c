#include "dcb/opening_movie.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/sound.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/sort.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/overlay_calls.h"

void playOpeningMovie(s32 movieMode, s32 parentTask) {
    waitFrames(2);
    spawnTask(0, -1, 0, 0x1000, &loadFileToAddress, "P:\\openseg.bin", OVERLAY_LOAD_ADDR, getCurrentTaskId());
    waitFrames(0x7FFFFFFF);
    waitFrames(2);
    OPEN_findMovieFile("\\DIGIMON.MOV;1");
    OPEN_playMovie(movieMode);
    resumeTask(parentTask);
}
