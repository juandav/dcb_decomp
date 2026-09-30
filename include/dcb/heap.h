#ifndef DCB_HEAP_H
#define DCB_HEAP_H

#include "game.h"

/* One entry of the heap's block table. The table lists the arena's blocks in
   address order and ends at the first entry whose addr is 0. */
typedef struct {
    /* 0x0 */ s32 addr; /* the block's address; bit 31 set while it's in use */
    /* 0x4 */ s32 size;
    /* 0x8 */ s32 tag;  /* -1 free, -2 permanent, else the owning task's id */
} HeapBlock;

extern s32 HEAP_ARENA;
extern HeapBlock HEAP_BLOCKS[0x400];

void resetHeap(s32 initialize);
s32 freeHeapBlock(void *ptr);
s32 freeHeapBlocksByTag(s32 tag);
void *allocPermanentHeapBlock(s32 size);
s32 getLargestFreeHeapBlock(void);
void *allocHeapBlock(s32 size, s32 ownerTag);
void releaseHeapBlock(void *ptr);
void *shrinkHeapBlock(void *ptr, s32 size);
void *allocTaskHeapBlock(s32 size);
s32 freeHeapBlocksByTag(s32);

#endif /* DCB_HEAP_H */
