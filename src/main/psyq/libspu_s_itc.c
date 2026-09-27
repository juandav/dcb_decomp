#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuIsTransferCompleted);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __SN_ENTRY_POINT);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __main);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __sn_cpp_structors);
