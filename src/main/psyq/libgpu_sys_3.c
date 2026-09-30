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

INCLUDE_ASM("main/nonmatchings/psyq", DecDCTvlcSize2);

INCLUDE_ASM("main/nonmatchings/psyq", DecDCTvlc2);

extern u_char D_80076A28[];

void DecDCTvlcBuild(u_short *table) {
    long back;
    u_char *src;
    u_char *dst;
    u_char c;
    long n;
    long i;

    back = 0;
    src = D_80076A28;
    dst = (u_char *)table;
    do {
        c = *src++;
        n = c;
        if (c < 0xF0) {
            if (back != 0) {
                for (; n >= 0; n--) {
                    *dst = *(dst - back);
                    dst++;
                }
            } else {
                for (; n >= 0; n--) {
                    *dst++ = *src++;
                }
            }
        } else {
            back = 0;
            if (c != 0xF0) {
                back = ((c << 8) | *src++) - 0xF0FF;
            }
        }
    } while (back != 0xF00);
    for (i = 4; i < 0x8800; i++) {
        table[i] ^= table[i - 4];
    }
}

OBJECT_END(3);

INCLUDE_ASM("main/nonmatchings/psyq", _bu_init);

INCLUDE_ASM("main/nonmatchings/psyq", _card_info);

INCLUDE_ASM("main/nonmatchings/psyq", _card_load);
