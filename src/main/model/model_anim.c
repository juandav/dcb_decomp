#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/model_anim.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/sound_play.h"

/* Moves the model by the root's z translation, turned by the model's rotation */
void applyRootMotion(Model *model) {
    BoneKeys *root;
    SVECTOR offset;
    MATRIX mat;
    VECTOR worldOffset;
    s32 flag;

    root = model->keys;
    if (ANIM_CHAN_INT(root->pos[2]) == 0) {
        return;
    }
    offset.vy = 0;
    offset.vx = 0;
    offset.vz = ANIM_CHAN_INT(root->pos[2]);
    PushMatrix();
    mat.t[2] = 0;
    mat.t[1] = 0;
    mat.t[0] = 0;
    gte_SetTransMatrix(&mat);
    RotMatrix(&model->rot, &mat);
    gte_SetRotMatrix(&mat);
    gte_ldv0(&offset);
    gte_rtv0tr();
    gte_stlvnl(&worldOffset);
    gte_stflg(&flag);
    ANIM_CHAN_INT(root->pos[2]) = 0;
    model->pos.vx += worldOffset.vx;
    model->pos.vy += worldOffset.vy;
    model->pos.vz += worldOffset.vz;
    PopMatrix();
}

/*
 * Eases a channel from `from` to `to` over `length` frames: it arrives with
 * the mean of this key's slope and the next one's (towards `next`), and the
 * velocity changes at a constant rate in each half of the key.
 */
void setupRotationCurve(AnimChan *chan, s32 length, s32 nextLength, s32 halfLength, s32 from, s32 to, s32 next) {
    s32 start;
    s32 nextDelta;
    s32 end;
    s32 slope;
    s32 endVelocity;
    s32 midVelocity;

    start = from << 20;
    chan->value = start;
    end = to << 20;
    midVelocity = end - start;
    slope = midVelocity / length;
    /* endVelocity first holds the next key's step */
    endVelocity = next - to;
    nextDelta = endVelocity << 20;
    endVelocity = (nextDelta / nextLength + slope) / 2;
    midVelocity = slope * 2 - (endVelocity + chan->velocity) / 2;
    chan->accel0 = (midVelocity - chan->velocity) / halfLength;
    chan->accel1 = (endVelocity - midVelocity) / halfLength;
}

/* setupRotationCurve for the channels kept << 16 */
void setupTranslationCurve(AnimChan *chan, s32 length, s32 nextLength, s32 halfLength, s32 from, s32 to, s32 next) {
    s32 endVelocity;
    s32 velocity;

    velocity = from << 16;
    chan->value = velocity;
    velocity = ((to << 16) - velocity) / length;
    endVelocity = (((next - to) << 16) / nextLength + velocity) / 2;
    velocity = velocity * 2 - (endVelocity + chan->velocity) / 2;
    chan->accel0 = (velocity - chan->velocity) / halfLength;
    chan->accel1 = (endVelocity - velocity) / halfLength;
}

/* Steps every bone's velocities: accel0 in the first half of the key, accel1 after */
void accelerateModelBones(Model *model) {
    BoneKeys *bone;
    s32 i;
    ModelAnimState *anim;

    bone = model->keys;
    anim = &model->anim;
    anim->halfTimer--;
    i = 0;
    if (model->rootOnly != 0) {
        /* only the entry after the last bone */
        i = model->nobj;
        bone += i;
    }
    for (; i < model->nobj + 1; bone++, i++) {
        if (anim->halfTimer >= 0) {
            bone->rot[0].velocity += bone->rot[0].accel0;
            bone->rot[1].velocity += bone->rot[1].accel0;
            bone->rot[2].velocity += bone->rot[2].accel0;
            bone->pos[0].velocity += bone->pos[0].accel0;
            bone->pos[1].velocity += bone->pos[1].accel0;
            bone->pos[2].velocity += bone->pos[2].accel0;
            bone->scale[0].velocity += bone->scale[0].accel0;
            bone->scale[1].velocity += bone->scale[1].accel0;
            bone->scale[2].velocity += bone->scale[2].accel0;
        } else {
            bone->rot[0].velocity += bone->rot[0].accel1;
            bone->rot[1].velocity += bone->rot[1].accel1;
            bone->rot[2].velocity += bone->rot[2].accel1;
            bone->pos[0].velocity += bone->pos[0].accel1;
            bone->pos[1].velocity += bone->pos[1].accel1;
            bone->pos[2].velocity += bone->pos[2].accel1;
            bone->scale[0].velocity += bone->scale[0].accel1;
            bone->scale[1].velocity += bone->scale[1].accel1;
            bone->scale[2].velocity += bone->scale[2].accel1;
        }
    }
}

