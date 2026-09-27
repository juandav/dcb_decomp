#include "psyq.h"

void _SsVmKeyOn(int seq_sep_no, short vab, short prog, u_short note, u_short voll, u_short volr);
int _SsVmKeyOff(short seq_sep_no, short vab, short prog, u_short note);

void _SsNoteOn(short seq, short sep, u_char note, u_char vol) {
    SeqStruct *score = &D_801D8618[seq][sep];
    u_short velocity = vol;
    u_char ch = score->channel;
    u_char pan = score->panpot[ch];

    if (vol != 0) {
        if (!((score->unk80 >> ch) & 1)) {
            _SsVmKeyOn((short)(seq | (sep << 8)), score->vabId, score->programs[ch], note, velocity, pan);
        }
    } else {
        _SsVmKeyOff((short)(seq | (sep << 8)), score->vabId, score->programs[ch], note);
    }
}

OBJECT_END(3);
