#include "psyq.h"

short SsVabOpenHeadSticky(unsigned char *addr, short vabId, unsigned long sbaddr) {
    return _SsVabOpenHeadWithMode(addr, vabId, func_80055BE8, sbaddr);
}

short SsVabFakeHead(unsigned char *addr, short vabId, unsigned long sbaddr) {
    return _SsVabOpenHeadWithMode(addr, vabId, func_80055BE8, sbaddr);
}

int func_80055BE8(int arg0, int arg1) {
    return arg1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _SsVabOpenHeadWithMode);
