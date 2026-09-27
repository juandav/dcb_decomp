#include "psyq.h"

/* ProgAtr as the loader fills it: reserved2 holds two SPU addresses */
typedef struct ProgAtrV {
    /* 0x0 */ u_char tones;
    /* 0x1 */ u8 unk1[7];
    /* 0x8 */ u_long reserved1;
    /* 0xC */ u_short reserved2;
    /* 0xE */ u_short reserved3;
} ProgAtrV;

extern u_char D_801D9700[16];
extern short D_801D9758;
extern long D_801D96D8;
extern short D_801D96C2;
extern VabHdr *D_801D9638[16];
extern ProgAtrV *D_801D95F8[16];
extern VagAtr *D_801D9680[16];
extern u_long D_801D9760[16];
extern long D_801D9718[16];

int _spu_getInTransfer(void);
void _spu_setInTransfer(int);

short _SsVabOpenHeadWithMode(unsigned char *addr, short vabId, long (*alloc)(), unsigned long sbaddr) {
    long vagLens[256];
    long i;
    long size;
    short id;
    u_char *p;
    VabHdr *hdr;
    u_long magic;
    u_char vags;
    ProgAtrV *prog;
    u_long spuAddr;
    long len;

    id = 16;
    if (_spu_getInTransfer() == 1) {
        return -1;
    }
    _spu_setInTransfer(1);
    if (vabId >= 16) {
        _spu_setInTransfer(0);
        return -1;
    }
    if (vabId == -1) {
        for (i = 0; i < 16; i++) {
            if (D_801D9700[i] == 0) {
                D_801D9700[i] = 1;
                D_801D9758++;
                id = i;
                break;
            }
        }
    } else if (D_801D9700[vabId] == 0) {
        D_801D9700[vabId] = 1;
        D_801D9758++;
        id = vabId;
    }
    if (id >= 16) {
        _spu_setInTransfer(0);
        return -1;
    }
    p = addr;
    D_801D9638[id] = (VabHdr *)p;
    p += 0x20;
    hdr = (VabHdr *)addr;
    magic = hdr->form;
    D_801D96D8 = 0;
    if (magic >> 8 != ('V' << 16 | 'A' << 8 | 'B')) {
        D_801D9700[id] = 0;
        _spu_setInTransfer(0);
        D_801D9758--;
        return -1;
    }
    if ((magic & 0xFF) == 'p' && hdr->ver >= 5) {
        D_801D96C2 = 128;
    } else {
        D_801D96C2 = 64;
    }
    if (hdr->ps > D_801D96C2) {
        D_801D9700[id] = 0;
        _spu_setInTransfer(0);
        D_801D9758--;
        return -1;
    }
    D_801D95F8[id] = (ProgAtrV *)p;
    prog = (ProgAtrV *)p;
    p += D_801D96C2 * 16;
    size = 0;
    for (i = 0; i < D_801D96C2; i++) {
        prog[i].reserved1 = size;
        if (prog[i].tones != 0) {
            size++;
        }
    }
    size = 0;
    D_801D9680[id] = (VagAtr *)p;
    p += hdr->ps << 9;
    vags = hdr->vs;
    for (i = 0; i < 256; i++) {
        if (i <= vags) {
            len = *(u_short *)p;
            if (hdr->ver >= 5) {
                vagLens[i] = len * 8;
            } else {
                vagLens[i] = len * 4;
            }
            size += vagLens[i];
        }
        p += 2;
    }
    size = (size + 0x3F) & ~0x3F;
    spuAddr = alloc(size, sbaddr, id);
    if (spuAddr == -1) {
        return -1;
    }
    if (spuAddr + size > 0x80000) {
        D_801D9700[id] = 0;
        _spu_setInTransfer(0);
        D_801D9758--;
        return -1;
    }
    D_801D9760[id] = spuAddr;
    size = 0;
    for (i = 0; i <= vags; i++) {
        size += vagLens[i];
        if (!(i & 1)) {
            prog[i / 2].reserved2 = (spuAddr + size) >> 3;
        } else {
            prog[i / 2].reserved3 = (spuAddr + size) >> 3;
        }
    }
    D_801D9718[id] = size;
    D_801D9700[id] = 2;
    return id;
}

OBJECT_END(1);
