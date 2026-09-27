#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_card_info);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068A08);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068A34);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068A78);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_card);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _patch_card2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _copy_memcard_patch);
