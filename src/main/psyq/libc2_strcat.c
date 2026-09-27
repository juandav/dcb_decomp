#include "psyq.h"

char *strcat(char *dst, char *src) {
    char *ret;

    if (dst == NULL || src == NULL) {
        return NULL;
    }
    if (dst + strlen(dst) != src + strlen(src)) {
        ret = dst;
        while (*dst++ != 0) {
        }
        dst--;
        while ((*dst++ = *src++) != 0) {
        }
        return ret;
    }
    return NULL;
}

OBJECT_END(3);