/* Builds each bone's local matrix from its channels, then moves the channels on a frame */
s32 updateModelBoneMatrices(Model *model) {
    s32 i;
    MATRIX *matrix;
    BoneKeys *bone;
    GsCOORDINATE2 *coord;
    SVECTOR *rot;

    bone = model->keys;
    coord = model->coord;
    rot = model->rots;
    for (i = 0; i < model->nobj; i++, bone++, coord++, rot++) {
        /* jp also clears the flag before rebuilding the matrix */
#if VERSION_JP
        coord->flg = 0;
#endif
        rot->vx = bone->rot[0].value / 0x100000;
        rot->vy = bone->rot[1].value / 0x100000;
        rot->vz = bone->rot[2].value / 0x100000;
        coord->coord.t[0] = ANIM_CHAN_INT(bone->pos[0]) + model->bonepos[i][0];
        coord->coord.t[1] = ANIM_CHAN_INT(bone->pos[1]) + model->bonepos[i][1];
        coord->coord.t[2] = ANIM_CHAN_INT(bone->pos[2]) + model->bonepos[i][2];
        model->boneScale[i].vx = ANIM_CHAN_INT(bone->scale[0]);
        model->boneScale[i].vy = ANIM_CHAN_INT(bone->scale[1]);
        model->boneScale[i].vz = ANIM_CHAN_INT(bone->scale[2]);
        matrix = &coord->coord;
        RotMatrixYXZ(rot, matrix);
        coord->flg = 0;
        ScaleMatrix(matrix, &model->boneScale[i]);
        bone->rot[0].value += bone->rot[0].velocity;
        bone->rot[1].value += bone->rot[1].velocity;
        bone->rot[2].value += bone->rot[2].velocity;
        bone->pos[0].value += bone->pos[0].velocity;
        bone->pos[1].value += bone->pos[1].velocity;
        bone->pos[2].value += bone->pos[2].velocity;
        bone->scale[0].value += bone->scale[0].velocity;
        bone->scale[1].value += bone->scale[1].velocity;
        bone->scale[2].value += bone->scale[2].velocity;
    }
    /* the entry after the last bone has no matrix */
    bone->rot[0].value += bone->rot[0].velocity;
    bone->rot[1].value += bone->rot[1].velocity;
    bone->rot[2].value += bone->rot[2].velocity;
    bone->pos[0].value += bone->pos[0].velocity;
    bone->pos[1].value += bone->pos[1].velocity;
    bone->pos[2].value += bone->pos[2].velocity;
    bone->scale[0].value += bone->scale[0].velocity;
    bone->scale[1].value += bone->scale[1].velocity;
    return bone->scale[2].value += bone->scale[2].velocity;
}

/*
 * Starts easing every bone towards the next key of the clip (mode 0), or
 * towards loopKey (otherwise, or after the last key). With rootOnly set the
 * bones instead move linearly to the key in half the time.
 */
