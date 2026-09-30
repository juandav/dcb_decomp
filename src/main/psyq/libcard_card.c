#include "psyq.h"

long _card_clear(long chan) {
    func_80068884();
    return func_80068874(chan, 0x3F, 0);
}

OBJECT_END(3);

INCLUDE_ASM("main/nonmatchings/psyq", func_80068874);

INCLUDE_ASM("main/nonmatchings/psyq", func_80068884);
