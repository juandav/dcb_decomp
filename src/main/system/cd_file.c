#include "common.h"
#include "gte.h"
#include "game.h"
#include "dcb/cd_file.h"
#include "dcb/text.h"
#include "dcb/str_util.h"

/* a path's characters go to toupper as they are in jp, as signed chars in
   us and eu, and eu takes toupper's result as a signed char too */
#if VERSION_JP
#define TO_UPPER(ch) toupper(ch)
#elif VERSION_US
#define TO_UPPER(ch) toupper((s8)(ch))
#elif VERSION_EU
#define TO_UPPER(ch) (s8)toupper((s8)(ch))
#endif

/* after reading the file's last sector: the bytes of it that are the
   file's, worked out from what is left to read (us) or taken off the whole
   sector (jp, eu) */
#if VERSION_JP || VERSION_EU
#define TRIM_LAST_SECTOR(file) ((file)->avail += (file)->remaining)
#elif VERSION_US
#define TRIM_LAST_SECTOR(file) ((file)->avail = (file)->remaining + 0x1000)
#endif

FileEntry ROOT_DIRECTORY_ENTRY = { 0 };

void initDiscDrive(void) {
    u8 cdMode[8];
    CdFile *file;
    s32 i;

    ResetCallback();
    while (CdInit() == 0) {
    }
    VSync(4);
    for (;;) {
        cdMode[0] = 0x80;
        if (CdControlB(0xE, cdMode, 0) != 0) {
            break;
        }
        VSync(0);
    }
    VSync(4);
    CdSetDebug(0);
    file = DISC_FILES;
    for (i = 3; i >= 0; i--, file++) {
        file->openMode = 0;
    }
    DRIVE_DIRECTORY_CACHED = 0;
}

s32 mountDrive(s32 path) {
    CdFile *file;

    file = openDiscFile((s8 *)path, 0);
    if (file != 0 && readDiscFile(file, 0x4000, (u8 *)&DRIVE_DIRECTORY) != 0) {
        closeDiscFile(file);
        DRIVE_DIRECTORY_CACHED = 1;
        return 0;
    }
    return 1;
}

FileEntry *findDirectoryEntryOnDisc(CdFile *file, char *name, s32 key) {
    u8 cdResult[8];
    FileEntry *entry;
    s32 bytesLeft;
    s32 i;
    s32 readStatus;

    if (key == 0x80 && *(s32 *)name == 0) {
        return &ROOT_DIRECTORY_ENTRY;
    }
    for (;;) {
        if (file->remaining <= 0) {
            return 0;
        }
        CdIntToPos(file->sector, file->loc);
        entry = (FileEntry *)file->buf;
        do {
            while (CdControlB(2, file->loc, cdResult) == 0) {
            }
            while (CdRead(2, (u8 *)entry, 0x80) == 0) {
            }
            while ((readStatus = CdReadSync(1, 0)) > 0) {
                waitFrames(1);
            }
        } while (readStatus != 0);
        file->sector += 2;
        bytesLeft = 0x1000;
        if ((file->remaining -= 0x1000) < 0) {
            bytesLeft = file->remaining + 0x1000;
        }
        for (; bytesLeft > 0; bytesLeft -= 0x20, entry++) {
            if (entry->key == 0) {
                return 0;
            }
            if (entry->key == key) {
                /* the 16-byte names are compared a word at a time */
                for (i = 0; i < 4; i++) {
                    if (entry->name[i] != *(s32 *)(name + (i << 2))) {
                        break;
                    }
                }
                if (i == 4) {
                    return entry;
                }
            }
        }
    }
}

FileEntry *findDirectoryEntryInCache(char *name, s32 key) {
    FileEntry *entry;
    s32 entriesLeft;
    s32 i;

    entry = (FileEntry *)&DRIVE_DIRECTORY;
    for (entriesLeft = 0x1FF; entriesLeft >= 0; entriesLeft--, entry++) {
        if (entry->key == 0) {
            return 0;
        }
        if (entry->key == key) {
            for (i = 0; i < 4; i++) {
                if (entry->name[i] != *(s32 *)(name + (i << 2))) {
                    break;
                }
            }
            if (i == 4) {
                return entry;
            }
        }
    }
    return 0;
}

