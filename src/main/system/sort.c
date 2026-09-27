#include "dcb/sort.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/archive.h"
#include "dcb/decompress.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"

void sortArray(s8 *base, u32 count, s32 size, s32 (*cmp)(s8 *, s8 *)) {
    u32 i;
    u32 j;
    u32 k;
    s8 *left;
    s8 *right;

    if (count < 2) {
        return;
    }
    left = base;
    if (count == 2) {
        right = left + size;
        if (cmp(left, right) > 0) {
            swapBytes(left, right, size);
        }
        return;
    }
    for (i = 0; i < count; i++, left += size) {
        right = left + size;
        for (j = i; j < count - 1; j++, right += size) {
            if (cmp(left, right) > 0) {
                for (k = i; k <= j; k++) {
                    swapBytes(base + size * k, right, size);
                }
            }
        }
    }
}

void swapBytes(s8 *a, s8 *b, s32 size) {
    u32 i;
    s8 tmp;

    for (i = 0; i < size; i++) {
        tmp = a[i];
        a[i] = b[i];
        b[i] = tmp;
    }
}
