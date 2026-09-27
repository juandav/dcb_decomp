#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/memcard.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/main.h"
#include "dcb/task.h"
#include "dcb/sound.h"
#include "dcb/opening_movie.h"
#include "dcb/sound_play.h"

s32 PLAYER_PROFILES = 0;
void *D_8006E054 = 0;

void playMenuSound(u32 kind) {
    s32 sound;

    sound = 0;
    if (D_801D813C == 0) {
        switch (kind) {
        case 0:
            sound = 0xA1;
            break;
        case 1:
            sound = 0xA0;
            break;
        case 2:
            sound = 0xA2;
            break;
        case 3:
            sound = 0xA3;
            break;
        case 4:
            sound = 0xA4;
            break;
        }
    } else {
        switch (kind) {
        case 0:
            sound = 1;
            break;
        case 1:
            sound = 0;
            break;
        case 2:
            sound = 2;
            break;
        case 3:
            sound = 3;
            break;
        case 4:
            sound = 4;
            break;
        }
    }
    playSoundEffect(sound);
}

s32 isMusicIdle(void) {
    s32 idle;

    idle = 0;
    if (MUSIC_CHANGE_BUSY == 0) {
        idle = PENDING_MUSIC_CHANGES == 0;
    }
    return idle;
}

void initMemoryCard(void) {
    InitCARD(0);
    startMemoryCardEvents();
}

void startMemoryCardEvents(void) {
    s32 i;

    VSync(2);
    MEMORY_CARD_EVENT_DONE = func_8006A794(0xF4000001, 4, 0x2000, 0);
    MEMORY_CARD_EVENT_ERROR = func_8006A794(0xF4000001, 0x8000, 0x2000, 0);
    MEMORY_CARD_EVENT_TIMEOUT = func_8006A794(0xF4000001, 0x100, 0x2000, 0);
    MEMORY_CARD_EVENT_NEW_CARD = func_8006A794(0xF4000001, 0x2000, 0x2000, 0);
    MEMORY_CARD_HW_EVENT_DONE = func_8006A794(0xF0000011, 4, 0x2000, 0);
    MEMORY_CARD_HW_EVENT_ERROR = func_8006A794(0xF0000011, 0x8000, 0x2000, 0);
    MEMORY_CARD_HW_EVENT_TIMEOUT = func_8006A794(0xF0000011, 0x100, 0x2000, 0);
    MEMORY_CARD_HW_EVENT_NEW_CARD = func_8006A794(0xF0000011, 0x2000, 0x2000, 0);
    StartCARD();
    func_80068804();
    func_8006A7C4(MEMORY_CARD_EVENT_DONE);
    func_8006A7C4(MEMORY_CARD_EVENT_ERROR);
    func_8006A7C4(MEMORY_CARD_EVENT_TIMEOUT);
    func_8006A7C4(MEMORY_CARD_EVENT_NEW_CARD);
    func_8006A7C4(MEMORY_CARD_HW_EVENT_DONE);
    func_8006A7C4(MEMORY_CARD_HW_EVENT_ERROR);
    func_8006A7C4(MEMORY_CARD_HW_EVENT_TIMEOUT);
    func_8006A7C4(MEMORY_CARD_HW_EVENT_NEW_CARD);
    for (i = 0; i < 2; i++) {
        MEMORY_CARD_DIRECTORIES[i] = allocPermanentHeapBlock(0x260);
    }
    MEMORY_CARD_SAVE_HEADER = allocPermanentHeapBlock(0x200);
}

s32 waitForMemoryCardEvent(s32 waitFrames) {
    s32 tries;

    tries = 0;
    D_801D8198 = 0;
    do {
        if (func_8006A7B4(MEMORY_CARD_EVENT_DONE) == 1) {
            return 0;
        }
        if (func_8006A7B4(MEMORY_CARD_EVENT_ERROR) == 1) {
            return 1;
        }
        if (func_8006A7B4(MEMORY_CARD_EVENT_TIMEOUT) == 1) {
            return 2;
        }
        if (func_8006A7B4(MEMORY_CARD_EVENT_NEW_CARD) == 1) {
            return 3;
        }
        if (waitFrames != 0) {
            if (tries++ >= 0x1F) {
                break;
            }
            func_80014C08(waitFrames);
        }
    } while (D_801D8198 < 0x259);
    return 2;
}

