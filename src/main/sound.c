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

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80010598);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800105A8);

void func_8002BB58(u32 arg0) {
    s32 var_a0;

    var_a0 = 0;
    if (D_801D813C == 0) {
        switch (arg0) {
        case 0:
            var_a0 = 0xA1;
            break;
        case 1:
            var_a0 = 0xA0;
            break;
        case 2:
            var_a0 = 0xA2;
            break;
        case 3:
            var_a0 = 0xA3;
            break;
        case 4:
            var_a0 = 0xA4;
            break;
        }
    } else {
        switch (arg0) {
        case 0:
            var_a0 = 1;
            break;
        case 1:
            var_a0 = 0;
            break;
        case 2:
            var_a0 = 2;
            break;
        case 3:
            var_a0 = 3;
            break;
        case 4:
            var_a0 = 4;
            break;
        }
    }
    func_8002B498(var_a0);
}

s32 func_8002BC2C(void) {
    s32 var_v1;

    var_v1 = 0;
    if (D_8006E03C == 0) {
        var_v1 = D_8006E040 == 0;
    }
    return var_v1;
}

void func_8002BC58(void) {
    InitCARD(0);
    func_8002BC80();
}

void func_8002BC80(void) {
    s32 i;

    VSync(2);
    D_801D8160 = func_8006A794(0xF4000001, 4, 0x2000, 0);
    D_801D8164 = func_8006A794(0xF4000001, 0x8000, 0x2000, 0);
    D_801D8168 = func_8006A794(0xF4000001, 0x100, 0x2000, 0);
    D_801D816C = func_8006A794(0xF4000001, 0x2000, 0x2000, 0);
    D_801D8170 = func_8006A794(0xF0000011, 4, 0x2000, 0);
    D_801D8174 = func_8006A794(0xF0000011, 0x8000, 0x2000, 0);
    D_801D8178 = func_8006A794(0xF0000011, 0x100, 0x2000, 0);
    D_801D817C = func_8006A794(0xF0000011, 0x2000, 0x2000, 0);
    StartCARD();
    func_80068804();
    func_8006A7C4(D_801D8160);
    func_8006A7C4(D_801D8164);
    func_8006A7C4(D_801D8168);
    func_8006A7C4(D_801D816C);
    func_8006A7C4(D_801D8170);
    func_8006A7C4(D_801D8174);
    func_8006A7C4(D_801D8178);
    func_8006A7C4(D_801D817C);
    for (i = 0; i < 2; i++) {
        D_801D8190[i] = func_8001ACEC(0x260);
    }
    D_801D81A0 = func_8001ACEC(0x200);
}

s32 func_8002BE84(s32 arg0) {
    s32 n;

    n = 0;
    D_801D8198 = 0;
    do {
        if (func_8006A7B4(D_801D8160) == 1) {
            return 0;
        }
        if (func_8006A7B4(D_801D8164) == 1) {
            return 1;
        }
        if (func_8006A7B4(D_801D8168) == 1) {
            return 2;
        }
        if (func_8006A7B4(D_801D816C) == 1) {
            return 3;
        }
        if (arg0 != 0) {
            if (n++ >= 0x1F) {
                break;
            }
            func_80014C08(arg0);
        }
    } while (D_801D8198 < 0x259);
    return 2;
}

void func_8002BF60(void) {
    func_8006A7B4(D_801D8160);
    func_8006A7B4(D_801D8164);
    func_8006A7B4(D_801D8168);
    func_8006A7B4(D_801D816C);
}

s32 func_8002BFB8(s32 arg0) {
    s32 n;

    n = 0;
    D_801D8198 = 0;
    do {
        if (func_8006A7B4(D_801D8170) == 1) {
            return 0;
        }
        if (func_8006A7B4(D_801D8174) == 1) {
            return 1;
        }
        if (func_8006A7B4(D_801D8178) == 1) {
            return 2;
        }
        if (func_8006A7B4(D_801D817C) == 1) {
            return 3;
        }
        if (arg0 != 0) {
            if (n++ >= 0x1F) {
                break;
            }
            func_80014C08(arg0);
        }
    } while (D_801D8198 < 0x259);
    return 2;
}

void func_8002C094(void) {
    func_8006A7B4(D_801D8170);
    func_8006A7B4(D_801D8174);
    func_8006A7B4(D_801D8178);
    func_8006A7B4(D_801D817C);
}

s32 func_8002C0EC(s32 arg0) {
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s0;

    var_s0 = 0;
loop_1:
    func_8002BF60();
    func_80068814(arg0 * 0x10);
    temp_v0 = func_8002BE84(0);
    if ((u32) (temp_v0 - 1) < 2U) {
        if (var_s0 >= 5) {
            return 1;
        }
        goto block_6;
    }
    if (temp_v0 == 3) {
        if (var_s0 < 3) {
block_6:
            var_s0 += 1;
            func_80014C08(D_800794F0);
            goto loop_1;
        }
        if (temp_v0 == 3) {
            temp_s0 = arg0 * 0x10;
            func_8002C094();
            _card_clear(temp_s0);
            func_8002BFB8(1);
            func_8002BF60();
            func_80068824(temp_s0);
            func_8002BE84(0);
        }
        /* Duplicate return node #9. Try simplifying control flow for better match */
        return 0;
    }
    return 0;
}

s32 func_8002C1C0(s32 port) {
    s32 r;
    s32 tries;
    s32 retry;

    tries = 0;
    retry = 0;
loop:
    func_8002BF60();
    func_80068814(port * 16);
    r = func_8002BE84(1);
    if (r == 1 || r == 2) {
        if (retry >= 5) {
            return 1;
        }
        retry++;
    } else {
        if (r == 3) {
            if (retry < 3) {
                retry++;
                goto wait;
            }
            retry++;
            if (r == 3) {
                func_8002C094();
                _card_clear(port * 16);
                r = func_8002BFB8(1);
                if (r == 1 || r == 2) {
                    retry = 0;
                    if (tries >= 5) {
                        return 1;
                    }
                    tries++;
                    goto wait;
                }
            }
        }
        func_8002BF60();
        func_80068824(port * 16);
        r = func_8002BE84(0);
        if (r == 0) {
            goto done;
        }
        retry = 0;
        if (tries >= 5) {
            if (r == 3) {
                return 2;
            }
            return 1;
        }
        tries++;
    }
wait:
    func_80014C08(4);
    goto loop;
done:
    return 0;
}

s32 func_8002C2E4(s32 arg0) {
    return _card_format(arg0 * 0x10) == 1;
}

s32 func_8002C30C(s32 slot, u8 blocks, s32 arg2, s32 arg3, McHeader *hdr) {
    char name[32];
    s32 fd;

    ((u8 *)hdr)[3] = blocks;
    sprintf(name, &D_800105E4, slot, arg3);
    func_8006A864(func_8006A824(name, (((u8 *)hdr)[3] << 16) | 0x200));
    *(McHeader *)D_801D81A0 = *hdr;
    D_801D8184 = fd = func_8006A824(name, 0x8002);
    if (fd == -1) {
        return -1;
    }
    D_801D8180 = 0;
    D_801D8188 = arg2;
    if (func_8002C0EC(slot) != 0) {
        return -1;
    }
    return 0;
}

s32 func_8002C468(void) {
    s32 r;
    s32 start;
    s32 end;
    s32 off;

    func_8002BF60();
    switch (D_801D8180) {
    case 0:
        start = D_801D81A0[2] * 128 - 0x780;
        func_8006A834(D_801D8184, 0, 0);
        if (func_8006A854(D_801D8184, D_801D81A0, start) == -1) {
            return -1;
        }
        D_801D8180++;
    case 1:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        start = D_801D81A0[2] * 128 - 0x780;
        end = D_801D81A0[3] * 0x2000;
        D_801D819C = (end - start) / 128;
        D_801D818C = 0;
        D_801D8180++;
        return 0;
    case 2:
        func_8006A834(D_801D8184, ((D_801D81A0[2] - 0x10) << 7) + 0x80 + (D_801D818C << 7), 0);
        if (func_8006A854(D_801D8184, (void *)(D_801D8188 + (D_801D818C << 7)), 0x80) == -1) {
            return -1;
        }
        D_801D8180++;
    case 3:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        D_801D818C++;
        D_801D8180 = 2;
        if (D_801D818C == D_801D819C) {
            func_8006A864(D_801D8184);
        }
        break;
    }
    return D_801D818C * 100 / D_801D819C;
}

s32 func_8002C6EC(s32 slot, s32 arg1, s32 arg2) {
    char name[32];
    s32 fd;

    sprintf(name, &D_800105E4, slot, arg2);
    D_801D8184 = fd = func_8006A824(name, 0x8001);
    if (fd == -1) {
        return -1;
    }
    D_801D8180 = 0;
    D_801D8188 = arg1;
    if (func_8002C0EC(slot) != 0) {
        return -1;
    }
    return 0;
}

s32 func_8002C784(void) {
    s32 r;
    s32 start;
    s32 end;
    s32 off;

    func_8002BF60();
    switch (D_801D8180) {
    case 0:
        func_8006A834(D_801D8184, 0, 0);
        if (func_8006A844(D_801D8184, D_801D81A0, 0x80) == -1) {
            return -1;
        }
        D_801D8180++;
    case 1:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        start = D_801D81A0[2] * 128 - 0x780;
        end = D_801D81A0[3] * 0x2000;
        D_801D819C = (end - start) / 128;
        D_801D818C = 0;
        D_801D8180++;
        return 0;
    case 2:
        func_8006A834(D_801D8184, ((D_801D81A0[2] - 0x10) << 7) + 0x80 + (D_801D818C << 7), 0);
        if (func_8006A844(D_801D8184, (void *)(D_801D8188 + (D_801D818C << 7)), 0x80) == -1) {
            return -1;
        }
        D_801D8180++;
    case 3:
        r = func_8002BE84(1);
        if (r == 1 || r == 2) {
            func_8006A864(D_801D8184);
            return -1;
        }
        D_801D818C++;
        D_801D8180 = 2;
        if (D_801D818C == D_801D819C) {
            func_8006A864(D_801D8184);
        }
        break;
    }
    return D_801D818C * 100 / D_801D819C;
}

s32 func_8002C9E8(s32 arg0, void *arg1, s32 arg2) {
    char name[32];
    s32 fd;

    sprintf(name, &D_800105E4, arg0, arg2);
    fd = func_8006A824(name, 1);
    if (fd == -1) {
        return 1;
    }
    if (func_8006A844(fd, D_801D81A0, 0x80) == -1) {
        func_8006A864(fd);
        return 1;
    }
    if (func_8006A834(fd, ((*(u8 *)((s8 *)D_801D81A0 + 2)) - 0x10) << 7, 1) == -1) {
        func_8006A864(fd);
        return 1;
    }
    if (func_8006A844(fd, arg1, 0x80) == -1) {
        func_8006A864(fd);
        return 1;
    }
    func_8006A864(fd);
    return 0;
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800105E4);

void func_8002CAC8(s32 port) {
    char name[8];
    DirEntry *d;
    s32 count;
    s32 total;

    count = 0;
    total = 0;
    sprintf(name, "bu%1d0:*", port);
    d = D_801D8190[port]->files;
    if (firstfile(name, d) == d) {
        do {
            total += d->size;
            count++;
            d++;
        } while (func_8006A874(d) == d);
    }
    D_801D8190[port]->count = count;
    D_801D8190[port]->blocks = total /= 8192;
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800105FC);

s32 func_8002CBA0(s32 len, u8 *p) {
    s32 i;
    u8 x = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        x ^= *p;
        sum += *p;
        p++;
    }
    if (p[0] != x || p[1] != sum) {
        return 1;
    }
    return 0;
}

void func_8002CC04(s32 len, u8 *p) {
    s32 i;
    u8 x = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        x ^= *p;
        sum += *p;
        p++;
    }
    p[0] = x;
    p[1] = sum;
}

void func_8002CC44(s32 p) {
    s32 count[6];
    s32 total;
    s32 rank;
    s32 i;
    s32 n;

    rank = PLAYER_DATA(p).rankA;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(p).unk18 < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(p).unk18 < 25) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(p).unk18 < 50) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(p).unk18 < 100) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(p).unk18 < 200) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(p).unk18 < 300) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(p).unk18 < 500) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(p).rankA = rank;

    total = 0;
    for (i = 0; i < 6; i++) {
        count[i] = 0;
    }
    for (i = 0; i < 0xAC; i++) {
        n = PLAYER_DATA(p).unk14B2[i] & 7;
        if (n != 0) {
            total += n;
            count[D_801D8408[i * 0x13C + 0x1A] >> 4]++;
        }
    }
    for (i = 0xBF; i < 0x125; i++) {
        n = PLAYER_DATA(p).unk14B2[i] & 7;
        if (n != 0) {
            total += n;
            count[5]++;
        }
    }
    for (i = 0x125; i < 0x12D; i++) {
        n = PLAYER_DATA(p).unk14B2[i] & 7;
        if (n != 0) {
            total += n;
            count[5]++;
        }
    }
    n = 0;
    for (i = 0; i < 6; i++) {
        if (count[i] == D_8006E0B8[i]) {
            n++;
        }
    }

    rank = PLAYER_DATA(p).rankB;
    switch (rank) {
    case 0:
        if (total < 100) {
            break;
        }
        rank = 1;
    case 1:
        if (total < 200) {
            break;
        }
        rank = 2;
    case 2:
        if (n <= 0) {
            break;
        }
        rank = 3;
    case 3:
        if (n < 3) {
            break;
        }
        rank = 4;
    case 4:
        if (n < 5) {
            break;
        }
        rank = 5;
    case 5:
        if (n < 6) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(p).unk28_11) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(p).rankB = rank;

    rank = PLAYER_DATA(p).rankC;
    switch (rank) {
    case 0:
        if (PLAYER_DATA(p).unk1C < 10) {
            break;
        }
        rank = 1;
    case 1:
        if (PLAYER_DATA(p).unk1C < 20) {
            break;
        }
        rank = 2;
    case 2:
        if (PLAYER_DATA(p).unk1C < 30) {
            break;
        }
        rank = 3;
    case 3:
        if (PLAYER_DATA(p).unk1C < 40) {
            break;
        }
        rank = 4;
    case 4:
        if (PLAYER_DATA(p).unk1C < 60) {
            break;
        }
        rank = 5;
    case 5:
        if (PLAYER_DATA(p).unk1C < 80) {
            break;
        }
        rank = 6;
    case 6:
        if (PLAYER_DATA(p).unk1C < 100) {
            break;
        }
        rank = 7;
    }
    PLAYER_DATA(p).rankC = rank;
}

void func_8002D404(void) {
    void *p;

    func_800457FC();
    D_8006E050 = func_8001ACEC(0x4EE8);
    D_8006E054 = p = func_8001ACEC(0x102C);
    (*(void **)((s8 *)D_8006E054 + 0x100C)) = func_8001ACEC(0x1AC);
    func_8002D51C();
}

void func_8002D458(void) {
    s32 i;

    ((Unk8006E054 *)D_8006E054)->unk1027 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A4 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A2 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A9 = 0;
    ((Unk8006E054 *)D_8006E054)->unk100C->unk1A8 = 0;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)D_8006E050)->unk23FC[i] = 0;
    }
    for (i = 0; i < 9; i++) {
        ((Unk8006E050 *)D_8006E050)->unk242C[i] = 0;
    }
    ((Unk8006E050 *)D_8006E050)->unk2C = 0;
    ((Unk8006E050 *)D_8006E050)->unk14 = 0;
}

void func_8002D51C(void) {
    Unk8006E050 *e;
    s32 p;
    s32 j;
    s32 i;

    e = (Unk8006E050 *)D_8006E050;
    for (i = 0; i < 12; i++) {
        ((Unk8006E050 *)D_8006E050)->unk23FC[i] = 0;
    }
    ((Unk8006E050 *)D_8006E050)->unk28_9 = 0;
    for (p = 0; p < 2; p++, e++) {
        e->name[0] = 0;
        e->unk18 = 0;
        e->unk1A = 0;
        e->unk1C = 0;
        e->unk1E = 0;
        e->unkE = 0;
        e->unk10 = rand();
        e->unk28_10 = 0;
        e->unk28_13 = 0;
        e->unkD = 0;
        e->unk28_11 = 0;
        e->unk28_12 = 0;
        e->rankA = 0;
        e->rankB = 0;
        e->rankC = 0;
        e->unk16 = 0x2774;
        e->unk4C = 0;
        e->unk4E = 0;
        e->unk50 = 0;
        e->unk52 = 0;
        e->unk54 = 0;
        e->unk56 = 0;
        for (i = 0; i < 3; i++) {
            e->unk36[i] = 0;
        }
        for (i = 0; i < 0x28; i++) {
            e->unk58[i] = 0;
        }
        for (i = 0; i < 0x12D; i++) {
            e->unk14B2[i] = 0;
            for (j = 0; j < 8; j++) {
                func_80045968(p, i, j);
            }
        }
        for (i = 0; i < 0xBF; i++) {
            for (j = 0; j < 3; j++) {
                e->unkD3C[i][j] = 0;
            }
            e->unk11B6[i] = 0;
            e->unk1334[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            e->unk80[i].unk288 = 0;
        }
        for (i = 0; i < 0x10; i++) {
            e->unk3C[i] = 0;
        }
        for (i = 0; i < 3; i++) {
            e->unk2438[i].unk0 = 0;
            e->unk2438[i].unk108[0] = 0;
            e->unk2438[i].unk108[1] = 0;
            e->unk2438[i].unk108[2] = 0;
        }
        for (i = 0; i < 0x9F; i++) {
            e->unkAC0[i] = 0;
            e->unkBFE[i] = 0;
        }
        for (i = 0; i < 0x8E; i++) {
            e->unk888[i] = 0;
            e->unk9A4[i] = 0;
        }
        for (j = 0; j < 0x20; j++) {
            e->unk848[j] = 0;
        }
        e->unk20_0 = 0;
        e->unk20_1 = 0;
        e->unk20_2 = 0;
        e->unk20_3 = 0;
        e->unk24 = 0;
    }
    strcpy(((Unk8006E050 *)D_8006E050)->name, "Player");
    func_8002D458();
}

void func_8002D898(void) {
    CUR_SPRT->sp.x0 = 0;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x100;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x85);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
    CUR_SPRT->sp.x0 = 0x100;
    CUR_SPRT->sp.y0 = 0;
    CUR_SPRT->sp.u0 = 0;
    CUR_SPRT->sp.v0 = 0;
    CUR_SPRT->sp.clut = 0x3FD4;
    CUR_SPRT->sp.w = 0x40;
    CUR_SPRT->sp.h = 0xF0;
    setSemiTrans(&CUR_SPRT->sp, 0);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x87);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
}

void func_8002DAAC(s32 arg0, s32 arg1) {
    void *temp_s1;

    temp_s1 = D_801D6A4C->unk13C[arg0];
    if ((*(s32 *)((s8 *)temp_s1 + 0x2200)) != arg1) {
        func_8001AFF0(arg0 + 0x84);
        func_80023094(temp_s1, (s32 *)func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s1 + 0x26F4), 1, arg1), arg0 + 0x84), arg1);
    }
    func_80022D34(arg0, arg1, -2, 0);
}

void func_8002DB58(s32 arg0, s32 arg1) {
    s32 temp_s2;
    void *temp_s3;

    temp_s3 = D_801D6A4C->unk13C[arg0];
    temp_s2 = arg0 + 0x84;
    func_8001AFF0(temp_s2);
    func_80023094(temp_s3, (s32 *)func_8001BFF8((s32)func_8001BB44(*(Chunk **)((s8 *)temp_s3 + 0x26F4), 1, arg1), temp_s2), arg1);
    func_80023148(arg0, arg1);
}

void *func_8002DBEC(s32 arg0) {
    u8 *p;
    s32 i;

    p = D_801D8408;
    if (p[0xE5] != arg0) {
        i = 0;
        do {
            i++;
            p += 0x13C;
            if (i >= 0xBF) {
                break;
            }
        } while (p[0xE5] != arg0);
    }
    return p;
}

s32 func_8002DC30(s32 arg0, s32 arg1) {
    char sp10[32];
    s32 var_v0;

    var_v0 = (s32)func_8001BB44((Chunk *)arg1, 2, arg0);
    if (var_v0 == 0) {
        sprintf(sp10, &D_800107F8, arg0);
        var_v0 = func_8001B248(sp10, func_800148B0(), 0x81);
    }
    return var_v0;
}

void func_8002DC90(s32 arg0) {
    func_8002DC30(arg0, 0);
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800107F8);

s32 func_8002DCB0(s32 slot, s32 id, s8 kind, s32 anims) {
    char path[32];
    s32 pak;

    if (kind == 1) {
        sprintf(path, "G:\\%03d.PAK", id);
    } else {
        sprintf(path, "F:\\%03d.PAK", id);
    }
    pak = func_8001B248((s32 *)path, func_800148B0(), slot + 0x1F4);
    if (func_8002386C(slot, id, -1, pak, kind) == 0) {
        return pak;
    }
    if (anims != 0) {
        func_800230B8(slot, 0, 0, pak);
        func_800230B8(slot, 7, 7, pak);
        func_800230B8(slot, 1, 1, pak);
        func_800230B8(slot, 2, 2, pak);
        func_800230B8(slot, 3, 3, pak);
        func_800230B8(slot, 4, 4, pak);
        func_800230B8(slot, 5, 5, pak);
        func_800230B8(slot, 6, 6, pak);
    } else {
        func_80023094(D_801D6A4C->unk13C[slot],
                      (s32 *)func_8001BFF8(
                          (s32)func_8001BB44((Chunk *)((Model2220 *)D_801D6A4C->unk13C[slot])->unk26F4, 1, 7), slot + 0x84),
                      7);
        func_80023148(slot, 7);
    }
    D_801D6A4C->unk114[slot] = -1;
    func_8001BC14((Chunk *)pak);
    return pak;
}

void func_8002DEA0(s32 arg0, void *arg1) {
    s32 temp_s5;
    s32 temp_s6;
    s32 temp_v0;
    void *temp_s1;
    void *temp_s2;

    temp_s1 = (void *)((s8 *)&D_801D81B8 + (arg0 << 5));
    temp_s5 = (*(u8 *)((s8 *)arg1 + 0xE5));
    temp_s6 = (*(s32 *)((s8 *)temp_s1 + 0));
    if (temp_s5 != temp_s6) {
        (*(s32 *)((s8 *)temp_s1 + 0)) = -2;
        if ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1) {
            do {
                func_80014C08(D_800794F0);
            } while ((*(s8 *)((s8 *)D_801D8340 + 0x811)) == 1);
        }
        (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 1;
        if (temp_s6 > 0) {
            func_800235C8(arg0);
            func_8001AFF0(arg0 + 0x1F4);
            func_8001AFF0(arg0 + 0x84);
        }
        if (temp_s5 > 0) {
            temp_s2 = func_8002DBEC(temp_s5);
            temp_v0 = func_8002DCB0(arg0, temp_s5, 0, 0);
            if (temp_v0 != 0) {
                (*(s32 *)((s8 *)temp_s1 + 8)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x22)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0xC)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x3E)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x10)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x5A)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x14)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x24)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x18)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x40)), temp_v0);
                (*(s32 *)((s8 *)temp_s1 + 0x1C)) = func_8002DC30((s32) (*(s16 *)((s8 *)temp_s2 + 0x5C)), temp_v0);
                goto block_9;
            }
        } else {
block_9:
            (*(s32 *)((s8 *)temp_s1 + 0)) = temp_s5;
            (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
        }
    }
}

void func_8002E034(s32 bg) {
    s32 pak;
    s32 i;
    u8 *s;

    if (*((s8 *)D_801D8340 + 0x811) == 1) {
        do {
            func_80014C08(D_800794F0);
        } while (*((s8 *)D_801D8340 + 0x811) == 1);
    }
    *((s8 *)D_801D8340 + 0x811) = 1;
    *((s8 *)D_801D8340 + 0x813) = 0;
    pak = func_8001B144((s32) "A:\\BATTLE.PAK", func_800148B0());
    if (pak != 0) {
        func_8001B5BC(func_8001BB44((Chunk *)pak, 5, 0x68));
        D_801D81AC = (void *)func_8002DC30(999, pak);
        D_801D81B0 = (void *)func_8002DC30(998, pak);
        func_8001BC14((Chunk *)pak);
    }
    func_8002E42C(bg);
    D_801D81B8 = (&D_801D81B8)[8] = -1;
    *((s8 *)D_801D8340 + 0x811) = 0;
    do {
        func_80014C08(D_800794F0);
        for (i = 0; i < 2; i++) {
            s = *(u8 **)(D_801D8348[i] + 0x114);
            if (s != 0 && s[0xE5] != (&D_801D81B8)[i * 8]) {
                func_8002DEA0(i, s);
            }
        }
    } while (*((s8 *)D_801D8340 + 0x813) == 0);
    for (i = 0; i < 2; i++) {
        if ((&D_801D81B8)[i * 8] > 0) {
            func_80022DBC(i);
            func_800235C8(i);
        }
    }
    func_8002E7B8();
    func_8001AFF0(0x1F4);
    func_8001AFF0(0x84);
    func_8001AFF0(0x1F5);
    func_8001AFF0(0x85);
    func_8001AFF0(0x81);
    *((s8 *)D_801D8340 + 0x813) = 0;
}

void func_8002E26C(void) {
    do {
        func_80014C08(D_800794F0);
    } while (D_801D81B8 <= 0 || (&D_801D81B8)[8] <= 0 || *((s8 *)D_801D8340 + 0x811) == 1);
    *((s8 *)D_801D8340 + 0x811) = 1;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\sugseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_8002B858(1);
    func_800149B8(0, -1, 0, 0x2000, D_801EEE90, 0, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    D_80079544 = 0;
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\kawseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_8002B858(0);
    *((s8 *)D_801D8340 + 0x811) = 0;
}

void func_8002E42C(s32 n) {
    char path[32];

    if (n < 0) {
        n = rand() % 12 + 0x2C;
    }
    sprintf(path, "F:\\bg%d.pak", D_8006E0C0[n].bg + 900);
    func_800149B8(0, -1, 0, 0x400, func_8001B248, path, func_800148B0(), 0x81);
    D_801D81A8 = func_80014C08(0x7FFFFFFF);
    func_8002386C(0x17, D_8006E0C0[n].bg + 900, 0, D_801D81A8, 0);
    D_801D6A4C->unk114[0x17] = -1;
    ((Model2220 *)D_801D6A4C->unk13C[23])->unk26D4 = 0xA0000;
    ((Model2220 *)D_801D6A4C->unk13C[23])->unk26D0 = 0x280000;
    if (D_8006E0C0[n].flags & 2) {
        func_800230B8(0x17, 0, 0, D_801D81A8);
        func_80023148(0x17, 0);
        func_80022D34(0x17, 0, -2, 0);
    }
    D_801D6A4C->unk114[0x18] = D_801D6A4C->unk114[0x19] = 0;
    D_801D6A4C->unk114[0x1B] = D_8006E0C0[n].unk2;
    D_801D6A4C->unk114[0x1A] = D_8006E0C0[n].unk1;
    *(s32 *)&D_801D6A4C->unk114[0x24] = D_8006E0C0[n].flags;
    D_801D6A60[0] = D_8006E0C0[n].rgb[0];
    D_801D6A60[1] = D_8006E0C0[n].rgb[1];
    D_801D6A60[2] = D_8006E0C0[n].rgb[2];
    D_8006DF80 = D_8006E0C0[n].unk7;
}

void func_8002E658(s16 id) {
    Unk801D6A4C *p;
    s32 tim;

    D_80079544 = 1;
    p = D_801D6A4C;
    *(s16 *)((u8 *)p->unk13C[23] + 0xA78) = id;
    tim = func_8001C078((s32)func_8001BB44((Chunk *)D_801D81A8, 5, *(s16 *)((u8 *)p->unk13C[23] + 6)));
    func_8001B438((u32 *)tim, 0x3C0, 0, 0x3F0, 0x70);
    DrawSync(0);
    func_8001AE90((void *)tim);
    if (id != 0 && (*(s32 *)&D_801D6A4C->unk114[0x24] & 2)) {
        func_80014A00(0x1B);
        func_800149B8(0x1B, -1, 0, 0x1000, func_80022B98, 1);
        func_80023148(0x17, 0);
        func_80022D34(0x17, 0, -2, 0);
    }
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.r0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.r0 = D_801D6A60[0];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.g0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.g0 = D_801D6A60[1];
    ((Unk800794F8 *)&D_800794F8)->unk98[0].draw.b0 = ((Unk800794F8 *)&D_800794F8)->unk98[1].draw.b0 = D_801D6A60[2];
}

void func_8002E7B8(void) {
    func_800235C8(0x17);
    func_8001AE90(D_801D81A8);
}

void func_8002E7E8(u8 *arg0) {
    s32 f;

    if (D_801D6A4C->unk114[0x1B] != 0) {
        if (++D_801D6A4C->unk114[0x19] >= D_801D6A4C->unk114[0x1B]) {
            f = *(s32 *)(arg0 + 0x26D4) / 0x10000 + 5;
            D_801D6A4C->unk114[0x19] = 0;
            if (++D_801D6A4C->unk114[0x18] >= (u8)D_801D6A4C->unk114[0x24] >> 3) {
                D_801D6A4C->unk114[0x18] = 0;
            }
            *(s32 *)(arg0 + 0x26D0) =
                (((((f & 0x10) << 4) + D_801D6A4C->unk114[0x18]) << 6 | (f & 0xF) << 2) - 0x14) << 16;
        }
    }
}

void func_8002E8EC(s32 mode) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, mode, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 2:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 1, 1, func_800148B0(), 0);
        break;
    case 4:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

void func_8002EB1C(void) {
    func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 0xFF, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    if (D_801F80C1 != 0) {
        func_8002FAC8();
        func_800149B8(0, -1, 0, 0x100, func_8002F4F4, 0, 0, 0, 0);
        return;
    }
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    if (*(u8 *)(D_8006E050 + 0xF) == 0) {
        func_8002B024(0, 0x6F, 0x7F);
        func_8002B858(0);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 0, func_800148B0(), 0);
    } else {
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
    }
}

void func_8002ECDC(s8 arg0) {
    func_80014C08(2);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010884, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E8E88, (s32 *) arg0, 0, 0, 0);
}

void func_8002ED9C(void) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
}

