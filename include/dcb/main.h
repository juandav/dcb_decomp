#ifndef DCB_MAIN_H
#define DCB_MAIN_H

#include "game.h"

typedef struct {
    /* 0x00 */ char unk0[0x14];
    /* 0x14 */ int id;
} TaskHeader;
typedef struct Task {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 waitFrames;
    /* 0x08 */ struct Task *next;
    /* 0x0C */ struct Task *prev;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 id;
    /* 0x18 */ s32 wakeResult;
    /* 0x1C */ s32 stack;
    /* 0x20 */ s32 regs[40];
} Task;

extern s32 TASK_VSYNC_MODE;
extern s16 *D_80077AEC;
extern s16 TASKS;
extern s16 CURRENT_TASK_PRIORITY;
extern s16 PREEMPTED_TASK_PRIORITY;
extern s16 D_80077A1C;
extern s32 TASK_LIST_END;
extern s32 KERNEL_TCB;
extern s32 VSYNC_EVENT;
extern s32 D_80077BC0;
extern TaskHeader *CURRENT_TASK;
extern void *PREEMPTED_TASK;
extern void *D_80077A14;
extern s32 TASK_GP;

int main(void);

#endif /* DCB_MAIN_H */
