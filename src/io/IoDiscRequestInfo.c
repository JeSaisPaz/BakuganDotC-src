// bdc 0x089f9974 IoDiscRequestInfo
#include "bdc.h"

/* Queues a mode-2 request on the `CODiscSimple` disc reader (`g_discSimple`): open `path`, query
   its start sector (state 8, `IoDiscSimpleStateGetLba`) and size (state 2, size only), then
   close. Same bookkeeping as `IoDiscRequestRead` (no destination, not streamed). Returns the
   `BootWakeupThread(2)` result, or 0 when the reader was not idle or was busy (nothing queued). */

int IoDiscRequestInfo(IoDiscSimple *self, char *path)
{
    int result = 0;

    CoreLockAcquire(self->lock);
    if (self->idle != 0 && self->busy == 0) {
        result = 1;
        self->state = 1;
        self->mode = 2;
        self->streamed = 0;
        self->asyncIssued = 0;
        self->asyncPending = 0;
        self->busy = 1;
        sceRtcGetCurrentClockLocalTime(&g_ioDiscClock.requestTime);
        g_ioDiscClock.lastActiveTime = g_ioDiscClock.requestTime;
        self->idle = 0;
        self->path = path;
        self->fd = -1;
        self->dest = NULL;
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
