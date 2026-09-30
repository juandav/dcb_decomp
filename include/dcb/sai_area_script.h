#ifndef DCB_SAI_AREA_SCRIPT_H
#define DCB_SAI_AREA_SCRIPT_H

#include "game.h"
#include "dcb/saiseg.h"

s32 SAI_stepAreaScript(ScriptRunner *runner);
ScriptRunner *SAI_createAreaScript(void);
s32 *SAI_allocScriptRegisters(s32 count);

#endif /* DCB_SAI_AREA_SCRIPT_H */
