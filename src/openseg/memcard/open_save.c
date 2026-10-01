#include "common.h"
#include "game.h"
#include "dcb/open_save.h"
#include "dcb/memcard.h"
#include "dcb/save_checksum.h"
#include "dcb/openseg.h"
#include "dcb/open_memcard.h"
#include "dcb/dialog.h"

typedef struct {
    u8 data[0x20];
} IconClut;

typedef struct {
    u8 data[0x80];
} IconFrame;

/* the save's icon: three frames, stored from VRAM together */
typedef struct {
    IconFrame frames[3];
} IconImage;

typedef struct {
    /* 0x00 */ char magic[2];
    /* 0x02 */ u8 type;
    /* 0x03 */ u8 blocks;
    /* 0x04 */ char title[64];
    /* 0x44 */ u8 reserve[28];
    /* 0x60 */ IconClut clut;
    /* 0x80 */ IconImage icon;
} SaveHeader;

extern PlayerProfile *OPEN_MEMCARD_BUFFER;
extern u8 OPEN_MEMCARD_MESSAGE_PORT;

void StoreImage(Rect16 *rect, void *p);
void OPEN_applyLoadedSave();
void OPEN_prepareSaveData(s32 port);
s32 OPEN_readSavePreview(s32 port, s32 slot);
s32 OPEN_ensureMemoryCardReady(s32 port);
s32 OPEN_checkMemoryCard(s32 port);
s16 OPEN_waitMemoryCardSave(s32 part, s32 port);
s16 OPEN_waitMemoryCardLoad(s32 unused, s32 port);
s16 OPEN_countFreeBlocks(s32 port);
void OPEN_buildSaveHeader(s32 port, s32 slot);
u8 OPEN_checkSaveIsCurrent(s32 player, s32 port, s32 slot);

/* the save file names: the region's product code and the slot; the memory
   card functions take them as s32 */
#if VERSION_EU
#define SAVE_FILE_PREFIX "BESLES-03900"
#elif VERSION_US
#define SAVE_FILE_PREFIX "BASLUS-01328"
#else
#error "openseg/memcard/open_save: version not checked"
#endif
s32 OPEN_SAVE_FILE_NAMES[3] = {
    (s32)SAVE_FILE_PREFIX "_A",
    (s32)SAVE_FILE_PREFIX "_B",
    (s32)SAVE_FILE_PREFIX "_C",
};

#if VERSION_EU
/* not referenced by any code */
s32 D_801F2B0C = 0;
#elif VERSION_US
#else
#error "openseg/memcard/open_save: version not checked"
#endif

u8 *OPEN_formatSjisNumber(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;
    s32 minus;

    minus = 0;
    if (width < 0) {
        if (value < 0) {
            *dst++ = 0x81;
            *dst++ = 0x7C;
        } else {
            *dst++ = 0x81;
            *dst++ = 0x7B;
        }
    } else if (value < 0) {
        minus = 1;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x81;
        *dst++ = 0x40;
    }
    *dst = 0;
    if (minus) {
        width--;
    }
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    if (minus) {
        *--dst = 0x7C;
        *--dst = 0x81;
    }
    return dst;
}

u8 *OPEN_formatSjisNumberZeros(s32 value, s32 width, u8 *dst) {
    s32 i;
    s32 digit;
    s32 quot;

    if (value < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7C;
    } else if (width < 0) {
        *dst++ = 0x81;
        *dst++ = 0x7B;
    }
    width = abs(width);
    value = abs(value);
    for (i = 0; i < width; i++) {
        *dst++ = 0x82;
        *dst++ = 0x4F;
    }
    *dst = 0;
    do {
        if (width-- <= 0) {
            break;
        }
        dst--;
        quot = value / 10;
        digit = value % 10;
        *dst = digit + 0x4F;
        *--dst = (digit + 0x824F) >> 8;
        value = quot;
    } while (value != 0);
    return dst;
}

