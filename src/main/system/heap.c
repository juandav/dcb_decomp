#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/main.h"
#include "dcb/task.h"

/* the bytes the heap hands out, from HEAP_ARENA on */
#if VERSION_JP
#define HEAP_SIZE 0x146F00
#elif VERSION_US || VERSION_EU
#define HEAP_SIZE 0x148000
#endif

/* initialize: make the whole arena one free block; otherwise free every block
   a task owns, keeping the permanent ones */
void resetHeap(s32 initialize) {
    HeapBlock *block;
    s32 i;

    if (initialize != 0) {
        block = HEAP_BLOCKS;
        /* free blocks keep their address without the KSEG0 bit, so they're > 0 */
        block->addr = (s32)&HEAP_ARENA & 0x3FFFFFFF;
        block->size = HEAP_SIZE;
        block->tag = -1;
        i = 0x3FF;
        do {
            block++;
            block->addr = 0;
            block->size = 0;
            i--;
            block->tag = 0;
        } while (i > 0);
        return;
    }
    block = HEAP_BLOCKS;
    for (i = 0x3FF; i >= 0 && block->addr != 0; i--, block++) {
        /* freeing merges entries: look at this one again */
        while (block->addr < 0 && block->tag >= 0) {
            if (freeHeapBlock((void *)block->addr) != 0) {
                break;
            }
        }
    }
}

s32 getLargestFreeHeapBlock(void) {
    HeapBlock *block;
    s32 i;
    s32 largest;

    largest = 0;
    block = HEAP_BLOCKS;
    for (i = 0x3FF; i >= 0 && block->addr != 0; i--, block++) {
        if (block->addr > 0 && largest < block->size) {
            largest = block->size;
        }
    }
    return largest;
}

/* first fit; what is left of the free block goes in a new entry after it */
void *allocHeapBlock(s32 size, s32 ownerTag) {
    HeapBlock *block;
    HeapBlock *shiftBlock;
    s32 i;
    s32 blockAddr;
    s32 freeSize;
    s32 ptr;

    size = (size + 3) & ~3;
    if (size == 0) {
        return 0;
    }
    disableInterrupts();
    block = HEAP_BLOCKS;
    for (i = 0x3FF; i >= 0 && (blockAddr = block->addr) != 0; i--, block++) {
        if (blockAddr >= 0) {
            freeSize = block->size;
            if (freeSize >= size) {
                ptr = blockAddr | 0x80000000;
                block->addr = ptr;
                block->size = size;
                freeSize -= size;
                block->tag = ownerTag;
                if (freeSize != 0) {
                    blockAddr += size;
                    /* shift the rest of the table down one entry */
                    shiftBlock = &HEAP_BLOCKS[0x3FF];
                    for (i--; i > 0; i--) {
                        *shiftBlock = shiftBlock[-1];
                        shiftBlock--;
                    }
                    shiftBlock->addr = blockAddr;
                    shiftBlock->size = freeSize;
                    shiftBlock->tag = -1;
                }
                restoreInterrupts();
                return (void *)ptr;
            }
        }
    }
    restoreInterrupts();
    return 0;
}

void *allocPermanentHeapBlock(s32 size) {
    return allocHeapBlock(size, -2);
}

void *allocTaskHeapBlock(s32 size) {
    return allocHeapBlock(size, getCurrentTaskId());
}

/* gives the end of the block back: to the next block if that one is free,
   else as a new free entry after it */
void *shrinkHeapBlock(void *ptr, s32 size) {
    HeapBlock *block;
    s32 i;
    s32 blockAddr;
    s32 leftover;

    size = (size + 3) & ~3;
    disableInterrupts();
    block = HEAP_BLOCKS;
    for (i = 0x3FF; i >= 0 && (blockAddr = block->addr) != 0; i--, block++) {
        if (blockAddr == (s32)ptr) {
            leftover = block->size - size;
            if (leftover < 0) {
                restoreInterrupts();
                return 0;
            }
            if (leftover != 0 && i != 0) {
                block->size = size;
                block++;
#if VERSION_JP || VERSION_EU
                blockAddr = (s32)ptr + size;
#elif VERSION_US
                blockAddr += size;
#endif
                if (block->addr > 0) {
                    leftover += block->size;
                } else {
                    block = &HEAP_BLOCKS[0x3FF];
                    for (i--; i > 0; i--) {
                        *block = block[-1];
                        block--;
                    }
                }
                block->addr = blockAddr & 0x3FFFFFFF;
                block->size = leftover;
                block->tag = -1;
            }
            restoreInterrupts();
            return ptr;
        }
    }
    restoreInterrupts();
    return 0;
}

void releaseHeapBlock(void *ptr) {
    freeHeapBlock(ptr);
}

/* merges the block with a free neighbour on either side, then moves the rest
   of the table up over the entries that merged */
s32 freeHeapBlock(void *ptr) {
    HeapBlock *block;
    HeapBlock *nextBlock;
    s32 i;
    s32 blockAddr;
    s32 size;

    if (ptr == 0) {
        return 0;
    }
    disableInterrupts();
    block = HEAP_BLOCKS;
    for (i = 0x3FF; i >= 0 && (blockAddr = block->addr) != 0; i--, block++) {
#if VERSION_EU
        /* compared through xor: eu masks ptr, not the equal blockAddr */
        if ((blockAddr ^ (s32)ptr) == 0) {
#elif VERSION_JP || VERSION_US
        if (blockAddr == (s32)ptr) {
#else
#error "main/system/heap: version not checked"
#endif
            blockAddr = (s32)ptr & 0x3FFFFFFF;
            size = block->size;
            nextBlock = block;
            if (block != HEAP_BLOCKS && block[-1].addr > 0) {
                block--;
                blockAddr = block->addr;
                size += block->size;
                i++;
            }
            if (i > 0 && nextBlock[1].addr > 0) {
                if (nextBlock != block) {
                    i--;
                }
                nextBlock++;
                size += nextBlock->size;
            }
            block->addr = blockAddr;
            block->size = size;
            block->tag = -1;
            if (block != nextBlock) {
                for (i--; i > 0; i--) {
                    block++;
                    nextBlock++;
                    *block = *nextBlock;
                }
                while (block < nextBlock) {
                    block++;
                    block->addr = 0;
                    block->tag = 0;
                }
            }
            restoreInterrupts();
            return 0;
        }
    }
    restoreInterrupts();
    return 0;
}

s32 freeHeapBlocksByTag(s32 tag) {
    HeapBlock *block;
    s32 blockAddr;
    s32 blocksLeft;

    block = HEAP_BLOCKS;
    for (blocksLeft = 0x3FF; blocksLeft >= 0 && block->addr != 0; blocksLeft--, block++) {
        /* freeing it merges the next block into this one: look at it again */
        while (block->addr < 0 && block->tag == tag) {
            if (freeHeapBlock((void *)block->addr) != 0) {
                break;
            }
        }
    }
    return 0;
}
