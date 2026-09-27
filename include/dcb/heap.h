#ifndef DCB_HEAP_H
#define DCB_HEAP_H

#include "game.h"

extern s32 HEAP_ARENA;
extern s32 HEAP_BLOCKS;

void resetHeap(s32 initialize);
s32 freeHeapBlock(void *ptr);
s32 freeHeapBlocksByTag(s32 tag);
void *allocPermanentHeapBlock(s32 size);
s32 getLargestFreeHeapBlock(void);
void *allocHeapBlock(s32 size, s32 ownerTag);
void releaseHeapBlock(void *ptr);
s32 computeVectorAngle(s32 y, s32 x);
void *shrinkHeapBlock(void *ptr, s32 size);
void *allocTaskHeapBlock(s32 size);
s32 freeHeapBlocksByTag(s32);

#endif /* DCB_HEAP_H */