void clearMemoryCardEvents(void) {
    func_8006A7B4(MEMORY_CARD_EVENT_DONE);
    func_8006A7B4(MEMORY_CARD_EVENT_ERROR);
    func_8006A7B4(MEMORY_CARD_EVENT_TIMEOUT);
    func_8006A7B4(MEMORY_CARD_EVENT_NEW_CARD);
}

s32 waitForMemoryCardHwEvent(s32 waitFrames) {
    s32 tries;

    tries = 0;
    D_801D8198 = 0;
    do {
        if (func_8006A7B4(MEMORY_CARD_HW_EVENT_DONE) == 1) {
            return 0;
        }
        if (func_8006A7B4(MEMORY_CARD_HW_EVENT_ERROR) == 1) {
            return 1;
        }
        if (func_8006A7B4(MEMORY_CARD_HW_EVENT_TIMEOUT) == 1) {
            return 2;
        }
        if (func_8006A7B4(MEMORY_CARD_HW_EVENT_NEW_CARD) == 1) {
            return 3;
        }
        if (waitFrames != 0) {
            if (tries++ >= 0x1F) {
                break;
            }
            func_80014C08(waitFrames);
        }
    } while (D_801D8198 < 0x259);
    return 2;
}

void clearMemoryCardHwEvents(void) {
    func_8006A7B4(MEMORY_CARD_HW_EVENT_DONE);
    func_8006A7B4(MEMORY_CARD_HW_EVENT_ERROR);
    func_8006A7B4(MEMORY_CARD_HW_EVENT_TIMEOUT);
    func_8006A7B4(MEMORY_CARD_HW_EVENT_NEW_CARD);
}

s32 ensureMemoryCardReady(s32 port) {
    s32 channel;
    s32 event;
    s32 retries;

    retries = 0;
loop_1:
    clearMemoryCardEvents();
    func_80068814(port * 0x10);
    event = waitForMemoryCardEvent(0);
    if ((u32) (event - 1) < 2U) {
        if (retries >= 5) {
            return 1;
        }
        goto block_6;
    }
    if (event == 3) {
        if (retries < 3) {
block_6:
            retries += 1;
            func_80014C08(FRAME_INTERVAL);
            goto loop_1;
        }
        if (event == 3) {
            channel = port * 0x10;
            clearMemoryCardHwEvents();
            _card_clear(channel);
            waitForMemoryCardHwEvent(1);
            clearMemoryCardEvents();
            func_80068824(channel);
            waitForMemoryCardEvent(0);
        }
        /* Duplicate return node #9. Try simplifying control flow for better match */
        return 0;
    }
    return 0;
}

s32 getMemoryCardStatus(s32 port) {
    s32 event;
    s32 tries;
    s32 retry;

    tries = 0;
    retry = 0;
loop:
    clearMemoryCardEvents();
    func_80068814(port * 16);
    event = waitForMemoryCardEvent(1);
    if (event == 1 || event == 2) {
        if (retry >= 5) {
            return 1;
        }
        retry++;
    } else {
        if (event == 3) {
            if (retry < 3) {
                retry++;
                goto wait;
            }
            retry++;
            if (event == 3) {
                clearMemoryCardHwEvents();
                _card_clear(port * 16);
                event = waitForMemoryCardHwEvent(1);
                if (event == 1 || event == 2) {
                    retry = 0;
                    if (tries >= 5) {
                        return 1;
                    }
                    tries++;
                    goto wait;
                }
            }
        }
        clearMemoryCardEvents();
        func_80068824(port * 16);
        event = waitForMemoryCardEvent(0);
        if (event == 0) {
            goto done;
        }
        retry = 0;
        if (tries >= 5) {
            if (event == 3) {
                return 2;
            }
            return 1;
        }
        tries++;
    }
wait:
    func_80014C08(4);
    goto loop;
done:
    return 0;
}

s32 formatMemoryCard(s32 port) {
    return _card_format(port * 0x10) == 1;
}

s32 startMemoryCardSave(s32 port, u8 blocks, s32 data, s32 fileName, McHeader *header) {
    char name[32];
    s32 fd;

    ((u8 *)header)[3] = blocks;
    sprintf(name, &FMT_MEMORY_CARD_FILE_PATH, port, fileName);
    func_8006A864(func_8006A824(name, (((u8 *)header)[3] << 16) | 0x200));
    *(McHeader *)MEMORY_CARD_SAVE_HEADER = *header;
    MEMORY_CARD_FILE = fd = func_8006A824(name, 0x8002);
    if (fd == -1) {
        return -1;
    }
    MEMORY_CARD_TRANSFER_STEP = 0;
    MEMORY_CARD_TRANSFER_DATA = data;
    if (ensureMemoryCardReady(port) != 0) {
        return -1;
    }
    return 0;
}