s32 loadNextAnimationKeyframe(Model *m, s32 loopKey, s32 mode) {
    ModelAnimState *anim;
    KeyBone *key;
    KeyBone *nextKey;
    GsDOBJ4 *obj;
    BoneKeys *b;
    s32 length;
    s32 nextLength;
    s32 halfLength;
    s32 to;
    s32 i;

    anim = &m->anim;
    if (anim->key < 0) {
        return anim->keyTimer = -1;
    }
    if (((KeyFrame *)m->anims[anim->clip].data)[anim->key].sound != 0) {
        playSoundEffect(((KeyFrame *)m->anims[anim->clip].data)[anim->key].sound - 1);
    }
    key = ((KeyFrame *)m->anims[anim->clip].data)[anim->key].bone;
    nextKey = key;
    halfLength = ((KeyFrame *)m->anims[anim->clip].data)[anim->key].duration * anim->timeScale;
    anim->halfTimer = halfLength;
    length = halfLength * 2;
    anim->keyTimer = length;
    nextLength = 1;
    if (mode == 0 && anim->key + 1 < anim->keyCount) {
        to = anim->key + 1;
    } else {
        to = loopKey;
    }
    if (to >= 0) {
        anim->key = to;
        nextLength = ((KeyFrame *)m->anims[anim->clip].data)[to].duration * 2 * anim->timeScale;
        nextKey = ((KeyFrame *)m->anims[anim->clip].data)[to].bone;
    } else {
        anim->key = -1;
    }
    b = m->keys;
    obj = m->obj;
    if (m->rootOnly == 0) {
        for (i = 0; i < m->nobj; i++, key++, nextKey++, obj++, b++) {
            if (obj->tmd != 0) {
                if (obj->attribute != 2) {
                    obj->attribute = key->attribute;
                }
                setupRotationCurve(&b->rot[0], length, nextLength, halfLength, b->rot[0].value / 0x100000, key->rx, nextKey->rx);
                setupRotationCurve(&b->rot[1], length, nextLength, halfLength, b->rot[1].value / 0x100000, key->ry, nextKey->ry);
                setupRotationCurve(&b->rot[2], length, nextLength, halfLength, b->rot[2].value / 0x100000, key->rz, nextKey->rz);
                setupTranslationCurve(&b->pos[0], length, nextLength, halfLength, (s16)(b->pos[0].value >> 16), key->tx, nextKey->tx);
                setupTranslationCurve(&b->pos[1], length, nextLength, halfLength, (s16)(b->pos[1].value >> 16), key->ty, nextKey->ty);
                setupTranslationCurve(&b->pos[2], length, nextLength, halfLength, (s16)(b->pos[2].value >> 16), key->tz, nextKey->tz);
                setupTranslationCurve(&b->scale[0], length, nextLength, halfLength, (s16)(b->scale[0].value >> 16), key->sx, nextKey->sx);
                setupTranslationCurve(&b->scale[1], length, nextLength, halfLength, (s16)(b->scale[1].value >> 16), key->sy, nextKey->sy);
                setupTranslationCurve(&b->scale[2], length, nextLength, halfLength, (s16)(b->scale[2].value >> 16), key->sz, nextKey->sz);
            }
        }
        setupRotationCurve(&b->rot[0], length, nextLength, halfLength, b->rot[0].value / 0x100000, key->rx, nextKey->rx);
        setupRotationCurve(&b->rot[1], length, nextLength, halfLength, b->rot[1].value / 0x100000, key->ry, nextKey->ry);
        setupRotationCurve(&b->rot[2], length, nextLength, halfLength, b->rot[2].value / 0x100000, key->rz, nextKey->rz);
        setupTranslationCurve(&b->pos[0], length, nextLength, halfLength, (s16)(b->pos[0].value >> 16), key->tx, nextKey->tx);
        setupTranslationCurve(&b->pos[1], length, nextLength, halfLength, (s16)(b->pos[1].value >> 16), key->ty, nextKey->ty);
        setupTranslationCurve(&b->pos[2], length, nextLength, halfLength, (s16)(b->pos[2].value >> 16), key->tz, nextKey->tz);
        setupTranslationCurve(&b->scale[0], length, nextLength, halfLength, (s16)(b->scale[0].value >> 16), key->sx, nextKey->sx);
        setupTranslationCurve(&b->scale[1], length, nextLength, halfLength, (s16)(b->scale[1].value >> 16), key->sy, nextKey->sy);
        setupTranslationCurve(&b->scale[2], length, nextLength, halfLength, (s16)(b->scale[2].value >> 16), key->sz, nextKey->sz);
    } else {
        length /= 2;
        anim->keyTimer = length;
        anim->halfTimer /= 2;
        for (i = 0; i < m->nobj; i++, key++, obj++, b++) {
            if (obj->tmd != 0) {
                if (obj->attribute != 2) {
                    obj->attribute = key->attribute;
                }
                b->rot[0].velocity = ((key->rx << 20) - b->rot[0].value) / length;
                b->rot[1].velocity = ((key->ry << 20) - b->rot[1].value) / length;
                b->rot[2].velocity = ((key->rz << 20) - b->rot[2].value) / length;
                b->pos[0].velocity = ((key->tx << 16) - b->pos[0].value) / length;
                b->pos[1].velocity = ((key->ty << 16) - b->pos[1].value) / length;
                b->pos[2].velocity = ((key->tz << 16) - b->pos[2].value) / length;
                b->scale[0].velocity = ((key->sx << 16) - b->scale[0].value) / length;
                b->scale[1].velocity = ((key->sy << 16) - b->scale[1].value) / length;
                b->scale[2].velocity = ((key->sz << 16) - b->scale[2].value) / length;
            }
        }
        b->rot[0].velocity = ((key->rx << 20) - b->rot[0].value) / length;
        b->rot[1].velocity = ((key->ry << 20) - b->rot[1].value) / length;
        b->rot[2].velocity = ((key->rz << 20) - b->rot[2].value) / length;
        b->pos[0].velocity = ((key->tx << 16) - b->pos[0].value) / length;
        b->pos[1].velocity = ((key->ty << 16) - b->pos[1].value) / length;
        b->pos[2].velocity = ((key->tz << 16) - b->pos[2].value) / length;
        b->scale[0].velocity = ((key->sx << 16) - b->scale[0].value) / length;
        b->scale[1].velocity = ((key->sy << 16) - b->scale[1].value) / length;
        b->scale[2].velocity = ((key->sz << 16) - b->scale[2].value) / length;
    }
}


/* The task that plays the animations of the 24 scene models, once per frame */
void runModelAnimationTask(void) {
    s32 slot;
    Model *model;
    ModelAnimState *anim;

    SCENE_3D_ENABLED = 1;
    for (;;) {
        for (slot = 0; slot < 24; slot++) {
            model = SCENE_3D->models[slot];
            if (SCENE_3D->modelState[slot] > 0) {
                anim = &model->anim;
                if (model->anim.keyTimer >= 0) {
                    if (--anim->keyTimer <= 0) {
                        loadNextAnimationKeyframe(model, anim->loopKey, 0);
                    }
                    updateModelBoneMatrices(model);
                    accelerateModelBones(model);
                }
            }
        }
        waitFrames(FRAME_INTERVAL);
    }
}
