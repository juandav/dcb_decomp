#include "psyq.h"

extern PadPort *(*D_80077974)(int);

int PadGetState(int port) {
    PadPort *p = D_80077974(port);

    if (p->cmd == 0 && p->prevCmd == 0 && (p == p->unk10 || p->unk39 == 0) && *p->unk30 == 0) {
        return p->unk49;
    }
    switch (p->unk49) {
    case 2:
        return 1;
    case 3:
        return 1;
    case 6:
        return 4;
    }
    return p->unk49;
}

OBJECT_END(1);
