#ifndef DCB_EVO_FUSION_SCRIPT_H
#define DCB_EVO_FUSION_SCRIPT_H

#include "game.h"
#include "dcb/evoseg.h"

EvoProgram *EVO_loadUnitScript(s32 index);
s32 *EVO_allocScriptRegisters(s32 count);
s32 EVO_tickFusionScript(EvoProgram *program);

#endif /* DCB_EVO_FUSION_SCRIPT_H */
