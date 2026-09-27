#ifndef DCB_MODEL_LOAD_H
#define DCB_MODEL_LOAD_H

#include "game.h"
#include "dcb/model.h"

void relocateOmdObjects(Tmd18 *tmd);
void linkOmdObject(s32 tmd, void *obj, s32 index);
s32 *readModelBonePositions(u8 *model, s32 *data);
void unloadModel(s32 slot);
void unloadAllModels(void);
void *findLoadedModelById(s32 id);
s32 reuseLoadedModelTexture(u8 *);
void initModelBoneHierarchy(Model *model);
s32 loadModel(s32 slot, s32 id, s32 vramSlot, s32 pak, s8 format);
void loadOmdModelFromDisc(s32 slot, s32 id, s32 vramSlot);
void loadTmdModelFromDisc(s32 slot, s32 id, s32 vramSlot);

#endif /* DCB_MODEL_LOAD_H */
