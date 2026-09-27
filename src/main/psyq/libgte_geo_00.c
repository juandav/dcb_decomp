#include "psyq.h"

int rsin(int a) {
    if (a < 0) {
        return -sin_1(-a & 0xFFF);
    }
    return sin_1(a & 0xFFF);
}

/* rsin_tbl; splat also labels rsin_tbl - 0x800 and rsin_tbl - 0x400 */
extern short D_80071058[];
extern short D_80070058[];
extern short D_80070858[];
extern short D_8006F858[];

long sin_1(long a) {
    if (a <= 0x800) {
        if (a <= 0x400) {
            return D_80071058[a];
        }
        return D_80071058[0x800 - a];
    }
    if (a <= 0xC00) {
        return -D_80070058[a];
    }
    return -D_80071058[0x1000 - a];
}

OBJECT_END(1);
