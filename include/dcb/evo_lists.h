#ifndef DCB_EVO_LISTS_H
#define DCB_EVO_LISTS_H

#include "game.h"

void EVO_openPartnerList(void);
void EVO_closePartnerList(void);
void EVO_openCardList(void);
void EVO_closeCardList(void);
void EVO_tickPartnerList(void);
void EVO_tickCardList(void);
void EVO_showBothTrays(void);
void EVO_repickSecondCard(void);
void EVO_slideTrayOut(s32 index);
void EVO_slideTrayIn(s32 index);

#endif /* DCB_EVO_LISTS_H */