void func_8002EE50(s32 mode) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, 0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, func_800148B0(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

void func_8002F074(s32 mode) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, 0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    switch (mode) {
    case 0:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x400, D_801F00F4, 0, 1, func_800148B0(), 0);
        break;
    case 1:
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
        break;
    }
}

void func_8002F298(s32 *arg0) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x600, D_801E8C04, arg0, func_800148B0(), 0, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

void func_8002F3C4(s32 *arg0) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010894, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, D_801E4B34, arg0, func_800148B0(), 1, 0);
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
}

void func_8002F4F4(void) {
    s32 stack;
    s32 again;
    s32 r;

    stack = func_800148B0();
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010864, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    do {
        func_800149B8(0, -1, 0, 0x800, D_801EA2F8, stack, 0, 0, 0);
        r = func_80014C08(0x7FFFFFFF);
        again = 0;
        switch (r) {
        case 0:
            func_8002F920(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, D_801E6454, stack, 0, 0, 0);
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_80010874, D_80010C9C, func_800148B0());
            func_80014C08(0x7FFFFFFF);
            func_80014C08(2);
            func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
            break;
        case 1:
            func_8002F920(6, 0x380, 0, 0x380, 0x80);
            func_800149B8(0, -1, 0, 0x800, func_8002EB1C, 0, 0, 0, 0);
            break;
        case 2:
            func_8002F920(7, 0x380, 0, 0x380, 0x80);
            *((u8 *)D_8006E054 + 0x1028) = 0;
            again = func_801EBD34();
            if (again == 0) {
                func_800149B8(0, -1, 0, 0x800, D_801EB2E8, stack, 0, 0, 0);
            } else {
                func_8002FAC8();
            }
            break;
        }
    } while (again);
}

void func_8002F79C(void) {
    D_801D8260 = 0;
}

void func_8002F7A8(void) {
    s16 r[4];
    s32 i;
    s8 *p;
    s8 *e;

    p = (s8 *)&D_801D81F8;
    if ((*(s32 *)(p + 0x68)) != 0) {
        return;
    }
    (*(s8 *)(p + 0x6C)) = -1;
    (*(s8 *)(p + 0x6D)) = -1;
    (*(s16 *)(p + 0x72)) = 0;
    (*(s16 *)(p + 0x70)) = 0;
    (*(s8 *)(p + 0x6E)) = 0;
    (*(s8 *)(p + 0x6F)) = 5;
    for (i = 0; i < 2; i++) {
        e = (s8 *)&D_801D81F8 + i * 0x34;
        func_8001E6EC(0xE, e, 0, 0);
        (*(s16 *)(e + 0x10)) = 0x141;
        (*(s16 *)(e + 0x12)) = 0xF0;
        r[0] = 0;
        r[1] = 0;
        r[2] = 0;
        r[3] = 0;
        SetTexWindow((s8 *)&D_801D8220 + i * 0x34, r);
    }
    func_800149B8(0, -1, 0, 0x800, func_8001B248, &D_800108A4, func_800148B0(), -2);
    D_801D8260 = func_80014C08(0x7FFFFFFF);
}

void func_8002F8E8(void) {
    s8 *p = (s8 *)&D_801D81F8;

    func_8001AE90(*(void **)(p + 0x68));
    *(void **)(p + 0x68) = 0;
    func_8002FAA8();
}

void func_8002F920(s32 mode, s32 x, s32 y, s32 w, s32 h) {
    s32 i;

    if (D_801D81F8.unk72 != 0 && D_801D81F8.unk72 != 0x80) {
        do {
            func_80014C08(D_800794F0);
        } while (D_801D81F8.unk72 != 0 && D_801D81F8.unk72 != 0x80);
    }
    if (D_801D81F8.unk72 == 0) {
        D_801D81F8.unk6D = -1;
    }
    D_801D81F8.mode = mode;
    if (mode >= 0) {
        D_801D81F8.x = x;
        D_801D81F8.y = y;
        D_801D81F8.w = w;
        D_801D81F8.h = h;
        for (i = 0; i < 2; i++) {
            (D_801D81F8.buf + i)->clut = getClut(w, h);
            SetDrawTPage((D_801D81F8.buf + i)->tpage, 0, 0, GetTPage(0, 0, x, y));
        }
    }
    D_801D81F8.unk6E = 0;
    D_801D81F8.unk6F = 0x1E;
}

void func_8002FAA8(void) {
    s8 *p;

    p = (s8 *)&D_801D81F8;
    p[0x6C] = -1;
    p[0x6D] = -1;
    (*(s16 *)(p + 0x72)) = 0;
    (*(s16 *)(p + 0x70)) = 0;
}

void func_8002FAC8(void) {
    D_801D8264 = -1;
}

void func_8002FAD8(s8 arg0) {
    D_801D8266 = arg0;
}

void func_8002FAE4(void) {
    Fade *f;
    s32 tim;
    s16 r[4];
    u8 db;

    if (D_801D81F8.tim == 0 || *(u16 *)&D_801D81F8.mode == 0xFFFF) {
        return;
    }
    switch (D_801D81F8.unk6E) {
    case 0:
        if ((s8)D_801D81F8.unk6F < 30) {
            D_801D81F8.unk6F++;
        }
        break;
    case 1:
        if ((s8)D_801D81F8.unk6F >= -59) {
            D_801D81F8.unk6F--;
        }
        break;
    }
    f = &D_801D81F8;
    f->unk70 = (f->unk70 + (s8)f->unk6F) % 7680;
    if (f->mode != f->unk6D) {
        if (f->unk6D == -1) {
            if (f->unk72 == 0) {
                tim = func_8001BFCC(f->tim, f->mode);
                func_8001B438((u32 *)tim, f->x, f->y, f->w, f->h);
                if (f->mode != 6) {
                    f->unk7C = 0x40;
                } else {
                    D_801D81F8.unk7C = 0x80;
                }
                D_801D81F8.unk7E = 0x80;
                DrawSync(0);
                func_8001AE90((void *)tim);
            }
            D_801D81F8.unk72 += 6;
            if (D_801D81F8.unk72 > 0x80) {
                D_801D81F8.unk72 = 0x80;
                D_801D81F8.unk6D = D_801D81F8.mode;
            }
        } else {
            D_801D81F8.unk72 -= 6;
            if (D_801D81F8.unk72 < 0) {
                D_801D81F8.unk72 = 0;
                D_801D81F8.unk6D = -1;
            }
        }
    }
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].twin0);
    db = D_800794F4;
    D_801D81F8.buf[db].x0 = -((D_801D81F8.unk70 / 60) & 1);
    D_801D81F8.buf[D_800794F4].y0 = 0;
    D_801D81F8.buf[D_800794F4].u0 = (D_801D81F8.unk70 / 60) & 0xFE;
    D_801D81F8.buf[D_800794F4].v0 = D_801D81F8.unk70 / 60;
    D_801D81F8.buf[D_800794F4].r0 = D_801D81F8.unk72;
    D_801D81F8.buf[D_800794F4].g0 = D_801D81F8.unk72;
    D_801D81F8.buf[D_800794F4].b0 = D_801D81F8.unk72;
    r[0] = (D_801D81F8.x % 64) * 4;
    r[1] = D_801D81F8.y % 256;
    r[2] = D_801D81F8.unk7C;
    r[3] = D_801D81F8.unk7E;
    SetTexWindow(D_801D81F8.buf[D_800794F4].twin, r);
    addPrim(&D_800793A0->ot[0xFFF], &D_801D81F8.buf[D_800794F4]);
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].twin);
    addPrim(&D_800793A0->ot[0xFFF], D_801D81F8.buf[D_800794F4].tpage);
}

void func_80030130(void *arg0) {
    s32 t;
    s32 sum;

    t = (*(s16 *)((s8 *)arg0 + 0x122)) * (*(s32 *)((s8 *)arg0 + 0x104)) * (*(s32 *)((s8 *)arg0 + 0x104));
    sum = (*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)) + t;
    (*(s32 *)((s8 *)arg0 + 0x20)) = (*(s16 *)((s8 *)arg0 + 0xD4)) + ((sum * (*(s32 *)((s8 *)arg0 + 0x9C))) >> 12);
    (*(s32 *)((s8 *)arg0 + 0x24)) = (*(s16 *)((s8 *)arg0 + 0xD6)) + ((sum * (*(s32 *)((s8 *)arg0 + 0xA0))) >> 12);
    (*(s32 *)((s8 *)arg0 + 0x28)) = (*(s16 *)((s8 *)arg0 + 0xD8)) + ((sum * (*(s32 *)((s8 *)arg0 + 0xA4))) >> 12);
}

void func_800301D0(void *arg0) {
    func_80030130(arg0);
    *(s32 *)((s8 *)arg0 + 0x24) +=
        -*(s16 *)((s8 *)arg0 + 0x120) * *(s32 *)((s8 *)arg0 + 0x100) +
        *(s16 *)((s8 *)arg0 + 0x120) * *(s32 *)((s8 *)arg0 + 0x100) *
            *(s32 *)((s8 *)arg0 + 0x100) / 56;
}

void func_80030264(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x20)));
}

void func_800302E0(void *arg0) {
    s32 temp_a1;

    func_80030130(arg0);
    temp_a1 = (*(s16 *)((s8 *)arg0 + 0x128)) + ((*(s16 *)((s8 *)arg0 + 0x120)) * (*(s32 *)((s8 *)arg0 + 0x100)));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (((s32) ((*(s16 *)((s8 *)arg0 + 0x12A)) * rsin(temp_a1 * (*(s16 *)((s8 *)arg0 + 0x126)))) >> 0xA) + (*(s32 *)((s8 *)arg0 + 0x24)));
}

void func_8003035C(u8 *p) {
    s32 h;
    s32 sx;
    s32 cz;

    func_80030130(p);
    h = -*(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) +
        *(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) * *(s32 *)(p + 0x100) / 56;
    sx = h * rsin(*(s16 *)(p + 0xE8)) >> 12;
    cz = h * rcos(*(s16 *)(p + 0xE8)) >> 12;
    *(s32 *)(p + 0x20) -= sx;
    *(s32 *)(p + 0x24) += cz;
}

void func_80030440(u8 *p) {
    s32 d;
    s32 h;
    s32 sx;
    s32 cz;

    d = *(s16 *)(p + 0x120) * *(s32 *)(p + 0x100);
    *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4) + (d * *(s32 *)(p + 0x9C) >> 12);
    *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6) + (d * *(s32 *)(p + 0xA0) >> 12);
    *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8) + (d * *(s32 *)(p + 0xA4) >> 12);
    h = -*(s16 *)(p + 0x120) * *(s32 *)(p + 0x100) +
        *(s16 *)(p + 0x122) * *(s32 *)(p + 0x100) * *(s32 *)(p + 0x100) / 56;
    sx = h * rsin(*(s16 *)(p + 0xE8)) >> 12;
    cz = h * rcos(*(s16 *)(p + 0xE8)) >> 12;
    *(s32 *)(p + 0x20) -= sx;
    *(s32 *)(p + 0x24) += cz;
}

void func_8003058C(u8 *p) {
    s32 x;
    s32 y;
    s32 z;

    x = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    y = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    z = *(s16 *)(p + 0x120) / 2 - rand() % *(s16 *)(p + 0x120);
    *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4) + x;
    *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6) + y;
    *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8) + z;
}

s32 func_80030694(SVECTOR *a, SVECTOR *b) {
    VECTOR d;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    return SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz);
}

s32 func_80030718(SVECTOR *a, SVECTOR *b, SVECTOR *c) {
    VECTOR ab;
    VECTOR cb;
    VECTOR ca;
    s32 d0;
    s32 d1;

    ab.vx = b->vx - a->vx;
    ab.vy = b->vy - a->vy;
    ab.vz = b->vz - a->vz;
    cb.vx = b->vx - c->vx;
    cb.vy = b->vy - c->vy;
    cb.vz = b->vz - c->vz;
    ca.vx = a->vx - c->vx;
    ca.vy = a->vy - c->vy;
    ca.vz = a->vz - c->vz;
    d0 = (cb.vx * ab.vx + cb.vy * ab.vy + cb.vz * ab.vz) >> 12;
    d1 = (ca.vx * ab.vx + ca.vy * ab.vy + ca.vz * ab.vz) >> 12;
    if ((d0 <= 0 && d1 >= 0) || (d0 >= 0 && d1 <= 0)) {
        return 1;
    }
    return 0;
}

void func_80030828(SVECTOR *a, SVECTOR *p, SVECTOR *b, VECTOR *out) {
    VECTOR d;
    VECTOR ap;
    VECTOR n;
    VECTOR proj;
    s32 t;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    if (SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz) < 20000) {
        VectorNormal(&d, &n);
    } else {
        d.vx >>= 4;
        d.vy >>= 4;
        d.vz >>= 4;
        VectorNormal(&d, &n);
    }
    ap.vx = p->vx - a->vx;
    ap.vy = p->vy - a->vy;
    ap.vz = p->vz - a->vz;
    t = (n.vx * ap.vx + n.vy * ap.vy + n.vz * ap.vz) >> 12;
    proj.vx = t * n.vx >> 12;
    proj.vy = t * n.vy >> 12;
    proj.vz = t * n.vz >> 12;
    out->vx = a->vx + proj.vx;
    out->vy = a->vy + proj.vy;
    out->vz = a->vz + proj.vz;
}

s32 func_800309F0(SVECTOR *arg0, SVECTOR *arg1, s32 arg2) {
    s32 v;

    v = func_80030694(arg0, arg1);
    if (-arg2 < v && v < arg2) {
        return 1;
    }
    return 0;
}

s32 func_80030A34(SVECTOR *a, SVECTOR *b, SVECTOR *c, s16 r) {
    SVECTOR sv;
    VECTOR v;

    if (func_80030718(a, b, c) != 0) {
        func_80030828(a, c, b, &v);
        sv.vx = v.vx;
        sv.vy = v.vy;
        sv.vz = v.vz;
        if (func_800309F0(c, &sv, r) != 0) {
            return 1;
        }
        return -1;
    }
    return 0;
}

Unk13C *func_80030AE4(Unk13C *src) {
    Unk13C *dst;

    dst = func_8001AD0C(0x13C);
    *dst = *src;
    func_80030E3C(dst);
    return dst;
}

void func_80030B6C(s32 arg0) {
    PushMatrix();
    func_80030F90(arg0, 0);
    PopMatrix();
}

void func_80030BA4(void *arg0) {
    func_8001AE90(arg0);
}

s32 func_80030BC4(SVECTOR *a, SVECTOR *b, VECTOR *out) {
    VECTOR d;

    d.vx = b->vx - a->vx;
    d.vy = b->vy - a->vy;
    d.vz = b->vz - a->vz;
    if (SquareRoot0(d.vx * d.vx + d.vy * d.vy + d.vz * d.vz) < 20000) {
        VectorNormal(&d, out);
        return 1;
    }
    d.vx >>= 4;
    d.vy >>= 4;
    d.vz >>= 4;
    VectorNormal(&d, out);
    return -1;
}

void func_80030CA8(u8 *p) {
    s32 i;

    p[0x139] = *(s16 *)(p + 0x12E) >= 0x5B;
    *(s32 *)(p + 0x118) = -1;
    for (i = 0; i < 3; i++) {
        ((Unk80030CA8 *)p)->unk10C[i] = 0;
    }
    *(s32 *)(p + 0x108) = 0;
    *(s32 *)(p + 0x11C) = 0;
    *(s32 *)(p + 0x100) = 0;
    *(s32 *)(p + 0x104) = 0;
    if (*(s16 *)(p + 0x12E) != 0 && *(s16 *)(p + 0x12E) != 0x5A) {
        *(s32 *)(p + 0x38) = *(s32 *)(p + 0xAC);
        *(s32 *)(p + 0x3C) = *(s32 *)(p + 0xB0);
        *(s32 *)(p + 0x40) = *(s32 *)(p + 0xB4);
        *(s16 *)(p + 0x30) = *(s16 *)(p + 0xE4);
        *(s16 *)(p + 0x32) = *(s16 *)(p + 0xE6);
        *(s16 *)(p + 0x34) = *(s16 *)(p + 0xE8);
        *(s32 *)(p + 0x20) = *(s16 *)(p + 0xD4);
        *(s32 *)(p + 0x24) = *(s16 *)(p + 0xD6);
        *(s32 *)(p + 0x28) = *(s16 *)(p + 0xD8);
        *(s32 *)(p + 0x6C) = *(s16 *)(p + 0xDC);
        *(s32 *)(p + 0x70) = *(s16 *)(p + 0xDE);
        *(s32 *)(p + 0x74) = *(s16 *)(p + 0xE0);
        func_80030BC4((SVECTOR *)(p + 0xD4), (SVECTOR *)(p + 0xDC), (VECTOR *)(p + 0x9C));
    } else {
        *(s32 *)(p + 0xAC) = *(s32 *)(p + 0x38);
        *(s32 *)(p + 0xB0) = *(s32 *)(p + 0x3C);
        *(s32 *)(p + 0xB4) = *(s32 *)(p + 0x40);
        *(s16 *)(p + 0xE4) = *(s16 *)(p + 0x30);
        *(s16 *)(p + 0xE6) = *(s16 *)(p + 0x32);
        *(s16 *)(p + 0xE8) = *(s16 *)(p + 0x34);
        *(s16 *)(p + 0xD4) = *(s32 *)(p + 0x20);
        *(s16 *)(p + 0xD6) = *(s32 *)(p + 0x24);
        *(s16 *)(p + 0xD8) = *(s32 *)(p + 0x28);
    }
}

void *func_80030E3C(void *arg0) {
    func_8001EFDC(arg0, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xD4)), (s32) (*(s16 *)((s8 *)arg0 + 0xD6)), (s32) (*(s16 *)((s8 *)arg0 + 0xD8)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE4)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE6)), (s16) (s32) (*(s16 *)((s8 *)arg0 + 0xE8)));
    func_8001EFDC(arg0 + 0x4C, (*(s32 *)((s8 *)arg0 + 0x98)), (s32) (*(s16 *)((s8 *)arg0 + 0xDC)), (s32) (*(s16 *)((s8 *)arg0 + 0xDE)), (s32) (*(s16 *)((s8 *)arg0 + 0xE0)), 0, 0, 0);
    (*(s32 *)((s8 *)arg0 + 0x14)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x18)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x1C)) = 0;
    (*(s32 *)((s8 *)arg0 + 0x38)) = (s32) (*(s32 *)((s8 *)arg0 + 0xAC));
    (*(s32 *)((s8 *)arg0 + 0x3C)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB0));
    (*(s32 *)((s8 *)arg0 + 0x40)) = (s32) (*(s32 *)((s8 *)arg0 + 0xB4));
    (*(u16 *)((s8 *)arg0 + 0x30)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE4));
    (*(u16 *)((s8 *)arg0 + 0x32)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE6));
    (*(u16 *)((s8 *)arg0 + 0x34)) = (u16) (*(s16 *)((s8 *)arg0 + 0xE8));
    (*(s32 *)((s8 *)arg0 + 0x20)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD4));
    (*(s32 *)((s8 *)arg0 + 0x24)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD6));
    (*(s32 *)((s8 *)arg0 + 0x28)) = (s32) (*(s16 *)((s8 *)arg0 + 0xD8));
    func_80030CA8(arg0);
    (*(s32 *)((s8 *)arg0 + 0x6C)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDC));
    (*(s32 *)((s8 *)arg0 + 0x70)) = (s32) (*(s16 *)((s8 *)arg0 + 0xDE));
    (*(s32 *)((s8 *)arg0 + 0x74)) = (s32) (*(s16 *)((s8 *)arg0 + 0xE0));
    func_80030BC4(arg0 + 0xD4, arg0 + 0xDC, arg0 + 0x9C);
    (*(s32 *)((s8 *)arg0 + 0xFC)) = 0;
    (*(s16 *)((s8 *)arg0 + 0x132)) = 0;
    (*(s8 *)((s8 *)arg0 + 0x138)) = 0;
    return arg0;
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80010864);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80010874);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80010884);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80010894);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800108A4);

s32 func_80030F90(s32 arg, s32 flag) {
    Anim *o = (Anim *)arg;
    u8 f = flag;
    SVECTOR v0;
    SVECTOR v1;
    SVECTOR v2;
    s32 r;

    if (o->mode != 10 && o->mode < 90) {
        if (o->sx != o->sxT) {
            o->sx = o->dsx * o->t + o->sx0;
            if (o->dsx >= 0) {
                if (o->sx >= o->sxT) {
                    o->sx = o->sxT;
                    o->doneX = 1;
                }
            } else if (o->sx <= o->sxT) {
                o->sx = o->sxT;
                o->doneX = 1;
            }
        } else {
            o->doneX = 1;
        }
        if (o->sy != o->syT) {
            o->sy = o->dsy * o->t + o->sy0;
            if (o->dsy >= 0) {
                if (o->sy >= o->syT) {
                    o->sy = o->syT;
                    o->doneY = 1;
                }
            } else if (o->sy <= o->syT) {
                o->sy = o->syT;
                o->doneY = 1;
            }
        } else {
            o->doneY = 1;
        }
        if (o->sz != o->szT) {
            o->sz = o->dsz * o->t + o->sz0;
            if (o->dsz >= 0) {
                if (o->sz >= o->szT) {
                    o->sz = o->szT;
                    o->doneZ = 1;
                }
            } else if (o->sz <= o->szT) {
                o->sz = o->szT;
                o->doneZ = 1;
            }
        } else {
            o->doneZ = 1;
        }
        o->rotX = o->rx0 + o->drx * o->t + o->ddrx * o->t2 * o->t2 / 64;
        o->rotY = o->ry0 + o->dry * o->t + o->ddry * o->t2 * o->t2 / 64;
        o->rotZ = o->rz0 + o->drz * o->t + o->ddrz * o->t2 * o->t2 / 64;
    }
    func_8001F01C(o, &v0);
    switch (o->mode) {
    case 6:
        o->state = -1;
    case 0:
    case 47:
    case 48:
    case 49:
    case 56:
    case 69:
    case 82:
    case 90:
        o->posX = o->px;
        o->posY = o->py;
        o->posZ = o->pz;
        break;
    case 1:
    case 11:
    case 12:
    case 13:
    case 29:
    case 30:
    case 31:
    case 50:
    case 57:
    case 63:
    case 70:
    case 76:
    case 83:
        func_80030130(o);
        break;
    case 2:
    case 14:
    case 15:
    case 16:
    case 32:
    case 33:
    case 34:
    case 51:
    case 58:
    case 64:
    case 71:
    case 77:
    case 84:
        func_800301D0(o);
        break;
    case 3:
    case 17:
    case 18:
    case 19:
    case 35:
    case 36:
    case 37:
    case 52:
    case 59:
    case 65:
    case 72:
    case 78:
    case 85:
        func_80030264(o);
        break;
    case 4:
    case 20:
    case 21:
    case 22:
    case 38:
    case 39:
    case 40:
    case 53:
    case 60:
    case 66:
    case 73:
    case 79:
    case 86:
        func_800302E0(o);
        break;
    case 5:
    case 23:
    case 24:
    case 25:
    case 41:
    case 42:
    case 43:
    case 54:
    case 61:
    case 67:
    case 74:
    case 80:
    case 87:
        func_8003035C((u8 *)o);
        break;
    case 7:
    case 26:
    case 27:
    case 28:
    case 44:
    case 45:
    case 46:
    case 55:
    case 62:
    case 68:
    case 75:
    case 81:
    case 88:
        func_80030440((u8 *)o);
        break;
    case 8:
    case 89:
        func_8003058C((u8 *)o);
        break;
    }
    if (o->state != -1 && o->mode != 0 && o->mode < 90) {
        func_8001EEA0(o, f);
        func_8001EEA0((u8 *)o + 0x4C, 0);
        func_8001F01C(o, &v1);
        func_8001F01C((u8 *)o + 0x4C, &v2);
        r = func_80030A34(&v0, &v1, &v2, o->unk12C);
        if (r == 1) {
            switch (o->mode) {
            case 11:
            case 14:
            case 17:
            case 20:
            case 23:
            case 26:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                break;
            case 12:
            case 15:
            case 18:
            case 21:
            case 24:
            case 27:
                o->mode = 0;
                o->posX = o->px2;
                o->posY = o->py2;
                o->posZ = o->pz2;
                o->px = o->px2;
                o->py = o->py2;
                o->pz = o->pz2;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
                break;
            case 13:
            case 16:
            case 19:
            case 22:
            case 25:
            case 28:
                func_80030CA8((u8 *)o);
                break;
            case 50:
            case 51:
            case 52:
            case 53:
            case 54:
            case 55:
                o->unk139 = 1;
                break;
            case 76:
            case 77:
            case 78:
            case 79:
            case 80:
            case 81:
                o->mode = 0;
                o->posX = o->px2;
                o->posY = o->py2;
                o->posZ = o->pz2;
                o->px = o->px2;
                o->py = o->py2;
                o->pz = o->pz2;
            case 63:
            case 64:
            case 65:
            case 66:
            case 67:
            case 68:
                o->unk137 = 2;
                o->speed = -abs(o->speed);
                break;
            }
            o->state = 1;
        } else if (r == -1) {
            o->state = 2;
        }
    }
    if (o->mode < 90 && o->mode != 0 && o->state == -1) {
        o->state = 0;
    }
    if (o->flag != 0) {
        switch (o->mode) {
    case 31:
    case 34:
    case 37:
    case 40:
    case 43:
    case 46:
    case 49:
            func_80030CA8((u8 *)o);
            break;
        }
    }
    if (o->mode < 90) {
        o->t++;
        o->t2++;
        if (o->period != 0 && o->flag == 0 && o->period < o->cnt++) {
            o->cnt = o->period;
            switch (o->mode) {
            case 29:
            case 32:
            case 35:
            case 38:
            case 41:
            case 44:
            case 47:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                break;
            case 30:
            case 33:
            case 36:
            case 39:
            case 42:
            case 45:
            case 48:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
                break;
            case 31:
            case 34:
            case 37:
            case 40:
            case 43:
            case 46:
            case 49:
                func_80030CA8((u8 *)o);
                break;
            case 56:
            case 57:
            case 58:
            case 59:
            case 60:
            case 61:
            case 62:
                o->unk139 = 1;
                break;
            case 82:
            case 83:
            case 84:
            case 85:
            case 86:
            case 87:
            case 88:
            case 89:
                o->mode = 0;
                func_80030CA8((u8 *)o);
                o->sxT = o->sx;
                o->syT = o->sy;
                o->szT = o->sz;
                o->swT = o->sw;
                o->drx = 0;
                o->dry = 0;
                o->drz = 0;
                o->ddrx = 0;
                o->ddry = 0;
                o->ddrz = 0;
            case 69:
            case 70:
            case 71:
            case 72:
            case 73:
            case 74:
            case 75:
                o->unk137 = 2;
                o->speed = -abs(o->speed);
                break;
            }
            o->flag = 1;
        }
    }
    func_8001EEA0(o, f);
}

void func_80031754(void *arg0) {
    if ((*(s16 *)((s8 *)arg0 + 0x12E)) >= 0x5B) {
        if ((*(s16 *)((s8 *)arg0 + 0x128)) > (*(s32 *)((s8 *)arg0 + 0x100))) {
            (*(s32 *)((s8 *)arg0 + 0x100)) += 1;
            return;
        }
        (*(s32 *)((s8 *)arg0 + 0x100)) = 0;
        (*(s8 *)((s8 *)arg0 + 0x139)) = 0;
        (*(s16 *)((s8 *)arg0 + 0x12E)) = (s16) ((u16) (*(s16 *)((s8 *)arg0 + 0x12E)) - 0x64);
    }
}

s16 func_800317A8(void *arg0, s16 arg1) {
    u8 *o;

    o = arg0;
    switch (o[0x137]) {
    case 1:
        arg1 = (*(s32 *)(o + 0x38)) / 16;
        if ((*(s32 *)(o + 0x38)) > 0x1000) {
            arg1 = 0x100 - ((*(s32 *)(o + 0x38)) - 0x1000) / 16;
        }
        break;
    case 2:
        arg1 += (*(u16 *)(o + 0x130));
        break;
    case 3:
        if (o[0x138] == 2) {
            break;
        }
        if (o[0x138] == 0) {
            arg1 += (*(u16 *)(o + 0x130));
            if (arg1 > 0x100) {
                arg1 = 0x100;
                o[0x138] = 1;
            }
        } else {
            arg1 -= (*(u16 *)(o + 0x130));
            if (arg1 < 0) {
                arg1 = 0;
                o[0x138] = 2;
            }
        }
        break;
    case 4:
        if (o[0x138] == 0) {
            arg1 += (*(u16 *)(o + 0x130));
            if (arg1 > 0x100) {
                arg1 = 0x100;
                o[0x138] = 1;
            }
        } else {
            arg1 -= (*(u16 *)(o + 0x130));
            if (arg1 < 0) {
                arg1 = 0;
                o[0x138] = 0;
            }
        }
        break;
    case 5:
        o[0x138] += (*(u16 *)(o + 0x130));
        if ((s8)o[0x138] >= 0) {
            arg1 = o[0x138] + 0x80;
        } else {
            arg1 = 0xFF - (o[0x138] & 0x7F);
        }
        break;
    }
    if (arg1 < 0) {
        arg1 = 0;
    }
    if (arg1 > 0x100) {
        arg1 = 0x100;
    }
    if ((*(s16 *)(o + 0x12E)) == 0xA) {
        arg1 = (*(s16 *)(o + 0x132));
    } else {
        (*(s16 *)(o + 0x132)) = arg1;
    }
    return arg1;
}