s32 stepMemoryCardSave(void) {
    s32 event;
    s32 dataOffset;
    s32 fileSize;
    s32 off;

    clearMemoryCardEvents();
    switch (MEMORY_CARD_TRANSFER_STEP) {
    case 0:
        dataOffset = MEMORY_CARD_SAVE_HEADER[2] * 128 - 0x780;
        func_8006A834(MEMORY_CARD_FILE, 0, 0);
        if (func_8006A854(MEMORY_CARD_FILE, MEMORY_CARD_SAVE_HEADER, dataOffset) == -1) {
            return -1;
        }
        MEMORY_CARD_TRANSFER_STEP++;
    case 1:
        event = waitForMemoryCardEvent(1);
        if (event == 1 || event == 2) {
            func_8006A864(MEMORY_CARD_FILE);
            return -1;
        }
        dataOffset = MEMORY_CARD_SAVE_HEADER[2] * 128 - 0x780;
        fileSize = MEMORY_CARD_SAVE_HEADER[3] * 0x2000;
        MEMORY_CARD_SECTORS_TOTAL = (fileSize - dataOffset) / 128;
        MEMORY_CARD_SECTORS_DONE = 0;
        MEMORY_CARD_TRANSFER_STEP++;
        return 0;
    case 2:
        func_8006A834(MEMORY_CARD_FILE, ((MEMORY_CARD_SAVE_HEADER[2] - 0x10) << 7) + 0x80 + (MEMORY_CARD_SECTORS_DONE << 7), 0);
        if (func_8006A854(MEMORY_CARD_FILE, (void *)(MEMORY_CARD_TRANSFER_DATA + (MEMORY_CARD_SECTORS_DONE << 7)), 0x80) == -1) {
            return -1;
        }
        MEMORY_CARD_TRANSFER_STEP++;
    case 3:
        event = waitForMemoryCardEvent(1);
        if (event == 1 || event == 2) {
            func_8006A864(MEMORY_CARD_FILE);
            return -1;
        }
        MEMORY_CARD_SECTORS_DONE++;
        MEMORY_CARD_TRANSFER_STEP = 2;
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            func_8006A864(MEMORY_CARD_FILE);
        }
        break;
    }
    return MEMORY_CARD_SECTORS_DONE * 100 / MEMORY_CARD_SECTORS_TOTAL;
}

s32 startMemoryCardLoad(s32 port, s32 data, s32 fileName) {
    char name[32];
    s32 fd;

    sprintf(name, &FMT_MEMORY_CARD_FILE_PATH, port, fileName);
    MEMORY_CARD_FILE = fd = func_8006A824(name, 0x8001);
    if (fd == -1) {
        return -1;
    }
    MEMORY_CARD_TRANSFER_STEP = 0;
    MEMORY_CARD_TRANSFER_DATA = data;
    if (ensureMemoryCardReady(port) != 0) {
        return -1;
    }
    return 0;
}

