#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/pad.h"
#include "dcb/heap.h"
#include "dcb/angle.h"

#if VERSION_JP
/* jp has no resetPadStates and no PAD_INPUT_ENABLED: initPads clears part of
   each pad's state itself */
void initPads(void) {
    s32 i;
    u8 *padMemory;

    padMemory = allocPermanentHeapBlock(sizeof(PadState) * 2);
    for (i = 0; i < 2; i++) {
        PAD_STATES[i] = (PadState *)(padMemory + i * sizeof(PadState));
        PAD_STATES[i]->rawHeld = 0;
        PAD_STATES[i]->repeatEnabled = 1;
        PAD_STATES[i]->repeating = 0;
        PAD_STATES[i]->holdTime = 0;
        PAD_STATES[i]->padStatus = 0;
        PAD_STATES[i]->padType = 0;
        setPadRepeatRate(i, 0x1E, 2);
    }
    PadInitDirect(PAD_RECEIVE_BUFFERS[0], PAD_RECEIVE_BUFFERS[1]);
    PadStartCom();
}
#elif VERSION_US || VERSION_EU
void initPads(void) {
    s32 i;
    u8 *padMemory;

    PAD_INPUT_ENABLED = 1;
    padMemory = allocPermanentHeapBlock(sizeof(PadState) * 2);
    for (i = 0; i < 2; i++) {
        PAD_STATES[i] = (PadState *)(padMemory + i * sizeof(PadState));
    }
    resetPadStates();
    PadInitDirect(PAD_RECEIVE_BUFFERS[0], PAD_RECEIVE_BUFFERS[1]);
    PadStartCom();
}
#endif

void setPadRepeatRate(s32 port, s16 repeatDelay, s16 repeatRate) {
    PAD_STATES[port]->repeatDelay = repeatDelay;
    PAD_STATES[port]->repeatRate = repeatRate;
}

#if VERSION_US || VERSION_EU
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
#endif

/* rawData is the port's receive buffer: a status byte (0 when the transfer
   worked), the controller's id, then its two bytes of buttons (0 = pressed). */
s32 updatePadState(s32 port, PadState *pad, u8 *rawData) {
    s32 skip;
    s32 changed;
    u16 pressed;
    s32 held;
    s32 prevHoldTime;

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
    /* mice (1) and light guns (3, 6) are ignored */
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
    /* the edges since the last poll */
    changed = pad->rawHeld;
    pad->rawHeld = ~((rawData[2] << 8) | rawData[3]);
    changed ^= pad->rawHeld;
    pressed = changed & pad->rawHeld;
    pad->rawPressed = pressed;
    pad->rawReleased = changed & ~pad->rawHeld;
    pad->rawRepeat = pad->rawPressed;
    /* auto-repeat: holding the same buttons fires once after repeatDelay,
       then every repeatRate (counted in vblanks) */
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
                    /* the same value: rawRepeat still holds pressed here */
#if VERSION_US
                    pad->rawRepeat = pressed | held;
#elif VERSION_JP || VERSION_EU
                    pad->rawRepeat |= held;
#else
#error "main/system/pad: version not checked"
#endif
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

#if VERSION_JP
/* jp's game reads the pads' raw masks: there are no copies to update */
void pollPads(void) {
    s32 port;

    for (port = 0; port < 2; port++) {
        updatePadState(port * 0x10, PAD_STATES[port], PAD_RECEIVE_BUFFERS[port]);
    }
}
#elif VERSION_US || VERSION_EU
void pollPads(void) {
    s32 port;
    PadState *pad;

    port = 0;
    do {
        pad = PAD_STATES[port];
        /* libpad numbers the ports 0x00 and 0x10 */
        updatePadState(port * 0x10, pad, PAD_RECEIVE_BUFFERS[port]);
        /* the game reads the copies, which stay clear while input is off */
        if (PAD_INPUT_ENABLED != 0) {
            pad->held = pad->rawHeld;
            pad->repeat = pad->rawRepeat;
            pad->released = pad->rawReleased;
            pad->pressed = pad->rawPressed;
        } else {
            pad->held = 0;
            pad->repeat = 0;
            pad->released = 0;
            pad->pressed = 0;
        }
        port += 1;
    } while (port < 2);
}
#endif
