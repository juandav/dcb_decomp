#include "psyq.h"

extern int D_801D860C;
extern short D_801D8E98;
extern short D_801D8E9A;
extern long D_801D8610;
void _SsVmFlush(void);
void _SsSndPlay(short seq, short sep);
void _SsSndCrescendo(short seq, short sep);
void _SsSndTempo(short seq, short sep);
void _SsSndPause(short seq, short sep);
void _SsSndReplay(short seq, short sep);
void _SsSndStop(short seq, short sep);

void SsSeqCalledTbyT(void) {
    int i;
    int j;

    if (D_801D860C != 1) {
        D_801D860C = 1;
        _SsVmFlush();
        i = 0;
        if (D_801D8E98 > 0) do {
            if (D_801D8610 & (1 << i)) {
                j = 0;
                if (D_801D8E9A > 0) do {
                    if (D_801D8618[i][j].flags & 1) {
                        _SsSndPlay(i, j);
                        if (D_801D8618[i][j].flags & 0x10) {
                            _SsSndCrescendo(i, j);
                        }
                        if (D_801D8618[i][j].flags & 0x20) {
                            _SsSndCrescendo(i, j);
                        }
                        if (D_801D8618[i][j].flags & 0x40) {
                            _SsSndTempo(i, j);
                        }
                        if (D_801D8618[i][j].flags & 0x80) {
                            _SsSndTempo(i, j);
                        }
                    }
                    if (D_801D8618[i][j].flags & 2) {
                        _SsSndPause(i, j);
                    }
                    if (D_801D8618[i][j].flags & 8) {
                        _SsSndReplay(i, j);
                    }
                    if (D_801D8618[i][j].flags & 4) {
                        _SsSndStop(i, j);
                        D_801D8618[i][j].flags = 0;
                    }
                } while (++j < D_801D8E9A);
            }
        } while (++i < D_801D8E98);
        D_801D860C = 0;
    }
}
