#include "dcb/task.h"
#include "common.h"
#include "game.h"
#include "dcb/main.h"
#include "dcb/heap.h"
#include "dcb/angle.h"
#include "dcb/vblank.h"
#include "dcb/screen_copy.h"
#include "dcb/render_loop.h"
#include "dcb/boot.h"
#include <kernel.h>

/* The scheduler runs with the kernel's TCB: switching tasks means copying a
 * task's saved registers in or out of KERNEL_TCB->reg. */

void setTaskVsyncMode(s32 vsyncMode) {
    Task *task;
    s32 i;

    EnterCriticalSection();
    if (vsyncMode != 0) {
        if (TASK_VSYNC_MODE == 0) {
            TASK_VSYNC_MODE = 1;
            /* the list end now wraps around to the first task that isn't the main one */
            task = TASKS;
            for (i = 31; i >= 0; i--, task++) {
                if (task->status.priority > 0) {
                    TASK_LIST_END.next = task;
                    break;
                }
            }
        }
    } else if (TASK_VSYNC_MODE != 0) {
        TASK_VSYNC_MODE = 0;
        TASK_LIST_END.next = TASKS;
    }
    ExitCriticalSection();
}

s32 startTaskScheduler(s32 mode, s32 stackSize, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3) {
    Task *task;
    Task *mainTask;
    struct TCBH *pcb;
    unsigned long *src;
    s32 *dst;
    s32 i;
    s32 j;
    s32 stack;
    s32 vsyncEvent;

    EnterCriticalSection();
    TASK_VSYNC_MODE = mode;
    for (task = TASKS, i = 31; i >= 0; i--, task++) {
        task->status.flags = 0;
    }
    CURRENT_TASK_PRIORITY = PREEMPTED_TASK_PRIORITY = *(u16 *)&DEFERRED_TASK_PRIORITY = 0xFFFF;
    CURRENT_TASK = TASKS - 1;
    TASK_LIST_END.status.flags = TASK_IN_USE | 0xFFFF;
    TASK_LIST_END.prev = CURRENT_TASK + 1;
    if (TASK_VSYNC_MODE != 0) {
        TASK_LIST_END.next = &TASK_LIST_END;
    } else {
        TASK_LIST_END.next = TASKS;
    }
    TASK_LIST_END.id = -1;
    /* 0x108 holds the kernel's process control block, whose first word is
     * the running TCB */
    pcb = *(struct TCBH **)0x108;
    KERNEL_TCB = pcb->entry;
    src = KERNEL_TCB->reg;
    dst = TASKS[0].regs;
    for (j = 39; j >= 0; j--) {
        *dst++ = *src++;
    }
    /* the main task is TASKS[0], priority 0 */
    mainTask = TASKS;
    mainTask->status.flags = TASK_IN_USE | TASK_CONTEXT_SAVED;
    mainTask->regs[R_EPC] = entry;
    mainTask->regs[R_SR] = TASK_START_SR;
    mainTask->regs[R_A0] = a0;
    mainTask->regs[R_A1] = a1;
    mainTask->regs[R_A2] = a2;
    mainTask->regs[R_A3] = a3;
    mainTask->prev = &TASK_LIST_END;
    mainTask->next = &TASK_LIST_END;
    mainTask->id = 0;
    mainTask->wakeResult = 0;
    stack = (s32)allocHeapBlock(stackSize, -3);
    if (stack == 0) {
        return -6;
    }
    mainTask->stack = stack;
    mainTask->regs[R_SP] = stack + (stackSize & ~7) - 0x20;
    /* preempt the running task at every vsync (root counter 3) */
    vsyncEvent = OpenEvent(0xF2000003, 2, 0x1000, (long (*)())handleVsyncPreemption);
    EnableEvent(vsyncEvent);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
#if VERSION_US || VERSION_EU
    VSYNC_EVENT = vsyncEvent;
#endif
    ExitCriticalSection();
    return 0;
}

