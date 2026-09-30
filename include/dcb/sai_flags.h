#ifndef DCB_SAI_FLAGS_H
#define DCB_SAI_FLAGS_H

#include "game.h"

void SAI_clearOpponents(void);
void SAI_unlockArmorsFromFlags(s32 *regs);
void SAI_loadScriptFlags(void);
void SAI_saveScriptFlags(void);
void SAI_addOpponent(s32 opponent);
void func_801E09F4(void);
void SAI_setPartnerObtainedFlag(s32 index);

#endif /* DCB_SAI_FLAGS_H */
