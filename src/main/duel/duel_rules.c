#include "common.h"
#include "gte.h"
#include "game.h"

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

INCLUDE_RODATA("asm/main/nonmatchings/duel/duel_rules", D_800113C0);

INCLUDE_RODATA("asm/main/nonmatchings/duel/duel_rules", D_800113D0);
