#include "psyq.h"

char *strcpy(char *dst, char *src) {
    char *r;

    if (dst == NULL || src == NULL) {
        return NULL;
    }
    r = dst;
    while ((*dst++ = *src++) != 0) {
    }
    return r;
}

OBJECT_END(3);