void OPEN_runMemcardAccess(void) {
    s32 player;
    s32 port;
    s32 slot;
    s32 status;
    s32 result;
    s32 i;
    s32 card;

    OPEN_MEMCARD_MESSAGE = -1;
    do {
        waitFrames(FRAME_INTERVAL);
        player = OPEN_MEMCARD.card;
        port = ((SessionView *)SESSION_DATA)->saves[player].slot;
        slot = ((SessionView *)SESSION_DATA)->saves[player].file;
        status = 0;
        switch (OPEN_MEMCARD.state) {
        case 1:
            if (OPEN_MEMCARD.mode == 7 && port == 0) {
                for (card = 0; card < 2; card++) {
                    status = OPEN_checkMemoryCard(card);
                    if (status != 0) {
                        break;
                    }
                }
            } else {
                status = OPEN_checkMemoryCard(port);
            }
            OPEN_MEMCARD.previewsRead = 0;
            if (status == 1) {
                if (OPEN_MEMCARD.mode == 0) {
                    OPEN_MEMCARD.message = 4;
                    OPEN_MEMCARD.state = 2;
                } else {
                    OPEN_MEMCARD.message = 12;
                    OPEN_MEMCARD.state = 29;
                }
            } else if (status == 2) {
                if (OPEN_MEMCARD.mode == 0) {
                    OPEN_MEMCARD.state = 7;
                } else {
                    OPEN_MEMCARD.message = 3;
                    OPEN_MEMCARD.state = 11;
                }
            } else {
                if (OPEN_MEMCARD.mode == 7 && port == 0) {
                    for (card = 0; card < 2; card++) {
                        scanMemoryCardFiles(card);
                        for (i = 0; i < 3; i++) {
                            OPEN_readSavePreview(card, i);
                        }
                        status = (s8)(OPEN_MEMCARD.empty[card][0] & OPEN_MEMCARD.empty[card][1] & OPEN_MEMCARD.empty[card][2]);
                        if (status == 1) {
                            OPEN_MEMCARD.messagePort = card;
                            status = OPEN_checkMemoryCard(card);
                            if (status == 1) {
                                OPEN_MEMCARD.message = 12;
                                OPEN_MEMCARD.state = 29;
                            } else if (status == 2) {
                                OPEN_MEMCARD.message = 3;
                                OPEN_MEMCARD.state = 11;
                            } else {
                                OPEN_MEMCARD.state = 28;
                            }
                            break;
                        }
                    }
                    if (OPEN_MEMCARD.state != 1) {
                        break;
                    }
                } else {
                    scanMemoryCardFiles(port);
                    for (i = 0; i < 3; i++) {
                        OPEN_readSavePreview(port, i);
                    }
                }
                OPEN_MEMCARD.freeBlocks = OPEN_countFreeBlocks(port);
                switch (OPEN_MEMCARD.mode) {
                case 0:
                    status = OPEN_MEMCARD.freeBlocks;
                    for (i = 0; i < 3; i++) {
                        if (OPEN_MEMCARD.empty[port][i] == 0) {
                            status += 2;
                        }
                    }
                    if (status < 2) {
                        OPEN_MEMCARD.state = 6;
                    } else {
                        OPEN_showSaveSlots();
                        OPEN_MEMCARD.state = 3;
                    }
                    break;
                case 7:
                case 0xFF:
                    status = (s8)(OPEN_MEMCARD.empty[port][0] & OPEN_MEMCARD.empty[port][1] & OPEN_MEMCARD.empty[port][2]);
                    if (status == 1) {
                        OPEN_MEMCARD.messagePort = port;
                        OPEN_MEMCARD.state = 28;
                    } else {
                        OPEN_showSaveSlots();
                        OPEN_MEMCARD.state = 10;
                    }
                    break;
                }
            }
            break;
        case 17:
            OPEN_MEMCARD.message = -1;
            break;
        case 23:
            if (OPEN_MEMCARD.mode == 6 && port == 0) {
                for (card = 0; card < 2; card++) {
                    status = OPEN_checkSaveIsCurrent(card, card, ((SessionView *)SESSION_DATA)->saves[card].file);
                    if (status != 3) {
                        break;
                    }
                }
            } else {
                status = OPEN_checkSaveIsCurrent(player, port, slot);
            }
            switch (status) {
            case 0:
            case 2:
                OPEN_MEMCARD.message = 13;
                OPEN_MEMCARD.state = 19;
                break;
            case 1:
                OPEN_MEMCARD.message = 12;
                OPEN_MEMCARD.state = 19;
                break;
            case 3:
                OPEN_MEMCARD.message = -1;
                OPEN_MEMCARD.state = 24;
                break;
            }
            break;
        case 24:
            status = OPEN_ensureMemoryCardReady(port);
            if (status == 1) {
                OPEN_MEMCARD.message = 12;
                OPEN_MEMCARD.state = 19;
                ((Dialog *)&OPEN_DIALOG)->closed = status;
            }
            break;
        case 4:
            OPEN_prepareSaveData(player);
            OPEN_buildSaveHeader(player, slot);
            writeSaveChecksum(0x2774, OPEN_MEMCARD.buffer);
            if (startMemoryCardSave(port, 2, (s32)OPEN_MEMCARD.buffer, OPEN_SAVE_FILE_NAMES[slot], (McHeader *)MEMORY_CARD_SAVE_HEADER) == -1) {
                OPEN_MEMCARD.state = 5;
            } else if (OPEN_waitMemoryCardSave(player, port) == -1) {
                OPEN_MEMCARD.state = 5;
            } else {
                OPEN_MEMCARD.message = 21;
                OPEN_MEMCARD.state = 25;
            }
            break;
        case 18:
            OPEN_prepareSaveData(player);
            OPEN_buildSaveHeader(player, slot);
            writeSaveChecksum(0x2774, OPEN_MEMCARD.buffer);
            if (startMemoryCardSave(port, 2, (s32)OPEN_MEMCARD.buffer, OPEN_SAVE_FILE_NAMES[slot], (McHeader *)MEMORY_CARD_SAVE_HEADER) == -1) {
                OPEN_MEMCARD.state = 20;
            } else {
                status = OPEN_waitMemoryCardSave(player, port);
                if (status == -1) {
                    OPEN_MEMCARD.state = 20;
                } else {
                    OPEN_MEMCARD.cancelled = 0;
                    if (OPEN_MEMCARD.mode != 6 || player != 0) {
                        playMenuSound(1);
                        OPEN_MEMCARD.message = 22;
                        OPEN_MEMCARD.animPhase = 1;
                        OPEN_MEMCARD.state = 26;
                    } else {
                        OPEN_MEMCARD.animPhase = 3;
                        OPEN_MEMCARD.state = 27;
                    }
                }
            }
            break;
        case 3:
        case 10:
            result = OPEN_ensureMemoryCardReady(port);
            if (result == 1) {
                ((Dialog *)&OPEN_DIALOG)->closed = result;
                OPEN_MEMCARD.state = result;
                OPEN_hideSaveSlots();
            }
            break;
        case 7:
            result = OPEN_ensureMemoryCardReady(port);
            if (result == 1) {
                ((Dialog *)&OPEN_DIALOG)->closed = result;
                OPEN_MEMCARD.state = result;
            }
            break;
        case 8:
            formatMemoryCard(port);
            OPEN_MEMCARD.state = 1;
            break;
        case 9:
            if (startMemoryCardLoad(port, (s32)OPEN_MEMCARD.buffer, OPEN_SAVE_FILE_NAMES[slot]) == -1) {
                OPEN_MEMCARD.state = 13;
                break;
            }
            OPEN_MEMCARD.animPhase = 0;
            if (OPEN_waitMemoryCardLoad(player, port) == -1) {
                OPEN_MEMCARD.state = 13;
                break;
            }
            if (verifySaveChecksum(((SaveSlot *)OPEN_MEMCARD.buffer)->size, OPEN_MEMCARD.buffer) == 1) {
                OPEN_MEMCARD.progress = 0;
                OPEN_MEMCARD.state = 15;
            } else {
                OPEN_MEMCARD.message = 20;
                OPEN_MEMCARD.state = 25;
                if (player == 0) {
                    if ((((PlayerProfile *)OPEN_MEMCARD.buffer)->monoSound) == 1) {
                        SsSetMono();
                    } else {
                        SsSetStereo();
                    }
                }
            }
            break;
        case 25:
            OPEN_MEMCARD.animPhase = 1;
            playMenuSound(1);
            OPEN_applyLoadedSave(player, port, slot);
            OPEN_MEMCARD.state = 12;
            break;
        case 20:
            ((Dialog *)&OPEN_DIALOG)->closed = 1;
            OPEN_MEMCARD.message = 15;
            OPEN_MEMCARD.state = 21;
            break;
        case 13:
            ((Dialog *)&OPEN_DIALOG)->closed = 1;
            OPEN_MEMCARD.message = 19;
            OPEN_MEMCARD.state = 14;
            break;
        case 29:
            break;
        }
    } while (OPEN_MEMCARD.ready != 1);
    waitFrames(FRAME_INTERVAL);
}

