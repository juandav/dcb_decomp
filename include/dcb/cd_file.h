#ifndef DCB_CD_FILE_H
#define DCB_CD_FILE_H

#include "game.h"

typedef struct {
    /* 0x00 */ s32 key;
    /* 0x04 */ s32 sector;
    /* 0x08 */ s32 size;
    /* 0x0C */ u8 unkC[4];
    /* 0x10 */ s32 name[4];
} FileEntry;

extern CdFile DISC_FILES[4];
extern s32 DRIVE_DIRECTORY_CACHED;
extern s32 DRIVE_DIRECTORY;
extern FileEntry ROOT_DIRECTORY_ENTRY;
extern s32 DRIVE_SECTOR;
extern s32 DRIVE_SIZE;

void initDiscDrive(void);
int closeAllDiscFiles(void);
CdFile *openDiscFile(s8 *path, s32 openMode);
s32 closeDiscFile(CdFile *file);
s32 readDiscFile(CdFile *file, s32 size, u8 *dst);
s32 mountDrive(s32 path);
FileEntry *findDirectoryEntryOnDisc(CdFile *file, char *name, s32 key);
FileEntry *findDirectoryEntryInCache(char *name, s32 key);
s32 readDiscFileByte(CdFile *file);
s32 readDiscFileU16(CdFile *file);
s32 readDiscFileU32(CdFile *file);
s8 *readDiscFileLine(s8 *line, s32 maxLength, CdFile *file);

#endif /* DCB_CD_FILE_H */
