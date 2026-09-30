#ifndef DCB_TASK_H
#define DCB_TASK_H

#include "game.h"
#include "dcb/main.h"

void setTaskVsyncMode(s32 vsyncMode);
long handleVsyncPreemption();
s32 startTaskScheduler(s32 mode, s32 stackSize, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3);
s32 createTask(s32 taskId, s32 insertPos, s32 priority, s32 stackSize, s32 unused, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3);
s32 killTask(s32 taskId);
Task *selectNextTask(Task *current);
void exitCurrentTask(void);
int killOtherTasks(void);
s32 wakeTask(s32 taskId, s32 result);
s32 getTaskWaitFrames(s32 taskId);
s32 getCurrentTaskId();

#endif /* DCB_TASK_H */
