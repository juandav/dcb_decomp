#include "psyq.h"

/* libcd_bios_1's padding: GCC outputs its deferred inline function
   func_8005A088 after all of that file's top-level asm */
OBJECT_END(1);

INCLUDE_ASM("asm/main/nonmatchings/psyq", StRingStatus);
