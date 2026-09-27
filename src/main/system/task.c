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

void setTaskVsyncMode(s32 vsyncMode) {
    s16 *task;
    s32 i;

    func_8006A804();
    if (vsyncMode != 0) {
        if (TASK_VSYNC_MODE == 0) {
            TASK_VSYNC_MODE = 1;
            task = &TASKS;
            for (i = 0x1F; i >= 0; i--, task += 0x60) {
                if (*task > 0) {
                    D_80077AEC = task;
                    break;
                }
            }
        }
    } else if (TASK_VSYNC_MODE != 0) {
        TASK_VSYNC_MODE = 0;
        D_80077AEC = &TASKS;
    }
    func_8006A814();
}

s32 startTaskScheduler(s32 mode, s32 stackSize, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3) {
    s32 *slot;
    s32 *mainTask;
    s32 *tcbTable;
    s32 *src;
    s32 *dst;
    s32 i;
    s32 j;
    s32 stack;
    s32 vsyncEvent;

    func_8006A804();
    TASK_VSYNC_MODE = mode;
    for (slot = (s32 *)&TASKS, i = 0x1F; i >= 0; i--, slot += 0x30) {
        *slot = 0;
    }
    CURRENT_TASK_PRIORITY = PREEMPTED_TASK_PRIORITY = *(u16 *)&D_80077A1C = 0xFFFF;
    CURRENT_TASK = (Unk80077A0C *)((Thread *)&TASKS - 1);
    ((Thread *)&TASK_LIST_END)->flags = 0x8000FFFF;
    ((Thread *)&TASK_LIST_END)->next = (Thread *)CURRENT_TASK + 1;
    if (TASK_VSYNC_MODE != 0) {
        ((Thread *)&TASK_LIST_END)->prev = (Thread *)&TASK_LIST_END;
    } else {
        D_80077AEC = &TASKS;
    }
    ((Thread *)&TASK_LIST_END)->unk14 = -1;
    tcbTable = *(s32 **)0x108;
    KERNEL_TCB = *tcbTable;
    src = (s32 *)(KERNEL_TCB + 8);
    dst = &D_80077BC0;
    for (j = 0x27; j >= 0; j--) {
        *dst++ = *src++;
    }
    mainTask = (s32 *)&TASKS;
    mainTask[0] = 0xA0000000;
    mainTask[0x28] = entry;
    mainTask[0x2B] = 0x4000FF04;
    mainTask[0xC] = a0;
    mainTask[0xD] = a1;
    mainTask[0xE] = a2;
    mainTask[0xF] = a3;
    mainTask[2] = (s32)&TASK_LIST_END;
    mainTask[3] = (s32)&TASK_LIST_END;
    mainTask[5] = 0;
    mainTask[6] = 0;
    stack = (s32)allocHeapBlock(stackSize, -3);
    if (stack == 0) {
        return -6;
    }
    mainTask[7] = stack;
    mainTask[0x25] = stack + (stackSize & ~7) - 0x20;
    vsyncEvent = func_8006A794(0xF2000003, 2, 0x1000, (long (*)())handleVsyncPreemption);
    func_8006A7C4(vsyncEvent);
    SetRCnt(0xF2000003, 1, 0x1000);
    StartRCnt(0xF2000003);
    VSYNC_EVENT = vsyncEvent;
    func_8006A814();
    return 0;
}

long handleVsyncPreemption(void) {
    Thread *task;
    s32 *tcbRegs;
    s32 *regs;
    s32 i;

    task = (Thread *)CURRENT_TASK;
    tickVblankCounters();
    tcbRegs = (s32 *)(KERNEL_TCB + 8);
    regs = task->regs;
    for (i = 0x27; i >= 0; i--) {
        *regs++ = *tcbRegs++;
    }
    task->flags |= 0x20000000;
    if ((PREEMPTED_TASK_PRIORITY = CURRENT_TASK_PRIORITY) == 0) {
        if (TASK_VSYNC_MODE == 0) {
            D_80077A14 = &TASKS;
            D_80077A1C = 0;
        }
    } else {
        PREEMPTED_TASK = task;
        task = (Thread *)&TASKS;
        CURRENT_TASK = (Unk80077A0C *)task;
        CURRENT_TASK_PRIORITY = TASKS;
        tcbRegs = (s32 *)(KERNEL_TCB + 8);
        regs = task->regs;
        for (i = 0x27; i >= 0; i--) {
            *tcbRegs++ = *regs++;
        }
    }
}

