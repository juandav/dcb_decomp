#include "common.h"
#include "gte.h"
#include "game.h"

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

INCLUDE_ASM("asm/main/nonmatchings/card_render", func_80042174);

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_80011440);

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

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_8001174C);

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

INCLUDE_ASM("asm/main/nonmatchings/card_render", func_8004635C);

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

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_800119CC);

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

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_80012770);

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_80012D68);

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_80012DB8);

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_80012DF8);

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

INCLUDE_RODATA("asm/main/nonmatchings/card_render", D_80012FAC);
