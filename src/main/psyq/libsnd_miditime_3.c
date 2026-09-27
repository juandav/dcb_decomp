#include "psyq.h"

void SsSeqPlay(short access_num, char play_mode, short l_count) {
    Snd_SetPlayMode(access_num, 0, play_mode, l_count);
}

OBJECT_END(2);
