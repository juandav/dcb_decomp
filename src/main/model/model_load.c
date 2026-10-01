#include "dcb/model_load.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/anim_control.h"
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
#include "dcb/model_anim.h"

void relocateOmdObjects(Tmd18 *tmd) {
    Obj18 *obj;
    s32 objCount;
    s32 i;

    if (tmd->flags == 0) {
        tmd->flags = 1;
        objCount = tmd->nobj;
        obj = tmd->obj;
        for (i = 0; i < objCount; i++) {
            obj->unk14 = (s32)tmd + obj->unk14;
            obj++;
        }
    }
}

void linkOmdObject(s32 tmd, GsDOBJ4 *obj, s32 index) {
    obj->id = index + 1;
    obj->attribute = 0;
    obj->tmd = (u32 *)tmd;
}

s32 *readModelBonePositions(Model *model, s32 *data) {
    s32 i;

    for (i = 0; i < model->nobj; i++) {
        model->bonepos[i] = (s16 *)(data + 1);
        data += 3;
    }
    return data;
}

void initModelBoneHierarchy(Model *model) {
    GsCOORDINATE2 *coord;
    s8 *parent;
    GsDOBJ4 *obj;
    s32 i;

    coord = model->coord;
    GsInitCoordinate2(&SCENE_3D->root, &model->root);
    for (i = 0, parent = model->parent, obj = model->obj; i < model->nobj; i++, parent++, obj++, coord++) {
        obj->coord2 = coord;
        if (*parent < 0) {
            GsInitCoordinate2(&model->root, coord);
        } else {
            GsInitCoordinate2(&model->coord[*parent], coord);
        }
        coord->coord.t[0] = model->bonepos[i][0];
        coord->coord.t[1] = model->bonepos[i][1];
        coord->coord.t[2] = model->bonepos[i][2];
    }
}

void unloadModel(s32 slot) {
    SCENE_3D->modelState[slot] = 0;
    SCENE_3D->models[slot] = 0;
    freeHeapBlocksByTag(slot + 0x40);
}

void unloadAllModels(void) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (SCENE_3D->modelState[i] != 0) {
            unloadModelAnimations(i);
            SCENE_3D->modelState[i] = 0;
            /* sic: the original clears the wrong slot */
            SCENE_3D->models[i - 0x40] = 0;
        }
    }
    waitFrames(FRAME_INTERVAL);
    for (i = 0x40; i < 0x7F; i++) {
        freeHeapBlocksByTag(i);
    }
}

Model *findLoadedModelById(s32 id) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        if (SCENE_3D->modelState[i] != 0 && ((Model *)SCENE_3D->models[i])->id == id) {
            return SCENE_3D->models[i];
        }
    }
    return 0;
}

/* If another loaded model already has this model's texture, copies it into
   this model's VRAM slot instead of loading it again. Returns 0 when it did. */
s32 reuseLoadedModelTexture(Model *model) {
    Model *loaded;
    s32 vramSlot;
    s32 px;
    s32 py;
    s32 cx;
    s32 cy;

    loaded = findLoadedModelById(model->id);
    if (loaded != 0) {
        if (loaded->tpageOffset != model->tpageOffset) {
            vramSlot = model->tpageOffset / 0x10000 + 5;
            px = ((vramSlot & 0xF) << 6) + (loaded->prect.x & 0x3F);
            py = ((vramSlot & 0x10) << 4) + (loaded->prect.y & 0xFF);
            cx = ((vramSlot & 0xF) << 6) + (loaded->crect.x & 0x3F);
            cy = ((vramSlot & 0x10) << 4) + (loaded->crect.y & 0xFF);
            MoveImage(&loaded->prect, px, py);
            MoveImage(&loaded->crect, cx, cy);
            DrawSync(0);
            model->prect.x = px;
            model->prect.y = py;
            model->prect.h = loaded->prect.h;
            model->prect.w = loaded->prect.w;
            model->crect.x = cx;
            model->crect.y = cy;
#if VERSION_JP
            model->crect.h = loaded->prect.h; /* jp copies the wrong height */
#elif VERSION_US || VERSION_EU
            model->crect.h = loaded->crect.h;
#endif
            model->crect.w = loaded->crect.w;
        }
        return 0;
    }
    return 1;
}

/* format (us and eu): 0 for an OMD's objects, 1 for TMDs; jp only loads
   OMDs */
