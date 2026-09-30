#ifndef DCB_MAIN_H
#define DCB_MAIN_H

#include "game.h"

/* The low half of a task's flags is its priority: 0 is the main task, and
 * 0xFFFF the end of the list. */
#define TASK_IN_USE 0x80000000        /* the slot holds a task (flags < 0) */
#define TASK_CONTEXT_SAVED 0x20000000 /* every register is saved (a new or preempted task), not only the s registers */
/* the Status register a task starts with: COP2 usable, every interrupt
 * unmasked, and interrupts enabled once rfe pops the stack */
#define TASK_START_SR 0x4000FF04
typedef union {
    /* 0x00 */ s32 flags;
    /* 0x00 */ s16 priority;
} TaskStatus;

/* A task: a kernel thread context that the scheduler switches by hand. The
 * tasks form a list ordered by priority, walked through `next`. */
typedef struct Task {
    /* 0x00 */ TaskStatus status; /* bit 31: slot in use, bit 29: context saved */
    /* 0x04 */ s32 waitFrames;
    /* 0x08 */ struct Task *prev;
    /* 0x0C */ struct Task *next;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 id; /* also the tag of the task's heap blocks */
    /* 0x18 */ s32 wakeResult;
    /* 0x1C */ s32 stack;
    /* 0x20 */ s32 regs[40]; /* as in the kernel's TCB, indexed by R_* (asm.h) */
} Task;

extern s32 TASK_VSYNC_MODE;
extern Task TASKS[32];
extern s16 CURRENT_TASK_PRIORITY;
extern s16 PREEMPTED_TASK_PRIORITY;
extern s16 DEFERRED_TASK_PRIORITY;
extern Task TASK_LIST_END;
extern struct TCB *KERNEL_TCB;
extern s32 VSYNC_EVENT;
extern Task *CURRENT_TASK;
extern Task *PREEMPTED_TASK;
extern Task *DEFERRED_TASK;
extern s32 TASK_GP;

int main(void);

#endif /* DCB_MAIN_H */
