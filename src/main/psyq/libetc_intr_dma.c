#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

void *startIntrDMA(void) {
    func_8005704C((long *)D_80070AFC, 8);
    *D_80070AF8 = 0;
    InterruptCallback(3, func_80056E20);
    return func_80056FA0;
}

typedef struct DmaChannel {
    u_long madr;
    u_long bcr;
    u_long chcr;
    u_long pad;
} DmaChannel;

extern DmaChannel *D_80070B1C;
int printf(char *fmt, ...);

void func_80056E20(void) {
    u_long mask;
    int i;

    while ((mask = (*D_80070AF8 >> 24) & 0x7F) != 0) {
        for (i = 0; mask != 0 && i < 7; i++, mask >>= 1) {
            if (mask & 1) {
                *D_80070AF8 &= (1 << (i + 24)) | 0xFFFFFF;
                if (D_80070AFC[i] != NULL) {
                    D_80070AFC[i]();
                }
            }
        }
    }
    if ((*D_80070AF8 & 0xFF000000) == 0x80000000 || (*D_80070AF8 & 0x8000)) {
        printf("DMA bus error: code=%08x\n", *D_80070AF8);
        for (i = 0; i < 7; i++) {
            printf("MADR[%d]=%08x\n", i, D_80070B1C[i].madr);
        }
    }
}

/* ASPSX padded the string table of the object as well */
__asm__(".section .rodata\n\t.space 4\n\t.section .text\n");

void (*func_80056FA0(int index, void (*callback)(void)))(void) {
    void (*prev)(void) = D_80070AFC[index];

    if (callback != prev) {
        if (callback != NULL) {
            D_80070AFC[index] = callback;
            *D_80070AF8 = (*D_80070AF8 & 0xFFFFFF) | 0x800000 | (1 << (index + 16));
        } else {
            D_80070AFC[index] = NULL;
            *D_80070AF8 = ((*D_80070AF8 & 0xFFFFFF) | 0x800000) & ~(1 << (index + 16));
        }
    }
    return prev;
}

void func_8005704C(long *p, int n) {
    int i = n - 1;

    if (n != 0) {
        do {
            *p++ = 0;
        } while (i-- != 0);
    }
}

OBJECT_END(1);
