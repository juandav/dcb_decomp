#ifndef DCB_MODEL_LOAD_H
#define DCB_MODEL_LOAD_H

#include "game.h"
#include "dcb/anim_control.h"

void relocateOmdObjects(Tmd18 *tmd);
void linkOmdObject(s32 tmd, GsDOBJ4 *obj, s32 index);
s32 *readModelBonePositions(Model *model, s32 *data);
void unloadModel(s32 slot);
void unloadAllModels(void);
Model *findLoadedModelById(s32 id);
s32 reuseLoadedModelTexture(Model *model);
void initModelBoneHierarchy(Model *model);
#if VERSION_JP
s32 loadModel(s32 slot, s32 id, s32 vramSlot, s32 pak);
#elif VERSION_US || VERSION_EU
s32 loadModel(s32 slot, s32 id, s32 vramSlot, s32 pak, s8 format);
#endif
void loadOmdModelFromDisc(s32 slot, s32 id, s32 vramSlot);
#if VERSION_US || VERSION_EU
void loadTmdModelFromDisc(s32 slot, s32 id, s32 vramSlot);
#endif

#endif /* DCB_MODEL_LOAD_H */
