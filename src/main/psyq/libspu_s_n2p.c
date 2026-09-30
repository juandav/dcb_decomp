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

/* Inverse of _spu_note2pitch: the octave is the top bit of the pitch, the
   semitone and the 1/128 fine step come from the two tables */
int _spu_pitch2note(short note_high, int note_low, u_short pitch) {
    int octave = 0;
    int i;
    u_short semitone;
    int fine;
    int step;
    u_int lo;
    u_short frac;

    if (pitch >= 0x4000) {
        pitch = 0x3FFF;
    }
    for (i = 0; i < 14; i++) {
        if ((pitch >> i) & 1) {
            octave = i;
        }
    }
    pitch = pitch << (15 - octave);
    for (i = 11; i >= 0; i--) {
        if (pitch >= D_8006F414[i]) {
            semitone = i;
            break;
        }
    }
    frac = ((u_int)pitch << 15) / D_8006F414[semitone];
    for (i = 127; i >= 0; i--) {
        if (frac >= D_8006F42C[i]) {
            fine = i;
            break;
        }
    }
    step = fine + 1;
    fine = note_low;
    fine += step;
    semitone = semitone + (note_high + (octave - 12) * 12) + ((lo = (u_short)fine) >> 7);
    return (semitone << 8) | (lo & 0x7E);
}

