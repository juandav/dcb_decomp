#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("asm/main/nonmatchings/psyq", _ExitCard);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80068C64);

INCLUDE_ASM("asm/main/nonmatchings/psyq", _card_format);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80069024);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_80069034);

void *bcopy(u_char *src, u_char *dst, int n) {
    u_char *ret = NULL;
    u_char *s;

    if (src != NULL) {
        s = src;
        while (n > 0) {
            *dst++ = *src++;
            n--;
        }
        ret = s;
    }
    return ret;
}

OBJECT_END(3);
