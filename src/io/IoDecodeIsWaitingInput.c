// bdc 0x089fddf4 IoDecodeIsWaitingInput
#include "bdc.h"

/* Returns the waiting-for-input flag (`+0x27`) of a decode job (`CODecode`, 0x10b0 bytes) under its
   lock. */

bool IoDecodeIsWaitingInput(IoDecodeJob *self)

{
  u8 v;
  
  CoreLockAcquire(self->lock);
  v = self->waitingInput;
  CoreLockRelease(self->lock);
  return (bool)v;
}

