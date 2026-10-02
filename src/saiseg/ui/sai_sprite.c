#include "common.h"
#include "game.h"
#include "dcb/sai_sprite.h"
#include "dcb/heap.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "dcb/loader.h"
#include "dcb/archive.h"
#include "dcb/card_render.h"

typedef struct {
    s32 x;
    s32 y;
    s32 clutX;
    u16 clutY;
    u16 padE;
    u8 w;
    u8 h;
    u8 mode;
    u8 pad13;
} SpriteTemplate;

typedef struct {
    s16 type;
    s16 unk2;
    s32 size;
} PackEntry;

SpriteTemplate SAI_SPRITE_TEMPLATES[66] = {
    { 0x180, 0xA8, 0x200, 0xF8, 0, 0x1A, 0x28, 1, 0 },
    { 0x1E8, 0xA8, 0x200, 0xF8, 0, 0x18, 0x10, 1, 0 },
    { 0x180, 0x100, 0x180, 0x1A8, 0, 0xC6, 0xA2, 1, 0 },
    { 0x180, 0x100, 0x280, 0x1F8, 0, 0xFF, 0x80, 1, 0 },
    { 0x180, 0x180, 0x280, 0x1F9, 0, 0xFF, 0x7F, 1, 0 },
    { 0x200, 0x100, 0x280, 0x1FA, 0, 0xFF, 0x80, 1, 0 },
    { 0x2C0, 0xC0, 0x200, 0xF3, 0, 0x80, 0x20, 0, 0 },
    { 0x2E0, 0, 0x200, 0xED, 0, 0x74, 0x18, 0, 0 },
    { 0x2C0, 0, 0x200, 0xF4, 0, 0x50, 0x46, 0, 0 },
    { 0x229, 0, 0x200, 0xE8, 0, 0x10, 0xD, 0, 0 },
    { 0x320, 0x100, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x128, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x150, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x320, 0x178, 0x280, 0x1F7, 0, 0x77, 0x28, 0, 0 },
    { 0x340, 0x100, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x340, 0x128, 0x280, 0x1F7, 0, 0x27, 0x70, 0, 0 },
    { 0x340, 0x198, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x34A, 0x100, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x34A, 0x128, 0x280, 0x1F7, 0, 0x27, 0x70, 0, 0 },
    { 0x34A, 0x198, 0x280, 0x1F7, 0, 0x27, 0x28, 0, 0 },
    { 0x300, 0xA0, 0x200, 0xEF, 0, 0x58, 0x10, 0, 0 },
    { 0x340, 0x78, 0x200, 0xE3, 0, 0x70, 0x6C, 0, 0 },
    { 0x300, 0xB0, 0x200, 0xE3, 0, 0x54, 0x4C, 0, 0 },
    { 0x318, 0x30, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x318, 0x60, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x318, 0x70, 0x200, 0xF1, 0, 0x44, 0x10, 0, 0 },
    { 0x360, 0x78, 0x200, 0xE6, 0, 0x6C, 0x7A, 0, 0 },
    { 0x318, 0xC8, 0x200, 0xEB, 0, 0x40, 0x37, 0, 0 },
    { 0x180, 0, 0x200, 0xF9, 0, 0x40, 0x37, 1, 0 },
    { 0x200, 0x30, 0x200, 0xEA, 0, 0x84, 0x30, 0, 0 },
    { 0x2E0, 0xB4, 0x200, 0xE5, 0, 0x68, 0x1C, 0, 0 },
    { 0x340, 0, 0x200, 0xE4, 0, 0x60, 0x14, 0, 0 },
    { 0x240, 0, 0x200, 0xE5, 0, 0xAF, 0x30, 0, 0 },
    { 0x240, 0x30, 0x200, 0xE5, 0, 0xAF, 0x30, 0, 0 },
    { 0x280, 0, 0x200, 0xE5, 0, 0x27, 0x30, 0, 0 },
    { 0x280, 0x30, 0x200, 0xE5, 0, 0x27, 0x90, 0, 0 },
    { 0x280, 0xC0, 0x200, 0xE5, 0, 0x27, 0x30, 0, 0 },
    { 0x28A, 0, 0x200, 0xE5, 0, 0x23, 0x30, 0, 0 },
    { 0x28A, 0x30, 0x200, 0xE5, 0, 0x23, 0x90, 0, 0 },
    { 0x28A, 0xC0, 0x200, 0xE5, 0, 0x23, 0x30, 0, 0 },
    { 0x240, 0x60, 0x200, 0xE2, 0, 0xAF, 0x30, 0, 0 },
    { 0x240, 0x90, 0x200, 0xE2, 0, 0xAF, 0x34, 0, 0 },
    { 0x298, 0, 0x200, 0xE2, 0, 0x37, 0x30, 0, 0 },
    { 0x298, 0x30, 0x200, 0xE2, 0, 0x37, 0x88, 0, 0 },
    { 0x298, 0xB8, 0x200, 0xE2, 0, 0x37, 0x34, 0, 0 },
    { 0x2A6, 0, 0x200, 0xE2, 0, 0x2B, 0x30, 0, 0 },
    { 0x2A6, 0x30, 0x200, 0xE2, 0, 0x2B, 0x88, 0, 0 },
    { 0x2A6, 0xB8, 0x200, 0xE2, 0, 0x2B, 0x34, 0, 0 },
    { 0x300, 0x100, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x128, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x150, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x300, 0x178, 0x280, 0x1F6, 0, 0x77, 0x28, 0, 0 },
    { 0x358, 0x100, 0x280, 0x1F6, 0, 0x27, 0x28, 0, 0 },
    { 0x358, 0x128, 0x280, 0x1F6, 0, 0x27, 0x78, 0, 0 },
    { 0x358, 0x1A0, 0x280, 0x1F6, 0, 0x27, 0x28, 0, 0 },
    { 0x362, 0x100, 0x280, 0x1F6, 0, 0x23, 0x28, 0, 0 },
    { 0x362, 0x128, 0x280, 0x1F6, 0, 0x23, 0x78, 0, 0 },
    { 0x362, 0x1A0, 0x280, 0x1F6, 0, 0x23, 0x28, 0, 0 },
    { 0x318, 0xA4, 0x200, 0xEB, 0, 0x2A, 0x24, 0, 0 },
    { 0x2C0, 0x82, 0x200, 0xFA, 0, 0x40, 0x38, 1, 0 },
    { 0x218, 0, 0x200, 0xEC, 0, 0x34, 0x30, 0, 0 },
    { 0x318, 0x80, 0x200, 0xF1, 0, 0x40, 0x10, 0, 0 },
    { 0x2E0, 0x80, 0x200, 0xEE, 0, 0x2A, 0x24, 0, 0 },
    { 0x2EB, 0x80, 0x200, 0xEE, 0, 0x2A, 0x24, 0, 0 },
    { 0x22D, 0, 0x200, 0xE8, 0, 0x10, 0xD, 0, 0 },
    { 0x328, 0, 0x210, 0xF2, 0, 0x58, 0x50, 0, 0 },
};

