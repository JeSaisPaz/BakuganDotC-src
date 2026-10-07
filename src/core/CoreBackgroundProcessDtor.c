// bdc 0x089ff910 CoreBackgroundProcessDtor
#include "bdc.h"

/* Destructor of the `COBackGroundProcess` worker (`CoreBackgroundProcess`): destroys its lock
   (`CoreLockDestroy` flag 3) and its job list (`CoreCallbackListDestroy` flag 3), clearing
   each field, and frees the object under `MemLock` when bit 0 of `flags` is set. NULL `proc`
   does nothing. */
void CoreBackgroundProcessDtor(CoreBackgroundProcess *proc, u32 flags)
{
    if (proc == NULL)
        return;
    if (proc->lock != NULL) {
        CoreLockDestroy(proc->lock, 3);
        proc->lock = NULL;
    }
    if (proc->jobs != NULL) {
        CoreCallbackListDestroy(proc->jobs, 3);
        proc->jobs = NULL;
    }
    if ((flags & 1) != 0) {
        MemLock();
        MemFree(proc, NULL, 0);
        MemUnlock();
    }
}