long handleVsyncPreemption(void) {
    Task *task;
    unsigned long *tcbRegs;
    s32 *regs;
    s32 i;

    task = CURRENT_TASK;
    tickVblankCounters();
    tcbRegs = KERNEL_TCB->reg;
    regs = task->regs;
    for (i = 39; i >= 0; i--) {
        *regs++ = *tcbRegs++;
    }
    task->status.flags |= TASK_CONTEXT_SAVED;
    if ((PREEMPTED_TASK_PRIORITY = CURRENT_TASK_PRIORITY) == 0) {
        if (TASK_VSYNC_MODE == 0) {
            DEFERRED_TASK = TASKS;
            DEFERRED_TASK_PRIORITY = 0;
        }
    } else {
        /* switch to the main task */
        PREEMPTED_TASK = task;
        task = TASKS;
        CURRENT_TASK = task;
        CURRENT_TASK_PRIORITY = TASKS[0].status.priority;
        tcbRegs = KERNEL_TCB->reg;
        regs = task->regs;
        for (i = 39; i >= 0; i--) {
            *tcbRegs++ = *regs++;
        }
    }
}

Task *selectNextTask(Task *current) {
    Task *next = current->next;
    s16 priority = next->status.priority;

    if (next->status.priority > 0 && priority == PREEMPTED_TASK_PRIORITY) {
        /* go back to the task that the vsync preempted */
        DEFERRED_TASK = next;
        DEFERRED_TASK_PRIORITY = priority;
        next = PREEMPTED_TASK;
        PREEMPTED_TASK_PRIORITY = -1;
    } else {
#if VERSION_US
        priority = next->status.priority;
#endif
        if ((u16)priority > (u16)DEFERRED_TASK_PRIORITY) {
            priority = DEFERRED_TASK_PRIORITY;
            next = DEFERRED_TASK;
            *(u16 *)&DEFERRED_TASK_PRIORITY = 0xFFFF;
        }
    }
    CURRENT_TASK = next;
    CURRENT_TASK_PRIORITY = priority;
    return next;
}

/* Starts a task in slot taskId (0: the first free one). insertPos < 0 places
 * it by priority; 0-31 right after that task and 32-63 right before task
 * insertPos - 32, clamping the priority so the list stays sorted. */
s32 createTask(s32 taskId, s32 insertPos, s32 priority, s32 stackSize, s32 unused, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3) {
    Task *task;
    Task *after;
    Task *before;
    s32 ret;
    s32 afterFlags;
    s32 beforeFlags;
    unsigned long *src;
    s32 *dst;
    s32 i;
    s32 stack;

    task = TASKS + taskId;
    if (taskId != 0) {
        if (task->status.flags < 0) {
            return -1;
        }
    } else {
        for (taskId++; taskId < 32; taskId++) {
            if ((++task)->status.flags >= 0) {
                break;
            }
        }
        if (taskId >= 32) {
            return -1;
        }
    }
    ret = 0;
    if (insertPos < 0) {
        after = TASKS;
        if (priority >= (u16)after->status.priority) {
            do {
                after = after->next;
            } while (priority >= (u16)after->status.priority);
        }
        goto link;
    }
    if (insertPos >= 32) {
        insertPos -= 32;
        after = TASKS + insertPos;
        if (after->status.flags >= 0) {
            return -3;
        }
        afterFlags = after->status.flags;
        if ((u16)afterFlags < priority) {
            ret = -0x86;
            priority = afterFlags;
        }
    link:
        before = after->prev;
        before->next = task;
    } else {
        before = TASKS + insertPos;
        if (before->status.flags >= 0) {
            return -3;
        }
        beforeFlags = before->status.flags;
        if (priority < (u16)beforeFlags) {
            ret = -0x86;
            priority = beforeFlags;
        }
        after = before->next;
        before->next = task;
    }
    after->prev = task;
    task->prev = before;
    task->next = after;
    if (TASK_VSYNC_MODE != 0 && priority > 0 && before->status.priority == 0) {
        TASK_LIST_END.next = task;
    }
    src = KERNEL_TCB->reg;
    dst = task->regs;
    for (i = 39; i >= 0; i--) {
        *dst++ = *src++;
    }
    task->status.flags = priority | TASK_IN_USE | TASK_CONTEXT_SAVED;
    task->regs[R_EPC] = entry;
    task->regs[R_SR] = TASK_START_SR;
    task->regs[R_A0] = a0;
    task->regs[R_A1] = a1;
    task->regs[R_A2] = a2;
    task->regs[R_A3] = a3;
    /* returning from the entry point ends the task */
    task->regs[R_RA] = (s32)exitTask;
    task->regs[R_GP] = TASK_GP;
    task->id = taskId;
    task->wakeResult = 0;
    stack = (s32)allocHeapBlock(stackSize, -3);
    if (stack == 0) {
        return -6;
    }
    task->stack = stack;
    task->regs[R_SP] = stack + (stackSize & ~7) - 0x20;
    return ret;
}

