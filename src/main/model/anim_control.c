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

/* the heap tag of a model slot's animations: ANIM_HEAP_TAG + slot */
#if VERSION_JP
#define ANIM_HEAP_TAG 0x50
#elif VERSION_US || VERSION_EU
#define ANIM_HEAP_TAG 0x5A
#endif

void initModelScene(void) {
    Scene3D *scene;

    scene = SCENE_3D = allocHeapBlock(sizeof(Scene3D), 0x7F);
    bzero(scene, sizeof(Scene3D));
    GsInitCoordinate2(NULL, &SCENE_3D->root);
    SCENE_3D_ENABLED = 1;
}

/* A negative keyTimer holds the animation; resumeModelAnimation flips it back. */
void pauseModelAnimation(s32 slot) {
    Model *model;
    Model *model2;

    model = SCENE_3D->models[slot];
    if (model->anim.keyTimer <= 0) {
        model->anim.keyTimer = -1;
        return;
    }
    model2 = SCENE_3D->models[slot];
    model2->anim.keyTimer = -model2->anim.keyTimer;
}

void resumeModelAnimation(s32 slot) {
    s32 timer;
    Model *model;

    model = SCENE_3D->models[slot];
    timer = model->anim.keyTimer;
    if (timer < 0) {
        model->anim.keyTimer = -timer;
    }
}

/* loopKey -2 takes the loop key stored in the clip's data. */
s32 startModelAnimation(s32 slot, s32 anim, s32 loopKey, s32 rootOnly) {
    Model *model;
    ModelAnimState *animState;
    s32 keyCount;

    model = SCENE_3D->models[slot];
    animState = &model->anim;
    keyCount = model->anims[anim].frameCount;
    model->rootOnly = rootOnly;
    animState->unk4 = keyCount;
    animState->keyCount = keyCount;
    animState->clip = anim;
    if (loopKey == -2) {
        loopKey = ((KeyFrame *)model->anims[anim].data)->bone[0].pad16;
    }
    animState->loopKey = loopKey;
    animState->key = 0;
    animState->timeScale = 1.0f;
    return loadNextAnimationKeyframe(model, 0, -1);
}

/* The animation cache keys are 0x10000000 | modelId << 8 | anim. */
void unloadModelAnimations(s32 slot) {
    s32 key;
    s32 i;

    key = ((Model *)SCENE_3D->models[slot])->id;
    freeHeapBlocksByTag(slot + ANIM_HEAP_TAG);
    key = (key << 8) | 0x10000000;
    for (i = 0; i < 0x20; i++) {
        if ((SCENE_3D->animCache[i].key & ~0xFF) == key) {
            SCENE_3D->animCache[i].key = 0;
            SCENE_3D->animCache[i].value = 0;
        }
    }
}

/* Drops the cached animations of the models from 63 up and frees the TAM
   files SUGSEG loaded (heap tag 0x82). */
void unloadEffectAnimations(void) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        if ((SCENE_3D->animCache[i].key & 0x0FFFFF00) >= 0x3E80) {
            SCENE_3D->animCache[i].key = 0;
            SCENE_3D->animCache[i].value = 0;
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

    cacheEntry = (KeyValue *)SCENE_3D->animCache;
    animData = findAnimationCacheEntry((id << 8) | 0x10000000 | anim, 32, &cacheEntry);
    if (animData == 0) {
        if (cacheEntry == 0) {
            return 0;
        }
        if (id > 1000) {
#if VERSION_JP
            sprintf(path, "M:\\HDF%03d\\%03d_%d.hdf", id / 10, id / 10, id % 10);
#elif VERSION_US || VERSION_EU
            sprintf(path, "M:\\HDF%d\\%d_%d.hdf", id / 10, id / 10, id % 10);
#endif
            chunkSub = id;
        } else {
            sprintf(path, "M:\\HDF%03d\\%c.hdf", id, anim + 'a');
            chunkSub = anim;
        }
        animData = (s32)findPakChunk(pak, 1, chunkSub);
        if (animData == 0) {
            animData = loadFileTagged((s32 *)path, getCurrentTaskId(), slot + ANIM_HEAP_TAG);
            if (animData == 0) {
                return 0;
            }
            cacheEntry->key = (id << 8) | 0x10000000 | anim;
            cacheEntry->value = animData;
        }
    }
    return animData;
}

