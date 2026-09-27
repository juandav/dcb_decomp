#include "psyq.h"

int strlen(char *s) {
    int n = 0;
    int ret = 0;

    if (s != NULL) {
        while (*s++ != 0) {
            n++;
        }
        ret = n;
    }
    return ret;
}

OBJECT_END(3);

int strncmp(char *s1, char *s2, int n) {
    if (s1 == NULL || s2 == NULL) {
        int r = 0;

        if (s1 != s2) {
            r = -1;
            if (s1 != NULL) {
                r = 1;
            }
        }
        return r;
    }
    while (--n >= 0) {
        if (*s1 != *s2++) {
            break;
        }
        if (*s1++ == 0) {
            return 0;
        }
    }
    if (n < 0) {
        return 0;
    }
    return *s1 - s2[-1];
}