void *selectNextTask(void *current) {
    s16 *next = *(s16 **)((s8 *)current + 0xC);
    s16 prio = *next;

    if (*next > 0 && prio == PREEMPTED_TASK_PRIORITY) {
        D_80077A14 = next;
        D_80077A1C = prio;
        next = PREEMPTED_TASK;
        PREEMPTED_TASK_PRIORITY = -1;
    } else {
        prio = *next;
        if ((u16)prio > (u16)D_80077A1C) {
            prio = D_80077A1C;
            next = D_80077A14;
            *(u16 *)&D_80077A1C = 0xFFFF;
        }
    }
    CURRENT_TASK = next;
    CURRENT_TASK_PRIORITY = prio;
    return next;
}

s32 createTask(s32 taskId, s32 insertPos, s32 priority, s32 stackSize, s32 unused, s32 entry, s32 a0, s32 a1, s32 a2, s32 a3) {
    Thread *task;
    Thread *prev;
    Thread *next;
    s32 ret;
    s32 prevFlags;
    s32 nextFlags;
    s32 *src;
    s32 *dst;
    s32 i;
    s32 stack;

    task = (Thread *)&TASKS + taskId;
    if (taskId != 0) {
        if ((s32)task->flags < 0) {
            return -1;
        }
    } else {
        for (taskId++; taskId < 32; taskId++) {
            if ((s32)(++task)->flags >= 0) {
                break;
            }
        }
        if (taskId >= 32) {
            return -1;
        }
    }
    ret = 0;
    if (insertPos < 0) {
        prev = (Thread *)&TASKS;
        if (priority >= *(u16 *)prev) {
            do {
                prev = prev->prev;
            } while (priority >= *(u16 *)prev);
        }
        goto after;
    }
    if (insertPos >= 32) {
        insertPos -= 32;
        prev = (Thread *)&TASKS + insertPos;
        if ((s32)prev->flags >= 0) {
            return -3;
        }
        prevFlags = prev->flags;
        if ((u16)prevFlags < priority) {
            ret = -0x86;
            priority = prevFlags;
        }
    after:
        next = prev->next;
        next->prev = task;
    } else {
        next = (Thread *)&TASKS + insertPos;
        if ((s32)next->flags >= 0) {
            return -3;
        }
        nextFlags = next->flags;
        if (priority < (u16)nextFlags) {
            ret = -0x86;
            priority = nextFlags;
        }
        prev = next->prev;
        next->prev = task;
    }
    prev->next = task;
    task->next = next;
    task->prev = prev;
    if (TASK_VSYNC_MODE != 0 && priority > 0 && *(s16 *)next == 0) {
        D_80077AEC = (s16 *)task;
    }
    src = (s32 *)(KERNEL_TCB + 8);
    dst = task->regs;
    for (i = 0x27; i >= 0; i--) {
        *dst++ = *src++;
    }
    task->flags = priority | 0xA0000000;
    task->regs[32] = entry;
    task->regs[35] = 0x4000FF04;
    task->regs[4] = a0;
    task->regs[5] = a1;
    task->regs[6] = a2;
    task->regs[7] = a3;
    task->regs[31] = (s32)func_80014A90;
    task->regs[28] = TASK_GP;
    task->unk14 = taskId;
    task->unk18 = 0;
    stack = (s32)allocHeapBlock(stackSize, -3);
    if (stack == 0) {
        return -6;
    }
    task->stack = stack;
    task->regs[29] = stack + (stackSize & ~7) - 0x20;
    return ret;
}