void OPEN_applyLoadedSave(s32 port, s32 slot, s32 file) {
    PlayerProfile *src;

    switch (OPEN_MEMCARD_MODE) {
    case 0:
        break;
    case 7:
    case 0xFF:
        src = OPEN_MEMCARD_BUFFER;
        ((PlayerProfile *)PLAYER_PROFILES)[port] = *src;
        ((SessionView *)SESSION_DATA)->saves[port].playTime = src->playTime;
        break;
    }
}

void OPEN_prepareSaveData(s32 port) {
    PlayerProfile *buffer;

    buffer = OPEN_MEMCARD.buffer;
    if (OPEN_MEMCARD.mode == 6) {
        OPEN_MEMCARD.progress = port * 50;
    } else {
        OPEN_MEMCARD_PROGRESS = 0;
    }
    switch (OPEN_MEMCARD_MODE) {
    case 0:
        PLAYER_DATA(port).resumeInArea = 1;
        PLAYER_DATA(port).unk28_13 = OPEN_MEMCARD.unk538;
        ((SessionView *)SESSION_DATA)->saves[port].playTime = PLAYER_DATA(port).playTime;
        *buffer = PLAYER_DATA(port);
        return;
    case 2:
    case 4:
    case 5:
    case 8:
        if (PLAYER_DATA(port).saveCount < 255) {
            PLAYER_DATA(port).saveCount++;
        }
    case 6:
        ((SessionView *)SESSION_DATA)->saves[port].playTime = PLAYER_DATA(port).playTime;
        *buffer = PLAYER_DATA(port);
        break;
    }
}

