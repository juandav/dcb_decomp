#include "dcb/save_checksum.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/memcard.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"

s32 verifySaveChecksum(s32 len, u8 *data) {
    s32 i;
    u8 xorSum = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        xorSum ^= *data;
        sum += *data;
        data++;
    }
    if (data[0] != xorSum || data[1] != sum) {
        return 1;
    }
    return 0;
}

void writeSaveChecksum(s32 len, u8 *data) {
    s32 i;
    u8 xorSum = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        xorSum ^= *data;
        sum += *data;
        data++;
    }
    data[0] = xorSum;
    data[1] = sum;
}
