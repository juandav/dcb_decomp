#include "psyq.h"

INCLUDE_ASM("asm/main/nonmatchings/psyq", SpuIsTransferCompleted);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __SN_ENTRY_POINT);

INCLUDE_ASM("asm/main/nonmatchings/psyq", __main);

void __sn_cpp_structors(long start, long end) {
    void (*fn)(void);

    while (start < end) {
        fn = *(void (**)(void))start;
        if (fn != NULL) {
            fn();
        }
        start += 4;
    }
}