void func_80031970(Obj32 *o) {
    SVECTOR *v;
    s32 i;
    s16 x;
    s16 y;

    v = o->unk16C;
    for (i = 0; i < o->n; i++) {
        x = rsin((i << 12) / o->n) * o->unk19C[0] / 4096;
        y = rcos((i << 12) / o->n) * o->unk19C[0] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3];
        v++;
        x = rsin(((i + 1) << 12) / o->n) * o->unk19C[0] / 4096;
        y = rcos(((i + 1) << 12) / o->n) * o->unk19C[0] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3];
        v++;
        x = rsin((i << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        y = rcos((i << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3] + (o->unk19C[4] - o->unk19C[3]) * o->unk19C[2] / 100;
        v++;
        x = rsin(((i + 1) << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        y = rcos(((i + 1) << 12) / o->n) * (o->unk19C[0] + (o->unk19C[1] - o->unk19C[0]) * o->unk19C[2] / 100) / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[3] + (o->unk19C[4] - o->unk19C[3]) * o->unk19C[2] / 100;
        v++;
        x = rsin((i << 12) / o->n) * o->unk19C[1] / 4096;
        y = rcos((i << 12) / o->n) * o->unk19C[1] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[4];
        v++;
        x = rsin(((i + 1) << 12) / o->n) * o->unk19C[1] / 4096;
        y = rcos(((i + 1) << 12) / o->n) * o->unk19C[1] / 4096;
        v->vx = x;
        v->vy = y;
        v->vz = o->unk19C[4];
        v++;
    }
}

Obj32 *func_80031F58(s16 id, Bytes4 *a, Bytes4 *b, Bytes4 *c, Unk13C *src, s32 n, u8 abr, u8 tp, s32 type,
                     s16 p0, s16 p1, s16 p2, s16 p3, s16 p4, Bytes8 *q, s32 r, s32 s, s32 t, u8 u1, u8 u2,
                     s32 w, s32 x) {
    Obj32 *o;
    u8 *prim;
    s32 i;
    s32 j;

    o = func_8001AD0C(0x1B0);
    o->type = type;
    o->n = n;
    o->unk19C[0] = p0;
    o->unk19C[1] = p1;
    o->unk19C[2] = p2;
    o->unk19C[3] = p3;
    o->unk19C[4] = p4;
    o->unk16C = func_8001AD0C(n * 48);
    func_80031970(o);
    if (type == 13) {
        o->unk170 = *q;
        o->unk178 = r;
        o->unk17C = s;
        if (t >= 0 && func_801E6C78(t, 1, o, o->unk13C, x) != 0) {
            o->unk1AD = 1;
        } else {
            o->unk1AD = -1;
        }
    } else {
        o->unk1AD = -1;
    }
    o->unk198 = tp;
    o->unk1A6 = id;
    o->unk1A8 = -1;
    o->unk185 = *a;
    o->unk189 = *b;
    o->unk18D = *c;
    *(Unk13C *)o = *src;
    func_80030E3C(o);
    o->unk1AB = u2;
    o->unk194 = w;
    o->unk1AA = u1;
    o->unk1AC = abr;
    for (i = 0; i < 2; i++) {
        if (o->type < 10) {
            o->unk15C[i] = func_8001AD0C(o->n * 16);
        } else {
            o->unk15C[i] = 0;
        }
        prim = o->unk164[i] = func_8001AD0C(D_8006DEF4[o->type] * o->n * 2);
        for (j = 0; j < o->n * 2; j++) {
            func_8001E6EC(o->type, prim, abr, 0);
            if (o->type < 10) {
                SetDrawTPage(o->unk15C[i] + j * 8, 0, 0, GetTPage(0, tp, 0, 0));
            }
            prim += D_8006DEF4[o->type];
        }
    }
    return o;
}

void func_8003230C(Obj32 *o) {
    u8 c0[8];
    u8 c1[8];
    u8 c2[8];
    SVECTOR *v;
    s32 i;

    if (o->unk0[0x139] != 0) {
        func_80031754(o);
        return;
    }
    PushMatrix();
    func_80030F90((s32)o, o->unk1AA);
    o->unk1A6 = func_800317A8(o, o->unk1A6);
    if (o->unk1A6 == 0) {
        PopMatrix();
        return;
    }
    v = o->unk16C;
    if (o->unk1A6 != o->unk1A8) {
        c0[0] = o->unk185.b[0] * o->unk1A6 / 256;
        c0[1] = o->unk185.b[1] * o->unk1A6 / 256;
        c0[2] = o->unk185.b[2] * o->unk1A6 / 256;
        c1[0] = o->unk189.b[0] * o->unk1A6 / 256;
        c1[1] = o->unk189.b[1] * o->unk1A6 / 256;
        c1[2] = o->unk189.b[2] * o->unk1A6 / 256;
        c2[0] = o->unk18D.b[0] * o->unk1A6 / 256;
        c2[1] = o->unk18D.b[1] * o->unk1A6 / 256;
        c2[2] = o->unk18D.b[2] * o->unk1A6 / 256;
    }
    switch (o->type) {
    case 9: {
        u8 *p;
        u8 *q;
        u8 *tp;

        p = o->unk164[D_800794F4];
        q = o->unk164[D_800794F4 ^ 1];
        tp = o->unk15C[D_800794F4];
        for (i = 0; i < o->n; i++) {
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c0[0], c0[1], c0[2]);
                func_8001E76C(p, c0[0], c0[1], c0[2]);
                func_8001E7B8(p, c1[0], c1[1], c1[2]);
                func_8001E804(p, c1[0], c1[1], c1[2]);
                func_8001E75C(q, c0[0], c0[1], c0[2]);
                func_8001E76C(q, c0[0], c0[1], c0[2]);
                func_8001E7B8(q, c1[0], c1[1], c1[2]);
                func_8001E804(q, c1[0], c1[1], c1[2]);
            }
            func_8001DFE0((s32)p, (s32)tp, (s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], o->unk1AC, o->unk1AB, o->unk194);
            p += 0x24;
            q += 0x24;
            tp += 8;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c1[0], c1[1], c1[2]);
                func_8001E76C(p, c1[0], c1[1], c1[2]);
                func_8001E7B8(p, c2[0], c2[1], c2[2]);
                func_8001E804(p, c2[0], c2[1], c2[2]);
                func_8001E75C(q, c1[0], c1[1], c1[2]);
                func_8001E76C(q, c1[0], c1[1], c1[2]);
                func_8001E7B8(q, c2[0], c2[1], c2[2]);
                func_8001E804(q, c2[0], c2[1], c2[2]);
            }
            func_8001DFE0((s32)p, (s32)tp, (s32)&v[2], (s32)&v[3], (s32)&v[4], (s32)&v[5], o->unk1AC, o->unk1AB, o->unk194);
            p += 0x24;
            q += 0x24;
            tp += 8;
            v += 6;
        }
        break;
    }
    case 13: {
        u8 *p;
        u8 *q;

        if (o->unk1AD >= 0) {
            func_801E7020(o->unk13C);
        }
        p = o->unk164[D_800794F4];
        q = o->unk164[D_800794F4 ^ 1];
        for (i = 0; i < o->n; i++) {
            func_8001EC3C(p, o->unk170.b[0], o->unk170.b[2], o->unk170.b[4], o->unk170.b[6]);
            *(u16 *)(p + 0x1A) = o->unk178;
            *(u16 *)(p + 0xE) = o->unk17C;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c0[0], c0[1], c0[2]);
                func_8001E76C(p, c0[0], c0[1], c0[2]);
                func_8001E7B8(p, c1[0], c1[1], c1[2]);
                func_8001E804(p, c1[0], c1[1], c1[2]);
                func_8001E75C(q, c0[0], c0[1], c0[2]);
                func_8001E76C(q, c0[0], c0[1], c0[2]);
                func_8001E7B8(q, c1[0], c1[1], c1[2]);
                func_8001E804(q, c1[0], c1[1], c1[2]);
            }
            func_8001D900((s32)p, (s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], o->unk1AB, o->unk194);
            p += 0x34;
            q += 0x34;
            func_8001EC3C(p, o->unk170.b[0], o->unk170.b[2], o->unk170.b[4], o->unk170.b[6]);
            *(u16 *)(p + 0x1A) = o->unk178;
            *(u16 *)(p + 0xE) = o->unk17C;
            if (o->unk1A6 != o->unk1A8) {
                func_8001E75C(p, c1[0], c1[1], c1[2]);
                func_8001E76C(p, c1[0], c1[1], c1[2]);
                func_8001E7B8(p, c2[0], c2[1], c2[2]);
                func_8001E804(p, c2[0], c2[1], c2[2]);
                func_8001E75C(q, c1[0], c1[1], c1[2]);
                func_8001E76C(q, c1[0], c1[1], c1[2]);
                func_8001E7B8(q, c2[0], c2[1], c2[2]);
                func_8001E804(q, c2[0], c2[1], c2[2]);
            }
            func_8001D900((s32)p, (s32)&v[2], (s32)&v[3], (s32)&v[4], (s32)&v[5], o->unk1AB, o->unk194);
            p += 0x34;
            q += 0x34;
            v += 6;
        }
        break;
    }
    }
    PopMatrix();
    o->unk1A8 = o->unk1A6;
}

void func_80032AA0(Obj32 *p) {
    s32 i;

    for (i = 0; i < 2; i++) {
        func_8001AE90(p->unk15C[i]);
        func_8001AE90(p->unk164[i]);
    }
    if (p->unk1AD >= 0) {
        func_801E72D4(p->unk13C);
    }
    func_8001AE90(p->unk16C);
    func_8001AE90(p);
}

Particles *func_80032B44(u8 *c0, u8 *c1, Unk13C *src, s16 sx, s16 sy, s16 a5, s16 a6, s16 frames, s16 a8, s16 a9,
                         s16 count, s16 a11, s16 a12, s16 a13, s16 kind, s16 semi, s32 flags, s32 a17) {
    Particles *o;
    Particle *p;
    LINE_G2 *l;
    s32 i;
    s32 k;
    s32 angle;

    o = func_8001AD0C(0x15C);
    o->p = p = func_8001AD0C(count * 0x88);
    k = 0;
    if (src == 0) {
        o->parent = (u8 *)D_801D6A4C + 0x78;
        o->own = 0;
    } else {
        o->base = *src;
        func_80030E3C(o);
        o->parent = o;
        o->own = 1;
    }
    o->unk14E = a11;
    o->unk154 = a17;
    o->unk15A = flags & 1;
    o->count = count;
    o->unk150 = 0;
    o->frames = frames;
    o->rgb[0] = c0[0];
    o->rgb[1] = c0[1];
    o->rgb[2] = c0[2];
    o->drgb[0] = (c1[0] - o->rgb[0]) / o->frames;
    o->drgb[1] = (c1[1] - o->rgb[1]) / o->frames;
    o->drgb[2] = (c1[2] - o->rgb[2]) / o->frames;
    o->unk158 = a9 == 0 ? 1 : -1;
    o->kind = kind;
    for (i = 0; i < o->count; i++, p++) {
        if (o->kind == 0) {
            l = &p->line[0];
            func_800678E4(l);
            setSemiTrans(l, semi);
            l = &p->line[1];
            func_800678E4(l);
            setSemiTrans(l, semi);
        } else {
            l = &p->line[0];
            func_80067904(l);
            setSemiTrans(l, semi);
            l->r0 = c0[0];
            l->g0 = c0[1];
            l->b0 = c0[2];
            l->r1 = c1[0];
            l->g1 = c1[1];
            l->b1 = c1[2];
            l++;
            func_80067904(l);
            setSemiTrans(l, semi);
            l->r0 = c0[0];
            l->g0 = c0[1];
            l->b0 = c0[2];
            l->r1 = c1[0];
            l->g1 = c1[1];
            l->b1 = c1[2];
        }
        func_8001EFDC(p, (s32)o->parent, 0, 0, 0, 0, 0, 0);
        p->unk7C = a5 * 8;
        p->unk7E = rand() % a8 + 1;
        if (sy == 0) {
            sy = 1;
        }
        if (sx == 0) {
            sx = 1;
        }
        if (a13 < 3) {
            p->unk32 = rand() % sx - sx / 2;
            p->unk30 = rand() % sy - sy / 2;
            p->unk34 = 0;
            p->unk7A = 0;
        } else {
            p->unk32 = 0;
            p->unk30 = 0;
            p->unk34 = 0;
            p->unk7A = sx - 0xB4;
        }
        p->unk80 = p->unk7E * frames;
        p->unk78 = 0;
        p->unk76 = 0;
        p->unk74 = 0;
        if (a12 != 0) {
            switch ((s16)(a13 % 3)) {
            case 0:
                angle = i << 12;
                k = angle / o->count;
                p->unk76 = a12;
                break;
            case 1:
                k = rand() % 4096;
                p->unk76 = a12;
                break;
            case 2:
                k = rand() % 4096;
                p->unk76 = rand() % a12;
                break;
            }
            p->unk34 = k;
        }
        p->unk82 = rand() & 0xFFF;
        p->unk84 = rand() & 0x1FF;
        if (i & 1) {
            p->unk84 = -p->unk84;
        }
    }
    o->unk147 = flags & 2;
    if (a6 == 0) {
        o->unk156 = 0;
    } else {
        o->unk156 = (a6 - a5) * 8 / o->frames;
    }
    return o;
}

void func_80033258(Particles *o) {
    Particle *p;
    LINE_G2 *l;
    SVECTOR *v;
    s32 limit;
    s32 i;
    s32 t;
    s32 d;
    u32 z;
    s16 dx;
    s16 dz;
    s32 a;
    u8 r;
    u8 g;
    u8 bl;
    s32 b;

    p = o->p;
    limit = 10000;
    if (o->own != 0) {
        if (((u8 *)o)[0x139] != 0) {
            func_80031754(o);
            return;
        }
        PushMatrix();
        func_80030F90((s32)o, o->unk15A);
        PopMatrix();
        limit = (*(s16 *)((u8 *)o + 0x124) + o->frames - 1) / o->frames * o->frames;
    }
    PushMatrix();
    if (o->kind == 0) {
        if (o->unk147 == 0) {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 8;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vz += d;
                    z = RotTransPers((s32)v, (s32)&l->r1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        r = o->rgb[0] + o->drgb[0] * t;
                        g = o->rgb[1] + o->drgb[1] * t;
                        bl = o->rgb[2] + o->drgb[2] * t;
                        l->r0 = r;
                        l->g0 = g;
                        l->b0 = bl;
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p++;
        }
        } else {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 16;
                v->vx += dx = d * rsin(p->unk82) / 4096;
                v->vz += dz = d * rcos(p->unk82) / 4096;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vx -= dx;
                    v->vz -= dz;
                    z = RotTransPers((s32)v, (s32)&l->r1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        r = o->rgb[0] + o->drgb[0] * t;
                        g = o->rgb[1] + o->drgb[1] * t;
                        bl = o->rgb[2] + o->drgb[2] * t;
                        l->r0 = r;
                        l->g0 = g;
                        l->b0 = bl;
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p->unk82 += p->unk84;
            p++;
        }
        }
    } else {
        if (o->unk147 == 0) {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 8;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vz += d;
                    z = RotTransPers((s32)v, (s32)&l->x1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p++;
        }
        } else {
        for (i = 0; i < o->count; i++) {
            t = o->unk150 + i;
            if (t < limit) {
                l = &p->line[D_800794F4];
                v = (SVECTOR *)&p->unk74;
                t %= o->frames;
                func_8001EEA0(p, 0);
                if (o->unk158 < 0) {
                    v->vz = p->unk80 - p->unk7E * t;
                } else {
                    v->vz = p->unk7E * t;
                }
                v->vz += o->unk14E;
                d = (p->unk7C + o->unk156 * t) * o->unk158 / 16;
                v->vx += dx = d * rsin(p->unk82) / 4096;
                v->vz += dz = d * rcos(p->unk82) / 4096;
                if (RotTransPers((s32)v, (s32)&l->x0, &a, &b) < 0x1000U) {
                    v->vx -= dx;
                    v->vz -= dz;
                    z = RotTransPers((s32)v, (s32)&l->x1, &a, &b);
                    if (z < 0x1000U) {
                        if (o->unk154 != 0) {
                            z = o->unk154;
                        }
                        addPrim(&D_800793A0->ot[z], l);
                    }
                }
            }
            p->unk82 += p->unk84;
            p++;
        }
        }
    }
    o->unk150++;
    PopMatrix();
}

void func_80033CD4(void *arg0) {
    func_8001AE90((*(void **)((s8 *)arg0 + 0x140)));
    func_8001AE90(arg0);
}

void func_80033D08(s32 n) {
    while (n > 0) {
        func_80014C08(D_800794F0);
        if (((s8 *)D_801D8340)[0x823] == 0) {
            n--;
        }
        if (((s8 *)D_801D8340)[0x815] != 0) {
            ((s8 *)D_801D8340)[0x815] = 0;
            func_80014A90();
            return;
        }
    }
}

s32 func_80033D9C(void) {
    void *var_v0_2;

    if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
        (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
        func_80014A90();
        return -1;
    }
    if ((((u32) (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178)) >> 0x11) & 3) == 1) {
        var_v0_2 = *D_80089840;
    } else {
        var_v0_2 = D_80089840[(*(s8 *)((s8 *)D_801D8340 + 0x817))];
    }
    if (!((*(u16 *)((s8 *)var_v0_2 + 0xA)) & 0x40)) {
        return 0;
    }
    func_8002B498(0xA0);
    return 1;
}

void func_80033E7C(void) {
    s32 n;

    (*(s32 *)((s8 *)D_801D8340 + 0x7FC)) = 0;
    while (1) {
        if ((*(s8 *)((s8 *)D_801D8340 + 0x815)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x815)) = 0;
            func_80014A90();
            return;
        }
        func_80014C08(D_800794F0);
        if ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) != 0) {
            (*(s8 *)((s8 *)D_801D8340 + 0x816)) = 0;
            return;
        }
        if ((*(s8 *)((s8 *)D_801D8340 + 0x816)) == 0) {
            return;
        }
        n = (*(s32 *)((s8 *)D_801D8340 + 0x7FC))++;
        if (n >= 0xF1) {
            return;
        }
    }
}

void func_80033F34(void) {
    s32 i;
    s32 j;
    s32 x;
    s32 y;

    if (D_801D8330 != 0) {
        D_801D8330--;
    }
    for (i = 0; i < 2; i++) {
        x = 0x80;
        y = i * -125 + 0x99;
        for (j = 0; j < 3; j++) {
            if (func_80029990() != 0) {
                return;
            }
            if (D_801D8330 == 0 && ((*(u32 *)(D_801D8348[i] + 0x178) >> 2) & 3) != j) {
                continue;
            }
            CUR_SPRT->sp.x0 = x + (x - D_8006E280[j]) * D_801D8330 / 32;
            CUR_SPRT->sp.y0 = y + (y - D_8006E288[i][j]) * D_801D8330 / 32;
            CUR_SPRT->sp.u0 = j * 64 + 64;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7FF0;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = x;
            CUR_SPRT->sp.g0 = x;
            CUR_SPRT->sp.b0 = x;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1D);
            addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        }
    }
}

void func_800341EC(void) {
    s32 var_a1;
    u32 temp_v0;

    temp_v0 = (*(u32 *)((s8 *)(D_801D8348[(*(s8 *)((s8 *)D_801D8340 + 0x817))]) + 0x178));
    var_a1 = (temp_v0 >> 0x11) & 1;
    if (((temp_v0 >> 0x11) & 3) == 1) {
        var_a1 = 0;
    }
    func_8001A164(&D_801D8278, var_a1);
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80010C9C);