#if VERSION_JP
s32 loadModel(s32 slot, s32 id, s32 vramSlot, s32 pak) {
#elif VERSION_US || VERSION_EU
s32 loadModel(s32 slot, s32 id, s32 vramSlot, s32 pak, s8 format) {
#endif
    char path[16];
    TIM_IMAGE tim;
    Model *model;
    u8 *data;
    u32 *img;
    s32 texX;
    s32 texY;
    s32 i;
    s32 j;

    if (slot >= 0x18) {
        return 0;
    }
    if (SCENE_3D->models[slot] != 0) {
        unloadModelAnimations(slot);
        unloadModel(slot);
    }
    waitFrames(FRAME_INTERVAL);
    model = SCENE_3D->models[slot] = allocHeapBlock(sizeof(Model), slot + 0x40);
    bzero(model, sizeof(Model));
#if VERSION_US || VERSION_EU
    model->pak = (void *)pak;
#endif
    pauseModelAnimation(slot);
    RotMatrixYXZ(&model->rot, &model->root.coord);
    model->root.flg = 0;
    model->scale.vz = 0x1000;
    model->scale.vy = 0x1000;
    model->scale.vx = 0x1000;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 3; j++) {
            model->keys[i].pos[j].value = 0;
            model->keys[i].rot[j].value = 0;
            model->keys[i].scale[j].value = 0x10000000;
        }
    }
    /* vramSlot: bits 0-3 pick a 64-pixel VRAM column, bit 4 the lower half.
       By default slots 2n and 2n+1 share a column, the odd one below. */
    if (vramSlot < 0) {
        vramSlot = (slot >> 1) + (slot & 1) * 16 + 5;
    }
    model->tpageOffset = (vramSlot - 5) << 16;
    model->clutOffset = ((((vramSlot & 0x10) << 10) | ((vramSlot & 0xF) * 4)) - 0x14) << 16;
    model->id = id;
    if (id > 1000) {
#if VERSION_JP
        sprintf(path, "M:\\%03d_%d.omd", id / 10, id % 10);
#elif VERSION_US || VERSION_EU
        sprintf(path, "M:\\%d_%d.omd", id / 10, id % 10);
#endif
    } else {
        sprintf(path, "M:\\%03d.omd", id);
    }
#if VERSION_EU
    printf("%s\n", path);
#endif
    data = findPakChunk((Chunk *)pak, 0, id);
    if (data == 0) {
        data = (u8 *)loadFileTagged((s32 *)path, getCurrentTaskId(), slot + 0x40);
        if (data == 0) {
            return 0;
        }
        model->dataSize = LOADED_FILE_SIZE;
    } else {
        model->dataSize = ((Chunk *)data)[-1].size; /* the header before the chunk */
    }
    model->data = data;
    if (vramSlot != 0) {
        i = 0; /* set when the TIM was loaded from disc and has to be freed */
        img = findPakChunk((Chunk *)pak, 5, id);
        /* not in the PAK: reuse another model's copy, or load the TIM named in the OMD */
        if (img == 0) {
            if (reuseLoadedModelTexture(model) == 0) {
                goto skip;
            }
            sprintf(path, "M:\\%s", data);
            i = 1;
            img = (u32 *)loadFile(path, getCurrentTaskId());
        }
        if (img != 0) {
            texX = (vramSlot & 0xF) << 6;
            texY = (vramSlot & 0x10) << 4;
            uploadTimListOffset(img, texX - 0x140, texY);
            OpenTIM(img);
            ReadTIM(&tim);
            model->prect = *tim.prect;
            model->crect = *tim.crect;
            model->crect.x += texX - 0x140;
            model->crect.y += texY;
            model->prect.x += texX - 0x140;
            model->prect.y += texY;
            if (i) {
                freeHeapBlock(img);
            }
        }
    }
skip:
#if VERSION_US || VERSION_EU
    StoreImage2(&model->crect, (u32 *)model->clut);
#endif
    /* the OMD header: the TIM's name, the bone count, each bone's parent and
       position, then the objects */
    data += 0x10;
    model->nobj = *(u16 *)data;
    data += 4;
    for (i = 0; i < 32; i++) {
        model->parent[i] = *data++;
    }
    data = (u8 *)readModelBonePositions(model, (s32 *)data);
#if VERSION_US || VERSION_EU
    if (format == 0) {
#endif
        for (i = 0; i < model->nobj; i++) {
            if (i != 0) {
                /* skip to the next "OMD0" */
                for (data += 4; *(s32 *)data != 0x30444D4F; data += 4) {
                }
            }
            model->obj[i].tmd = 0;
            relocateOmdObjects((Tmd18 *)data);
            linkOmdObject((s32)(data + 12), &model->obj[i], i);
        }
#if VERSION_US || VERSION_EU
    } else {
        for (i = 0; i < model->nobj; i++) {
            if (i != 0) {
                /* skip to the next TMD header (id 0x41, flags 0) */
                for (data += 4; *(s32 *)data != 0x41 || ((s32 *)data)[1] != 0; data += 4) {
                }
            }
            model->obj[i].tmd = 0;
            GsMapModelingData((u32 *)(data + 4));
            GsLinkObject4((u32)(data + 12), &model->obj[i], 0);
        }
    }
#endif
    initModelBoneHierarchy(model);
    return 1;
}

#if VERSION_JP
void loadOmdModelFromDisc(s32 slot, s32 id, s32 vramSlot) {
    loadModel(slot, id, vramSlot, 0);
}
#elif VERSION_US || VERSION_EU
void loadOmdModelFromDisc(s32 slot, s32 id, s32 vramSlot) {
    loadModel(slot, id, vramSlot, 0, 0);
}

void loadTmdModelFromDisc(s32 slot, s32 id, s32 vramSlot) {
    loadModel(slot, id, vramSlot, 0, 1);
}
#endif
