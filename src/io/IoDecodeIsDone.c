// bdc 0x089fdeac IoDecodeIsDone
#include "bdc.h"

/* Returns the done flag (`+0x24`) of a decode job (`CODecode`, 0x10b0 bytes) under its lock. */

bool IoDecodeIsDone(IoDecodeJob *self)

{
  u8 v;
  
  CoreLockAcquire(self->lock);
  v = self->done;
  CoreLockRelease(self->lock);
  return (bool)v;
}

