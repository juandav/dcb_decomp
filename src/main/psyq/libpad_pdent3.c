#include "psyq.h"

extern PadPort *(*D_80077974)(int);

int PadInfoMode(int port, int term, int offs) {
    PadPort *p = D_80077974(port);

    switch (term) {
    case 1:
        return p->unkE8;
    case 2:
        return p->unkE6;
    case 3:
        return p->unkE4;
    case 4:
        if (offs < 0) {
            return p->unkE3;
        }
        if (offs < p->unkE3) {
            return p->unk0[offs];
        }
        return 0;
    case 100:
        return p->unk4C;
    }
    return 0;
}

OBJECT_END(2);
