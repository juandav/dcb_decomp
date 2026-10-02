#include "common.h"
#include "game.h"
#include "dcb/sug_tex_anim.h"
#include "dcb/heap.h"
#include "dcb/archive.h"
#include "dcb/loader.h"
#include "dcb/task.h"

/* A TAM header's loop byte: in us and eu, the frame to loop back to in bits
   1-7 (0x7F: stay on the last one) and in bit 0 whether the frames are moved
   in VRAM rather than scrolled through the UVs; jp's is the loop frame, and
   moves the frames when it isn't 0 */
#if VERSION_JP
#define TAM_LOOP_FRAME(header) ((header)->loop)
#define TAM_MOVES_FRAMES(header) ((header)->loop != 0)
#elif VERSION_US || VERSION_EU
#define TAM_LOOP_FRAME(header) ((header)->loop >> 1)
#define TAM_MOVES_FRAMES(header) ((header)->loop & 1)
#else
#error "untested version"
#endif

extern TamEntry SUG_TAM_CACHE[8];

s32 LoadImage2(Rect16 *rect, u8 *pixels);
s32 MoveImage2(Rect16 *rect, s32 x, s32 y);
void SUG_scrollTexAnimLeft(TexAnim *image);
void SUG_scrollTexAnimRight(TexAnim *image);
void SUG_scrollTexAnimUp(TexAnim *image);
void SUG_scrollTexAnimDown(TexAnim *image);

const Rect16 SUG_MODEL_CLUT_RECT = { 0x30, 0x70, 0x10, 0x10 };

void SUG_initTamCache(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        SUG_TAM_CACHE[i].key = -1;
        SUG_TAM_CACHE[i].data = NULL;
    }
}

void *SUG_loadTamFile(s32 key, s32 *path, s32 sub, Chunk *pak) {
    void *data;
    s32 free;
    s32 i;

    free = -1;
    for (i = 0; i < 8; i++) {
        if (SUG_TAM_CACHE[i].key == key) {
            return SUG_TAM_CACHE[i].data;
        }
        if (free < 0 && SUG_TAM_CACHE[i].key < 0) {
            free = i;
        }
    }
    if (free >= 0) {
        data = findPakChunk(pak, 4, sub);
        if (data == NULL) {
            data = (void *)loadFileTagged(path, getCurrentTaskId(), 0x82);
            if (data != NULL) {
                SUG_TAM_CACHE[free].key = key;
                SUG_TAM_CACHE[free].data = data;
            }
        }
        return data;
    }
    return NULL;
}

s32 SUG_startTexAnim(s32 id, s32 kind, RingEffect *owner, TexAnim *anim, s32 pak) {
    Rect16 *uv;
    s32 slot;
    s32 y;
    s32 dx;
    s32 sub;
    char path[32];
    s16 n;
    ModelData *model;
    RingEffect *ring;
    SphereEffect *other;

    uv = NULL;
    slot = 0;
    /* the match depends on the extra block, for the register allocation */
    do {
        y = 0;
        dx = 0;
    } while (0);
    sub = id;
    switch (kind) {
    case 0:
        model = (ModelData *)owner;
        if (model != NULL) {
            slot = model->tpageOffset / 0x10000 + 5;
            if (model->id > 1000) {
                n = model->id / 10;
#if VERSION_JP
                sprintf(path, "M:\\HDF%03d\\%03d_%d.tam", n, n, id);
                if ((u32)((u16)model->id - 4000) >= 1000) {
                    y = 0x80;
                }
#elif VERSION_US || VERSION_EU
                sprintf(path, "M:\\HDF%d\\%d_%d.tam", n, n, id);
                if (((ModelEffect *)model->owner)->clutBank == 0) {
                    y = 0x80;
                }
#else
#error "untested version"
#endif
                sub = model->id;
            } else {
                sprintf(path, "M:\\HDF%03d\\%d.tam", model->id, id);
            }
            id |= model->id << 8;
        }
        break;
    case 1:
        ring = owner;
        if (ring->type == 0xD) {
            uv = (Rect16 *)&ring->texCoords;
            y = (u8)uv->y;
            dx = uv->x / 4;
            slot = ring->tpage;
            sprintf(path, "E:\\ANM\\%d_%d.tam", id / 10, id % 10);
        }
        break;
    case 3:
        other = (SphereEffect *)owner;
        if (other->kind == 0xD) {
            uv = &other->uv;
            y = (u8)uv->y;
            dx = uv->x / 4;
            slot = other->tpage;
            sprintf(path, "E:\\ANM\\%d_%d.tam", id / 10, id % 10);
        }
        break;
#if VERSION_US || VERSION_EU
    case 4:
        if (((TrailEffect *)owner)->primKind == 12 || ((TrailEffect *)owner)->primKind == 13) {
            uv = &((TrailEffect *)owner)->uv;
            y = (u8)uv->y;
            slot = ((TrailEffect *)owner)->tpage;
            sprintf(path, "E:\\ANM\\%d_%d.tam", id / 10, id % 10);
        }
        break;
#elif VERSION_JP
#else
#error "sugseg/effect/sug_tex_anim: version not checked"
#endif
    }
    if (slot != 0) {
        anim->timer = 0;
        anim->frame = 0;
        anim->rect.x = (slot & 0xF) << 6;
        anim->rect.y = ((slot >> 4) << 8) + y;
        if (id >= 4) {
            anim->header = SUG_loadTamFile(id, (s32 *)path, sub, (Chunk *)pak);
            if (anim->header == NULL) {
                return 0;
            }
            anim->frames = (AnimFrame *)(anim->header + 1);
            anim->rect.w = anim->header->w;
            anim->rect.h = anim->header->h;
            anim->pixels = NULL;
        } else {
            anim->rect.w = (uv->w + 1) / 4;
            anim->rect.h = uv->h + 1;
            anim->pixels = allocTaskHeapBlock(anim->rect.w * 2 * anim->rect.h + 4);
            StoreImage(&anim->rect, anim->pixels);
            DrawSync(0);
        }
        anim->dst = (s16 *)uv;
        anim->type = id;
        if (!TAM_MOVES_FRAMES(anim->header)) {
            anim->rect.x += dx;
        }
        return (s32)anim;
    }
    return 0;
}

