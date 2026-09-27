#include "psyq.h"

extern u_short D_8006F414[];
extern u_short D_8006F42C[];

u_short _spu_note2pitch(int cen_note, int cen_fine, int note, int fine) {
    u_long pitch;
    u_int f;
    short n;
    short octave;
    short semi;

    fine += cen_fine;
    f = (u_short)fine;
    n = note + (f >> 7) - cen_note;
    octave = n / 12 - 2;
    semi = n % 12;
    fine = f & 0x7F;
    if (semi < 0) {
        semi += 12;
        octave = n / 12 - 3;
    }
    pitch = (D_8006F414[semi] * D_8006F42C[(u_short)fine]) >> 16;
    if (octave >= 0) {
        pitch = 0x3FFF;
    } else {
        pitch += 1 << (-octave - 1);
        pitch >>= -octave;
    }
    return pitch;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", _spu_pitch2note);
