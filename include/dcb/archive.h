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

#endif /* DCB_ARCHIVE_H */
