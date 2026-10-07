// bdc 0x08a29678 CoreTask20000Dtor
#include "bdc.h"

/* Deleting destructor of the bare task class with id 20000 (`g_coreTask20000Vtbl`, built inline by
   `CoreTaskNewById` as a 0x10-byte `CoreTask`): resets the vtable, runs `CoreTaskDestroy` and
   frees the object (under `MemLock`) when `flags & 1`. Does nothing for NULL. */
void CoreTask20000Dtor(CoreTask *task, u32 flags)
{
    if (task == NULL) {
        return;
    }
    task->vtable = g_coreTask20000Vtbl;
    CoreTaskDestroy(task, 0);
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(task, NULL, 0);
        MemUnlock();
    }
}
