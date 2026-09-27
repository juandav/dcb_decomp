#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/pad.h"
#include "dcb/heap.h"

void initPads(void) {
    s32 i;
    u8 *padMemory;

    PAD_INPUT_ENABLED = 1;
    padMemory = allocPermanentHeapBlock(0x3C);
    for (i = 0; i < 2; i++) {
        PAD_STATES[i] = (PadState *)(padMemory + i * 0x1E);
    }
    resetPadStates();
    PadInitDirect(&PAD_RECEIVE_BUFFERS, (s8 *)&PAD_RECEIVE_BUFFERS + 0x22);
    PadStartCom();
}

void setPadRepeatRate(s32 port, s16 repeatDelay, s16 repeatRate) {
    PAD_STATES[port]->repeatDelay = repeatDelay;
    PAD_STATES[port]->repeatRate = repeatRate;
}

void resetPadStates(void) {
    s32 i;

    for (i = 0; i < 2; i++) {
        PAD_STATES[i]->unk10 = 1;
        PAD_STATES[i]->unk11 = 0;
        PAD_STATES[i]->unk12 = 0;
        PAD_STATES[i]->unk1A = 0;
        PAD_STATES[i]->unk1B = 0;
        PAD_STATES[i]->unk0 = 0;
        PAD_STATES[i]->unk6 = 0;
        PAD_STATES[i]->unk4 = 0;
        PAD_STATES[i]->unk2 = 0;
        PAD_STATES[i]->unk8 = 0;
        PAD_STATES[i]->unkE = 0;
        PAD_STATES[i]->unkC = 0;
        PAD_STATES[i]->unkA = 0;
        setPadRepeatRate(i, 0x1E, 2);
    }
}

s32 updatePadState(s32 port, PadState *pad, u8 *rawData) {
    s32 skip;
    s32 changed;
    u16 pressed;
    s32 held;
    s16 prevHoldTime;

    if (rawData[1] == 0x80) {
        pad->unk0 = 0;
        pad->unk6 = 0;
        pad->unk4 = 0;
        pad->unk2 = 0;
        return 0;
    }
    pad->unk1A = PadGetState(port);
    pad->unk1B = PadInfoMode(port, 1, 0);
    pad->unk1C = PadInfoMode(port, 2, 0);
    if (pad->unk1A == 0 || rawData[0] != 0) {
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
    pad->unk0 = ~((rawData[2] << 8) | rawData[3]);
    changed ^= pad->unk0;
    pressed = changed & pad->unk0;
    pad->unk2 = pressed;
    pad->unk4 = changed & ~pad->unk0;
    pad->unk6 = pressed;
    if (pad->unk10) {
        held = pad->unk0;
        if (held == pad->unk14 && held != 0) {
            prevHoldTime = pad->unk12;
            pad->unk12 = prevHoldTime + ((Unk800794F8 *)&GRAPHICS)->unk50;
            if (pad->unk11 == 0) {
                if (pad->unk12 < pad->repeatDelay) {
                    return 0;
                }
                if (prevHoldTime != 0) {
                    pad->unk11 = 1;
                    pad->unk12 = 0;
                    pad->unk6 = pressed | held;
                }
            } else {
                if (pad->unk12 < pad->repeatRate) {
                    return 0;
                }
                pad->unk12 = 0;
                pad->unk6 |= held;
            }
        } else {
            pad->unk14 = held;
            pad->unk11 = 0;
            pad->unk12 = 0;
        }
    } else if (pad->unk4) {
        pad->unk10 = 1;
    }
    return 0;
}

void pollPads(void) {
    s32 port;
    void *pad;

    port = 0;
    do {
        pad = PAD_STATES[port];
        updatePadState(port * 0x10, pad, (u8 *)&PAD_RECEIVE_BUFFERS + port * 0x22);
        if (PAD_INPUT_ENABLED != 0) {
            (*(u16 *)((s8 *)pad + 8)) = (u16) (*(u16 *)((s8 *)pad + 0));
            (*(u16 *)((s8 *)pad + 0xE)) = (u16) (*(u16 *)((s8 *)pad + 6));
            (*(u16 *)((s8 *)pad + 0xC)) = (u16) (*(u16 *)((s8 *)pad + 4));
            (*(u16 *)((s8 *)pad + 0xA)) = (u16) (*(u16 *)((s8 *)pad + 2));
        } else {
            (*(u16 *)((s8 *)pad + 8)) = 0U;
            (*(u16 *)((s8 *)pad + 0xE)) = 0U;
            (*(u16 *)((s8 *)pad + 0xC)) = 0U;
            (*(u16 *)((s8 *)pad + 0xA)) = 0U;
        }
        port += 1;
    } while (port < 2);
}
