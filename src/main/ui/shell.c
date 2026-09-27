#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/shell.h"
#include "dcb/card_db.h"
#include "dcb/loader.h"
#include "dcb/main.h"
#include "dcb/memcard.h"
#include "dcb/menu.h"
#include "dcb/sound.h"
#include "dcb/stage.h"
#include "dcb/system.h"
#include "dcb/text.h"
#include "dcb/window.h"

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

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", D_80012770);

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", D_80012D68);

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", D_80012DB8);

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", D_80012DF8);

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

INCLUDE_RODATA("asm/main/nonmatchings/ui/shell", D_80012FAC);
