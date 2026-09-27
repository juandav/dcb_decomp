#include "dcb/vram_upload.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/loader.h"
#include "dcb/cd_file.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"

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
