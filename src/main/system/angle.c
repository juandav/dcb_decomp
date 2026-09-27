#include "dcb/angle.h"
#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/heap.h"
#include "dcb/main.h"
#include "dcb/task.h"

s32 computeVectorAngle(s32 y, s32 x) {
    s32 angle;

    if (x == 0) {
        if (y > 0) {
            return 0x400;
        }
        if (y < 0) {
            return -0x400;
        }
        return 0;
    }
    angle = catan((y << 12) / x);
    if (x < 0) {
        if (y <= 0) {
            angle -= 0x800;
        } else {
            angle += 0x800;
        }
    }
    return angle;
}
