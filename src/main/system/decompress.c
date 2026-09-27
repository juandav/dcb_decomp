#include "dcb/decompress.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/archive.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"

s32 readBitstreamBit(void) {
    if (--BITSTREAM_BITS_LEFT >= 0) {
        return (BITSTREAM_BYTE >> BITSTREAM_BITS_LEFT) & 1;
    }
    BITSTREAM_BITS_LEFT = 7;
    BITSTREAM_BYTE = *BITSTREAM_SRC++;
    return BITSTREAM_BYTE >> 7;
}

u32 readBitstreamBits(s32 bitCount) {
    u16 value;

    value = 0;
    while (BITSTREAM_BITS_LEFT < bitCount) {
        bitCount -= BITSTREAM_BITS_LEFT;
        value |= (BITSTREAM_BYTE & ((1 << BITSTREAM_BITS_LEFT) - 1)) << bitCount;
        BITSTREAM_BYTE = *BITSTREAM_SRC++;
        BITSTREAM_BITS_LEFT = 8;
    }
    BITSTREAM_BITS_LEFT -= bitCount;
    return value | ((BITSTREAM_BYTE >> BITSTREAM_BITS_LEFT) & ((1 << bitCount) - 1));
}

s32 readHuffmanTree(void) {
    s32 node;

    if (readBitstreamBit() != 0) {
        node = HUFFMAN_NEXT_NODE++;
        if (node >= 0x21F) {
            return -1;
        }
        (&HUFFMAN_LEFT)[node] = readHuffmanTree();
        (&HUFFMAN_RIGHT)[node] = readHuffmanTree();
    } else {
        node = readBitstreamBits(9);
    }
    return node;
}

void decompressLzHuffman(u32 outputSize) {
    s32 windowPos;
    s32 i;
    s32 k;
    u32 written;
    s32 root;
    s32 symbol;
    s32 offset;
    u8 byte;

    HUFFMAN_SYMBOLS_DECODED = 0x1000;
    written = 0;
    root = 0;
    windowPos = 0xFEE;
    for (k = 0; k < windowPos; k++) {
        LZ_WINDOW[k] = 0;
    }
    while (written < outputSize) {
        if (HUFFMAN_SYMBOLS_DECODED == 0x1000) {
            HUFFMAN_NEXT_NODE = 0x110;
            root = readHuffmanTree();
            HUFFMAN_SYMBOLS_DECODED = 0;
        }
        symbol = root;
        while (symbol >= 0x110) {
            if (readBitstreamBit() != 0) {
                symbol = (&HUFFMAN_RIGHT)[symbol];
            } else {
                symbol = (&HUFFMAN_LEFT)[symbol];
            }
        }
        HUFFMAN_SYMBOLS_DECODED++;
        if (symbol < 0x100) {
            *DECOMPRESS_DST++ = symbol;
            LZ_WINDOW[windowPos] = symbol;
            windowPos++;
            windowPos &= 0xFFF;
            written++;
        } else {
            symbol -= 0xFD;
            offset = readBitstreamBits(12);
            for (i = 0; i < symbol; i++) {
                byte = LZ_WINDOW[(offset + i) & 0xFFF];
                *DECOMPRESS_DST++ = byte;
                LZ_WINDOW[windowPos] = byte;
                windowPos++;
            windowPos &= 0xFFF;
            }
            written += symbol;
        }
    }
}

s32 decompressArchiveEntry(s32 archive, s32 index) {
    return decompressForTask(archive + ((s32 *)archive)[index]);
}

s32 decompressToHeap(s32 src, s32 heapTag) {
    s32 sizeHigh;
    s32 size;
    s32 dst;

    BITSTREAM_BITS_LEFT = 0;
    BITSTREAM_BYTE = 0;
    BITSTREAM_SRC = (u8 *)src;
    sizeHigh = readBitstreamBits(0x10);
    size = (sizeHigh << 0x10) | readBitstreamBits(0x10);
    dst = allocHeapBlock(size, heapTag);
    DECOMPRESS_DST = (u8 *)dst;
    decompressLzHuffman(size);
    return dst;
}

s32 decompressForTask(s32 src) {
    return decompressToHeap(src, getCurrentTaskId());
}
