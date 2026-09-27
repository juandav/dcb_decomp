#include "psyq.h"

u_long func_800682F8(void);

extern volatile u_long *D_800769EC;

u_long func_800682F8(void) {
    return *D_800769EC;
}

extern volatile u_long *D_800769C0;
extern volatile u_long *D_800769CC;

int func_80068310(char *s) {
    printf("%s timeout:\n", s);
    *D_800769EC = 0x80000000;
    *D_800769C0 = 0;
    *D_800769CC = 0;
    *D_800769CC;
    *D_800769EC = 0x60000000;
    return 0;
}

/* ASPSX padded the string table of the object as well */
__asm__(".section .rodata\n\t.space 4\n");

OBJECT_END(2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTvlcSize2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTvlc2);

INCLUDE_ASM("asm/main/nonmatchings/psyq", DecDCTvlcBuild);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068804);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068814);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068824);
