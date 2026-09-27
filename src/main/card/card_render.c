#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/card_render.h"
#include "dcb/battle_hud.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/memcard.h"
#include "dcb/prim_util.h"
#include "dcb/text.h"

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

INCLUDE_ASM("asm/main/nonmatchings/card/card_render", func_80042174);

INCLUDE_RODATA("asm/main/nonmatchings/card/card_render", D_80011440);

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

INCLUDE_RODATA("asm/main/nonmatchings/card/card_render", D_8001174C);
