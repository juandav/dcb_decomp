#include "psyq.h"

INCLUDE_ASM("main/nonmatchings/psyq", setjmp);

INCLUDE_ASM("main/nonmatchings/psyq", longjmp);

extern u_char D_80077879[];

char toupper(char c) {
    if (D_80077879[(u_char)c] & 2) {
        c -= 0x20;
    }
    return c;
}

OBJECT_END(3);

INCLUDE_ASM("main/nonmatchings/psyq", InitHeap);

INCLUDE_ASM("main/nonmatchings/psyq", FlushCache);

INCLUDE_ASM("main/nonmatchings/psyq", func_8006A754);

INCLUDE_ASM("main/nonmatchings/psyq", func_8006A76C);

INCLUDE_ASM("main/nonmatchings/psyq", DeliverEvent);

INCLUDE_ASM("main/nonmatchings/psyq", OpenEvent);

INCLUDE_ASM("main/nonmatchings/psyq", WaitEvent);

INCLUDE_ASM("main/nonmatchings/psyq", TestEvent);

INCLUDE_ASM("main/nonmatchings/psyq", EnableEvent);

INCLUDE_ASM("main/nonmatchings/psyq", ReturnFromException);

INCLUDE_ASM("main/nonmatchings/psyq", ResetEntryInt);

INCLUDE_ASM("main/nonmatchings/psyq", HookEntryInt);
