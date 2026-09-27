#include "psyq.h"

void SsSetSerialAttr(char s_num, char attr, char mode) {
    SpuCommonAttr c_attr;

    if (s_num == 0) {
        if (attr == 0) {
            c_attr.mask = SPU_COMMON_CDMIX;
            c_attr.cd.mix = mode;
        }
        if (attr == 1) {
            c_attr.mask = SPU_COMMON_CDREV;
            c_attr.cd.reverb = mode;
        }
    }
    if (s_num == 1) {
        if (attr == 0) {
            c_attr.mask = SPU_COMMON_EXTMIX;
            c_attr.ext.mix = mode;
        }
        if (attr == 1) {
            c_attr.mask = SPU_COMMON_EXTREV;
            c_attr.ext.reverb = mode;
        }
    }
    SpuSetCommonAttr(&c_attr);
}
