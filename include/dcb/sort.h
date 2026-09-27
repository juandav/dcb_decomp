#ifndef DCB_SORT_H
#define DCB_SORT_H

#include "game.h"
#include "dcb/archive.h"

void swapBytes(s8 *a, s8 *b, s32 size);
void sortArray(s8 *base, u32 n, s32 size, s32 (*cmp)(s8 *, s8 *));

#endif /* DCB_SORT_H */
