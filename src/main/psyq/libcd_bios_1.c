#include "psyq.h"

extern void (*D_8005B850[2])(void);

extern void (*D_8005B850[2])(void);

extern void (*D_8006F59C[])();

typedef struct {
    u_char sync;
    u_char ready;
    u_char c;
} CD_intr;

extern volatile CD_intr D_80070F1C[1];
extern u_char D_801DBD20[8];
extern u_char D_801DBD28[8];
extern u_char D_801DBD30[8];
extern int D_801DBD38;
extern int D_801DBD3C;
extern char *D_801DBD40;
extern long D_80070C40, D_80070C44, D_80070C48, D_80070C54;
extern u_char D_80070C58[4];
extern u_char D_80070C5C;
extern u_char D_80070C5D;
extern char *D_80070C64[];
extern char *D_80070CE4[];
extern int D_80070D04[];
extern int D_80070D84[];
extern int D_80070E04[];
extern int D_80070E84[];
extern volatile u_long *D_80070F0C;

/* CD_sync / CD_ready report a timeout with these */
const char D_80013538[] = "CD timeout: ";
const char D_80013548[] = "%s:(%s) Sync=%s, Ready=%s\n";

static inline void _memcpy(u_char *dst, u_char *src, int n) {
    if (dst != NULL) {
        while (n--) {
            *dst++ = *src++;
        }
    }
}

int func_80058BE4(void) {
    volatile u_char nReg;
    volatile u_char buf[8];
    int i, j;
    int err;

    *D_80070F04 = 1;
    nReg = *D_80070F08 & 7;
    if (nReg == 0) {
        return 0;
    }
    err = 0;
    while (nReg != (*D_80070F08 & 7)) {
        nReg = *D_80070F08 & 7;
    }
    for (i = 0; i < 8; i++) {
        if (!(*D_80070F04 & 0x20)) {
            break;
        }
        buf[i] = *D_80070F10;
    }
    for (j = i; j < 8; j++) {
        buf[j] = 0;
    }
    *D_80070F04 = 1;
    *D_80070F08 = 7;
    *D_80070F14 = 7;
    if (nReg != 3 || D_80070E04[D_80070C5D]) {
        if (!(D_80070C4C & CdlStatShellOpen) && (buf[0] & CdlStatShellOpen)) {
            D_80070C54++;
        }
        D_80070C4C = buf[0];
        D_80070C50 = buf[1];
        err = D_80070C4C & 0x1D;
    }
    if (nReg == 5) {
        if (D_80070C48 > 2) {
            printf("DiskError: ");
        }
        if (D_80070C48 > 2) {
            printf("com=%s,code=(%02x:%02x)\n", D_80070C64[D_80070C5D], D_80070C4C, D_80070C50);
        }
    }
    switch (nReg) {
    case 3:
        if (err) {
            D_80070F1C->sync = CdlDiskError;
            _memcpy(D_801DBD20, (u_char *)buf, 8);
            return 2;
        }
        if (D_80070D04[D_80070C5D]) {
            D_80070F1C->sync = CdlAcknowledge;
            _memcpy(D_801DBD20, (u_char *)buf, 8);
            return 1;
        }
        D_80070F1C->sync = CdlComplete;
        _memcpy(D_801DBD20, (u_char *)buf, 8);
        return 2;
    case 2:
        D_80070F1C->sync = err ? CdlDiskError : CdlComplete;
        _memcpy(D_801DBD20, (u_char *)buf, 8);
        return 2;
    case 1:
        if (err && i == 1) {
            err = 0;
        }
        D_80070F1C->ready = err ? CdlDiskError : CdlDataReady;
        _memcpy(D_801DBD28, (u_char *)buf, 8);
        *D_80070F04 = 0;
        *D_80070F08 = 0;
        return 4;
    case 4:
        D_80070F1C->ready = D_80070F1C->c = CdlDataEnd;
        _memcpy(D_801DBD30, (u_char *)buf, 8);
        _memcpy(D_801DBD28, (u_char *)buf, 8);
        return 4;
    case 5:
        D_80070F1C->sync = D_80070F1C->ready = CdlDiskError;
        _memcpy(D_801DBD20, (u_char *)buf, 8);
        _memcpy(D_801DBD28, (u_char *)buf, 8);
        return 6;
    default:
        puts("CDROM: unknown intr");
        printf("(%d)\n", nReg);
        return 0;
    }
}

