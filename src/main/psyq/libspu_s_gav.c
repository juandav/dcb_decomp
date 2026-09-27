#include "psyq.h"

u_long _SpuGetAnyVoice(int lo, int hi) {
    u_long h = D_8006EF24[hi] & 0xFF;

    return D_8006EF24[lo] | (h << 16);
}
