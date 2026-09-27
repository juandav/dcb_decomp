#ifndef DCB_MAIN_H
#define DCB_MAIN_H

#include "game.h"

typedef struct {
    /* 0x00 */ char unk0[0x14];
    /* 0x14 */ int unk14;
} Unk80077A0C;
typedef struct Thread {
    /* 0x00 */ u32 flags;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ struct Thread *next;
    /* 0x0C */ struct Thread *prev;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 stack;
    /* 0x20 */ s32 regs[40];
} Thread;

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
extern Unk80077A0C *CURRENT_TASK;
extern void *PREEMPTED_TASK;
extern void *D_80077A14;
extern s32 TASK_GP;

int main(void);
void setTaskVsyncMode(s32 vsyncMode);
long handleVsyncPreemption();
s32 startTaskScheduler(s32 mode, s32 stackSize, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3);
s32 createTask(s32 taskId, s32 insertPos, s32 priority, s32 stackSize, s32 unused, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3);
s32 killTask(s32 taskId);
void *selectNextTask(void *current);
void exitCurrentTask(void);
int killOtherTasks(void);
s32 wakeTask(s32 taskId, s32 result);
s32 getTaskWaitFrames(s32 taskId);
s32 getCurrentTaskId();

#endif /* DCB_MAIN_H */
