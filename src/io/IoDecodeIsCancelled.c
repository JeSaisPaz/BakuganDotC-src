// bdc 0x089fdf20 IoDecodeIsCancelled
#include "bdc.h"

/* Returns the cancel flag (`+0x25`) of a decode job (`CODecode`, 0x10b0 bytes) under its lock. */

bool IoDecodeIsCancelled(IoDecodeJob *self)

{
  u8 v;
  
  CoreLockAcquire(self->lock);
  v = self->cancelled;
  CoreLockRelease(self->lock);
  return (bool)v;
}