s32 stepMemoryCardLoad(void) {
    s32 event;
    s32 dataOffset;
    s32 fileSize;
    s32 off;

    clearMemoryCardEvents();
    switch (MEMORY_CARD_TRANSFER_STEP) {
    case 0:
        func_8006A834(MEMORY_CARD_FILE, 0, 0);
        if (func_8006A844(MEMORY_CARD_FILE, MEMORY_CARD_SAVE_HEADER, 0x80) == -1) {
            return -1;
        }
        MEMORY_CARD_TRANSFER_STEP++;
    case 1:
        event = waitForMemoryCardEvent(1);
        if (event == 1 || event == 2) {
            func_8006A864(MEMORY_CARD_FILE);
            return -1;
        }
        dataOffset = MEMORY_CARD_SAVE_HEADER[2] * 128 - 0x780;
        fileSize = MEMORY_CARD_SAVE_HEADER[3] * 0x2000;
        MEMORY_CARD_SECTORS_TOTAL = (fileSize - dataOffset) / 128;
        MEMORY_CARD_SECTORS_DONE = 0;
        MEMORY_CARD_TRANSFER_STEP++;
        return 0;
    case 2:
        func_8006A834(MEMORY_CARD_FILE, ((MEMORY_CARD_SAVE_HEADER[2] - 0x10) << 7) + 0x80 + (MEMORY_CARD_SECTORS_DONE << 7), 0);
        if (func_8006A844(MEMORY_CARD_FILE, (void *)(MEMORY_CARD_TRANSFER_DATA + (MEMORY_CARD_SECTORS_DONE << 7)), 0x80) == -1) {
            return -1;
        }
        MEMORY_CARD_TRANSFER_STEP++;
    case 3:
        event = waitForMemoryCardEvent(1);
        if (event == 1 || event == 2) {
            func_8006A864(MEMORY_CARD_FILE);
            return -1;
        }
        MEMORY_CARD_SECTORS_DONE++;
        MEMORY_CARD_TRANSFER_STEP = 2;
        if (MEMORY_CARD_SECTORS_DONE == MEMORY_CARD_SECTORS_TOTAL) {
            func_8006A864(MEMORY_CARD_FILE);
        }
        break;
    }
    return MEMORY_CARD_SECTORS_DONE * 100 / MEMORY_CARD_SECTORS_TOTAL;
}

s32 readMemoryCardSavePreview(s32 port, void *dst, s32 fileName) {
    char name[32];
    s32 fd;

    sprintf(name, &FMT_MEMORY_CARD_FILE_PATH, port, fileName);
    fd = func_8006A824(name, 1);
    if (fd == -1) {
        return 1;
    }
    if (func_8006A844(fd, MEMORY_CARD_SAVE_HEADER, 0x80) == -1) {
        func_8006A864(fd);
        return 1;
    }
    if (func_8006A834(fd, ((*(u8 *)((s8 *)MEMORY_CARD_SAVE_HEADER + 2)) - 0x10) << 7, 1) == -1) {
        func_8006A864(fd);
        return 1;
    }
    if (func_8006A844(fd, dst, 0x80) == -1) {
        func_8006A864(fd);
        return 1;
    }
    func_8006A864(fd);
    return 0;
}

INCLUDE_RODATA("asm/main/nonmatchings/system/memcard", FMT_MEMORY_CARD_FILE_PATH);

void scanMemoryCardFiles(s32 port) {
    char name[8];
    DirEntry *entry;
    s32 count;
    s32 total;

    count = 0;
    total = 0;
    sprintf(name, "bu%1d0:*", port);
    entry = MEMORY_CARD_DIRECTORIES[port]->files;
    if (firstfile(name, entry) == entry) {
        do {
            total += entry->size;
            count++;
            entry++;
        } while (func_8006A874(entry) == entry);
    }
    MEMORY_CARD_DIRECTORIES[port]->count = count;
    MEMORY_CARD_DIRECTORIES[port]->blocks = total /= 8192;
}

/* the rank titles, lowest first */
char *STR_TAMER_RANKS[8] = {
    "Beginner Tamer", "Regular Tamer", "Mid Level Tamer", "High Level Tamer",
    "Expert Tamer", "Master Tamer", "Genius Tamer", "Invincible Tamer",
};
char *STR_COLLECTOR_RANKS[8] = {
    "General Public", "Hobby Collector", "Serious Collector", "Top Level Collector",
    "Famous Collector", "Great Collector", "Perfect Collector", "Legendary Collector",
};
char *STR_BATTLE_RANKS[8] = {
    "Battle Beginner", "Battle Expert", "Battle Specialist", "Battle Champion",
    "Battle Master", "Battle Lord", "Battle King", "Battle Emperor",
};
u8 COMPLETE_SET_CARD_COUNTS[6] = { 0x22, 0x23, 0x22, 0x24, 0x21, 0x6E };

s32 verifySaveChecksum(s32 len, u8 *data) {
    s32 i;
    u8 xorSum = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        xorSum ^= *data;
        sum += *data;
        data++;
    }
    if (data[0] != xorSum || data[1] != sum) {
        return 1;
    }
    return 0;
}

void writeSaveChecksum(s32 len, u8 *data) {
    s32 i;
    u8 xorSum = 0;
    u8 sum = 0;

    for (i = 0; i < len; i++) {
        xorSum ^= *data;
        sum += *data;
        data++;
    }
    data[0] = xorSum;
    data[1] = sum;
}
