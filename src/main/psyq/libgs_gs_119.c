#include "psyq.h"

void gte_rotate_z_matrix(MATRIX *m, long angle) {
    MATRIX rot;
    long a = angle / 360;
    long c = rcos(a);
    long s = rsin(a);

    if (angle != 0) {
        rot.m[0][0] = c;
        rot.m[0][1] = -s;
        rot.m[0][2] = 0;
        rot.m[1][0] = s;
        rot.m[1][1] = c;
        rot.m[1][2] = 0;
        rot.m[2][0] = 0;
        rot.m[2][1] = 0;
        rot.m[2][2] = 4096;
        rot.t[0] = 0;
        rot.t[1] = 0;
        rot.t[2] = 0;
        MulMatrix(m, &rot);
    }
}

OBJECT_END(3);
