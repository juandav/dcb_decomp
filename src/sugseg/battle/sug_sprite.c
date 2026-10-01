#include "common.h"
#include "game.h"
#include "dcb/sug_sprite.h"
#include "dcb/heap.h"
#include "dcb/archive.h"
#include "dcb/loader.h"
#include "dcb/task.h"
#include "dcb/vram_upload.h"
#include "gte.h"

#define setEntry(e, _key, _data, _subKey) \
    do {                                  \
        (e)->key = (_key);                \
        (e)->data = (_data);              \
        (e)->subKey = (_subKey);          \
    } while (0)

/* the header of a sprite's data (Sprite.tex); its frames follow at 0x10 */
typedef struct {
    s32 frameCount;
#if VERSION_JP
    u8 loopFrame;
#elif VERSION_US || VERSION_EU
    s8 loopFrame; /* the frame to go on from after the last, negative: hide */
#else
#error "untested version"
#endif
    u8 unk5;
    u16 mode;
    u16 clutX;
    u16 clutY;
    u16 x;
    u16 y;
} SpriteSheet;

extern SpriteEntry *SUG_SPRITE_CACHE;
extern const char SUG_FMT_SPRITE_PATH[];

/* jp gives the sheet of a loaded sprite instead of its entry, and a free
   entry otherwise */
#if VERSION_JP
SpriteEntry *SUG_findSpriteEntry(s32 key, s32 subKey) {
    SpriteEntry *entry;
    SpriteEntry *free;
    s32 i;

    entry = SUG_SPRITE_CACHE;
    free = NULL;
    for (i = 0; i < 128; i++, entry++) {
        if (key == entry->key && subKey == entry->subKey) {
            return (SpriteEntry *)entry->data;
        }
        if (free == NULL && entry->data == NULL) {
            free = entry;
        }
    }
    return free;
}
#elif VERSION_US || VERSION_EU
SpriteEntry *SUG_findSpriteEntry(s32 key, s32 subKey) {
    SpriteEntry *entry;
    SpriteEntry *free;
    s32 i;

    entry = SUG_SPRITE_CACHE;
    free = NULL;
    for (i = 0; i < 128; i++, entry++) {
        if (key == entry->key && subKey == entry->subKey) {
            return entry;
        }
        if (free == NULL && entry->data == NULL) {
            free = entry;
        }
    }
    return free;
}
#else
#error "untested version"
#endif

void SUG_initSpriteCache(void) {
    s32 i;

    SUG_SPRITE_CACHE = allocHeapBlock(0x600, 0x80);
    for (i = 0; i < 128; i++) {
        SUG_SPRITE_CACHE[i].key = -1;
        SUG_SPRITE_CACHE[i].data = NULL;
        SUG_SPRITE_CACHE[i].subKey = 0;
    }
}

void SUG_initSpritePolys(Sprite *sprite) {
    SpriteSheet *sheet;
    SpriteFrame *frame;
    POLY_FT4 *poly;
    u8 u0;
    u8 u1;
    u8 v0;
    u8 v1;
    s32 i;
    s32 mode;

    frame = sprite->frames;
    sheet = (SpriteSheet *)sprite->tex;
    for (i = 0; i < 2; i++) {
        poly = &sprite->polys[i];
        u0 = frame->u0;
        u1 = frame->u1;
        v0 = frame->v0;
        v1 = frame->v1;
        SetPolyFT4(poly);
        poly->clut = getClut(sheet->clutX, sheet->clutY);
        mode = sheet->mode;
        /* x and y rounded down to their texture page */
        poly->tpage = getTPage(mode & 3, 0, sheet->x & (-64 << mode), sheet->y & ~0xFF);
        poly->pad2 = 1;
        poly->u0 = u0;
        poly->v0 = v0;
        poly->u1 = u1;
        poly->v1 = v0;
        poly->u2 = u0;
        poly->v2 = v1;
        poly->u3 = u1;
        poly->v3 = v1;
        poly->r0 = 0x80;
        poly->g0 = 0x80;
        poly->b0 = 0x80;
        setShadeTex(poly, 0);
    }
}

