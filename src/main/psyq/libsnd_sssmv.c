#include "psyq.h"

void SsSetMVol(short left, short right) {
    SpuCommonAttr attr;

    attr.mask = 3;
    attr.mvol.left = left * 129;
    attr.mvol.right = right * 129;
    SpuSetCommonAttr(&attr);
}
