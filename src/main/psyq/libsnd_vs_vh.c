#include "psyq.h"

short SsVabOpenHeadSticky(unsigned char *addr, short vabId, unsigned long sbaddr) {
    return _SsVabOpenHeadWithMode(addr, vabId, func_80055BE8, sbaddr);
}

short SsVabFakeHead(unsigned char *addr, short vabId, unsigned long sbaddr) {
    return _SsVabOpenHeadWithMode(addr, vabId, func_80055BE8, sbaddr);
}

long func_80055BE8(long size, long sbaddr) {
    return sbaddr;
}