void func_80034260(void) {
    char buf[0x88];
    s32 mode;
    s32 i;
    s32 j;
    s32 id;
    s8 c;
    s8 *card;
    s32 over;
    Player *a;
    Player *b;

    while (DUEL->state < 0) {
        func_80014C08(D_800794F0);
    }
    DUEL->unk815 = 0;
    for (;;) {
        func_80033D08(1);
        func_801EAB4C();
        switch (DUEL->unk818) {
        case 0:
            D_801D83EC[0x9D] = 1;
            D_801D83EC[0x175] = 1;
            D_801D83D0.next = -1;
            D_801D83D0.next2 = -1;
            func_80014C08(0x1E);
            if (PLAYER(1)->unk178_17 == 1 && ((u8 *)D_8006E054)[4] == 0x8C) {
                for (i = 0, j = 0; i < 30; i++) {
                    if ((u32)func_80047B84(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                    if ((u32)func_80048150(0, PLAYER(0)->cards[i].id) < 3) {
                        j = 1;
                    }
                }
                if (j) {
                    for (i = 0; i < 3; i++) {
                        id = func_800402CC(0);
                        if (id == -1) {
                            break;
                        }
                        for (j = 0; j < 29; j++) {
                            PLAYER(0)->unk17D[j] = PLAYER(0)->unk17D[j + 1];
                        }
                        PLAYER(0)->unk17D[29] = id;
                    }
                    func_800149B8(0, -1, 0, 0x800, func_80049EF8, 2, func_800148B0(), 0, 0);
                    func_80014C08(0x7FFFFFFF);
                }
            }
            DUEL->unk818++;
            break;
        case 1:
            DUEL->unk822 = 0;
            if (ME == 0) {
                D_801D83EC[0xC1] = 1;
                D_801D83EC[0x199] = 6;
            } else {
                D_801D83EC[0xC1] = 6;
                D_801D83EC[0x199] = 1;
            }
            D_801D83EC[0x9D] = 1;
            D_801D83EC[0x175] = 1;
            PLAYER(0)->unk178_2 = 3;
            PLAYER(1)->unk178_2 = 3;
            DUEL->unk80A = -1;
            DUEL->unk80E = -1;
            DUEL->unk81D = -1;
            DUEL->unk818++;
            break;
        case 2:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 0;
            D_801D83D0.unk1 = PLAYER(ME)->unk178_17;
            D_801D83D0.next = 0;
            D_801D83D0.next2 = 0;
            func_80033D08(0x3C);
            while (1) {
            wait:
                if (D_801D83EC[0x9D] != 4) {
                    goto wait;
                }
                if (func_801EC570(ME) == -1) {
                    break;
                }
                func_80014C08(0x14);
                func_801FA780(ME);
            }
            DUEL->unk818++;
            break;
        case 3:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            if (func_80040764(ME) >= 0) {
                DUEL->unk818 = 4;
            } else if (func_80040570(ME) != 0) {
                if (func_80040220(ME) == 0) {
                    D_801D83D4 = 2;
                    sprintf(buf, "There are no more Cards, so %s loses!", PLAYER(ME)->unk1CE);
                    func_80019EA4((u8 *)&D_801D8278, buf, 0);
                    func_800341EC();
                    DUEL->unk81E = ME ^ 1;
                    DUEL->unk818 = 0x26;
                } else {
                    D_801D83D4 = 1;
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_80019EA4((u8 *)&D_801D8278, "Redrawing Cards because there are\nno Digimon Cards.", 0);
                        func_800341EC();
                    }
                    DUEL->unk818 = 6;
                }
            } else {
                DUEL->unk818 = 4;
            }
            break;
        case 4:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0;
            D_801D83D0.next2 = 0;
            if (func_80040220(ME) == 0) {
                DUEL->unk818 = 8;
            } else if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 1;
                func_80033E7C();
                if (DUEL->unk804 != 0) {
                    func_80033D08(0x1E);
                    DUEL->unk818 = 6;
                } else {
                    DUEL->unk818 = 8;
                }
            } else {
                DUEL->unk818++;
            }
            break;
        case 5:
            DUEL->unk822 = 1;
            D_801D83D0.next2 = 1;
            if (D_80089840[ME]->unkA & 0x10) {
                func_8002B498(0xA0);
                D_801D83D0.next = 3;
                D_801D83D0.next2 = 0;
                do {
                    func_80019EA4((u8 *)&D_801D8278, "This will discard all Cards.\nIs this OK?", 1);
                    func_800341EC();
                    switch (CHOICE) {
                    case 0:
                    case 2:
                        if (DUEL->unk81F != 0) {
                            func_801EA8B4(0x78, "Please press \"Yes\"!");
                        } else {
                            DUEL->unk818 = 4;
                        }
                        break;
                    case 1:
                        PLAYER(ME)->unk110 |= 0x20;
                        DUEL->unk818++;
                        break;
                    }
                } while (DUEL->unk818 == 5);
            } else if (D_80089840[ME]->unkA & 0x40) {
                func_8002B498(0xA0);
                DUEL->unk818 = 8;
            } else if (D_80089840[ME]->unkA & 0x80) {
                func_8002B498(0xA0);
                DUEL->unk818 = 7;
                DUEL->unk819 = 4;
                DUEL->unk81A = ME;
            }
            break;
        case 6:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0x10;
            D_801D83D0.next2 = 0;
            func_801ECC58(ME);
            DUEL->unk818 = 3;
            break;
        case 7:
            DUEL->unk822 = 1;
            D_801D83D7 = 8;
            if (DUEL->unk819 != 0x19) {
                func_801EC4CC(DUEL->unk81A);
            }
            func_801EBACC(DUEL->unk81A, 0);
            func_80033D08(0x14);
            for (;;) {
                func_80033D08(1);
                func_801EBACC(DUEL->unk81A, 0);
                if (D_80089840[DUEL->unk81A]->unkA & 0x10) {
                    func_8002B498(0xA1);
                    if (DUEL->unk819 == 0x19) {
                        D_801D83EC[0x31] = 1;
                        D_801D83EC[0x109] = 1;
                    }
                    func_801EC528(DUEL->unk81A);
                    DUEL->unk81C = -1;
                    DUEL->unk818 = DUEL->unk819;
                    func_80033D08(0x14);
                    break;
                }
            }
            break;
        case 8:
            DUEL->unk822 = 0;
            if (func_80040764(ME) >= 0) {
                if (PLAYER(ME)->unk178_17 == 1) {
                    DUEL->unk818 = 0xB;
                } else {
                    DUEL->unk818 = 0xA;
                }
            } else {
                D_801D83D4 = 4;
                if (PLAYER(ME)->unk178_17 == 1) {
                    DUEL->unk827 = ME;
                    DUEL->unk816 = 2;
                    func_80033E7C();
                    if (DUEL->unk804 == -1) {
                        for (i = 0; i < 4; i++) {
                            if (PLAYER(ME)->unk1B9[i] != -1 && PLAYER(ME)->cards[(s8)(PLAYER(ME)->unk1B9[i] % 30)].state == 0) {
                                DUEL->unk804 = PLAYER(ME)->unk1B9[i];
                                break;
                            }
                        }
                    }
                    CUR_CARD = DUEL->unk804;
                    func_801EA558(CUR_CARD, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    i = func_80047B84(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (func_80048014(ME, func_80047A58(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            func_80033D08(0x3C);
                            func_801F6214(1, ME);
                            func_80040A48(ME, i);
                        }
                    }
                    func_80033D08(0x78);
                    DUEL->unk818 = 0xB;
                } else {
                    DUEL->unk818++;
                    func_801EC4CC(ME);
                }
            }
            break;
        case 9:
            DUEL->unk822 = 1;
            if (func_80040220(ME) != 0) {
                D_801D83D7 = 5;
            } else {
                D_801D83D7 = 3;
            }
            if (func_801EBACC(ME, 1) == 0) {
                if (PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].state == 0) {
                    func_8002B498(0xA0);
                    func_801EC528(ME);
                    func_801EA558(CUR_CARD, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    i = func_80047B84(ME, PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id);
                    if (i != -1) {
                        if (func_80048014(ME, func_80047A58(PLAYER(ME)->cards[(s16)(CUR_CARD % 30)].id)) != -1) {
                            D_801D83D4 = 5;
                            func_80019EA4((u8 *)&D_801D8278, "Do you want to Armor Digivolve?", 1);
                            func_800341EC();
                            switch (CHOICE) {
                            case 0:
                                if (DUEL->unk80A >= 0) {
                                    func_801EC84C(CUR_CARD, ME, DUEL->unk80A);
                                    DUEL->unk80A = -1;
                                    DUEL->unk818 = 8;
                                } else {
                                    DUEL->unk818 = 3;
                                }
                                break;
                            case 1:
                                func_801F6214(1, ME);
                                func_80040A48(ME, i);
                                PLAYER(ME)->unk110 |= 8;
                                func_801FA4E4(ME);
                                DUEL->unk818 = 0xB;
                                break;
                            case 2:
                                DUEL->unk818++;
                                break;
                            }
                        } else {
                            DUEL->unk818++;
                        }
                    } else {
                        DUEL->unk818++;
                    }
                }
            } else if ((D_80089840[ME]->unkA & 0x10) && func_80040220(ME) != 0) {
                func_8002B498(0xA1);
                func_801EC528(ME);
                DUEL->unk818 = 4;
            }
            break;
        case 10:
            DUEL->unk822 = 0;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 6;
            func_80019EA4((u8 *)&D_801D8278, "Is it OK to end the Preparation Phase?", 1);
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Press \"Yes\" to go to the next Phase!");
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC84C(CUR_CARD, ME, DUEL->unk80A);
                        DUEL->unk80A = -1;
                    }
                    DUEL->unk818 = 4;
                }
                break;
            case 1:
                if (DUEL->unk80A >= 0) {
                    func_801FA4E4(ME);
                }
                DUEL->unk818++;
                break;
            }
            break;
        case 11:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk80A = -2;
                DUEL->unk80E = -1;
                DUEL->unk80C = -1;
                DUEL->unk827 = ME;
                DUEL->unk816 = 3;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0xE;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 7;
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801ECBCC(CUR_CARD, ME);
                    func_80033D08(0x78);
                    DUEL->unk818 = 0xE;
                }
            } else {
                DUEL->unk818++;
            }
            break;
        case 12:
            DUEL->unk822 = 0;
            DUEL->unk80A = -2;
            DUEL->unk80E = -1;
            DUEL->unk80C = -1;
            if (func_80040570(ME) != 0) {
                DUEL->unk818 = 0xE;
            } else if (func_80041214(ME) != 0) {
                func_801EC4CC(ME);
                DUEL->unk818++;
            } else {
                DUEL->unk818 = 0xE;
            }
            break;
        case 13:
            DUEL->unk822 = 1;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next = 7;
            D_801D83D0.next2 = 7;
            if (func_801EBACC(ME, 2) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                if (PLAYER(ME)->cards[i % 30].state == 0) {
                    func_8002B498(0xA0);
                    DUEL->unk80E = func_801ECBCC(CUR_CARD, ME);
                    DUEL->unk818++;
                }
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0xD) {
                func_801EC528(ME);
            }
            break;
        case 14:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 4;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x13;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 8;
                    D_801D83EC[ME * 0xD8 + 0x55] = 6;
                    func_80033D08(0x1E);
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                    func_80033D08(0x1E);
                    DUEL->unk818 = 0x10;
                }
            } else if (func_800406BC(ME) != 0) {
                DUEL->unk80A = -1;
                DUEL->unk818 = 0x13;
            } else {
                DUEL->unk818++;
                func_801EC4CC(ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 6;
                D_801D83D0.unk3 = 1;
                D_801D83D0.next = 8;
                D_801D83D0.next2 = 6;
                func_80033D08(0x1E);
            }
            break;
        case 15:
            DUEL->unk822 = 1;
            if (func_801EBACC(ME, 5) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                if (PLAYER(ME)->cards[i % 30].state == 2) {
                    func_8002B498(0xA0);
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                    if (func_801EA374(ME) != 0) {
                        func_80019EA4((u8 *)&D_801D8278, "This Digivolve Option has no Effect.\nDo you still want to use it?", 1);
                        func_800341EC();
                        switch (CHOICE) {
                        case 0:
                        case 2:
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                            break;
                        case 1:
                            func_801EC528(ME);
                            DUEL->unk818++;
                            break;
                        }
                    } else {
                        DUEL->unk818++;
                    }
                }
            } else if (D_80089840[ME]->unkA & 0x30) {
                DUEL->unk81C = -1;
                if (D_80089840[ME]->unkA & 0x20) {
                    func_8002B498(0xA0);
                    func_801EC528(ME);
                    D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    DUEL->unk818 = 0x13;
                } else if (D_80089840[ME]->unkA & 0x10) {
                    func_8002B498(0xA1);
                    if (DUEL->unk80E >= 0) {
                        func_801ECA30(func_800411C4(ME), ME, DUEL->unk80E);
                        DUEL->unk80E = -1;
                    }
                    D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    DUEL->unk818 = 0xB;
                }
            }
            break;
        case 16:
            DUEL->unk822 = 0;
            D_801D83D4 = 9;
            mode = 0;
            if (func_801EA374(ME) == 0) {
                card = PLAYER(ME)->cards[func_80041340(ME) % 30].card;
                switch (card[0x1A]) {
                case 4:
                    func_80019EA4((u8 *)&D_801D8278, "Current Digimon will be discarded,\ndo you still want to \"Digi-devolve\"?", 1);
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_800341EC();
                    } else {
                        D_801D831D = 1;
                    }
                    switch (D_801D831D) {
                    case 0:
                    case 2:
                        mode = 2;
                        func_801EC8E0(ME, DUEL->unk80A);
                        DUEL->unk80A = -2;
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                        break;
                    case 1:
                        mode = 3;
                        func_801F6214(9, ME);
                        func_801EC608(func_80040764(ME), ME);
                        PLAYER(ME)->unk178_15 = 0;
                        func_8004080C(func_80040764(ME), ME);
                        PLAYER(ME)->unk11C[0] *= 2;
                        func_80033D08(0x14);
                        break;
                    }
                    break;
                case 7:
                    func_80019EA4((u8 *)&D_801D8278, "Your Digimon's Level will become *e3,\ndo you still want to \"Armor Digi-devolve\"?", 1);
                    if (PLAYER(ME)->unk178_17 != 1) {
                        func_800341EC();
                    } else {
                        D_801D831D = 1;
                    }
                    switch (D_801D831D) {
                    case 0:
                    case 2:
                        mode = 2;
                        func_801EC8E0(ME, DUEL->unk80A);
                        DUEL->unk80A = -2;
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                        break;
                    case 1:
                        mode = 3;
                        func_801F6214(8, ME);
                        func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                        break;
                    }
                    break;
                default:
                    mode = 1;
                    break;
                }
            }
            if (mode == 0 || mode == 3) {
                DUEL->unk80C = DUEL->unk80A;
                i = func_80041408(ME);
                D_801D833C[i * 0x24 + 0x22] = 8;
                func_800400B4(i, ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 1;
                func_801EC528(ME);
            }
            switch (mode) {
            case 0:
                DUEL->unk818 = 0x13;
                break;
            case 1:
                DUEL->unk818 = 0x11;
                break;
            case 2:
                DUEL->unk818 = 0xE;
                break;
            case 3:
                PLAYER(ME)->unk110 |= 0x40000000;
                DUEL->unk818 = 0x17;
                break;
            }
            if (PLAYER(ME)->unk178_17 == 1) {
                func_80033D08(0x3C);
            }
            break;
        case 17:
            DUEL->unk822 = 0;
            DUEL->unk818 = 0x12;
            break;
        case 18:
            DUEL->unk822 = 1;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 5;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x17;
                    i = 0;
                    func_801EC528(ME);
                } else {
                    func_80033D08(0x3C);
                    CUR_CARD = DUEL->unk804;
                    i = 0;
                }
            } else {
                D_801D83D7 = 6;
                i = func_801EBACC(ME, 6);
                if (i != 0) {
                    if (D_80089840[ME]->unkA & 0x20) {
                        func_8002B498(0xA0);
                        if (DUEL->unk80A >= 0) {
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                        }
                        DUEL->unk818 = 0x13;
                        func_801EC528(ME);
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    } else if (D_80089840[ME]->unkA & 0x10) {
                        func_8002B498(0xA1);
                        if (DUEL->unk80A >= 0) {
                            func_801EC8E0(ME, DUEL->unk80A);
                            DUEL->unk80A = -2;
                        }
                        DUEL->unk818 = 0xE;
                        func_801EC528(ME);
                        D_801D83EC[ME * 0xD8 + 0x55] = 1;
                    }
                }
            }
            if (i == 0 && func_801E9F5C(CUR_CARD, ME) == 0) {
                func_801EC528(ME);
                card = PLAYER(ME)->cards[func_80041340(ME) % 30].card;
                switch (card[0x1A]) {
                case 0:
                    func_801F6214(3, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 1:
                    func_801F6214(4, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 2:
                    func_801F6214(5, ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 3:
                    func_801F6214(7, ME);
                    func_801EC608(func_80040764(ME), ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                case 5:
                    func_801F6214(2, ME);
                    func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                    while (func_80040764(ME) != -1) {
                        func_801EC608(func_80040764(ME), ME);
                        func_80033D08(0x14);
                    }
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    break;
                case 6:
                    func_801F6214(6, ME);
                    func_80040D88(ME, func_80048150(ME, PLAYER(ME)->cards[func_80040764(ME) % 30].id));
                    func_801EC608(func_80040764(ME), ME);
                    PLAYER(ME)->unk178_15 = 0;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    break;
                }
                PLAYER(ME)->unk110 |= 8;
                func_801FA4E4(ME);
                DUEL->unk818 = 0x17;
            }
            if (DUEL->unk818 != 0x12 && DUEL->unk818 != 0xE && DUEL->unk818 != 0x13) {
                i = func_80041408(ME);
                D_801D833C[i * 0x24 + 0x22] = 8;
                func_800400B4(i, ME);
                D_801D83EC[ME * 0xD8 + 0x55] = 1;
                func_80033D08(0x14);
                PLAYER(ME)->unk110 |= 0x40000000;
            }
            break;
        case 19:
            DUEL->unk822 = 0;
            if (PLAYER(ME)->unk178_17 == 1) {
                DUEL->unk827 = ME;
                DUEL->unk816 = 5;
                func_80033E7C();
                if (DUEL->unk804 == -1) {
                    DUEL->unk818 = 0x17;
                } else {
                    D_801D83D0.unk3 = 1;
                    D_801D83D0.next = 9;
                    func_801F6214(0, ME);
                    CUR_CARD = DUEL->unk804;
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                }
            } else if (func_80040570(ME) != 0) {
                DUEL->unk818 = 0x16;
            } else if (func_801EA374(ME) != 0) {
                DUEL->unk818 = 0x16;
            } else {
                DUEL->unk818++;
            }
            break;
        case 20:
            DUEL->unk822 = 0;
            func_801EC4CC(ME);
            DUEL->unk818++;
            break;
        case 21:
            DUEL->unk822 = 1;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next = 9;
            if (func_801EBACC(ME, 3) == 0) {
                if (func_801E9F5C(PLAYER(ME)->unk1B9[DUEL->unk81C], ME) == 0) {
                    func_801EC528(ME);
                    func_801F6214(0, ME);
                    DUEL->unk80A = func_801EC7C0(CUR_CARD, ME);
                    func_801ECE24();
                    PLAYER(ME)->unk110 |= 8;
                    func_801FA4E4(ME);
                }
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                DUEL->unk818++;
                func_801EC528(ME);
            } else if (D_80089840[ME]->unkA & 0x10) {
                func_8002B498(0xA1);
                func_801ECD68();
                func_801EC528(ME);
            }
            break;
        case 22:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 1;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 0xA;
            D_801D83EC[ME * 0xD8 + 0x55] = 1;
            func_80019EA4((u8 *)&D_801D8278, "Is it OK to end the Digivolve Phase?", 1);
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Choose \"Yes\" to go to next Phase!");
                } else if (DUEL->unk80A == -2) {
                    DUEL->unk818 = 0xE;
                } else {
                    func_801ECD68();
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 23:
            DUEL->unk822 = 0;
            PLAYER(ME)->unk178_30 = 1;
            PLAYER(ME)->unk114 = PLAYER(ME)->cards[func_80040764(ME) % 30].card;
            if (func_80040764(OPP) == -1) {
                if (PLAYER(ME)->unk178_17 != 1) {
                    D_801D83D0.unk3 = 2;
                    D_801D83D0.next = 0xB;
                    sprintf(buf, "Since %s has no Digimon,\nthere is no Battle Phase.", PLAYER(OPP)->unk1CE);
                    func_80019EA4((u8 *)&D_801D8278, buf, 0);
                    func_800341EC();
                }
                DUEL->unk818 = 0x25;
            } else {
                DUEL->unk80A = -1;
                DUEL->unk80E = -1;
                DUEL->unk818++;
            }
            break;
        case 24:
            DUEL->unk822 = 0;
            D_801D83D0.unk3 = 2;
            D_801D83D0.next = 0xC;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->unk178_17 == 1) {
                    DUEL->unk827 = i;
                    if (DUEL->unk81F != 0) {
                        DUEL->unk816 = 0;
                    } else {
                        DUEL->unk816 = 6;
                    }
                }
            }
            D_801D83EC[0x31] = 1;
            D_801D83EC[0x109] = 1;
            func_80014C08(0x1E);
            DUEL->unk818++;
            break;
        case 25:
            DUEL->unk822 = 1;
            D_801D83D7 = 2;
            for (i = 0; i < 2; i++) {
                if (PLAYER(i)->unk178_17 == 1) {
                    if (DUEL->unk816 == 0 && PLAYER(i)->unk178_2 == 3) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = DUEL->unk804;
                    }
                } else if (PLAYER(i)->unk178_2 == 3) {
                    if (D_80089840[i]->unkA & 0x20) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 0;
                    } else if (D_80089840[i]->unkA & 0x10) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 1;
                    } else if (D_80089840[i]->unkA & 0x40) {
                        func_8002B498(0xA0);
                        PLAYER(i)->unk178_2 = 2;
                    } else if (D_80089840[i]->unkA & 0x80) {
                        func_8002B498(0xA0);
                        func_801EC4CC(i);
                        D_801D83EC[0x31] = 4;
                        D_801D83EC[0x109] = 4;
                        DUEL->unk818 = 7;
                        DUEL->unk819 = 0x19;
                        DUEL->unk81A = i;
                        break;
                    }
                }
            }
            if (PLAYER(0)->unk178_2 != 3 && PLAYER(1)->unk178_2 != 3) {
                func_80033D08(0x78);
                D_801D83EC[0x31] = 4;
                D_801D83EC[0x109] = 4;
                for (i = 0; i < 2; i++) {
                    PLAYER(i)->unk178_0 = PLAYER(i)->unk178_2;
                    if (((Unk8006E050 *)D_8006E050)[i].unk36[PLAYER(i)->unk178_0] != 0xFFFF) {
                        ((Unk8006E050 *)D_8006E050)[i].unk36[PLAYER(i)->unk178_0]++;
                    }
                }
                func_80033D08(0x78);
                DUEL->unk818++;
            }
            break;
        case 26:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0xD;
            D_801D83D0.unk1 = PLAYER(OPP)->unk178_17;
            D_801D83EC[OPP * 0xD8 + 0x55] = 6;
            func_80033D08(0x1E);
            if (PLAYER(OPP)->unk178_17 == 1) {
                D_801D83D0.next2 = 0;
                DUEL->unk827 = OPP;
                DUEL->unk816 = 7;
                func_80033E7C();
                if (DUEL->unk804 == -2) {
                    func_80033D08(0x3C);
                    func_801ECAC4(OPP);
                    func_80033D08(0x78);
                } else if (DUEL->unk804 != -1) {
                    if (PLAYER(OPP)->cards[DUEL->unk804 % 30].card[2] < 2) {
                        func_80033D08(0x3C);
                        DUEL->unk80A = func_801ECB40(DUEL->unk804, OPP);
                        func_80033D08(0x78);
                    }
                }
                DUEL->unk818 = 0x1D;
            } else {
                DUEL->unk80A = -1;
                if (func_80040468(OPP) == 4 && func_80040220(OPP) == 0) {
                    func_80019EA4((u8 *)&D_801D8278, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    func_8001A164(&D_801D8278, PLAYER(OPP)->unk178_17 & 1);
                    DUEL->unk818 = 0x1D;
                } else {
                    func_801EC4CC(OPP);
                    DUEL->unk818++;
                }
            }
            break;
        case 27:
            DUEL->unk822 = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(OPP, 4) == 0) {
                i = PLAYER(OPP)->unk1B9[DUEL->unk81C];
                c = DUEL->unk81C;
                if (c == 4) {
                    DUEL->unk80A = c;
                    func_801ECAC4(OPP);
                } else {
                    if (PLAYER(OPP)->cards[i % 30].state == 2) {
                        break;
                    }
                    DUEL->unk80A = func_801ECB40(CUR_CARD, OPP);
                }
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                DUEL->unk818++;
            } else if (D_80089840[OPP]->unkA & 0x20) {
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0x1B) {
                func_801EC528(OPP);
            }
            break;
        case 28:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            func_8001A164(&D_801D8278, PLAYER(OPP)->unk178_17 & 1);
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC8E0(OPP, DUEL->unk80A);
                    }
                    DUEL->unk818 = 0x1A;
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 29:
            DUEL->unk822 = 0;
            D_801D83D0.next = 0xE;
            D_801D83D0.unk1 = PLAYER(ME)->unk178_17;
            D_801D83EC[ME * 0xD8 + 0x55] = 6;
            func_80033D08(0x1E);
            if (PLAYER(ME)->unk178_17 == 1) {
                D_801D83D0.next2 = 0;
                DUEL->unk827 = ME;
                DUEL->unk816 = 7;
                func_80033E7C();
                if (DUEL->unk804 == -2) {
                    func_80033D08(0x3C);
                    func_801ECAC4(ME);
                    func_80033D08(0x78);
                } else if (DUEL->unk804 != -1) {
                    if (PLAYER(ME)->cards[DUEL->unk804 % 30].card[2] < 2) {
                        func_80033D08(0x3C);
                        DUEL->unk80A = func_801ECB40(DUEL->unk804, ME);
                        func_80033D08(0x78);
                    }
                }
                DUEL->unk818 = 0x20;
            } else {
                DUEL->unk80A = -1;
                if (func_80040468(ME) == 4 && func_80040220(ME) == 0) {
                    func_80019EA4((u8 *)&D_801D8278, "You have no Cards left, so\nyou can't use any Support Cards!", 0);
                    func_8001A164(&D_801D8278, PLAYER(ME)->unk178_17 & 1);
                    DUEL->unk818 = 0x20;
                } else {
                    func_801EC4CC(ME);
                    DUEL->unk818++;
                }
            }
            break;
        case 30:
            DUEL->unk822 = 1;
            D_801D83D7 = 7;
            if (func_801EBACC(ME, 4) == 0) {
                i = PLAYER(ME)->unk1B9[DUEL->unk81C];
                c = DUEL->unk81C;
                if (c == 4) {
                    DUEL->unk80A = c;
                    func_801ECAC4(ME);
                } else {
                    if (PLAYER(ME)->cards[i % 30].state == 2) {
                        break;
                    }
                    DUEL->unk80A = func_801ECB40(CUR_CARD, ME);
                }
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                DUEL->unk818++;
            } else if (D_80089840[ME]->unkA & 0x20) {
                func_8002B498(0xA0);
                func_80019EA4((u8 *)&D_801D8278, "You're not using any Support Card.\nIs this OK?", 1);
                DUEL->unk818++;
            }
            if (DUEL->unk818 != 0x1E) {
                func_801EC528(ME);
            }
            break;
        case 31:
            DUEL->unk822 = 0;
            D_801D83D7 = 0;
            func_800341EC();
            switch (CHOICE) {
            case 0:
            case 2:
                if (DUEL->unk81F != 0) {
                    func_801EA8B4(0x78, "Please choose \"Yes\"!");
                    func_80019EA4((u8 *)&D_801D8278, "Do you want to use this Support Card?", 1);
                } else {
                    if (DUEL->unk80A >= 0) {
                        func_801EC8E0(ME, DUEL->unk80A);
                    }
                    DUEL->unk818 = 0x1D;
                }
                break;
            case 1:
                DUEL->unk818++;
                break;
            }
            break;
        case 32:
            DUEL->unk822 = 0;
            D_801D83D0.next2 = 0;
            D_801D83D0.next = 0xF;
            func_80033D08(0x3C);
            if (((Unk8006E050 *)D_8006E050)->unk20_3) {
                D_801D8330 = 0x20;
                func_8001683C((s32)func_80033F34);
                while (D_801D8330 != 0) {
                    func_80014C08(D_800794F0);
                }
                func_80014C08(0x14);
            }
            func_801E6AA4(0);
            DUEL->unk818++;
            break;
        case 33:
            DUEL->unk822 = 0;
            D_801D83D4 = 0x11;
            DUEL->unk818++;
            break;
        case 34:
            DUEL->unk822 = 0;
            DUEL->unk81C = -1;
            if (!((Unk8006E050 *)D_8006E050)->unk20_3) {
                func_80014C08(0x3C);
                DUEL->state = 1;
                func_80014C08(2);
                DUEL->unk83C = 1;
                func_8002E26C();
                DUEL->unk83C = 0;
                func_80014C08(2);
                ((Unk800794F8 *)&D_800794F8)->unk54 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk56 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk58 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk7C = 0;
                ((Unk800794F8 *)&D_800794F8)->unk80 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk84 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk8E = 0;
                ((Unk800794F8 *)&D_800794F8)->unk90 = 0x1C0;
                ((Unk800794F8 *)&D_800794F8)->unk92 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk94 = 0;
                ((Unk800794F8 *)&D_800794F8)->unk8C = -1;
                ((Unk800794F8 *)&D_800794F8)->unk74 = 1;
                func_80014C08(2);
                DUEL->state = 6;
            }
            DUEL->unk818++;
            break;
        case 35:
            DUEL->unk822 = 0;
            over = 0;
            if (((Unk8006E050 *)D_8006E050)->unk20_3) {
                a = DUEL->unk50;
                b = DUEL->unk54;
                j = a->unk178_17 & 1;
                func_80014C08(0x14);
                if (a->unk178_11 && !b->unk178_6) {
                    func_801F6268(0x1B, j);
                    a->unk11C[0] = 10;
                }
                if (b->unk162 == 0) {
                    func_801F6268(0x1C, j);
                } else if (a->unk178_12) {
                    func_801F6268(0x1A, j);
                    i = a->unk11C[0] + a->unk164;
                    func_80039354(j, i, 0);
                    if (i > 9990) {
                        i = 9990;
                    }
                    a->unk11C[0] = i;
                    if (i != 0 && i % 1110 == 0) {
                        func_801FB444(j, 0x1A);
                    }
                } else if (!a->unk178_11) {
                    func_801F6268(0x18, j);
                }
                i = b->unk11C[0] - b->unk162;
                if (b->unk162 != 0) {
                    func_80039354(j ^ 1, i, 0);
                    if (b->unk162 != 0 && b->unk162 % 1110 == 0) {
                        func_801FB444(j, 0x19);
                    }
                }
                if (i == 0 && a->unk17C == 2) {
                    func_801FB444(j, 0x13);
                }
                if (i < 0) {
                    i = 0;
                }
                b->unk11C[0] = i;
                if (i != 0 && i % 1110 == 0 && b->unk162 != 0) {
                    func_801FB444(j ^ 1, 0x1A);
                }
                func_8003917C();
                if (func_801ECF0C(j ^ 1) != 0) {
                    over = 1;
                }
                func_80014C08(0x14);
                if (b->unk15A != 0) {
                    if (b->unk178_11 && a->unk162 != 0) {
                        func_801F6268(0x1B, j ^ 1);
                        b->unk11C[0] = 10;
                    }
                    if (a->unk162 == 0) {
                        func_801F6268(0x1C, j ^ 1);
                    } else if (b->unk178_6 || b->unk178_12) {
                        if (b->unk178_6) {
                            func_801F6268(0x19, j ^ 1);
                        }
                        if (b->unk178_12) {
                            func_801F6268(0x1A, j ^ 1);
                            i = b->unk11C[0] + b->unk164;
                            func_80039354(j ^ 1, i, 0);
                            if (i > 9990) {
                                i = 9990;
                            }
                            b->unk11C[0] = i;
                            if (i != 0 && i % 1110 == 0) {
                                func_801FB444(j ^ 1, 0x1A);
                            }
                        }
                    } else if (!b->unk178_11) {
                        func_801F6268(0x18, j ^ 1);
                    }
                    i = a->unk11C[0] - a->unk162;
                    if (a->unk162 != 0) {
                        func_80039354(j, i, 0);
                        if (a->unk162 != 0 && a->unk162 % 1110 == 0) {
                            func_801FB444(j ^ 1, 0x19);
                        }
                    }
                    if (i == 0 && b->unk17C == 2) {
                        func_801FB444(j ^ 1, 0x13);
                    }
                    if (i < 0) {
                        i = 0;
                    }
                    a->unk11C[0] = i;
                    if (i != 0 && i % 1110 == 0 && a->unk162 != 0) {
                        func_801FB444(j, 0x1A);
                    }
                    func_8003917C();
                    if (func_801ECF0C(j) != 0) {
                        over = 1;
                    }
                }
                func_80016878((s32)func_80033F34);
                if (over) {
                    DUEL->unk818++;
                } else {
                    DUEL->unk818 = 0x25;
                }
            } else {
                func_80014C08(0x3C);
                PLAYER(0)->unk11C[0] = PLAYER(0)->unk15A;
                PLAYER(1)->unk11C[0] = PLAYER(1)->unk15A;
                if (PLAYER(ME)->unk11C[0] == PLAYER(ME)->unk126[0] && PLAYER(OPP)->unk11C[0] == PLAYER(OPP)->unk126[0]) {
                    if (func_801ECF0C(ME) != 0) {
                        DUEL->unk818++;
                    } else if (func_801ECF0C(OPP) != 0) {
                        DUEL->unk818++;
                    } else {
                        DUEL->unk818 = 0x25;
                    }
                }
            }
            break;
        case 36:
            DUEL->unk822 = 0;
            DUEL->unk818++;
            if (PLAYER(DUEL->unk81E)->unk17C == 2 && PLAYER(DUEL->unk81E ^ 1)->unk17C == 0) {
                PLAYER(DUEL->unk81E ^ 1)->unk110 |= 0x80;
                PLAYER(DUEL->unk81E)->unk110 |= 0x100;
            }
            if (func_80048150(DUEL->unk81E, PLAYER(DUEL->unk81E)->cards[func_80040764(DUEL->unk81E) % 30].id) >= 0) {
                func_801FB444(DUEL->unk81E, 0xA);
                PLAYER(DUEL->unk81E)->unk110 |= 0x4000;
            } else if (func_80047B84(DUEL->unk81E, PLAYER(DUEL->unk81E)->cards[func_80040764(DUEL->unk81E) % 30].id) >= 0) {
                func_801FB444(DUEL->unk81E, 0xA);
                PLAYER(DUEL->unk81E)->unk110 |= 0x4000;
            }
            if (PLAYER(DUEL->unk81E)->unk17C == 3) {
                if (PLAYER(DUEL->unk81E)->unk178_31) {
                    PLAYER(DUEL->unk81E)->unk110 |= 0x20000;
                    PLAYER(DUEL->unk81E ^ 1)->unk110 |= 0x40000;
                }
                sprintf(buf, "%d Wins, %d Losses-%s WINS!", PLAYER(DUEL->unk81E)->unk17C, PLAYER(DUEL->unk81E ^ 1)->unk17C, PLAYER(DUEL->unk81E)->unk1CE);
                func_80019EA4((u8 *)&D_801D8278, buf, 0);
                func_800341EC();
                DUEL->unk818 = 0x26;
            } else if (func_80040764(DUEL->unk81E ^ 1) == -1 && func_80040570(DUEL->unk81E ^ 1) != 0 && func_80040220(DUEL->unk81E ^ 1) == 0) {
                sprintf(buf, "Since %s has no more Digimon,\nthe winner is %s!", PLAYER(DUEL->unk81E ^ 1)->unk1CE, PLAYER(DUEL->unk81E)->unk1CE);
                func_80019EA4((u8 *)&D_801D8278, buf, 0);
                func_800341EC();
                DUEL->unk818 = 0x26;
            }
            break;
        case 37:
            DUEL->unk822 = 0;
            D_801D83D1 = PLAYER(ME)->unk178_17;
            DUEL->unk818++;
            for (i = 0; i < 2; i++) {
                PLAYER(i)->unk11C[1] = PLAYER(i)->unk15C[0];
                PLAYER(i)->unk11C[2] = PLAYER(i)->unk15C[1];
                PLAYER(i)->unk11C[3] = PLAYER(i)->unk15C[2];
                j = func_80041408(i);
                if (j != -1) {
                    D_801D833C[j * 0x24 + 0x22] = 8;
                    func_800400B4(j, i);
                }
                D_801D83EC[0x55] = 1;
                D_801D83EC[0x12D] = 1;
            }
            DUEL->unk817 ^= 1;
            DUEL->unk818 = 1;
            break;
        case 38:
            DUEL->unk822 = 0;
            DUEL->unk818++;
        case 39:
            DUEL->unk822 = 0;
            break;
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/sound", func_80038F68);

void func_8003917C(void) {
    s32 diff;
    s32 p;
    s32 i;

    do {
        func_80014C08(D_800794F0);
        diff = 0;
        for (p = 0; p < 2; p++) {
            for (i = 0; i < 5; i++) {
                if (((Player *)D_801D8348[p])->unk11C[i] !=
                    ((Player *)D_801D8348[p])->unk126[i]) {
                    diff = 1;
                }
            }
        }
    } while (diff);
}

void func_80039220(s32 p) {
    s32 idx;
    u8 *q;

    idx = func_800411C4(p);
    if (idx != -1) {
        q = D_801D83EC + (p * 0xD8 + 0x48);
        PLAYER(p)->unk130[4].value = (s8)PLAYER(p)->cards[idx % 30].card[0x1C];
        PLAYER(p)->unk130[4].type = 5;
        PLAYER(p)->unk130[4].timer = 0x30;
        PLAYER(p)->unk130[4].x = *(s16 *)(q + 0x10) + (s16)(p * 93 + 0x10);
        PLAYER(p)->unk130[4].y = *(u16 *)(q + 0x12) + 2;
    } else {
        PLAYER(p)->unk130[4].timer = 0;
    }
}

void func_80039354(s32 p, s32 v, s32 k) {
    s32 c;
    u8 *q;

    PLAYER(p)->unk130[k].value = v - PLAYER(p)->unk11C[k];
    if (PLAYER(p)->unk130[k].value == 0) {
        PLAYER(p)->unk130[k].type = 7;
    } else if (PLAYER(p)->unk130[k].value > 0) {
        PLAYER(p)->unk130[k].type = 5;
    } else {
        PLAYER(p)->unk130[k].type = 2;
    }
    PLAYER(p)->unk130[k].value = abs(PLAYER(p)->unk130[k].value);
    PLAYER(p)->unk130[k].timer = 0x30;
    if (k == 0) {
        c = func_80040764(p);
        func_8004480C(*(void **)(D_801D833C + c * 36), c);
        PLAYER(p)->unk130[0].x = *(u16 *)(*(u8 **)(D_801D833C + c * 36) + 0x34) + 0x19;
        PLAYER(p)->unk130[0].y = *(u16 *)(*(u8 **)(D_801D833C + c * 36) + 0x36) + 0x15;
    } else {
        q = D_801D83EC + (p * 0xD8 + 0x48);
        PLAYER(p)->unk130[k].x = *(u16 *)(q + 0x10) + p * 25 + 0x1C;
        PLAYER(p)->unk130[k].y = *(s16 *)(q + 0x12) + (s16)((k - 1) * 13 + 3);
    }
}

void func_800395A0(void) {
    char buf[24];
    s32 p;
    s32 k;
    char *sign;
    s32 size;

    for (p = 0; p < 2; p++) {
        for (k = 4; k >= 0; k--) {
            if (((Player *)D_801D8348[p])->unk130[k].timer != 0) {
                ((Player *)D_801D8348[p])->unk130[k].timer--;
                if (((Player *)D_801D8348[p])->unk130[k].type == 7) {
                    sign = "=";
                } else if (((Player *)D_801D8348[p])->unk130[k].type == 5) {
                    sign = "+";
                } else {
                    sign = "-";
                }
                size = ((Player *)D_801D8348[p])->unk130[k].timer;
                if (size < 0x2C) {
                    size = 0x2C;
                }
                sprintf(buf, "%s%d", sign, ((Player *)D_801D8348[p])->unk130[k].value);
                func_8002961C(((Player *)D_801D8348[p])->unk130[k].x + (0x30 - size),
                              ((Player *)D_801D8348[p])->unk130[k].y - (0x30 - size) * 2, (u8 *)buf, (u8 *)&D_8006E298,
                              ((Player *)D_801D8348[p])->unk130[k].type, 0);
            }
        }
    }
}

