#include "psyq.h"

typedef struct CdReadEnv {
    /* 0x00 */ int nsector;
    /* 0x04 */ u_long *addr;
    /* 0x08 */ u_long *cur;
    /* 0x0C */ int mode;
    /* 0x10 */ int size;
    /* 0x14 */ int rest;
    /* 0x18 */ int ctime;
    /* 0x1C */ int stime;
    /* 0x20 */ int pos;
    /* 0x24 */ int busy;
    /* 0x28 */ long oldSync;
    /* 0x2C */ long oldReady;
    /* 0x30 */ long oldData;
    /* 0x34 */ u_char *result;
} CdReadEnv;

/* The original reaches some fields from the address of the struct and others
   from the address of the field itself: D_80070FD8->x and D_80070FD8[0].x
   reproduce the two */
extern volatile CdReadEnv D_80070FD8[1];
extern long D_80070B4C;
extern void (*D_80070B48)(int, u_char *);

long func_8005B1C4(long func);
long func_8005A3A4(long func);
int func_8005A7A4(void (*func)());
int func_8005B174(void);
int func_8005B184(void);
CdlLOC *func_8005B194(void);
void func_8005B1A4(void);
int func_8005B1E4(void *madr, int size);
int func_8005B304(void *madr, int size);
int func_8005B414(int mode);
void func_8005A384(int mode, u_char *result);
int func_8005AB4C(int retry);
void func_8005AA7C(void);
void func_8005A808(u_char intr, u_char *result);

void func_8005A7D4(void) {
    func_8005B1C4(D_80070FD8[0].oldSync);
    D_80070FD8->busy = 0;
}

static __inline__ void cd_read_end(void) {
    func_8005A3A4(D_80070FD8->oldReady);
    if (D_80070B4C & 1) {
        func_8005A7A4((void (*)())D_80070FD8->oldData);
    }
    func_8005B1C4((long)func_8005A7D4);
    CdControlF(CdlPause, NULL);
    D_80070FD8->busy = 1;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005A808);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005AA7C);

INCLUDE_ASM("asm/main/nonmatchings/psyq", func_8005AB4C);

static __inline__ int cd_read_wait(void) {
    u_long t;

    if (D_80070FD8[0].busy) {
        t = VSync(-1);
        while (D_80070FD8[0].busy) {
            if (VSync(-1) - t > 120) {
                func_8005B1C4(D_80070FD8[0].oldSync);
                D_80070FD8->busy = 0;
                return -1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdReadBreak);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdRead);

INCLUDE_ASM("asm/main/nonmatchings/psyq", CdReadSync);
