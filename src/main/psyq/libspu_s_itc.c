#include "psyq.h"

extern long D_8006EF94;
extern long D_8006EF58;
extern long D_8006EF8C;

long SpuIsTransferCompleted(long flag) {
    long ret;

    if (D_8006EF94 == 1 || D_8006EF58 == 1) {
        return 1;
    }
    ret = TestEvent(D_8006EF8C);
    if (flag == 1) {
        if (ret == 0) {
            while (TestEvent(D_8006EF8C) == 0) {
            }
        }
        ret = 1;
        D_8006EF58 = ret;
    } else if (ret == 1) {
        D_8006EF58 = ret;
    }
    return ret;
}

OBJECT_END(3);

INCLUDE_ASM("main/nonmatchings/psyq", __SN_ENTRY_POINT);

INCLUDE_ASM("main/nonmatchings/psyq", __main);

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
