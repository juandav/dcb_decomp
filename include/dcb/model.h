#ifndef DCB_MODEL_H
#define DCB_MODEL_H

#include "game.h"

typedef struct {
    /* 0x00 */ s32 unk0[5];
    /* 0x14 */ s32 unk14;
} Obj18;
typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ s32 flags;
    /* 0x08 */ s32 nobj;
    /* 0x0C */ Obj18 obj[1];
} Tmd18;
typedef struct {
    s32 key;
    s32 value;
} KeyValue;

void func_80022C4C(void);
void func_80022CA4(s32 arg0);
void func_80022D00(s32 arg0);
void func_80022DBC(s32 arg0);
void func_80022E58(void);
s32 func_80022ED0(s32 key, s32 n, KeyValue **pp);
s32 func_80022F34(s32, s32, s32, Chunk *);
void func_80023094(Model2220 *, s32 *, s32);
s32 func_800230B8(s32, s32, s32, s32);
void func_80023128(s32 arg0, s32 arg1, s32 arg2);
void func_80023408(Tmd18 *t);
void func_80023454(s32 arg0, void *arg1, s32 arg2);
s32 *func_80023468(u8 *m, s32 *p);
void func_800235C8(s32 arg0);
void func_8002360C(void);
void *func_800236B4(s32 id);
s32 func_8002371C(u8 *);
void func_800234AC(Model *);
s32 func_8002386C(s32 slot, s32 id, s32 pos, s32 pak, s8 kind);
void func_80023DA4(s32 arg0, s32 arg1, s32 arg2);
void func_80023DC8(s32 arg0, s32 arg1, s32 arg2);
s32 func_80022D34(s32, s32, s32, s32);
void func_80023148(s32, s32);

#endif /* DCB_MODEL_H */