CdFile *openDiscFile(s8 *path, s32 openMode) {
    s32 entryName[4];
    char drivePath[16];
    char *drivePathCursor;
    CdFile *file;
    FileEntry *entry;
    s32 i;
    s32 extensionKey;
    s32 sector;
    s32 size;
    u8 *cursor;
    s32 ch;

retry:
    file = DISC_FILES;
    for (i = 3; i >= 0; i--, file++) {
        if (file->openMode == 0) {
            break;
        }
    }
    if (i < 0) {
        return 0;
    }
    cursor = (u8 *)path;
    if (cursor[1] != ':') {
        if (DRIVE_DIRECTORY_CACHED == 0) {
            return 0;
        }
        sector = DRIVE_SECTOR;
        size = 0;
    } else {
        drivePathCursor = drivePath;
        *drivePathCursor++ = '\\';
        *drivePathCursor++ = TO_UPPER(*cursor);
        cursor += 2;
        copyString((s8 *)drivePathCursor, (s8 *)".DRV;1");
        if (CdSearchFile(file->loc, drivePath) == 0) {
            return 0;
        }
        size = file->fsize;
        file->remaining = size;
        sector = CdPosToInt(file->loc);
        file->sector = sector;
        if (openMode == 0) {
            DRIVE_SECTOR = sector;
            DRIVE_SIZE = size;
        }
    }
    for (;;) {
        for (i = 0; i < 4; i++) {
            entryName[i] = 0;
        }
        extensionKey = 0;
        for (i = 0; i < 16; i++) {
            switch (ch = *cursor++) {
            case '.':
                goto ext;
            case 0:
                goto end;
            case '\\':
                goto dir;
            }
            ((u8 *)entryName)[i] = TO_UPPER(ch);
        }
        while ((ch = *cursor++) != '.') {
            if (ch == 0) {
                goto end;
            }
            if (ch == '\\') {
                goto dir;
            }
        }
    ext:
        for (i = 0; i < 24; i += 8) {
            ch = *cursor++;
            if (ch == 0) {
                goto end;
            }
            if (ch == '\\') {
                goto dir;
            }
            extensionKey += TO_UPPER(ch) << i;
        }
        break;
    dir:
        if (size == 0) {
            size = DRIVE_SIZE;
            entry = findDirectoryEntryInCache((char *)entryName, 0x80);
            if (entry == 0) {
                return 0;
            }
        } else {
            entry = findDirectoryEntryOnDisc(file, (char *)entryName, 0x80);
            if (entry == 0) {
                return 0;
            }
        }
        file->sector = sector + entry->sector;
        file->remaining = size - (entry->sector << 11);
    }
end:
    if (openMode == 0) {
        if (size == 0) {
            entry = findDirectoryEntryInCache((char *)entryName, 0x80);
        } else {
            entry = findDirectoryEntryOnDisc(file, (char *)entryName, 0x80);
        }
        if (entry == 0) {
            return 0;
        }
        file->sector = sector + entry->sector;
        file->avail = 0;
        file->remaining = 0x4000;
    } else {
        if (size == 0) {
            size = DRIVE_SIZE;
            entry = findDirectoryEntryInCache((char *)entryName, (extensionKey << 8) + 1);
        } else {
            entry = findDirectoryEntryOnDisc(file, (char *)entryName, (extensionKey << 8) + 1);
        }
        if (entry == 0) {
            return 0;
        }
        file->sector = sector + entry->sector;
        file->remaining = size - (entry->sector << 11);
        file->avail = 0;
        if ((file->remaining = file->size = entry->size) == 0) {
            goto retry;
        }
        file->openMode = openMode;
    }
    return file;
}

s32 closeDiscFile(CdFile *file) {
    file->openMode = 0;
    return CdSync(0, 0) == 5;
}

int closeAllDiscFiles(void) {
    CdFile *file = DISC_FILES;
    int i;

    for (i = 3; i >= 0; i--, file++) {
        if (file->openMode > 0) {
            file->openMode = 0;
        }
    }
    return CdSync(0, 0) == 5;
}

s32 readDiscFile(CdFile *file, s32 size, u8 *dst) {
    u8 cdResult[8];
    u8 cdResult2[8];
    s32 total;
    s32 sectors;
    s32 chunkSize;
    s32 readStatus;
    u8 *src;

    total = 0;
    chunkSize = file->avail;
    if (chunkSize > 0) {
        if (size < chunkSize) {
            chunkSize = size;
        }
        total = chunkSize;
        file->avail -= total;
        size -= total;
        src = file->cur;
        for (chunkSize = total - 4; chunkSize >= 0; chunkSize -= 4) {
            *(s32 *)dst = *(s32 *)src;
            src += 4;
            dst += 4;
        }
        file->cur = src;
    }
    if (file->remaining < size) {
        size = file->remaining;
    }
    if (size <= 0 || file->remaining <= 0) {
        return total;
    }
    sectors = size / 0x800;
    if (sectors > 0) {
        CdIntToPos(file->sector, file->loc);
        do {
            while (CdControlB(2, file->loc, cdResult) == 0) {
            }
            while (CdRead(sectors, dst, 0x80) == 0) {
            }
            while ((readStatus = CdReadSync(1, 0)) > 0) {
                waitFrames(1);
            }
        } while (readStatus != 0);
        file->sector += sectors;
        dst += sectors << 11;
        chunkSize = sectors << 11;
        total += chunkSize;
        size -= chunkSize;
        file->remaining -= chunkSize;
    }
    if (size <= 0 || file->remaining <= 0) {
        return total;
    }
    CdIntToPos(file->sector, file->loc);
    do {
        while (CdControlB(2, file->loc, cdResult2) == 0) {
        }
        do {
            file->cur = file->buf;
        } while (CdRead(2, file->buf, 0x80) == 0);
        while ((readStatus = CdReadSync(1, 0)) > 0) {
            waitFrames(1);
        }
    } while (readStatus != 0);
    file->sector += 2;
    file->avail = 0x1000;
    if ((file->remaining -= 0x1000) < 0) {
        TRIM_LAST_SECTOR(file);
    }
    chunkSize = file->avail;
    if (chunkSize > 0) {
        if (size < chunkSize) {
            chunkSize = size;
        }
        total += chunkSize;
        file->avail -= chunkSize;
        src = file->cur;
        do {
            *(s32 *)dst = *(s32 *)src;
            src += 4;
            dst += 4;
            chunkSize -= 4;
        } while (chunkSize > 0);
        file->cur = src;
    }
    return total;
}

