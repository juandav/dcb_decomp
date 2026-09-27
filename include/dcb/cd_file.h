#ifndef DCB_CD_FILE_H
#define DCB_CD_FILE_H

#include "game.h"

typedef struct {
    /* 0x0000 */ int unk0;
    /* 0x0004 */ char unk4[0x102C];
} Unk80081710;
typedef struct {
    /* 0x00 */ s32 key;
    /* 0x04 */ s32 sector;
    /* 0x08 */ s32 size;
    /* 0x0C */ u8 unkC[4];
    /* 0x10 */ s32 name[4];
} FileEntry;

extern Unk80081710 D_80081710[4];
extern s32 D_800857D0;
extern s32 D_800857E0;
extern FileEntry D_8006DD50;
extern s32 D_800897E0;
extern s32 D_800897E4;

void func_800157B0(void);
int func_80015EDC(void);
CdFile *func_80015AD8(s8 *path, s32 mode);
s32 func_80015EAC(CdFile *f);
s32 func_80015F34(CdFile *f, s32 size, u8 *dst);
s32 func_80015848(s32 arg0);
FileEntry *func_800158B0(CdFile *f, char *name, s32 key);
FileEntry *func_80015A3C(char *name, s32 key);
s32 func_800161D8(CdFile *f);
s32 func_800162F0(CdFile *f);
s32 func_80016500(CdFile *f);
s8 *func_80016724(s8 *buf, s32 n, CdFile *f);

#endif /* DCB_CD_FILE_H */
