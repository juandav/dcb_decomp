#include "psyq.h"

void SsInit(void) {
    ResetCallback();
    func_8004C5B0();
    SpuClearReverbWorkArea(7);
    _SsInit();
}

OBJECT_END(2);

void func_8004C5B0(void) {
    _SpuInit(0);
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", SsSeqOpen);