/* the same walk over the pack; the match depends on the form of the loop:
   each version's compiler needs its own to lay it out as the original does */
#if VERSION_US
void SAI_uploadPakTextures(u8 *pack) {
    PackEntry *entry;

    if (pack == NULL) {
        return;
    }
    do {
        entry = (PackEntry *)pack;
        pack += sizeof(PackEntry);
        if (entry->type < 0) {
            break;
        }
        if (entry->type == 5) {
            uploadTexturePack((u32 *)pack);
        }
        pack += entry->size;
    } while (1);
}
#elif VERSION_EU
void SAI_uploadPakTextures(u8 *pack) {
    PackEntry *entry;

    if (pack == NULL) {
        return;
    }
    while (1) {
        entry = (PackEntry *)pack;
        pack += sizeof(PackEntry);
        if (entry->type < 0) {
            break;
        }
        if (entry->type == 5) {
            uploadTexturePack((u32 *)pack);
        }
        pack += entry->size;
    }
}
#else
#error "saiseg/ui/sai_sprite: version not checked"
#endif

void SAI_loadAreaPak(void) {
    char path[0x48];
    s32 area = ((PlayerProfile *)PLAYER_PROFILES)->areaId;

    SESSION->loading = 1;
    sprintf(path, "C:\\area%2.2d.pak", area);
    spawnTask(0, -1, 0, 0x400, loadFileTagged, path, getCurrentTaskId(), 0x31);
    SESSION->pak = (Chunk *)waitFrames(0x7FFFFFFF);
    SAI_uploadPakTextures((u8 *)SESSION->pak);
    truncatePakTextures(SESSION->pak);
    SESSION->script = findPakChunk(SESSION->pak, 2, area + 200);
    SESSION->loading = 0;
}

Sprite3D *SAI_createSprite(s32 id) {
    SpriteTemplate *tmpl = &SAI_SPRITE_TEMPLATES[id];
    s32 abr = 0;
    Sprite3D *sprite = allocHeapBlock(sizeof(Sprite3D), 0x2E);
    POLY_FT4 *quad;
    s32 i;

    sprite->corners[0].vx = sprite->corners[2].vx = -(tmpl->w >> 1);
    sprite->corners[1].vx = sprite->corners[3].vx = tmpl->w >> 1;
    sprite->corners[0].vy = sprite->corners[1].vy = -(tmpl->h >> 1);
    sprite->corners[2].vy = sprite->corners[3].vy = (tmpl->h >> 1) - 1;
    for (i = 0; i < 4; i++) {
        sprite->corners[i].vz = 0;
    }
    sprite->rot.vx = sprite->rot.vy = sprite->rot.vz = 0;
    sprite->pos.vx = sprite->pos.vy = sprite->pos.vz = 0;
    sprite->w = tmpl->w;
    sprite->h = tmpl->h - 1;
    sprite->otz = 0x23;
    for (i = 0; i < 2; i++) {
        quad = &sprite->quads[i];
        SetPolyFT4(quad);
        quad->r0 = 0x80;
        quad->g0 = 0x80;
        quad->b0 = 0x80;
        SetSemiTrans(quad, 0);
        quad->clut = getClut(tmpl->clutX, tmpl->clutY);
        quad->tpage = ((tmpl->mode & 3) << 7) | ((abr & 3) << 5) | ((tmpl->y & 0x100) >> 4) | ((tmpl->x & 0x3C0) >> 6) | ((tmpl->y & 0x200) << 2);
        if (tmpl->mode != 0) {
            quad->u2 = quad->u0 = (tmpl->x % 64) << 1;
            quad->u1 = quad->u3 = ((tmpl->x % 64) << 1) + tmpl->w;
        } else {
            quad->u2 = quad->u0 = (tmpl->x % 64) << 2;
            quad->u1 = quad->u3 = ((tmpl->x % 64) << 2) + tmpl->w;
        }
        quad->v0 = quad->v1 = tmpl->y;
        quad->v2 = quad->v3 = tmpl->y + tmpl->h - 1;
    }
    return sprite;
}

