#include "common.h"
#include "gte.h"
#include "game.h"

void func_8002ADEC(void) {
    s8 *p;

    SsSetTableSize(&D_801D6B28, 0x20, 1);
    SsSetMVol(0, 0);
    SsSetTickMode(1);
    SsStart();
    func_8002B258(1);
    func_80055740();
    p = (s8 *)&D_801D8128;
    *(void **)(p + 0x1C) = func_8001ABCC(0x2100, -2);
    *(void **)(p + 0x28) = func_8001ABCC(0x9300, -2);
    *(void **)(p + 0x34) = func_8001ABCC(0x9300, -2);
    *(s16 *)(p + 0x2C) = 0xFF;
    *(s16 *)(p + 0x20) = 0xFF;
    *(s16 *)(p + 0x14) = 0xFF;
    *(s16 *)(p + 2) = -1;
    func_8002AEA4(1);
    SsSetMVol(0x7F, 0x7F);
}

void func_8002AEA4(s32 id) {
    char name[32];
    u8 *pak;
    SndSlot *se;

    se = &((SndState *)&D_801D8128)->unk14;
    if (se->id != id) {
        while (D_8006DFFC != 0) {
            func_80014C08(D_800794F0);
        }
        D_8006DFFC = 1;
        if (se->id != 0xFF) {
            func_8002B668();
            SsVabClose(se->vab);
        }
        se->id = id;
        /* written as a word here, read as a halfword by the SFX players */
        *(s32 *)&D_8006E048 = D_8006E000[id][0xF];
        sprintf(name, "A:\\SE%d.PAK", id);
        pak = (u8 *)func_8001B248((s32 *)name, func_800148B0(), -2);
        if (pak == 0) {
            se->id = 0xFF;
        } else {
            bcopy(pak, se->buf, 0x2030);
            if (func_8002B300(se, 0, 0x1010) != 0) {
                func_8002B38C(se, (s32)func_8001BB44((Chunk *)pak, 8, se->id), se->vab);
            } else {
                se->id = 0xFF;
            }
            func_8001AE90(pak);
        }
        D_8006DFFC = 0;
    }
}

void func_8002B024(s32 n, s32 id, u8 vol) {
    char name[32];
    u8 *pak;
    SndSlot *sl;

    sl = &((SndState *)&D_801D8128)->slot[n];
    if (sl->id == id) {
        return;
    }
    while (D_8006DFFC != 0) {
        func_80014C08(D_800794F0);
    }
    D_8006DFFC = 1;
    func_8002B2C0();
    if (sl->id != 0xFF) {
        if (((SndState *)&D_801D8128)->cur == n) {
            func_8002B688();
        }
        SsSeqClose(((SndState *)&D_801D8128)->seq[n]);
        SsVabClose(sl->vab);
        func_80014C08(D_800794F0);
    }
    sl->id = id;
    ((SndState *)&D_801D8128)->vol[n] = vol;
    sprintf(name, "A:\\BGM\\BGM%02d.PAK", id);
    pak = (u8 *)func_8001B248((s32 *)name, func_800148B0(), -2);
    if (pak == 0) {
        sl->id = 0xFF;
    } else {
        bcopy(pak, sl->buf, 0x9210);
        if (func_8002B300(sl, n + 1, n * 0x1A300 + 0x49E90) == 0) {
            func_8001AE90(pak);
            sl->id = 0xFF;
        } else {
            func_8002B38C(sl, (s32)func_8001BB44((Chunk *)pak, 8, sl->id), sl->vab);
            ((SndState *)&D_801D8128)->data[n] = func_8001BB44((Chunk *)sl->buf, 6, sl->id);
            ((SndState *)&D_801D8128)->seq[n] = SsSeqOpen(((SndState *)&D_801D8128)->data[n], sl->vab);
            func_8001AE90(pak);
        }
    }
    D_8006DFFC = 0;
}

void func_8002B258(s32 arg0) {
    if (arg0 == 0) {
        func_80051C70();
        SsUtSetReverbType(0);
        SsUtSetReverbDepth(0, 0);
        SpuClearReverbWorkArea(0);
        return;
    }
    SsUtSetReverbType((s16) arg0);
    func_80051C90();
    SsUtSetReverbDepth(0x64, 0x64);
}

void func_8002B2C0(void) {
    SpuVoiceAttr attr;

    attr.mask = 0x4000;
    attr.voice = 0xFFFFFF;
    attr.rr = 0;
    SpuSetVoiceAttr(&attr);
    VSync(0);
}

s32 func_8002B300(void *arg0, s16 arg1, s32 arg2) {
    u8 *vh;

    vh = func_8001BB44(*(Chunk **)((s8 *)arg0 + 8), 7, (*(s16 *)((s8 *)arg0 + 0)));
    if (vh != 0) {
        (*(s32 *)((s8 *)arg0 + 4)) = (*(s32 *)(vh - 4));
        if (((*(s16 *)((s8 *)arg0 + 2)) = SsVabOpenHeadSticky(vh, arg1, arg2)) != -1) {
            return 1;
        }
    }
    return 0;
}

