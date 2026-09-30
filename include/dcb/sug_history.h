#ifndef DCB_SUG_HISTORY_H
#define DCB_SUG_HISTORY_H

#include "game.h"
#include "dcb/sugseg.h"

void SUG_freePosHistory(void **obj);
PosHistory *SUG_createPosHistory(s32 columns, s32 rows);
void SUG_pushPosHistory(PosHistory *table, Quad *quad, Short4 *s4);
void SUG_getPosHistory(PosHistory *table, s32 back, Quad *quad, Short4 *s4);

#endif /* DCB_SUG_HISTORY_H */
