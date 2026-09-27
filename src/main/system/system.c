#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/system.h"
#include "dcb/cd_file.h"
#include "dcb/effect.h"
#include "dcb/fade.h"
#include "dcb/heap.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/memcard.h"
#include "dcb/pad.h"
#include "dcb/player_data.h"
#include "dcb/sound.h"
#include "dcb/stage.h"
#include "dcb/text.h"
#include "dcb/window.h"

s32 D_8006DD4C = 1;

void func_80014CF0(void) {
    s32 i;

    if (PLAYER_PROFILES != 0) {
        for (i = 0; i < 2; i++) {
            ((Unk8006E050 *)PLAYER_PROFILES)[i].unk24++;
        }
    }
    D_800794EC++;
}

void func_80014D64(void) {
    s32 i;
    s32 j;
    POLY_FT4 *p;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            D_800793A8.sprt[i][j].tag = 0x05000000;
            D_800793A8.sprt[i][j].code = 0x66;
            D_800793A8.sprt[i][j].u0 = 0;
            D_800793A8.sprt[i][j].v0 = 0;
            D_800793A8.sprt[i][j].w = 256 - j * 192;
            D_800793A8.sprt[i][j].h = 240;
            p = &D_800793A8.poly[i][j];
            func_800677A4(p);
            p->u0 = j * 32;
            p->v0 = 0;
            p->u1 = j * 32 - 96;
            p->v1 = 0;
            p->u2 = j * 32;
            p->v2 = 240;
            p->u3 = j * 32 - 96;
            p->v3 = 240;
            setShadeTex(p, 0);
            SetSemiTrans(p, 1);
        }
    }
    SetDrawStp(&D_800793A8.stp[0], 1);
    SetDrawStp(&D_800793A8.stp[1], 0);
    D_800793A8.r = 0xA8;
    D_800793A8.g = 0xA8;
    D_800793A8.b = 0xA8;
    D_800793A8.x = 0;
    D_800793A8.y = 0;
    D_800793A8.mode = 0;
}

void func_80014EF0(void) {
    s32 i;
    POLY_FT4 *p;
    s32 unused[4];

    if (D_800794E7 == 0) {
        return;
    }
    if (D_800794E7 != 1) {
        func_801EAD04();
    }
    for (i = 1; i >= 0; i--) {
        if (D_800793A8.mode == 1) {
            D_800793A8.sprt[D_800794F4][i].tpage = GetTPage(2, 0, i * 0x100, 0x100 - D_800794F4 * 0x100) | 0xE1000000;
            (D_800793A8.sprt[D_800794F4] + i)->x0 = D_800793A8.x + (i << 8);
            (D_800793A8.sprt[D_800794F4] + i)->y0 = D_800793A8.y;
            (D_800793A8.sprt[D_800794F4] + i)->r0 = D_800793A8.r;
            (D_800793A8.sprt[D_800794F4] + i)->g0 = D_800793A8.g;
            (D_800793A8.sprt[D_800794F4] + i)->b0 = D_800793A8.b;
            addPrim(&D_800793A0->ot[0], &D_800793A8.sprt[D_800794F4][i]);
        } else {
            p = &D_800793A8.poly[D_800794F4][i];
            p->tpage = getTPage(2, D_800793A8.abr & 3, i * 160, 0x100 - D_800794F4 * 0x100);
            p->x0 = D_800793A8.px[i][0];
            p->y0 = D_800793A8.py[i][0];
            p->x1 = D_800793A8.px[i][1];
            p->y1 = D_800793A8.py[i][1];
            p->x2 = D_800793A8.px[i][2];
            p->y2 = D_800793A8.py[i][2];
            p->x3 = D_800793A8.px[i][3];
            p->y3 = D_800793A8.py[i][3];
            p->r0 = D_800793A8.r;
            p->g0 = D_800793A8.g;
            p->b0 = D_800793A8.b;
            addPrim(&D_800793A0->ot[0], p);
        }
    }
}