s32 readDiscFileByte(CdFile *file) {
    u8 cdResult[8];
    s32 readStatus;

    if (file->avail <= 0) {
        if (file->remaining <= 0) {
            return -1;
        }
        CdIntToPos(file->sector, file->loc);
        do {
            while (CdControlB(2, file->loc, cdResult) == 0) {
            }
            do {
                file->cur = file->buf;
            } while (CdRead(2, file->buf, 0x80) == 0);
            while ((readStatus = CdReadSync(1, 0)) > 0) {
                waitFrames(1);
            }
        } while (readStatus != 0);
        file->sector += 2;
        file->avail = 0x1000;
        if ((file->remaining -= 0x1000) < 0) {
            TRIM_LAST_SECTOR(file);
        }
        if (file->avail <= 0) {
            return -1;
        }
    }
    file->avail--;
    return *file->cur++;
}

s32 readDiscFileU16(CdFile *file) {
    u8 cdResult[8];
    s16 i;
    s16 value;
    s32 readStatus;

    if (file->avail < 2) {
        i = 0;
        value = 0;
        while (file->avail != 0 && i++ < 2) {
            value += *file->cur++ << ((i - 1) * 8);
            file->avail--;
        }
        if (file->remaining <= 0) {
            return -1;
        }
        CdIntToPos(file->sector, file->loc);
        do {
            while (CdControlB(2, file->loc, cdResult) == 0) {
            }
            do {
                file->cur = file->buf;
            } while (CdRead(2, file->buf, 0x80) == 0);
            while ((readStatus = CdReadSync(1, 0)) > 0) {
                waitFrames(1);
            }
        } while (readStatus != 0);
        file->sector += 2;
        file->avail = 0x1000;
        if ((file->remaining -= 0x1000) < 0) {
            TRIM_LAST_SECTOR(file);
        }
        if (file->avail + i < 2) {
            return -1;
        }
        while (file->avail != 0 && i++ < 2) {
            value += *file->cur++ << ((i - 1) * 8);
            file->avail--;
        }
        return value;
    }
    file->avail -= 2;
    /* sic: undefined order; the original reads one byte twice and advances once */
    return *file->cur++ + (*file->cur++ << 8);
}

s32 readDiscFileU32(CdFile *file) {
    u8 cdResult[8];
    s16 i;
    s32 value;
    s32 word;
    s32 readStatus;

    if (file->avail < 4) {
        i = 0;
        value = 0;
        while (file->avail != 0 && i++ < 4) {
            value |= *file->cur++ << ((i - 1) * 8);
            file->avail--;
        }
        if (file->remaining <= 0) {
            return -1;
        }
        CdIntToPos(file->sector, file->loc);
        do {
            while (CdControlB(2, file->loc, cdResult) == 0) {
            }
            do {
                file->cur = file->buf;
            } while (CdRead(2, file->buf, 0x80) == 0);
            while ((readStatus = CdReadSync(1, 0)) > 0) {
                waitFrames(1);
            }
        } while (readStatus != 0);
        file->sector += 2;
        file->avail = 0x1000;
        if ((file->remaining -= 0x1000) < 0) {
            TRIM_LAST_SECTOR(file);
        }
        if (file->avail + i < 4) {
            return -1;
        }
        while (file->avail != 0 && i++ < 4) {
            value |= *file->cur++ << ((i - 1) * 8);
            file->avail--;
        }
        return value;
    }
    file->avail -= 4;
    word = (((file->cur[3] << 8) + file->cur[2] << 8) + file->cur[1] << 8) + file->cur[0];
    file->cur += 4;
    return word;
}

s8 *readDiscFileLine(s8 *line, s32 maxLength, CdFile *file) {
    s8 *out;
    s32 ch;

    ch = 0;
    out = line;
    while (--maxLength > 0) {
        ch = readDiscFileByte(file);
        if (ch == -1) {
            break;
        }
        if (ch == 0) {
            break;
        }
        *out++ = ch;
        if (ch == '\n') {
            break;
        }
        if (ch == 0x1A) {
            break;
        }
    }
    /* skip the rest of a line that was too long */
    if (maxLength <= 0) {
        while (!(ch == -1 || ch == 0 || ch == '\n' || ch == 0x1A)) {
            ch = readDiscFileByte(file);
        }
    }
    *out = 0;
    if (*line == 0) {
        return 0;
    }
    return line;
}