/* jp's sprites can't be flipped or placed by an origin */
#if VERSION_JP
void SUG_setSpriteFrameVerts(Sprite *sprite, s32 frame) {
    SpriteFrame *f;
    s32 x0;
    s32 x1;
    s32 y0;
    s32 y1;

    f = &sprite->frames[frame];
    x0 = -(f->w * sprite->scaleX / 2);
    x1 = x0 + f->w * sprite->scaleX - 1;
    y0 = -(f->h * sprite->scaleY / 2);
    y1 = y0 + f->h * sprite->scaleY - 1;
    sprite->v[0].vx = sprite->v[2].vx = x0;
    sprite->v[1].vx = sprite->v[3].vx = x1;
    sprite->v[0].vy = sprite->v[1].vy = y0;
    sprite->v[2].vy = sprite->v[3].vy = y1;
}
#elif VERSION_US || VERSION_EU
void SUG_setSpriteFrameVerts(Sprite *sprite, s32 frame) {
    SpriteFrame *f;
    s32 xs[2];
    s32 ys[2];

    f = &sprite->frames[frame];
    xs[0] = -(f->w * sprite->scaleX / 2);
    xs[1] = f->w * sprite->scaleX + xs[0] - 1;
    ys[0] = -(f->h * sprite->scaleY / 2);
    ys[1] = f->h * sprite->scaleY + ys[0] - 1;
    sprite->v[0].vx = sprite->v[2].vx = xs[sprite->flipX];
    sprite->v[1].vx = sprite->v[3].vx = xs[sprite->flipX ^ 1];
    sprite->v[0].vy = sprite->v[1].vy = ys[sprite->flipY];
    sprite->v[2].vy = sprite->v[3].vy = ys[sprite->flipY ^ 1];
    if (sprite->useOrigin != 0) {
        sprite->v[0].vx = sprite->v[2].vx = sprite->v[0].vx - f->originX * sprite->scaleX;
        sprite->v[1].vx = sprite->v[3].vx = sprite->v[1].vx - f->originX * sprite->scaleX;
        sprite->v[0].vy = sprite->v[1].vy = sprite->v[0].vy - f->originY * sprite->scaleY;
        sprite->v[2].vy = sprite->v[3].vy = sprite->v[2].vy - f->originY * sprite->scaleY;
    }
}
#else
#error "untested version"
#endif

#if VERSION_JP
void SUG_initSprite(Sprite *sprite, s32 key, u16 scaleX, u16 scaleY, s16 x, s16 y, s16 z, s32 otz, s32 subKey) {
    sprite->tex = (u8 *)SUG_findSpriteEntry(key, subKey);
    if (sprite->tex != NULL) {
        sprite->frames = (SpriteFrame *)(sprite->tex + sizeof(SpriteSheet));
        SUG_initSpritePolys(sprite);
        sprite->pos.vx = x;
        sprite->pos.vy = y;
        sprite->pos.vz = z;
        sprite->frame = 0;
        sprite->frameTimer = -1;
        sprite->scaleX = scaleX;
        sprite->scaleY = scaleY;
        sprite->otz = otz;
        SUG_setSpriteFrameVerts(sprite, 0);
        sprite->v[0].vz = sprite->v[1].vz = sprite->v[2].vz = sprite->v[3].vz = 0;
    }
}
#elif VERSION_US || VERSION_EU
void SUG_initSprite(Sprite *sprite, s32 key, u16 scaleX, u16 scaleY, s16 x, s16 y, s16 z, s32 otz, s32 useOrigin, s32 subKey) {
    sprite->tex = SUG_findSpriteEntry(key, subKey)->data;
    if (sprite->tex != NULL) {
        sprite->frames = (SpriteFrame *)(sprite->tex + sizeof(SpriteSheet));
        SUG_initSpritePolys(sprite);
        sprite->useOrigin = useOrigin;
        sprite->pos.vx = x;
        sprite->pos.vy = y;
        sprite->pos.vz = z;
        sprite->frame = 0;
        sprite->frameTimer = -1;
        sprite->scaleX = scaleX;
        sprite->scaleY = scaleY;
        sprite->otz = otz;
        SUG_setSpriteFrameVerts(sprite, 0);
        sprite->v[0].vz = sprite->v[1].vz = sprite->v[2].vz = sprite->v[3].vz = 0;
    }
}
#else
#error "untested version"
#endif

