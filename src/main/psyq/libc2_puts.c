#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void puts(char *s) {
    char c;

    if (s == NULL) {
        s = "<NULL>";
    }
    while ((c = *s++) != 0) {
        _putchar(c);
    }
    _putchar_flash();
}

/* ASPSX padded the string table of the object as well */
__asm__(".section .rodata\n\t.space 9\n");

OBJECT_END(3);
