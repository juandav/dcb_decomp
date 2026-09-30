#include "psyq.h"

extern long D_80077848;
extern long D_8007784C;
extern char D_801DDC20[];
extern char D_80077879[];

void _putchar(char c) {
    switch (c) {
    case '\n':
        _putchar('\r');
        D_80077848 = 0;
        break;
    case '\t':
        do {
            _putchar(' ');
        } while (D_80077848 & 7);
        return;
    default:
        if (D_80077879[(u_char)c] & 0x97) {
            D_80077848++;
        }
        break;
    }
    if (D_8007784C >= 0x20) {
        write(1, D_801DDC20, D_8007784C);
        D_8007784C = 0;
    }
    D_801DDC20[D_8007784C++] = c;
}

void _putchar_flash(void) {
    if (D_8007784C > 0) {
        write(1, D_801DDC20, D_8007784C);
        D_8007784C = 0;
    }
}

void putchar(char c) {
    _putchar(c);
    _putchar_flash();
}
