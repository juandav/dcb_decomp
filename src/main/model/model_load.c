#include "dcb/model_load.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/model.h"
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

void linkOmdObject(s32 tmd, void *obj, s32 index) {
    (*(s32 *)((s8 *)obj + 0xC)) = (s32) (index + 1);
    (*(s32 *)((s8 *)obj + 0)) = 0;
    (*(s32 *)((s8 *)obj + 8)) = tmd;
}

s32 *readModelBonePositions(u8 *model, s32 *data) {
    s32 i;

    for (i = 0; i < *(s16 *)(model + 4); i++) {
        ((Unk1F80 *)model)->unk1F80[i] = (s16 *)(data + 1);
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
    SCENE_3D->unk114[slot] = 0;
    SCENE_3D->unk13C[slot] = 0;
    freeHeapBlocksByTag(slot + 0x40);
}

void unloadAllModels(void) {
    s32 i;

    for (i = 0; i < 24; i++) {
        if (SCENE_3D->unk114[i] != 0) {
            unloadModelAnimations(i);
            SCENE_3D->unk114[i] = 0;
            /* sic: the original clears the wrong slot */
            SCENE_3D->unk13C[i - 0x40] = 0;
        }
    }
    func_80014C08(FRAME_INTERVAL);
    for (i = 0x40; i < 0x7F; i++) {
        freeHeapBlocksByTag(i);
    }
}

void *findLoadedModelById(s32 id) {
    s32 i;

    for (i = 0; i < 0x18; i++) {
        if (SCENE_3D->unk114[i] != 0 &&
            *(s16 *)((u8 *)SCENE_3D->unk13C[i] + 6) == id) {
            return SCENE_3D->unk13C[i];
        }
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/model/model_load", reuseLoadedModelTexture);

s32 loadModel(s32 slot, s32 id, s32 vramSlot, s32 pak, s8 format) {
    char path[16];
    TIM_IMAGE tim;
    u8 *model;
    u8 *data;
    u32 *img;
    s32 texX;
    s32 texY;
    s32 i;
    s32 j;

    if (slot >= 0x18) {
        return 0;
    }
    if (SCENE_3D->unk13C[slot] != 0) {
        unloadModelAnimations(slot);
        unloadModel(slot);
    }
    func_80014C08(FRAME_INTERVAL);
    model = SCENE_3D->unk13C[slot] = allocHeapBlock(0x28F8, slot + 0x40);
    bzero(model, 0x28F8);
    *(s32 *)(model + 0x26F4) = pak;
    pauseModelAnimation(slot);
    RotMatrixYXZ(model + 0xA78, model + 0x2C);
    *(s32 *)(model + 0x28) = 0;
    *(s32 *)(model + 0x20) = 0x1000;
    *(s32 *)(model + 0x1C) = 0x1000;
    *(s32 *)(model + 0x18) = 0x1000;
    for (i = 0; i < 32; i++) {
        for (j = 0; j < 3; j++) {
            ((Model *)model)->keys[i].rot[j].unk0 = 0;
            ((Model *)model)->keys[i].pos[j].unk0 = 0;
            ((Model *)model)->keys[i].scale[j].unk0 = 0x10000000;
        }
    }
    if (vramSlot < 0) {
        vramSlot = (slot >> 1) + (slot & 1) * 16 + 5;
    }
    *(s32 *)(model + 0x26D4) = (vramSlot - 5) << 16;
    *(s32 *)(model + 0x26D0) = ((((vramSlot & 0x10) << 10) | ((vramSlot & 0xF) * 4)) - 0x14) << 16;
    *(s16 *)(model + 6) = id;
    if (id > 1000) {
        sprintf(path, "M:\\%d_%d.omd", id / 10, id % 10);
    } else {
        sprintf(path, "M:\\%03d.omd", id);
    }
    data = findPakChunk((Chunk *)pak, 0, id);
    if (data == 0) {
        data = (u8 *)loadFileTagged((s32 *)path, getCurrentTaskId(), slot + 0x40);
        if (data == 0) {
            return 0;
        }
        *(s32 *)model = LOADED_FILE_SIZE;
    } else {
        *(s32 *)model = ((s32 *)data)[-1];
    }
    *(u8 **)(model + 0x26DC) = data;
    if (vramSlot != 0) {
        i = 0;
        img = findPakChunk((Chunk *)pak, 5, id);
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
            *(Rect16 *)(model + 0x26E4) = *tim.prect;
            *(Rect16 *)(model + 0x26EC) = *tim.crect;
            ((Rect16 *)(model + 0x26EC))->x += texX - 0x140;
            ((Rect16 *)(model + 0x26EC))->y += texY;
            ((Rect16 *)(model + 0x26E4))->x += texX - 0x140;
            ((Rect16 *)(model + 0x26E4))->y += texY;
            if (i) {
                freeHeapBlock(img);
            }
        }
    }
skip:
    StoreImage2((Rect16 *)(model + 0x26EC), (u32 *)(model + 0x26F8));
    data += 0x10;
    *(s16 *)(model + 4) = *(u16 *)data;
    data += 4;
    for (i = 0; i < 32; i++) {
        ((Model *)model)->parent[i] = *data++;
    }
    data = (u8 *)readModelBonePositions(model, (s32 *)data);
    if (format == 0) {
        for (i = 0; i < *(s16 *)(model + 4); i++) {
            if (i != 0) {
                for (data += 4; *(s32 *)data != 0x30444D4F; data += 4) {
                }
            }
            ((Model *)model)->obj[i].tmd = 0;
            relocateOmdObjects((Tmd18 *)data);
            linkOmdObject((s32)(data + 12), &((Model *)model)->obj[i], i);
        }
    } else {
        for (i = 0; i < *(s16 *)(model + 4); i++) {
            if (i != 0) {
                for (data += 4; *(s32 *)data != 0x41 || ((s32 *)data)[1] != 0; data += 4) {
                }
            }
            ((Model *)model)->obj[i].tmd = 0;
            GsMapModelingData((u32 *)(data + 4));
            GsLinkObject4((u32)(data + 12), &((Model *)model)->obj[i], 0);
        }
    }
    initModelBoneHierarchy((Model *)model);
    return 1;
}

void loadOmdModelFromDisc(s32 slot, s32 id, s32 vramSlot) {
    loadModel(slot, id, vramSlot, 0, 0);
}

void loadTmdModelFromDisc(s32 slot, s32 id, s32 vramSlot) {
    loadModel(slot, id, vramSlot, 0, 1);
}
