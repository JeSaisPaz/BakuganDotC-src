// bdc 0x089038dc BtlStageCamCreate
#include "bdc.h"

/* Creates the stage camera demo task: allocates 0x58 bytes from the low end of the heap (under
   `MemLock`, previous policy restored), constructs it with `BtlStageCamCtor``(task, index, unit)`
   (camera script `index`, followed unit `unit`), sets its id to 0x6b and inserts it at priority 100
   (`CoreTaskInsert`). Returns the task. On allocation failure the id store goes through NULL, as in
   the binary. */

CoreTask *BtlStageCamCreate(u32 index, void *unit)
{
    bool wasFromLow;
    CoreTask *task;

    MemLock();
    wasFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    task = MemAlloc(sizeof(BtlStageCam), NULL, 0);
    MemSetAllocFromLow(wasFromLow);
    MemUnlock();
    if (task != NULL) {
        BtlStageCamCtor(task, index, unit);
    }
    task->id = 0x6b;
    CoreTaskInsert(task, 100);
    return task;
}