void SAI_freeSprite(void *ptr) {
    freeHeapBlock(ptr);
}

void SAI_drawSprite(Sprite3D *sprite) {
    MATRIX matrix;
    SVECTOR corners[4];
    s32 sxy[4];
    s32 depthCue;
    s32 flag;

    buildRotTransMatrix(&sprite->pos, &sprite->rot, &matrix);
    CompMatrix((MATRIX *)SCENE_3D->viewMatrix, &matrix, &matrix);
    SetRotMatrix((s32)&matrix);
    SetTransMatrix(&matrix);
    corners[0] = sprite->corners[0];
    corners[1] = sprite->corners[1];
    corners[2] = sprite->corners[2];
    corners[3] = sprite->corners[3];
    RotAverage4(&corners[0], &corners[1], &corners[2], &corners[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &depthCue, &flag);
    sprite->quads[FRAME_BUFFER_INDEX].x0 = sxy[0];
    sprite->quads[FRAME_BUFFER_INDEX].y0 = sxy[0] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x1 = sxy[1];
    sprite->quads[FRAME_BUFFER_INDEX].y1 = sxy[1] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x2 = sxy[2];
    sprite->quads[FRAME_BUFFER_INDEX].y2 = sxy[2] >> 16;
    sprite->quads[FRAME_BUFFER_INDEX].x3 = sxy[3];
    sprite->quads[FRAME_BUFFER_INDEX].y3 = sxy[3] >> 16;
    addPrim(&CURRENT_FRAME_BUFFER->ot[sprite->otz], &sprite->quads[FRAME_BUFFER_INDEX]);
}

void SAI_moveSpriteCorners(Sprite3D *sprite, s16 dx, s16 dy) {
    sprite->corners[2].vx += dx;
    sprite->corners[0].vx = sprite->corners[2].vx;
    sprite->corners[3].vx += dx;
    sprite->corners[1].vx = sprite->corners[3].vx;
    sprite->corners[1].vy += dy;
    sprite->corners[0].vy = sprite->corners[1].vy;
    sprite->corners[3].vy += dy;
    sprite->corners[2].vy = sprite->corners[3].vy;
}

void SAI_setSpriteDepth(Sprite3D *sprite, s32 otz) {
    sprite->otz = otz;
}

void SAI_setSpriteBrightness(Unk801EBD94 *quads, u8 value) {
    s32 i;

    for (i = 0; i < 2; i++) {
        quads[i].r = value;
        quads[i].g = value;
        quads[i].b = value;
    }
}

void SAI_setSpriteSize(Sprite3D *sprite, s16 width, s16 height) {
    s32 halfWidth;
    s32 halfHeight;

    halfWidth = width >> 1;
    sprite->corners[2].vx = -halfWidth;
    sprite->corners[0].vx = -halfWidth;
    sprite->corners[3].vx = halfWidth;
    sprite->corners[1].vx = halfWidth;
    halfHeight = height >> 1;
    sprite->corners[1].vy = -halfHeight;
    sprite->corners[0].vy = -halfHeight;
    sprite->corners[3].vy = halfHeight;
    sprite->corners[2].vy = halfHeight;
}

void SAI_setSpriteBlendMode(Sprite3D *sprite, s32 abr) {
    /* clears the tpage's blend rate (bits 5 and 6); the European version's
       match depends on the mask being in a variable, set apart from its use */
    s16 clearAbr = ~(3 << 5);
    s16 tpage;

    if (abr >= 0) {
        SetSemiTrans(&sprite->quads[0], 1);
        SetSemiTrans(&sprite->quads[1], 1);
        tpage = sprite->quads[0].tpage & clearAbr;
        tpage |= (abr & 3) << 5;
        sprite->quads[1].tpage = tpage;
        sprite->quads[0].tpage = tpage;
    } else {
        SetSemiTrans(&sprite->quads[0], 0);
        SetSemiTrans(&sprite->quads[1], 0);
    }
}