void func_80039730(s32 n, s32 z) {
    s32 p = n / 6;
    InfoPanel *panel = (InfoPanel *)D_801D83EC + n;
    char buf[72];
    u8 rgb[2][4] = { { 0x80, 0x80, 0x80, 0 }, { 0x40, 0x40, 0x40, 0 } };
    char buf2[40];
    u8 *cols[10];
    CardInfo *card;
    s32 color;
    s32 i;
    s32 k;
    s32 rival;
    s32 x;
    s32 y;

    switch (n) {
    case 2:
    case 8: {
        s32 idx;

        idx = func_80040764(p);
        if (idx >= 0) {
            color = PLAYER(p)->unk178_15 ? 3 : 7;
            card = (CardInfo *)PLAYER(p)->cards[idx % 30].card;
            func_80027DB8(panel->x - p * 14 + 17, panel->y + 2, (s32)card->name, 7, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[1]);
            func_80028D18(panel->x + 42 + p * 25, panel->y + 11, (s32)buf, color, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[2]);
            func_80028D18(panel->x + 42 + p * 25, panel->y + 24, (s32)buf, color, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk126[3]);
            func_80028D18(panel->x + 42 + p * 25, panel->y + 37, (s32)buf, color, z);
            func_80027DB8(panel->x + 24 + p * 24, panel->y + 51, (s32)D_8006E47C[card->unkE4], 7, z);
        }
        sprintf(buf, "*s0%2d", PLAYER(p)->unk126[4]);
        func_80028D18(panel->x + 6 + p * 93, panel->y + 9, (s32)buf, 7, z);
        k = 8 - func_80041214(p);
        if (k != 0) {
            CUR_SPRT->sp.x0 = panel->x + 3 + p * 94;
            CUR_SPRT->sp.y0 = panel->y + 50;
            CUR_SPRT->sp.u0 = 0xF0;
            CUR_SPRT->sp.v0 = 0x47;
            CUR_SPRT->sp.clut = getClut(800, k + 0x1F7);
            CUR_SPRT->sp.w = 16;
            CUR_SPRT->sp.h = 8;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        }
        break;
    }
    case 5:
    case 11:
        if (panel->state == 5) {
            CUR_SPRT->sp.x0 = panel->x + 5;
            CUR_SPRT->sp.y0 = panel->y - 56 + p * 64;
            CUR_SPRT->sp.u0 = 0;
            CUR_SPRT->sp.v0 = 0xBA;
            CUR_SPRT->sp.clut = 0x7CF3;
            CUR_SPRT->sp.w = 32;
            CUR_SPRT->sp.h = 62;
            setSemiTrans(&CUR_SPRT->sp, 1);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x3D);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        }
        break;
    case 4:
    case 10:
        sprintf(buf, "%s Deck", PLAYER(p)->unk0 + 1);
        func_80028D18(panel->x + 1 + (0x82 - func_800293FC((u8 *)buf)) / 2, panel->y + 0x33 + p * -50, (s32)buf, 7, z);
        func_80028D18(panel->x + 0x85 + (0x78 - func_800293FC((u8 *)PLAYER(p)->unk1CE)) / 2, panel->y + 0x33 + p * -50,
                      (s32)PLAYER(p)->unk1CE, 7, z);
        k = func_80040220(p) >= 8 ? 7 : 2;
        sprintf(buf, "*s0%2d", func_80040220(p));
        func_80028D18(panel->x + 4 + p * 0xEC, panel->y + 0x1E + p * 14, (s32)buf, k, z);
        sprintf(buf, "*s0%2d", func_80040124(p));
        func_80028D18(panel->x + 4 + p * 0xEC, panel->y + 6 + p * 14, (s32)buf, 7, z);
        for (k = 0; k < PLAYER(p)->unk17C; k++) {
            func_800446A4(panel->x + 0xDF + p * -0xDD, panel->y + 4 + p * 13 + k * 15, 0x4A);
        }
        break;
    case 0:
    case 6: {
        s32 back;
        s32 shift;

        if (*(s16 *)(DUEL->unk58 + 2) == -1) {
            break;
        }
        back = DUEL->cache[DUEL->unk826].used;
        if (back == 1) {
            if (func_80029990() != 0) {
                break;
            }
            CUR_SPRT->sp.x0 = panel->x;
            CUR_SPRT->sp.y0 = panel->y + 7;
            CUR_SPRT->sp.u0 = (DUEL->unk826 & 1) << 6;
            CUR_SPRT->sp.v0 = ((DUEL->unk826 >> 1) << 6) + 0x40;
            CUR_SPRT->sp.clut = (0x1FF - DUEL->unk826) << 6;
            CUR_SPRT->sp.w = 64;
            CUR_SPRT->sp.h = 64;
            setSemiTrans(&CUR_SPRT->sp, 0);
            CUR_SPRT->sp.r0 = 0x80;
            CUR_SPRT->sp.g0 = 0x80;
            CUR_SPRT->sp.b0 = 0x80;
            setDrawMode(&CUR_SPRT->dm, 0, 0, 0x9A);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
            addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
            D_801D6B24 += sizeof(SprtPacket);
        } else if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19 || DUEL->unk81C == 4) {
            func_80042BBC(panel->x, panel->y + 7, z, p, 0);
        } else {
            func_80042BBC(panel->x, panel->y + 7, z, p, SPRITE(*(s16 *)(DUEL->unk58 + 2)));
        }
        if (SPRITE_KIND(*(s16 *)(DUEL->unk58 + 2)) == 0x19) {
            func_80028D18(panel->x + 0x8E, panel->y + 0x10, (s32)"*h-1All-or-Nothing\nGamble!", 7, z);
            break;
        }
        if (DUEL->unk81C == 4) {
            if (DUEL->unk81D == 4) {
                sprintf(buf2, "*h-1All-or-Nothing\nGamble!\nCards left in the\nOnline Deck are %d.",
                        func_80040220(DUEL->unk81B));
            } else {
                sprintf(buf2, "*h-1Cards left in the\nOnline Deck is %d.", func_80040220(DUEL->unk81B));
            }
            func_80028D18(panel->x + 0x8E, panel->y + 0x10, (s32)buf2, 7, z);
            break;
        }
        for (i = 0; i < 10; i++) {
            cols[i] = rgb[0];
        }
        switch (DUEL->unk81D) {
        case 1:
            cols[0] = rgb[1];
            cols[1] = rgb[1];
            cols[7] = rgb[1];
            cols[8] = rgb[1];
            cols[9] = rgb[1];
            break;
        case 2:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[1] = rgb[0];
            break;
        case 3:
        case 6:
            cols[1] = rgb[1];
            cols[7] = rgb[1];
            cols[8] = rgb[1];
            cols[9] = rgb[1];
            break;
        case 4:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[7] = rgb[0];
            cols[8] = rgb[0];
            break;
        case 5:
            for (i = 0; i < 10; i++) {
                cols[i] = rgb[1];
            }
            cols[9] = rgb[0];
            break;
        }
        switch (PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].state) {
        case 0:
            card = (CardInfo *)PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            sprintf(buf, "*s0%2d", card->unk1B);
            func_80028D48(panel->x + 0x7A, panel->y + 0x17, (s32)buf, (s32 *)cols[0], 7, z);
            sprintf(buf, "*s0%2d", card->level);
            func_80028D48(panel->x + 0x7C, panel->y + 0x2D, (s32)buf, (s32 *)cols[1], 7, z);
            if (DUEL->unk81D == 1 || DUEL->unk81D == 3) {
                if (DUEL->unk81C < 4 && DUEL->unk81B == DUEL->unk817) {
                    if (DUEL->unk81D == 1) {
                        shift = card->attr & 0xF;
                    } else {
                        shift = PLAYER(p)->unk178_15;
                    }
                    shift--;
                    color = 3;
                    if (shift <= 0) {
                        color = 7;
                        shift = 0;
                    }
                    sprintf(buf, "*s0%4d", (card->hp >> shift) / 10 * 10);
                    func_80028D48(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], color, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(buf, "*s0%4d", (card->attack[i].power >> shift) / 10 * 10);
                        x = panel->x;
                        y = panel->y;
                        func_80028D48(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], color, z);
                    }
                    func_800299DC(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                } else if (DUEL->unk81C == 6) {
                    color = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                    for (i = 0; i < 4; i++) {
                        sprintf(buf, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                        x = panel->x;
                        y = panel->y;
                        func_80028D48(x + 0x56, i * 12 + y + 0xE, (s32)buf, (s32 *)cols[i + 2], color, z);
                    }
                    func_800299DC(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
                } else {
                    sprintf(buf, "*s0%4d", card->hp);
                    func_80028D48(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], 7, z);
                    for (i = 0; i < 3; i++) {
                        sprintf(buf, "*s0%4d", card->attack[i].power);
                        x = panel->x;
                        y = panel->y;
                        func_80028D48(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], 7, z);
                    }
                    func_800299DC(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
                }
            } else if (DUEL->unk81C == 6) {
                color = PLAYER(DUEL->unk81B)->unk178_15 ? 3 : 7;
                for (i = 0; i < 4; i++) {
                    sprintf(buf, "*s0%4d", PLAYER(DUEL->unk81B)->unk11C[i]);
                    x = panel->x;
                    y = panel->y;
                    func_80028D48(x + 0x56, i * 12 + y + 0xE, (s32)buf, (s32 *)cols[i + 2], color, z);
                }
                func_800299DC(panel->x + 0xC3, panel->y + 1, 0, PLAYER(DUEL->unk81B)->unk178_19, z);
            } else {
                sprintf(buf, "*s0%4d", card->hp);
                func_80028D48(panel->x + 0x56, panel->y + 0xE, (s32)buf, (s32 *)cols[2], 7, z);
                for (i = 0; i < 3; i++) {
                    sprintf(buf, "*s0%4d", card->attack[i].power);
                    x = panel->x;
                    y = panel->y;
                    func_80028D48(x + 0x56, i * 12 + y + 0x1A, (s32)buf, (s32 *)cols[i + 3], 7, z);
                }
                func_800299DC(panel->x + 0xC3, panel->y + 1, 0, card->attr >> 4, z);
            }
            func_80027DE8(panel->x + 0x44, panel->y + 0x40, D_8006E47C[card->unkE4], 7, cols[6], z);
            if (D_8006E4FC[card->unkE4] != 0) {
                func_800299DC(panel->x + 0x75, panel->y + 0x3B, 0, D_8006E4FC[card->unkE4] + 0x14, z);
            }
            func_80028D18(panel->x + 0x44, panel->y + 1, (s32)card->name, 7, z);
            func_800299DC(panel->x + 0xD4, panel->y + 1, 0, (card->attr & 0xF) + 0x10, z);
            if (card->unkE6 != 0) {
                func_800299DC(panel->x + 0xE3, panel->y + 2, 0, card->unkE6 + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                func_80028D48(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)card->text[i], (s32 *)cols[7], 7, z);
            }
            break;
        case 1: {
            s8 *opt;

            opt = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            func_80028D18(panel->x + 0x44, panel->y + 1, (s32)(opt + 3), 7, z);
            func_800299DC(panel->x + 0xC3, panel->y + 1, 0, 5, z);
            if (opt[0x8C] != 0) {
                func_800299DC(panel->x + 0xE3, panel->y + 2, 0, opt[0x8C] + 0x14, z);
            }
            for (i = 0; i < 4; i++) {
                func_80028D48(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(opt + 0x8D + i * 21), (s32 *)cols[8], 7,
                              z);
            }
            break;
        }
        case 2: {
            s8 *opt;

            opt = PLAYER(DUEL->unk81B)->cards[(s16)(*(s16 *)(DUEL->unk58 + 2) % 30)].card;
            func_80028D18(panel->x + 0x44, panel->y + 1, (s32)(opt + 3), 7, z);
            func_800299DC(panel->x + 0xC3, panel->y + 1, 0, 6, z);
            for (i = 0; i < 4; i++) {
                func_80028D48(panel->x + 0x8E, panel->y + 0x10 + i * 12, (s32)(opt + 0x1B + i * 21), (s32 *)cols[9], 7,
                              z);
            }
            break;
        }
        }
        break;
    }
    case 1:
    case 7:
        card = (CardInfo *)PLAYER(p)->cards[func_80040764(p) % 30].card;
        color = PLAYER(p)->unk178_15 ? 3 : 7;
        func_80028D18(panel->x + 4, panel->y + 1, (s32)card->name, 6, z);
        sprintf(buf, "*s0%4d", PLAYER(p)->unk11C[0]);
        func_80028D18(panel->x + 0x82, panel->y + 1, (s32)buf, color, z);
        func_80029A0C(panel->x + 0xA4, panel->y + 2, 0, (card->attr & 0xF) + 0x10, rgb[0], z);
        func_80029A0C(panel->x + 0xB6, panel->y + 2, 0, PLAYER(p)->unk178_19, rgb[0], z);
        for (k = 0; k < 3; k++) {
            func_80028D18(panel->x + 0x25, panel->y + 13 + k * 12, (s32)card->attack[k].name, 7, z);
            sprintf(buf, "*s0%4d", PLAYER(p)->unk15C[k]);
            func_80028D18(panel->x + 0xA2, panel->y + 13 + k * 12, (s32)buf, color, z);
        }
        func_80028D18(panel->x + 0x47, panel->y + 0x32, (s32)D_8006E4BC[card->unkE4], 7, z);
        if (D_8006E4FC[card->unkE4] != 0) {
            func_800299DC(panel->x + 0x95, panel->y + 0x32, 0, D_8006E4FC[card->unkE4] + 0x14, z);
        }
        if (PLAYER(p)->unk178_2 != 3) {
            if (PLAYER(p)->unk178_4 != PLAYER(p)->unk178_2) {
                PLAYER(p)->unk16E = 0;
            }
            PLAYER(p)->unk178_4 = PLAYER(p)->unk178_2;
            if (PLAYER(p)->unk16E < 28) {
                PLAYER(p)->unk16E++;
                panel->clut = getClut(784, p * 8 + 0x1F0 + PLAYER(p)->unk16E / 4);
            } else {
                panel->clut = getClut(784, p * 8 + 0x1F7);
            }
        } else {
            PLAYER(p)->unk178_4 = 3;
            panel->clut = getClut(784, p * 8 + 0x1F0);
        }
        rival = DUEL->unk817 != p;
        func_80044504(panel->x + 0xA7, panel->y + 0x32, rival, 0x80, z);
        break;
    }
}

void func_8003B210(s32 c, s32 p) {
    CardAnim *a;

    a = (CardAnim *)(D_801D833C + c * 36);
    a->spr->flags |= 0x80;
    switch (SPRITE_KIND(c)) {
    case 0:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x80 + p * 0xBE;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x54 + p * 0xE;
        a->spr->pos.vz = 0;
        UNK7F8(c).rx = 0x2000;
        UNK7F8(c).ry = 0x2800;
        UNK7F8(c).rz = 0x1C00;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        a->count = 0;
        break;
    case 1:
    case 21:
    case 26:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
        break;
    case 2:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x80 + p * 0xBE);
            ty = (s16)(SLOT(p, 0x90)->y - 0x54 + p * 0xE);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x1C00;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state = 0;
        }
        break;
    case 3:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
    case 4:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B);
            ty = (s16)(SLOT(p, 0x90)->y - 0x69 + p * 0x21);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            if (a->count != 0) {
                break;
            }
        }
        a->total = 4;
        a->count = 4;
        a->state++;
        func_8002B498(0xA7);
        break;
    case 5:
    case 13:
        if (--a->count == 0) {
            ANIM_SAVE(a);
            a->total = 0xE;
            a->count = 0xC;
            a->state++;
        }
        break;
    case 6:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B);
            ty = (s16)(SLOT(p, 0x90)->y - 0x61 + p * 0x11);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 7:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x5C + p * -10 + a->unk23 * 0x2B;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x61 + p * 0x11;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        break;
    case 8:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
    case 9:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x90)->x - 0x80 + p * 0xBE);
            ty = (s16)(SLOT(p, 0x90)->y - 0x6C + p * 0xE);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2400;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 10:
        a->spr->pos.vx = SLOT(p, 0x90)->x - 0x80 + p * 0xBE;
        a->spr->pos.vy = SLOT(p, 0x90)->y - 0x6C + p * 0xE;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2400;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x90)->unkC;
        break;
    case 11:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        func_8002B498(0xA5);
        a->state++;
    case 12: {
        s32 n;
        s32 i;

            n = 0;
            if (a->count != 0) {
                s32 tx;
                s32 ty;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 sc;

                for (i = 2; i >= 0; i--) {
                    if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                        break;
                    }
                    n++;
                }
                tx = (s16)(SLOT(p, 0x48)->x + (s16)(n * 2 - 0x46) + (s16)((-0x40 - (n * 2 + 8) * 2) * p + 8));
                ty = (s16)(SLOT(p, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                sc = 0x1000;
                ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            } else {
                func_8002B498(0xA7);
                a->total = 4;
                a->count = 4;
                a->state++;
            }
            break;
    }
    case 14: {
        s32 n;
        s32 i;

            n = 0;
            if (a->count != 0) {
                s32 tx;
                s32 ty;
                s16 rx;
                s16 ry;
                s16 rz;
                s16 sc;

                for (i = 2; i >= 0; i--) {
                    if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                        break;
                    }
                    n++;
                }
                tx = (s16)(SLOT(p, 0x48)->x + (s16)(n * 2 - 0x46) + (s16)((-0x40 - n * 4) * p));
                ty = (s16)(SLOT(p, 0x48)->y - 0x54);
                rx = 0x2000;
                ry = 0x2000;
                rz = 0x2000;
                sc = 0x1000;
                ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
            } else {
                a->state++;
                func_8002B498(0xA7);
            }
            break;
    }
    case 15: {
        s32 n;
        s32 i;

            n = 0;
            for (i = 2; i >= 0; i--) {
                if (c == ((Player *)D_801D8348[p])->unk1CA[i]) {
                    break;
                }
                n++;
            }
            a->spr->pos.vx = SLOT(p, 0x48)->x - 0x46 + n * 2 + (-0x40 - n * 4) * p;
            a->spr->pos.vy = SLOT(p, 0x48)->y - 0x54;
            a->spr->pos.vz = 0;
            a->spr->rot.vx = 0x2000;
            a->spr->rot.vy = 0x2000;
            a->spr->rot.vz = 0x2000;
            a->spr->scale = 0x1000;
            a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x48)->unkC;
            break;
    }
    case 16:
        ANIM_SAVE(a);
        a->total = 0x10;
        a->count = 0x10;
        if (a->spr->rot.vy == 0x2000) {
            func_8002B498(0xA5);
        } else {
            func_8002B498(0xA6);
        }
        a->state++;
        break;
    case 17:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x50 + p * -0x3E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            func_8002B498(0xA7);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 18:
        if (--a->count == 0) {
            ANIM_SAVE(a);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 19:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x58 + p * -0x2E);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 20:
        a->spr->pos.vx = SLOT(p, 0x6C)->x - p - 0x89;
        a->spr->pos.vy = SLOT(p, 0x6C)->y - p * 0x2E - 0x58;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x6C)->unkC;
        break;
    case 22:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x50 + p * -0x3E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->total = 0x20;
            a->count = 0x20;
            a->state++;
        }
        break;
    case 23:
        if (a->count != 0) {
            a->count--;
            ANIM_SAVE(a);
            a->total = 4;
            a->count = 4;
            a->state++;
        }
        break;
    case 24:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x6C)->x - 0x89 + p * -1);
            ty = (s16)(SLOT(p, 0x6C)->y - 0x58 + p * -0x2E);
            rx = 0x2000;
            ry = 0x2800;
            rz = 0x2000;
            sc = 0x1000;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_8002B498(0xA7);
        }
        break;
    case 25:
        a->spr->pos.vx = SLOT(p, 0x6C)->x - p - 0x89;
        a->spr->pos.vy = SLOT(p, 0x6C)->y - p * 0x2E - 0x58;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2800;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x1000;
        break;
    case 27:
        if (a->count != 0) {
            s32 tx;
            s32 ty;
            s16 rx;
            s16 ry;
            s16 rz;
            s16 sc;

            tx = (s16)(SLOT(p, 0x48)->x - 0x94 + p * 0x5D);
            ty = (s16)(SLOT(p, 0x48)->y - 0x54);
            rx = 0x2000;
            ry = 0x2000;
            rz = 0x2000;
            sc = 0x800;
            ANIM_STEP(a, tx, ty, rx, ry, rz, sc);
        } else {
            a->state++;
            func_80039220(p);
            func_8002B498(0xA7);
        }
        break;
    case 28:
        a->spr->pos.vx = SLOT(p, 0x48)->x - 0x94 + p * 0x5D;
        a->spr->pos.vy = SLOT(p, 0x48)->y - 0x54;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x800;
        a->spr->flags = (a->spr->flags & 0x7F) | SLOT(p, 0x48)->unkC;
        break;
    case 29:
        ANIM_SAVE(a);
        a->total = 0x20;
        a->count = 0x20;
        a->state++;
        break;
    case 30:
        if (a->count != 0) {
            s32 ty;
            s16 r;

            ty = (s16)(0x3C - p * 0x78);
            r = 0x2000;
            ANIM_STEP(a, 0, ty, r, r, r, r);
        } else {
            a->state++;
        }
        break;
    case 31:
        a->spr->pos.vx = 0;
        a->spr->pos.vy = 0x3C - p * 0x78;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2000;
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x2000;
        break;
    case 32:
        ANIM_SAVE(a);
        a->total = 0x20;
        a->count = 0x20;
        func_8002B498(0xA6);
        a->state++;
        break;
    case 33:
        if (a->count != 0) {
            s32 ty;
            s16 ry;

            s16 r;

            ty = (s16)(0xA0 - p * 0x140);
            r = 0x2000;
            ry = 0x2800 - (p << 12);
            ANIM_STEP(a, 0, ty, r, ry, r, r);
        } else {
            a->state++;
        }
        break;
    case 34:
        a->spr->pos.vx = 0;
        a->spr->pos.vy = 0xA0 - p * 0x140;
        a->spr->pos.vz = 0;
        a->spr->rot.vx = 0x2000;
        a->spr->rot.vy = 0x2800 - (p << 12);
        a->spr->rot.vz = 0x2000;
        a->spr->scale = 0x2000;
        break;
    }
}

void func_8003D4C4(void) {
    char buf[8];
    Rect16 rect;
    u8 rgb[4] = "@@@";
    s32 i;
    s32 j;
    s32 done;
    s8 c;
    s32 color;
    s32 z;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 30; j++) {
            func_8003B210(i * 30 + j, i);
        }
    }
    func_80044800();
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 8; j++) {
            c = PLAYER(i)->unk1C2[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
                if (SPRITE_KIND(c) == 0x1C) {
                    break;
                }
            }
        }
        c = PLAYER(i)->unk1CD;
        if (c >= 0) {
            func_80044AB0(SPRITE(c), c);
        }
        done = 0;
        for (j = 0; j < 3; j++) {
            c = PLAYER(i)->unk1CA[j];
            if (c >= 0) {
                if (!done) {
                    ((u8 *)SPRITE(c))[0x14] = PLAYER(i)->unk178_19;
                    done = 1;
                    if (SPRITE_KIND(c) < 0x1D) {
                        color = PLAYER(i)->unk178_15 ? 3 : 7;
                        func_8004480C(SPRITE(c), c);
                        z = *(s32 *)((u8 *)SPRITE(c) + 0x38);
                        func_800299DC(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 2, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30, 0,
                                      0x1A, z);
                        sprintf(buf, "%4d", PLAYER(i)->unk126[0]);
                        func_80028D18(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 15, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30,
                                      (s32)buf, color, z);
                        rect.x = 0x60;
                        rect.y = 0xDB;
                        rect.w = 0x26;
                        rect.h = 0xC;
                        func_80027228(*(s16 *)((u8 *)SPRITE(c) + 0x34) + 1, *(s16 *)((u8 *)SPRITE(c) + 0x36) + 30,
                                      &rect, rgb, getTPage(0, 2, D_801D6B12, D_801D6B14), 0xC, z);
                        done = 1;
                    }
                }
                func_80044AB0(SPRITE(c), c);
            }
        }
        for (j = 0; j < 30; j++) {
            c = PLAYER(i)->unk19B[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
                if (SPRITE_KIND(c) == 10) {
                    break;
                }
            }
        }
        for (j = 3; j >= 0; j--) {
            c = PLAYER(i)->unk1B9[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
            }
        }
        for (j = 0; j < 30; j++) {
            c = PLAYER(i)->unk17D[j];
            if (c >= 0) {
                func_80044AB0(SPRITE(c), c);
                if (SPRITE_KIND(c) == 0) {
                    break;
                }
            }
        }
    }
}

/* the original file padded its strings with an empty word here */
__asm__(".section .rodata\n\t.word 0\n\t.section .text\n");

void func_8003D9C0(Panel *p, s16 x, s16 y, s32 speed) {
    if (speed == 0) {
        speed = 1;
    }
    p->unkC |= 0x80;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        p->unk18 = p->unk10 - p->parent->unk10;
        p->unk1A = p->unk12 - p->parent->unk12;
    } else {
        p->unk18 = p->unk10;
        p->unk1A = p->unk12;
    }
    p->unk14 = x;
    p->unk16 = y;
    p->unkE = speed;
    p->unkF = speed;
    p->unkD++;
}

s32 func_8003DA64(Panel *p) {
    s16 px;
    s16 py;

    px = 0;
    py = 0;
    p->unkF--;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        px = p->parent->unk10;
        py = p->parent->unk12;
    }
    p->unk10 = px + (p->unk14 - (p->unk14 - p->unk18) * p->unkF / p->unkE);
    p->unk12 = py + (p->unk16 - (p->unk16 - p->unk1A) * p->unkF / p->unkE);
    if (p->unkF == 0) {
        p->unkD++;
    }
    return p->unkF;
}

void func_8003DB64(Panel *p) {
    s16 x;
    s16 y;

    x = p->unk14;
    y = p->unk16;
    if (p->parent != 0) {
        p->unkC = p->parent->unkC;
        x += p->parent->unk10;
        y += p->parent->unk12;
    }
    p->unk10 = x;
    p->unk12 = y;
}

void func_8003DBBC(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x90);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x20;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * -0x12F + 0xF0;
        break;
    case 1:
        (D_801D83EC + arg0 * 0xD8)[0x55] = 1;
        (D_801D83EC + arg0 * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)p + 0xD)) += 1;
        break;
    case 2:
        func_8003D9C0(p, 0x20, -(arg0 * 0x7D) + 0x99, 0x10);
        break;
    case 3:
        func_8003DA64(p);
        break;
    case 4:
        func_8003DB64(p);
        break;
    case 7:
        (D_801D83EC + arg0 * 0xD8)[0x55] = 6;
        (D_801D83EC + arg0 * 0xD8)[0xD] = 5;
        (*(u8 *)((s8 *)p + 0xD)) = 2;
        break;
    case 11:
        func_8003D9C0(p, 0xE8, -(arg0 * 0x7D) + 0x99, 0x10);
        break;
    case 12:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DD9C(s32 arg0) {
    void *p;
    s32 y;

    p = D_801D83EC + (arg0 * 0xD8 + 0xB4);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x164;
        y = 0x31 - arg0 * 0x31;
        (*(s16 *)((s8 *)p + 0x12)) = y;
        func_8003D9C0(p, 0x164, y, 0);
        func_8003DA64(p);
        (*(u8 *)((s8 *)p + 0xD)) = 0;
        break;
    case 1:
        func_8003D9C0(p, 0x100, 0x31 - arg0 * 0x31, 8);
        break;
    case 2:
        if (func_8003DA64(p) == 0) {
            func_8002B498(0xA7);
        }
        break;
    case 3:
        func_8003D9C0(p, 0xF9, 0x31 - arg0 * 0x31, 8);
        break;
    case 4:
        func_8003DA64(p);
        break;
    case 5:
        func_8003DB64(p);
        break;
    case 6:
        func_8003D9C0(p, 0x164, 0x31 - arg0 * 0x31, 8);
        break;
    case 7:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003DF48(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x48);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = arg0 * 0x2A0 - 0xEC;
        (*(s16 *)((s8 *)p + 0x12)) = 0x5C;
        break;
    case 1:
        func_8003D9C0(p, arg0 * 0x7C + 0x28, 0x5C, 0x10);
        (D_801D83EC + arg0 * 0xD8)[0x79] = 4;
        break;
    case 2:
        func_8003DA64(p);
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, arg0 * 0x2A0 - 0xEC, 0x5C, 0x10);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    case 6:
        func_8003D9C0(p, arg0 * 0xCC, 0x5C, 0x10);
        break;
    case 7:
        if (func_8003DA64(p) == 0) {
            (D_801D83EC + arg0 * 0xD8)[0x79] = 1;
        }
        break;
    case 8:
        func_8003DB64(p);
        break;
    }
}

void func_8003E11C(s32 arg0) {
    void *p;
    s32 x;
    s32 y;

    p = D_801D83EC + (arg0 * 0xD8 + 0x6C);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        x = -(arg0 * 0x41) + 0x44;
        (*(s16 *)((s8 *)p + 0x10)) = x;
        y = arg0 * 0x1E + 0xA;
        (*(s16 *)((s8 *)p + 0x12)) = y;
        func_8003D9C0(p, x, y, 0);
        func_8003DA64(p);
        (*(u8 *)((s8 *)p + 0xD)) = 0;
        break;
    case 1:
        func_8003D9C0(p, -(arg0 * 0xA1) + 0x74, arg0 * 0x1E + 0xA, 8);
        break;
    case 2:
        if (func_8003DA64(p) == 0) {
            func_8002B498(0xA7);
        }
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, -(arg0 * 0x41) + 0x44, arg0 * 0x1E + 0xA, 8);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E298(s32 arg0) {
    void *p;

    p = D_801D83EC + (arg0 * 0xD8 + 0x24);
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = 0x38;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * -0x12F + 0xF0;
        break;
    case 1:
        func_8003D9C0(p, 0x38, arg0 * -0x7F + 0x99, 0x10);
        break;
    case 2:
        func_8003DA64(p);
        break;
    case 3:
        func_8003DB64(p);
        break;
    case 4:
        func_8003D9C0(p, 0x38, arg0 * -0x12F + 0xF0, 0x10);
        break;
    case 5:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E3C8(s32 arg0) {
    void *p;

    p = D_801D83EC + arg0 * 0xD8;
    switch ((*(u8 *)((s8 *)p + 0xD))) {
    case 0:
        (*(u8 *)((s8 *)p + 0xC)) &= 0x7F;
        (*(s16 *)((s8 *)p + 0x10)) = -0xFF;
        (*(s16 *)((s8 *)p + 0x12)) = arg0 * 0x7E + 0x16;
        break;
    case 1:
        func_8003D9C0(p, 0x22, 0x16, 0xC);
        (*(u8 *)((s8 *)p + 0xD)) = 3;
        break;
    case 2:
        func_8003D9C0(p, 0x22, 0x94, 0xA);
        (*(u8 *)((s8 *)p + 0xD)) = 3;
        break;
    case 3:
        func_8003DA64(p);
        break;
    case 4:
        func_8003DB64(p);
        break;
    case 5:
        func_8003D9C0(p, -0xFF, (*(s16 *)((s8 *)p + 0x12)), 0xC);
        break;
    case 6:
        if (func_8003DA64(p) == 0) {
            (*(u8 *)((s8 *)p + 0xD)) = 0;
        }
        break;
    }
}

void func_8003E4F0(void) {
    s32 p;
    s32 i;
    s32 d;
    s32 step;
    s32 count;

    for (i = 0; i < 2; i++) {
        func_8003DBBC(i);
        func_8003DD9C(i);
        func_8003E298(i);
        func_8003DF48(i);
        func_8003E11C(i);
        func_8003E3C8(i);
    }
    for (p = 0; p < 2; p++) {
        PLAYER(p)->unk11C[4] = func_8004110C(p);
        if (func_80040764(p) == -1) {
            for (i = 0; i < 4; i++) {
                PLAYER(p)->unk126[i] = 0;
                PLAYER(p)->unk11C[i] = 0;
            }
        }
        for (i = 0; i < 5; i++) {
            d = PLAYER(p)->unk126[i] - PLAYER(p)->unk11C[i];
            step = (d < 0 ? -d : d) / 16 + 1;
            if (PLAYER(p)->unk126[i] < PLAYER(p)->unk11C[i]) {
                PLAYER(p)->unk126[i] += step;
                if (PLAYER(p)->unk126[i] > PLAYER(p)->unk11C[i]) {
                    PLAYER(p)->unk126[i] = PLAYER(p)->unk11C[i];
                }
            } else if (PLAYER(p)->unk126[i] > PLAYER(p)->unk11C[i]) {
                PLAYER(p)->unk126[i] -= step;
                if (PLAYER(p)->unk126[i] < PLAYER(p)->unk11C[i]) {
                    PLAYER(p)->unk126[i] = PLAYER(p)->unk11C[i];
                }
            }
        }
    }
    count = 0;
    for (p = 0; p < 2; p++) {
        for (i = 0; i < 5; i++) {
            if (PLAYER(p)->unk126[i] != PLAYER(p)->unk11C[i]) {
                count++;
            }
        }
    }
    if (count != 0 && !(((Unk8006E050 *)D_8006E050)->unk24 & 3)) {
        func_8002B498(0xAA);
    }
    func_800395A0();
    for (i = 0; i < 12; i++) {
        if (PANEL(i).flags & 0x80) {
            func_8004269C((SprtInfo *)&PANEL(i), i, i * 2 + PANEL(i).z + 1);
            func_80039730(i, i * 2 + PANEL(i).z);
        }
    }
}

void func_8003E844(s32 arg0) {
    void *p;

    D_801D833C = p = func_8001AD0C(0x870);
    D_801D8340 = p = func_8001AD0C(0x86C);
    (*(s32 *)((s8 *)D_801D8340 + 0x7F8)) = func_801F8854();
    (*(s8 *)((s8 *)D_801D8340 + 0x817)) = (s8) (rand() % 2);
    (*(s8 *)((s8 *)D_801D8340 + 0x818)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81B)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x81C)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x810)) = -1;
    (*(s8 *)((s8 *)D_801D8340 + 0x81F)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x825)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x823)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x822)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x824)) = 0;
    (*(s8 *)((s8 *)D_801D8340 + 0x820)) = 0;
    func_801F8200();
    func_8003FB3C(arg0);
    (*(s8 *)((s8 *)D_801D8340 + 0x81D)) = -1;
}