/* brightness: 0x100 is full in us, 0xFF in jp */
#if VERSION_JP
void SUG_drawSprite(Sprite *sprite, u8 brightness) {
#elif VERSION_US || VERSION_EU
void SUG_drawSprite(Sprite *sprite, s16 brightness) {
#else
#error "untested version"
#endif
    POLY_FT4 *poly;
    MATRIX *m;
    SpriteFrame *frame;
    u32 otz;
    s32 p;
    s32 flag;
    s32 mode;
    u8 u0, u1, v0, v1;

    poly = &sprite->polys[FRAME_BUFFER_INDEX];
    m = (MATRIX *)0x1F800008;
    if (sprite->frame < 0) {
        return;
    }
    frame = &sprite->frames[sprite->frame];
    if (++sprite->frameTimer >= frame->duration) {
        if (++sprite->frame >= ((SpriteSheet *)sprite->tex)->frameCount) {
            if ((sprite->frame = ((SpriteSheet *)sprite->tex)->loopFrame) < 0) {
                sprite->frame = -8;
                return;
            }
        }
        sprite->frameTimer -= frame->duration;
        SUG_setSpriteFrameVerts(sprite, sprite->frame);
        frame = &sprite->frames[sprite->frame];
    }
    gte_ldv0(&sprite->pos);
    gte_rtv0tr();
    gte_stlvnl(m->t);
    gte_stflg(&flag);
    gte_SetTransMatrix(m);
    otz = RotAverage4(&sprite->v[0], &sprite->v[1], &sprite->v[2], &sprite->v[3], (s32 *)&poly->x0, (s32 *)&poly->x1,
                      (s32 *)&poly->x2, (s32 *)&poly->x3, &p, &flag);
    if (sprite->otz != 0) {
        otz = sprite->otz;
    }
    if (otz < 0x1000) {
#if VERSION_JP
        poly->r0 = poly->g0 = poly->b0 = frame->shade * brightness / 255;
#elif VERSION_US || VERSION_EU
        poly->r0 = poly->g0 = poly->b0 = frame->shade * brightness / 256;
#endif
        poly->clut = getClut(((SpriteSheet *)sprite->tex)->clutX, ((SpriteSheet *)sprite->tex)->clutY + frame->clutRow);
        if (frame->attr >= 0) {
            mode = ((SpriteSheet *)sprite->tex)->mode;
            poly->tpage = getTPage(mode & 3, frame->attr & 3, ((SpriteSheet *)sprite->tex)->x & (-64 << mode),
                                   ((SpriteSheet *)sprite->tex)->y & ~0xFF);
            SetSemiTrans(poly, 1);
        } else {
            SetSemiTrans(poly, 0);
        }
        u0 = frame->u0;
        u1 = frame->u1;
        v0 = frame->v0;
        v1 = frame->v1;
        poly->u0 = u0;
        poly->v0 = v0;
        poly->u1 = u1;
        poly->v1 = v0;
        poly->u2 = u0;
        poly->v2 = v1;
        poly->u3 = u1;
        poly->v3 = v1;
        addPrim(&CURRENT_FRAME_BUFFER->ot[otz], poly);
    }
}

/* jp's sprite files hold several sheets, us's one */
#if VERSION_JP
void SUG_loadSprite(s32 id, s32 x, s32 y, s32 subKey) {
    char path[20];
    s32 task;
    SpriteEntry *entry;
    u8 *data;
    u8 *copy;
    u32 *tims;
    SpriteFrame *frame;
    s32 count;
    s32 n;
    s32 i;

    task = getCurrentTaskId();
    entry = SUG_findSpriteEntry(id, subKey);
    if (entry == NULL || entry->key >= 0) {
        return;
    }
    data = findPakChunk((Chunk *)subKey, 3, id);
    if (data == NULL) {
        sprintf(path, "E:\\SPRITE\\%d.a2d", id);
        data = (u8 *)loadFileTagged((s32 *)path, task, 0x80);
    } else {
        copy = allocHeapBlock(((s32 *)data)[-1], 0x80);
        bcopy(data, copy, ((s32 *)data)[-1]);
        data = copy;
    }
    if (data == NULL) {
        return;
    }
    x -= 0x140;
    tims = findPakChunk((Chunk *)subKey, 5, id);
    if (tims == NULL) {
        sprintf(path, SUG_FMT_SPRITE_PATH, data);
        tims = (u32 *)loadFile(path, task);
        uploadTimListOffset(tims, x, y);
        freeHeapBlock(tims);
    } else {
        uploadTimListOffset(tims, x, y);
    }
    data += 0x10;
    /* the file's sheets, each with its frames */
    count = *(s32 *)data;
    data += 4;
    for (n = 0; n < count; n++) {
        entry = SUG_findSpriteEntry(id, subKey);
        if (entry == NULL) {
            return;
        }
        if (entry->key < 0) {
            setEntry(entry, id + n, data, subKey);
            data = data + ((SpriteSheet *)entry->data)->frameCount * sizeof(SpriteFrame) + sizeof(SpriteSheet);
            ((SpriteSheet *)entry->data)->x += x;
            ((SpriteSheet *)entry->data)->y += y;
            ((SpriteSheet *)entry->data)->clutX += x;
            ((SpriteSheet *)entry->data)->clutY += y;
            frame = (SpriteFrame *)(entry->data + sizeof(SpriteSheet));
            for (i = 0; i < ((SpriteSheet *)entry->data)->frameCount; i++) {
                frame->v0 += y;
                frame->v1 += y;
                frame++;
            }
        }
    }
}
#elif VERSION_US || VERSION_EU
void SUG_loadSprite(s32 id, s32 x, s32 y, s32 subKey) {
    char path[20];
    s32 task;
    SpriteEntry *entry;
    u8 *data;
    u8 *copy;
    u32 *tims;
    SpriteFrame *frame;
    s32 i;

    task = getCurrentTaskId();
    entry = SUG_findSpriteEntry(id, subKey);
    if (entry == NULL) {
        return;
    }
    data = findPakChunk((Chunk *)subKey, 3, id);
    if (data == NULL) {
        sprintf(path, "E:\\SPRITE\\%d.a2d", id);
        data = (u8 *)loadFileTagged((s32 *)path, task, 0x80);
    } else {
        copy = allocHeapBlock(((s32 *)data)[-1], 0x80);
        bcopy(data, copy, ((s32 *)data)[-1]);
        data = copy;
    }
    if (data == NULL) {
        return;
    }
    x -= 0x140;
    tims = findPakChunk((Chunk *)subKey, 5, id);
    if (tims == NULL) {
        sprintf(path, SUG_FMT_SPRITE_PATH, data);
        tims = (u32 *)loadFile(path, task);
        uploadTimListOffset(tims, x, y);
        freeHeapBlock(tims);
    } else {
        uploadTimListOffset(tims, x, y);
    }
    data += 0x14;
    setEntry(entry, id, data, subKey);
    ((SpriteSheet *)entry->data)->x += x;
    ((SpriteSheet *)entry->data)->y += y;
    ((SpriteSheet *)entry->data)->clutX += x;
    ((SpriteSheet *)entry->data)->clutY += y;
    frame = (SpriteFrame *)(entry->data + sizeof(SpriteSheet));
    for (i = 0; i < ((SpriteSheet *)entry->data)->frameCount; i++) {
        frame->v0 += y;
        frame->v1 += y;
        frame++;
    }
}
#else
#error "untested version"
#endif

/* the last three bytes are leftovers in the original, not zero padding, and
   not the same in every version */
#if VERSION_JP
const char SUG_FMT_SPRITE_PATH[16] = "E:\\SPRITE\\%s\0\xDC\x33\xC0";
#elif VERSION_US
const char SUG_FMT_SPRITE_PATH[16] = "E:\\SPRITE\\%s\0\0\xA2\xAF";
#elif VERSION_EU
const char SUG_FMT_SPRITE_PATH[16] = "E:\\SPRITE\\%s\0\x94\x08";
#else
#error "untested version"
#endif

void SUG_loadSpriteFile(s32 id, s32 x, s32 y) {
    SUG_loadSprite(id, x, y, 0);
}

void SUG_freeSprites(void) {
    freeHeapBlocksByTag(0x80);
}

void SUG_hideSprite(Sprite *sprite) {
    sprite->frame = -1;
}
