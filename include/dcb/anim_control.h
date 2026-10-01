#ifndef DCB_ANIM_CONTROL_H
#define DCB_ANIM_CONTROL_H

#include "game.h"

typedef struct {
    /* 0x00 */ s32 unk0[5];
    /* 0x14 */ s32 unk14;
} Obj18;
typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 flags;
    /* 0x08 */ s32 nobj;
    /* 0x0C */ Obj18 obj[1];
} Tmd18;
typedef struct {
    s32 key;
    s32 value;
} KeyValue;

void initModelScene(void);
void pauseModelAnimation(s32 slot);
void resumeModelAnimation(s32 slot);
void unloadModelAnimations(s32 slot);
void unloadEffectAnimations(void);
s32 findAnimationCacheEntry(s32 key, s32 count, KeyValue **freeEntry);
s32 loadAnimationData(s32 id, s32 anim, s32 slot, Chunk *pak);
void setModelAnimationData(Model *model, s32 *data, s32 anim);
s32 loadModelAnimation(s32 slot, s32 anim, s32 index, s32 pak);
s32 loadModelAnimationFile(s32 slot, s32 anim, s32 index);
s32 startModelAnimation(s32 slot, s32 anim, s32 loopKey, s32 rootOnly);
void applyAnimationFirstFrame(s32 slot, s32 anim);

#endif /* DCB_ANIM_CONTROL_H */
