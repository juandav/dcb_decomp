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