void SUG_tickTexAnim(TexAnim *anim) {
    Rect16 rect;

    anim->timer++;
    switch (anim->type) {
    case 0:
        SUG_scrollTexAnimLeft(anim);
        return;
    case 1:
        SUG_scrollTexAnimRight(anim);
        return;
    case 2:
        SUG_scrollTexAnimUp(anim);
        return;
    case 3:
        SUG_scrollTexAnimDown(anim);
        return;
    }
    if (anim->timer < anim->frames[anim->frame].duration) {
        return;
    }
    anim->timer = 0;
    anim->frame++;
    if (anim->frame >= anim->header->count) {
        anim->frame = TAM_LOOP_FRAME(anim->header);
#if VERSION_US || VERSION_EU
        if (anim->frame == 0x7F) {
            anim->frame--;
        }
#elif VERSION_JP
#else
#error "sugseg/effect/sug_tex_anim: version not checked"
#endif
    }
    if (!TAM_MOVES_FRAMES(anim->header)) {
        anim->dst[0] = anim->frames[anim->frame].u + ((anim->rect.x * 4) & 0xFF);
        anim->dst[1] = (anim->rect.y & 0xFF) + anim->frames[anim->frame].v;
    } else {
        rect.x = anim->frames[anim->frame].u + anim->rect.x;
        rect.y = anim->frames[anim->frame].v + anim->rect.y;
        rect.w = anim->rect.w;
        rect.h = anim->rect.h;
        MoveImage2(&rect, anim->rect.x + anim->frames[0].u, anim->rect.y + anim->frames[0].v);
        DrawSync(0);
    }
}

void SUG_freeTamCache(void) {
    s32 i;

    for (i = 0; i < 8; i++) {
        if (SUG_TAM_CACHE[i].key != -1) {
            freeHeapBlock(SUG_TAM_CACHE[i].data);
            SUG_TAM_CACHE[i].key = -1;
            SUG_TAM_CACHE[i].data = NULL;
        }
    }
}

void SUG_freeTexAnim(TexAnim *anim) {
    void *pixels = anim->pixels;

    if (pixels != NULL) {
        freeHeapBlock(pixels);
    }
}

void SUG_scrollTexAnimLeft(TexAnim *image) {
    TexAnim *self;
    u8 *row;
    u8 *p;
    s8 first;
    s32 x;
    s32 y;

    self = image;
    row = image->pixels;
    for (y = 0; y < image->rect.h; y++, row += image->rect.w * 2) {
        first = row[0];
        p = row;
        for (x = 0; x < image->rect.w * 2 - 1; x++) {
            p[0] = p[1];
            p++;
        }
        *p = first;
    }
    LoadImage2(&image->rect, self->pixels);
    DrawSync(0);
}

void SUG_scrollTexAnimRight(TexAnim *image) {
    TexAnim *self;
    u8 *row;
    u8 *p;
    s8 first;
    s32 x;
    s32 y;

    self = image;
    y = 0;
    row = image->pixels;
    for (; y < image->rect.h; y++, row += image->rect.w * 2) {
        p = row + image->rect.w * 2 - 1;
        first = *p;
        for (x = 0; x < image->rect.w * 2 - 1; x++) {
            p[0] = p[-1];
            p--;
        }
        *p = first;
    }
    LoadImage2(&image->rect, self->pixels);
    DrawSync(0);
}

void SUG_scrollTexAnimUp(TexAnim *image) {
    Rect16 top;
    Rect16 bottom;
    s32 shift;

    shift = image->timer % image->rect.h;
    if (shift != 0) {
        bottom = image->rect;
        top = bottom;
        top.h -= shift;
        LoadImage2(&top, image->pixels + image->rect.w * (shift << 1));
        bottom.y += image->rect.h - shift;
        bottom.h = shift;
        LoadImage2(&bottom, image->pixels);
        DrawSync(0);
    } else {
        LoadImage2(&image->rect, image->pixels);
        DrawSync(0);
    }
}

void SUG_scrollTexAnimDown(TexAnim *image) {
    Rect16 top;
    Rect16 bottom;
    s32 shift;

    shift = image->timer % image->rect.h;
    if (shift != 0) {
        bottom = image->rect;
        top = bottom;
        top.h = shift;
        LoadImage2(&top, image->pixels + image->rect.w * ((image->rect.h - shift) << 1));
        bottom.y += shift;
        bottom.h -= shift;
        LoadImage2(&bottom, image->pixels);
        DrawSync(0);
    } else {
        LoadImage2(&image->rect, image->pixels);
        DrawSync(0);
    }
}
