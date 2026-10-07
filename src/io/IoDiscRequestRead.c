// bdc 0x089f9724 IoDiscRequestRead
#include "bdc.h"

/* Queues a whole-file read on the `CODiscSimple` disc reader (`g_discSimple`): under its lock,
   when the reader is idle and not busy, sets state 1 (open), mode 0, the streaming flag, path and
   destination (-1 = allocate), clears the result/abort fields and the error flags
   (`IoDiscSetErrorFlags`, `IoDiscSetAllocator`), timestamps the request (`g_ioDiscClock`),
   and after unlocking wakes the disc thread (`BootWakeupThread(2)`). Returns the wake result,
   or 0 when the reader was not ready (nothing queued). */

int IoDiscRequestRead(IoDiscSimple *self, char *path, void *dest, bool streamed)
{
    int result = 0;

    CoreLockAcquire(self->lock);
    if (self->idle != 0 && self->busy == 0) {
        result = 1;
        self->state = 1;
        self->mode = 0;
        self->streamed = streamed;
        self->asyncIssued = 0;
        self->asyncPending = 0;
        self->busy = 1;
        sceRtcGetCurrentClockLocalTime(&g_ioDiscClock.requestTime);
        g_ioDiscClock.lastActiveTime = g_ioDiscClock.requestTime;
        self->idle = 0;
        self->path = path;
        self->fd = -1;
        self->dest = dest;
        self->size = 0;
        self->streamBytes = 0;
        self->destResolved = 0;
        self->suspendAfterClose = 0;
        self->powerSuspend = 0;
        IoDiscSetErrorFlags(self, 0);
        IoDiscSetAllocator(self, NULL);
        self->abortFlag = 0;
        self->msError = 0;
        self->dataLoaded = 0;
    }
    CoreLockRelease(self->lock);
    if (result) {
        result = BootWakeupThread(2);
    }
    return result;
}
