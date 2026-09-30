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
void runDeckEditorFromFriendMenu(s32 *param);
void runPartnerEquipmentFromFriendMenu(s32 *param);

#endif /* DCB_GAME_FLOW_H */
