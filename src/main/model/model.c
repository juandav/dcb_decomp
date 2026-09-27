#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/model.h"
#include "dcb/archive.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/loader.h"
#include "dcb/display.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/model_anim.h"

void initModelScene(void) {
    Unk801D6A4C *scene;

    scene = SCENE_3D = allocHeapBlock(0x29C, 0x7F);
    bzero(scene, 0x29C);
    GsInitCoordinate2(NULL, &SCENE_3D->root);
    SCENE_3D_ENABLED = 1;
}

void pauseModelAnimation(s32 slot) {
    void *model;
    void *model2;

    model = SCENE_3D->unk13C[slot];
    if ((*(s32 *)((s8 *)model + 0x2208)) <= 0) {
        (*(s32 *)((s8 *)model + 0x2208)) = -1;
        return;
    }
    model2 = SCENE_3D->unk13C[slot];
    (*(s32 *)((s8 *)model2 + 0x2208)) = (s32) -(*(s32 *)((s8 *)model2 + 0x2208));
}

void resumeModelAnimation(s32 slot) {
    s32 timer;
    void *model;

    model = SCENE_3D->unk13C[slot];
    timer = (*(s32 *)((s8 *)model + 0x2208));
    if (timer < 0) {
        (*(s32 *)((s8 *)model + 0x2208)) = -timer;
    }
}

s32 startModelAnimation(s32 slot, s32 anim, s32 nextAnim, s32 rootOnly) {
    void *model;
    void *animState;
    s32 frameCount;

    model = SCENE_3D->unk13C[slot];
    animState = (s8 *)model + 0x2200;
    frameCount = ((Model2220 *)model)->unk2220[anim].unk0;
    (*(s32 *)((s8 *)model + 0x26D8)) = rootOnly;
    (*(s32 *)((s8 *)animState + 4)) = frameCount;
    (*(s32 *)((s8 *)animState + 0x14)) = frameCount;
    (*(s32 *)((s8 *)model + 0x2200)) = anim;
    if (nextAnim == -2) {
        nextAnim = (*(s16 *)((s8 *)((Model2220 *)model)->unk2220[anim].unk4 + 0x1A));
    }
    (*(s32 *)((s8 *)animState + 0x18)) = nextAnim;
    (*(s32 *)((s8 *)animState + 0xC)) = 0;
    (*(s32 *)((s8 *)animState + 0x1C)) = 0x3F800000;
    return loadNextAnimationKeyframe(model, 0, -1);
}

void unloadModelAnimations(s32 slot) {
    s32 key;
    s32 i;

    key = *(s16 *)((s8 *)SCENE_3D->unk13C[slot] + 6);
    freeHeapBlocksByTag(slot + 0x5A);
    key = (key << 8) | 0x10000000;
    for (i = 0; i < 0x20; i++) {
        if ((SCENE_3D->unk19C[i].key & ~0xFF) == key) {
            SCENE_3D->unk19C[i].key = 0;
            SCENE_3D->unk19C[i].value = 0;
        }
    }
}

void func_80022E58(void) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if ((SCENE_3D->unk19C[i].key & 0x0FFFFF00) >= 0x3E80) {
            SCENE_3D->unk19C[i].key = 0;
            SCENE_3D->unk19C[i].value = 0;
        }
    }
    freeHeapBlocksByTag(0x82);
}

s32 findAnimationCacheEntry(s32 key, s32 count, KeyValue **freeEntry) {
    KeyValue *entry;
    KeyValue *emptyEntry;
    s32 i;

    entry = *freeEntry;
    emptyEntry = 0;
    for (i = 0; i < count; i++, entry++) {
        if (entry->key == key) {
            return entry->value;
        }
        if (emptyEntry == 0 && entry->key == 0) {
            emptyEntry = entry;
        }
    }
    *freeEntry = emptyEntry;
    return 0;
}

s32 loadAnimationData(s32 id, s32 anim, s32 slot, Chunk *pak) {
    char path[24];
    KeyValue *cacheEntry;
    s32 animData;
    s32 chunkSub;

    cacheEntry = (KeyValue *)SCENE_3D->unk19C;
    animData = findAnimationCacheEntry((id << 8) | 0x10000000 | anim, 32, &cacheEntry);
    if (animData == 0) {
        if (cacheEntry == 0) {
            return 0;
        }
        if (id > 1000) {
            sprintf(path, "M:\\HDF%d\\%d_%d.hdf", id / 10, id / 10, id % 10);
            chunkSub = id;
        } else {
            sprintf(path, "M:\\HDF%03d\\%c.hdf", id, anim + 'a');
            chunkSub = anim;
        }
        animData = (s32)findPakChunk(pak, 1, chunkSub);
        if (animData == 0) {
            animData = loadFileTagged((s32 *)path, getCurrentTaskId(), slot + 0x5A);
            if (animData == 0) {
                return 0;
            }
            cacheEntry->key = (id << 8) | 0x10000000 | anim;
            cacheEntry->value = animData;
        }
    }
    return animData;
}