void func_8003E94C(void) {
    Unk800794F8 *p;

    func_80024460(0);
    func_800149B8(0x19, -1, 0, 0x800, &func_800250F4, 0);
    func_80014C08(2);
    p = (Unk800794F8 *)&D_800794F8;
    p->unk54 = 0;
    p->unk56 = 0;
    p->unk58 = 0;
    p->unk7C = 0;
    p->unk80 = 0;
    p->unk84 = 0;
    p->unk8E = 0;
    p->unk90 = 0x1C0;
    p->unk92 = 0;
    p->unk94 = 0;
    p->unk8C = -1;
    p->unk74 = 1;
    (*(s8 *)((s8 *)D_801D8340 + 0x811)) = 0;
    func_80014C08(2);
}

void func_8003E9F4(s32 arg0) {
    s32 var_a0;
    s32 var_v1;

    (*(s32 *)((s8 *)D_801D8340 + 0x58)) = func_801F8998(0, 0x26, 0x2E, 0xA, 1);
    func_800149B8(0x1E, -1, 0, 0x800, &func_80034260, 0, 0, 0, 0);
    if ((arg0 != 0) && ((*(s8 *)((s8 *)D_801D8340 + 0x81F)) == 0)) {
        func_800149B8(0, -1, 0, 0x800, func_80038F68, 0, 0, 0, 0);
    }
    func_800149B8(0, -1, 0, 0x800, &func_80041E00, 0, 0, 0, 0);
    if (arg0 != 0) {
        var_a0 = (*(u8 *)((s8 *)D_8006E054 + 0x72));
        var_v1 = (*(u8 *)((s8 *)D_8006E054 + 0x71));
    } else {
        var_a0 = -1;
        var_v1 = -1;
    }
    func_800149B8(0, -1, 0, 0x1000, &func_8002E034, var_a0, var_v1, 0, 0);
}

void func_8003EB50(void) {
    func_80014A00(0x19);
    func_801F848C();
    func_801F88E8();
    func_8001AFF0(0x7F);
}

void func_8003EB88(void) {
    s16 temp_a0;

    temp_a0 = (*(s16 *)((s8 *)D_801D8340 + 0x808));
    if (temp_a0 != 0) {
        func_80042824(temp_a0);
        func_80043D00((*(s16 *)((s8 *)D_801D8340 + 0x808)));
        func_80044074((*(s16 *)((s8 *)D_801D8340 + 0x808)));
    }
    func_80042E78();
    func_8003E4F0();
    func_8003D4C4();
    if ((*(s32 *)((s8 *)D_801D8340 + 0x83C)) == 0) {
        if ((*(s32 *)((s8 *)D_801D8340 + 0x828)) != -1) {
            func_801F97F4();
        }
        func_801EB53C(D_801D83D1);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80011350);

INCLUDE_ASM("asm/main/nonmatchings/sound", func_8003EC4C);

void func_8003F9EC(s32 player) {
    SavedDeck *decks;
    SavedDeck *d;
    s32 i;
    s32 k;
    u16 id;

    decks = (SavedDeck *)(((Unk8006E054 *)D_8006E054)->unk0 + 8);
    if (((Unk8006E054 *)D_8006E054)->unk1008[player] != -1) {
        d = &decks[((Unk8006E054 *)D_8006E054)->unk1008[player]];
        func_80047248(player);
        strcpy(D_801D8348[player] + 1, d->name);
        for (i = 0; i < 30; i++) {
            id = d->cards[i];
            func_80046BAC(D_801D8348[player] + 0x14 + i * 8, id);
            k = func_80047A58(id);
            if (k >= 0) {
                func_80047620(player, k, 0);
                if (d->unk6D != 0) {
                    func_80047C38(player, k, d->unk6D - 1);
                }
            }
        }
        func_80046A38(player, (Unk110 *)D_801D8348[player]);
    }
}

void func_8003FB3C(s32 arg) {
    s32 i;
    s32 j;
    s32 k;
    u16 id;

    for (i = 0; i < 2; i++) {
        D_801D8348[i] = func_8001AD0C(0x1E4);
        PLAYER(i)->unk178_17 = (1 - arg) * 2 + i;
        PLAYER(i)->unk0[0] = 1;
        for (j = 0; j < 30; j++) {
            PLAYER(i)->cards[j].id = 0;
            PLAYER(i)->cards[j].state = 0;
            PLAYER(i)->cards[j].unk1 = 0;
            PLAYER(i)->unk17D[j] = i * 30 + j;
            PLAYER(i)->unk19B[j] = -1;
        }
        for (j = 0; j < 4; j++) {
            PLAYER(i)->unk1B9[j] = -1;
        }
        for (j = 0; j < 8; j++) {
            PLAYER(i)->unk1C2[j] = -1;
        }
        for (j = 0; j < 3; j++) {
            PLAYER(i)->unk1CA[j] = -1;
        }
        PLAYER(i)->unk1CD = -1;
        PLAYER(i)->unk17C = 0;
        *(s32 *)(D_801D8348[i] + 0x114) = 0;
        for (j = 0; j < 5; j++) {
            PLAYER(i)->unk11C[j] = 0;
            PLAYER(i)->unk126[j] = 0;
            PLAYER(i)->unk130[j].value = 0;
            PLAYER(i)->unk130[j].type = 0;
            PLAYER(i)->unk130[j].timer = 0;
            PLAYER(i)->unk130[j].x = 0;
            PLAYER(i)->unk130[j].y = 0;
        }
    }
    if (arg != 0) {
        strcpy((char *)D_801D8348[0] + 0x1CE, (char *)D_8006E050);
        strcpy((char *)D_801D8348[1] + 0x1CE, (char *)D_8006E054 + 0x57);
        if (((Unk8006E054 *)D_8006E054)->unk4 == 0) {
            func_801EA708();
            for (i = 0; i < 2; i++) {
                func_80046A38(i, D_801D8348[i]);
            }
        } else {
            func_8003F9EC(0);
            for (i = 0; i < 3; i++) {
                PLAYER_DATA(1).unk80[i].unk288 = 0;
            }
            PLAYER(1)->unk178_22 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[0];
            PLAYER(1)->unk178_24 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[1];
            PLAYER(1)->unk178_26 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[2];
            PLAYER(1)->unk178_28 = ((Unk8006E054 *)D_8006E054)->unk8.unk64[3];
            strcpy((char *)D_801D8348[1] + 1, ((Unk8006E054 *)D_8006E054)->unk8.name);
            for (i = 0; i < 30; i++) {
                id = ((Unk8006E054 *)D_8006E054)->unk8.cards[i];
                func_80046BAC(D_801D8348[1] + 0x14 + i * 8, id);
                k = func_80047A58(id);
                if (k >= 0) {
                    func_80047620(1, k, 0);
                    if (((Unk8006E054 *)D_8006E054)->unk8.unk6D != 0) {
                        func_80047C38(1, k, ((Unk8006E054 *)D_8006E054)->unk8.unk6D - 1);
                    }
                }
            }
            func_80046A38(1, D_801D8348[1]);
        }
    } else {
        for (i = 0; i < 2; i++) {
            strcpy((char *)D_801D8348[i] + 0x1CE, PLAYER_DATA(i).name);
        }
    }
}

s32 func_80040064(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x19B;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    v = -1;
end:
    return v;
}

s32 func_800400B4(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 29; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x19B] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x19B;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040124(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x19B;
    do {
        if (p[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 func_8004017C(s32 arg0) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 30; i++) {
        base = (s8 *)D_801D8348[arg0] + 0x19B;
        p = base + i;
        if (*p != -1) {
            s32 v = *p;
            *p = -1;
            return v;
        }
    }
    return -1;
}

s32 func_800401D0(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x17D;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 30);
    v = -1;
end:
    return v;
}

s32 func_80040220(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x17D;
    do {
        if (p[i] != -1) {
            count++;
        }
        i++;
    } while (i < 30);
    return count;
}

s32 func_80040278(s32 arg0) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 30; i++) {
        base = (s8 *)D_801D8348[arg0] + 0x17D;
        p = base + i;
        if (*p != -1) {
            s32 v = *p;
            *p = -1;
            return v;
        }
    }
    return -1;
}

s32 func_800402CC(s32 p) {
    s32 i;
    s32 j;
    s32 c;

    for (i = 0; i < 30; i++) {
        if (PLAYER(p)->unk17D[i] != -1) {
            c = PLAYER(p)->unk17D[i];
            if (func_80047B84(p, PLAYER(p)->cards[c % 30].id) >= 0) {
                for (j = i; j > 0; j--) {
                    PLAYER(p)->unk17D[j] = PLAYER(p)->unk17D[j - 1];
                }
                PLAYER(p)->unk17D[0] = -1;
                return c;
            }
        }
    }
    return -1;
}

s32 func_800403F8(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 29; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x17D] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x17D;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040468(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1B9;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 4);
    return count;
}

s32 func_800404C0(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 4; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1B9;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return i;
        }
    }
    return -1;
}

s32 func_80040518(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 4; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1B9;
        p = base + i;
        if (*p == arg0) {
            *p = -1;
            return i;
        }
    }
    return -1;
}

s32 func_80040570(s32 player) {
    Player *p;
    s32 i;
    s32 c;

    i = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 4; i++) {
        c = p->unk1B9[i];
        if (c != -1 && p->cards[c % 30].state == 0) {
            return 0;
        }
    }
    return -1;
}

s32 func_80040614(s32 player) {
    Player *p;
    s32 i;
    s32 c;

    i = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 4; i++) {
        c = p->unk1B9[i];
        if (c != -1 && p->cards[c % 30].state == 1) {
            return 0;
        }
    }
    return -1;
}

s32 func_800406BC(s32 player) {
    Player *p;
    s32 i;
    s32 c;

    i = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 4; i++) {
        c = p->unk1B9[i];
        if (c != -1 && p->cards[c % 30].state == 2) {
            return 0;
        }
    }
    return -1;
}

s32 func_80040764(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1CA;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 3);
    v = -1;
end:
    return v;
}

s32 func_800407B4(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1CA;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 3);
    return count;
}

s32 func_8004080C(s32 idx, s32 p) {
    s8 *card;
    s32 shift;
    s32 k;

    if (idx == -1) {
        return -1;
    }
    card = PLAYER(p)->cards[idx % 30].card;
    shift = PLAYER(p)->unk178_15 - 1;
    if (shift < 0) {
        shift = 0;
    }
    for (k = 2; k >= 0; k--) {
        if (PLAYER(p)->unk1CA[k] == -1 || PLAYER(p)->unk1CA[k] == idx) {
            PLAYER(p)->unk1CA[k] = idx;
            PLAYER(p)->unk178_19 = ((u8)card[0x1A] >> 4);
            PLAYER(p)->unk11C[0] = (*(s16 *)(card + 0x1E) >> shift) / 10 * 10;
            PLAYER(p)->unk15C[0] = (*(s16 *)(card + 0x20) >> shift) / 10 * 10;
            PLAYER(p)->unk15C[1] = (*(s16 *)(card + 0x3C) >> shift) / 10 * 10;
            PLAYER(p)->unk15C[2] = (*(s16 *)(card + 0x58) >> shift) / 10 * 10;
            PLAYER(p)->unk11C[1] = PLAYER(p)->unk15C[0];
            PLAYER(p)->unk11C[2] = PLAYER(p)->unk15C[1];
            PLAYER(p)->unk11C[3] = PLAYER(p)->unk15C[2];
            PLAYER(p)->unk178_30 = 0;
            return 0;
        }
    }
    return -1;
}

s32 func_80040A48(s32 p, s32 deck) {
    Rect16 r;
    Rect16 unused;
    s8 *card;
    s32 c;
    s32 k;

    if (deck == -1) {
        return -1;
    }
    c = func_80040764(p);
    func_80046BAC(&PLAYER(p)->cards[c % 30], PLAYER_DATA(p).unk80[deck].unk292[0]);
    card = (s8 *)&PLAYER_DATA(p).unk80[deck] + 0x13C;
    PLAYER(p)->cards[c % 30].card = card;
    r.x = ((p << 8) + (deck + 3) * 40 >> 1) + 0x2C0;
    r.y = 0xC8;
    r.w = 0x14;
    r.h = 0x28;
    MoveImage2(&r, ((p << 8) + c % 30 % 6 * 40 >> 1) + 0x2C0, c % 30 / 6 * 40);
    for (k = 0; k < 3; k++) {
        if (PLAYER(p)->unk1CA[k] == c) {
            PLAYER(p)->unk178_19 = (u8)card[0x1A] >> 4;
            PLAYER(p)->unk11C[0] = *(s16 *)(card + 0x1E);
            PLAYER(p)->unk15C[0] = *(s16 *)(card + 0x20);
            PLAYER(p)->unk15C[1] = *(s16 *)(card + 0x3C);
            PLAYER(p)->unk15C[2] = *(s16 *)(card + 0x58);
            PLAYER(p)->unk11C[1] = PLAYER(p)->unk15C[0];
            PLAYER(p)->unk11C[2] = PLAYER(p)->unk15C[1];
            PLAYER(p)->unk11C[3] = PLAYER(p)->unk15C[2];
            PLAYER(p)->unk178_30 = 0;
            PLAYER(p)->unk170[0] = *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + c * 60 + 0x10);
            *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + c * 60 + 0x10) = PLAYER(p)->unk170[deck + 1];
            return 0;
        }
    }
    return -1;
}

s32 func_80040D88(s32 p, s32 deck) {
    Rect16 r;
    Rect16 unused;
    s8 *card;
    s32 c;
    s32 k;

    if (deck == -1) {
        return -1;
    }
    c = func_80040764(p);
    func_80046BAC(&PLAYER(p)->cards[c % 30], PLAYER_DATA(p).unk80[deck].unk288);
    card = (s8 *)&PLAYER_DATA(p).unk80[deck];
    PLAYER(p)->cards[c % 30].card = card;
    r.x = ((p << 8) + deck * 40 >> 1) + 0x2C0;
    r.y = 0xC8;
    r.w = 0x14;
    r.h = 0x28;
    MoveImage2(&r, ((p << 8) + c % 30 % 6 * 40 >> 1) + 0x2C0, c % 30 / 6 * 40);
    for (k = 0; k < 3; k++) {
        if (PLAYER(p)->unk1CA[k] == c) {
            PLAYER(p)->unk178_19 = (u8)card[0x1A] >> 4;
            PLAYER(p)->unk11C[0] = *(s16 *)(card + 0x1E);
            PLAYER(p)->unk15C[0] = *(s16 *)(card + 0x20);
            PLAYER(p)->unk15C[1] = *(s16 *)(card + 0x3C);
            PLAYER(p)->unk15C[2] = *(s16 *)(card + 0x58);
            PLAYER(p)->unk11C[1] = PLAYER(p)->unk15C[0];
            PLAYER(p)->unk11C[2] = PLAYER(p)->unk15C[1];
            PLAYER(p)->unk11C[3] = PLAYER(p)->unk15C[2];
            PLAYER(p)->unk178_30 = 0;
            *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x7F8) + c * 60 + 0x10) = PLAYER(p)->unk170[0];
            return 0;
        }
    }
    return -1;
}

s32 func_800410B4(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 3; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1CA;
        p = base + i;
        if (*p == arg0) {
            *p = -1;
            return 0;
        }
    }
    return -1;
}

s32 func_8004110C(s32 player) {
    Player *p;
    s32 i;
    s32 sum;
    s32 c;

    i = 0;
    sum = 0;
    p = (Player *)D_801D8348[player];
    for (; i < 8; i++) {
        c = p->unk1C2[i];
        if (c != -1) {
            sum += p->cards[c % 30].card[0x1C];
        }
    }
    if (sum > 90) {
        sum = 90;
    }
    return sum;
}

s32 func_800411C4(s32 arg0) {
    s32 i;
    s8 *p;
    s32 v;

    i = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1C2;
    do {
        v = p[i];
        if (v != -1) {
            goto end;
        }
        i++;
    } while (i < 8);
    v = -1;
end:
    return v;
}

s32 func_80041214(s32 arg0) {
    s32 i;
    s32 count;
    s8 *p;

    i = 0;
    count = 0;
    p = (s8 *)D_801D8348[arg0] + 0x1C2;
    do {
        if (p[i] == -1) {
            count++;
        }
        i++;
    } while (i < 8);
    return count;
}

s32 func_8004126C(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 7; i >= 0; i--) {
        if ((((s8 *)D_801D8348[arg1]) + i)[0x1C2] == arg0) {
            return -1;
        }
        base = (s8 *)D_801D8348[arg1] + 0x1C2;
        p = base + i;
        if (*p == -1) {
            *p = arg0;
            return 0;
        }
    }
    return -1;
}

s32 func_800412DC(s32 arg0, s32 arg1) {
    s32 i;
    s8 *base;
    s8 *p;

    for (i = 0; i < 8; i++) {
        base = (s8 *)D_801D8348[arg1] + 0x1C2;
        p = base + i;
        if (*p != -1 && *p == arg0) {
            *p = -1;
            return 0;
        }
    }
    return -1;
}

s32 func_80041340(s32 arg0) {
    return ((s8 *)D_801D8348[arg0])[0x1CD];
}

s32 func_80041364(s32 arg0) {
    return ((s8 *)D_801D8348[arg0])[0x1CD] == -1;
}

s32 func_80041390(s32 id, s32 player) {
    if ((s8)D_801D8348[player][0x1CD] == id) {
        return -1;
    }
    if ((s8)D_801D8348[player][0x1CD] == -1) {
        D_801D8348[player][0x1CD] = id;
        return 0;
    }
    return -1;
}

s32 func_80041408(s32 arg0) {
    s8 *p = (s8 *)D_801D8348[arg0];
    s32 v = p[0x1CD];

    p[0x1CD] = -1;
    return v;
}

void func_80041430(s32 player) {
    s32 n;
    s32 k;
    s32 i;
    s32 j;
    s8 t;

    n = func_80040220(player);
    if (n >= 2) {
        for (k = 0; k < ((Player *)D_801D8348[player])->unk11A; k++) {
            for (i = 30 - n; i < 30; i++) {
                j = rand() % n + (30 - n);
                t = ((Player *)D_801D8348[player])->unk17D[i];
                ((Player *)D_801D8348[player])->unk17D[i] = ((Player *)D_801D8348[player])->unk17D[j];
                ((Player *)D_801D8348[player])->unk17D[j] = t;
            }
        }
        ((Player *)D_801D8348[player])->unk11A = 0;
    }
}

void func_80041584(s32 player) {
    s32 n;
    s32 k;
    s32 i;
    s32 j;
    s8 t;

    n = func_80040124(player);
    if (n >= 2) {
        for (k = 0; k < ((Player *)D_801D8348[player])->unk11A; k++) {
            for (i = 30 - n; i < 30; i++) {
                j = rand() % n + (30 - n);
                t = ((Player *)D_801D8348[player])->unk19B[i];
                ((Player *)D_801D8348[player])->unk19B[i] = ((Player *)D_801D8348[player])->unk19B[j];
                ((Player *)D_801D8348[player])->unk19B[j] = t;
            }
        }
        ((Player *)D_801D8348[player])->unk11A = 0;
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800113C0);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800113D0);

void func_800416D8(s32 n) {
    u8 *buf;
    SavedDeck *decks;
    s32 r;

    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_800113C0, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, func_8001B144, &D_800113D0, func_800148B0());
    buf = (u8 *)func_80014C08(0x7FFFFFFF);
    ((Unk8006E054 *)D_8006E054)->unk0 = buf;
    decks = (SavedDeck *)(buf + 8);
    ((Unk8006E054 *)D_8006E054)->unk4 = n;
    ((Unk8006E054 *)D_8006E054)->unk8 = decks[n];
    func_800149B8(0, -1, 0, 0x800, func_8003EC4C, 1, func_800148B0(), 0, 0);
    r = func_80014C08(0x7FFFFFFF);
    if (*((s8 *)D_801D8340 + 0x81F) == 0) {
        if (r != 0) {
            if (++PLAYER_DATA(0).unk1A >= 1000) {
                PLAYER_DATA(0).unk1A = 999;
            }
        } else {
            if (++PLAYER_DATA(0).unk18 >= 1000) {
                PLAYER_DATA(0).unk18 = 999;
            }
        }
        func_8002CC44(0);
    }
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\saiseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    ((u8 *)((Unk8006E054 *)D_8006E054)->unk100C)[0x1A6] = r;
    func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, func_800148B0(), 0, 0);
}

void func_80041A1C(void) {
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, &D_800113C0, D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, func_8001B144, &D_800113D0, func_800148B0());
    ((Unk8006E054 *)D_8006E054)->unk0 = (u8 *)func_80014C08(0x7FFFFFFF);
    ((Unk8006E054 *)D_8006E054)->unk1010[0x12] = 0;
    func_800149B8(0, -1, 0, 0x800, func_8003EC4C, 0, func_800148B0(), 0, 0);
    if (func_80014C08(0x7FFFFFFF) != 0) {
        if (++PLAYER_DATA(0).unk1E >= 1000) {
            PLAYER_DATA(0).unk1E = 999;
        }
        if (++PLAYER_DATA(1).unk1C >= 1000) {
            PLAYER_DATA(1).unk1C = 999;
        }
    } else {
        if (++PLAYER_DATA(0).unk1C >= 1000) {
            PLAYER_DATA(0).unk1C = 999;
        }
        if (++PLAYER_DATA(1).unk1E >= 1000) {
            PLAYER_DATA(1).unk1E = 999;
        }
    }
    func_8002CC44(0);
    func_8002CC44(1);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\openseg.bin", D_80010C9C, func_800148B0());
    func_80014C08(0x7FFFFFFF);
    func_80014C08(2);
    func_800149B8(0, -1, 0, 0x800, D_801EB2E8, func_800148B0(), 0, 0, 0);
}

void func_80041CA8(u8 *s, s32 row, s32 arg2) {
    char path[64]; /* unused, but it is in the original stack frame */
    u8 *arc;
    s32 i;

    D_8006E294 = 1;
    D_801D8344 = 0;
    func_800149B8(0, -1, 0, 0x800, &func_8001B144, "B:\\FONT.ARC", func_800148B0());
    arc = (u8 *)func_80014C08(0x7FFFFFFF);
    for (i = 0; *s != 0;) {
        func_8001B438((u32 *)(arc + ((s32 *)arc)[*s - 0x20]), i * 4 + 0x2C0, (row << 5) + 0x1C0, 0x2F0,
                      row + 0x1D7);
        DrawSync(0);
        s++;
        func_80014C08(D_800794F0);
        if (++i >= 12) {
            break;
        }
    }
    func_8001AE90(arc);
    D_8006E294 = 0;
    func_80014A48(arg2);
}

void func_80041E00(void) {
    char path[72];
    u32 *tim;
    s32 i;
    s16 k;
    s32 id;

    D_801D8350 = -1;
    DUEL->unk812 = 0;
    DUEL->unk826 = 0;
    for (i = 0; i < 6; i++) {
        DUEL->cache[i].id = -1;
        DUEL->cache[i].used = 0;
        DUEL->cache[i].age = 100;
    }
    for (;;) {
        s32 slot;

        func_80014C08(D_800794F0);
        slot = DUEL->unk826 % 6;
        DUEL->cache[slot].used = 0;
        if (DUEL->unk812 != 0) {
            break;
        }
        if (DUEL->unk81C == -1 || DUEL->unk81C == 4) {
            continue;
        }
        k = *(s16 *)(DUEL->unk58 + 2);
        if (k == -1) {
            continue;
        }
        if (SPRITE_KIND(k) == 0x19) {
            continue;
        }
        if (k != D_801D8350) {
            id = PLAYER(DUEL->unk81B)->cards[k % 30].id;
            if (DUEL->unk811 != 0) {
                continue;
            }
            DUEL->cache[slot].used = 0;
            D_801D8350 = *(s16 *)(DUEL->unk58 + 2);
            if (DUEL->cache[slot].id != id) {
                DUEL->unk811 = 1;
                DUEL->cache[slot].id = id;
                sprintf(path, "B:\\CARD\\LC%3.3d.TIM", id);
                func_800149B8(0, -1, 0, 0x800, func_8001B144, path, func_800148B0());
                tim = (u32 *)func_80014C08(0x7FFFFFFF);
                func_8001B438(tim, slot % 2 * 32 + 0x280, slot / 2 * 64 + 0x140, 0, 0x1FF - slot);
                DrawSync(0);
                func_8001AE90(tim);
                DUEL->unk811 = 0;
            }
            DUEL->cache[slot].used = 1;
            for (i = 0; i < 6; i++) {
                if (DUEL->cache[i].age != 0) {
                    DUEL->cache[i].age--;
                }
            }
            DUEL->cache[slot].age = 100;
        } else {
            DUEL->cache[slot].used = 1;
        }
    }
    DUEL->unk812 = 0;
}

INCLUDE_ASM("asm/main/nonmatchings/sound", func_80042174);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80011440);

