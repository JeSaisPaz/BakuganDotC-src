// bdc 0x089fdf90 IoDecodeGetOutputSize
#include "bdc.h"

/* Returns the output size (`+0x106c`) of a decode job (`CODecode`, 0x10b0 bytes) under its lock. */

u32 IoDecodeGetOutputSize(IoDecodeJob *self)

{
  u32 v;
  
  CoreLockAcquire(self->lock);
  v = self->outputSize;
  CoreLockRelease(self->lock);
  return v;
}