void setModelAnimationData(Model *model, s32 *data, s32 anim) {
    model->anims[anim].frameCount = *data++;
    model->anims[anim].data = data;
}

s32 loadModelAnimation(s32 slot, s32 anim, s32 index, s32 pak) {
    s32 animData;
    Model *model;

    model = SCENE_3D->models[slot];
    animData = loadAnimationData(model->id, anim, slot, (Chunk *)pak);
    if (animData != 0) {
        setModelAnimationData(model, (s32 *)animData, index);
        return 1;
    }
    return 0;
}

s32 loadModelAnimationFile(s32 slot, s32 anim, s32 index) {
    return loadModelAnimation(slot, anim, index, 0);
}

/* Poses the model on the first key of an animation and holds it there. */
void applyAnimationFirstFrame(s32 slot, s32 anim) {
    Model *model;
    BoneAnim *bone;
    GsCOORDINATE2 *coord;
    SVECTOR *rot;
    KeyBone *key;
    s32 *rotChan;
    s32 *posChan;
    s32 *scaleChan;
    s32 i;
    s32 j;

    model = SCENE_3D->models[slot];
    bone = (BoneAnim *)model->keys;
    coord = model->coord;
    rot = model->rots;
    key = ((KeyFrame *)model->anims[anim].data)->bone;
    /* nobj bones plus the root */
    for (i = 0; i < model->nobj + 1; i++, key++, bone++, coord++, rot++) {
        /* zero the velocity of the nine channels (j << 4: one AnimChan) */
#if VERSION_JP || VERSION_EU
        for (j = 0; j < 3; j++) {
            AnimChan *chan = &bone->ch[j];

            chan[0].velocity = chan[3].velocity = chan[6].velocity = 0;
        }
#elif VERSION_US
        for (j = 0, rotChan = &bone->ch[0].velocity, posChan = &bone->ch[3].velocity, scaleChan = &bone->ch[6].velocity; j < 3; j++) {
            *(s32 *)((u8 *)rotChan + (j << 4)) = *(s32 *)((u8 *)posChan + (j << 4)) = *(s32 *)((u8 *)scaleChan + (j << 4)) = 0;
        }
#endif
        bone->ch[0].value = key->rx << 20;
        bone->ch[1].value = key->ry << 20;
        bone->ch[2].value = key->rz << 20;
        bone->ch[3].value = key->tx << 16;
        bone->ch[4].value = key->ty << 16;
        bone->ch[5].value = key->tz << 16;
        bone->ch[6].value = key->sx << 16;
        bone->ch[7].value = key->sy << 16;
        bone->ch[8].value = key->sz << 16;
        if (i < model->nobj) {
            rot->vx = bone->ch[0].value / 0x100000;
            rot->vy = bone->ch[1].value / 0x100000;
            rot->vz = bone->ch[2].value / 0x100000;
            coord->coord.t[0] = (s16)(bone->ch[3].value >> 16) + model->bonepos[i][0];
            coord->coord.t[1] = (s16)(bone->ch[4].value >> 16) + model->bonepos[i][1];
            coord->coord.t[2] = (s16)(bone->ch[5].value >> 16) + model->bonepos[i][2];
            model->boneScale[i].vx = (s16)(bone->ch[6].value >> 16);
            model->boneScale[i].vy = (s16)(bone->ch[7].value >> 16);
            model->boneScale[i].vz = (s16)(bone->ch[8].value >> 16);
            RotMatrixYXZ(rot, &coord->coord);
            coord->flg = 0;
            ScaleMatrix(&coord->coord, &model->boneScale[i]);
        }
    }
    pauseModelAnimation(slot);
}
