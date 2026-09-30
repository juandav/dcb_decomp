#include "psyq.h"

int prnt(void *unused, u_char *fmt0, char *argp);

/* Not variadic: the three register arguments after fmt are spilled into
   their home slots so prnt can walk them as an argument list. */
int printf(char *fmt, long arg1, long arg2, long arg3) {
    char **fmtp = &fmt;
    long *args = &arg1;

    args[1] = arg2;
    args[2] = arg3;
    return prnt((void *)1, *fmtp, (char *)args);
}

OBJECT_END(1);
