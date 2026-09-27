#include "psyq.h"

typedef struct {
    long mode;
    short left;
    short right;
    long delay;
    long feedback;
} SpuRevAttr8006EFA8;

extern SpuRevAttr8006EFA8 D_8006EFA8;
extern long D_8006EFA0;
extern long D_8006F534[];
extern SpuReverbRegs D_8006F5B4[];

long _SpuIsInAllocateArea_(u_long addr);
void _spu_setReverbAttr(SpuReverbRegs *attr);
void _spu_FsetRXX(int reg, u_long value, int mode);

static __inline__ void _memcpy(char *dst, char *src, long size) {
    while (size--) {
        *dst++ = *src++;
    }
}

long SpuSetReverbModeParam(SpuReverbAttr *attr) {
    SpuReverbRegs regs;
    long clear;
    u_long mask;
    long all;
    u_long mode;
    long setMode;
    long setDelay;
    long setFeedback;
    long spuOff;

    spuOff = 0;
    setMode = 0;
    setDelay = 0;
    clear = 0;
    setFeedback = 0;
    mask = attr->mask;
    all = mask == 0;
    regs.mask = 0;
    if (all || (mask & SPU_REV_MODE)) {
        mode = attr->mode;
        if (mode & SPU_REV_MODE_CLEAR_WA) {
            mode &= ~SPU_REV_MODE_CLEAR_WA;
            clear = 1;
        }
        if (mode >= 10 || _SpuIsInAllocateArea_(D_8006F534[mode])) {
            return -1;
        }
        setMode = 1;
        D_8006EFA8.mode = mode;
        D_8006EFA0 = D_8006F534[D_8006EFA8.mode];
        _memcpy((char *)&regs, (char *)&D_8006F5B4[D_8006EFA8.mode], sizeof(SpuReverbRegs));
        switch (D_8006EFA8.mode) {
        case SPU_REV_MODE_ECHO:
            D_8006EFA8.feedback = 0x7F;
            D_8006EFA8.delay = 0x7F;
            break;
        case SPU_REV_MODE_DELAY:
            D_8006EFA8.feedback = 0;
            D_8006EFA8.delay = 0x7F;
            break;
        default:
            D_8006EFA8.feedback = 0;
            D_8006EFA8.delay = 0;
            break;
        }
    }
    if (all || (mask & SPU_REV_DELAYTIME)) {
        switch (D_8006EFA8.mode) {
        case SPU_REV_MODE_ECHO:
        case SPU_REV_MODE_DELAY:
            setDelay = 1;
            if (!setMode) {
                _memcpy((char *)&regs, (char *)&D_8006F5B4[D_8006EFA8.mode], sizeof(SpuReverbRegs));
                regs.mask = 0xC011C00;
            }
            D_8006EFA8.delay = attr->delay;
            regs.param[10] = (D_8006EFA8.delay << 13) / 127 - regs.param[0];
            regs.param[11] = (D_8006EFA8.delay << 12) / 127 - regs.param[1];
            regs.param[12] = (D_8006EFA8.delay << 12) / 127 + regs.param[13];
            regs.param[16] = (D_8006EFA8.delay << 12) / 127 + regs.param[17];
            regs.param[26] = (D_8006EFA8.delay << 12) / 127 + regs.param[28];
            regs.param[27] = (D_8006EFA8.delay << 12) / 127 + regs.param[29];
            break;
        }
    }
    if (all || (mask & SPU_REV_FEEDBACK)) {
        switch (D_8006EFA8.mode) {
        case SPU_REV_MODE_ECHO:
        case SPU_REV_MODE_DELAY:
            setFeedback = 1;
            if (!setMode) {
                if (!setDelay) {
                    _memcpy((char *)&regs, (char *)&D_8006F5B4[D_8006EFA8.mode], sizeof(SpuReverbRegs));
                    regs.mask = 0x80;
                } else {
                    regs.mask |= 0x80;
                }
            }
            D_8006EFA8.feedback = attr->feedback;
            regs.param[7] = (D_8006EFA8.feedback * 0x8100) / 127;
            break;
        }
    }
    if (setMode) {
        spuOff = (D_8006EF24[0xD5] >> 7) & 1;
        if (spuOff) {
            D_8006EF24[0xD5] &= ~0x80;
        }
    }
    if (!setMode) {
        if (all || (mask & SPU_REV_DEPTHL)) {
            D_8006EF24[0xC2] = attr->depth.left;
            D_8006EFA8.left = attr->depth.left;
        }
        if (all || (mask & SPU_REV_DEPTHR)) {
            D_8006EF24[0xC3] = attr->depth.right;
            D_8006EFA8.right = attr->depth.right;
        }
    } else {
        D_8006EF24[0xC2] = 0;
        D_8006EF24[0xC3] = 0;
        D_8006EFA8.left = 0;
        D_8006EFA8.right = 0;
    }
    if (setMode || setDelay || setFeedback) {
        _spu_setReverbAttr(&regs);
    }
    if (clear) {
        SpuClearReverbWorkArea(D_8006EFA8.mode);
    }
    if (setMode) {
        _spu_FsetRXX(0xD1, D_8006EFA0, 0);
        if (spuOff) {
            D_8006EF24[0xD5] |= 0x80;
        }
    }
    return 0;
}

OBJECT_END(3);
