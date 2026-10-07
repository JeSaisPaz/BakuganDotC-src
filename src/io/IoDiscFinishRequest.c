// bdc 0x089f9dfc IoDiscFinishRequest
#include "bdc.h"

/* Acknowledges a finished request of the `CODiscSimple` disc reader (`g_discSimple`): when idle
   and still marked busy, clears busy, the path (`+0x10`) and destination (`+0x24`). */

void IoDiscFinishRequest(IoDiscSimple *self)

{
  CoreLockAcquire(self->lock);
  if ((self->idle != '\0') && (self->busy != '\0')) {
    self->busy = '\0';
    self->path = (char *)0x0;
    self->dest = (void *)0x0;
  }
  CoreLockRelease(self->lock);
  return;
}

