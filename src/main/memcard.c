#include "common.h"
#include "gte.h"
#include "game.h"

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

INCLUDE_RODATA("asm/main/nonmatchings/memcard", D_800105E4);

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

INCLUDE_RODATA("asm/main/nonmatchings/memcard", D_800105FC);

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

INCLUDE_RODATA("asm/main/nonmatchings/memcard", D_800107F8);
