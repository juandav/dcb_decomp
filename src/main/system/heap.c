#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/main.h"
#include "dcb/task.h"

void resetHeap(s32 initialize) {
    s32 *block;
    s32 i;

    if (initialize != 0) {
        block = &HEAP_BLOCKS;
        block[0] = (s32)&HEAP_ARENA & 0x3FFFFFFF;
        block[1] = 0x148000;
        block[2] = -1;
        i = 0x3FF;
        do {
            block += 3;
            block[0] = 0;
            block[1] = 0;
            i--;
            block[2] = 0;
        } while (i > 0);
        return;
    }
    block = &HEAP_BLOCKS;
    i = 0x3FF;
    if (block[0] != 0) {
loop:
        if (block[0] < 0 && block[2] >= 0) {
            if (freeHeapBlock((void *)block[0]) == 0) {
                goto loop;
            }
        }
        i--;
        block += 3;
        if (i >= 0 && block[0] != 0) {
            goto loop;
        }
    }
}

s32 getLargestFreeHeapBlock(void) {
    s32 *block;
    s32 i;
    s32 largest;

    largest = 0;
    block = &HEAP_BLOCKS;
    i = 0x3FF;
    if (HEAP_BLOCKS != 0) {
        do {
            if (block[0] > 0 && largest < block[1]) {
                largest = block[1];
            }
            i--;
            block += 3;
        } while (i >= 0 && block[0] != 0);
    }
    return largest;
}

void *allocHeapBlock(s32 size, s32 ownerTag) {
    s32 *block;
    s32 *shiftBlock;
    s32 i;
    s32 blockAddr;
    s32 freeSize;
    s32 ptr;

    size = (size + 3) & ~3;
    if (size == 0) {
        return 0;
    }
    func_80014970();
    block = &HEAP_BLOCKS;
    i = 0x3FF;
    if ((blockAddr = HEAP_BLOCKS) != 0) {
        do {
            if (blockAddr >= 0) {
                freeSize = block[1];
                if (freeSize >= size) {
                    ptr = blockAddr | 0x80000000;
                    block[0] = ptr;
                    block[1] = size;
                    freeSize -= size;
                    block[2] = ownerTag;
                    if (freeSize != 0) {
                        blockAddr += size;
                        shiftBlock = &HEAP_BLOCKS + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            shiftBlock[0] = shiftBlock[-3];
                            shiftBlock[1] = shiftBlock[-2];
                            shiftBlock[2] = shiftBlock[-1];
                            shiftBlock -= 3;
                        }
                        shiftBlock[0] = blockAddr;
                        shiftBlock[1] = freeSize;
                        shiftBlock[2] = -1;
                    }
                    func_800149A0();
                    return (void *)ptr;
                }
            }
            i--;
            block += 3;
        } while (i >= 0 && (blockAddr = block[0]) != 0);
    }
    func_800149A0();
    return 0;
}

void *allocPermanentHeapBlock(s32 size) {
    return allocHeapBlock(size, -2);
}

void *allocTaskHeapBlock(s32 size) {
    return allocHeapBlock(size, getCurrentTaskId());
}

void *shrinkHeapBlock(void *ptr, s32 size) {
    s32 *block;
    s32 i;
    s32 blockAddr;
    s32 leftover;

    size = (size + 3) & ~3;
    func_80014970();
    block = &HEAP_BLOCKS;
    i = 0x3FF;
    if ((blockAddr = HEAP_BLOCKS) != 0) {
        do {
            if (blockAddr == (s32)ptr) {
                leftover = block[1] - size;
                if (leftover < 0) {
                    func_800149A0();
                    return 0;
                }
                if (leftover != 0 && i != 0) {
                    block[1] = size;
                    block += 3;
                    blockAddr += size;
                    if (block[0] > 0) {
                        leftover += block[1];
                    } else {
                        block = &HEAP_BLOCKS + 0x3FF * 3;
                        for (i--; i > 0; i--) {
                            block[0] = block[-3];
                            block[1] = block[-2];
                            block[2] = block[-1];
                            block -= 3;
                        }
                    }
                    block[0] = blockAddr & 0x3FFFFFFF;
                    block[1] = leftover;
                    block[2] = -1;
                }
                func_800149A0();
                return ptr;
            }
            i--;
            block += 3;
        } while (i >= 0 && (blockAddr = block[0]) != 0);
    }
    func_800149A0();
    return 0;
}

void releaseHeapBlock(void *ptr) {
    freeHeapBlock(ptr);
}

s32 freeHeapBlock(void *ptr) {
    s32 *block;
    s32 *nextBlock;
    s32 i;
    s32 blockAddr;
    s32 size;

    if (ptr == 0) {
        return 0;
    }
    func_80014970();
    block = &HEAP_BLOCKS;
    i = 0x3FF;
    if ((blockAddr = HEAP_BLOCKS) != 0) {
        do {
            if (blockAddr == (s32)ptr) {
                blockAddr &= 0x3FFFFFFF;
                size = block[1];
                nextBlock = block;
                if (block != &HEAP_BLOCKS && block[-3] > 0) {
                    block -= 3;
                    blockAddr = block[0];
                    size += block[1];
                    i++;
                }
                if (i > 0 && nextBlock[3] > 0) {
                    if (nextBlock != block) {
                        i--;
                    }
                    nextBlock += 3;
                    size += nextBlock[1];
                }
                block[0] = blockAddr;
                block[1] = size;
                block[2] = -1;
                if (block != nextBlock) {
                    for (i--; i > 0; i--) {
                        block += 3;
                        nextBlock += 3;
                        block[0] = nextBlock[0];
                        block[1] = nextBlock[1];
                        block[2] = nextBlock[2];
                    }
                    while (block < nextBlock) {
                        block += 3;
                        block[0] = 0;
                        block[2] = 0;
                    }
                }
                func_800149A0();
                return 0;
            }
            i--;
            block += 3;
        } while (i >= 0 && (blockAddr = block[0]) != 0);
    }
    func_800149A0();
    return 0;
}

s32 freeHeapBlocksByTag(s32 tag) {
    s32 *block;
    s32 blockAddr;
    s32 blocksLeft;

    block = &HEAP_BLOCKS;
    blocksLeft = 0x3FF;
    if (HEAP_BLOCKS != 0) {
loop_1:
        blockAddr = (*(s32 *)((s8 *)block + 0));
        if ((blockAddr < 0) && ((*(s32 *)((s8 *)block + 8)) == tag)) {
            if (freeHeapBlock(blockAddr) == 0) {
                goto loop_1;
            }
        }
        blocksLeft -= 1;
        block += 3;
        if ((blocksLeft >= 0) && (*block != 0)) {
            goto loop_1;
        }
    }
    return 0;
}
