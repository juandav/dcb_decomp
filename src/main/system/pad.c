#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/pad.h"
#include "dcb/heap.h"
#include "dcb/angle.h"

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
        PAD_STATES[i]->repeatEnabled = 1;
        PAD_STATES[i]->repeating = 0;
        PAD_STATES[i]->holdTime = 0;
        PAD_STATES[i]->padStatus = 0;
        PAD_STATES[i]->padType = 0;
        PAD_STATES[i]->rawHeld = 0;
        PAD_STATES[i]->rawRepeat = 0;
        PAD_STATES[i]->rawReleased = 0;
        PAD_STATES[i]->rawPressed = 0;
        PAD_STATES[i]->held = 0;
        PAD_STATES[i]->repeat = 0;
        PAD_STATES[i]->released = 0;
        PAD_STATES[i]->pressed = 0;
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
        pad->rawHeld = 0;
        pad->rawRepeat = 0;
        pad->rawReleased = 0;
        pad->rawPressed = 0;
        return 0;
    }
    pad->padStatus = PadGetState(port);
    pad->padType = PadInfoMode(port, 1, 0);
    pad->padExId = PadInfoMode(port, 2, 0);
    if (pad->padStatus == 0 || rawData[0] != 0) {
        pad->rawHeld = 0;
        pad->rawRepeat = 0;
        pad->rawReleased = 0;
        pad->rawPressed = 0;
        return 0;
    }
    skip = 0;
    switch (pad->padType) {
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
    changed = pad->rawHeld;
    pad->rawHeld = ~((rawData[2] << 8) | rawData[3]);
    changed ^= pad->rawHeld;
    pressed = changed & pad->rawHeld;
    pad->rawPressed = pressed;
    pad->rawReleased = changed & ~pad->rawHeld;
    pad->rawRepeat = pressed;
    if (pad->repeatEnabled) {
        held = pad->rawHeld;
        if (held == pad->repeatButtons && held != 0) {
            prevHoldTime = pad->holdTime;
            pad->holdTime = prevHoldTime + ((Graphics *)&GRAPHICS)->vblanksPerFrame;
            if (pad->repeating == 0) {
                if (pad->holdTime < pad->repeatDelay) {
                    return 0;
                }
                if (prevHoldTime != 0) {
                    pad->repeating = 1;
                    pad->holdTime = 0;
                    pad->rawRepeat = pressed | held;
                }
            } else {
                if (pad->holdTime < pad->repeatRate) {
                    return 0;
                }
                pad->holdTime = 0;
                pad->rawRepeat |= held;
            }
        } else {
            pad->repeatButtons = held;
            pad->repeating = 0;
            pad->holdTime = 0;
        }
    } else if (pad->rawReleased) {
        pad->repeatEnabled = 1;
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