void func_8004269C(SprtInfo *info, s32 arg1, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = info->x;
        CUR_SPRT->sp.y0 = info->y;
        CUR_SPRT->sp.u0 = info->u;
        CUR_SPRT->sp.v0 = info->v;
        CUR_SPRT->sp.clut = info->clut;
        CUR_SPRT->sp.w = info->w;
        CUR_SPRT->sp.h = info->h;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = info->r;
        CUR_SPRT->sp.g0 = info->g;
        CUR_SPRT->sp.b0 = info->b;
        setDrawMode(&CUR_SPRT->dm, 0, 0, info->tpage);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_80042824(s32 c) {
    POLY_FT4 *p;
    u8 *buf;

    buf = (u8 *)D_800793A0->unk4078[11];
    p = (POLY_FT4 *)(buf + 0x1E0);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0;
    p->y0 = 0xB;
    p->x1 = 0xA0;
    p->y1 = 0xB;
    p->x2 = 0;
    p->y2 = 0x7A;
    p->x3 = 0xA0;
    p->y3 = 0x7A;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
    p = (POLY_FT4 *)(buf + 0x208);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0x13F;
    p->y0 = 0xB;
    p->x1 = 0x9F;
    p->y1 = 0xB;
    p->x2 = 0x13F;
    p->y2 = 0x7A;
    p->x3 = 0x9F;
    p->y3 = 0x7A;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
    p = (POLY_FT4 *)(buf + 0x230);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0;
    p->y0 = 0xE8;
    p->x1 = 0xA0;
    p->y1 = 0xE8;
    p->x2 = 0;
    p->y2 = 0x79;
    p->x3 = 0xA0;
    p->y3 = 0x79;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
    p = (POLY_FT4 *)(buf + 0x258);
    func_8001E6EC(0xC, p, 0, 0);
    p->r0 = c;
    p->g0 = c;
    p->b0 = c;
    p->u0 = 0;
    p->v0 = 0x47;
    p->u1 = 0xA0;
    p->v1 = 0x47;
    p->u2 = 0;
    p->v2 = 0xB6;
    p->u3 = 0xA0;
    p->v3 = 0xB6;
    p->x0 = 0x13F;
    p->y0 = 0xE8;
    p->x1 = 0x9F;
    p->y1 = 0xE8;
    p->x2 = 0x13F;
    p->y2 = 0x79;
    p->x3 = 0x9F;
    p->y3 = 0x79;
    p->tpage = 0x1C;
    p->clut = 0x7C33;
    addPrim(&D_800793A0->ot[0xFFF], p);
}

void func_80042BBC(s32 x, s32 y, s32 z, s32 n, u8 *tex) {
    POLY_FT4 *p;
    s32 u;

    p = (POLY_FT4 *)((u8 *)D_800793A0->unk4078[11] + (n * 80 + 0x280));
    u = ((((Unk8006E050 *)D_8006E050)->unk24 / 4) % 4) * 32;
    func_8001E6EC(0xC, p, 1, 0);
    p->r0 = 0x80;
    p->g0 = 0x80;
    p->b0 = 0x80;
    p->u0 = u;
    p->v0 = 0x40;
    p->u1 = u + 0x20;
    p->v1 = 0x40;
    p->u2 = u;
    p->v2 = 0x80;
    p->u3 = u + 0x20;
    p->v3 = 0x80;
    p->x0 = x;
    p->y0 = y;
    p->x1 = x + 0x40;
    p->y1 = y;
    p->x2 = x;
    p->y2 = y + 0x40;
    p->x3 = x + 0x40;
    p->y3 = y + 0x40;
    p->tpage = 0x1E;
    p->clut = 0x7FB0;
    addPrim(&D_800793A0->ot[z], p);
    if (tex != 0) {
        p++;
        func_8001E6EC(0xC, p, 1, 0);
        p->r0 = 0x80;
        p->g0 = 0x80;
        p->b0 = 0x80;
        p->u0 = tex[0x16];
        p->v0 = tex[0x17];
        p->u1 = tex[0x16] + 0x28;
        p->v1 = tex[0x17];
        p->u2 = tex[0x16];
        p->v2 = tex[0x17] + 0x27;
        p->u3 = tex[0x16] + 0x28;
        p->v3 = tex[0x17] + 0x27;
        p->x0 = x;
        p->y0 = y;
        p->x1 = x + 0x40;
        p->y1 = y;
        p->x2 = x;
        p->y2 = y + 0x40;
        p->x3 = x + 0x40;
        p->y3 = y + 0x40;
        p->tpage = *(u16 *)(tex + 0x12);
        p->clut = *(u16 *)(tex + 0x10);
        addPrim(&D_800793A0->ot[z], p);
    }
}

void func_80042E78(void) {
    POLY_FT4 *p;
    s32 i;
    s32 d;
    s32 c;
    s8 k;
    s8 m;
    u8 n;

    m = D_801D83D0.unk3;
    if (m == -1) {
        return;
    }
    n = D_801D83D0.unk1;
    k = D_8006E2E4[D_801D83D0.next];
    if (D_801D83D0.unkC != n || D_801D83D0.unkB != k || D_801D83D0.unkD != m) {
        D_801D83D0.unk0 = 0;
        D_801D83D0.unkE = 0;
        D_801D83D0.unk10 = 0;
        D_801D83D0.px = 0x154;
        D_801D83D0.py = 0x66;
        D_801D83D0.unkC = n;
        D_801D83D0.unkB = k;
        D_801D83D0.unkD = m;
    }
    p = (POLY_FT4 *)(D_800793A0->unk4078[11] + 0x320);
    switch ((u8)D_801D83D0.unk0) {
    case 0:
        D_801D83D0.px -= 14;
        if (D_801D83D0.px < 0x5B) {
            D_801D83D0.px = 0x5A;
            D_801D83D0.unk0++;
        }
        break;
    case 1:
        D_801D83D0.unkE += 2;
        for (i = 0; i < 6; i++) {
            d = D_801D83D0.unkE - i * 3;
            c = 0x100 - d * 20;
            if (c >= 0) {
                func_8001E6EC(0xC, p, 1, 0);
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = 0xD0;
                p->v0 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100;
                p->u1 = 0xFF;
                p->v1 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100;
                p->u2 = 0xD0;
                p->v2 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100 + 12;
                p->u3 = 0xFF;
                p->v3 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100 + 12;
                p->x0 = D_801D83D0.px - d * 2;
                p->y0 = D_801D83D0.py - d * 2;
                p->x1 = D_801D83D0.px + 0x30;
                p->y1 = D_801D83D0.py - d * 2;
                p->x2 = D_801D83D0.px - d * 2;
                p->y2 = D_801D83D0.py + 12;
                p->x3 = D_801D83D0.px + 0x30;
                p->y3 = D_801D83D0.py + 12;
                p->tpage = 0x3E;
                p->clut = 0x7CB3;
                addPrim(&D_800793A0->ot[0x1E], p);
                p++;
                func_8001E6EC(0xC, p, 1, 0);
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = 0xD0;
                p->v0 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100;
                p->u1 = 0xFC;
                p->v1 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100;
                p->u2 = 0xD0;
                p->v2 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100 + 0x18;
                p->u3 = 0xFC;
                p->v3 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100 + 0x18;
                p->x0 = D_801D83D0.px - (s16)(d * 2 - 10);
                p->y0 = D_801D83D0.py - (s16)(d - 8);
                p->x1 = (s16)(D_801D83D0.px - (s16)(d * 2 - 10) + 0x2C) + d * 2;
                p->y1 = D_801D83D0.py - (s16)(d - 8);
                p->x2 = D_801D83D0.px - (s16)(d * 2 - 10);
                p->y2 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->x3 = (s16)(D_801D83D0.px - (s16)(d * 2 - 10) + 0x2C) + d * 2;
                p->y3 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->tpage = 0x3E;
                p->clut = 0x7CB3;
                addPrim(&D_800793A0->ot[0x1E], p);
                p++;
                func_8001E6EC(0xC, p, 1, 0);
                p->r0 = c;
                p->g0 = c;
                p->b0 = c;
                p->u0 = 0xA0;
                p->v0 = 0xA0;
                p->u1 = 0xF0;
                p->v1 = 0xA0;
                p->u2 = 0xA0;
                p->v2 = 0xB8;
                p->u3 = 0xF0;
                p->v3 = 0xB8;
                p->x0 = D_801D83D0.px + 0x30;
                p->y0 = D_801D83D0.py - (s16)(d - 8);
                p->x1 = D_801D83D0.px + 0x80 + d * 2;
                p->y1 = D_801D83D0.py - (s16)(d - 8);
                p->x2 = D_801D83D0.px + 0x30;
                p->y2 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->x3 = D_801D83D0.px + 0x80 + d * 2;
                p->y3 = (s16)(D_801D83D0.py - (s16)(d - 8) + 0x18) + d * 2;
                p->tpage = 0x3C;
                p->clut = 0x7CB3;
                addPrim(&D_800793A0->ot[0x1E], p);
                p++;
            }
        }
        if (++D_801D83D0.unk10 > 0x10) {
            D_801D83D0.unk0++;
        }
        break;
    case 2:
        D_801D83D0.unkE = 0;
        D_801D83D0.unk10 = 0;
        D_801D83D0.tx = (D_801D83D0.unk1 % 2) * -170 + 0xB8;
        D_801D83D0.ty = (D_801D83D0.unk1 % 2) * -136 + 0xA8;
        D_801D83D0.unk0++;
        break;
    case 3:
        D_801D83D0.unk10++;
        D_801D83D0.px = (D_801D83D0.tx - 0x5A) * D_801D83D0.unk10 / 8 + 0x5A;
        D_801D83D0.py = (D_801D83D0.ty - 0x66) * D_801D83D0.unk10 / 8 + 0x66;
        if (D_801D83D0.unk10 >= 8) {
            D_801D83D0.unk0++;
        }
        break;
    case 4:
        D_801D83D0.px = D_801D83D0.tx;
        D_801D83D0.py = D_801D83D0.ty;
        break;
    }
    if (D_8006E2E4[D_801D83D0.next] != -1) {
        if (func_80029990() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = D_801D83D0.px + 0xE;
        CUR_SPRT->sp.y0 = D_801D83D0.py + 0x18;
        CUR_SPRT->sp.u0 = D_8006E2E4[D_801D83D0.next] / 4 * 100;
        CUR_SPRT->sp.v0 = ((s8)(D_8006E2E4[D_801D83D0.next] % 4) * 14 + 0x1B8) % 0x100;
        CUR_SPRT->sp.clut = 0x7DF3;
        CUR_SPRT->sp.w = 100;
        CUR_SPRT->sp.h = 14;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
    if (func_80029990() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = D_801D83D0.px;
    CUR_SPRT->sp.y0 = D_801D83D0.py;
    CUR_SPRT->sp.u0 = 0xD0;
    CUR_SPRT->sp.v0 = (D_801D83D0.unk1 * 12 + 0x100) % 0x100;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x2F;
    CUR_SPRT->sp.h = 12;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
    if (func_80029990() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = D_801D83D0.px + 10;
    CUR_SPRT->sp.y0 = D_801D83D0.py + 8;
    CUR_SPRT->sp.u0 = 0xD0;
    CUR_SPRT->sp.v0 = (D_801D83D0.unk3 * 24 + 0x130) % 0x100;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x2C;
    CUR_SPRT->sp.h = 0x18;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
    if (func_80029990() != 0) {
        return;
    }
    CUR_SPRT->sp.x0 = D_801D83D0.px + 0x30;
    CUR_SPRT->sp.y0 = D_801D83D0.py + 8;
    CUR_SPRT->sp.u0 = 0xA0;
    CUR_SPRT->sp.v0 = 0xA0;
    CUR_SPRT->sp.clut = 0x7C73;
    CUR_SPRT->sp.w = 0x50;
    CUR_SPRT->sp.h = 0x18;
    setSemiTrans(&CUR_SPRT->sp, 1);
    CUR_SPRT->sp.r0 = 0x80;
    CUR_SPRT->sp.g0 = 0x80;
    CUR_SPRT->sp.b0 = 0x80;
    setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->sp);
    addPrim(&D_800793A0->ot[0x1E], &CUR_SPRT->dm);
    D_801D6B24 += sizeof(SprtPacket);
}

void func_80043D00(s32 c) {
    DISPENV env;
    Rect16 r;
    u8 rgb[4];
    u8 buf[0x48];
    u8 *s;
    u8 *d;
    s32 i;

    if (D_801D83D0.next == -1) {
        return;
    }
    rgb[0] = c;
    rgb[1] = c;
    rgb[2] = c;
    GetDispEnv(&env);
    SetDrawArea(&D_801D8358[D_800794F4], (Rect16 *)env.disp);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D8358[D_800794F4]);
    if (D_801D83D0.cur != D_801D83D0.next) {
        if (++D_801D83D0.y > 0x10) {
            D_801D83D0.cur = D_801D83D0.next;
            D_801D83D0.player = ((u8 *)D_801D8340)[0x817];
        }
    } else if (D_801D83D0.y != 0) {
        D_801D83D0.y--;
    }
    if (D_801D83D0.cur != -1) {
        s = D_8006E29C[D_801D83D0.cur];
        d = buf;
        do {
            if (*s < 0x81 || *s >= 0x99) {
                if (*s == '*' && s[1] == 'P') {
                    s += 2;
                    i = *s++ - '0';
                    i ^= D_801D83D0.player;
                    *d = 0;
                    strcpy((char *)d, (char *)D_801D8348[i] + 0x1CE);
                    d += strlen(D_801D8348[i] + 0x1CE);
                    continue;
                }
            } else {
                *d++ = *s++;
            }
            *d++ = *s++;
        } while (s[-1] != 0);
        func_80028D48(0x10, D_801D83D0.y + 0xE, (s32)buf, (s32 *)rgb, 7, 0xFFE);
    }
    r.x = env.disp[0] + 0x10;
    r.y = env.disp[1] + 0xE;
    r.w = 0x120;
    r.h = 0xC;
    SetDrawArea(&D_801D8378[D_800794F4], &r);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D8378[D_800794F4]);
}

void func_80044074(s32 c) {
    DISPENV env;
    Rect16 r;
    u8 rgb[4];

    if (D_801D83D0.next2 == -1) {
        return;
    }
    rgb[0] = c;
    rgb[1] = c;
    rgb[2] = c;
    GetDispEnv(&env);
    SetDrawArea(&D_801D8398[D_800794F4], (Rect16 *)env.disp);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D8398[D_800794F4]);
    if (D_801D83D0.cur2 != D_801D83D0.next2 || D_801D83D0.unkA != D_801D83D0.unk1) {
        if (++D_801D83D0.y2 > 0x10) {
            D_801D83D0.cur2 = D_801D83D0.next2;
            D_801D83D0.unkA = D_801D83D0.unk1;
        }
    } else if (D_801D83D0.y2 != 0) {
        D_801D83D0.y2--;
    }
    if (D_801D83D0.cur2 != -1) {
        if (func_80029990() != 0) {
            return;
        }
        CUR_SPRT->sp.x0 = 0x10;
        CUR_SPRT->sp.y0 = 0xDB - D_801D83D0.y2;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = (D_801D83D0.unkA * 12 + 0x100) % 256;
        CUR_SPRT->sp.clut = 0x7C73;
        CUR_SPRT->sp.w = 0x2F;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = c;
        CUR_SPRT->sp.g0 = c;
        CUR_SPRT->sp.b0 = c;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&D_800793A0->ot[0xFFE], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[0xFFE], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
        if (D_801D83D0.unkA == 1 && D_801D83D0.cur2 != 2 && D_801D83D0.cur2 != 0) {
            func_80028D48(0x50, 0xDB - D_801D83D0.y2, (s32)D_8001174C, (s32 *)rgb, 7, 0xFFE);
        } else {
            func_80028D48(0x40, 0xDB - D_801D83D0.y2, (s32)D_8006E2F8[D_801D83D0.cur2], (s32 *)rgb, 7, 0xFFE);
        }
    }
    r.x = env.disp[0] + 0x10;
    r.y = env.disp[1] + 0xDB;
    r.w = 0x120;
    r.h = 0xC;
    SetDrawArea(&D_801D83B8[D_800794F4], &r);
    addPrim(&D_800793A0->ot[0xFFE], &D_801D83B8[D_800794F4]);
}

void func_80044504(s32 x, s32 y, s32 n, s32 c, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = (n * 12 + 0x153) % 256;
        CUR_SPRT->sp.clut = getClut(0x300, n + 0x1FC);
        CUR_SPRT->sp.w = 0x18;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = c;
        CUR_SPRT->sp.g0 = c;
        CUR_SPRT->sp.b0 = c;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_800446A4(s32 x, s32 y, s32 z) {
    if (func_80029990() == 0) {
        CUR_SPRT->sp.x0 = x;
        CUR_SPRT->sp.y0 = y;
        CUR_SPRT->sp.u0 = 0xD0;
        CUR_SPRT->sp.v0 = 0x47;
        CUR_SPRT->sp.clut = 0x7EF0;
        CUR_SPRT->sp.w = 0x20;
        CUR_SPRT->sp.h = 0xC;
        setSemiTrans(&CUR_SPRT->sp, 0);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1C);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[z], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
}

void func_80044800(void) {
    D_801D83F0 = 0;
}

void func_8004480C(void *arg0, s32 k) {
    u8 *o;
    MATRIX m;
    SVECTOR v[4];
    s32 sxy[4];
    s32 p;
    s32 otz;
    s32 flag;

    o = arg0;
    if (!(o[0x15] & 0x80)) {
        return;
    }
    PushMatrix();
    func_80045700((VECTOR *)(o + 0x18), (SVECTOR *)(o + 0x28), &m);
    CompMatrix((MATRIX *)((u8 *)D_801D6A4C + 0x78), &m, &m);
    SetRotMatrix((s32)&m);
    func_8005C444(&m);
    v[0].vx = -(*(s32 *)(o + 0x30) * 40) / 8192;
    v[0].vy = -(*(s32 *)(o + 0x30) * 48) / 8192;
    v[0].vz = 0;
    v[1].vx = (*(s32 *)(o + 0x30) * 40) / 8192;
    v[1].vy = -(*(s32 *)(o + 0x30) * 48) / 8192;
    v[1].vz = 0;
    v[2].vx = -(*(s32 *)(o + 0x30) * 40) / 8192;
    v[2].vy = (*(s32 *)(o + 0x30) * 48) / 8192;
    v[2].vz = 0;
    v[3].vx = (*(s32 *)(o + 0x30) * 40) / 8192;
    v[3].vy = (*(s32 *)(o + 0x30) * 48) / 8192;
    v[3].vz = 0;
    RotAverageNclip4((s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], (s32)&sxy[0], (s32)&sxy[1], (s32)&sxy[2],
                     (s32)&sxy[3], &p, &otz, &flag);
    *(s32 *)(o + 0x38) = 0x57 - *(s16 *)(D_801D833C + k * 36 + 0x20);
    if (*((s8 *)D_801D8340 + 0x81C) >= 0 && k == *(s16 *)(*(u8 **)((u8 *)D_801D8340 + 0x58) + 2)) {
        *(s32 *)(o + 0x38) = 0x33;
    }
    *(s16 *)(o + 0x34) = sxy[0];
    *(s16 *)(o + 0x36) = sxy[0] >> 16;
    PopMatrix();
}

void func_80044AB0(CardSprite *o, s32 k) {
    MATRIX m;
    SVECTOR v[4];
    SVECTOR w[4];
    s32 sxy[4];
    s32 p;
    s32 otz;
    s32 flag;
    s32 nclip;
    u32 *col;
    u32 *fade;
    RawPolyFT4 *buf;
    RawPolyFT4 *pk;
    u8 *duel;
    u8 *t;
    u16 clut;
    u16 tpage;
    u8 u0, v0, u1, v1, u2, v2, u3, v3;

    if (!(o->flags & 0x80)) {
        return;
    }
    PushMatrix();
    func_80045700(&o->pos, &o->rot, &m);
    CompMatrix((MATRIX *)((u8 *)D_801D6A4C + 0x78), &m, &m);
    SetRotMatrix((s32)&m);
    func_8005C444(&m);
    v[0].vx = -(o->scale * 40) / 8192;
    v[0].vy = -(o->scale * 48) / 8192;
    v[0].vz = 0;
    v[1].vx = (o->scale * 40) / 8192;
    v[1].vy = -(o->scale * 48) / 8192;
    v[1].vz = 0;
    v[2].vx = -(o->scale * 40) / 8192;
    v[2].vy = (o->scale * 48) / 8192;
    v[2].vz = 0;
    v[3].vx = (o->scale * 40) / 8192;
    v[3].vy = (o->scale * 48) / 8192;
    v[3].vz = 0;
    col = (u32 *)o->rgbc;
    fade = (u32 *)o->fade;
    buf = (RawPolyFT4 *)D_800793A0->unk4078[10];
    nclip = RotAverageNclip4((s32)&v[0], (s32)&v[1], (s32)&v[2], (s32)&v[3], (s32)&sxy[0], (s32)&sxy[1],
                             (s32)&sxy[2], (s32)&sxy[3], &p, &otz, &flag);
    if (nclip <= 0) {
        otz = RotAverage4(&v[1], &v[0], &v[3], &v[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
    }
    if ((o->flags & 0x20) && func_80029990() == 0) {
        CUR_SPRT->sp.x0 = sxy[0] - 10;
        CUR_SPRT->sp.y0 = (sxy[0] >> 16) + 6;
        CUR_SPRT->sp.u0 = (u8)(o->num / 5) * 60;
        CUR_SPRT->sp.v0 = (u8)(o->num % 5) * 21 - 0x80;
        CUR_SPRT->sp.clut = 0x7DF2;
        CUR_SPRT->sp.w = 60;
        CUR_SPRT->sp.h = 21;
        setSemiTrans(&CUR_SPRT->sp, 1);
        CUR_SPRT->sp.r0 = 0x80;
        CUR_SPRT->sp.g0 = 0x80;
        CUR_SPRT->sp.b0 = 0x80;
        setDrawMode(&CUR_SPRT->dm, 0, 0, 0x1E);
        addPrim(&D_800793A0->ot[0], &CUR_SPRT->sp);
        addPrim(&D_800793A0->ot[0], &CUR_SPRT->dm);
        D_801D6B24 += sizeof(SprtPacket);
    }
    o->z = 0x57 - *(s16 *)(D_801D833C + k * 36 + 0x20);
    duel = D_801D8340;
    if (*(s8 *)(duel + 0x81C) >= 0) {
        t = *(u8 **)(duel + 0x58);
        if (k == *(s16 *)(t + 2)) {
            *(CardSprite **)(t + 4) = o;
            o->z = 0x33;
            func_801F8E34(*(u8 **)(duel + 0x58), 0x33);
        }
    }
    if (o->flags & 0x40) {
        if (o->t < 16) {
            o->t++;
        } else if ((o->to[0] | o->to[1] | o->to[2]) == 0) {
            o->flags &= ~0x40;
        }
        o->fade[0] = o->from[0] + (o->to[0] - o->from[0]) * o->t / 16;
        o->fade[1] = o->from[1] + (o->to[1] - o->from[1]) * o->t / 16;
        o->fade[2] = o->from[2] + (o->to[2] - o->from[2]) * o->t / 16;
        o->sx = sxy[0];
        o->sy = sxy[0] >> 16;
        pk = &buf[D_801D83F0++];
        pk->tag = 0x09000000;
        pk->rgbc = *fade;
        pk->xy0 = sxy[0];
        pk->uv0 = 0x7DB24080;
        pk->xy1 = sxy[1];
        pk->uv1 = 0x3E40A8;
        pk->xy2 = sxy[2];
        pk->uv2 = 0x7080;
        pk->xy3 = sxy[3];
        pk->uv3 = 0x70A8;
        addPrim(&D_800793A0->ot[o->z], pk);
    }
    if (nclip <= 0) {
        otz = RotAverage4(&v[1], &v[0], &v[3], &v[2], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
        tpage = 0x1C;
        clut = 0x7C32;
        u0 = 0xA0;
        v0 = 0x6F;
        u1 = 0xC8;
        v1 = 0x6F;
        u2 = 0xA0;
        v2 = 0x9F;
        u3 = 0xC8;
        v3 = 0x9F;
    } else {
        w[0].vx = -(o->scale * 18) / 4096;
        w[0].vy = (o->scale * -19) / 4096;
        w[0].vz = 0;
        w[1].vx = (o->scale * 18) / 4096;
        w[1].vy = (o->scale * -19) / 4096;
        w[1].vz = 0;
        w[2].vx = -(o->scale * 18) / 4096;
        w[2].vy = (o->scale * 17) / 4096;
        w[2].vz = 0;
        w[3].vx = (o->scale * 18) / 4096;
        w[3].vy = (o->scale * 17) / 4096;
        w[3].vz = 0;
        u0 = o->u + 2;
        v0 = o->v + 2;
        u1 = o->u + 38;
        v1 = v0;
        u2 = u0;
        v2 = o->v + 38;
        u3 = u1;
        v3 = v2;
        otz = RotAverage4(&w[0], &w[1], &w[2], &w[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
        clut = o->clut;
        tpage = o->tpage;
        pk = &buf[D_801D83F0++];
        pk->tag = 0x09000000;
        pk->rgbc = *col;
        pk->xy0 = sxy[0];
        pk->uv0 = (clut << 16) | (v0 << 8) | u0;
        pk->xy1 = sxy[1];
        pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
        pk->xy2 = sxy[2];
        pk->uv2 = (v2 << 8) | u2;
        pk->xy3 = sxy[3];
        pk->uv3 = (v3 << 8) | u3;
        addPrim(&D_800793A0->ot[o->z], pk);
        otz = RotAverage4(&v[0], &v[1], &v[2], &v[3], &sxy[0], &sxy[1], &sxy[2], &sxy[3], &p, &flag);
        tpage = 0x1C;
        clut = getClut(800, 497 + o->pal);
        u0 = 0xCC;
        v0 = 0x6F;
        u1 = 0xF4;
        v1 = 0x6F;
        u2 = 0xCC;
        v2 = 0x9F;
        u3 = 0xF4;
        v3 = 0x9F;
    }
    o->sx = sxy[0];
    o->sy = sxy[0] >> 16;
    pk = &buf[D_801D83F0++];
    pk->tag = 0x09000000;
    pk->rgbc = *col;
    pk->xy0 = sxy[0];
    pk->uv0 = (clut << 16) | (v0 << 8) | u0;
    pk->xy1 = sxy[1];
    pk->uv1 = (tpage << 16) | (v1 << 8) | u1;
    pk->xy2 = sxy[2];
    pk->uv2 = (v2 << 8) | u2;
    pk->xy3 = sxy[3];
    pk->uv3 = (v3 << 8) | u3;
    addPrim(&D_800793A0->ot[o->z], pk);
    PopMatrix();
}

MATRIX *func_80045700(VECTOR *pos, SVECTOR *rot, MATRIX *m) {
    MATRIX tmp;
    SVECTOR r;

    r.vx = 0;
    r.vy = rot->vy;
    r.vz = 0;
    RotMatrix(&r, m);
    r.vx = rot->vx;
    r.vy = 0;
    r.vz = 0;
    RotMatrix(&r, &tmp);
    MulMatrix(m, &tmp);
    r.vx = 0;
    r.vy = 0;
    r.vz = rot->vz;
    RotMatrix(&r, &tmp);
    MulMatrix2(&tmp, m);
    MatrixNormal(m, &tmp);
    TransposeMatrix(&tmp, m);
    m->t[0] = pos->vx;
    m->t[1] = pos->vy;
    m->t[2] = pos->vz;
    return m;
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_8001174C);

void func_800457FC(void) {
    u8 *hdr;
    s32 i;
    s32 n;

    func_800149B8(0, -1, 0, 0x800, func_8001B248, "B:\\CARD2.CDD", func_800148B0(), -2);
    D_801D840C = hdr = (u8 *)func_80014C08(0x7FFFFFFF);
    D_801D8408 = hdr + 8;
    D_801D8400 = D_801D8408 + *(u16 *)(hdr + 4) * 0x13C;
    D_801D8404 = D_801D8400 + hdr[6] * 0xE2;
    n = 0;
    for (i = 0; i < 0xBF; i++) {
        ((CardInfo *)D_801D8408)[i].id = n++;
    }
    for (i = 0; i < 0x66; i++) {
        ((Unk801D8400 *)D_801D8400)[i].id = n++;
    }
    for (i = 0; i < 8; i++) {
        ((Unk801D8404 *)D_801D8404)[i].id = n++;
    }
}

void func_80045968(s32 a, s32 row, s32 n) {
    s32 r;
    s32 i;

retry:
    r = rand();
    for (i = 0; i < n; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk15E0[row][i] == r) {
            goto retry;
        }
    }
    ((Unk8006E050 *)D_8006E050)[a].unk15E0[row][n] = r;
}

void func_80045A58(s32 arg0) {
    s32 row;
    s32 i;
    s32 *base;
    u8 *p;

    i = 0;
    do {
        base = &D_8006E050;
        row = arg0 * 0x2774 + *base + 0x14B2;
        p = (u8 *)(row + i);
        *p &= 0x7F;
        i++;
    } while (i < 0x12D);
}

void func_80045AB8(s32 arg0) {
    s32 row;
    s32 i;
    s32 *base;
    u8 *p;

    i = 0;
    do {
        base = &D_8006E050;
        row = arg0 * 0x2774 + *base + 0x14B2;
        p = (u8 *)(row + i);
        *p &= 0xDF;
        i++;
    } while (i < 0x12D);
}

s8 func_80045B18(s32 p, s32 id, s32 n) {
    s32 k;

    if (id >= 0xAC && id <= 0xBE) {
        return -3;
    }
    for (k = PLAYER_DATA(p).unk14B2[id] & 7; k < 6; k++) {
        func_80045968(p, id, k);
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 6) {
        PLAYER_DATA(p).unk14B2[id] |= 0x50;
        return -2;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 0 && !(PLAYER_DATA(p).unk14B2[id] & 0x40)) {
        PLAYER_DATA(p).unk14B2[id] |= 0x20;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) + n >= 7) {
        PLAYER_DATA(p).unk14B2[id] &= 0xF8;
        PLAYER_DATA(p).unk14B2[id] |= 0x56;
        return -1;
    }
    PLAYER_DATA(p).unk14B2[id] += n;
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 6) {
        PLAYER_DATA(p).unk14B2[id] |= 0x10;
    }
    if (((u8 *)func_80046088(id))[0x19] == 0) {
        PLAYER_DATA(p).unk14B2[id] |= 0x10;
    }
    PLAYER_DATA(p).unk14B2[id] |= 0xC0;
    func_8002CC44(p);
    return PLAYER_DATA(p).unk14B2[id] & 7;
}

s8 func_80045E1C(s32 p, s32 id, s32 n) {
    if (id >= 0xAC && id <= 0xBE) {
        return -3;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) == 0) {
        return -2;
    }
    if ((PLAYER_DATA(p).unk14B2[id] & 7) - n < 0) {
        PLAYER_DATA(p).unk14B2[id] &= 0xF8;
        return -1;
    }
    PLAYER_DATA(p).unk14B2[id] -= n;
    func_8002CC44(p);
    return PLAYER_DATA(p).unk14B2[id] & 7;
}

s32 func_80045F5C(s32 arg0, s32 arg1) {
    return (*(u8 *)((s8 *)(((arg0 * 0x2774) + D_8006E050 + arg1)) + 0x14B2)) & 7;
}

s32 func_80045F94(s32 arg0, s32 arg1) {
    switch (arg0) {
    case 0:
        return arg1;
    case 1:
        return arg1 + 0xBF;
    case 2:
        return arg1 + 0x125;
    }
    return -1;
}

s32 func_80045FE8(s32 id) {
    if (id < 0xBF) {
        return D_801D8408[id * 0x13C + 0x1A] >> 4;
    }
    if (id < 0x125) {
        return 5;
    }
    return 6;
}

s32 func_80046038(s32 id) {
    if (id < 0xBF) {
        return D_801D8408[id * 0x13C + 0x1A] & 0xF;
    }
    if (id < 0x125) {
        return 4;
    }
    return 5;
}

void *func_80046088(s32 arg0) {
    if (arg0 < 0xBF) {
        return D_801D8408 + arg0 * 0x13C;
    }
    if (arg0 < 0x125) {
        return D_801D8400 + (arg0 * 0xE2 - 0xA89E);
    }
    return D_801D8404 + (arg0 * 0x70 - 0x8030);
}

void func_80046118(s32 p) {
    s32 i;

    for (i = 0; i < 30; i++) {
        ((Unk8006E050 *)D_8006E050)[p].unk14B2[func_80045F94(((Player *)D_801D8348[p])->cards[i].state,
                                                             ((Player *)D_801D8348[p])->cards[i].unk1)] |= 0x40;
    }
}

void func_800461C0(s32 a) {
    u8 count[0x12D];
    SavedDeck *decks;
    s32 i;
    s32 j;
    s32 missing;

    decks = (SavedDeck *)(((Unk8006E054 *)D_8006E054)->unk0 + 8);
    for (i = 0; i < 0x9F; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unkAC0[i] & 0x8000) {
            for (j = 0; j < 0x12D; j++) {
                count[j] = 0;
            }
            for (j = 0; j < 30; j++) {
                count[decks[i].cards[j]]++;
            }
            missing = 0;
            for (j = 0; j < 0x12D; j++) {
                if ((((Unk8006E050 *)D_8006E050)[a].unk14B2[j] & 7) < count[j]) {
                    missing = 1;
                    break;
                }
            }
            if (!missing) {
                ((Unk8006E050 *)D_8006E050)[a].unkAC0[i] |= 0x4000;
            }
        }
    }
}

INCLUDE_ASM("asm/main/nonmatchings/sound", func_8004635C);

void func_80046864(s32 a) {
    s32 i;

    for (i = 0; i < 3; i++) {
        ((Unk8006E050 *)D_8006E050)[a].unk276E[i] =
            func_80045B18(a, ((Unk8006E050 *)D_8006E050)[a].unk2768[i], 1);
    }
}

void func_80046908(s32 i) {
    s32 j;

    ((Unk8006E050 *)D_8006E050)[i].unk12 = 0;
    for (j = 0; j < 0x12D; j++) {
        if (((Unk8006E050 *)D_8006E050)[i].unk14B2[j] & 0x40) {
            ((Unk8006E050 *)D_8006E050)[i].unk12++;
        }
    }
}

void func_800469A4(s32 a) {
    s32 i;

    for (i = 0; i < 3; i++) {
        func_80046A38(a, &((Unk8006E050 *)D_8006E050)[a].unk2438[i]);
    }
}

void func_80046A38(s32 a, Unk110 *d) {
    CardSlot *c;
    s32 i;
    s32 j;

    if (d->unk0 != 0) {
        c = d->cards;
        for (i = 0; i < 30; i++) {
            switch (c->state) {
            case 0:
                c->card = (s8 *)(D_801D8408 + c->unk1 * 0x13C);
                for (j = 0; j < 3; j++) {
                    if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 != 0 &&
                        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == c->id) {
                        c->card = (s8 *)&((Unk8006E050 *)D_8006E050)[a].unk80[j];
                        break;
                    }
                }
                break;
            case 1:
                c->card = (s8 *)(D_801D8400 + c->unk1 * 0xE2);
                break;
            case 2:
                c->card = (s8 *)(D_801D8404 + c->unk1 * 0x70);
                break;
            }
            c++;
        }
    }
}

void func_80046BAC(u8 *out, s32 id) {
    s32 type;

    type = 2;
    if (id < 0xBF) {
        type = 0;
    } else {
        id -= 0xBF;
        if (id < 0x66) {
            type = 1;
        } else {
            id -= 0x66;
        }
    }
    out[0] = type;
    out[1] = id;
    *(s16 *)(out + 2) = func_80045F94(type, id);
}

s32 func_80046C0C(s32 unused, Unk110 *deck, s32 mask) {
    s32 count;
    s32 i;
    CardInfo *info;
    s32 level;
    s32 attr;

    count = 0;
    for (i = 0; i < 30; i++) {
        switch (deck->cards[i].state) {
        case 0:
            info = (CardInfo *)(D_801D8408 + deck->cards[i].unk1 * 0x13C);
            level = info->attr & 0xF;
            attr = info->attr >> 4;
            if (mask & 0x1E00) {
                if (mask & 0x1F) {
                    if ((mask >> (level + 9)) & 1) {
                        if (!(mask & 0x20) || (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                            count++;
                        }
                    }
                } else if ((mask >> (level + 9)) & 1) {
                    count++;
                }
            } else if ((mask >> attr) & 1) {
                if (!(mask & 0x20) || (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                    count++;
                }
            } else if ((mask & 0x20) && (deck->cards[i].unk1 >= 0xAC && deck->cards[i].unk1 < 0xBF)) {
                count++;
            }
            break;
        case 1:
            if (mask & 0x40) {
                count++;
            }
            break;
        case 2:
            if (mask & 0x80) {
                count++;
            }
            break;
        }
    }
    return count;
}

s32 func_80046D68(s32 a, Unk110 *src, s32 slot) {
    Unk110 *d;
    s32 i;

    if (slot == -1) {
        for (slot = 0; slot < 3; slot++) {
            if (PLAYER_DATA(a).unk2438[slot].unk0 == 0) {
                break;
            }
        }
        if (slot >= 3) {
            return -1;
        }
    }
    d = &PLAYER_DATA(a).unk2438[slot];
    *d = *src;
    d->unk0 = 1;
    d->unk108[0]++;
    if (d->unk108[1] >= 10000) {
        d->unk108[1] = 9999;
    }
    if (d->unk108[2] >= 10000) {
        d->unk108[2] = 9999;
    }
    for (i = 0; i < 30; i++) {
        switch (d->cards[i].state) {
        case 0:
            d->cards[i].card = (s8 *)(D_801D8408 + d->cards[i].unk1 * 0x13C);
            d->cards[i].id = d->cards[i].unk1;
            break;
        case 1:
            d->cards[i].card = (s8 *)(D_801D8400 + d->cards[i].unk1 * 0xE2);
            d->cards[i].id = d->cards[i].unk1 + 0xBF;
            break;
        case 2:
            d->cards[i].card = (s8 *)(D_801D8404 + d->cards[i].unk1 * 0x70);
            d->cards[i].id = d->cards[i].unk1 + 0x125;
            break;
        }
    }
    return 0;
}

s32 func_80046FB8(s32 a, Unk110 *out, s32 i) {
    if (((Unk8006E050 *)D_8006E050)[a].unk2438[i].unk0 == 0) {
        return -1;
    }
    *out = ((Unk8006E050 *)D_8006E050)[a].unk2438[i];
    return 0;
}

s32 func_8004707C(s32 a, s32 b) {
    s32 i;

    if (((Unk8006E050 *)D_8006E050)[a].unk2438[b].unk0 == 0) {
        return -1;
    }
    ((Unk8006E050 *)D_8006E050)[a].unk2438[b].unk0 = 0;
    for (i = 0; i < 2; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk2438[i].unk0 == 0) {
            ((Unk8006E050 *)D_8006E050)[a].unk2438[i] = ((Unk8006E050 *)D_8006E050)[a].unk2438[i + 1];
            ((Unk8006E050 *)D_8006E050)[a].unk2438[i + 1].unk0 = 0;
        }
    }
    return 0;
}

s32 func_800471F4(s32 arg0) {
    s32 var_a0;

    var_a0 = arg0;
    switch (var_a0) {
    case 0x75:
    case 0x79:
    case 0x7A:
    case 0x7B:
    case 0x7C:
    case 0x7D:
    case 0x7E:
    case 0x7F:
        var_a0 = 0x72;
        break;
    case 0x80:
    case 0x81:
    case 0x82:
    case 0x83:
        var_a0 = 0x77;
        break;
    case 0x84:
    case 0x85:
    case 0x86:
    case 0x87:
    case 0x88:
    case 0x89:
    case 0x8A:
    case 0x8B:
    case 0x8D:
        var_a0 = D_8006E50C[var_a0 - 0x84];
        break;
    }
    return var_a0;
}

void func_80047248(s32 a) {
    s32 j;

    for (j = 0; j < 3; j++) {
        ((Unk8006E054 *)D_8006E054)->unk78[a][j] = ((Unk8006E050 *)D_8006E050)[a].unk80[j];
        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 = 0;
    }
}

void func_80047364(s32 a) {
    s32 j;

    for (j = 0; j < 3; j++) {
        ((Unk8006E050 *)D_8006E050)[a].unk80[j] =
            ((Unk8006E054 *)D_8006E054)->unk78[a][j];
    }
}

void func_80047438(s32 a) {
    s32 j;
    u8 id;
    u8 alt;

    for (j = 0; j < 3; j++) {
        id = ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288;
        if (id != 0) {
            if ((s8)((Unk8006E050 *)D_8006E050)[a].unk80[j].unk289 >= 0x63) {
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk289 = 0x63;
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28A = func_80049934(0x62);
            }
            ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk278 = D_801D8408 + id * 0x13C;
            alt = ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0];
            if (alt == 0) {
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk27C = D_801D8408 + id * 0x13C;
            } else {
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk27C = D_801D8408 + alt * 0x13C;
            }
            func_80048230(a, j);
        }
    }
}

void func_80047620(s32 p, s32 k, s32 flag) {
    s32 i;
    s32 j;
    s32 c;

    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(p).unk80[i].unk288 == D_8006E518[k]) {
            return;
        }
        if (PLAYER_DATA(p).unk80[i].unk288 == 0) {
            PLAYER_DATA(p).unk80[i].unk278 = D_801D8408 + D_8006E518[k] * 0x13C;
            PLAYER_DATA(p).unk80[i].unk27C = D_801D8408 + D_8006E518[k] * 0x13C;
            PLAYER_DATA(p).unk80[i].unk288 = D_8006E518[k];
            PLAYER_DATA(p).unk80[i].unk289 = 1;
            PLAYER_DATA(p).unk80[i].unk28A = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[i].unk28C[j] = -1;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[i].unk28F[j] = 0;
            }
            PLAYER_DATA(p).unk80[i].unk292[0] = 0;
            PLAYER_DATA(p).unk80[i].unk292[1] = 0;
            PLAYER_DATA(p).unk80[i].unk292[2] = 0;
            PLAYER_DATA(p).unk80[i].unk280 = 0;
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[i].unk282[j] = 0;
            }
            func_80048230(p, i);
            if (flag != 0) {
                c = PLAYER_DATA(p).unk80[i].unk288;
                PLAYER_DATA(p).unk14B2[c] = 1;
                func_80045968(p, c, 0);
                func_8002CC44(p);
                func_8004950C(p, D_8006EEFC[k]);
                PLAYER_DATA(p).unk14B2[D_8006E518[k]] |= 0xF0;
            } else {
                PLAYER_DATA(p).unk14B2[D_8006E518[k]] |= 0x50;
            }
            return;
        }
    }
}

