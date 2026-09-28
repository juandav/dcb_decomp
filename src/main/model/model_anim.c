#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/model_anim.h"
#include "dcb/main.h"
#include "dcb/task.h"

void applyRootMotion(u8 *model) {
    u8 *bones;
    SVECTOR offset;
    MATRIX mat;
    VECTOR worldOffset;
    s32 flag;

    bones = model + 0xD80;
    if (*(s16 *)(bones + 0x52) == 0) {
        return;
    }
    offset.vy = 0;
    offset.vx = 0;
    offset.vz = *(s16 *)(bones + 0x52);
    PushMatrix();
    mat.t[2] = 0;
    mat.t[1] = 0;
    mat.t[0] = 0;
    gte_SetTransMatrix(&mat);
    RotMatrix(model + 0xA78, &mat);
    gte_SetRotMatrix(&mat);
    gte_ldv0(&offset);
    gte_rtv0tr();
    gte_stlvnl(&worldOffset);
    gte_stflg(&flag);
    *(s16 *)(bones + 0x52) = 0;
    *(s32 *)(model + 8) += worldOffset.vx;
    *(s32 *)(model + 0xC) += worldOffset.vy;
    *(s32 *)(model + 0x10) += worldOffset.vz;
    PopMatrix();
}

void setupRotationCurve(s32 *chan, s32 span, s32 nextSpan, s32 halfSpan, s32 key, s32 nextKey, s32 afterKey) {
    s32 start;
    s32 end;
    s32 slope;
    s32 endVelocity;
    s32 midVelocity;

    start = key << 0x14;
    chan[0] = start;
    end = nextKey << 0x14;
    midVelocity = end - start;
    slope = midVelocity / span;
    endVelocity = (((afterKey - nextKey) << 0x14) / nextSpan + slope) / 2;
    midVelocity = slope * 2 - (endVelocity + chan[1]) / 2;
    chan[2] = (midVelocity - chan[1]) / halfSpan;
    chan[3] = (endVelocity - midVelocity) / halfSpan;
}

void setupTranslationCurve(s32 *chan, s32 span, s32 nextSpan, s32 halfSpan, s32 key, s32 nextKey, s32 afterKey) {
    s32 endVelocity;
    s32 velocity;

    velocity = key << 0x10;
    chan[0] = velocity;
    velocity = ((nextKey << 0x10) - velocity) / span;
    endVelocity = (((afterKey - nextKey) << 0x10) / nextSpan + velocity) / 2;
    velocity = velocity * 2 - (endVelocity + chan[1]) / 2;
    chan[2] = (velocity - chan[1]) / halfSpan;
    chan[3] = (endVelocity - velocity) / halfSpan;
}

void accelerateModelBones(u8 *model) {
    BoneAnim *bone;
    s32 i;
    s32 *animState;

    bone = (BoneAnim *)(model + 0xD80);
    animState = (s32 *)(model + 0x2200);
    animState[4]--;
    i = 0;
    if (*(s32 *)(model + 0x26D8) != 0) {
        i = *(s16 *)(model + 4);
        bone += i;
    }
    for (; i < *(s16 *)(model + 4) + 1; bone++, i++) {
        if (animState[4] >= 0) {
                bone->ch[0].val += bone->ch[0].d0;
                bone->ch[1].val += bone->ch[1].d0;
                bone->ch[2].val += bone->ch[2].d0;
                bone->ch[3].val += bone->ch[3].d0;
                bone->ch[4].val += bone->ch[4].d0;
                bone->ch[5].val += bone->ch[5].d0;
                bone->ch[6].val += bone->ch[6].d0;
                bone->ch[7].val += bone->ch[7].d0;
                bone->ch[8].val += bone->ch[8].d0;
        } else {
                bone->ch[0].val += bone->ch[0].d1;
                bone->ch[1].val += bone->ch[1].d1;
                bone->ch[2].val += bone->ch[2].d1;
                bone->ch[3].val += bone->ch[3].d1;
                bone->ch[4].val += bone->ch[4].d1;
                bone->ch[5].val += bone->ch[5].d1;
                bone->ch[6].val += bone->ch[6].d1;
                bone->ch[7].val += bone->ch[7].d1;
                bone->ch[8].val += bone->ch[8].d1;
        }
    }
}