static inline void func_8005A088(void) {
    u_char mask;
    int intr;

    mask = *D_80070F04 & 3;
    while ((intr = func_80058BE4()) != 0) {
        if ((intr & 4) && D_80070C44) {
            ((void (*)(u_char, u_char *))D_80070C44)(D_80070F1C[0].ready, D_801DBD28);
        }
        if ((intr & 2) && D_80070C40) {
            ((void (*)(u_char, u_char *))D_80070C40)(D_80070F1C[0].sync, D_801DBD20);
        }
    }
    *D_80070F04 = mask;
}


INCLUDE_ASM("main/nonmatchings/psyq", CD_sync);

INCLUDE_ASM("main/nonmatchings/psyq", CD_ready);

INCLUDE_ASM("main/nonmatchings/psyq", CD_cw);

int CD_vol(CdlATV *vol) {
    *D_80070F04 = 2;
    *D_80070F14 = vol->val0;
    *D_80070F08 = vol->val1;
    *D_80070F04 = 3;
    *D_80070F10 = vol->val2;
    *D_80070F14 = vol->val3;
    *D_80070F08 = 0x20;
    return 0;
}

void CD_flush(void) {
    *D_80070F04 = 1;
    while (*D_80070F08 & 7) {
        *D_80070F04 = 1;
        *D_80070F08 = 7;
        *D_80070F14 = 7;
    }
    D_80070F1C->ready = D_80070F1C->c = CdlNoIntr;
    D_80070F1C->sync = CdlComplete;
    *D_80070F04 = 0;
    *D_80070F08 = 0;
    *D_80070F0C = 0x1325;
}

extern volatile u_short *D_80070F18;
extern volatile u_char *D_80070F04;
extern volatile u_char *D_80070F08;
extern volatile u_char *D_80070F10;
extern volatile u_char *D_80070F14;

int CD_initvol(void) {
    CdlATV vol;

    if (D_80070F18[0xDC] == 0 && D_80070F18[0xDD] == 0) {
        D_80070F18[0xC0] = 0x3FFF;
        D_80070F18[0xC1] = 0x3FFF;
    }
    D_80070F18[0xD8] = 0x3FFF;
    D_80070F18[0xD9] = 0x3FFF;
    D_80070F18[0xD5] = 0xC001;
    vol.val0 = vol.val2 = 0x80;
    vol.val1 = vol.val3 = 0;
    *D_80070F04 = 2;
    *D_80070F14 = vol.val0;
    *D_80070F08 = vol.val1;
    *D_80070F04 = 3;
    *D_80070F10 = vol.val2;
    *D_80070F14 = vol.val3;
    *D_80070F08 = 0x20;
    return 0;
}

extern long D_80070C40;

void CD_initintr(void) {
    D_80070C44 = 0;
    D_80070C40 = 0;
    D_80070C50 = 0;
    D_80070C4C = 0;
    ResetCallback();
    InterruptCallback(2, func_8005A088);
}

extern long D_80070F20[];

int CD_init(void) {
    puts("CD_init:");
    printf("addr=%08x\n", &D_80070F20);
    D_80070C5D = 0;
    D_80070C5C = 0;
    D_80070C44 = 0;
    D_80070C40 = 0;
    D_80070C50 = 0;
    D_80070C4C = 0;
    ResetCallback();
    InterruptCallback(2, func_8005A088);
    *D_80070F04 = 1;
    while (*D_80070F08 & 7) {
        *D_80070F04 = 1;
        *D_80070F08 = 7;
        *D_80070F14 = 7;
    }
    D_80070F1C->ready = D_80070F1C->c = CdlNoIntr;
    D_80070F1C->sync = CdlComplete;
    *D_80070F04 = 0;
    *D_80070F08 = 0;
    *D_80070F0C = 0x1325;
    CD_cw(CdlNop, NULL, NULL, 0);
    if (D_80070C4C & CdlStatShellOpen) {
        CD_cw(CdlNop, NULL, NULL, 0);
    }
    if (CD_cw(0x0A /* CdlReset */, NULL, NULL, 0)) {
        return -1;
    }
    if (CD_cw(CdlDemute, NULL, NULL, 0)) {
        return -1;
    }
    if (CD_sync(0, NULL) != CdlComplete) {
        return -1;
    }
    return 0;
}

INCLUDE_ASM("main/nonmatchings/psyq", CD_datasync);

extern int D_80070EE8;

void CD_set_test_parmnum(int num) {
    D_80070EE8 = num;
}