void func_80047A38(s32 arg0, s32 arg1) {
    func_80047620(arg0, arg1, 1);
}

s32 func_80047A58(s32 arg0) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (arg0 == D_8006E518[i]) {
            return i;
        }
    }
    return -1;
}

s32 func_80047A98(s32 a, s32 b) {
    s32 i;

    if (((Unk8006E050 *)D_8006E050)[a].unk80[b].unk288 == 0) {
        return -1;
    }
    for (i = 0; i < 6; i++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[b].unk288 == D_8006E518[i]) {
            return i;
        }
    }
    return -1;
}

s32 func_80047B84(s32 a, s32 id) {
    s32 i;
    s32 j;

    for (i = 0; i < 6; i++) {
        if (id == D_8006E518[i]) {
            for (j = 0; j < 3; j++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == id) {
                    return j;
                }
            }
            return 3;
        }
    }
    return -1;
}

void func_80047C38(s32 a, s32 b, s32 c) {
    s32 j;

    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
            if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[c] != D_8006E520[b][c]) {
                ((Unk8006E050 *)D_8006E050)[a].unk14B2[D_8006E520[b][c]] |= 0x50;
                ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[c] = D_8006E520[b][c];
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == 0) {
                    func_80047E64(a, b, c);
                }
            }
            return;
        }
    }
}

s32 func_80047D5C(s32 a, s32 b) {
    s32 j;
    s32 k;
    s32 n;

    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
            k = 0;
            n = 0;
            for (; k < 3; k++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[k] != 0) {
                    n++;
                }
            }
            return n;
        }
    }
    return 0;
}

void func_80047E64(s32 a, s32 b, s32 c) {
    s32 j;
    s32 k;

    if (b != -1 && D_8006E520[b][c] != 0) {
        for (j = 0; j < 3; j++) {
            if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
                for (k = 0; k < 3; k++) {
                    if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28F[k] == D_8006E520[b][c]) {
                        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] = D_8006E520[b][c];
                        ((Unk8006E050 *)D_8006E050)[a].unk80[j].unk27C = D_801D8408 + D_8006E520[b][c] * 0x13C;
                        func_80048230(a, j);
                        return;
                    }
                }
            }
        }
    }
}

s32 func_80048014(s32 a, s32 b) {
    s32 j;
    s32 k;

    if (b == -1) {
        return -1;
    }
    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 == D_8006E518[b]) {
            if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == 0) {
                return -1;
            }
            for (k = 0; k < 3; k++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == D_8006E520[b][k]) {
                    return k;
                }
            }
        }
    }
    return -1;
}

s32 func_80048150(s32 a, s32 id) {
    s32 i;
    s32 k;
    s32 j;

    for (i = 0; i < 6; i++) {
        for (k = 0; k < 3; k++) {
            if (D_8006E520[i][k] != 0 && id == D_8006E520[i][k]) {
                for (j = 0; j < 3; j++) {
                    if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk292[0] == id) {
                        return j;
                    }
                }
                return 3;
            }
        }
    }
    return -1;
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_800119CC);

s32 func_80048230(s32 p, s32 d) {
    s32 i;
    s32 j;
    s32 k;
    s32 n;
    s32 line;
    s32 col;
    s32 ret;
    u8 *s;

    PLAYER_DATA(p).unk80[d].unk292[1] = 0;
    PLAYER_DATA(p).unk80[d].unk292[2] = 0;
    ret = 0;
    PLAYER_DATA(p).unk80[d].card[0] = *(CardInfo *)PLAYER_DATA(p).unk80[d].unk278;
    PLAYER_DATA(p).unk80[d].card[1] = *(CardInfo *)PLAYER_DATA(p).unk80[d].unk27C;
    if (PLAYER_DATA(p).unk80[d].card[0].attack[2].power == 0) {
        PLAYER_DATA(p).unk80[d].card[0].attack[2].power = 100;
    }
    if (PLAYER_DATA(p).unk80[d].card[1].attack[2].power == 0) {
        PLAYER_DATA(p).unk80[d].card[1].attack[2].power = 100;
    }
    PLAYER_DATA(p).unk80[d].card[0].hp += PLAYER_DATA(p).unk80[d].unk280;
    PLAYER_DATA(p).unk80[d].card[1].hp += PLAYER_DATA(p).unk80[d].unk280;
    for (i = 0; i < 3; i++) {
        PLAYER_DATA(p).unk80[d].card[0].attack[i].power += PLAYER_DATA(p).unk80[d].unk282[i];
        PLAYER_DATA(p).unk80[d].card[1].attack[i].power += PLAYER_DATA(p).unk80[d].unk282[i];
    }
    for (i = 0; i < 3; i++) {
        k = PLAYER_DATA(p).unk80[d].unk28C[i];
        if (k == -1) {
            continue;
        }
        switch (D_8006E9B4[k].type) {
        case 0:
            PLAYER_DATA(p).unk80[d].card[0].hp += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].hp += D_8006E9B4[k].value;
            break;
        case 1:
            PLAYER_DATA(p).unk80[d].card[0].attack[0].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[0].attack[1].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[0].attack[2].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[0].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[1].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[2].power += D_8006E9B4[k].value;
            break;
        case 2:
            PLAYER_DATA(p).unk80[d].card[0].attack[0].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[0].power += D_8006E9B4[k].value;
            break;
        case 3:
            PLAYER_DATA(p).unk80[d].card[0].attack[1].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[1].power += D_8006E9B4[k].value;
            break;
        case 4:
            PLAYER_DATA(p).unk80[d].card[0].attack[2].power += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].attack[2].power += D_8006E9B4[k].value;
            break;
        case 5:
            PLAYER_DATA(p).unk80[d].card[0].unkE4 = D_8006E9B4[k].unk1;
            PLAYER_DATA(p).unk80[d].card[1].unkE4 = D_8006E9B4[k].unk1;
            if (D_8006E9B4[k].value != 0) {
                PLAYER_DATA(p).unk80[d].card[0].attack[2].power += D_8006E9B4[k].value;
                PLAYER_DATA(p).unk80[d].card[1].attack[2].power += D_8006E9B4[k].value;
            } else {
                PLAYER_DATA(p).unk80[d].card[0].attack[2].power = 0;
                PLAYER_DATA(p).unk80[d].card[1].attack[2].power = 0;
            }
            break;
        case 6:
            PLAYER_DATA(p).unk80[d].card[0].level += D_8006E9B4[k].value;
            PLAYER_DATA(p).unk80[d].card[1].level += D_8006E9B4[k].value;
            break;
        case 7:
            for (j = 0; j < 2; j++) {
                PLAYER_DATA(p).unk80[d].card[0].unk74[j].unk0[0] = 0;
            }
            for (j = 0; j < 3; j++) {
                PLAYER_DATA(p).unk80[d].card[0].unkB4[j].unk0[0] = 0;
            }
            for (j = 0; j < 4; j++) {
                for (n = 0; n < 0x15; n++) {
                    PLAYER_DATA(p).unk80[d].card[0].text[j][n] = 0;
                    PLAYER_DATA(p).unk80[d].card[1].text[j][n] = 0;
                }
            }
            j = D_8006E9B4[k].unk1;
            if (j != 0) {
                PLAYER_DATA(p).unk80[d].card[0].unk74[0] = D_8006E534[j - 1];
                PLAYER_DATA(p).unk80[d].card[0].unk74[0].unkE = D_8006E9B4[k].value;
            }
            if (D_8006E9B4[k].unk2 != 0) {
                for (j = 0; j < D_8006E9B4[k].unk3; j++) {
                    PLAYER_DATA(p).unk80[d].card[0].unkB4[j] = D_8006E774[D_8006E9B4[k].unk2 - 1 + j];
                    PLAYER_DATA(p).unk80[d].card[0].unkB4[j].unkC = D_8006E9B4[k].value;
                }
            }
            s = D_8006EDB4[k - 0x29];
            line = 0;
            col = 0;
            while (*s != 0) {
                if (*s == '\n') {
                    line++;
                    col = 0;
                } else {
                    PLAYER_DATA(p).unk80[d].card[0].text[line][col] = *s;
                    PLAYER_DATA(p).unk80[d].card[1].text[line][col] = *s;
                    col++;
                }
                s++;
            }
            PLAYER_DATA(p).unk80[d].card[0].unkE6 = D_8006E9B4[k].unk6;
            PLAYER_DATA(p).unk80[d].card[1].unkE6 = D_8006E9B4[k].unk6;
            ret = 1;
            break;
        case 8:
            switch (D_8006E9B4[k].unk1) {
            case 0:
                PLAYER_DATA(p).unk80[d].unk292[1] += D_8006E9B4[k].value;
                break;
            case 1:
                PLAYER_DATA(p).unk80[d].unk292[2] += D_8006E9B4[k].value;
                break;
            }
            break;
        }
    }
    for (i = 0; i < 3; i++) {
        if (PLAYER_DATA(p).unk80[d].card[0].attack[i].power < 0) {
            PLAYER_DATA(p).unk80[d].card[0].attack[i].power = 0;
        }
        if (PLAYER_DATA(p).unk80[d].card[1].attack[i].power < 0) {
            PLAYER_DATA(p).unk80[d].card[1].attack[i].power = 0;
        }
    }
    if (((u8)(PLAYER_DATA(p).unk80[d].card[0].unkE4 - 5) < 4) | ((u8)(PLAYER_DATA(p).unk80[d].card[1].unkE4 - 5) < 4)) {
        if ((u8)(PLAYER_DATA(p).unk80[d].card[0].unkE4 - 5) < 4) {
            PLAYER_DATA(p).unk80[d].card[0].attack[2].power = 0;
        }
        if ((u8)(PLAYER_DATA(p).unk80[d].card[1].unkE4 - 5) < 4) {
            PLAYER_DATA(p).unk80[d].card[1].attack[2].power = 0;
        }
    }
    return ret;
}

void func_800493EC(s32 a, s32 b, s32 c, s32 v) {
    if (func_800496E4(a, v) == 1) {
        ((Unk8006E050 *)D_8006E050)[a].unk80[b].unk28C[c] = v;
        func_80048230(a, b);
    }
}

void func_8004949C(s32 arg0, s32 arg1, s32 arg2) {
    ((Unk8006E050 *)D_8006E050)[arg0].unk80[arg1].unk28C[arg2] = -1;
    func_80048230(arg0, arg1);
}

void func_8004950C(s32 a, s32 b) {
    ((Unk8006E050 *)D_8006E050)[a].unk3C[b / 8] |= 1 << (b % 8);
}

s32 func_800495B4(s32 a, s32 b, s32 skip, s32 card) {
    s32 ok;
    s32 i;
    s32 c;

    ok = 1;
    for (i = 0; i < 3; i++) {
        if (skip == i) {
            continue;
        }
        c = ((Unk8006E050 *)D_8006E050)[a].unk80[b].unk28C[i];
        if (c == -1) {
            continue;
        }
        if (D_8006E9B4[card].type == 1) {
            if (D_8006E9B4[c].type >= 1 && D_8006E9B4[c].type <= 4) {
                ok = 0;
            }
        } else if (D_8006E9B4[c].type == 1) {
            if (D_8006E9B4[card].type >= 1 && D_8006E9B4[card].type <= 4) {
                ok = 0;
            }
        } else if (D_8006E9B4[card].type == D_8006E9B4[c].type) {
            ok = 0;
        }
    }
    return ok;
}

s32 func_800496E4(s32 a, s32 id) {
    s32 j;
    s32 k;

    if (id < 0) {
        return 0;
    }
    if (!((((Unk8006E050 *)D_8006E050)[a].unk3C[id / 8] >> (id % 8)) & 1)) {
        return 0;
    }
    for (j = 0; j < 3; j++) {
        if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk288 != 0) {
            for (k = 0; k < 3; k++) {
                if (((Unk8006E050 *)D_8006E050)[a].unk80[j].unk28C[k] == id) {
                    return 2;
                }
            }
        }
    }
    return 1;
}

s32 func_80049840(Entry12 *tbl, s32 a, s32 b) {
    s8 v;
    s32 idx;
    s32 i;

    v = ((Unk8006E050 *)D_8006E050)[a].unk80[b].unk289;
    idx = func_80047A98(a, b);
    if (idx >= 0) {
        for (i = 0; i < 0x80; i++) {
            if (tbl[i].unk4[idx] == v) {
                if (func_800496E4(a, i) == 0) {
                    return i;
                }
                return -1;
            }
        }
    }
    return -1;
}

s32 func_80049934(s32 arg0) {
    arg0++;
    return (arg0 + 2) * arg0;
}

s32 func_8004994C(s32 a, s32 b) {
    if ((s8)((s8)((Unk8006E050 *)D_8006E050)[a].unk80[b].unk289 % 5) != 0) {
        return -1;
    }
    return rand() % 4;
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80012770);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80012D68);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80012DB8);

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80012DF8);

void func_80049A14(s16 *arg0) {
    s16 r[4];
    s32 x;
    s32 y;
    s16 z;
    u8 *p;

    x = arg0[0] + 1;
    y = arg0[1];
    if (D_801D8544 >= 12) {
        y -= (D_801D8544 - 11) * 7;
    }
    z = arg0[0x1D];
    if (D_801D8538 > 0 || D_801D854C != 0) {
        D_801D8538--;
    } else {
        do {
            switch (*D_801D8558) {
            case 1:
                D_801D8540 = 1;
                break;
            case 2:
                p = D_801D8558;
                D_801D8558 = p + 1;
                D_801D8538 = p[1];
                break;
            case 4:
                func_800293FC(D_80012D68);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = 0x28;
                r[1] = 0x28;
                func_80016F38((Unk80016F38 *)&D_801D84B0, (Rect16 *)r);
                func_8002BB58(3);
                break;
            case 5:
                func_800293FC(D_80012DB8);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = 0x50;
                r[1] = 0x78;
                func_80016F38((Unk80016F38 *)&D_801D84F4, (Rect16 *)r);
                func_8002BB58(3);
                break;
            case 6:
                func_800293FC(D_80012DF8);
                r[2] = (D_801D6B18 + 1) / 2 * 2;
                r[3] = (D_801D6B1C + 1) / 2 * 2;
                r[0] = (0x140 - r[2]) >> 1;
                r[1] = 0xB4 - r[3] / 2;
                func_80016F38((Unk80016F38 *)&D_801D8460, (Rect16 *)r);
                func_8002BB58(3);
                break;
            case '>':
                D_801D8540 = 1;
                goto copy;
            case '\n':
                D_801D8540 = 0;
                D_801D8538 = 20;
                D_801D8544++;
            default:
            copy:
                *D_801D8554++ = *D_801D8558;
                break;
            }
            if (*++D_801D8558 == 0) {
                D_801D854C = 1;
                break;
            }
        } while (D_801D8540 == 0 && D_801D8538 == 0);
    }
    if ((D_801D853C & 0x10) || D_801D8538 == 0) {
        *D_801D8554 = '|';
    } else {
        *D_801D8554 = ' ';
    }
    D_801D853C++;
    D_801D8554[1] = 0;
    func_80028558(x, y, D_801D8550, 4, z);
}

void func_80049DC0(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012D68, 0, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}

void func_80049E00(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012DB8, 7, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}

void func_80049E40(void *arg0) {
    func_80028D18((*(s16 *)((s8 *)arg0 + 0)), (*(s16 *)((s8 *)arg0 + 2)), &D_80012DF8, 7, (s32) (*(s16 *)((s8 *)arg0 + 0x3A)));
}

void func_80049E80(void) {
    func_800170F0((Unk80016F38 *)&D_801D8460, &func_80049E40, 0xA);
    func_800170F0((Unk80016F38 *)&D_801D84F4, &func_80049E00, 0xA);
    func_800170F0((Unk80016F38 *)&D_801D84B0, &func_80049DC0, 0xA);
    func_800170F0((Unk80016F38 *)&D_801D8410, &func_80049A14, 0xA);
}

void func_80049EF8(s32 n, s32 arg1) {
    Rect16 r;
    u8 buf[0x401];
    s32 i;
    s32 done;

    done = 0;
    D_801D8548 = n;
    D_801D8538 = 20;
    D_801D853C = 0;
    D_801D8540 = 0;
    D_801D854C = 0;
    D_801D8544 = 0;
    for (i = 0; i < 0x401; i++) {
        buf[i] = 0;
    }
    D_801D8550 = (s32)buf;
    D_801D8554 = buf;
    D_801D8558 = D_8006EF04[D_801D8548];
    r.x = 0x94;
    r.y = 0x20;
    r.w = 0xA0;
    r.h = 0x54;
    func_80016C08(&D_801D8410, &r, -1, (s16 *)-1, 8, 0x58, 0x80, 0xC);
    ((Unk80016F38 *)&D_801D8410)->unk2C = (s32)"SHELL COMMAND";
    ((Unk80016F38 *)&D_801D8410)->unk38 = 2;
    ((Unk80016F38 *)&D_801D8410)->unk39 = 8;
    func_8002BB58(3);
    func_800293FC(D_80012D68);
    r.w = (D_801D6B18 + 1) / 2 * 2;
    r.h = (D_801D6B1C + 1) / 2 * 2;
    func_80016C08(&D_801D84B0, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    func_80016F38((Unk80016F38 *)&D_801D84B0, (Rect16 *)-1);
    ((Unk80016F38 *)&D_801D84B0)->unk38 = 2;
    func_800293FC(D_80012DB8);
    r.w = (D_801D6B18 + 1) / 2 * 2;
    r.h = (D_801D6B1C + 1) / 2 * 2;
    func_80016C08((Unk80016F38 *)&D_801D84B0 + 1, &r, -1, (s16 *)-1, 0, 0x77, 0x80, 0xC);
    func_80016F38((Unk80016F38 *)&D_801D84B0 + 1, (Rect16 *)-1);
    ((Unk80016F38 *)&D_801D84B0)[1].unk38 = 2;
    func_800293FC(D_80012DF8);
    r.w = (D_801D6B18 + 1) / 2 * 2;
    r.h = (D_801D6B1C + 1) / 2 * 2;
    r.x = (0x140 - r.w) >> 1;
    r.y = 0xB4 - r.h / 2;
    func_80016C08(&D_801D8460, &r, -1, (s16 *)-1, 8, 0x15, 0x80, 8);
    ((Unk80016F38 *)&D_801D8460)->unk2C = (s32)"MESSAGE";
    ((Unk80016F38 *)&D_801D8460)->unk38 = 4;
    func_80016F38((Unk80016F38 *)&D_801D8460, (Rect16 *)-1);
    func_8001683C((s32)func_80049E80);
    do {
        func_80014C08(D_800794F0);
        if (D_801D854C != 0) {
            done = 1;
        }
    } while (done == 0);
    func_8002BB58(4);
    func_80016F38((Unk80016F38 *)&D_801D8410, (Rect16 *)-1);
    func_80016F38((Unk80016F38 *)&D_801D84B0, (Rect16 *)-1);
    func_80016F38((Unk80016F38 *)&D_801D84F4, (Rect16 *)-1);
    func_80016F38((Unk80016F38 *)&D_801D8460, (Rect16 *)-1);
    func_80014C08(20);
    func_80016878((s32)func_80049E80);
    func_80014A48(arg1);
}

void func_8004A2DC(s32 mode) {
    u8 dlg[0xB8];
    Rect16 r = { 0, 0, 480, 512 };
    s32 stack;
    s32 done;

    stack = func_800148B0();
    if (mode == 0) {
        func_8002F8E8();
        func_80014C08(10);
        ClearImage(&r, 0, 0, 0);
        DrawSync(0);
        func_80014C08(10);
        done = 0;
        func_8002B688();
        func_80014C08(10);
        func_800149B8(0, -1, 0, 0x800, func_8002B3EC, 1, stack);
        func_80014C08(0x7FFFFFFF);
        func_8001B90C(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, func_80015328, 0, 0, 0, 0);
        func_80014C08(2);
        do {
            func_800149B8(0, -1, 0, 0x600, D_801EBAFC, 8, stack, 0, 0);
            func_80014C08(0x7FFFFFFF);
            func_8002BB58(3);
            func_80019EA4(dlg,
                          "*c6 Is it OK to return to Title Screen?\n*c3(Unless you save the game now,\nyou won't be able "
                          "to continue.)",
                          1);
            func_8001A100(dlg);
            switch ((s8)dlg[0xA5]) {
            case 1:
                done = 1;
                break;
            case 0:
            case 2:
                done = 0;
                break;
            }
        } while (!done);
        func_80014C08(20);
        func_80014A48(0);
        func_80014A90();
    } else {
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, "P:\\endseg.bin", D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x800, D_801DF47C, stack, mode, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(10);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1000, func_8001B358, D_80012FAC, D_80010C9C, func_800148B0());
        func_80014C08(0x7FFFFFFF);
        func_80014C08(2);
        func_800149B8(0, -1, 0, 0x1600, D_801E4D80, 0, stack, 0, 0);
    }
}

INCLUDE_RODATA("asm/main/nonmatchings/sound", D_80012FAC);
