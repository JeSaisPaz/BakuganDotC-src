// bdc 0x08849428 BtlSlowMotionTaskCreate
#include "bdc.h"

/* Allocates a slow-motion task (0x2d0 bytes, from the low end of the heap), constructs it with the
   three creation arguments passed through unchanged, sets task id 0x14b and inserts it at
   priority 100. A failed allocation is not handled: the id is then written through NULL, as in
   the original. Returns the task. */
BtlSlowMotionTask *BtlSlowMotionTaskCreate(BtlBakugan *unit, void *target, u32 argC)
{
    bool fromLow;
    BtlSlowMotionTask *mem;
    BtlSlowMotionTask *task;

    MemLock();
    fromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    mem = MemAlloc(sizeof(BtlSlowMotionTask), NULL, 0);
    MemSetAllocFromLow(fromLow);
    MemUnlock();
    task = NULL;
    if (mem != NULL) {
        BtlSlowMotionTaskCtor(&mem->base, unit, target, argC);
        task = mem;
    }
    task->base.id = 0x14b;
    CoreTaskInsert(task, 100);
    return task;
}