void func_800152AC(void) {
    s32 *p;
    s32 i;

    SetGraphDebug(0);
    InitGeom();
    p = (s32 *)0x1F800000;
    for (i = 0; i < 0x100; i++) {
        *p++ = 0;
    }
    (*(s16 *)((s8 *)(&D_800794F8) + 0)) = 0;
    (*(s16 *)((s8 *)(&D_800794F8) + 2)) = 0;
    (*(s16 *)((s8 *)(&D_800794F8) + 4)) = 0x100;
    (*(s16 *)((s8 *)(&D_800794F8) + 6)) = 0x100;
    (*(s32 *)((s8 *)(&D_800794F8) + 0x50)) = 2;
    (*(s32 *)((s8 *)(&D_800794F8) + 0x48)) = 0;
    D_800794F0 = 1;
    func_80014D64();
}

void func_80015328(void) {
    Unk800794F8 *g;
    s32 n;
    void (**cb)(Unk800793A0 *, s32);

    g = (Unk800794F8 *)&D_800794F8;
    g->unk8[0] = 0;
    D_800794EC = 0;
    for (; g->unk48 <= 0; g->unk48++) {
        func_8001A9B0();
        func_80014C08(1);
        g->unk50 = D_800794EC;
        if (D_800794EC == 0) {
            g->unk50 = 1;
        }
        D_800794EC = 0;
    }
    SetDispMask(1);
    D_800794F4 = 0;
    D_800793A0 = &g->unk98[0];
    ClearOTagR(g->unk98[0].ot, 0x1000);
    for (;;) {
        n = D_800794F0;
        func_8001A9B0();
        while (n >= 2 || g->unk48 == 0) {
            func_80014AC8();
            n--;
        }
        D_800794F4 ^= 1;
        D_800793A0 = &g->unk98[D_800794F4];
        ClearOTagR(D_800793A0->ot, 0x1000);
        if (D_800793A8.mode != 0) {
            addPrim(&D_800793A0->ot[0], &D_800793A8.stp[1]);
            addPrim(&D_800793A0->ot[0xFFF], &D_800793A8.stp[0]);
        }
        func_8002FAE4();
        func_800271D0();
        func_80016BEC();
        if (D_8006DD4C != 0) {
            for (cb = g->unk8; *cb != 0; cb++) {
                (*cb)(D_800793A0, D_800794F4);
            }
        }
        func_80014EF0();
        func_80014AC8();
        DrawSync(0);
        if (g->unk4C != 0) {
            GsSwapDispBuff();
        }
        PutDispEnv(&D_800793A0->disp);
        PutDrawEnv(&D_800793A0->draw);
        DrawOTag(&D_800793A0->ot[0xFFF]);
        g->unk50 = D_800794EC;
        if (D_800794EC == 0) {
            g->unk50 = 1;
        }
        D_800794EC = 0;
    }
}

void func_800155F4(void) {
    s32 t;

    t = func_800148B0();
    func_8002BC58();
    func_8001A600();
    func_8006A884(0);
    func_800157B0();
    func_800152AC();
    func_80014840();
    func_80015EDC();
    func_8001AA80(0);
    func_8002ADEC(t);
    func_80026E90(0x3C0, 0x100, 0x3E8);
    func_80016948(0xD);
    func_8001B90C(0x140, 0xF0, 0);
    initPlayerData();
    func_8002F79C();
    for (;;) {
        func_80014840();
        func_80015EDC();
        func_8001AA80(0);
        func_80014C08(0xA);
        func_800168C4();
        initScreenFade();
        D_8008983C = 1;
        func_800149B8(0, -1, 0, 0x800, func_8002B3EC, 2, t);
        func_80014C08(0x7FFFFFFF);
        func_801E055C(0);
        func_8002AEA4(1);
        func_8002B688();
        func_8001B90C(0x140, 0xF0, 0);
        func_800149B8(0x1F, 0, 0, 0x800, func_80015328, 0, 0, 0, 0);
        func_80014C08(0xA);
        func_800149B8(0, -1, 0, 0x400, func_8002F4F4, 0, 0, 0, 0);
        func_80014C08(0x7FFFFFFF);
        func_80014C08(0xA);
    }
}