s32 updateModelBoneMatrices(void *model) {
    s32 scaleOffset;
    s32 lastValue;
    s32 boneIndex;
    s32 angleX;
    s32 angleY;
    s32 angleZ;
    void *matrix;
    void *scaleEntry;
    void *bone;
    void *coord;
    void *rot;

    bone = model + 0xD80;
    coord = model + 0x78;
    rot = model + 0xA80;
    boneIndex = 0;
    if ((*(s16 *)((s8 *)model + 4)) > 0) {
        do {
            angleX = (*(s32 *)((s8 *)bone + 0));
            if (angleX < 0) {
                angleX += 0xFFFFF;
            }
            (*(s16 *)((s8 *)rot + 0)) = (s16) (angleX >> 0x14);
            angleY = (*(s32 *)((s8 *)bone + 0x10));
            if (angleY < 0) {
                angleY += 0xFFFFF;
            }
            (*(s16 *)((s8 *)rot + 2)) = (s16) (angleY >> 0x14);
            angleZ = (*(s32 *)((s8 *)bone + 0x20));
            if (angleZ < 0) {
                angleZ += 0xFFFFF;
            }
            (*(s16 *)((s8 *)rot + 4)) = (s16) (angleZ >> 0x14);
            (*(s32 *)((s8 *)coord + 0x18)) = (s32) ((*(s16 *)((s8 *)bone + 0x32)) + (*(s16 *)((s8 *)((Unk1F80 *)model)->unk1F80[boneIndex] + 0)));
            (*(s32 *)((s8 *)coord + 0x1C)) = (s32) ((*(s16 *)((s8 *)bone + 0x42)) + (*(s16 *)((s8 *)((Unk1F80 *)model)->unk1F80[boneIndex] + 2)));
            (*(s32 *)((s8 *)coord + 0x20)) = (s32) ((*(s16 *)((s8 *)bone + 0x52)) + (*(s16 *)((s8 *)((Unk1F80 *)model)->unk1F80[boneIndex] + 4)));
            scaleOffset = boneIndex * 0x10;
            scaleEntry = model + scaleOffset;
            (*(s32 *)((s8 *)scaleEntry + 0x2000)) = (s32) (*(s16 *)((s8 *)bone + 0x62));
            (*(s32 *)((s8 *)scaleEntry + 0x2004)) = (s32) (*(s16 *)((s8 *)bone + 0x72));
            (*(s32 *)((s8 *)scaleEntry + 0x2008)) = (s32) (*(s16 *)((s8 *)bone + 0x82));
            matrix = coord + 4;
            RotMatrixYXZ(rot, matrix);
            (*(s32 *)((s8 *)coord + 0)) = 0;
            ScaleMatrix(matrix, model + (scaleOffset + 0x2000));
            (*(s32 *)((s8 *)bone + 0)) = (s32) ((*(s32 *)((s8 *)bone + 0)) + (*(s32 *)((s8 *)bone + 4)));
            (*(s32 *)((s8 *)bone + 0x10)) = (s32) ((*(s32 *)((s8 *)bone + 0x10)) + (*(s32 *)((s8 *)bone + 0x14)));
            (*(s32 *)((s8 *)bone + 0x20)) = (s32) ((*(s32 *)((s8 *)bone + 0x20)) + (*(s32 *)((s8 *)bone + 0x24)));
            (*(s32 *)((s8 *)bone + 0x30)) = (s32) ((*(s32 *)((s8 *)bone + 0x30)) + (*(s32 *)((s8 *)bone + 0x34)));
            (*(s32 *)((s8 *)bone + 0x40)) = (s32) ((*(s32 *)((s8 *)bone + 0x40)) + (*(s32 *)((s8 *)bone + 0x44)));
            (*(s32 *)((s8 *)bone + 0x50)) = (s32) ((*(s32 *)((s8 *)bone + 0x50)) + (*(s32 *)((s8 *)bone + 0x54)));
            (*(s32 *)((s8 *)bone + 0x60)) = (s32) ((*(s32 *)((s8 *)bone + 0x60)) + (*(s32 *)((s8 *)bone + 0x64)));
            (*(s32 *)((s8 *)bone + 0x70)) = (s32) ((*(s32 *)((s8 *)bone + 0x70)) + (*(s32 *)((s8 *)bone + 0x74)));
            (*(s32 *)((s8 *)bone + 0x80)) = (s32) ((*(s32 *)((s8 *)bone + 0x80)) + (*(s32 *)((s8 *)bone + 0x84)));
            boneIndex += 1;
            bone += 0x90;
            coord += 0x50;
            rot += 8;
        } while (boneIndex < (*(s16 *)((s8 *)model + 4)));
    }
    (*(s32 *)((s8 *)bone + 0)) = (s32) ((*(s32 *)((s8 *)bone + 0)) + (*(s32 *)((s8 *)bone + 4)));
    (*(s32 *)((s8 *)bone + 0x10)) = (s32) ((*(s32 *)((s8 *)bone + 0x10)) + (*(s32 *)((s8 *)bone + 0x14)));
    (*(s32 *)((s8 *)bone + 0x20)) = (s32) ((*(s32 *)((s8 *)bone + 0x20)) + (*(s32 *)((s8 *)bone + 0x24)));
    (*(s32 *)((s8 *)bone + 0x30)) = (s32) ((*(s32 *)((s8 *)bone + 0x30)) + (*(s32 *)((s8 *)bone + 0x34)));
    (*(s32 *)((s8 *)bone + 0x40)) = (s32) ((*(s32 *)((s8 *)bone + 0x40)) + (*(s32 *)((s8 *)bone + 0x44)));
    (*(s32 *)((s8 *)bone + 0x50)) = (s32) ((*(s32 *)((s8 *)bone + 0x50)) + (*(s32 *)((s8 *)bone + 0x54)));
    (*(s32 *)((s8 *)bone + 0x60)) = (s32) ((*(s32 *)((s8 *)bone + 0x60)) + (*(s32 *)((s8 *)bone + 0x64)));
    (*(s32 *)((s8 *)bone + 0x70)) = (s32) ((*(s32 *)((s8 *)bone + 0x70)) + (*(s32 *)((s8 *)bone + 0x74)));
    lastValue = (*(s32 *)((s8 *)bone + 0x80)) + (*(s32 *)((s8 *)bone + 0x84));
    (*(s32 *)((s8 *)bone + 0x80)) = lastValue;
    return lastValue;
}

INCLUDE_ASM("asm/main/nonmatchings/model/model_anim", loadNextAnimationKeyframe);

void runModelAnimationTask(void) {
    s32 timer;
    s32 slot;
    void *model;
    void *animState;

    SCENE_3D_ENABLED = 1;
    slot = 0;
loop_1:
    model = SCENE_3D->models[slot];
    if (SCENE_3D->modelState[slot] > 0) {
        animState = model + 0x2200;
        if ((*(s32 *)((s8 *)model + 0x2208)) >= 0) {
            timer = (*(s32 *)((s8 *)animState + 8)) - 1;
            (*(s32 *)((s8 *)animState + 8)) = timer;
            if (timer <= 0) {
                loadNextAnimationKeyframe(model, (*(s32 *)((s8 *)animState + 0x18)), 0);
            }
            updateModelBoneMatrices(model);
            accelerateModelBones(model);
        }
    }
    slot += 1;
    if (slot < 0x18) {
        goto loop_1;
    }
    slot = 0;
    func_80014C08(FRAME_INTERVAL);
    goto loop_1;
}
