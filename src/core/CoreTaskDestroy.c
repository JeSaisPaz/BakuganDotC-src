// bdc 0x089bf21c CoreTaskDestroy
#include "bdc.h"

/* Base destructor of `CoreTask` (vtable slot 1 of the base class, chained to by the derived
   destructors, e.g. `SndBgmCmdDestroy`): resets the vtable word to the base vtable, removes and
   destroys the child task stored at `+8` if there is one (`CoreTaskRemove` with the destroy flag)
   and, when bit 0 of `flags` is set, frees the object (`MemFree` under `MemLock`). Does nothing for
   NULL. */
void CoreTaskDestroy(CoreTask *task, u32 flags)
{
    if (task != NULL) {
        task->vtable = g_coreTaskVtbl;
        if (task->child != NULL) {
            CoreTaskRemove(task->child, true);
        }
        if ((flags & 1) != 0) {
            MemLock();
            MemFree(task, NULL, 0);
            MemUnlock();
        }
    }
}
