#ifndef DCB_MODEL_ANIM_H
#define DCB_MODEL_ANIM_H

#include "game.h"

/* A bone in a key of an animation clip */
typedef struct {
    /* 0x00 */ s16 rx;
    /* 0x02 */ s16 ry;
    /* 0x04 */ s16 rz;
    /* 0x06 */ s16 attribute; /* copied to the bone's GsDOBJ4 */
    /* 0x08 */ s16 tx;
    /* 0x0A */ s16 ty;
    /* 0x0C */ s16 tz;
    /* 0x0E */ s16 padE;
    /* 0x10 */ s16 sx;
    /* 0x12 */ s16 sy;
    /* 0x14 */ s16 sz;
    /* 0x16 */ s16 pad16; /* in bone[0] of the first key: the clip's loop key */
} KeyBone;

/* A key of an animation clip: AnimClip.data is an array of them */
typedef struct {
    /* 0x00 */ s16 duration; /* in frames, halved */
    /* 0x02 */ s16 sound;    /* sound effect + 1, or 0 */
    /* 0x04 */ KeyBone bone[32];
} KeyFrame;

void applyRootMotion(Model *model);
void setupRotationCurve(AnimChan *chan, s32 length, s32 nextLength, s32 halfLength, s32 from, s32 to, s32 next);
void setupTranslationCurve(AnimChan *chan, s32 length, s32 nextLength, s32 halfLength, s32 from, s32 to, s32 next);
void accelerateModelBones(Model *model);
s32 updateModelBoneMatrices(Model *model);
s32 loadNextAnimationKeyframe(Model *model, s32 loopKey, s32 mode);
void runModelAnimationTask(void);

#endif /* DCB_MODEL_ANIM_H */
