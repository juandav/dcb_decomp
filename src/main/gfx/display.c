#include "dcb/display.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/loader.h"
#include "dcb/vram_upload.h"
#include "dcb/cd_file.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"

void resetDisplay(s32 w, s32 h, s32 interlace) {
    FRAME_CALLBACKS = 0;
    initDisplayBuffers(w, h, interlace);
}

void initDisplayBuffers(s32 w, s32 h, s32 interlace) {
    s32 i;

    setTaskVsyncMode(interlace);
    for (i = 0; i < 2; i++) {
        if (h > 240) {
            SetDefDrawEnv(&DB(i).draw, 0, 0, w, h);
            SetDefDispEnv(&DB(i).disp, 0, 0, w, h);
            DB(i).disp.isinter = 1;
        } else {
            if (interlace == 0) {
                SetDefDrawEnv(&DB(i).draw, 0, i * 256, w, h);
                SetDefDispEnv(&DB(i).disp, 0, 256 - i * 256, w, h);
            } else {
                SetDefDrawEnv(&DB(i).draw, 0, i * 240, w, h);
                SetDefDispEnv(&DB(i).disp, 0, 240 - i * 240, w, h);
            }
            DB(i).disp.isinter = 0;
        }
        DB(i).draw.dtd = 0;
        DB(i).draw.dfe = 0;
        DB(i).draw.isbg = interlace ^ 1;
        DB(i).draw.tpage = GetTPage(0, 0, 0, 0);
        setRGB0(&DB(i).draw, 0, 0, 0);
        DB(i).disp.isrgb24 = interlace;
    }
    SCENE_3D_ENABLED = 0;
}