s32 OPEN_readSavePreview(s32 port, s32 slot) {
    s16 i;

    OPEN_MEMCARD.empty[port][slot] = 1;
    for (i = 0; i < 5; i++) {
        if ((OPEN_MEMCARD.empty[port][slot] = readMemoryCardSavePreview(port, &OPEN_MEMCARD.slots[port][slot], OPEN_SAVE_FILE_NAMES[slot])) == 0) {
            OPEN_MEMCARD.previewsRead++;
            return 0;
        }
    }
    return 1;
}

s32 OPEN_ensureMemoryCardReady(s32 port) {
    OPEN_MEMCARD_MESSAGE_PORT = port;
    return ensureMemoryCardReady(port);
}

s32 OPEN_checkMemoryCard(s32 port) {
    switch (OPEN_MEMCARD.mode) {
    case 6:
    case 7:
        OPEN_MEMCARD.message = 2;
        break;
    default:
        OPEN_MEMCARD_MESSAGE = 0;
        break;
    }
    OPEN_MEMCARD_MESSAGE_PORT = port;
    return getMemoryCardStatus(port);
}

s16 OPEN_waitMemoryCardSave(s32 part, s32 port) {
    s32 result;

    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (OPEN_ensureMemoryCardReady(port) != 0) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        result = stepMemoryCardSave();
        if (result == -1) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        if (OPEN_MEMCARD.mode == 6) {
            OPEN_MEMCARD.progress = part * 50 + result / 2;
        } else {
            OPEN_MEMCARD.progress = result;
        }
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            return 0;
        }
    }
}