s32 killTask(s32 taskId) {
    void *task;
    void *prev;
    void *next;
    void **sentinel;

    task = (s8 *)&TASKS + taskId * 0xC0;
    if ((*(s32 *)((s8 *)task + 0)) >= 0) {
        return -0x83;
    }
    if (task == CURRENT_TASK) {
        return -4;
    }
    if (task == &TASKS) {
        return -5;
    }
    prev = (*(void **)((s8 *)task + 8));
    next = (*(void **)((s8 *)task + 0xC));
    (*(void **)((s8 *)prev + 0xC)) = next;
    (*(void **)((s8 *)next + 8)) = prev;
    if (TASK_VSYNC_MODE != 0) {
        sentinel = (void **)&TASK_LIST_END;
        if (sentinel[3] == task) {
            sentinel[3] = next;
        }
    }
    if ((PREEMPTED_TASK_PRIORITY > 0) && (task == PREEMPTED_TASK)) {
        PREEMPTED_TASK = next;
        PREEMPTED_TASK_PRIORITY = (*(u16 *)((s8 *)next + 0));
    }
    if ((D_80077A1C >= 0) && (task == D_80077A14)) {
        D_80077A14 = next;
        D_80077A1C = (*(u16 *)((s8 *)next + 0));
    }
    freeHeapBlocksByTag((*(s32 *)((s8 *)task + 0x14)));
    freeHeapBlock((*(void **)((s8 *)task + 0x1C)));
    (*(s32 *)((s8 *)task + 0)) = 0;
    return 0;
}

void exitCurrentTask(void) {
    void *task;
    void *prev;
    void *next;
    void **sentinel;

    task = CURRENT_TASK;
    prev = (*(void **)((s8 *)task + 8));
    next = (*(void **)((s8 *)task + 0xC));
    (*(void **)((s8 *)prev + 0xC)) = next;
    (*(void **)((s8 *)next + 8)) = prev;
    if (TASK_VSYNC_MODE != 0) {
        sentinel = (void **)&TASK_LIST_END;
        if (sentinel[3] == task) {
            sentinel[3] = next;
        }
    }
    if ((PREEMPTED_TASK_PRIORITY > 0) && (task == PREEMPTED_TASK)) {
        PREEMPTED_TASK = next;
        PREEMPTED_TASK_PRIORITY = (*(u16 *)((s8 *)next + 0));
    }
    if ((D_80077A1C >= 0) && (task == D_80077A14)) {
        D_80077A14 = next;
        D_80077A1C = (*(u16 *)((s8 *)next + 0));
    }
    freeHeapBlocksByTag((*(s32 *)((s8 *)task + 0x14)));
    freeHeapBlock((*(void **)((s8 *)task + 0x1C)));
    (*(s32 *)((s8 *)task + 0)) = 0;
    selectNextTask(task);
}

int killOtherTasks(void) {
    int selfId = CURRENT_TASK->unk14;
    int i;
    int killedCount = 0;

    for (i = 1; i < 0x20; i++) {
        if (i == selfId) {
            continue;
        }
        if (func_80014A00(i) == 0) {
            killedCount++;
        }
    }
    return killedCount;
}

s32 getCurrentTaskId(void) {
    return (*(s32 *)((s8 *)CURRENT_TASK + 0x14));
}

s32 wakeTask(s32 taskId, s32 result) {
    void *task;

    task = (s8 *)&TASKS + taskId * 0xC0;
    if ((*(s32 *)((s8 *)task + 0)) >= 0) {
        return -3;
    }
    if (task == CURRENT_TASK) {
        return -0x84;
    }
    (*(s32 *)((s8 *)task + 0x18)) = result;
    (*(s32 *)((s8 *)task + 4)) = 0;
    return 0;
}

s32 getTaskWaitFrames(s32 taskId) {
    void *task;

    task = (s8 *)&TASKS + taskId * 0xC0;
    if ((*(s32 *)((s8 *)task + 0)) >= 0) {
        return -3;
    }
    if (task == CURRENT_TASK) {
        return -0x84;
    }
    return (*(s32 *)((s8 *)task + 4));
}
