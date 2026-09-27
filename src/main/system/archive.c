#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/archive.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"

void *findPakChunk(Chunk *cursor, s32 id, s32 sub) {
    Chunk *header;

    if (cursor == 0) {
        return 0;
    }
    for (;;) {
        header = cursor++;
        if (header->id < 0) {
            return 0;
        }
        if (header->id == id && header->sub == sub) {
            return cursor;
        }
        cursor = (Chunk *)((u8 *)cursor + header->size);
    }
}

void truncatePakAtChunk(Chunk *cursor, s32 id, s32 sub) {
    Chunk *pak;
    Chunk *header;

    pak = cursor;
    if (cursor == 0) {
        return;
    }
    for (;;) {
        header = cursor++;
        if (header->id < 0) {
            return;
        }
        if (header->id == id && (sub < 0 || header->sub == sub)) {
            header->id = -1;
            shrinkHeapBlock(pak, (u8 *)cursor - (u8 *)pak);
            return;
        }
        cursor = (Chunk *)((u8 *)cursor + header->size);
    }
}

void truncatePakTextures(Chunk *pak) {
    truncatePakAtChunk(pak, 5, -1);
}