void setModelAnimationData(Model2220 *model, s32 *data, s32 anim) {
    model->unk2220[anim].unk0 = *data++;
    model->unk2220[anim].unk4 = data;
}

s32 loadModelAnimation(s32 slot, s32 anim, s32 index, s32 pak) {
    s32 animData;
    void *model;

    model = SCENE_3D->unk13C[slot];
    animData = loadAnimationData((*(s16 *)((s8 *)model + 6)), anim, slot, (Chunk *)pak);
    if (animData != 0) {
        setModelAnimationData(model, (s32 *)animData, index);
        return 1;
    }
    return 0;
}

void loadModelAnimationFile(s32 slot, s32 anim, s32 index) {
    loadModelAnimation(slot, anim, index, 0);
}

void applyAnimationFirstFrame(s32 slot, s32 anim) {
    u8 *model;
    BoneAnim *bone;
    u8 *coord;
    SVECTOR *rot;
    s16 *keyframe;
    s32 *rotChan;
    s32 *posChan;
    s32 *scaleChan;
    s32 i;
    s32 j;

    model = SCENE_3D->unk13C[slot];
    bone = (BoneAnim *)(model + 0xD80);
    coord = model + 0x78;
    rot = (SVECTOR *)(model + 0xA80);
    keyframe = (s16 *)((u8 *)((Model2220 *)model)->unk2220[anim].unk4 + 4);
    for (i = 0; i < *(s16 *)(model + 4) + 1; i++, keyframe += 12, bone++, coord += 0x50, rot++) {
        for (j = 0, rotChan = &bone->ch[0].val, posChan = &bone->ch[3].val, scaleChan = &bone->ch[6].val; j < 3; j++) {
            *(s32 *)((u8 *)rotChan + (j << 4)) = *(s32 *)((u8 *)posChan + (j << 4)) = *(s32 *)((u8 *)scaleChan + (j << 4)) = 0;
        }
        bone->ch[0].unk0 = keyframe[0] << 20;
        bone->ch[1].unk0 = keyframe[1] << 20;
        bone->ch[2].unk0 = keyframe[2] << 20;
        bone->ch[3].unk0 = keyframe[4] << 16;
        bone->ch[4].unk0 = keyframe[5] << 16;
        bone->ch[5].unk0 = keyframe[6] << 16;
        bone->ch[6].unk0 = keyframe[8] << 16;
        bone->ch[7].unk0 = keyframe[9] << 16;
        bone->ch[8].unk0 = keyframe[10] << 16;
        if (i < *(s16 *)(model + 4)) {
            rot->vx = bone->ch[0].unk0 / 0x100000;
            rot->vy = bone->ch[1].unk0 / 0x100000;
            rot->vz = bone->ch[2].unk0 / 0x100000;
            *(s32 *)(coord + 0x18) = (s16)(bone->ch[3].unk0 >> 16) + ((Model2220 *)model)->bonepos[i][0];
            *(s32 *)(coord + 0x1C) = (s16)(bone->ch[4].unk0 >> 16) + ((Model2220 *)model)->bonepos[i][1];
            *(s32 *)(coord + 0x20) = (s16)(bone->ch[5].unk0 >> 16) + ((Model2220 *)model)->bonepos[i][2];
            ((Model2220 *)model)->scale[i][0] = (s16)(bone->ch[6].unk0 >> 16);
            ((Model2220 *)model)->scale[i][1] = (s16)(bone->ch[7].unk0 >> 16);
            ((Model2220 *)model)->scale[i][2] = (s16)(bone->ch[8].unk0 >> 16);
            RotMatrixYXZ(rot, coord + 4);
            *(s32 *)coord = 0;
            ScaleMatrix(coord + 4, ((Model2220 *)model)->scale[i]);
        }
    }
    pauseModelAnimation(slot);
}

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

INCLUDE_ASM("asm/main/nonmatchings/model/model", reuseLoadedModelTexture);

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
