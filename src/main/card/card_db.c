#include "common.h"
#include "gte.h"
#include "game.h"

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

INCLUDE_ASM("asm/main/nonmatchings/card/card_db", func_8004635C);

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

INCLUDE_RODATA("asm/main/nonmatchings/card/card_db", D_800119CC);

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
