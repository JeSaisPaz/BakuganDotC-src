// bdc 0x088fffd4 BtlAppearDemoCreate
#include "bdc.h"

/* Creates the appear-demo task: allocates 0x7a0 bytes from the low end of the heap (under
   `MemLock`, previous policy restored), constructs it with `BtlAppearDemoCtor``(task,
   ctorArg)`, sets its id to 0x67 and inserts it at priority 100 (`CoreTaskInsert`); then, when
   the battle main task (id 100, `CoreTaskFind`) exists, refreshes its scene
   (`BtlMainUpdateScene`). Returns the task. On allocation failure the id store goes through NULL,
   as in the binary. */

CoreTask *BtlAppearDemoCreate(u32 ctorArg)
{
    bool wasFromLow;
    CoreTask *task;
    BtlMain *battleMain;

    MemLock();
    wasFromLow = MemIsAllocFromLow();
    MemSetAllocFromLow(true);
    task = MemAlloc(sizeof(BtlAppearDemo), NULL, 0);
    MemSetAllocFromLow(wasFromLow);
    MemUnlock();
    if (task != NULL) {
        BtlAppearDemoCtor(task, ctorArg);
    }
    task->id = 0x67;
    CoreTaskInsert(task, 100);
    battleMain = CoreTaskFind(100);
    if (battleMain != NULL) {
        BtlMainUpdateScene(battleMain);
    }
    return task;
}
