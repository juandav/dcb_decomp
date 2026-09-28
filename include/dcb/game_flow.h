#ifndef DCB_GAME_FLOW_H
#define DCB_GAME_FLOW_H

#include "game.h"
#include "dcb/stage.h"

void runTitleMenu(void);
void openSaveScreenFromMap(s32 saveMode);
void runTitleMenu();
void continueSavedGame(void);
void openPartnerFusion(s8 mode);
void returnToWorldMap(void);
void openDeckEditor(s32 returnTo);
void openPartnerEquipment(s32 returnTo);
void func_8002F298(s32 *param);
void func_8002F3C4(s32 *param);

#endif /* DCB_GAME_FLOW_H */
