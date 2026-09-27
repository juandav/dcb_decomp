#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/pad.h"
#include "dcb/heap.h"

void func_8001A600(void) {
    s32 i;
    u8 *buf;

    D_8008983C = 1;
    buf = func_8001ACEC(0x3C);
    for (i = 0; i < 2; i++) {
        D_80089840[i] = (PadState *)(buf + i * 0x1E);
    }
    func_8001A6B0();
    PadInitDirect(&D_800897F8, (s8 *)&D_800897F8 + 0x22);
    PadStartCom();
}

void func_8001A688(s32 arg0, s16 arg1, s16 arg2) {
    D_80089840[arg0]->repeatDelay = arg1;
    D_80089840[arg0]->repeatRate = arg2;
}

void func_8001A6B0(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        D_80089840[i]->unk10 = 1;
        D_80089840[i]->unk11 = 0;
        D_80089840[i]->unk12 = 0;
        D_80089840[i]->unk1A = 0;
        D_80089840[i]->unk1B = 0;
        D_80089840[i]->unk0 = 0;
        D_80089840[i]->unk6 = 0;
        D_80089840[i]->unk4 = 0;
        D_80089840[i]->unk2 = 0;
        D_80089840[i]->unk8 = 0;
        D_80089840[i]->unkE = 0;
        D_80089840[i]->unkC = 0;
        D_80089840[i]->unkA = 0;
        func_8001A688(i, 0x1E, 2);
    }
}

s32 func_8001A7A4(s32 port, PadState *pad, u8 *buf) {
    s32 skip;
    s32 changed;
    u16 pressed;
    s32 cur;
    s16 t;

    if (buf[1] == 0x80) {
        pad->unk0 = 0;
        pad->unk6 = 0;
        pad->unk4 = 0;
        pad->unk2 = 0;
        return 0;
    }
    pad->unk1A = PadGetState(port);
    pad->unk1B = PadInfoMode(port, 1, 0);
    pad->unk1C = PadInfoMode(port, 2, 0);
    if (pad->unk1A == 0 || buf[0] != 0) {
        pad->unk0 = 0;
        pad->unk6 = 0;
        pad->unk4 = 0;
        pad->unk2 = 0;
        return 0;
    }
    skip = 0;
    switch (pad->unk1B) {
    case 1:
    case 3:
    case 6:
        skip = 1;
        break;
    case 2:
    case 4:
    case 5:
    case 7:
        break;
    }
    if (skip) {
        return 0;
    }
    changed = pad->unk0;
    pad->unk0 = ~((buf[2] << 8) | buf[3]);
    changed ^= pad->unk0;
    pressed = changed & pad->unk0;
    pad->unk2 = pressed;
    pad->unk4 = changed & ~pad->unk0;
    pad->unk6 = pressed;
    if (pad->unk10) {
        cur = pad->unk0;
        if (cur == pad->unk14 && cur != 0) {
            t = pad->unk12;
            pad->unk12 = t + ((Unk800794F8 *)&D_800794F8)->unk50;
            if (pad->unk11 == 0) {
                if (pad->unk12 < pad->repeatDelay) {
                    return 0;
                }
                if (t != 0) {
                    pad->unk11 = 1;
                    pad->unk12 = 0;
                    pad->unk6 = pressed | cur;
                }
            } else {
                if (pad->unk12 < pad->repeatRate) {
                    return 0;
                }
                pad->unk12 = 0;
                pad->unk6 |= cur;
            }
        } else {
            pad->unk14 = cur;
            pad->unk11 = 0;
            pad->unk12 = 0;
        }
    } else if (pad->unk4) {
        pad->unk10 = 1;
    }
    return 0;
}

void func_8001A9B0(void) {
    s32 var_s1;
    void *temp_s0;

    var_s1 = 0;
    do {
        temp_s0 = D_80089840[var_s1];
        func_8001A7A4(var_s1 * 0x10, temp_s0, (u8 *)&D_800897F8 + var_s1 * 0x22);
        if (D_8008983C != 0) {
            (*(u16 *)((s8 *)temp_s0 + 8)) = (u16) (*(u16 *)((s8 *)temp_s0 + 0));
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = (u16) (*(u16 *)((s8 *)temp_s0 + 6));
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = (u16) (*(u16 *)((s8 *)temp_s0 + 4));
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = (u16) (*(u16 *)((s8 *)temp_s0 + 2));
        } else {
            (*(u16 *)((s8 *)temp_s0 + 8)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xE)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xC)) = 0U;
            (*(u16 *)((s8 *)temp_s0 + 0xA)) = 0U;
        }
        var_s1 += 1;
    } while (var_s1 < 2);
}
