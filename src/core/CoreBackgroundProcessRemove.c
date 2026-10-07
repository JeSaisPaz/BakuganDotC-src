// bdc 0x089ffa0c CoreBackgroundProcessRemove
#include "bdc.h"

/* Removes callback `fn` from the background worker's job list (`CoreCallbackListRemove` under
   the worker lock). */
void CoreBackgroundProcessRemove(CoreBackgroundProcess *proc, void *fn)
{
    CoreLockAcquire(proc->lock);
    CoreCallbackListRemove(proc->jobs, fn);
    CoreLockRelease(proc->lock);
}
