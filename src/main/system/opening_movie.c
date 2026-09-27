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

void playOpeningMovie(s32 movieMode, s32 parentTask) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, &loadFileToAddress, &PATH_OPENSEG_BIN, OVERLAY_LOAD_ADDR, getCurrentTaskId());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_801DFBAC(&PATH_DIGIMON_MOV);
    func_801E055C(movieMode);
    func_80014A48(parentTask);
}
