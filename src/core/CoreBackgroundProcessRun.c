// bdc 0x089ff99c CoreBackgroundProcessRun
#include "bdc.h"

/* Job loop of the background thread (never returns): under the worker lock it purges removed
   entries (`CoreCallbackListPurgeRemoved`) and takes the first queued callback
   (`CoreCallbackListGetAt` index 0); if there is one it calls it and dequeues it
   (`CoreBackgroundProcessRemove`), otherwise the thread sleeps (`BootSleepCurrentThread`)
   until woken. */
void CoreBackgroundProcessRun(CoreBackgroundProcess *proc)
{
    void (*job)(void);

    for (;;) {
        CoreLockAcquire(proc->lock);
        CoreCallbackListPurgeRemoved(proc->jobs);
        job = (void (*)(void))CoreCallbackListGetAt(proc->jobs, 0);
        CoreLockRelease(proc->lock);
        if (job != NULL) {
            job();
            CoreBackgroundProcessRemove(proc, (void *)job);
        } else {
            BootSleepCurrentThread();
        }
    }
}
