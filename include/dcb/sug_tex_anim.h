#ifndef DCB_SUG_TEX_ANIM_H
#define DCB_SUG_TEX_ANIM_H

#include "game.h"
#include "dcb/sugseg.h"

extern const Rect16 SUG_MODEL_CLUT_RECT;

/* main's createRingEffect, renderRingEffect and freeRingEffect call these three
   through their addresses (func_801E6C78, func_801E7020, func_801E72D4 in game.h) */
s32 SUG_startTexAnim(s32 id, s32 kind, RingEffect *owner, TexAnim *anim, s32 pak);

void SUG_tickTexAnim(TexAnim *anim);
void SUG_freeTexAnim(TexAnim *anim);
void SUG_initTamCache(void);
void SUG_freeTamCache(void);

#endif /* DCB_SUG_TEX_ANIM_H */
