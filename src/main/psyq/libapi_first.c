#include "psyq.h"

typedef struct DevEntry {
    /* 0x00 */ char *name;
    /* 0x04 */ u8 unk4[0x30];
    /* 0x34 */ long (*func)();
    /* 0x38 */ u8 unk38[0x18];
} DevEntry;

extern char D_801DDC48[];
extern long (*D_801DDC40)();
long func_8006ABB0();
struct DIRENTRY *func_8006ACB4(char *name, struct DIRENTRY *dir);
int strcmp(char *a, char *b);

/* BIOS device control block table and its size in bytes */
#define DEV_TABLE (*(DevEntry **)0x150)
#define DEV_TABLE_SIZE (*(u_long *)0x154)

static __inline__ int find_device(void) {
    DevEntry *dev;
    DevEntry *table = DEV_TABLE;
    u_long count = DEV_TABLE_SIZE / sizeof(DevEntry);

    for (dev = table; dev < table + count; dev++) {
        if (dev->name != NULL && strcmp(dev->name, D_801DDC48) == 0) {
            D_801DDC40 = dev->func;
            return 1;
        }
    }
    return 0;
}

static __inline__ void hook_device(void) {
    DevEntry *dev;
    DevEntry *table = DEV_TABLE;
    u_long count = DEV_TABLE_SIZE / sizeof(DevEntry);

    for (dev = table; dev < table + count; dev++) {
        if (dev->name != NULL && strcmp(dev->name, D_801DDC48) == 0) {
            dev->func = func_8006ABB0;
            return;
        }
    }
}

struct DIRENTRY *firstfile(char *name, struct DIRENTRY *dir) {
    char *s;
    char *d;

    s = name;
    d = D_801DDC48;
    while (*s > ':') {
        *d++ = *s++;
    }
    *d = 0;
    if (!find_device()) {
        return NULL;
    }
    hook_device();
    return func_8006ACB4(name, dir);
}

static __inline__ void restore_device(long (*func)()) {
    DevEntry *dev;
    DevEntry *table = DEV_TABLE;
    u_long count = DEV_TABLE_SIZE / sizeof(DevEntry);

    for (dev = table; dev < table + count; dev++) {
        if (dev->name != NULL && strcmp(dev->name, D_801DDC48) == 0) {
            dev->func = func;
            return;
        }
    }
}

long func_8006ABB0(long *fcb, long a1, long a2) {
    if (*fcb == 0) {
        *fcb = 1;
    }
    restore_device(D_801DDC40);
    return D_801DDC40(fcb, a1, a2);
}

OBJECT_END(1);

INCLUDE_ASM("main/nonmatchings/psyq", func_8006ACB4);
