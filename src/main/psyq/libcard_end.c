#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

INCLUDE_ASM("main/nonmatchings/psyq", _ExitCard);

void func_80068884(void);
int func_80068874(long chan, long block, u_char *buf);
int func_80069024(long chan, long block, u_char *buf);
int func_80069034(long port);

int func_80068C64(long chan, long block, u_char *buf) {
    u_char rbuf[128];
    u_char *p;
    u_char *rp;
    u_char sum;
    int i;
    int n;
    u_char *q;
    int k;

    i = 0;
    p = buf;
    sum = 0;
    for (n = 0; n < 127; n++) {
        sum ^= *p++;
    }
    *p = sum;
    while (1) {
        if (i >= 8) {
            return 0;
        }
        func_80068884();
        if (func_80068874(chan, block, buf) != 1) {
            return 0;
        }
        while (!(func_80069034(chan >> 4) & 1)) {
        }
        rp = rbuf;
        bzero(rp, 128);
        func_80068884();
        if (func_80069024(chan, block, rp) == 1) {
            while (!(func_80069034(chan >> 4) & 1)) {
            }
        }
        q = rbuf;
        sum = 0;
        for (k = 0; k < 127; k++) {
            sum ^= *q++;
        }
        if (buf[127] == sum) {
            return 1;
        }
        i++;
    }
}

typedef struct CardDir {
    /* 0x00 */ long attr;
    /* 0x04 */ long size;
    /* 0x08 */ u_short next;
    /* 0x0A */ char name[22];
} CardDir;

/* the sectors are copied into the buffer as unaligned byte blocks */
typedef struct {
    char c[32];
} CardBlk32;
typedef struct {
    char c[4];
} CardBlk4;

extern CardDir D_801DD960[15];
extern long D_801DDB40[20];
extern u_char D_801DDB90[128];

/* read a block back and wait for the card to finish */
static __inline__ int card_read_sync(long chan, long block, u_char *buf) {
    if (func_80069024(chan, block, buf) != 1) {
        return 0;
    }
    while (!(func_80069034(chan >> 4) & 1)) {
    }
    return 1;
}

long _card_format(long chan) {
    int i;

    for (i = 0; i < 15; i++) {
        bzero(D_801DDB90, 128);
        bzero((u_char *)&D_801DD960[i], 32);
        D_801DD960[i].attr = 0xA0;
        D_801DD960[i].size = 0;
        D_801DD960[i].next = 0xFFFF;
        *(CardBlk32 *)D_801DDB90 = *(CardBlk32 *)&D_801DD960[i];
        if (func_80068C64(chan, i + 1, D_801DDB90) != 1) {
            return 0;
        }
    }
    for (i = 0; i < 20; i++) {
        D_801DDB40[i] = -1;
        bzero(D_801DDB90, 128);
        *(CardBlk4 *)D_801DDB90 = *(CardBlk4 *)&D_801DDB40[i];
        if (func_80068C64(chan, i + 16, D_801DDB90) != 1) {
            return 0;
        }
    }
    bzero(D_801DDB90, 128);
    D_801DDB90[0] = 'M';
    D_801DDB90[1] = 'C';
    if (func_80068C64(chan, 0, D_801DDB90) != 1) {
        return 0;
    }
    bzero(D_801DDB90, 128);
    func_80068884();
    if (card_read_sync(chan, 0, D_801DDB90) != 1) {
        return 0;
    }
    func_80068884();
    if (_card_load(chan) != 1) {
        return 0;
    }
    while (!(func_80069034(chan >> 4) & 1)) {
    }
    return 1;
}

/* the BIOS call stubs after it are another object */
OBJECT_END(1);

INCLUDE_ASM("main/nonmatchings/psyq", func_80069024);

INCLUDE_ASM("main/nonmatchings/psyq", func_80069034);

void *bcopy(u_char *src, u_char *dst, int n) {
    u_char *ret = NULL;
    u_char *s;

    if (src != NULL) {
        s = src;
        while (n > 0) {
            *dst++ = *src++;
            n--;
        }
        ret = s;
    }
    return ret;
}

OBJECT_END(3);
