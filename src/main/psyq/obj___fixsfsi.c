#include "psyq.h"

INCLUDE_ASM("main/nonmatchings/psyq", __fixsfsi);

INCLUDE_ASM("main/nonmatchings/psyq", __floatsisf);

extern int D_8006EF1C;
extern int D_8006EF20;

int _err_math(int code, int arg) {
    D_8006EF1C = code;
    D_8006EF20 = arg;
    switch (code) {
    case 33:
        DeliverEvent(0xF4000002, 0x301);
        break;
    case 34:
        DeliverEvent(0xF4000002, 0x302);
        break;
    }
    return 0;
}

OBJECT_END(3);
