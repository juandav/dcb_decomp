#ifndef DCB_SUG_TRAIL_H
#define DCB_SUG_TRAIL_H

#include "game.h"
#include "dcb/scroll_bg.h"
#include "dcb/sugseg.h"

#if VERSION_JP
/* jp's trails take no pak, and a byte for the texture animation */
TrailEffect *SUG_createTrailEffect(s16 brightness, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, EffectTemplate *template, s16 x0, s16 x1,
                           s32 count, s16 rows, u8 followMode, u8 colorMode, u8 semiTrans, u8 blend, u8 primKind, u8 texAnimId, Rect16 *uv, s32 tpage, s32 clut,
                           s32 otz);
#elif VERSION_US || VERSION_EU
TrailEffect *SUG_createTrailEffect(s16 brightness, Bytes4 *c0, Bytes4 *c1, Bytes4 *c2, Bytes4 *c3, EffectTemplate *template, s16 x0, s16 x1,
                           s32 count, s16 rows, u8 followMode, u8 colorMode, u8 semiTrans, u8 blend, u8 primKind, s32 texAnimId, Rect16 *uv, s32 tpage, s32 clut,
                           s32 otz, s32 pak);
#endif

void SUG_tickTrailEffect(TrailEffect *obj);
void SUG_freeTrailEffect(TrailEffect *obj);

#endif /* DCB_SUG_TRAIL_H */