s16 OPEN_waitMemoryCardLoad(s32 unused, s32 port) {
    s32 result;

    while (1) {
        waitFrames(FRAME_INTERVAL);
        if (OPEN_ensureMemoryCardReady(port) != 0) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        result = stepMemoryCardLoad();
        if (result == -1) {
            OPEN_MEMCARD.progress = 0;
            return -1;
        }
        OPEN_MEMCARD.progress = result;
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            return 0;
        }
    }
}

s16 OPEN_countFreeBlocks(s32 port) {
    return 15 - MEMORY_CARD_DIRECTORIES[port]->blocks;
}

void OPEN_buildSaveHeader(s32 port, s32 slot) {
    SaveHeader *header = (SaveHeader *)MEMORY_CARD_SAVE_HEADER;
    IconImage icon;
    IconClut clut;
    s16 iconX[3] = { 0x158, 0x15C, 0x160 };
    Rect16 iconRect;
    Rect16 clutRect;
    char title[72];
    u8 file[16];
    u8 hours[16];
    u8 minutes[16];
    s32 time;
    s32 hour;
    s32 minute;
    s32 i;

    iconRect.x = iconX[slot];
    iconRect.y = 0x154;
    iconRect.w = 4;
    iconRect.h = 0x30;
    StoreImage(&iconRect, &icon);
    clutRect.x = 0x170;
    clutRect.y = slot + 0x1FA;
    clutRect.w = 0x10;
    clutRect.h = 1;
    StoreImage(&clutRect, &clut);
    DrawSync(0);
    bzero((Scene3D *)title, 0x42);
    time = ((SessionView *)SESSION_DATA)->saves[port].playTime;
    hour = time / 216000;
    minute = (time - hour * 216000) / 3600;
    if (hour >= 1000) {
        hour = 999;
        minute = 59;
    }
    OPEN_formatSjisNumber(slot + 1, 1, file);
    OPEN_formatSjisNumber(hour, 3, hours);
    OPEN_formatSjisNumberZeros(minute, 2, minutes);
#if VERSION_EU
    {
        /* "ＤＣＢ［%s］%s：%s", and eu's leftover byte after it */
        static const char format[20] = "\x82" "c" "\x82" "b" "\x82" "a" "\x81" "m%s" "\x81" "n%s" "\x81" "F%s\0l";

        sprintf(title, format, file, hours, minutes);
    }
#elif VERSION_US
    /* "ＤＣＢ［%s］%s：%s" */
    sprintf(title, "\x82" "c" "\x82" "b" "\x82" "a" "\x81" "m%s" "\x81" "n%s" "\x81" "F%s", file, hours, minutes);
#else
#error "openseg/memcard/open_save: version not checked"
#endif
    header->magic[0] = 'S';
    header->magic[1] = 'C';
    header->type = 0x13;
    header->blocks = 2;
    for (i = 0; i < 64; i++) {
        header->title[i] = 0;
    }
    strcpy(header->title, title);
    for (i = 0; i < 28; i++) {
        header->reserve[i] = 0;
    }
    header->clut = clut;
#if VERSION_EU
    /* eu shows the first frame three times: its icon doesn't move */
    header->icon.frames[0] = icon.frames[0];
    header->icon.frames[1] = icon.frames[0];
    header->icon.frames[2] = icon.frames[0];
#elif VERSION_US
    header->icon = icon;
#else
#error "openseg/memcard/open_save: version not checked"
#endif
}

u8 OPEN_checkSaveIsCurrent(s32 player, s32 port, s32 slot) {
    SaveSlot *save;
    s32 status;

    save = &OPEN_MEMCARD.slots[port][slot];
    status = OPEN_checkMemoryCard(port);
    if (status == 0) {
        scanMemoryCardFiles(port);
        OPEN_readSavePreview(port, slot);
        if (OPEN_MEMCARD.empty[port][slot] == 0) {
            if ((u16)((PlayerProfile *)PLAYER_PROFILES)[player].profileId != save->profileId) {
                return 0;
            }
            return 3;
        }
        OPEN_MEMCARD_MESSAGE_PORT = port;
    }
    return status;
}
