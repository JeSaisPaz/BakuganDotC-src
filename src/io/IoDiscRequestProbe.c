// bdc 0x089f9854 IoDiscRequestProbe
#include "bdc.h"

/* Queues a mode-1 request on the `CODiscSimple` disc reader (`g_discSimple`): open `path`, rewind
   (state 7, `sceIoLseekAsync` to 0) and close, without reading; used to check that a file can be
   opened. Same bookkeeping as `IoDiscRequestRead`. Returns 0 when the reader is not idle or still
   holds an unacknowledged request; otherwise wakes the disc thread (slot 2) and returns
   `BootWakeupThread`'s result. */

int IoDiscRequestProbe(IoDiscSimple *self, char *path)
{
  int result;
  bool accepted;

  accepted = false;
  CoreLockAcquire(self->lock);
  if (self->idle != 0 && self->busy == 0) {
    accepted = true;
    self->state = 1;
    self->mode = 1;
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
  result = 0;
  if (accepted) {
    result = BootWakeupThread(2);
  }
  return result;
}
