#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

int func_8005B1E4(void *madr, int size) {
    return CD_getsector(madr, size) == 0;
}