void func_8002B38C(void *arg0, s32 arg1, s32 vab) {
    if ((arg1 == 0) || (SsVabTransBody(arg1, (*(s16 *)((s8 *)arg0 + 2))) == (*(s16 *)((s8 *)arg0 + 2)))) {
        SsVabTransCompleted(1);
    }
}

void func_8002B3DC(void) {
}

void func_8002B3E4(void) {
}

void func_8002B3EC(s32 arg0, s32 arg1) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, &func_8001B358, &D_80010598, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_801DFBAC(&D_800105A8);
    func_801E055C(arg0);
    func_80014A48(arg1);
}

void func_8002B498(s32 arg0) {
    s32 tone = arg0 & 0xF;

    SsUtKeyOnV(D_8006E044, D_801D813E, arg0 >> 4, tone, D_8006E048,
               D_8006E04C + tone, 0x6E, 0x6E);
    if (++D_8006E044 >= 0x16) {
        D_8006E044 = 0x12;
    }
}

void func_8002B530(s32 arg0, s32 vol) {
    s32 tone = arg0 & 0xF;

    SsUtKeyOnV(D_8006E044, D_801D813E, arg0 >> 4, tone, D_8006E048,
               D_8006E04C + tone, vol, vol);
    if (++D_8006E044 >= 0x16) {
        D_8006E044 = 0x12;
    }
}

void func_8002B5D0(s32 arg0, s32 arg1) {
    s32 tone = arg1 & 0xF;

    SsUtKeyOnV(arg0, D_801D813E, arg1 >> 4, tone, D_8006E048,
               D_8006E04C + tone, 0x6E, 0x6E);
}

void func_8002B644(s16 arg0) {
    SsUtKeyOffV(arg0);
}

void func_8002B668(void) {
    SsUtAllKeyOff(0);
}

void func_8002B688(void) {
    s16 *p;

    func_80014A00(0x1C);
    p = (s16 *)&D_801D8128;
    if (((s16 *)&D_801D8128)[1] >= 0) {
        SsSeqStop(((s16 *)&D_801D8128)[p[1] + 2]);
        func_80014C08(4);
        ((s16 *)&D_801D8128)[1] = -1;
    }
}

void func_8002B6E4(s32 idx, s32 step) {
    s16 vl;
    s16 vr;

    for (;;) {
        func_80014C08(D_800794F0);
        if (((SndState *)&D_801D8128)->cur != idx) {
            func_80014A90();
        }
        SsSeqGetVol(((SndState *)&D_801D8128)->seq[idx], 0, &vl, &vr);
        if (vl == 0) {
            SsSeqStop(((SndState *)&D_801D8128)->seq[idx]);
            func_80014C08(4);
            ((SndState *)&D_801D8128)->cur = -1;
            func_80014A90();
        }
        vl -= step;
        if (vl < 0) {
            vl = 0;
        }
        SsSeqSetVol(((SndState *)&D_801D8128)->seq[idx], vl, vl);
    }
}

void func_8002B7DC(s32 arg0) {
    s16 *p = (s16 *)&D_801D8128;

    if (p[1] >= 0) {
        func_80014A00(0x1C);
        func_800149B8(0x1C, -1, 0, 0x1000, &func_8002B6E4, p[1], arg0);
    }
}

void func_8002B850(void) {
}

void func_8002B858(s32 arg0) {
    if (((SndState *)&D_801D8128)->slot[arg0].id != 0xFF) {
        if (((SndState *)&D_801D8128)->cur >= 0) {
            func_8002B688();
        }
        SsSeqPlay(((SndState *)&D_801D8128)->seq[arg0], 1, 0);
        SsSeqSetVol(((SndState *)&D_801D8128)->seq[arg0],
                    ((SndState *)&D_801D8128)->vol[arg0],
                    ((SndState *)&D_801D8128)->vol[arg0]);
        ((SndState *)&D_801D8128)->cur = arg0;
    }
}

void func_8002B900(s32 seq, s32 arg1, s32 arg2, s32 load) {
    D_8006E040++;
    do {
        func_80014C08(D_800794F0);
    } while (D_8006E03C != 0);
    D_8006E03C = 1;
    if (((SndState *)&D_801D8128)->cur >= 0) {
        func_8002B7DC(2);
        while (((SndState *)&D_801D8128)->cur >= 0) {
            func_80014C08(D_800794F0);
        }
    }
    if (load) {
        func_8002B024(seq, arg1, arg2);
    }
    func_8002B858(seq);
    D_8006E03C = 0;
    D_8006E040--;
    func_80014A90();
}

void func_8002BA24(void) {
    do {
        func_80014C08(D_800794F0);
    } while (D_8006E040 != 0);
}

void func_8002BA6C(s32 arg0, s32 arg1, s32 arg2) {
    s8 *base;

    base = (s8 *)&D_801D8128;
    if ((*(s16 *)(base + arg0 * 0xC + 0x20)) != arg1) {
        func_8002BA24();
        func_800149B8(0, -1, 0, 0x1000, &func_8002B900, arg0, arg1, arg2, 1);
        return;
    }
    if (D_801D812A != arg0) {
        func_8002BA24();
        func_800149B8(0, -1, 0, 0x1000, &func_8002B900, arg0, arg1, arg2, 0);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/system/sound", D_80010598);

INCLUDE_RODATA("asm/main/nonmatchings/system/sound", D_800105A8);
