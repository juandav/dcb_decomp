#ifndef DCB_ARCHIVE_H
#define DCB_ARCHIVE_H

#include "game.h"

extern s32 BITSTREAM_BITS_LEFT;
extern u32 BITSTREAM_BYTE;
extern u8 *BITSTREAM_SRC;
extern s32 HUFFMAN_NEXT_NODE;
extern s32 HUFFMAN_LEFT;
extern s32 HUFFMAN_RIGHT;
extern s32 HUFFMAN_SYMBOLS_DECODED;
extern u8 *DECOMPRESS_DST;
extern u8 LZ_WINDOW[0x1000];

void *findPakChunk(Chunk *cursor, s32 id, s32 sub);
void truncatePakAtChunk(Chunk *cursor, s32 id, s32 sub);
void truncatePakTextures(Chunk *pak);
s32 readBitstreamBit(void);
u32 readBitstreamBits(s32 bitCount);
s32 readHuffmanTree(void);
s32 decompressForTask(s32 src);
s32 decompressArchiveEntry(s32 archive, s32 index);
void decompressLzHuffman(u32 outputSize);
s32 decompressToHeap(s32 src, s32 heapTag);
void swapBytes(s8 *a, s8 *b, s32 size);
void sortArray(s8 *base, u32 n, s32 size, s32 (*cmp)(s8 *, s8 *));

#endif /* DCB_ARCHIVE_H */
