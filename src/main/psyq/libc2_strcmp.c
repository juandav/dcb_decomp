#include "psyq.h"

int strcmp(char *s1, char *s2) {
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
    while (*s1 == *s2++) {
        if (*s1++ == 0) {
            return 0;
        }
    }
    return *s1 - s2[-1];
}

OBJECT_END(3);
