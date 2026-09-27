#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8004C300);

void SsSeqClose(short seq) {
    func_8004C300(seq);
}

void SsSepClose(short sep_access_num) {
    func_8004C300(sep_access_num);
}

OBJECT_END(3);
