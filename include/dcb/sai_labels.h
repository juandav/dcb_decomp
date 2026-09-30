#ifndef DCB_SAI_LABELS_H
#define DCB_SAI_LABELS_H

#include "game.h"
#include "dcb/saiseg.h"

void SAI_drawAreaName(void);
void SAI_drawLocationLabel(void);
void SAI_openWindows(UiWindow *window, WindowDef *def, s32 count);
void SAI_toggleMessageWindow(s32 close);
void SAI_createAreaName(void);
void SAI_createLocationLabel(void);
void SAI_runChoiceScript(ScriptRunner *runner);

#endif /* DCB_SAI_LABELS_H */
