#include "psyq.h"

void SsSeqGetVol(short access_num, short seq_num, short *voll, short *volr) {
    _SsVmGetSeqVol((short)(access_num | (seq_num << 8)), voll, volr);
}

OBJECT_END(3);