s32 killTask(s32 taskId) {
    Task *task;
    Task *prev;
    Task *next;
    Task *listEnd;

    task = TASKS + taskId;
    if (task->status.flags >= 0) {
        return -0x83;
    }
    if (task == CURRENT_TASK) {
        return -4;
    }
    if (task == TASKS) {
        return -5;
    }
    prev = task->prev;
    next = task->next;
    prev->next = next;
    next->prev = prev;
    if (TASK_VSYNC_MODE != 0) {
        listEnd = &TASK_LIST_END;
        if (listEnd->next == task) {
            listEnd->next = next;
        }
    }
    if ((PREEMPTED_TASK_PRIORITY > 0) && (task == PREEMPTED_TASK)) {
        PREEMPTED_TASK = next;
        PREEMPTED_TASK_PRIORITY = (u16)next->status.priority;
    }
    if ((DEFERRED_TASK_PRIORITY >= 0) && (task == DEFERRED_TASK)) {
        DEFERRED_TASK = next;
        DEFERRED_TASK_PRIORITY = (u16)next->status.priority;
    }
    freeHeapBlocksByTag(task->id);
    freeHeapBlock((void *)task->stack);
    task->status.flags = 0;
    return 0;
}

void exitCurrentTask(void) {
    Task *task;
    Task *prev;
    Task *next;
    Task *listEnd;

    task = CURRENT_TASK;
    prev = task->prev;
    next = task->next;
    prev->next = next;
    next->prev = prev;
    if (TASK_VSYNC_MODE != 0) {
        listEnd = &TASK_LIST_END;
        if (listEnd->next == task) {
            listEnd->next = next;
        }
    }
    if ((PREEMPTED_TASK_PRIORITY > 0) && (task == PREEMPTED_TASK)) {
        PREEMPTED_TASK = next;
        PREEMPTED_TASK_PRIORITY = (u16)next->status.priority;
    }
    if ((DEFERRED_TASK_PRIORITY >= 0) && (task == DEFERRED_TASK)) {
        DEFERRED_TASK = next;
        DEFERRED_TASK_PRIORITY = (u16)next->status.priority;
    }
    freeHeapBlocksByTag(task->id);
    freeHeapBlock((void *)task->stack);
    task->status.flags = 0;
    selectNextTask(task);
}

/* endTask is killTask with the interrupts off */
int killOtherTasks(void) {
    int selfId = CURRENT_TASK->id;
    int i;
    int killedCount = 0;

    for (i = 1; i < 32; i++) {
        if (i == selfId) {
            continue;
        }
        if (endTask(i) == 0) {
            killedCount++;
        }
    }
    return killedCount;
}

s32 getCurrentTaskId(void) {
    return CURRENT_TASK->id;
}

s32 wakeTask(s32 taskId, s32 result) {
    Task *task;

    task = TASKS + taskId;
    if (task->status.flags >= 0) {
        return -3;
    }
    if (task == CURRENT_TASK) {
        return -0x84;
    }
    task->wakeResult = result;
    task->waitFrames = 0;
    return 0;
}

s32 getTaskWaitFrames(s32 taskId) {
    Task *task;

    task = TASKS + taskId;
    if (task->status.flags >= 0) {
        return -3;
    }
    if (task == CURRENT_TASK) {
        return -0x84;
    }
    return task->waitFrames;
}
