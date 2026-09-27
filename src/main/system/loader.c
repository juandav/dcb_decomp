#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/loader.h"
#include "dcb/cd_file.h"
#include "dcb/heap.h"
#include "dcb/main.h"

s32 FILE_LOADER_BUSY = 0;

void mountDriveTask(s32 path, s32 parentTask) {
    func_80014A48(parentTask, mountDrive(path) == 0 ? 1 : -1);
}

s32 loadFile(s32 path, s32 parentTask) {
    s32 size;
    CdFile *file;
    s32 buf;

    size = 0;
    while (FILE_LOADER_BUSY != 0) {
        func_80014C08(FRAME_INTERVAL);
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
    func_80014A48(parentTask, buf);
    FILE_LOADER_BUSY = 0;
    return buf;
}

s32 loadFileTagged(s32 *path, s32 parentTask, s32 heapTag) {
    s32 size;
    CdFile *file;
    s32 buf;

    size = 0;
    while (FILE_LOADER_BUSY != 0) {
        func_80014C08(FRAME_INTERVAL);
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
    LOADED_FILE_SIZE = size;
    func_80014A48(parentTask, buf);
    FILE_LOADER_BUSY = 0;
    return buf;
}

void loadFileToAddress(s32 path, s32 *dst, s32 parentTask) {
    s32 file;
    s32 size;

    size = 0;
    if (FILE_LOADER_BUSY != 0) {
        do {
            func_80014C08(FRAME_INTERVAL);
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
    func_80014A48(parentTask);
    FILE_LOADER_BUSY = 0;
}

void uploadTim(u32 *tim, s16 pixelX, s16 pixelY, s16 clutX, s16 clutY) {
    Rect16 rect;

    OpenTIM(tim);
    ReadTIM(&LOADED_TIM);
    if (pixelX == -1) {
        pixelX = LOADED_TIM.prect->x;
        pixelY = LOADED_TIM.prect->y;
    } else {
        LOADED_TIM.prect->x = pixelX;
        LOADED_TIM.prect->y = pixelY;
    }
    if (clutX == -1) {
        clutX = LOADED_TIM.crect->x;
        clutY = LOADED_TIM.crect->y;
    } else if (clutX != -2) {
        LOADED_TIM.crect->x = clutX;
        LOADED_TIM.crect->y = clutY;
    }
    rect.x = pixelX;
    rect.y = pixelY;
    rect.w = LOADED_TIM.prect->w;
    rect.h = LOADED_TIM.prect->h;
    LoadImage((s16 *)&rect, (s32)LOADED_TIM.paddr);
    if ((LOADED_TIM.mode & 8) && clutX != -2) {
        rect.x = clutX;
        rect.y = clutY;
        rect.w = LOADED_TIM.crect->w;
        rect.h = LOADED_TIM.crect->h;
        LoadImage((s16 *)&rect, (s32)LOADED_TIM.caddr);
    }
}

void uploadTimList(u32 *tims) {
    TIM_IMAGE img;

    OpenTIM(tims);
    while (ReadTIM(&img) != 0) {
        if (img.caddr != 0) {
            LoadImage((s16 *)img.crect, (s32)img.caddr);
        }
        if (img.paddr != 0) {
            LoadImage((s16 *)img.prect, (s32)img.paddr);
        }
    }
    DrawSync(0);
}

void uploadTimListOffset(u32 *tims, s32 dx, s32 dy) {
    TIM_IMAGE img;
    Rect16 rect;

    OpenTIM(tims);
    while (ReadTIM(&img) != 0) {
        if (img.caddr != 0) {
            rect.w = img.crect->w;
            rect.h = img.crect->h;
            rect.x = img.crect->x + dx;
            rect.y = img.crect->y + dy;
            LoadImage((s16 *)&rect, (s32)img.caddr);
        }
        if (img.paddr != 0) {
            rect.w = img.prect->w;
            rect.h = img.prect->h;
            rect.x = img.prect->x + dx;
            rect.y = img.prect->y + dy;
            LoadImage((s16 *)&rect, (s32)img.paddr);
        }
    }
    DrawSync(0);
}

void uploadTexturePack(u32 *pack) {
    u32 *base;
    u32 *image;
    s32 count;

    count = *pack++;
    base = pack;
    if ((count & 0xFFFF) == 0x7054) {
        count >>= 16;
        do {
            image = base + pack[count - 1];
            if (*image++ & 8) {
                LoadImage((s16 *)(image + 1), (s32)(image + 3));
                image += *image >> 2;
            }
            LoadImage((s16 *)(image + 1), (s32)(image + 3));
            DrawSync(0);
        } while (--count > 0);
    }
}

void uploadTexturePackOffset(u32 *pack, s32 dx, s32 dy) {
    u32 *base;
    u32 *image;
    s32 count;

    count = *pack++;
    base = pack;
    if ((count & 0xFFFF) == 0x7054) {
        count >>= 16;
        do {
            image = base + pack[count - 1];
            if (*image++ & 8) {
                ((Rect16 *)(image + 1))->x += dx;
                ((Rect16 *)(image + 1))->y += dy;
                LoadImage((s16 *)(image + 1), (s32)(image + 3));
                image += *image >> 2;
            }
            ((Rect16 *)(image + 1))->x += dx;
            ((Rect16 *)(image + 1))->y += dy;
            LoadImage((s16 *)(image + 1), (s32)(image + 3));
            DrawSync(0);
        } while (--count > 0);
    }
}

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
